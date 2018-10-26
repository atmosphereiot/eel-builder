#include "lps22hb.h"
#include "lps22hb_reg.h"

static ATMO_LPS22HB_Config_t _LPS22HB_PrivConfig;
static lps22hb_ctx_t _LPS22HB_Ctx;

static int32_t _ATMO_LPS22HB_PlatformWrite(void *handle, uint8_t Reg, uint8_t *Bufp,
                              uint16_t len)
{
	if(_LPS22HB_PrivConfig.driverType.type == ATMO_DRIVERTYPE_I2C )
	{
		uint8_t i2cAddress = *((uint8_t*)handle);
		ATMO_I2C_Status_t status = ATMO_I2C_MasterWrite(_LPS22HB_PrivConfig.i2cInstance, i2cAddress,
		&Reg, 1, Bufp, len, 1000);
		return (status == ATMO_I2C_Status_Success) ? 0 : 1;
	}
	else
	{
		ATMO_GPIO_Device_Pin_t csPin = *((ATMO_GPIO_Device_Pin_t *)handle);
		ATMO_GPIO_SetPinState(_LPS22HB_PrivConfig.gpioInstance, csPin, ATMO_GPIO_PinState_Low);
		ATMO_SPI_Status_t status = ATMO_SPI_MasterWrite(_LPS22HB_PrivConfig.spiInstance, ATMO_SPI_CS_NONE, &Reg, 1, Bufp, len, 1000);
		ATMO_GPIO_SetPinState(_LPS22HB_PrivConfig.gpioInstance, csPin, ATMO_GPIO_PinState_High);
		return (status == ATMO_SPI_Status_Success) ? 0 : 1;
	}
  

}

static int32_t _ATMO_LPS22HB_PlatformRead(void *handle, uint8_t Reg, uint8_t *Bufp,
                             uint16_t len)
{
	if(_LPS22HB_PrivConfig.driverType.type == ATMO_DRIVERTYPE_I2C )
	{
		uint8_t i2cAddress = *((uint8_t*)handle);
		ATMO_I2C_Status_t status = ATMO_I2C_Status_Success;
		uint8_t currentReg = Reg;

		// Need to do discrete reads or block update doesn't work
		for(unsigned int i = 0; (i < len) && (status == ATMO_I2C_Status_Success); i++)
		{
			status = ATMO_I2C_MasterRead(_LPS22HB_PrivConfig.i2cInstance, i2cAddress,
				&currentReg, 1, &Bufp[i], 1, 1000);
			currentReg++;
		}

		return (status == ATMO_I2C_Status_Success) ? 0 : 1;
	}
	else
	{
		ATMO_GPIO_Device_Pin_t csPin = *((ATMO_GPIO_Device_Pin_t *)handle);

		Reg |= 0x80;

		ATMO_GPIO_SetPinState(_LPS22HB_PrivConfig.gpioInstance, csPin, ATMO_GPIO_PinState_Low);
		ATMO_SPI_Status_t status = ATMO_SPI_MasterRead(_LPS22HB_PrivConfig.spiInstance, ATMO_SPI_CS_NONE, &Reg, 1, Bufp, len, 1000);
		ATMO_GPIO_SetPinState(_LPS22HB_PrivConfig.gpioInstance, csPin, ATMO_GPIO_PinState_High);

		return (status == ATMO_SPI_Status_Success) ? 0 : 1;
	}
}

