// Cwin32test.cpp : Defines the entry point for the application.
//

#include <stdafx.h>
#include <windows.h>
#include "spToolHelp.h"


spTHcomponentList::spTHcomponentList()
{
	PriInit();
}

spTHcomponentList::~spTHcomponentList()
{
	PriInit();
	InitializeCriticalSection( &m_csTHcomponentList );
}

BOOL spTHcomponentList::PriInit()
{
	BOOL bRet = FALSE;

	m_dwTotalListNum = 0;
	m_dwCurListIndex = 0;
	pHeadLink = NULL;
	pTailLink = NULL;
	pCurLink = NULL;
	m_pLog = NULL;
	///m_csTHcomponentList = -1;	///	
	
	return bRet;
}

BOOL spTHcomponentList::Init()
{
	BOOL bRet = FALSE;
	
	return bRet;
}

BOOL spTHcomponentList::DeInit()
{
	BOOL bRet = FALSE;
	
	return bRet;
}

void spTHcomponentList::FillLinkItem( pspLinkList pItem, DWORD dwIdx, DWORD dwFlag, pspLinkList pPre, pspLinkList pNext, PVOID pContent )
{
	pItem->dwIndex = dwIdx;
	pItem->dwFlag = dwFlag;
	pItem->pPreLink = (PVOID)(pPre);
	pItem->pNextLink = (PVOID)(pNext);
	pItem->pContent = pContent;
}

BOOL spTHcomponentList::AddLink( PVOID pNew )
{
	BOOL bRet = FALSE;
	pspLinkList pTmp = NULL;
	
	if( pNew )
	{
		TakeCS();
		
		if( (NULL == pHeadLink) && (NULL == pTailLink) )
		{	///the first item
			pTmp = new spLinkList;
			if( pTmp )
			{	///add the new one to list
				pHeadLink = pTmp;
				pTailLink = pTmp;
				FillLinkItem( pTmp, 1, 0, NULL, NULL, pNew );
				bRet = TRUE;
				m_dwTotalListNum++;
			}
		}
		else
		if( (NULL != pHeadLink) && (NULL != pTailLink) )
		{	///normal item
			pTmp = new spLinkList;
			if( pTmp )
			{	///add the new one to list
				pTailLink->pNextLink = pTmp;
				FillLinkItem( pTmp, (pTailLink->dwIndex+1), 0, pTailLink, NULL, pNew );
				pTailLink = pTmp;
				bRet = TRUE;
				m_dwTotalListNum++;
			}
		}
		else
		{
		}
	
		ReleaseCS();
	}
	
	return bRet;
}

PVOID spTHcomponentList::DeleteLink( PVOID pThisContent )
{
	PVOID pRet = NULL;
	pspLinkList pTmp = NULL;
	
	pTmp = IspContentValid( pThisContent );
	
	if( NULL != pTmp )
	{	
		pRet = pTmp->pContent;
		///delete this
		if( !DeleteLink( pTmp ) )
			pRet = NULL;
	}
 	
	return pRet;
}

PVOID spTHcomponentList::DeleteLink( DWORD dwIndex )
{
	PVOID pRet = NULL;
	pspLinkList pTmp = NULL;
	
	pTmp = IsIndexValid( dwIndex );
	
	if( NULL != pTmp )
	{	
		pRet = pTmp->pContent;
		///delete this
		if( !DeleteLink( pTmp ) )
			pRet = NULL;
	}
 	
	return pRet;
}

BOOL spTHcomponentList::DeleteLink( pspLinkList pLink )
{
	BOOL bRet = FALSE;
	
	if( pLink && pHeadLink )
	{
		pspLinkList pTmp = pHeadLink;
		
		do{
			if( pTmp == pLink )
				bRet = TRUE;
			else	
				pTmp = (pspLinkList)pTmp->pNextLink;
		}while( (NULL != pTmp) && (!bRet) );
		
		if( bRet )///found
		{
			pspLinkList pPre = (pspLinkList)pTmp->pPreLink;
			
			TakeCS();
			
			if( NULL == pPre )
			{	///first one
				pHeadLink = NULL;
				pTailLink = NULL;
				delete pLink;
				m_dwTotalListNum = 0;
			}
			else
			if( NULL == pTmp->pNextLink )
			{	///last one
				pPre->pNextLink = NULL;
				pTailLink = pPre;
				delete pLink;
				m_dwTotalListNum--;
			}
			else
			{
				pspLinkList pNext = (pspLinkList)pTmp->pNextLink;
				pPre->pNextLink = pNext;
				pNext->pPreLink = pPre;
				delete pLink;
				m_dwTotalListNum--;
			}
			m_dwCurListIndex = 0;
			pCurLink = NULL;

			ReleaseCS();
		}
	}
	
	return bRet;
}

DWORD spTHcomponentList::GetTotalListNum()
{
	return m_dwTotalListNum;
}

DWORD spTHcomponentList::GetCurListIndex()
{
	BOOL dwRet = 0;
	
	return dwRet;
}

pspLinkList spTHcomponentList::IspContentValid( PVOID pThisContent )
{
	pspLinkList pRet = NULL;
	
	if( pThisContent && pHeadLink )
	{
		pspLinkList pTmp = pHeadLink;
		
		do{
			if( pTmp->pContent == pThisContent )
				pRet = pTmp;
				
			pTmp = (pspLinkList)pTmp->pNextLink;
		}while( (NULL != pTmp) && (NULL == pRet) );
	}
	
	return pRet;
}

pspLinkList spTHcomponentList::IsIndexValid( DWORD dwIndex )
{
	pspLinkList pRet = NULL;
#if 0	
	if( dwIndex && pHeadLink )
	{
		pspLinkList pTmp = pHeadLink;
		
		do{
			if( pTmp->dwIndex == dwIndex )
				pRet = pTmp;
				
			pTmp = (pspLinkList)pTmp->pNextLink;
		}while( (NULL != pTmp) && (NULL == pRet) );
	}
#else
	if( dwIndex && pCurLink )
	{
		pspLinkList pTmp = pCurLink;
		
		do{
			if( pTmp->dwIndex == dwIndex )
			{
				pRet = pTmp;
				m_dwCurListIndex = pTmp->dwIndex;
				pCurLink = pTmp;
			}	
				
			pTmp = (pspLinkList)pTmp->pNextLink;
		}while( (NULL != pTmp) && (NULL == pRet) );
	}
	
	if( dwIndex && pHeadLink && (NULL == pRet) )
	{
		pspLinkList pTmp = pHeadLink;
		
		do{
			if( pTmp->dwIndex == dwIndex )
			{
				pRet = pTmp;
				m_dwCurListIndex = pTmp->dwIndex;
				pCurLink = pTmp;
			}	
				
			pTmp = (pspLinkList)pTmp->pNextLink;
		}while( (NULL != pTmp) && (NULL == pRet) );
	}
#endif
	return pRet;
}

void spTHcomponentList::DumpListMsg()
{
	pspLinkList pTmp = pHeadLink;
	
	while( NULL != pTmp )
	{
		NKDbgPrintfW(L"SPD@DumpListMsg Index=%04d,Flag=0x%08x,pPre=0x%08X,pNext=0x%08X,pCont=pPre=0x%08X",
			pTmp->dwIndex,
			pTmp->dwFlag,
			pTmp->pPreLink,
			pTmp->pNextLink,
			pTmp->pContent
			);
		NKDbgPrintfW(L"\r\n" );
		pTmp = (pspLinkList)pTmp->pNextLink;///next one
	}
}

void spTHcomponentList::LogListMsg()
{
	pspLinkList pTmp = pHeadLink;
	
	while( NULL != pTmp && NULL != m_pLog )
	{
		m_pLog->WriteLog(L"Index=%04d,Flag=0x%08x,pPre=0x%08X,pNext=0x%08X,pCont=pPre=0x%08X",
			pTmp->dwIndex,
			pTmp->dwFlag,
			pTmp->pPreLink,
			pTmp->pNextLink,
			pTmp->pContent
			);
		m_pLog->WriteLog(L"\r\n" );
		pTmp = (pspLinkList)pTmp->pNextLink;///next one
	}
}

BOOL spTHcomponentList::SetupLogPointer( spLog *pLogPointer )
{
	BOOL bRet = FALSE;
	
	if( pLogPointer )
	{
		m_pLog = pLogPointer;
		bRet = TRUE;
	}
	return bRet;
}


spTHprocList::spTHprocList()
{
}

spTHprocList::~spTHprocList()
{
}

BOOL spTHprocList::AddEntry( PPROCESSENTRY32 pProcEntry )
{
	BOOL bRet = FALSE;
	PPROCESSENTRY32 pNewEntry = NULL;
	
	pNewEntry = new PROCESSENTRY32;
	if( pNewEntry && pProcEntry )
	{
		memcpy( pNewEntry, pProcEntry, sizeof( PROCESSENTRY32 ) );	///fill the new content
		bRet = AddLink( (PVOID)pNewEntry );							///add new content to list
		if( !bRet )
			delete pNewEntry;
	}
	
	return bRet;
}

BOOL spTHprocList::DelAllEntry()
{
	BOOL bRet = TRUE;
	
	while( pTailLink )
	{
		bRet = DelTailEntry();
	}
	
	return bRet;
}

BOOL spTHprocList::DelTailEntry()
{
	BOOL bRet = FALSE;
	PPROCESSENTRY32 pTailEntry = NULL;
	
	if( pTailLink )
	{
		pTailEntry = (PPROCESSENTRY32)pTailLink->pContent;
		if( DeleteLink( pTailLink ) )
		{
			delete pTailEntry;
		}
	}
	
	return bRet;
}

BOOL spTHprocList::SnapIt()
{
	BOOL bRet = FALSE;

    HANDLE hSnapShot = INVALID_HANDLE_VALUE;
	DWORD dwSnapFlag = 0;
	
	dwSnapFlag = ( TH32CS_SNAPPROCESS | 0x40000000 );
	hSnapShot = CreateToolhelp32Snapshot( dwSnapFlag, 0 );	


	if( INVALID_HANDLE_VALUE == hSnapShot )
	{
		bRet = FALSE;
	}
	else
	{
		PROCESSENTRY32 ProcEntery;
	
		ProcEntery.dwSize = sizeof( PROCESSENTRY32 );
		ProcEntery.cntUsage = 1;
		ProcEntery.th32ModuleID = 0;
		ProcEntery.th32ParentProcessID = 0;
		
		bRet = Process32First( hSnapShot, &ProcEntery );
		
		if( bRet )
		{
			DelAllEntry();	///clear all
			///store the process info
			AddEntry( &ProcEntery );

			while( Process32Next( hSnapShot, &ProcEntery ) )
			{
				///store the module info
				AddEntry( &ProcEntery );
			}
		}
		else
		{
			bRet = FALSE;
		}
	}

	CloseToolhelp32Snapshot( hSnapShot );	
	
	return bRet;
}

