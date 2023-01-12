#include <linux/module.h>	//module stuff
#include <linux/kernel.h>	//printk
#include <linux/init.h>		//__init
#include <linux/fs.h>
#include <linux/cdev.h>
#include <asm/uaccess.h>
#include <asm/io.h>
#include <linux/ioport.h>
#include <linux/delay.h>
#include <linux/spinlock.h>


#include "obmskm_gpio.h"

//-------------------------------------------------------------------------------------------------
// Global variables
#define DEVICE_NAME "rpigpio"

struct gpio_dev
    {
    struct gpio_qset *data;
    int quantum;
    int qset;
    unsigned long size;
    unsigned int access_key;
    struct semaphore sem;
    struct cdev *gpio_cdev;
    };
int majornumber;
int ret;

dev_t dev_num;
//-------------------------------------------------------------------------------------------------
//Extern functions
/*
extern int obmskm_gpio_is_valid(unsigned);
extern int obmskm_gpio_direction_input(unsigned);
extern int obmskm_gpio_direction_output(unsigned);
extern int obmskm_gpio_get_value(unsigned);
extern void obmskm_gpio_set_value(unsigned,int);
extern int obmskm_gpio_request(unsigned);
extern void obmskm_gpio_free(unsigned);
*/
//-------------------------------------------------------------------------------------------------


int gpio_open
//-------------------------------------------------------------------------------------------------

int gpio_release

//-------------------------------------------------------------------------------------------------

ssize_t gpio_read



//-------------------------------------------------------------------------------------------------

ssize_t gpio_write



//-------------------------------------------------------------------------------------------------

struct file_operations gpio_creat_fops=
    {
    .owner = THIS_MODULE,
    .read  = gpio_read,
    .write = gpio_write,
    .open  = gpio_open,
    .release = gpio_release,
    };

//-------------------------------------------------------------------------------------------------


static int gpio_usr_space_init(struct gpio_dev *dev)
    {

    ret = alloc_chrdev_region(&dev_num,0,1,DEVICE_NAME);
    if(ret<0)
        {
        printk(KERN_ALERT DEVICE_NAME":Failed to allocate a major number.\n");
        return ret;
        }
    int major_number=MAJOR(dev_num);
    int devno=MKDEV(major_number,0);
    printk(KERN_INFO "The major number is %d",major_number);
    printk(KERN_INFO "'mknod /dev/%s c %d 0'.\n",DEVICE_NAME,major_number);
    cdev_init(&gpio_cdev->cdev,&gpio_fops);
    //    gpio_cdev=cdev_alloc();

    gpio_cdev->owner = THIS_MODULE;
    gpio_cdev->ops   = &gpio_fops;
    ret=cdev_add(&gpio_cdev->cdev,devno,1);
    if(ret<0)
        {
        printk(KERN_ALERT DEVICE_NAME"unable to add char device to kernel");
        return ret;
        }
    return SUCCESS;
//testing led
//       for(uint32_t i = 0; i < uNoGPIO_Pins ; i++)
//           {
//           const unsigned gpio_pin_number = auValidGPIO_Pins[i];
//           obmskm_gpio_request(gpio_pin_number,0);
//           obmskm_gpio_direction_output(gpio_pin_number);
//           if(obmskm_gpio_direction_output(gpio_pin_number) < 0)
//               {
//               // Error condition
//               printk("Output set fail\n");
//               return -533;
//               }
//           }
//
//       // Blink each LED ten times
//       for(uint32_t j = 0; j < 10; j++)
//           {
//           for(uint32_t i = 0; i < uNoGPIO_Pins ; i++)
//               {
//               const unsigned gpio_pin_number = auValidGPIO_Pins[i];
//               mdelay(100);    // 100 mS delay
//               obmskm_gpio_set_value(gpio_pin_number, TRUE);
//               }
//           for(uint32_t i = 0; i < uNoGPIO_Pins ; i++)
//               {
//               const unsigned gpio_pin_number = auValidGPIO_Pins[i];
//               mdelay(100);    // 100 mS delay
//               obmskm_gpio_set_value(gpio_pin_number, FALSE);
//               }
//           }
//
//       for(uint32_t i = 0; i < uNoGPIO_Pins ; i++)
//           {
//           const unsigned gpio_pin_number = auValidGPIO_Pins[i];
//           obmskm_gpio_free(gpio_pin_number);
//           }

    }

//-------------------------------------------------------------------------------------------------

static int __exit gpio_usr_space_exit(void)
    {
    cdev_del(gpio_cdev);
    unregister_chrdev_region(dev_num,1);
    printk(KERN_INFO "The gpio device is removed");
    return SUCCESS;
    }
//-------------------------------------------------------------------------------------------------

struct file_operations gpio_ctl_fops=
    {
    .owner = THIS_MODULE,
    .read  = gpio_ctl_read,
    .write = gpio_ctl_write,
    .open  = gpio_ctl_open,
    .release = gpio_ctl_release,
    };

//-------------------------------------------------------------------------------------------------


