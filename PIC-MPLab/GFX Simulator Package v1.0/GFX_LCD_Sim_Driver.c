/*****************************************************************************
 *  Module for Microchip Graphics Library
 *  Custom display controller driver template
 *****************************************************************************
 * FileName:        CustomDisplayDriver.c
 * Dependencies:    Graphics.h
 * Processor:       PIC24, PIC32
 * Compiler:       	MPLAB C30, MPLAB C32
 * Linker:          MPLAB LINK30, MPLAB LINK32
 * Company:         Microchip Technology Incorporated
 *
 * Software License Agreement
 *
 * Copyright © 2008 Microchip Technology Inc.  All rights reserved.
 * Microchip licenses to you the right to use, modify, copy and distribute
 * Software only when embedded on a Microchip microcontroller or digital
 * signal controller, which is integrated into your product or third party
 * product (pursuant to the sublicense terms in the accompanying license
 * agreement).  
 *
 * You should refer to the license agreement accompanying this Software
 * for additional information regarding your rights and obligations.
 *
 * SOFTWARE AND DOCUMENTATION ARE PROVIDED “AS IS” WITHOUT WARRANTY OF ANY
 * KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY
 * OF MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR
 * PURPOSE. IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR
 * OBLIGATED UNDER CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION,
 * BREACH OF WARRANTY, OR OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT
 * DAMAGES OR EXPENSES INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL,
 * INDIRECT, PUNITIVE OR CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA,
 * COST OF PROCUREMENT OF SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY
 * CLAIMS BY THIRD PARTIES (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF),
 * OR OTHER SIMILAR COSTS.
 *
 * Author               Date        Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 
 *****************************************************************************/
#include "Graphics\Graphics.h"

void TrigRegTrace(void);
void TrigRegInjection(void);
void GetEmulatorTouchMsg(GOL_MSG * pMsg);

//Command codes sent to GfxLCDSimulator for hardware accellerated functions
typedef enum {
	GFXCMD_NONE = 0,
	GFXCMD_INIT,
	GFXCMD_BAR,
	GFXCMD_CIRCLE,
	GFXCMD_LINE
}GFX_CMD;

//Input and Output variables for MPSIM
UINT64_VAL	RegInjectVal;	//inbound
UINT64_VAL	DotData;		//outbound

// Color
WORD_VAL    _color;

// Clipping region control
SHORT       _clipRgn;

// Clipping region borders
SHORT       _clipLeft;
SHORT       _clipTop;
SHORT       _clipRight;
SHORT       _clipBottom;

/*********************************************************************
* Function:  void  DelayMs(WORD time)
*
* PreCondition: none
*
* Input: time - delay in ms
*
* Output: none
*
* Side Effects: none
*
* Overview: delays execution on time specified in ms
*
* Note: none
*
********************************************************************/
#ifdef __PIC32MX

/* */
void DelayMs(WORD time)
{
    while(time--)
    {
        unsigned int    int_status;

        int_status = INTDisableInterrupts();
        OpenCoreTimer(GetSystemClock() / 2000); // core timer is at 1/2 system clock
        INTRestoreInterrupts(int_status);

        mCTClearIntFlag();

        while(!mCTGetIntFlag());
    }

    mCTClearIntFlag();
}

#else
    #define DELAY_1MS   16000 / 5               // for 16MIPS

/* */
void DelayMs(WORD time)
{
    unsigned    delay;
    while(time--)
        for(delay = 0; delay < DELAY_1MS; delay++);
}

#endif

/*********************************************************************
* Function:  void ResetDevice()
*
* PreCondition: none
*
* Input: none
*
* Output: none
*
* Side Effects: none
*
* Overview: resets LCD, initializes PMP
*
* Note: none
*
********************************************************************/
void ResetDevice(void)
{
	DotData.Val = 0L;
	DotData.v[7] = GFXCMD_INIT;
	DotData.word.LW = DISP_HOR_RESOLUTION;
	DotData.word.HW = DISP_VER_RESOLUTION;
	DotData.word.UW = DISP_ORIENTATION;
	TrigRegTrace();
}