BOOL spTHprocList::ClearIt()
{
	BOOL bRet = FALSE;

	bRet = DelAllEntry();	///clear all
	
	return bRet;
}

void spTHprocList::DumpListMsg()
{
	pspLinkList pTmp = pHeadLink;
	
	while( NULL != pTmp )
	{
		NKDbgPrintfW(L"SPD@DumpListMsg Idx=%04d,Flag=0x%08x,pPre=0x%08X,pNext=0x%08X,pCont=0x%08X",
			pTmp->dwIndex,
			pTmp->dwFlag,
			pTmp->pPreLink,
			pTmp->pNextLink,
			pTmp->pContent
			);
		NKDbgPrintfW(L"\r\n", pTmp->dwIndex );
		DumpProcContentMsg( (PPROCESSENTRY32)pTmp->pContent );
		pTmp = (pspLinkList)pTmp->pNextLink;///next one
	}
}

void spTHprocList::LogListMsg()
{
	pspLinkList pTmp = pHeadLink;
	
	if( NULL != m_pLog )
		m_pLog->WriteLog( TEXT("Idx Name                      |  ProcessID | MemoryBase |Threads|DefaultHeapID|PriB|    dwFlags |  AccessKey.\r\n") );
	else
		return;
		
	while( NULL != pTmp )
	{
		LogProcContentMsg( (PPROCESSENTRY32)pTmp->pContent, pTmp->dwIndex );
		pTmp = (pspLinkList)pTmp->pNextLink;///next one
	}
	
	m_pLog->WriteLog(L"\r\n" );
}


void spTHprocList::DumpProcContentMsg( PPROCESSENTRY32 pProcCont )
{
	if( NULL != pProcCont )
	{
		///NKDbgPrintfW( TEXT("Process %d content dump.\r\n"), dwIndex );
		NKDbgPrintfW( TEXT("dwSize             =%d.\r\n"), pProcCont->dwSize );
		NKDbgPrintfW( TEXT("cntUsage           =%d.\r\n"), pProcCont->cntUsage );
		NKDbgPrintfW( TEXT("th32ProcessID      =0x%08X.\r\n"), pProcCont->th32ProcessID );
		NKDbgPrintfW( TEXT("th32DefaultHeapID  =0x%08X.\r\n"), pProcCont->th32DefaultHeapID );
		NKDbgPrintfW( TEXT("th32ModuleID       =0x%08X.\r\n"), pProcCont->th32ModuleID );
		NKDbgPrintfW( TEXT("cntThreads         =%d.\r\n"), pProcCont->cntThreads );
		NKDbgPrintfW( TEXT("th32ParentProcessID=0x%08X.\r\n"), pProcCont->th32ParentProcessID );
		NKDbgPrintfW( TEXT("pcPriClassBase     =%d.\r\n"), pProcCont->pcPriClassBase );
		NKDbgPrintfW( TEXT("dwFlags            =0x%08X(%d).\r\n"), pProcCont->dwFlags, pProcCont->dwFlags );
		NKDbgPrintfW( TEXT("szExeFile          =%s\r\n"), pProcCont->szExeFile );
		NKDbgPrintfW( TEXT("th32MemoryBase     =0x%08X.\r\n"), pProcCont->th32MemoryBase );
		NKDbgPrintfW( TEXT("th32AccessKey      =0x%08X.\r\n"), pProcCont->th32AccessKey );
				
		///NKDbgPrintfW( TEXT("\r\n") );
	}
}

void spTHprocList::LogProcContentMsg( PPROCESSENTRY32 pProcCont, DWORD dwIndex )
{
	if( NULL != pProcCont && NULL != m_pLog )
	{
		m_pLog->WriteLog( TEXT("%03d %25s | 0x%08X | 0x%08X | %5d |  0x%08X | %2d | 0x%08X | 0x%08X.\r\n"),
			dwIndex,
			pProcCont->szExeFile,
			pProcCont->th32ProcessID,
			pProcCont->th32MemoryBase,
			pProcCont->cntThreads,
			pProcCont->th32DefaultHeapID,
			pProcCont->pcPriClassBase,
			pProcCont->dwFlags,
			pProcCont->th32AccessKey
		);
	}
}


DWORD spTHprocList::GetProcCount()
{
	return GetTotalListNum();
}

PPROCESSENTRY32 spTHprocList::GetProcContent( DWORD dwIndex )
{
	PPROCESSENTRY32 pProcEntry = NULL;
	pspLinkList pList = NULL;
	
	pList = IsIndexValid( dwIndex );
	if( NULL != pList )
	{	
		pProcEntry = (PPROCESSENTRY32)pList->pContent;
	}
	
	return pProcEntry;
}

DWORD spTHprocList::FindProcIDwAddress( DWORD dwVA )
{
	DWORD dwRet = 0;
	PPROCESSENTRY32 pProcEntry = NULL;
	pspLinkList pList = pHeadLink;
	
	while( pList && !dwRet )
	{
		pProcEntry = (PPROCESSENTRY32)pList->pContent;
		if( (dwVA&0xFE000000) == pProcEntry->th32MemoryBase )
			dwRet = pProcEntry->th32ProcessID;
		else
			pList = (pspLinkList)pList->pNextLink;	///next one
	}
	
	return dwRet;
}

DWORD spTHprocList::FindBaseAddrwProcID( DWORD dwProcID )
{
	DWORD dwRet = 0;
	PPROCESSENTRY32 pProcEntry = NULL;
	pspLinkList pList = pHeadLink;
	
	while( pList && !dwRet )
	{
		pProcEntry = (PPROCESSENTRY32)pList->pContent;
		if( dwProcID == pProcEntry->th32ProcessID )
			dwRet = pProcEntry->th32MemoryBase;
		else
			pList = (pspLinkList)pList->pNextLink;	///next one
	}
	
	return dwRet;
}

DWORD spTHprocList::FindListIndexwAddress( DWORD dwVA )
{
	DWORD dwRet = 0;
	PPROCESSENTRY32 pProcEntry = NULL;
	pspLinkList pList = pHeadLink;
	
	while( pList && !dwRet )
	{
		pProcEntry = (PPROCESSENTRY32)pList->pContent;
		if( (dwVA&0xFE000000) == pProcEntry->th32MemoryBase )
			dwRet = pList->dwIndex;
		else
			pList = (pspLinkList)pList->pNextLink;	///next one
	}
	
	return dwRet;
}

DWORD spTHprocList::FindListIndexwProcID( DWORD dwProcID )
{
	DWORD dwRet = 0;
	PPROCESSENTRY32 pProcEntry = NULL;
	pspLinkList pList = pHeadLink;
	
	while( pList && !dwRet )
	{
		pProcEntry = (PPROCESSENTRY32)pList->pContent;
		if( dwProcID == pProcEntry->th32ProcessID )
			dwRet = pList->dwIndex;
		else
			pList = (pspLinkList)pList->pNextLink;	///next one
	}
	
	return dwRet;
}


spTHthreadList::spTHthreadList()
{
}

spTHthreadList::~spTHthreadList()
{
}

BOOL spTHthreadList::AddEntry( PTHREADENTRY32 pThreadEntry )
{
	BOOL bRet = FALSE;
	PTHREADENTRY32 pNewEntry = NULL;
	
	pNewEntry = new THREADENTRY32;
	if( pNewEntry && pThreadEntry )
	{
		memcpy( pNewEntry, pThreadEntry, sizeof( THREADENTRY32 ) );	///fill the new content
		bRet = AddLink( (PVOID)pNewEntry );							///add new content to list
		if( !bRet )
			delete pNewEntry;
	}
	
	return bRet;
}

BOOL spTHthreadList::DelAllEntry()
{
	BOOL bRet = TRUE;
	
	while( pTailLink )
	{
		bRet = DelTailEntry();
	}
	
	return bRet;
}

BOOL spTHthreadList::DelTailEntry()
{
	BOOL bRet = FALSE;
	PTHREADENTRY32 pTailEntry = NULL;
	
	if( pTailLink )
	{
		pTailEntry = (PTHREADENTRY32)pTailLink->pContent;
		if( DeleteLink( pTailLink ) )
		{
			delete pTailEntry;
		}
	}
	
	return bRet;
}

BOOL spTHthreadList::SnapIt()
{
	BOOL bRet = FALSE;

    HANDLE hSnapShot = INVALID_HANDLE_VALUE;
	DWORD dwSnapFlag = 0;
	
	dwSnapFlag = ( TH32CS_SNAPTHREAD | 0x40000000 );
	hSnapShot = CreateToolhelp32Snapshot( dwSnapFlag, 0 );	


	if( INVALID_HANDLE_VALUE == hSnapShot )
	{
		bRet = FALSE;
	}
	else
	{
		THREADENTRY32 ThreadEntry;
					
		///list each thread
		ThreadEntry.dwSize = sizeof( THREADENTRY32 );
		bRet = Thread32First( hSnapShot, &ThreadEntry );
		
		if( bRet )
		{
			DelAllEntry();	///clear all
			///store the thread info
			AddEntry( &ThreadEntry );

			while( Thread32Next( hSnapShot, &ThreadEntry ) )
			{
				///store the thread info
				AddEntry( &ThreadEntry );
			}
		}
		else
		{
			bRet = FALSE;
		}
	}

	CloseToolhelp32Snapshot( hSnapShot );	
	
	return bRet;
}

BOOL spTHthreadList::ClearIt()
{
	BOOL bRet = FALSE;

	bRet = DelAllEntry();	///clear all
	
	return bRet;
}

void spTHthreadList::DumpListMsg()
{
	pspLinkList pTmp = pHeadLink;
	
	while( NULL != pTmp )
	{
		NKDbgPrintfW(L"SPD@DumpListMsg Idx=%04d,Flag=0x%08x,pPre=0x%08X,pNext=0x%08X,pCont=0x%08X",
			pTmp->dwIndex,
			pTmp->dwFlag,
			pTmp->pPreLink,
			pTmp->pNextLink,
			pTmp->pContent
			);
		NKDbgPrintfW(L"\r\n", pTmp->dwIndex );
		DumpThreadContentMsg( (PTHREADENTRY32)pTmp->pContent );
		pTmp = (pspLinkList)pTmp->pNextLink;///next one
	}
}

