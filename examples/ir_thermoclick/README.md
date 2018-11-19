# IR Thermoclick EEL
This example demonstrates a very basic I2C sensor, designed to be read synchronously. This is as bare bones as it gets for an I2C device.

## Notes
* Setting the I2C configuration is optional. In general, the platform setup routine for each hardware type will initialize all I2C buses with reasonable defaults (master mode & standard speed).
* The I2C driver API can be seen [here](https://developer.atmosphereiot.com/documents/apis/i2cdriver.html).