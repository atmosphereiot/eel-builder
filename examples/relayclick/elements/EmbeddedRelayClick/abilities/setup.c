ATMO_RELAYCLICK_Config_t config;
config.relay1Pin = ATMO_PROPERTY(undefined, relay1Pin);
config.relay2Pin = ATMO_PROPERTY(undefined, relay2Pin);
config.gpioDriverInstance = ATMO_PROPERTY(undefined, gpioDriverInstance);
ATMO_RELAYCLICK_Init(&ATMO_VARIABLE(undefined, handle), &config);
return ATMO_Status_Success;