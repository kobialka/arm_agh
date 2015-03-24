#include <LPC21xx.H>
#include "keyboard.h"
#include "led.h"
#include "timer.h"

typedef enum tAutoState{MOVING_LEFT, MOVING_RIGHT, WAITING} tAutoState;
tAutoState eAutoState = WAITING;

int main()
{
	tKeyboardState eKeyboardState;
	
	InitTimer0Match0(100000);
	Led_Init();
	KeyboardInit();
	
	
	while(1)
	{
		switch(eAutoState)
		{
			case WAITING:
				if( BUTTON_2 == eKeyboard_Read())
				{
					eAutoState = WAITING;
				}
				else if (BUTTON_1 == eKeyboard_Read())
				{
					eAutoState = MOVING_LEFT;
					Led_StepLeft();
				}
				else if (BUTTON_3 == eKeyboard_Read())
				{
					eAutoState = MOVING_RIGHT;
					Led_StepRight();
				}
				break;
			
			case MOVING_LEFT:
				if(BUTTON_2 == eKeyboard_Read())
				{
					eAutoState = WAITING;
				}
				else
				{
					eAutoState = MOVING_LEFT;
					Led_StepLeft();
				}
				break;
			
			case MOVING_RIGHT:
				if(BUTTON_2 == eKeyboard_Read())
				{
					eAutoState = WAITING;
				}
				else
				{
					eAutoState = MOVING_RIGHT;
					Led_StepRight();
				}
				break;
		}
		WaitOnTimer0Match0();
	}
}
