#include "MKL25Z4.h"                    // Device header

void InitPIT()
{
	//Enable power to PIT module
	SIM->SCGC6 |= SIM_SCGC6_PIT_MASK;
	
	//Turn on PIT
	PIT->MCR &= ~PIT_MCR_MDIS_MASK;
	
	//Timer 0
	PIT_LDVAL0 = 239; // Load in start count value, assumes 48Mhz clock to produce 5us pulse
	PIT_TCTRL0 |= PIT_TCTRL_TIE_MASK; // Enable Timer 0 Interrupt 
	PIT_TFLG0 |= PIT_TFLG_TIF_MASK; // Clear Interrupt flag
	
	// Enable Interrupts for PIT
	NVIC_SetPriority(PIT_IRQn, 2);
	NVIC_ClearPendingIRQ(PIT_IRQn);
	NVIC_EnableIRQ(PIT_IRQn);
	
	//Start PIT Timer
	PIT_TCTRL0 |= PIT_TCTRL_TEN_MASK;
}
