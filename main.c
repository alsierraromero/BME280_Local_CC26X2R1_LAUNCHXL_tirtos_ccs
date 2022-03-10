
/*
 * Reads temperature, humidity and pressure from a BME280 sensor cyclically
 *
 * *** BME280 Pin config: ***
 * I2C interface is used. DIO1 is used to enable I2C: DIO1='1'
 * SDO is connected directly to GND, so primary I2C address is used 0x76.
 *
 */

/***** Header Files *****/
// XDC module Header Files
#include <xdc/std.h>                                                            // XDC "base types" - must be included FIRST
#include <xdc/runtime/Types.h>                                                  // XDC constants/types used in xdc.runtime pkg
#include <xdc/cfg/global.h>                                                     // For all BIOS instances created statically in RTOS .cfg file
#include <xdc/runtime/Error.h>                                                  // For error handling (e.g. Error block)
#include <xdc/runtime/System.h>                                                 // XDC System functions (e.g. System_abort(), System_printf())
#include <xdc/runtime/Log.h>                                                    // For any Log_info() call

// TI-RTOS Kernel Header Files
#include <ti/sysbios/BIOS.h>                                                    // BIOS module API
#include <ti/sysbios/knl/Task.h>                                                // Task APIs
#include <ti/sysbios/knl/Semaphore.h>                                           // Semaphore APIs
#include <ti/sysbios/knl/Clock.h>                                               // Clock APIs
#include <ti/sysbios/knl/Mailbox.h>

#include <ti/drivers/Board.h>

#include "ti_drivers_config.h"                                                  // Syscfg Board/Drivers Header file
#include <ti/drivers/GPIO.h>                                                    // TI GPIO Driver header file
#include <ti/drivers/I2C.h>

/* Display Header files */
#include <ti/display/Display.h>
#include <ti/display/DisplayUart.h>

// Standard C Header Files
#include <stdint.h>                                                             // Standard integer types
#include <stddef.h>

//Bosch's Sensortec Files
#include "bme280.h"


#define MEASUREMENT_PERIOD_US           60000000
#define I2CINSTANCE                     0x00                                      // I2C0

#define CTRL_HUM_F2_REG_CFG             0x01                                      // Hum oversampling set to 1
#define MEAS_F4_REG_CFG                 0x25                                      // Temp and Press oversampling set to 1. Forced mode
#define CONFIG_F5_REG_CFG               0x00

#define TMEASURE_TICKS                  1000                                     //10 ms: Time that it takes for the data to be ready in Forced mode w/ 1x oversample

#define BME280_CALIB_T_P_LEN            24                                       //Created this one because the one in "bme280_defs.h" is wrong (26)
#define DIG_H1_ADD                      0xA1
#define DIG_H3_ADD                      0xE3
#define DIG_H4_ADD                      0xE4


/******  Globals *******/
Task_Handle taskReadData, taskprint;
Task_Params taskParams;
Semaphore_Handle semTrigger, dataready;
Clock_Struct clkTriggerStruct;
Mailbox_Handle data_mb;
Mailbox_Params mbParams;
Display_Params disparams;

char configData[4]={BME280_CTRL_HUM_ADDR,CTRL_HUM_F2_REG_CFG,BME280_CTRL_MEAS_ADDR,MEAS_F4_REG_CFG};
char calibData[24];
char calibAdd_T_P[]={BME280_TEMP_PRESS_CALIB_DATA_ADDR};
char calibAdd_DIGH1[]={DIG_H1_ADD};                                                         //Hum Dig1
char calibAdd_H[]={BME280_HUMIDITY_CALIB_DATA_ADDR};

/******* Prototypes ******/
void taskGetDataFxn(void);
void clkFxnTrigger(UArg arg0);
void Clock_Trigger_init(void);
void printData(void);
void datai2c(char* wbuff, char nwdata, char* rbuff, char nrdata);
void parseCalib(uint8_t shiftqty, char* nparsedCalib, uint16_t* parsedCalib);


