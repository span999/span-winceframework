// BattTestClass.h : interface of the Battery Test class
//
/////////////////////////////////////////////////////////////////////////////

#ifndef __IOCTLTESTCLASS_H__
#define __IOCTLTESTCLASS_H__

#include "TestClass.h"
#include "winioctl.h"
#include "pm.h"
#include "mio_ioctl.h"
#include "Thales_defines.h"



#define		DRVPRIFIX		TEXT("MPM1:")
///#define		DRVPRIFIX		TEXT("PRX1:")


typedef struct tDEVICEIOCTLPARAMETER {
    DWORD 	Index;
    TCHAR	Descrip[64];
    HANDLE 	DevHandle;
    DWORD	IOcode;
    BOOL 	hasIN;
    DWORD	sizeIN;
    DWORD	setIN;
    BOOL 	hasOUT;
    DWORD	sizeOUT;
    DWORD	setOUT;    
    BOOL 	hasRET;
} DEVICEIOCTLPARAMETER, *PDEVICEIOCTLPARAMETER;

#define 	DEVICEIOCTLLISTNUM		13

static DEVICEIOCTLPARAMETER DevIoctlList[DEVICEIOCTLLISTNUM] =
{
///		Index,	Descrip,									DevHandle, 	IOcode,								hasIN,	sizeIN,						setIN,	hasOUT,	sizeOUT,						setOUT,	hasRET
	{ 	0, 		TEXT("IOCTL_OEM_SUSPEND_TIMER_SET"),		NULL,		IOCTL_OEM_SUSPEND_TIMER_SET,		TRUE,	sizeof(DWORD),				60,		FALSE,	0,								0,		FALSE },
	{ 	1, 		TEXT("IOCTL_OEM_SUSPEND_TIMER_ENABLE"),		NULL,		IOCTL_OEM_SUSPEND_TIMER_ENABLE,		FALSE,	0,							0,		FALSE,	0,								0,		FALSE },	
	{ 	2, 		TEXT("IOCTL_OEM_SUSPEND_TIMER_DISABLE"),	NULL,		IOCTL_OEM_SUSPEND_TIMER_DISABLE,	FALSE,	0,							0,		FALSE,	0,								0,		FALSE },	
	{ 	3, 		TEXT("IOCTL_OEM_SUSPEND_TIMER_RESET"),		NULL,		IOCTL_OEM_SUSPEND_TIMER_RESET,		FALSE,	0,							0,		FALSE,	0,								0,		FALSE },	
	{ 	4, 		TEXT("IOCTL_OEM_GET_POWER_STATUS"),			NULL,		IOCTL_OEM_GET_POWER_STATUS,			FALSE,	0,							0,		TRUE,	4,								0,		FALSE },	
	{ 	5, 		TEXT("IOCTL_OEM_GET_DEVICE_STATUS"),		NULL,		IOCTL_OEM_GET_DEVICE_STATUS,		FALSE,	0,							0,		TRUE,	4,								0,		FALSE },	
	{ 	6, 		TEXT("IOCTL_OEM_SCREEN_TIMER_RESET"),		NULL,		IOCTL_OEM_SCREEN_TIMER_RESET,		FALSE,	0,							0,		FALSE,	0,								0,		FALSE },	
	{ 	7, 		TEXT("IOCTL_OEM_SCREEN_TIMER_ENABLE"),		NULL,		IOCTL_OEM_SCREEN_TIMER_ENABLE,		FALSE,	0,							0,		FALSE,	0,								0,		FALSE },	
	{ 	8, 		TEXT("IOCTL_OEM_SCREEN_TIMER_DISABLE"),		NULL,		IOCTL_OEM_SCREEN_TIMER_DISABLE,		FALSE,	0,							0,		FALSE,	0,								0,		FALSE },	
	{ 	9, 		TEXT("IOCTL_OEM_SCREEN_TIMER_SET"),			NULL,		IOCTL_OEM_SCREEN_TIMER_SET,			TRUE,	sizeof(DWORD),				30,		FALSE,	0,								0,		FALSE },	
	{ 	10,		TEXT("IOCTL_POWER_CAPABILITIES"),			NULL,		IOCTL_POWER_CAPABILITIES,			FALSE,	0,							0,		TRUE,	sizeof(POWER_CAPABILITIES),		0,		TRUE },	
	{ 	11,		TEXT("IOCTL_POWER_SET"),					NULL,		IOCTL_POWER_SET,					FALSE,	0,							0,		TRUE,	sizeof(CEDEVICE_POWER_STATE),	0,		TRUE },	
	{ 	12,		TEXT("IOCTL_POWER_GET"),					NULL,		IOCTL_POWER_GET,					FALSE,	0,							0,		TRUE,	sizeof(CEDEVICE_POWER_STATE),	0,		TRUE }
};

/////////////////////////////////////////////////////////////////////////////
/// STestItemPRX
/*
STestItemPRX()
*/


class STestItemPRX : public STestItemClass
{
/// Function Member
	/// Overrides
public:
protected:
	virtual void TestContent( DWORD dwIndex );
	virtual BOOL _InitLog();
	
	/// Implementation 	/// interface
public:
	~STestItemPRX(){;}
	STestItemPRX(){ m_hPRX = INVALID_HANDLE_VALUE; }
	STestItemPRX( UINT uiID ){ m_hPRX = INVALID_HANDLE_VALUE; Init(); SetTestItemID( uiID ); }
	
	/// Implementation 	/// Operations	
protected:
	BOOL TryOpenPortName( LPTSTR lpszPortName );
	BOOL RunDevIoctlList();
/// Data Member
public:
protected:
	HANDLE m_hPRX;
};





/////////////////////////////////////////////////////////////////////////////
/// SComBasicTest
/*
SIoctlBasicTest()
*/


class SIoctlBasicTest : public STestClass
{
/// Function Member
	/// Overrides
public:
	virtual void AddTestItem( UINT uiID );
protected:
	virtual BOOL _InitLog();

	/// Implementation 	/// interface
public:
	~SIoctlBasicTest(){;}
	SIoctlBasicTest(){ Init(); }
	SIoctlBasicTest( DWORD dwNumber, DWORD dwLoop ){ Init( dwNumber, dwLoop ); }
	
	/// Implementation 	/// Operations
protected:

/// Data Member
public:
protected:
};

/////////////////////////////////////////////////////////////////////////////


#endif ///#ifndef __IOCTLTESTCLASS_H__
