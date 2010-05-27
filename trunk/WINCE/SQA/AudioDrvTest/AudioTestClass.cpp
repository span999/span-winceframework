// AudioTestClass.cpp : implementation of Audio Test class.
//
/////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "Windows.h"
#include <commctrl.h>
#include "winbase.h"
#include "winioctl.h"
#include "AudioDrvTest.h"
#include "AudioTestClass.h"



/////////////////////////////////////////////////////////////////////////////
/// STestItemAPI
/*
STestItemAPI_______________________________________________________________()
*/

void STestItemAPI::TestContent( DWORD dwIndex )
{
///	MMRESULT mmRet;			//refer mmsystem.h, type define from UINT
///	HWAVEOUT hwaveout;
///	UINT uiMsg;
///	DWORD dwPara1;
///	DWORD dwPara2;
	UINT uiDevNum;
	DWORD dwLoop;

	///hwaveout = (HWAVEOUT)uiDevNum
	///mmRet = waveOutMessage( hwaveout, uiMsg, dwPara1, dwPara2 );
	
	pMsg->MsgOut( dDBUG, _T("API test Run %d %d\0"), dwIndex, uiTestItemID );
	
	uiDevNum = waveOutGetNumDevs();
	pLog->WriteLog( _T("STestItemAPI::waveOutGetNumDevs=%d %d\r\n"), uiDevNum, dwIndex );
	
	for( dwLoop = 0; dwLoop < uiDevNum; dwLoop++ )	///test for each wave device
	{
		GetDevOutCaps( dwLoop );		
	}///for

	uiDevNum = waveInGetNumDevs();
	pLog->WriteLog( _T("STestItemAPI::waveInGetNumDevs=%d %d\r\n"), uiDevNum, dwIndex );
	
	for( dwLoop = 0; dwLoop < uiDevNum; dwLoop++ )	///test for each wave device
	{
		GetDevInCaps( dwLoop );		
	}///for

   	
}

BOOL STestItemAPI::GetDevOutCaps( UINT uiDevID )
{
	BOOL bRet = TRUE;
	MMRESULT mmRet;			//refer mmsystem.h, type define from UINT
	WAVEOUTCAPS woc;
		
	mmRet = waveOutGetDevCaps( uiDevID, &woc, sizeof(woc) );
	pLog->WriteLog( _T("Wave Dev[%d] waveOutGetDevCaps mmRet=0x%08x\r\n"), uiDevID, mmRet );
	
	if( MMSYSERR_NOERROR == mmRet )
	{
		/* Display its Device ID and name */
		///pLog->WriteLog( _T("Wave Dev[%d] waveOutGetDevCaps mmRet=0x%08x\r\n"), uiDevID, mmRet);
		pLog->WriteLog( _T("Device ID #%u: %s\r\n"), uiDevID, woc.szPname );
		pLog->WriteLog( _T("MID:0x%04x PID:0x%04x\r\n"), woc.wMid, woc.wPid );
		pLog->WriteLog( _T("DrvVer:0x%08x Channels:%d\r\n"), woc.vDriverVersion , woc.wChannels );
		
		pLog->WriteLog( _T("WAVE_FORMAT_1M08 %s\r\n"), (0==(woc.dwFormats&WAVE_FORMAT_1M08))?L"not supported":L"supported");
		pLog->WriteLog( _T("WAVE_FORMAT_1M16 %s\r\n"), (0==(woc.dwFormats&WAVE_FORMAT_1M16))?L"not supported":L"supported");
		pLog->WriteLog( _T("WAVE_FORMAT_1S08 %s\r\n"), (0==(woc.dwFormats&WAVE_FORMAT_1S08))?L"not supported":L"supported");
		pLog->WriteLog( _T("WAVE_FORMAT_1S16 %s\r\n"), (0==(woc.dwFormats&WAVE_FORMAT_1S16))?L"not supported":L"supported");
		pLog->WriteLog( _T("WAVE_FORMAT_2M08 %s\r\n"), (0==(woc.dwFormats&WAVE_FORMAT_2M08))?L"not supported":L"supported");
		pLog->WriteLog( _T("WAVE_FORMAT_2M16 %s\r\n"), (0==(woc.dwFormats&WAVE_FORMAT_2M16))?L"not supported":L"supported");
		pLog->WriteLog( _T("WAVE_FORMAT_2S08 %s\r\n"), (0==(woc.dwFormats&WAVE_FORMAT_2S08))?L"not supported":L"supported");
		pLog->WriteLog( _T("WAVE_FORMAT_2S16 %s\r\n"), (0==(woc.dwFormats&WAVE_FORMAT_2S16))?L"not supported":L"supported");
		pLog->WriteLog( _T("WAVE_FORMAT_4M08 %s\r\n"), (0==(woc.dwFormats&WAVE_FORMAT_4M08))?L"not supported":L"supported");
		pLog->WriteLog( _T("WAVE_FORMAT_4M16 %s\r\n"), (0==(woc.dwFormats&WAVE_FORMAT_4M16))?L"not supported":L"supported");
		pLog->WriteLog( _T("WAVE_FORMAT_4S08 %s\r\n"), (0==(woc.dwFormats&WAVE_FORMAT_4S08))?L"not supported":L"supported");
		pLog->WriteLog( _T("WAVE_FORMAT_4S16 %s\r\n"), (0==(woc.dwFormats&WAVE_FORMAT_4S16))?L"not supported":L"supported");
		pLog->WriteLog( _T("WAVECAPS_LRVOLUME %s\r\n"), (0==(woc.dwSupport&WAVECAPS_LRVOLUME))?L"not supported":L"supported");
		pLog->WriteLog( _T("WAVECAPS_PITCH %s\r\n"), (0==(woc.dwSupport&WAVECAPS_PITCH))?L"not supported":L"supported");
		pLog->WriteLog( _T("WAVECAPS_PLAYBACKRATE %s\r\n"), (0==(woc.dwSupport&WAVECAPS_PLAYBACKRATE))?L"not supported":L"supported");
		pLog->WriteLog( _T("WAVECAPS_VOLUME %s\r\n"), (0==(woc.dwSupport&WAVECAPS_VOLUME))?L"not supported":L"supported");
		pLog->WriteLog( _T("WAVECAPS_SAMPLEACCURATE %s\r\n"), (0==(woc.dwSupport&WAVECAPS_SAMPLEACCURATE))?L"not supported":L"supported");

    }
    else
    {
    	if( MMSYSERR_INVALHANDLE == mmRet )
    		pLog->WriteLog( _T("Error MMSYSERR_INVALHANDLE\r\n") );
    	else	
    	if( MMSYSERR_NODRIVER == mmRet )
    		pLog->WriteLog( _T("Error MMSYSERR_NODRIVER\r\n") );
    	else	
    	if( MMSYSERR_NOMEM == mmRet )
    		pLog->WriteLog( _T("Error MMSYSERR_NOMEM\r\n") );
    		
    	bRet = FALSE;
	}

	return bRet;
}


