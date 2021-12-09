#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/version.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/i2c.h>
#include <linux/kernel.h>

#include "config.h"

#define DRIVER_NAME "bme280"
#define DRIVER_CLASS "bme280Class"

typedef uint32_t BME280_U32_t;
typedef int32_t BME280_S32_t;

static struct i2c_adapter * bme_i2c_adapter = NULL;
static struct i2c_client * bme280_i2c_client = NULL;

int32_t calib_T1,calib_T2,calib_T3,calib_Tfine;
int32_t calib_P1,calib_P2,calib_P3,calib_P4,calib_P5,calib_P6,calib_P7,calib_P8,calib_P9;;
int32_t calib_H1,calib_H2,calib_H3,calib_H4,calib_H5,calib_H6;

MODULE_AUTHOR("Jeevaraam K");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("BME280 Sensor Kernel Driver");

#define I2C_BUS_USED       1               /* The I2C Bus available on the raspberry */
#define BME280_SENSOR_NAME       "BME280"        /* Device and Driver Name */
#define BME280_SLAVE_ADDRESS    0x77            /* BMP280 I2C address */

static const struct i2c_device_id bme280_dev_id[]={
{ BME280_SENSOR_NAME, 0},
{ }
};

static struct i2c_driver bme280_driver = {
 .driver = {
   .name = BME280_SENSOR_NAME,
   .owner = THIS_MODULE
  }
};

static struct i2c_board_info bme280_i2c_board_info = {
 I2C_BOARD_INFO(BME280_SENSOR_NAME, BME280_SLAVE_ADDRESS)
};


static dev_t bme280_device_number;
static struct class *bme280_class;
static struct cdev bme280_device;

