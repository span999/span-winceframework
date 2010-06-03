// spCElogDMC.c : Defines the entry point for the application.
//


#include <windows.h>
#include "spCElogDMC.h"
#include "spCElogCmdRsp.h"
#include "spCElogCRC.h"

///
#define SPERR						NKDbgPrintfW
#define SPDBG						NKDbgPrintfW
///#define SPDBG						///void
///#define FIXED_DLF_CRC_TABLE

#define EVENTIDS_NUMBER			2112	///0 ~ 2111
#define LOGCODE_NUMBER_01		1104	///0 ~ 1103
#define LOGCODE_NUMBER_04		2328	///0 ~ 2327
#define LOGCODE_NUMBER_05		1064	///0 ~ 1063
#define LOGCODE_NUMBER_07		1288	///0 ~ 1287
#define MESSAGE_NUMBER_000		54		///0 ~ 53
#define MESSAGE_NUMBER_005		7		///0 ~ 6
#define MESSAGE_NUMBER_030		13		///0 ~ 12
#define MESSAGE_NUMBER_040		13		///0 ~ 12
#define MESSAGE_NUMBER_050		19		///0 ~ 18
#define MESSAGE_NUMBER_055		15		///0 ~ 14
#define MESSAGE_NUMBER_060		51		///0 ~ 50
#define MESSAGE_NUMBER_065		22		///0 ~ 21
#define MESSAGE_NUMBER_070		4		///0 ~ 3
#define MESSAGE_NUMBER_071		12		///0 ~ 11
#define MESSAGE_NUMBER_072		2		///0 ~ 1
#define MESSAGE_NUMBER_090		9		///0 ~ 8


BYTE bMSGcfgArray_00_1[] = {
								0x7D, 0x5D, 0x04, 0x00, 0x00, 0x35, 0x00, 0x00, 0x00, 0x1F, 0x00, 0x00, 0x00, 0x1E, 0x00, 0x00, 
								0x00, 0x1E, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1E, 0x00, 0x00, 
								0x00, 0x1E, 0x00, 0x00, 0x00, 0x1C, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 
								0x00, 0x18, 0x00, 0x00, 0x00, 0x1E, 0x00, 0x00, 0x00, 0x1C, 0x00, 0x00, 0x00, 0x1C, 0x00, 0x00, 
								0x00, 0x1C, 0x00, 0x00, 0x00, 0x1F, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
								0x00, 0x1E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1E, 0x00, 0x00, 0x00, 0x1F, 0x00, 0x00, 
								0x00, 0x1E, 0x00, 0x00, 0x00, 0x1F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1E, 0x00, 0x00, 
								0x00, 0x1E, 0x00, 0x00, 0x00, 0x1E, 0x00, 0x00, 0x00, 0x1E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
							};

DWORD dwMSGcfgArray_00_1 = 128;			

BYTE bMSGcfgArray_00_2[] = {
								0x00, 0x9E, 0xFF, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1E, 0x00, 0x00, 0x00, 0x1E, 0x00, 0x00, 
								0x00, 0x1C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
								0x00, 0x00, 0x00, 0x00, 0x00, 0x1E, 0x00, 0x00, 0x00, 0xFE, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,
								0x00, 0x1F, 0x00, 0x00, 0x00, 0x1C, 0x00, 0x00, 0x00, 0x1C, 0x00, 0x00, 0x00, 0x1C, 0x00, 0x00, 
								0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
								0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0x00, 0x00, 0x00, 0x1E, 0x00, 0x00, 0x00, 0x1F, 0x00, 0x00, 
								0x00, 0x00, 0xD2, 0x7E 
							};

DWORD dwMSGcfgArray_00_1 = 100;

BYTE bMSGcfgArray_05[] = {
								0x7D, 0x5D, 0x04, 0xF4, 0x01, 0xFA, 0x01, 0x00, 0x00, 0x1E, 0x00, 0x00, 0x00, 0x1C, 0x00, 0x00,
								0x00, 0x1C, 0x00, 0x00, 0x00, 0x1F, 0x00, 0x00, 0x00, 0x1C, 0x00, 0x00, 0x00, 0x1C, 0x00, 0x00, 
								0x00, 0x1C, 0x00, 0x00, 0x00, 0x02, 0xE6, 0x7E
							};

