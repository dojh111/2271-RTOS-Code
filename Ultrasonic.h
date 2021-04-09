#include "MKL25Z4.h"                    // Device header

#define PORTD6 6				 								// Ultrasonic receive port - Pulled to high when receive return signal
#define PORTD7 7												// Ultrasonic transmit port - Set output high 10us to start ranging

#define MASK(x)					(1 << (x))

#define CLOCK24MHZCOUNT 119
#define TIMEOUT 10000										// Timeout after 50ms

#define SPEED_OF_SOUND 0.034						// In centimeters per second

volatile int elapsedTime = 0;						// Multiply by 5 to get time in us
volatile int ultrasonicMode = 0;				// Mode 0 = Transmit, 1 = Echo
volatile float distance = 0;

/*----------------------------------------------------------------------------
 * Interrupt Handlers
 *---------------------------------------------------------------------------*/
 
// Interrupt called every 5us
void PIT_IRQHandler()
{
	elapsedTime++;
	
	// Finish transmit - 10us has elapsed
	if ((ultrasonicMode == 0) && (elapsedTime == 2))
	{
		// Stop transmit
		PTD->PCOR |= MASK(PORTD7);
		// Clear elapsed time
		elapsedTime = 0;
		// Disable PIT Timer
		PIT->MCR |= PIT_MCR_MDIS_MASK;
	}
	// PIT Timeout - 50us
	else if ((ultrasonicMode == 1) && (elapsedTime >= TIMEOUT))
	{
			ultrasonicMode = 0;
			// Disable PIT Timer
			PIT->MCR |= PIT_MCR_MDIS_MASK;
			elapsedTime = 0;
	}
	
	// Clear Interrupt flag
	PIT_TFLG0 |= PIT_TFLG_TIF_MASK;
}


// Interrupt handler when receiving ultrasonic signal
void PORTD_IRQHandler()
{
	// Check if interrupt triggered by ultrasonic
	if ((PORTD->ISFR & MASK(PORTD6)))
	{
		// On receive rising edge
		if (ultrasonicMode == 0)
		{
			ultrasonicMode = 1;
			elapsedTime = 0;
			// Restart PIT timer
			PIT->MCR &= ~PIT_MCR_MDIS_MASK;
			PIT_TCTRL0 |= PIT_TCTRL_TEN_MASK;
		}
		// On dropping edge - Times out after 38ms
		else if (ultrasonicMode == 1)
		{
			ultrasonicMode = 0;
			// Disable PIT Timer
			PIT->MCR |= PIT_MCR_MDIS_MASK;
			// Calculate distance
			distance = (SPEED_OF_SOUND * (elapsedTime * 5)) / 2;
			elapsedTime = 0;
		}
	}
	
	// Clear Interrupt Flag
	PORTD->ISFR |= MASK(PORTD6);
}

/*----------------------------------------------------------------------------
 *---------------------------------------------------------------------------*/

void InitPIT()
{
	//Enable power to PIT module
	SIM->SCGC6 |= SIM_SCGC6_PIT_MASK;
	
	//Turn on PIT
	PIT->MCR &= ~PIT_MCR_MDIS_MASK;
	
	//Timer 0
	PIT_LDVAL0 = CLOCK24MHZCOUNT; // Load in start count value to produce 5us PIT
	PIT_TCTRL0 |= PIT_TCTRL_TIE_MASK; // Enable Timer 0 Interrupt 
	PIT_TFLG0 |= PIT_TFLG_TIF_MASK; // Clear Interrupt flag
	
	// Enable Interrupts for PIT
	NVIC_SetPriority(PIT_IRQn, 2);
	NVIC_ClearPendingIRQ(PIT_IRQn);
	NVIC_EnableIRQ(PIT_IRQn);
	
}

void InitUltrasonic()
{
	//Enable Clock to Port D
	SIM->SCGC5 |= SIM_SCGC5_PORTD_MASK;
	
	//Enable GPIO on both pins and setup pull-down resistors and interrupts on both edge of pin connected to ultrasonic receive
	PORTD->PCR[PORTD6] &= ~PORT_PCR_MUX_MASK;
	PORTD->PCR[PORTD6] |= (PORT_PCR_MUX(1) | PORT_PCR_PE_MASK | PORT_PCR_IRQC(0x0b));
	PORTD->PCR[PORTD6] &= ~PORT_PCR_PS_MASK; // Clear Pull select to 0 - Internal pulldown resistor
	
	PORTD->PCR[PORTD7] &= ~PORT_PCR_MUX_MASK;
	PORTD->PCR[PORTD7] |= PORT_PCR_MUX(1);
	
	//Set as input and output pins
	PTD->PDDR &= ~MASK(PORTD6);
	PTD->PDDR |= MASK(PORTD7);
	
	//Clear both pins to 0
	PTD->PCOR |= MASK(PORTD6);
	PTD->PCOR |= MASK(PORTD7);
	
	//Enable PORTD Interrupt
	NVIC_SetPriority(PORTD_IRQn, 1);
	NVIC_ClearPendingIRQ(PORTD_IRQn);
	NVIC_EnableIRQ(PORTD_IRQn);
	
}

//Starts the process of determining range
void startRanging()
{
	//Reset
	ultrasonicMode = 0;
	elapsedTime = 0;
	
	PIT->MCR &= ~PIT_MCR_MDIS_MASK;
	//Start transmission
	PTD->PSOR |= MASK(PORTD7);
	//Start PIT Timer
	PIT_TCTRL0 |= PIT_TCTRL_TEN_MASK;
}