/*
 *  ======== main ========
 */
int main()
{
    // Initialize Peripherals
    Board_init();                                                                                   // Call TI Driver config init function
    GPIO_init();

    // Create Task(s)
    Task_Params_init(&taskParams);                                                                      // Init Task Params with pri=2, stackSize = 512
    taskParams.priority = 2;

    taskReadData = Task_create((Task_FuncPtr)taskGetDataFxn, &taskParams, Error_IGNORE);                   // Create tasktrigger with tasktriggerFxn (Error Block ignored, we explicitly test 'tasktrigger' handle)
    if (taskReadData == NULL) {                                                                              // Verify that Task1 was created
        System_abort("Task Get Data create failed");                                                        // If not abort program
    }

    taskParams.priority = 3;
    taskprint = Task_create((Task_FuncPtr)printData, &taskParams, Error_IGNORE);                   // Create tasktrigger with tasktriggerFxn (Error Block ignored, we explicitly test 'tasktrigger' handle)
    if (taskprint == NULL) {                                                                        // Verify that Task1 was created
        System_abort("Task Print Data create failed");                                              // If not abort program
    }

    // Create Semaphore(s)
    semTrigger = Semaphore_create(0, NULL, Error_IGNORE);                                       // Create Sem, count=0, params default, Error_Block ignored
    if (semTrigger == NULL) {                                                                   // Verify that item was created
       System_abort("Semaphore Trigger create failed");                                         // If not abort program
    }

    Clock_Trigger_init();
    Mailbox_Params_init(&mbParams);

    //Create Mailbox
    data_mb = Mailbox_create(sizeof(struct bme280_data), 2, &mbParams,Error_IGNORE);
    if (data_mb == NULL) {                                                                      // Verify that item was created
      System_abort(" Mailbox with data parameter create failed");                               // If not abort program
      while(1){}
    }

    BIOS_start();    /* does not return */
    return(0);
}

/* clkTriggerInit */
void Clock_Trigger_init(void){
    /* Construct BIOS Objects */
    Clock_Params clkParams;
    Clock_Params_init(&clkParams);
    clkParams.period = MEASUREMENT_PERIOD_US/Clock_tickPeriod;
    clkParams.startFlag = TRUE;

    /* Construct a periodic Clock Instance */
    Clock_construct(&clkTriggerStruct, (Clock_FuncPtr)clkFxnTrigger,
                    MEASUREMENT_PERIOD_US/Clock_tickPeriod, &clkParams);
}

/* clkFxnTrigger */
void clkFxnTrigger(UArg arg0){
    Semaphore_post(semTrigger);
}

/* Transfer of data via I2C
 * Write Operation: If only data will be written to a reg, since the IC does not support writing contiguous reg addresses, the write buffer will have the reg address and the data byte interleaved i.e: Reg 0xF2, Data 0x01, Reg 0xF4, Data 0x25 -> writebuff = {0xF2,0x01,0xF4,0x25}
 * Read Operation: Write buffer will have reg address and read buffer will receive the data.
 */
void datai2c(char* wbuff, char nwdata, char* rbuff, char nrdata){
    I2C_init();
    I2C_Params i2cparams;
    i2cparams.bitRate = I2C_400kHz;
    I2C_Params_init(&i2cparams);
    I2C_Handle i2cHandle = I2C_open(I2CINSTANCE, &i2cparams);

    // Initialize slave address of transaction
    I2C_Transaction transaction = {0};
    transaction.slaveAddress = BME280_I2C_ADDR_PRIM;

    transaction.writeBuf = wbuff;
    transaction.writeCount = nwdata;
    transaction.readCount = nrdata;
    transaction.readBuf = rbuff;
    I2C_transfer(i2cHandle, &transaction);
    I2C_close(i2cHandle);
}

