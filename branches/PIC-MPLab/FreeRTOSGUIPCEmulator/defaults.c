



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
#include "FontGentium.h"


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

extern const IMAGE_FLASH flower1bit;
extern const IMAGE_FLASH flower4bit;
extern const IMAGE_FLASH flower8bit;
extern const IMAGE_FLASH flower16bit;

void PrimitiveTest( void )
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
///return;		
		// draw concentric RED circles in the middle of the screen
        SetColor(BRIGHTRED);
        for(counter = 10; counter < MIN(GetMaxX(), GetMaxY()) >> 1; counter += 10)
        {
            WAIT_UNTIL_FINISH(Circle(GetMaxX() >> 1, GetMaxY() >> 1, counter));
        }

        DelayMs(DEMODELAY);	
///return;			
		// draw concentric filled circles in the middle of the screen
        SetColor(BRIGHTBLUE);
        WAIT_UNTIL_FINISH(FillCircle(GetMaxX() >> 1, GetMaxY() >> 1, 60));
        SetColor(BRIGHTGREEN);
        WAIT_UNTIL_FINISH(FillCircle(GetMaxX() >> 1, GetMaxY() >> 1, 40));
        SetColor(BRIGHTRED);
        WAIT_UNTIL_FINISH(FillCircle(GetMaxX() >> 1, GetMaxY() >> 1, 20));

        DelayMs(DEMODELAY);
///return;	
        SetColor(BLACK);
        ClearDevice();

		// draw concentric beveled objects in the middle of the screen
///#define BEVEL_STEP	10
#define BEVEL_STEP	5
		if( ((MIN( GetMaxX(), GetMaxY() ) >> 1) - BEVEL_STEP*6) > 0 )
		{
        SetColor(BRIGHTBLUE);
        WAIT_UNTIL_FINISH(Bevel((GetMaxX() >> 1) - BEVEL_STEP*6, (GetMaxY() >> 1) - BEVEL_STEP*6, (GetMaxX() >> 1) + BEVEL_STEP*6, (GetMaxY() >> 1) + BEVEL_STEP*6, BEVEL_STEP*3));
        }
		if( ((MIN( GetMaxX(), GetMaxY() ) >> 1) - BEVEL_STEP*4) > 0 )
		{
		SetColor(BRIGHTGREEN);
        WAIT_UNTIL_FINISH(Bevel((GetMaxX() >> 1) - BEVEL_STEP*4, (GetMaxY() >> 1) - BEVEL_STEP*4, (GetMaxX() >> 1) + BEVEL_STEP*4, (GetMaxY() >> 1) + BEVEL_STEP*4, BEVEL_STEP*3));
		}
		if( ((MIN( GetMaxX(), GetMaxY() ) >> 1) - BEVEL_STEP*2) > 0 )
		{
        SetColor(BRIGHTRED);
        WAIT_UNTIL_FINISH(Bevel((GetMaxX() >> 1) - BEVEL_STEP*2, (GetMaxY() >> 1) - BEVEL_STEP*2, (GetMaxX() >> 1) + BEVEL_STEP*2, (GetMaxY() >> 1) + BEVEL_STEP*2, BEVEL_STEP*3));
		}
		
        DelayMs(DEMODELAY);
///return;	
        SetColor(BLACK);
        ClearDevice();

