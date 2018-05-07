Example Summary
---------------
In this example you will learn how to setup the radio to do bi-directional 
communication. The Echo RX example start off by putting the radio in receiver 
mode and waits for a packet to be transmitted. When it receives a packet it 
switches the radio to transmitter mode and re-transmits the received
packet (echo). This example is meant to be used with the Echo TX example or 
SmartRF Studio. For every packet echo, Board_PIN_LED2 is toggled. The 
frequency and other RF settings can be modified using SmartRF Studio.

Peripherals Exercised
---------------------
* `Board_PIN_LED2` - Blinking indicates a successful reception and 
  re-transmission of a packet (echo)
* `Board_PIN_LED1` - Indicates an error in either reception of a packet or 
  in its re-transmission

Resources & Jumper Settings
---------------------------
> If you're using an IDE (such as CCS or IAR), please refer to Board.html in 
your project directory for resources used and board-specific jumper settings. 
Otherwise, you can find Board.html in the directory 
&lt;SDK_INSTALL_DIR&gt;/source/ti/boards/&lt;BOARD&gt;.

Example Usage
-------------
The user will require two launchpads, one running rfEchoTx (`Board_1`), 
another running rfEchoRx (`Board_2`). Run Board_2 first, followed by 
Board_1. Board_1 is set to transmit a packet every second while Board_2 is 
set to receive the packet and then turnaround and transmit it after a delay of
100ms. Board_PIN_LED1 on Board_1 will toggle when it's able to successfully 
transmits a packet, and when it receives its echo. Board_PIN_LED2 on Board_2 
will toggle when it receives a packet, and once again when it's able to 
re-transmit it (see [figure 1]).

![perfect_echo_ref][figure 1]

If there is an issue in receiving a packet then Board_PIN_LED1 on Board_2 is 
toggled while Board_PIN_LED2 is cleared, to indicate an error condition

![echo_error_ref][figure 2]

Application Design Details
--------------------------
This examples consists of a single task and the exported SmartRF Studio radio
settings.

The default frequency is 868.0 MHz (433.92 MHz for the 
CC1350-LAUNCHXL-433 board). In order to change frequency, modify the
smartrf_settings.c file. This can be done using the code export feature in
Smart RF Studio, or directly in the file.

When the task is executed it:

1. Configures the radio for Proprietary mode
2. Gets access to the radio via the RF drivers RF_open
3. Sets up the radio using CMD_PROP_RADIO_DIV_SETUP command
4. Set the output power to 14 dBm (requires that CCFG_FORCE_VDDR_HH = 1 in ccfg.c)
5. Sets the frequency using CMD_FS command
6. Run the CMD_PROP_RX command and wait for a packet to be transmitted. The 
   receive command is chained with a transmit command, CMD_PROP_TX, which runs
   once a packet is received
7. When a packet is successfully received Board_PIN_LED2 is toggled, the radio
   switches over to the transmit mode and schedules the packet for transmission
   100 ms in the future
8. If there is an issue either with the receive or transmit, an error, both 
   LEDs are set
9. The devices repeat steps 6-8 forever.

Note for IAR users: When using the CC1310DK, the TI XDS110v3 USB Emulator must
be selected. For the CC1310_LAUNCHXL, select TI XDS110 Emulator. In both cases,
select the cJTAG interface.

[figure 1]:rfEcho_PerfectEcho.png "Perfect Echo"
[figure 2]:rfEcho_ErrorTxRx.png "Echo Error"