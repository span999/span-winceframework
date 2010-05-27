


#ifndef __VDBG_DLG_H__
#define __VDBG_DLG_H__

///define
typedef struct {
    DWORD PortContext;
    DWORD PortID;
} PORT_IN_COMBO;

///callback funtion of 
LRESULT	CALLBACK	Vdbg(HWND, UINT, WPARAM, LPARAM);
///
DWORD VdbgDialogBoxHook( HINSTANCE hInstance, HWND hWnd);


#endif	///#ifndef __VDBG_DLG_H__