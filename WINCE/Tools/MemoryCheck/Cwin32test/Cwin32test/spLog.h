/*
	header file for spLog class 
	spLog.cpp will be main file.
*/

#ifndef __SPLOG_H__
#define __SPLOG_H__




#define	SPLOG_STATUS_FLAG_INITED		0x00000001
#define	SPLOG_STATUS_FLAG_ENABLE		0x00000002
#define	SPLOG_STATUS_FLAG_LOGREADYE		0x00000004
#define	SPLOG_STATUS_FLAG_NEWFILE		0x00000010
#define	SPLOG_STATUS_FLAG_SAMEFILE		0x00000020
#define	SPLOG_STATUS_FLAG_TITLE_OK		0x00000100
#define	SPLOG_STATUS_FLAG_NAME_OK		0x00000200
#define	SPLOG_STATUS_FLAG_SEPARA_OK		0x00000400
#define	SPLOG_STATUS_FLAG_INFO_OK		0x00000800


/*
	how to use spLog.
	1.create spLog object.
	2.specifiy the log file name/path if you want, or it will be in root folder and named as spDefault.log.
	3.call method LogEnable()
	4.then you write to you log with method WriteLog();
	5.be careful, you need to save the log file with method 	
*/



class spLog {

///method
private:
public:
	spLog(){ Init(); }
	~spLog(){ DeInit(); }
	virtual DWORD WriteLog( LPWSTR lpszWriteString, ... );
	BOOL LogEnable( void );
	BOOL LogDisable( void );
	BOOL IsLogReady(){ return (m_dwLogStatus&SPLOG_STATUS_FLAG_LOGREADYE)?TRUE:FALSE; }
	BOOL IsLogEnable(){ return (m_dwLogStatus&SPLOG_STATUS_FLAG_ENABLE)?TRUE:FALSE; }
	BOOL SetupTitle( LPWSTR lpszTitleString );
	BOOL SetupFileName( LPWSTR lpszFileNameString );
	BOOL SetupInformation( LPWSTR lpszInfoString );
	BOOL SetupSeparator( LPWSTR lpszInfoString );
	void WriteTitle( void );
	void WriteInformation( void );
	void WriteSeparator( void );
	void WriteCurrentTime( void );
	BOOL LogDone( void );
	
protected:

	virtual BOOL Init(void);
	virtual BOOL DeInit(void);
	BOOL PriInit(void);
	BOOL LogPrepare(void);
	BOOL CheckLogFileParameter(void);
	BOOL WriteLogFileHeader(void);
	void TakeCS(){ EnterCriticalSection( &m_csLogAccess ); }
	void ReleaseCS(){ LeaveCriticalSection( &m_csLogAccess ); }

///data
private:
public:
protected:

	DWORD 					m_dwLogStatus;
	CRITICAL_SECTION  		m_csLogAccess;	///
	HANDLE 					m_hMainLogFilehandle;		///
	DWORD 					m_dwMainLogCharNum;			///
	TCHAR					m_szTempString[256];		///
	TCHAR 					m_szTitle[128];				///
	TCHAR 					m_szPathName[128];			///
	TCHAR 					m_szSeparator[128];			///
	TCHAR 					m_szInformation[256];		///

};





















#endif	///#ifndef __SPLOG_H__