void spTHthreadList::LogListMsg()
{
	pspLinkList pTmp = pHeadLink;
	
	if( NULL != m_pLog )
		m_pLog->WriteLog( TEXT("Idx   ThreadID |OwnerProcID |  CurProcID |cntUsage|BasePri|DeltaPri|  dwFlags   | AccessKey.\r\n") );
	else
		return;
		
	while( NULL != pTmp )
	{
		LogThreadContentMsg( (PTHREADENTRY32)pTmp->pContent, pTmp->dwIndex );
		pTmp = (pspLinkList)pTmp->pNextLink;///next one
	}
	
	m_pLog->WriteLog(L"\r\n" );
}

void spTHthreadList::DumpThreadContentMsg( PTHREADENTRY32 pThreadCont )
{
	if( NULL != pThreadCont )
	{
		///psLog->WriteLog( TEXT("Thread %d content dump.\r\n"), dwIndex );
		NKDbgPrintfW( TEXT("dwSize              =%d.\r\n"), pThreadCont->dwSize );
		NKDbgPrintfW( TEXT("cntUsage            =%d.\r\n"), pThreadCont->cntUsage );
		NKDbgPrintfW( TEXT("th32ThreadID        =0x%08X.\r\n"), pThreadCont->th32ThreadID );
		NKDbgPrintfW( TEXT("th32OwnerProcessID  =0x%08X.\r\n"), pThreadCont->th32OwnerProcessID );
		NKDbgPrintfW( TEXT("tpBasePri           =%d.\r\n"), pThreadCont->tpBasePri );
		NKDbgPrintfW( TEXT("tpDeltaPri          =%d.\r\n"), pThreadCont->tpDeltaPri );
		NKDbgPrintfW( TEXT("dwFlags             =0x%08X(%d).\r\n"), pThreadCont->dwFlags, pThreadCont->dwFlags );
		NKDbgPrintfW( TEXT("th32AccessKey       =0x%08X.\r\n"), pThreadCont->th32AccessKey );
		NKDbgPrintfW( TEXT("th32CurrentProcessID=0x%08X.\r\n"), pThreadCont->th32CurrentProcessID );
				
		///NKDbgPrintfW( TEXT("\r\n") );
	}
}

void spTHthreadList::LogThreadContentMsg( PTHREADENTRY32 pThreadCont, DWORD dwIndex )
{
	if( NULL != pThreadCont && NULL != m_pLog )
	{
		m_pLog->WriteLog( TEXT("%03d 0x%08X | 0x%08X | 0x%08X | %6d | %5d | %6d | 0x%08X | 0x%08X.\r\n"),
			dwIndex,
			pThreadCont->th32ThreadID,
			pThreadCont->th32OwnerProcessID,
			pThreadCont->th32CurrentProcessID,
			pThreadCont->cntUsage,
			pThreadCont->tpBasePri,
			pThreadCont->tpDeltaPri,
			pThreadCont->dwFlags,
			pThreadCont->th32AccessKey
		);
	}
}

DWORD spTHthreadList::GetThreadCount()
{
	return GetTotalListNum();
}

PTHREADENTRY32 spTHthreadList::GetThreadContent( DWORD dwIndex )
{
	PTHREADENTRY32 pThreadEntry = NULL;
	pspLinkList pList = NULL;
	
	pList = IsIndexValid( dwIndex );
	if( NULL != pList )
	{	
		pThreadEntry = (PTHREADENTRY32)pList->pContent;
	}
	
	return pThreadEntry;
}

spTHstackList::spTHstackList()
{
}

spTHstackList::~spTHstackList()
{
}

BOOL spTHstackList::AddEntry( pspHTSTACKINFO pStackEntry )
{
	BOOL bRet = FALSE;
	pspHTSTACKINFO pNewEntry = NULL;
	
	pNewEntry = new spHTSTACKINFO;
	if( pNewEntry && pStackEntry )
	{
		memcpy( pNewEntry, pStackEntry, sizeof( spHTSTACKINFO ) );	///fill the new content
		bRet = AddLink( (PVOID)pNewEntry );							///add new content to list
		if( !bRet )
			delete pNewEntry;
	}
	
	return bRet;
}

BOOL spTHstackList::DelAllEntry()
{
	BOOL bRet = TRUE;
	
	while( pTailLink )
	{
		bRet = DelTailEntry();
	}
	
	return bRet;
}

BOOL spTHstackList::DelTailEntry()
{
	BOOL bRet = FALSE;
	pspHTSTACKINFO pTailEntry = NULL;
	
	if( pTailLink )
	{
		pTailEntry = (pspHTSTACKINFO)pTailLink->pContent;
		if( DeleteLink( pTailLink ) )
		{
			delete pTailEntry;
		}
	}
	
	return bRet;
}

BOOL spTHstackList::SnapIt( spTHthreadList *pThreadList )
{
	BOOL bRet = FALSE;
	DWORD dwIndex = 1;
	HANDLE hThread = NULL;

	if( NULL == pThreadList )
	{
		bRet = FALSE;
	}
	else
	{
		PTHREADENTRY32 pThreadEntry = NULL;
		pspHTSTACKINFO pNewStackInfo = NULL;
		
		DelAllEntry();	///clear all
		
		while( dwIndex <= pThreadList->GetThreadCount() )
		{
			pThreadEntry = pThreadList->GetThreadContent( dwIndex );
			if( NULL == pThreadEntry )
			{
				bRet = FALSE;
				break;
			}
			
			hThread = (HANDLE)pThreadEntry->th32ThreadID;
			///pNewStackInfo = new spHTSTACKINFO;
			bRet = GetStackInfowThreadID( hThread );
			
			dwIndex++;
		}
	}

	return bRet;
}

BOOL spTHstackList::ClearIt()
{
	BOOL bRet = FALSE;

	bRet = DelAllEntry();	///clear all
	
	return bRet;
}

void spTHstackList::DumpListMsg()
{
	pspLinkList pTmp = pHeadLink;
	
	while( NULL != pTmp )
	{
		NKDbgPrintfW(L"SPD@DumpListMsg Idx=%04d,Flag=0x%08x,pPre=0x%08X,pNext=0x%08X,pCont=0x%08X",
			pTmp->dwIndex,
			pTmp->dwFlag,
			pTmp->pPreLink,
			pTmp->pNextLink,
			pTmp->pContent
			);
		NKDbgPrintfW(L"\r\n", pTmp->dwIndex );
		DumpStackContentMsg( (pspHTSTACKINFO)pTmp->pContent );
		pTmp = (pspLinkList)pTmp->pNextLink;///next one
	}
}

void spTHstackList::LogListMsg()
{
	pspLinkList pTmp = pHeadLink;
	
	if( NULL != m_pLog )
		m_pLog->WriteLog( TEXT("Idx  dwStackAddr|dwReturnAddr.\r\n") );
	else
		return;
		
	while( NULL != pTmp )
	{
		LogStackContentMsg( (pspHTSTACKINFO)pTmp->pContent, pTmp->dwIndex );
		pTmp = (pspLinkList)pTmp->pNextLink;///next one
	}
	
	m_pLog->WriteLog(L"\r\n" );
}

void spTHstackList::DumpStackContentMsg( pspHTSTACKINFO pStackCont )
{
	if( NULL != pStackCont )
	{
		///psLog->WriteLog( TEXT("Thread %d content dump.\r\n"), dwIndex );
		NKDbgPrintfW( TEXT("dwStackAddr         =0x%08X.\r\n"), pStackCont->dwStackAddr );
		NKDbgPrintfW( TEXT("dwReturnAddr        =0x%08X.\r\n"), pStackCont->dwReturnAddr );
	}
}

void spTHstackList::LogStackContentMsg( pspHTSTACKINFO pStackCont, DWORD dwIndex )
{
	if( NULL != pStackCont && NULL != m_pLog )
	{
		m_pLog->WriteLog( TEXT("%04d 0x%08X | 0x%08X .\r\n"),
			dwIndex,
			pStackCont->dwStackAddr,
			pStackCont->dwReturnAddr
		);
	}
}

DWORD spTHstackList::GetStackCount()
{
	return GetTotalListNum();
}

pspHTSTACKINFO spTHstackList::GetStackContent( DWORD dwIndex )
{
	pspHTSTACKINFO pStackEntry = NULL;
	pspLinkList pList = NULL;
	
	pList = IsIndexValid( dwIndex );
	if( NULL != pList )
	{	
		pStackEntry = (pspHTSTACKINFO)pList->pContent;
	}
	
	return pStackEntry;
}

BOOL spTHstackList::GetStackInfowThreadID( HANDLE hThread )
{
	BOOL bRet = FALSE;
	
	if( hThread )
	{
		CallSnapshotEx lpFrames[STACK_MAX_FRAMES];
		DWORD dwCnt, dwSkip = 0;
		DWORD dwLastError;
		DWORD dwTemp = 0;
		spHTSTACKINFO StackInfoTemp;

		dwLastError = GetLastError();
	
		do{
			dwCnt = GetThreadCallStack( hThread, STACK_MAX_FRAMES, lpFrames, 2, dwSkip );
			if( dwCnt )
			{
				// Process the frames retrieved so far
				///MyProcessFrames (dwCnt, lpFrames);
				while( dwTemp < dwCnt )
				{
					StackInfoTemp.dwStackAddr = lpFrames[dwTemp].dwFramePtr;
					StackInfoTemp.dwReturnAddr = lpFrames[dwTemp].dwReturnAddr;
					StackInfoTemp.dwThreadID = (DWORD)hThread;
					///psLog->WriteLog( TEXT("Stack:ReAddress=0x%08X\r\n"), lpFrames[dwTemp].dwReturnAddr );
					///psLog->WriteLog( TEXT("Stack:PtrAddress=0x%08X\r\n"), lpFrames[dwTemp].dwFramePtr );
					AddEntry( &StackInfoTemp );
					dwTemp++;
				}
				dwSkip += dwCnt;
			}
			dwTemp = 0;
		}while (STACK_MAX_FRAMES == dwCnt);

		SetLastError(dwLastError);

	}
	
	return bRet;
}


