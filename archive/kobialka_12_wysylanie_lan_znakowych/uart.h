/*  uart.h  */


#define TERMINATOR		13
#define RECIEVER_SIZE	11

// TRANSMITER BUFF SIZE
#define TRANSMITER_SIZE 19

// =======================================================================================================
// typy zmiennych
typedef enum teRecieverStatus { EMPTY, READY, OVERFLOW} teRecieverStatus;
enum eTransmiterStatus {FREE, BUSY};

typedef struct tsRecieverBuffer
{
	char							cData[RECIEVER_SIZE]; 
	unsigned char			ucCharCtr;
	teRecieverStatus	eStatus;
} tsRecieverBuffer;

typedef struct TransmiterBuffer{
	char cData[TRANSMITER_SIZE]; 								// Lancuch, ktory ma byc wyslany
	enum eTransmiterStatus eStatus;							// Status bufora
	unsigned char fLastCharacter;								// Czy pobrano ostatni znak, czyli NULL'a. Flaga.
	unsigned char cCharCtr;											// Do iteracji po lancuchu znakowym.
} tsTransmiterBuffer;


// =======================================================================================================
// funkcje
void UART_InitWithInt(unsigned int uiBaudRate);
void Reciever_PutCharacterToBuffer(char cCharacter);
teRecieverStatus eReciever_GetStatus(void);
void Reciever_GetStringCopy(char * ucDestination);

char Transmiter_GetCharacterFromBuffer(void);
void Transmiter_SendString( char cString[]);
enum eTransmiterStatus Transmiter_GetStatus(void);

