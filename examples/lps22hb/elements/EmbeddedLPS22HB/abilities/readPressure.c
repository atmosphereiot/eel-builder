	float pressure = 0.0;
	if( ATMO_LPS22HB_GetPressure(&pressure) != ATMO_LPS22HB_Status_Success )
	{
		return ATMO_Status_Fail;
	}

	ATMO_CreateValueFloat(out, pressure);
	
	return ATMO_Status_Success;
