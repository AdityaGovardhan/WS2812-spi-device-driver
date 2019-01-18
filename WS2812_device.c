/* 
 * Device module for WS2812_device
 * bus_num = 1
 * chip_select = 1
 * max_spped_hz = 8000000
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/spi/spi.h>

#include"WS2812_driver.h"

static struct spi_device *spi_device;
 
static int __init device_init(void){
	
	struct spi_master *master;

	struct spi_board_info device_info = {
		.modalias = DEVICE_NAME,
		.max_speed_hz = 8000000,
		.bus_num = 1,
		.chip_select = 1,
		.mode = 3,
	};

	master = spi_busnum_to_master(device_info.bus_num);
	if(!master){
		printk(KERN_ALERT "master not found\n");
		return -ENODEV;
	}
	 
	spi_device = spi_new_device(master, &device_info);

	if(!spi_device){
		printk(KERN_ALERT "failed to create slave\n");
		return -ENODEV;
	}

	spi_device->bits_per_word = 10;
	spi_setup(spi_device);

	printk(KERN_INFO "DEVICE INIT complete\n");
	 
	return 0;
}

static void __exit device_exit(void){

	spi_unregister_device(spi_device);
    printk(KERN_INFO "DEVICE EXIT complete\n");
}
 
module_init(device_init);
module_exit(device_exit);
 
MODULE_LICENSE("GPL");