/*  main.c  */

#include <LPC21xx.H>
#include "uart.h"
#include "string.h"
#include "zegar.h"
#include "timer_interrupts.h"
#include "command_decoder.h"
#include "servo.h"



#define SERVO_FREQ	(190)

#define FLAG_TX_CALC_bm 	(1<<0)
#define FLAG_TX_GOTO_bm 		(1<<1)
#define FLAG_TX_CALIB_bm		(1<<2)






extern					tsRecieverBuffer	sRecieverBuffer;				
extern					tKeyword					asKeywordList[];				// lista slów kluczowych.
extern					tToken						asToken[MAX_TOKEN_NR];	// tablica tokenów. 
extern unsigned char 							ucTokenNr;							// ilosc nalezionych tokenow.


extern tsRecieverBuffer	sRecieverBuffer;
extern tsTransmiterBuffer sTransmiterBuffer;
extern struct sWatch Watch;







int main(){
	volatile char cTempVar = 0;
	char	fCalc = 0, fOk = 0;
	char acStringSec[]="sec ";		
	char acStringMin[]="min ";
	char acTempString[25];
	char acMessageBuffer[RECIEVER_SIZE];
	tToken *psToken = asToken;
	
	CopyString(acStringSec,acTempString);
	AppendUIntToString(Watch.ucSeconds,acTempString);
	AppendString("\n", acTempString);
	
	UART_InitWithInt(9600);
	Timer0Interrupts_Init(1000000,  WatchUpdate);	 // microseconds
	Servo_Init(SERVO_FREQ);
	
	
	
	while(1){
		if(Transmiter_GetStatus() == FREE){
			if(Watch.fSecondsValueChanged == 1){
				CopyString(acStringSec,acTempString);
				AppendUIntToString(Watch.ucSeconds,acTempString);
				Transmiter_SendString(acTempString);
				Watch.fSecondsValueChanged = 0;
			}
			else if(Watch.fMinutesValuedChanged == 1){
				CopyString(acStringMin,acTempString);
				AppendUIntToString(Watch.ucMinutes,acTempString);
				while( BUSY == Transmiter_GetStatus() ){};
				Transmiter_SendString(acTempString);
				Watch.fMinutesValuedChanged = 0;
			}
			else if(0 != fCalc ){
				char pcTempString[17] = "calc ";
				unsigned int uiTempVar = (psToken+1)->uValue.uiNumber * 2;
							
				AppendUIntToString(uiTempVar,pcTempString);
				AppendString(" \n",pcTempString);
				Transmiter_SendString(pcTempString);
				fCalc = 0;
				
			}
			else if(0 != fOk ){
				char pcTempString[17] = "ok";
				AppendString(" \n",pcTempString);
				Transmiter_SendString(pcTempString);
				fOk = 0;
				
			}
		}
		
		if(READY == eReciever_GetStatus() )
		{
			
			
			Reciever_GetStringCopy(acMessageBuffer);
			DecodeMsg(acMessageBuffer);
			
			if((0 != ucTokenNr) && (KEYWORD == psToken->eType) ) 
				{
				switch(psToken->uValue.eKeyword)
				{
					case CALC:								
						if( NUMBER == (psToken+1)->eType ){
							fCalc = 1;
						}
						
						break;
					
					case CALLIB:
						Servo_Calib();
					fOk = 1;
						break;
					
					case GOTO:
						if( NUMBER == (psToken+1)->eType ){
							Servo_GoTo((psToken+1)->uValue.uiNumber);
							fOk = 1;
						}
						break;
				}
			}
		}
	}
}






//int main()
//{
//	char acString[]="licznik ";		
//	char acTempString[25];
//	unsigned int uiTempVar = 0;
//	
//	UART_InitWithInt(9600);
//	
//	CopyString(acString,acTempString);
//	AppendUIntToString(uiTempVar, acTempString);
//	AppendString("\n", acTempString);
//	Transmiter_SendString(acTempString);

//	
//	while(1){
//		if(Transmiter_GetStatus() == FREE){
//				uiTempVar = uiTempVar + 1;		
//				CopyString(acString,acTempString);
//				AppendUIntToString(uiTempVar, acTempString);
//				Transmiter_SendString(acTempString);
//		}
//	}
//}












//int main()
//{
//	char acString[]="test123\n";		
//	
//	UART_InitWithInt(9600);
//	
//	Transmiter_SendString(acString );

//	while(1){
//		if(sTransmiterBuffer.eStatus == FREE){
//				Transmiter_SendString(acString);
//		}
//	}
//}


//int main()
//{
//	volatile char cTempVar = 0;
//	
//	UART_InitWithInt(9600);
//	
//	U0THR = ++cTempVar;
//	while(1){		
//	}
//}


