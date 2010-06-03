// spCElog.c : Defines the entry point for the application.
//

///#include <stdafx.h>
#include <windows.h>
///#include "spCElog.h"
#include "spCEbuf.h"
///#include "spCElog2txt.h"
#include "spCElogDLF.h"
#include "spCElogCmdRsp.h"
#include "spCElogCRC.h"

///
#define SPERR						NKDbgPrintfW
#define SPDBG						NKDbgPrintfW
///#define SPDBG						///void
///#define FIXED_DLF_CRC_TABLE

typedef struct SPCELOG_DLF_T
{
	DWORD 		dwInited;
} spCElog_Dlf;

#define _ASYNC_CONTROL_CHAR 		0x7E /* Flag character value.  Corresponds  to the ~ character                  */
#define _ASYNC_ESC_CHAR     		0x7D /* Escape sequence 1st character value */
#define _ASYNC_ESC_MASK     		0x20 /* Escape sequence complement value    */
#define _ASYNC_TAIL_SIZE    		3    /* CRC plus tail character             */

static spCElog_Dlf					dlf_status;

#define	DLFHeaderLen		220

static BYTE DLFHeader[DLFHeaderLen] = { 
	0xA1, 0x00, 0xFD, 0x1F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00,
	0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x49, 0x53, 0x46, 0x43, 0x6F, 0x6E, 0x76, 0x65, 0x72,
	0x74, 0x65, 0x72, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30,
	0x33, 0x2F, 0x30, 0x39, 0x2F, 0x30, 0x39, 0x20, 0x31, 0x36, 0x3A, 0x30, 0x39, 0x3A, 0x30, 0x32,
	0x00, 0x7E, 0x16, 0x18, 0x4A, 0x00, 0x00, 0x00, 0x00, 0x2A, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x3B, 0x00, 0xFC, 0x1F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x51, 0x58, 0x44,
	0x4D, 0x20, 0x50, 0x72, 0x6F, 0x66, 0x65, 0x73, 0x73, 0x69, 0x6F, 0x6E, 0x61, 0x6C, 0x20, 0x30,
	0x33, 0x2E, 0x31, 0x32, 0x2E, 0x33, 0x30, 0x34, 0x20, 0x28, 0x50, 0x72, 0x6F, 0x64, 0x75, 0x63,
	0x74, 0x69, 0x6F, 0x6E, 0x20, 0x42, 0x75, 0x69, 0x6C, 0x64, 0x29, 0x00///, 0x0D, 0x00
    };

static BYTE DLFHostCmdHead[2] = { 0xEA, 0x1F };			///DIAG request
static BYTE DLFDeviceRspHead[2] = { 0xF0, 0x1F };		///DIAG response
static BYTE DLFDeviceEventHead[2] = { 0xFB, 0x1F };		///EVENT record

static BYTE DLFDeviceGSPHead[2] = { 0xF1, 0x1F };			///GPS info
static BYTE DLFExtDebugMsgHead[2] = { 0xEB, 0x1F };			///Ext debug msg record
static BYTE DLF31DebugMsgHead[2] = { 0xEC, 0x1F };			///Cmd 31 debug msg record

static BYTE DLFCmdRspPAD[8] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

static BYTE DLFCmdChksum[2] = { 0x10, 0x00 };
static BYTE DLFRspChksum[2] = { 0x0D, 0x00 };

static BOOL bReport_Control_On = FALSE;
static DWORD dwPriCmdDataSize = 0;


//proto type
BOOL spCElog_WriteEvent2DLF( PUCHAR puRawData, DWORD dwRawSize, HANDLE hDLFhandler );
#ifdef USE_SEPARATE_PARSE    ///span0719, support mixed data type in single packet 
BOOL spCElog_WriteMSG2DLF( PUCHAR puRawData, DWORD dwRawSize, HANDLE hDLFhandler );
BOOL spCElog_WriteLOG2DLF( PUCHAR puRawData, DWORD dwRawSize, HANDLE hDLFhandler );
#else
BOOL spCElog_WriteMSGLOG2DLF( PUCHAR puRawData, DWORD dwRawSize, HANDLE hDLFhandler );
#endif	///USE_SEPARATE_PARSE





BOOL Is_DLF_FileHeader_Inited()
{
	BOOL bRet;
	
	///if( dlf_status.dwInited > 3 )
	if( dlf_status.dwInited > 0 )
		bRet = TRUE;
	else
		bRet = FALSE;
	
	return bRet;
}

BOOL Set_DLF_FileHeader_Init( BOOL bSet )
{
	BOOL bRet;
	
	if( bSet )
		dlf_status.dwInited++;
	else	
		dlf_status.dwInited = 0;

	bRet = Is_DLF_FileHeader_Inited();
	return bRet;
}

PBYTE spCElog_GetDLF_Cmd_Chksum()
{
	return DLFCmdChksum;
}

PBYTE spCElog_GetDLF_Rsp_Chksum()
{
	return DLFRspChksum;
}

