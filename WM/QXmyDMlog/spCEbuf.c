// spCEbuf.c : Defines the entry point for the application.
//

///#include <stdafx.h>
#include <windows.h>
#include "spCEbuf.h"

///
#define SPERR						NKDbgPrintfW
///#define SPDBG						NKDbgPrintfW
#define SPDBG						///void
#define SPCEBUF_LOCK				EnterCriticalSection(&spCEbuf_stat.cs);
#define SPCEBUF_UNLOCK				LeaveCriticalSection(&spCEbuf_stat.cs);


///-///-///-///-///-///-///-///-///-///-///-///-///-///-///-///-///-///-///-///-
BOOL spCEbuf_Inited();
BOOL spCEbuf_Try_Init();
BOOL spCEbuf_Try_DeInit();

///-///-///-///-///-///-///-///-///-///-///-///-///-///-///-///-///-///-///-///-
static spCEbuf_status 	spCEbuf_stat;



///-///-///-///-///-///-///-///-///-///-///-///-///-///-///-///-///-///-///-///-
BOOL spCEbuf_Init()
{
	BOOL bRet = TRUE;
	
	bRet = spCEbuf_Try_Init();
	
	return bRet;
}

BOOL spCEbuf_DeInit()
{
	BOOL bRet = TRUE;
	
	bRet = spCEbuf_Try_DeInit();
	
	return bRet;
}

BOOL spCEbuf_Get_Status_Inited()
{
	BOOL bRet;
	
	SPCEBUF_LOCK
	bRet = spCEbuf_stat.bInited;
	SPCEBUF_UNLOCK
	
	return bRet;
}

BOOL spCEbuf_Set_Status_Inited( BOOL bInit )
{
	BOOL bRet;
	
	SPCEBUF_LOCK
	if( TRUE == bInit )
		spCEbuf_stat.bInited += 1;
	else
		spCEbuf_stat.bInited = 0;
	SPCEBUF_UNLOCK	
		
	bRet = spCEbuf_Get_Status_Inited();
	
	return bRet;
}

DWORD spCEbuf_Get_Status_ExtraInfo()
{
	DWORD dwRet;
	
	SPCEBUF_LOCK
	dwRet = spCEbuf_stat.dwExtraInfo;
	SPCEBUF_UNLOCK
	
	return dwRet;
}

DWORD spCEbuf_Set_Status_ExtraInfo( DWORD dwInit )
{
	DWORD dwRet;
	
	SPCEBUF_LOCK
	spCEbuf_stat.dwExtraInfo = dwInit;
	SPCEBUF_UNLOCK	
		
	dwRet = spCEbuf_Get_Status_ExtraInfo();
	
	return dwRet;
}

BOOL spCEbuf_Get_Status_ExtraInfo_Enable()
{
	BOOL bRet;
	
	SPCEBUF_LOCK
	bRet = (spCEbuf_stat.dwExtraInfo & SPCEBUF_EXTRAINFO_ENABLE_MASK) > 0 ? TRUE : FALSE;
	SPCEBUF_UNLOCK
	
	return bRet;
}

BOOL spCEbuf_Set_Status_ExtraInfo_Enable( BOOL bEnable )
{
	BOOL bRet;
	
	SPCEBUF_LOCK
	if( TRUE == bEnable )
		spCEbuf_stat.dwExtraInfo  |= SPCEBUF_EXTRAINFO_ENABLE_MASK;
	else
		spCEbuf_stat.dwExtraInfo  &= ~SPCEBUF_EXTRAINFO_ENABLE_MASK;;
	SPCEBUF_UNLOCK	
		
	bRet = spCEbuf_Get_Status_ExtraInfo_Enable();
	
	return bRet;
}

BOOL spCEbuf_Get_Status_ExtraInfo_TimeStamp_Enable()
{
	BOOL bRet;
	
	SPCEBUF_LOCK
	bRet = (spCEbuf_stat.dwExtraInfo & SPCEBUF_EXTRAINFO_TIMESTAMP_ENABLE_MASK) > 0 ? TRUE : FALSE;
	SPCEBUF_UNLOCK
	
	return bRet;
}

