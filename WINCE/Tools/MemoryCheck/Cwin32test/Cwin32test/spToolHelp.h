/*
	header file for spLog class 
	spLog.cpp will be main file.
*/

#ifndef __SPTOOLHELP_H__
#define __SPTOOLHELP_H__

#include <tlhelp32.h>
#include "spLog.h"

#define	SPTOOLHELP_STATUS_FLAG_ENABLE		0x00000001

#define 	NK_SLOT			0x00000001
#define 	ZERO_SLOT		0x00000010
#define 	XIP_SLOT		0x00000100
#define 	RES_SLOT		0x00001000
#define 	SLOT_2_32		0x10000000

#define		VMMAPMARKSIGN_MODULE	'D'
#define		VMMAPMARKSIGN_XIP		'X'
#define		VMMAPMARKSIGN_RES		'R'

#define		VMMAPMARK_IDX_OFFSET	16
#define		VMMAPMARK_IDX_MASK		0xFFFF0000
#define		VMMAPMARK_IDX_MODULE	0x00000008
#define		VMMAPMARK_IDX_XIP		0x00000080
#define		VMMAPMARK_IDX_RES		0x00000800
#define		VMMAPMARK_IDX_HEAP		0x00008000
#define		VMMAPMARK_IDX_STACK		0x00004000

#define		VMMAPMARK_LIST_MASK		0x0000FFFF

#define		MAKEMAPMARKRET( x, y )	((x << VMMAPMARK_IDX_OFFSET)&0xFFFF0000)|(y & 0x0000FFFF)

/* -------------------------------------------------------------------------------------------------------------------- */

typedef struct T_LINKLIST {
	DWORD dwIndex;
	DWORD dwFlag;
	PVOID pPreLink;
	PVOID pNextLink;
	PVOID pContent;  
} spLinkList, *pspLinkList;

typedef struct T_HTSTACKINFO {
	DWORD	dwThreadID;
	DWORD	dwStackAddr;
	DWORD 	dwReturnAddr;
} spHTSTACKINFO, *pspHTSTACKINFO;

typedef struct T_PROCDLLSSUMMARY
{
	DWORD 				dwTotalDllNum;
	DWORD 				dwIndex;
	DWORD				dwDllLow;
	DWORD				dwDllBig;
	DWORD				dwDllLowNK;
	DWORD				dwDllBigNK;
	DWORD				dwDllLowXIP;
	DWORD				dwDllBigXIP;
	DWORD				dwDllLowRES;
	DWORD				dwDllBigRES;
	DWORD				dwDllLow0;
	DWORD				dwDllBig0;
	DWORD				dwDllLowElse;
	DWORD				dwDllBigElse;

} spPROCDLLSSUMMARY, *pspPROCDLLSSUMMARY;


typedef struct _CallSnapshotEx {
  DWORD dwReturnAddr;
  DWORD dwFramePtr;
  DWORD dwCurProc;
  DWORD dwParams[4];
} CallSnapshotEx;

typedef struct _CallSnapshot {
  DWORD dwReturnAddr;
} CallSnapshot;

EXTERN_C ULONG GetThreadCallStack (HANDLE hThrd, ULONG dwMaxFrames, LPVOID lpFrames, DWORD dwFlags, DWORD dwSkip);


#define STACK_MAX_FRAMES		5



/* -------------------------------------------------------------------------------------------------------------------- */

