// TestClass.h : interface of the Test class
//
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
/// WARNING !!! you should not change any code in this file !!!
/// for modification, please create your own class base on this.
/// or contact with development center for detail discussion. Thanks!
/////////////////////////////////////////////////////////////////////////////


#ifndef __LOGCLASS_H__
#define __LOGCLASS_H__



/////////////////////////////////////////////////////////////////////////////
/// define

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




#endif ///#ifndef __LOGCLASS_H__
