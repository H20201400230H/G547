#ifndef RISHI_H
#define RISHI_H

#include <linux/ioctl.h>

 
#define MAJOR_NUM 88                    //deciding the major number on our own


 
#define IOCTL_GYRO1 _IOR(MAJOR_NUM, 0, char *)     //GRYOSCOPE1

#define IOCTL_GYRO2 _IOR(MAJOR_NUM, 1, char *)     //GRYOSCOPE2

#define IOCTL_GYRO3 _IOR(MAJOR_NUM, 2, char *)		//GRYOSCOPE3

#define IOCTL_ACC1 _IOR(MAJOR_NUM, 3, char *)      //ACCELERATOR1   

#define IOCTL_ACC2 _IOR(MAJOR_NUM, 4, char *)   	//ACCELERATOR2

#define IOCTL_ACC3 _IOR(MAJOR_NUM, 5, char *)   	//ACCELERATOR3

#define IOCTL_COMP1 _IOR(MAJOR_NUM, 6, char *)      //COMPASS1

#define IOCTL_COMP2 _IOR(MAJOR_NUM, 7, char *)      //COMPASS2      

#define IOCTL_COMP3 _IOR(MAJOR_NUM, 8, char *)      //COMPASS3

#define IOCTL_PRESS _IOR(MAJOR_NUM, 9, char *)       //PRESSURE     

 
 
#define DEVICE_FILE_NAME "/dev/imu_char"

#endif
