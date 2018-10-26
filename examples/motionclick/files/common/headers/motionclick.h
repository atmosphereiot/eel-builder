#ifndef _ATMO_MOTION_CLICK_H_
#define _ATMO_MOTION_CLICK_H_

#include "../app_src/atmosphere_platform.h"
#include "../gpio/gpio.h"

typedef enum {
    ATMO_MOTION_CLICK_Status_Success              = 0x00u,  // Common - Operation was successful
    ATMO_MOTION_CLICK_Status_Fail                 = 0x01u,  // Common - Operation failed
    ATMO_MOTION_CLICK_Status_Initialized          = 0x02u,  // Common - Peripheral already initialized
    ATMO_MOTION_CLICK_Status_Invalid              = 0x03u,  // Common - Invalid operation or result
    ATMO_MOTION_CLICK_Status_NotSupported         = 0x04u,  // Common - Feature not supported by platform
} ATMO_MOTION_CLICK_Status_t;

typedef struct {
    ATMO_GPIO_Device_Pin_t enablePin;
    ATMO_GPIO_Device_Pin_t motionPin;
    ATMO_DriverInstanceHandle_t gpioDriverInstance;
} ATMO_MOTION_CLICK_Config_t;

/**
 * Initialize MOTION_CLICK Driver
 *
 * @param[in] config - Device configuration (optional)
 */
ATMO_MOTION_CLICK_Status_t ATMO_MOTION_CLICK_Init(ATMO_MOTION_CLICK_Config_t *config);

/**
 * Set basic device configuration
 *
 * @param[in] config
 */
ATMO_MOTION_CLICK_Status_t ATMO_MOTION_CLICK_SetConfiguration(const ATMO_MOTION_CLICK_Config_t *config);

/**
 * Get Temperature in degrees celsius
 * 
 * @param[out] temperature
 */
ATMO_MOTION_CLICK_Status_t ATMO_MOTION_CLICK_RegisterMotionDetectedAbilityHandle(unsigned int abilityHandle);

#endif
