// AudioTestClass.h : interface of the Battery Test class
//
/////////////////////////////////////////////////////////////////////////////

#ifndef __AUDIOTESTCLASS_H__
#define __AUDIOTESTCLASS_H__

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
	STestItemAPI(){;}
	STestItemAPI( UINT uiID ){ Init(); SetTestItemID( uiID ); }
	
	/// Implementation 	/// Operations
protected:
	BOOL GetDevOutCaps( UINT uiDevID );
	BOOL GetDevInCaps( UINT uiDevID );	


/// Data Member
public:
protected:

};


/////////////////////////////////////////////////////////////////////////////
/// STestItemPlayBack
/*
STestItemPlayBack()
*/

class STestItemPlayBack : public STestItemClass
{
/// Function Member
	/// Overrides
public:
protected:
	virtual void TestContent( DWORD dwIndex );
	virtual BOOL _InitLog();

	/// Implementation 	/// interface
public:
	~STestItemPlayBack(){;}
	STestItemPlayBack(){;}
	STestItemPlayBack( UINT uiID ){ Init(); SetTestItemID( uiID ); }
	
	/// Implementation 	/// Operations
protected:
	
/// Data Member
public:
protected:

};


/////////////////////////////////////////////////////////////////////////////
/// STestItemOnOff
/*
STestItemVolume()
*/

class STestItemVolume : public STestItemClass
{
/// Function Member
	/// Overrides
public:
protected:
	virtual void TestContent( DWORD dwIndex );
	virtual BOOL _InitLog();

	/// Implementation 	/// interface
public:
	~STestItemVolume(){;}
	STestItemVolume(){;}
	STestItemVolume( UINT uiID ){ Init(); SetTestItemID( uiID ); }
	
	/// Implementation 	/// Operations
protected:
	
/// Data Member
public:
protected:

};



/////////////////////////////////////////////////////////////////////////////
/// SAudioBasicTest
/*
SAudioBasicTest()
*/


class SAudioBasicTest : public STestClass
{
/// Function Member
	/// Overrides
public:
	virtual void AddTestItem( UINT uiID );
protected:
	virtual BOOL _InitLog();

	/// Implementation 	/// interface
public:
	~SAudioBasicTest(){;}
///	SAudioBasicTest(){ STestClass::STestClass(); }
///	SAudioBasicTest( DWORD dwNumber, DWORD dwLoop ){ STestClass::STestClass( dwNumber, dwLoop ); }
	SAudioBasicTest(){ Init(); }
	SAudioBasicTest( DWORD dwNumber, DWORD dwLoop ){ Init( dwNumber, dwLoop ); }


	/// Implementation 	/// Operations
protected:

/// Data Member
public:
protected:

};

/////////////////////////////////////////////////////////////////////////////


#endif ///#ifndef __BKLTESTCLASS_H__
