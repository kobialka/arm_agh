// SPI Advanced KOBIALKA

#include <LPC21xx.H>
#include "spi_lab.h"
#include "timer.h"

/* PINY  
	SPI0:
		sck0 		- P0.4
		miso0		-	P0.5
		mosi0		- P0.6
		ssel0		- P0.7
	SPI1:
		sck1 		- P0.17
		miso1		-	P0.18
		mosi1		- P0.19
		ssel1		- P0.20
*/


// DEFINICJE
#define SPI_TX_BUFF_SIZE 256
#define SPI_RX_BUFF_SIZE 256

#define SPI_MCP4921_CS_PIN_bm	(1<<10)
#define SPI_MCP23S09_CS_PIN_bm	(1<<11)

#define SPI0_BITS_bm	 (0x0000FF00) //11 11 11 11 | 00 00 00 00 
#define SPI0_BITS_FUNCTION_SET_bm	 (0x00005500) //01 01 01 01 | 00 00 00 00 

#define SPI_LSBF_bm (1<<5)
#define SPI_MSTR_bm (1<<5)
#define SPI_CPOL_bm (1<<4)
#define SPI_CPHA_bm (1<<3)
#define SPI_SPIF_bm	(1<<7)
#define DAC_GA_bm		(1<<13)
#define DAC_SHDN_bm	(1<<12)

#define MCP23S09_DIR_REG_ADDR 0
#define MCP23S09_PORT_REG_ADDR 9

// ZMIENNE GLOBALNE
unsigned char pucSPI_TxBuffor[SPI_TX_BUFF_SIZE];
unsigned char pucSPI_RxBuffor[SPI_RX_BUFF_SIZE];
SPI_FrameParams_typedef sFrameParams_DAC = {0,0,0,255};   
SPI_FrameParams_typedef sFrameParams_PORT = {0,0,0,255};	
SPI_TransactionParams_typedef	sTransactionparams = {pucSPI_TxBuffor,0,0, pucSPI_RxBuffor,0,0};

// ====================================================================
// ================================  BASIC  ===========================
// ====================================================================


// FUNKCJE
void DAC_MCP4921_Set(unsigned int uiVoltage){
	// 0-4096
	
	unsigned int uiDataToTx;
	
	// PIN FUNCTION
	PINSEL0 &= ~(SPI0_BITS_bm);								
	PINSEL0 |= SPI0_BITS_FUNCTION_SET_bm;	
	
	// SLAVE SELECT PIN
	IO0DIR  |= SPI_MCP4921_CS_PIN_bm;						// Set as output
	IO0SET	|= SPI_MCP4921_CS_PIN_bm;
	
	// SPI MODULE
	S0SPCR |= SPI_MSTR_bm;										// Master mode select
	S0SPCR &= ~(SPI_CPHA_bm | SPI_CPOL_bm);		// rising edge
	S0SPCCR = 8;															// SPI rate = PCLK/SPCCR
	

	IO0CLR |= SPI_MCP4921_CS_PIN_bm;					// Slave select
	uiDataToTx = (uiVoltage & 0x0fff);				// limit voltage value
	uiDataToTx |= DAC_GA_bm | DAC_SHDN_bm;		// MCP4921 Control bits
	
	// TRANSMIT	
	S0SPDR = (char)(uiDataToTx >> 8);					// high byte
	while(!(S0SPSR & SPI_SPIF_bm)){} ;				// wait
	S0SPDR = (char)(uiDataToTx & 0x00ff);			// Low byte
	while(!(S0SPSR & SPI_SPIF_bm)){} ;				// wait
	IO0SET |= SPI_MCP4921_CS_PIN_bm;					// Slave select
}




// ====================================================================
// ==============================  ADVANCED  ==========================
// ====================================================================


