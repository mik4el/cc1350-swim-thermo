## Example Summary

Application that toggles board LEDs using a Pin interrupt.

## Peripherals Exercised

* `Board_LED0` - Toggled by Board_BUTTON0
* `Board_LED1` - Toggled by Board_BUTTON1
* `Board_BUTTON0` - Toggles Board_LED0
* `Board_BUTTON1` - Toggles Board_LED1

## Resources & Jumper Settings

> Please refer to the development board's specific __Settings and Resources__
section in the Getting Started Guide. For convenience, a short summary is also
shown below.

| Development board | Notes                                                  |
| ----------------- | ------                                                 |
| CC1310DK          | Close `BTN_RIGHT`, `BTN_LEFT`, `BTN_UP`, `BTN_DOWN`, and `BTN_SELECT`|
| CC2650DK          | ^                                                      |
| CC1310STK-BLE     | Close BTN_RIGHT AND BTN_LEFT                           |
| CC2650STK-BLE     | ^                                                       |
| CC1310LP          | Close RED LED, GREEN LED                               |
| CC1350LP          | ^                                                      |
| CC2650LP          | ^                                                      |

> Fields left blank have no specific settings for this example.
> Fields containing ^, refer to the above settings.

## Example Usage

Run the example.

1. `Board_LED0` is toggled by pushing `Board_BUTTON0`.

2. `Board_LED1` is toggled by pushing `Board_BUTTON1`.

* Each key's interrupt is configured for a falling edge of the pulse. A small
delay has been added to provide button de-bounce logic.

## Application Design Details

* The `pinCallbackFxn` function is defined in the *pinInterrupt.c* file. This function
is called in the context of the pin interrupt (Hwi).

> For IAR users using any SensorTag(STK) Board, the XDS110 debugger must be
selected with the 4-wire JTAG connection within your projects' debugger
configuration.

## References
* For GNU and IAR users, please read the following website for details
  about enabling [semi-hosting](http://processors.wiki.ti.com/index.php/TI-RTOS_Examples_SemiHosting)
  in order to view console output.
