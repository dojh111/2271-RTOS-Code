#include "MKL25Z4.h"                    // Device header
#include "Delay.h"

#define PTB0_Pin 0 // TPM1_CH0
#define PTB1_Pin 1 // TPM1_CH1

#define PTB2_Pin 2 // TPM2_CH0
#define PTB3_Pin 3 // TPM2_CH1

// Default set as 48MHz - Change accordingly
#define CLOCK_FREQUENCY 48000000
#define PRESCALAR 128

// Musical note target frequencies
int musicalNotes[] = {131, 147, 165, 175, 196, 220, 247, 262, 294, 330, 349, 392, 440, 494, 523};

// Initialise PWM
void initPWM(void)
{
	// Enable power to the PORTB sub unit
	SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK;
	
	// Enable PWM ability on pins by setting through multiplexer - All are Alt3 (Refer to datasheet multiplexing)
	PORTB->PCR[PTB0_Pin] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[PTB0_Pin] |= PORT_PCR_MUX(3);
	
	PORTB->PCR[PTB1_Pin] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[PTB1_Pin] |= PORT_PCR_MUX(3);
	
	PORTB->PCR[PTB2_Pin] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[PTB2_Pin] |= PORT_PCR_MUX(3);
	
	PORTB->PCR[PTB3_Pin] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[PTB3_Pin] |= PORT_PCR_MUX(3);
	
	// Enable clock gate for TPM1 and TPM2 module
	SIM->SCGC6 |= SIM_SCGC6_TPM1_MASK;
	SIM->SCGC6 |= SIM_SCGC6_TPM2_MASK;
	
	//Selecting clock (TPM source)
	SIM->SOPT2 &= ~SIM_SOPT2_TPMSRC_MASK;
	SIM->SOPT2 |= SIM_SOPT2_TPMSRC(1);
	
	//Setting of prescalar and selecting internal clock again
	TPM1->SC &= ~((TPM_SC_CMOD_MASK) | (TPM_SC_PS_MASK));
	TPM1->SC |= (TPM_SC_CMOD(1)	| TPM_SC_PS(7));
	TPM1->SC &= ~(TPM_SC_CPWMS_MASK);
	
	TPM2->SC &= ~((TPM_SC_CMOD_MASK) | (TPM_SC_PS_MASK));
	TPM2->SC |= (TPM_SC_CMOD(1)	| TPM_SC_PS(7));
	TPM2->SC &= ~(TPM_SC_CPWMS_MASK);
	
	//First line to clear, set as edge-aligned PWM (1010), count-up mode only, for Timer 1 & 2 channels 1 & 2
	TPM1_C0SC &= ~((TPM_CnSC_ELSB_MASK) | (TPM_CnSC_ELSA_MASK) | (TPM_CnSC_MSB_MASK) | (TPM_CnSC_MSA_MASK));
	TPM1_C0SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1));
	
	TPM1_C1SC &= ~((TPM_CnSC_ELSB_MASK) | (TPM_CnSC_ELSA_MASK) | (TPM_CnSC_MSB_MASK) | (TPM_CnSC_MSA_MASK));
	TPM1_C1SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1));
	
	TPM2_C0SC &= ~((TPM_CnSC_ELSB_MASK) | (TPM_CnSC_ELSA_MASK) | (TPM_CnSC_MSB_MASK) | (TPM_CnSC_MSA_MASK));
	TPM2_C0SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1));
	
	TPM2_C1SC &= ~((TPM_CnSC_ELSB_MASK) | (TPM_CnSC_ELSA_MASK) | (TPM_CnSC_MSB_MASK) | (TPM_CnSC_MSA_MASK));
	TPM2_C1SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1));
}

// Outputs the MOD value to acheive desired PWM frequency
int calculateMODValue(int targetFrequency)
{
	return (CLOCK_FREQUENCY / PRESCALAR) / targetFrequency;
}

// Plays musical notes according to delay
void playMusicalNotes()
{
	for (int i = 0; i < 15; i++)
	{
		int modValue = calculateMODValue(musicalNotes[i]);
		TPM1->MOD = modValue;
		TPM1_C0V = modValue / 2; //Channel 0
		delay(400);
	}
}

// TEST FUNCTION. Generates 50Hz 50% duty cycle waves for PTB0-3
void generateTestPWMSignal()
{
	// The MOD value will determine the frequency of the PWM signal - (CLOCK_FREQ / Prescalar) / (Wanted Frequency) = MOD value
	// Set duty cycle: Percentage of MOD value
	int modValue1 = calculateMODValue(musicalNotes[12]);
	int dutyCycle1 = modValue1 / 2;
	
	int modValue2 = calculateMODValue(musicalNotes[13]);
	int dutyCycle2 = modValue2 / 2;

	TPM1->MOD = modValue1;
	TPM1_C0V = dutyCycle1;
	TPM1_C1V = dutyCycle1;
	
	TPM2->MOD = modValue2;
	TPM2_C0V = dutyCycle2;
	TPM2_C1V = dutyCycle2;
}
