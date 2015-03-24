#include <LPC21xx.H>

#define TetraMask_bm 0x0000000f

#define NULL 0
#define MAX_KEYWORD_NR 3																				// maksymalna dopuszczalna ilosc slow kluczowych.
#define MAX_TOKEN_NR 3																					// maksymalna dopuszczalna ilosc tokenów.
#define MAX_KEYWORD_STRING_LTH 10																// maksymalna dlugosc komendy.

unsigned char 		ucTokenNr;

typedef enum			tResult						{OK, ERROR}								tResult;
typedef enum			tTokenFind_state	{TOKEN, DELIMITER}				tTokenFind_state;


// --------------------------------------------------------------------------------------------------------
typedef enum			tKeywordCode			{LD, ST, RST}							tKeywordCode;	

typedef struct		tKeyword
{
	tKeywordCode	eCode;
	char 					cString[MAX_KEYWORD_STRING_LTH + 1];
}tKeyword;

tKeyword 					asKeywordList[MAX_KEYWORD_NR] = 									// asKeywordList jest tablica struktur typu keyword. Czyli maja pole kodu typu KeywordCod i pole Stringa.
{
	{LD,"load"},
	{ST,"store"},
	{RST,"reset"}
};
// --------------------------------------------------------------------------------------------------------



// --------------------------------------------------------------------------------------------------------
typedef enum			tTokenType 				{KEYWORD, NUMBER, STRING}	tTokenType;			
typedef union 		tTokenValue																				
{
	tKeywordCode	eKeyword; 						// jezeli KEYWORD
	unsigned int	uiNumber; 						// jezeli NUMBER
	char *				pcString; 						// jezeli STRING
}tTokenValue;


typedef struct		tToken
{
	tTokenType 	eType;									// KEYWORD, NUMBER, STRING
	tTokenValue uValue;									// albo enum albo unsigned int albo char*
}tToken;

tToken 						asToken[MAX_TOKEN_NR];	// tablica tokenów.
// --------------------------------------------------------------------------------------------------------





// --------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------
typedef enum			tCompResult				{DIFFERENT, EQUAL}				tCompResult;

void CopyString(char pcSource[], char pcDestination[])
{	
	unsigned char ucCharacterCounter;

	for(ucCharacterCounter=0; pcSource[ucCharacterCounter] != NULL; ucCharacterCounter++)
	{
		pcDestination[ucCharacterCounter] = pcSource[ucCharacterCounter];
	}
	pcDestination[ucCharacterCounter] = NULL;
}


//---------------------------------------------------------------------------------------
tCompResult eCompareString(char pcStr1[], char pcStr2[])
{
	unsigned char ucCharacterCounter;
	
	for(ucCharacterCounter=0; (pcStr1[ucCharacterCounter] == pcStr2[ucCharacterCounter]); ucCharacterCounter++)
	{
		if(pcStr1[ucCharacterCounter] == NULL)
		{
			return EQUAL;
		}
	}
	return DIFFERENT;
}


//---------------------------------------------------------------------------------------
void AppendString (char pcSourceStr[], char pcDestinationStr[])
{
	unsigned char ucCharacterCounter;
	
	for(ucCharacterCounter=0; pcDestinationStr[ucCharacterCounter] != NULL; ucCharacterCounter++){};
	
	CopyString(pcSourceStr, pcDestinationStr+ucCharacterCounter);
}


//---------------------------------------------------------------------------------------
void ReplaceCharactersInString(char pcString[], char cOldChar, char cNewChar)
{
	unsigned char ucCharacterCounter;
	
	for(ucCharacterCounter=0; pcString[ucCharacterCounter] != NULL; ucCharacterCounter++)
	{
		if(pcString[ucCharacterCounter] == cOldChar)
		{
			pcString[ucCharacterCounter] = cNewChar;
		}
	}
}


//---------------------------------------------------------------------------------------
void UIntToHexStr (unsigned int uiValue, char pcStr[])
{
	unsigned char uiTetraCounter;
	unsigned char uiValueBuff;
	
	for (uiTetraCounter = 0; uiTetraCounter < 8; uiTetraCounter++)
	{
		uiValueBuff = ( (uiValue >> (uiTetraCounter*4)) & TetraMask_bm );
		if (uiValueBuff > 9)
		{
			pcStr[9 - uiTetraCounter] = uiValueBuff + ('A' - 10);
		}
		else
		{
			pcStr[9 - uiTetraCounter] = uiValueBuff + '0';
		}
	}
	pcStr[1] = 'x';
	pcStr[0] = '0';
}


