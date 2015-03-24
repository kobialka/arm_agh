/*  main.c  */

#include <LPC21xx.H>
#include "keyboard.h"
#include "servo.h"
#include "uart.h"
#include "string.h"
#include "command_decoder.h"


#define SERVO_FREQ	(100)


extern char												cOdebranyZnak;   
extern					tsRecieverBuffer	sRecieverBuffer;				
extern					tKeyword					asKeywordList[];				// lista slów kluczowych.
extern					tToken						asToken[MAX_TOKEN_NR];	// tablica tokenów. 
extern unsigned char 							ucTokenNr;							// ilosc nalezionych tokenow.

tKeyboardState eServoButt;



int main()
{
	
	char acMessageBuffer[RECIEVER_SIZE];
		
	Servo_Init(SERVO_FREQ);
	UART_InitWithInt(9600);
	
	Reciever_PutCharacterToBuffer('g');
	Reciever_PutCharacterToBuffer('o');
	Reciever_PutCharacterToBuffer('t');
	Reciever_PutCharacterToBuffer('o');
	Reciever_PutCharacterToBuffer(' ');
	Reciever_PutCharacterToBuffer('1');
	Reciever_PutCharacterToBuffer('2');
	Reciever_PutCharacterToBuffer(TERMINATOR);
	while(1)
	{
		if(READY == eReciever_GetStatus() )
		{
			tToken *psToken = asToken;
			
			Reciever_GetStringCopy(acMessageBuffer);
			DecodeMsg(acMessageBuffer);
			
			if((0 != ucTokenNr) && (KEYWORD == psToken->eType) ) 
				{
				switch(psToken->uValue.eKeyword)
				{
					case CALLIB:
						Servo_Calib();
						break;
					
					case GOTO:
						if (NUMBER == (psToken+1)->eType)
						{
							Servo_GoTo((++psToken)->uValue.uiNumber);
						}
						break;
				}
			}
		}
	}
}
