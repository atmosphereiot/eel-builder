# Motion Click EEL
This example demonstrates a more advanced GPIO eel that uses interrupts and asynchronous events. 

When the click detects motion, it sends an interrupt over GPIO. This interrupt is handled by the EEL and then the appropriate ability is dispatched. 

## Notes
* The `detectMotion` ability is hidden, since it's not intended to be accessed by the user. In this case, this ability is just used as a way to get data to the `motionDetected` trigger. Although this ability is hidden, it still has code, albeit an empty function. In the Atmosphere Code View, you can inject code into this function for debug purposes. 
* The `ATMO_ABILITY` macro is used to get the integer handle of the `detectMotion` ability. 
* The ISR here is not a true ISR. The true ISR is handled by the Atmosphere Framework. An event is then sent through the atmosphere framework to execute a callback registered by the EEL. This callback is _not_ executed within the context of the ISR. This callback is registered using the `ATMO_GPIO_RegisterInterruptCallback` function.

