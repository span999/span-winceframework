// TestClass.h : interface of the Test class
//
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
/// WARNING !!! you should not change any code in this file !!!
/// for modification, please create your own class base on this.
/// or contact with development center for detail discussion. Thanks!
/////////////////////////////////////////////////////////////////////////////


#ifndef __TESTCLASS_H__
#define __TESTCLASS_H__



/////////////////////////////////////////////////////////////////////////////
/// define

/// messaeg level 
#define		dINIT		0x00000001
#define		dWARN		0x00000010
#define		dINFO		0x00000100
#define		dDBUG		0x00001000
#define		dFAIL		0x00010000
#define		dCLAS		0x00100000

///#define 	MESSAGELEVEL	( dINIT | dWARN | dINFO | dDBUG | dFAIL | dCLAS )


#define		DEFAULT_SHOWAREA_HEIGHT			22
#define		DEFAULT_SHOWLINE_HEIGHT			10

/////////////////////////////////////////////////////////////////////////////
/// enum define

///enum for test item status
typedef enum
{
    _TestItemDisable  = 0,		///	test item not been inited
    _TestItemEnable,			///	test item has been inited
    _TestItemActive,			///	test item has been inited and wait run command
    _TestItemPause,				///	test item is pause
///    _TestItemStop,				///	test item is stop
    _TestItemRuning				///	test item is running
}
enumTestItemStatus;

///enum for test stage
typedef enum
{
    _TestStagePreEVT  = 0,		///	test in preEVT stage
    _TestStageEVT,				///	test in EVT stage
    _TestStageDVT,				///	test in DVT stage
    _TestStagePVT,				///	test in PVT stage
    _TestStageMP				///	test in MP stage
}
enumTestStage;


/////////////////////////////////////////////////////////////////////////////
/// SLogClass 
/* 
SLogClass()
*/

class SLogClass
{
/// Function Member
	/// Overrides
public:
protected:

	/// Implementation 	/// interface
public:
	~SLogClass(){ DeInit(); }
	SLogClass(){ Init(); }
	BOOL SetTitle( LPWSTR lpszTitleString );
	BOOL SetInfo( LPWSTR lpszInfoString );
	BOOL SetLogName( LPWSTR lpszLogNameString );
	BOOL SetSeparator( LPWSTR lpszSeparatorString );	
	BOOL EnableLog( BOOL bSet );
	BOOL WriteLog( BOOL bON, LPWSTR lpszWriteString, ... );	
	BOOL WriteLog( LPWSTR lpszWriteString, ... );
	BOOL SaveLog();
	
	/// Implementation 	/// Operations
protected:
	void TakeCS(){ EnterCriticalSection(&m_csLogAccess); }
	void ReleaseCS(){ LeaveCriticalSection(&m_csLogAccess); }
	BOOL Init();
	BOOL DeInit();
	BOOL IsLogReady();
	BOOL LogCurrentTime();
	BOOL privateWriteLog( LPWSTR lpszWriteString, ... );
	

/// Data Member
public:
protected:
	TCHAR szTempString[256];
	TCHAR szTestTitle[128];
	TCHAR szTestInfo[256];
	TCHAR szLogName[128];
	TCHAR szSeparator[128];
	CRITICAL_SECTION  m_csLogAccess;
	BOOL m_bLogEnable;
	HANDLE m_hLogFile;

};


/////////////////////////////////////////////////////////////////////////////
/// SShowMsgClass 
/* 
SShowMsgClass()
*/

class SShowMsgClass
{
/// Function Member
	/// Overrides
public:
protected:

	/// Implementation 	/// interface
public:
	~SShowMsgClass(){ DeInit(); }
	SShowMsgClass(){ Init(); }
	SShowMsgClass( HWND hMainWnd, RECT rtSetArea ){ Init( hMainWnd, rtSetArea ); }
	DWORD SetDebugLevel( DWORD dwLevel ){ m_dwDebugLevel = dwLevel; return m_dwDebugLevel; }
	BOOL SetFrameWindowHwnd( HWND hMainWnd );
	BOOL SetShowAreaRect( RECT rtSetArea );
	///BOOL MsgOut( BOOL bON, LPWSTR lpszWriteString, ... );
	BOOL MsgOut( DWORD dwFlag, LPWSTR lpszWriteString, ... );
	BOOL MsgOut( LPWSTR lpszWriteString, ... );
	void SetLineHeight( DWORD dwHeight ){ m_dwLineHeight = dwHeight; }
	
