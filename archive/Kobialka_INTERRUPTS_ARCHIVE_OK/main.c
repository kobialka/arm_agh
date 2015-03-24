/*  main.c  */

typedef enum Butts3State{MOVING_LEFT, WAITING, MOVING_RIGHT} Butts3State;
Butts3State eButts3_State;

#include "timer_interrupts.h"
#include "led.h"
#include "keyboard.h"

void Automat(void)
{
	
// *******  7.  *******
	  
	
		switch(eButts3_State)
		{
			case MOVING_LEFT:
				if(BUTTON_2 == eKeyboard_Read())
				{
					eButts3_State = WAITING;
				}
				else
				{
					eButts3_State = MOVING_LEFT;
					Led_StepLeft();
				}
				break;
			
			case WAITING:
				if(BUTTON_1 == eKeyboard_Read())
				{
					eButts3_State = MOVING_LEFT;
				}
				else if(BUTTON_3 == eKeyboard_Read())
				{
					eButts3_State = MOVING_RIGHT;
				}
				else
				{
					eButts3_State = WAITING;
				}
				break;
			
			case MOVING_RIGHT:
				if(BUTTON_2 == eKeyboard_Read())
				{
					eButts3_State = WAITING;
				}
				else
				{
					eButts3_State = MOVING_RIGHT;
					Led_StepRight();
				}
				break;
		}
		//Delay(100);
}


int main (){
	unsigned int iMainLoopCtr;
	Butts3State eButts3_State = WAITING;
	
	Led_Init();
	Keyboard_Init();
	Timer0Interrupts_Init(100000,&Automat);

	while(1){
	 	iMainLoopCtr++;
	}
}