///#define ARC_STEP 10
#define ARC_STEP 5
		// draw concentric thick beveled objects in the middle of the screen
        SetColor(BRIGHTBLUE);
        WAIT_UNTIL_FINISH
        (
            _Arc_
                (
                    (GetMaxX() >> 1) - ARC_STEP*6,
                    (GetMaxY() >> 1) - ARC_STEP*6,
                    (GetMaxX() >> 1) + ARC_STEP*6,
                    (GetMaxY() >> 1) + ARC_STEP*6,
                    ARC_STEP*2,
                    ARC_STEP*3,
                    0xFF
                )
        );
        SetColor(BRIGHTGREEN);
        WAIT_UNTIL_FINISH
        (
            _Arc_
                (
                    (GetMaxX() >> 1) - ARC_STEP*4,
                    (GetMaxY() >> 1) - ARC_STEP*4,
                    (GetMaxX() >> 1) + ARC_STEP*4,
                    (GetMaxY() >> 1) + ARC_STEP*4,
                    ARC_STEP*2,
                    ARC_STEP*3,
                    0xFF
                )
        );
        SetColor(BRIGHTRED);
        WAIT_UNTIL_FINISH
        (
            _Arc_
                (
                    (GetMaxX() >> 1) - ARC_STEP*2,
                    (GetMaxY() >> 1) - ARC_STEP*2,
                    (GetMaxX() >> 1) + ARC_STEP*2,
                    (GetMaxY() >> 1) + ARC_STEP*2,
                    ARC_STEP*2,
                    ARC_STEP*3,
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
                    (GetMaxX() >> 1) - BEVEL_STEP*6,
                    (GetMaxY() >> 1) - BEVEL_STEP*6,
                    (GetMaxX() >> 1) + BEVEL_STEP*6,
                    (GetMaxY() >> 1) + BEVEL_STEP*6,
                    BEVEL_STEP*3
                )
        );
        SetColor(BRIGHTGREEN);
        WAIT_UNTIL_FINISH
        (
            FillBevel
                (
                    (GetMaxX() >> 1) - BEVEL_STEP*4,
                    (GetMaxY() >> 1) - BEVEL_STEP*4,
                    (GetMaxX() >> 1) + BEVEL_STEP*4,
                    (GetMaxY() >> 1) + BEVEL_STEP*4,
                    BEVEL_STEP*3
                )
        );
        SetColor(BRIGHTRED);
        WAIT_UNTIL_FINISH
        (
            FillBevel
                (
                    (GetMaxX() >> 1) - BEVEL_STEP*2,
                    (GetMaxY() >> 1) - BEVEL_STEP*2,
                    (GetMaxX() >> 1) + BEVEL_STEP*2,
                    (GetMaxY() >> 1) + BEVEL_STEP*2,
                    BEVEL_STEP*3
                )
        );

        DelayMs(DEMODELAY);

        SetColor(BLACK);
        ClearDevice();

		// draw concentric thick beveled objects in the middle of the screen
        SetColor(BRIGHTBLUE);
        WAIT_UNTIL_FINISH(_Arc_((GetMaxX() >> 1), (GetMaxY() >> 1) - ARC_STEP*5, (GetMaxX() >> 1), (GetMaxY() >> 1) + ARC_STEP*5, ARC_STEP*5, ARC_STEP*6, 0x11));
        SetColor(BRIGHTGREEN);
        WAIT_UNTIL_FINISH(_Arc_((GetMaxX() >> 1), (GetMaxY() >> 1) - ARC_STEP*5, (GetMaxX() >> 1), (GetMaxY() >> 1) + ARC_STEP*5, ARC_STEP*5, ARC_STEP*6, 0x22));
        SetColor(BRIGHTRED);
        WAIT_UNTIL_FINISH(_Arc_((GetMaxX() >> 1), (GetMaxY() >> 1) - ARC_STEP*5, (GetMaxX() >> 1), (GetMaxY() >> 1) + ARC_STEP*5, ARC_STEP*5, ARC_STEP*6, 0x44));
        SetColor(BRIGHTYELLOW);
        WAIT_UNTIL_FINISH(_Arc_((GetMaxX() >> 1), (GetMaxY() >> 1) - ARC_STEP*5, (GetMaxX() >> 1), (GetMaxY() >> 1) + ARC_STEP*5, ARC_STEP*5, ARC_STEP*6, 0x88));

		DelayMs(DEMODELAY);
		
        SetColor(BRIGHTBLUE);
        WAIT_UNTIL_FINISH(_Arc_((GetMaxX() >> 1), (GetMaxY() >> 1) - ARC_STEP*3, (GetMaxX() >> 1), (GetMaxY() >> 1) + ARC_STEP*3, ARC_STEP*3, ARC_STEP*4, 0x11));
        SetColor(BRIGHTGREEN);
        WAIT_UNTIL_FINISH(_Arc_((GetMaxX() >> 1), (GetMaxY() >> 1) - ARC_STEP*3, (GetMaxX() >> 1), (GetMaxY() >> 1) + ARC_STEP*3, ARC_STEP*3, ARC_STEP*4, 0x22));
        SetColor(BRIGHTRED);
        WAIT_UNTIL_FINISH(_Arc_((GetMaxX() >> 1), (GetMaxY() >> 1) - ARC_STEP*3, (GetMaxX() >> 1), (GetMaxY() >> 1) + ARC_STEP*3, ARC_STEP*3, ARC_STEP*4, 0x44));
        SetColor(BRIGHTYELLOW);
        WAIT_UNTIL_FINISH(_Arc_((GetMaxX() >> 1), (GetMaxY() >> 1) - ARC_STEP*3, (GetMaxX() >> 1), (GetMaxY() >> 1) + ARC_STEP*3, ARC_STEP*3, ARC_STEP*4, 0x88));

		DelayMs(DEMODELAY);
		
        SetColor(BRIGHTBLUE);
        WAIT_UNTIL_FINISH(_Arc_((GetMaxX() >> 1), (GetMaxY() >> 1), (GetMaxX() >> 1), (GetMaxY() >> 1), ARC_STEP*2, ARC_STEP*3, 0x11));
        SetColor(BRIGHTGREEN);
        WAIT_UNTIL_FINISH(_Arc_((GetMaxX() >> 1), (GetMaxY() >> 1), (GetMaxX() >> 1), (GetMaxY() >> 1), ARC_STEP*2, ARC_STEP*3, 0x22));
        SetColor(BRIGHTRED);
        WAIT_UNTIL_FINISH(_Arc_((GetMaxX() >> 1), (GetMaxY() >> 1), (GetMaxX() >> 1), (GetMaxY() >> 1), ARC_STEP*2, ARC_STEP*3, 0x44));
        SetColor(BRIGHTYELLOW);
        WAIT_UNTIL_FINISH(_Arc_((GetMaxX() >> 1), (GetMaxY() >> 1), (GetMaxX() >> 1), (GetMaxY() >> 1), ARC_STEP*2, ARC_STEP*3, 0x88));

        DelayMs(DEMODELAY);

        SetColor(BLACK);
        ClearDevice();


