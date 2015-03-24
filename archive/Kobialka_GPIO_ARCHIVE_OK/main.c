/* 	
		Autor: Michal Kobialka
*/

#include <LPC21xx.H>

#define LED0_bm 0x00010000
#define LED1_bm 0x00020000
#define LED2_bm 0x00040000
#define LED3_bm 0x00080000

#define Butt1_bm 0x00000010
#define Butt2_bm 0x00000020
#define Butt3_bm 0x00000040
#define Butt4_bm 0x00000080


unsigned int uiLoopCounter;
// enum eButtonState{RELASED, PRESSED};
enum eKeyboardState {RELASED, BUTTON_1, BUTTON_2, BUTTON_3, BUTTON_4};
enum LedDirection{LEFT, RIGHT};

unsigned int uiLedTurnedOn;

void Delay1Sec(void)
{
	for(uiLoopCounter=0; uiLoopCounter<2399999; uiLoopCounter++);
}


void Delay(unsigned int mSeconds)
{
	unsigned int uiLoopCounter;
	
	for(uiLoopCounter=0; uiLoopCounter<(2143*mSeconds); uiLoopCounter++);
}


void Led_Init(void)
{		
	IO1DIR |= LED0_bm | LED1_bm | LED2_bm | LED3_bm;
	IO1SET |= LED1_bm;
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


void ButtonInit(void)
{
	IO0DIR &= ~(Butt1_bm);
}


//enum eButtonState ReadButton1(void)
//{
//	if(IO0PIN & Butt1_bm) return RELASED;
//	else return PRESSED;
//}


void KeyboardInit(void)
{
	IO0DIR &= ~(Butt1_bm | Butt2_bm | Butt3_bm | Butt4_bm);
}


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
	

void StepLeft(void)
{
	uiLedTurnedOn = (uiLedTurnedOn+1)%4;
	Led_On(uiLedTurnedOn);
}


void StepRight(void)
{
	uiLedTurnedOn = (uiLedTurnedOn-1)%4;
	Led_On(uiLedTurnedOn);
}


void Led_Step(enum LedDirection eLedDirection)
{
	static unsigned int suiLedTurnedOn_local = 1;
	
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
	
void Led_StepLeft(void)
{
	Led_Step(LEFT);
}

void Led_StepRight(void)
{
	Led_Step(RIGHT);
}


int main()
{
	Led_Init();
	KeyboardInit();

	while(1)
	{
		switch(eKeyboard_Read())
	{
		case BUTTON_1:
			Led_StepRight();
			break;
		case BUTTON_2:
			Led_StepLeft();
			break;
		case RELASED:
			break;
	}
	}
}



// *******  23. ******
// int main()
// {
// 	Led_Init();
// 	KeyboardInit();

// 	while(1)
// 	{
// 		switch(eKeyboard_Read())
// 	{
// 		case BUTTON_1:
// 			Led_Step(RIGHT);
// 			break;
// 		case BUTTON_2:
// 			Led_Step(LEFT);
// 			break;
// 		case RELASED:
// 			break;
// 	}
// 	}
// }



// ******* 21. Led_Step  *******
//int main()
//{
//	Led_Init();
//	KeyboardInit();
//	
//	while(1)
//	{
//		unsigned int uiLoopCounter;
//		
//		for (uiLoopCounter=0; uiLoopCounter<9; uiLoopCounter++)
//		{
// 			Led_Step(RIGHT);
//			Delay(100);
//		}
//		for (uiLoopCounter=0; uiLoopCounter<9; uiLoopCounter++)
//		{
//			Delay(100);
//			Led_Step(LEFT);
//		}
//	}
//}
		
		
//int main()
//{
//	Led_Init();
//	
//	while(1)
//	{
//		StepRight();
//		Delay(250);
//	}
//}


// *******  9. Miganie diody  *******
// 		IO1SET |= LED3_bm;
// 		Delay(50);
// 		IO1CLR |= LED3_bm;
// 		Delay(50);
		
		
// *******  10. zapalanie kolejnych diod  *******
// 		IO1SET |= LED0_bm;
// 		IO1SET |= LED1_bm;
// 		IO1SET |= LED2_bm;
// 		IO1SET |= LED3_bm;
		
		
// *******  11. przesuwanie punktu swietlnego  *******
//int main()
//{
//	Led_Init();
//	
//	while(1)
//	{
// 		IO1SET |= LED0_bm;
// 		Delay(250);
// 		IO1CLR |= LED0_bm;
// 		IO1SET |= LED1_bm;
// 		Delay(250);
// 		IO1CLR |= LED1_bm;
// 		IO1SET |= LED2_bm;
// 		Delay(250);
// 		IO1CLR |= LED2_bm;
// 		IO1SET |= LED3_bm;
// 		Delay(250);
// 		IO1CLR |= LED3_bm;		
//	}
//}





// *******  13. Led_On  *******
//int main()
//{
//	Led_On(0);
//}
		
	
		
		
// ******* 16. ReadButton1 wersja ENUM *******
// int main()
// {
//	 Led_Init();
//	 ButtonInit();
//	 
//		while(1)
//		{
//			switch (ReadButton1())
//			{
//				case PRESSED:
//					IO1CLR |= LED0_bm;
//					IO1SET |= LED1_bm;
//					break;
//				case RELASED:
//					IO1CLR |= LED1_bm;
//					IO1SET |= LED0_bm;
//				break;
//			}
// 		}
//	}


// *******  18. eKeyboardRead()  *******
// int main()
// {
//	Led_Init();
//	KeyboardInit();
//	
//	while(1)
//	{
//			// *******18. eKeyboardRead  *******
//			switch (eKeyboard_Read())
//			{
//			case BUTTON_1:
//				IO1CLR |= LED1_bm | LED2_bm | LED3_bm;
//				IO1SET |= LED0_bm;
//				break;
//			case BUTTON_2:
//				IO1CLR |= LED0_bm | LED2_bm | LED3_bm;
//				IO1SET |= LED1_bm;
//				break;
//			case BUTTON_3:
//				IO1CLR |= LED0_bm | LED1_bm | LED3_bm;
//				IO1SET |= LED2_bm;
//				break;
//			case BUTTON_4:
//				IO1CLR |= LED0_bm | LED1_bm | LED2_bm;
//				IO1SET |= LED3_bm;
//				break;
//			case RELASED:
//				IO1CLR |= LED0_bm | LED1_bm | LED2_bm | LED3_bm;
//				break;
//			}
//	}
// }


// *******  19. StepLeft  *******
//int main()
//{
//	Led_Init();
//	
//	while(1)
//	{
//		StepLeft();
//		Delay(250);
//	}
//}


// *******  20. StepRight  *******
//int main()
//{
//	Led_Init();
//	
//	while(1)
//	{
//		StepRight();
//		Delay(250);
//	}
//}



	




