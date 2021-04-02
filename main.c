/*----------------------------------------------------------------------------
 * CMSIS-RTOS 'main' function template
 *---------------------------------------------------------------------------*/

#ifdef RTE_Components
#include "RTE_Components.h"
#include  CMSIS_device_header
#include "cmsis_os2.h"
#endif

#include "MKL25Z4.h"                    // Device header

#ifdef Delay
#include "Delay.h"
#endif
#define MASK(x) (1 << (x))

#include "GPIO.h"
#include "UART.h"
#include "PWM.h"
#include "LED.h"

#define BAUD_RATE 9600

const osThreadAttr_t thread_attr = {
	.priority = osPriorityNormal1
};

osMutexId_t myMutex;

/*----------------------------------------------------------------------------
 * Global Variables
 *---------------------------------------------------------------------------*/
volatile uint8_t UARTCommand = 0;							// Updated through UART interrupt
volatile int motorSelection = 0;
volatile int LEDMode = 20;
volatile int audioMode = 30;

/*----------------------------------------------------------------------------
 * Interrupt Handlers
 *---------------------------------------------------------------------------*/
void UART2_IRQHandler()
{
	// Read data from incoming UART port
	if (UART2->S1 & UART_S1_RDRF_MASK)
	{
		UARTCommand = UART2->D;
	}
	
	// Error cases
	if (UART2->S1 & (UART_S1_OR_MASK | UART_S1_NF_MASK | UART_S1_FE_MASK | UART_S1_PF_MASK))
	{
	}
}

/*----------------------------------------------------------------------------
 * Motor control thread - tMotor
 *---------------------------------------------------------------------------*/
void tMotor(void *arguments)
{
	for (;;)
	{
		int modvalue = calculateMODValue(50);
		int dutyCycle = modvalue;
		
		// Set PWM Mod values for TPM1 and TPM2 (Motors)
		TPM1->MOD = modvalue;
		TPM2->MOD = modvalue;
		
		switch (motorSelection)
		{
			case 0:
				//Stop
				TPM1_C0V = 0;
				TPM1_C1V = 0;
		
				TPM2_C0V = 0;
				TPM2_C1V = 0;
				break;
			case 1:
				//Forward
				TPM1_C0V = dutyCycle;
				TPM1_C1V = 0;
			
				TPM2_C0V = dutyCycle;
				TPM2_C1V = 0;
				break;
			case 2:
				//Reverse
				TPM1_C0V = 0;
				TPM1_C1V = dutyCycle;
			
				TPM2_C0V = 0;
				TPM2_C1V = dutyCycle;
				break;
			case 3:
				//Pivot Right
				TPM2_C0V = dutyCycle;
				TPM2_C1V = dutyCycle;
		
				TPM1_C0V = 0;
				TPM1_C1V = 0;
				break;
			case 4:
				//Pivot Left
				TPM1_C0V = dutyCycle;
				TPM1_C1V = dutyCycle;
		
				TPM2_C0V = 0;
				TPM2_C1V = 0;
				break;
			case 5:
				//Moving Right
				TPM1_C0V = dutyCycle / 2;
				TPM1_C1V = 0;
			
				TPM2_C0V = dutyCycle;
				TPM2_C1V = 0;
				break;
			case 6:
				//Moving Left
				TPM1_C0V = dutyCycle;
				TPM1_C1V = 0;
			
				TPM2_C0V = dutyCycle / 2;
				TPM2_C1V = 0;
				break;
		}
	}
}

/*----------------------------------------------------------------------------
 * LED Control Thread - tLED
 *---------------------------------------------------------------------------*/
// Control current LED pattern to be displayed on robot
void tLED(void *arguments)
{
	for (;;)
	{
		switch (LEDMode)
		{
			// Default Mode - All LEDs are off
			case 20:
				offLEDgreen();
				offLEDred();
				break;
			// Connection to Bluetooth Successful - Front green LED flashes twice	
			case 21:
				for(int i=0;i<2;i++)
				{
					onLEDgreen();
					osDelay(500);
					offLEDgreen();
					osDelay(250);
				}
			break;
			// Running Mode - Front green LED Running mode (1 LED at a time) + RED flashing 500ms ON/OFF
			case 22:
				while(1)
				{
					greenLEDMoving(counterLED);
					onLEDred();
					osDelay(500);
					if (LEDMode != 20)
						break;
					offLEDred();
					osDelay(500);
					counterLED++;
					if (LEDMode != 20)
						break;
				}
				break;
			// Stationary Mode - Front green LED all lit up + RED flashing 250ms ON/OFF
			case 23:
				while(1)
				{
					onLEDgreen();
					onLEDred();
					osDelay(250);
					if (LEDMode != 21)
						break;
					offLEDred();
					osDelay(250);
					if (LEDMode != 21)
						break;
				}
				break;
		}
	}
}

