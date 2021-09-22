#ifndef RISHI_H
#define RISHI_H

#include <linux/ioctl.h>

 
#define MAJOR_NUM 88


 
#define IOCTL_GYRO1 _IOR(MAJOR_NUM, 0, char *)     

#define IOCTL_GYRO2 _IOR(MAJOR_NUM, 1, char *)

#define IOCTL_GYRO3 _IOR(MAJOR_NUM, 2, char *)

#define IOCTL_ACC1 _IOR(MAJOR_NUM, 3, char *)   

#define IOCTL_ACC2 _IOR(MAJOR_NUM, 4, char *)   

#define IOCTL_ACC3 _IOR(MAJOR_NUM, 5, char *)   

#define IOCTL_COMP1 _IOR(MAJOR_NUM, 6, char *)

#define IOCTL_COMP2 _IOR(MAJOR_NUM, 7, char *)            

#define IOCTL_COMP3 _IOR(MAJOR_NUM, 8, char *)


#define IOCTL_PRESS _IOR(MAJOR_NUM, 9, char *)

 
 
#define DEVICE_FILE_NAME "/dev/imu_char"

#endif

