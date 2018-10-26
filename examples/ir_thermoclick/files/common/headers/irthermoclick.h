#ifndef _ATMO_THERMOCLICK_H_
#define _ATMO_THERMOCLICK_H_

#include "../app_src/atmosphere_platform.h"
#include "../i2c/i2c.h"

typedef enum {
    ATMO_THERMOCLICK_Status_Success              = 0x00u,  // Common - Operation was successful
    ATMO_THERMOCLICK_Status_Fail                 = 0x01u,  // Common - Operation failed
    ATMO_THERMOCLICK_Status_Initialized          = 0x02u,  // Common - Peripheral already initialized
    ATMO_THERMOCLICK_Status_Invalid              = 0x03u,  // Common - Invalid operation or result
    ATMO_THERMOCLICK_Status_NotSupported         = 0x04u,  // Common - Feature not supported by platform
} ATMO_THERMOCLICK_Status_t;

typedef struct {
    uint16_t address;
    ATMO_DriverInstanceHandle_t i2cDriverInstance;
} ATMO_THERMOCLICK_Config_t;

/**
 * Initialize THERMOCLICK Driver
 *
 * @param[in] config - Device configuration (optional)
 */
ATMO_THERMOCLICK_Status_t ATMO_THERMOCLICK_Init(ATMO_THERMOCLICK_Config_t *config);

/**
 * Set basic device configuration
 *
 * @param[in] config
 */
ATMO_THERMOCLICK_Status_t ATMO_THERMOCLICK_SetConfiguration(const ATMO_THERMOCLICK_Config_t *config);

/**
 * Get device configuration
 * 
 * @param[out] config
 */
ATMO_THERMOCLICK_Status_t ATMO_THERMOCLICK_GetConfiguration(ATMO_THERMOCLICK_Config_t *config);

/**
 * Get Temperature in degrees celsius
 * 
 * @param[out] temperature
 */
ATMO_THERMOCLICK_Status_t ATMO_THERMOCLICK_GetTemperature(int32_t *temperatureCelsius);

#endif
