// spCElog.c : Defines the entry point for the application.
//

///#include <stdafx.h>
#include <windows.h>
#include "spCElogFakeHost.h"
#include "spCEbuf.h"

///
#define SPERR						NKDbgPrintfW
///#define SPDBG						NKDbgPrintfW
#define SPDBG						///void
#define SPCELOGFAKEHOST_STATUS_LOCK			EnterCriticalSection( &FakeHost_stat.cs );
#define SPCELOGFAKEHOST_STATUS_UNLOCK		LeaveCriticalSection( &FakeHost_stat.cs );


///-///-///-///-///-///-///-///-///-///-///-///-///-///-///-///-///-///-///-///-
static spCElog_db_Cmd_Mapping DB_Cmd_Table[] = 
	{
		{  0,   3, FALSE,   4 },
		{  1,   5, FALSE,   6 },
		{  2,   7, FALSE,   8 },
		{  3,   9, 	TRUE,  11 },
		{  4,  10, FALSE,  11 },
		{  5,  12, FALSE,  13 },
		{  6,  14, FALSE,  15 },
		{  7,  16, FALSE,  17 },
		{  8,  18, FALSE,  19 },
		{  9,  20,  TRUE,  22 },
		{ 10,  21, FALSE,  22 },
		{ 11,  23, FALSE,  24 },
		{ 12,  25, FALSE,  26 },
		{ 13,  27, FALSE,  28 },
		{ 14,  29, FALSE,  30 },
		{ 15,  31, FALSE,  32 },
		{ 16,  33, FALSE,  34 },
		{ 17,  35, FALSE,  36 },
		{ 18,  37, FALSE,  38 },
		{ 19,  39, FALSE,  40 },
		{ 20,  41, FALSE,  42 },
		{ 21,  43, FALSE,  44 },
		{ 22,  45, FALSE,  46 },
		{ 23,  47, FALSE,  48 },
		{ 24,  49, FALSE,  50 },
		{ 25,  51, FALSE,  52 },
		{ 26,  53, FALSE,  54 },
		{ 27,  55, FALSE,  56 },
		{ 28,  57, FALSE,  58 },
		{ 29,  59, FALSE,  60 },
		{ 30,  61, FALSE,  62 },
		{ 31,  63, FALSE,  64 },
		{ 32,  65, FALSE,  66 },
		{ 33,  67, FALSE,  68 },
		{ 34,  69, FALSE,  70 },
		{ 35,  71, FALSE,  72 },
		{ 36,  73, FALSE,  74 },
		{ 37,  75, FALSE,  76 },
		{ 38,  77, FALSE,  78 },
		{ 39,  79, FALSE,  80 },
		{ 40,  81, FALSE,  82 },
		{ 41,  83, FALSE,  84 },
		{ 42, 126, FALSE, 127 },	///start here
		{ 43, 128, FALSE, 129 },
		{ 44, 130, FALSE, 131 },
		{ 45, 132, FALSE, 133 },
		{ 46, 134, FALSE, 135 },
		{ 47, 136, FALSE, 137 },
		{ 48, 138, FALSE, 139 },
		{ 49, 140, FALSE, 141 },
		{ 50, 142, FALSE, 143 },
		{ 51, 144, FALSE, 145 },
		{ 52, 146, FALSE, 147 },
		{ 53, 148, FALSE, 159 },
		{ 54, 150, FALSE, 151 },
		{ 55, 152,  TRUE, 154 },
		{ 56, 153, FALSE, 154 },
		{ 57, 155,  TRUE, 158 },
		{ 58, 156,  TRUE, 158 },
		{ 59, 157, FALSE, 158 },
		{ 60, 159,  TRUE, 161 },
		{ 61, 160, FALSE, 161 },
		{ 62, 162,  TRUE, 164 },
		{ 63, 163, FALSE, 164 },
		{ 64, 165, FALSE, 166 },
		{ 65, 167,  TRUE, 169 },
		{ 66, 168, FALSE, 169 },
		{ 67, 170, FALSE, 171 },
		{ 68, 172, FALSE, 173 },
		{ 69, 174, FALSE, 175 },
		{ 70, 176, FALSE, 177 },
		{ 71, 178, FALSE, 179 },
		{ 72, 180,  TRUE, 182 },
		{ 73, 181, FALSE, 182 },
		{ 74, 183, FALSE, 184 },
		{ 75, 185, FALSE, 186 },
		{ 76, 187, FALSE, 188 },
		{ 77, 189, FALSE, 190 },
		{ 78, 191, FALSE, 192 },
		{ 79, 193, FALSE, 194 },
		{ 80, 195,  TRUE, 198 },
		{ 81, 196,  TRUE, 198 },
		{ 82, 197, FALSE, 198 },
		{ 83, 199, FALSE, 200 },
		{ 84, 201,  TRUE, 207 },
		{ 85, 202,  TRUE, 207 },
		{ 86, 203,  TRUE, 207 },
		{ 87, 204,  TRUE, 207 },
		{ 88, 205, FALSE, 207 },
		{ 89, 208,  TRUE, 213 },
		{ 90, 209,  TRUE, 213 },
		{ 91, 210,  TRUE, 213 },
		{ 92, 211,  TRUE, 213 },
		{ 93, 212, FALSE, 213 },
		{ 94, 214,  TRUE, 220 },
		{ 95, 215,  TRUE, 220 },
		{ 96, 216,  TRUE, 220 },
		{ 97, 217,  TRUE, 220 },
		{ 98, 218, FALSE, 220 },
		{ 99, 221,  TRUE, 226 },
		{100, 222,  TRUE, 226 },
		{101, 223,  TRUE, 226 },
		{102, 224,  TRUE, 226 },
		{103, 225, FALSE, 226 },
		{104, 227, FALSE, 228 },
		{105, 229, FALSE, 230 },
		{106, 231, FALSE, 232 },
		{ -1,   0, FALSE,   0 }
	};
