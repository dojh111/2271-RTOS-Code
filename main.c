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

#include "GPIO.h"
#include "UART.h"
#include "PWM.h"

#define BAUD_RATE 9600

/*----------------------------------------------------------------------------
 * Instruction set list
 *---------------------------------------------------------------------------*/
void chooseColor(int command)
{
	switch (command)
	{
		//LED Control Zone
		case 2:
			led_control(RED, OFF);
			break;
		case 3:
			led_control(RED, ON);
			break;
		case 4:
			led_control(GREEN, OFF);
			break;
		case 5:
			led_control(GREEN, ON);
			break;
	}
}

void driveMotor(int command)
{
	int modValue = calculateMODValue(7500);
	int dutyCycle = modValue;
	
	TPM1->MOD = modValue;
	TPM2->MOD = modValue;

	switch (command)
	{
		case 6:
			//Make motor go forward PTB0
			TPM1_C0V = dutyCycle;
			TPM1_C1V = 0;
			
			TPM2_C0V = dutyCycle;
			TPM2_C1V = 0;
			break;
		case 7:
			//Make motor go backward PTB1
			TPM1_C0V = 0;
			TPM1_C1V = dutyCycle;
			
			TPM2_C0V = 0;
			TPM2_C1V = dutyCycle;
			break;
		case 8:
			//Stop all motors
			TPM1_C0V = 0;
			TPM1_C1V = 0;
		
			TPM2_C0V = 0;
			TPM2_C1V = 0;
			break;
	}
}

void selectInstruction(int command)
{
	//LED Control Set
	if (command < 6)
	{
		chooseColor(command);
	}
	//Motor Control Set
	else if (command >= 6 && command <= 8)
	{
		driveMotor(command);
	}
}


/*----------------------------------------------------------------------------
 * Application main thread
 *---------------------------------------------------------------------------*/
void app_main(void *arguments) {

	int selection;
	
	for (;;)
	{
		//Receive UART instructions
		selection = UART2_Receive_Poll();
		selectInstruction(selection);
	}
}

/*----------------------------------------------------------------------------
 * Application sub threads
 *---------------------------------------------------------------------------*/
void led_red_thread(void *argument) {
	
  for (;;) 
	{
		led_control(RED, ON);
		//osDelay(1000);
		delay(0x80000);
		led_control(RED, OFF);
		//osDelay(1000);
		delay(0x80000);
	}
}

void led_green_thread(void *argument) {
	
	for (;;) 
	{
		led_control(GREEN, ON);
		//osDelay(1000);
		delay(0x80000);
		led_control(GREEN, OFF);
		//osDelay(1000);
		delay(0x80000);
	}
}
 
int main (void) {
 
  // System Initialization
  SystemCoreClockUpdate();
	InitGPIO();
	InitUART2(BAUD_RATE);
	
	// Set all RGB values to 1, to turn off
	offLED();
 
  osKernelInitialize();                 // Initialize CMSIS-RTOS
	
	//Create threads
	osThreadNew(app_main, NULL, NULL);
	//osThreadNew(led_red_thread, NULL, NULL);
	//osThreadNew(led_green_thread, NULL, NULL);
	
  osKernelStart();                      // Start thread execution
	
  for (;;) {}
}
