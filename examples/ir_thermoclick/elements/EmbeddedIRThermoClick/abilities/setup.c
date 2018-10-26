	ATMO_THERMOCLICK_Config_t config;
	config.address = ATMO_PROPERTY(undefined, i2cAddress);
	config.i2cDriverInstance = ATMO_PROPERTY(undefined, i2cInstance);

	return ( ATMO_THERMOCLICK_Init(&config) == ATMO_THERMOCLICK_Status_Success ) ? ATMO_Status_Success : ATMO_Status_Fail;
