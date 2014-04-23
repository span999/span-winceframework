/******************************************************************************/
/*Files to Include                                                            */
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
#include "system.h"

/* Refer to the device datasheet for information about available
oscillator configurations. */
void ConfigureOscillator(void)
{
    /* TODO Add clock switching code if appropriate.  */
    OSCCON = 0b10001011;
    OSCCONbits.IDLEN = 1;
    ///OSCCONbits.OSTS = 1;
    OSCCONbits.OSTS = 0;
    ///OSCCONbits.SCS = 0b11;
    ///OSCCONbits.SCS = 0b00;
    OSCCONbits.SCS = 0b10;

    OSCTUNE = 0b00000000;
    OSCTUNEbits.PLLEN = 1;
    OSCTUNEbits.PLLEN = 0;
    OSCTUNEbits.PPST0 = 0;


    /* Typical actions in this function are to tweak the oscillator tuning
    register, select new clock sources, and to wait until new clock sources
    are stable before resuming execution of the main project. */
}


#define _XTAL_FREQ (25000000U)

#if 1
static spVOIDt __1ms_delay(spVOIDt)
{
    _delay((spUINT32t)(_XTAL_FREQ/4000.0));
}

static spVOIDt __1us_delay(spVOIDt)
{
    _delay((spUINT32t)(_XTAL_FREQ/4000000.0));
}

spVOIDt MSdelay(spUINT16t ms)
{
    while( 0 < ms ) {
        ms--;
        __1ms_delay();
    }
}

spVOIDt USdelay(spUINT16t us)
{
    while( 0 < us ) {
        us--;
        __1us_delay();
    }
}
#else
spVOIDt MSdelay(spUINT16t ms)
{
    _delay( (spUINT32t)((_XTAL_FREQ/4000.0)*(ms)) );
}
#endif
