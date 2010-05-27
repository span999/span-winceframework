

#ifndef __TVOVERLAYFUN_H__
#define __TVOVERLAYFUN_H__


extern HRESULT DDInitApp( HINSTANCE hInstance, HWND hWnd );
extern void DDMoveOverlay();
extern void DDdeInitApp();

extern BOOL DDOverlayBitmap( 
	HINSTANCE hInstance, 
	HWND hWnd,
	DWORD dwWidth,
	DWORD dwHeight,
	DWORD dwX,
	DWORD dwY,
	DWORD dwSrcColorkey,
	DWORD dwBitMapID,
	BOOL bSetup
);

extern BOOL DDOverlayText( 
	HINSTANCE hInstance, 
	HWND hWnd,
	DWORD dwWidth,
	DWORD dwHeight,
	DWORD dwX,
	DWORD dwY,
	DWORD dwSrcColorkey,
	DWORD dwBitMapID,
	BOOL bSetup
);


extern BOOL DDOverlayOSD( 
	HINSTANCE hInstance, 
	HWND hWnd,
	DWORD dwWidth,
	DWORD dwHeight,
	DWORD dwX,
	DWORD dwY,
	DWORD dwSrcColorkey,
	LPCTSTR lpstrText,
	DWORD dwBitMapID1,
	DWORD dwBM1Width,
	DWORD dwBM1Height,
	DWORD dwBM1X,
	DWORD dwBM1Y,
	DWORD dwBitMapID2,
	DWORD dwBM2Width,
	DWORD dwBM2Height,
	DWORD dwBM2X,
	DWORD dwBM2Y,
	BOOL bSetup
);
#endif	///#ifndef __TVOVERLAYFUN_H__