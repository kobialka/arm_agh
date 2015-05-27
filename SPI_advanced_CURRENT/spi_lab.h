// SPI Advanced KOBIALKA

#ifndef _SPI_LAB_H_
#define _SPI_LAB_H_



// TYPY
typedef struct SPI_FrameParams_typedef{
			unsigned char ucCpha;					// Clock phase: 0-data is sampled on the first clock edge of SCK
			unsigned char ucCpol;					// Clock polarity: 0-positive
			unsigned char ucClsbf;				// 1-data is tx'ed LSB (bit 0) first
			unsigned char ClkDivider;			
		}SPI_FrameParams_typedef;


typedef struct SPI_TransactionParams_typedef{
		unsigned char *pucBytesForTx;			// wskaznik na tablice z bajtami do wyslania 
		unsigned char ucNrOfBytesForTx; 	// ilosc bajtów do wyslania
		unsigned char ucTxBytesOffset; 		// offset bajtów do wyslania
		unsigned char *pucBytesForRx;   	// wskaznik na tablice na odebrane bajty
		unsigned char ucNrOfBytesForRx; 	// ilosc bajtów do odebrania
		unsigned char ucRxBytesOffset; 		// offset bajtów do odebrania
	}SPI_TransactionParams_typedef;


// FUNKCJE
// ===================  SPI BASIC  ==================
void DAC_MCP4921_Set(unsigned int uiVoltage);

// ===================  SPI ADVANCED  ==================
void SPI_ConfigMaster(SPI_FrameParams_typedef);
void SPI_ExecuteTransaction(SPI_TransactionParams_typedef);
void DAC_MCP4921_InitCSPin(void);
void DAC_MCP4921_Set_Adv(unsigned int uiData);


void Port_MCP23S09_InitCSPin(void);			// Slave select - inactive
void Port_MCP23S09_Set(unsigned char);	// piny jako wyjscia
unsigned char Port_MCP23S09_Get(void);

#endif // _SPI_LAB_H_ //


