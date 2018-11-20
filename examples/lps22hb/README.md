# LPS22HB EEL
This example demonstrates a sensor that can be read over multiple protocols. the LPS22HB is an atmospheric presure sensor that can be read over either I2C or SPI.

## Notes
* In the EEL metadata, there is a property called `communicationMode`. The available driver types are:
    - `ATMO_DRIVERTYPE_ADC`
    - `ATMO_DRIVERTYPE_BLE`
    - `ATMO_DRIVERTYPE_BLOCK`
    - `ATMO_DRIVERTYPE_CLOUD`
    - `ATMO_DRIVERTYPE_DATETIME`
    - `ATMO_DRIVERTYPE_FILESYSTEM`
    - `ATMO_DRIVERTYPE_GPIO`
    - `ATMO_DRIVERTYPE_HTTP`
    - `ATMO_DRIVERTYPE_I2C`
    - `ATMO_DRIVERTYPE_INTERVAL`
    - `ATMO_DRIVERTYPE_NFC`
    - `ATMO_DRIVERTYPE_PWM`
    - `ATMO_DRIVERTYPE_SPI`
    - `ATMO_DRIVERTYPE_UART`
    - `ATMO_DRIVERTYPE_WIFI`
    
    These are constants defined in the base Atmosphere embedded firmware. 

* There is currently no way to hide irrelevant properties (eg. SPI CS if I2C is being used).