DWORD dwMSGcfgArray_05 = 40;

BYTE bMSGcfgArray_30[] = {
								0x7D, 0x5D, 0x04, 0xB8, 0x0B, 0xC4, 0x0B, 0x00, 0x00, 0x1C, 0x00, 0x00, 0x00, 0x1C, 0x00, 0x00, 
								0x00, 0x1C, 0x00, 0x00, 0x00, 0x1C, 0x00, 0x00, 0x00, 0x1C, 0x00, 0x00, 0x00, 0x1C, 0x00, 0x00,
								0x00, 0x1C, 0x00, 0x00, 0x00, 0x1C, 0x00, 0x00, 0x00, 0x1C, 0x00, 0x00, 0x00, 0x1C, 0x00, 0x00,
								0x00, 0x1C, 0x00, 0x00, 0x00, 0x1C, 0x00, 0x00, 0x00, 0x1C, 0x00, 0x00, 0x00, 0xA5, 0x7F, 0x7E 
							};

DWORD dwMSGcfgArray_30 = 64;

BYTE bMSGcfgArray_40[] = {
								7D 5D 04 A0 0F AA 0F 00 00 1C 00 00 00 1E 00 00 
								00 1C 00 00 00 1E 00 00 00 1E 00 00 00 1E 00 00 
								00 1E 00 00 00 1E 00 00 00 1E 00 00 00 1E 00 00 
								00 1E 00 00 00 88 A8 7E
							};

DWORD dwMSGcfgArray_40 = 56;

BYTE bMSGcfgArray_50[] = {
								7D 5D 04 88 13 9A 13 00 00 1E 00 00 00 1C 00 00 
								00 1C 00 00 00 1C 00 00 00 1C 00 00 00 1C 00 00 
								00 1C 00 00 00 1C 00 00 00 1C 00 00 00 1C 00 00 
								00 1C 00 00 00 1C 00 00 00 1C 00 00 00 1C 00 00 
								00 1E 00 00 00 1E 00 00 00 1F 00 00 00 1F 00 00 
								00 1F 00 00 00 54 B8 7E
							};

DWORD dwMSGcfgArray_50 = 88;

BYTE bMSGcfgArray_55[] = {
								7D 5D 04 7C 15 8A 15 00 00 3E 00 00 00 3E 00 00 
								00 3E 00 00 00 3E 00 00 00 3E 00 00 00 3E 00 00 
								00 3E 00 00 00 3E 00 00 00 3E 00 00 00 3E 00 00 
								00 3E 00 00 00 3E 00 00 00 3E 00 00 00 3E 00 00 
								00 3E 00 00 00 57 31 7E 
							};

DWORD dwMSGcfgArray_55 = 72;


BYTE bMSGcfgArray_60_1[] = {
								7D 5D 04 70 17 A2 17 00 00 00 00 00 00 00 00 00 
								00 1F 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
								00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
								00 00 00 00 00 1F 00 00 00 00 00 00 00 00 00 00 
								00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
								00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
								00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
								00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
							};

DWORD dwMSGcfgArray_60_1 = 128;

BYTE bMSGcfgArray_60_2[] = {
								00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
								00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
								00 7F 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
								00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
								00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
								00 00 00 00 00 F3 FF 7E 
							};

DWORD dwMSGcfgArray_60_2 = 88;

BYTE bMSGcfgArray_65[] = {
								7D 5D 04 64 19 79 19 00 00 00 00 00 00 00 00 00 
								00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
								00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
								00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
								00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
								00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
								00 D2 D0 7E 
							};

DWORD dwMSGcfgArray_65 = 100;

BYTE bMSGcfgArray_70[] = {
								7D 5D 04 58 1B 5B 1B 00 00 00 00 00 00 00 00 00 
								00 00 00 00 00 00 00 00 00 A2 AC 7E 
							};

DWORD dwMSGcfgArray_70 = 28;

BYTE bMSGcfgArray_71[] = {
								7D 5D 04 BC 1B C7 1B 00 00 00 00 00 00 00 00 00 
								00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
								00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
								00 00 00 00 00 00 00 00 00 2D C8 7E 
							};

