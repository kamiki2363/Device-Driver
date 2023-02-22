// Raspberry Pi Model 2B specific constants

#ifndef OBMSK_GPIO_RPI2B_DEFS_H_INCLUDED
#   define OBMSK_GPIO_RPI2B_DEFS_H_INCLUDED

//*************************************************************************************************

#define PERIPHERAL_BASE_ADDRESS 0x3F000000
#define GPIO_BASE_ADDRESS (PERIPHERAL_BASE_ADDRESS + 0x200000)
#define ADDRESS_SPAN 0xB4 // = 180

#define FIRST_MINOR_NUMBER 2

#define NO_GPIO_PINS 26	// Number of GPIO pins

uint32_t auValidGPIO_Pins[NO_GPIO_PINS] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27};

#define RPI2B_GET_REGISTER 13
#define RPI2B_SET_REGISTER 7
#define RPI2B_CLEAR_REGISTER 10

// RPI2B-specific data access functions

#define SET_PIN_TO_INPUT(gpio_pin_number) {	\
	uint32_t uMask_Set_Input  = ~(0x00000111 << (((gpio_pin_number) % 10) * 3));	\
	uint32_t* puFunctionSelectRegister = g_pBaseRegister + (gpio_pin_number / 10);	\
	uint32_t uCurrentRegisterContents_Input = ioread32(puFunctionSelectRegister);	\
	uint32_t uNewRegisterContents = (uCurrentRegisterContents_Input & uMask_Set_Input);	\
	iowrite32(uNewRegisterContents, puFunctionSelectRegister);	\
    }

#define SET_PIN_TO_OUTPUT(gpio_pin_number) {    \
	uint32_t* puFunctionSelectRegister = g_pBaseRegister + (gpio_pin_number / 10);	\
	uint32_t uCurrentRegisterContents = ioread32(puFunctionSelectRegister);	\
	uint32_t uBitShift = (gpio_pin_number % 10) * 3;	\
	uint32_t uBitMask = (0x00000001 << uBitShift);	\
	uint32_t uNBitMask = ~(0x00000110 << uBitShift);	\
	uint32_t uNewRegisterContents = (uCurrentRegisterContents | uBitMask) & uNBitMask;	\
	iowrite32(uNewRegisterContents, puFunctionSelectRegister);	\
    }

#define GET_PIN_VALUE(gpio_pin_number) {	\
	uint32_t* puGPIO_GetRegister = (g_pBaseRegister + RPI2B_GET_REGISTER);	\
	uint32_t uInput32 = ioread32(puGPIO_GetRegister);	\
	uint32_t uMask32  = (0x00000001 << gpio_pin_number);	\
	nResult = (uInput32 & uMask32);\
    }

#define SET_PIN_VALUE(gpio_pin_number, value) { \
	uint32_t uMaskOutPut32 = (0x00000001 << gpio_pin_number);	\
	if(value)	\
		{	\
		uint32_t* volatile puGPIO_SetRegister = (g_pBaseRegister + RPI2B_SET_REGISTER);	\
		volatile uint32_t uCurrentRegisterContents = ioread32(puGPIO_SetRegister);	\
		uint32_t uNewRegisterContents = (uCurrentRegisterContents | uMaskOutPut32);	\
		uNewRegisterContents = (uNewRegisterContents & uMaskOutPut32);	\
		iowrite32(uNewRegisterContents, puGPIO_SetRegister);	\
		}	\
	else	\
		{	\
		uint32_t* volatile puGPIO_ClearRegister = (g_pBaseRegister + RPI2B_CLEAR_REGISTER);	\
		volatile uint32_t uCurrentRegisterContents = ioread32(puGPIO_ClearRegister);	\
		uint32_t uNewRegisterContents = (uCurrentRegisterContents | uMaskOutPut32);	\
		uNewRegisterContents = (uNewRegisterContents & uMaskOutPut32);	\
		iowrite32(uNewRegisterContents, puGPIO_ClearRegister);	\
		}\
    }

//*************************************************************************************************

#endif // OBMSK_GPIO_RPI2B_DEFS_H_INCLUDED
