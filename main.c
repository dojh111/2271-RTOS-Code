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

#define STOP_DISTANCE 25								// Robot ultrasonic sensor stop distance

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

volatile int manualMode = 1;

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
void tSelfDrivingMode(void *arguments)
{
	// Turn on GREEN Led to signal selfDrivingMode
	led_control(GREEN, ON);
	manualMode = 0;
	
	UARTCommand = 0;
	osDelay(300);
	
	startRanging();
	osDelay(100);
	
	UARTCommand = 1;
	osDelay(5);
	// Constantly range distance and move forward
	while (distance > STOP_DISTANCE)
	{
		startRanging();
		osDelay(100);
	}
	
	UARTCommand = 0;
	manualMode = 1;
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
	
	for (;;)
	{
		// Speed Selection
		switch (manualMode)
		{
			case 0: 
				dutyCycle = modvalue * 0.2;
				break;
			case 1:
				dutyCycle = modvalue;
				break;
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
				TPM1_C0V = dutyCycle * 0.3;
				TPM1_C1V = 0;
			
				TPM2_C0V = dutyCycle;
				TPM2_C1V = 0;
				break;
			case 6:
				//Moving Left
				TPM1_C0V = dutyCycle;
				TPM1_C1V = 0;
			
				TPM2_C0V = dutyCycle * 0.3;
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
 * Songs
 *---------------------------------------------------------------------------*/
// Plays musical notes according to delay
void playBlindingLights()
{
	for (int i = 0; i < BLINDING_LIGHTS_NOTE_COUNT; i++)
	{
		if (audioMode != 31) {break;}
			
		int modValue = calculateMODValue(blindingLights[i]);
		TPM0->MOD = modValue;
		TPM0_C3V = modValue * 0.2;
		osDelay(blindingLightsTempo[i]);
	}
}

// Plays musical notes according to delay
void playTakeOnMe()
{
	for (int i = 0; i < TAKE_ON_ME_NOTE_COUNT; i++)
	{
		if (audioMode != 32) {break;}
		
		int modValue = calculateMODValue(takeOnMe[i]);
		TPM0->MOD = modValue;
		TPM0_C3V = modValue * 0.2;
		osDelay(takeOnMeTempo[i]);
	}
}

// Play bluetooth connect tone
void playNokiaTheme()
{
	for (int i = 0; i < NOKIA_THEME_NOTE_COUNT; i++)
	{
		int modValue = calculateMODValue(nokiaTheme[i]);
		TPM0->MOD = modValue;
		TPM0_C3V = modValue * 0.2;
		osDelay(nokiaThemeTempo[i]);
	}
}

void playNokiaRingtone()
{
	for (int i = 0; i < NOKIA_RINGTONE_NOTE_COUNT; i++)
	{
		if (audioMode != 31) {break;}
		
		int modValue = calculateMODValue(nokiaRingtone[i]);
		TPM0->MOD = modValue;
		TPM0_C3V = modValue * 0.2;
		osDelay(nokiaRingtoneTempo[i]);
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

/*----------------------------------------------------------------------------
 * LED Control Thread - tAudio
 *---------------------------------------------------------------------------*/
void tAudio(void *arguments)
{
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
				playNokiaRingtone();
				//playScale();
				//playBlindingLights();
				break;
			// End Song
			case 32:
				playTakeOnMe();
				break;
			// Connected to bluetooth
			case 33:
				osDelay(300);
				playNokiaTheme();
				soundOff();
				osDelay(1000);
				audioMode = 31;
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
		//Movement Commands
		if (command < 20)
		{
			// Update LED pattern when not moving
			if (command == 0) 
			{
				LEDMode = 23;
			}
			else
			{
				LEDMode = 22;
			}
			motorSelection = command;
		}
		//LED Commands
		else if (command < 30)
		{
			LEDMode = command;
		}
		//Audio Commands
		else if (command < 40)
		{
			audioMode = command;
		}
		//Start self driving mode
		else if (command == 100)
		{
			osThreadNew(tSelfDrivingMode, NULL, NULL);
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
	//for (;;)
	//{
		//UARTCommand = 31;
		//osDelay(12000);
		//UARTCommand = 32;
		//osDelay(12000);
	//}
			
	osDelay(5500);
	UARTCommand = 33;
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

// Repeatedly range for distance
void testUltrasonic(void *argument)
{
	for (;;)
	{
		startRanging();
		osDelay(100);
	}
}

void toggleSelfDrivingMode(void *argument)
{
	for (;;)
	{
		osDelay(8000);
		UARTCommand = 100;
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
	InitPIT();
	InitUltrasonic();
	
	// Set all RGB values to 1, to turn off
	offLED();
 
  osKernelInitialize();                 // Initialize CMSIS-RTOS
	myMutex = osMutexNew(NULL);
	
	//Create threads
	osThreadNew(tBrain, NULL, NULL);
	osThreadNew(tMotor, NULL, NULL);
	osThreadNew(tLED, NULL, NULL);
	osThreadNew(tAudio, NULL, NULL);
	
	// Test Threads - For testing purposes
	osThreadNew(toggleLED, NULL, NULL);
	osThreadNew(toggleAudio, NULL, NULL);
	//osThreadNew(toggleSelfDrivingMode, NULL, NULL);
	//osThreadNew(toggleMOTOR, NULL, NULL);
	//osThreadNew(testUltrasonic, NULL, NULL);
	
  osKernelStart();                      // Start thread execution
	
  for (;;) {}
}
