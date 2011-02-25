#include <stdio.h>

///#include <FreeRTOS.h>
///#include <task.h>

#include <guic.h>



void gcCleanScreen(void)
{
	SetColor(BLACK);
	ClearDevice();
}


WORD gcColFntOutTextXY( SHORT x, SHORT y, XCHAR *textString, void *font, WORD color )
{
	SetFont(font);
	SetColor(color);
	return OutTextXY(x, y, textString);
}