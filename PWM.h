#include "RTE_Components.h"
#include  CMSIS_device_header
#include "cmsis_os2.h"

#include "MKL25Z4.h"                    // Device header
#include "Delay.h"
#include "MusicalNotes.h"

#define PTB0_Pin 0 // TPM1_CH0
#define PTB1_Pin 1 // TPM1_CH1

#define PTB2_Pin 2 // TPM2_CH0
#define PTB3_Pin 3 // TPM2_CH1

#define PTE30_Pin 30 // TPM0_CH3 - Speaker

// Default set as 48MHz - Change accordingly
#define CLOCK_FREQUENCY 48000000
#define PRESCALAR 128

// Initialise PWM
void InitPWM(void)
{
	// Enable power to the PORTB sub unit
	SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK;
	// Enable power to the PORTE sub unit
	SIM_SCGC5 |= SIM_SCGC5_PORTE_MASK;
	
	// Enable PWM ability on pins by setting through multiplexer - All are Alt3 (Refer to datasheet multiplexing)
	PORTB->PCR[PTB0_Pin] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[PTB0_Pin] |= PORT_PCR_MUX(3);
	
	PORTB->PCR[PTB1_Pin] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[PTB1_Pin] |= PORT_PCR_MUX(3);
	
	PORTB->PCR[PTB2_Pin] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[PTB2_Pin] |= PORT_PCR_MUX(3);
	
	PORTB->PCR[PTB3_Pin] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[PTB3_Pin] |= PORT_PCR_MUX(3);
	
	// Enable PWM ability to pin PTE30
	PORTE->PCR[PTE30_Pin] &= ~PORT_PCR_MUX_MASK;
	PORTE->PCR[PTE30_Pin] |= PORT_PCR_MUX(3);
	
	// Enable clock gate for TPM0, TPM1 and TPM2 module
	SIM->SCGC6 |= SIM_SCGC6_TPM0_MASK;
	SIM->SCGC6 |= SIM_SCGC6_TPM1_MASK;
	SIM->SCGC6 |= SIM_SCGC6_TPM2_MASK;
	
	//Selecting clock (TPM source)
	SIM->SOPT2 &= ~SIM_SOPT2_TPMSRC_MASK;
	SIM->SOPT2 |= SIM_SOPT2_TPMSRC(1);
	
	//Setting of prescalar and selecting internal clock again
	TPM0->SC &= ~((TPM_SC_CMOD_MASK) | (TPM_SC_PS_MASK));
	TPM0->SC |= (TPM_SC_CMOD(1)	| TPM_SC_PS(7));
	TPM0->SC &= ~(TPM_SC_CPWMS_MASK);
	
	TPM1->SC &= ~((TPM_SC_CMOD_MASK) | (TPM_SC_PS_MASK));
	TPM1->SC |= (TPM_SC_CMOD(1)	| TPM_SC_PS(7));
	TPM1->SC &= ~(TPM_SC_CPWMS_MASK);
	
	TPM2->SC &= ~((TPM_SC_CMOD_MASK) | (TPM_SC_PS_MASK));
	TPM2->SC |= (TPM_SC_CMOD(1)	| TPM_SC_PS(7));
	TPM2->SC &= ~(TPM_SC_CPWMS_MASK);
	
	//First line to clear, set as edge-aligned PWM (1010), count-up mode only, for Timer 0 channel 3,1 & 2 channels 1 & 2
	TPM0_C3SC &= ~((TPM_CnSC_ELSB_MASK) | (TPM_CnSC_ELSA_MASK) | (TPM_CnSC_MSB_MASK) | (TPM_CnSC_MSA_MASK));
	TPM0_C3SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1));
	
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

/*----------------------------------------------------------------------------
 * Songs
 *---------------------------------------------------------------------------*/
// Plays musical notes according to delay
void playBlindingLights(int currentAudio)
{
	for (int i = 0; i < BLINDING_LIGHTS_NOTE_COUNT; i++)
	{
		if (currentAudio != 31) {break;}
			
		int modValue = calculateMODValue(blindingLights[i]);
		TPM0->MOD = modValue;
		TPM0_C3V = modValue * 0.2;
		osDelay(blindingLightsTempo[i]);
	}
}

// Plays musical notes according to delay
void playTakeOnMe(int currentAudio)
{
	for (int i = 0; i < TAKE_ON_ME_NOTE_COUNT; i++)
	{
		if (currentAudio != 32) {break;}
		
		int modValue = calculateMODValue(takeOnMe[i]);
		TPM0->MOD = modValue;
		TPM0_C3V = modValue * 0.2;
		osDelay(takeOnMeTempo[i]);
	}
}

// Play bluetooth connect tone
void playNokiaTheme(int currentAudio)
{
	for (int i = 0; i < NOKIA_THEME_NOTE_COUNT; i++)
	{
		if (currentAudio != 33) {break;}
		
		int modValue = calculateMODValue(nokiaTheme[i]);
		TPM0->MOD = modValue;
		TPM0_C3V = modValue * 0.2;
		osDelay(nokiaThemeTempo[i]);
	}
}

void playScale()
{
	for (int i = 0; i < SCALE_NOTE_COUNT; i++)
	{
		int modValue = calculateMODValue(scale[i]);
		TPM0->MOD = modValue;
		TPM0_C3V = modValue * 0.2;
		osDelay(QUAVER);
	}
}

// Stops all sound
void soundOff()
{
	TPM0_C3V = 0;
}