///#define	HOST_CMD_TABLE_SIZE		106	///move to header file

///-///-///-///-///-///-///-///-///-///-///-///-///-///-///-///-///-///-///-///-
static spCElogFakeHost_status		FakeHost_stat;



///-///-///-///-///-///-///-///-///-///-///-///-///-///-///-///-///-///-///-///-



///-///-///-///-///-///-///-///-///-///-///-///-///-///-///-///-///-///-///-///-

BOOL spCElogFakeHost_Init( spCElogFakeHost_status *pFakeHostStat )
{
	BOOL bRet = TRUE;
	
	if( NULL != pFakeHostStat )
	{
		///pFakeHostStat->bInited = TRUE;
		InitializeCriticalSection( &pFakeHostStat->cs );
		///pFakeHostStat->cs = NULL;
		pFakeHostStat->dwNextCmdStatus = 0;
		pFakeHostStat->hDBfileHandle = NULL;
		///pFakeHostStat->bNextCmd;
		pFakeHostStat->dwNextCmdBufSize = 0;
		pFakeHostStat->bNextCmdCont = FALSE;
		pFakeHostStat->bNextCmdRead = TRUE;		///for the first time 
		pFakeHostStat->bNextCmdGotRsp = FALSE;
	}	
	
	return bRet;
}

DWORD spCElogFakeHost_Get_StatusNum( spCElogFakeHost_status *pFakeHostStat )
{
	DWORD dwRet;
	
	if( NULL != pFakeHostStat )
	{
		SPCELOGFAKEHOST_STATUS_LOCK
		dwRet = pFakeHostStat->dwNextCmdStatus;
		SPCELOGFAKEHOST_STATUS_UNLOCK
	}	
	
	return dwRet;
}

DWORD spCElogFakeHost_Set_StatusNum( spCElogFakeHost_status *pFakeHostStat, DWORD dwSet )
{
	DWORD dwRet;
	
	if( NULL != pFakeHostStat )
	{
		SPCELOGFAKEHOST_STATUS_LOCK
		pFakeHostStat->dwNextCmdStatus = dwSet;
		SPCELOGFAKEHOST_STATUS_UNLOCK
		
		dwRet = spCElogFakeHost_Get_StatusNum( pFakeHostStat );
	}	
	
	return dwRet;
}

DWORD spCElogFakeHost_Set_StatusNum_Stepup( spCElogFakeHost_status *pFakeHostStat )
{
	DWORD dwRet;
	
	if( NULL != pFakeHostStat )
	{
		dwRet = spCElogFakeHost_Get_StatusNum( pFakeHostStat );
		dwRet++;
		dwRet = spCElogFakeHost_Set_StatusNum( pFakeHostStat, dwRet );
	}
	
	return dwRet;
}


