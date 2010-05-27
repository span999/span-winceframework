
#include "stdafx.h"
///#include <winuserm.h>
///#include "Thales_defines.h"
#include <commctrl.h>
#include "resource.h"
///#include "CODEC_Lib.h"
#include "SWvolume_Dlg.h"


#define MRCHECK(r,str,label)\
    if ((r != MMSYSERR_NOERROR)) { RETAILMSG(1, (TEXT(#str) TEXT(" failed. mr=%08x\r\n"), r)); mr = r; goto label;}

// -----------------------------------------------------------------------------
//                              FileHeader
// -----------------------------------------------------------------------------
typedef struct
{
   DWORD   dwRiff;     // Type of file header.
   DWORD   dwSize;     // Size of file header.
   DWORD   dwWave;     // Type of wave.
} RIFF_FILEHEADER, *PRIFF_FILEHEADER;


// -----------------------------------------------------------------------------
//                              ChunkHeader
// -----------------------------------------------------------------------------
typedef struct
{
   DWORD   dwCKID;        // Type Identification for current chunk header.
   DWORD   dwSize;        // Size of current chunk header.
} RIFF_CHUNKHEADER, *PRIFF_CHUNKHEADER;

/*  Chunk Types  
*/
#define RIFF_FILE       mmioFOURCC('R','I','F','F')
#define RIFF_WAVE       mmioFOURCC('W','A','V','E')
#define RIFF_FORMAT     mmioFOURCC('f','m','t',' ')
#define RIFF_CHANNEL    mmioFOURCC('d','a','t','a')


#define SOUND1			0
#define SOUND2			1
#define SOUND3			2

#define SOUND1FILE		_T("\\Windows\\startup.wav")
#define SOUND2FILE		_T("\\Windows\\recstart.wav")
#define SOUND3FILE		_T("\\Windows\\exclam.wav")

#define MAX_LOADSTRING 100

// Global Variables:
///HINSTANCE			hInst;			// The current instance
//HWND				hwndCB;			// The command bar handle

HWAVEOUT			hwo[3];
HANDLE				hevDone[3];
UINT				nAudioDeviceID;
static DWORD dwVolume[3] = {0,0,0};
static DWORD dwMainVolume = 0;
#if 0
static DWORD dwVolMap[16] = {
							0x0000,
							0x1111,
							0x2222,
							0x3333,
							0x4444,
							0x5555,
							0x6666,
							0x7777,
							0x8888,
							0x9999,
							0xAAAA,
							0xBBBB,
							0xCCCC,
							0xDDDD,
							0xEEEE,
							0xFFFF
							};
#else
static DWORD dwVolMap[16] = {
							0xFFFF,
							0xEEEE,
							0xDDDD,
							0xCCCC,
							0xBBBB,
							0xAAAA,
							0x9999,
							0x8888,
							0x7777,
							0x6666,
							0x5555,
							0x4444,
							0x3333,
							0x2222,
							0x1111,
							0x0000
							};
#endif
///prototype
DWORD SWvolumeDialogBoxHook( HINSTANCE hInstance, HWND hWnd );
LRESULT CALLBACK SWvolume(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);



DWORD SWvolumeDialogBoxHook( HINSTANCE hInstance, HWND hWnd)
{
	DWORD dwRet = 0;

	DialogBox(hInstance, (LPCTSTR)IDD_SWVOLUMEBOX, hWnd, (DLGPROC)SWvolume);
	
	return dwRet;
}


DWORD SetSWvolume(UINT uiIndex, DWORD dwVol)
{
	MMRESULT mr;
	DWORD dwRet = 0;
	WORD wVol = LOWORD(dwVol);
/*	
	if ((wVol - 0x1111) > 0x0)
	{
		wVol -= 0x1111;
	}
*/	
	dwRet = ((wVol << 16) | wVol);
	
	// Using the handle to the open waveform-audio output device
	// results in the audio volume only being set for this
	// instance of the application.
	mr = waveOutSetVolume(hwo[uiIndex], dwRet);
	
	if (MMSYSERR_NOERROR != mr)
	{
		MessageBox(NULL, _T("Init Audio Error 0005"), _T("Error"), MB_OK);
	}
	
	return dwRet;
}
					
					
BOOL ReadChunk(HANDLE fh, DWORD dwChunkType, PVOID * ppBuffer, DWORD * pdwSize, PDWORD pdwBytesLeft)
{ 
	DWORD dwBytesRead;
	PVOID pBuffer;
	RIFF_CHUNKHEADER Chunk;

    if ((!pdwBytesLeft) || (*pdwBytesLeft <= 0) || (!pdwSize) || (!ppBuffer)) {
        RETAILMSG(1, (TEXT("Invalid parameter to ReadChunk()\r\n")));
        return FALSE;
    }

    // now scan for the format chunk
    while (*pdwBytesLeft > 0) {
        // now read the wave header (or what we hope is the wave header)
        if (! ReadFile(fh, &Chunk, sizeof(Chunk), &dwBytesRead, NULL) || dwBytesRead < sizeof(Chunk)) {
            RETAILMSG(1, (TEXT("Error reading chunk header\n")));
            return FALSE;
        }
        *pdwBytesLeft -= dwBytesRead;
        RETAILMSG(1, (TEXT("Chunk: \"%c%c%c%c\" size=0x%08x\r\n"), 
            (Chunk.dwCKID >>  0) & 0xff, 
            (Chunk.dwCKID >>  8) & 0xff, 
            (Chunk.dwCKID >> 16) & 0xff, 
            (Chunk.dwCKID >> 24) & 0xff, 
            Chunk.dwSize));
        if (Chunk.dwCKID == dwChunkType) {
            // found the desired chunk
            break;
        }
        // skip the data we don't know or care about...
        if (0xFFFFFFFF == SetFilePointer (fh, Chunk.dwSize, NULL, FILE_CURRENT)) {
            RETAILMSG(1,  (TEXT("Error setting file pointer while scanning for chunk\n")));
            return FALSE;
        }
        *pdwBytesLeft -= Chunk.dwSize;
    }
    // found the desired chunk.
    // allocate a buffer and read in the data
    pBuffer = new BYTE[Chunk.dwSize];
    if (pBuffer == NULL) {
        RETAILMSG(1, (TEXT("Unable to allocate chunk buffer\r\n")));
        return FALSE;
    }
    if (! ReadFile(fh, pBuffer, Chunk.dwSize, &dwBytesRead, NULL) || dwBytesRead < Chunk.dwSize) {
        delete [] pBuffer;
        RETAILMSG(1, (TEXT("Unable to read chunk data\r\n")));
        return FALSE;
    }
    *pdwBytesLeft -= dwBytesRead;
    *ppBuffer = pBuffer;
    *pdwSize = Chunk.dwSize;
    return TRUE;
}


MMRESULT ReadWaveFile(LPCTSTR pszFilename, PWAVEFORMATEX * ppWFX, PDWORD pdwBufferSize, PBYTE * ppBufferBits)
{
  RIFF_FILEHEADER FileHeader;
  DWORD dwBytesRead;
  DWORD dwBufferSize;
  DWORD dwFormatSize;
  PBYTE pBufferBits = NULL;
  PWAVEFORMATEX pwfx = NULL;
  DWORD dwBytesInChunk;
  HANDLE fh;
  MMRESULT mmRet = MMSYSERR_ERROR;

    fh = CreateFile(pszFilename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
    if( fh == INVALID_HANDLE_VALUE ) {
        RETAILMSG(1, (TEXT("Error opening %s. Error code = 0x%08x\n"), pszFilename, GetLastError()));
        return mmRet;
    }

    // Read file and determine sound format
    // Start with RIFF header:

    if (! ReadFile(fh, &FileHeader, sizeof(FileHeader), &dwBytesRead, NULL) || dwBytesRead < sizeof(FileHeader)) {
        RETAILMSG(1, (TEXT("Error reading file header\n")));
        goto ERROR_EXIT;
    }

    if ( FileHeader.dwRiff != RIFF_FILE || FileHeader.dwWave != RIFF_WAVE) {
        RETAILMSG(1, (TEXT("Invalid wave file header\n")));
        goto ERROR_EXIT;
    }

    dwBytesInChunk = FileHeader.dwSize;

    // load the wave format
    if (! ReadChunk(fh, RIFF_FORMAT, (PVOID*) &pwfx, &dwFormatSize, &dwBytesInChunk)) {
        RETAILMSG(1, (TEXT("Unable to read format chunk\r\n")));
        goto ERROR_EXIT;
    }
    if (dwFormatSize < sizeof(PCMWAVEFORMAT)) {
        RETAILMSG(1, (TEXT("Format record too small\r\n")));
        goto ERROR_EXIT;
    }

    // load the wave data
    if (! ReadChunk(fh, RIFF_CHANNEL, (PVOID*) &pBufferBits, &dwBufferSize, &dwBytesInChunk)) {
        RETAILMSG(1, (TEXT("Unable to read format chunk\r\n")));
        goto ERROR_EXIT;
    }

    *ppWFX = pwfx;
    *pdwBufferSize = dwBufferSize;
    *ppBufferBits = pBufferBits;

    // Success
    mmRet = MMSYSERR_NOERROR;
    goto EXIT;

ERROR_EXIT:
    delete [] pBufferBits;
    delete [] pwfx;
EXIT:
    CloseHandle(fh);
    return mmRet;   
}


MMRESULT PlayFile(UINT uiIndex, LPCTSTR pszFilename)
{
	MMRESULT		mr;
	DWORD			dwBufferSize;
	PBYTE			pBufferBits = NULL;
	PWAVEFORMATEX	pwfx = NULL;
	DWORD			dwSlop;
	DWORD			dwWait; 
	DWORD			dwDuration;
  
    mr = ReadWaveFile(pszFilename,&pwfx,&dwBufferSize,&pBufferBits);
    MRCHECK(mr, ReadWaveFile, ERROR_READ);

    dwDuration = dwBufferSize * 1000 / pwfx->nAvgBytesPerSec;

    RETAILMSG(1, (TEXT("\"%s\" %c%02d %5dHz %6d bytes %5d ms\r\n")
        , pszFilename
        , pwfx->nChannels == 2 ? L'S' : L'M' 
        , pwfx->wBitsPerSample
        , pwfx->nSamplesPerSec
        , dwBufferSize
        , dwDuration
        ));
    
    WAVEHDR hdr;
    memset(&hdr, 0, sizeof(hdr));
    hdr.dwBufferLength = dwBufferSize;
    hdr.lpData = (char *) pBufferBits;

    mr = waveOutPrepareHeader(hwo[uiIndex], &hdr, sizeof(hdr));
    MRCHECK(mr, waveOutPrepareHeader, ERROR_PLAY);

    mr = waveOutWrite(hwo[uiIndex], &hdr, sizeof(hdr));
    MRCHECK(mr, waveOutWrite, ERROR_PLAY);

    // wait for play + 1 second slop
    dwSlop = 1000;
    dwWait = WaitForSingleObject(hevDone[uiIndex], dwDuration + dwSlop); 
    if (dwWait != WAIT_OBJECT_0) {
        // not much to here, other than issue a warning
        RETAILMSG(1, (TEXT("Timeout waiting for playback to complete\r\n")));
    }

    mr = waveOutUnprepareHeader(hwo[uiIndex], &hdr, sizeof(hdr));
    MRCHECK(mr, waveOutUnprepareHeader, ERROR_PLAY);
    
ERROR_PLAY:
    delete [] pBufferBits;
    delete [] pwfx;

ERROR_READ:
    return mr;
}


MMRESULT InitSound(UINT uiIndex)
{
	MMRESULT		mr;
	DWORD			dwBufferSize;
	PBYTE			pBufferBits = NULL;
	PWAVEFORMATEX	pwfx;
	HWAVEOUT		*phwo = NULL;	

    hevDone[uiIndex] = CreateEvent(NULL, FALSE, FALSE, NULL);
    if (hevDone[uiIndex] == NULL)
	{
		return MMSYSERR_NOMEM;
    }

	if( uiIndex == SOUND1 )
    	mr = ReadWaveFile(SOUND1FILE, &pwfx, &dwBufferSize, &pBufferBits);
    else
	if( uiIndex == SOUND2 )
    	mr = ReadWaveFile(SOUND2FILE, &pwfx, &dwBufferSize, &pBufferBits);
    else
	if( uiIndex == SOUND3 )
    	mr = ReadWaveFile(SOUND3FILE, &pwfx, &dwBufferSize, &pBufferBits);
    else
    	MessageBox(NULL, _T("Init Audio Error 0001"), _T("Error"), MB_OK);

	phwo = hwo+uiIndex;
    mr = waveOutOpen(phwo, 0, pwfx, (DWORD)hevDone[uiIndex], NULL, CALLBACK_EVENT);
	if (MMSYSERR_NOERROR != mr)
	{
		MessageBox(NULL, _T("Init Audio Error 0002"), _T("Error"), MB_OK);
	}

	mr = waveOutGetID(hwo[uiIndex], &nAudioDeviceID);
	if (MMSYSERR_NOERROR != mr)
	{
		MessageBox(NULL, _T("Init Audio Error 0003"), _T("Error"), MB_OK);
	}

	delete [] pBufferBits;
	delete [] pwfx;

    return mr;
}


void CloseSound()
{
	waveOutClose(hwo[SOUND1]);
    CloseHandle(hevDone[SOUND1]);
	waveOutClose(hwo[SOUND2]);
    CloseHandle(hevDone[SOUND2]);
	waveOutClose(hwo[SOUND3]);
    CloseHandle(hevDone[SOUND3]);
}


DWORD ChangeChannelVolume(DWORD dwBase, DWORD dwOffset, BOOL bIncrease)
{
	DWORD dwRet;
	
	if( bIncrease )
	{	///increase
		if( (0xFFFF-dwBase) >= dwOffset )
			dwRet = dwBase + dwOffset;
		else	
			dwRet = 0xFFFF;
	}
	else
	{	///decrease
		if( dwOffset <= dwBase )
			dwRet = dwBase - dwOffset;
		else
			dwRet = 0x0;
	}
	
	return dwRet;
}



DWORD ChangeMainVolume(DWORD dwBase, DWORD dwOffset, BOOL bIncrease)
{
	DWORD dwRet = 0;
	
	dwRet |= (ChangeChannelVolume(((dwBase&0xFFFF0000)>>16), dwOffset, bIncrease)&0xFFFF)<<16;
	dwRet |= (ChangeChannelVolume((dwBase&0x0000FFFF), dwOffset, bIncrease)&0xFFFF);
	
	return dwRet;
}


// Mesage handler for the Volume box.
LRESULT CALLBACK SWvolume(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	RECT rt, rt1;
	int DlgWidth, DlgHeight;	// dialog width and height in pixel units
	int NewPosX, NewPosY;
	TCHAR szString[64];         // Temperary string
	static HWND SliderContentArrary[3];	
	
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

			///Sound1
			SliderContentArrary[0] = GetDlgItem(hDlg,IDC_SWVOLUME_SLIDER1);
			///Sound2
			SliderContentArrary[1] = GetDlgItem(hDlg,IDC_SWVOLUME_SLIDER2);
			///Sound3
			SliderContentArrary[2] = GetDlgItem(hDlg,IDC_SWVOLUME_SLIDER3);

			///adjust slider1
			SendMessage(SliderContentArrary[0],TBM_SETRANGE,TRUE,MAKELONG(0, 16));	// range
			SendMessage(SliderContentArrary[0],TBM_SETTICFREQ,1,0L);				// tick freq.
			SendMessage(SliderContentArrary[0],TBM_SETPAGESIZE,0L,1);				// page up/dn size
			///adjust slider2
			SendMessage(SliderContentArrary[1],TBM_SETRANGE,TRUE,MAKELONG(0, 16));	// range
			SendMessage(SliderContentArrary[1],TBM_SETTICFREQ,1,0L);				// tick freq.
			SendMessage(SliderContentArrary[1],TBM_SETPAGESIZE,0L,1);				// page up/dn size
			///adjust slider3
			SendMessage(SliderContentArrary[2],TBM_SETRANGE,TRUE,MAKELONG(0, 16));	// range
			SendMessage(SliderContentArrary[2],TBM_SETTICFREQ,1,0L);				// tick freq.
			SendMessage(SliderContentArrary[2],TBM_SETPAGESIZE,0L,1);				// page up/dn size


			waveOutGetVolume( 0, &dwMainVolume );
			wsprintf(szString,_T("x%08X"),dwMainVolume);
			SetDlgItemText(hDlg, IDC_SWVOLUME_EDIT4, szString);				

			
			///TODO: prepare and play the three DEMO file
			if (MMSYSERR_NOERROR == InitSound(SOUND1))
			{
				// Using the handle to the open waveform-audio output device
				// results in the audio volume only being set for this
				// instance of the application.
				waveOutGetVolume(hwo[SOUND1], (dwVolume+SOUND1));
				wsprintf(szString,_T("x%08X"),dwVolume[SOUND1]);
				SetDlgItemText(hDlg, IDC_SWVOLUME_EDIT1, szString);				
			}
			if (MMSYSERR_NOERROR == InitSound(SOUND2))
			{
				// Using the handle to the open waveform-audio output device
				// results in the audio volume only being set for this
				// instance of the application.
				waveOutGetVolume(hwo[SOUND2], (dwVolume+SOUND2));
				wsprintf(szString,_T("x%08X"),dwVolume[SOUND2]);
				SetDlgItemText(hDlg, IDC_SWVOLUME_EDIT2, szString);				
			}
			if (MMSYSERR_NOERROR == InitSound(SOUND3))
			{
				// Using the handle to the open waveform-audio output device
				// results in the audio volume only being set for this
				// instance of the application.
				waveOutGetVolume(hwo[SOUND3], (dwVolume+SOUND3));
				wsprintf(szString,_T("x%08X"),dwVolume[SOUND3]);
				SetDlgItemText(hDlg, IDC_SWVOLUME_EDIT3, szString);								
			}

			
			return TRUE;

		case WM_HSCROLL:            // track bar message
		case WM_VSCROLL:            // track bar message
			DWORD deIndex;
			DWORD dwValue;

///			DWORD dwAddr, dwVal, dwRet, dwSuccess;
			
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
					///TODO:
						if( (HWND)lParam == SliderContentArrary[0] )
						{	
							///it's sound1 volume
							deIndex = 0;
							if( BST_CHECKED == SendDlgItemMessage(hDlg, IDC_SWVOLUME_CHECK1, BM_GETCHECK, (WPARAM)NULL, (LPARAM)NULL ) )
							{
								dwValue = SetSWvolume(SOUND1, 0x0000);
								wsprintf(szString,_T("mute"));
								SetDlgItemText(hDlg, IDC_SWVOLUME_EDIT1, szString);								
							}
							else
							{
								dwValue = SendMessage(SliderContentArrary[deIndex],TBM_GETPOS,0,0);
								dwValue = SetSWvolume(SOUND1, dwVolMap[dwValue]);
								wsprintf(szString,_T("x%08X"),dwValue);
								SetDlgItemText(hDlg, IDC_SWVOLUME_EDIT1, szString);
							}	

						}
						else
						if( (HWND)lParam == SliderContentArrary[1] )
						{	
							///it's sound2 volume
							deIndex = 1;
							if( BST_CHECKED == SendDlgItemMessage(hDlg, IDC_SWVOLUME_CHECK2, BM_GETCHECK, (WPARAM)NULL, (LPARAM)NULL ) )
							{
								dwValue = SetSWvolume(SOUND2, 0x0000);
								wsprintf(szString,_T("mute"));
								SetDlgItemText(hDlg, IDC_SWVOLUME_EDIT2, szString);								
							}
							else
							{
								dwValue = SendMessage(SliderContentArrary[deIndex],TBM_GETPOS,0,0);
								dwValue = SetSWvolume(SOUND2, dwVolMap[dwValue]);
								wsprintf(szString,_T("x%08X"),dwValue);
								SetDlgItemText(hDlg, IDC_SWVOLUME_EDIT2, szString);
							}	

						}
						else
						if( (HWND)lParam == SliderContentArrary[2] )
						{	
							///it's sound3 volume
							deIndex = 2;
							if( BST_CHECKED == SendDlgItemMessage(hDlg, IDC_SWVOLUME_CHECK3, BM_GETCHECK, (WPARAM)NULL, (LPARAM)NULL ) )
							{
								dwValue = SetSWvolume(SOUND3, 0x0000);
								wsprintf(szString,_T("mute"));
								SetDlgItemText(hDlg, IDC_SWVOLUME_EDIT3, szString);								
							}
							else
							{
								dwValue = SendMessage(SliderContentArrary[deIndex],TBM_GETPOS,0,0);
								dwValue = SetSWvolume(SOUND3, dwVolMap[dwValue]);
								wsprintf(szString,_T("x%08X"),dwValue);
								SetDlgItemText(hDlg, IDC_SWVOLUME_EDIT3, szString);
							}	

						}
																			
					return TRUE;
				default:
					// Default case
					return FALSE;
			}
			break;

		case WM_COMMAND:

			if ((LOWORD(wParam) == IDC_SWVOLUME_PLAY) )
			{
				PlayFile(SOUND1, SOUND1FILE);
				PlayFile(SOUND2, SOUND2FILE);
				PlayFile(SOUND3, SOUND3FILE);
				return TRUE;
			}
			else
			if ((LOWORD(wParam) == IDC_SWVOLUME_UP) )
			{				
				waveOutGetVolume( 0, &dwMainVolume );
				
				dwMainVolume = ChangeMainVolume(dwMainVolume, 1000, TRUE);
				
				waveOutSetVolume( 0, dwMainVolume );
				wsprintf(szString,_T("x%08X"),dwMainVolume);
				SetDlgItemText(hDlg, IDC_SWVOLUME_EDIT4, szString);				
				
				return TRUE;
			}
			else
			if ((LOWORD(wParam) == IDC_SWVOLUME_DOWN) )
			{
				waveOutGetVolume( 0, &dwMainVolume );
				
				dwMainVolume = ChangeMainVolume(dwMainVolume, 1000, FALSE);
				
				waveOutSetVolume( 0, dwMainVolume );
				wsprintf(szString,_T("x%08X"),dwMainVolume);
				SetDlgItemText(hDlg, IDC_SWVOLUME_EDIT4, szString);				
				
				return TRUE;
			}
			else				
			if ((LOWORD(wParam) == IDC_SWVOLUME_UP2) )
			{				
				waveOutGetVolume( 0, &dwMainVolume );
				
				dwMainVolume = ChangeMainVolume(dwMainVolume, 100, TRUE);
				
				waveOutSetVolume( 0, dwMainVolume );
				wsprintf(szString,_T("x%08X"),dwMainVolume);
				SetDlgItemText(hDlg, IDC_SWVOLUME_EDIT4, szString);				
				
				return TRUE;
			}
			else
			if ((LOWORD(wParam) == IDC_SWVOLUME_DOWN2) )
			{
				waveOutGetVolume( 0, &dwMainVolume );
				
				dwMainVolume = ChangeMainVolume(dwMainVolume, 100, FALSE);
				
				waveOutSetVolume( 0, dwMainVolume );
				wsprintf(szString,_T("x%08X"),dwMainVolume);
				SetDlgItemText(hDlg, IDC_SWVOLUME_EDIT4, szString);				
				
				return TRUE;
			}
			else				
#if 0
			if ((LOWORD(wParam) == IDC_SWVOLUME_STOP) )
			{
				return TRUE;
			}
			else
#endif				
			if ((LOWORD(wParam) == IDOK) || (LOWORD(wParam) == IDCANCEL))
			{
				EndDialog(hDlg, LOWORD(wParam));
				return TRUE;
			}
			break;
	}
    return FALSE;
}
