#ifndef __ATMO_RELAYCLICK_H_
#define __ATMO_RELAYCLICK_H_

#include "../app_src/atmosphere_platform.h"

typedef enum {
    ATMO_RELAYCLICK_Status_Success              = 0x00u,  // Common - Operation was successful
    ATMO_RELAYCLICK_Status_Fail                 = 0x01u,  // Common - Operation failed
    ATMO_RELAYCLICK_Status_Initialized          = 0x02u,  // Common - Peripheral already initialized
    ATMO_RELAYCLICK_Status_Invalid              = 0x03u,  // Common - Invalid operation or result
    ATMO_RELAYCLICK_Status_NotSupported         = 0x04u,  // Common - Feature not supported by platform
} ATMO_RELAYCLICK_Status_t;

typedef struct {
    unsigned int relay1Pin;
    unsigned int relay2Pin;
    ATMO_DriverInstanceHandle_t gpioDriverInstance;
} ATMO_RELAYCLICK_Config_t;

typedef enum {
    RELAY_ONE,
    RELAY_TWO,
} ATMO_RELAYCLICK_RelayId_t;

/**
 * Initialize RELAYCLICK Driver
 *
 * @param[out] handle
 * @param[in] config - Device configuration (optional)
 */
ATMO_RELAYCLICK_Status_t ATMO_RELAYCLICK_Init(ATMO_DriverInstanceHandle_t *handle, ATMO_RELAYCLICK_Config_t *config);

/**
 * Set basic device configuration
 *
 * @param[in] handle
 * @param[in] config
 */
ATMO_RELAYCLICK_Status_t ATMO_RELAYCLICK_SetConfiguration(ATMO_DriverInstanceHandle_t handle, const ATMO_RELAYCLICK_Config_t *config);

/**
 * Set relay on or off
 * 
 * @param[in] handle
 * @param[in] relay
 * @param[in] on
 */
ATMO_RELAYCLICK_Status_t ATMO_RELAYCLICK_SetRelay(ATMO_DriverInstanceHandle_t handle, ATMO_RELAYCLICK_RelayId_t relay, bool on);

/**
 * Toggle relay
 * 
 * @param[in] handle
 * @param[in] relay
 */ 
ATMO_RELAYCLICK_Status_t ATMO_RELAYCLICK_ToggleRelay(ATMO_DriverInstanceHandle_t handle, ATMO_RELAYCLICK_RelayId_t relay);

#endif