BOOL spCEbuf_Set_Status_ExtraInfo_TimeStamp_Enable( BOOL bEnable )
{
	BOOL bRet;
	
	SPCEBUF_LOCK
	if( TRUE == bEnable )
		spCEbuf_stat.dwExtraInfo  |= SPCEBUF_EXTRAINFO_TIMESTAMP_ENABLE_MASK;
	else
		spCEbuf_stat.dwExtraInfo  &= ~SPCEBUF_EXTRAINFO_TIMESTAMP_ENABLE_MASK;;
	SPCEBUF_UNLOCK	
		
	bRet = spCEbuf_Get_Status_ExtraInfo_TimeStamp_Enable();
	
	return bRet;
}

BOOL spCEbuf_Get_Status_ExtraInfo_Order_Enable()
{
	BOOL bRet;
	
	SPCEBUF_LOCK
	bRet = (spCEbuf_stat.dwExtraInfo & SPCEBUF_EXTRAINFO_ORDER_ENABLE_MASK) > 0 ? TRUE : FALSE;
	SPCEBUF_UNLOCK
	
	return bRet;
}

BOOL spCEbuf_Set_Status_ExtraInfo_Order_Enable( BOOL bEnable )
{
	BOOL bRet;
	
	SPCEBUF_LOCK
	if( TRUE == bEnable )
		spCEbuf_stat.dwExtraInfo  |= SPCEBUF_EXTRAINFO_ORDER_ENABLE_MASK;
	else
		spCEbuf_stat.dwExtraInfo  &= ~SPCEBUF_EXTRAINFO_ORDER_ENABLE_MASK;;
	SPCEBUF_UNLOCK	
		
	bRet = spCEbuf_Get_Status_ExtraInfo_Order_Enable();
	
	return bRet;
}

BOOL spCEbuf_Get_Status_ExtraInfo_Direction_Enable()
{
	BOOL bRet;
	
	SPCEBUF_LOCK
	bRet = (spCEbuf_stat.dwExtraInfo & SPCEBUF_EXTRAINFO_DIRECTION_ENABLE_MASK) > 0 ? TRUE : FALSE;
	SPCEBUF_UNLOCK
	
	return bRet;
}

BOOL spCEbuf_Set_Status_ExtraInfo_Direction_Enable( BOOL bEnable )
{
	BOOL bRet;
	
	SPCEBUF_LOCK
	if( TRUE == bEnable )
		spCEbuf_stat.dwExtraInfo  |= SPCEBUF_EXTRAINFO_DIRECTION_ENABLE_MASK;
	else
		spCEbuf_stat.dwExtraInfo  &= ~SPCEBUF_EXTRAINFO_DIRECTION_ENABLE_MASK;;
	SPCEBUF_UNLOCK	
		
	bRet = spCEbuf_Get_Status_ExtraInfo_Direction_Enable();
	
	return bRet;
}

DWORD spCEbuf_Get_Status_Direction()
{
	DWORD dwRet;
	
	SPCEBUF_LOCK
	dwRet = spCEbuf_stat.dwDirection;
	SPCEBUF_UNLOCK
	
	return dwRet;
}

DWORD spCEbuf_Set_Status_Direction( DWORD dwSet )
{
	DWORD dwRet;
	
	SPCEBUF_LOCK
	spCEbuf_stat.dwDirection = dwSet;
	SPCEBUF_UNLOCK	
		
	dwRet = spCEbuf_Get_Status_Direction();
	
	return dwRet;
}


DWORD spCEbuf_Get_Status_Count()
{
	DWORD dwRet;
	
	SPCEBUF_LOCK
	dwRet = spCEbuf_stat.dwCount;
	SPCEBUF_UNLOCK
	
	return dwRet;
}

DWORD spCEbuf_Get_Status_IdCount()
{
	DWORD dwRet;
	
	SPCEBUF_LOCK
	dwRet = spCEbuf_stat.dwIdCount;
	SPCEBUF_UNLOCK
	
	return dwRet;
}

spCEbuf* spCEbuf_Get_Status_Head()
{
	spCEbuf* pBufRet;
	
	SPCEBUF_LOCK
	pBufRet = spCEbuf_stat.pHead;
	SPCEBUF_UNLOCK
	
	return pBufRet;
}


