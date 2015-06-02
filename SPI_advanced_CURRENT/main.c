// SPI Advanced KOBIALKA TEST

#include <LPC21xx.H>
#include "spi_lab.h"
#include "timer.h"
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

// ZMIENNE
extern					tsRecieverBuffer	sRecieverBuffer;				
extern					tKeyword					asKeywordList[];				// lista slów kluczowych.
extern					tToken						asToken[MAX_TOKEN_NR];	// tablica tokenów. 
extern unsigned char 							ucTokenNr;							// ilosc nalezionych tokenow.

extern tsRecieverBuffer	sRecieverBuffer;
extern tsTransmiterBuffer sTransmiterBuffer;
extern struct sWatch Watch;

extern unsigned char pucSPI_TxBuffor[];
extern unsigned char pucSPI_RxBuffor[];
extern SPI_FrameParams_typedef sFrameParams_DAC;
extern SPI_FrameParams_typedef sFrameParams_PORT;
extern SPI_TransactionParams_typedef	sTransactionparams;

unsigned char ucTempVar = 0;

// ==================================================================================
int main(void){
	char	fCalc = 0, fOk = 0, fId = 0, fUnknownComm = 0, fGate = 0;
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
	//InitTimer0();
	Servo_Init(SERVO_FREQ);
	//SPI_ConfigMaster(sFrameParams_DAC);		
	//DAC_MCP4921_InitCSPin();
	SPI_ConfigMaster(sFrameParams_PORT);		
	Port_MCP23S09_InitCSPin();
	
	
//	while(1){
//		//DAC_MCP4921_Set(1000);	
//		DAC_MCP4921_Set_Adv(0);	
//		WaitOnTimer0(500000);
//		DAC_MCP4921_Set_Adv(2047);	
//		WaitOnTimer0(500000);
//		DAC_MCP4921_Set_Adv(4095);	
//		WaitOnTimer0(500000);
//	}
	
	
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
				Transmiter_SendString(acTempString);
				Watch.fMinutesValuedChanged = 0;
			}
			else if(1 == fCalc ){
				char pcTempString[17] = "calc ";
				unsigned int uiTempVar = (psToken+1)->uValue.uiNumber * 2;
							
				AppendUIntToString(uiTempVar,pcTempString);
				AppendString(" \n",pcTempString);
				Transmiter_SendString(pcTempString);
				fCalc = 0;
			}
			else if(1 == fOk ){
				Transmiter_SendString("ok\n");
				fOk = 0;
			}
			else if(1 == fGate){
				UCharToHexStr(ucTempVar, acTempString); 
				AppendString("\n",acTempString);
				Transmiter_SendString(acTempString);
				fGate = 0;				
			}
			else if(1 == fId){
				Transmiter_SendString("id lpc2129\n");
				fId = 0;
			}
			else if(1 == fUnknownComm){
				Transmiter_SendString("unknown command\n");
				fUnknownComm = 0;
			}
		}
		
		if(READY == eReciever_GetStatus() )
		{			
			Reciever_GetStringCopy(acMessageBuffer);
			DecodeMsg(acMessageBuffer);
			
			if((0 != ucTokenNr) && (KEYWORD == psToken->eType) ){
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
						
					case DACSET:
						
						if( NUMBER == (psToken+1)->eType ){
							// ====  BASIC  ====
							//DAC_MCP4921_Set((psToken+1)->uValue.uiNumber);
							
							// ====  ADVANCED  ====
							DAC_MCP4921_Set_Adv((psToken+1)->uValue.uiNumber);
							fOk = 1;
						}
						break;
						
					case ID:
						fId = 1;
						break;
					
					case PORTSET:
						if( NUMBER == (psToken+1)->eType ){
							Port_MCP23S09_Set((psToken+1)->uValue.uiNumber);
							fOk = 1;
						}
						break;
					
					case MCP_PORT_RD:
							ucTempVar = 0;
							ucTempVar = Port_MCP23S09_Get();
							Port_MCP23S09_Set(ucTempVar);
							fGate = 1;
						break;
				}
			}
			else {
				fUnknownComm = 1;
			}
		}
	}	
}




// ====  BASIC  ====
							//DAC_MCP4921_Set(0xc69);  // 1100 0110 1001
							//WaitOnTimer0(500000);
							//DAC_MCP4921_Set(2048);
							//WaitOnTimer0(500000);
							//DAC_MCP4921_Set(4095);
							//WaitOnTimer0(500000);



