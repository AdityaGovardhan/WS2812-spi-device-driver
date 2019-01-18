#include"WS2812_ioctl.h"

#define DRIVER_NAME 	"WS2812_driver"
#define DRIVER_MAJOR	242				// hardcoded the major number for simplicity
#define DEVICE_NAME 	"WS2812"

// device specific structure
typedef struct dev_struct{
	char 				device_name[20];
	struct spi_device 	*spi_device;
	spinlock_t 			spi_lock;		// used when spi_device is operated on
	struct mutex		buf_lock;		// used when tx_buffer is operated on
	u8					*tx_buffer;
} *dev_ptr;