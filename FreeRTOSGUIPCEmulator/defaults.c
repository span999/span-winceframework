



///#include <stdio.h>
///#include <stdlib.h>


#include <FreeRTOS.h>
#include <task.h>

///include this for grlib
///#include "grlib/grlib_set.h"

/// include this for PIC graphic lib
#include "PICgraphic_set.h"

///#include "SDLDislpayEmulator/SDLDisplay.h"

#ifndef _WINGDI_
	#define _WINGDI_
#endif	

///#include "GenericTypeDefs.h"
///#include "Graphics\Primitive.h"
///#include "Graphics\Graphics.h"
///#include "Graphics\GOL.h"


void vApplicationIdleHook()
{
	Sleep(INFINITE);	// to reduce processor usage

}

#if 0
WORD GOLDrawCallback()
{
	return 0;
}

WORD GOLMsgCallback(WORD objMsg, OBJ_HEADER* pObj, GOL_MSG* pMsg)
{
	return 0;
}
#endif


#define MIN(x,y)                ((x > y)? y: x)
#define WAIT_UNTIL_FINISH(x)    while(!x)	
#define DEMODELAY				1000
extern const char Font25[];

void ArcTest( void )
{
    SHORT       width, height;
    SHORT       counter;
    const SHORT polyPoints[] = {
        (GetMaxX()+1)/2,    (GetMaxY()+1)/4,
        (GetMaxX()+1)*3/4,  (GetMaxY()+1)/2,
        (GetMaxX()+1)/2,    (GetMaxY()+1)*3/4,
        (GetMaxX()+1)/4,    (GetMaxY()+1)/2,
        (GetMaxX()+1)/2,    (GetMaxY()+1)/4,
    };

	
	
	    // draw border lines to show the limits of the 
	    // left, right, top and bottom pixels of the screen
	    // draw the top most horizontal line
        SetColor(BRIGHTRED);
        WAIT_UNTIL_FINISH(Line(0,0,GetMaxX(),0));
        // draw the right most vertical line
        SetColor(BRIGHTYELLOW);
        WAIT_UNTIL_FINISH(Line(GetMaxX(),0,GetMaxX(),GetMaxY()));
	    // draw the bottom most horizontal line
        SetColor(BRIGHTGREEN);
        WAIT_UNTIL_FINISH(Line(0,GetMaxY(),GetMaxX(),GetMaxY()));
        // draw the left most vertical line
        SetColor(BRIGHTBLUE);
        WAIT_UNTIL_FINISH(Line(0,0,0,GetMaxY()));

        DelayMs(DEMODELAY);
		
		// draw WHITE lines intersecting in the middle of the screen
        SetColor(WHITE);
        for(counter = 0; counter < GetMaxX(); counter += 20)
        {
            WAIT_UNTIL_FINISH(Line(counter, 0, GetMaxX() - 1 - counter, GetMaxY() - 1));
        }

        DelayMs(DEMODELAY);
		
		// draw concentric RED circles in the middle of the screen
        SetColor(BRIGHTRED);
        for(counter = 10; counter < MIN(GetMaxX(), GetMaxY()) >> 1; counter += 10)
        {
            WAIT_UNTIL_FINISH(Circle(GetMaxX() >> 1, GetMaxY() >> 1, counter));
        }

        DelayMs(DEMODELAY);	
		
		// draw concentric filled circles in the middle of the screen
        SetColor(BRIGHTBLUE);
        WAIT_UNTIL_FINISH(FillCircle(GetMaxX() >> 1, GetMaxY() >> 1, 60));
        SetColor(BRIGHTGREEN);
        WAIT_UNTIL_FINISH(FillCircle(GetMaxX() >> 1, GetMaxY() >> 1, 40));
        SetColor(BRIGHTRED);
        WAIT_UNTIL_FINISH(FillCircle(GetMaxX() >> 1, GetMaxY() >> 1, 20));

        ///DelayMs(DEMODELAY);

        SetColor(BLACK);
        ClearDevice();

		// draw concentric beveled objects in the middle of the screen
        SetColor(BRIGHTBLUE);
        WAIT_UNTIL_FINISH(Bevel((GetMaxX() >> 1) - 60, (GetMaxY() >> 1) - 60, (GetMaxX() >> 1) + 60, (GetMaxY() >> 1) + 60, 30));
        SetColor(BRIGHTGREEN);
        WAIT_UNTIL_FINISH(Bevel((GetMaxX() >> 1) - 40, (GetMaxY() >> 1) - 40, (GetMaxX() >> 1) + 40, (GetMaxY() >> 1) + 40, 30));
        SetColor(BRIGHTRED);
        WAIT_UNTIL_FINISH(Bevel((GetMaxX() >> 1) - 20, (GetMaxY() >> 1) - 20, (GetMaxX() >> 1) + 20, (GetMaxY() >> 1) + 20, 30));

        DelayMs(DEMODELAY);

        SetColor(BLACK);
        ClearDevice();

		// draw concentric thick beveled objects in the middle of the screen
        SetColor(BRIGHTBLUE);
        WAIT_UNTIL_FINISH
        (
            _Arc_
                (
                    (GetMaxX() >> 1) - 60,
                    (GetMaxY() >> 1) - 60,
                    (GetMaxX() >> 1) + 60,
                    (GetMaxY() >> 1) + 60,
                    20,
                    30,
                    0xFF
                )
        );
        SetColor(BRIGHTGREEN);
        WAIT_UNTIL_FINISH
        (
            _Arc_
                (
                    (GetMaxX() >> 1) - 40,
                    (GetMaxY() >> 1) - 40,
                    (GetMaxX() >> 1) + 40,
                    (GetMaxY() >> 1) + 40,
                    20,
                    30,
                    0xFF
                )
        );
        SetColor(BRIGHTRED);
        WAIT_UNTIL_FINISH
        (
            _Arc_
                (
                    (GetMaxX() >> 1) - 20,
                    (GetMaxY() >> 1) - 20,
                    (GetMaxX() >> 1) + 20,
                    (GetMaxY() >> 1) + 20,
                    20,
                    30,
                    0xFF
                )
        );

        DelayMs(DEMODELAY);

		// draw concentric filled beveled objects in the middle of the screen
        SetColor(BRIGHTBLUE);
        WAIT_UNTIL_FINISH
        (
            FillBevel
                (
                    (GetMaxX() >> 1) - 60,
                    (GetMaxY() >> 1) - 60,
                    (GetMaxX() >> 1) + 60,
                    (GetMaxY() >> 1) + 60,
                    30
                )
        );
        SetColor(BRIGHTGREEN);
        WAIT_UNTIL_FINISH
        (
            FillBevel
                (
                    (GetMaxX() >> 1) - 40,
                    (GetMaxY() >> 1) - 40,
                    (GetMaxX() >> 1) + 40,
                    (GetMaxY() >> 1) + 40,
                    30
                )
        );
        SetColor(BRIGHTRED);
        WAIT_UNTIL_FINISH
        (
            FillBevel
                (
                    (GetMaxX() >> 1) - 20,
                    (GetMaxY() >> 1) - 20,
                    (GetMaxX() >> 1) + 20,
                    (GetMaxY() >> 1) + 20,
                    30
                )
        );

        DelayMs(DEMODELAY);

        SetColor(BLACK);
        ClearDevice();

		// draw concentric thick beveled objects in the middle of the screen
        SetColor(BRIGHTBLUE);
        WAIT_UNTIL_FINISH(_Arc_((GetMaxX() >> 1), (GetMaxY() >> 1) - 50, (GetMaxX() >> 1), (GetMaxY() >> 1) + 50, 50, 60, 0x11));
        SetColor(BRIGHTGREEN);
        WAIT_UNTIL_FINISH(_Arc_((GetMaxX() >> 1), (GetMaxY() >> 1) - 50, (GetMaxX() >> 1), (GetMaxY() >> 1) + 50, 50, 60, 0x22));
        SetColor(BRIGHTRED);
        WAIT_UNTIL_FINISH(_Arc_((GetMaxX() >> 1), (GetMaxY() >> 1) - 50, (GetMaxX() >> 1), (GetMaxY() >> 1) + 50, 50, 60, 0x44));
        SetColor(BRIGHTYELLOW);
        WAIT_UNTIL_FINISH(_Arc_((GetMaxX() >> 1), (GetMaxY() >> 1) - 50, (GetMaxX() >> 1), (GetMaxY() >> 1) + 50, 50, 60, 0x88));

        SetColor(BRIGHTBLUE);
        WAIT_UNTIL_FINISH(_Arc_((GetMaxX() >> 1), (GetMaxY() >> 1) - 30, (GetMaxX() >> 1), (GetMaxY() >> 1) + 30, 35, 45, 0x11));
        SetColor(BRIGHTGREEN);
        WAIT_UNTIL_FINISH(_Arc_((GetMaxX() >> 1), (GetMaxY() >> 1) - 30, (GetMaxX() >> 1), (GetMaxY() >> 1) + 30, 35, 45, 0x22));
        SetColor(BRIGHTRED);
        WAIT_UNTIL_FINISH(_Arc_((GetMaxX() >> 1), (GetMaxY() >> 1) - 30, (GetMaxX() >> 1), (GetMaxY() >> 1) + 30, 35, 45, 0x44));
        SetColor(BRIGHTYELLOW);
        WAIT_UNTIL_FINISH(_Arc_((GetMaxX() >> 1), (GetMaxY() >> 1) - 30, (GetMaxX() >> 1), (GetMaxY() >> 1) + 30, 35, 45, 0x88));

        SetColor(BRIGHTBLUE);
        WAIT_UNTIL_FINISH(_Arc_((GetMaxX() >> 1), (GetMaxY() >> 1), (GetMaxX() >> 1), (GetMaxY() >> 1), 20, 30, 0x11));
        SetColor(BRIGHTGREEN);
        WAIT_UNTIL_FINISH(_Arc_((GetMaxX() >> 1), (GetMaxY() >> 1), (GetMaxX() >> 1), (GetMaxY() >> 1), 20, 30, 0x22));
        SetColor(BRIGHTRED);
        WAIT_UNTIL_FINISH(_Arc_((GetMaxX() >> 1), (GetMaxY() >> 1), (GetMaxX() >> 1), (GetMaxY() >> 1), 20, 30, 0x44));
        SetColor(BRIGHTYELLOW);
        WAIT_UNTIL_FINISH(_Arc_((GetMaxX() >> 1), (GetMaxY() >> 1), (GetMaxX() >> 1), (GetMaxY() >> 1), 20, 30, 0x88));

        DelayMs(DEMODELAY);

        SetColor(BLACK);
        ClearDevice();


		// draw rectangles in the middle of the screen
        SetColor(BRIGHTBLUE);
        for(counter = 0; counter < MIN(GetMaxX(), GetMaxY()) >> 1; counter += 20)
        {
            WAIT_UNTIL_FINISH
            (
                Rectangle
                    (
                        GetMaxX() / 2 - counter,
                        GetMaxY() / 2 - counter,
                        GetMaxX() / 2 + counter,
                        GetMaxY() / 2 + counter
                    )
            );
        }

        DelayMs(DEMODELAY);

        SetColor(BRIGHTBLUE);
        WAIT_UNTIL_FINISH(Bar(GetMaxX() / 2 - 80, GetMaxY() / 2 - 80, GetMaxX() / 2 + 80, GetMaxY() / 2 + 80));
        SetColor(BRIGHTGREEN);
        WAIT_UNTIL_FINISH(Bar(GetMaxX() / 2 - 60, GetMaxY() / 2 - 60, GetMaxX() / 2 + 60, GetMaxY() / 2 + 60));
        SetColor(BRIGHTRED);
        WAIT_UNTIL_FINISH(Bar(GetMaxX() / 2 - 40, GetMaxY() / 2 - 40, GetMaxX() / 2 + 40, GetMaxY() / 2 + 40));

        DelayMs(DEMODELAY);
        SetColor(BLACK);
        ClearDevice();

		// draw ploygon shape in the middle of the screen
        SetColor(WHITE);
        WAIT_UNTIL_FINISH(DrawPoly(5, (SHORT *)polyPoints));

        DelayMs(DEMODELAY);
        SetColor(BLACK);
        ClearDevice();

		// draw fonts in the screen
        SetFont((void *) &Font25);
        SetColor(BRIGHTGREEN);
        width = GetTextWidth("Microchip Technology Inc.", (void *) &Font25);
        height = GetTextHeight((void *) &Font25);

        OutTextXY((GetMaxX() - width) >> 1, (GetMaxY() - height) >> 1, "Microchip Technology Inc.");

        DelayMs(DEMODELAY);
        SetColor(BLACK);
        ClearDevice();

///        SetFont((void *) &Font35);
        SetColor(WHITE);
///        width = GetTextWidth("Microchip Tech.", (void *) &Font35);
///        height = GetTextHeight((void *) &Font35);

///        OutTextXY((GetMaxX() - width) >> 1, (GetMaxY() - height) >> 1, "Microchip Tech.");

        DelayMs(DEMODELAY);
        SetColor(BLACK);
        ClearDevice();

		// draw pictures in the screen with different bits per pixel
///        WAIT_UNTIL_FINISH(PutImage(0, 0, (void *) &flower1bit, 2));
        SetColor(WHITE);
///        OutTextXY(200, 0, "1BPP");
        DelayMs(DEMODELAY);
        SetColor(BLACK);
        ClearDevice();

///        WAIT_UNTIL_FINISH(PutImage(0, 0, (void *) &flower4bit, 2));
        SetColor(WHITE);
///        OutTextXY(200, 0, "4BPP");
        DelayMs(DEMODELAY);
        SetColor(BLACK);
        ClearDevice();

///        WAIT_UNTIL_FINISH(PutImage(0, 0, (void *) &flower8bit, 2));
        SetColor(WHITE);
///       OutTextXY(200, 0, "8BPP");
        DelayMs(DEMODELAY);
        SetColor(BLACK);
        ClearDevice();

///        WAIT_UNTIL_FINISH(PutImage(0, 0, (void *) &flower16bit, 2));
        SetColor(WHITE);
///        OutTextXY(200, 0, "16BPP");
        DelayMs(DEMODELAY);
        SetColor(BLACK);
        ClearDevice();

///        width = GetImageWidth((void *) &flower1bit);
///        height = GetImageHeight((void *) &flower1bit);

///        WAIT_UNTIL_FINISH(PutImage((GetMaxX() + 1) / 2 - width  , (GetMaxY() + 1) / 2 - height  , (void *) &flower1bit,  1));
///        WAIT_UNTIL_FINISH(PutImage((GetMaxX() + 1) / 2          , (GetMaxY() + 1) / 2 - height  , (void *) &flower4bit,  1));
///        WAIT_UNTIL_FINISH(PutImage((GetMaxX() + 1) / 2 - width  , (GetMaxY() + 1) / 2           , (void *) &flower8bit,  1));
///        WAIT_UNTIL_FINISH(PutImage((GetMaxX() + 1) / 2          , (GetMaxY() + 1) / 2           , (void *) &flower16bit, 1));
        DelayMs(DEMODELAY*2);
        SetColor(BLACK);
        ClearDevice();

		
		
		
		
		
		
		
		
}

