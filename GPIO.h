//This header file initialises GPIO functionality to ports, and hosts the LED control library

#include "MKL25Z4.h"                    // Device header

#define RED_LED 18 // PortB Pin 18
#define GREEN_LED 19 // PortB Pin 19
#define BLUE_LED 1 // PortD Pin 1
#define MASK(x) (1 << (x))

// LED colors
typedef enum {
	RED,
	GREEN,
	BLUE,
} color_t;

// LED status
typedef enum {
	ON,
	OFF
} status_t;

// Initialise GPIO
void InitGPIO(void)
{
	// Enable Clock to PORTB and PORTD
	SIM->SCGC5 |= ((SIM_SCGC5_PORTB_MASK) | (SIM_SCGC5_PORTD_MASK));
	
	// Configure MUX settings to make all 3 pins GPIO
	PORTB->PCR[RED_LED] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[RED_LED] |= PORT_PCR_MUX(1);
	
	PORTB->PCR[GREEN_LED] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[GREEN_LED] |= PORT_PCR_MUX(1);
	
	PORTD->PCR[BLUE_LED] &= ~PORT_PCR_MUX_MASK;
	PORTD->PCR[BLUE_LED] |= PORT_PCR_MUX(1);
	
	// Set Data Direction Registers for PortB and PortD
	PTB->PDDR |= (MASK(RED_LED) | MASK(GREEN_LED));
	PTD->PDDR |= MASK(BLUE_LED);
}

// Turn off all LED at start
void offLED()
{
		PTB->PSOR |= MASK(RED_LED);
		PTD->PSOR |= MASK(BLUE_LED);
		PTB->PSOR |= MASK(GREEN_LED);
}

// Controls on board LED color
// PSOR = Port Set Output Register: Writing 1 will set PDORn to logic 1
// PCOR = Port Clear Ouput Register: Writing 1 will set PDORn to logic 0
void led_control(color_t color, status_t status)
{
	switch (color)
	{
		case RED:
			if (status == ON) 
			{
				PTB->PCOR |= MASK(RED_LED);
			}
			else if (status == OFF)
			{
				PTB->PSOR |= MASK(RED_LED);
			}
			break;
			
		case GREEN:
			if (status == ON) 
			{
				PTB->PCOR |= MASK(GREEN_LED);
			}
			else if (status == OFF)
			{
				PTB->PSOR |= MASK(GREEN_LED);
			}
			break;
			
		case BLUE:
			if (status == ON) 
			{
				PTD->PCOR |= MASK(BLUE_LED);
			}
			else if (status == OFF)
			{
				PTD->PSOR |= MASK(BLUE_LED);
			}
			break;
	}
}
