# Relay Click EEL
This example demonstrates a basic GPIO EEL. This click controls two micro-relays via GPIO. 

## Notes
* This click contains a variable with a driver-implemented handle. This allows for multiple relay click elements to be used in a project. Variables are global and shared between objects and can be accessed or set using the ATMO_VARIABLE macro, which can be seen in the element code of this EEL. Using a handle variable is considered good practice, and in the future all EELs will implement this feature.

