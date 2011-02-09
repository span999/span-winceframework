#include <FreeRTOS.h>
#include <task.h>

/// include this for PIC graphic lib
#include "PICgraphic_set.h"

#include "FontGentium.h"
#include "1bpp_icons.h"
#include "4bpp_icons.h"



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

/***********************************************************************************************************/
        SetColor(BLACK);
        ClearDevice();
		// draw fonts in the screen
        SetFont((void *) &Gentium_Normal15);
		SetColor(LIGHTRED);
		width = GetTextWidth("This Demo bases on", (void *) &Gentium_Normal15);
        height = GetTextHeight((void *) &Gentium_Normal15);
		OutTextXY((GetMaxX() - width) >> 1, ((GetMaxY() - height) >> 1)-20, "This Demo bases on");
        DelayMs(DEMODELAY);
		DelayMs(DEMODELAY);	

		// draw fonts in the screen
        SetFont((void *) &Gentium_Normal15);
		SetColor(LIGHTGREEN);
		width = GetTextWidth("freeRTOS:v6.1.0", (void *) &Gentium_Normal15);
        height = GetTextHeight((void *) &Gentium_Normal15);
		OutTextXY((GetMaxX() - width) >> 1, (GetMaxY() - height) >> 1, "freeRTOS:v6.1.0");
        DelayMs(DEMODELAY);
		DelayMs(DEMODELAY);	

		// draw fonts in the screen
        SetFont((void *) &Gentium_Normal15);
		SetColor(LIGHTBLUE);
		width = GetTextWidth("MicroChip AppLib:v2.11", (void *) &Gentium_Normal15);
        height = GetTextHeight((void *) &Gentium_Normal15);
		OutTextXY((GetMaxX() - width) >> 1, ((GetMaxY() - height) >> 1)+20, "MicroChip AppLib:v2.11");
        DelayMs(DEMODELAY);
		DelayMs(DEMODELAY);	
        DelayMs(DEMODELAY);
		DelayMs(DEMODELAY);	
        DelayMs(DEMODELAY);
		DelayMs(DEMODELAY);	


/***********************************************************************************************************/		
        SetColor(BLACK);
        ClearDevice();
        SetFont((void *) &Gentium_Normal15);
		SetColor(WHITE);
		width = GetTextWidth("draw border lines...", (void *) &Gentium_Normal15);
        height = GetTextHeight((void *) &Gentium_Normal15);
		OutTextXY((GetMaxX() - width) >> 1, (GetMaxY() - height) >> 1, "draw border lines...");
        DelayMs(DEMODELAY);
		DelayMs(DEMODELAY);			
        SetColor(BLACK);
        ClearDevice();

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
        DelayMs(DEMODELAY);
		
		
/***********************************************************************************************************/
        SetColor(BLACK);
        ClearDevice();
        SetFont((void *) &Gentium_Normal15);
		SetColor(WHITE);
		width = GetTextWidth("draw lines intersecting...", (void *) &Gentium_Normal15);
        height = GetTextHeight((void *) &Gentium_Normal15);
		OutTextXY((GetMaxX() - width) >> 1, (GetMaxY() - height) >> 1, "draw lines intersecting...");
        DelayMs(DEMODELAY);
		DelayMs(DEMODELAY);			
        SetColor(BLACK);
        ClearDevice();
		
		// draw WHITE lines intersecting in the middle of the screen
        SetColor(WHITE);
        for(counter = 0; counter < GetMaxX(); counter += 20)
        {
            WAIT_UNTIL_FINISH(Line(counter, 0, GetMaxX() - 1 - counter, GetMaxY() - 1));
        }

        DelayMs(DEMODELAY);
        DelayMs(DEMODELAY);

