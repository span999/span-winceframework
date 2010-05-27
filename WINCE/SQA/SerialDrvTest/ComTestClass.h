// BattTestClass.h : interface of the Battery Test class
//
/////////////////////////////////////////////////////////////////////////////

#ifndef __COMTESTCLASS_H__
#define __COMTESTCLASS_H__

#include "TestClass.h"
#include "spComXClass.h"


/////////////////////////////////////////////////////////////////////////////
/// STestItemGPS
/*
STestItemGPS()
*/


class STestItemGPS : public STestItemClass
{
/// Function Member
	/// Overrides
public:
protected:
	virtual void TestContent( DWORD dwIndex );
	virtual BOOL _InitLog();
	
	/// Implementation 	/// interface
public:
	~STestItemGPS(){;}
	STestItemGPS(){;}
	STestItemGPS( UINT uiID ){ Init(); SetTestItemID( uiID ); }
	
	/// Implementation 	/// Operations	
protected:

/// Data Member
public:
protected:
	SGPSComClass *pCom;
};





/////////////////////////////////////////////////////////////////////////////
/// SComBasicTest
/*
SComBasicTest()
*/


class SComBasicTest : public STestClass
{
/// Function Member
	/// Overrides
public:
	virtual void AddTestItem( UINT uiID );
protected:
	virtual BOOL _InitLog();

	/// Implementation 	/// interface
public:
	~SComBasicTest(){;}
	SComBasicTest(){ Init(); }
	SComBasicTest( DWORD dwNumber, DWORD dwLoop ){ Init( dwNumber, dwLoop ); }
	
	/// Implementation 	/// Operations
protected:

/// Data Member
public:
protected:
};

/////////////////////////////////////////////////////////////////////////////


#endif ///#ifndef __COMTESTCLASS_H__
