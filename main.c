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
#include "Ultrasonic.h"

#define BAUD_RATE 9600
#define UART_NO_COMMAND 101

#define STOP_DISTANCE 25								// Robot ultrasonic sensor stop distance
#define DISTANCE_ERROR_MARGIN 20				// Margin for error to determine if robot is off course

const osThreadAttr_t thread_attr = {
	.priority = osPriorityAboveNormal
};

osSemaphoreId_t startupSem;

/*----------------------------------------------------------------------------
 * Global Variables
 *---------------------------------------------------------------------------*/
volatile uint8_t UARTCommand = UART_NO_COMMAND;							// Updated through UART interrupt
volatile int motorSelection = 0;
volatile int LEDMode = 20;
volatile int audioMode = 30;

volatile int speedSelection = 1;							// Default at high speed

/*----------------------------------------------------------------------------
 * Interrupt Handlers
 *---------------------------------------------------------------------------*/
// Interrupt to handle received bluetooth commands
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
 * Self-Driving Mode thread
 *---------------------------------------------------------------------------*/
// Code for executing turn around cone
void coneTurn()
{
	speedSelection = 1;
	
	// Pivot Left
	UARTCommand = 4;
	osDelay(325);
	UARTCommand = 0;
	osDelay(300);
	
	// Forward 1
	UARTCommand = 1;
	osDelay(275);
	UARTCommand = 0;
	osDelay(300);
	
	// Pivot Right 1
	UARTCommand = 3;
	osDelay(450);
	UARTCommand = 0;
	osDelay(300);
	
	// Forward 2
	UARTCommand = 1;
	osDelay(450);
	UARTCommand = 0;
	osDelay(300);
	
	// Pivot Right 2
	UARTCommand = 3;
	osDelay(500);
	UARTCommand = 0;
	osDelay(300);
	
	// Forward 3
	UARTCommand = 1;
	osDelay(450);
	UARTCommand = 0;
	osDelay(300);
	
	// Pivot Right
	UARTCommand = 3;
	osDelay(350);
	UARTCommand = 0;
	osDelay(300);
	
	// Forward 4
	UARTCommand = 1;
	osDelay(400);
	UARTCommand = 0;
	osDelay(300);
	
	// Pivot Left
	UARTCommand = 4;
	osDelay(260);
	UARTCommand = 0;
	osDelay(300);
	
	speedSelection = 0;
}

/*----------------------------------------------------------------------------
 * Self Driving Mode Thread - tSelfDrivingMode
 *---------------------------------------------------------------------------*/
void tSelfDrivingMode(void *arguments)
{
	// Save current speed mode
	int currentSpeed = speedSelection;
	
	// Turn on GREEN Led to signal selfDrivingMode
	led_control(GREEN, ON);
	speedSelection = 0;
	
	UARTCommand = 0;
	osDelay(300);
	
	// Initial range
	startRanging();
	osDelay(100);
	
	UARTCommand = 1;
	// Constantly range distance and move forward
	while (distance > STOP_DISTANCE)
	{
		startRanging();
		osDelay(100);
	}
	UARTCommand = 0;
	osDelay(100);
	
	// Initiate Turn Sequence
	coneTurn();
	
	// Initiate Return Sequence
	startRanging();
	osDelay(100);
	
	UARTCommand = 1;
	// Constantly range distance and move forward
	while (distance > STOP_DISTANCE)
	{
		startRanging();
		osDelay(100);
	}
	UARTCommand = 0;
	osDelay(100);

	
	// End of self-driving mode
	UARTCommand = 0;
	speedSelection = currentSpeed;
	led_control(GREEN, OFF);
}

/*----------------------------------------------------------------------------
 * Motor control thread - tMotor
 *---------------------------------------------------------------------------*/
void tMotor(void *arguments)
{
	int modvalue = calculateMODValue(50);
	int dutyCycle = 0;
		
	// Set PWM Mod values for TPM1 and TPM2 (Motors)
	TPM1->MOD = modvalue;
	TPM2->MOD = modvalue;
	
	osSemaphoreAcquire(startupSem, osWaitForever);
	
	for (;;)
	{
		// Speed Selection
		switch (speedSelection)
		{
			// Slowest
			case 0: 
				dutyCycle = modvalue * 0.2;
				break;
			// Fastest
			case 1:
				dutyCycle = modvalue;
				break;
			// Half speed
			case 2:
				dutyCycle = modvalue * 0.5;
		}
		
		// Direction
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
				TPM2_C0V = dutyCycle * 0.5;
				TPM2_C1V = dutyCycle * 0.5;
		
				TPM1_C0V = 0;
				TPM1_C1V = 0;
				break;
			case 4:
				//Pivot Left
				TPM1_C0V = dutyCycle * 0.5;
				TPM1_C1V = dutyCycle * 0.5;
		
				TPM2_C0V = 0;
				TPM2_C1V = 0;
				break;
			case 5:
				//Moving Right
				TPM1_C0V = dutyCycle * 0.2;
				TPM1_C1V = 0;
			
				TPM2_C0V = dutyCycle * 0.8;
				TPM2_C1V = 0;
				break;
			case 6:
				//Moving Left
				TPM1_C0V = dutyCycle * 0.8;
				TPM1_C1V = 0;
			
				TPM2_C0V = dutyCycle * 0.2;
				TPM2_C1V = 0;
				break;
		}
		osDelay(25);
	}
}

