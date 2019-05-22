    float temperature, objectTemperature = 0;
    if(tsd305_read_temperature_and_object_temperature(&temperature, &objectTemperature) != tsd305_status_ok)
    {
        return ATMO_Status_Fail;
    }
    ATMO_CreateValueFloat(out, objectTemperature);
    return ATMO_Status_Success;