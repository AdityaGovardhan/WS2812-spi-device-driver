insmod WS2812_device.ko
insmod WS2812_driver.ko
./testapp
rmmod WS2812_device.ko
rmmod WS2812_driver.ko