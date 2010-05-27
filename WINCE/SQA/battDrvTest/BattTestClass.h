// BattTestClass.h : interface of the Battery Test class
//
/////////////////////////////////////////////////////////////////////////////

#ifndef __BATTTESTCLASS_H__
#define __BATTTESTCLASS_H__

#include "TestClass.h"


typedef struct tPWRSTATREC {
    DWORD Index;
    DWORD Tick;
    SYSTEM_POWER_STATUS_EX2 syspwrstatex2;
} PWRSTATREC, *PPWRSTATREC;


/////////////////////////////////////////////////////////////////////////////
/// STestItemAPI
/*
STestItemAPI()
*/


class STestItemAPI : public STestItemClass
{
/// Function Member
	/// Overrides
public:
protected:
	virtual void TestContent( DWORD dwIndex );
	virtual BOOL _InitLog();
	
	/// Implementation 	/// interface
public:
	~STestItemAPI(){;}
	STestItemAPI(){;}
	STestItemAPI( UINT uiID ){ Init(); SetTestItemID( uiID ); }

	/// Implementation 	/// Operations	
protected:
	BOOL FindPowerStatusDiff( PWRSTATREC *pNowPwrSat, PWRSTATREC *pPrePwrSat );

/// Data Member
public:
protected:
	PWRSTATREC PrePwrSat;
	PWRSTATREC NowPwrSat;	
};


/////////////////////////////////////////////////////////////////////////////
/// STestItemBattLevel
/*
STestItemBattLevel()
*/

class STestItemBattLevel : public STestItemClass
{
/// Function Member
	/// Overrides
public:
protected:
	virtual void TestContent( DWORD dwIndex );
	virtual BOOL _InitLog();
	
	/// Implementation 	/// interface
public:
	~STestItemBattLevel(){;}
	STestItemBattLevel(){;}
	STestItemBattLevel( UINT uiID ){ Init(); SetTestItemID( uiID ); }

	/// Implementation 	/// Operations
protected:
	BOOL FindBattLifeDiff( PWRSTATREC *pNowPwrSat, PWRSTATREC *pPrePwrSat );
	
/// Data Member
public:
protected:
	PWRSTATREC PrePwrSat;
	PWRSTATREC NowPwrSat;
};


/////////////////////////////////////////////////////////////////////////////
/// STestItemBattTemper
/*
STestItemBattTemper()
*/

class STestItemBattTemper : public STestItemClass
{
/// Function Member
	/// Overrides
public:
protected:
	virtual void TestContent( DWORD dwIndex );
	virtual BOOL _InitLog();
	
	/// Implementation 	/// interface
public:
	~STestItemBattTemper(){;}
	STestItemBattTemper(){;}
	STestItemBattTemper( UINT uiID ){ Init(); SetTestItemID( uiID ); }
	
	/// Implementation 	/// Operations
protected:
	BOOL FindBattTemperaDiff( PWRSTATREC *pNowPwrSat, PWRSTATREC *pPrePwrSat );
	
/// Data Member
public:
protected:
	PWRSTATREC PrePwrSat;
	PWRSTATREC NowPwrSat;
};



/////////////////////////////////////////////////////////////////////////////
/// SBattBasicTest
/*
SBattBasicTest()
*/


class SBattBasicTest : public STestClass
{
/// Function Member
	/// Overrides
public:
	virtual void AddTestItem( UINT uiID );
protected:
	virtual BOOL _InitLog();

	/// Implementation 	/// interface
public:
	~SBattBasicTest(){;}
///	SBattBasicTest(){ STestClass::STestClass(); }
///	SBattBasicTest( DWORD dwNumber, DWORD dwLoop ){ STestClass::STestClass( dwNumber, dwLoop ); }
	SBattBasicTest(){ Init(); }
	SBattBasicTest( DWORD dwNumber, DWORD dwLoop ){ Init( dwNumber, dwLoop ); }

	/// Implementation 	/// Operations
protected:

/// Data Member
public:
protected:

};

/////////////////////////////////////////////////////////////////////////////


#endif ///#ifndef __BATTTESTCLASS_H__
