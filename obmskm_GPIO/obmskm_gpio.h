#ifndef OBMSKM_GPIO_INCLUDE
#   define OBMSKM_GPIO_INCLUDE

// Cross-reference to https://git.kernel.org/cgit/linux/kernel/git/stable/linux-stable.git/tree/Documentation/gpio/gpio.txt?id=refs/tags/v4.6.4

#define TRUE       1
#define FALSE      0
#define SUCCESS 0
#define EROUT      531 //Output fail
#define ERWRPIN    532 //Wrong Pin number
#define ERNOPIN    533 //Output Set fail
#define ERFUN      534 //Wrong function
#define ERALOPIN   535 //Pin is allocated
#define ERDEALOPIN 536 //Pin is not allocated

#define uNoGPIO_Pins   26
//#define uNoGPIO_Pins   8

unsigned auValidGPIO_Pins[uNoGPIO_Pins]= {2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27};
//unsigned auValidGPIO_Pins[uNoGPIO_Pins]= {4,14,10,9,15,11,8,7};


int obmskm_gpio_is_valid(unsigned gpio_pin_number);	// Is this needed?

int obmskm_gpio_direction_input(unsigned gpio_pin_number);
/* set as input, returning 0 or negative errno */

int obmskm_gpio_direction_output(unsigned gpio_pin_number);
/* set as output, returning 0 or negative errno */

int obmskm_gpio_get_value(unsigned gpio_pin_number);
/* GPIO INPUT:  return zero or nonzero */

void obmskm_gpio_set_value(unsigned gpio_pin_number, int value);
/* GPIO OUTPUT */

int obmskm_gpio_cansleep(unsigned gpio_pin_number); // Always returns false

int obmskm_gpio_request(unsigned gpio_pin_number, const char *label);
/* request GPIO, returning 0 or negative errno. non-null labels may be useful for diagnostics.*/

void obmskm_gpio_free(unsigned gpio_pin_number);
/* release previously-claimed GPIO */

int obmskm_gpio_to_irq(unsigned gpio_pin_number);
/* map GPIO numbers to IRQ numbers */

int obmskm_gpio_to_irq(unsigned gpio_pin_number);
/* map GPIO numbers to IRQ numbers */

#endif // #ifndef OBMSKM_GPIO_INCLUDE

