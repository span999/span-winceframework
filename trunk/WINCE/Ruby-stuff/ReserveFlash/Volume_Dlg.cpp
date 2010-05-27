
#include "stdafx.h"
///#include <winuserm.h>
///#include "Thales_defines.h"
#include <commctrl.h>
#include "resource.h"
#include "CODEC_Lib.h"
#include "Volume_Dlg.h"

///prototype
DWORD VolumeDialogBoxHook( HINSTANCE hInstance, HWND hWnd );
LRESULT CALLBACK Volume(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);



DWORD VolumeDialogBoxHook( HINSTANCE hInstance, HWND hWnd)
{
	DWORD dwRet = 0;

	DialogBox(hInstance, (LPCTSTR)IDD_VOLUMEBOX, hWnd, (DLGPROC)Volume);
	
	return dwRet;
}



// Mesage handler for the Volume box.
LRESULT CALLBACK Volume(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	RECT rt, rt1;
	int DlgWidth, DlgHeight;	// dialog width and height in pixel units
	int NewPosX, NewPosY;
	static Volume_Content SliderContentArrary[9];
	
	switch (message)
	{
		case WM_INITDIALOG:
			// trying to center the About dialog
			if (GetWindowRect(hDlg, &rt1)) {
				GetClientRect(GetParent(hDlg), &rt);
				DlgWidth	= rt1.right - rt1.left;
				DlgHeight	= rt1.bottom - rt1.top ;
				NewPosX		= (rt.right - rt.left - DlgWidth)/2;
				NewPosY		= (rt.bottom - rt.top - DlgHeight)/2;
				
				// if the About box is larger than the physical screen 
				if (NewPosX < 0) NewPosX = 0;
				if (NewPosY < 0) NewPosY = 0;
				SetWindowPos(hDlg, 0, NewPosX, NewPosY,
					0, 0, SWP_NOZORDER | SWP_NOSIZE);
			}
			
			///MONO
			SliderContentArrary[0].hwndSlider = GetDlgItem(hDlg,IDC_VOLUME_SLIDER1);
			SliderContentArrary[0].dwRegAddr = 0x0008;
			SliderContentArrary[0].dwRegVolMask = 0x003F;
			SliderContentArrary[0].dwRegMuteMask = 0x0080;
			///DACL
			SliderContentArrary[1].hwndSlider = GetDlgItem(hDlg,IDC_VOLUME_SLIDER2);
			SliderContentArrary[1].dwRegAddr = 0x000C;
			SliderContentArrary[1].dwRegVolMask = 0x1F00;
			SliderContentArrary[1].dwRegMuteMask = 0xE000;
			///DACR
			SliderContentArrary[2].hwndSlider = GetDlgItem(hDlg,IDC_VOLUME_SLIDER3);
			SliderContentArrary[2].dwRegAddr = 0x000C;
			SliderContentArrary[2].dwRegVolMask = 0x001F;
			SliderContentArrary[2].dwRegMuteMask = 0xE000;
			///HPL
			SliderContentArrary[3].hwndSlider = GetDlgItem(hDlg,IDC_VOLUME_SLIDER4);
			SliderContentArrary[3].dwRegAddr = 0x0004;
			SliderContentArrary[3].dwRegVolMask = 0x3F00;
			SliderContentArrary[3].dwRegMuteMask = 0x8000;
			///HPR
			SliderContentArrary[4].hwndSlider = GetDlgItem(hDlg,IDC_VOLUME_SLIDER5);
			SliderContentArrary[4].dwRegAddr = 0x0004;
			SliderContentArrary[4].dwRegVolMask = 0x003F;
			SliderContentArrary[4].dwRegMuteMask = 0x0080;
			///SPKL
			SliderContentArrary[5].hwndSlider = GetDlgItem(hDlg,IDC_VOLUME_SLIDER6);
			SliderContentArrary[5].dwRegAddr = 0x0002;
			SliderContentArrary[5].dwRegVolMask = 0x3F00;
			SliderContentArrary[5].dwRegMuteMask = 0x8000;
			///SPKR
			SliderContentArrary[6].hwndSlider = GetDlgItem(hDlg,IDC_VOLUME_SLIDER7);
			SliderContentArrary[6].dwRegAddr = 0x0002;
			SliderContentArrary[6].dwRegVolMask = 0x003F;
			SliderContentArrary[6].dwRegMuteMask = 0x0080;
			///MICL
			SliderContentArrary[7].hwndSlider = GetDlgItem(hDlg,IDC_VOLUME_SLIDER8);
			SliderContentArrary[7].dwRegAddr = 0x0012;
			SliderContentArrary[7].dwRegVolMask = 0x3F00;
			SliderContentArrary[7].dwRegMuteMask = 0x8000;
			///MICR
			SliderContentArrary[8].hwndSlider = GetDlgItem(hDlg,IDC_VOLUME_SLIDER9);
			SliderContentArrary[8].dwRegAddr = 0x0012;
			SliderContentArrary[8].dwRegVolMask = 0x003F;
			SliderContentArrary[8].dwRegMuteMask = 0x8000;
			
			///adjust slider1
			SendMessage(SliderContentArrary[0].hwndSlider,TBM_SETRANGE,TRUE,MAKELONG(0, 63));	// range
			SendMessage(SliderContentArrary[0].hwndSlider,TBM_SETTICFREQ,4,0L);				// tick freq.
			SendMessage(SliderContentArrary[0].hwndSlider,TBM_SETPAGESIZE,0L,1);				// page up/dn size
			///SendMessage(SliderContentArrary[0].hwndSlider,TBM_SETPOS,TRUE,dwValue);   
			///adjust slider2
			SendMessage(SliderContentArrary[1].hwndSlider,TBM_SETRANGE,TRUE,MAKELONG(0, 31));	// range
			SendMessage(SliderContentArrary[1].hwndSlider,TBM_SETTICFREQ,2,0L);				// tick freq.
			SendMessage(SliderContentArrary[1].hwndSlider,TBM_SETPAGESIZE,0L,1);				// page up/dn size
			///SendMessage(SliderContentArrary[1].hwndSlider,TBM_SETPOS,TRUE,dwValue);   
			///adjust slider3
			SendMessage(SliderContentArrary[2].hwndSlider,TBM_SETRANGE,TRUE,MAKELONG(0, 31));	// range
			SendMessage(SliderContentArrary[2].hwndSlider,TBM_SETTICFREQ,2,0L);				// tick freq.
			SendMessage(SliderContentArrary[2].hwndSlider,TBM_SETPAGESIZE,0L,1);				// page up/dn size
			///SendMessage(SliderContentArrary[2].hwndSlider,TBM_SETPOS,TRUE,dwValue);   
			///adjust slider4
			SendMessage(SliderContentArrary[3].hwndSlider,TBM_SETRANGE,TRUE,MAKELONG(0, 63));	// range
			SendMessage(SliderContentArrary[3].hwndSlider,TBM_SETTICFREQ,4,0L);				// tick freq.
			SendMessage(SliderContentArrary[3].hwndSlider,TBM_SETPAGESIZE,0L,1);				// page up/dn size
			///SendMessage(SliderContentArrary[3].hwndSlider,TBM_SETPOS,TRUE,dwValue);   
			///adjust slider5
			SendMessage(SliderContentArrary[4].hwndSlider,TBM_SETRANGE,TRUE,MAKELONG(0, 63));	// range
			SendMessage(SliderContentArrary[4].hwndSlider,TBM_SETTICFREQ,4,0L);				// tick freq.
			SendMessage(SliderContentArrary[4].hwndSlider,TBM_SETPAGESIZE,0L,1);				// page up/dn size
			///SendMessage(SliderContentArrary[4].hwndSlider,TBM_SETPOS,TRUE,dwValue);   
			///adjust slider6
			SendMessage(SliderContentArrary[5].hwndSlider,TBM_SETRANGE,TRUE,MAKELONG(0, 63));	// range
			SendMessage(SliderContentArrary[5].hwndSlider,TBM_SETTICFREQ,4,0L);				// tick freq.
			SendMessage(SliderContentArrary[5].hwndSlider,TBM_SETPAGESIZE,0L,1);				// page up/dn size
			///SendMessage(SliderContentArrary[5].hwndSlider,TBM_SETPOS,TRUE,dwValue);   
			///adjust slider7
			SendMessage(SliderContentArrary[6].hwndSlider,TBM_SETRANGE,TRUE,MAKELONG(0, 63));	// range
			SendMessage(SliderContentArrary[6].hwndSlider,TBM_SETTICFREQ,4,0L);				// tick freq.
			SendMessage(SliderContentArrary[6].hwndSlider,TBM_SETPAGESIZE,0L,1);				// page up/dn size
			///SendMessage(SliderContentArrary[6].hwndSlider,TBM_SETPOS,TRUE,dwValue);   
			///adjust slider8
			SendMessage(SliderContentArrary[7].hwndSlider,TBM_SETRANGE,TRUE,MAKELONG(0, 63));	// range
			SendMessage(SliderContentArrary[7].hwndSlider,TBM_SETTICFREQ,4,0L);				// tick freq.
			SendMessage(SliderContentArrary[7].hwndSlider,TBM_SETPAGESIZE,0L,1);				// page up/dn size
			///SendMessage(SliderContentArrary[7].hwndSlider,TBM_SETPOS,TRUE,dwValue);   
			///adjust slider9
			SendMessage(SliderContentArrary[8].hwndSlider,TBM_SETRANGE,TRUE,MAKELONG(0, 63));	// range
			SendMessage(SliderContentArrary[8].hwndSlider,TBM_SETTICFREQ,4,0L);				// tick freq.
			SendMessage(SliderContentArrary[8].hwndSlider,TBM_SETPAGESIZE,0L,1);				// page up/dn size
			///SendMessage(SliderContentArrary[8].hwndSlider,TBM_SETPOS,TRUE,dwValue);   


			SetDlgItemText(hDlg, IDC_VOLUME_EDIT1, TEXT("??"));
			SetDlgItemText(hDlg, IDC_VOLUME_EDIT2, TEXT("??"));
			SetDlgItemText(hDlg, IDC_VOLUME_EDIT3, TEXT("??"));
			SetDlgItemText(hDlg, IDC_VOLUME_EDIT4, TEXT("??"));
			SetDlgItemText(hDlg, IDC_VOLUME_EDIT5, TEXT("??"));
			SetDlgItemText(hDlg, IDC_VOLUME_EDIT6, TEXT("??"));
			SetDlgItemText(hDlg, IDC_VOLUME_EDIT7, TEXT("??"));
			SetDlgItemText(hDlg, IDC_VOLUME_EDIT8, TEXT("??"));
			SetDlgItemText(hDlg, IDC_VOLUME_EDIT9, TEXT("??"));
			
			return TRUE;

		case WM_HSCROLL:            // track bar message
		case WM_VSCROLL:            // track bar message
			DWORD deIndex;
			DWORD dwValue;
			TCHAR szString[64];         // Temperary string
			DWORD dwAddr, dwVal, dwRet, dwSuccess;
			
			switch LOWORD(wParam) 
			{
				case TB_BOTTOM:
				case TB_THUMBPOSITION:
				case TB_LINEUP:
				case TB_LINEDOWN:
				case TB_PAGEUP:
				case TB_PAGEDOWN:
				case TB_TOP:
				case TB_THUMBTRACK:
				case TB_ENDTRACK:
					///switch (lParam)
					{
						if( (HWND)lParam == SliderContentArrary[0].hwndSlider )
						{	
							///it's MONO volume
							deIndex = 0;
							if( BST_CHECKED == SendDlgItemMessage(hDlg, IDC_VOLUME_CHECK1, BM_GETCHECK, (WPARAM)NULL, (LPARAM)NULL ) )
							{
								wsprintf(szString,_T("mute"));
								SetDlgItemText(hDlg, IDC_VOLUME_EDIT1, szString);								
								dwAddr = SliderContentArrary[deIndex].dwRegAddr;
								///get register value
								spAccessAudioCodec( TRUE, &dwAddr, &dwVal, &dwRet, &dwSuccess);
								if( dwSuccess == 0)
								{
									///mute bit
									dwVal = (dwRet|(SliderContentArrary[deIndex].dwRegMuteMask));
									///set register value
									spAccessAudioCodec( FALSE, &dwAddr, &dwVal, &dwRet, &dwSuccess);
								}	
								else
								{
									wsprintf(szString,_T("Error"));
									SetDlgItemText(hDlg, IDC_VOLUME_EDIT1, szString);		
								}
							}
							else
							{
								dwValue = SendMessage(SliderContentArrary[deIndex].hwndSlider,TBM_GETPOS,0,0);
								wsprintf(szString,_T("x%02X"),dwValue);
								SetDlgItemText(hDlg, IDC_VOLUME_EDIT1, szString);
								dwAddr = SliderContentArrary[deIndex].dwRegAddr;
								///get register value
								spAccessAudioCodec( TRUE, &dwAddr, &dwVal, &dwRet, &dwSuccess);
								if( dwSuccess == 0)
								{
									dwVal = (dwRet & ~SliderContentArrary[deIndex].dwRegVolMask)|(dwValue&SliderContentArrary[deIndex].dwRegVolMask);
									dwVal = (dwVal & ~(SliderContentArrary[deIndex].dwRegMuteMask));	///un-mute
									spAccessAudioCodec( FALSE, &dwAddr, &dwVal, &dwRet, &dwSuccess);
								}	
								else
								{
									wsprintf(szString,_T("Error"));
									SetDlgItemText(hDlg, IDC_VOLUME_EDIT1, szString);		
								}
							}	
							break;
						}
						else
						if( (HWND)lParam == SliderContentArrary[1].hwndSlider )
						{	
							///it's DACL volume
							deIndex = 1;
							if( BST_CHECKED == SendDlgItemMessage(hDlg, IDC_VOLUME_CHECK2, BM_GETCHECK, (WPARAM)NULL, (LPARAM)NULL ) )
							{
								wsprintf(szString,_T("mute"));
								SetDlgItemText(hDlg, IDC_VOLUME_EDIT2, szString);								
								dwAddr = SliderContentArrary[deIndex].dwRegAddr;
								///get register value
								spAccessAudioCodec( TRUE, &dwAddr, &dwVal, &dwRet, &dwSuccess);
								if( dwSuccess == 0)
								{
									///mute bit
									dwVal = (dwRet|(SliderContentArrary[deIndex].dwRegMuteMask));
									///set register value
									spAccessAudioCodec( FALSE, &dwAddr, &dwVal, &dwRet, &dwSuccess);
								}	
								else
								{
									wsprintf(szString,_T("Error"));
									SetDlgItemText(hDlg, IDC_VOLUME_EDIT2, szString);		
								}
							}
							else
							{									
								dwValue = SendMessage(SliderContentArrary[deIndex].hwndSlider,TBM_GETPOS,0,0);
								wsprintf(szString,_T("x%02X"),dwValue);
								SetDlgItemText(hDlg, IDC_VOLUME_EDIT2, szString);
								dwAddr = SliderContentArrary[deIndex].dwRegAddr;
								///get register value
								spAccessAudioCodec( TRUE, &dwAddr, &dwVal, &dwRet, &dwSuccess);
								if( dwSuccess == 0)
								{
									dwVal = (dwRet & ~SliderContentArrary[deIndex].dwRegVolMask)|(dwValue<<8&SliderContentArrary[deIndex].dwRegVolMask);
									dwVal = (dwVal & ~(SliderContentArrary[deIndex].dwRegMuteMask));	///un-mute									
									spAccessAudioCodec( FALSE, &dwAddr, &dwVal, &dwRet, &dwSuccess);
								}	
								else
								{
									wsprintf(szString,_T("Error"));
									SetDlgItemText(hDlg, IDC_VOLUME_EDIT2, szString);		
								}
							}		
							break;
						}
						else
						if( (HWND)lParam == SliderContentArrary[2].hwndSlider )	
						{	
							///it's DACR volume
							deIndex = 2;
							if( BST_CHECKED == SendDlgItemMessage(hDlg, IDC_VOLUME_CHECK3, BM_GETCHECK, (WPARAM)NULL, (LPARAM)NULL ) )
							{
								wsprintf(szString,_T("mute"));
								SetDlgItemText(hDlg, IDC_VOLUME_EDIT3, szString);								
								dwAddr = SliderContentArrary[deIndex].dwRegAddr;
								///get register value
								spAccessAudioCodec( TRUE, &dwAddr, &dwVal, &dwRet, &dwSuccess);
								if( dwSuccess == 0)
								{
									///mute bit
									dwVal = (dwRet|(SliderContentArrary[deIndex].dwRegMuteMask));
									///set register value
									spAccessAudioCodec( FALSE, &dwAddr, &dwVal, &dwRet, &dwSuccess);
								}	
								else
								{
									wsprintf(szString,_T("Error"));
									SetDlgItemText(hDlg, IDC_VOLUME_EDIT3, szString);		
								}
							}
							else
							{									
								dwValue = SendMessage(SliderContentArrary[deIndex].hwndSlider,TBM_GETPOS,0,0);
								wsprintf(szString,_T("x%02X"),dwValue);
								SetDlgItemText(hDlg, IDC_VOLUME_EDIT3, szString);
								dwAddr = SliderContentArrary[deIndex].dwRegAddr;
								///get register value
								spAccessAudioCodec( TRUE, &dwAddr, &dwVal, &dwRet, &dwSuccess);
								if( dwSuccess == 0)
								{
									dwVal = (dwRet & ~SliderContentArrary[deIndex].dwRegVolMask)|(dwValue&SliderContentArrary[deIndex].dwRegVolMask);
									dwVal = (dwVal & ~(SliderContentArrary[deIndex].dwRegMuteMask));	///un-mute									
									spAccessAudioCodec( FALSE, &dwAddr, &dwVal, &dwRet, &dwSuccess);
								}	
								else
								{
									wsprintf(szString,_T("Error"));
									SetDlgItemText(hDlg, IDC_VOLUME_EDIT3, szString);		
								}
							}							
							break;
						}
						else
						if( (HWND)lParam == SliderContentArrary[3].hwndSlider )	
						{	
							///it's HPL volume
							deIndex = 3;
							if( BST_CHECKED == SendDlgItemMessage(hDlg, IDC_VOLUME_CHECK4, BM_GETCHECK, (WPARAM)NULL, (LPARAM)NULL ) )
							{
								wsprintf(szString,_T("mute"));
								SetDlgItemText(hDlg, IDC_VOLUME_EDIT4, szString);								
								dwAddr = SliderContentArrary[deIndex].dwRegAddr;
								///get register value
								spAccessAudioCodec( TRUE, &dwAddr, &dwVal, &dwRet, &dwSuccess);
								if( dwSuccess == 0)
								{
									///mute bit
									dwVal = (dwRet|(SliderContentArrary[deIndex].dwRegMuteMask));
									///set register value
									spAccessAudioCodec( FALSE, &dwAddr, &dwVal, &dwRet, &dwSuccess);
								}	
								else
								{
									wsprintf(szString,_T("Error"));
									SetDlgItemText(hDlg, IDC_VOLUME_EDIT4, szString);		
								}
							}
							else
							{									
								dwValue = SendMessage(SliderContentArrary[deIndex].hwndSlider,TBM_GETPOS,0,0);
								wsprintf(szString,_T("x%02X"),dwValue);
								SetDlgItemText(hDlg, IDC_VOLUME_EDIT4, szString);
								dwAddr = SliderContentArrary[deIndex].dwRegAddr;
								///get register value
								spAccessAudioCodec( TRUE, &dwAddr, &dwVal, &dwRet, &dwSuccess);
								if( dwSuccess == 0)
								{
									dwVal = (dwRet & ~SliderContentArrary[deIndex].dwRegVolMask)|((dwValue<<8)&SliderContentArrary[deIndex].dwRegVolMask);
									dwVal = (dwVal & ~(SliderContentArrary[deIndex].dwRegMuteMask));	///un-mute									
									spAccessAudioCodec( FALSE, &dwAddr, &dwVal, &dwRet, &dwSuccess);
								}	
								else
								{
									wsprintf(szString,_T("Error"));
									SetDlgItemText(hDlg, IDC_VOLUME_EDIT4, szString);		
								}
							}							
							break;
						}
						else
						if( (HWND)lParam == SliderContentArrary[4].hwndSlider )	
						{	
							///it's HPR volume
							deIndex = 4;
							if( BST_CHECKED == SendDlgItemMessage(hDlg, IDC_VOLUME_CHECK5, BM_GETCHECK, (WPARAM)NULL, (LPARAM)NULL ) )
							{
								wsprintf(szString,_T("mute"));
								SetDlgItemText(hDlg, IDC_VOLUME_EDIT5, szString);								
								dwAddr = SliderContentArrary[deIndex].dwRegAddr;
								///get register value
								spAccessAudioCodec( TRUE, &dwAddr, &dwVal, &dwRet, &dwSuccess);
								if( dwSuccess == 0)
								{
									///mute bit
									dwVal = (dwRet|(SliderContentArrary[deIndex].dwRegMuteMask));
									///set register value
									spAccessAudioCodec( FALSE, &dwAddr, &dwVal, &dwRet, &dwSuccess);
								}	
								else
								{
									wsprintf(szString,_T("Error"));
									SetDlgItemText(hDlg, IDC_VOLUME_EDIT5, szString);		
								}
							}
							else
							{									
								dwValue = SendMessage(SliderContentArrary[deIndex].hwndSlider,TBM_GETPOS,0,0);
								wsprintf(szString,_T("x%02X"),dwValue);
								SetDlgItemText(hDlg, IDC_VOLUME_EDIT5, szString);
								dwAddr = SliderContentArrary[deIndex].dwRegAddr;
								///get register value
								spAccessAudioCodec( TRUE, &dwAddr, &dwVal, &dwRet, &dwSuccess);
								if( dwSuccess == 0)
								{
									dwVal = (dwRet & ~SliderContentArrary[deIndex].dwRegVolMask)|(dwValue&SliderContentArrary[deIndex].dwRegVolMask);
									dwVal = (dwVal & ~(SliderContentArrary[deIndex].dwRegMuteMask));	///un-mute									
									spAccessAudioCodec( FALSE, &dwAddr, &dwVal, &dwRet, &dwSuccess);
								}	
								else
								{
									wsprintf(szString,_T("Error"));
									SetDlgItemText(hDlg, IDC_VOLUME_EDIT5, szString);		
								}
							}							
							break;
						}
						else
						if( (HWND)lParam == SliderContentArrary[5].hwndSlider )	
						{	
							///it's SPKL volume
							deIndex = 5;
							if( BST_CHECKED == SendDlgItemMessage(hDlg, IDC_VOLUME_CHECK6, BM_GETCHECK, (WPARAM)NULL, (LPARAM)NULL ) )
							{
								wsprintf(szString,_T("mute"));
								SetDlgItemText(hDlg, IDC_VOLUME_EDIT6, szString);
								dwAddr = SliderContentArrary[deIndex].dwRegAddr;
								///get register value
								spAccessAudioCodec( TRUE, &dwAddr, &dwVal, &dwRet, &dwSuccess);
								if( dwSuccess == 0)
								{
									///mute bit
									dwVal = (dwRet|(SliderContentArrary[deIndex].dwRegMuteMask));
									///set register value
									spAccessAudioCodec( FALSE, &dwAddr, &dwVal, &dwRet, &dwSuccess);
								}	
								else
								{
									wsprintf(szString,_T("Error"));
									SetDlgItemText(hDlg, IDC_VOLUME_EDIT6, szString);		
								}
							}
							else
							{									
								dwValue = SendMessage(SliderContentArrary[deIndex].hwndSlider,TBM_GETPOS,0,0);
								wsprintf(szString,_T("x%02X"),dwValue);
								SetDlgItemText(hDlg, IDC_VOLUME_EDIT6, szString);
								dwAddr = SliderContentArrary[deIndex].dwRegAddr;
								///get register value
								spAccessAudioCodec( TRUE, &dwAddr, &dwVal, &dwRet, &dwSuccess);
								if( dwSuccess == 0)
								{
									dwVal = (dwRet & ~SliderContentArrary[deIndex].dwRegVolMask)|((dwValue<<8)&SliderContentArrary[deIndex].dwRegVolMask);
									dwVal = (dwVal & ~(SliderContentArrary[deIndex].dwRegMuteMask));	///un-mute									
									spAccessAudioCodec( FALSE, &dwAddr, &dwVal, &dwRet, &dwSuccess);
								}	
								else
								{
									wsprintf(szString,_T("Error"));
									SetDlgItemText(hDlg, IDC_VOLUME_EDIT6, szString);
								}
							}							
							break;
						}
						else
						if( (HWND)lParam == SliderContentArrary[6].hwndSlider )	
						{	
							///it's SPKR volume
							deIndex = 6;
							if( BST_CHECKED == SendDlgItemMessage(hDlg, IDC_VOLUME_CHECK7, BM_GETCHECK, (WPARAM)NULL, (LPARAM)NULL ) )
							{
								wsprintf(szString,_T("mute"));
								SetDlgItemText(hDlg, IDC_VOLUME_EDIT7, szString);
								dwAddr = SliderContentArrary[deIndex].dwRegAddr;
								///get register value
								spAccessAudioCodec( TRUE, &dwAddr, &dwVal, &dwRet, &dwSuccess);
								if( dwSuccess == 0)
								{
									///mute bit
									dwVal = (dwRet|(SliderContentArrary[deIndex].dwRegMuteMask));
									///set register value
									spAccessAudioCodec( FALSE, &dwAddr, &dwVal, &dwRet, &dwSuccess);
								}	
								else
								{
									wsprintf(szString,_T("Error"));
									SetDlgItemText(hDlg, IDC_VOLUME_EDIT7, szString);		
								}
							}
							else
							{									
								dwValue = SendMessage(SliderContentArrary[deIndex].hwndSlider,TBM_GETPOS,0,0);
								wsprintf(szString,_T("x%02X"),dwValue);
								SetDlgItemText(hDlg, IDC_VOLUME_EDIT7, szString);
								dwAddr = SliderContentArrary[deIndex].dwRegAddr;
								///get register value
								spAccessAudioCodec( TRUE, &dwAddr, &dwVal, &dwRet, &dwSuccess);
								if( dwSuccess == 0)
								{
									dwVal = (dwRet & ~SliderContentArrary[deIndex].dwRegVolMask)|(dwValue&SliderContentArrary[deIndex].dwRegVolMask);
									dwVal = (dwVal & ~(SliderContentArrary[deIndex].dwRegMuteMask));	///un-mute									
									spAccessAudioCodec( FALSE, &dwAddr, &dwVal, &dwRet, &dwSuccess);
								}	
								else
								{
									wsprintf(szString,_T("Error"));
									SetDlgItemText(hDlg, IDC_VOLUME_EDIT7, szString);		
								}
							}							
							break;
						}
						else
						if( (HWND)lParam == SliderContentArrary[7].hwndSlider )
						{	
							///it's MICL volume
							deIndex = 7;
							if( BST_CHECKED == SendDlgItemMessage(hDlg, IDC_VOLUME_CHECK8, BM_GETCHECK, (WPARAM)NULL, (LPARAM)NULL ) )
							{
								wsprintf(szString,_T("mute"));
								SetDlgItemText(hDlg, IDC_VOLUME_EDIT8, szString);								
								dwAddr = SliderContentArrary[deIndex].dwRegAddr;
								///get register value
								spAccessAudioCodec( TRUE, &dwAddr, &dwVal, &dwRet, &dwSuccess);
								if( dwSuccess == 0)
								{
									///mute bit
									dwVal = (dwRet|(SliderContentArrary[deIndex].dwRegMuteMask));
									///set register value
									spAccessAudioCodec( FALSE, &dwAddr, &dwVal, &dwRet, &dwSuccess);
								}	
								else
								{
									wsprintf(szString,_T("Error"));
									SetDlgItemText(hDlg, IDC_VOLUME_EDIT8, szString);		
								}
							}
							else
							{									
								dwValue = SendMessage(SliderContentArrary[deIndex].hwndSlider,TBM_GETPOS,0,0);
								wsprintf(szString,_T("x%02X"),dwValue);
								SetDlgItemText(hDlg, IDC_VOLUME_EDIT8, szString);
								dwAddr = SliderContentArrary[deIndex].dwRegAddr;
								///get register value
								spAccessAudioCodec( TRUE, &dwAddr, &dwVal, &dwRet, &dwSuccess);
								if( dwSuccess == 0)
								{
									dwVal = (dwRet & ~SliderContentArrary[deIndex].dwRegVolMask)|(dwValue<<8&SliderContentArrary[deIndex].dwRegVolMask);
									dwVal = (dwVal & ~(SliderContentArrary[deIndex].dwRegMuteMask));	///un-mute									
									spAccessAudioCodec( FALSE, &dwAddr, &dwVal, &dwRet, &dwSuccess);
								}	
								else
								{
									wsprintf(szString,_T("Error"));
									SetDlgItemText(hDlg, IDC_VOLUME_EDIT8, szString);		
								}
							}		
							break;
						}
						else
						if( (HWND)lParam == SliderContentArrary[8].hwndSlider )	
						{	
							///it's MICR volume
							deIndex = 8;
							if( BST_CHECKED == SendDlgItemMessage(hDlg, IDC_VOLUME_CHECK9, BM_GETCHECK, (WPARAM)NULL, (LPARAM)NULL ) )
							{
								wsprintf(szString,_T("mute"));
								SetDlgItemText(hDlg, IDC_VOLUME_EDIT9, szString);								
								dwAddr = SliderContentArrary[deIndex].dwRegAddr;
								///get register value
								spAccessAudioCodec( TRUE, &dwAddr, &dwVal, &dwRet, &dwSuccess);
								if( dwSuccess == 0)
								{
									///mute bit
									dwVal = (dwRet|(SliderContentArrary[deIndex].dwRegMuteMask));
									///set register value
									spAccessAudioCodec( FALSE, &dwAddr, &dwVal, &dwRet, &dwSuccess);
								}	
								else
								{
									wsprintf(szString,_T("Error"));
									SetDlgItemText(hDlg, IDC_VOLUME_EDIT9, szString);		
								}
							}
							else
							{									
								dwValue = SendMessage(SliderContentArrary[deIndex].hwndSlider,TBM_GETPOS,0,0);
								wsprintf(szString,_T("x%02X"),dwValue);
								SetDlgItemText(hDlg, IDC_VOLUME_EDIT9, szString);
								dwAddr = SliderContentArrary[deIndex].dwRegAddr;
								///get register value
								spAccessAudioCodec( TRUE, &dwAddr, &dwVal, &dwRet, &dwSuccess);
								if( dwSuccess == 0)
								{
									dwVal = (dwRet & ~SliderContentArrary[deIndex].dwRegVolMask)|(dwValue&SliderContentArrary[deIndex].dwRegVolMask);
									dwVal = (dwVal & ~(SliderContentArrary[deIndex].dwRegMuteMask));	///un-mute									
									spAccessAudioCodec( FALSE, &dwAddr, &dwVal, &dwRet, &dwSuccess);
								}	
								else
								{
									wsprintf(szString,_T("Error"));
									SetDlgItemText(hDlg, IDC_VOLUME_EDIT9, szString);		
								}
							}							
							break;
						}
						else
						{		
							;
						}	
					}///switch ((HWND)lParam)
					
///	SendMessage(GetDlgItem(hDlg,IDC_STATIC1),WM_SETTEXT,0,(LPARAM)szStatic);

					return TRUE;
				default:
					// Default case
					return FALSE;
			}
			break;

		case WM_COMMAND:

			if ((LOWORD(wParam) == IDOK) || (LOWORD(wParam) == IDCANCEL))
			{
				EndDialog(hDlg, LOWORD(wParam));
				return TRUE;
			}
			break;
	}
    return FALSE;
}