/* Going to Forced mode, reading hum, temp and press data and calib parameters, parsing the data and compensating */
void taskGetDataFxn(void){
    char nparsedData[8];
    char tempReadbuff[1];
    char dataAdd[]={BME280_DATA_ADDR};
    uint16_t itick;
    struct bme280_uncomp_data udata;
    struct bme280_data data;
    struct bme280_calib_data calib_coef;

    //Temp and Press calib data
    datai2c(calibAdd_T_P, sizeof(calibAdd_T_P), calibData, sizeof(calibData));
    parseCalib(8, calibData, &calib_coef.dig_t1);
    parseCalib(8, &calibData[2], &calib_coef.dig_t2);
    parseCalib(8, &calibData[4], &calib_coef.dig_t3);
    parseCalib(8, &calibData[6], &calib_coef.dig_p1);
    parseCalib(8, &calibData[8], &calib_coef.dig_p2);
    parseCalib(8, &calibData[10], &calib_coef.dig_p3);
    parseCalib(8, &calibData[12], &calib_coef.dig_p4);
    parseCalib(8, &calibData[14], &calib_coef.dig_p5);
    parseCalib(8, &calibData[16], &calib_coef.dig_p6);
    parseCalib(8, &calibData[18], &calib_coef.dig_p7);
    parseCalib(8, &calibData[20], &calib_coef.dig_p8);
    parseCalib(8, &calibData[22], &calib_coef.dig_p9);

    //Hum calibration data
    datai2c(calibAdd_DIGH1, 1, &calib_coef.dig_h1, 1);                              //dig_H1
    datai2c(calibAdd_H, 1, calibData, 7);                                           //Reading humidity calib data from 0xE1 onwards
    parseCalib(8, calibData, &calib_coef.dig_h2);
    calib_coef.dig_h3 = calibData[2];                                                 //dig_H3
    calib_coef.dig_h4 = (calibData[3]<<4) | (calibData[4] & 0x0F);                    //dig_H4
    calib_coef.dig_h5 = (calibData[5] << 4) | ((calibData[4] & 0xF0) >> 4);           //dig_H5
    calib_coef.dig_h6 = calibData[6];                                                  //dig_H6


    while(1){
        Semaphore_pend(semTrigger, BIOS_WAIT_FOREVER);
        datai2c(configData,sizeof(configData),tempReadbuff,0);                                               //Config IC
        itick = Clock_getTicks();
        while((Clock_getTicks() - itick) < TMEASURE_TICKS);                                             //Waiting on data to be ready
        datai2c(dataAdd, sizeof(dataAdd), nparsedData, BME280_P_T_H_DATA_LEN);                          //Reading data from the registers
        bme280_parse_sensor_data(nparsedData, &udata);                                                  //Parsing data with Bosch's Sensortec API
        bme280_compensate_data(BME280_ALL,&udata,&data,&calib_coef);                                    //Compensating data
        Mailbox_post(data_mb, &data,BIOS_WAIT_FOREVER);
    }

}

void printData(void){
    struct bme280_data compData;
    /* Initialize display and try to open both UART and LCD types of display. */
   Display_init();
   Display_Params_init(&disparams);
   disparams.lineClearMode = DISPLAY_CLEAR_BOTH;

   Display_Handle hSerial = Display_open(Display_Type_UART, &disparams);

   if (hSerial == NULL) {
           /* Failed to open a display */
           while (1) {}
   }
   else
       Display_printf(hSerial, 0, 0, "Hello. This is the weather channel!");

    while(1){
            Mailbox_pend(data_mb,&compData,BIOS_WAIT_FOREVER);
            Display_printf(hSerial, 0, 0, "Temperature: %d Celcius, Humidity: %u%%, Pressure: %u Pa",compData.temperature/100 ,compData.humidity/1024, compData.pressure/100);
        }
}

void parseCalib(uint8_t shiftqty, char* nparsedCalib, uint16_t* parsedCalib){
    *parsedCalib = (nparsedCalib[1]<<shiftqty) | nparsedCalib[0];
}



