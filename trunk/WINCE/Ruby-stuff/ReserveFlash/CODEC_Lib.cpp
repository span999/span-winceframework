/*


*/
#include "stdafx.h"
#include "CODEC_Lib.h"

///prototype
DWORD spAccessAudioCodec(BOOL bRead, DWORD* pdwAddr, DWORD* pdwVal, DWORD* pdwRet, DWORD* pdwSuccess );



DWORD spAccessAudioCodec(BOOL bRead, DWORD* pdwAddr, DWORD* pdwVal, DWORD* pdwRet, DWORD* pdwSuccess )
{
	DWORD dwRet = 0;
	
	AUDIO_TEST audioTest;

	audioTest.dwAddress = *pdwAddr;
	audioTest.dwValue = *pdwVal;
	audioTest.dwResult = *pdwRet;
	audioTest.dwSucc = *pdwSuccess;

	if( bRead )					
		waveOutMessage( 0, WIDM_GET_REGISTER, (DWORD)&audioTest, 0 );
	else	
		waveOutMessage( 0, WIDM_SET_REGISTER, (DWORD)&audioTest, 0 );
		
	*pdwAddr = audioTest.dwAddress;
	*pdwVal = audioTest.dwValue;
	*pdwRet = audioTest.dwResult;
	*pdwSuccess = audioTest.dwSucc;

	return dwRet;
}