PBYTE spCElog_FindDLF_Cmd_Chksum( PUCHAR puData, DWORD dwSize )
{
	PBYTE pRet = NULL;
	
	pRet = Find_Cmd_Chksum( puData, dwSize );

	if( NULL == pRet )
		pRet = DLFCmdChksum;
	
	return pRet;
}

PBYTE spCElog_FindDLF_Rsp_Chksum( PUCHAR puData, DWORD dwSize )
{
	PBYTE pRet = NULL;
	
	pRet = Find_Rsp_Chksum( puData, dwSize );
	
	if( NULL == pRet )
		pRet = DLFRspChksum;
	
	return pRet;
}

BOOL Is_Ignore_Cmd( PUCHAR puData, DWORD dwSize )
{
	BOOL bRet = FALSE;
	
	bRet = Is_Polling_Cmd( puData, dwSize );
	
	if( bRet )
		SPDBG( TEXT("SPD@Is_Ignore_Rsp: Ignore request !!\r\n") );
	
	return bRet;
}

BOOL Is_Ignore_Rsp( PUCHAR puData, DWORD dwSize )
{
	BOOL bRet = FALSE;
	
	bRet = Is_Polling_Rsp( puData, dwSize );
	
	if( bRet )
		SPDBG( TEXT("SPD@Is_Ignore_Rsp: Ignore response !!\r\n") );
	
	return bRet;
}

BOOL Reverse_escape_coding( PUCHAR pBuffer, PDWORD pdwDataSize )
{
	BOOL 	bRet = FALSE;	///not chnaged
	PUCHAR	pTemp = NULL;
	PUCHAR	pTempbuf = NULL;
	DWORD 	dwCount = 0;
	DWORD	dwESCcnt = 0;
	DWORD	dwLen = 0;
	
	
	dwLen = (128 == (*pdwDataSize)) ? (*pdwDataSize) : (*pdwDataSize)-3;
	///allocate memory
	pTemp = (PUCHAR)malloc( *pdwDataSize );
	pTempbuf = pTemp;
	
	SPDBG( TEXT("SPD@reverse_escape_coding: 0x%08x 0x%08x size=%d\r\n"), pTemp, pBuffer, *pdwDataSize );

	memcpy( pTemp, pBuffer, *pdwDataSize );
	
	///check each byte...
	for( dwCount = 0; dwCount < dwLen; dwCount++ )
	{
		///SPDBG( TEXT("SPD@reverse_escape_coding: %d < %d \r\n"), dwCount, dwLen );
		if( _ASYNC_ESC_CHAR == *pTemp )
		{
			pTemp++;	///move next
			dwCount++;
			dwESCcnt++;
			*pBuffer = ((*pTemp)^_ASYNC_ESC_MASK);
		}
		else	///just copy it
		{
			*pBuffer = *pTemp;
		}
		
		pTemp++;
		pBuffer++;
	}
	
	SPDBG( TEXT("SPD@reverse_escape_coding:10 0x%02x 0x%02x\r\n"), *pTemp, *pBuffer );
	///free( pTemp );
	free( pTempbuf );
	
	*pdwDataSize = *pdwDataSize - dwESCcnt;
	
	if( dwESCcnt > 0 )
	{
		bRet = TRUE;
		SPERR( TEXT("SPD@reverse_escape_coding: escape code found %d\r\n"), dwESCcnt );
	}
	else
		SPDBG( TEXT("SPD@reverse_escape_coding: NO escape code found \r\n") );
		
	return bRet;
}

PUCHAR Raw_Command_data_combine( PUCHAR pBuffer, PDWORD pdwDataSize )
{
	static PUCHAR pCombineBuf = NULL;
	static DWORD dwTotalCnt = 0;
	static PUCHAR pCurCnt = NULL;
	PUCHAR pRet = NULL;
	
	if( NULL == pCombineBuf )
	{
		pCombineBuf = (PUCHAR)malloc( 1024 );	///1k at least
		pCurCnt = pCombineBuf;
	}	

	dwTotalCnt = dwTotalCnt + (*pdwDataSize);	///total
	
	if( 1024 < dwTotalCnt )
		SPERR( TEXT("SPD@Raw_Command_data_combine: Buffer full !!!!!!!!!!!!!!. now pCurCnt=0x%08x, size=%d \r\n"), pCurCnt, dwTotalCnt );
	
	memcpy( pCurCnt, pBuffer, *pdwDataSize );	///copy the data to buffer
	pCurCnt = pCurCnt + (*pdwDataSize);			///move the index
	
	if( 128 == *pdwDataSize )
	{	///wait for cont. data
		SPDBG( TEXT("SPD@Raw_Command_data_combine: wait coming cmd. now pCurCnt=0x%08x, size=%d \r\n"), pCurCnt, dwTotalCnt );
		*pdwDataSize = dwTotalCnt;	///size
		pRet = NULL;				///still wait for more data, no address return
	}
	else
	{
		///returns
		pRet = pCombineBuf;			///buffer
		*pdwDataSize = dwTotalCnt;	///size

		///reset it
		pCurCnt = pCombineBuf;
		dwTotalCnt = 0;		
	}
	
	SPDBG( TEXT("SPD@Raw_Command_data_combine: 0x%08x 0x%08x %d %d \r\n"), pRet, pCombineBuf, *pdwDataSize, dwTotalCnt );
	return pRet;
}

