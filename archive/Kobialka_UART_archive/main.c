/*  main.c  */

#include <LPC21xx.H>
#include "keyboard.h"
#include "servo.h"
#include "uart.h"


#define SERVO_FREQ	(100)


extern char cOdebranyZnak;   


tKeyboardState eServoButt;

int main ()
{
	Servo_Init(SERVO_FREQ);
	UART_InitWithInt(9600);
	
	eServoButt = eKeyboard_Read();
	
	while(1)
	{
		eServoButt = eKeyboard_Read();
		
		switch (eServoButt)
		{
			case BUTTON_1:
				eServoButt = RELASED;
				cOdebranyZnak = 0;
				Servo_Calib();
				break;
			
			case BUTTON_2:
				cOdebranyZnak = 0;
				Servo_GoTo(50);
				break;
			
			case BUTTON_3:
				cOdebranyZnak = 0;
				Servo_GoTo(100);
				break;
			
			case BUTTON_4:
				cOdebranyZnak = 0;
				Servo_GoTo(150);
				break;
			
			case RELASED:
				break;
		}
		
		
		
		switch (cOdebranyZnak)
		{
			case '1':
				Servo_Step();
				cOdebranyZnak = 0;
				break;
			
			case '2':
				Servo_GoTo(100);
				cOdebranyZnak = 0;
				break;
			
			case '3':
				Servo_GoTo(150);
				cOdebranyZnak = 0;
				break;
						
			case 'c':
				cOdebranyZnak = 0;
				Servo_Calib();
				break;
		}
	
		
	
		
		
	}
}