//---------------------------------------------------------------------------------------
tResult eHexStringToUInt(char pcStr[], unsigned int *puiValue)
{
	unsigned char ucCharacterCounter; 
	unsigned char ucCharacterBuff;
	unsigned int  uiValueBuff = 0;
	
	
	if ((pcStr[0] != '0') || (pcStr[1] != 'x') || (pcStr[2] == NULL))
	{
		return ERROR;
	}
		
	for (ucCharacterCounter = 2; pcStr[ucCharacterCounter] != NULL; ucCharacterCounter++)
	{
		if (ucCharacterCounter > 5)
		{
			return ERROR;
		}
		
		uiValueBuff = uiValueBuff << 4;	
		if( (ucCharacterBuff = pcStr[ucCharacterCounter]) > ('a'-1) )  // 0x60 => '''
		{
			ucCharacterBuff = ucCharacterBuff -'a'+10;
		}
		else if( ucCharacterBuff > '@')
		{
			ucCharacterBuff = ucCharacterBuff -'A'+10;
		}
		else
		{
			ucCharacterBuff = ucCharacterBuff -'0';
		}
		uiValueBuff = uiValueBuff + ucCharacterBuff;
	}		
	*puiValue = uiValueBuff;
	return OK;
}




//---------------------------------------------------------------------------------------
void AppendUIntToString(unsigned int uiValue, char pcDestinationStr[])
{
	unsigned char ucCharacterCounter;
	
	for (ucCharacterCounter = 0; pcDestinationStr[ucCharacterCounter] != NULL; ucCharacterCounter++){};
	UIntToHexStr((uiValue), pcDestinationStr + ucCharacterCounter);
}
// --------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------






// --------------------------------------------------------------------------------------------------------
unsigned char ucFindTokensInString (char *pcMessage)
{
	unsigned char ucCharacterCounter;
	unsigned char ucCharacterBuff;

	tTokenFind_state eTokenFind_state = DELIMITER;
	ucTokenNr = 0;
	
	for(ucCharacterCounter = 0;; ucCharacterCounter++)
	{
		ucCharacterBuff = pcMessage[ucCharacterCounter];

		switch(eTokenFind_state)
		{
			case DELIMITER:
				if(ucTokenNr == MAX_TOKEN_NR)
				{
					return ucTokenNr;
				}
				else if(ucCharacterBuff == NULL)
				{
					return ucTokenNr;
				}
				else if(ucCharacterBuff != ' ')
				{
					eTokenFind_state = TOKEN;
					asToken[ucTokenNr].uValue.pcString = pcMessage + ucCharacterCounter;					
					ucTokenNr++;
				}
				else
				{
					eTokenFind_state = DELIMITER;
				}
				break;

			case TOKEN:
				if(ucCharacterBuff == NULL)
				{
					return ucTokenNr;
				}
				else if(ucCharacterBuff == ' ')
				{
					eTokenFind_state = DELIMITER;
				}
				else 
				{
					eTokenFind_state = TOKEN;
				}	
				break;
		}
	}
}




// --------------------------------------------------------------------------------------------------------
tResult  eStringToKeyword(char pcStr[], tKeywordCode *peKeyword)
{
	unsigned char ucKeywordCounter;
		             
 	for(ucKeywordCounter = 0; ucKeywordCounter < MAX_KEYWORD_NR; ucKeywordCounter++)
 	{
 		if(EQUAL == (eCompareString(pcStr, asKeywordList[ucKeywordCounter].cString)) )
 		{
 			*peKeyword = asKeywordList[ucKeywordCounter].eCode;
 			return OK;
 		}
 	}
	return ERROR;
}




// --------------------------------------------------------------------------------------------------------
void DecodeTokens(void)
{
	unsigned char	ucTokenCounter;	
	
	tToken				*psOneToken = asToken;
 	unsigned int	*puiHexStringValue;
 	tKeywordCode	*peKeyword; 
	
	
	for(ucTokenCounter = 0; ucTokenCounter < MAX_TOKEN_NR; ucTokenCounter++)
	{
 		peKeyword					= &(psOneToken->uValue.eKeyword);
 		puiHexStringValue	= &(psOneToken->uValue.uiNumber);
		
		if(OK == eStringToKeyword(psOneToken->uValue.pcString, peKeyword) )
		{
			psOneToken->eType = KEYWORD;
		}
		else if( OK == eHexStringToUInt(psOneToken->uValue.pcString, puiHexStringValue) )
		{
			psOneToken->eType = NUMBER;
		}
		else
		{
			psOneToken->eType = STRING;
		}
		psOneToken++;
	}
}

// --------------------------------------------------------------------------------------------------------
void DecodeMsg(char *pcString)
{
	ucFindTokensInString(pcString);
	ReplaceCharactersInString(pcString,' ', NULL);
	DecodeTokens();
}



// --------------------------------------------------------------------------------------------------------

int main(void)
{
	char pcMessage_main[] = "load 0 sf     reset 0xFFfA reset 0x   szesc 0x0 reset";
	
	DecodeMsg(pcMessage_main);
	
	while(1);
}
