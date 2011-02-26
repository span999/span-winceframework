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
 * Copyright ?2008 Microchip Technology Inc.  All rights reserved.
 * Microchip licenses to you the right to use, modify, copy and distribute
 * Software only when embedded on a Microchip microcontroller or digital
 * signal controller, which is integrated into your product or third party
 * product (pursuant to the sublicense terms in the accompanying license
 * agreement).  
 *
 * You should refer to the license agreement accompanying this Software
 * for additional information regarding your rights and obligations.
 *
 * SOFTWARE AND DOCUMENTATION ARE PROVIDED “AS IS?WITHOUT WARRANTY OF ANY
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
#include "Graphics/Graphics.h"

#include <SDL.h>

///#include "SDLDislpayEmulator/SDLDisplay.h"

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

#if defined(WIN32)


void WIN32_ms_sleep( UINT16 mswait )
{
	UINT uiLoop = 0;
	UINT16 uiWait = mswait;
	for( uiLoop = 0; uiLoop < 10000; uiLoop++ )
	{
		uiWait = mswait;
		while( uiWait > 0 )
		{
			uiWait--;
		}
	}	
}


void DelayMs(WORD time)
{
///    unsigned    delay;
///    while(time--)
///        for(delay = 0; delay < DELAY_1MS; delay++);
///		Sleep( time );
	WIN32_ms_sleep( 1000 * time );
}
#else
/* */
void DelayMs(WORD time)
{
    unsigned    delay;
    while(time--)
        for(delay = 0; delay < DELAY_1MS; delay++);
}
#endif

#endif

// global surface - screen
static SDL_Surface *screen = NULL;

