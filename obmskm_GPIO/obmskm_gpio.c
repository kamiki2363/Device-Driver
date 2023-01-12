#include <linux/module.h>	//module stuff
#include <linux/kernel.h>	//printk
#include <linux/init.h>		//__init
#include <asm/io.h>
#include <linux/ioport.h>
#include <linux/delay.h>
#include <linux/spinlock.h>

#include "obmskm_gpio.h"

//-------------------------------------------------------------------------------------------------
// Global variables

#define Get_Register 13
#define Set_Register 7
#define Clr_Register 10
#define Bit_High 1
#define Bit_110  6
#define Bit_111  7
#define PERIPHERAL_BASE_ADDRESS 0x3F000000
#define GPIO_BASE_ADDRESS (PERIPHERAL_BASE_ADDRESS + 0x200000)

static unsigned* pBaseRegister;   // Register mapping in kernel space

static uint32_t auPinAllocated[uNoGPIO_Pins];

//-------------------------------------------------------------------------------------------------



//-------------------------------------------------------------------------------------------------

static int __init rpigpio_init(void)
    {
    // Get kernel address
    pBaseRegister = ioremap_nocache(GPIO_BASE_ADDRESS, 180);
    printk("pBaseRegister is %x\n", pBaseRegister);

    // Initialise all pins to unallocated
    for(uint32_t i = 0; i < uNoGPIO_Pins; i++)
        {
        auPinAllocated[i] = FALSE;
        }
    printk("all pin is allocated\n");

    // Test pins by blinking LEDs
    for(uint32_t i = 0; i < uNoGPIO_Pins ; i++)
        {
        const unsigned gpio_pin_number = auValidGPIO_Pins[i];
        obmskm_gpio_request(gpio_pin_number,0);
        obmskm_gpio_direction_output(gpio_pin_number);
        if(obmskm_gpio_direction_output(gpio_pin_number) < 0)
            {
            // Error condition
            printk("Output set fail\n");
            return -ERNOPIN;
            }
        }

    // Blink each LED ten times
    for(uint32_t j = 0; j < 10; j++)
        {
        for(uint32_t i = 0; i < uNoGPIO_Pins ; i++)
            {
            const unsigned gpio_pin_number = auValidGPIO_Pins[i];
            mdelay(100);    // 100 mS delay
            obmskm_gpio_set_value(gpio_pin_number, TRUE);
            }
        for(uint32_t i = 0; i < uNoGPIO_Pins ; i++)
            {
            const unsigned gpio_pin_number = auValidGPIO_Pins[i];
            mdelay(100);    // 100 mS delay
            obmskm_gpio_set_value(gpio_pin_number, FALSE);
            }
        }

    for(uint32_t i = 0; i < uNoGPIO_Pins ; i++)
        {
        const unsigned gpio_pin_number = auValidGPIO_Pins[i];
        obmskm_gpio_free(gpio_pin_number);
        }


    //printk("test finished gpio free,auPinAllocated[i] %d \n",auPinAllocated[i]);


//test pin for checking button press
    /*int gpio_pin_number=2;
    obmskm_gpio_request(gpio_pin_number,0);
    obmskm_gpio_direction_input(gpio_pin_number);
    obmskm_gpio_direction_output(4);
    obmskm_gpio_request(4,0);
    for(uint32_t rep=0 ; rep<1000000; rep++)
        {
        int check = obmskm_gpio_get_value(gpio_pin_number);
        if(!check)
            {

            obmskm_gpio_set_value(4,TRUE);
            printk("the button press\n");
            }
        else
            {
            obmskm_gpio_set_value(4,FALSE);
            }
        obmskm_gpio_free(4);
        obmskm_gpio_free(gpio_pin_number);

        }*/
    }

//-------------------------------------------------------------------------------------------------

static int __exit rpigpio_exit(void)
    {
    iounmap(pBaseRegister);
    printk("clean up module\n");

    return SUCCESS;
    } // __exit rpigpio_exit()

//-------------------------------------------------------------------------------------------------

