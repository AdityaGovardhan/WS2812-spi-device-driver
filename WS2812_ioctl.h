// use 'h' as a magic number
#define IOC_MAGIC 	'h'

// command with no arguments
#define RESET 	_IOW(IOC_MAGIC, 0, unsigned long) 