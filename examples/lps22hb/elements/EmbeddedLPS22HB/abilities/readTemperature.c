	
	float temperature = 0.0;
	if( ATMO_LPS22HB_GetTempData(&temperature) != ATMO_LPS22HB_Status_Success )
	{
		return ATMO_Status_Fail;
	}

	ATMO_CreateValueFloat(out, temperature);
	
	return ATMO_Status_Success;
