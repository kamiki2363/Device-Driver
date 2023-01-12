#define Skip_Rom   0xCC
#define Convert_T  0x44
#define Read_Scratchpad  0xBE
#define high 1
#define low 0
#define Read_Rom 0x33





#define WRITE_0(uPin) {\
    DEFINE_SPINLOCK(request_lock);\
    spin_lock(&request_lock);\
    obmsk_gpio_set_value(uPin, low);\
    udelay(60); \
    obmsk_gpio_set_value(uPin, high);\
    spin_unlock(&request_lock);\
    udelay(1);\
    }

#define WRITE_1(uPin) {\
    DEFINE_SPINLOCK(request_lock);\
    spin_lock(&request_lock);\
    obmsk_gpio_set_value(uPin, low);\
    udelay(15); \
    obmsk_gpio_set_value(uPin, high);\
    udelay(45);\
    spin_unlock(&request_lock);\
    udelay(1);\
    }

#define READ_bit(uPin) {\
    DEFINE_SPINLOCK(request_lock);\
    spin_lock(&request_lock);\
    obmsk_gpio_set_value(uPin, low);\
    udelay(1); \
    obmsk_gpio_set_value(uPin, high);\
    udelay(12);\
    nValue = obmsk_gpio_get_value(uPin);\
    spin_unlock(&request_lock);\
    udelay(47);\
    }