class spTHcomponentList {

///method
public:
	spTHcomponentList();
	~spTHcomponentList();
	BOOL AddLink( PVOID pNew );
	PVOID DeleteLink( PVOID pThisContent );
	PVOID DeleteLink( DWORD dwIndex );
	DWORD GetTotalListNum();
	DWORD GetCurListIndex();
	virtual void DumpListMsg();
	virtual void LogListMsg();
	BOOL SetupLogPointer( spLog *pLogPointer );
	
protected:
	virtual BOOL Init(void);
	virtual BOOL DeInit(void);
	BOOL PriInit(void);
	void TakeCS(){ EnterCriticalSection( &m_csTHcomponentList ); }
	void ReleaseCS(){ LeaveCriticalSection( &m_csTHcomponentList ); }
	void FillLinkItem( pspLinkList pItem, DWORD dwIdx, DWORD dwFlag, pspLinkList pPre, pspLinkList pNext, PVOID pContent );
	pspLinkList IspContentValid( PVOID pThisContent );
	pspLinkList IsIndexValid( DWORD dwIndex );
	BOOL DeleteLink( pspLinkList pLink );

private:

///data
public:
protected:
	DWORD m_dwTotalListNum;
	DWORD m_dwCurListIndex;
	pspLinkList pHeadLink;
	pspLinkList pTailLink;
	pspLinkList pCurLink;
	CRITICAL_SECTION  m_csTHcomponentList;	///
	spLog *m_pLog;

private:

};

/* -------------------------------------------------------------------------------------------------------------------- */

class spTHprocList : public spTHcomponentList {

///method
public:
	spTHprocList();
	~spTHprocList();

	BOOL SnapIt();
	BOOL ClearIt();
	virtual void DumpListMsg();
	virtual void LogListMsg();
	void DumpProcContentMsg( PPROCESSENTRY32 pProcCont );
	void LogProcContentMsg( PPROCESSENTRY32 pProcCont, DWORD dwIndex );

	///help function
	DWORD GetProcCount();
	PPROCESSENTRY32 GetProcContent( DWORD dwIndex );
	DWORD FindProcIDwAddress( DWORD dwVA );
	DWORD FindBaseAddrwProcID( DWORD dwProcID );
	DWORD FindListIndexwAddress( DWORD dwVA );
	DWORD FindListIndexwProcID( DWORD dwProcID );
	
protected:

	BOOL PriInit(void);
	BOOL AddEntry( PPROCESSENTRY32 pProcEntry );
	BOOL DelAllEntry();
	BOOL DelTailEntry();

private:

///data
public:
protected:

private:

};

/* -------------------------------------------------------------------------------------------------------------------- */

class spTHthreadList : public spTHcomponentList {

///method
public:
	spTHthreadList();
	~spTHthreadList();

	BOOL SnapIt();
	BOOL ClearIt();
	virtual void DumpListMsg();
	virtual void LogListMsg();
	void DumpThreadContentMsg( PTHREADENTRY32 pThreadCont );
	void LogThreadContentMsg( PTHREADENTRY32 pThreadCont, DWORD dwIndex );

	///help function
	DWORD GetThreadCount();
	PTHREADENTRY32 GetThreadContent( DWORD dwIndex );
	
protected:

	BOOL PriInit(void);
	BOOL AddEntry( PTHREADENTRY32 pThreadEntry );
	BOOL DelAllEntry();
	BOOL DelTailEntry();

private:

///data
public:
protected:

private:

};

/* -------------------------------------------------------------------------------------------------------------------- */

class spTHstackList : public spTHcomponentList {

///method
public:
	spTHstackList();
	~spTHstackList();

	BOOL SnapIt( spTHthreadList *pThreadList );
	BOOL ClearIt();
	virtual void DumpListMsg();
	virtual void LogListMsg();
	void DumpStackContentMsg( pspHTSTACKINFO pStackCont );
	void LogStackContentMsg( pspHTSTACKINFO pStackCont, DWORD dwIndex );

	///help function
	DWORD GetStackCount();
	pspHTSTACKINFO GetStackContent( DWORD dwIndex );
	
protected:

	BOOL PriInit(void);
	BOOL AddEntry( pspHTSTACKINFO pStackEntry );
	BOOL DelAllEntry();
	BOOL DelTailEntry();
	
	BOOL GetStackInfowThreadID( HANDLE hThread );
private:

///data
public:
protected:

private:

};