spTHmodList::spTHmodList()
{
}

spTHmodList::~spTHmodList()
{
}

BOOL spTHmodList::AddEntry( PMODULEENTRY32 pModEntry )
{
	BOOL bRet = FALSE;
	PMODULEENTRY32 pNewEntry = NULL;
	
	pNewEntry = new MODULEENTRY32;
	if( pNewEntry && pModEntry )
	{
		memcpy( pNewEntry, pModEntry, sizeof( MODULEENTRY32 ) );	///fill the new content
		bRet = AddLink( (PVOID)pNewEntry );							///add new content to list
		if( !bRet )
			delete pNewEntry;
		else
		{
			///if( m_dwCurSnapProcess )
				CheckDllsSummary( pNewEntry, &DllsSummayInProc );
		}
	}
	
	return bRet;
}

BOOL spTHmodList::DelAllEntry()
{
	BOOL bRet = TRUE;
	
	while( pTailLink )
	{
		bRet = DelTailEntry();
	}
	
	CheckDllsSummary( (PMODULEENTRY32)0xFFFFFFFF, &DllsSummayInProc );
	
	return bRet;
}

BOOL spTHmodList::DelTailEntry()
{
	BOOL bRet = FALSE;
	PMODULEENTRY32 pTailEntry = NULL;
	
	if( pTailLink )
	{
		pTailEntry = (PMODULEENTRY32)pTailLink->pContent;
		if( DeleteLink( pTailLink ) )
		{
			delete pTailEntry;
		}
	}
	
	return bRet;
}

BOOL spTHmodList::SnapIt()
{
	BOOL bRet = FALSE;

	bRet = SnapIt( 0 );
	
	return bRet;
}

BOOL spTHmodList::SnapIt( DWORD dwProcID )
{
	BOOL bRet = FALSE;

    HANDLE hSnapShot = INVALID_HANDLE_VALUE;
	DWORD dwSnapFlag = 0;
	
	if( 0 == dwProcID )
		dwSnapFlag = ( TH32CS_SNAPMODULE | TH32CS_GETALLMODS | 0x40000000 );
	else
		dwSnapFlag = ( TH32CS_SNAPMODULE | 0x40000000 );
		
	hSnapShot = CreateToolhelp32Snapshot( dwSnapFlag, dwProcID );	
	m_dwCurSnapProcess = dwProcID;

	if( INVALID_HANDLE_VALUE == hSnapShot )
	{
		bRet = FALSE;
	}
	else
	{
		MODULEENTRY32 ModEntry;
					
		///list each module in the process
		ModEntry.dwSize = sizeof( MODULEENTRY32 );
		bRet = Module32First( hSnapShot, &ModEntry );
		
		if( bRet )
		{
			DelAllEntry();	///clear all
			///store the module info
			AddEntry( &ModEntry );

			while( Module32Next( hSnapShot, &ModEntry ) )
			{
				///store the module info
				AddEntry( &ModEntry );
			}
		}
		else
		{
			bRet = FALSE;
		}
	}
	
	CloseToolhelp32Snapshot( hSnapShot );
	
	return bRet;
}


BOOL spTHmodList::ClearIt()
{
	BOOL bRet = FALSE;

	bRet = DelAllEntry();	///clear all
	
	return bRet;
}

void spTHmodList::DumpListMsg()
{
	pspLinkList pTmp = pHeadLink;
	
	while( NULL != pTmp )
	{
		NKDbgPrintfW(L"SPD@DumpListMsg Idx=%04d,Flag=0x%08x,pPre=0x%08X,pNext=0x%08X,pCont=0x%08X",
			pTmp->dwIndex,
			pTmp->dwFlag,
			pTmp->pPreLink,
			pTmp->pNextLink,
			pTmp->pContent
			);
		NKDbgPrintfW(L"\r\n" );
		DumpModContentMsg( (PMODULEENTRY32)pTmp->pContent );
		pTmp = (pspLinkList)pTmp->pNextLink;///next one
	}
}

void spTHmodList::LogListMsg()
{
	pspLinkList pTmp = pHeadLink;
	
	if( NULL != m_pLog )
		m_pLog->WriteLog( TEXT("000               Module name |  module ID |  base addr |     size | Gcnt | Pcnt \r\n") );
	else
		return;
		
	while( NULL != pTmp )
	{
		LogModContentMsg( (PMODULEENTRY32)pTmp->pContent, pTmp->dwIndex );
		pTmp = (pspLinkList)pTmp->pNextLink;///next one
	}
	
	m_pLog->WriteLog(L"\r\n" );
}

void spTHmodList::DumpModContentMsg( PMODULEENTRY32 pModCont )
{
	if( NULL != pModCont )
	{
		DWORD dwModAddr = (DWORD)pModCont->modBaseAddr;
		
		///NKDbgPrintfW( TEXT("Module %d content dump.\r\n"), dwIndex );
		NKDbgPrintfW( TEXT("dwSize             =%d.\r\n"), pModCont->dwSize );
		NKDbgPrintfW( TEXT("th32ModuleID       =0x%08X.\r\n"), pModCont->th32ModuleID );
		NKDbgPrintfW( TEXT("th32ProcessID      =0x%08X.\r\n"), pModCont->th32ProcessID );
		NKDbgPrintfW( TEXT("GlblcntUsage       =%d.\r\n"), pModCont->GlblcntUsage );
		NKDbgPrintfW( TEXT("ProccntUsage       =%d.\r\n"), pModCont->ProccntUsage );
		if( dwModAddr >= 0x00000000 && dwModAddr < 0x02000000 )
			NKDbgPrintfW( TEXT("modBaseAddr        =0x%08X.---------->%s\r\n"), pModCont->modBaseAddr, TEXT("Slot 0") );
		else if( dwModAddr >= 0x02000000 && dwModAddr < 0x04000000 )
			NKDbgPrintfW( TEXT("modBaseAddr        =0x%08X.---------->%s\r\n"), pModCont->modBaseAddr, TEXT("Slot XIP") );
		else if( dwModAddr >= 0x7E000000 && dwModAddr < 0x80000000 )
			NKDbgPrintfW( TEXT("modBaseAddr        =0x%08X.---------->%s\r\n"), pModCont->modBaseAddr, TEXT("Slot resource") );
		else if( dwModAddr >= 0xC2000000 && dwModAddr < 0xC4000000 )
			NKDbgPrintfW( TEXT("modBaseAddr        =0x%08X.---------->%s\r\n"), pModCont->modBaseAddr, TEXT("Slot NK") );
		else
			NKDbgPrintfW( TEXT("modBaseAddr        =0x%08X.---------->\r\n"), pModCont->modBaseAddr );
			///NKDbgPrintfW( TEXT("modBaseAddr        =0x%08X.\r\n"), pModEntry->modBaseAddr );
		NKDbgPrintfW( TEXT("modBaseSize        =0x%08X(%d).\r\n"), pModCont->modBaseSize, pModCont->modBaseSize );
		NKDbgPrintfW( TEXT("hModule            =0x%08X.\r\n"), pModCont->hModule );
		NKDbgPrintfW( TEXT("szModule           =%s\r\n"), pModCont->szModule );
		NKDbgPrintfW( TEXT("szExePath          =%s\r\n"), pModCont->szExePath );
		NKDbgPrintfW( TEXT("dwFlags            =0x%08X(%d).\r\n"), pModCont->dwFlags, pModCont->dwFlags );
				
		NKDbgPrintfW( TEXT("\r\n") );
	}
	
}

void spTHmodList::LogModContentMsg( PMODULEENTRY32 pModCont, DWORD dwIndex )
{
	if( NULL != pModCont && NULL != m_pLog )
	{
		DWORD dwModAddr = (DWORD)pModCont->modBaseAddr;

		m_pLog->WriteLog( TEXT("%03d %25s | 0x%08X | 0x%08x | %8d | %4d | %4d .\r\n"),
			dwIndex,
			pModCont->szModule,
			pModCont->th32ModuleID,
			dwModAddr,
			pModCont->modBaseSize,
			pModCont->GlblcntUsage,
			pModCont->ProccntUsage
		);	
	}
}


DWORD spTHmodList::GetModCount()
{
	return GetTotalListNum();
}

PMODULEENTRY32 spTHmodList::GetModContent( DWORD dwIndex )
{
	PMODULEENTRY32 pModEntry = NULL;
	pspLinkList pList = NULL;
	
	pList = IsIndexValid( dwIndex );
	if( NULL != pList )
	{	
		pModEntry = (PMODULEENTRY32)pList->pContent;
	}
	
	return pModEntry;
}

DWORD spTHmodList::GetSnapProcID()
{
	return m_dwCurSnapProcess;
}

BOOL spTHmodList::CheckDllsSummary( PMODULEENTRY32 pThisModule, pspPROCDLLSSUMMARY pSummary )
{
	BOOL bRet = FALSE;
	
	if( pSummary && pThisModule )
	{
		if( 0xFFFFFFFF == (DWORD)pThisModule )
		{
			//clear it
			memset( pSummary, 0, sizeof(spPROCDLLSSUMMARY) );
			pSummary->dwDllLow = 0x02000000;
			pSummary->dwDllLowNK = 0x02000000;
			pSummary->dwDllLowXIP = 0x02000000;
			pSummary->dwDllLowRES = 0x02000000;
			pSummary->dwDllLow0 = 0x02000000;
			pSummary->dwDllLowElse = 0x02000000;
		}
		else
		if( NULL != pThisModule  )
		{
			DWORD dwDllAddrs = (DWORD)pThisModule->modBaseAddr;

			if( IsThisSlot( dwDllAddrs, NK_SLOT|ZERO_SLOT|SLOT_2_32 ) )
				CheckDllLowBigUpdate( &(pSummary->dwDllLow), &(pSummary->dwDllBig), pThisModule );		
			if( !IsThisSlot( dwDllAddrs, NK_SLOT|ZERO_SLOT|XIP_SLOT|RES_SLOT|SLOT_2_32) )
				CheckDllLowBigUpdate( &(pSummary->dwDllLowElse), &(pSummary->dwDllBigElse), pThisModule );
			if( IsThisSlot( dwDllAddrs, NK_SLOT ) )
				CheckDllLowBigUpdate( &(pSummary->dwDllLowNK), &(pSummary->dwDllBigNK), pThisModule );
			if( IsThisSlot( dwDllAddrs, ZERO_SLOT ) )
				CheckDllLowBigUpdate( &(pSummary->dwDllLow0), &(pSummary->dwDllBig0), pThisModule );
			if( IsThisSlot( dwDllAddrs, XIP_SLOT ) )
				CheckDllLowBigUpdate( &(pSummary->dwDllLowXIP), &(pSummary->dwDllBigXIP), pThisModule );
			if( IsThisSlot( dwDllAddrs, RES_SLOT ) )
				CheckDllLowBigUpdate( &(pSummary->dwDllLowRES), &(pSummary->dwDllBigRES), pThisModule );

		}

	
		bRet = TRUE;
	}
	
	return bRet;
}

