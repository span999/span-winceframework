/*
	header file for spLog class 
	spCEbuf.h will be main file.
*/

#ifndef __SPCEBUF_H__
#define __SPCEBUF_H__

#define SPCEBUF_EXTRAINFO_TAG1			0x61727865
#define SPCEBUF_EXTRAINFO_TAG2			0x6f666e69

#define SPCEBUF_EXTRAINFO_ENABLE_MASK				0x00008000
#define SPCEBUF_EXTRAINFO_TIMESTAMP_ENABLE_MASK		0x00004000
#define SPCEBUF_EXTRAINFO_ORDER_ENABLE_MASK			0x00002000
#define SPCEBUF_EXTRAINFO_DIRECTION_ENABLE_MASK		0x00001000

#define SPCEBUF_EXTRAINFO_DIRECTION_NO		0x12345678
#define SPCEBUF_EXTRAINFO_DIRECTION_1		0x11115678
#define SPCEBUF_EXTRAINFO_DIRECTION_2		0x22225678
#define SPCEBUF_EXTRAINFO_DIRECTION_3		0x33335678


typedef struct SPCEBUFEXTINFO_T
{
	DWORD       dwTag1;
	DWORD       dwTag2;
	DWORD		TimeStamp;
	DWORD		Order;
    DWORD   	dwDirection;
	DWORD   	dwDataSize;
} spCEbufExtinfo;


typedef struct SPCEBUF_T
{
	DWORD		dwID;
    struct SPCEBUF_T*	pPrevBuf;
	struct SPCEBUF_T*	pNextBuf;
	PUCHAR		pThisBuf;
    DWORD   	dwBufSize;
	spCEbufExtinfo*		pExtraInfo;
} spCEbuf;


typedef struct SPCEBUF_STATUS_T
{
	BOOL		bInited;
	CRITICAL_SECTION	cs;
	spCEbuf*	pHead;
	spCEbuf*	pTail;
	DWORD		dwSize;
	DWORD		dwIdCount;
	DWORD		dwCount;
	DWORD		dwExtraInfo;
	DWORD   	dwDirection;
    DWORD		dwRESERVED[2];
} spCEbuf_status;


///
BOOL spCEbuf_Init();
BOOL spCEbuf_DeInit();
DWORD spCEbuf_Get_Status_Count();
DWORD spCEbuf_Get_Status_IdCount();
DWORD spCEbuf_Get_Status_ExtraInfo();
DWORD spCEbuf_Set_Status_ExtraInfo( DWORD dwInit );

PUCHAR spCEbuf_New( DWORD dwSize );
BOOL spCEbuf_Del( PUCHAR pucBuf );

BOOL spCEbuf_Add( PUCHAR pData, DWORD dwSize, DWORD dwDir );
BOOL spCEbuf_RemBuf( PDWORD pData, PDWORD pdwSize );
BOOL spCEbuf_GetBuf( PDWORD pData, PDWORD pdwSize );

BOOL spCEbuf_DumpAll();

#endif	///#ifndef __SPCEBUF_H__