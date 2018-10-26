ATMO_MOTION_CLICK_Config_t config;
config.enablePin = ATMO_PROPERTY(undefined, enablePin);
config.motionPin = ATMO_PROPERTY(undefined, motionPin);
config.gpioDriverInstance = ATMO_PROPERTY(undefined, gpioDriverInstance);

if(ATMO_MOTION_CLICK_Init(&config) != ATMO_MOTION_CLICK_Status_Success)
{
    return ATMO_Status_Fail;
}

ATMO_MOTION_CLICK_RegisterMotionDetectedAbilityHandle(ATMO_ABILITY(undefined, detectMotion));

return ATMO_Status_Success;