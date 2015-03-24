#include "led.h"
#include "timer_interrupts.h"
#include "keyboard.h"
#include "servo.h"

tKeyboardState eServoButt;

int main ()
{
	Servo_Init(180);
	
	while(1)
	{
		eServoButt = eKeyboard_Read();
		
		switch (eServoButt)
		{
			case BUTTON_1:
				Servo_Calib();
				break;
			
			case BUTTON_2:
				Servo_GoTo(50);
				break;
			
			case BUTTON_3:
				Servo_GoTo(100);
				break;
			
			case BUTTON_4:
				Servo_GoTo(150);
				break;
			
			case RELASED:
				break;
		}
	}
}