ssize_t gpio_ctl_write(struct file* filp,unsigned int gpio_function,int gpio_pin_number,size_t bufCount)
    {
    int set;
    switch(gpio_function)
        {
        case set_gpio:

            copy_from_user(&set,set,&bufCount);
            obmskm_gpio_set_value(gpio_pin_number,set);
            break;

        case gpio_direction_output:

            obmskm_gpio_direction_output(gpio_pin_number);
            break;

        case gpio_direction_input:

            obmskm_gpio_direction_input(gpio_pin_number);
            break;

        case gpio_request:

            if(obmskm_gpio_request(gpio_pin_number,0)<0)
                {
                printk(KERN_ALERT "Pin  %d is resevered or wrong pin number",gpio_pin_number);
                return -541;
                };
            break;

        case gpio_free:

            obmskm_gpio_free(gpio_pin_number);
//                {
//                printk(KERN_ALERT "Pin %d is not have allocated",gpio_pin_number);
//                return -542;
//                }
            break;

        default :

            printk(KERN_ALERT "unknown command");
            return -540;

        }
    return SUCCESS;
    }


//-------------------------------------------------------------------------------------------------

ssize_t gpio_ctl_read(struct file* filp,unsigned int gpio_function,int gpio_pin_number,size_t bufCount,int gpio_pin_bit)
    {
    switch(gpio_function)
        {
        case get_gpio:

            gpio_pin_bit=obmskm_gpio_get_value(gpio_pin_number);
            copy_to_user(gpio_pin_bit,gpio_pin_bit,bufCount);
            return SUCCESS;

        case gpio_vaild:

            if(obmskm_gpio_is_valid(gpio_pin_number)<0)
                {
                printk("invalid gpio pin number should be between 2-27");
                return -532;
                }
            return SUCCESS;

        default:

            printk(KERN_ALERT "unknown command");
            return 540;

        }
    }

//-------------------------------------------------------------------------------------------------

int gpio_ctl_open(struct inode *inode,struct file *filp)
    {
    printk(KERN_INFO "The gpio device is open");
    return SUCCESS;
    }


//-------------------------------------------------------------------------------------------------

int gpio_ctl_release(struct inode *inode,struct file *filp)
    {
    printk(KERN_INFO "The gpio device is close");
    return SUCCESS;
    }

//-------------------------------------------------------------------------------------------------

static void gpio_setup_ctl_cdev(struct gpio_ctl_dev *gpio_ctl_cdev,int index)
    {
    int err,devno= MKDEV(gpio_ctl_major,gpio_ctl_minor+index);

    cdev_init(&gpio_ctl_cdev->cdev,&gpio_ctl_fops);
    gpio_ctl_cdev->cdev.owner = THIS_MODULE;
    gpio_ctl_cdev->cdev.ops   = &gpio_ctle_fops;
    if(cdev_add(&gpio_ctl_cdev->cdev, devno,1))
        {
        printk(KERN_ALERT "Error %d Adding char device %d fail",err,index);
        }

    }

//-------------------------------------------------------------------------------------------------

int gpio_ctl_trim(struct gpio_ctl_dev *gpio_ctl_cdev)
{
    struct gpio_crte
}

//-------------------------------------------------------------------------------------------------

static int __exit gpio_ctl_exit(void)
    {
    int i;
    dev_t devno=MKDEV(gpio_ctl_major,gpio_ctl_minor);

    if(gpio_ctl_devices)
        {
        for(i=0; i<gpio_ctl_nr_cdevs; i++)
            {
            gpio_ctl_trim(gpio_ctl_devices+i);
            cdev(&gpio_ctl_devices[i].cdev);
            }
        kfree(gpio_ctl_devices);
        }
    }

//-------------------------------------------------------------------------------------------------

static int __init gpio_ctl_init(void)
    {
    int result,i;
    dev_t gpio_ctl_cdev = 0;

    if(gpio_ctl_major)
        {
        dev=MKDEV(gpio_ctl_major,gpio_ctl_minor);
        result=register_chrdev_region(gpio_ctl_cdev,gpio_nr_cdevs,"gpio pin ")
               }
           else
        {
        result = alloc_chrdev_region(&gpio_ctl_cdev,gpio_ctl_minor,gpio_nr_cdevs,"gpio pin");
        spio_ctl_major=MAJOR(gpio_ctl_cdev);
        }
    if(result<0)
        {
        printk(KERN_WARNING "gpio:cannot get major %d\n",gpio_ctl_major);
        return result;
        }

    gpio_ctl_devices =kmalloc(gpio_nr_cdevs * sizeof(struct gpio_ctl_cdev), GFP_KERNEL);
    if(!gpio_ctl_devices)
        {
        result=-ENOMEM;
        goto fail;
        }
    memset(gpio_ctl_device,0,gpio_nr_cdevs * sizeof(struct gpio_ctl_cdev));
    //ctle multi device
    gpio_ctl_devices[i].quantum=gpio_ctl_quantum;
    gpio_ctl_devices[i].qset   = gpio_ctl_qset;
    init_MUTEX(&gpio_ctl_devices[i].sem);
    gpio_ctl_setup_cdev(&gpio_ctl_devices[i],i);

    dev=MKDEV(gpio_ctl_major,gpio_ctl_minor+gpio_ctl_nr_devs);
    dev+=gpio_ctl_p_init(gpio_ctl_cdev);
    dev+=gpio_ctl_access_init(gpio_ctl_cdev);

    return SUCCESS;

fail:
    gpio_ctl_cleanup_module();
    return result;


    }

//-------------------------------------------------------------------------------------------------



//*************************************************************************************************
module_init(gpio_usr_space_init);
module_exit(gpio_usr_space_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Ambrose Chan");
MODULE_DESCRIPTION("GPIO base on iomem");
MODULE_SUPPORTED_DEVICE(RPIGPIO);

