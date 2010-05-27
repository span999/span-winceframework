
#include <windows.h>
#include <windowsx.h>
#include <aygshell.h>
#include "resource.h"

#include "misc.h"

/*******************************************************************************
		
********************************************************************************/
void ShowToScreen(HWND hwnd)
{
/*
	HDC				hdc;
	RECT			rect;
    HBRUSH			hbrush;              // current brush

	hdc = GetDC(hwnd); 
	GetClientRect(hwnd, &rect);
	x_total = rect.right - rect.left;
	y_total = rect.bottom - rect.top;

	rect.left = rect.left + x_offset; 
	rect.top = rect.top + y_offset;
	DrawText(hdc, TEXT(" "), -1, &rect, DT_LEFT | DT_TOP);		
	DrawText(hdc, &tcByte, 1, &rect, DT_LEFT | DT_TOP);

	x_offset = x_offset + X_OFFSET;

	if (x_offset > x_total - X_OFFSET)
	{
		y_offset = y_offset + Y_OFFSET;
		x_offset = 0;
	}

	if (y_offset > y_total - Y_OFFSET)
	{
		y_offset = 0;
		x_offset = 0;
		rect.left = 0;
		rect.top = 0;
		// Create a monochrome bitmap. 
 
		///hbm = CreateBitmap(8, 8, 1, 1, (LPBYTE)bBrushBits); 
 
		// Select the custom brush into the DC. 
 
		//hbrush = CreatePatternBrush(hbm); 
		hbrush = CreateSolidBrush(WHITE_BRUSH);
		///SelectObject(hdc, (HBRUSH)GetStockObject(GRAY_BRUSH));
		///SelectObject(hdc, (HBRUSH)CreateSolidBrush(BLACK_BRUSH));
		SelectObject(hdc, hbrush);
		
		///FillRect(hdc, &rect, hbrush);
		FillRect(hdc, &rect, (HBRUSH)GetStockObject(WHITE_BRUSH));

	}
	
	//	MoveToEx(hdc, ptPrevious.x, ptPrevious.y, NULL); 
//	LineTo(hdc, LOWORD(lParam), HIWORD(lParam)); 
	ReleaseDC(hwnd, hdc); 

*/
}