// ==========================  // funkcja nie odpowiada za sygnal CS
void SPI_ConfigMaster(SPI_FrameParams_typedef sFrameParams){	
	// PIN FUNCTION
	PINSEL0 &= ~(SPI0_BITS_bm);								
	PINSEL0 |= SPI0_BITS_FUNCTION_SET_bm;	

	// CPHA, CPOL, LSBF
	S0SPCR &= ~(SPI_CPHA_bm | SPI_CPOL_bm | SPI_LSBF_bm);		
	if(sFrameParams.ucCpha == 1){
		S0SPCR |= SPI_CPHA_bm;
	}
	if(sFrameParams.ucCpol == 1){
		S0SPCR |= SPI_CPOL_bm;
	}
	if(sFrameParams.ucClsbf == 1){
		S0SPCR |= SPI_LSBF_bm;
	}
	
	// CLOCK DIVIDER
	S0SPCCR = sFrameParams.ClkDivider;				// SPI rate = PCLK/SPCCR
	
	// MASTER MODE
	S0SPCR |= SPI_MSTR_bm;										// Master mode select
}


// ==========================  //  funkcja nie odpowiada za sygnal CS
void SPI_ExecuteTransaction(SPI_TransactionParams_typedef sTransactionParams){
	unsigned char ucTotalByteCounter;
	unsigned char ucTotalByteNr = 0;  			// calkowita liczba bajtow to 'przetaktowania' przez zegar SPI.
	unsigned char  ucTxCounter = 0;		// zastosowano w celu skrocenia zapisu
	unsigned char  ucRxCounter = 0;		// zastosowano w celu skrocenia zapisu
	unsigned char ucTxByte;
	unsigned char ucRxByte;
	
	// okreslamy liczbe bajtow to 'przetaktowania' przez zegar SPI
	if( (sTransactionparams.ucNrOfBytesForTx + sTransactionparams.ucTxBytesOffset) >= (sTransactionparams.ucNrOfBytesForRx + sTransactionparams.ucRxBytesOffset) ){
		ucTotalByteNr = sTransactionparams.ucNrOfBytesForTx +sTransactionparams.ucTxBytesOffset;
	}
	else {
		ucTotalByteNr = sTransactionparams.ucNrOfBytesForRx + sTransactionparams.ucRxBytesOffset;
	}
	
	for( ucTotalByteCounter = 0; ucTotalByteCounter < ucTotalByteNr; ucTotalByteCounter++){
		if( (ucTxCounter < sTransactionparams.ucNrOfBytesForTx ) && (ucTotalByteCounter >= sTransactionparams.ucTxBytesOffset) ){  
			// Wykonaj jezeli: 'jest cos do wyslania' I 'wyszlismy z offsetu'
			ucTxByte = sTransactionparams.pucBytesForTx[ucTxCounter];
			ucTxCounter++;
		}
		else{
			ucTxByte = 0;
		}
		
		S0SPDR = ucTxByte;
		while(!(S0SPSR & SPI_SPIF_bm)){} ;
		ucRxByte = S0SPDR;
		
		if( (ucRxCounter < sTransactionparams.ucNrOfBytesForRx ) && ( ucTotalByteCounter >= sTransactionparams.ucRxBytesOffset) ){ 
			// Wykonaj jezeli: 'jest cos do odczytu' I 'przekroczono offset'
			sTransactionparams.pucBytesForRx[ucRxCounter] = S0SPDR;
			ucRxCounter++;
		}
	}
}


// ==========================
void DAC_MCP4921_InitCSPin(void){
	// SLAVE SELECT PIN
	IO0DIR  |= SPI_MCP4921_CS_PIN_bm;						// Set as output
	IO0SET	|= SPI_MCP4921_CS_PIN_bm;						// Set as HIGH - slave device not active.
}


