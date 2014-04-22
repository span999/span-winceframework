/******************************************************************************/
/* System Level #define Macros                                                */
/******************************************************************************/

/* TODO Define system operating frequency */
#ifndef __SP_USERDEF_H__
#define __SP_USERDEF_H__

#include "sp_typedef.h"



#define PWM_COUNT_8BIT





#define         spSYSTICKt                      spUINT16t
#define         spPWMt                          spFLOATt
    #ifdef PWM_COUNT_8BIT
#define         spPWMCnt                        spUINT8t
    #else
#define         spPWMCnt                        spUINT16t
    #endif
#define         spCHARt                         spUINT8t



#define         GLOBLE_STR_SIZE                 (64)


/* baudrate selection */
#define         UART_USE_9600                   (0)
#define         UART_USE_19200                  (0)
#define         UART_USE_57600                  (0)
#define         UART_USE_115200                 (1)
#define         UART_USE_USER                   (0)

#if UART_USE_9600
#define         UART_BAUDRATE                   9600
#define         UART_BAUDRATE_SPBRG             162         /* 9600, FOSC = 25Mhz, SPBRG = (Fosc/Baud_Rate/16)-1 */
#define         UART_BAUDRATE_SPBRGH            0
#endif
#if UART_USE_19200
#define         UART_BAUDRATE                   19200
#define         UART_BAUDRATE_SPBRG             80         /* 19200, FOSC = 25Mhz, SPBRG = (Fosc/Baud_Rate/16)-1 */
#define         UART_BAUDRATE_SPBRGH            0
#endif
#if UART_USE_57600
#define         UART_BAUDRATE                   57600
#define         UART_BAUDRATE_SPBRG             26         /* 57600, FOSC = 25Mhz, SPBRG = (Fosc/Baud_Rate/16)-1 */
#define         UART_BAUDRATE_SPBRGH            0
#endif
#if UART_USE_115200
#define         UART_BAUDRATE                   115200
#define         UART_BAUDRATE_SPBRG             13         /* 9600, FOSC = 25Mhz, SPBRG = (Fosc/Baud_Rate/16)-1 */
#define         UART_BAUDRATE_SPBRGH            0
#endif
#if UART_USE_USER
#define         UART_BAUDRATE                   0
#define         UART_BAUDRATE_SPBRG             162         /* 9600, FOSC = 25Mhz, SPBRG = (Fosc/Baud_Rate/16)-1 */
#define         UART_BAUDRATE_SPBRGH            0
#endif





#define         USER_UART_INT_PENDING           (PIR1bits.RCIF)
#define         USER_UART_INT_CLEAR             {PIR1bits.RCIF = 0;}


#define         UART_USE_INPUTQUEUE             1
#define         UART_USE_OUTPUTQUEUE            0


#define         USER_CMD_START                  ':'
#define         USER_CMD_PARAM                  '@'
#define         USER_CMD_END                    '!'
#define         USER_CMD_BUFFERSIZE             (8)                 /* must be power of 2 */
#define         USER_CMD_BUFFERMASK             (USER_CMD_BUFFERSIZE-1)
#define         USER_CMD_SIZE                   (3)
#define         USER_CMD_PARAM_SIZE             (3)

typedef enum {
    DF_normal = 0,     /* no auto CR & LF */
    DF_CR,
    DF_LF,
    DF_CRLF
} DbgStrFlag;

/* debug pin control */
#if 1
    #define sp__RB1_PEAK \
                        LATBbits.LATB1 = !LATBbits.LATB1; \
                        LATBbits.LATB1 = !LATBbits.LATB1;
    #define sp__RB1_KEEP \
                        LATBbits.LATB1 = LATBbits.LATB1; \
                        LATBbits.LATB1 = LATBbits.LATB1;
#else
    #define sp__RB1_PEAK        {;}
    #define sp__RB1_KEEP        {;}
#endif /*  */






#define         LCM_RS                          LATDbits.LATD0
#define         LCM_WR                          LATDbits.LATD1
#define         LCM_EN                          LATDbits.LATD2

#define         LCM_D4                          LATEbits.LATE0
#define         LCM_D5                          LATEbits.LATE1
#define         LCM_D6                          LATEbits.LATE2
#define         LCM_D7                          LATEbits.LATE3


#define         LED1                            LATEbits.LATE4
#define         LED2                            LATEbits.LATE5
#define         LED_ON                          (0)
#define         LED_OFF                         (1)







#endif /* __SP_USERDEF_H__ */