/* -------------------------------------------------------------------------------------------------------------------- */

class spTHmodList : public  spTHcomponentList {

///method
public:
	spTHmodList();
	~spTHmodList();

	BOOL SnapIt();
	BOOL SnapIt( DWORD dwProcID );
	BOOL ClearIt();
	virtual void DumpListMsg();
	virtual void LogListMsg();
	void DumpModContentMsg( PMODULEENTRY32 pModCont );
	void LogModContentMsg( PMODULEENTRY32 pModCont, DWORD dwIndex );
	DWORD GetMuduleDllLow(){ return DllsSummayInProc.dwDllLow; }
	
	///help function
	DWORD GetModCount();
	PMODULEENTRY32 GetModContent( DWORD dwIndex );
	DWORD GetSnapProcID();
	
protected:
	BOOL PriInit(void);
	BOOL AddEntry( PMODULEENTRY32 pModEntry );
	BOOL DelAllEntry();
	BOOL DelTailEntry();
	
	BOOL CheckDllsSummary( PMODULEENTRY32 pThisModule, pspPROCDLLSSUMMARY pSummary );
	BOOL IsThisSlot( DWORD dwAddress, DWORD dwFlag );
	BOOL CheckDllLowBigUpdate( PDWORD pLow, PDWORD pBig, PMODULEENTRY32 pModEntry );

private:

///data
public:
protected:
	DWORD m_dwCurSnapProcess;
	spPROCDLLSSUMMARY DllsSummayInProc;
private:

};

/* -------------------------------------------------------------------------------------------------------------------- */

class spTHheaplistList : public  spTHcomponentList {

///method
public:
	spTHheaplistList();
	~spTHheaplistList();

	BOOL SnapIt();
	BOOL SnapIt( DWORD dwProcID );
	BOOL SnapIt( HANDLE hSnapShot );
	BOOL ClearIt();
	virtual void DumpListMsg();
	virtual void LogListMsg();
	void DumpHeapListContentMsg( PHEAPLIST32 pHeapListCont );
	void LogHeapListContentMsg( PHEAPLIST32 pHeapListCont, DWORD dwIndex );
	
	///help function
	DWORD GetHeapListCount();
	PHEAPLIST32 GetHeapListContent( DWORD dwIndex );
	DWORD GetSnapProcID();
	
protected:
	BOOL PriInit(void);
	BOOL AddEntry( PHEAPLIST32 pHeapListEntry );
	BOOL DelAllEntry();
	BOOL DelTailEntry();

private:

///data
public:
protected:
	DWORD m_dwCurSnapProcess;
private:

};


/* -------------------------------------------------------------------------------------------------------------------- */

class spTHheapList : public  spTHcomponentList {

///method
public:
	spTHheapList();
	~spTHheapList();

	BOOL SnapIt();
	BOOL SnapIt( DWORD dwProcID );
	BOOL ClearIt();
	virtual void DumpListMsg();
	virtual void LogListMsg();
	void DumpHeapContentMsg( PHEAPENTRY32 pHeapCont );
	void LogHeapContentMsg( PHEAPENTRY32 pHeapCont, DWORD dwIndex );
	
	///help function
	DWORD GetHeapCount();
	PHEAPENTRY32 GetHeapContent( DWORD dwIndex );
	DWORD GetSnapProcID();
	
protected:
	BOOL PriInit(void);
	BOOL AddEntry( PHEAPENTRY32 pHeapEntry );
	BOOL DelAllEntry();
	BOOL DelTailEntry();

private:

///data
public:
protected:
	DWORD m_dwCurSnapProcess;
	spTHheaplistList m_HeapListList;
private:

};


/* -------------------------------------------------------------------------------------------------------------------- */

class spToolHelp {

///method
public:
	spToolHelp();
	~spToolHelp();
	BOOL ClearIt();
	
