#ifndef CHAR_CONFIG_H
#define CHAR_CONFIG_H

#include <linux/ioctl.h>

#define MAGIC_NUM 225

//IOCTL interface prototypes

#define IOCTL_TEMPERATURE _IOWR(MAGIC_NUM, 0, int32_t *)
#define IOCTL_HUMIDITY _IOWR(MAGIC_NUM, 1, int32_t *)
#define IOCTL_PRESSURE _IOWR(MAGIC_NUM, 2, int32_t *)


//Device file interface
#define DEVICE_FILE_NAME "/dev/bme280"

#endif
