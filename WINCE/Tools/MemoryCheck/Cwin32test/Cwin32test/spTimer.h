/*
	header file for spLog class 
	spLog.cpp will be main file.
*/

#ifndef __SPTIMER_H__
#define __SPTIMER_H__



class spTimer {

///method
public:
	spTimer();
	~spTimer();

	
protected:
private:
	virtual BOOL Init(void);
	virtual BOOL DeInit(void);
	BOOL PriInit(void);
	BOOL TakeCS(){ EnterCriticalSection( &m_csTimer ); }
	BOOL ReleaseCS(){ LeaveCriticalSection( &m_csTimer ); }

///data
public:
protected:
private:
	DWORD m_dwTimerStatus;
	CRITICAL_SECTION  m_csTimer;	///

};






#endif	///#ifndef __SPTIMER_H__