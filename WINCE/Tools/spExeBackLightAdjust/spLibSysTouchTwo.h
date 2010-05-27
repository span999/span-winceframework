#ifndef __SPLIBSYSTOUCHTWO_H__
#define __SPLIBSYSTOUCHTWO_H__


#define		NAMED_TOUCHTWO_SIZE_UP						TEXT("named touchtwo size up event")
#define		NAMED_TOUCHTWO_SIZE_DOWN					TEXT("named touchtwo size down event")
#define		NAMED_TOUCHTWO_ROTATE_CLOCKWISE				TEXT("named touchtwo rotate clockwise event")
#define		NAMED_TOUCHTWO_ROTATE_ANTICLOCKWISE			TEXT("named touchtwo rotate anti clockwise event")


DWORD spLibSysTouchTwo_Init( DWORD dwParam );
DWORD spLibSysTouchTwo_Deinit( DWORD dwParam );
BOOL spLibSysTouchTwo_Process( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );


#endif	///#ifndef __SPLIBSYSTOUCHTWO_H__