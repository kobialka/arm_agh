/*  led.c  */


#include <LPC21xx.H>
#include "led.h"

#define LED0_bm (1<<16)	
#define LED1_bm (1<<17)	
#define LED2_bm (1<<18)	
#define LED3_bm (1<<19)	

#define Butt1_bm (1<<4)
#define Butt2_bm (1<<5)
#define Butt3_bm (1<<6)
#define Butt4_bm (1<<7)






// -------------------------------------------------------------------------------
void Delay(unsigned int mSeconds)
{
	unsigned int uiLoopCounter;
	
	for(uiLoopCounter=0; uiLoopCounter<(2143*mSeconds); uiLoopCounter++);
}

// -------------------------------------------------------------------------------
void Led_Init(void)
{		
	IO1DIR |= LED0_bm | LED1_bm | LED2_bm | LED3_bm;
	IO1SET |= LED0_bm;
}

// -------------------------------------------------------------------------------
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


// -------------------------------------------------------------------------------
void Led_Step(enum LedDirection eLedDirection)
{
	static unsigned int suiLedTurnedOn_local = 0;
	
	switch (eLedDirection)
	{
		case LEFT:
			suiLedTurnedOn_local = (suiLedTurnedOn_local+1)%4;
			Led_On(suiLedTurnedOn_local);
		break;
		
		case RIGHT:
			suiLedTurnedOn_local = (suiLedTurnedOn_local-1)%4;
			Led_On(suiLedTurnedOn_local);
		break;
	}
}


// -------------------------------------------------------------------------------
void Led_StepLeft(void)
{
	Led_Step(LEFT);
}


// -------------------------------------------------------------------------------
void Led_StepRight(void)
{
	Led_Step(RIGHT);
}