BOOL spTHmodList::IsThisSlot( DWORD dwAddress, DWORD dwFlag )
{
	BOOL bRet = TRUE;
	
	do{
		if( dwFlag&NK_SLOT )
			if( (dwAddress >= 0xC2000000) && (dwAddress < 0xC4000000) )
				break;;
		if( dwFlag&ZERO_SLOT )
			if( (dwAddress >= 0x00000000) && (dwAddress < 0x02000000) )
				break;;
		if( dwFlag&XIP_SLOT )
			if( (dwAddress >= 0x02000000) && (dwAddress < 0x04000000) )
				break;;
		if( dwFlag&RES_SLOT )
			if( (dwAddress >= 0x7E000000) && (dwAddress < 0x80000000) )
				break;;
		if( dwFlag&SLOT_2_32 )
			if( (dwAddress >= 0x04000000) && (dwAddress < 0x42000000) )
				break;;
				
		bRet = FALSE;	///not found
	}
	while(0);
	
	return bRet;
}

BOOL spTHmodList::CheckDllLowBigUpdate( PDWORD pLow, PDWORD pBig, PMODULEENTRY32 pModEntry )
{
	BOOL bRet = TRUE;
	
	if( pLow && pBig && pModEntry )
	{
		DWORD dwDllAddrs = (DWORD)pModEntry->modBaseAddr;
		
		if( *pLow > (dwDllAddrs&0x01FFFFFF) )
			*pLow = (dwDllAddrs&0x01FFFFFF);
		if( *pBig < pModEntry->modBaseSize )
			*pBig = pModEntry->modBaseSize;	
	}
	else
		bRet = FALSE;
	
	return bRet;
}


spTHheaplistList::spTHheaplistList()
{
}

spTHheaplistList::~spTHheaplistList()
{
}

BOOL spTHheaplistList::AddEntry( PHEAPLIST32 pHeapListEntry )
{
	BOOL bRet = FALSE;
	PHEAPLIST32 pNewEntry = NULL;
	
	pNewEntry = new HEAPLIST32;
	if( pNewEntry && pHeapListEntry )
	{
		memcpy( pNewEntry, pHeapListEntry, sizeof( HEAPLIST32 ) );	///fill the new content
		bRet = AddLink( (PVOID)pNewEntry );							///add new content to list
		if( !bRet )
			delete pNewEntry;
	}
	
	return bRet;
}

BOOL spTHheaplistList::DelAllEntry()
{
	BOOL bRet = TRUE;
	
	while( pTailLink )
	{
		bRet = DelTailEntry();
	}
	
	return bRet;
}

BOOL spTHheaplistList::DelTailEntry()
{
	BOOL bRet = FALSE;
	PHEAPLIST32 pTailEntry = NULL;
	
	if( pTailLink )
	{
		pTailEntry = (PHEAPLIST32)pTailLink->pContent;
		if( DeleteLink( pTailLink ) )
		{
			delete pTailEntry;
		}
	}
	
	return bRet;
}

BOOL spTHheaplistList::SnapIt()
{
	BOOL bRet = FALSE;

	bRet = SnapIt( (DWORD)0 );
	
	return bRet;
}

BOOL spTHheaplistList::SnapIt( DWORD dwProcID )
{
	BOOL bRet = FALSE;

    HANDLE hSnapShot = INVALID_HANDLE_VALUE;
	DWORD dwSnapFlag = 0;
	
	dwSnapFlag = ( TH32CS_SNAPHEAPLIST );
		
	hSnapShot = CreateToolhelp32Snapshot( dwSnapFlag, dwProcID );	
	m_dwCurSnapProcess = dwProcID;

	if( INVALID_HANDLE_VALUE == hSnapShot )
	{
		bRet = FALSE;
	}
	else
	{
		HEAPLIST32 HeaplistEntry;
					
		///list each module in the process
		HeaplistEntry.dwSize = sizeof( HEAPLIST32 );
		bRet = Heap32ListFirst( hSnapShot, &HeaplistEntry );
		
		if( bRet )
		{
			DelAllEntry();	///clear all
			///store the heaplist info
			AddEntry( &HeaplistEntry );

			while( Heap32ListNext( hSnapShot, &HeaplistEntry ) )
			{
				///store the module info
				AddEntry( &HeaplistEntry );
			}
		}
		else
		{
			bRet = FALSE;
		}
	}
	
	CloseToolhelp32Snapshot( hSnapShot );
	
	return bRet;
}

BOOL spTHheaplistList::SnapIt( HANDLE hSnapShot )
{
	BOOL bRet = FALSE;

	if( INVALID_HANDLE_VALUE == hSnapShot )
	{
		bRet = FALSE;
	}
	else
	{
		HEAPLIST32 HeaplistEntry;
					
		///list each module in the process
		HeaplistEntry.dwSize = sizeof( HEAPLIST32 );
		bRet = Heap32ListFirst( hSnapShot, &HeaplistEntry );
		
		if( bRet )
		{
			DelAllEntry();	///clear all
			///store the heaplist info
			AddEntry( &HeaplistEntry );

			while( Heap32ListNext( hSnapShot, &HeaplistEntry ) )
			{
				///store the module info
				AddEntry( &HeaplistEntry );
			}
		}
		else
		{
			bRet = FALSE;
		}
	}
	
	return bRet;
}



BOOL spTHheaplistList::ClearIt()
{
	BOOL bRet = FALSE;

	bRet = DelAllEntry();	///clear all
	
	return bRet;
}

void spTHheaplistList::DumpListMsg()
{
	pspLinkList pTmp = pHeadLink;
	
	while( NULL != pTmp )
	{
		NKDbgPrintfW(L"SPD@DumpListMsg Idx=%04d,Flag=0x%08x,pPre=0x%08X,pNext=0x%08X,pCont=0x%08X",
			pTmp->dwIndex,
			pTmp->dwFlag,
			pTmp->pPreLink,
			pTmp->pNextLink,
			pTmp->pContent
			);
		NKDbgPrintfW(L"\r\n", pTmp->dwIndex );
		DumpHeapListContentMsg( (PHEAPLIST32)pTmp->pContent );
		pTmp = (pspLinkList)pTmp->pNextLink;///next one
	}
}

void spTHheaplistList::LogListMsg()
{
	pspLinkList pTmp = pHeadLink;
	
	if( NULL != m_pLog )
		m_pLog->WriteLog( TEXT("Idx     HeapID |    ProcID |  dwFlags .\r\n") );
	else
		return;
		
	while( NULL != pTmp )
	{
		LogHeapListContentMsg( (PHEAPLIST32)pTmp->pContent, pTmp->dwIndex );
		pTmp = (pspLinkList)pTmp->pNextLink;///next one
	}
	
	m_pLog->WriteLog(L"\r\n" );
}

void spTHheaplistList::DumpHeapListContentMsg( PHEAPLIST32 pHeapListCont )
{
	if( NULL != pHeapListCont )
	{
		NKDbgPrintfW( TEXT("dwSize             =%d.\r\n"), pHeapListCont->dwSize );
		NKDbgPrintfW( TEXT("th32HeapID         =0x%08X.\r\n"), pHeapListCont->th32HeapID );
		NKDbgPrintfW( TEXT("th32ProcessID      =0x%08X.\r\n"), pHeapListCont->th32ProcessID );
		NKDbgPrintfW( TEXT("dwFlags            =0x%08X(%d).\r\n"), pHeapListCont->dwFlags, pHeapListCont->dwFlags );

		NKDbgPrintfW( TEXT("\r\n") );
	}
}

void spTHheaplistList::LogHeapListContentMsg( PHEAPLIST32 pHeapListCont, DWORD dwIndex )
{
	if( NULL != pHeapListCont && NULL != m_pLog )
	{
		m_pLog->WriteLog( TEXT("%03d 0x%08X | 0x%08X | 0x%08X.\r\n"),
			dwIndex,
			pHeapListCont->th32HeapID,
			pHeapListCont->th32ProcessID,
			pHeapListCont->dwFlags
		);
	}
}

DWORD spTHheaplistList::GetHeapListCount()
{
	return GetTotalListNum();
}

PHEAPLIST32 spTHheaplistList::GetHeapListContent( DWORD dwIndex )
{
	PHEAPLIST32 pHeapListEntry = NULL;
	pspLinkList pList = NULL;
	
	pList = IsIndexValid( dwIndex );
	if( NULL != pList )
	{	
		pHeapListEntry = (PHEAPLIST32)pList->pContent;
	}
	
	return pHeapListEntry;
}

DWORD spTHheaplistList::GetSnapProcID()
{
	return m_dwCurSnapProcess;
}



spTHheapList::spTHheapList()
{
}

spTHheapList::~spTHheapList()
{
}

BOOL spTHheapList::AddEntry( PHEAPENTRY32 pHeapEntry )
{
	BOOL bRet = FALSE;
	PHEAPENTRY32 pNewEntry = NULL;
	
	pNewEntry = new HEAPENTRY32;
	if( pNewEntry && pHeapEntry )
	{
		memcpy( pNewEntry, pHeapEntry, sizeof( HEAPLIST32 ) );	///fill the new content
		bRet = AddLink( (PVOID)pNewEntry );							///add new content to list
		if( !bRet )
			delete pNewEntry;
	}
	
	return bRet;
}

BOOL spTHheapList::DelAllEntry()
{
	BOOL bRet = TRUE;
	
	while( pTailLink )
	{
		bRet = DelTailEntry();
	}
	
	return bRet;
}

BOOL spTHheapList::DelTailEntry()
{
	BOOL bRet = FALSE;
	PHEAPENTRY32 pTailEntry = NULL;
	
	if( pTailLink )
	{
		pTailEntry = (PHEAPENTRY32)pTailLink->pContent;
		if( DeleteLink( pTailLink ) )
		{
			delete pTailEntry;
		}
	}
	
	return bRet;
}