///span0719, support response data size over 1k
PUCHAR Raw_Rsponse_data_combine( PUCHAR pBuffer, PDWORD pdwDataSize )
{
	static PUCHAR pCombineBuf = NULL;
	static DWORD dwTotalCnt = 0;
	static PUCHAR pCurCnt = NULL;
	PUCHAR pRet = NULL;
	
	if( NULL == pCombineBuf )
	{
		pCombineBuf = (PUCHAR)malloc( 2048 );	///2k at least
		pCurCnt = pCombineBuf;
	}	

	dwTotalCnt = dwTotalCnt + (*pdwDataSize);	///total
	
	if( 2048 < dwTotalCnt )
		SPERR( TEXT("SPD@Raw_Rsponse_data_combine: Buffer full !!!!!!!!!!!!!!. now pCurCnt=0x%08x, size=%d \r\n"), pCurCnt, dwTotalCnt );
	
	memcpy( pCurCnt, pBuffer, *pdwDataSize );	///copy the data to buffer
	pCurCnt = pCurCnt + (*pdwDataSize);			///move the index
	
	if( 1024 == *pdwDataSize )
	{	///wait for cont. data
		SPDBG( TEXT("SPD@Raw_Rsponse_data_combine: wait coming cmd. now pCurCnt=0x%08x, size=%d \r\n"), pCurCnt, dwTotalCnt );
		*pdwDataSize = dwTotalCnt;	///size
		pRet = NULL;				///still wait for more data, no address return
	}
	else
	{
		///returns
		pRet = pCombineBuf;			///buffer
		*pdwDataSize = dwTotalCnt;	///size

		///reset it
		pCurCnt = pCombineBuf;
		dwTotalCnt = 0;		
	}
	
	SPDBG( TEXT("SPD@Raw_Rsponse_data_combine: 0x%08x 0x%08x %d %d \r\n"), pRet, pCombineBuf, *pdwDataSize, dwTotalCnt );
	return pRet;
}


void spCElog_CheckDLFFile( HANDLE hDLFhandler )
{
	DWORD dwFileSizeLow = 0;
	DWORD dwFileSizeHigh = 0;
	DWORD dwWriten = 0;
	BOOL bRet = TRUE;
	
	dwFileSizeLow = GetFileSize( hDLFhandler, &dwFileSizeHigh );
	
	if( -1 != dwFileSizeLow )
	{	///GetFileSize() ok!
		if( (0 == dwFileSizeHigh) && (dwFileSizeLow < 132) )
		{	///write header...
			SetFilePointer( hDLFhandler, 0, 0, FILE_BEGIN );
			bRet = WriteFile( hDLFhandler, DLFHeader, DLFHeaderLen, &dwWriten, NULL );
			if( bRet && (DLFHeaderLen == dwWriten) )
			{
				Set_DLF_FileHeader_Init( TRUE );
				SPDBG( TEXT("SPD@spCElog_CheckDLFFile: File header writel !!! (%d)\r\n"), dwWriten );
			}
			else
				Set_DLF_FileHeader_Init( FALSE );
		}
	}
	else
		SPERR( TEXT("SPD@spCElog_CheckDLFFile: GetFileSize fail !!! (0x%08x)\r\n"), dwFileSizeLow );
}

