/*  uart.h  */


#define TERMINATOR		13
#define RECIEVER_SIZE	11


// =======================================================================================================
// typy zmiennych
typedef enum teRecieverStatus { EMPTY, READY, OVERFLOW} teRecieverStatus;

typedef struct tsRecieverBuffer
{
	char							cData[RECIEVER_SIZE]; 
	unsigned char			ucCharCtr;
	teRecieverStatus	eStatus;
} tsRecieverBuffer;


// =======================================================================================================
// funkcje
void UART_InitWithInt(unsigned int uiBaudRate);
void Reciever_PutCharacterToBuffer(char cCharacter);
teRecieverStatus eReciever_GetStatus(void);
void Reciever_GetStringCopy(char * ucDestination);











