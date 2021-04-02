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
#define FIRST_LED_R				1			// PortA Pin 1			
#define SECOND_LED_R			2			// PortA Pin 2
#define THIRD_LED_R				4			// PortD Pin 4
#define FOURTH_LED_R			12		// PortA Pin 12
#define FIFTH_LED_R				4			// PortA Pin 4
#define SIXTH_LED_R				5			// PortA Pin 5
#define SEVENTH_LED_R			13		// PortA Pin 13
#define EIGHTH_LED_R			5			// PortD Pin 5
#define NINTH_LED_R				0			// PortD Pin 0
#define TENTH_LED_R				17		// PortA Pin 17

#define MASK(x)					(1 << (x))

unsigned int counterLED = 0;

void InitGPIOred(void)
{
	// Enabling clock to PORT A and PORT D
	SIM->SCGC5 |= (SIM_SCGC5_PORTA_MASK | SIM_SCGC5_PORTD_MASK);
	
	// Configure MUX settings to make all 10 pins GPIO
	PORTA->PCR[FIRST_LED_R] &= ~PORT_PCR_MUX_MASK;
	PORTA->PCR[FIRST_LED_R] |= PORT_PCR_MUX(1);
	
	PORTA->PCR[SECOND_LED_R] &= ~PORT_PCR_MUX_MASK;
	PORTA->PCR[SECOND_LED_R] |= PORT_PCR_MUX(1);
	
	PORTD->PCR[THIRD_LED_R] &= ~PORT_PCR_MUX_MASK;
	PORTD->PCR[THIRD_LED_R] |= PORT_PCR_MUX(1);
	
	PORTA->PCR[FOURTH_LED_R] &= ~PORT_PCR_MUX_MASK;
	PORTA->PCR[FOURTH_LED_R] |= PORT_PCR_MUX(1);
	
	PORTA->PCR[FIFTH_LED_R] &= ~PORT_PCR_MUX_MASK;
	PORTA->PCR[FIFTH_LED_R] |= PORT_PCR_MUX(1);
	
	PORTA->PCR[SIXTH_LED_R] &= ~PORT_PCR_MUX_MASK;
	PORTA->PCR[SIXTH_LED_R] |= PORT_PCR_MUX(1);
	
	PORTA->PCR[SEVENTH_LED_R] &= ~PORT_PCR_MUX_MASK;
	PORTA->PCR[SEVENTH_LED_R] |= PORT_PCR_MUX(1);
	
	PORTD->PCR[EIGHTH_LED_R] &= ~PORT_PCR_MUX_MASK;
	PORTD->PCR[EIGHTH_LED_R] |= PORT_PCR_MUX(1);
	
	PORTD->PCR[NINTH_LED_R] &= ~PORT_PCR_MUX_MASK;
	PORTD->PCR[NINTH_LED_R] |= PORT_PCR_MUX(1);
	
	PORTA->PCR[TENTH_LED_R] &= ~PORT_PCR_MUX_MASK;
	PORTA->PCR[TENTH_LED_R] |= PORT_PCR_MUX(1);
	
	// Set Data Direction Registers for PortA and PortD
	PTA->PDDR |= (MASK(FIRST_LED_R) | MASK(SECOND_LED_R) | MASK(FOURTH_LED_R) | MASK(FIFTH_LED_R) | MASK(SIXTH_LED_R) | MASK(SEVENTH_LED_R) | MASK(TENTH_LED_R));
	PTD->PDDR |= (MASK(THIRD_LED_R) | MASK(EIGHTH_LED_R) | MASK(NINTH_LED_R));
	
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
	PTA->PCOR = (MASK(FIRST_LED_R) | MASK(SECOND_LED_R) | MASK(FOURTH_LED_R) | MASK(FIFTH_LED_R) | MASK(SIXTH_LED_R) | MASK(SEVENTH_LED_R) | MASK(TENTH_LED_R));
	PTD->PCOR = (MASK(THIRD_LED_R) | MASK(EIGHTH_LED_R) | MASK(NINTH_LED_R));
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
	PTA->PSOR = MASK(FIRST_LED_R);
	PTA->PSOR = MASK(SECOND_LED_R);
	PTD->PSOR = MASK(THIRD_LED_R);
	PTA->PSOR = MASK(FOURTH_LED_R);
	PTA->PSOR = MASK(FIFTH_LED_R);
	PTA->PSOR = MASK(SIXTH_LED_R);
	PTA->PSOR = MASK(SEVENTH_LED_R);
	PTD->PSOR = MASK(EIGHTH_LED_R);
	PTD->PSOR = MASK(NINTH_LED_R);
	PTA->PSOR = MASK(TENTH_LED_R);
}
