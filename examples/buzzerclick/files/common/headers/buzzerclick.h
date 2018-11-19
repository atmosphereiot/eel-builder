#ifndef __ATMO_BUZZERCLICK_H_
#define __ATMO_BUZZERCLICK_H_

#include "../app_src/atmosphere_platform.h"
#include "../pwm/pwm.h"

typedef enum {
    ATMO_BUZZERCLICK_Status_Success              = 0x00u,  // Common - Operation was successful
    ATMO_BUZZERCLICK_Status_Fail                 = 0x01u,  // Common - Operation failed
    ATMO_BUZZERCLICK_Status_Initialized          = 0x02u,  // Common - Peripheral already initialized
    ATMO_BUZZERCLICK_Status_Invalid              = 0x03u,  // Common - Invalid operation or result
    ATMO_BUZZERCLICK_Status_NotSupported         = 0x04u,  // Common - Feature not supported by platform
} ATMO_BUZZERCLICK_Status_t;

typedef struct {
    unsigned int pwmPin;
    unsigned int initialVolumePct;
    unsigned int initialFrequency;
    ATMO_DriverInstanceHandle_t pwmDriverInstance;
} ATMO_BUZZERCLICK_Config_t;

ATMO_BUZZERCLICK_Status_t ATMO_BUZZERCLICK_Init(ATMO_BUZZERCLICK_Config_t *config);

ATMO_BUZZERCLICK_Status_t ATMO_BUZZERCLICK_TurnOn();

ATMO_BUZZERCLICK_Status_t ATMO_BUZZERCLICK_TurnOff();

ATMO_BUZZERCLICK_Status_t ATMO_BUZZERCLICK_SetVolumePct(unsigned int volume);

ATMO_BUZZERCLICK_Status_t ATMO_BUZZERCLICK_Beep();


#endif