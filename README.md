# WS2812-device-driver

This software is a SPI driver for WS2812 LED controller based LED ring.

D_in pin of device should be connected to MOSI of SPI bus 1.
Vcc and GND at appropriate pin locations.

Driver Compilation and Usage:
=============================

The driver is installed using command line directed at the driver file location, with following commands
1) make (only in host system)
2) insmod WS2812_device.ko
3) insmod WS2812_driver.ko

The user level code can access the device files and run the user level code using
4) ./testapp

The driver can be uninstalled using
5) rmmod WS2812_device.ko
6) rmmod WS2812_driver.ko
7) make clean (only in host system)

OR

1) bash run.sh (only in host system)
2) bash runapp.sh


OPEN:
-----
Device file is created in /dev folder with "WS2812" name. This needs to be opened to interact with the device.

CLOSE:
------
The device file should be closed after use to maintain proper functioning of the devices.

IOCTL:
------
To reset the device, RESET command is passed to the device with no arguments i.e. NULL.

WRITE:
------
0 and 1 are represented by 10 bits.
Writing to the device is done using an array of 480 consecutive bytes. ((16leds * 24bitRGB * 10bits) / 8bits = 480 bytes)
0 = 1110000000
1 = 1111110000
