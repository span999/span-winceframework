/*
	header file for spLog class 
	spCElog.h will be main file.
*/

#ifndef __SPCELOGFAKEHOST_H__
#define __SPCELOGFAKEHOST_H__


#define SPCELOG_DBFILE_NAME				TEXT("spCElog-db.set")
#define SPCELOG_SDDBFILE_NAME			TEXT("\\Storage Card\\spCElog-db.set")

#define	HOST_CMD_TABLE_SIZE		106


typedef struct SPCELOGFAKEHOST_STATUS_T
{
	BOOL		bInited;
	CRITICAL_SECTION	cs;
	DWORD		dwNextCmdStatus;
	HANDLE		hDBfileHandle;
	BYTE		bNextCmd[512];
    DWORD   	dwNextCmdBufSize;
	BOOL   		bNextCmdCont;
	BOOL		bNextCmdRead;
	BOOL		bNextCmdGotRsp;
} spCElogFakeHost_status;

typedef struct SPCELOG_DB_CMD_MAPPING_T
{
	DWORD		dwStatIndex;
    DWORD   	dwDbCmdIndex;
	BOOL		bContCmd;
	DWORD   	dwDbRspIndex;
} spCElog_db_Cmd_Mapping;


BOOL __spCElogFakeHost_Init();
DWORD __spCElogFakeHost_Get_StatusNum();
void __spCElogFakeHost_Reset_StatusNum();	///span0714, add for reset
BOOL __spCElogFakeHost_SetupNextCmd();
PUCHAR __spCElogFakeHost_Get_NextCmdBuffer();
DWORD __spCElogFakeHost_Get_NextCmdBufSize();
BOOL __spCElogFakeHost_Get_NextCmdCont();
BOOL __spCElogFakeHost_Get_NextCmdRead();
BOOL __spCElogFakeHost_Set_NextCmdRead();
BOOL __spCElogFakeHost_Get_NextCmdGotRsp();
BOOL __spCElogFakeHost_Set_NextCmdGotRsp();


#endif	///#ifndef __SPCELOGFAKEHOST_H__