int obmskm_gpio_is_valid(unsigned gpio_pin_number)
    {
    for(unsigned i = 0; i < uNoGPIO_Pins; i++)
        {
        if(gpio_pin_number == auValidGPIO_Pins[i])
            {
            return SUCCESS;
            }
        }

    return -ERWRPIN;
    } // obmskm_gpio_is_valid()

EXPORT_SYMBOL(obmskm_gpio_is_valid);

//-------------------------------------------------------------------------------------------------

int obmskm_gpio_direction_input(unsigned gpio_pin_number)
/* set as input, returning 0 or negative errno */
    {
    unsigned uMask_Set_Input  = ~(Bit_111<<(((gpio_pin_number)%10)*3));
    //Alt register have 5 of them choose one with devied by 10
    unsigned* puFunctionSelectRegister= pBaseRegister+(gpio_pin_number/10);
    //move 000 to the register with pin 0-9
    unsigned uCurrentRegisterContents_Input = ioread32(puFunctionSelectRegister);
    unsigned uNewRegisterContents_Input = uCurrentRegisterContents_Input & uMask_Set_Input;
    iowrite32(uNewRegisterContents_Input, puFunctionSelectRegister);

    return SUCCESS;
    }//obmskm_gpio_direction_input()

EXPORT_SYMBOL(obmskm_gpio_direction_input);

//-------------------------------------------------------------------------------------------------

int obmskm_gpio_direction_output(unsigned gpio_pin_number)
/* set as output, returning 0 or negative errno */
    {
    unsigned* puFunctionSelectRegister = pBaseRegister + (gpio_pin_number/10);

    unsigned uCurrentRegisterContents = ioread32(puFunctionSelectRegister);
    unsigned uBitShift = (gpio_pin_number % 10) * 3;
    unsigned long uBitMask = (Bit_High << uBitShift);
    unsigned long uNBitMask = ~(Bit_110 << uBitShift);

    unsigned uNewRegisterContents = (uCurrentRegisterContents | uBitMask) & uNBitMask;

    iowrite32(uNewRegisterContents, puFunctionSelectRegister);

    return SUCCESS;
    } // obmskm_gpio_direction_output()

EXPORT_SYMBOL(obmskm_gpio_direction_output);

//-------------------------------------------------------------------------------------------------

int obmskm_gpio_get_value(unsigned gpio_pin_number)
/* GPIO INPUT:  return zero or nonzero */
    {
    unsigned* puGPIO_Get_Register= (pBaseRegister+Get_Register);
    unsigned uInput32 = ioread32(puGPIO_Get_Register);

    unsigned uMask32  = Bit_High << gpio_pin_number;
    return (uInput32 & uMask32);
    }//obmskm_gpio_get_value()
EXPORT_SYMBOL(obmskm_gpio_get_value);

//-------------------------------------------------------------------------------------------------

void obmskm_gpio_set_value(unsigned gpio_pin_number, int value)
/* GPIO OUTPUT */
    {
    uint32_t uMask_OutPut32 = Bit_High << gpio_pin_number;
    printk("uOutput32 is %x\n",uMask_OutPut32);
    //printk("pBaseRegister is %x\n",pBaseRegister);
    //printk("uGPIO_Set_Register is %x\n",uGPIO_Set_Register);

    if(value)
        {
        unsigned* volatile puGPIO_Set_Register= (pBaseRegister + Set_Register);
        volatile unsigned uCurrentRegisterContents_Set= ioread32(puGPIO_Set_Register);
        unsigned uNewRegisterContents_Set = uCurrentRegisterContents_Set | uMask_OutPut32;
        uNewRegisterContents_Set = uNewRegisterContents_Set & uMask_OutPut32;
        iowrite32(uNewRegisterContents_Set, puGPIO_Set_Register);
        printk("the pin %d is set\n",gpio_pin_number);
        }
    else
        {
        unsigned* volatile puGPIO_Clr_Register = (pBaseRegister+Clr_Register);
        volatile unsigned uCurrentRegisterContents_Clr = ioread32(puGPIO_Clr_Register);
        unsigned uNewRegisterContents_Clr = uCurrentRegisterContents_Clr | uMask_OutPut32;
        uNewRegisterContents_Clr = uNewRegisterContents_Clr & uMask_OutPut32;
        iowrite32(uNewRegisterContents_Clr, puGPIO_Clr_Register);
        printk("the pin %d is clr\n",gpio_pin_number);
        }
    return SUCCESS;
    } // obmskm_gpio_set_value()
