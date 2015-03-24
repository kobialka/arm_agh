/*  timer_interrupts.h  */

void Timer0Interrupts_Init(unsigned int, void (*tFunctionPointer)(void));
void Timer1Interrupts_Init(unsigned int, void (*tFunctionPointer)(void));

