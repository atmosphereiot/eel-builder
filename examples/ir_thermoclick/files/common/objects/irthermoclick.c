#include "irthermoclick.h"

#define THERMOCLICK_TEMP_REG_ADDR (0x7)

typedef struct {
    ATMO_THERMOCLICK_Config_t config;
    bool configured;
} ATMO_THERMOCLICK_Priv_Config;

static ATMO_THERMOCLICK_Priv_Config _ATMO_THERMOCLICK_config;

static ATMO_I2C_Peripheral_t _ATMO_THERMOCLICK_i2cConfig = {
    .operatingMode = ATMO_I2C_OperatingMode_Master,
    .baudRate = ATMO_I2C_BaudRate_Standard_Mode
};

ATMO_THERMOCLICK_Status_t ATMO_THERMOCLICK_Init(ATMO_THERMOCLICK_Config_t *config)
{
    // Did the user supply a configuration?
    if( config )
    {
        ATMO_THERMOCLICK_SetConfiguration(config);
    }
    else
    {
        _ATMO_THERMOCLICK_config.configured = false;
    }

    return ATMO_THERMOCLICK_Status_Success;
}

ATMO_THERMOCLICK_Status_t ATMO_THERMOCLICK_SetConfiguration(const ATMO_THERMOCLICK_Config_t *config)
{
    if( config == NULL )
    {
        return ATMO_THERMOCLICK_Status_Fail;
    }

    if( ATMO_I2C_SetConfiguration(config->i2cDriverInstance, &_ATMO_THERMOCLICK_i2cConfig) != ATMO_I2C_Status_Success )
    {
        return ATMO_THERMOCLICK_Status_Fail;
    }
    memcpy( &_ATMO_THERMOCLICK_config.config, config, sizeof(ATMO_THERMOCLICK_Config_t) );
    _ATMO_THERMOCLICK_config.configured = true;

    return ATMO_THERMOCLICK_Status_Success;
}

ATMO_THERMOCLICK_Status_t ATMO_THERMOCLICK_GetConfiguration(ATMO_THERMOCLICK_Config_t *config)
{
    if( config == NULL || !_ATMO_THERMOCLICK_config.configured )
    {
        return ATMO_THERMOCLICK_Status_Fail;
    }

    memcpy(config, &_ATMO_THERMOCLICK_config.config, sizeof(ATMO_THERMOCLICK_Config_t));

    return ATMO_THERMOCLICK_Status_Success;
}

ATMO_THERMOCLICK_Status_t ATMO_THERMOCLICK_GetTemperature(int32_t *temperatureCelsius)
{
    if( !_ATMO_THERMOCLICK_config.configured )
    {
        return ATMO_THERMOCLICK_Status_Fail;
    }

	uint8_t resp[2] = {0};
    uint8_t reg_addr = THERMOCLICK_TEMP_REG_ADDR;
	ATMO_I2C_Status_t readStatus = ATMO_I2C_MasterRead(_ATMO_THERMOCLICK_config.config.i2cDriverInstance, _ATMO_THERMOCLICK_config.config.address, &reg_addr, 1, resp, 2, 0);

    if( readStatus != ATMO_I2C_Status_Success )
    {
        return ATMO_THERMOCLICK_Status_Fail;
    }

	uint16_t rawTemp = (resp[1] << 8) | resp[0];
	*temperatureCelsius = (uint32_t)((rawTemp * .02) - 273.0);

    return ATMO_THERMOCLICK_Status_Success;
}
