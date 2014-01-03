#include <stdlib.h>
#include <stdio.h>

#include <SDL.h>

///#include "grlib/grlib.h"
// touch screen emulation and debug variables
#if defined(WIN32)

#include "kbkeymap.h"


typedef struct
{
    //
    //! The minimum X coordinate of the rectangle.
    //
    short sXMin;

    //
    //! The minimum Y coordinate of the rectangle.
    //
    short sYMin;

    //
    //! The maximum X coordinate of the rectangle.
    //
    short sXMax;

    //
    //! The maximum Y coordinate of the rectangle.
    //
    short sYMax;
}
tRectangle;

#else
///#include "grlib/widget.h"
#endif

#include "SDLDisplay.h"

extern Uint32 my_callbackfunc(Uint32 interval, void *param);

// global surface - screen
///SDL_Surface *screen;
///SDL_Surface *screen = NULL;
///we do it in driver!!
extern "C" SDL_Surface *screen;

// global timer
SDL_TimerID timerID = NULL;

//*****************************************************************************
//! Period in which is emulated grlib autorepeat functionality 
//*****************************************************************************
#define BUTTON_REPEAT_PERIOD_MS 10


//*****************************************************************************
//! Determines if mouse button is held down
//*****************************************************************************
bool buttDown = false;

//*****************************************************************************
//! Call back for touch screen(emulated by mouse)
//*****************************************************************************
static void (*g_pfnTSHandler)(unsigned long ulMessage, long lX, long lY) = NULL;


///we do it in driver!!
extern "C" void Init_SDL_ScreenSurface(void);

//*****************************************************************************
//
//! Initializes the display driver.
//!
//! This function initializes the SDL display.
//!
//! \return None.
//
//*****************************************************************************
void SDLInit(void)
{
	 
	if( NULL != screen )
	{
		printf("Already initialized SDL: %x\n", screen);
		return;
	}
	
	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) == -1)
	{
		printf("Unable to initialize SDL: %s\n", SDL_GetError());
		SDL_Quit();
		exit(1);
	}
#if 0	///we do it in driver!!
	screen = SDL_SetVideoMode(320, 240, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);  
#else
	Init_SDL_ScreenSurface();
#endif
	if(screen == NULL)
	{
		printf(" Unable to set %dx%d video: %s\n", 320, 240, SDL_GetError());
		SDL_Quit();
		exit(1);
	}

	timerID = SDL_AddTimer(BUTTON_REPEAT_PERIOD_MS, my_callbackfunc, 0);		 
	if (NULL == timerID)
	{
		printf(" Unable to create timer\n");
		SDL_Quit();
		exit(1);
	}
	else
	{
		printf(" Timer created\n");
	}
	
	///SDL_WM_SetCaption("Luminary Grlib emulator", NULL);
	SDL_WM_SetCaption("Mitac professional GUI emulator", NULL);
	
	return;
}


//*****************************************************************************
//
//! Sets the callback function for touch screen emulation events.
//!
//! \param pfnCallback is a pointer to the function to be called when touch
//! screen events occur.
//!
//! This function sets the address of the function to be called when touch
//! screen events occur.  The events that are recognized are the screen being
//! touched (``pen down''), the touch position moving while the screen is
//! touched (``pen move''), and the screen no longer being touched (``pen
//! up'').
//!
//! \return None.
//
//*****************************************************************************
void SDLScreenCallbackSet(void (*pfnCallback)(unsigned long ulMessage, long lX, long lY))
{
	g_pfnTSHandler = pfnCallback;
}


//*****************************************************************************
//
//! Locks a surface for directly access..
//!
//! \param screen
//!
//! \return None.
//
//*****************************************************************************
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

//*****************************************************************************
//
//! Unlocks a surface for directly access..
//!
//! \param screen
//!
//! \return None.
//
//*****************************************************************************
void Unlock(SDL_Surface *screen)
{
	if ( SDL_MUSTLOCK(screen) )
	{
		SDL_UnlockSurface(screen);
	}
}

