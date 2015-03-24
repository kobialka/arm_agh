/* 	
		Autor: Michal Kobialka
*/

#include "led.h"
#include "keyboard.h"





// *******  7.  *******


typedef enum Butts3State{MOVING_LEFT, WAITING, MOVING_RIGHT} Butts3State;

int main()
{
	Butts3State eButts3_State = WAITING;
	
	Led_Init();
	Keyboard_Init();
	
	while(1)
	{
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
		Delay(100);
	}
}










// ******* 6.  *******

// typedef enum ButtsState{WAITING, MOVING} ButtsState;

// int main()
// {
// 	ButtsState eButts_State = MOVING;
// 	
// 	Led_Init();
// 	Keyboard_Init();
// 	
// 	
// 	while(1)
// 	{
// 		switch(eButts_State)
// 		{
// 			case WAITING:
// 				if(BUTTON_2 == eKeyboard_Read())
// 				{
// 					eButts_State = MOVING;
// 				}
// 				else
// 				{
// 					eButts_State = WAITING;
// 				}
// 				break;
// 			
// 			case MOVING:
// 				if(BUTTON_1 == eKeyboard_Read())
// 				{
// 					eButts_State = WAITING;
// 				}
// 				else
// 				{
// 					eButts_State = MOVING;
// 					Led_StepRight();
// 				}
// 				break;
// 		}
// 		//Delay(300);
// 	}
// }







// *******  5.  *******

// typedef enum LedButtOnceState{WAITING, MOVING} LedButtOnceState;

// int main()
// {
// 	LedButtOnceState eLedButtOnce_State = WAITING;
// 	unsigned char ucStepCounter = 0;
// 	
// 	Led_Init();
// 	Keyboard_Init();
// 		
// 	while(1)
// 	{
// 		switch(eLedButtOnce_State)
// 		{
// 			case WAITING:
// 				if(BUTTON_1 == eKeyboard_Read())
// 				{
// 					eLedButtOnce_State = MOVING;
// 				}
// 				else
// 				{
// 					eLedButtOnce_State = WAITING;
// 				}
// 				break;
// 			
// 			case MOVING:
// 				if(0 == (++ucStepCounter%4) )
// 				{
// 					eLedButtOnce_State = WAITING;
// 				}
// 				else
// 				{
// 					eLedButtOnce_State = MOVING;
// 					Led_StepLeft();
// 				}
// 				break;
// 		}
// 		Delay(200);
// 	}
// }






// *******  4.  *******

// typedef enum LedOnce3StepsState{STOP, MOVING} LedOnce3StepsState;

// int main()
// {
// 	unsigned char ucStepCounter = 0;
// 	LedOnce3StepsState eLedOnce3Steps_State = MOVING;
// 	
// 	Led_Init();
// 	Keyboard_Init();
// 	
// 	while(1)
// 	{
// 		switch(eLedOnce3Steps_State)
// 		{
// 			case STOP:
// 				eLedOnce3Steps_State = STOP;
// 				break;
// 			
// 			case MOVING:
// 				if(0 == (++ucStepCounter%3) )
// 				{
// 					eLedOnce3Steps_State = STOP;
// 				}
//				else
//				{
//					eLedOnce3Steps_State = MOVING;
//				}
// 				Led_StepLeft();
// 				break;
// 		}
// 		Delay(500);
// 	}	
// }








// *******  3.  *******

// typedef enum Led3StepsState{MOVING_LEFT,MOVING_RIGHT} Led3StepsState;

// int main()
// {
// 	unsigned char ucStepCounter = 0;
// 	
// 	Led3StepsState eLed3Steps_State = MOVING_LEFT;
// 	
// 	
// 	Led_Init();
// 	Keyboard_Init();
// 	
// 	while(1)
// 	{
// 		switch(eLed3Steps_State)
// 		{
// 			case MOVING_LEFT:
// 				if(0 == (++ucStepCounter%3) )
// 				{
// 					eLed3Steps_State = MOVING_RIGHT;
// 				}
// 				else
// 				{
// 					eLed3Steps_State = MOVING_LEFT;
// 				}
// 				Led_StepLeft();
// 				break;
// 				
// 			case MOVING_RIGHT:
// 				if(0 == (++ucStepCounter%3))
// 				{
// 					eLed3Steps_State = MOVING_LEFT;
// 				}
// 				else
// 				{
// 					eLed3Steps_State = MOVING_RIGHT;
// 				}
// 				Led_StepRight();
// 				break;
// 		}
// 		Delay(333);
// 	}
// }






// *******  2.  *******

// typedef enum LedStepsState{STATE0, STATE1, STATE2, STATE3, STATE4, STATE5} LedStepsState;

// int main()
// {
// 	LedStepsState eLedSteps_State = STATE0;
// 	
// 	Led_Init();
// 	Keyboard_Init();
// 	
// 	while(1)
// 	{
// 		switch(eLedSteps_State)
// 		{
// 			case STATE0:
// 				eLedSteps_State = STATE1;
// 				Led_StepLeft();
// 				break;
// 			
// 			case STATE1:
// 				eLedSteps_State = STATE2;
// 				Led_StepLeft();
// 				break;
// 			
// 			case STATE2:
// 				eLedSteps_State = STATE3;
// 				Led_StepLeft();
// 				break;
// 			
// 			case STATE3:
// 				eLedSteps_State = STATE4;
// 				Led_StepRight();
// 				break;
// 			
// 			case STATE4:
// 				eLedSteps_State = STATE5;
// 				Led_StepRight();
// 				break;
// 			
// 			case STATE5:
// 				eLedSteps_State = STATE0;
// 				Led_StepRight();
// 				break;
// 		}
// 		Delay(333);
// 	}
// }




// *******  1.  *******

// typedef enum LedDirState{LED_LEFT,LED_RIGHT} LedDirState;

// int main()
// {
// 	LedDirState eLedDir_State = LED_LEFT;
// 	
// 	Led_Init();
// 	Keyboard_Init();

// 	while(1)
// 	{
// 		switch(eLedDir_State)
// 		{
// 			case LED_LEFT:
// 				eLedDir_State = LED_RIGHT;
// 				Led_StepRight();
// 				break;
// 			
// 			case LED_RIGHT:
// 				eLedDir_State = LED_LEFT;
// 				Led_StepRight();
// 				break;
// 		}
// 		Delay(500);
// 	}
// }



