/* General Includes */

#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/ioctl.h>
#include<signal.h>
#include<unistd.h>
#include<time.h>

/* IOCTL Interface Definition */

#include "config.h"

/* Char Device Variable */

int file_desc;

/* Log File */

FILE *fptr;

/* SIGINT Signal Handler */

void exitHandler(int sig_num)
{
 signal(SIGINT, exitHandler);
 printf("\nExit Initiated");
 close(file_desc);
 fprintf(fptr,"\n");
 fclose(fptr);
 printf("\nShutdown Procedures Complete.Bye\n");
 exit(0);
}

/* IOCTL Userspace Call for Pressure */
 
int ioctl_pressure(int file_desc, int32_t *msg)
{
 int ret_val;

 ret_val = ioctl(file_desc, IOCTL_PRESSURE,msg);
 return ret_val;
}

/* IOCTL Userspace Call for Temperature */

int ioctl_temperature(int file_desc, int32_t *msg)
{
 int ret_val;

 ret_val = ioctl(file_desc, IOCTL_TEMPERATURE,msg);
 return 0;
}

/* IOCTL Userspace Call for Humidity */

int ioctl_humidity(int file_desc, int32_t *msg)
{
 int ret_val;

 ret_val = ioctl(file_desc, IOCTL_HUMIDITY,msg);
 return 0;
}

/* Main Routine */

int main(void)
{
 int ret_val,i;
 int32_t recv_msg;
 time_t ltime;
 struct tm result;
 char time_string[32];
 float temperature,pressure,humidity;

 /* Register SIGINT Handler */

 signal(SIGINT, exitHandler);

 /* Open Char Device */

 file_desc = open(DEVICE_FILE_NAME,0);

 /* Log File Open */

 fptr = fopen("data.log","a");

 if(file_desc<0)
 {
  printf("Device Open Failed for %s\n",DEVICE_FILE_NAME);
  exit(-1);
 }
 printf("Logging started...\n");

 /* Infinite Execution Block */

 while(1)
 {
  /* Current Timestame Retrieval */

  ltime = time(NULL);
  localtime_r(&ltime, &result);
  asctime_r(&result, time_string);
  i=0;

  /* Format Timestamp for Log Print */

  while(time_string[i]!='\0')
  {
   if(time_string[i]=='\n')
    time_string[i]=0;
   i++;
  }

  /* Print Timestamp to Log File */
 
  fprintf(fptr, "%s : ",time_string);

  /* Retrieve Temperature and Print to Log File */

  ioctl_temperature(file_desc,&recv_msg);
  temperature = (float)recv_msg/100.0;
  fprintf(fptr, "Temperature:%f ",temperature);

  /* Retrieve Pressure and Print to Log File */

  ioctl_pressure(file_desc,&recv_msg);
  pressure = (float)recv_msg/100.0;
  fprintf(fptr, "Pressure:%f ",pressure);

  /* Retrieve Humidity and Print to Log File */

  ioctl_humidity(file_desc,&recv_msg);
  humidity = (float)recv_msg/1024.0;
  fprintf(fptr, "Humidity:%f\n", humidity);

  /* Delay for 5 Seconds */

  sleep(5);
 }
}
