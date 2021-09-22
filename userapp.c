#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/ioctl.h>

#include "config.h"

int ioctl_gyro1(int file_desc, char *msg)
{
 int ret_val;

 ret_val = ioctl(file_desc, IOCTL_GYRO1,msg);
 return 0;
}

int ioctl_gyro2(int file_desc, char *msg)
{
 int ret_val;

 ret_val = ioctl(file_desc, IOCTL_GYRO2,msg);
 return 0;
}

int ioctl_gyro3(int file_desc, char *msg)
{
 int ret_val;

 ret_val = ioctl(file_desc, IOCTL_GYRO3,msg);
 return 0;
}

int ioctl_acc1(int file_desc, char *msg)
{
 int ret_val;

 ret_val = ioctl(file_desc, IOCTL_ACC1,msg);
 return 0;
}

int ioctl_acc2(int file_desc, char *msg)
{
 int ret_val;

 ret_val = ioctl(file_desc, IOCTL_ACC2,msg);
 return 0;
}

int ioctl_acc3(int file_desc, char *msg)
{
 int ret_val;

 ret_val = ioctl(file_desc, IOCTL_ACC3,msg);
 return 0;
}

int ioctl_press(int file_desc, char *msg)
{
 int ret_val;

 ret_val = ioctl(file_desc, IOCTL_PRESS,msg);
 return 0;
}

int ioctl_comp1(int file_desc, char *msg)
{
 int ret_val;

 ret_val = ioctl(file_desc, IOCTL_COMP1,msg);
 return 0;
}

int ioctl_comp2(int file_desc, char *msg)
{
 int ret_val;

 ret_val = ioctl(file_desc, IOCTL_COMP2,msg);
 return 0;
}

int ioctl_comp3(int file_desc, char *msg)
{
 int ret_val;

 ret_val = ioctl(file_desc, IOCTL_COMP3,msg);
 return 0;
}

int main()
{
 int file_desc, ret_val;
 char *msg="From User space\n";
 char message_rec[100];

 file_desc = open(DEVICE_FILE_NAME,0);

 if(file_desc<0)
 {
  printf("Device file not opened. Name:%s\n",DEVICE_FILE_NAME);
  exit(-1);
 }

 ioctl_gyro1(file_desc,message_rec);
 printf("Gyroscope 1: %X",message_rec[0]);
 printf("%X\n",message_rec[1]);

 ioctl_gyro2(file_desc,message_rec);
 printf("Gyroscope 2: %X",message_rec[0]);
 printf("%X\n",message_rec[1]);
 
 ioctl_gyro3(file_desc,message_rec);
 printf("Gyroscope 3: %X",message_rec[0]);
 printf("%X\n",message_rec[1]);

 ioctl_acc1(file_desc,message_rec);
 printf("Accelerator 1: %X",message_rec[0]);
 printf("%X\n",message_rec[1]);

 ioctl_acc2(file_desc,message_rec);
 printf("Accelerator 2: %X",message_rec[0]);
 printf("%X\n",message_rec[1]);

 ioctl_acc3(file_desc,message_rec);
 printf("Accelerator 3: %X",message_rec[0]);
 printf("%X\n",message_rec[1]);

 ioctl_comp1(file_desc,message_rec);
 printf("Compass 1: %X",message_rec[0]);
 printf("%X\n",message_rec[1]);

 ioctl_comp2(file_desc,message_rec);
 printf("Compass 2: %X",message_rec[0]);
 printf("%X\n",message_rec[1]);

 ioctl_comp3(file_desc,message_rec);
 printf("Compass 3: %X",message_rec[0]);
 printf("%X\n",message_rec[1]);

 ioctl_press(file_desc,message_rec);
 printf("Pressure: %X",message_rec[0]);
 printf("%X\n",message_rec[1]);

 close(file_desc);
 return 0;
}



