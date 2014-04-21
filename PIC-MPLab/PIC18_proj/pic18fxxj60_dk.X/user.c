/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

#if defined(__XC)
    #include <xc.h>         /* XC8 General Include File */
#elif defined(HI_TECH_C)
    #include <htc.h>        /* HiTech General Include File */
#elif defined(__18CXX)
    #include <p18cxxx.h>    /* C18 General Include File */
#endif

#if defined(__XC) || defined(HI_TECH_C)

#include <stdint.h>         /* For uint8_t definition */
#include <stdbool.h>        /* For true/false definition */

#endif

#include "sp_userdef.h"

#include "user.h"

/******************************************************************************/
/* User Functions                                                             */
/******************************************************************************/

/* <Initialize variables in user.h and insert code for user algorithms.> */


spVOIDt Initialize_UART(spVOIDt)
{
    ///PORTC = 0;
    ///PORTCbits.RC6 = 0;
    ///PORTCbits.RC7 = 0;

    LATC = 0;
    LATCbits.LATC6 = 0;
    LATCbits.LATC7 = 0;

    TXSTA = 0b00100100;     /* 8bits, Tx enable, Async mode, high speed, */
    TXSTA1 = 0b00100100;     /* 8bits, Tx enable, Async mode, high speed, */
    ///TXSTAbits.BRGH = 1;     /* bit 2 */
    ///TXSTAbits.SYNC = 0;     /* bit 4 */
    ///TXSTAbits.TXEN = 1;     /* bit 5 */
    RCSTA = 0b10010000;     /* serial port, 8bits, signle recv, Rx enable, non-Addr, */
    RCSTA1 = 0b10010000;     /* serial port, 8bits, signle recv, Rx enable, non-Addr, */
    ///RCSTAbits.CREN = 1;     /* bit 4 */
    ///RCSTAbits.SPEN = 1;     /* bit 7 */
    ///RCSTAbits.SREN = 0;     /* bit 5 */
    ///RCSTAbits.RX9 = 0;      /* bit 6 */

    ///BRG16 = 0;              //
    BAUDCONbits.BRG16 = 0;  /* */
    BAUDCON1bits.BRG16 = 0;  /* */
    /*
     *  SYNC=0, BRGH=1, BRG16=0,
     *  formulas => Fosc/[16(n+1)]
     *  check table 21-1
     */
#if UART_USE_9600
    SPBRGH = 0;
    SPBRGH1 = 0;
    SPBRG = 162;           /* 9600, FOSC = 25Mhz, SPBRG = (Fosc/Baud_Rate/16)-1 */
    SPBRG1 = 162;           /* 9600, FOSC = 25Mhz, SPBRG = (Fosc/Baud_Rate/16)-1 */
#endif
#if UART_USE_19200
    SPBRGH = 0;
    SPBRG = 80;            /* 19200, FOSC = 25Mhz, SPBRG = (Fosc/Baud_Rate/16)-1 */
#endif
#if UART_USE_57600
    SPBRGH = 0;
    SPBRG = 26;            /* 57600, FOSC = 25Mhz, SPBRG = (Fosc/Baud_Rate/16)-1 */
#endif
#if UART_USE_115200
    SPBRGH = 0;
    SPBRG = 13;             /* 115200, FOSC = 25Mhz, SPBRG = (Fosc/Baud_Rate/16)-1 */
#endif

    RCONbits.IPEN = 0;
    INTCON = 0;
    PIE1 = 0;
    PIR1 = 0;

    ///INTCONbits.PEIE = 1;
    ///INTCONbits.INTE = 1;
    INTCONbits.INT0E = 1;
    PIE1bits.RCIE = 1;          /* enable Rx interrupt */
    PIE1bits.TXIE = 0;          /* NOT enable Tx interrupt */
    PIR1bits.RCIF = 0;          /* clear pending */

    ///TRISC6 = 0; // TXD
    TRISCbits.TRISC6 = 0;       /* output, Tx: RC6 */
    ///TRISC7 = 1; // RXD
    TRISCbits.TRISC7 = 1;       /* input, Rx: RC7 */


    ///INTCONbits.GIE = 1;
}



void InitApp(void)
{
    /* TODO Initialize User Ports/Peripherals/Project here */
    /* init GPIOs */
    PORTE = 0;
    PORTEbits.RE4 = 0;
    PORTEbits.RE5 = 0;
    PORTEbits.RE0 = 0;     /* LCM, D4 */
    PORTEbits.RE1 = 0;     /* LCM, D5 */
    PORTEbits.RE2 = 0;     /* LCM, D6 */
    PORTEbits.RE3 = 0;     /* LCM, D7 */
    PORTD = 0;
    PORTDbits.RD0 = 0;     /* LCM, RS */
    PORTDbits.RD1 = 0;     /* LCM, WR */
    PORTDbits.RD2 = 0;     /* LCM, E */
    /* level init */
    LATE = 0x00;
    LATEbits.LATE4 = 0;
    LATEbits.LATE5 = 0;
    LATEbits.LATE0 = 0;     /* LCM, D4 */
    LATEbits.LATE1 = 0;     /* LCM, D5 */
    LATEbits.LATE2 = 0;     /* LCM, D6 */
    LATEbits.LATE3 = 0;     /* LCM, D7 */
    LATD = 0x00;
    LATDbits.LATD0 = 0;     /* LCM, RS */
    LATDbits.LATD1 = 0;     /* LCM, WR */
    LATDbits.LATD2 = 0;     /* LCM, E */
    /* direction */
    TRISE = 0xFF;
    TRISEbits.TRISE4 = 0;       /* output */
    TRISEbits.TRISE5 = 0;       /* output */
    TRISEbits.TRISE0 = 0;       /* output *//* LCM, D4 */
    TRISEbits.TRISE1 = 0;       /* output *//* LCM, D5 */
    TRISEbits.TRISE2 = 0;       /* output *//* LCM, D6 */
    TRISEbits.TRISE3 = 0;       /* output *//* LCM, D7 */
    TRISD = 0xFF;
    TRISDbits.TRISD0 = 0;       /* output *//* LCM, RS */
    TRISDbits.TRISD1 = 0;       /* output *//* LCM, WR */
    TRISDbits.TRISD2 = 0;       /* output *//* LCM, E */

    /* Setup analog functionality and port direction */

    /* Initialize peripherals */
    Initialize_UART();
    
    /* Configure the IPEN bit (1=on) in RCON to turn on/off int priorities */

    /* Enable interrupts */
}

