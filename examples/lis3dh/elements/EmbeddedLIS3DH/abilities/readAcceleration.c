	
    ATMO_3dFloatVector_t data;
    data.x = LIS3DH_GetXAxisAcceleration(
			ATMO_PROPERTY(undefined, spiInstance), 
			ATMO_PROPERTY(undefined, csPin));
    
    data.y = LIS3DH_GetYAxisAcceleration(
			ATMO_PROPERTY(undefined, spiInstance), 
			ATMO_PROPERTY(undefined, csPin));

    data.z = LIS3DH_GetZAxisAcceleration(
			ATMO_PROPERTY(undefined, spiInstance), 
			ATMO_PROPERTY(undefined, csPin));

    ATMO_CreateValue3dVectorFloat(out, &data);
	return ATMO_Status_Success;
