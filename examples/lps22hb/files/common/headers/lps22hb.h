/**
 ******************************************************************************
 * @file    lps22hb.h
 * @author
 * @version
 * @date
 * @brief   Atmosphere API - lps22hb header file
 ******************************************************************************
 * @attention
 *
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *   1. Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright notice,
 *      this list of conditions and the following disclaimer in the documentation
 *      and/or other materials provided with the distribution.
 *   3. Neither the name of Atmosphere IoT Corp. nor the names of its contributors
 *      may be used to endorse or promote products derived from this software
 *      without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************
 */
#ifndef _ATMO_LPS22HB_H_
#define _ATMO_LPS22HB_H_

#include "../app_src/atmosphere_platform.h"

typedef struct {
    ATMO_DriverInstanceHandle_t i2cInstance;
    ATMO_DriverInstanceHandle_t spiInstance;
    ATMO_DriverInstanceHandle_t gpioInstance;
    uint8_t i2cAddress;
    bool spi3Wire;
    ATMO_GPIO_Device_Pin_t csPin; // Only used for SPI
    ATMO_CORE_DeviceHandle_t driverType;
}ATMO_LPS22HB_Config_t;

typedef enum {
    ATMO_LPS22HB_Status_Success = 0 ,
    ATMO_LPS22HB_Status_Fail
} ATMO_LPS22HB_Status_t;

ATMO_LPS22HB_Status_t ATMO_LPS22HB_Init(ATMO_LPS22HB_Config_t *config);

/**
 * @brief Get pressure data in hPa
 * 
 * @param pressureHpa 
 * @return ATMO_LPS22HB_Status_t 
 */
ATMO_LPS22HB_Status_t ATMO_LPS22HB_GetPressure(float *pressureHpa);

/**
 * @brief Get temperature data in degrees C
 * 
 * @param tempC 
 * @return ATMO_LPS22HB_Status_t 
 */
ATMO_LPS22HB_Status_t ATMO_LPS22HB_GetTempData(float *tempC);

#endif