	/// Implementation 	/// Operations
protected:
	void TakeCS(){ EnterCriticalSection(&m_csShowLine); }
	void ReleaseCS(){ LeaveCriticalSection(&m_csShowLine); }
	BOOL DeInit();	
	BOOL Init();
	BOOL Init( HWND hMainWnd, RECT rtSetArea );
	BOOL PrepareHdc();
	RECT GetCurrentLineRect();

/// Data Member
public:
protected:
	DWORD m_dwDebugLevel;			///Debug level index	
	HWND m_hWndFrameWindow;			///Main windows handle that we show msg
	RECT m_rtShowArea;				///Area define that we show msg
	HDC m_hdc;
	DWORD m_dwLineIndex;
	DWORD m_dwLineMax;
	DWORD m_dwLineHeight;
	CRITICAL_SECTION m_csShowLine;
};


/////////////////////////////////////////////////////////////////////////////
/// STestItemClass
/* 
STestItemClass()
*/

class STestItemClass
{
/// Function Member
	/// Overrides
public:
protected:
	virtual void TestContent( DWORD dwIndex );
	virtual BOOL _InitLog();										///init log file parameter
	virtual BOOL InitMsg();										///init Message

	/// Implementation 	/// interface
public:
	~STestItemClass(){;}
	STestItemClass(){;}
	STestItemClass( UINT uiID );
	DWORD SetDebugLevel( DWORD dwLevel );
	///void SetFrameWindowHwnd( HWND hWndIn ){ hWndFrameWindow = hWndIn; }
	void SetFrameWindowHwnd( HWND hWndIn );
	///void SetItemWindowRect( RECT rtIn ){ rtItemWindow = rtIn; }
	void SetItemWindowRect( RECT rtIn );
	UINT GetTestItemID(){ return uiTestItemID; }
	void SetTestItemLoop( DWORD dwIn ){ dwTestItemLoop = dwIn; }
	void SetTestItemStage( enumTestStage stage ){ dwTestItemStage = stage; }
	void SetTestItemLoopInterval( DWORD dwInterval ){ dwTestItemLoopInterval = dwInterval; }
	enumTestItemStatus GetTestItemStatus(){ return TestItemStatus; }
	void SetTestItemActive(){ TestItemStatus = _TestItemActive; }
	void SetTestItemDeActive(){ TestItemStatus = _TestItemEnable; }
	BOOL Run();
	BOOL Stop();
	DWORD GetTestItemTotalTime(){ return (TestItemStopTick - TestItemStartTick); }
	DWORD GetTestItemCurrentTime(){ return (GetTickCount() - TestItemStartTick); }
	///BOOL EnableLog( BOOL bEnable ){ bEnableLog = bEnable; return bEnableLog; }
	BOOL EnableLog( BOOL bEnable ){ bEnableLog = bEnable; pLog->EnableLog(bEnableLog); return bEnableLog; }
	BOOL SaveLog();	
	