///-///-///-///-///-///-///-///-///-///-///-///-///-///-///-///-///-///-///-///-

/*
	spCEbuf_Inited() is not protect by critical section.
	so it could be used while critical sction possiblily not inited.
*/
BOOL spCEbuf_Inited()
{
	BOOL bRet;
	
	///bRet = spCEbuf_Get_Status_Inited() > 0 ? TRUE : FALSE;
	bRet = spCEbuf_stat.bInited > 0 ? TRUE : FALSE;
	
	return bRet;
}

BOOL spCEbuf_Try_Init()
{
	BOOL bRet = TRUE;
	HANDLE hTmp = NULL;
	
	///try init the log system
	///init parameter
	InitializeCriticalSection( &spCEbuf_stat.cs );

	SPCEBUF_LOCK
	spCEbuf_stat.pHead = NULL;
	spCEbuf_stat.pTail = NULL;
	spCEbuf_stat.dwSize = 0;
	spCEbuf_stat.dwIdCount = 0;
	spCEbuf_stat.dwCount = 0;
	spCEbuf_stat.dwExtraInfo = 0;
	spCEbuf_stat.dwDirection = SPCEBUF_EXTRAINFO_DIRECTION_NO;
	SPCEBUF_UNLOCK

	spCEbuf_Set_Status_Inited( TRUE );
	
	return bRet;
}

BOOL spCEbuf_Try_DeInit()
{
	BOOL bRet = TRUE;
	HANDLE hTmp = NULL;
	
	if( spCEbuf_Get_Status_Count() > 0 )
	{
		SPERR( TEXT("SPD@spCEbuf_Try_DeInit: still buffer %d , deinit may possible cuase memory leak !!!!!!!!!!!!!!!!.\r\n"), spCEbuf_Get_Status_Count() );
	}
	
	SPCEBUF_LOCK
	spCEbuf_stat.pHead = NULL;
	spCEbuf_stat.pTail = NULL;
	spCEbuf_stat.dwSize = 0;
	spCEbuf_stat.dwIdCount = 0;
	spCEbuf_stat.dwCount = 0;
	spCEbuf_stat.dwExtraInfo = 0;
	spCEbuf_stat.dwDirection = SPCEBUF_EXTRAINFO_DIRECTION_NO;
	SPCEBUF_UNLOCK

	spCEbuf_Set_Status_Inited( FALSE );	
	
	DeleteCriticalSection( &spCEbuf_stat.cs );

	return bRet;
}


void spCEbuf_DumpTag( spCEbuf* pTag )
{
	if( NULL != pTag )
	{
		SPDBG( TEXT("SPD@spCEbuf_DumpTag: %d-0x%08x-0x%08x-0x%08x-%d.\r\n"), pTag->dwID, pTag->pPrevBuf, pTag->pNextBuf, pTag->pThisBuf, pTag->dwBufSize );
		if( NULL != pTag->pExtraInfo )
		{
			spCEbufExtinfo* pInfo = pTag->pExtraInfo;
			SPDBG( TEXT("SPD@spCEbuf_DumpTag: ExtraInfo: 0x%08x-%d-0x%08x-%d.\r\n"), pInfo->TimeStamp, pInfo->Order, pInfo->dwDirection, pInfo->dwDataSize );	
		}
	}
}

PUCHAR spCEbuf_NewBuf( DWORD dwSize )
{
	PUCHAR puRet = NULL;
	
	puRet = (PUCHAR)malloc( dwSize );
	
	return puRet;
}

PUCHAR spCEbuf_New( DWORD dwSize )
{
	return spCEbuf_NewBuf( dwSize );
}


BOOL spCEbuf_DelBuf( PUCHAR pucBuf )
{
	BOOL bRet = TRUE;
	
	free( pucBuf );
	pucBuf = NULL;
	
	return bRet;
}

BOOL spCEbuf_Del( PUCHAR pucBuf )
{
	return spCEbuf_DelBuf( pucBuf );
}