static uint32_t read_humidity(void)
{
    int32_t var1;
    int32_t var2;
    int32_t var3;
    int32_t var4;
    int32_t var5;
    uint32_t humidity;
    uint32_t humidity_max = 102400;

    uint8_t lsb,msb;
    uint32_t current_humidity;

    lsb = i2c_smbus_read_byte_data(bme280_i2c_client, 0xFE);
    msb = i2c_smbus_read_byte_data(bme280_i2c_client, 0xFD);

    current_humidity = ((uint32_t)msb<<8) | ((uint32_t)lsb);

    var1 = calib_Tfine - ((int32_t)76800);
    var2 = (int32_t)(current_humidity * 16384);
    var3 = (int32_t)(((int32_t)calib_H4) * 1048576);
    var4 = ((int32_t)calib_H5) * var1;
    var5 = (((var2 - var3) - var4) + (int32_t)16384) / 32768;
    var2 = (var1 * ((int32_t)calib_H6)) / 1024;
    var3 = (var1 * ((int32_t)calib_H3)) / 2048;
    var4 = ((var2 * (var3 + (int32_t)32768)) / 1024) + (int32_t)2097152;
    var2 = ((var4 * ((int32_t)calib_H2)) + 8192) / 16384;
    var3 = var5 * var2;
    var4 = ((var3 / 32768) * (var3 / 32768)) / 128;
    var5 = var3 - ((var4 * ((int32_t)calib_H1)) / 16);
    var5 = (var5 < 0 ? 0 : var5);
    var5 = (var5 > 419430400 ? 419430400 : var5);
    humidity = (uint32_t)(var5 / 4096);

    if (humidity > humidity_max)
    {
        humidity = humidity_max;
    }

    return humidity;
}
static uint32_t read_pressure(void)
{
 /*int32_t var1;
 int32_t var2;
 int32_t var3;
 int32_t var4;
 uint32_t var5;*/
 uint32_t pressure;
 uint32_t current_pressure;
 uint8_t lsb,msb,xlsb;
 uint32_t pressure_min = 30000;
 uint32_t pressure_max = 110000;

 BME280_S32_t var1, var2;
 BME280_U32_t p;
 var1 = (((BME280_S32_t)calib_Tfine)>>1) - (BME280_S32_t)64000;
 var2 = (((var1>>2) * (var1>>2)) >> 11 ) * ((BME280_S32_t)calib_P6);
 var2 = var2 + ((var1*((BME280_S32_t)calib_P5))<<1);
 var2 = (var2>>2)+(((BME280_S32_t)calib_P4)<<16);
 var1 = (((calib_P3 * (((var1>>2) * (var1>>2)) >> 13 )) >> 3) + ((((BME280_S32_t)calib_P2) * var1)>>1))>>18;
 var1 =((((32768+var1))*((BME280_S32_t)calib_P1))>>15);
 if (var1 == 0)
 {
  return 0; // avoid exception caused by division by zero
 }
 p = (((BME280_U32_t)(((BME280_S32_t)1048576)-current_pressure)-(var2>>12)))*3125;
 if (p < 0x80000000) 
 {
  p = (p << 1) / ((BME280_U32_t)var1);
 } 
 else
 {
  p = (p / (BME280_U32_t)var1) * 2;
 }
 var1 = (((BME280_S32_t)calib_P9) * ((BME280_S32_t)(((p>>3) * (p>>3))>>13)))>>12;
 var2 = (((BME280_S32_t)(p>>2)) * ((BME280_S32_t)calib_P8))>>13;
 p = (BME280_U32_t)((BME280_S32_t)p + ((var1 + var2 + calib_P7) >> 4));
 return p;

 /*int64_t var1;
 int64_t var2;
 int64_t var3;
 int64_t var4;
 uint32_t pressure;*/

 lsb = i2c_smbus_read_byte_data(bme280_i2c_client,0xF8);
 msb = i2c_smbus_read_byte_data(bme280_i2c_client,0xF7);
 xlsb = i2c_smbus_read_byte_data(bme280_i2c_client, 0xF9);
 current_pressure = (((uint32_t)msb)<<12) | ((uint32_t)lsb<<4) | ((uint32_t)xlsb>>4);
// current_pressure = ( (current_pressure<<4) | ( (uint32_t) (xlsb&0xF0) >>4 ) );
 printk("Pressure:%x\n",current_pressure);

 /*var1 = (((int32_t)calib_Tfine) / 2) - (int32_t)64000;
 var2 = (((var1 / 4) * (var1 / 4)) / 2048) * ((int32_t)calib_P6);
 var2 = var2 + ((var1 * ((int32_t)calib_P5)) * 2);
 var2 = (var2 / 4) + (((int32_t)calib_P4) * 65536);
 var3 = (calib_P3 * (((var1 / 4) * (var1 / 4)) / 8192)) / 8;
 var4 = (((int32_t)calib_P2) * var1) / 2;
 var1 = (var3 + var4) / 262144;
 var1 = (((32768 + var1)) * ((int32_t)calib_P1)) / 32768;

 if (var1)
    {
        var5 = (uint32_t)((uint32_t)1048576) - current_pressure;
        pressure = ((uint32_t)(var5 - (uint32_t)(var2 / 4096))) * 3125;

        if (pressure < 0x80000000)
        {
            pressure = (pressure << 1) / ((uint32_t)var1);
        }
        else
        {
            pressure = (pressure / (uint32_t)var1) * 2;
        }

        var1 = (((int32_t)calib_P9) * ((int32_t)(((pressure / 8) * (pressure / 8)) / 8192))) / 4096;
        var2 = (((int32_t)(pressure / 4)) * ((int32_t)calib_P8)) / 8192;
        pressure = (uint32_t)((int32_t)pressure + ((var1 + var2 + calib_P7) / 16));

    }
    else
    {
        pressure = pressure_min;
    }
*/
    /*var1 = ((int64_t)calib_Tfine) - 128000;
    var2 = var1 * var1 * (int64_t)calib_P6;
    var2 = var2 + ((var1 * (int64_t)calib_P5) * 131072);
    var2 = var2 + (((int64_t)calib_P4) * 34359738368);
    var1 = ((var1 * var1 * (int64_t)calib_P3) / 256) + ((var1 * ((int64_t)calib_P2) * 4096));
    var3 = ((int64_t)1) * 140737488355328;
    var1 = (var3 + var1) * ((int64_t)calib_P1) / 8589934592;

    
    if (var1 != 0)
    {
        var4 = 1048576 - current_pressure;
        var4 = (((var4 * S64_C(2147483648)) - var2) * 3125) / var1;
        var1 = (((int64_t)calib_P9) * (var4 / 8192) * (var4 / 8192)) / 33554432;
        var2 = (((int64_t)calib_P8) * var4) / 524288;
        var4 = ((var4 + var1 + var2) / 256) + (((int64_t)calib_P7) * 16);
        pressure = (uint32_t)(((var4 / 2) * 100) / 128);

        if (pressure < pressure_min)
        {
            pressure = pressure_min;
        }
        else if (pressure > pressure_max)
        {
            pressure = pressure_max;
        }
    }
    else
    {
        pressure = pressure_min;
    }
*/
 //   return pressure;
}

