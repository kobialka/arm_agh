/* servo.c  */

#include <LPC21xx.H>
#include "servo.h"
#include "led.h"
#include "timer_interrupts.h"


#define DETECTOR_PIN_bm	(1<<10)


// =======================================================================================================
typedef enum tServoState{CALIB, IDDLE, IN_PROGRESS} tServoState;
typedef enum tDetectorState{ACTIVE, INACTIVE} tDetectorState;
typedef struct tsServo
{
	tServoState eState;
	unsigned int uiCurrentPosition;
	unsigned int uiDesiredPosition;
} tsServo;


// =======================================================================================================
tsServo sServo;
tServoState eAutoState = CALIB;


// =======================================================================================================
tDetectorState eReadDetector(void)
{
	if(0 == (IO0PIN & DETECTOR_PIN_bm) )
	{
		return INACTIVE;
	}
	else
	{
			return ACTIVE;
	}
}


// =======================================================================================================
void Automat(void)
{
	switch(eAutoState)
		{
			case CALIB:
				if( INACTIVE == eReadDetector() )
				{
					eAutoState = CALIB;
					Led_StepRight();
				}
				else
				{
					sServo.uiCurrentPosition = 0;
					sServo.uiDesiredPosition = 0;
					eAutoState = IDDLE;
				}
				break;
				
			case IDDLE:
				if(sServo.uiDesiredPosition == sServo.uiCurrentPosition)
				{
					eAutoState = IDDLE;
				}
				else if(sServo.uiDesiredPosition != sServo.uiCurrentPosition)
				{
					eAutoState = IN_PROGRESS;
				}
				break;
			
			case IN_PROGRESS:
				if(sServo.uiCurrentPosition < sServo.uiDesiredPosition)
				{
					Led_StepRight();
					sServo.uiCurrentPosition++;
					eAutoState = IN_PROGRESS;
				}
				else if(sServo.uiCurrentPosition > sServo.uiDesiredPosition)
				{
					Led_StepLeft();
					sServo.uiCurrentPosition--;
					eAutoState = IN_PROGRESS;
				}
				else if(sServo.uiDesiredPosition == sServo.uiCurrentPosition)
				{
					eAutoState = IDDLE;
				}
				break;
		}
}


// =======================================================================================================
void DetectorInit(void)
{
	IO0DIR &= ~DETECTOR_PIN_bm;
}



// =======================================================================================================
void Servo_Init(unsigned int uiServoFrequency)
{
	eAutoState = CALIB;
	DetectorInit();
	Led_Init();
	Timer1Interrupts_Init( (1000000/uiServoFrequency), &Automat);
}



// =======================================================================================================
void Servo_Calib(void)
{
	eAutoState = CALIB;
}



// =======================================================================================================
void Servo_GoTo(unsigned int uiPosition)
{
	sServo.uiDesiredPosition = uiPosition;
}

// =======================================================================================================
void Servo_Step(void)
{
	sServo.uiDesiredPosition = sServo.uiCurrentPosition + 50;
}







