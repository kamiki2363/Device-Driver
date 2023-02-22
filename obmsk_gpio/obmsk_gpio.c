#include <linux/module.h>	// module stuff
#include <linux/kernel.h>	// printk
#include <linux/init.h>		// __init
#include <asm/io.h>
#include <linux/ioport.h>
#include <linux/delay.h> // debug mdelay
#include <linux/spinlock.h> // spinlock
#include <asm-generic/errno-base.h> // error numbers

#include "obmsk_gpio.h"

//-------------------------------------------------------------------------------------------------
// Global variables

#define OBMSK_SUCCESS 0
#define OBMSK_ERROR (-1)

static uint32_t* g_pBaseRegister;   // Register mapping in kernel space

static uint32_t g_auPinAllocated[NO_GPIO_PINS];

//-------------------------------------------------------------------------------------------------

static int32_t __init gpio_init(void)
    {
    // Get kernel address
    g_pBaseRegister = ioremap_nocache(GPIO_BASE_ADDRESS, ADDRESS_SPAN);

    // Initialise all pins to unallocated
    for(uint32_t i = 0; i < NO_GPIO_PINS; i++)
        {
        g_auPinAllocated[i] = OBMSK_FALSE;
        }
    printk(KERN_INFO "all GPIO pins allocated\n");

#if OBMSK_GPIO_STROBE_ALL_PINS_TEST

    // Test pins by strobing all GPIO pins
    for(uint32_t i = 0; i < NO_GPIO_PINS ; i++)
        {
        const uint32_t gpio_pin_number = auValidGPIO_Pins[i];
        obmsk_gpio_request(gpio_pin_number, NULL);
        obmsk_gpio_direction_output(gpio_pin_number);
        if(obmsk_gpio_direction_output(gpio_pin_number) < 0)
            {
            // Error condition
            printk("Output set fail\n");
            return -EINVAL;
            }
        }

    // Strobe each pin ten times with 100mS delays
    const uint32_t uDelayPeriod = 100;	// 100 mS
    for(uint32_t j = 0; j < 10; j++)
        {
        for(uint32_t i = 0; i < NO_GPIO_PINS ; i++)
            {
            const uint32_t gpio_pin_number = auValidGPIO_Pins[i];
            mdelay(uDelayPeriod);
            obmsk_gpio_set_value(gpio_pin_number, OBMSK_TRUE);
            }
        for(uint32_t i = 0; i < NO_GPIO_PINS ; i++)
            {
            const uint32_t gpio_pin_number = auValidGPIO_Pins[i];
            mdelay(uDelayPeriod);
            obmsk_gpio_set_value(gpio_pin_number, OBMSK_FALSE);
            }
        }

    // Free all pins
    for(uint32_t i = 0; i < NO_GPIO_PINS ; i++)
        {
        const uint32_t gpio_pin_number = auValidGPIO_Pins[i];
        obmsk_gpio_free(gpio_pin_number);
        }

#endif // OBMSK_GPIO_STROBE_ALL_PINS_TEST


#if OBMSK_GPIO_CHECK_BUTTON_PRESS_TEST

    // Test pin for checking button press
    const uint32_t uInputPinNumber = 2;
    const uint32_t uOutputPinNumber = 4;

    obmsk_gpio_request(uInputPinNumber, NULL);
    obmsk_gpio_direction_input(uInputPinNumber);

    obmsk_gpio_direction_output(uOutputPinNumber);
    obmsk_gpio_request(uOutputPinNumber, NULL);

    uint32_t uNoPresses = 0;
    while(uNoPresses < 10)
        {
        int32_t nCheck = obmsk_gpio_get_value(uInputPinNumber);
        if(!nCheck)
            {
            obmsk_gpio_set_value(uOutputPinNumber, OBMSK_TRUE);
            uNoPresses++;
            }
        else
            {
            obmsk_gpio_set_value(uOutputPinNumber, OBMSK_FALSE);
            }

        mdelay(100);
        }

    obmsk_gpio_set_value(uOutputPinNumber, OBMSK_FALSE);

    obmsk_gpio_free(uInputPinNumber);
    obmsk_gpio_free(uOutputPinNumber);

#endif // OBMSK_GPIO_CHECK_BUTTON_PRESS_TEST


#if OBMSK_SPEED_TEST

    const uint32_t uTestPinNumber = 4;
    obmsk_gpio_request(uTestPinNumber, NULL);
    if(obmsk_gpio_direction_output(uTestPinNumber) < 0)
        {
        return -EINVAL;
        }

    for(uint32_t i = 0; i < 100000; i++)
        {
        obmsk_gpio_set_value(uTestPinNumber, OBMSK_TRUE);
        obmsk_gpio_set_value(uTestPinNumber, OBMSK_FALSE);
        }

    obmsk_gpio_free(uTestPinNumber);

#endif // OBMSK_SPEED_TEST


    return OBMSK_SUCCESS;
    } // __init gpio_init()

//-------------------------------------------------------------------------------------------------

static void __exit gpio_exit(void)
    {
    iounmap(g_pBaseRegister);
    printk("clean up module\n");

    return;
    } // __exit gpio_exit()

//-------------------------------------------------------------------------------------------------

int32_t obmsk_gpio_is_valid(uint32_t gpio_pin_number)
    {
    for(uint32_t i = 0; i < NO_GPIO_PINS; i++)
        {
        if(gpio_pin_number == auValidGPIO_Pins[i])
            {
            return OBMSK_SUCCESS;
            }
        }

    return -ENODEV; // No such device
    } // obmsk_gpio_is_valid()

