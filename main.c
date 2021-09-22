#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/ioctl.h>
#include <linux/random.h>

#include "config.h"
 
 
#define SUCCESS 0


static dev_t first; // variable for device number
static struct cdev c_dev; // variable for the character device structure
static struct class *cls; // variable for the device class

/*****************************************************************************
closefile(), openfile(), readfile(), writefile() functions are defined here
these functions will be called for close, open, read and write system calls respectively. 
*****************************************************************************/

static int openfile(struct inode *i, struct file *f)         // to open the driver file
{
	printk(KERN_INFO "open() the file\n");
	return 0;
}

static int closefile(struct inode *i, struct file *f)        // to close the driver file
{
	printk(KERN_INFO "close() the file\n");
	return 0;
}

static ssize_t readfile(struct file *f, char __user *buf, size_t len, loff_t *off)     // to read from the driver file
{
	
	uint16_t val,i=0;
    char *addr;
    get_random_bytes(&val, sizeof(val));                       // for generating random numbers
    addr = (char *)&val;
    while(i<4)
       {
         printk(KERN_INFO "Copied Char:%d\n",*addr);
         put_user(*(addr++),buf++);
         i++;
       }
      put_user('\0',buf++);
	
	printk(KERN_INFO "read() the file\n");
	return 0;
}

static ssize_t writefile(struct file *f, const char __user *buf, size_t len, loff_t *off)     // to write into the driver file
{
	printk(KERN_INFO "write() the file\n");
	return len;
}



long ioctl_dev(struct file *file, unsigned int ioctl_num, unsigned long ioctl_param)     //total 10 IOCTL have to be implemented
{
 int i;

 switch(ioctl_num)
 {
  case IOCTL_COMP1:
   i = readfile(file, (char *)ioctl_param,2,0);
   put_user('\0',(char *)ioctl_param+i);
   break;

  case IOCTL_COMP2:
   i = readfile(file, (char *)ioctl_param,2,0);
   put_user('\0',(char *)ioctl_param+i);
   break;

  case IOCTL_COMP3:
   i = readfile(file, (char *)ioctl_param,2,0);
   put_user('\0',(char *)ioctl_param+i);
   break;

  case IOCTL_ACC1:
   i = readfile(file, (char *)ioctl_param,2,0);
   put_user('\0',(char *)ioctl_param+i);
   break;

  case IOCTL_ACC2:
   i = readfile(file, (char *)ioctl_param,2,0);
   put_user('\0',(char *)ioctl_param+i);
   break;

  case IOCTL_ACC3:
   i = readfile(file, (char *)ioctl_param,2,0);
   put_user('\0',(char *)ioctl_param+i);
   break;

  case IOCTL_GYRO1 :
   i = readfile(file, (char *)ioctl_param,2,0);
   put_user('\0',(char *)ioctl_param+i);
   break;

  case IOCTL_GYRO2 :
   i = readfile(file, (char *)ioctl_param,2,0);
   put_user('\0',(char *)ioctl_param+i);
   break;

  case IOCTL_GYRO3 :
   i = readfile(file, (char *)ioctl_param,2,0);
   put_user('\0',(char *)ioctl_param+i);
   break;

  case IOCTL_PRESS :
   i = readfile(file, (char *)ioctl_param,2,0);
   put_user('\0',(char *)ioctl_param+i);
   break;

 }

return SUCCESS;
}















static struct file_operations fops =
{
  .owner 	= THIS_MODULE,
  .open 	= openfile,
  .release 	= closefile,
  .unlocked_ioctl = ioctl_dev,
  .read 	= readfile,
  .write 	= writefile
};
 
//########## INITIALIZATION FUNCTION ##################

static int __init mychar_init(void) 
{
	printk(KERN_INFO "Hello driver registered");
	
	// STEP 1 : reserve <major, minor>
	if (alloc_chrdev_region(&first, 0, 1, "imu_char") < 0)
	{
		return -1;
	}
	
	// STEP 2 : dynamically create device node in /dev directory
    if ((cls = class_create(THIS_MODULE, "chardrv")) == NULL)
	{
		unregister_chrdev_region(first, 1);
		return -1;
	}
    if (device_create(cls, NULL, first, NULL, "imu_char") == NULL)
	{
		class_destroy(cls);
		unregister_chrdev_region(first, 1);
		return -1;
	}
	
	// STEP 3 : Link fops and cdev to device node
    cdev_init(&c_dev, &fops);
	
    if (cdev_add(&c_dev, first, 1) == -1)
	{
		device_destroy(cls, first);
		class_destroy(cls);
		unregister_chrdev_region(first, 1);
		return -1;
	}
	return 0;
}
 
static void __exit mychar_exit(void) 
{
	cdev_del(&c_dev);
	device_destroy(cls, first);
	class_destroy(cls);
	unregister_chrdev_region(first, 1);
	printk(KERN_INFO "Bye driver unregistered\n\n");
}
 
module_init(mychar_init);
module_exit(mychar_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("RISHI");
MODULE_DESCRIPTION("DDAssignment1");