ATMO_LPS22HB_Status_t ATMO_LPS22HB_Init(ATMO_LPS22HB_Config_t *config)
{
    if(config == NULL)
    {
        return ATMO_LPS22HB_Status_Fail;
    }

    memcpy(&_LPS22HB_PrivConfig, config, sizeof(_LPS22HB_PrivConfig));

    _LPS22HB_Ctx.write_reg = _ATMO_LPS22HB_PlatformWrite;
    _LPS22HB_Ctx.read_reg = _ATMO_LPS22HB_PlatformRead;

    if(config->driverType.type == ATMO_DRIVERTYPE_SPI)
    {
		// Initialize SPI
		ATMO_SPI_Peripheral_t spiConf;
		spiConf.operatingMode = ATMO_SPI_OperatingMode_Master;
		spiConf.deviceConfig.baudRate = 2500000;
		spiConf.deviceConfig.clockContinuous = false;
		spiConf.deviceConfig.clockMode = ATMO_SPI_ClockMode_3;
		spiConf.deviceConfig.msbFirst = true;
		spiConf.deviceConfig.ssActiveLow = true;
		ATMO_SPI_SetConfiguration(config->spiInstance, &spiConf);

    	_LPS22HB_Ctx.handle = (void*)&_LPS22HB_PrivConfig.csPin;

    	ATMO_GPIO_Config_t gpioConfig;
    	gpioConfig.initialState = ATMO_GPIO_PinState_High;
    	gpioConfig.pinMode = ATMO_GPIO_PinMode_Output_PushPull;
    	ATMO_GPIO_SetPinConfiguration(config->gpioInstance, config->csPin, &gpioConfig);

    	uint8_t spiModeByte = 0x01;
    	if(!config->spi3Wire)
    	{
    		spiModeByte = 0x00;
    	}

    	lps22hb_write_reg(&_LPS22HB_Ctx, LPS22HB_CTRL_REG1, &spiModeByte, 1);

    	// Reset and mem boot
    	uint8_t resetByte = 0x84;
    	lps22hb_write_reg(&_LPS22HB_Ctx, LPS22HB_CTRL_REG2, &resetByte, 1);

    	ATMO_PLATFORM_DelayMilliseconds(1000);


    	lps22hb_write_reg(&_LPS22HB_Ctx, LPS22HB_CTRL_REG1, &spiModeByte, 1);
    }
    else
    {
		ATMO_I2C_Peripheral_t i2cConfig;
		i2cConfig.operatingMode = ATMO_I2C_OperatingMode_Master;
		i2cConfig.baudRate = ATMO_I2C_BaudRate_Standard_Mode;
		ATMO_I2C_SetConfiguration(config->i2cInstance, &i2cConfig);
    	_LPS22HB_Ctx.handle = (void*)&_LPS22HB_PrivConfig.i2cAddress;

		// Reset and mem boot
    	uint8_t resetByte = 0x84;
    	lps22hb_write_reg(&_LPS22HB_Ctx, LPS22HB_CTRL_REG2, &resetByte, 1);

		ATMO_PLATFORM_DelayMilliseconds(1000);
    }

    uint8_t whoamI = 0;
    lps22hb_device_id_get(&_LPS22HB_Ctx, &whoamI);

    if ( whoamI != LPS22HB_ID )
    {
        ATMO_PLATFORM_DebugPrint("Invalid lps22hb Accel WhoAmI (Rcv %02X Expect %02X\r\n", whoamI, LPS22HB_ID);
        return ATMO_LPS22HB_Status_Fail;
    }

    if(config->driverType.type == ATMO_DRIVERTYPE_SPI)
    {
    	lps22hb_auto_add_inc_set(&_LPS22HB_Ctx, 1);
    }

    /*
    *  Enable Block Data Update
    */
    lps22hb_block_data_update_set(&_LPS22HB_Ctx, 1);
    /*
    * Set Output Data Rate
    */
    lps22hb_data_rate_set(&_LPS22HB_Ctx, LPS22HB_ODR_10_Hz);

    return ATMO_LPS22HB_Status_Success;
}

/**
 * @brief Get pressure data in hPa
 * 
 * @param pressureHpa 
 * @return ATMO_LPS22HB_Status_t 
 */
ATMO_LPS22HB_Status_t ATMO_LPS22HB_GetPressure(float *pressureHpa)
{
    uint8_t pressureData[3] = {0};
    lps22hb_pressure_raw_get(&_LPS22HB_Ctx, pressureData);

    int32_t pressure = (pressureData[2] << 16) | (pressureData[1] << 8) | pressureData[0];
    *pressureHpa = LPS22HB_FROM_LSB_TO_hPa(pressure);
    return ATMO_LPS22HB_Status_Success;
}

/**
 * @brief Get temperature data in degrees C
 * 
 * @param data 
 * @return ATMO_LPS22HB_Status_t 
 */
ATMO_LPS22HB_Status_t ATMO_LPS22HB_GetTempData(float *tempC)
{
    uint8_t rawData[2] = {0};
    lps22hb_temperature_raw_get(&_LPS22HB_Ctx, rawData);

    int16_t tempDataInt = (rawData[1] << 8) | rawData[0];
    *tempC = LPS22HB_FROM_LSB_TO_degC( tempDataInt );
    return ATMO_LPS22HB_Status_Success;
}
