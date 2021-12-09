#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/ioctl.h>
#include<signal.h>
#include<unistd.h>
#include<time.h>

#include "config.h"

int file_desc;
FILE *fptr;

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
 
int ioctl_pressure(int file_desc, int32_t *msg)
{
 int ret_val;

 ret_val = ioctl(file_desc, IOCTL_PRESSURE,msg);
 return ret_val;
}

int ioctl_temperature(int file_desc, int32_t *msg)
{
 int ret_val;

 ret_val = ioctl(file_desc, IOCTL_TEMPERATURE,msg);
 return 0;
}

int ioctl_humidity(int file_desc, int32_t *msg)
{
 int ret_val;

 ret_val = ioctl(file_desc, IOCTL_HUMIDITY,msg);
 return 0;
}

int main(void)
{
 int ret_val,i;
 int32_t recv_msg;
 time_t ltime;
 struct tm result;
 char time_string[32];
 float temperature,pressure,humidity;

 signal(SIGINT, exitHandler);

 file_desc = open(DEVICE_FILE_NAME,0);
 fptr = fopen("data.log","a");

 if(file_desc<0)
 {
  printf("Device Open Failed for %s\n",DEVICE_FILE_NAME);
  exit(-1);
 }
 printf("Logging started...\n");

 while(1)
 {
  ltime = time(NULL);
  localtime_r(&ltime, &result);
  asctime_r(&result, time_string);
  i=0;
  
  while(time_string[i]!='\0')
  {
   if(time_string[i]=='\n')
    time_string[i]=0;
   i++;
  }

  fprintf(fptr, "%s : ",time_string);
  ioctl_temperature(file_desc,&recv_msg);
  temperature = (float)recv_msg/100.0;
  fprintf(fptr, "Temperature:%f ",temperature);

  ioctl_pressure(file_desc,&recv_msg);
  pressure = (float)recv_msg/100.0;
  fprintf(fptr, "Pressure:%f ",pressure);

  ioctl_humidity(file_desc,&recv_msg);
  humidity = (float)recv_msg/1024.0;
  fprintf(fptr, "Humidity:%f\n", humidity);

  sleep(5);
 }
}