EXPORT_SYMBOL(obmsk_gpio_is_valid);

//-------------------------------------------------------------------------------------------------

int32_t obmsk_gpio_direction_input(uint32_t gpio_pin_number)
// Set as input, returning 0 or negative errno
    {
    SET_PIN_TO_INPUT(gpio_pin_number)

    return OBMSK_SUCCESS;
    }//obmsk_gpio_direction_input()

EXPORT_SYMBOL(obmsk_gpio_direction_input);

//-------------------------------------------------------------------------------------------------

int32_t obmsk_gpio_direction_output(uint32_t gpio_pin_number)
// Set as output, returning 0 or negative errno
    {
    SET_PIN_TO_OUTPUT(gpio_pin_number)

    return OBMSK_SUCCESS;
    } // obmsk_gpio_direction_output()

EXPORT_SYMBOL(obmsk_gpio_direction_output);

//-------------------------------------------------------------------------------------------------

int32_t obmsk_gpio_get_value(uint32_t gpio_pin_number)
// GPIO INPUT:  return zero or nonzero
    {
    int32_t nResult;
    GET_PIN_VALUE(gpio_pin_number)

    return nResult;
    } //obmsk_gpio_get_value()

EXPORT_SYMBOL(obmsk_gpio_get_value);

//-------------------------------------------------------------------------------------------------

void obmsk_gpio_set_value(uint32_t gpio_pin_number, int32_t value)
// GPIO OUTPUT
    {
    SET_PIN_VALUE(gpio_pin_number, value)

    return;
    } // obmsk_gpio_set_value()

EXPORT_SYMBOL(obmsk_gpio_set_value);

//-------------------------------------------------------------------------------------------------

int32_t obmsk_gpio_cansleep(uint32_t gpio)
// Always returns false
    {
    return OBMSK_ERROR;
    } // obmsk_gpio_cansleep()

//-------------------------------------------------------------------------------------------------

int32_t obmsk_gpio_request(uint32_t gpio_pin_number, const char* label)
// Request GPIO, returning 0 or negative errno. non-null labels may be useful for diagnostics
    {
    DEFINE_SPINLOCK(request_lock);
    spin_lock(&request_lock);

    uint32_t uMatchingIndex = NO_GPIO_PINS;
    for(uint32_t i = 0; i < NO_GPIO_PINS; i++)
        {
        if(gpio_pin_number == auValidGPIO_Pins[i])
            {
            uMatchingIndex = i;
            break;
            }
        }

    if(uMatchingIndex >= NO_GPIO_PINS)
        {
        // Error condition!
        printk("Wrong Pin number, it should in 2-27\n");
        spin_unlock(&request_lock);
        return -ENODEV; // No such device
        }

    if(OBMSK_FALSE == g_auPinAllocated[uMatchingIndex])
        {
        // Allocate pin
        g_auPinAllocated[uMatchingIndex] = OBMSK_TRUE;
        spin_unlock(&request_lock);
        return OBMSK_SUCCESS;
        }
    else
        {
        // Pin already allocated
        for(uint32_t i = 0; i < NO_GPIO_PINS; i++)
            {
            if(!g_auPinAllocated[i])
                {
                printk(KERN_INFO  "in %s: pin %d is available\n", __FUNCTION__, auValidGPIO_Pins[i]);
                }
            }
        spin_unlock(&request_lock);

        return -EBUSY; // Device or resource busy
        }
    } // obmsk_gpio_request()

EXPORT_SYMBOL(obmsk_gpio_request);

//-------------------------------------------------------------------------------------------------

void obmsk_gpio_free(uint32_t gpio_pin_number)
// Release previously-claimed GPIO
    {
    uint32_t uMatchingIndex = NO_GPIO_PINS;

    for(uint32_t i = 0; i < NO_GPIO_PINS; i++)
        {
        if(gpio_pin_number == auValidGPIO_Pins[i])
            {
            uMatchingIndex = i;
            break;
            }
        }

    if(uMatchingIndex >= NO_GPIO_PINS)
        {
        // Error condition!
        printk(KERN_INFO "Invalid pin number in %s - should in [2 .. 27]\n", __FUNCTION__);
        return;
        }

    if(OBMSK_TRUE == g_auPinAllocated[uMatchingIndex])
        {
        // Deallocate pin
        g_auPinAllocated[uMatchingIndex] = OBMSK_FALSE;
        return;
        }
    else
        {
        // Pin already deallocated
        printk(KERN_INFO "%s: Pin %u has not been allocated\n", __FUNCTION__, gpio_pin_number);
        return;
        }
    } // obmsk_gpio_free()

EXPORT_SYMBOL(obmsk_gpio_free);

//-------------------------------------------------------------------------------------------------

//int32_t obmsk_gpio_to_irq(uint32_t gpio)
//// map GPIO numbers to IRQ numbers
//    {
//
//    } //

//-------------------------------------------------------------------------------------------------

//int32_t obmsk_irq_to_gpio(uint32_t irq);
//// map IRQ numbers to GPIO numbers (avoid using this)
//	{
//
//	} // obmsk_irq_to_gpio

//*************************************************************************************************

module_init(gpio_init);
module_exit(gpio_exit);

MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("Ambrose Chan");
MODULE_DESCRIPTION("GPIO base on iomem");
MODULE_SUPPORTED_DEVICE(OBMSK_GPIO);

//*************************************************************************************************