BOOL spCElog_Write1Buffer2DLF( PUCHAR puData, DWORD dwSize, HANDLE hDLFhandler )
{
	BOOL bRet = FALSE;
	spCEbufExtinfo*	pExtInfo = NULL;
	PUCHAR	pBuffer = NULL;
	DWORD dwDataSize = 0;
	DWORD dwWriten = 0;
	USHORT	usDLFLenSign = 0;
	
	if( (NULL != puData) && (dwSize > 0) && (NULL != hDLFhandler) )
	{
		static BYTE	bChksum[2] = { 0x00, 0x00 };	///for DLF checksum local buffer
		BYTE *pChksum = NULL;						///for DLF checksum local buffer
		
		///get what we will need in raw data packet info
		///get the start of raw data log packet info
		pExtInfo = (spCEbufExtinfo *)puData;
		///get the start of raw data log packet
		pBuffer = puData + sizeof(spCEbufExtinfo);
		dwDataSize = pExtInfo->dwDataSize;
				
		///check if we already finish the header of DLF file
		spCElog_CheckDLFFile( hDLFhandler );
		
		
		if( SPCEBUF_EXTRAINFO_DIRECTION_1 == pExtInfo->dwDirection && !Is_Ignore_Rsp( pBuffer, dwDataSize ) )
		{	/// device response
			PUCHAR pCombineBuf = NULL;
			
			SPDBG( TEXT("SPD@spCElog_Write1Buffer2DLF: response\r\n") );
			
			pCombineBuf = Raw_Rsponse_data_combine( pBuffer, &dwDataSize );   ///span0719, support response data size over 1k
			///if( 128 <= dwDataSize )			///this resp is 1024 size, there will be coming cont. data
			if( NULL == pCombineBuf )	///no address avaliable, there will be coming cont. data
			{
				goto EXIT_WRITEBUFFER2DLF_WAIT_RSP;
			}
			
			///reverse escape code modification
			Reverse_escape_coding( pCombineBuf, &dwDataSize );
			
			if( Is_EVENT_Rsp( pCombineBuf, dwDataSize ) )
			{
				spCElog_WriteEvent2DLF( pCombineBuf, dwDataSize, hDLFhandler );
			}
			else
	#ifdef USE_SEPARATE_PARSE    ///span0719, support mixed data type in single packet
			if( Is_MSG_Rsp( pCombineBuf, dwDataSize ) )
			{
				spCElog_WriteMSG2DLF( pCombineBuf, dwDataSize, hDLFhandler );
			}
			else
			if( Is_LOG_Rsp( pCombineBuf, dwDataSize ) )
			{
				spCElog_WriteLOG2DLF( pCombineBuf, dwDataSize, hDLFhandler );
			}
			else
	#else
			if( Is_MSG_Rsp( pCombineBuf, dwDataSize ) || Is_LOG_Rsp( pCombineBuf, dwDataSize ) )
			{
				spCElog_WriteMSGLOG2DLF( pCombineBuf, dwDataSize, hDLFhandler );
			}
			else
	#endif	///USE_SEPARATE_PARSE
			
			{
				SPDBG( TEXT("SPD@spCElog_Write1Buffer2DLF: Normal response\r\n") );

				usDLFLenSign = 2+2+8+(USHORT)dwDataSize-3;
				bRet = WriteFile( hDLFhandler, &usDLFLenSign, 2, &dwWriten, NULL );
				
				bRet = WriteFile( hDLFhandler, DLFDeviceRspHead, 2, &dwWriten, NULL );
				bRet = WriteFile( hDLFhandler, DLFCmdRspPAD, 8, &dwWriten, NULL );
				if( Is_ExtMsgReportCfg6_Cmd( pCombineBuf, dwDataSize ) )	///work around, since we always got a extra 0x2C in the last of payload.
					bRet = WriteFile( hDLFhandler, pCombineBuf, dwDataSize-4, &dwWriten, NULL );
				else
					bRet = WriteFile( hDLFhandler, pCombineBuf, dwDataSize-3, &dwWriten, NULL );
				
			}
EXIT_WRITEBUFFER2DLF_WAIT_RSP:
			;			
		}
		else
		if( SPCEBUF_EXTRAINFO_DIRECTION_2 == pExtInfo->dwDirection && !Is_Ignore_Cmd( pBuffer, dwDataSize ) )
		{	/// host command
			PUCHAR pCombineBuf = NULL;
			
			SPDBG( TEXT("SPD@spCElog_Write1Buffer2DLF: command\r\n") );
			
			pCombineBuf = Raw_Command_data_combine( pBuffer, &dwDataSize );
			///if( 128 <= dwDataSize )			///this cmd is 128 size, there will be coming cont. data
			if( NULL == pCombineBuf )	///no address avaliable, there will be coming cont. data
			{
				goto EXIT_WRITEBUFFER2DLF_WAIT_CMD;
			}
			
			///reverse escape code modification
			Reverse_escape_coding( pCombineBuf, &dwDataSize );			
			
				SPDBG( TEXT("SPD@spCElog_Write1Buffer2DLF: Normal request\r\n") );
	#if 0			
				if( Is_TimeStamp_Request_Cmd( pCombineBuf, dwDataSize ) )	///check if it's Timestamp request cmd
				{
					SPERR( TEXT("SPD@spCElog_Write1Buffer2DLF: TimeStamp_Request Found !!!\r\n") );
					if( bReport_Control_On == FALSE )
					{
						///back to the start!!!
						SPERR( TEXT("SPD@spCElog_Write1Buffer2DLF: Move file point to start !!!\r\n") );
						SetFilePointer( hDLFhandler, DLFHeaderLen, 0, FILE_BEGIN );
					}
				}
				else
				if( Is_Report_Control_On_Cmd( pCombineBuf, dwDataSize ) )	///check if it's Report control ON
				{
					bReport_Control_On = TRUE;
					SPERR( TEXT("SPD@spCElog_Write1Buffer2DLF: Report_Control_On Found !!!\r\n") );
				}
				else
					;
	#endif
				usDLFLenSign = 2+2+8+(USHORT)dwDataSize-3;
				bRet = WriteFile( hDLFhandler, &usDLFLenSign, 2, &dwWriten, NULL );
					
				bRet = WriteFile( hDLFhandler, DLFHostCmdHead, 2, &dwWriten, NULL );
				bRet = WriteFile( hDLFhandler, DLFCmdRspPAD, 8, &dwWriten, NULL );
				bRet = WriteFile( hDLFhandler, pCombineBuf, dwDataSize-3, &dwWriten, NULL );

			
EXIT_WRITEBUFFER2DLF_WAIT_CMD:
			;
		}
		else
			;
	}
		
	return bRet;
}


