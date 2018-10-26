#include "relayclick.h"

#define ATMO_MAX_RELAY_CLICKS (3)

static unsigned int currentNumRelayClicks = 0;

static ATMO_RELAYCLICK_Config_t _config[ATMO_MAX_RELAY_CLICKS];


ATMO_RELAYCLICK_Status_t ATMO_RELAYCLICK_Init(ATMO_DriverInstanceHandle_t *handle, ATMO_RELAYCLICK_Config_t *config)
{
    if(currentNumRelayClicks >= ATMO_MAX_RELAY_CLICKS)
    {
        return ATMO_RELAYCLICK_Status_Fail;
    }

    *handle = currentNumRelayClicks++;
    
    if(config != NULL)
    {
        if( ATMO_RELAYCLICK_SetConfiguration(*handle, config) != ATMO_RELAYCLICK_Status_Success )
        {
            return ATMO_RELAYCLICK_Status_Fail;
        }
    }

    return ATMO_RELAYCLICK_Status_Success;
}


ATMO_RELAYCLICK_Status_t ATMO_RELAYCLICK_SetConfiguration(ATMO_DriverInstanceHandle_t handle, const ATMO_RELAYCLICK_Config_t *config)
{
    if(handle >= currentNumRelayClicks)
    {
        return ATMO_RELAYCLICK_Status_Fail;
    }

    if(config != NULL)
    {
        memcpy(&_config[handle], config, sizeof(ATMO_RELAYCLICK_Config_t));
    }
    else
    {
        return ATMO_RELAYCLICK_Status_Fail;
    }

    // Configure GPIO pins
    ATMO_GPIO_Config_t gpioConfig;
    gpioConfig.pinMode = ATMO_GPIO_PinMode_Output_PushPull;
    gpioConfig.initialState = ATMO_GPIO_PinState_Low; // off

    ATMO_GPIO_SetPinConfiguration(config->gpioDriverInstance, config->relay1Pin, &gpioConfig);
    ATMO_GPIO_SetPinConfiguration(config->gpioDriverInstance, config->relay2Pin, &gpioConfig);
    return ATMO_RELAYCLICK_Status_Success;
}

ATMO_RELAYCLICK_Status_t ATMO_RELAYCLICK_SetRelay(ATMO_DriverInstanceHandle_t handle, ATMO_RELAYCLICK_RelayId_t relay, bool on)
{
    if(handle >= currentNumRelayClicks)
    {
        return ATMO_RELAYCLICK_Status_Fail;
    }

    unsigned int pin = (relay == RELAY_ONE) ? _config[handle].relay1Pin : _config[handle].relay2Pin;

    ATMO_GPIO_SetPinState(_config[handle].gpioDriverInstance, pin, on ? ATMO_GPIO_PinState_High : ATMO_GPIO_PinState_Low);

    return ATMO_RELAYCLICK_Status_Success;
}

ATMO_RELAYCLICK_Status_t ATMO_RELAYCLICK_ToggleRelay(ATMO_DriverInstanceHandle_t handle, ATMO_RELAYCLICK_RelayId_t relay)
{
    if(handle >= currentNumRelayClicks)
    {
        return ATMO_RELAYCLICK_Status_Fail;
    }

    unsigned int pin = (relay == RELAY_ONE) ? _config[handle].relay1Pin : _config[handle].relay2Pin;

    ATMO_GPIO_Toggle(_config[handle].gpioDriverInstance, pin);

    return ATMO_RELAYCLICK_Status_Success;  
}