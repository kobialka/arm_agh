/*  command_decoder.c  */

#include <LPC21xx.H>
#include "command_decoder.h"
#include "string.h"



#define NULL 0



typedef enum			tTokenFind_state	{TOKEN, DELIMITER}				tTokenFind_state;


// =======================================================================================================
// zmienne globalne
unsigned char 		ucTokenNr;
tToken 						asToken[MAX_TOKEN_NR];	// tablica token�w.
tKeyword 					asKeywordList[MAX_KEYWORD_NR] = 									// asKeywordList jest tablica struktur typu keyword. Czyli maja pole kodu typu KeywordCod i pole Stringa.
{
	{CALLIB,"callib"},
	{GOTO,"goto"},
	{CALC,"calc"},
};




// =======================================================================================================
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




// =======================================================================================================
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




// =======================================================================================================
void DecodeTokens(void)
{
	unsigned char	ucTokenCounter;	
	
	tToken				*psOneToken = asToken;
 	unsigned int	*puiHexStringValue;
 	tKeywordCode	*peKeyword; 
	
	
	for(ucTokenCounter = 0; ucTokenCounter < ucTokenNr; ucTokenCounter++)
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


// =======================================================================================================
void DecodeMsg(char *pcString)
{
	ucFindTokensInString(pcString);
	ReplaceCharactersInString(pcString,' ', NULL);
	DecodeTokens();
}

