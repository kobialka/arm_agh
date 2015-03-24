//#include "../inc/spi_lab.h"

#include "spi_lab.h"


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



#define SPI0_MSTR_bm (1<<5)
#define SPI_SLAVE_SELECT_PIN_bm	(1<<10)
#define SPI0_BITS_bm	 (0x0000FF00) //01 01 01 01 | 00 00 00 00 
#define SPI0_BITS_FUNCTION_SET_bm	 (0x00005500) //01 01 01 01 | 00 00 00 00 

#define SPI_CPOL_bm (1<<4)
#define SPI_CPHA_bm (1<<3)
#define SPI_SPIF_bm	(1<<7)
#define DAC_GA_bm		(1<<13)
#define DAC_SHDN_bm	(1<<12)

void DAC_MCP4921_Set(unsigned int uiVoltage){
	// 0-4096
	
	unsigned int uiDataToTx;
	
	PINSEL0 &= ~(SPI0_BITS_bm);								
	PINSEL0 &= ~(SPI0_BITS_FUNCTION_SET_bm);	
	IO0DIR  |= SPI_SLAVE_SELECT_PIN_bm;						// Set as output
	IO0SET	|= SPI_SLAVE_SELECT_PIN_bm;
	
	S0SPCR |= SPI0_MSTR_bm;										// Master mode select
	S0SPCR &= ~(SPI_CPHA_bm | SPI_CPOL_bm);		// rising edge
	S0SPCCR = 8;															// SPI rate = PCLK/SPCCR
	
	IO0CLR |= SPI_SLAVE_SELECT_PIN_bm;				// Slave select
	uiDataToTx = (uiVoltage & 0x0fff);				// limit voltage value
	uiDataToTx |= DAC_GA_bm | DAC_SHDN_bm;
	S0SPDR = (char)(uiDataToTx & 0x00ff);			// Low
	while(!(S0SPSR & SPI_SPIF_bm)){} ;				// wait
	S0SPDR = (char)(uiDataToTx >> 8);					// high
	while(!(S0SPSR & SPI_SPIF_bm)){} ;				// wait
	IO0SET |= SPI_SLAVE_SELECT_PIN_bm;				// Slave select
}

