#include "MKL25Z4.h"            // Device header

// Ports for GREEN LED
#define FIRST_LED_G				8			// Port B Pin 8	
#define SECOND_LED_G			9			// Port B Pin 9
#define THIRD_LED_G				10		// Port B Pin 10
#define FOURTH_LED_G			11		// Port B Pin 11
#define FIFTH_LED_G				2			// Port E Pin 2
#define SIXTH_LED_G				3			// Port E Pin 3
#define SEVENTH_LED_G			4			// Port E Pin 4
#define EIGHTH_LED_G			5			// Port E Pin 5
#define NINTH_LED_G				20		// Port E Pin 20
#define TENTH_LED_G				21		// Port E Pin 21

// Ports for RED LED
#define LED_R							17		// PortA Pin 17			

#define MASK(x)					(1 << (x))

unsigned int counterLED = 0;

void InitGPIOred(void)
{
	// Enabling clock to PORT A and PORT D
	SIM->SCGC5 |= (SIM_SCGC5_PORTA_MASK);
	
	// Configure MUX settings to make the pin GPIO
	PORTA->PCR[LED_R] &= ~PORT_PCR_MUX_MASK;
	PORTA->PCR[LED_R] |= PORT_PCR_MUX(1);
	
	// Set Data Direction Registers for PortA and PortD
	PTA->PDDR |= MASK(LED_R);
	
}

void InitGPIOgreen(void)
{
	// Enabling clock to PORT B and PORT E
	SIM->SCGC5 |= (SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTE_MASK);
	
	// Configure MUX settings to make all 10 pins GPIO
	PORTB->PCR[FIRST_LED_G] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[FIRST_LED_G] |= PORT_PCR_MUX(1);
	
	PORTB->PCR[SECOND_LED_G] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[SECOND_LED_G] |= PORT_PCR_MUX(1);
	
	PORTB->PCR[THIRD_LED_G] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[THIRD_LED_G] |= PORT_PCR_MUX(1);
	
	PORTB->PCR[FOURTH_LED_G] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[FOURTH_LED_G] |= PORT_PCR_MUX(1);
	
	PORTE->PCR[FIFTH_LED_G] &= ~PORT_PCR_MUX_MASK;
	PORTE->PCR[FIFTH_LED_G] |= PORT_PCR_MUX(1);
	
	PORTE->PCR[SIXTH_LED_G] &= ~PORT_PCR_MUX_MASK;
	PORTE->PCR[SIXTH_LED_G] |= PORT_PCR_MUX(1);
	
	PORTE->PCR[SEVENTH_LED_G] &= ~PORT_PCR_MUX_MASK;
	PORTE->PCR[SEVENTH_LED_G] |= PORT_PCR_MUX(1);
	
	PORTE->PCR[EIGHTH_LED_G] &= ~PORT_PCR_MUX_MASK;
	PORTE->PCR[EIGHTH_LED_G] |= PORT_PCR_MUX(1);
	
	PORTE->PCR[NINTH_LED_G] &= ~PORT_PCR_MUX_MASK;
	PORTE->PCR[NINTH_LED_G] |= PORT_PCR_MUX(1);
	
	PORTE->PCR[TENTH_LED_G] &= ~PORT_PCR_MUX_MASK;
	PORTE->PCR[TENTH_LED_G] |= PORT_PCR_MUX(1);
	
	// Set Data Direction Registers for Port B and Port E
	PTB->PDDR |= (MASK(FIRST_LED_G) | MASK(SECOND_LED_G) | MASK(THIRD_LED_G) | MASK(FOURTH_LED_G));
	PTE->PDDR |= (MASK(FIFTH_LED_G) | MASK(SIXTH_LED_G) | MASK(SEVENTH_LED_G) | MASK(EIGHTH_LED_G) | MASK(NINTH_LED_G) | MASK(TENTH_LED_G));
	
}

void offLEDred(void)
{
	PTA->PCOR = MASK(LED_R);
}

void offLEDgreen(void)
{
	PTB->PCOR = (MASK(FIRST_LED_G) | MASK(SECOND_LED_G) | MASK(THIRD_LED_G) | MASK(FOURTH_LED_G));
	PTE->PCOR = (MASK(FIFTH_LED_G) | MASK(SIXTH_LED_G) | MASK(SEVENTH_LED_G) | MASK(EIGHTH_LED_G) | MASK(NINTH_LED_G) | MASK(TENTH_LED_G));
}

void greenLEDMoving(int counterLED)
{
	offLEDgreen();
	int number = (counterLED%10) + 1;
	switch(number)
	{
		case 1:
			PTB->PSOR = MASK(FIRST_LED_G);
			break;
		case 2:
			PTB->PSOR = MASK(SECOND_LED_G);
			break;
		case 3:
			PTB->PSOR = MASK(THIRD_LED_G);
			break;
		case 4:
			PTB->PSOR = MASK(FOURTH_LED_G);
			break;
		case 5:
			PTE->PSOR = MASK(FIFTH_LED_G);
			break;
		case 6:
			PTE->PSOR = MASK(SIXTH_LED_G);
			break;
		case 7:
			PTE->PSOR = MASK(SEVENTH_LED_G);
			break;
		case 8:
			PTE->PSOR = MASK(EIGHTH_LED_G);
			break;
		case 9:
			PTE->PSOR = MASK(NINTH_LED_G);
			break;
		case 10:
			PTE->PSOR = MASK(TENTH_LED_G);
			break;
		default:
			offLEDgreen();
	}
}

void onLEDgreen(void)
{
	PTB->PSOR = MASK(FIRST_LED_G);
	PTB->PSOR = MASK(SECOND_LED_G);
	PTB->PSOR = MASK(THIRD_LED_G);
	PTB->PSOR = MASK(FOURTH_LED_G);
	PTE->PSOR = MASK(FIFTH_LED_G);
	PTE->PSOR = MASK(SIXTH_LED_G);
	PTE->PSOR = MASK(SEVENTH_LED_G);
	PTE->PSOR = MASK(EIGHTH_LED_G);
	PTE->PSOR = MASK(NINTH_LED_G);
	PTE->PSOR = MASK(TENTH_LED_G);
}

void onLEDred(void)
{
	PTA->PSOR = MASK(LED_R);
}
