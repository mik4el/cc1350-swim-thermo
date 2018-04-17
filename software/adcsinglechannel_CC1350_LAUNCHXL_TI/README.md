## Example Summary

Sample application to read ADC sampling results.

## Peripherals Exercised

* `Board_ADCCHANNEL_A0` - ADC channel A0
* `Board_ADCCHANNEL_A1` - ADC channel A1

## Resources & Jumper Settings

>Please refer to the development board's specific __Settings and Resources__
section in the Getting Started Guide. For convenience, a short summary is also
shown below.

| Development board | Notes                                                  |
| ----------------- | ------                                                 |
| CC3200            |                                                        |
| MSP-EXP432P401R   |                                                        |

> Fields left blank have no specific settings for this example.

## Example Usage

* Connect the ADC channel 0 and channel 1 to the sampling sources. For the quick
testing, the pin of channel 0 is connected to 3V3 and the pin of channel 1 is
connected to the ground.

* The example will sample on the specified ADC channels and print the sampling
result on the console. There are two tasks sampling the different ADC channel.
One task does only one sampling and the other task does multiple samplings:
```
        ADC channel 0 initialized
        ADC channel 0 convert result: 0x3FFF
        ADC channel 1 initialized
        ADC channel 1 convert result (0): 0x0000
        ADC channel 1 convert result (1): 0x0000
        ADC channel 1 convert result (2): 0x0000
```
The actual convert result value may vary depending on the reference voltage
settings. Please refer to board specific datasheet for more details.

## Application Design Details

This application uses two tasks:

`'taskFxn0'` - performs the following actions:

1. Opens and initializes an ADC driver object.

2. Uses the ADC driver object to do the one-shot sampling and prints the sampling
result.

3. Closes the ADC driver object.

`'taskFxn1'` - performs the following actions:

1. Opens and initializes an ADC driver object.

2. Uses the ADC driver object to do the multiple samplings and prints each sampling
result.

3. Closes the ADC driver object.

## References
* For GNU and IAR users, please read the following website for details
  about enabling [semi-hosting](http://processors.wiki.ti.com/index.php/TI-RTOS_Examples_SemiHosting)
  in order to view console output.
