// SPI Advanced KOBIALKA

/*  timer.c  */
#include <LPC21xx.H>
#include "timer.h"

#define COUNTER_ENABLE_bm 	(1<<0)
#define COUNTER_RESET_bm	 	(1<<1)

#define INTERRUPT_ON_MR0_bm	(1<<0)
#define RESET_ON_MR0_bm			(1<<1)

#define MR0_INTERRUPT_bm		(1<<0)
void InitTimer0(void)
{
	T0TCR |= COUNTER_ENABLE_bm;
}

void WaitOnTimer0(unsigned int uiTime)
{
	T0TCR |= COUNTER_RESET_bm;
	T0TCR &= ~COUNTER_RESET_bm;
	while(T0TC < (uiTime*15));
}

void InitTimer0Match0(unsigned int uiDelayTime)
{
	T0TCR |= COUNTER_ENABLE_bm;
	T0MCR |= RESET_ON_MR0_bm | INTERRUPT_ON_MR0_bm;
	T0MR0 = uiDelayTime*15;
}

void WaitOnTimer0Match0()
{
	while(0 == (T0IR & MR0_INTERRUPT_bm)){};
	T0IR = MR0_INTERRUPT_bm;
}



