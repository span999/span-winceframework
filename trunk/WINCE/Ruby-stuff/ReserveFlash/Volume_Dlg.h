


#ifndef __VOLUME_DLG_H__
#define __VOLUME_DLG_H__

///define
typedef struct {
    HWND hwndSlider;
    DWORD dwRegAddr;
    DWORD dwRegVolMask;
    DWORD dwRegMuteMask;
} Volume_Content;



///callback funtion of EQ
LRESULT	CALLBACK	Volume(HWND, UINT, WPARAM, LPARAM);
///
DWORD VolumeDialogBoxHook( HINSTANCE hInstance, HWND hWnd);


#endif	///#ifndef __VOLUME_DLG_H__