BOOL spCElog_WriteEvent2DLF( PUCHAR puRawData, DWORD dwRawSize, HANDLE hDLFhandler )
{
	static BYTE	bPrivTimeSec[8] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	UINT16 	uiEventPackSize = 0;
	UINT16 	uiEventPackSizeleft = 0;
	BYTE	bThisCmd[2] = { 0x00, 0x00 };
	BYTE	bThisPayload = 0;
	UINT16	uiThisPayload = 0;
	PUCHAR	pBuffer = puRawData;
	PUCHAR	puThisCmd = NULL;
	PUCHAR	puThisTimeSec = NULL;
	PUCHAR	puThisPayload = NULL;
	DWORD	dwThiskey = 0;
	BOOL 	bSameTime = FALSE;
	BYTE    bLengthType = 0;
	UINT16	dwThisShift = 0;
	BOOL 	bRet = 0;
	DWORD	dwWriten = 0;
	static BYTE	bChksum[2] = { 0x00, 0x00 };	///for DLF checksum local buffer
	BYTE *pChksum = NULL;						///for DLF checksum local buffer
	USHORT	usDLFLenSign = 0;
				
	SPDBG( TEXT("SPD@spCElog_WriteEvent2DLF: EVENT to DLF file\r\n") );
				
	///get packet size
	///2nd & 3rd byte is the event packet length
	///dwEventPackSize = *(UINT16*)(pBuffer+1); ///data abort???
	
	uiEventPackSize = *(pBuffer+2);	///3rd byte
	uiEventPackSize = uiEventPackSize * 256;
	uiEventPackSize = uiEventPackSize + *(pBuffer+1);	///2nd byte
				
	uiEventPackSizeleft = uiEventPackSize;
	puThisCmd = (pBuffer+3);	///get the FIRST command in this event
				
	SPDBG( TEXT("SPD@spCElog_WriteEvent2DLF: event packet size=%d Cmd pointer=(0x%08x)\r\n"), uiEventPackSize, puThisCmd );
				
	while( uiEventPackSizeleft > 5 )
	{
		bThisCmd[0] = *(puThisCmd+0);
		bThisCmd[1] = *(puThisCmd+1);

		///get command key
		dwThiskey = (( bThisCmd[1] & 0x1F ) * 256) + bThisCmd[0];	

		///get length type, bit 5 & bit 6
		bLengthType = ( bThisCmd[1] & 0x60 ) >> 5;
		
		///get same time stamp flag, bit 7
		bSameTime = (( bThisCmd[1] & 0x80 ) == 0) ? FALSE : TRUE;
		
		///reset command 
		///bThisCmd[0] = *(puThisCmd+0);
		///bThisCmd[1] = (bThisCmd[1] & 0x1F);
		bThisCmd[1] = bThisCmd[1];
		
		///get time scetion start
		puThisTimeSec = (puThisCmd+2);
					
		dwThisShift = 0;
					
		///re-construct time scetion
		if( bSameTime )
		{	///if same time, only update byte 1 & byte 2
			bPrivTimeSec[1] = *puThisTimeSec;
			bPrivTimeSec[2] = *(puThisTimeSec+1);
			///dwThisShift = 0;
		}
		else
		{	//update all,
			bPrivTimeSec[0] = *puThisTimeSec;
			bPrivTimeSec[1] = *(puThisTimeSec+1);
			bPrivTimeSec[2] = *(puThisTimeSec+2);
			bPrivTimeSec[3] = *(puThisTimeSec+3);
			///check time section, work around
			if( 
				(bPrivTimeSec[4] != 0x00) && (bPrivTimeSec[5] != 0x00) 	///not first init
				&& (bPrivTimeSec[5] != *(puThisTimeSec+5)) 				///byte that NOT changed in a log
			)
			{
				///puThisPAD = puThisPAD + 1;
				SPERR( TEXT("SPD@spCElog_WriteEvent2DLF: work around [0x%02x] [0x%02x]!!!\r\n"), bPrivTimeSec[5], *(puThisTimeSec+5));
				bPrivTimeSec[3] = *(puThisTimeSec+4);
				///bPrivTimeSec[4] = *(puThisTimeSec+5);
				///bPrivTimeSec[5] = *(puThisTimeSec+6);
				///bPrivTimeSec[6] = *(puThisTimeSec+7);
				///bPrivTimeSec[7] = *(puThisTimeSec+8);
				puThisTimeSec = puThisTimeSec + 1;	///shift for work around
				dwThisShift = dwThisShift + 1;	///shift for work around
			}
			else
			{
				bPrivTimeSec[4] = *(puThisTimeSec+4);
				bPrivTimeSec[5] = *(puThisTimeSec+5);
				bPrivTimeSec[6] = *(puThisTimeSec+6);
				bPrivTimeSec[7] = *(puThisTimeSec+7);
				///dwThisShift = 0;
			}
		}
		
		///get start of payload by move from start of time section
		puThisPayload = puThisTimeSec + (bSameTime?2:8);
					
		if( bLengthType > 2 )
			bThisPayload = *puThisPayload; ///get this event paylaod size
		else
			bThisPayload = 0;				///get this event paylaod size
		uiThisPayload = 2 + 8 + (bLengthType > 2 ? 1 : bLengthType) + bThisPayload;

		SPDBG( TEXT("SPD@spCElog_WriteEvent2DLF: this cmd [0x%02x] [0x%02x] payload=%d totalsize=%d\r\n"), bThisCmd[0], bThisCmd[1], bThisPayload, uiThisPayload );
				
		usDLFLenSign = 2+2+8+2+2+8+(uiThisPayload-10);
		bRet = WriteFile( hDLFhandler, &usDLFLenSign, 2, &dwWriten, NULL );
		
		bRet = WriteFile( hDLFhandler, DLFDeviceEventHead, 2, &dwWriten, NULL );
		bRet = WriteFile( hDLFhandler, bPrivTimeSec, 8, &dwWriten, NULL );
		bRet = WriteFile( hDLFhandler, (PUCHAR)&uiThisPayload, 2, &dwWriten, NULL );
		bRet = WriteFile( hDLFhandler, bThisCmd, 2, &dwWriten, NULL );
		bRet = WriteFile( hDLFhandler, bPrivTimeSec, 8, &dwWriten, NULL );
		bRet = WriteFile( hDLFhandler, puThisPayload, (uiThisPayload-10), &dwWriten, NULL );

					
		dwThisShift = dwThisShift + (2+(bSameTime?2:8)+(bLengthType > 2 ? 1 : bLengthType) + bThisPayload);
			
		if( dwThisShift > uiEventPackSizeleft )
		{
			SPERR( TEXT("SPD@spCElog_Write1Buffer2DLF: PackSizeleft=%d  < ThisShift=%d\r\n"), uiEventPackSizeleft, dwThisShift );
			uiEventPackSizeleft = 0;
		}
		else					
		{
			uiEventPackSizeleft = uiEventPackSizeleft - dwThisShift;
		}
					
		SPDBG( TEXT("SPD@spCElog_Write1Buffer2DLF: PackSizeleft=%d ThisShift=%d\r\n"), uiEventPackSizeleft, dwThisShift );
		puThisCmd = puThisCmd + dwThisShift;
	}	///end of while

	return bRet;
}

