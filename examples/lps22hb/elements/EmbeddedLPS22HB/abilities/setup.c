	ATMO_LPS22HB_Config_t config;
	config.i2cInstance = ATMO_PROPERTY(undefined, i2cInstance);
	config.spiInstance = ATMO_PROPERTY(undefined, spiInstance);
	config.gpioInstance = ATMO_PROPERTY(undefined, gpioInstance);
	config.csPin = ATMO_PROPERTY(undefined, csPin);
	config.i2cAddress = ATMO_PROPERTY(undefined, i2cAddress);
	config.spi3Wire = ATMO_PROPERTY(undefined, spi3Wire);
	config.driverType.type = ATMO_PROPERTY(undefined, communicationMode);
	return ATMO_LPS22HB_Init(&config) == ATMO_LPS22HB_Status_Success ? ATMO_Status_Success : ATMO_Status_Fail;

