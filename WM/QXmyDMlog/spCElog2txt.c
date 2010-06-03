// spCElogEx.c : Defines the entry point for the application.
//


#include <windows.h>
#include "spCElog2txt.h"
#include "spCEbuf.h"
#include "spCElog.h"
#include "spCElogCmdRsp.h"
#include "spCElogCRC.h"


///
#define SPERR						NKDbgPrintfW
///#define SPDBG						NKDbgPrintfW
#define SPDBG						///void


///-///-///-///-///-///-///-///-///-///-///-///-///-///-///-///-///-///-///-///-




///-///-///-///-///-///-///-///-///-///-///-///-///-///-///-///-///-///-///-///-




///-///-///-///-///-///-///-///-///-///-///-///-///-///-///-///-///-///-///-///-
BOOL StopFile_Found()
{
	BOOL bRet = FALSE;
	HANDLE hFile = NULL;
	
	hFile = CreateFile( SPCELOG_STOPFILE_NAME, GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
	if( hFile == INVALID_HANDLE_VALUE )
		SPDBG( TEXT("SPD@StopFile_Found: not found \r\n") );
	else
	{
		SPERR( TEXT("SPD@StopFile_Found: found !!!\r\n") );
		CloseHandle( hFile );
		bRet = TRUE;
	}
	
	return bRet;
}

void DumpTag( spCEbufExtinfo* pTag )
{
	if( NULL != pTag )
	{
		///spCEbufExtinfo* pInfo = pTag->pExtraInfo;
		SPDBG( TEXT("SPD@DumpTag: ExtraInfo: 0x%08x-%d-0x%08x-%d.\r\n"), pTag->TimeStamp, pTag->Order, pTag->dwDirection, pTag->dwDataSize );

	}
}

BYTE HighHex( BYTE bNum )
{	
	BYTE bRet = 0;
	
	bRet = bNum/16;
	
	if( bRet > 9 )
		bRet += (65-10);
	else
		bRet += (48);
	
	return bRet;
}

BYTE LowHex( BYTE bNum )
{	
	BYTE bRet = 0;
	
	bRet = bNum%16;
	
	if( bRet > 9 )
		bRet += (65-10);
	else
		bRet += (48);
	
	return bRet;
}



BOOL Log2Txt_Action( DWORD dwOption )
{
	BOOL 	bRet;
	HANDLE 	hFileTarget = NULL;
	HANDLE 	hFileSource = NULL;
	spCEbufExtinfo HeadTag;
	DWORD 	dwRead = 0;
	DWORD 	dwWritten = 0;
	DWORD 	dwPayload = 0;
	DWORD 	dwCount = 0;
	///TCHAR 	szString[128];				// Temperary string
	char 	String[128];				// Temperary string
	///WCHAR 	String[128];				// Temperary string
	///USHORT 	String[128];				// Temperary string
	BYTE  	bByte = 0;
	BYTE	bChkTmp[1024];
	DWORD	dwChkCnt = 0;
	UINT16	uiCRC = 0;				///add for CRC
	

	if( !StopFile_Found() )
	{
		SPERR( TEXT("SPD@Action: not found \r\n") );
		goto EXIT_ACTION;
	}
	
	if( 0 != dwOption )
		hFileSource = CreateFile( SPCELOG_LOGFILE_NAME, GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
	else
		hFileSource = CreateFile( SPCELOG_STOPFILE_NAME, GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
	hFileTarget = CreateFile( SPCELOG_POSTFILE_NAME, GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );
	
	if( (INVALID_HANDLE_VALUE == hFileSource) || (INVALID_HANDLE_VALUE == hFileTarget) )
	{
		SPERR( TEXT("SPD@Action: CreateFile fail !!! \r\n") );
		goto EXIT_ACTION_CLOSEFILE;
	}
	
	while( dwCount < 2600000 )
	{
	
		bRet = ReadFile( hFileSource, &HeadTag, sizeof(spCEbufExtinfo), &dwRead, NULL );

		if( bRet && (dwRead == 0) )
		{
			/// you are at the end of the file.
			SPERR( TEXT("SPD@Action: Done !!!!! \r\n") );
			break;
		}
		
		if( (SPCEBUF_EXTRAINFO_TAG1 != HeadTag.dwTag1) || (SPCEBUF_EXTRAINFO_TAG2 != HeadTag.dwTag2) )
		{
			SPERR( TEXT("SPD@Action: Unkown file format ! \r\n") );
			break;
		}
	
		///DumpTag( &HeadTag );

		sprintf( String, "-----------------------------\r\n" );
		bRet = WriteFile( hFileTarget, &String, 31, &dwWritten, NULL );

		sprintf( String, "Time stamp   : %10d\r\n", HeadTag.TimeStamp );
		bRet = WriteFile( hFileTarget, &String, 27, &dwWritten, NULL );
	
		sprintf( String, "Order        : %10d\r\n", HeadTag.Order );
		bRet = WriteFile( hFileTarget, &String, 27, &dwWritten, NULL );

		if( SPCEBUF_EXTRAINFO_DIRECTION_1 == HeadTag.dwDirection )
		{
			sprintf( String, "Direction    : <<<<<<<<<<\r\n" );
			bRet = WriteFile( hFileTarget, &String, 27, &dwWritten, NULL );
			///sprintf( String, "Client Resp  : <<<<<<<<<<\r\n" );
			///bRet = WriteFile( hFileTarget, &String, 27, &dwWritten, NULL );
		}	
		else
		if( SPCEBUF_EXTRAINFO_DIRECTION_2 == HeadTag.dwDirection )
		{
			sprintf( String, "Direction    : >>>>>>>>>>\r\n" );
			bRet = WriteFile( hFileTarget, &String, 27, &dwWritten, NULL );
			///sprintf( String, "Host Command : >>>>>>>>>>\r\n" );
			///bRet = WriteFile( hFileTarget, &String, 27, &dwWritten, NULL );
		}
		else
		{
			sprintf( String, "Direction    : ==========\r\n" );
			bRet = WriteFile( hFileTarget, &String, 27, &dwWritten, NULL );
		}

		sprintf( String, "Payload Size : %10d\r\n", HeadTag.dwDataSize );
		bRet = WriteFile( hFileTarget, &String, 27, &dwWritten, NULL );

		sprintf( String, "-----------------------------\r\n" );
		bRet = WriteFile( hFileTarget, &String, 31, &dwWritten, NULL );
	
	
		dwPayload = 1;

		if( SPCEBUF_EXTRAINFO_DIRECTION_2 == HeadTag.dwDirection )
		{
			sprintf( String, ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>" );
			bRet = WriteFile( hFileTarget, &String, 50, &dwWritten, NULL );
		}

		dwChkCnt = 0;
		
		while( dwPayload <= HeadTag.dwDataSize )
		{
			bRet = ReadFile( hFileSource, &bByte, 1, &dwRead, NULL );
			
			bChkTmp[dwChkCnt] = bByte;	///we need it later for command & response check.
			
			sprintf( String, "%c%c ", HighHex( bByte ), LowHex( bByte ) );
			bRet = WriteFile( hFileTarget, &String, 3, &dwWritten, NULL );
		
			if( 0 == dwPayload%16 )
			{
				sprintf( String, "\r\n" );
				bRet = WriteFile( hFileTarget, &String, 2, &dwWritten, NULL );
				if( SPCEBUF_EXTRAINFO_DIRECTION_2 == HeadTag.dwDirection )
				{
					sprintf( String, ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>" );
					bRet = WriteFile( hFileTarget, &String, 50, &dwWritten, NULL );
				}
			}
		
			dwPayload++;
			dwChkCnt++;
		}	///end of while		

		sprintf( String, "\r\n" );
		bRet = WriteFile( hFileTarget, &String, 2, &dwWritten, NULL );	
		
		///now, parsinf the data( host command or device response?)
		SPDBG( TEXT("SPD@Log2Txt_Action: bChkTmp-0x%08x dwChkCnt-%d\r\n"), bChkTmp, dwChkCnt );

		if( SPCEBUF_EXTRAINFO_DIRECTION_1 == HeadTag.dwDirection )
		{
			///sprintf( String, "Client Resp  : <<<<<<<<<<\r\n" );
			///bRet = WriteFile( hFileTarget, &String, 27, &dwWritten, NULL );
			///sprintf( String, "Client Resp  : %s\r\n", Find_Rsp_Descript( bChkTmp, dwChkCnt ) );
			sprintf( String, "Client Resp  : %s CRC:0x%04x\r\n", Find_Rsp_Descript( bChkTmp, dwChkCnt ), FindCRC( bChkTmp, dwChkCnt-3 ) );
			bRet = WriteFile( hFileTarget, &String, strlen(String), &dwWritten, NULL );
		}	
		else
		if( SPCEBUF_EXTRAINFO_DIRECTION_2 == HeadTag.dwDirection )
		{
			///sprintf( String, "Host Command : %s\r\n", Find_Cmd_Descript( bChkTmp, dwChkCnt ) );
			sprintf( String, "Host Command : %s CRC:0x%04x\r\n", Find_Cmd_Descript( bChkTmp, dwChkCnt ), FindCRC( bChkTmp, dwChkCnt-3 ) );
			bRet = WriteFile( hFileTarget, &String, strlen(String), &dwWritten, NULL );
		}
		else
			;

		sprintf( String, "\r\n" );
		bRet = WriteFile( hFileTarget, &String, 2, &dwWritten, NULL );	
			
		dwCount++;
	}	///end of while
	
	
	
EXIT_ACTION_CLOSEFILE:	
	CloseHandle( hFileTarget );
	CloseHandle( hFileSource );
	
EXIT_ACTION:
	
	return bRet;
}

BOOL MakeLog2txt()
{
	return Log2Txt_Action( 1 );
}

BOOL MakeCopyLog2txt()
{
	return Log2Txt_Action( 0 );
}

//-------------------------------------------------------------------
#ifndef LIB_BUILD
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpszCmdLine, int cmdShow )
{

	Log2Txt_Action( 0 );
 
	return 0 ;
}
#endif
