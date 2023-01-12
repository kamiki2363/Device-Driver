#include <linux/module.h>	// module
#include <linux/kernel.h>	// printk
#include <linux/init.h>		//init
#include <linux/delay.h>	//delay function
#include <linux/spinlock.h> //spinlock
#include <asm-generic/errno.h> //error number
#include <linux/fs.h>		        /* file_operations */
#include <linux/uaccess.h>	        /* copy_(to,from)_user */
#include <linux/cdev.h>		        /* cdev utilities */
#include <linux/device.h>	        /* class_create */


#include "../obmsk_GPIO/obmsk_gpio.h"
#include "Therm_sensor_rpi2b_defs.h"
//-------------------------------------------------------------------------------------------------


#define DEVICE_NAME "DS18B20_sensor"
#define SUCCESS 0
#define array_size 8

static dev_t g_DeviceTuple;
static struct cdev* g_pCharDevice;
static struct class* g_pDeviceClass;

//-------------------------------------------------------------------------------------------------

static uint8_t CRC_check(uint32_t uData_string_1,uint32_t uData_string_2,bool ROM)
	{
	bool bCRC[array_size]= {0};
	int nNumber_size;
	if (ROM)
		{
		nNumber_size=24;
		}
	else
		{
		nNumber_size=32;
		}
	for (uint32_t i=0; i<32; i++)
		{
		bool bCRCTemp=(bCRC[0] ^ (uData_string_1&0x01));
		bCRC[0]=bCRC[1];
		bCRC[1]=bCRC[2];
		bCRC[2]=bCRC[3]^bCRCTemp;
		bCRC[3]=bCRC[4]^bCRCTemp;
		bCRC[4]=bCRC[5];
		bCRC[5]=bCRC[6];
		bCRC[6]=bCRC[7];
		bCRC[7]=bCRCTemp;
		uData_string_1>>=1;

		}
	for (uint32_t i=0; i<nNumber_size; i++)
		{
		bool bCRCTemp=(bCRC[0] ^ (uData_string_2&0x01));
		bCRC[0]=bCRC[1];
		bCRC[1]=bCRC[2];
		bCRC[2]=bCRC[3]^bCRCTemp;
		bCRC[3]=bCRC[4]^bCRCTemp;
		bCRC[4]=bCRC[5];
		bCRC[5]=bCRC[6];
		bCRC[6]=bCRC[7];
		bCRC[7]=bCRCTemp;
		uData_string_2>>=1;

		}
	uint8_t uCRC_Final=0;
	for(uint32_t i=0; i<8; i++)
		{
		uint8_t uCRCBit = bCRC[i];
		uCRCBit<<=i;
		uCRC_Final|=uCRCBit;
		}
	return uCRC_Final;
	}

//-------------------------------------------------------------------------------------------------

int reset_DS18B20_sensor(unsigned uPin)
	{
	DEFINE_SPINLOCK(request_lock);
	spin_lock(&request_lock);

	// Apply reset pulse
	obmsk_gpio_direction_output(uPin);
	obmsk_gpio_set_value(uPin, high);
	udelay(10);
	obmsk_gpio_set_value(uPin, low);
	udelay(480);
	obmsk_gpio_set_value(uPin, high);

	udelay(60); // Wait for device to respond

	uint32_t uTotalDelay = 0;
	uint32_t uSamplingTime = 10;    // Loop sampling time
	int32_t nPinValue = obmsk_gpio_get_value(uPin);
	while(nPinValue & (uTotalDelay < 60))
		{
		udelay(uSamplingTime);
		uTotalDelay += uSamplingTime;
		nPinValue = obmsk_gpio_get_value(uPin);
		}

	if(nPinValue | (uTotalDelay > 60))
		{
		// No presence pulse detected
		return -1;
		}

	spin_unlock(&request_lock);

	udelay(480 - 60 - uTotalDelay);

	return SUCCESS;

	} // reset_DS18B20_sensor()

//-------------------------------------------------------------------------------------------------

uint32_t Read_8(unsigned uPin)
	{
	int nValue =0;
	uint8_t uData=0;
	uint8_t suData=0;
	for (uint32_t i=0; i<8; i++)
		{
		READ_bit(uPin)
		suData=nValue;
		if(suData & 0x01)
			{
			suData<<=i;
			uData |= suData;
			}
		}
	return(uData);
	}

//-------------------------------------------------------------------------------------------------

uint32_t Read_32(unsigned uPin)
	{
	int nValue =0;
	uint32_t uData=0;

	for (uint32_t i=0; i<32; i++)
		{
		READ_bit(uPin)
		uint32_t suData=nValue;

		if(suData & 0x01)
			{
			suData<<=i;
			uData |= suData;
			}
		}
	return(uData);
	}//Read_32()

//-------------------------------------------------------------------------------------------------


void Write_Command(uint8_t suCommand, uint32_t uPin)
	{

	for(uint32_t i = 0; i < 8; i++)
		{
		uint8_t suData=suCommand;

		if((suData >> i) & high)
			{
			WRITE_1(uPin)
			}
		else
			{
			WRITE_0(uPin)
			}
		}
	return;
	}//Write_Command()

//-------------------------------------------------------------------------------------------------


