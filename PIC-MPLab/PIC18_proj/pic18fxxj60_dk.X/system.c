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
    OSCCONbits.OSTS = 1;
    OSCCONbits.SCS = 0b11;

    OSCTUNE = 0b00000000;
    OSCTUNEbits.PLLEN = 1;


    /* Typical actions in this function are to tweak the oscillator tuning
    register, select new clock sources, and to wait until new clock sources
    are stable before resuming execution of the main project. */
}


static spVOIDt __delay(spVOIDt)
{
    spUINT16t iLoop1 = 250;
    spUINT16t iLoop2 = 100;

    while( 0 < iLoop1 ) {
        iLoop1--;
        while( 0 < iLoop2 ) {
            iLoop2--;
            _delay(1);
        }
    }
}


spVOIDt MSdelay(spUINT16t ms)
{
    while( 0 < ms ) {
        ms--;
        _delay(1);
    }
}