///#define  RECT_STEP	10
#define  RECT_STEP	5
		// draw rectangles in the middle of the screen
        SetColor(BRIGHTBLUE);
        for(counter = 0; counter < MIN(GetMaxX(), GetMaxY()) >> 1; counter += (RECT_STEP*2))
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
        WAIT_UNTIL_FINISH(Bar(GetMaxX() / 2 - RECT_STEP*8, GetMaxY() / 2 - RECT_STEP*8, GetMaxX() / 2 + RECT_STEP*8, GetMaxY() / 2 + RECT_STEP*8));
        SetColor(BRIGHTGREEN);
        WAIT_UNTIL_FINISH(Bar(GetMaxX() / 2 - RECT_STEP*6, GetMaxY() / 2 - RECT_STEP*6, GetMaxX() / 2 + RECT_STEP*6, GetMaxY() / 2 + RECT_STEP*6));
        SetColor(BRIGHTRED);
        WAIT_UNTIL_FINISH(Bar(GetMaxX() / 2 - RECT_STEP*4, GetMaxY() / 2 - RECT_STEP*4, GetMaxX() / 2 + RECT_STEP*4, GetMaxY() / 2 + RECT_STEP*4));

        DelayMs(DEMODELAY);
        SetColor(BLACK);
        ClearDevice();

		// draw ploygon shape in the middle of the screen
        SetColor(WHITE);
        WAIT_UNTIL_FINISH(DrawPoly(5, (SHORT *)polyPoints));

        DelayMs(DEMODELAY);
		
		
