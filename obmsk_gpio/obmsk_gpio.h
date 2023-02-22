#ifndef OBMSK_GPIO_INCLUDE
#   define OBMSK_GPIO_INCLUDE

// Cross-reference to https://git.kernel.org/cgit/linux/kernel/git/stable/linux-stable.git/tree/Documentation/gpio/gpio.txt?id=refs/tags/v4.6.4

#define OBMSK_TRUE       1
#define OBMSK_FALSE      0

#define OBMSK_GPIO_STROBE_ALL_PINS_TEST 0
#define OBMSK_GPIO_CHECK_BUTTON_PRESS_TEST 0
#define OBMSK_SPEED_TEST 1

#include "obmsk_gpio_rpi2b_defs.h"

//*************************************************************************************************

extern int32_t obmsk_gpio_is_valid(uint32_t gpio_pin_number);	// Is this needed?

extern int32_t obmsk_gpio_direction_input(uint32_t gpio_pin_number);
/* set as input, returning 0 or negative errno */

extern int32_t obmsk_gpio_direction_output(uint32_t gpio_pin_number);
/* set as output, returning 0 or negative errno */

extern int32_t obmsk_gpio_get_value(uint32_t gpio_pin_number);
/* GPIO INPUT:  return zero or nonzero */

extern void obmsk_gpio_set_value(uint32_t gpio_pin_number, int32_t value);
/* GPIO OUTPUT */

extern int32_t obmsk_gpio_cansleep(uint32_t gpio_pin_number); // Always returns false

extern int32_t obmsk_gpio_request(uint32_t gpio_pin_number, const char *label);
/* request GPIO, returning 0 or negative errno. non-null labels may be useful for diagnostics.*/

extern void obmsk_gpio_free(uint32_t gpio_pin_number);
/* release previously-claimed GPIO */

extern int32_t obmsk_gpio_to_irq(uint32_t gpio_pin_number);
/* map GPIO numbers to IRQ numbers */

extern int32_t obmsk_irq_to_gpio(uint32_t irq);
/* map IRQ numbers to GPIO numbers (avoid using this) */

//*************************************************************************************************

#endif // OBMSK_GPIO_INCLUDE