BOOL STestItemAPI::GetDevInCaps( UINT uiDevID )
{
	BOOL bRet = TRUE;
	MMRESULT mmRet;			//refer mmsystem.h, type define from UINT
	WAVEINCAPS wic;	
		
	mmRet = waveInGetDevCaps( uiDevID, &wic, sizeof(wic) );
	pLog->WriteLog( _T("Wave Dev[%d] waveInGetDevCaps mmRet=0x%08x\r\n"), uiDevID, mmRet);
	
	if( MMSYSERR_NOERROR == mmRet )
	{
		/* Display its Device ID and name */
		pLog->WriteLog( _T("Wave Dev[%d] waveInGetDevCaps mmRet=0x%08x\r\n"), uiDevID, mmRet);
		pLog->WriteLog( _T("Device ID #%u: %s\r\n"), uiDevID, wic.szPname );
		pLog->WriteLog( _T("MID:0x%04x PID:0x%04x\r\n"), wic.wMid, wic.wPid );
		pLog->WriteLog( _T("DrvVer:0x%08x Channels:%d\r\n"), wic.vDriverVersion , wic.wChannels );
		
		pLog->WriteLog( _T("WAVE_FORMAT_1M08 %s\r\n"), (0==(wic.dwFormats&WAVE_FORMAT_1M08))?L"not supported":L"supported");
		pLog->WriteLog( _T("WAVE_FORMAT_1M16 %s\r\n"), (0==(wic.dwFormats&WAVE_FORMAT_1M16))?L"not supported":L"supported");
		pLog->WriteLog( _T("WAVE_FORMAT_1S08 %s\r\n"), (0==(wic.dwFormats&WAVE_FORMAT_1S08))?L"not supported":L"supported");
		pLog->WriteLog( _T("WAVE_FORMAT_1S16 %s\r\n"), (0==(wic.dwFormats&WAVE_FORMAT_1S16))?L"not supported":L"supported");
		pLog->WriteLog( _T("WAVE_FORMAT_2M08 %s\r\n"), (0==(wic.dwFormats&WAVE_FORMAT_2M08))?L"not supported":L"supported");
		pLog->WriteLog( _T("WAVE_FORMAT_2M16 %s\r\n"), (0==(wic.dwFormats&WAVE_FORMAT_2M16))?L"not supported":L"supported");
		pLog->WriteLog( _T("WAVE_FORMAT_2S08 %s\r\n"), (0==(wic.dwFormats&WAVE_FORMAT_2S08))?L"not supported":L"supported");
		pLog->WriteLog( _T("WAVE_FORMAT_2S16 %s\r\n"), (0==(wic.dwFormats&WAVE_FORMAT_2S16))?L"not supported":L"supported");
		pLog->WriteLog( _T("WAVE_FORMAT_4M08 %s\r\n"), (0==(wic.dwFormats&WAVE_FORMAT_4M08))?L"not supported":L"supported");
		pLog->WriteLog( _T("WAVE_FORMAT_4M16 %s\r\n"), (0==(wic.dwFormats&WAVE_FORMAT_4M16))?L"not supported":L"supported");
		pLog->WriteLog( _T("WAVE_FORMAT_4S08 %s\r\n"), (0==(wic.dwFormats&WAVE_FORMAT_4S08))?L"not supported":L"supported");
		pLog->WriteLog( _T("WAVE_FORMAT_4S16 %s\r\n"), (0==(wic.dwFormats&WAVE_FORMAT_4S16))?L"not supported":L"supported");

    }
    else
    {
    	if( MMSYSERR_INVALHANDLE == mmRet )
    		pLog->WriteLog( _T("Error MMSYSERR_INVALHANDLE\r\n") );
    	else	
    	if( MMSYSERR_NODRIVER == mmRet )
    		pLog->WriteLog( _T("Error MMSYSERR_NODRIVER\r\n") );
    	else	
    	if( MMSYSERR_NOMEM == mmRet )
    		pLog->WriteLog( _T("Error MMSYSERR_NOMEM\r\n") );
    		
    	bRet = FALSE;
	}

	return bRet;
}