spCEbufExtinfo* spCEbuf_Gen_ExtraInfo( DWORD dwDir )
{
	spCEbufExtinfo* pNewExtInfo = NULL;
	
	if( spCEbuf_Get_Status_ExtraInfo_Enable() )
	{
		pNewExtInfo = (spCEbufExtinfo*)malloc( sizeof(spCEbufExtinfo) );
		
		pNewExtInfo->dwTag1 = SPCEBUF_EXTRAINFO_TAG1;
		pNewExtInfo->dwTag2 = SPCEBUF_EXTRAINFO_TAG2;
		
		if( spCEbuf_Get_Status_ExtraInfo_TimeStamp_Enable() )
			pNewExtInfo->TimeStamp = GetTickCount();
		else
			pNewExtInfo->TimeStamp = 0;
			
		if( spCEbuf_Get_Status_ExtraInfo_Order_Enable() )
			pNewExtInfo->Order = spCEbuf_Get_Status_IdCount();
		else
			pNewExtInfo->Order = 0;

		if( spCEbuf_Get_Status_ExtraInfo_Direction_Enable() )
			pNewExtInfo->dwDirection = dwDir;///spCEbuf_Get_Status_Direction();
		else
			pNewExtInfo->dwDirection = 0;

	}
	
	return pNewExtInfo;
}

BOOL spCEbuf_AddBuf_Tail( PUCHAR pData, DWORD dwSize, DWORD dwDir )
{
	BOOL bRet = TRUE;
	
	spCEbuf* pNewTag = NULL;
	
	
	SPDBG( TEXT("SPD@spCEbuf_AddBuf_Tail:+++ \r\n") );
	
	if( !spCEbuf_Inited() )
	{
		SPERR( TEXT("SPD@spCEbuf_AddBuf_Tail: buffer not inited !!!!! \r\n") );
		bRet = FALSE;
		goto EXIT_ADDBUF_TAIL;
	}
	
	pNewTag = (spCEbuf*)malloc( sizeof(spCEbuf) );
	
	SPCEBUF_LOCK
	pNewTag->dwID = spCEbuf_stat.dwIdCount;
	pNewTag->pThisBuf = pData;
	pNewTag->dwBufSize = dwSize;
	pNewTag->pExtraInfo = spCEbuf_Gen_ExtraInfo( dwDir );
	pNewTag->pExtraInfo->dwDataSize = dwSize;
	spCEbuf_stat.dwIdCount++;
	spCEbuf_stat.dwCount++;
	if( NULL == spCEbuf_stat.pHead )	///first init
	{
		spCEbuf_stat.pHead = pNewTag;
		spCEbuf_stat.pTail = pNewTag;
		pNewTag->pPrevBuf = NULL;
		pNewTag->pNextBuf = NULL;
	}
	else
	{
		///spCEbuf_stat.pHead = pNewTag;
		pNewTag->pPrevBuf = spCEbuf_stat.pTail;
		(spCEbuf_stat.pTail)->pNextBuf = pNewTag;
		spCEbuf_stat.pTail = pNewTag;
		
		pNewTag->pNextBuf = NULL;
	}
	SPCEBUF_UNLOCK
	
	spCEbuf_DumpTag( pNewTag );
	
EXIT_ADDBUF_TAIL:	
	return bRet;
}

BOOL spCEbuf_Add( PUCHAR pData, DWORD dwSize, DWORD dwDir )
{
	return spCEbuf_AddBuf_Tail( pData, dwSize, dwDir );
}

