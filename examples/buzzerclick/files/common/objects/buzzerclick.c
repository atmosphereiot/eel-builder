#include "buzzerclick.h"

static ATMO_BUZZERCLICK_Config_t _ATMO_BUZZER_Config;

void _ATMO_BUZZERCLICK_SetPwmConfig()
{
    ATMO_PWM_Config_t pwmConfig;
    pwmConfig.pwmFreqHz = _ATMO_BUZZER_Config.initialFrequency;
    pwmConfig.dutyCycle = _ATMO_BUZZER_Config.initialVolumePct / 2;

    ATMO_PWM_SetPinConfiguration(_ATMO_BUZZER_Config.pwmDriverInstance, _ATMO_BUZZER_Config.pwmPin, &pwmConfig);   
}

ATMO_BUZZERCLICK_Status_t ATMO_BUZZERCLICK_Init(ATMO_BUZZERCLICK_Config_t *config)
{
    if(config->initialVolumePct > 100)
    {
        return ATMO_BUZZERCLICK_Status_Invalid;
    }

    memcpy(&_ATMO_BUZZER_Config, config, sizeof(ATMO_BUZZERCLICK_Config_t));

    _ATMO_BUZZERCLICK_SetPwmConfig();
    return ATMO_BUZZERCLICK_Status_Success;
}

ATMO_BUZZERCLICK_Status_t ATMO_BUZZERCLICK_TurnOn()
{
    ATMO_PWM_Enable(_ATMO_BUZZER_Config.pwmDriverInstance, _ATMO_BUZZER_Config.pwmPin);
    return ATMO_BUZZERCLICK_Status_Success;
}

ATMO_BUZZERCLICK_Status_t ATMO_BUZZERCLICK_TurnOff()
{
    ATMO_PWM_Disable(_ATMO_BUZZER_Config.pwmDriverInstance, _ATMO_BUZZER_Config.pwmPin);
    return ATMO_BUZZERCLICK_Status_Success;
}

ATMO_BUZZERCLICK_Status_t ATMO_BUZZERCLICK_SetVolumePct(unsigned int volume)
{
    if(volume > 100)
    {
        return ATMO_BUZZERCLICK_Status_Invalid;
    }

    _ATMO_BUZZER_Config.initialVolumePct = volume;

    _ATMO_BUZZERCLICK_SetPwmConfig();
    return ATMO_BUZZERCLICK_Status_Success;
}

ATMO_BUZZERCLICK_Status_t ATMO_BUZZERCLICK_Beep()
{
    ATMO_BUZZERCLICK_TurnOn();
    ATMO_PLATFORM_DelayMilliseconds(200);
    ATMO_BUZZERCLICK_TurnOff();
}
