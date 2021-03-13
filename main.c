/*----------------------------------------------------------------------------
 * CMSIS-RTOS 'main' function template
 *---------------------------------------------------------------------------*/
 
#include "RTE_Components.h"
#include  CMSIS_device_header
#include "cmsis_os2.h"

#include "MKL25Z4.h"                    // Device header

#ifdef Delay
#include "Delay.h"
#endif
#define MASK(x) (1 << (x))

#include "GPIO.h"
#include "UART.h"
#include "PWM.h"

#define BAUD_RATE 9600
#define UART_RECEIVE_PTD2 2

const osThreadAttr_t thread_attr = {
	.priority = osPriorityNormal1
};

osMutexId_t myMutex;

//#define MOTOR_MODVALUE 50								// Motor Driver Mod Value

/*----------------------------------------------------------------------------
 * Global Variables
 *---------------------------------------------------------------------------*/
volatile int UARTCommand = 0;							// Updated through UART interrupt
volatile int motorSelection = 0;
volatile int LEDMode = 20;
volatile int audioMode = 30;

/*----------------------------------------------------------------------------
 * Interrupt Handlers
 *---------------------------------------------------------------------------*/
void UART2_IRQHandler()
{
	// Read data from incoming UART port
	UARTCommand = UART2_Receive_Poll();
	
	// Clear INT flag
	PORTD->ISFR |= MASK(UART_RECEIVE_PTD2);
}

/*----------------------------------------------------------------------------
 * Motor control thread - tMotor
 *---------------------------------------------------------------------------*/
void tMotor(void *arguments)
{
	for (;;)
	{
		int modvalue = calculateMODValue(7500);
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
void tLED(void *arguments)
{
	for (;;)
	{
		switch (LEDMode)
		{
			//LED Control Zone
			case 20:
				led_control(RED, OFF);
				break;
			case 21:
				led_control(RED, ON);
				break;
			case 22:
				led_control(GREEN, OFF);
				break;
			case 23:
				led_control(GREEN, ON);
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
			case 30:
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
		int command = UARTCommand;
		
		if (command < 20)
		{
			motorSelection = command;
		}
		else if (command < 30)
		{
			LEDMode = command;
		}
		else if (command < 40)
		{
			audioMode = command;
		}
	}
}

/*----------------------------------------------------------------------------
 * LAB CODE - TO REMOVE
 *---------------------------------------------------------------------------*/
void led_red_thread(void *argument) {
	
	for (;;) 
	{
		osMutexAcquire(myMutex, osWaitForever);
		
		led_control(RED, ON);
		osDelay(1000);
		//delay(0x80000);
		led_control(RED, OFF);
		osDelay(1000);
		//delay(0x80000);
		
		osMutexRelease(myMutex);
	}
}

void led_green_thread(void *argument) {
	
	for (;;) 
	{
		osMutexAcquire(myMutex, osWaitForever);
		
		led_control(GREEN, ON);
		osDelay(1000);
		//delay(0x80000);
		led_control(GREEN, OFF);
		osDelay(1000);
		//delay(0x80000);
		
		osMutexRelease(myMutex);
	}
}

/*----------------------------------------------------------------------------
 * LAB CODE - TO REMOVE
 *---------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------
 * TEST CODE - TO REMOVE
 *---------------------------------------------------------------------------*/
void toggleLED(void *argument)
{
	for (;;)
	{
		osDelay(2000);
		UARTCommand = 21;
		osDelay(2000);
		UARTCommand = 20;
		osDelay(2000);
		UARTCommand = 23;
		osDelay(2000);
		UARTCommand = 22;
	}
}

void toggleMOTOR(void *argument)
{
	for (;;)
	{
		osDelay(1000);
		UARTCommand = 1;
		osDelay(1000);
		UARTCommand = 2;
		osDelay(1000);
		UARTCommand = 3;
		osDelay(1000);
		UARTCommand = 4;
		osDelay(1000);
		UARTCommand = 5;
		osDelay(1000);
		UARTCommand = 6;
	}
}

/*----------------------------------------------------------------------------
 * TEST CODE - TO REMOVE
 *---------------------------------------------------------------------------*/
 
int main (void) {
 
  // System Initialization
  SystemCoreClockUpdate();
	InitGPIO();
	InitUART2(BAUD_RATE);
	initPWM();
	
	// Set all RGB values to 1, to turn off
	offLED();
 
  osKernelInitialize();                 // Initialize CMSIS-RTOS
	myMutex = osMutexNew(NULL);
	
	//Create threads
	osThreadNew(tBrain, NULL, NULL);
	osThreadNew(tMotor, NULL, NULL);
	osThreadNew(tLED, NULL, NULL);
	osThreadNew(tAudio, NULL, NULL);
	
	osThreadNew(toggleLED, NULL, NULL);
	osThreadNew(toggleMOTOR, NULL, NULL);
	//osThreadNew(led_red_thread, NULL, NULL);
	//osThreadNew(led_green_thread, NULL, NULL);
	
  osKernelStart();                      // Start thread execution
	
  for (;;) {}
}