BOOL STestItemAPI::_InitLog()
{
	BOOL bRet = TRUE;
	
	wsprintf( szTempString, _T("Audio driver API test.\r\n") );
	pMsg->MsgOut( szTempString );
	pLog->SetTitle( szTempString );
		
	wsprintf( szTempString, _T("This test will test the API of audio driver.\r\n") );
	pMsg->MsgOut( szTempString );
	pLog->SetInfo( szTempString );

	wsprintf( szTempString, _T("-----------------------------------------------------------\r\n") );
	pMsg->MsgOut( szTempString );
	pLog->SetSeparator( szTempString );
	
	wsprintf( szTempString, _T("AudioTestAPI.log\0") );
	pMsg->MsgOut( szTempString );
	pLog->SetLogName( szTempString );

	return bRet;
}


/////////////////////////////////////////////////////////////////////////////
/// STestItemPlayBack
/*
STestItemPlayBack__________________________________________________________()
*/

void STestItemPlayBack::TestContent( DWORD dwIndex )
{
#define 	PLAYBACK_LOOP	10
	BOOL bRet = TRUE;
	DWORD dwLoop;
	
	pMsg->MsgOut( dDBUG, _T("Playback test Run %d %d\0"), dwIndex, uiTestItemID );
	
	
	for( dwLoop = 0; dwLoop <= PLAYBACK_LOOP; dwLoop++ )
	{
		bRet = TRUE;
		
		if( 0 == ((dwLoop+dwIndex)%3) )
		{	
			if( !sndPlaySound( _T( "\\Windows\\startup.wav"), SND_ASYNC ) )
				bRet = FALSE;
		}	
		else	
		if( 1 == ((dwLoop+dwIndex)%3) )
		{	
			if( !sndPlaySound( _T( "\\Windows\\recstart.wav"), SND_ASYNC ) )
				bRet = FALSE;
		}	
		else
		{		
			if( !sndPlaySound( _T( "\\Windows\\exclam.wav"), SND_ASYNC ) )
				bRet = FALSE;			
		}	

		pLog->WriteLog( _T("Playback test sndPlaySound %s %d\r\n"), (bRet)?L"OK":L"Fail", dwIndex );
	}	

}

BOOL STestItemPlayBack::_InitLog()
{
	BOOL bRet = TRUE;
	
	wsprintf( szTempString, _T("Audio driver Playback test.\r\n") );
	pMsg->MsgOut( szTempString );
	pLog->SetTitle( szTempString );
	
	wsprintf( szTempString, _T("This test will test the function of sound playback for audio driver.\r\n") );
	pMsg->MsgOut( szTempString );
	pLog->SetInfo( szTempString );

	wsprintf( szTempString, _T("-----------------------------------------------------------\r\n") );
	pMsg->MsgOut( szTempString );
	pLog->SetSeparator( szTempString );
	
	wsprintf( szTempString, _T("AudioTestPlayBack.log\0") );
	pMsg->MsgOut( szTempString );
	pLog->SetLogName( szTempString );

	return bRet;
}