//*****************************************************************************
//
//! Unlocks a surface for directly access..
//!
//! \param screen
//! \param x coordinate
//! \param y coordinate
//! \param color
//!
//! \return None.
//
//*****************************************************************************
void DrawPixel(SDL_Surface *screen, int x, int y, unsigned long val)
{

	Uint8 R;
	Uint8 G;
	Uint8 B;

	// extract Red Green Blue colors
	R = (Uint8) (val >> 16) & 0xFF;
	G = (Uint8) (val >> 8 ) & 0xFF;
	B = (Uint8) (val >> 0 ) & 0xFF;

	Uint32 color = SDL_MapRGB(screen->format, R, G, B);
  
 
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


//*****************************************************************************
//
//! Draws a pixel on the screen.
//!
//! \param pvDisplayData is a pointer to the driver-specific data for this
//! display driver.
//! \param lX is the X coordinate of the pixel.
//! \param lY is the Y coordinate of the pixel.
//! \param ulValue is the color of the pixel.
//!
//! This function sets the given pixel to a particular color.  The coordinates
//! of the pixel are assumed to be within the extents of the display.
//!
//! \return None.
//
//*****************************************************************************
static void displayPixelDraw(void *pvDisplayData, long lX, long lY, unsigned long ulValue)
{
	printf("%u-%u-%u ",lX, lY, ulValue);
	Lock(screen);
	DrawPixel(screen, lX, lY, ulValue);
	Unlock(screen);
//	SDL_UpdateRect(screen, lX, lY, 1, 1);

	
}

//*****************************************************************************
//
//! Draws a horizontal sequence of pixels on the screen.
//!
//! \param pvDisplayData is a pointer to the driver-specific data for this
//! display driver.
//! \param lX is the X coordinate of the first pixel.
//! \param lY is the Y coordinate of the first pixel.
//! \param lX0 is sub-pixel offset within the pixel data, which is valid for 1
//! or 4 bit per pixel formats.
//! \param lCount is the number of pixels to draw.
//! \param lBPP is the number of bits per pixel; must be 1, 4, or 8.
//! \param pucData is a pointer to the pixel data.  For 1 and 4 bit per pixel
//! formats, the most significant bit(s) represent the left-most pixel.
//! \param pucPalette is a pointer to the palette used to draw the pixels.
//!
//! This function draws a horizontal sequence of pixels on the screen, using
//! the supplied palette.  For 1 bit per pixel format, the palette contains
//! pre-translated colors; for 4 and 8 bit per pixel formats, the palette
//! contains 24-bit RGB values that must be translated before being written to
//! the display.
//!
//! \return None.
//
//*****************************************************************************
static void displayDrawMultiple(void *pvDisplayData, long lX,
                                long lY, long lX0, long lCount,
                                long lBPP,
                                const unsigned char *pucData,
                                const unsigned char *pucPalette)
{
    unsigned long ulByte;
	
	unsigned long ulXRelative = lX;
	unsigned long ulYRelative = lY;
	unsigned long ulColor;

	Lock(screen);
    
	//
    // Determine how to interpret the pixel data based on the number of bits
    // per pixel.
    //
	
    switch(lBPP)
    {
        //
        // The pixel data is in 1 bit per pixel format.
        //
        case 1:
        {
            //
            // Loop while there are more pixels to draw.
            //
            while(lCount)
            {
                //
                // Get the next byte of image data.
                //
                ulByte = *pucData++;

                //
                // Loop through the pixels in this byte of image data.
                //
                for(; (lX0 < 8) && lCount; lX0++, lCount--)
                {
                    //
                    // Draw this pixel in the appropriate color.
                    // 
					ulColor = ((unsigned long *)pucPalette)[(ulByte >>(7 - lX0)) & 1];
					DrawPixel(screen, ulXRelative++, lY, ulColor);
                }

                //
                // Start at the beginning of the next byte of image data.
                //
                lX0 = 0;
            }

            //
            // The image data has been drawn.
            //
            break;
        }

        //
        // The pixel data is in 4 bit per pixel format.
        //
        case 4:
        {
            //
            // Loop while there are more pixels to draw.  "Duff's device" is
            // used to jump into the middle of the loop if the first nibble of
            // the pixel data should not be used.  Duff's device makes use of
            // the fact that a case statement is legal anywhere within a
            // sub-block of a switch statement.  See
            // http://en.wikipedia.org/wiki/Duff's_device for detailed
            // information about Duff's device.
            //
            switch(lX0 & 1)
            {
                case 0:
                    while(lCount)
                    {
                        //
                        // Get the upper nibble of the next byte of pixel data
                        // and extract the corresponding entry from the
                        // palette.
                        //
                        ulByte = (*pucData >> 4) * 3;
                        ulByte = (*(unsigned long *)(pucPalette + ulByte) &
                                  0x00ffffff);

                        //
                        // Translate this palette entry and write it to the
                        // screen.
                        //
                        DrawPixel(screen, ulXRelative++,  lY, ulByte);

                        //
                        // Decrement the count of pixels to draw.
                        //
                        lCount--;

                        //
                        // See if there is another pixel to draw.
                        //
                        if(lCount)
                        {
                case 1:
                            //
                            // Get the lower nibble of the next byte of pixel
                            // data and extract the corresponding entry from
                            // the palette.
                            //
                            ulByte = (*pucData++ & 15) * 3;
                            ulByte = (*(unsigned long *)(pucPalette + ulByte) &
                                      0x00ffffff);

                            //
                            // Translate this palette entry and write it to the
                            // screen.
                            //
							DrawPixel(screen, ulXRelative++, lY, ulByte);

                            //
                            // Decrement the count of pixels to draw.
                            //
                            lCount--;
                        }
                    }
            }

            //
            // The image data has been drawn.
            //
            break;
        }

        //
        // The pixel data is in 8 bit per pixel format.
        //
        case 8:
        {
            //
            // Loop while there are more pixels to draw.
            //
            while(lCount--)
            {
                //
                // Get the next byte of pixel data and extract the
                // corresponding entry from the palette.
                //
                ulByte = *pucData++ * 3;
                ulByte = *(unsigned long *)(pucPalette + ulByte) & 0x00ffffff;
				
				DrawPixel(screen, ulXRelative++,  lY, ulByte);
            }
			

            //
            // The image data has been drawn.
            //
            break;
        }
    }
	Unlock(screen);	
}

//*****************************************************************************
//
//! Draws a horizontal line.
//!
//! \param pvDisplayData is a pointer to the driver-specific data for this
//! display driver.
//! \param lX1 is the X coordinate of the start of the line.
//! \param lX2 is the X coordinate of the end of the line.
//! \param lY is the Y coordinate of the line.
//! \param ulValue is the color of the line.
//!
//! This function draws a horizontal line on the display.  The coordinates of
//! the line are assumed to be within the extents of the display.
//!
//! \return None.
//
//*****************************************************************************
static void displayLineDrawH(void *pvDisplayData, long lX1, long lX2,
                             long lY, unsigned long ulValue)
{
	SDL_Rect rect;
	
	// line over screen not allowed
	if(lX1 > lX2)
	{
		printf("err: LineDrawH cannot paint out of screen");
		return;
	}
	

    rect.x = (short)lX1;
	rect.y = (short)lY;
	// count from zero.. How many pixels from 0..319? 319-0 = 319 but we need 320
	rect.w = (unsigned short)(lX2 - lX1) + 1;
	rect.h = 1;
	
	SDL_FillRect(screen, &rect, ulValue);
	
	//SDL_Flip(screen);
}

//*****************************************************************************
//
//! Draws a vertical line.
//!
//! \param pvDisplayData is a pointer to the driver-specific data for this
//! display driver.
//! \param lX is the X coordinate of the line.
//! \param lY1 is the Y coordinate of the start of the line.
//! \param lY2 is the Y coordinate of the end of the line.
//! \param ulValue is the color of the line.
//!
//! This function draws a vertical line on the display.  The coordinates of the
//! line are assumed to be within the extents of the display.
//!
//! \return None.
//
//*****************************************************************************
static void displayLineDrawV(void *pvDisplayData, long lX, long lY1,
                             long lY2, unsigned long ulValue)
{
	SDL_Rect rect;

	// line over screen not allowed
	if(lY1 > lY2)
	{
		printf("err: LineDrawV cannot paint out of screen");
		return;
	}   
	

    rect.x = (short)lX;
	rect.y = (short)lY1;
	rect.w = 1;
	// count from zero.. How many pixels from 0..319? 319-0 = 319 but we need 320
	rect.h = (unsigned short)(lY2 - lY1) + 1;
	
	SDL_FillRect(screen, &rect, ulValue);
	
	//SDL_Flip(screen);
}

//*****************************************************************************
//
//! Fills a rectangle.
//!
//! \param pvDisplayData is a pointer to the driver-specific data for this
//! display driver.
//! \param pRect is a pointer to the structure describing the rectangle.
//! \param ulValue is the color of the rectangle.
//!
//! This function fills a rectangle on the display.  The coordinates of the
//! rectangle are assumed to be within the extents of the display, and the
//! rectangle specification is fully inclusive (in other words, both sXMin and
//! sXMax are drawn, along with sYMin and sYMax).
//!
//! \return None.
//
//*****************************************************************************
static void displayRectFill(void *pvDisplayData,
							const tRectangle *pRect,
                            unsigned long ulValue)
{
    SDL_Rect rect;

    rect.x = pRect->sXMin;
	rect.y = pRect->sYMin;
	rect.w = pRect->sXMax - pRect->sXMin + 1;
	rect.h = pRect->sYMax - pRect->sYMin + 1;
	
	SDL_FillRect(screen, &rect, ulValue);
	
	//SDL_Flip(screen);
	
}

//*****************************************************************************
//
//! Translates a 24-bit RGB color to a display driver-specific color.
//
//*****************************************************************************
static unsigned long displayTranslate(void *pvDisplayData, unsigned long ulValue)
{
    return(ulValue);
}

//*****************************************************************************
//
//! Flushes any cached drawing operations.
//!
//! \param pvDisplayData is a pointer to the driver-specific data for this
//! display driver.
//!
//! This functions flushes any cached drawing operations to the display.  This
//! is useful when a local frame buffer is used for drawing operations, and the
//! flush would copy the local frame buffer to the display. 
//!
//! \return None.
//
//*****************************************************************************
static void displayFlush(void *pvDisplayData)
{
    
	SDL_Flip(screen);

}

Uint32 my_callbackfunc(Uint32 interval, void *param)
{	
	int x;
	int y;
	SDL_Event user_event;
	SDL_Event events;

	user_event.type=SDL_USEREVENT;
	user_event.user.code=2;
	user_event.user.data1=NULL;
	user_event.user.data2=NULL;

	if(true == buttDown)
	{
		Uint8 buttons= SDL_GetMouseState(&x, &y);
		
		buttons &= SDL_BUTTON(1) | SDL_BUTTON(2) | SDL_BUTTON(3) ;

		if(g_pfnTSHandler && buttons)
		{
			
			// Send the pen up message to the touch screen event handler.
		   g_pfnTSHandler(WIDGET_MSG_PTR_MOVE, x, y);
	
			// simply said, this does not work
			if (0 == SDL_PeepEvents(&events, 1, SDL_PEEKEVENT, SDL_ALLEVENTS))
			{
				SDL_PushEvent(&user_event);
			}
			/*
			WidgetMessageQueueProcess();
			displayFlush(NULL);
			*/
		}			
	}
	return(interval);
}


//*****************************************************************************
//
//! Handles events from keyboard and mouse.
//!
//! \param None.
//!
//! Esc causes exit, F1 displays on std output debug messages about 
//! widget message queue
//!
//! \return true if user exits application.
//
//*****************************************************************************
bool SDLProcessEvent(void)
{
	SDL_Event event;		
	
	while(SDL_PollEvent(&event))
	{
		switch(event.type)
		{
			case SDL_KEYDOWN:
				switch(event.key.keysym.sym)
				{
						case SDLK_ESCAPE:
							SDL_RemoveTimer(timerID);
							return false;				
							break;

						default:
							break;
				
						case SDLK_F1:
#if defined(WIN32)
							;
#else				
// 1. All messages discarded due to queue overflow (g_ulMQOverflow)
// 2. Messages other than WIDGET_MSG_PTR_MOVE discarded due to queue
//    overflow (g_ulMQNonMouseOverflow).  In this case, we also remember the
//    last message that was discarded (g_ulMQLastLostMsg).
// 3. The number of calls to WidgetMessageQueueAdd that fail due to the queue
//    mutex already being held.
// 4. The number of cases where WidgetMessageQueueAdd reused an unread
//    WIDGET_MSG_PTR_MOVE message when a second one arrived before the previous
//    one had been processed.
//
//*****************************************************************************
					printf("g_ulMQOverflow %u\n", g_ulMQOverflow); 
					printf("g_ulMQNonMouseOverflow %u\n", g_ulMQNonMouseOverflow); 
					printf("g_ulMQLastLostMsg %u\n",g_ulMQLastLostMsg); 
					printf("g_ulMQMutexClash %u\n",g_ulMQMutexClash); 
					printf("g_ulMQMoveOverwrite %u\n", g_ulMQMoveOverwrite); 
					printf("-------------------------------\n"); 
					
					fflush(stdout);
#endif				
				} //switch(event.key.keysym.sym)
				break;

			case SDL_MOUSEBUTTONDOWN:
				printf("SDL_MOUSEBUTTONDOWN %ux%u", event.button.x, event.button.y); fflush(stdout);
				buttDown = true;
				
				
				if(g_pfnTSHandler)
				{
						// Send the pen up message to the touch screen event handler.
					   g_pfnTSHandler(WIDGET_MSG_PTR_DOWN, event.button.x, event.button.y);
				}			   			
				break;

			case SDL_MOUSEBUTTONUP:
				printf("SDL_MOUSEBUTTONUP %ux%u", event.button.x, event.button.y); fflush(stdout);		
				buttDown = false;

				if(g_pfnTSHandler)
				{
						// Send the pen up message to the touch screen event handler.
					   g_pfnTSHandler(WIDGET_MSG_PTR_UP, event.button.x, event.button.y);
				}
				break;

			case SDL_MOUSEMOTION: 
				printf("MOUSEMOTION %u,%u", event.motion.x, event.motion.y); fflush(stdout); 
				if(g_pfnTSHandler)
				{
						// Send the pen up message to the touch screen event handler.
					   g_pfnTSHandler(WIDGET_MSG_PTR_MOVE, event.button.x, event.button.y);
				}
				break; 
			case SDL_USEREVENT: 
				//printf("user event"); fflush(stdout); 
				//printf("%u", buttDown); fflush(stdout); 
				/*if(g_pfnTSHandler)
				{
				}
				*/	
				break; 		

			case SDL_QUIT:
				return false;
				break;

			default:				
					break;
		} //switch(event.type)	
		
	} // while(SDL_PollEvent(&event))

	displayFlush(NULL);

	return true;
} // bool SDLProcessEvent(void)

#if defined(WIN32)
//*****************************************************************************
//
//! Handles events from keyboard and mouse.
//!
//! \param None.
//!
//! Esc causes exit, F1 displays on std output debug messages about 
//! widget message queue
//!
//! \return true if user exits application.
//
//*****************************************************************************
bool SDLProcessEventWIN32(void)
{
	SDL_Event event;		
	
	while(SDL_PollEvent(&event))
	{
		switch(event.type)
		{
			case SDL_KEYDOWN:
				switch(event.key.keysym.sym)
				{
						case SDLK_ESCAPE:
							SDL_RemoveTimer(timerID);
							return false;				
							break;

						default:
							break;

#if defined(WIN32)
						case SDLK_0:
						case SDLK_1:
						case SDLK_2:
						case SDLK_3:
						case SDLK_4:
						case SDLK_5:
						case SDLK_6:
						case SDLK_7:
						case SDLK_8:
						case SDLK_9:
						case SDLK_y:
						case SDLK_u:
						case SDLK_i:
						case SDLK_o:
						case SDLK_h:
						case SDLK_j:
						case SDLK_k:
						case SDLK_l:
						case SDLK_RSHIFT:
						case SDLK_LSHIFT:
							///printf("  Number key %d %d\n", event.key.keysym.sym-KB_KEY_0, event.key.keysym.sym);
							if(g_pfnTSHandler)
							{
								long kbdinfo = ((event.key.keysym.mod << 16)&KB_MOD_MASK)|(event.key.keysym.sym&KB_CODE_MASK);
								// Send the PC keybd message to the io event handler.
								///g_pfnTSHandler(WIDGET_MSG_KEY_DOWN, 0, event.key.keysym.sym);
								g_pfnTSHandler(WIDGET_MSG_KEY_DOWN, 0, kbdinfo);
							}
							break;
						
#endif
							
						case SDLK_F1:
#if defined(WIN32)
							;
#else
// 1. All messages discarded due to queue overflow (g_ulMQOverflow)
// 2. Messages other than WIDGET_MSG_PTR_MOVE discarded due to queue
//    overflow (g_ulMQNonMouseOverflow).  In this case, we also remember the
//    last message that was discarded (g_ulMQLastLostMsg).
// 3. The number of calls to WidgetMessageQueueAdd that fail due to the queue
//    mutex already being held.
// 4. The number of cases where WidgetMessageQueueAdd reused an unread
//    WIDGET_MSG_PTR_MOVE message when a second one arrived before the previous
//    one had been processed.
//
//*****************************************************************************
					printf("g_ulMQOverflow %u\n", g_ulMQOverflow); 
					printf("g_ulMQNonMouseOverflow %u\n", g_ulMQNonMouseOverflow); 
					printf("g_ulMQLastLostMsg %u\n",g_ulMQLastLostMsg); 
					printf("g_ulMQMutexClash %u\n",g_ulMQMutexClash); 
					printf("g_ulMQMoveOverwrite %u\n", g_ulMQMoveOverwrite); 
					printf("-------------------------------\n"); 
					
					fflush(stdout);
#endif				
				} //switch(event.key.keysym.sym)
				break;

			case SDL_MOUSEBUTTONDOWN:
				//printf("SDL_MOUSEBUTTONDOWN %ux%u", event.button.x, event.button.y);
				fflush(stdout);
				buttDown = true;
				
				
				if(g_pfnTSHandler)
				{
					// Send the pen up message to the touch screen event handler.
					g_pfnTSHandler(WIDGET_MSG_PTR_DOWN, event.button.x, event.button.y);
				}			   			
				break;

			case SDL_MOUSEBUTTONUP:
				//printf("SDL_MOUSEBUTTONUP %ux%u", event.button.x, event.button.y);
				fflush(stdout);		
				buttDown = false;

				if(g_pfnTSHandler)
				{
					// Send the pen up message to the touch screen event handler.
					g_pfnTSHandler(WIDGET_MSG_PTR_UP, event.button.x, event.button.y);
				}
				break;

			case SDL_MOUSEMOTION: 
				//printf("MOUSEMOTION %u,%u", event.motion.x, event.motion.y); fflush(stdout); 
				if(g_pfnTSHandler)
				{
					// Send the pen up message to the touch screen event handler.
					g_pfnTSHandler(WIDGET_MSG_PTR_MOVE, event.button.x, event.button.y);
				}
				break; 
			case SDL_USEREVENT: 
				//printf("user event"); fflush(stdout); 
				//printf("%u", buttDown); fflush(stdout); 
				/*if(g_pfnTSHandler)
				{
				}
				*/	
				break; 		

			case SDL_QUIT:
				return false;
				break;

			default:				
					break;
		} //switch(event.type)	
		
	} // while(SDL_PollEvent(&event))

	displayFlush(NULL);

	return true;
} // bool SDLProcessEvent(void)
#endif



void SDLDestroy()
{
	if (NULL != timerID)
	{
		SDL_RemoveTimer(timerID);
		printf("Timer removed\n"); fflush(stdout); 
	}
	SDL_Quit();
}

//*****************************************************************************
//
//! The display structure that describes the driver for the LibSDL driver.
//
//*****************************************************************************
#if defined(WIN32)
extern "C" void TransIOMsg(unsigned long ulMessage, long lX, long lY);
extern "C" void GUI_X_Delay( int );
///extern "C" INT8U OSTimeDlyHMSM(INT8U hours, INT8U minutes, INT8U seconds, INT16U ms);
void SDLIODriver( void * pvParameters )
{
	bool done = false;

	SDLInit();	
	SDLScreenCallbackSet(TransIOMsg);	

	while(!done)
	{
		done = !SDLProcessEventWIN32();			
		///
		///printf("\nIn SDLIODriver\n");
		///vTaskDelay(1);
		///OSTimeDlyHMSM(0, 0, 0, 1);
		///GUI_X_Delay( 10 );
		GUI_X_Delay( 1 );
	}
	SDLDestroy();
	while(1)
	{;}	
}

#else
const tDisplay SDLDisplay =
{
    sizeof(tDisplay),
    0,
    144,///320,
    168,///240,
    displayPixelDraw,
    displayDrawMultiple,
    displayLineDrawH,
    displayLineDrawV,
    displayRectFill,
    displayTranslate,
    displayFlush
};
#endif


