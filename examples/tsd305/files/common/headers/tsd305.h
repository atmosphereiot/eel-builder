#ifndef _ATMO_TSD305_H_
#define _ATMO_TSD305_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "../atmo/core.h"
#include "../app_src/atmosphere_platform.h"


enum tsd305_status
{
	tsd305_status_ok,
	tsd305_status_no_i2c_acknowledge,
	tsd305_status_i2c_transfer_error,
	tsd305_status_busy,
	tsd305_status_memory_error,
	tsd305_status_out_of_range
};

// Functions
void tsd305_init( ATMO_DriverInstanceHandle_t i2cInstance, ATMO_DriverInstanceHandle_t gpioInstance, bool weatherShieldEnable, ATMO_GPIO_Device_Pin_t weatherShieldCsA, ATMO_GPIO_Device_Pin_t weatherShieldCsB, ATMO_GPIO_Device_Pin_t weatherShieldEn );

/**
* \brief Reads the temperature and pressure ADC value and compute the
* compensated values.
*
* \param[out] float* : Celsius Degree temperature value
* \param[out] float* : mbar pressure value
*
* \return tsd305_status : status of TSD305
*       - tsd305_status_ok : I2C transfer completed successfully
*       - tsd305_status_i2c_transfer_error : Problem with i2c transfer
*       - tsd305_status_no_i2c_acknowledge : I2C did not acknowledge
*       - tsd305_status_busy : Sensor is busy
*       - tsd305_status_memory_error : Sensor EEPROM memory error
*/
enum tsd305_status
tsd305_read_temperature_and_object_temperature( float *temperature,
        float *object_temperature );

#ifdef __cplusplus
}
#endif

#endif