	void SnapProcess(){ m_ProcessListStore.SnapIt(); }
	void ClearSnapProcess(){ m_ProcessListStore.ClearIt(); }
	DWORD GetCurProcessCount(){ return m_ProcessListStore.GetProcCount(); }
	PPROCESSENTRY32 GetProcessContent( DWORD dwIndex ){ return m_ProcessListStore.GetProcContent( dwIndex ); }

	void SnapThread(){ m_ThreadListStore.SnapIt(); }
	void ClearSnapThread(){ m_ThreadListStore.ClearIt(); }
	DWORD GetCurThreadCount(){ return m_ThreadListStore.GetThreadCount(); }
	PTHREADENTRY32 GetThreadContent( DWORD dwIndex ){ return m_ThreadListStore.GetThreadContent( dwIndex ); }

	void SnapStack(){ m_StackListStore.SnapIt( &m_ThreadListStore ); }
	void ClearSnapStack(){ m_StackListStore.ClearIt(); }
	DWORD GetCurStackCount(){ return m_StackListStore.GetStackCount(); }
	pspHTSTACKINFO GetStackContent( DWORD dwIndex ){ return m_StackListStore.GetStackContent( dwIndex ); }

	void SnapModule(){ m_ModuleListStore.SnapIt( 0 ); }
	void SnapModule( DWORD dwProcID ){ m_ModuleListStore.SnapIt( dwProcID ); }
	void ClearSnapModule(){ m_ModuleListStore.ClearIt(); }
	DWORD GetCurModuleCount(){ return m_ModuleListStore.GetModCount(); }
	PMODULEENTRY32 GetModuleContent( DWORD dwIndex ){ return m_ModuleListStore.GetModContent( dwIndex ); }
	DWORD GetMuduleDllLow(){ return m_ModuleListStore.GetMuduleDllLow(); }

	///void SnapHeap(){ m_HeapListStore.SnapIt( 0 ); }
	void SnapHeap( DWORD dwProcID ){ m_HeapListStore.SnapIt( dwProcID ); }
	void ClearSnapHeap(){ m_HeapListStore.ClearIt(); }
	DWORD GetCurHeapCount(){ return m_HeapListStore.GetHeapCount(); }
	PHEAPENTRY32 GetHeapContent( DWORD dwIndex ){ return m_HeapListStore.GetHeapContent( dwIndex ); }

    CHAR CheckVMblockSign( DWORD dwCheckAddrBase, DWORD dwCheckAddrRange );

	void DumpInfoAllProcess();
	void LogInfoAllProcess();
	void DumpInfoProcess( DWORD dwProcID );
	void LogInfoProcess( DWORD dwProcID );
	void LogInfoSlot( DWORD dwSlotSet );
	
	BOOL LogPrepare();
	BOOL LogEnding();
	
protected:
	BOOL Init(void);
	BOOL DeInit(void);
	BOOL PriInit(void);
	BOOL TakeCS(){ EnterCriticalSection( &m_csToolHelp ); }
	BOOL ReleaseCS(){ LeaveCriticalSection( &m_csToolHelp ); }

	BOOL LogVMmap( DWORD dwSlotFlag );
	CHAR CheckVMblock( DWORD dwCheckAddr, DWORD dwBlockSize, DWORD dwSlotFlag, PDWORD pMapMarkRet );
	BOOL IsThisSlot( DWORD dwAddress, DWORD dwFlag );
private:

///data
public:
protected:
	DWORD 				m_dwToolHelpStatus;
	CRITICAL_SECTION  	m_csToolHelp;	///
	
	spTHprocList 		m_ProcessListStore;
	spTHthreadList 		m_ThreadListStore;
	spTHstackList 		m_StackListStore;
	spTHmodList 		m_ModuleListStore;
	spTHheapList 		m_HeapListStore;
	
	spLog*				m_pLog;
	
	
private:

};




















#endif	///#ifndef __SPTOOLHELP_H__