/***********************************************************************************************************/
        SetColor(BLACK);
        ClearDevice();
        SetFont((void *) &Gentium_Normal15);
		SetColor(WHITE);
		width = GetTextWidth("draw concentric circles...", (void *) &Gentium_Normal15);
        height = GetTextHeight((void *) &Gentium_Normal15);
		OutTextXY((GetMaxX() - width) >> 1, (GetMaxY() - height) >> 1, "draw concentric circles...");
        DelayMs(DEMODELAY);
		DelayMs(DEMODELAY);			
        SetColor(BLACK);
        ClearDevice();

		// draw concentric RED circles in the middle of the screen
        SetColor(BRIGHTRED);
        for(counter = 10; counter < MIN(GetMaxX(), GetMaxY()) >> 1; counter += 10)
        {
            WAIT_UNTIL_FINISH(Circle(GetMaxX() >> 1, GetMaxY() >> 1, counter));
        }

        DelayMs(DEMODELAY);	
        DelayMs(DEMODELAY);	
		
/***********************************************************************************************************/
        SetColor(BLACK);
        ClearDevice();
        SetFont((void *) &Gentium_Normal15);
		SetColor(WHITE);
		width = GetTextWidth("draw concentric filled circles...", (void *) &Gentium_Normal15);
        height = GetTextHeight((void *) &Gentium_Normal15);
		OutTextXY((GetMaxX() - width) >> 1, (GetMaxY() - height) >> 1, "draw concentric filled circles...");
        DelayMs(DEMODELAY);
		DelayMs(DEMODELAY);			
        SetColor(BLACK);
        ClearDevice();
		
		// draw concentric filled circles in the middle of the screen
        SetColor(BRIGHTBLUE);
        WAIT_UNTIL_FINISH(FillCircle(GetMaxX() >> 1, GetMaxY() >> 1, 60));
        SetColor(BRIGHTGREEN);
        WAIT_UNTIL_FINISH(FillCircle(GetMaxX() >> 1, GetMaxY() >> 1, 40));
        SetColor(BRIGHTRED);
        WAIT_UNTIL_FINISH(FillCircle(GetMaxX() >> 1, GetMaxY() >> 1, 20));

        DelayMs(DEMODELAY);
        DelayMs(DEMODELAY);		
		
		
/***********************************************************************************************************/
        SetColor(BLACK);
        ClearDevice();
        SetFont((void *) &Gentium_Normal15);
		SetColor(WHITE);
		width = GetTextWidth("draw concentric beveled objects...", (void *) &Gentium_Normal15);
        height = GetTextHeight((void *) &Gentium_Normal15);
		OutTextXY((GetMaxX() - width) >> 1, (GetMaxY() - height) >> 1, "draw concentric beveled objects...");
        DelayMs(DEMODELAY);
		DelayMs(DEMODELAY);			
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
        DelayMs(DEMODELAY);
		
		
/***********************************************************************************************************/
        SetColor(BLACK);
        ClearDevice();
        SetFont((void *) &Gentium_Normal15);
		SetColor(WHITE);
		width = GetTextWidth("draw concentric thick beveled...", (void *) &Gentium_Normal15);
        height = GetTextHeight((void *) &Gentium_Normal15);
		OutTextXY((GetMaxX() - width) >> 1, (GetMaxY() - height) >> 1, "draw concentric thick beveled...");
        DelayMs(DEMODELAY);
		DelayMs(DEMODELAY);			
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
        DelayMs(DEMODELAY);
		
		
/***********************************************************************************************************/
        SetColor(BLACK);
        ClearDevice();
        SetFont((void *) &Gentium_Normal15);
		SetColor(WHITE);
		width = GetTextWidth("draw concentric filled beveled...", (void *) &Gentium_Normal15);
        height = GetTextHeight((void *) &Gentium_Normal15);
		OutTextXY((GetMaxX() - width) >> 1, (GetMaxY() - height) >> 1, "draw concentric filled beveled...");
        DelayMs(DEMODELAY);
		DelayMs(DEMODELAY);			
        SetColor(BLACK);
        ClearDevice();
		
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

