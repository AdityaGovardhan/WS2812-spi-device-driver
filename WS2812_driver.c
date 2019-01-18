/* 
 * Driver module for WS2812_device
 * init and exit functions are called when driver is inserted or removed
 * probe and remove functions are called when device is inserted or removed
 * file operation functions are called by user level programs
 */

#include<linux/device.h>
#include<linux/module.h>
#include<linux/fs.h>
#include<linux/slab.h>
#include<linux/uaccess.h>
#include<linux/spi/spi.h>
#include<linux/err.h>
#include<linux/cdev.h>

#include"WS2812_driver.h"

struct class *dev_class;
static struct device *dev_device;

dev_ptr device;


int WS2812_open(struct inode *inode, struct file *filp){
	
	printk(KERN_INFO "OPEN %s\n", device->device_name);
	device->tx_buffer = kmalloc(480, GFP_KERNEL);
	
	return 0;
}

int WS2812_release(struct inode *inode, struct file *filp){
	
	printk(KERN_INFO "RELEASE %s\n", device->device_name);
	
	return 0;
}

ssize_t WS2812_write(struct file *filp, const char __user *buff, size_t count, loff_t *offp){

	int status;

	if(copy_from_user(device->tx_buffer, (void __user *) buff, count)){
		printk(KERN_INFO "WRITE %s copy from user failed\n", device->device_name);
		return -ENOMEM;
	}
	status = spi_write(device->spi_device, device->tx_buffer, count);
	printk(KERN_INFO "WRITE status = %d\n", status);

	return count;
}

long WS2812_ioctl(struct file *filp, unsigned int cmd, unsigned long arg){
	
	printk(KERN_INFO "IOCTL %s\n", device->device_name);

	switch(cmd){
		case RESET:
			printk(KERN_INFO "RESET %s\n", device->device_name);
	}

	return 0;
}

static int WS2812_probe(struct spi_device *spi_device){

	device = kmalloc(sizeof(struct dev_struct), GFP_KERNEL);

	sprintf(device->device_name, "%s", DEVICE_NAME);
	device->spi_device = spi_device;
	spin_lock_init(&device->spi_lock);
	mutex_init(&device->buf_lock);

	dev_device = device_create(dev_class, &spi_device->dev, MKDEV(DRIVER_MAJOR, 0), device, device->device_name);
	
	spi_set_drvdata(spi_device, device);
	
	printk(KERN_INFO "PROBE complete\n");
	return 0;
}

static int WS2812_remove(struct spi_device *spi_device){

	dev_ptr	device = spi_get_drvdata(spi_device);

	spin_lock_irq(&device->spi_lock);
	device->spi_device = NULL;
	spin_unlock_irq(&device->spi_lock);

	device_destroy(dev_class, MKDEV(DRIVER_MAJOR, 0));

	kfree(device);

	printk(KERN_INFO "REMOVE complete\n");
	return 0;
}

static const struct spi_device_id WS2812_id[] = {
	{DEVICE_NAME, 0},
	{}
};

static struct spi_driver WS2812_driver = {
	.driver = {
		.name = DRIVER_NAME,
		.owner = THIS_MODULE,
	},
	.probe = WS2812_probe,
	.remove = WS2812_remove,
	.id_table = WS2812_id,
};

static struct file_operations fops = {
	.owner 			= 	THIS_MODULE,
	.open 			= 	WS2812_open,
	.release 		= 	WS2812_release,
	.write 			= 	WS2812_write,
	.unlocked_ioctl = 	WS2812_ioctl,
};

static int __init WS2812_init(void){

	int status;

	status = register_chrdev(DRIVER_MAJOR, DRIVER_NAME, &fops);
	if(status < 0){
		return status;
	}

	dev_class = class_create(THIS_MODULE, DRIVER_NAME);
	if (IS_ERR(dev_class)) {
		unregister_chrdev(DRIVER_MAJOR, DRIVER_NAME);
		return PTR_ERR(dev_class);
	}

	status = spi_register_driver(&WS2812_driver);
	if (status < 0) {
		class_destroy(dev_class);
		unregister_chrdev(DRIVER_MAJOR, DRIVER_NAME);
		return status;
	}

	printk(KERN_INFO "DRIVER INIT complete\n");
	return 0;
}

static void __exit WS2812_exit(void){

	spi_unregister_driver(&WS2812_driver);

	class_destroy(dev_class);

	unregister_chrdev(DRIVER_MAJOR, DRIVER_NAME);

	printk(KERN_INFO "DRIVER EXIT complete\n");
}

module_init(WS2812_init);
module_exit(WS2812_exit);
MODULE_LICENSE("Dual BSD/GPL");