#define	TEXT_STRING		"Mitac MBU."
        SetColor(BLACK);
        ClearDevice();

		// draw fonts in the screen
        SetFont((void *) &Gentium_Normal25);
		SetColor(BRIGHTGREEN);
        ///width = GetTextWidth("Microchip Technology Inc.", (void *) &Font25);
		width = GetTextWidth(TEXT_STRING, (void *) &Gentium_Normal25);
        height = GetTextHeight((void *) &Gentium_Normal25);

        ///OutTextXY((GetMaxX() - width) >> 1, (GetMaxY() - height) >> 1, "Microchip Technology Inc.");
		OutTextXY((GetMaxX() - width) >> 1, (GetMaxY() - height) >> 1, TEXT_STRING);

        DelayMs(DEMODELAY);
		DelayMs(DEMODELAY);
        SetColor(BLACK);
        ClearDevice();

        SetFont((void *) &Gentium_Normal33);
        SetColor(WHITE);
        ///width = GetTextWidth("Microchip Tech.", (void *) &Font35);
		width = GetTextWidth(TEXT_STRING, (void *) &Gentium_Normal33);
        height = GetTextHeight((void *) &Gentium_Normal33);

        ///OutTextXY((GetMaxX() - width) >> 1, (GetMaxY() - height) >> 1, "Microchip Tech.");
		OutTextXY((GetMaxX() - width) >> 1, (GetMaxY() - height) >> 1, TEXT_STRING);

        DelayMs(DEMODELAY);
		DelayMs(DEMODELAY);
        SetColor(BLACK);
        ClearDevice();


		// draw fonts in the screen
        SetFont((void *) &Gentium_Bold25);
		SetColor(RED);
        ///width = GetTextWidth("Microchip Technology Inc.", (void *) &Font25);
		width = GetTextWidth(TEXT_STRING, (void *) &Gentium_Bold25);
        height = GetTextHeight((void *) &Gentium_Bold25);

        ///OutTextXY((GetMaxX() - width) >> 1, (GetMaxY() - height) >> 1, "Microchip Technology Inc.");
		OutTextXY((GetMaxX() - width) >> 1, (GetMaxY() - height) >> 1, TEXT_STRING);

        DelayMs(DEMODELAY);
		DelayMs(DEMODELAY);
        SetColor(BLACK);
        ClearDevice();

        SetFont((void *) &Gentium_Bold33);
        SetColor(BRIGHTBLUE);
        ///width = GetTextWidth("Microchip Tech.", (void *) &Font35);
		width = GetTextWidth(TEXT_STRING, (void *) &Gentium_Bold33);
        height = GetTextHeight((void *) &Gentium_Bold33);

        ///OutTextXY((GetMaxX() - width) >> 1, (GetMaxY() - height) >> 1, "Microchip Tech.");
		OutTextXY((GetMaxX() - width) >> 1, (GetMaxY() - height) >> 1, TEXT_STRING);

        DelayMs(DEMODELAY);
		DelayMs(DEMODELAY);
        SetColor(BLACK);
        ClearDevice();


		// draw fonts in the screen
        SetFont((void *) &Gentium_Italic25);
		SetColor(RED);
        ///width = GetTextWidth("Microchip Technology Inc.", (void *) &Font25);
		width = GetTextWidth(TEXT_STRING, (void *) &Gentium_Italic25);
        height = GetTextHeight((void *) &Gentium_Italic25);

        ///OutTextXY((GetMaxX() - width) >> 1, (GetMaxY() - height) >> 1, "Microchip Technology Inc.");
		OutTextXY((GetMaxX() - width) >> 1, (GetMaxY() - height) >> 1, TEXT_STRING);

        DelayMs(DEMODELAY);
		DelayMs(DEMODELAY);
        SetColor(BLACK);
        ClearDevice();

        SetFont((void *) &Gentium_Italic33);
        SetColor(BRIGHTBLUE);
        ///width = GetTextWidth("Microchip Tech.", (void *) &Font35);
		width = GetTextWidth(TEXT_STRING, (void *) &Gentium_Italic33);
        height = GetTextHeight((void *) &Gentium_Italic33);

        ///OutTextXY((GetMaxX() - width) >> 1, (GetMaxY() - height) >> 1, "Microchip Tech.");
		OutTextXY((GetMaxX() - width) >> 1, (GetMaxY() - height) >> 1, TEXT_STRING);

        DelayMs(DEMODELAY);
		DelayMs(DEMODELAY);
        SetColor(BLACK);
        ClearDevice();

		

        SetFont((void *) &Gentium_Normal33);
        SetColor(WHITE);		
		// draw pictures in the screen with different bits per pixel
        ///WAIT_UNTIL_FINISH(PutImage(0, 0, (void *) &flower1bit, 2));
		WAIT_UNTIL_FINISH(PutImage(0, 0, (void *) &flower1bit, 1));
        SetColor(WHITE);
        ///OutTextXY(200, 0, "1BPP");
		OutTextXY((GetMaxX() - width) >> 1, (GetMaxY() - height) >> 1, "1BPP");
        DelayMs(DEMODELAY);
		DelayMs(DEMODELAY);
        SetColor(BLACK);
        ClearDevice();

        ///WAIT_UNTIL_FINISH(PutImage(0, 0, (void *) &flower4bit, 2));
		WAIT_UNTIL_FINISH(PutImage(0, 0, (void *) &flower4bit, 1));
        SetColor(WHITE);
        ///OutTextXY(200, 0, "4BPP");
		OutTextXY((GetMaxX() - width) >> 1, (GetMaxY() - height) >> 1, "4BPP");
        DelayMs(DEMODELAY);
		DelayMs(DEMODELAY);
        SetColor(BLACK);
        ClearDevice();

        ///WAIT_UNTIL_FINISH(PutImage(0, 0, (void *) &flower8bit, 2));
		WAIT_UNTIL_FINISH(PutImage(0, 0, (void *) &flower8bit, 1));
        SetColor(WHITE);
        ///OutTextXY(200, 0, "8BPP");
		OutTextXY((GetMaxX() - width) >> 1, (GetMaxY() - height) >> 1, "8BPP");
        DelayMs(DEMODELAY);
		DelayMs(DEMODELAY);
        SetColor(BLACK);
        ClearDevice();

        ///WAIT_UNTIL_FINISH(PutImage(0, 0, (void *) &flower16bit, 2));
		WAIT_UNTIL_FINISH(PutImage(0, 0, (void *) &flower16bit, 1));
        SetColor(WHITE);
        ///OutTextXY(200, 0, "16BPP");
		OutTextXY((GetMaxX() - width) >> 1, (GetMaxY() - height) >> 1, "16BPP");
        DelayMs(DEMODELAY);
		DelayMs(DEMODELAY);
        SetColor(BLACK);
        ClearDevice();

        width = GetImageWidth((void *) &flower1bit);
        height = GetImageHeight((void *) &flower1bit);

        WAIT_UNTIL_FINISH(PutImage((GetMaxX() + 1) / 2 - width  , (GetMaxY() + 1) / 2 - height  , (void *) &flower1bit,  1));
        WAIT_UNTIL_FINISH(PutImage((GetMaxX() + 1) / 2          , (GetMaxY() + 1) / 2 - height  , (void *) &flower4bit,  1));
        WAIT_UNTIL_FINISH(PutImage((GetMaxX() + 1) / 2 - width  , (GetMaxY() + 1) / 2           , (void *) &flower8bit,  1));
        WAIT_UNTIL_FINISH(PutImage((GetMaxX() + 1) / 2          , (GetMaxY() + 1) / 2           , (void *) &flower16bit, 1));
        DelayMs(DEMODELAY*2);
        SetColor(BLACK);
        ClearDevice();

		
		
		
		
		
		
		
		
}