DWORD SeekNullByte( PCHAR pbStart )
{
	DWORD dwRet = 0;
	DWORD dwCount = 0;
	
	for( dwCount = 0; dwCount < 256; dwCount++ )
	{
		if( 00 == *(pbStart+dwCount) )
		{
			dwRet = dwCount;
			break;
		}
	}
	
	dwRet = dwRet + 1;
	SPDBG( TEXT("SPD@SeekNullByte: offset %d \r\n"), dwRet );
	return dwRet;
}

DWORD Seek2ndNullByte( PCHAR pbStart )
{
	DWORD dwRet = 0;
	DWORD dw1stNull = 0;
	
	dw1stNull = SeekNullByte( pbStart );
	dwRet = SeekNullByte( pbStart+dw1stNull );
	
	dwRet = dwRet + dw1stNull;
	
	return dwRet;
}

#ifdef USE_SEPARATE_PARSE    ///span0719, support mixed data type in single packet

BOOL spCElog_WriteMSG2DLF( PUCHAR puRawData, DWORD dwRawSize, HANDLE hDLFhandler )
{
	DWORD dwTotalSizeLeft = 0;	///total length of data that needs to handle
	DWORD dwThisMSGPacketSize = 0;
	PCHAR pbThisMSGPacket = NULL;
	PCHAR pbThisMSGPacketTimestamp = NULL;
	DWORD dwThisMSGPacketBasicSize = 0;
	DWORD dwThisMSGPacketVarSize = 0;
	DWORD dwArgsNum = 0;
	USHORT	usDLFLenSign = 0;
	DWORD dwWriten = 0;
	BOOL bRet;
	
	SPDBG( TEXT("SPD@spCElog_WriteMSG2DLF: MSG to DLF file %d \r\n"), dwRawSize );
				
	dwTotalSizeLeft = dwRawSize;
	pbThisMSGPacket = puRawData;	///head
	
	while( dwTotalSizeLeft > 0 )
	{
		if( Is_MSG_Rsp( pbThisMSGPacket, 2 ) )	///check if it's MSG packet
		{
			///prepare packet data
			pbThisMSGPacketTimestamp = pbThisMSGPacket + 4;		///offset 4 
			dwArgsNum = *(pbThisMSGPacket + 2);
			dwThisMSGPacketBasicSize = 1+1+1+1+8+2+2+4+(dwArgsNum*4);
			dwThisMSGPacketVarSize = Seek2ndNullByte( (pbThisMSGPacket+dwThisMSGPacketBasicSize) );
			dwThisMSGPacketSize = dwThisMSGPacketBasicSize + dwThisMSGPacketVarSize;
			
			///write to file
			usDLFLenSign = 2+2+8+(USHORT)dwThisMSGPacketSize;
			bRet = WriteFile( hDLFhandler, &usDLFLenSign, 2, &dwWriten, NULL );
			bRet = WriteFile( hDLFhandler, DLFExtDebugMsgHead, 2, &dwWriten, NULL );
			bRet = WriteFile( hDLFhandler, pbThisMSGPacketTimestamp, 8, &dwWriten, NULL );
			
			bRet = WriteFile( hDLFhandler, pbThisMSGPacket, dwThisMSGPacketSize, &dwWriten, NULL );

			///prepare next packet
			dwTotalSizeLeft = dwTotalSizeLeft - dwThisMSGPacketSize - 3;	///count down, include CRC & end byte
			pbThisMSGPacket = pbThisMSGPacket + dwThisMSGPacketSize + 3;	///move to next packet
		}
		else
			break;
			
		SPDBG( TEXT("SPD@spCElog_WriteMSG2DLF: ArgsNum=%d packet=%d total=%d byte left \r\n"), dwArgsNum, dwThisMSGPacketSize, dwTotalSizeLeft );
	}	
	
	SPDBG( TEXT("SPD@spCElog_WriteMSG2DLF: %d byte left \r\n"), dwTotalSizeLeft );
	
	return bRet;
}