///BOOL spCEbuf_RemoveBuf_Head( PUCHAR pData, PDWORD pdwSize )
BOOL spCEbuf_RemoveBuf_Head( PDWORD pData, PDWORD pdwSize )
{
	BOOL bRet = TRUE;
	
	spCEbuf* pNewTag = NULL;
	
	SPDBG( TEXT("SPD@spCEbuf_RemoveBuf_Head:+++ \r\n") );

	if( !spCEbuf_Inited() )
	{
		SPERR( TEXT("SPD@spCEbuf_RemoveBuf_Head: buffer not inited !!!!! \r\n") );
		bRet = FALSE;
		goto EXIT_REMOVEBUF_HEAD;
	}
	
	SPCEBUF_LOCK

	if( NULL == spCEbuf_stat.pHead )	///first init
	{	///nothing to remove
		goto EXIT_REMOVEBUF_HEAD;
	}
	else
	if ( 1 == spCEbuf_stat.dwCount )
	{	///the last one
		pNewTag = spCEbuf_stat.pHead;
		spCEbuf_stat.pHead = NULL;
		spCEbuf_stat.pTail = NULL;
		spCEbuf_stat.dwCount--;
		///pData = pNewTag->pThisBuf;
		*pData = (DWORD)pNewTag->pThisBuf;
		*pdwSize = pNewTag->dwBufSize;		
	}
	else
	{
		///spCEbuf_stat.pHead = pNewTag;
		pNewTag = spCEbuf_stat.pHead;
		spCEbuf_stat.pHead = (spCEbuf_stat.pHead)->pNextBuf;
		(spCEbuf_stat.pHead)->pPrevBuf = NULL;		
		spCEbuf_stat.dwCount--;

		///pData = pNewTag->pThisBuf;
		*pData = (DWORD)pNewTag->pThisBuf;
		*pdwSize = pNewTag->dwBufSize;
	}

	SPCEBUF_UNLOCK
	
	spCEbuf_DumpTag( pNewTag );

	
	///free tag memory
	///if( NULL != spCEbuf_stat.pHead )
	if( 1 )
	{
		if( spCEbuf_Get_Status_ExtraInfo_Enable() )
		{
			PUCHAR pDataExtra = NULL;
			DWORD  dwSizeExtra = 0;
		
			dwSizeExtra = pNewTag->dwBufSize + sizeof(spCEbufExtinfo);
			pDataExtra = (PUCHAR)malloc( dwSizeExtra );
		
			if( NULL != pDataExtra )
			{
				DWORD dwInfoSize;
				dwInfoSize = sizeof(spCEbufExtinfo);
				memcpy( pDataExtra, pNewTag->pExtraInfo, dwInfoSize );
				memcpy( pDataExtra+dwInfoSize, pNewTag->pThisBuf, pNewTag->dwBufSize );
				
				*pData = (DWORD)pDataExtra;
				*pdwSize = dwSizeExtra;
				
				free( pNewTag->pThisBuf );
			}
		}

		free( pNewTag->pExtraInfo );
		free( pNewTag );
	}

EXIT_REMOVEBUF_HEAD:

	return bRet;
}

///BOOL spCEbuf_RemBuf( PUCHAR pData, PDWORD pdwSize )
BOOL spCEbuf_RemBuf( PDWORD pData, PDWORD pdwSize )
{
	return spCEbuf_RemoveBuf_Head( pData, pdwSize );
}

///BOOL spCEbuf_GetBuf( PUCHAR pData, PDWORD pdwSize )
BOOL spCEbuf_GetBuf( PDWORD pData, PDWORD pdwSize )
{
	return spCEbuf_RemoveBuf_Head( pData, pdwSize );
}

void spCEbuf_DumpBuf( spCEbuf* pTemp )
{
	DWORD dwCnt = 0;
	PUCHAR puChar = NULL;
	
	SPDBG( TEXT("SPD@spCEbuf_DumpBuf: 0x%x-%d.\r\n"), pTemp->pThisBuf, pTemp->dwBufSize );
	dwCnt = pTemp->dwBufSize;
	puChar = pTemp->pThisBuf;
	while( dwCnt > 0 )
	{
		SPDBG( TEXT("%c"), puChar++ );
		dwCnt--;
	}
	
	SPDBG( TEXT("\r\n") );
}

BOOL spCEbuf_DumpAll()
{
	BOOL bRet = TRUE;
	spCEbuf* pTemp = NULL;
	
	if( !spCEbuf_Inited() )
	{
		SPERR( TEXT("SPD@spCEbuf_DumpAll: buffer not inited !!!!! \r\n") );
		bRet = FALSE;
		goto EXIT_DUMPALL;
	}
	
	pTemp = spCEbuf_Get_Status_Head();
	
	if( NULL == pTemp )
	{
		return FALSE;
	}
	
	do
	{
		spCEbuf_DumpBuf( pTemp );
		pTemp = pTemp->pNextBuf;
	}
	while( NULL != pTemp->pNextBuf );

EXIT_DUMPALL:
	
	return bRet;
}