SYSROOTS = /home/adi/Software/iot-devkit/1.7.2/sysroots

PATH := $(PATH):$(SYSROOTS)/x86_64-pokysdk-linux/usr/bin/i586-poky-linux
KDIR = $(SYSROOTS)/i586-poky-linux/usr/src/kernel
SROOT = $(SYSROOTS)/i586-poky-linux/

CC = i586-poky-linux-gcc
ARCH = x86
CROSS_COMPILE = i586-poky-linux-

APP = testapp

obj-m = WS2812_driver.o WS2812_device.o

all:
	make -Wall ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) -C $(KDIR) M=$(PWD) modules
	$(CC) -Wall -o $(APP) testapp.c --sysroot=$(SROOT)

clean:
	make -Wall ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) -C $(KDIR) M=$(PWD) clean
	rm -f $(APP)