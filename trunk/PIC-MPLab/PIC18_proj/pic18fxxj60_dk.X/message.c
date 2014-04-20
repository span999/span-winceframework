/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

#if defined(__XC)
    #include <xc.h>         /* XC8 General Include File */
#elif defined(HI_TECH_C)
    #include <htc.h>        /* HiTech General Include File */
#endif


#include <stdint.h>         /* For uint8_t definition */
#include <stdbool.h>        /* For true/false definition */
#include <string.h>         /* for memset() ... */
#include <stdarg.h>         /* for ... */
#include <stdio.h>          /* for vsprintf() */


#include "sp_userdef.h"

///#include "queue.h"


/******************************************************************************/
/* User Function Declaration                                                  */
/******************************************************************************/
spVOIDt Help_Msg(spVOIDt);




/******************************************************************************/
/* User Functions                                                             */
/******************************************************************************/

/* <Initialize variables in user.h and insert code for user algorithms.> */
extern spCHARt                  g_str[GLOBLE_STR_SIZE];






#if 0
spVOIDt Uart_Tx_Char(spCHARt ii)
{
    while( 0 == TXSTAbits.TRMT ) /* check if shift reg full or not */
        ;

    TXREG = ii;
}

spVOIDt Uart_Tx_String(spCHARt* pString, DbgStrFlag flag )
{
    while( '\0' != *pString ) {
        Uart_Tx_Char(*pString);
        pString++;
    }

    if( DF_CR == flag || DF_CRLF == flag ) {
        Uart_Tx_Char(0xD);  /* CR */
    }
    if( DF_LF == flag || DF_CRLF == flag ) {
        Uart_Tx_Char(0xA);  /* LF */
    }
}
#else
#define     Uart_Tx_Char(c)                 {while( 0 == TXSTAbits.TRMT );TXREG=c;}
spVOIDt Uart_Tx_String(spCHARt* pString, DbgStrFlag flag )
{
    while( '\0' != *pString ) {
        Uart_Tx_Char(*pString)
        pString++;
    }

    if( DF_CR == flag || DF_CRLF == flag ) {
        Uart_Tx_Char(0xD)  /* CR */
    }
    if( DF_LF == flag || DF_CRLF == flag ) {
        Uart_Tx_Char(0xA)  /* LF */
    }
}
#endif


spCHARt num2char(spCHARt num)
{
    spCHARt cRet = 0;

    if( num >= 0x0 && num <= 0x9 ) {    /* num */
        cRet = num + '0';
    }
    if( num >= 0x0a && num <= 0x0f ) {    /* a ~ f */
        cRet = num + 'a';
    }

    return cRet;
}


static const spCHARt welcomeSTR_[] = "++";
static const spCHARt welcomeSTRt[] = "+";
static const spCHARt welcomeSTRs[] = " ";
static const spCHARt welcomeSTRH[] = "     1000NXT verification board.";
static const spCHARt helpSTR1[]    = "     format <:cmd[@para][!]>";
static const spCHARt helpSTR2[]    = "     <:hlp[!]> for cmd help";

static const spCHARt ERR_FERR_STR[]        = "fix RCSTA.FERR";
static const spCHARt ERR_OERR_STR[]        = "fix RCSTA.OERR";

static const spCHARt CMD_help_sign[]        = "hlp: help.";
static const spCHARt CMD_powerup_sign[]     = "Pup: AMP_ON up.";
static const spCHARt CMD_poweroff_sign[]    = "Pdn: AMP_ON down.";
static const spCHARt CMD_relayCtl_sign[]    = "Rly: relay control.";

#if 0
static const spCHARt* CMD_list[] = {
    CMD_help_sign,
    CMD_powerup_sign,
    CMD_poweroff_sign,
    CMD_relayCtl_sign,
    0
};


spVOIDt UserCmdParser(spCHARt* pBuffer)
{
    spCHARt Cnt;
    spCHARt CmdCnt = 0;
    spCHARt CmdNumTmp = 0;
    spCHARt CmdNum = 0xFF;

    for( Cnt = 0; Cnt < (USER_CMD_BUFFERSIZE-USER_CMD_SIZE); Cnt++ ) {
        if( USER_CMD_START == pBuffer[Cnt] ) {
            sprintf( g_str, "\r\n[%04u.%03u]=>CMD: %c%c%c", _SEC, _MS, pBuffer[Cnt+1], pBuffer[Cnt+2], pBuffer[Cnt+3] );
            Uart_Tx_String(g_str, DF_CRLF);
#if 1
            while( 0 != CMD_list[CmdNumTmp] ) {
                if( CMD_list[CmdNumTmp][0] == pBuffer[Cnt+1] ) {
                    if( CMD_list[CmdNumTmp][1] == pBuffer[Cnt+2] ) {
                        if( CMD_list[CmdNumTmp][2] == pBuffer[Cnt+3] ) {
                            CmdNum = CmdNumTmp;
                        }
                    }
                }
                CmdNumTmp++;
            }
#else
            while( 0 != CMD_list[CmdNumTmp] ) {
                if( 0 == memcmp( CMD_list+CmdNumTmp, (pBuffer+Cnt+1), 3) ){
                    CmdNum = CmdNumTmp;
                }
                CmdNumTmp++;
            }
#endif
            Cnt = Cnt + USER_CMD_SIZE;
            CmdCnt++;
        }
        if( USER_CMD_PARAM == pBuffer[Cnt] ) {
            sprintf( g_str, "\r\n[%04u.%03u]=>PAR: %c%c%c", _SEC, _MS, pBuffer[Cnt+1], pBuffer[Cnt+2], pBuffer[Cnt+3] );
            Uart_Tx_String(g_str, DF_CRLF);
            Cnt = Cnt + USER_CMD_PARAM_SIZE;
        }
    }

    if( 0 == CmdCnt ) {
        sprintf( g_str, "\r\n=>use':'promp!" );
    } else {
        switch (CmdNum) {
            case 0:
                sprintf( g_str, "=>help!" );
                Uart_Tx_String(g_str, DF_CRLF);
                Help_Msg();
                sprintf( g_str, "" );
                break;
            case 1:
                sprintf( g_str, "=>power on!" );
                AMP_ON_HI
                g_sysInfo.Statusbits.AMP_ON = 1;
                break;
            case 2:
                sprintf( g_str, "=>power down!" );
                AMP_ON_LO
                g_sysInfo.Statusbits.AMP_ON = 0;
                break;
            case 3:
                sprintf( g_str, "=>relay control!" );
                break;
            case 0xFF:
            default:
                sprintf( g_str, "=>Unknow cmd!" );
                break;
        } /* switch */
    }
    Uart_Tx_String(g_str, DF_CRLF);
}


