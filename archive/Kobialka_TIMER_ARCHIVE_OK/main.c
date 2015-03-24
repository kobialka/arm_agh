/* 	
		Program: TIMERY
		Autor: Michal Kobialka
*/

#include <LPC21xx.H>
#include "timer.h"
#include "led.h"





int main(void)
{
	InitTimer0Match0(1000000);
	Led_Init();
	
	while(1)
	{
		Led_StepLeft();
		WaitOnTimer0Match0();
	}
}


