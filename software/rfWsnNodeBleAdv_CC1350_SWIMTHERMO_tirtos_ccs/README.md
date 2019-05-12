## Example Summary

The WSN Node example illustrates how to create a Wireless Sensor
Network Node device which sends packets to a concentrator. This example is
meant to be used together with the WSN Concentrator example to form a one-
to-many network where the nodes send messages to the concentrator.

This examples showcases the use of several Tasks, Semaphores and Events to
get sensor updates and send packets with acknowledgment from the concentrator.
For the radio layer, this example uses the EasyLink API which provides an
easy-to-use API for the most frequently used radio operations.

## Peripherals Exercised

* `Board_PIN_LED0` - Toggled when the a packet is sent
* `Board_ADCCHANNEL_A0` - Used to measure the Analog Light Sensor by the SCE task
* `Board_PIN_BUTTON0` - Selects fast report or slow report mode. In slow report
mode the sensor data is sent every 5s or as fast as every 1s if there is a
significant change in the ADC reading. in fast reporting mode the sensor data
is sent every 1s regardless of the change in ADC value. The default is slow
reporting mode.


## Resources & Jumper Settings

> If you're using an IDE (such as CCS or IAR), please refer to Board.html in your project
directory for resources used and board-specific jumper settings. Otherwise, you can find
Board.html in the directory &lt;SDK_INSTALL_DIR&gt;/source/ti/boards/&lt;BOARD&gt;.

## Example Usage

* Run the example. On another board run the WSN Concentrator example.
This node should show up on the serial console and LCD of the Concentrator. 
As the LED toggles the Node ADC Reading will update on the display. 

```shell
Node ID: 0xa9
Node ADC Reading: 0010
```

## Application Design Details

* This examples consists of two tasks, one application task and one radio
protocol task. It also consists of an Sensor Controller Engine, SCE, Task which
samples the ADC.

* On initialization the CM3 application sets the minimum report interval and
the minimum change value which is used by the SCE task to wake up the CM3. The
ADC task on the SCE checks the ADC value once per second. If the ADC value has
changed by the minimum change amount since the last time it notified the CM3,
it wakes it up again. If the change is less than the masked value, then it
does not wake up the CM3 unless the minimum report interval time has expired.

* The NodeTask waits to be woken up by the SCE. When it wakes up it toggles
`Board_PIN_LED1` and sends the new ADC value to the NodeRadioTask.

* The NodeRadioTask handles the radio protocol. This sets up the EasyLink
API and uses it to send new ADC values to the concentrator. After each sent
packet it waits for an ACK packet back. If it does not get one, then it retries
three times. If it did not receive an ACK by then, then it gives up.

*RadioProtocol.h* can also be used to change the
PHY settings to be either the default IEEE 802.15.4g 50kbit,
Long Range Mode or custom settings. In the case of custom settings,
the *smartrf_settings.c* file is used. This can be changed either
by exporting from Smart RF Studio or directly in the file.

Note for IAR users: When using the CC1310DK, the TI XDS110v3 USB Emulator must
be selected. For the CC1310_LAUNCHXL, select TI XDS110 Emulator. In both cases,
select the cJTAG interface.

## References
* For more information on the EasyLink API and usage refer to [SimpleLink-EasyLink](http://processors.wiki.ti.com/index.php/SimpleLink-EasyLink).
