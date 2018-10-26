/**
 ******************************************************************************
 * @file		lid3dh.c
 * @author Atmosphere IoT Corp.
 * @version
 * @date
 * @brief	 Atmosphere Driver - LIS3DH driver code file
 ******************************************************************************
 * @attention
 *
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *	 1. Redistributions of source code must retain the above copyright notice,
 *			this list of conditions and the following disclaimer.
 *	 2. Redistributions in binary form must reproduce the above copyright notice,
 *			this list of conditions and the following disclaimer in the documentation
 *			and/or other materials provided with the distribution.
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

#include "lis3dh.h"

// -----------------------------------------------------------------------------
/**
 *	Global data
 */

// -----------------------------------------------------------------------------
/**
 *	Private interface
 */

// -----------------------------------------------------------------------------
/**
 *	Public interface
 */

void LIS3DH_Init(ATMO_DriverInstanceHandle_t spiInstance, uint16_t csPin)
{
	ATMO_SPI_Device_t deviceConfig;
	deviceConfig.baudRate = 2000000;
	deviceConfig.clockContinuous = false;
	deviceConfig.clockMode = ATMO_SPI_ClockMode_0;
	deviceConfig.msbFirst = true;
	deviceConfig.ssActiveLow = true;
	deviceConfig.ssContinuous = false;
	ATMO_SPI_MasterSetDeviceConfiguration(0, csPin, &deviceConfig);

	LIS3DH_WriteReg(spiInstance, csPin, LIS3DH_CTRL_REG1_ADDR, 0x77);
}

void LIS3DH_WriteReg(ATMO_DriverInstanceHandle_t spiInstance, uint16_t csPin, uint8_t addr, uint8_t data)
{
	uint8_t writeBytes[2];

	writeBytes[0] = addr;
	writeBytes[1] = data;
	ATMO_SPI_MasterWrite(spiInstance, csPin, NULL, 0, writeBytes, 2, 0);
}

uint8_t LIS3DH_ReadReg(ATMO_DriverInstanceHandle_t spiInstance, uint16_t csPin, uint8_t addr)
{
	uint8_t writeBytes[2];
	uint8_t readBytes[2];

	writeBytes[0] = addr | 0x80;	// set msb high for read operations
	writeBytes[1] = 0xFF;			// dummy byte

	ATMO_SPI_MasterRead(spiInstance, csPin, writeBytes, 2, readBytes, 2, 0);
	return readBytes[1];
}


uint8_t LIS3DH_GetWhoAmIReg(ATMO_DriverInstanceHandle_t spiInstance, uint16_t csPin)
{
	return LIS3DH_ReadReg(spiInstance, csPin, LIS3DH_WHO_AM_I_REG_ADDR);
}

int16_t LIS3DH_GetXAxisRawAcceleration(ATMO_DriverInstanceHandle_t spiInstance, uint16_t csPin)
{
	uint16_t returnData = (uint16_t)LIS3DH_ReadReg(spiInstance, csPin, LIS3DH_OUT_X_H_REG_ADDR) << 8;
	
	returnData |= LIS3DH_ReadReg(spiInstance, csPin, LIS3DH_OUT_X_L_REG_ADDR);
	
	return (int16_t)returnData;
}

int16_t LIS3DH_GetYAxisRawAcceleration(ATMO_DriverInstanceHandle_t spiInstance, uint16_t csPin)
{
	uint16_t returnData = (uint16_t)LIS3DH_ReadReg(spiInstance, csPin, LIS3DH_OUT_Y_H_REG_ADDR) << 8;
	
	returnData |= LIS3DH_ReadReg(spiInstance, csPin, LIS3DH_OUT_Y_L_REG_ADDR);
	
	return (int16_t)returnData;
}

int16_t LIS3DH_GetZAxisRawAcceleration(ATMO_DriverInstanceHandle_t spiInstance, uint16_t csPin)
{
	uint16_t returnData = (uint16_t)LIS3DH_ReadReg(spiInstance, csPin, LIS3DH_OUT_Z_H_REG_ADDR) << 8;
	
	returnData |= LIS3DH_ReadReg(spiInstance, csPin, LIS3DH_OUT_Z_L_REG_ADDR);
	
	return (int16_t)returnData;
}

float LIS3DH_GetXAxisAcceleration(ATMO_DriverInstanceHandle_t spiInstance, uint16_t csPin)
{
	return (float)LIS3DH_GetXAxisRawAcceleration(spiInstance, csPin) / 16384.0f;
}

float LIS3DH_GetYAxisAcceleration(ATMO_DriverInstanceHandle_t spiInstance, uint16_t csPin)
{
	return (float)LIS3DH_GetYAxisRawAcceleration(spiInstance, csPin) / 16384.0f;
}

float LIS3DH_GetZAxisAcceleration(ATMO_DriverInstanceHandle_t spiInstance, uint16_t csPin)
{
	return (float)LIS3DH_GetZAxisRawAcceleration(spiInstance, csPin) / 16384.0f;
}
