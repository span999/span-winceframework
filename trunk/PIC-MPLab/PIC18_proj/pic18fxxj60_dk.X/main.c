/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

#if defined(__XC)
    #include <xc.h>        /* XC8 General Include File */
#elif defined(HI_TECH_C)
    #include <htc.h>       /* HiTech General Include File */
#elif defined(__18CXX)
    #include <p18cxxx.h>   /* C18 General Include File */
#endif


#if defined(__XC) || defined(HI_TECH_C)

#include <stdint.h>        /* For uint8_t definition */
#include <stdbool.h>       /* For true/false definition */

#endif
#include <string.h>         /* for memset() ... */
#include <stdarg.h>         /* for ... */
#include <stdio.h>          /* for vsprintf() */


#include "sp_userdef.h"
#include "system.h"        /* System funct/params, like osc/peripheral config */
#include "user.h"          /* User funct/params, such as InitApp */
#include "message.h"
#include "LCM.h"


/******************************************************************************/
/* User Global Variable Declaration                                           */
/******************************************************************************/

/* i.e. uint8_t <variable_name>; */
spCHARt                  g_str[GLOBLE_STR_SIZE];



/******************************************************************************/
/* Main Program                                                               */
/******************************************************************************/



spVOIDt main(spVOIDt)
{
    spUINT16t uCounter = 0;

    /* Configure the oscillator for the device */
    ConfigureOscillator();

    /* Initialize I/O and Peripherals for application */
    InitApp();

    ///Initialize_UART();
    Welcome_Msg();

    /* TODO <INSERT USER APPLICATION CODE HERE> */
    LCM_Init();

    LED1 = LED_ON;
    LED2 = LED_OFF;

    while(1)
    {
        ///LCM_Char((spUINT8t)(uCounter&0x007F));

        sprintf( g_str, "%05u", uCounter );
        Uart_Tx_String(g_str, DF_CR);
        LCM_String(g_str);
        cmdLCM_CR
        LED1 = !LED1;
        ///LED2 = !LED2;
        ///__delay();
        MSdelay(500);
        uCounter++;
    }

}

