


#ifndef __HWVERSION_DLG_H__
#define __HWVERSION_DLG_H__

///define


extern "C" BOOL KernelIoControl(DWORD dwIoControlCode, LPVOID lpInBuf, DWORD nInBufSize, LPVOID lpOutBuf, DWORD nOutBufSize, LPDWORD lpBytesReturned);
///callback funtion of 
LRESULT	CALLBACK	HWVersion(HWND, UINT, WPARAM, LPARAM);
///
DWORD HWVersionDialogBoxHook( HINSTANCE hInstance, HWND hWnd);


#endif	///#ifndef __HWVERSION_DLG_H__