/***********************************************************************************************************/
        SetColor(BLACK);
        ClearDevice();
        SetFont((void *) &Gentium_Normal15);
		SetColor(WHITE);
		width = GetTextWidth("draw concentric thick beveled...", (void *) &Gentium_Normal15);
        height = GetTextHeight((void *) &Gentium_Normal15);
		OutTextXY((GetMaxX() - width) >> 1, (GetMaxY() - height) >> 1, "draw concentric thick beveled...");
        DelayMs(DEMODELAY);
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
        DelayMs(DEMODELAY);
		
/***********************************************************************************************************/
        SetColor(BLACK);
        ClearDevice();
        SetFont((void *) &Gentium_Normal15);
		SetColor(WHITE);
		width = GetTextWidth("draw rectangles...", (void *) &Gentium_Normal15);
        height = GetTextHeight((void *) &Gentium_Normal15);
		OutTextXY((GetMaxX() - width) >> 1, (GetMaxY() - height) >> 1, "draw rectangles...");
        DelayMs(DEMODELAY);
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
        DelayMs(DEMODELAY);
		DelayMs(DEMODELAY);

        SetColor(BRIGHTBLUE);
        WAIT_UNTIL_FINISH(Bar(GetMaxX() / 2 - RECT_STEP*8, GetMaxY() / 2 - RECT_STEP*8, GetMaxX() / 2 + RECT_STEP*8, GetMaxY() / 2 + RECT_STEP*8));
        SetColor(BRIGHTGREEN);
        WAIT_UNTIL_FINISH(Bar(GetMaxX() / 2 - RECT_STEP*6, GetMaxY() / 2 - RECT_STEP*6, GetMaxX() / 2 + RECT_STEP*6, GetMaxY() / 2 + RECT_STEP*6));
        SetColor(BRIGHTRED);
        WAIT_UNTIL_FINISH(Bar(GetMaxX() / 2 - RECT_STEP*4, GetMaxY() / 2 - RECT_STEP*4, GetMaxX() / 2 + RECT_STEP*4, GetMaxY() / 2 + RECT_STEP*4));

        DelayMs(DEMODELAY);
        DelayMs(DEMODELAY);

/***********************************************************************************************************/
        SetColor(BLACK);
        ClearDevice();
        SetFont((void *) &Gentium_Normal15);
		SetColor(WHITE);
		width = GetTextWidth("draw ploygon shape...", (void *) &Gentium_Normal15);
        height = GetTextHeight((void *) &Gentium_Normal15);
		OutTextXY((GetMaxX() - width) >> 1, (GetMaxY() - height) >> 1, "draw ploygon shape...");
        DelayMs(DEMODELAY);
		DelayMs(DEMODELAY);			
        SetColor(BLACK);
        ClearDevice();	

		// draw ploygon shape in the middle of the screen
        SetColor(WHITE);
        WAIT_UNTIL_FINISH(DrawPoly(5, (SHORT *)polyPoints));

        DelayMs(DEMODELAY);
        DelayMs(DEMODELAY);		

/***********************************************************************************************************/
        SetColor(BLACK);
        ClearDevice();
        SetFont((void *) &Gentium_Normal15);
		SetColor(WHITE);
		width = GetTextWidth("draw fonts...", (void *) &Gentium_Normal15);
        height = GetTextHeight((void *) &Gentium_Normal15);
		OutTextXY((GetMaxX() - width) >> 1, (GetMaxY() - height) >> 1, "draw fonts...");
        DelayMs(DEMODELAY);
		DelayMs(DEMODELAY);			
        SetColor(BLACK);
        ClearDevice();			