BOOL spCElog_WriteLOG2DLF( PUCHAR puRawData, DWORD dwRawSize, HANDLE hDLFhandler )
{
	DWORD dwTotalSizeLeft = 0;	///total length of data that needs to handle
	PCHAR pbThisLOGPacket = NULL;
	PCHAR pbThisLOGPacketPayload = NULL;
	DWORD dwThisLOGPacketPayloadSize = 0;
	USHORT	usDLFLenSign = 0;
	DWORD dwWriten = 0;
	BOOL bRet;
	
	SPDBG( TEXT("SPD@spCElog_WriteLOG2DLF: LOG to DLF file %d \r\n"), dwRawSize );
				
	dwTotalSizeLeft = dwRawSize;
	pbThisLOGPacket = puRawData;	///head
	
	while( dwTotalSizeLeft > 0 )
	{
		if( Is_LOG_Rsp( pbThisLOGPacket, 2 ) )	///check if it's LOG packet
		{
			///prepare packet data
			pbThisLOGPacketPayload = pbThisLOGPacket + 4;	///offset 4
			usDLFLenSign = *(USHORT*)(pbThisLOGPacket + 2);
			dwThisLOGPacketPayloadSize = (DWORD)usDLFLenSign;
			
			///write to file
			bRet = WriteFile( hDLFhandler, pbThisLOGPacketPayload, dwThisLOGPacketPayloadSize, &dwWriten, NULL );

			///prepare next packet
			dwTotalSizeLeft = dwTotalSizeLeft - dwThisLOGPacketPayloadSize - 3 - 4;	///count down, include CRC & end byte
			pbThisLOGPacket = pbThisLOGPacket + dwThisLOGPacketPayloadSize + 3 + 4;	///move to next packet
		}
		else
			break;
			
		SPDBG( TEXT("SPD@spCElog_WriteLOG2DLF:  packet payload=%d total=%d byte left \r\n"), dwThisLOGPacketPayloadSize, dwTotalSizeLeft );
	}	
	
	SPDBG( TEXT("SPD@spCElog_WriteLOG2DLF: %d byte left \r\n"), dwTotalSizeLeft );
	
	return bRet;
}

#else    ///span0719, support mixed data type in single packet

