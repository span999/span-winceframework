/* MiTAC Int'l Corp. 2001~2006

================================================================================
Description:
    header file of WinOverlay.cpp

================================================================================
Release notes

Modifier	        sign	        date		    Description
--------------------------------------------------------------------------------
Span Liu                            2007/03/09      first release
*/



#ifndef __WINOVERLAY_H__
#define __WINOVERLAY_H__

#include <windows.h>
#include "s3c2443_lcd.h"
#include "s3c2443_base_regs.h"
#include <ceddk.h>



#define WINOL_DEBUG_CODE

    #define		dINIT		0x00000001
    #define		dWARN		0x00000010
    #define		dINFO		0x00000100
    #define		dDBUG		0x00001000
    #define		dFAIL		0x00010000
    #define		dCLAS		0x00100000
    
#ifdef WINOL_DEBUG_CODE
    ///#define		DBGMSG_LEVEL	(dCLAS|dWARN|dFAIL)
    #define		DBGMSG_LEVEL	(dINIT|dWARN|dINFO|dDBUG|dFAIL|dCLAS)
    ///#define		DBGMSG_LEVEL	(dINFO|dFAIL)

    #define     WOLDBGMSG(level, Message)	((level&DBGMSG_LEVEL)?(NKDbgPrintfW Message):0)
#else    
    #define     WOLDBGMSG      (void)
#endif  ///#ifdef WINOL_DEBUG_CODE





typedef struct  T_WINOVERLAY_CONTENT
{
	DWORD dwWinSizeX;
	DWORD dwWinSizeY;
	DWORD dwWinPosiX;
	DWORD dwWinPosiY;
	PHYSICAL_ADDRESS PhysWinOverlayAddr;
	PBYTE pbVirtWinOverlayAddr;

} WINOVERLAY_CONTENT, *PWINOVERLAY_CONTENT;




class CWinOverlay
{
public:
    CWinOverlay();
    ~CWinOverlay();
    CWinOverlay( DWORD dwWinSizeX, DWORD dwWinSizeY, DWORD dwWinPosiX, DWORD dwWinPosiY );
    
    void DumpLCDRegEx();
    BOOL WinInit( DWORD dwWinSizeX, DWORD dwWinSizeY, DWORD dwWinPosiX, DWORD dwWinPosiY );
    BOOL WinReSize();
    BOOL WinRePosition( DWORD dwWinPosiX, DWORD dwWinPosiY );
    BOOL SetALPHAvalue0( BYTE bR, BYTE bG, BYTE bB );
    BOOL SetALPHAvalue1( BYTE bR, BYTE bG, BYTE bB );
    BOOL SetColorKey1( BOOL bEnableBlend, BOOL EnableKey, BOOL bKeyDirForeground, DWORD dwColorValue, DWORD dwKeyValue );
    BOOL SetColorKey2( BOOL bEnableBlend, BOOL EnableKey, BOOL bKeyDirForeground, DWORD dwColorValue, DWORD dwKeyValue );
    BOOL SetColorKey3( BOOL bEnableBlend, BOOL EnableKey, BOOL bKeyDirForeground, DWORD dwColorValue, DWORD dwKeyValue );
    BOOL SetColorKey4( BOOL bEnableBlend, BOOL EnableKey, BOOL bKeyDirForeground, DWORD dwColorValue, DWORD dwKeyValue );

    void DbgPrintString( PUSHORT puString );
    void DbgPrintColor( USHORT usColorFore, USHORT usColorBack );
    void OverlayOn(){ StartOverlay(); }
    void OverlayOff(){ StopOverlay(); }

protected:
    void DumpLCDReg();
    void ClearWinOLContent();
    void InitClear();
    BOOL SWInit();
    BOOL SWUnInit();

    void StartOverlay();
    void StopOverlay();
    
    void DbgPrintChar( PUSHORT puCharS );
    void DrawChar( PUCHAR puChar, UINT uiX, UINT uiY );
    void DrawPixel( UINT uiColor, PUSHORT psFrameBuf );
private:

protected:
    volatile S3C2443_LCD_REG *pLCD;
    WINOVERLAY_CONTENT WinOLContent;
    
    UINT uiXcurrent;
    UINT uiYcurrent;
    USHORT usDBGColorForeground;
    USHORT usDBGColorBackground;
private:

};





#endif      ///__WINOVERLAY_H__