BOOL spTHheapList::SnapIt()
{
	BOOL bRet = FALSE;

	bRet = SnapIt( 0 );
	
	return bRet;
}

BOOL spTHheapList::SnapIt( DWORD dwProcID )
{
	BOOL bRet = FALSE;

    HANDLE hSnapShot = INVALID_HANDLE_VALUE;
	DWORD dwSnapFlag = 0;
	
	dwSnapFlag = ( TH32CS_SNAPHEAPLIST );
		
	hSnapShot = CreateToolhelp32Snapshot( dwSnapFlag, dwProcID );	
	m_dwCurSnapProcess = dwProcID;
	///NKDbgPrintfW(L"MSG:line %d m_dwCurSnapProcess=0x%08x.\r\n", __LINE__, m_dwCurSnapProcess );

	if( INVALID_HANDLE_VALUE == hSnapShot )
	{
		bRet = FALSE;
		///NKDbgPrintfW(L"MSG:line %d.\r\n", __LINE__ );
	}
	else
	{
		///NKDbgPrintfW(L"MSG:line %d.\r\n", __LINE__ );
		m_HeapListList.ClearIt();
		///NKDbgPrintfW(L"MSG:line %d.\r\n", __LINE__ );
		bRet = m_HeapListList.SnapIt( hSnapShot );
		///NKDbgPrintfW(L"MSG:line %d.\r\n", __LINE__ );
	}
	
	if( bRet )
	{
		HEAPENTRY32 HeapEntery;
		PHEAPLIST32 pThisList = NULL;
		DWORD dwIdx = 1;
		DWORD dwThisHeapID = 0;
		DWORD dwOwnProcID = 0;
	
		HeapEntery.dwSize = sizeof( HEAPENTRY32 );
		
		///NKDbgPrintfW(L"MSG:line %d.\r\n", __LINE__ );
		DelAllEntry();	///clear all
		///NKDbgPrintfW(L"MSG:line %d.\r\n", __LINE__ );
		
		while( dwIdx <= m_HeapListList.GetHeapListCount() )
		{
			pThisList = m_HeapListList.GetHeapListContent( dwIdx );
			if( NULL == pThisList )
			{
				bRet = FALSE;
				break;
			}	

			dwThisHeapID = pThisList->th32HeapID;
			dwOwnProcID = pThisList->th32ProcessID;
				
			bRet = Heap32First( hSnapShot, &HeapEntery, dwOwnProcID, dwThisHeapID );
		
			if( bRet )
			{
				///store the heap info
				AddEntry( &HeapEntery );

				while( Heap32Next( hSnapShot, &HeapEntery ) )
				{
					///store the module info
					AddEntry( &HeapEntery );
				}
			}
			else
			{
				bRet = FALSE;
				break;
			}
			
			dwIdx++;
		}
	}
	///NKDbgPrintfW(L"MSG:line %d.\r\n", __LINE__ );
	CloseToolhelp32Snapshot( hSnapShot );
	///NKDbgPrintfW(L"MSG:line %d.\r\n", __LINE__ );
	
	return bRet;
}


BOOL spTHheapList::ClearIt()
{
	BOOL bRet = FALSE;

	bRet = DelAllEntry();	///clear all
	
	return bRet;
}

void spTHheapList::DumpListMsg()
{
	pspLinkList pTmp = pHeadLink;
	
	while( NULL != pTmp )
	{
		NKDbgPrintfW(L"SPD@DumpListMsg Idx=%04d,Flag=0x%08x,pPre=0x%08X,pNext=0x%08X,pCont=0x%08X",
			pTmp->dwIndex,
			pTmp->dwFlag,
			pTmp->pPreLink,
			pTmp->pNextLink,
			pTmp->pContent
			);
		NKDbgPrintfW(L"\r\n" );
		DumpHeapContentMsg( (PHEAPENTRY32)pTmp->pContent );
		pTmp = (pspLinkList)pTmp->pNextLink;///next one
	}
}

void spTHheapList::LogListMsg()
{
	pspLinkList pTmp = pHeadLink;
	
	if( NULL != m_pLog )
		m_pLog->WriteLog( TEXT("Idx     HeapID |    ProcID  |  dwAddress |  Size  |   hHandle  | LockCount | dwFlags .\r\n") );
	else
		return;
		
	while( NULL != pTmp )
	{
		LogHeapContentMsg( (PHEAPENTRY32)pTmp->pContent, pTmp->dwIndex );
		pTmp = (pspLinkList)pTmp->pNextLink;///next one
	}
	
	m_pLog->WriteLog(L"\r\n" );
}

void spTHheapList::DumpHeapContentMsg( PHEAPENTRY32 pHeapCont )
{
	if( NULL != pHeapCont )
	{
		NKDbgPrintfW( TEXT("dwSize             =%d.\r\n"), pHeapCont->dwSize );
		NKDbgPrintfW( TEXT("hHandle            =0x%08X.\r\n"), pHeapCont->hHandle );
		NKDbgPrintfW( TEXT("dwAddress          =0x%08X.\r\n"), pHeapCont->dwAddress );
		NKDbgPrintfW( TEXT("dwBlockSize        =%d.\r\n"), pHeapCont->dwBlockSize );
		NKDbgPrintfW( TEXT("dwFlags            =0x%08X(%d).\r\n"), pHeapCont->dwFlags, pHeapCont->dwFlags );
		NKDbgPrintfW( TEXT("dwLockCount        =0x%08X.\r\n"), pHeapCont->dwLockCount );
		NKDbgPrintfW( TEXT("th32ProcessID      =0x%08X.\r\n"), pHeapCont->th32ProcessID );
		NKDbgPrintfW( TEXT("th32HeapID         =0x%08X.\r\n"), pHeapCont->th32HeapID );
		
		NKDbgPrintfW( TEXT("\r\n") );
	}
}

void spTHheapList::LogHeapContentMsg( PHEAPENTRY32 pHeapCont, DWORD dwIndex )
{
	if( NULL != pHeapCont && NULL != m_pLog )
	{
		m_pLog->WriteLog( TEXT("%03d 0x%08X | 0x%08X | 0x%08X | %6d | 0x%08X | %9d | 0x%08X.\r\n"),
			dwIndex,
			pHeapCont->th32HeapID,
			pHeapCont->th32ProcessID,
			pHeapCont->dwAddress,
			pHeapCont->dwBlockSize,
			pHeapCont->hHandle,
			pHeapCont->dwLockCount,
			pHeapCont->dwFlags
		);
	}
}

DWORD spTHheapList::GetHeapCount()
{
	return GetTotalListNum();
}

PHEAPENTRY32 spTHheapList::GetHeapContent( DWORD dwIndex )
{
	PHEAPENTRY32 pHeapEntry = NULL;
	pspLinkList pList = NULL;
	
	pList = IsIndexValid( dwIndex );
	if( NULL != pList )
	{	
		pHeapEntry = (PHEAPENTRY32)pList->pContent;
	}
	
	return pHeapEntry;
}

DWORD spTHheapList::GetSnapProcID()
{
	return m_dwCurSnapProcess;
}




spToolHelp::spToolHelp()
{
	Init();
}

spToolHelp::~spToolHelp()
{
	DeInit();
}

BOOL spToolHelp::Init()
{
	BOOL bRet = FALSE;
	
	PriInit();
	
	return bRet;
}

BOOL spToolHelp::DeInit()
{
	BOOL bRet = FALSE;
	
	ClearIt();
	
	return bRet;
}

BOOL spToolHelp::PriInit()
{
	BOOL bRet = FALSE;
	
	m_pLog = NULL;
	
	return bRet;
}

BOOL spToolHelp::ClearIt()
{
	BOOL bRet = FALSE;
	
	m_ProcessListStore.ClearIt();
	m_ThreadListStore.ClearIt();
	m_StackListStore.ClearIt();
	m_ModuleListStore.ClearIt();
	m_HeapListStore.ClearIt();
	
	return bRet;
}

