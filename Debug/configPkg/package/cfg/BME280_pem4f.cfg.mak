# invoke SourceDir generated makefile for BME280.pem4f
BME280.pem4f: .libraries,BME280.pem4f
.libraries,BME280.pem4f: package/cfg/BME280_pem4f.xdl
	$(MAKE) -f C:\Users\ALBERT~1\workspace_v10\BME280_Local_CC26X2R1_LAUNCHXL_tirtos_ccs/src/makefile.libs

clean::
	$(MAKE) -f C:\Users\ALBERT~1\workspace_v10\BME280_Local_CC26X2R1_LAUNCHXL_tirtos_ccs/src/makefile.libs clean