#define	TEXT_STRING		"Mitac MBU."

		// draw fonts in the screen
        SetFont((void *) &Gentium_Normal11);
		SetColor(MAGENTA);
        ///width = GetTextWidth("Microchip Technology Inc.", (void *) &Font25);
		width = GetTextWidth(TEXT_STRING, (void *) &Gentium_Normal11);
        height = GetTextHeight((void *) &Gentium_Normal11);

        ///OutTextXY((GetMaxX() - width) >> 1, (GetMaxY() - height) >> 1, "Microchip Technology Inc.");
		OutTextXY((GetMaxX() - width) >> 1, (GetMaxY() - height) >> 1, TEXT_STRING);

        DelayMs(DEMODELAY);
		DelayMs(DEMODELAY);
        SetColor(BLACK);
        ClearDevice();

		// draw fonts in the screen
        SetFont((void *) &Gentium_Normal15);
		SetColor(LIGHTRED);
        ///width = GetTextWidth("Microchip Technology Inc.", (void *) &Font25);
		width = GetTextWidth(TEXT_STRING, (void *) &Gentium_Normal15);
        height = GetTextHeight((void *) &Gentium_Normal15);

        ///OutTextXY((GetMaxX() - width) >> 1, (GetMaxY() - height) >> 1, "Microchip Technology Inc.");
		OutTextXY((GetMaxX() - width) >> 1, (GetMaxY() - height) >> 1, TEXT_STRING);

        DelayMs(DEMODELAY);
		DelayMs(DEMODELAY);
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

/***********************************************************************************************************/
        SetColor(BLACK);
        ClearDevice();
        SetFont((void *) &Gentium_Normal15);
		SetColor(WHITE);
		width = GetTextWidth("draw pictures...", (void *) &Gentium_Normal15);
        height = GetTextHeight((void *) &Gentium_Normal15);
		OutTextXY((GetMaxX() - width) >> 1, (GetMaxY() - height) >> 1, "draw pictures...");
        DelayMs(DEMODELAY);
		DelayMs(DEMODELAY);			
        SetColor(BLACK);
        ClearDevice();	

        SetFont((void *) &Gentium_Normal33);
        SetColor(WHITE);
		width = GetTextWidth(TEXT_STRING, (void *) &Gentium_Normal33);
        height = GetTextHeight((void *) &Gentium_Normal33);		
		// draw pictures in the screen with different bits per pixel
        ///WAIT_UNTIL_FINISH(PutImage(0, 0, (void *) &flower1bit, 2));
		WAIT_UNTIL_FINISH(PutImage(0, 0, (void *) &flower1bit, 1));
        SetColor(CYAN);
        ///OutTextXY(200, 0, "1BPP");
		OutTextXY((GetMaxX() - width) >> 1, (GetMaxY() - height) >> 1, "1BPP");
        DelayMs(DEMODELAY);
		DelayMs(DEMODELAY);
        SetColor(BLACK);
        ClearDevice();

        ///WAIT_UNTIL_FINISH(PutImage(0, 0, (void *) &flower4bit, 2));
		WAIT_UNTIL_FINISH(PutImage(0, 0, (void *) &flower4bit, 1));
        SetColor(CYAN);
        ///OutTextXY(200, 0, "4BPP");
		OutTextXY((GetMaxX() - width) >> 1, (GetMaxY() - height) >> 1, "4BPP");
        DelayMs(DEMODELAY);
		DelayMs(DEMODELAY);
        SetColor(BLACK);
        ClearDevice();

        ///WAIT_UNTIL_FINISH(PutImage(0, 0, (void *) &flower8bit, 2));
		WAIT_UNTIL_FINISH(PutImage(0, 0, (void *) &flower8bit, 1));
        SetColor(CYAN);
        ///OutTextXY(200, 0, "8BPP");
		OutTextXY((GetMaxX() - width) >> 1, (GetMaxY() - height) >> 1, "8BPP");
        DelayMs(DEMODELAY);
		DelayMs(DEMODELAY);
        SetColor(BLACK);
        ClearDevice();

        ///WAIT_UNTIL_FINISH(PutImage(0, 0, (void *) &flower16bit, 2));
		WAIT_UNTIL_FINISH(PutImage(0, 0, (void *) &flower16bit, 1));
        SetColor(CYAN);
        ///OutTextXY(200, 0, "16BPP");
		OutTextXY((GetMaxX() - width) >> 1, (GetMaxY() - height) >> 1, "16BPP");
        DelayMs(DEMODELAY);
		DelayMs(DEMODELAY);
        SetColor(BLACK);
        ClearDevice();