CHAR spToolHelp::CheckVMblockSign( DWORD dwCheckAddrBase, DWORD dwCheckAddrRange )
{
    CHAR cSign = '~';
    DWORD dwCheckAddrTop = dwCheckAddrBase + dwCheckAddrRange;
	DWORD dwItemAddrBase = 0;
	DWORD dwItemAddrTop = 0;
	DWORD dwSlotBase = 0;
	DWORD dwSlotTop = 0;
	DWORD dwIndex = 1;
	BOOL bSignNotFound = TRUE;

	dwSlotBase = m_ProcessListStore.FindBaseAddrwProcID( m_ModuleListStore.GetSnapProcID() );
	dwSlotTop = dwSlotBase + 0x02000000;

	if( 0 != m_ModuleListStore.GetSnapProcID() )
	{
		if( bSignNotFound )
		{
		    dwIndex = 1;
		    PMODULEENTRY32 pMod = NULL;
		    while( dwIndex <= m_ModuleListStore.GetModCount() )
		    {
		    	pMod = m_ModuleListStore.GetModContent( dwIndex );
		    	if( pMod )
		    	{	
    		    	dwItemAddrBase = (DWORD)pMod->modBaseAddr;
	    	    	dwItemAddrTop = dwItemAddrBase + pMod->modBaseSize;

    		    	if( 
                        ((dwItemAddrBase >= dwSlotBase) && (dwItemAddrTop < dwSlotTop)) ||
						((dwItemAddrBase >= 0) && (dwItemAddrTop < 0x02000000))
                    )
                    {
                        dwItemAddrBase = dwItemAddrBase&0x01FFFFFF;
                        dwItemAddrTop = dwItemAddrTop&0x01FFFFFF;
                        if(
	    				    ((dwItemAddrBase >= dwCheckAddrBase) && (dwItemAddrBase < dwCheckAddrTop)) ||
	    				    ((dwItemAddrTop >= dwCheckAddrBase) && (dwItemAddrTop < dwCheckAddrTop)) ||
	    				    ((dwItemAddrBase <= dwCheckAddrBase) && (dwItemAddrTop >= dwCheckAddrTop))
                        )
                        {
                            cSign = 'D';
                            bSignNotFound = FALSE;
                            break;
                        }
                    }
		    	}
            
		    	dwIndex++;
		    }
		}

		if( bSignNotFound )
		{
		    dwIndex = 1;
		    PHEAPENTRY32 pHeap = NULL;
		    while( dwIndex <= m_HeapListStore.GetHeapCount() )
		    {
		    	pHeap = m_HeapListStore.GetHeapContent( dwIndex );
		    	if( pHeap )
		    	{	
    		    	dwItemAddrBase = pHeap->dwAddress;
	    	    	dwItemAddrTop = dwItemAddrBase + pHeap->dwBlockSize;

    		    	if( 
                        ((dwItemAddrBase >= dwSlotBase) && (dwItemAddrTop < dwSlotTop)) ||
						((dwItemAddrBase >= 0) && (dwItemAddrTop < 0x02000000))
                    )
                    {
                        dwItemAddrBase = dwItemAddrBase&0x01FFFFFF;
                        dwItemAddrTop = dwItemAddrTop&0x01FFFFFF;
                        if(
	    				    ((dwItemAddrBase >= dwCheckAddrBase) && (dwItemAddrBase < dwCheckAddrTop)) ||
	    				    ((dwItemAddrTop >= dwCheckAddrBase) && (dwItemAddrTop < dwCheckAddrTop)) ||
	    				    ((dwItemAddrBase <= dwCheckAddrBase) && (dwItemAddrTop >= dwCheckAddrTop))
                        )
                        {
                            cSign = 'H';
                            bSignNotFound = FALSE;
                            break;
                        }
                    }
		    	}
            
		    	dwIndex++;
		    }
		}

		if( bSignNotFound )
		{
		    dwIndex = 1;
		    pspHTSTACKINFO pStack = NULL;
		    while( dwIndex <= m_StackListStore.GetStackCount() )
		    {
		    	pStack = m_StackListStore.GetStackContent( dwIndex );
		    	if( pStack )
		    	{	
    		    	dwItemAddrBase = pStack->dwStackAddr;
	    	    	dwItemAddrTop = dwItemAddrBase + (64*1024);

    		    	if( 
                        ((dwItemAddrBase >= dwSlotBase) && (dwItemAddrTop < dwSlotTop)) ||
						((dwItemAddrBase >= 0) && (dwItemAddrTop < 0x02000000))
                    )
                    {
                        dwItemAddrBase = dwItemAddrBase&0x01FFFFFF;
                        dwItemAddrTop = dwItemAddrTop&0x01FFFFFF;
                        if(
	    				    ((dwItemAddrBase >= dwCheckAddrBase) && (dwItemAddrBase < dwCheckAddrTop)) ||
	    				    ((dwItemAddrTop >= dwCheckAddrBase) && (dwItemAddrTop < dwCheckAddrTop)) ||
	    				    ((dwItemAddrBase <= dwCheckAddrBase) && (dwItemAddrTop >= dwCheckAddrTop))
                        )
                        {
                            cSign = 'S';
                            bSignNotFound = FALSE;
                            break;
                        }
                    }
		    	}
            
		    	dwIndex++;
		    }
		}


	}

	if( 0 == m_ModuleListStore.GetSnapProcID() )
	{
	}
    
    return cSign;
}

BOOL spToolHelp::LogPrepare()
{
	BOOL bRet = FALSE;
	
	if( NULL == m_pLog )
		m_pLog = new spLog();	

	if( m_pLog )
	{
		///setup log file
		m_pLog->SetupFileName( TEXT("\\iPAQ File Store\\spMemlog.log") );
		m_pLog->SetupTitle( TEXT(" Dump info in all processes !!!") );
		///m_pLog->SetupTitle( TEXT(" Dump all processes !!!") );
		///m_pLog->SetupTitle( TEXT(" Dump all processes !!!") );
		bRet = m_pLog->LogEnable();
	}
	else
	{
		bRet = FALSE;
		NKDbgPrintfW( TEXT("SPD@spToolHelp::LogPrepare fail\r\n") );
	}	
		
	return bRet;
}

BOOL spToolHelp::LogEnding()
{
	BOOL bRet = FALSE;

	if( m_pLog )
	{
		if( m_pLog->LogDone() )
		{
			delete m_pLog;
			m_pLog = NULL;
			bRet = TRUE;
		}	
	}	
	
	return bRet;
}


void spToolHelp::DumpInfoAllProcess()
{
	m_ProcessListStore.SnapIt();
	m_ModuleListStore.SnapIt();
	m_ThreadListStore.SnapIt();
	m_StackListStore.SnapIt( &m_ThreadListStore );
	
	m_ProcessListStore.DumpListMsg();
	m_ModuleListStore.DumpListMsg();
	m_ThreadListStore.DumpListMsg();
	m_StackListStore.DumpListMsg();	
}

void spToolHelp::LogInfoAllProcess()
{
	m_ProcessListStore.SnapIt();
	m_ModuleListStore.SnapIt();
	m_ThreadListStore.SnapIt();
	m_StackListStore.SnapIt( &m_ThreadListStore );
	
	if( LogPrepare() )
	{
		m_ProcessListStore.SetupLogPointer( m_pLog );
		m_pLog->WriteLog( TEXT(" ===Dump all processes !!!===\r\n") );
		m_ProcessListStore.LogListMsg();

		m_ModuleListStore.SetupLogPointer( m_pLog );
		m_pLog->WriteLog( TEXT(" ===Dump all modules in all processes !!!===\r\n") );
		m_ModuleListStore.LogListMsg();

		m_ThreadListStore.SetupLogPointer( m_pLog );
		m_pLog->WriteLog( TEXT(" ===Dump Threads in all processes !!!===\r\n") );
		m_ThreadListStore.LogListMsg();

		m_StackListStore.SetupLogPointer( m_pLog );
		///m_pLog->WriteLog( TEXT(" ===Dump stack in all processes !!!===\r\n") );
		///m_StackListStore.LogListMsg();
		
		m_pLog->WriteLog( TEXT(" ===Dump Virtual memory Map for module allocation !!!===\r\n") );
		LogVMmap( (ZERO_SLOT|SLOT_2_32) );
		m_pLog->WriteLog( TEXT(" ===Dump Virtual memory Map for XIP module allocation !!!===\r\n") );
		LogVMmap( (XIP_SLOT) );
		m_pLog->WriteLog( TEXT(" ===Dump Virtual memory Map for RES module allocation !!!===\r\n") );
		LogVMmap( (RES_SLOT) );
		
		LogEnding();
	}
}

void spToolHelp::LogInfoProcess( DWORD dwProcID )
{
}

void spToolHelp::LogInfoSlot( DWORD dwSlotSet )
{
	DWORD dwOffset = 0;
	DWORD dwSlotBase = 0;
	DWORD dwProcID = 0;
	
	while( dwOffset < 32 ) 
	{
		if( 0 == (dwSlotSet&(0x1<<dwOffset)) )
		{
			dwOffset++;
			continue;
		}

		if( 0 == dwOffset || 1 == dwOffset )
		{
			dwOffset++;
			continue;
		}

		dwSlotBase = (dwOffset*2)<<24;
		///dwSlotBase = 0x08000000;
		NKDbgPrintfW(L"MSG:line %d dwOffset=%d.\r\n", __LINE__, dwOffset );
		NKDbgPrintfW(L"MSG:line %d dwSlotBase=0x%08x.\r\n", __LINE__, dwSlotBase );
		
		m_ProcessListStore.SnapIt();
		dwProcID = m_ProcessListStore.FindProcIDwAddress( dwSlotBase );
		
		if( 0 == dwProcID )
		{
			dwOffset++;
			continue;
		}		
		
		m_ThreadListStore.SnapIt();
		m_StackListStore.SnapIt( &m_ThreadListStore );

		m_ModuleListStore.SnapIt( dwProcID );
		m_HeapListStore.SnapIt( dwProcID );
	
		if( LogPrepare() )
		{
			m_pLog->WriteLog( TEXT(" ===ProcessID-0x%08X %s !!!===\r\n"), dwProcID, m_ProcessListStore.GetProcContent( m_ProcessListStore.FindListIndexwProcID( dwProcID ) )->szExeFile );
			
			m_ProcessListStore.SetupLogPointer( m_pLog );
			///m_pLog->WriteLog( TEXT(" ===Dump all processes !!!===\r\n") );
			///m_ProcessListStore.LogListMsg();

			m_ThreadListStore.SetupLogPointer( m_pLog );
			///m_pLog->WriteLog( TEXT(" ===Dump Threads in all processes !!!===\r\n") );
			///m_ThreadListStore.LogListMsg();

			m_StackListStore.SetupLogPointer( m_pLog );
			///m_pLog->WriteLog( TEXT(" ===Dump stack in all processes !!!===\r\n") );
			///m_StackListStore.LogListMsg();

			m_ModuleListStore.SetupLogPointer( m_pLog );
			m_pLog->WriteLog( TEXT(" ===Dump all modules in this processes !!!===\r\n") );
			m_ModuleListStore.LogListMsg();


			LogVMmap( (ZERO_SLOT|SLOT_2_32) );
		
			LogEnding();
		}	

		dwOffset++;
	}
	
	NKDbgPrintfW(L"MSG:line %d LogInfoSlot done =%d.\r\n", __LINE__, dwOffset );
}


