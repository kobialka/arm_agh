/*  keyboard.c  */


#include <LPC21xx.H>
#include "keyboard.h"

#define LED0_bm (1<<16)	
#define LED1_bm (1<<17)	
#define LED2_bm (1<<18)	
#define LED3_bm (1<<19)	

#define Butt1_bm (1<<4)
#define Butt2_bm (1<<5)
#define Butt3_bm (1<<6)
#define Butt4_bm (1<<7)




// -------------------------------------------------------------------------------
void Button_Init(void)
{
	IO0DIR &= ~(Butt1_bm);
}

// -------------------------------------------------------------------------------
void Keyboard_Init(void)
{
	IO0DIR &= ~(Butt1_bm | Butt2_bm | Butt3_bm | Butt4_bm);
}

// -------------------------------------------------------------------------------
enum eKeyboardState eKeyboard_Read(void)
{
	if(0 == (IO0PIN & Butt1_bm))
	{
		return BUTTON_1;
	}
	else if(0 == (IO0PIN & Butt2_bm))
	{
		return BUTTON_2;
	}
	else if(0 == (IO0PIN & Butt3_bm))
	{
		return BUTTON_3;
	}
	else if(0 == (IO0PIN & Butt4_bm))
	{
		return BUTTON_4;
	}
	else 
	{
		return RELASED;
	}
}