	/// Implementation 	/// Operations
protected:
	void Init();
	BOOL InitLog();										///init log file parameter
	BOOL  SetTestItemID( UINT uiID );
	enumTestItemStatus SetGetTestItemStatus( enumTestItemStatus newState ){ TestItemStatus = newState; return TestItemStatus; }
	static DWORD WINAPI TestItemThreadStub( LPVOID lpParam );
	DWORD TestItemThread();
	void SetTestItemStartTime( DWORD dwTick ){ TestItemStartTick = dwTick; }
	void SetTestItemStopTime( DWORD dwTick ){ TestItemStopTick = dwTick; }

	
/// Data Member
public:
protected:
	DWORD m_dwDebugLevel;			///Debug level index
	TCHAR szTempString[256];
	UINT uiTestItemID;
	HWND hWndFrameWindow;
	RECT rtItemWindow;
	enumTestItemStatus TestItemStatus;
	DWORD dwTestItemLoop;
	enumTestStage dwTestItemStage;
	DWORD dwTestItemLoopInterval;
	HANDLE hTestItemThread;
	HANDLE hTestItemThreadEvent;
	BOOL bTestItemThreadRun;
	DWORD TestItemStartTick;
	DWORD TestItemStopTick;
	BOOL bEnableLog;
	SLogClass *pLog;									///log file object
	SShowMsgClass *pMsg;								///Message object
};


/////////////////////////////////////////////////////////////////////////////
/// STestClass
/* 
STestClass()
*/

class STestClass
{
/// Function Member
	/// Overrides
public:
	virtual void AddTestItem( UINT uiID );

protected:	
	virtual BOOL _InitLog();										///init log file parameter
	virtual BOOL InitMsg();										///init Message		

	/// Implementation 	/// interface
public:
	~STestClass(){;}
	STestClass();
	STestClass( DWORD dwNumber, DWORD dwLoop );
	DWORD SetDebugLevel( DWORD dwLevel );
	DWORD GetTestItemNumber(){ m_dwTestItemNumber = TestItemEntryArraryIndex; return m_dwTestItemNumber; }
	UINT GetTestItemID( UINT uiIndex );
	enumTestItemStatus GetTestItemStatus( UINT uiIndex );
	enumTestItemStatus IDGetTestItemStatus( UINT uiID);
	void ActiveTestItem( UINT uiID );
	void DeActiveTestItem( UINT uiID );		
	void SetFrameWindowRect( RECT rtIn );
	void SetFrameWindowHwnd( HWND hWndIn );
	DWORD GetTestLoopNumber(){ return m_dwTestLoopNumber; }
	void SetTestLoopNumber( DWORD dwLoop );
	void SetTestStage( enumTestStage stage );
	void SetTestLoopInterval( DWORD dwInterval );
	DWORD GetTotalTestTime(){ return (m_TestStopTick - m_TestStartTick); }
	DWORD GetCurrentTestTime(){ return (GetTickCount() - m_TestStartTick); }
	BOOL EnableLog( BOOL bEnable );
	BOOL SaveLog();	
	void Run();
	void Stop();

	/// Implementation 	/// Operations
protected:
	void Init();
	void Init( DWORD dwNumber, DWORD dwLoop );	
	void InitAllItems();
	BOOL InitLog();										///init log file parameter	
	STestItemClass *STestClass::IDFindTestItem( UINT uiID );
	BOOL RunTestItem();
	BOOL RunTestItem( UINT uiID );
	BOOL StopTestItem();
	BOOL StopTestItem( UINT uiID );
	void SetTestStartTime( DWORD dwTick ){ m_TestStartTick = dwTick; }
	void SetTestStopTime( DWORD dwTick ){ m_TestStopTick = dwTick; }	
	void SetTestItemNumber( DWORD dwNumber ){ m_dwTestItemNumber = dwNumber; }

/// Data Member
public:
protected:
	DWORD m_dwDebugLevel;			///Debug level index
	DWORD m_dwTestItemNumber;		///total number of test items(1~16)
	DWORD m_dwTestLoopNumber;		///total loop for testing
	enumTestStage m_TestStage;
	DWORD m_TestLoopInterval;
	DWORD m_TestStartTick;
	DWORD m_TestStopTick;
	BOOL m_bEnableLog;
	RECT rtFrameWindow;
	HWND hWndFrameWindow;

	TCHAR szTempString[256];
	UINT TestItemEntryArraryIndex;
	STestItemClass* TestItemEntryArrary[16];
	SLogClass *pLog;									///log file object	
	SShowMsgClass *pMsg;								///Message object
};

/////////////////////////////////////////////////////////////////////////////


#endif ///#ifndef __TESTCLASS_H__