static int32_t read_temperature(void)
{
 int32_t var1,var2;
 uint8_t aTemp,bTemp,cTemp;
 int32_t temperature;
 int32_t finalTemp;

 aTemp = i2c_smbus_read_byte_data(bme280_i2c_client, 0xFA);
 bTemp = i2c_smbus_read_byte_data(bme280_i2c_client, 0xFB);
 cTemp = i2c_smbus_read_byte_data(bme280_i2c_client, 0xFC);

 temperature = (((int32_t)aTemp) << 12) | (((int32_t)bTemp) << 4) | (((int32_t)cTemp) >> 4); 
 var1 = (int32_t)((temperature / 8) - ((int32_t)calib_T1 * 2));
 var1 = (var1 * ((int32_t)calib_T2)) / 2048;
 var2 = (int32_t)((temperature / 16) - ((int32_t)calib_T1));
 var2 = (((var2 * var2) / 4096) * ((int32_t)calib_T3)) / 16384;
 calib_Tfine = var1 + var2;
 finalTemp = ((var1 + var2) * 5 + 128) / 256;

 printk("Temp:%d\n",finalTemp); 
 printk("Tfine:%d\n",calib_Tfine);
 return finalTemp;
}

static ssize_t bme280_driver_read(struct file *File, char __user *user_buffer, size_t count, loff_t *offs)
{
 int to_copy, not_copied, delta;
 char out_string[30];
 int32_t temperature;
 int32_t pressure;
 int32_t humidity;

 to_copy = min(sizeof(out_string), count);

 temperature = read_temperature();
 pressure = read_pressure();
 humidity = read_humidity();

 snprintf(out_string, sizeof(out_string), "T:%d,P:%d,H:%d\n",temperature,pressure,humidity);
 not_copied = copy_to_user(user_buffer, out_string, to_copy);

 delta = to_copy - not_copied;

 return delta;
}

long ioctl_dev(struct file *file, unsigned int ioctl_num, unsigned long ioctl_param)
{
 
 switch(ioctl_num)
 {
  case IOCTL_TEMPERATURE:
  put_user(read_temperature(),(int32_t*)ioctl_param);
  break;
 
  case IOCTL_PRESSURE:
  put_user(read_pressure(),(int32_t*)ioctl_param);
  break;

  case IOCTL_HUMIDITY:
  put_user(read_humidity(),(int32_t*)ioctl_param);
  break;
 }
return 0;
}

static int bme280_driver_open(struct inode *deviceFile, struct file *instance)
{
 printk("Driver Open\n");
 return 0;
}

static int bme280_driver_close(struct inode *deviceFile, struct file *instance) 
{
 printk("Driver Close\n");
 return 0;
}

static struct file_operations fops  = {
 .owner = THIS_MODULE,
 .open = bme280_driver_open,
 .release = bme280_driver_close,
 .unlocked_ioctl = ioctl_dev,
 .read = bme280_driver_read,
};