spVOIDt Uart_Rx_Handle(spVOIDt)
{
    spCHARt cRet;
    static spCHARt CmdBuffer[USER_CMD_BUFFERSIZE];
    static spCHARt buffCnt = 0;

    do {
        cRet = Queue_Pull( &g_userInput );
        if( COMM_QUEUENULL != cRet ) {      /* something in ... */
            CmdBuffer[buffCnt] = cRet;           /* to buffer */
            ///buff[buffCnt] = num2char( cRet );

            /* check USER_CMD_END code */
            if( cRet == USER_CMD_END || cRet == '\r' ) {
                /* it is cmd ... */
                UserCmdParser( CmdBuffer );
                buffCnt = 0;                /* reset */
                break;
            } else
            if( cRet == 0x1B ) {            /* ESC */
                /* mode change */
                if( spTRUE == g_sysInfo.Miscbits.IsCmdMode ) {
                    g_sysInfo.Miscbits.IsCmdMode = spFALSE;
                } else {
                    g_sysInfo.Miscbits.IsCmdMode = spTRUE;
                }
                ///buffCnt = 0;                /* reset */
                break;
            } else {
                Uart_Tx_Char(cRet);         /* echo to terminal */
                buffCnt++;                  /* wait for more input */
                buffCnt = buffCnt & USER_CMD_BUFFERMASK;
            }
        } else {                            /* get nothing */
            break;
        }
    } while(1);

#if 1
    if ( 1 == RCSTAbits.FERR ) {
        g_str[30] = RCREG;
        Uart_Tx_String((spCHARt*)ERR_FERR_STR, DF_CRLF);
    }
#endif
#if 1
    if ( 1 == RCSTAbits.OERR ) {
        Uart_Tx_String((spCHARt*)ERR_OERR_STR, DF_CRLF);
        RCSTAbits.CREN = 0;
        RCSTAbits.CREN = 1;
    }
#endif

}
#endif

spVOIDt Help_Msg(spVOIDt)
{
    Uart_Tx_String((spCHARt*)welcomeSTRs, DF_CRLF);
    Uart_Tx_String((spCHARt*)CMD_help_sign, DF_CRLF);
    Uart_Tx_String((spCHARt*)CMD_powerup_sign, DF_CRLF);
    Uart_Tx_String((spCHARt*)CMD_poweroff_sign, DF_CRLF);
    Uart_Tx_String((spCHARt*)CMD_relayCtl_sign, DF_CRLF);
    Uart_Tx_String((spCHARt*)welcomeSTRs, DF_CRLF);
}


spVOIDt Welcome_Msg(spVOIDt)
{
    Uart_Tx_String((spCHARt*)welcomeSTRs, DF_CRLF);
    Uart_Tx_String((spCHARt*)welcomeSTR_, DF_CRLF);
    Uart_Tx_String((spCHARt*)welcomeSTRt, DF_CRLF);
    Uart_Tx_String((spCHARt*)welcomeSTRH, DF_CRLF);
    Uart_Tx_String((spCHARt*)welcomeSTRs, DF_CRLF);
    Uart_Tx_String((spCHARt*)helpSTR1, DF_CRLF);
    Uart_Tx_String((spCHARt*)helpSTR2, DF_CRLF);
    Uart_Tx_String((spCHARt*)welcomeSTRs, DF_CRLF);
    sprintf( g_str, "                         ver: %d.%d", 0, 3 );
    Uart_Tx_String(g_str, DF_CRLF);
    sprintf( g_str, "                    baudrate: %u", UART_BAUDRATE );
    Uart_Tx_String(g_str, DF_CRLF);
    sprintf( g_str, "                 %s-%s", __DATE__, __TIME__ );
    Uart_Tx_String(g_str, DF_CRLF);
    Uart_Tx_String((spCHARt*)welcomeSTRt, DF_CRLF);
    Uart_Tx_String((spCHARt*)welcomeSTR_, DF_CRLF);
    Uart_Tx_String((spCHARt*)welcomeSTRs, DF_CRLF);
}