EXPORT_SYMBOL(obmskm_gpio_set_value);

//-------------------------------------------------------------------------------------------------

int obmskm_gpio_cansleep(unsigned gpio)
// Always returns false
    {
    return -ERFUN;
    }

//-------------------------------------------------------------------------------------------------

int obmskm_gpio_request(unsigned gpio_pin_number, const char *label)
/* request GPIO, returning 0 or negative errno. non-null labels may be useful for diagnostics.*/
    {
    // SPINLOCK HERE!!!
    DEFINE_SPINLOCK(request_lock);
    spin_lock(&request_lock);
    unsigned uMatchingIndex = uNoGPIO_Pins;

    for(uint32_t i = 0; i < uNoGPIO_Pins; i++)
        {
        if(gpio_pin_number == auValidGPIO_Pins[i])
            {
            uMatchingIndex = i;
            break;
            }
        }

    if(uMatchingIndex >= uNoGPIO_Pins) //should worry less than zero?
        {
        // Error condition!
        printk("Wrong Pin number, it should in 2-27\n");
        spin_unlock(&request_lock);
        return -ERWRPIN;
        }

    if(FALSE == auPinAllocated[uMatchingIndex])
        {
        // Allocate pin
        auPinAllocated[uMatchingIndex] = TRUE;
        spin_unlock(&request_lock);
        return SUCCESS;
        }
    else
        {
        // Pin already allocated
        printk(KERN_ALERT "pin %d is allocated",gpio_pin_number);
        for (uint32_t i=0; i<uNoGPIO_Pins; i++)
            {
            if(!auPinAllocated[i])
                {
                printk(KERN_INFO  "pin %d is avaliable\n",auValidGPIO_Pins[i]);
                }}
            spin_unlock(&request_lock);
            return -ERALOPIN;

        }
    }
EXPORT_SYMBOL(obmskm_gpio_request);


//-------------------------------------------------------------------------------------------------

void obmskm_gpio_free(unsigned gpio_pin_number)
/* release previously-claimed GPIO */
    {
    unsigned uMatchingIndex = uNoGPIO_Pins;

    for(unsigned i = 0; i < uNoGPIO_Pins; i++)
        {
        if(gpio_pin_number == auValidGPIO_Pins[i])
            {
            uMatchingIndex = i;
            break;
            }
        }

    if(uMatchingIndex >= uNoGPIO_Pins)
        {
        // Error condition!
        printk("Wrong Pin number, it should in 2-27\n");
        return -ERWRPIN;
        }

    if(TRUE == auPinAllocated[uMatchingIndex])
        {
        // Deallocate pin
        auPinAllocated[uMatchingIndex] = FALSE;
        return SUCCESS;
        }
    else
        {
        // Pin already deallocated
        printk("Pin is not allocated\n");
        return -ERDEALOPIN;
        }
    }
EXPORT_SYMBOL(obmskm_gpio_free);


//-------------------------------------------------------------------------------------------------
//
//int obmskm_gpio_to_irq(unsigned gpio)
//* map GPIO numbers to IRQ numbers */
//    {
//
//    }

//-------------------------------------------------------------------------------------------------

//int obmskm_gpio_to_irq(unsigned gpio)
/* map GPIO numbers to IRQ numbers */
//    {

//   }

//*************************************************************************************************
module_init(rpigpio_init);
module_exit(rpigpio_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Ambrose Chan");
MODULE_DESCRIPTION("GPIO base on iomem");
MODULE_SUPPORTED_DEVICE(RPIGPIO);
