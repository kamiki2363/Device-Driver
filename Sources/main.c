/* ###################################################################
**     Filename    : main.c
**     Project     : Assesment_1
**     Processor   : MKL25Z128VLK4
**     Version     : Driver 01.01
**     Compiler    : GNU C Compiler
**     Date/Time   : 2015-11-19, 11:07, # CodeGen: 0
**     Abstract    :
**         Main module.
**         This module contains user's application code.
**     Settings    :
**     Contents    :
**         No public methods
**
** ###################################################################*/
/*!
** @file main.c
** @version 01.01
** @brief
**         Main module.
**         This module contains user's application code.
*/         
/*!
**  @addtogroup main_module main module documentation
**  @{
*/         
/* MODULE main */


/* Including needed modules to compile this module/procedure */
#include "Cpu.h"
#include "Events.h"
#include "RED_LED.h"
#include "GREEN_LED.h"
#include "TU1.h"
#include "BLUE_LED.h"
#include "TU2.h"
#include "TSS1.h"
/* Including shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"


/* User includes (#include below this line is not maintained by Processor Expert) */

	/*  TSS_Task();
	 x=TSS1_cKey0.Position*(0.015625)*x; */


int turn_on_the_light(int R, int G,int B,int light)
{	int R_bright,G_bright,B_bright,speed;
	float brightness;
if (light==1){		/*because the brightness may not equal to zero so that the number should be count down while in the starting point*/
	R_bright=0;
	G_bright=0;
	B_bright=0;
}

	for(brightness=0;brightness<0x10000;brightness++)/* brightness is counting from 0-100%(0-65535) here*/
	{
	 	TSS_Task();							/*to do the TSS_task for detect the slider*/
	 	speed = (TSS1_cKey0.Position);		/*make the slider is equal the number that boost number of brightness*/
	 	brightness=brightness+speed;		/*adding the number of touch slider can boost up the brightness of the LED also choose 64 is because when 0xFFFF/255=257/64=4 that is mean when it in the maximum speed it can be tick 4 times to change the color of 1 in different light*/
		if(brightness>0x10000){brightness=0XFFFF;} /* to secure the number is not add over because when it add over it will recounter(turn_off) so that you may see the light is not smooth enough*/
		if (R_bright!=0xFFFF && R==1){		/*to secure the light is not turn on in the same time if is necessary to check that if the light is turn on already*/
		R_bright=brightness;}else
		if (G_bright!=0xFFFF && G==1){
		G_bright=brightness;}else
		if (B_bright!=0xFFFF && B==1){
		B_bright=brightness;}

		RED_LED_SetRatio16(RED_LED_DeviceData,R_bright); /* this is the light will relate to the value of bright in the if sentences*/
		GREEN_LED_SetRatio16(GREEN_LED_DeviceData,G_bright);
		BLUE_LED_SetRatio16(BLUE_LED_DeviceData,B_bright);
		}
}
	int turn_off (int R,int G, int B)
	{int R_bright,G_bright,B_bright,speed;
		float brightness;

		for(brightness=0x10000;brightness>0;brightness--) /* to do the count down here*/
		{
		 	 TSS_Task();
		 	 speed = (TSS1_cKey0.Position);
		 	brightness=brightness-speed;
		 	 if(brightness<0){brightness=1;}
			if (R_bright!=0 && R==0){
			R_bright=brightness-1;}else
			if (G_bright!=0 && G==0){
			G_bright=brightness-1;}else
			if (B_bright!=0 && B==0){
			B_bright=brightness-1;}

			RED_LED_SetRatio16(RED_LED_DeviceData,R_bright);
			GREEN_LED_SetRatio16(GREEN_LED_DeviceData,G_bright);
			BLUE_LED_SetRatio16(BLUE_LED_DeviceData,B_bright);
			}}



		/*  TSS_Task();
 	 	 x=TSS1_cKey0.Position*(0.015625)*x; */



/*lint -save  -e970 Disable MISRA rule (6.3) checking. */
int main(void)
/*lint -restore Enable MISRA rule (6.3) checking. */
{
	int light=1,R=1,G=0,B=0;;/* this parameter is set for weitch light*/
  /* Write your local variable definition here */

  /*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
  PE_low_level_init();
  /*** End of Processor Expert internal initialization.                    ***/
  /* Write your code here */


 for(;;){

	 	switch(light)				/*the switch will do the job for switch light in form RGB for using the value in 1 and 0 is turning on or off during that case*/
	      { case 1:			/*also the light can be set in the form G=R^G; to make the process be easy put this is chosen because it is convenient for other to read*/
	    	  turn_on_the_light(R,G,B,light); /* in this case R is light on in the first step as the number that set on the top to make sure the light will not lights up in any wide position*/

	    	 break;
	      case 2:			/* this case is turn the light on of GREEN light*/
	     	 R=1;
	     	 G=1;
	     	 B=0;
		     turn_on_the_light(R,G,B,light);

	     	 break;
	      case 3:			/* this case is turn off the light of RED*/
	    	 R=0;
	    	 G=1;
	    	 B=0;
		     turn_off(R,G,B);

		     break;
	      case 4:			/*this case is turn on the light of BLUE*/
	     	 R=0;
	     	 G=1;
	     	 B=1;
	     	 turn_on_the_light(R,G,B,light);

	     	break;
	      case 5:			/* this case is turn off the light of GREEN*/
	     	 R=0;
	     	 G=0;
	     	 B=1;
		     turn_off(R,G,B);

	     	break;
	      case 6:			/* this case is turn on the light of RED*/
	     	 R=1;
	     	 G=0;
	     	 B=1;
	     	turn_on_the_light(R,G,B,light);

	     	break;
	      case 7:			/* this case is turn off the light of BLUE*/
	     	 R=1;
	     	 G=0;
	     	 B=0;
	     	 turn_off(R,G,B);
	     	 break;

	      }
	 	light++;  /*while the light is switch the parameter of light will count up mean it is finish the step of turn on or off the light*/
	 	if(light>7){light=2;}/*when the number in light is over 7 mean the LED is finish the period so the period will start again for light up the green, because the red light is light up*/
 }							/*during in the case 6 so that the step of next period should start in case 2(light=2)*/


  /* For example: for(;;) { } */

  /*** Don't write any code pass this line, or it will be deleted during code generation. ***/
  /*** RTOS startup code. Macro PEX_RTOS_START is defined by the RTOS component. DON'T MODIFY THIS CODE!!! ***/
  #ifdef PEX_RTOS_START
    PEX_RTOS_START();                  /* Startup of the selected RTOS. Macro is defined by the RTOS component. */
  #endif
  /*** End of RTOS startup code.  ***/
  /*** Processor Expert end of main routine. DON'T MODIFY THIS CODE!!! ***/
  for(;;){}
  /*** Processor Expert end of main routine. DON'T WRITE CODE BELOW!!! ***/
  } /*** End of main routine. DO NOT MODIFY THIS TEXT!!! ***/

/* END main */
/*!
** @}
*/
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.5 [05.21]
**     for the Freescale Kinetis series of microcontrollers.
**
** ###################################################################
*/
