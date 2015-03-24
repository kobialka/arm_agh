/*  led.c  */
#include <LPC21xx.H>
#include "led.h"


#define LED0_bm (1<<16)
#define LED1_bm (1<<17)
#define LED2_bm (1<<18)
#define LED3_bm (1<<19)



typedef enum tLedDirection{LEFT, RIGHT} tLedDirection;

void Led_Init(void)
{
	IO1DIR |= LED0_bm | LED1_bm | LED2_bm | LED3_bm;
	IO1SET |= LED0_bm;
}

void Led_On(unsigned char ucLedIndeks)
{
	IO1CLR |= LED0_bm | LED1_bm | LED2_bm | LED3_bm;
	switch(ucLedIndeks)
	{
		case 0:
			IO1SET |= LED0_bm;
			break;
		
		case 1:
			IO1SET |= LED1_bm;
			break;
		
		case 2:
			IO1SET |= LED2_bm;
			break;
		case 3:
			IO1SET |= LED3_bm;
			break;
	}
}

void Led_Step(tLedDirection eLedDirection)
{
	static unsigned char ucLedIndeks = 0;
	
	switch(eLedDirection)
	{
		case LEFT:
			ucLedIndeks--;
			Led_On(ucLedIndeks%4);	
			break;
		case RIGHT:
			ucLedIndeks++;
			Led_On(ucLedIndeks%4);
			break;
	}
}

void Led_StepLeft(void)
{
	Led_Step(LEFT);
}
void Led_StepRight(void)
{
	Led_Step(RIGHT);
}