BOOL IsScreenValid( void )
{
	BOOL bRet = FALSE;

	if( NULL != screen )
		bRet = TRUE;
	else
	{
		///screen = SDL_SetVideoMode(320, 240, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
		screen = SDL_SetVideoMode(GetMaxX(), GetMaxY(), 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
		///screen = SDL_SetVideoMode(GetMaxX()+1, GetMaxY()+1, 16, SDL_HWSURFACE | SDL_DOUBLEBUF);
		if( NULL == screen )
		{
			printf(" Unable to set %dx%d video: %s\n", GetMaxX(), GetMaxY(), SDL_GetError());
			bRet = FALSE;
		}
		else
			printf(" Create to set %dx%d video: %s\n", GetMaxX(), GetMaxY(), SDL_GetError());
	}
	
	return bRet;
}

BOOL IsXYValid(SHORT x, SHORT y)
{
	BOOL bRet = FALSE;

	
	if( x >= 0 && x <= GetMaxX() )
		if( y >= 0 && y <= GetMaxY() )
			bRet = TRUE;
	
	if( !bRet )
		printf(" IsXYValid: out of XY=%d,%d\n", x, y);
		
	return bRet;
}


Uint32 TransColors()
{
	Uint32 cc = 0;

	/// reverse the define of RGB565CONVERT
	cc = cc | ((_color.Val & 0x0000001F) << (0+3)) ;
	cc = cc | ((_color.Val & 0x000007E0) << (3+2)) ;
	cc = cc | ((_color.Val & 0x0000F800) << (5+3)) ;

	return cc;
}

void Lock(SDL_Surface *screen)
{
	if ( SDL_MUSTLOCK(screen) )
	{
		if ( SDL_LockSurface(screen) < 0 )
		{
			//exit(1);
		}
	}
}


void Unlock(SDL_Surface *screen)
{
	if ( SDL_MUSTLOCK(screen) )
	{
		SDL_UnlockSurface(screen);
	}
}


void DrawPixel(SDL_Surface *screen, int x, int y, unsigned long val)
{

	Uint8 R;
	Uint8 G;
	Uint8 B;

	Uint32 color;

	// extract Red Green Blue colors
	R = (Uint8) (val >> 16) & 0xFF;
	G = (Uint8) (val >> 8 ) & 0xFF;
	B = (Uint8) (val >> 0 ) & 0xFF;

	color = SDL_MapRGB(screen->format, R, G, B);
///	Uint32 color = SDL_MapRGB(screen->format, R, G, B);
  
 
	switch (screen->format->BytesPerPixel)
	{
		case 1:
		{
			Uint8 *bufp;
			bufp = (Uint8 *)screen->pixels + y*screen->pitch + x;
			*bufp = color;
		}
		break;
		case 2:
		{
			Uint16 *bufp;
			bufp = (Uint16 *)screen->pixels + y*screen->pitch/2 + x;
			*bufp = color;
		}
		break;
		case 3:
		{
			Uint8 *bufp;
			bufp = (Uint8 *)screen->pixels + y*screen->pitch + x * 3;
			if(SDL_BYTEORDER == SDL_LIL_ENDIAN)
			{
				bufp[0] = color;
				bufp[1] = color >> 8;
				bufp[2] = color >> 16;
			} else {
				bufp[2] = color;
				bufp[1] = color >> 8;
				bufp[0] = color >> 16;
			}
		}
		break;
		case 4:
		{
			Uint32 *bufp;
			bufp = (Uint32 *)screen->pixels + y*screen->pitch/4 + x;
			*bufp = color;
		}
		break;
	}
  
}



static void displayPixelDraw(void *pvDisplayData, long lX, long lY, unsigned long ulValue)
{
	
//	printf("%u-%u-%u ",lX, lY, ulValue);
	Lock(screen);
	DrawPixel(screen, lX, lY, ulValue);
	Unlock(screen);
//	SDL_UpdateRect(screen, lX, lY, 1, 1);

	
}



static void displayRectFill(void *pvDisplayData,
							SHORT left, SHORT top, SHORT right, SHORT bottom,
                            unsigned long ulValue)
{
    SDL_Rect rect;

    rect.x = (left>right)?right:left;
	rect.y = (top>bottom)?bottom:top;
	rect.w = abs(right - left) + 1;
	rect.h = abs(top - bottom) + 1;
	
	if( rect.w > GetMaxX() )
		rect.w = GetMaxX();

	if( rect.h > GetMaxY() )
		rect.h = GetMaxY();

		
	SDL_FillRect(screen, &rect, ulValue);
	
	//SDL_Flip(screen);
	
}



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
{ }

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
	if( IsScreenValid() && IsXYValid( x, y ) )
	{
		///printf(">>>emulatorDisplayDriver: PutPixel x=%d,y=%d\n", x, y);
		///displayPixelDraw( NULL, x, y, _color.Val );
		displayPixelDraw( NULL, x, y, TransColors() );
	}
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
#if defined(WIN32)
WORD _GetPixel_(SHORT x, SHORT y)
#else
WORD GetPixel(SHORT x, SHORT y)
#endif
{
    return (0);
}


#ifdef USE_DRV_BAR

/* */
WORD Bar(SHORT left, SHORT top, SHORT right, SHORT bottom)
{

        #ifndef USE_NONBLOCKING_CONFIG
    while(IsDeviceBusy() != 0) Nop();

    /* Ready */
        #else
    if(IsDeviceBusy() != 0)
        return (0);
        #endif
	
	if( IsScreenValid() )
		///displayRectFill( NULL, left, top, right, bottom, _color.Val );
		displayRectFill( NULL, left, top, right, bottom, TransColors() );
	else
		return (0);

    return (1);
}

#endif


#ifdef USE_DRV_CLEARDEVICE

void ClearDevice(void)
{
	if( IsScreenValid() )
		displayRectFill( NULL, 0, GetMaxY(), GetMaxX(), 0, 0x00ffffff );	///clean in white
		///displayRectFill( NULL, 0, GetMaxY(), GetMaxX(), 0, 0x00000000 );	///clean in black
}

#endif

#ifdef USE_DRV_SETFONT
void SetFont(void *font)
{

    FONT_HEADER *pHeader;

        #ifdef USE_FONT_EXTERNAL
    FONT_HEADER header;
        #endif
    _font = font;
    switch(*((SHORT *)font))
    {
                #ifdef USE_FONT_FLASH

        case FLASH:
		#if defined(WIN32)
			pHeader = (FONT_HEADER *)font;
		#else
            pHeader = (FONT_HEADER *) ((FONT_FLASH *)font)->address;
		#endif	
            break;

				#endif // USE_FONT_FLASH
                #ifdef USE_FONT_EXTERNAL
				
        case EXTERNAL:
            pHeader = &header;
            ExternalMemoryCallback(font, 0, sizeof(FONT_HEADER), pHeader);
            break;

				#endif // USE_FONT_EXTERNAL

        default:
            return;
    }

    _fontFirstChar = pHeader->firstChar;
    _fontLastChar = pHeader->lastChar;
    _fontHeight = pHeader->height;
///	printf(" firstChar=%d[0x%x]\n", _fontFirstChar, _fontFirstChar);
///	printf(" lastChar=%d[0x%x]\n", _fontLastChar, _fontLastChar);
///	printf(" height=%d[0x%x]\n", _fontHeight, _fontHeight);
}
#endif //#ifndef USE_DRV_SETFONT

#ifdef USE_DRV_GETTEXTWIDTH
/*********************************************************************
* Function: SHORT GetTextWidth(XCHAR* textString, void* font)
*
* PreCondition: none
*
* Input: textString - pointer to the text string,
*        font - pointer to the font
*
* Output: text width in pixels
*
* Side Effects: none
*
* Overview: returns text width for the font
*
* Note: none
*
********************************************************************/
SHORT GetTextWidth(XCHAR *textString, void *font)
{
		#if defined (USE_FONT_RAM) || defined (USE_FONT_FLASH) 
    GLYPH_ENTRY *pChTable;
    FONT_HEADER *pHeader;
    	#endif
        #ifdef USE_FONT_EXTERNAL
    GLYPH_ENTRY chTable;
    FONT_HEADER header;
        #endif

    	#if defined (USE_FONT_RAM) || defined (USE_FONT_FLASH) || defined (USE_FONT_EXTERNAL)
    SHORT       textWidth;
    XCHAR       ch;
    XCHAR       fontFirstChar;
    XCHAR       fontLastChar;
    	#endif


    switch(*((SHORT *)font))
    {
                #ifdef USE_FONT_RAM

        case RAM:
            pHeader = (FONT_HEADER *) ((FONT_RAM *)font)->address;
            fontFirstChar = pHeader->firstChar;
            fontLastChar = pHeader->lastChar;
            pChTable = (GLYPH_ENTRY *) (pHeader + 1);
            textWidth = 0;
            while((unsigned XCHAR)15 < (unsigned XCHAR)(ch = *textString++))
            {
                if((unsigned XCHAR)ch < (unsigned XCHAR)fontFirstChar)
                    continue;
                if((unsigned XCHAR)ch > (unsigned XCHAR)fontLastChar)
                    continue;
                textWidth += (pChTable + ((unsigned XCHAR)ch - (unsigned XCHAR)fontFirstChar))->width;
            }

            return (textWidth);
                #endif
                	    
                #ifdef USE_FONT_FLASH

        case FLASH:
		#if defined(WIN32)
			pHeader = (FONT_HEADER *)font;
            fontFirstChar = (char)pHeader->firstChar;
            fontLastChar = (char)pHeader->lastChar;
		#else
            pHeader = (FONT_HEADER *) ((FONT_FLASH *)font)->address;
            fontFirstChar = pHeader->firstChar;
            fontLastChar = pHeader->lastChar;
		#endif	
            pChTable = (GLYPH_ENTRY *) (pHeader + 1);
            textWidth = 0;
            while((XCHAR)15 < (XCHAR)(ch = *textString++))
            {
				///printf(" this char=%c[0x%x]\n", *(XCHAR *)textString, *(XCHAR *)textString );
                if((XCHAR)ch < (XCHAR)fontFirstChar)
                    continue;
                if((XCHAR)ch > (XCHAR)fontLastChar)
                    continue;
                textWidth += (pChTable + ((XCHAR)ch - (XCHAR)fontFirstChar))->width;
            }
			///printf(" textWidth=%c[0x%x],%d[0x%x]\n", ch, ch, textWidth, textWidth);
            return (textWidth);
                #endif
                #ifdef USE_FONT_EXTERNAL

        case EXTERNAL:
            ExternalMemoryCallback(font, 0, sizeof(FONT_HEADER), &header);
            fontFirstChar = header.firstChar;
            fontLastChar = header.lastChar;
            textWidth = 0;
            while((XCHAR)15 < (XCHAR)(ch = *textString++))
            {
                if((XCHAR)ch < (XCHAR)fontFirstChar)
                    continue;
                if((XCHAR)ch > (XCHAR)fontLastChar)
                    continue;
                ExternalMemoryCallback
                (
                    font,
                    sizeof(FONT_HEADER) + sizeof(GLYPH_ENTRY) * ((XCHAR)ch - (XCHAR)fontFirstChar),
                    sizeof(GLYPH_ENTRY),
                    &chTable
                );
                textWidth += chTable.width;
            }

            return (textWidth);
                #endif

        default:
            return (0);
    }
}
#endif //#ifdef USE_DRV_GETTEXTWIDTH

#ifdef USE_DRV_GETTEXTHEIGHT
SHORT GetTextHeight(void *font)
{
        #ifdef USE_FONT_EXTERNAL

    char    height;
        #endif
		
    switch(*((SHORT *)font))
    {
                #ifdef USE_FONT_RAM
        case RAM:
            return ((FONT_HEADER *) ((FONT_RAM *)font)->address)->height;
                #endif
                
                #ifdef USE_FONT_FLASH
        case FLASH:
		#if defined(WIN32)
			///printf(" height=%d[0x%x]\n", ((FONT_HEADER *) ((FONT_HEADER *)font))->height, ((FONT_HEADER *) ((FONT_HEADER *)font))->height);
			return ((FONT_HEADER *) ((FONT_HEADER *)font))->height;
		#else
            return ((FONT_HEADER *) ((FONT_FLASH *)font)->address)->height;
		#endif
                #endif
                
                #ifdef USE_FONT_EXTERNAL
        case EXTERNAL:
            ExternalMemoryCallback(font, sizeof(FONT_HEADER) - 2, 1, &height);
            return (height);
                #endif

        default:
            return (0);
    }
}
#endif //#ifdef USE_DRV_GETTEXTHEIGHT



#ifdef USE_DRV_OUTCHAR
WORD OutChar(XCHAR ch)
{
   		#ifdef USE_FONT_FLASH	
    GLYPH_ENTRY *pChTable = NULL;
    	#endif
    BYTE        *pChImage = NULL;

        #ifdef USE_FONT_EXTERNAL
    GLYPH_ENTRY chTable;
    BYTE        chImage[EXTERNAL_FONT_BUFFER_SIZE];
    WORD        imageSize;
    DWORD_VAL   glyphOffset;
        #endif
    SHORT       chWidth = 0;
    SHORT       xCnt, yCnt, x = 0, y;
    BYTE        temp = 0, mask;

        #ifndef USE_NONBLOCKING_CONFIG
    while(IsDeviceBusy() != 0) Nop();

    /* Ready */
        #else
    if(IsDeviceBusy() != 0)
        return (0);
        #endif
    if((XCHAR)ch < (XCHAR)_fontFirstChar)
        return (-1);
    if((XCHAR)ch > (XCHAR)_fontLastChar)
        return (-1);

    switch(*((SHORT *)_font))
    {
                #ifdef USE_FONT_FLASH

        case FLASH:
		#if defined(WIN32)
            pChTable = (GLYPH_ENTRY *) ((BYTE *)_font + sizeof(FONT_HEADER)) + ((XCHAR)ch - (XCHAR)_fontFirstChar);

            pChImage = (BYTE *) ((BYTE *)_font + ((DWORD)(pChTable->offsetMSB) << 8) + pChTable->offsetLSB);
		#else
            pChTable = (GLYPH_ENTRY *) (((FONT_FLASH *)_font)->address + sizeof(FONT_HEADER)) + ((XCHAR)ch - (XCHAR)_fontFirstChar);

            pChImage = (BYTE *) (((FONT_FLASH *)_font)->address + ((DWORD)(pChTable->offsetMSB) << 8) + pChTable->offsetLSB);
		#endif
            chWidth = pChTable->width;

            break;
                #endif
                #ifdef USE_FONT_EXTERNAL
			
        case EXTERNAL:

            // get glyph entry
            ExternalMemoryCallback
            (
                _font,
                sizeof(FONT_HEADER) + ((XCHAR)ch - (XCHAR)_fontFirstChar) * sizeof(GLYPH_ENTRY),
                sizeof(GLYPH_ENTRY),
                &chTable
            );

            chWidth = chTable.width;

            // width of glyph in bytes
            imageSize = 0;
            if(chWidth & 0x0007)
                imageSize = 1;
            imageSize += (chWidth >> 3);

            // glyph image size
            imageSize *= _fontHeight;

            // get glyph image
            glyphOffset.w[1] = (chTable.offsetMSB >> 8);
            glyphOffset.w[0] = (chTable.offsetMSB << 8) + (chTable.offsetLSB);

            ExternalMemoryCallback(_font, glyphOffset.Val, imageSize, &chImage);
            pChImage = (BYTE *) &chImage;

            break;
                #endif

        default:
            break;
    }

    if(_fontOrientation == ORIENT_HOR)
    {
        y = GetY();
        for(yCnt = 0; yCnt < _fontHeight; yCnt++)
        {
            x = GetX();
            mask = 0;
            for(xCnt = 0; xCnt < chWidth; xCnt++)
            {
                if(mask == 0)
                {
                    temp = *pChImage++;
                    mask = 0x01;
                }

                if(temp & mask)
                {
                    PutPixel(x, y);
                }

                x++;
                mask <<= 1;
            }

            y++;
        }

        // move cursor
        _cursorX = x;
    }
    else
    {
        y = GetX();
        for(yCnt = 0; yCnt < _fontHeight; yCnt++)
        {
            x = GetY();
            mask = 0; 
            for(xCnt = 0; xCnt < chWidth; xCnt++)
            {
                if(mask == 0)
                {
                    temp = *pChImage++;
                    mask = 0x01; 
                }

                if(temp & mask)
                {
                    PutPixel(y, x);
                }

                x--;
                mask <<= 1;
            }

            y++;
        }

        // move cursor
        _cursorY = x;
    }

	return (1);
}

#endif