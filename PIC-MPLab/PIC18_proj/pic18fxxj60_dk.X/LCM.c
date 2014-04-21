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


#include "sp_userdef.h"
#include "system.h"        /* System funct/params, like osc/peripheral config */
///#include "user.h"          /* User funct/params, such as InitApp */
///#include "message.h"

/******************************************************************************/
/* User Global Variable Declaration                                           */
/******************************************************************************/

/* i.e. uint8_t <variable_name>; */
spCHARt                  g_str[GLOBLE_STR_SIZE];



/******************************************************************************/
/* Main Program                                                               */
/******************************************************************************/


spVOIDt LCM_DataPins(spUINT8t Pins)
{
    ///spUINT8t Loop = 0;

    Pins = Pins & 0x0F;

    if( ((0x01 << 0) & Pins) > 0 ) { /* D4 */
        LCM_D4 = 1;
    } else {
        LCM_D4 = 0;
    }

    if( ((0x01 << 1) & Pins) > 0 ) { /* D5 */
        LCM_D5 = 1;
    } else {
        LCM_D5 = 0;
    }

    if( ((0x01 << 2) & Pins) > 0 ) { /* D6 */
        LCM_D6 = 1;
    } else {
        LCM_D6 = 0;
    }

    if( ((0x01 << 3) & Pins) > 0 ) { /* D7 */
        LCM_D7 = 1;
    } else {
        LCM_D7 = 0;
    }
}


spVOIDt LCM_Send_Nibble(spUINT8t n)
{
    LCM_DataPins(n);
    LCM_EN = 1;     /* LCM, E */
    MSdelay(1);
    LCM_EN = 0;     /* LCM, E */
    MSdelay(1);
}


spVOIDt LCM_Send_Byte(spUINT8t nn)
{
    LCM_DataPins((nn>>4)&0x0F);
    LCM_EN = 1;     /* LCM, E */
    MSdelay(1);
    LCM_EN = 0;     /* LCM, E */
    ///MSdelay(5);

    LCM_DataPins((nn>>0)&0x0F);
    LCM_EN = 1;     /* LCM, E */
    MSdelay(1);
    LCM_EN = 0;     /* LCM, E */
    ///MSdelay(5);
    MSdelay(1);
}



spVOIDt LCM_Init(spVOIDt)
{

    MSdelay(200);
    MSdelay(15);

    /*
    reference of LCM JHD 162A
    http://www.8051projects.net/lcd-interfacing/lcd-4-bit.php
    http://www.geocities.com/dinceraydin/lcd/commands.htm
    */

    LCM_RS = 0;     /* LCM, RS */
    LCM_WR = 0;     /* LCM, WR */
    LCM_EN = 0;     /* LCM, E */

    /* select 4-bit mode */
    LCM_Send_Nibble(0x03);
    MSdelay(10);
    LCM_Send_Nibble(0x03);
    MSdelay(1);
    MSdelay(5);
    LCM_Send_Nibble(0x03);
    MSdelay(1);
    MSdelay(5);
    LCM_Send_Nibble(0x02);
    MSdelay(1);

    /* init LCM */
    LCM_Send_Byte(0x28);        /* Function set: 2 Line, 4-bit, 5x7 dots */
    MSdelay(1);
    ///LCM_Send_Byte(0x0C);        /* Display on, Cursor off, blink off command */
    LCM_Send_Byte(0x0F);        /* Display on, Cursor on, blinking command */
    MSdelay(1);
    LCM_Send_Byte(0x01);        /* Clear LCD */
    MSdelay(1);
    ///LCM_Send_Byte(0x02);        /* return home */
    ///MSdelay(1);
    LCM_Send_Byte(0x06);        /* Entry mode, auto increment with no shift */
    MSdelay(1);

    ///LCM_Send_Byte(0x80);


#if 0
    LCM_Send_Nibble(0x02);

    LCM_Send_Byte(0x28);

    LCM_Send_Byte(0x01);

    LCM_Send_Byte(0x06);

    LCM_Send_Byte(0x08);

    LCM_Send_Byte(0x0F);
#endif

    MSdelay(200);
}

spVOIDt LCM_Char(spVOIDt)
{
    LCM_RS = 1;     /* LCM, RS */
    LCM_WR = 0;     /* LCM, WR */
    LCM_EN = 0;     /* LCM, E */

    LCM_Send_Byte(0x61);    /* 'a' */

}



