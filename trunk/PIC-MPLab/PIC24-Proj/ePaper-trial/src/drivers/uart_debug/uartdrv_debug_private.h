#ifndef __UARTDRV_DEBUG_PRIVATE_H__
#define __UARTDRV_DEBUG_PRIVATE_H__



///#define UART_QUEUE_SIZE	4	// number of messages queue can contain
///#define UART_ENTRY_SIZE 60	// size of each message

// structure used to output messages via the uart
// the application must ensure that messages are less than 60 characters
///typedef struct {
///	char	buff[UART_ENTRY_SIZE];	
///} UARTMsg;

// size of the stack for this task
#define STACK_SIZE_UART		(configMINIMAL_STACK_SIZE * 1)


/// The UART marco
#define UART_BAUD_RATE		19200
#define UARTTX_TRIS			(TRISFbits.TRISF5)
#define UARTTX_IO			(PORTFbits.RF5)
#define UARTRX_TRIS			(TRISFbits.TRISF4)
#define UARTRX_IO			(PORTFbits.RF4)
#define UBRG				U1BRG
#define UMODE				U1MODE
#define USTA				U1STA
#define USTAbits			U1STAbits
#define UTXREG				U1TXREG
#define URXREG				U1RXREG

#if defined(__C30__) 	// PIC24F
	#define UART_CONFIG1 (UART_EN)
	#define UART_CONFIG2 (UART_TX_ENABLE | UART_INT_RX_CHAR)
	#define CLOSEST_BRG ((GetPeripheralClock()+8ul*UART_BAUD_RATE)/16/UART_BAUD_RATE-1)
	// because of bit alignment we have hardcoded the priority to be
	// configKERNEL_INTERRUPT_PRIORITY + 1 = 2
	#define UART_INT_CONFIG (UART_RX_INT_EN | UART_RX_INT_PR2)
#else					// PIC32
	#define UART_CONFIG1 (UART_EN)
	#define UART_CONFIG2 (UART_RX_ENABLE | UART_TX_ENABLE | UART_INT_RX_CHAR)
	#define CLOSEST_BRG ((GetPeripheralClock()+8ul*UART_BAUD_RATE)/16/UART_BAUD_RATE-1)
	#define UART_INT_CONFIG ((configKERNEL_INTERRUPT_PRIORITY + 1) | UART_RX_INT_EN)
#endif

#define OpenUART(a,b,c)			OpenUART1(a,b,c)
#define ConfigIntUART(a)		ConfigIntUART1(a)

/// The UART interrupt routine define
#if defined(__C30__)
#define		_ThisUARTINTtype_			__attribute__((__interrupt__, auto_psv))
#define		_ThisUARTInterrupt_			_U1RXInterrupt
#else // PIC32
#define		_ThisUARTINTtype_			__attribute__( (interrupt(ipl2), vector(_UART2_VECTOR)))
#define		_ThisUARTInterrupt_			vU1InterruptHandler
#endif


#endif	///#ifndef __UARTDRV_DEBUG_PRIVATE_H__