#if 0
        width = GetImageWidth((void *) &flower1bit);
        height = GetImageHeight((void *) &flower1bit);

        WAIT_UNTIL_FINISH(PutImage((GetMaxX() + 1) / 2 - width  , (GetMaxY() + 1) / 2 - height  , (void *) &flower1bit,  1));
        WAIT_UNTIL_FINISH(PutImage((GetMaxX() + 1) / 2          , (GetMaxY() + 1) / 2 - height  , (void *) &flower4bit,  1));
        WAIT_UNTIL_FINISH(PutImage((GetMaxX() + 1) / 2 - width  , (GetMaxY() + 1) / 2           , (void *) &flower8bit,  1));
        WAIT_UNTIL_FINISH(PutImage((GetMaxX() + 1) / 2          , (GetMaxY() + 1) / 2           , (void *) &flower16bit, 1));
        DelayMs(DEMODELAY*2);
#endif		
		
        SetColor(BLACK);
        ClearDevice();

		
/***********************************************************************************************************/
        SetColor(BLACK);
        ClearDevice();
        SetFont((void *) &Gentium_Normal15);
		SetColor(WHITE);
		width = GetTextWidth("draw ICONs...", (void *) &Gentium_Normal15);
        height = GetTextHeight((void *) &Gentium_Normal15);
		OutTextXY((GetMaxX() - width) >> 1, (GetMaxY() - height) >> 1, "draw ICONs...");
        DelayMs(DEMODELAY);
		DelayMs(DEMODELAY);			
        SetColor(BLACK);
        ClearDevice();

        SetFont((void *) &Gentium_Normal15);
		SetColor(WHITE);		
		width = GetTextWidth("1BPP 16x16...", (void *) &Gentium_Normal15);
        height = GetTextHeight((void *) &Gentium_Normal15);
		OutTextXY((GetMaxX() - width) >> 1, (GetMaxY() - height) >> 1, "1BPP 16x16...");
        DelayMs(DEMODELAY);
		DelayMs(DEMODELAY);			