static uint32_t DS18B20_read(struct file *pFile, uint16_t __user* pchBuffer, size_t BufferLength, loff_t* pPosition)
	{
	uint16_t uFinal_result=0;
	const uint32_t uPin = iminor(pFile->f_path.dentry->d_inode);

	reset_DS18B20_sensor(uPin);
	Write_Command(Read_Rom,uPin);

	uint32_t uROM_Number_1=Read_32(uPin);
	uint32_t uROM_Number_2=Read_32(uPin);

	Write_Command(Convert_T,uPin);

	reset_DS18B20_sensor(uPin);

	Write_Command(Skip_Rom,uPin);
	Write_Command(Read_Scratchpad,uPin);

	uint32_t uScratchpad_Number_1=Read_32(uPin);
	uint32_t uScratchpad_Number_2=Read_32(uPin);
	uint8_t	 uScratchpad_CRC=Read_8(uPin);
	uFinal_result |= uScratchpad_Number_1;
	uint8_t CRC_Ans_ROM=CRC_check(uROM_Number_1,uROM_Number_2,1);
	uint8_t CRC_Ans_Scratchpad=CRC_check(uScratchpad_Number_1,uScratchpad_Number_2,0);

	printk(KERN_INFO "CRC_Ans_ROM is %x\n",CRC_Ans_ROM);
	printk(KERN_INFO "CRC_Ans_Scratchpad is %x\n",CRC_Ans_Scratchpad);
	printk(KERN_INFO "uROM_Number is %x %x\n",uROM_Number_2,uROM_Number_1);
	printk(KERN_INFO "uScratchpad_Number is %x %x\n",uScratchpad_Number_2,uScratchpad_Number_1);
	printk(KERN_INFO "uScratchpad_CRC is %x\n",uScratchpad_CRC);
	printk(KERN_INFO "uFinal_result is %x\n",uFinal_result);
	put_user(uFinal_result, pchBuffer);
	return(BufferLength);
	}//DS18B20_read()

//-------------------------------------------------------------------------------------------------

static int32_t DS18B20_open(struct inode *inode, struct file *file)
	{
	const uint32_t uPin = iminor(inode);
	if(obmsk_gpio_request(uPin, NULL) < 0)
		{
		return -EBUSY;  // Error - pin already in use /* Device or resource busy */
		}

	if(((file->f_flags) & O_ACCMODE) == O_RDONLY)
		{
		return SUCCESS;
		}

	// Should never reach here!
	pr_info(" OPENING device failed\n");

	return -EINVAL; /* Invalid argument */
	}//DS18B20_open ()

//-------------------------------------------------------------------------------------------------

static int32_t DS18B20_release(struct inode *inode, struct file *file)
	{
	const uint32_t uPin = iminor(inode);
	pr_info(" CLOSING device: %s,%d:\n\n", DEVICE_NAME,uPin);
	obmsk_gpio_free(uPin);
	return SUCCESS;
	}//DS18B20_release()

//-------------------------------------------------------------------------------------------------

static const struct file_operations DS18B20_fops =
	{
	.owner = THIS_MODULE,
	.read = DS18B20_read,
	.open = DS18B20_open,
	.release = DS18B20_release,
	};

//-------------------------------------------------------------------------------------------------

static int32_t __init DS18B20_init(void)
	{
	// Dynamically allocate major number
	alloc_chrdev_region(&g_DeviceTuple,  FIRST_MINOR_NUMBER, NO_GPIO_PINS, DEVICE_NAME);
	int32_t nMajorNumber = MAJOR(g_DeviceTuple);

	g_pCharDevice = cdev_alloc();
	cdev_init(g_pCharDevice, &DS18B20_fops);
	g_pCharDevice->owner = THIS_MODULE;
	cdev_add(g_pCharDevice, g_DeviceTuple, NO_GPIO_PINS);

	g_pDeviceClass = class_create(THIS_MODULE, DEVICE_NAME);
	for (uint32_t i = 0; i < NO_GPIO_PINS; i++)
		{
		const uint32_t uPinNumber = auValidGPIO_Pins[i];
		dev_t TempDeviceTuple = MKDEV(nMajorNumber, uPinNumber);
		device_create(g_pDeviceClass, NULL, TempDeviceTuple, NULL, "DS18B20_pin%u",uPinNumber);
		}
	return SUCCESS;
	}//DS18B20_init()

//-------------------------------------------------------------------------------------------------

static void __exit DS18B20_exit(void)
	{
	const int32_t nMajorNumber = MAJOR(g_DeviceTuple);
	for(uint32_t i = 0;  i < NO_GPIO_PINS; i++)
		{
		const uint32_t uPinNumber = auValidGPIO_Pins[i];
		dev_t TempDeviceTuple = MKDEV(nMajorNumber, uPinNumber);
		device_destroy(g_pDeviceClass, TempDeviceTuple);
		}
	class_destroy(g_pDeviceClass);
	cdev_del(g_pCharDevice);
	unregister_chrdev_region(g_DeviceTuple, NO_GPIO_PINS);

	pr_info("\ndevice unregistered successfully\n");

	return;
	}//DS18B20_exit()




//*************************************************************************************************

module_init(DS18B20_init);
module_exit(DS18B20_exit);

MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("Ambrose Chan");
MODULE_DESCRIPTION("Temperature_sensor_Rpi_gpio");
MODULE_SUPPORTED_DEVICE(OBMSK_GPIO);

//*************************************************************************************************