DWORD dwMSGcfgArray_71 = 60;

BYTE bMSGcfgArray_72[] = {
								7D 5D 04 20 1C 21 1C 00 00 00 00 00 00 00 00 00 
								00 C1 F4 7E 
							};

DWORD dwMSGcfgArray_72 = 20;

BYTE bMSGcfgArray_90[] = {
								7D 5D 04 28 23 30 23 00 00 00 00 00 00 00 00 00 
								00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
								00 00 00 00 00 00 00 00 00 00 00 00 00 8B F4 7E 
							};

DWORD dwMSGcfgArray_90 = 48;



BYTE QXDMKey[] = { '<', 'Q', 'X', 'D', 'M', 'P', 'r', 'o', 'f', 'e', 's', 's', 'i', 'o', 'n', 'a', 'l', '>' };
DWORD dwQXDMKeyCnt = 18;
BYTE LogKey[] = { '<', 'L', 'o', 'g', 'g', 'i', 'n', 'g', 'V', 'i', 'e', 'w', '>' };
DWORD dwLogKeyCnt = 13;
BYTE EventIDKey[] = { '<', 'E', 'v', 'e', 'n', 't', 'I', 'D', 's', '>' };
DWORD dwEventKeyCnt = 10;
BYTE LogCodesKey[] = { '<', 'L', 'o', 'g', 'C', 'o', 'd', 'e', 's', '>' };
DWORD dwLogCodesKeyCnt = 10;
BYTE MessageLevelsKey[] = { '<', 'M', 'e', 's', 's', 'a', 'g', 'e', 'L', 'e', 'v', 'e', 'l', 's', '>' };
DWORD dwMessageLevelsKeyCnt = 15;


BYTE DMCarrary[512];
BYTE *pArrary = DMCarrary;
DWORD dwArrarySize = 0;

//proto type



///-///-///-///-///-///-///-///-///-///-///-///-///-///-///-///-///-///-///-///-