// ==========================  funkcja ODPOWIADA za sygnal CS
void DAC_MCP4921_Set_Adv(unsigned int uiData){
	uiData = (uiData & 0x0fff);																// limit voltage value
	uiData |= DAC_GA_bm | DAC_SHDN_bm;												// MCP4921 Control bits
	pucSPI_TxBuffor[0] = (unsigned char)(uiData >> 8);				// High byte first
	pucSPI_TxBuffor[1] = (unsigned char)(uiData & 0x00ff);		// Low byte second
	
	sTransactionparams.ucNrOfBytesForTx = 2;
	sTransactionparams.ucTxBytesOffset = 0;
	sTransactionparams.ucNrOfBytesForRx = 0;
	sTransactionparams.ucRxBytesOffset = 0;
		
	IO0CLR |= SPI_MCP4921_CS_PIN_bm;							// Slave active
	SPI_ExecuteTransaction(sTransactionparams);	
	IO0SET |= SPI_MCP4921_CS_PIN_bm;							// Slave inactive
}


void Port_MCP23S09_InitCSPin(void){	// Slave select - inactive
	// SLAVE SELECT PIN
	IO0DIR  |= SPI_MCP23S09_CS_PIN_bm;						// Set as output
	IO0SET	|= SPI_MCP23S09_CS_PIN_bm;						// Set as HIGH - slave device not active.
}

void Port_MCP23S09_Set(unsigned char ucData){	// piny jako wyjscia
	sTransactionparams.ucNrOfBytesForTx = 3;
	sTransactionparams.ucNrOfBytesForRx = 0;
	sTransactionparams.ucRxBytesOffset = 0;
	sTransactionparams.ucTxBytesOffset = 0;
	
	// WYJSCIA
	pucSPI_TxBuffor[0] = 0x40;										// zapis
	pucSPI_TxBuffor[1] = MCP23S09_DIR_REG_ADDR;		// adres
	pucSPI_TxBuffor[2] = 0;												// wyjscia
	
	IO0CLR |= SPI_MCP23S09_CS_PIN_bm;
	SPI_ExecuteTransaction(sTransactionparams);
	IO0SET |= SPI_MCP23S09_CS_PIN_bm;
	
	// DATA PORT
	pucSPI_TxBuffor[0] = 0x40;										// zapis
	pucSPI_TxBuffor[1] = MCP23S09_PORT_REG_ADDR;	// adres
	pucSPI_TxBuffor[2] = ucData;									// data
	
	IO0CLR |= SPI_MCP23S09_CS_PIN_bm;
	SPI_ExecuteTransaction(sTransactionparams);
	IO0SET |= SPI_MCP23S09_CS_PIN_bm;
}

unsigned char Port_MCP23S09_Get(void){
	char cTemp;
	sTransactionparams.ucNrOfBytesForTx = 3;
	sTransactionparams.ucNrOfBytesForRx = 0;
	sTransactionparams.ucRxBytesOffset = 0;
	sTransactionparams.ucTxBytesOffset = 0;
	
	// WYJSCIA
	pucSPI_TxBuffor[0] = 0x40;										// zapis
	pucSPI_TxBuffor[1] = MCP23S09_DIR_REG_ADDR;		// adres
	pucSPI_TxBuffor[2] = 0xff;										// WEJSCIA
	
	IO0CLR |= SPI_MCP23S09_CS_PIN_bm;
	SPI_ExecuteTransaction(sTransactionparams);
	IO0SET |= SPI_MCP23S09_CS_PIN_bm;
	
	//for( cTemp = 255; cTemp != 0; cTemp--){};
	
	sTransactionparams.ucNrOfBytesForTx = 2;
	sTransactionparams.ucNrOfBytesForRx = 1;
	sTransactionparams.ucRxBytesOffset = 2;
	sTransactionparams.ucTxBytesOffset = 0;
	
	pucSPI_TxBuffor[0] = 0x41;										// odczyt
	pucSPI_TxBuffor[1] = MCP23S09_PORT_REG_ADDR;	// PORT adress
	
	IO0CLR |= SPI_MCP23S09_CS_PIN_bm;
	SPI_ExecuteTransaction(sTransactionparams);
	IO0SET |= SPI_MCP23S09_CS_PIN_bm;
	
	return pucSPI_RxBuffor[0];
}