/////////////////////////////////////////////////////////////////////////////
/// STestItemVolume
/*
STestItemVolume____________________________________________________________()
*/
void STestItemVolume::TestContent( DWORD dwIndex )
{
#define 	VOLUME_LEVELS	10
	MMRESULT mmRet;			//refer mmsystem.h, type define from UINT
	DWORD dwLoop;
	DWORD dwOrgVolume;
	DWORD dwTempVolume;
	DWORD dwTemp;

	pMsg->MsgOut( dDBUG, _T("Volume test Run %d %d\0"), dwIndex, uiTestItemID );

	
	mmRet = waveOutGetVolume( 0, &dwOrgVolume );
	if( MMSYSERR_NOERROR != mmRet )
	{
		pLog->WriteLog( _T("Fail!! waveOutGetVolume mmRet=0x%08x\r\n"), mmRet );
		return;
	}
	
	for( dwLoop = 0; dwLoop <= VOLUME_LEVELS; dwLoop++ )
	{
		dwTempVolume = 0;
		dwTempVolume = (0xFFFF0000&(((dwLoop*0xFFFF)/VOLUME_LEVELS))<<16) | (0x0000FFFF&(((dwLoop*0xFFFF)/VOLUME_LEVELS))<<0) ;
		mmRet = waveOutSetVolume( 0 , dwTempVolume );
		if( dwLoop%2 )
			sndPlaySound( _T( "\\Windows\\startup.wav"), SND_SYNC );
		else	
			sndPlaySound( _T( "\\Windows\\recstart.wav"), SND_SYNC );
		mmRet = waveOutGetVolume( 0 , &dwTemp );
		pLog->WriteLog( _T("Volume test =0x%08x->0x%08x %s %d\r\n"), dwTempVolume, dwTemp, (dwTempVolume==dwTemp)?L"OK":L"Fail", dwIndex );
	}	

	mmRet = waveOutSetVolume( 0, dwOrgVolume );
	if( MMSYSERR_NOERROR != mmRet )
	{
		pLog->WriteLog( _T("Fail!! waveOutSetVolume mmRet=0x%08x\r\n"), mmRet );
		return;
	}

}

BOOL STestItemVolume::_InitLog()
{
	BOOL bRet = TRUE;
	
	wsprintf( szTempString, _T("Audio driver Volume test.\r\n") );
	pMsg->MsgOut( szTempString );
	pLog->SetTitle( szTempString );
	
	wsprintf( szTempString, _T("This test will test the function of volume control for audio driver.\r\n") );
	pMsg->MsgOut( szTempString );
	pLog->SetInfo( szTempString );
	
	wsprintf( szTempString, _T("-----------------------------------------------------------\r\n") );
	pMsg->MsgOut( szTempString );
	pLog->SetSeparator( szTempString );
	
	wsprintf( szTempString, _T("AudioTestVolume.log\0") );
	pMsg->MsgOut( szTempString );
	pLog->SetLogName( szTempString );

	return bRet;
}


/////////////////////////////////////////////////////////////////////////////
/// SAudioBasicTest
/*
SAudioBasicTest____________________________________________________________()
*/

void SAudioBasicTest::AddTestItem( UINT uiID )
{
	if( uiID == IDM_BASIC_API )
	{
		TestItemEntryArrary[TestItemEntryArraryIndex] = new STestItemAPI( uiID );
	}
	else		
	if( uiID == IDM_BASIC_PLAYBACK )
	{
		TestItemEntryArrary[TestItemEntryArraryIndex] = new STestItemPlayBack( uiID );
	}
	if( uiID == IDM_BASIC_VOLUME )
	{
		TestItemEntryArrary[TestItemEntryArraryIndex] = new STestItemVolume( uiID );
	}
	else
		;	/// error handle	

	///pLog->WriteLog( _T("SBattBasicTest::AddTestItem ID=%d Idx=%d\0"), uiID, TestItemEntryArraryIndex);
	
	TestItemEntryArraryIndex++;

	InitAllItems();
}

BOOL SAudioBasicTest::_InitLog()
{
	BOOL bRet = TRUE;
	
	wsprintf( szTempString, _T("Audio driver basic test set.\r\n") );
	pMsg->MsgOut( szTempString );
	pLog->SetTitle( szTempString );
	
	wsprintf( szTempString, _T("This test will test the basic function of audio driver.\r\n") );
	pMsg->MsgOut( szTempString );
	pLog->SetInfo( szTempString );
	
	wsprintf( szTempString, _T("-----------------------------------------------------------\r\n") );
	pMsg->MsgOut( szTempString );
	pLog->SetSeparator( szTempString );
	
	wsprintf( szTempString, _T("AudioBasicTest.log\0") );
	pMsg->MsgOut( szTempString );
	pLog->SetLogName( szTempString );

	return bRet;
}
