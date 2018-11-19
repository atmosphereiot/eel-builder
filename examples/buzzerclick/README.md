# Buzzer Click EEL
The buzzer click is a buzzer device driven by PWM.

## Notes
* The `ATMO_PLATFORM_DelayMilliseconds` in the `ATMO_BUZZERCLICK_Beep` function is blocking. The `ATMO_PLATFORM_DelayMilliseconds` should be called sparingly, as it affects the execution of the whole stack.