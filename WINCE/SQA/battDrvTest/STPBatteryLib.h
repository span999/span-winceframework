/* MiTAC Int'l Corp. 2001~2005

=========================================================================
Description:
    header file of PwrButton.c


=========================================================================
Release notes

Modifier	        sign	        date		    Description
-------------------------------------------------------------------------
jack                jack0325        2005/03/25      Change IOCTL value define
*/



#ifndef __STPBATTERYLIB_H__
#define __STPBATTERYLIB_H__


// =============================================================================
// =============================================================================
//  define
#define 	BATTERY_ADC_RANGE_MAX		5000
#define 	BATTERY_ADC_RANGE_MIN		0000
#define 	BATTERY_ADC_NORMAL_MAX		3600
#define 	BATTERY_ADC_NORMAL_MIN		3000

#define		BATTERY_ADC_TICK_RATIO		(1/1000)	/// ADC/Tick	
///Command
#define		FLAG_BATTERY_TEST_MASK			0x00FFFFFF
///#define		FLAG_BATTERY_NORMAL_DISCHARGE	0x00000001
///#define		FLAG_BATTERY_NORMAL_CHARGE		0x00000002
///#define		FLAG_BATTERY_BOUNCE_DISCHARGE	0x00000010
///#define		FLAG_BATTERY_BOUNCE_CHARGE		0x00000020
///#define		FLAG_BATTERY_UNUSUAL_DISCHARGE	0x00000100
///#define		FLAG_BATTERY_UNUSUAL_CHARGE		0x00000200
#define		FLAG_BATTERY_NORMAL_TEST	0x00000001
#define		FLAG_BATTERY_BOUNCE_TEST	0x00000010
#define		FLAG_BATTERY_UNUSUAL_TEST	0x00000100

#define		FLAG_BATTERY_STP_MONITOR		0x00100000		///send the data when change
#define		FLAG_BATTERY_GET_FLAG			0x00800000		///get current state

#define		FLAG_BATTERY_STATE_MASK			0xF0000000
#define		FLAG_BATTERY_STATE_INITED		0x40000000
#define		FLAG_BATTERY_STATE_UNKNOW		0x80000000

#define		FLAG_BATTERY_MODE_MASK			0x0F000000
#define		FLAG_BATTERY_MODE_FAILURE		0x01000000		///can't get anything from HW
#define		FLAG_BATTERY_MODE_ADC			0x02000000		///fake ADC
#define		FLAG_BATTERY_MODE_PERCENT		0x04000000		///fake battery percent
#define		FLAG_BATTERY_MODE_THERAML		0x08000000		///fake thermal

///Response
#define		STP_BATTERY_RSP_TYPE_MASK		0xFF000000
#define		STP_BATTERY_RSP_TYPE_ADC		0x01000000
#define		STP_BATTERY_RSP_TYPE_PERCENT	0x02000000
#define		STP_BATTERY_RSP_TYPE_THERMAL	0x04000000

#define		STP_BATTERY_RSP_DATA_MASK		0x00FFFFFF

///named event
#define		STP_BATTERY_CMD_EVENT			L"named STP battery driver command event"
#define		STP_BATTERY_RSP_EVENT			L"named STP battery driver response event"



// =============================================================================
// =============================================================================

///EXTERN_C BOOL BatteryOEMMutexInitialize(HANDLE hMutex, DWORD dwMutexTO);

// =============================================================================
// =============================================================================
//  forward declaration


// =============================================================================
// =============================================================================
//  extern function
EXTERN_C DWORD ADCHook( DWORD dwRealADC );
EXTERN_C DWORD PercentHook( DWORD dwRealPercent );
EXTERN_C DWORD ThermalHook( DWORD dwRealThermal );
EXTERN_C DWORD STPInit( DWORD dwOption );

// =============================================================================
// =============================================================================
//  extern pointer


// =============================================================================
// =============================================================================
//  extern handle


// =============================================================================
// =============================================================================
//  extern variable



#endif	///#ifndef __STPBATTERYLIB_H__