///        SetColor(BLACK);
///        ClearDevice();	
		
		WAIT_UNTIL_FINISH(PutImage(0, 0, (void *) &weighingScale_1bpp_16x16, 1));
		DelayMs(DEMODELAY);		
		WAIT_UNTIL_FINISH(PutImage(20, 0, (void *) &TrashFull_1bpp_16x16, 1));
		DelayMs(DEMODELAY);		
		WAIT_UNTIL_FINISH(PutImage(40, 0, (void *) &TrashEmpty_1bpp_16x16, 1));
		DelayMs(DEMODELAY);		
		WAIT_UNTIL_FINISH(PutImage(60, 0, (void *) &PCGaming2_1bpp_16x16, 1));
		DelayMs(DEMODELAY);		
		WAIT_UNTIL_FINISH(PutImage(80, 0, (void *) &PCGaming1_1bpp_16x16, 1));
		DelayMs(DEMODELAY);		
		WAIT_UNTIL_FINISH(PutImage(100, 0, (void *) &Language_1bpp_16x16, 1));
		DelayMs(DEMODELAY);		
		WAIT_UNTIL_FINISH(PutImage(120, 0, (void *) &FolderPhoto_1bpp_6x16, 1));
		DelayMs(DEMODELAY);		
		WAIT_UNTIL_FINISH(PutImage(140, 0, (void *) &FolderMusic_1bpp_16x16, 1));
		DelayMs(DEMODELAY);

		
		WAIT_UNTIL_FINISH(PutImage(0, 20, (void *) &FolderFile_1bpp_16x16, 1));
		DelayMs(DEMODELAY);				
		WAIT_UNTIL_FINISH(PutImage(20, 20, (void *) &FolderEmpty_1bpp_16x16, 1));
		DelayMs(DEMODELAY);		
		WAIT_UNTIL_FINISH(PutImage(40, 20, (void *) &ECG_1bpp_16x16, 1));
		DelayMs(DEMODELAY);		
		WAIT_UNTIL_FINISH(PutImage(60, 20, (void *) &DateAndTime_1bpp_16x16, 1));
		DelayMs(DEMODELAY);		

        DelayMs(DEMODELAY);
		DelayMs(DEMODELAY);
		
        SetFont((void *) &Gentium_Normal15);
		SetColor(WHITE);		
		width = GetTextWidth("4BPP 16x16...", (void *) &Gentium_Normal15);
        height = GetTextHeight((void *) &Gentium_Normal15);
		OutTextXY((GetMaxX() - width) >> 1, (GetMaxY() - height) >> 1, "             ");
		OutTextXY((GetMaxX() - width) >> 1, (GetMaxY() - height) >> 1, "4BPP 16x16...");
        DelayMs(DEMODELAY);
		DelayMs(DEMODELAY);	

		WAIT_UNTIL_FINISH(PutImage(0, 60, (void *) &weighingScale_4bpp_16x16, 1));
		DelayMs(DEMODELAY);		
		WAIT_UNTIL_FINISH(PutImage(20, 60, (void *) &Volume4bar_4bpp_16x16, 1));
		DelayMs(DEMODELAY);		
		WAIT_UNTIL_FINISH(PutImage(40, 60, (void *) &Volume3bar_4bpp_16x16, 1));
		DelayMs(DEMODELAY);		
		WAIT_UNTIL_FINISH(PutImage(60, 60, (void *) &Volume2bar_4bpp_16x16, 1));
		DelayMs(DEMODELAY);		
		WAIT_UNTIL_FINISH(PutImage(80, 60, (void *) &Volume1bar_4bpp_16x16, 1));
		DelayMs(DEMODELAY);		
		WAIT_UNTIL_FINISH(PutImage(100, 60, (void *) &Volume0bar_4bpp_16x16, 1));
		DelayMs(DEMODELAY);		
		WAIT_UNTIL_FINISH(PutImage(120, 60, (void *) &TrashFull_4bpp_16x16, 1));
		DelayMs(DEMODELAY);		
		WAIT_UNTIL_FINISH(PutImage(140, 60, (void *) &TrashEmpty_4bpp_16x16, 1));
		DelayMs(DEMODELAY);
		

		WAIT_UNTIL_FINISH(PutImage(0, 80, (void *) &Settings_4bpp_16x16, 1));
		DelayMs(DEMODELAY);		
		WAIT_UNTIL_FINISH(PutImage(20, 80, (void *) &Language_4bpp_16x16, 1));
		DelayMs(DEMODELAY);			
		WAIT_UNTIL_FINISH(PutImage(40, 80, (void *) &FolderPhoto_4bpp_16x16, 1));
		DelayMs(DEMODELAY);		
		WAIT_UNTIL_FINISH(PutImage(60, 80, (void *) &FolderMusic_4bpp_16x16, 1));
		DelayMs(DEMODELAY);		
		WAIT_UNTIL_FINISH(PutImage(80, 80, (void *) &FolderFile_4bpp_16x16, 1));
		DelayMs(DEMODELAY);		
		WAIT_UNTIL_FINISH(PutImage(100, 80, (void *) &FolderEmpty_4bpp_16x16, 1));
		DelayMs(DEMODELAY);		
		WAIT_UNTIL_FINISH(PutImage(120, 80, (void *) &ECG_4bpp_16x16, 1));
		DelayMs(DEMODELAY);	
		WAIT_UNTIL_FINISH(PutImage(140, 80, (void *) &Sun_4bpp_16x16, 1));
		DelayMs(DEMODELAY);	
		
        SetColor(BLACK);
        ClearDevice();		
}

