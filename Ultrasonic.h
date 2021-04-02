#include "MKL25Z4.h"                    // Device header

#define CLOCK_FREQ 48000000
#define PERIOD 0.000005                 // 5 Microseconds (5us)

//Returns the count to enable a periodic interrupt according to set time interval
uint32_t calculatePITCount()
{
	//int value = (PERIOD / (1 / CLOCK_FREQ)) - 1;
	return 239;
}

void InitPIT()
{
	//Enable power to PIT module
	SIM->SCGC6 |= SIM_SCGC6_PIT_MASK;
	
	//Turn on PIT
	PIT->MCR &= ~PIT_MCR_MDIS_MASK;
	
	//Timer 0
	PIT_LDVAL0 = 239; // Load in start count value
	PIT_TCTRL0 |= PIT_TCTRL_TIE_MASK; // Enable Timer 0 Interrupt 
	PIT_TFLG0 |= PIT_TFLG_TIF_MASK; // Clear Interrup flag
	
	// Enable Interrupts for PIT
	NVIC_SetPriority(PIT_IRQn, 1);
	NVIC_ClearPendingIRQ(PIT_IRQn);
	NVIC_EnableIRQ(PIT_IRQn);
}