BOOL DMCFile_Found()
{
	BOOL bRet = FALSE;
	HANDLE hFile = NULL;
	
	hFile = CreateFile( SPCELOG_DMCFILE_NAME, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
	if( hFile == INVALID_HANDLE_VALUE )
		SPDBG( TEXT("SPD@DMCFile_Found: not found \r\n") );
	else
	{
		SPERR( TEXT("SPD@DMCFile_Found: found !!!\r\n") );
		CloseHandle( hFile );
		bRet = TRUE;
	}
	
	return bRet;
}

BOOL Open_DMCFile( HANDLE *phFile )
{
	BOOL bRet = FALSE;
	
	*phFile = CreateFile( SPCELOG_DMCFILE_NAME, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
	if( *phFile == INVALID_HANDLE_VALUE )
		SPDBG( TEXT("SPD@Open_DMCFile: file open fail \r\n") );
	else
	{
		SPERR( TEXT("SPD@Open_DMCFile: file open success !!!\r\n") );
		SetFilePointer( *phFile, 0, 0, FILE_BEGIN );
		bRet = TRUE;
	}	
	
	return bRet;
}

void Close_DMCFile( HANDLE *phFile )
{
	CloseHandle( *phFile );
}

BOOL Find_DMCFile_Key( HANDLE *phFile, BYTE *pbKey, DWORD dwKeySize )
{
	BOOL bRet = FALSE;
	BOOL bKeyNotFound = TRUE;
	BYTE bRead = 0;
	DWORD dwRead = 0;
	DWORD dwMatchCount = 0;
	
	while( bKeyNotFound )
	{
		bRet = ReadFile( *phFile, &bRead, 1, &dwRead, NULL );	///read one byte for each time

		if( bRet && (dwRead == 0) )
		{	/// you are at the end of the file.
			SPERR( TEXT("SPD@Find_DMCFile_Key: end of DMC file!!!!! \r\n") );
			break;
		}
		else
		{
			if( bRead == *(pbKey+dwMatchCount) )	///if match, then...
			{
				SPDBG( TEXT("SPD@Find_DMCFile_Key: %d match '%c' \r\n"), dwMatchCount, bRead );
				dwMatchCount++;
				if( dwMatchCount > dwKeySize )	///match all...
				{
					bKeyNotFound = FALSE;
					SPDBG( TEXT("SPD@Find_DMCFile_Key: all match '%s' \r\n"), pbKey );
				}
			}
			else
				dwMatchCount = 0;	///reset
		}	
	}	///while
	
	if( bKeyNotFound )
	{
		bRet = FALSE;
		SPERR( TEXT("SPD@Find_DMCFile_Key: not match key '%s' \r\n"), pbKey );
	}
	else
	{
		bRet = TRUE;
		SPDBG( TEXT("SPD@Find_DMCFile_Key: found match key '%s' \r\n"), pbKey );
	}
	
	return bRet;
}

BOOL Find_DMCFile_QXDM_key( HANDLE *phFile )
{
	BOOL bRet;
	
	bRet = Find_DMCFile_Key( phFile, QXDMKey, dwQXDMKeyCnt );
	SPDBG( TEXT("SPD@Find_DMCFile_QXDM_key: %s match key '%s' \r\n"), bRet?TEXT("Found"):TEXT("Not found"), QXDMKey );
	
	return bRet;
}

BOOL Find_DMCFile_LogView_key( HANDLE *phFile )
{
	BOOL bRet;
	
	bRet = Find_DMCFile_Key( phFile, LogKey, dwLogKeyCnt );
	SPDBG( TEXT("SPD@Find_DMCFile_Log_key: %s match key '%s' \r\n"), bRet?TEXT("Found"):TEXT("Not found"), LogKey );
	
	return bRet; 
}

BOOL Find_DMCFile_EventID_key( HANDLE *phFile )
{
	BOOL bRet;
	
	bRet = Find_DMCFile_Key( phFile, EventIDKey, dwEventKeyCnt );
	SPDBG( TEXT("SPD@Find_DMCFile_EventID_key: %s match key '%s' \r\n"), bRet?TEXT("Found"):TEXT("Not found"), EventIDKey );

	return bRet;
}

BOOL Find_DMCFile_LogCodes_key( HANDLE *phFile )
{
	BOOL bRet;
	
	bRet = Find_DMCFile_Key( phFile, LogCodesKey, dwLogCodesKeyCnt );
	SPDBG( TEXT("SPD@Find_DMCFile_LogCodes_key: %s match key '%s' \r\n"), bRet?TEXT("Found"):TEXT("Not found"), LogCodesKey );

	return bRet;
}

BOOL Find_DMCFile_MessageLevels_key( HANDLE *phFile )
{
	BOOL bRet;
	
	bRet = Find_DMCFile_Key( phFile, MessageLevelsKey, dwMessageLevelsKeyCnt );
	SPDBG( TEXT("SPD@Find_DMCFile_MessageLevels_key: %s match key '%s' \r\n"), bRet?TEXT("Found"):TEXT("Not found"), MessageLevelsKey );

	return bRet;
}


DWORD Get_DMCFile_EventID_Number( HANDLE *phFile )
{
	DWORD dwRet = 0;
	BOOL bRet;
	BYTE bRead = 0;
	DWORD dwRead = 0;
	
	bRet = ReadFile( *phFile, &bRead, 1, &dwRead, NULL );
	SPDBG( TEXT("SPD@Get_DMCFile_EventID_Number: %c \r\n"), bRead );
	dwRet = dwRet + (bRead-48)*10000;
	bRet = ReadFile( *phFile, &bRead, 1, &dwRead, NULL );
	SPDBG( TEXT("SPD@Get_DMCFile_EventID_Number: %c \r\n"), bRead );
	dwRet = dwRet + (bRead-48)*1000;
	bRet = ReadFile( *phFile, &bRead, 1, &dwRead, NULL );
	SPDBG( TEXT("SPD@Get_DMCFile_EventID_Number: %c \r\n"), bRead );
	dwRet = dwRet + (bRead-48)*100;
	bRet = ReadFile( *phFile, &bRead, 1, &dwRead, NULL );
	SPDBG( TEXT("SPD@Get_DMCFile_EventID_Number: %c \r\n"), bRead );
	dwRet = dwRet + (bRead-48)*10;
	bRet = ReadFile( *phFile, &bRead, 1, &dwRead, NULL );
	SPDBG( TEXT("SPD@Get_DMCFile_EventID_Number: %c \r\n"), bRead );
	dwRet = dwRet + (bRead-48)*1;
	
	bRet = ReadFile( *phFile, &bRead, 1, &dwRead, NULL );	///should be ','
	SPDBG( TEXT("SPD@Get_DMCFile_EventID_Number: %c \r\n"), bRead );


	SPDBG( TEXT("SPD@Get_DMCFile_EventID_Number: %d \r\n"), dwRet );
	return dwRet;
}


DWORD Get_DMCFile_EventIDs( BYTE **ppByteAddr )
{
	DWORD dwRet = 0;
	HANDLE hDMCfile = NULL;
	DWORD dwIDnumber = -1;
	DWORD dwCurIDnumber = 0;
	
	if( DMCFile_Found() )
	{
		do
		{
			///openfile
			if( !Open_DMCFile( &hDMCfile ) )
				break;
			
			///find QXDM key 
			if( !Find_DMCFile_QXDM_key( &hDMCfile ) )
				break;
				
			///find log key 
			if( !Find_DMCFile_LogView_key( &hDMCfile ) )
				break;
		
			///find EventIDs key
			if( !Find_DMCFile_EventID_key( &hDMCfile ) )
				break;
			
			///copy EventIDs
			dwRet = 0;
			*((*ppByteAddr)+0) = 0x82;
			dwRet++;
			*((*ppByteAddr)+1) = 0x00;
			dwRet++;
			*((*ppByteAddr)+2) = 0x00;
			dwRet++;
			*((*ppByteAddr)+3) = 0x00;
			dwRet++;
			*((*ppByteAddr)+4) = 0x40;
			dwRet++;
			*((*ppByteAddr)+5) = 0x08;
			dwRet++;
			
			while( dwCurIDnumber < EVENTIDS_NUMBER )
			{
				if( dwIDnumber < dwCurIDnumber || (-1) == dwIDnumber )
					dwIDnumber = Get_DMCFile_EventID_Number( &hDMCfile );
					
				///fill arrary... 
				
				
				dwCurIDnumber++;	///next one
			}
			
		}
		while(0);
		
		///close file
		Close_DMCFile( &hDMCfile );
		
	}	
	
	return dwRet;
}

DWORD Get_DMCFile_LogCodes_Number( HANDLE *phFile )
{
	DWORD dwRet = 0;
	BOOL bRet;
	BYTE bRead = 0;
	DWORD dwRead = 0;
	
	bRet = ReadFile( *phFile, &bRead, 1, &dwRead, NULL );	///should be '0'
	SPDBG( TEXT("SPD@Get_DMCFile_LogCodes_Number: %c \r\n"), bRead );

	bRet = ReadFile( *phFile, &bRead, 1, &dwRead, NULL );	///should be 'x'
	SPDBG( TEXT("SPD@Get_DMCFile_LogCodes_Number: %c \r\n"), bRead );

	bRet = ReadFile( *phFile, &bRead, 1, &dwRead, NULL );	///should be '1'
	SPDBG( TEXT("SPD@Get_DMCFile_LogCodes_Number: %c \r\n"), bRead );
	dwRet = dwRet + (bRead-48)*(0x1000);
	bRet = ReadFile( *phFile, &bRead, 1, &dwRead, NULL );
	SPDBG( TEXT("SPD@Get_DMCFile_LogCodes_Number: %c \r\n"), bRead );
	dwRet = dwRet + (bRead-48)*(0x100);
	bRet = ReadFile( *phFile, &bRead, 1, &dwRead, NULL );
	SPDBG( TEXT("SPD@Get_DMCFile_LogCodes_Number: %c \r\n"), bRead );
	dwRet = dwRet + (bRead-48)*(0x10);
	bRet = ReadFile( *phFile, &bRead, 1, &dwRead, NULL );
	SPDBG( TEXT("SPD@Get_DMCFile_LogCodes_Number: %c \r\n"), bRead );
	dwRet = dwRet + (bRead-48)*(0x1);
	
	bRet = ReadFile( *phFile, &bRead, 1, &dwRead, NULL );	///should be ','
	SPDBG( TEXT("SPD@Get_DMCFile_LogCodes_Number: %c \r\n"), bRead );


	SPDBG( TEXT("SPD@Get_DMCFile_LogCodes_Number: %d \r\n"), dwRet );
	return dwRet;
}


DWORD Get_DMCFile_LogCode_01( BYTE **ppByteAddr )
{
	DWORD dwRet = 0;
	HANDLE hDMCfile = NULL;
	DWORD dwLognumber = -1;
	DWORD dwCurLognumber = 0;
	
	if( DMCFile_Found() )
	{
		do
		{
			///openfile
			if( !Open_DMCFile( &hDMCfile ) )
				break;
			
			///find QXDM key 
			if( !Find_DMCFile_QXDM_key( &hDMCfile ) )
				break;
				
			///find log key 
			if( !Find_DMCFile_LogView_key( &hDMCfile ) )
				break;
		
			///find LogCodes key
			if( !Find_DMCFile_LogCodes_key( &hDMCfile ) )
				break;
			
			///copy LogCodes
			dwRet = 0;
			*((*ppByteAddr)+0) = 0x73;
			dwRet++;
			*((*ppByteAddr)+1) = 0x00;
			dwRet++;
			*((*ppByteAddr)+2) = 0x00;
			dwRet++;
			*((*ppByteAddr)+3) = 0x00;
			dwRet++;
			*((*ppByteAddr)+4) = 0x03;
			dwRet++;
			*((*ppByteAddr)+5) = 0x00;
			dwRet++;
			*((*ppByteAddr)+6) = 0x00;
			dwRet++;
			*((*ppByteAddr)+7) = 0x00;
			dwRet++;
			*((*ppByteAddr)+8) = 0x01;
			dwRet++;
			*((*ppByteAddr)+9) = 0x00;
			dwRet++;
			*((*ppByteAddr)+10) = 0x00;
			dwRet++;
			*((*ppByteAddr)+11) = 0x00;
			dwRet++;
			*((*ppByteAddr)+12) = 0x3B;
			dwRet++;
			*((*ppByteAddr)+13) = 0x04;
			dwRet++;
			*((*ppByteAddr)+14) = 0x00;
			dwRet++;
			*((*ppByteAddr)+15) = 0x00;
			dwRet++;

			
			while( dwCurLognumber < LOGCODE_NUMBER_01 )
			{
				if( dwLognumber < dwCurLognumber || (-1) == dwLognumber )
					dwLognumber = (Get_DMCFile_LogCodes_Number( &hDMCfile ) - 0x1000);
					
				///fill arrary... 
				
				
				dwCurLognumber++;	///next one
			}
			
		}
		while(0);
		
		///close file
		Close_DMCFile( &hDMCfile );
		
	}	
	
	return dwRet;
}

DWORD Get_DMCFile_LogCode_04( BYTE **ppByteAddr )
{
	DWORD dwRet = 0;
	HANDLE hDMCfile = NULL;
	DWORD dwLognumber = -1;
	DWORD dwCurLognumber = 0;
	
	if( DMCFile_Found() )
	{
		do
		{
			///openfile
			if( !Open_DMCFile( &hDMCfile ) )
				break;
			
			///find QXDM key 
			if( !Find_DMCFile_QXDM_key( &hDMCfile ) )
				break;
				
			///find log key 
			if( !Find_DMCFile_LogView_key( &hDMCfile ) )
				break;
		
			///find LogCodes key
			if( !Find_DMCFile_LogCodes_key( &hDMCfile ) )
				break;
			
			///copy LogCodes
			dwRet = 0;
			*((*ppByteAddr)+0) = 0x73;
			dwRet++;
			*((*ppByteAddr)+1) = 0x00;
			dwRet++;
			*((*ppByteAddr)+2) = 0x00;
			dwRet++;
			*((*ppByteAddr)+3) = 0x00;
			dwRet++;
			*((*ppByteAddr)+4) = 0x03;
			dwRet++;
			*((*ppByteAddr)+5) = 0x00;
			dwRet++;
			*((*ppByteAddr)+6) = 0x00;
			dwRet++;
			*((*ppByteAddr)+7) = 0x00;
			dwRet++;
			*((*ppByteAddr)+8) = 0x04;
			dwRet++;
			*((*ppByteAddr)+9) = 0x00;
			dwRet++;
			*((*ppByteAddr)+10) = 0x00;
			dwRet++;
			*((*ppByteAddr)+11) = 0x00;
			dwRet++;
			*((*ppByteAddr)+12) = 0x18;
			dwRet++;
			*((*ppByteAddr)+13) = 0x09;
			dwRet++;
			*((*ppByteAddr)+14) = 0x00;
			dwRet++;
			*((*ppByteAddr)+15) = 0x00;
			dwRet++;

			
			while( dwCurLognumber < LOGCODE_NUMBER_04 )
			{
				if( dwLognumber < dwCurLognumber || (-1) == dwLognumber )
					dwLognumber = (Get_DMCFile_LogCodes_Number( &hDMCfile ) - 0x4000);
					
				///fill arrary... 
				
				
				dwCurLognumber++;	///next one
			}
			
		}
		while(0);
		
		///close file
		Close_DMCFile( &hDMCfile );
		
	}	
	
	return dwRet;
}

DWORD Get_DMCFile_LogCode_05( BYTE **ppByteAddr )
{
	DWORD dwRet = 0;
	HANDLE hDMCfile = NULL;
	DWORD dwLognumber = -1;
	DWORD dwCurLognumber = 0;
	
	if( DMCFile_Found() )
	{
		do
		{
			///openfile
			if( !Open_DMCFile( &hDMCfile ) )
				break;
			
			///find QXDM key 
			if( !Find_DMCFile_QXDM_key( &hDMCfile ) )
				break;
				
			///find log key 
			if( !Find_DMCFile_LogView_key( &hDMCfile ) )
				break;
		
			///find LogCodes key
			if( !Find_DMCFile_LogCodes_key( &hDMCfile ) )
				break;
			
			///copy LogCodes
			dwRet = 0;
			*((*ppByteAddr)+0) = 0x73;
			dwRet++;
			*((*ppByteAddr)+1) = 0x00;
			dwRet++;
			*((*ppByteAddr)+2) = 0x00;
			dwRet++;
			*((*ppByteAddr)+3) = 0x00;
			dwRet++;
			*((*ppByteAddr)+4) = 0x03;
			dwRet++;
			*((*ppByteAddr)+5) = 0x00;
			dwRet++;
			*((*ppByteAddr)+6) = 0x00;
			dwRet++;
			*((*ppByteAddr)+7) = 0x00;
			dwRet++;
			*((*ppByteAddr)+8) = 0x05;
			dwRet++;
			*((*ppByteAddr)+9) = 0x00;
			dwRet++;
			*((*ppByteAddr)+10) = 0x00;
			dwRet++;
			*((*ppByteAddr)+11) = 0x00;
			dwRet++;
			*((*ppByteAddr)+12) = 0x28;
			dwRet++;
			*((*ppByteAddr)+13) = 0x04;
			dwRet++;
			*((*ppByteAddr)+14) = 0x00;
			dwRet++;
			*((*ppByteAddr)+15) = 0x00;
			dwRet++;

			
			while( dwCurLognumber < LOGCODE_NUMBER_05 )
			{
				if( dwLognumber < dwCurLognumber || (-1) == dwLognumber )
					dwLognumber = (Get_DMCFile_LogCodes_Number( &hDMCfile ) - 0x5000);
					
				///fill arrary... 
				
				
				dwCurLognumber++;	///next one
			}
			
		}
		while(0);
		
		///close file
		Close_DMCFile( &hDMCfile );
		
	}	
	
	return dwRet;
}

DWORD Get_DMCFile_LogCode_07( BYTE **ppByteAddr )
{
	DWORD dwRet = 0;
	HANDLE hDMCfile = NULL;
	DWORD dwLognumber = -1;
	DWORD dwCurLognumber = 0;
	
	if( DMCFile_Found() )
	{
		do
		{
			///openfile
			if( !Open_DMCFile( &hDMCfile ) )
				break;
			
			///find QXDM key 
			if( !Find_DMCFile_QXDM_key( &hDMCfile ) )
				break;
				
			///find log key 
			if( !Find_DMCFile_LogView_key( &hDMCfile ) )
				break;
		
			///find LogCodes key
			if( !Find_DMCFile_LogCodes_key( &hDMCfile ) )
				break;
			
			///copy LogCodes
			dwRet = 0;
			*((*ppByteAddr)+0) = 0x73;
			dwRet++;
			*((*ppByteAddr)+1) = 0x00;
			dwRet++;
			*((*ppByteAddr)+2) = 0x00;
			dwRet++;
			*((*ppByteAddr)+3) = 0x00;
			dwRet++;
			*((*ppByteAddr)+4) = 0x03;
			dwRet++;
			*((*ppByteAddr)+5) = 0x00;
			dwRet++;
			*((*ppByteAddr)+6) = 0x00;
			dwRet++;
			*((*ppByteAddr)+7) = 0x00;
			dwRet++;
			*((*ppByteAddr)+8) = 0x07;
			dwRet++;
			*((*ppByteAddr)+9) = 0x00;
			dwRet++;
			*((*ppByteAddr)+10) = 0x00;
			dwRet++;
			*((*ppByteAddr)+11) = 0x00;
			dwRet++;
			*((*ppByteAddr)+12) = 0x07;
			dwRet++;
			*((*ppByteAddr)+13) = 0x05;
			dwRet++;
			*((*ppByteAddr)+14) = 0x00;
			dwRet++;
			*((*ppByteAddr)+15) = 0x00;
			dwRet++;

			
			while( dwCurLognumber < LOGCODE_NUMBER_07 )
			{
				if( dwLognumber < dwCurLognumber || (-1) == dwLognumber )
					dwLognumber = (Get_DMCFile_LogCodes_Number( &hDMCfile ) - 0x7000);
					
				///fill arrary... 
				
				
				dwCurLognumber++;	///next one
			}
			
		}
		while(0);
		
		///close file
		Close_DMCFile( &hDMCfile );
		
	}	
	
	return dwRet;
}

DWORD Get_DMCFile_MessageLevels_01( BYTE **ppByteAddr )
{
	DWORD dwRet = 0;
	HANDLE hDMCfile = NULL;
	DWORD dwLognumber = -1;
	DWORD dwCurLognumber = 0;
	
	if( DMCFile_Found() )
	{
		do
		{
			///openfile
			if( !Open_DMCFile( &hDMCfile ) )
				break;
			
			///find QXDM key 
			if( !Find_DMCFile_QXDM_key( &hDMCfile ) )
				break;
				
			///find log key 
			if( !Find_DMCFile_LogView_key( &hDMCfile ) )
				break;
		
			///find LogCodes key
			if( !Find_DMCFile_MessageLevels_key( &hDMCfile ) )
				break;
			
			///copy LogCodes
			dwRet = 0;
			*((*ppByteAddr)+0) = 0x7D;
			dwRet++;
			*((*ppByteAddr)+1) = 0x5D;
			dwRet++;
			*((*ppByteAddr)+2) = 0x04;
			dwRet++;
			*((*ppByteAddr)+3) = 0x00;
			dwRet++;
			*((*ppByteAddr)+4) = 0x00;
			dwRet++;
			*((*ppByteAddr)+5) = 0x35;
			dwRet++;
			*((*ppByteAddr)+6) = 0x00;
			dwRet++;
			*((*ppByteAddr)+7) = 0x00;
			dwRet++;
			*((*ppByteAddr)+8) = 0x00;
			dwRet++;

			
			while( dwCurLognumber < MESSAGE_NUMBER_000 )
			{
				if( dwLognumber < dwCurLognumber || (-1) == dwLognumber )
					dwLognumber = (Get_DMCFile_LogCodes_Number( &hDMCfile ) - 0x7000);
					
				///fill arrary... 
				
				
				dwCurLognumber++;	///next one
			}
			
		}
		while(0);
		
		///close file
		Close_DMCFile( &hDMCfile );
		
	}	
	
	return dwRet;
}


///-///-///-///-///-///-///-///-///-///-///-///-///-///-///-///-///-///-///-///-

BOOL __spCElogDMC_CfgFileFound()
{
	return DMCFile_Found();
}