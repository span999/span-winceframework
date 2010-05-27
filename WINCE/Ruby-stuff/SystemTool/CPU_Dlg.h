


#ifndef __CPUUSEAGE_DLG_H__
#define __CPUUSEAGE_DLG_H__

///define
#define		WM_TRAYICON		(WM_USER + 4096)

///callback funtion of 
LRESULT	CALLBACK	CPUusage(HWND, UINT, WPARAM, LPARAM);
///
DWORD CPUusageDialogBoxHook( HINSTANCE hInstance, HWND hWnd);


#endif	///#ifndef __CPUUSEAGE_DLG_H__