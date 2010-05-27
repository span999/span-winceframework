// BattTestClass.h : interface of the Battery Test class
//
/////////////////////////////////////////////////////////////////////////////

#ifndef __BKLTESTCLASS_H__
#define __BKLTESTCLASS_H__

#include "TestClass.h"



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
	STestItemAPI(){ hLGT = INVALID_HANDLE_VALUE; }
	STestItemAPI( UINT uiID ){ Init(); SetTestItemID( uiID ); hLGT = INVALID_HANDLE_VALUE; }
	
	/// Implementation 	/// Operations	
protected:

/// Data Member
public:
protected:
	HANDLE hLGT;
};


/////////////////////////////////////////////////////////////////////////////
/// STestItemBrightness
/*
STestItemBrightness()
*/

class STestItemBrightness : public STestItemClass
{
/// Function Member
	/// Overrides
public:
protected:
	virtual void TestContent( DWORD dwIndex );
	virtual BOOL _InitLog();
	
	/// Implementation 	/// interface
public:
	~STestItemBrightness(){ hLGT = INVALID_HANDLE_VALUE; }
	STestItemBrightness(){;}
	STestItemBrightness( UINT uiID ){ Init(); SetTestItemID( uiID ); hLGT = INVALID_HANDLE_VALUE; }
	
	/// Implementation 	/// Operations	
protected:

	
/// Data Member
public:
protected:
	HANDLE hLGT;
};


/////////////////////////////////////////////////////////////////////////////
/// STestItemOnOff
/*
STestItemOnOff()
*/

class STestItemOnOff : public STestItemClass
{
/// Function Member
	/// Overrides
public:
protected:
	virtual void TestContent( DWORD dwIndex );
	virtual BOOL _InitLog();
	
	/// Implementation 	/// interface
public:	~STestItemOnOff(){;}
	STestItemOnOff(){ hLGT = INVALID_HANDLE_VALUE; }
	STestItemOnOff( UINT uiID ){ Init(); SetTestItemID( uiID ); hLGT = INVALID_HANDLE_VALUE; }
	
	/// Implementation 	/// Operations	
protected:
	
/// Data Member
public:
protected:
	HANDLE hLGT;
};



/////////////////////////////////////////////////////////////////////////////
/// SBklBasicTest
/*
SBklBasicTest()
*/


class SBklBasicTest : public STestClass
{
/// Function Member
	/// Overrides
public:
	virtual void AddTestItem( UINT uiID );
protected:
	virtual BOOL _InitLog();

	/// Implementation 	/// interface
public:
	~SBklBasicTest(){;}
	///SBklBasicTest(){ STestClass::STestClass(); }
	///SBklBasicTest( DWORD dwNumber, DWORD dwLoop ){ STestClass::STestClass( dwNumber, dwLoop ); }
	SBklBasicTest(){ Init(); }
	SBklBasicTest( DWORD dwNumber, DWORD dwLoop ){ Init( dwNumber, dwLoop ); }
	
	/// Implementation 	/// Operations
protected:

/// Data Member
public:
protected:
};

/////////////////////////////////////////////////////////////////////////////


#endif ///#ifndef __BKLTESTCLASS_H__
