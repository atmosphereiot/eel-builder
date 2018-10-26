#include "motionclick.h"

typedef struct {
    ATMO_MOTION_CLICK_Config_t config;
    bool configured;
    bool abilityHandleRegistered;
    unsigned int abilityHandle;
} ATMO_MOTION_CLICK_Priv_Config;

static ATMO_MOTION_CLICK_Priv_Config _ATMO_MOTION_CLICK_Config;

void ATMO_MOTION_CLICK_ISR(void *data)
{
    if(_ATMO_MOTION_CLICK_Config.abilityHandleRegistered)
    {
        ATMO_Value_t value;
        ATMO_InitValue(&value);
        ATMO_AddAbilityExecute(_ATMO_MOTION_CLICK_Config.abilityHandle, &value);
    }
}

/**
 * Initialize MOTION_CLICK Driver
 *
 * @param[in] config - Device configuration (optional)
 */
ATMO_MOTION_CLICK_Status_t ATMO_MOTION_CLICK_Init(ATMO_MOTION_CLICK_Config_t *config)
{
    ATMO_MOTION_CLICK_Status_t status = ATMO_MOTION_CLICK_Status_Success;

    if(config != NULL)
    {
        status = ATMO_MOTION_CLICK_SetConfiguration(config);
    }

    return status;
}

/**
 * Set basic device configuration
 *
 * @param[in] config
 */
ATMO_MOTION_CLICK_Status_t ATMO_MOTION_CLICK_SetConfiguration(const ATMO_MOTION_CLICK_Config_t *config)
{
    if(config == NULL)
    {
        return ATMO_MOTION_CLICK_Status_Fail;
    }

    ATMO_GPIO_Config_t pinConfig;
    pinConfig.pinMode = ATMO_GPIO_PinMode_Output_PushPull;
    pinConfig.initialState = ATMO_GPIO_PinState_Low;
    ATMO_GPIO_SetPinConfiguration(config->gpioDriverInstance, config->enablePin, &pinConfig);

    pinConfig.pinMode = ATMO_GPIO_PinMode_Input_PullDown;
    ATMO_GPIO_SetPinConfiguration(config->gpioDriverInstance, config->motionPin, &pinConfig);

    // Register interrupt handler
    ATMO_GPIO_RegisterInterruptCallback(config->gpioDriverInstance, config->motionPin, ATMO_GPIO_InterruptTrigger_RisingEdge, ATMO_MOTION_CLICK_ISR);

    return ATMO_MOTION_CLICK_Status_Success;
}

/**
 * Get Temperature in degrees celsius
 * 
 * @param[out] temperature
 */
ATMO_MOTION_CLICK_Status_t ATMO_MOTION_CLICK_RegisterMotionDetectedAbilityHandle(unsigned int abilityHandle)
{
    _ATMO_MOTION_CLICK_Config.abilityHandle = abilityHandle;
    _ATMO_MOTION_CLICK_Config.abilityHandleRegistered = true;
    return ATMO_MOTION_CLICK_Status_Success;
}