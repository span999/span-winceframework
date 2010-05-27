#ifndef __SPLIBDDRAWOBJ_H__
#define __SPLIBDDRAWOBJ_H__


#include <ddraw.h>


class spLibDDraw {

///method
private:
public:
	spLibDDraw(){ Init(); }
	spLibDDraw( HWND hWnd );
	~spLibDDraw(){ DeInit(); }
	DWORD spLibInitDDraw( void );
	DWORD spLibInitDDrawBack( DWORD dwBackSurfNum );
	DWORD spLibInitDDrawBack( DWORD dwBackSurfNum, PVOID pfn );
	DWORD spLibFlipDDraw( void ) { return PrimaryFlip(); }
	DWORD spLibRestoreDDraw( void ) { return PrimaryRestore(); }
	DWORD spLibBltDDraw( RECT *prt, DWORD dwR, DWORD dwG, DWORD dwB ) { return PrimaryBlt( prt, CreateRGB( dwR, dwG, dwB ) ); }
	BOOL spLibPixelDraw( DWORD dwX, DWORD dwY ) { return PixelDraw( dwX, dwY ); }
	BOOL spLibPixelDraw( DWORD dwX, DWORD dwY, DWORD dwR, DWORD dwG, DWORD dwB ) { return PixelDraw( dwX, dwY, dwR, dwG, dwB ); }
	BOOL spLibTextDraw( LPCTSTR szError,... );
	BOOL spLibTextDraw( DWORD dwX, DWORD dwY, LPCTSTR szError,... );
	
protected:

	virtual BOOL Init( void );
	virtual BOOL DeInit( void );
	void spMessageBoxOut( LPCTSTR szError,... );
	void spMessageBoxOut( DWORD dwFlag, LPCTSTR szError,... );
	DWORD InitDDraw( void );
	BOOL GetDDCaps( void );
	BOOL IsOverlaySupport( void );
	DWORD InitDDSurface( void );
	DWORD InitDDPrimarySurface( void );
	DWORD InitDDPrimarySurfaceBack( DWORD dwBackBufNum );
	DWORD InitDDPrimarySurfaceBack( DWORD dwBackSurfNum, PVOID pfn );
	DWORD PrimaryFlip( void );
	DWORD PrimaryRestore( void );
	DWORD PrimaryBlt( RECT *prt, DWORD dwRGB );
	HRESULT SurfaceEnumFunctionStub( LPDIRECTDRAWSURFACE pSurface, LPDDSURFACEDESC lpSurfaceDesc, LPVOID  lpContext );
	HRESULT SurfaceEnumFunction( LPDIRECTDRAWSURFACE pSurface, LPDDSURFACEDESC lpSurfaceDesc, LPVOID  lpContext );
	BOOL PixelDraw( DWORD dwX, DWORD dwY );
	BOOL PixelDraw( DWORD dwX, DWORD dwY, DWORD dwR, DWORD dwG, DWORD dwB );
	BOOL TextDraw( IDirectDrawSurface *pThisSurf, TCHAR *szString );
	BOOL TextDraw( DWORD dwX, DWORD dwY, IDirectDrawSurface *pThisSurf, TCHAR *szString );
	DWORD CreateRGB( DWORD dwR, DWORD dwG, DWORD dwB );
	DWORD GetBitDepth( IDirectDrawSurface *pThisSurf );
	void TakeCS(){ EnterCriticalSection( &m_csDDaccess ); }
	void ReleaseCS(){ LeaveCriticalSection( &m_csDDaccess ); }

///data
private:
public:
protected:

	HWND 					m_hWnd;
	IDirectDraw 			*m_pDD;
	IDirectDrawSurface 		*m_pDDSPrimary;
	///IDirectDrawSurface		*m_pDDSBack[5];
	IDirectDrawSurface		*m_pDDSBack;
	BOOL					m_bSurfaceEnumFunctionCalled;
	DDSURFACEDESC 			m_ddsd;
	DDCAPS					m_ddcaps;
	DWORD					m_iBpp;
	DWORD 					m_dwDDobjStatus;
	CRITICAL_SECTION  		m_csDDaccess;	///

	HANDLE 					m_hMainLogFilehandle;		///
	DWORD 					m_dwMainLogCharNum;			///
	TCHAR					m_szTempString[256];		///
	TCHAR 					m_szTitle[128];				///
	TCHAR 					m_szPathName[128];			///
	TCHAR 					m_szSeparator[128];			///
	TCHAR 					m_szInformation[256];		///

};




#endif	///#ifndef __SPLIBDDRAWOBJ_H__