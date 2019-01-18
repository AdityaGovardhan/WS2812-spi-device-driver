#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<stdlib.h>
#include<inttypes.h>
#include<sys/ioctl.h>

#include"WS2812_ioctl.h"

#define SYSFS_GPIO_DIR "/sys/class/gpio"
#define MAX_BUF 64

/*
 * export gpio number
 * @gpio - gpio number to be exported
 */
int gpio_export(unsigned int gpio){
	int fd, len;
	char buf[MAX_BUF];
 
	fd = open(SYSFS_GPIO_DIR "/export", O_WRONLY);
	if (fd < 0) {
		perror("gpio/export\n");
		return fd;
	}
 
	len = snprintf(buf, sizeof(buf), "%d", gpio);
	write(fd, buf, len);
	close(fd);
 
	return 0;
}

/*
 * set value for gpio number
 * @gpio - gpio number to be set
 */
int gpio_set_value(unsigned int gpio, unsigned int level){
	int fd;
	char buf[MAX_BUF];
 
	snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/gpio%d/value", gpio);
 
	fd = open(buf, O_WRONLY);
	if (fd < 0) {
		perror("gpio/value\n");
		return fd;
	}

	if(level == 0){
		write(fd, "0", 1);
	}else if(level == 1){
		write(fd, "1", 1);
	}
	
	close(fd);
	return 0;
}

/*
 * unexport gpio number
 * @gpio - gpio number to be unexported
 */
int gpio_unexport(unsigned int gpio){
	int fd, len;
	char buf[MAX_BUF];
 
	fd = open(SYSFS_GPIO_DIR "/unexport", O_WRONLY);
	if (fd < 0) {
		perror("gpio/unexport\n");
		return fd;
	}
 
	len = snprintf(buf, sizeof(buf), "%d", gpio);
	write(fd, buf, len);
	close(fd);
	return 0;
}

int mux_gpio_set(unsigned int gpio, unsigned int level){
	gpio_export(gpio);
	gpio_set_value(gpio, level);

	return 0;
}

// GPIO install function
void IOSetup_init(void){
	mux_gpio_set(24, 0);

	mux_gpio_set(44, 1);
	mux_gpio_set(72, 0);
}

// GPIO uninstall function
void IOSetup_exit(void){
	gpio_unexport(24);

	gpio_unexport(44);
	gpio_unexport(72);
}

int main(){
	int fd, i, k;

	uint8_t array[480];

	k = 0;
	for(i = 0; i < 48; i++){
		array[k] 		= 0b11100000;
		array[k + 1] 	= 0b00111000;
		array[k + 2] 	= 0b00001110;
		array[k + 3] 	= 0b00000011;
		array[k + 4] 	= 0b10000000;
		array[k + 5] 	= 0b11100000;
		array[k + 6] 	= 0b00111000;
		array[k + 7] 	= 0b00001110;
		array[k + 8] 	= 0b00000011;
		array[k + 9] 	= 0b10000000;
		k = k + 10;
	}
	
	void *input;
	input = array;

	IOSetup_init();

	fd = open("/dev/WS2812", O_RDWR);
	
	write(fd, input, 480);
	ioctl(fd, RESET, NULL);

	IOSetup_exit();

	close(fd);
	return 0;
}