BOOL spCElog_WriteMSGLOG2DLF( PUCHAR puRawData, DWORD dwRawSize, HANDLE hDLFhandler )
{
	DWORD dwTotalSizeLeft = 0;	///total length of data that needs to handle
	PCHAR pbThisPacket = NULL;
	
	DWORD dwThisMSGPacketSize = 0;
	PCHAR pbThisMSGPacketTimestamp = NULL;
	DWORD dwThisMSGPacketBasicSize = 0;
	DWORD dwThisMSGPacketVarSize = 0;
	DWORD dwArgsNum = 0;
	
	PCHAR pbThisLOGPacketPayload = NULL;
	DWORD dwThisLOGPacketPayloadSize = 0;
	
	USHORT	usDLFLenSign = 0;
	DWORD dwWriten = 0;
	BOOL bRet;

	SPDBG( TEXT("SPD@spCElog_WriteMSGLOG2DLF: MSGLOG to DLF file %d \r\n"), dwRawSize );	
	
	dwTotalSizeLeft = dwRawSize;
	pbThisPacket = puRawData;	///head

	while( dwTotalSizeLeft > 0 )
	{
		usDLFLenSign = 0;	///clear it
		
		if( Is_MSG_Rsp( pbThisPacket, 2 ) )	///check if it's MSG packet
		{
			///prepare packet data
			pbThisMSGPacketTimestamp = pbThisPacket + 4;		///offset 4 
			dwArgsNum = *(pbThisPacket + 2);
			dwThisMSGPacketBasicSize = 1+1+1+1+8+2+2+4+(dwArgsNum*4);
			dwThisMSGPacketVarSize = Seek2ndNullByte( (pbThisPacket+dwThisMSGPacketBasicSize) );
			dwThisMSGPacketSize = dwThisMSGPacketBasicSize + dwThisMSGPacketVarSize;
			
			///write to file
			usDLFLenSign = 2+2+8+(USHORT)dwThisMSGPacketSize;
			bRet = WriteFile( hDLFhandler, &usDLFLenSign, 2, &dwWriten, NULL );
			bRet = WriteFile( hDLFhandler, DLFExtDebugMsgHead, 2, &dwWriten, NULL );
			bRet = WriteFile( hDLFhandler, pbThisMSGPacketTimestamp, 8, &dwWriten, NULL );
			
			bRet = WriteFile( hDLFhandler, pbThisPacket, dwThisMSGPacketSize, &dwWriten, NULL );

			///prepare next packet
			if( dwTotalSizeLeft < (dwThisMSGPacketSize + 3) )
			{
				SPERR( TEXT("SPD@spCElog_WriteMSGLOG2DLF: ERROR!! over size %d - %d \r\n"), dwTotalSizeLeft, (dwThisMSGPacketSize + 3) );
				SPDBG( TEXT("SPD@spCElog_WriteMSGLOG2DLF: ArgsNum=%d packet=%d total=%d byte left \r\n"), dwArgsNum, dwThisMSGPacketSize, dwTotalSizeLeft );
				break;
			}	
			dwTotalSizeLeft = dwTotalSizeLeft - dwThisMSGPacketSize - 3;	///count down, include CRC & end byte
			pbThisPacket = pbThisPacket + dwThisMSGPacketSize + 3;	///move to next packet
			
			SPDBG( TEXT("SPD@spCElog_WriteMSGLOG2DLF: ArgsNum=%d packet=%d total=%d byte left \r\n"), dwArgsNum, dwThisMSGPacketSize, dwTotalSizeLeft );
		}
		else
		if( Is_LOG_Rsp( pbThisPacket, 2 ) )	///check if it's LOG packet
		{
			///prepare packet data
			pbThisLOGPacketPayload = pbThisPacket + 4;	///offset 4
			///usDLFLenSign = *(USHORT*)(pbThisPacket + 2);		///data abort??
			usDLFLenSign = *(pbThisPacket + 3);					///start with high byte
			usDLFLenSign = usDLFLenSign & 0xFF;
			usDLFLenSign = usDLFLenSign * 256;
			usDLFLenSign = usDLFLenSign + (0xFF & (*(pbThisPacket + 2)));	///add on low byte
			dwThisLOGPacketPayloadSize = 0;
			dwThisLOGPacketPayloadSize = (DWORD)usDLFLenSign;
			
			///work around
			if( 65477 == dwThisLOGPacketPayloadSize )
			{
				dwThisLOGPacketPayloadSize = 190;
				SPERR( TEXT("SPD@spCElog_WriteMSGLOG2DLF: work around !! change size %d - >%d \r\n"), 65477, dwThisLOGPacketPayloadSize );
			}
			
			///write to file
			bRet = WriteFile( hDLFhandler, pbThisLOGPacketPayload, dwThisLOGPacketPayloadSize, &dwWriten, NULL );

			///prepare next packet
			if( dwTotalSizeLeft < (dwThisLOGPacketPayloadSize + 3 +4) )
			{
				SPERR( TEXT("SPD@spCElog_WriteMSGLOG2DLF: ERROR!! over size %d - %d \r\n"), dwTotalSizeLeft, (dwThisLOGPacketPayloadSize + 3 +4) );
				SPDBG( TEXT("SPD@spCElog_WriteMSGLOG2DLF:  packet payload=%d total=%d byte left \r\n"), dwThisLOGPacketPayloadSize, dwTotalSizeLeft );
				break;
			}	
			dwTotalSizeLeft = dwTotalSizeLeft - dwThisLOGPacketPayloadSize - 3 - 4;	///count down, include CRC & end byte
			pbThisPacket = pbThisPacket + dwThisLOGPacketPayloadSize + 3 + 4;	///move to next packet
			
			SPDBG( TEXT("SPD@spCElog_WriteMSGLOG2DLF:  packet payload=%d total=%d byte left \r\n"), dwThisLOGPacketPayloadSize, dwTotalSizeLeft );
		}
		else
		{
			DWORD dwTmp = 0;
			
			SPERR( TEXT("SPD@spCElog_WriteMSGLOG2DLF: unknow %d bytes left are "), dwTotalSizeLeft );
			for( dwTmp = 0; dwTmp < dwTotalSizeLeft; dwTmp++ )
			{
				SPERR( TEXT("%02x "), *(pbThisPacket+dwTmp) );
			}
			SPERR( TEXT("\r\n") );
			break;
		}
				
	}	
	
	SPDBG( TEXT("SPD@spCElog_WriteMSGLOG2DLF: %d bytes left \r\n"), dwTotalSizeLeft );
	
	return bRet;	
}

#endif	///USE_SEPARATE_PARSE