HANDLE spCElogFakeHost_Get_DBfileHandle( spCElogFakeHost_status *pFakeHostStat )
{
	HANDLE hRet = NULL;
	
	if( NULL != pFakeHostStat )
	{
		SPCELOGFAKEHOST_STATUS_LOCK
		hRet = pFakeHostStat->hDBfileHandle;
		SPCELOGFAKEHOST_STATUS_UNLOCK
		
		if( NULL == hRet )
		{
			hRet= CreateFile( SPCELOG_DBFILE_NAME, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
			if( INVALID_HANDLE_VALUE == hRet )
			{
				SPERR( TEXT("SPD@spCElogFakeHost_Get_DBfileHandle: CreateFile fail !!! \r\n") );		
			}
			else
			{
				SPCELOGFAKEHOST_STATUS_LOCK
				pFakeHostStat->hDBfileHandle = hRet;
				SPCELOGFAKEHOST_STATUS_UNLOCK
			}
		}
	}
	
	return hRet;
}


PUCHAR spCElogFakeHost_Get_NextCmdBuffer( spCElogFakeHost_status *pFakeHostStat )
{
	PUCHAR pucRet = NULL;
	
	if( NULL != pFakeHostStat )
	{
		SPCELOGFAKEHOST_STATUS_LOCK
		pucRet = pFakeHostStat->bNextCmd;
		SPCELOGFAKEHOST_STATUS_UNLOCK
	}
	
	return pucRet;
}

DWORD spCElogFakeHost_Get_NextCmdBufSize( spCElogFakeHost_status *pFakeHostStat )
{
	DWORD dwRet;
	
	if( NULL != pFakeHostStat )
	{
		SPCELOGFAKEHOST_STATUS_LOCK
		dwRet = pFakeHostStat->dwNextCmdBufSize;
		SPCELOGFAKEHOST_STATUS_UNLOCK
	}	
	
	return dwRet;
}

DWORD spCElogFakeHost_Set_NextCmdBufSize( spCElogFakeHost_status *pFakeHostStat, DWORD dwSet )
{
	DWORD dwRet;
	
	if( NULL != pFakeHostStat )
	{
		SPCELOGFAKEHOST_STATUS_LOCK
		pFakeHostStat->dwNextCmdBufSize = dwSet;
		SPCELOGFAKEHOST_STATUS_UNLOCK
		
		dwRet = spCElogFakeHost_Get_NextCmdBufSize( pFakeHostStat );
	}	
	
	return dwRet;
}

BOOL spCElogFakeHost_Get_NextCmdCont( spCElogFakeHost_status *pFakeHostStat )
{
	BOOL bRet;
	
	if( NULL != pFakeHostStat )
	{
		SPCELOGFAKEHOST_STATUS_LOCK
		bRet = pFakeHostStat->bNextCmdCont;
		SPCELOGFAKEHOST_STATUS_UNLOCK
	}	
	
	return bRet;
}

DWORD spCElogFakeHost_Set_NextCmdCont( spCElogFakeHost_status *pFakeHostStat, BOOL bSet )
{
	BOOL bRet;
	
	if( NULL != pFakeHostStat )
	{
		SPCELOGFAKEHOST_STATUS_LOCK
		pFakeHostStat->bNextCmdCont = bSet;
		SPCELOGFAKEHOST_STATUS_UNLOCK
		
		bRet = spCElogFakeHost_Get_NextCmdCont( pFakeHostStat );
	}	
	
	return bRet;
}

BOOL spCElogFakeHost_Get_NextCmdRead( spCElogFakeHost_status *pFakeHostStat )
{
	BOOL bRet;
	
	if( NULL != pFakeHostStat )
	{
		SPCELOGFAKEHOST_STATUS_LOCK
		bRet = pFakeHostStat->bNextCmdRead;
		SPCELOGFAKEHOST_STATUS_UNLOCK
	}	
	
	return bRet;
}

DWORD spCElogFakeHost_Set_NextCmdRead( spCElogFakeHost_status *pFakeHostStat, BOOL bSet )
{
	BOOL bRet;
	
	if( NULL != pFakeHostStat )
	{
		SPCELOGFAKEHOST_STATUS_LOCK
		pFakeHostStat->bNextCmdRead = bSet;
		SPCELOGFAKEHOST_STATUS_UNLOCK
		
		bRet = spCElogFakeHost_Get_NextCmdRead( pFakeHostStat );
	}	
	
	return bRet;
}

BOOL spCElogFakeHost_Get_NextCmdGotRsp( spCElogFakeHost_status *pFakeHostStat )
{
	BOOL bRet;
	
	if( NULL != pFakeHostStat )
	{
		SPCELOGFAKEHOST_STATUS_LOCK
		bRet = pFakeHostStat->bNextCmdGotRsp;
		SPCELOGFAKEHOST_STATUS_UNLOCK
	}	
	
	return bRet;
}

DWORD spCElogFakeHost_Set_NextCmdGotRsp( spCElogFakeHost_status *pFakeHostStat, BOOL bSet )
{
	BOOL bRet;
	
	if( NULL != pFakeHostStat )
	{
		SPCELOGFAKEHOST_STATUS_LOCK
		pFakeHostStat->bNextCmdGotRsp = bSet;
		SPCELOGFAKEHOST_STATUS_UNLOCK
		
		bRet = spCElogFakeHost_Get_NextCmdGotRsp( pFakeHostStat );
	}	
	
	return bRet;
}


BOOL spCElogFakeHost_SetupNextCmd( spCElogFakeHost_status *pFakeHostStat )
{
	BOOL bRet = TRUE;
	DWORD dwCmdStatNum = 0;
	PUCHAR pucCmd = NULL;
	spCElog_db_Cmd_Mapping ThisTableCmd;
	BYTE	bTmp[512];
	
	if( NULL != pFakeHostStat )
	{
		if( !spCElogFakeHost_Get_NextCmdRead( pFakeHostStat ) )
		{	///current cmd still not been read, need no update!!
			dwCmdStatNum = spCElogFakeHost_Get_StatusNum( pFakeHostStat );
			dwCmdStatNum--;
			if( HOST_CMD_TABLE_SIZE < dwCmdStatNum )
			{	///
				ThisTableCmd.dwDbCmdIndex = 5;
				ThisTableCmd.bContCmd = FALSE;
				ThisTableCmd.dwDbRspIndex = 6;
			}
			else
				ThisTableCmd = DB_Cmd_Table[dwCmdStatNum];
				
			SPERR( TEXT("SPD@spCElogFakeHost_SetupNextCmd: NO UPDATE!! StatNum=%d CmdIndex= %d  RspIndex=%d Cont=%d\r\n"), dwCmdStatNum, ThisTableCmd.dwDbCmdIndex, ThisTableCmd.dwDbRspIndex, ThisTableCmd.bContCmd );
			return FALSE;
		}
		
		dwCmdStatNum = spCElogFakeHost_Get_StatusNum( pFakeHostStat );
		pucCmd = spCElogFakeHost_Get_NextCmdBuffer( pFakeHostStat );

		///look up table
		if( HOST_CMD_TABLE_SIZE < dwCmdStatNum )
		{	///
			ThisTableCmd.dwDbCmdIndex = 5;
			ThisTableCmd.bContCmd = FALSE;
			ThisTableCmd.dwDbRspIndex = 6;
		}
		else
			ThisTableCmd = DB_Cmd_Table[dwCmdStatNum];
		
		SPDBG( TEXT("SPD@spCElogFakeHost_SetupNextCmd: StatNum=%d CmdIndex= %d  RspIndex=%d Cont=%d\r\n"), dwCmdStatNum, ThisTableCmd.dwDbCmdIndex, ThisTableCmd.dwDbRspIndex, ThisTableCmd.bContCmd );
		
		///find in the DB file
		///copy it to nextcommand buffer
		///set nextcommand buffer size
		if( NULL != spCElogFakeHost_Get_DBfileHandle( pFakeHostStat ) )
		{	
			BOOL 	bRet;
			spCEbufExtinfo HeadTag;
			DWORD 	dwRead = 0;
			DWORD 	dwPayload = 0;
			
			SetFilePointer( spCElogFakeHost_Get_DBfileHandle( pFakeHostStat ), 0, 0, FILE_BEGIN );
			
			while( 1 )
			{
				bRet = ReadFile( spCElogFakeHost_Get_DBfileHandle( pFakeHostStat ), &HeadTag, sizeof(spCEbufExtinfo), &dwRead, NULL );
				if( bRet && (dwRead == 0) )
				{	/// you are at the end of the file.
					SPERR( TEXT("SPD@spCElogFakeHost_SetupNextCmd: end of DB file!!!!! \r\n") );
					break;
				}
		
				if( (SPCEBUF_EXTRAINFO_TAG1 != HeadTag.dwTag1) || (SPCEBUF_EXTRAINFO_TAG2 != HeadTag.dwTag2) )
				{	///the tag is not right
					SPERR( TEXT("SPDspCElogFakeHost_SetupNextCmd: Unkown DB file format ! \r\n") );
					break;
				}
				
				if( HeadTag.Order == ThisTableCmd.dwDbCmdIndex )	///find it in DB
				{
					bRet = ReadFile( spCElogFakeHost_Get_DBfileHandle( pFakeHostStat ), pucCmd, HeadTag.dwDataSize, &dwRead, NULL );
					spCElogFakeHost_Set_NextCmdBufSize( pFakeHostStat, HeadTag.dwDataSize );
					///set nextcommand wait time
					spCElogFakeHost_Set_NextCmdCont( pFakeHostStat, ThisTableCmd.bContCmd );
					///index it's a new command
					spCElogFakeHost_Set_NextCmdRead( pFakeHostStat, FALSE );
					spCElogFakeHost_Set_NextCmdGotRsp( pFakeHostStat, FALSE );
					break;
				}
				else
					bRet = ReadFile( spCElogFakeHost_Get_DBfileHandle( pFakeHostStat ), bTmp, HeadTag.dwDataSize, &dwRead, NULL );
			}	///while
		}
		else
		{
			SPERR( TEXT("SPD@spCElogFakeHost_SetupNextCmd: No DB file found!\r\n") );
		}
				
		spCElogFakeHost_Set_StatusNum_Stepup( pFakeHostStat );	///move to next
		SPDBG( TEXT("SPD@spCElogFakeHost_SetupNextCmd: = 0x%08x size=%d\r\n"), spCElogFakeHost_Get_NextCmdBuffer( pFakeHostStat ), spCElogFakeHost_Get_NextCmdBufSize( pFakeHostStat ) );
	}
	
	return bRet;
}


///-///-///-///-///-///-///-///-///-///-///-///-///-///-///-///-///-///-///-///-

BOOL __spCElogFakeHost_Init()
{
	return spCElogFakeHost_Init( &FakeHost_stat );
}

DWORD __spCElogFakeHost_Get_StatusNum()
{
	return spCElogFakeHost_Get_StatusNum( &FakeHost_stat );
}

///span0714, add for reset
void __spCElogFakeHost_Reset_StatusNum()
{
	spCElogFakeHost_Set_StatusNum( &FakeHost_stat, 0 );
}

BOOL __spCElogFakeHost_SetupNextCmd()
{
	return spCElogFakeHost_SetupNextCmd( &FakeHost_stat );
}

PUCHAR __spCElogFakeHost_Get_NextCmdBuffer()
{
	return spCElogFakeHost_Get_NextCmdBuffer( &FakeHost_stat );
}

DWORD __spCElogFakeHost_Get_NextCmdBufSize()
{
	return spCElogFakeHost_Get_NextCmdBufSize( &FakeHost_stat );
}

BOOL __spCElogFakeHost_Get_NextCmdCont()
{
	return spCElogFakeHost_Get_NextCmdCont( &FakeHost_stat );
}

BOOL __spCElogFakeHost_Get_NextCmdRead()
{
	return spCElogFakeHost_Get_NextCmdRead( &FakeHost_stat );
}

BOOL __spCElogFakeHost_Set_NextCmdRead()
{
	return spCElogFakeHost_Set_NextCmdRead( &FakeHost_stat, TRUE );
}

BOOL __spCElogFakeHost_Get_NextCmdGotRsp()
{
	return spCElogFakeHost_Get_NextCmdGotRsp( &FakeHost_stat );
}

BOOL __spCElogFakeHost_Set_NextCmdGotRsp()
{
	return spCElogFakeHost_Set_NextCmdGotRsp( &FakeHost_stat, TRUE );
}