/*********************************************************************
* Function: void PutPixel(SHORT x, SHORT y)
*
* PreCondition: none
*
* Input: x,y - pixel coordinates
*
* Output: none
*
* Side Effects: none
*
* Overview: puts pixel
*
* Note: none
*
********************************************************************/
void PutPixel(SHORT x, SHORT y)
{
	DotData.Val = 0L;
	DotData.word.LW = x;
	DotData.word.HW = y;
	DotData.word.UW = _color.Val;
	TrigRegTrace();
}

/*********************************************************************
* Function: WORD GetPixel(SHORT x, SHORT y)
*
* PreCondition: none
*
* Input: x,y - pixel coordinates 
*
* Output: pixel color
*
* Side Effects: none
*
* Overview: returns pixel color at x,y position
*
* Note: none
*
********************************************************************/
WORD GetPixel(SHORT x, SHORT y)
{
    return (0);
}

/*********************************************************************
* Function: WORD Bar(SHORT left, SHORT top, SHORT right, SHORT bottom)
*
* PreCondition: none
*
* Input: left,top - top left corner coordinates,
*        right,bottom - bottom right corner coordinates
*
* Output: For NON-Blocking configuration:
*         - Returns 0 when device is busy and the shape is not yet completely drawn.
*         - Returns 1 when the shape is completely drawn.
*         For Blocking configuration:
*         - Always return 1.
*
* Side Effects: none
*
* Overview: draws rectangle filled with current color
*
* Note: none
*
********************************************************************/
#ifdef USE_DRV_BAR
WORD Bar(SHORT left, SHORT top, SHORT right, SHORT bottom)
{
	DotData.Val = 0L;
	DotData.v[7] = GFXCMD_BAR;
	DotData.word.LW = _color.Val;
	TrigRegTrace();

	DotData.word.LW = left;
	DotData.word.HW = top;
	DotData.word.UW = right;
	DotData.word.MW = bottom;
	TrigRegTrace();
	return 1;
}
#endif

#ifdef USE_DRV_PUTIMAGE
/*********************************************************************
* Function: WORD PutImage(SHORT left, SHORT top, void* bitmap, BYTE stretch)
*
* PreCondition: none
*
* Input: left,top - left top image corner,
*        bitmap - image pointer,
*        stretch - image stretch factor
*
* Output: For NON-Blocking configuration:
*         - Returns 0 when device is busy and the image is not yet completely drawn.
*         - Returns 1 when the image is completely drawn.
*         For Blocking configuration:
*         - Always return 1.
*
* Side Effects: none
*
* Overview: outputs image starting from left,top coordinates
*
* Note: image must be located in flash
*
********************************************************************/
WORD PutImage(SHORT left, SHORT top, void *bitmap, BYTE stretch)
{
	return 1;
}
#endif
	
void GetEmulatorTouchMsg(GOL_MSG * pMsg)
{
	PORTBbits.RB0 = 1;
	TrigRegInjection();
	//triggerin_gpr() in the scl module doesn't always fill in the first
	//2 bytes for some reason. LCDSimulator places the 6 bytes of data at
	//the end of the 8 byte packet
	pMsg->type = RegInjectVal.v[2];		// Type of input device.
	pMsg->uiEvent = RegInjectVal.v[3];		// The generic events for input device.
    pMsg->param1 = RegInjectVal.word.UW;	// Parameter 1 meaning is dependent on the type of input device.
    pMsg->param2 = RegInjectVal.word.MW;	// Parameter 2 meaning is dependent on the type of input device.
    PORTBbits.RB0 = 0;
}

void EmulatorTouchInit(void)
{
	AD1PCFGSET = 0xFFFF;
	TRISBbits.TRISB0 = 0;
}

void __attribute__((naked)) TrigRegTrace(void)
{
	_nop();
}

void __attribute__((naked)) TrigRegInjection(void)
{
	_nop();
}