/*----------------------------------------------------------------------------
 * LED Control Thread - tLED
 *---------------------------------------------------------------------------*/
// Control current LED pattern to be displayed on robot
void tLED(void *arguments)
{
	offLEDgreen();
	offLEDred();
	osSemaphoreAcquire(startupSem, osWaitForever);
	
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
				osDelay(1000);
				LEDMode = 23;
				break;
			// Running Mode - Front green LED Running mode (1 LED at a time) + RED flashing 500ms ON/OFF
			case 22:
				while(1)
				{
					greenLEDMoving(counterLED);
					onLEDred();
					osDelay(500);
					if (LEDMode != 22)
						break;
					offLEDred();
					osDelay(500);
					counterLED++;
					if (LEDMode != 22)
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
					if (LEDMode != 23)
						break;
					offLEDred();
					osDelay(250);
					if (LEDMode != 23)
						break;
				}
				break;
		}
		osDelay(100);
	}
}

/*----------------------------------------------------------------------------
 * LED Control Thread - tAudio
 *---------------------------------------------------------------------------*/
void tAudio(void *arguments)
{
	soundOff();
	osSemaphoreAcquire(startupSem, osWaitForever);
	
	for (;;)
	{
		switch (audioMode)
		{
			//Off song
			case 30:
				soundOff();
				break;
			// Default Song
			case 31:
				//playNokiaRingtone();
				playBlindingLights(audioMode);
				break;
			// End Song
			case 32:
				playTakeOnMe(audioMode);
				break;
			// Connected to bluetooth
			case 33:
				osDelay(500);
				playNokiaTheme(audioMode);
				audioMode = 31;
				break;
		}
		osDelay(50);
	}
}

/*----------------------------------------------------------------------------
 * Application main thread - tBrain
 *---------------------------------------------------------------------------*/
void tBrain(void *arguments) 
{
	//Receive UART instructions via interrupt, set global variable as interrupt instruction
	for (;;)
	{
		//Movement Commands
		if (UARTCommand < 10)
		{
			motorSelection = UARTCommand;
			// Update LED pattern when not moving
			if (motorSelection == 0) 
			{
				LEDMode = 23;
			}
			else
			{
				LEDMode = 22;
			}
		}
		//Speed Selection
		else if (UARTCommand < 20)
		{
			switch (UARTCommand)
			{
				// High Speed
				case 15:
					speedSelection = 1;
					break;
				// Half Speed
				case 16:
					speedSelection = 2;
					break;
			}
		}
		//Audio Commands
		else if (UARTCommand < 40)
		{
			audioMode = UARTCommand;
		}
		//Connection to bluetooth
		else if (UARTCommand == 90)
		{
			//Release sempahores to start threads
			osSemaphoreRelease(startupSem);
			osSemaphoreRelease(startupSem);
			osSemaphoreRelease(startupSem);
			
			audioMode = 33;
			LEDMode = 21;
			
			UARTCommand = 0;
		}
		//Start self driving mode
		else if (UARTCommand == 100)
		{
			UARTCommand = UART_NO_COMMAND;
			osThreadNew(tSelfDrivingMode, NULL, NULL);
		}
		osDelay(25);
	}
}
 
int main (void) 
{
 
  // System Initialization
  SystemCoreClockUpdate();
	InitGPIO();
	InitGPIOgreen();
	InitGPIOred();
	InitUART2(BAUD_RATE);
	InitPWM();
	InitPIT();
	InitUltrasonic();
	
	// Set all RGB values to 1, to turn off
	offLED();
 
  osKernelInitialize();                 // Initialize CMSIS-RTOS
	startupSem = osSemaphoreNew(3, 0, NULL);
	
	//Create threads
	osThreadNew(tBrain, NULL, &thread_attr);
	osThreadNew(tMotor, NULL, NULL);
	osThreadNew(tLED, NULL, NULL);
	osThreadNew(tAudio, NULL, NULL);

  osKernelStart();                      // Start thread execution
	
  for (;;) {}
}
