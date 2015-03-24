#include "zegar.h"
#define SEC_IN_MIN	(4)
struct sWatch Watch;

void WatchUpdate(void){
	if(Watch.ucSeconds == (SEC_IN_MIN-1) ){										// 59 dec  == 0x3b hex
		Watch.ucMinutes = Watch.ucMinutes + 1;
		Watch.fMinutesValuedChanged = 1;
	}
	Watch.ucSeconds = (Watch.ucSeconds+1)%SEC_IN_MIN;
	Watch.fSecondsValueChanged = 1;
}


