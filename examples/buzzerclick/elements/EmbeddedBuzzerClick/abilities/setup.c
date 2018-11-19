    ATMO_BUZZERCLICK_Config_t config;
    config.pwmDriverInstance = ATMO_PROPERTY(undefined, pwmDriverInstance);
    config.initialVolumePct = ATMO_PROPERTY(undefined, initialVolumePct);
    config.initialFrequency = ATMO_PROPERTY(undefined, frequencyHz);
    config.pwmPin = ATMO_PROPERTY(undefined, pwmPin);
    ATMO_BUZZERCLICK_Init(&config);
    return ATMO_Status_Success;