/*----------------------------------------------------------------------------
 * LED Control Thread - tAudio
 *---------------------------------------------------------------------------*/
void tAudio(void *arguments)
{
	for (;;)
	{
		switch (audioMode)
		{
			// Default Song
			case 30:
				playBlindingLights();
				break;
			// End Song
			case 31:
				playTakeOnMe();
				break;
		}
	}
}

/*----------------------------------------------------------------------------
 * Application main thread - tBrain
 *---------------------------------------------------------------------------*/
void tBrain(void *arguments) 
{
	//Receive UART instructions via interrupt, set global variable as interrupt instruction
	//tBrain will sort number, and set global variables accordingly - if number == 10, set DIRECTION = 10, DIRECTION = global variable
	//Should have highest priority as have to process UART data as it appears --> Else loss of data
	for (;;)
	{
		if (UARTCommand < 20)
		{
			// Update LED pattern when not moving
			if (UARTCommand == 0) 
			{
				LEDMode = 21;
			}
			else
			{
				LEDMode = 20;
			}
			motorSelection = UARTCommand;
		}
		else if (UARTCommand < 30)
		{
			LEDMode = UARTCommand;
		}
		else if (UARTCommand < 40)
		{
			audioMode = UARTCommand;
		}
	}
}

/*----------------------------------------------------------------------------
 * TEST & DEBUGGING CODE - TO REMOVE
 *---------------------------------------------------------------------------*/
// Flash on board LED between red and green
void toggleLED(void *argument)
{
	for (;;)
	{
		UARTCommand = 20;
		osDelay(5000);
		UARTCommand = 21;
		osDelay(5000);
	}
}

void toggleAudio(void *argument)
{
	for (;;)
	{
		UARTCommand = 30;
		osDelay(12000);
		UARTCommand = 31;
		osDelay(12000);
	}
}

// Repeatedly drive motors in all directions to move motor
void toggleMOTOR(void *argument)
{
	for (;;)
	{
		UARTCommand = 0;
		osDelay(500);
		UARTCommand = 1;
		osDelay(300);
		UARTCommand = 0;
		osDelay(500);
		UARTCommand = 2;
		osDelay(300);
		UARTCommand = 0;
		osDelay(500);
		UARTCommand = 3;
		osDelay(300);
		UARTCommand = 0;
		osDelay(500);
		UARTCommand = 4;
		osDelay(300);
		UARTCommand = 0;
		osDelay(500);
		UARTCommand = 5;
		osDelay(300);
		UARTCommand = 0;
		osDelay(500);
		UARTCommand = 6;
		osDelay(300);
	}
}

/*----------------------------------------------------------------------------
 * TEST CODE - TO REMOVE
 *---------------------------------------------------------------------------*/
 
int main (void) 
{
 
  // System Initialization
  SystemCoreClockUpdate();
	InitGPIO();
	InitGPIOgreen();
	InitGPIOred();
	InitUART2(BAUD_RATE);
	InitPWM();
	
	// Set all RGB values to 1, to turn off
	offLED();
 
  osKernelInitialize();                 // Initialize CMSIS-RTOS
	myMutex = osMutexNew(NULL);
	
	//Create threads
	osThreadNew(tBrain, NULL, NULL);
	osThreadNew(tMotor, NULL, NULL);
	osThreadNew(tLED, NULL, NULL);
	osThreadNew(tAudio, NULL, NULL);
	
	// Temp Threads - For testing purposes
	osThreadNew(toggleLED, NULL, NULL);
	osThreadNew(toggleAudio, NULL, NULL);
	//osThreadNew(toggleMOTOR, NULL, NULL);
	
  osKernelStart();                      // Start thread execution
	
  for (;;) {}
}