static int __init bme280Init(void)
{
 int ret =-1;
 int32_t tempH4a,tempH4b,tempH5a;
 u8 id;
 printk("Driver Init\n");

 if ( alloc_chrdev_region(&bme280_device_number, 0, 1, DRIVER_NAME) < 0) 
 {
  printk("Device Nr. could not be allocated!\n");
 }
 
 printk("Driver - Device Nr %d was registered\n", bme280_device_number);

        /* Create Device Class */
 if ((bme280_class = class_create(THIS_MODULE, DRIVER_CLASS)) == NULL) 
 {
  printk("Device Class can not be created!\n");
  goto ClassError;
 }

        /* Create Device file */
 if (device_create(bme280_class, NULL, bme280_device_number, NULL, DRIVER_NAME) == NULL) 
 {
  printk("Can not create device file!\n");
  goto FileError;
 }

        /* Initialize Device file */
 cdev_init(&bme280_device, &fops);

        /* register device to kernel */
 if (cdev_add(&bme280_device, bme280_device_number, 1) == -1) 
 {
  printk("Registering of device to kernel failed!\n");
  goto KernelError;
 }

 bme_i2c_adapter = i2c_get_adapter(I2C_BUS_USED);

 if(bme_i2c_adapter != NULL) 
 {
  bme280_i2c_client = i2c_new_client_device(bme_i2c_adapter, &bme280_i2c_board_info);
 
  if(bme280_i2c_client != NULL) 
  {
   if(i2c_add_driver(&bme280_driver) != -1) 
   {
    ret = 0;
   }
   else
    printk("Can't add driver...\n");
  }
  i2c_put_adapter(bme_i2c_adapter);
 }

 printk("BME280 Driver Init\n");

 id = i2c_smbus_read_byte_data(bme280_i2c_client, 0xD0);
 printk("ID: 0x%x\n",id);

 calib_T1 = i2c_smbus_read_word_data(bme280_i2c_client,0x88);
 calib_T2 = i2c_smbus_read_word_data(bme280_i2c_client,0x8A);
 calib_T3 = i2c_smbus_read_word_data(bme280_i2c_client,0x8C);
 calib_P1 = i2c_smbus_read_word_data(bme280_i2c_client,0x8E);
 calib_P2 = i2c_smbus_read_word_data(bme280_i2c_client,0x90);
 calib_P3 = i2c_smbus_read_word_data(bme280_i2c_client,0x92);
 calib_P4 = i2c_smbus_read_word_data(bme280_i2c_client,0x94);
 calib_P5 = i2c_smbus_read_word_data(bme280_i2c_client,0x96);
 calib_P6 = i2c_smbus_read_word_data(bme280_i2c_client,0x98);
 calib_P7 = i2c_smbus_read_word_data(bme280_i2c_client,0x9A);
 calib_P8 = i2c_smbus_read_word_data(bme280_i2c_client,0x9C);
 calib_P9 = i2c_smbus_read_word_data(bme280_i2c_client,0x9E);
 calib_H1 = i2c_smbus_read_byte_data(bme280_i2c_client,0xA1);
 calib_H2 = i2c_smbus_read_word_data(bme280_i2c_client,0xE1);
 calib_H3 = i2c_smbus_read_byte_data(bme280_i2c_client,0xE3);
 tempH4a = i2c_smbus_read_byte_data(bme280_i2c_client,0xE4);
 tempH4b = i2c_smbus_read_byte_data(bme280_i2c_client,0xE5);
 tempH5a = i2c_smbus_read_byte_data(bme280_i2c_client,0xE6);
 calib_H6 = i2c_smbus_read_byte_data(bme280_i2c_client,0xE7);
 calib_H4 = (tempH4a<<4) | (tempH4b&0x0F);
 calib_H5 = (tempH5a<<4) | ((tempH4b&0xF0)>>4);
	
 if(calib_T2 > 32767)
 {
  calib_T2 -= 65536;
 }
 
 if(calib_T3 > 32767)
 {
  calib_T3 -= 65536;
 }

 printk("BME280 Calib Values\n");
 printk("T1:%d\nT2:%d\nT3:%d\nP1:%d\nP2:%d\nP3:%d\nP4:%d\nP5:%d\nP6:%d\nP7:%d\nP8:%d\nP9:%d\n",calib_T1,calib_T2,calib_T3,calib_P1,calib_P2,calib_P3,calib_P4,calib_P5,calib_P6,calib_P7,calib_P8,calib_P9);

 i2c_smbus_write_byte_data(bme280_i2c_client, 0xf5, 5<<5);
 i2c_smbus_write_byte_data(bme280_i2c_client, 0xf4, ((5<<5) | (5<<2) | (3<<0)));
 return ret;

KernelError:
        device_destroy(bme280_class, bme280_device_number);
FileError:
        class_destroy(bme280_class);
ClassError:
        unregister_chrdev(bme280_device_number, DRIVER_NAME);
        return (-1);
}

static void __exit bme280Exit(void) 
{
 printk("bme280_deviceDriver - Goodbye, Kernel!\n");
 i2c_unregister_device(bme280_i2c_client);
 i2c_del_driver(&bme280_driver);
 cdev_del(&bme280_device);
 device_destroy(bme280_class, bme280_device_number);
 class_destroy(bme280_class);
 unregister_chrdev_region(bme280_device_number, 1);
}

module_init(bme280Init);
module_exit(bme280Exit);

