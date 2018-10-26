	ATMO_CreateValueFloat(
		out, 
		LIS3DH_GetXAxisAcceleration(
			ATMO_PROPERTY(undefined, spiInstance), 
			ATMO_PROPERTY(undefined, csPin)));
	
	return ATMO_Status_Success;