BOOL spToolHelp::LogVMmap( DWORD dwSlotFlag )
{
	BOOL bRet = FALSE;
	
	if( m_pLog )
	{
		int dwCount = 511;
		DWORD dwLineBaseAddr;
		DWORD MapMarkRet[16];
		BOOL bFirstSignDone = FALSE;
		
		m_pLog->WriteLog( TEXT("\r\n") );
		m_pLog->WriteLog( TEXT("Process ID = 0x%08X\r\n"), m_ModuleListStore.GetSnapProcID() );
		m_pLog->WriteLog( TEXT("VM map table: 4kB for each char.\r\n") );
		m_pLog->WriteLog( TEXT("VM ADDrs:################.\r\n") );
		m_pLog->WriteLog( TEXT("02000000:                .\r\n") );
		
		while( dwCount >= 0 )
		{
			memset( MapMarkRet, 0, (4*16) );
			dwLineBaseAddr = (dwCount<<16);
			m_pLog->WriteLog( TEXT("%08X:%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c"),
				dwLineBaseAddr,
				CheckVMblock( dwLineBaseAddr+0x0000, 0x1000, dwSlotFlag, &MapMarkRet[0] ),
				CheckVMblock( dwLineBaseAddr+0x1000, 0x1000, dwSlotFlag, &MapMarkRet[1] ),
				CheckVMblock( dwLineBaseAddr+0x2000, 0x1000, dwSlotFlag, &MapMarkRet[2] ),
				CheckVMblock( dwLineBaseAddr+0x3000, 0x1000, dwSlotFlag, &MapMarkRet[3] ),
				CheckVMblock( dwLineBaseAddr+0x4000, 0x1000, dwSlotFlag, &MapMarkRet[4] ),
				CheckVMblock( dwLineBaseAddr+0x5000, 0x1000, dwSlotFlag, &MapMarkRet[5] ),
				CheckVMblock( dwLineBaseAddr+0x6000, 0x1000, dwSlotFlag, &MapMarkRet[6] ),
				CheckVMblock( dwLineBaseAddr+0x7000, 0x1000, dwSlotFlag, &MapMarkRet[7] ),
				CheckVMblock( dwLineBaseAddr+0x8000, 0x1000, dwSlotFlag, &MapMarkRet[8] ),
				CheckVMblock( dwLineBaseAddr+0x9000, 0x1000, dwSlotFlag, &MapMarkRet[9] ),
				CheckVMblock( dwLineBaseAddr+0xA000, 0x1000, dwSlotFlag, &MapMarkRet[10] ),
				CheckVMblock( dwLineBaseAddr+0xB000, 0x1000, dwSlotFlag, &MapMarkRet[11] ),
				CheckVMblock( dwLineBaseAddr+0xC000, 0x1000, dwSlotFlag, &MapMarkRet[12] ),
				CheckVMblock( dwLineBaseAddr+0xD000, 0x1000, dwSlotFlag, &MapMarkRet[13] ),
				CheckVMblock( dwLineBaseAddr+0xE000, 0x1000, dwSlotFlag, &MapMarkRet[14] ),
				CheckVMblock( dwLineBaseAddr+0xF000, 0x1000, dwSlotFlag, &MapMarkRet[15] )
			);

			if( 1 )
			{
				DWORD dwTemp = 0;
				DWORD i, j;
				
				for( i = 0; i < 16; i++ )
				{
					dwTemp = MapMarkRet[i];
					if( 0 != (dwTemp&VMMAPMARK_IDX_MASK) )
					{
						if( !bFirstSignDone )
						{
							m_pLog->WriteLog( TEXT(" ---> ") );
							bFirstSignDone = TRUE;
						}
						if( dwTemp&(VMMAPMARK_IDX_HEAP<<VMMAPMARK_IDX_OFFSET) )	///it's a heap
							m_pLog->WriteLog( TEXT("0x%08X + "), m_HeapListStore.GetHeapContent( (dwTemp&VMMAPMARK_LIST_MASK) )->th32HeapID );
							///m_pLog->WriteLog( TEXT("0x%08X + "), m_HeapListStore.GetHeapContent( (dwTemp&VMMAPMARK_LIST_MASK) )->th32ProcessID );
						else
						if( dwTemp&(VMMAPMARK_IDX_STACK<<VMMAPMARK_IDX_OFFSET) )	///it's a stack
						{
							pspHTSTACKINFO pStackInfo = NULL;
							pStackInfo = m_StackListStore.GetStackContent( (dwTemp&VMMAPMARK_LIST_MASK) );
							m_pLog->WriteLog( TEXT("0x%08X(0x%08X) + "), pStackInfo->dwThreadID, pStackInfo->dwReturnAddr );
						}	
						else
							m_pLog->WriteLog( TEXT("%s + "), m_ModuleListStore.GetModContent( (dwTemp&VMMAPMARK_LIST_MASK) )->szModule );

						for( j = i+1; j < 16; j++ )
						{
							if( MapMarkRet[i] == MapMarkRet[j] )
								MapMarkRet[j] = 0;
						}
						
						MapMarkRet[i] = 0;	///clear it
					}
				}
				bFirstSignDone = FALSE;
			}

			
			m_pLog->WriteLog( TEXT("\r\n") );
			dwCount--;
		}
	}
	
	return bRet;
}

CHAR spToolHelp::CheckVMblock( DWORD dwCheckAddr, DWORD dwBlockSize, DWORD dwSlotFlag, PDWORD pMapMarkRet )
{
    CHAR cSign = '~';
    BOOL bNotFound = TRUE;
	DWORD dwIndex = 1;
    DWORD dwCheckBase = dwCheckAddr;
    DWORD dwCheckTop = (dwCheckAddr+dwBlockSize);

    ///if( 0 == m_ModuleListStore.GetSnapProcID() )
	if( 1 )
    {
        PMODULEENTRY32 pModEntry = NULL;
		DWORD dwModBase = 0;
		DWORD dwModTop = 0;
	    
		dwIndex = 1;
		///scan if any module in this address
        while( dwIndex <= m_ModuleListStore.GetModCount() )
        {
            pModEntry = m_ModuleListStore.GetModContent( dwIndex );
            if( pModEntry )
            {
			    dwModBase = (DWORD)pModEntry->modBaseAddr;
			    dwModTop = dwModBase+pModEntry->modBaseSize;
			    
			    if( IsThisSlot( dwModBase, dwSlotFlag ) )
			    {    
    				if( 
	    				(((dwModBase&0x01FFFFFF) >= dwCheckBase) && ((dwModBase&0x01FFFFFF) < dwCheckTop)) ||
	    				(((dwModTop&0x01FFFFFF) >= dwCheckBase) && ((dwModTop&0x01FFFFFF) < dwCheckTop)) ||
	    				(((dwModBase&0x01FFFFFF) <= dwCheckBase) && ((dwModTop&0x01FFFFFF) >= dwCheckTop))
	    	    	)
	    			{
	    			    if( 0 != (dwSlotFlag&XIP_SLOT) )
	    			    {
	    			        cSign = 'X';
							*pMapMarkRet = MAKEMAPMARKRET( VMMAPMARK_IDX_XIP, dwIndex );
	    			    }
	    			    else
	    			    if( 0 != (dwSlotFlag&RES_SLOT) )
	    			    {
	    			        cSign = 'R';
							*pMapMarkRet = MAKEMAPMARKRET( VMMAPMARK_IDX_RES, dwIndex );
	    			    }
	    			    else
	    			    {            
                            cSign = 'D';
							*pMapMarkRet = MAKEMAPMARKRET( VMMAPMARK_IDX_MODULE, dwIndex );
                        }    
                        bNotFound = FALSE;
                        break;
                    }
                }
            }
            dwIndex++;
        }
    }
    
    if( 0 != m_ModuleListStore.GetSnapProcID() && bNotFound )
    {
		if( bNotFound )
		{
			DWORD dwHeapBase;
			DWORD dwHeapTop;
			PHEAPENTRY32 pHeap = NULL;
		
			dwIndex = 1;
		
			while( dwIndex <= m_HeapListStore.GetHeapCount() )
			{
				pHeap = m_HeapListStore.GetHeapContent( dwIndex );
				if( NULL == pHeap )
				{
					dwIndex++;
					continue;
				}
					
				dwHeapBase = pHeap->dwAddress;
				dwHeapTop = dwHeapBase+(pHeap->dwBlockSize);
			
				if( 
					IsThisSlot( dwHeapBase, dwSlotFlag )
				)
				{
					if( 
						(((dwHeapBase&0x01FFFFFF) >= dwCheckBase) && ((dwHeapBase&0x01FFFFFF) < dwCheckTop)) ||
						(((dwHeapTop&0x01FFFFFF) >= dwCheckBase) && ((dwHeapTop&0x01FFFFFF) < dwCheckTop)) ||
						(((dwHeapBase&0x01FFFFFF) <= dwCheckBase) && ((dwHeapTop&0x01FFFFFF) >= dwCheckTop))
					)
					{
						if(1)
						{
							cSign = 'H';
							*pMapMarkRet = MAKEMAPMARKRET( VMMAPMARK_IDX_HEAP, dwIndex );
						}
						bNotFound = FALSE;
						break;
					}
					else
					{
						*pMapMarkRet = 0;
					}
				}
				dwIndex++;
			}
		}

		if( bNotFound )
		{
			pspHTSTACKINFO pStack = NULL;
			DWORD dwStackAddrInSlot = 0;
			DWORD dwSlotBase = 0;
		
			dwIndex = 1;
			
		
			while( dwIndex <= m_StackListStore.GetStackCount() )
			{
				pStack = m_StackListStore.GetStackContent( dwIndex );
				if( NULL == pStack )
				{
					dwIndex++;
					continue;
				}
				
				dwSlotBase = m_ProcessListStore.FindBaseAddrwProcID( m_ModuleListStore.GetSnapProcID() );
				
				if( 
					(0 == dwSlotBase) || 
					pStack->dwStackAddr > (dwSlotBase+0x02000000) ||
					pStack->dwStackAddr < (dwSlotBase)
				)
				{
					dwIndex++;
					continue;
				}
				
				dwStackAddrInSlot = (pStack->dwStackAddr)&0x01FFFFFF;
				
				if( 
					dwStackAddrInSlot >= dwCheckBase &&
					dwStackAddrInSlot < dwCheckTop
				)
				{
					cSign = 'S';
					bNotFound = FALSE;
					*pMapMarkRet = MAKEMAPMARKRET( VMMAPMARK_IDX_STACK, dwIndex );
					break;
				}
				else
				{
					*pMapMarkRet = 0;
				}
				dwIndex++;
			}
		}

	}
    
    return cSign;
}

BOOL spToolHelp::IsThisSlot( DWORD dwAddress, DWORD dwFlag )
{
	BOOL bRet = TRUE;
	
	do{
		if( dwFlag&NK_SLOT )
			if( (dwAddress >= 0xC2000000) && (dwAddress < 0xC4000000) )
				break;;
		if( dwFlag&ZERO_SLOT )
			if( (dwAddress >= 0x00000000) && (dwAddress < 0x02000000) )
				break;;
		if( dwFlag&XIP_SLOT )
			if( (dwAddress >= 0x02000000) && (dwAddress < 0x04000000) )
				break;;
		if( dwFlag&RES_SLOT )
			if( (dwAddress >= 0x7E000000) && (dwAddress < 0x80000000) )
				break;;
		if( dwFlag&SLOT_2_32 )
			if( (dwAddress >= 0x04000000) && (dwAddress < 0x42000000) )
				break;;
				
		bRet = FALSE;	///not found
	}
	while(0);
	
	return bRet;
}