	ATMO_CreateValueInt(
		out, 
		LIS3DH_GetZAxisRawAcceleration(
			ATMO_PROPERTY(undefined, spiInstance), 
			ATMO_PROPERTY(undefined, csPin)));
	
	return ATMO_Status_Success;
