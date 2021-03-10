#include "MKL25Z4.h"

#define UART_TX_PORTE22 22
#define UART_RX_PORTE23 23
#define UART2_INT_PRIO 128

void InitUART2(uint32_t baud_rate) 
{
	uint32_t divisor, bus_clock;
	
	// enable clock to UART and Port E
	SIM->SCGC4 |= SIM_SCGC4_UART2_MASK;
	SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;
	
	// connect UART to pins for PTE22, PTE23
	PORTE->PCR[22] = PORT_PCR_MUX(4);
	PORTE->PCR[23] = PORT_PCR_MUX(4);
	
	// ensure txand rxare disabled before configuration
	UART2->C2 &= ~(UARTLP_C2_TE_MASK | UARTLP_C2_RE_MASK);
	
	// Set baud rate to 4800 baud
	bus_clock = (DEFAULT_SYSTEM_CLOCK)/2;	
	divisor = bus_clock/(baud_rate*16);
	UART2->BDH = UART_BDH_SBR(divisor>>8);
	UART2->BDL = UART_BDL_SBR(divisor);
	
	// No parity, 8 bits, two stop bits, other settings;
	UART2->C1 = 0;
	UART2->S2 = 0;
	UART2->C3 = 0;
	
	// Enable transmitter and receiver
	UART2->C2 |= UART_C2_TE_MASK | UART_C2_RE_MASK;
}

void UART2_Transmit_Poll(uint8_t data) 
{
	// wait until transmit data register is empty
	while (!(UART2->S1 & UART_S1_TDRE_MASK))
	;
	UART2->D = data;
}

uint8_t UART2_Receive_Poll(void) 
{
	// wait until receive data register is full
	while (!(UART2->S1 & UART_S1_RDRF_MASK))
	;
	return UART2->D;
}
