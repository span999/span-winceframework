#include <stdio.h>

#include <FreeRTOS.h>
#include <task.h>


/// include this for PIC graphic lib
#include "PICgraphic_set.h"

#include "StringID.h"
#include "fonts.h"

LANGUAGE_TYPE	gLanguage = 0;

const XCHAR *StringTableEN[] = {
	PowerOffModeENStr,			// "PowerOff"				,0
	OptionsENStr,				// "Options"				,1
	FitnessModeENStr,			// "Fitness Mode"			,2
	PowerDownENStr,				// "Power Down"				,3
	WatchModeENStr,				// "Watch Mode"				,4
	NavigationENStr,			// "Navigation"				,5
	QuickInfoENStr,				// "Quick Info"				,6
	SettingsENStr,				// "Settings"				,7
	HistoryENStr,				// "History"				,8
	ActivityDataENStr,			// "Activity Data"			,9
	UserENStr,					// "User"					,10
	DeviceENStr,				// "Device"					,11
	ActivityENStr,				// "Activity"				,12
};

const XCHAR *StringTableSP[] = {
	PowerOffModeSPStr,			// "PowerOff"				,0
	OptionsSPStr,				// "Options"				,1
	FitnessModeSPStr,			// "Fitness Mode"			,2
	PowerDownSPStr,				// "Power Down"				,3
	WatchModeSPStr,				// "Watch Mode"				,4
	NavigationSPStr,			// "Navigation"				,5
	QuickInfoSPStr,				// "Quick Info"				,6
	SettingsSPStr,				// "Settings"				,7
	HistorySPStr,				// "History"				,8
	ActivityDataSPStr,			// "Activity Data"			,9
	UserSPStr,					// "User"					,10
	DeviceSPStr,				// "Device"					,11
	ActivitySPStr,				// "Activity"				,12
};

const XCHAR *StringTableFR[] = {
	PowerOffModeFRStr,			// "PowerOff"				,0
	OptionsFRStr,				// "Options"				,1
	FitnessModeFRStr,			// "Fitness Mode"			,2
	PowerDownFRStr,				// "Power Down"				,3
	WatchModeFRStr,				// "Watch Mode"				,4
	NavigationFRStr,			// "Navigation"				,5
	QuickInfoFRStr,				// "Quick Info"				,6
	SettingsFRStr,				// "Settings"				,7
	HistoryFRStr,				// "History"				,8
	ActivityDataFRStr,			// "Activity Data"			,9
	UserFRStr,					// "User"					,10
	DeviceFRStr,				// "Device"					,11
	ActivityFRStr,				// "Activity"				,12
};

const XCHAR *StringTableIT[] = {
	PowerOffModeITStr,			// "PowerOff"				,0
	OptionsITStr,				// "Options"				,1
	FitnessModeITStr,			// "Fitness Mode"			,2
	PowerDownITStr,				// "Power Down"				,3
	WatchModeITStr,				// "Watch Mode"				,4
	NavigationITStr,			// "Navigation"				,5
	QuickInfoITStr,				// "Quick Info"				,6
	SettingsITStr,				// "Settings"				,7
	HistoryITStr,				// "History"				,8
	ActivityDataITStr,			// "Activity Data"			,9
	UserITStr,					// "User"					,10
	DeviceITStr,				// "Device"					,11
	ActivityITStr,				// "Activity"				,12
};

const XCHAR *StringTablePO[] = {
	PowerOffModePOStr,			// "PowerOff"				,0
	OptionsPOStr,				// "Options"				,1
	FitnessModePOStr,			// "Fitness Mode"			,2
	PowerDownPOStr,				// "Power Down"				,3
	WatchModePOStr,				// "Watch Mode"				,4
	NavigationPOStr,			// "Navigation"				,5
	QuickInfoPOStr,				// "Quick Info"				,6
	SettingsPOStr,				// "Settings"				,7
	HistoryPOStr,				// "History"				,8
	ActivityDataPOStr,			// "Activity Data"			,9
	UserPOStr,					// "User"					,10
	DevicePOStr,				// "Device"					,11
	ActivityPOStr,				// "Activity"				,12
};

const XCHAR *StringTableDU[] = {
	PowerOffModeDUStr,			// "PowerOff"				,0
	OptionsDUStr,				// "Options"				,1
	FitnessModeDUStr,			// "Fitness Mode"			,2
	PowerDownDUStr,				// "Power Down"				,3
	WatchModeDUStr,				// "Watch Mode"				,4
	NavigationDUStr,			// "Navigation"				,5
	QuickInfoDUStr,				// "Quick Info"				,6
	SettingsDUStr,				// "Settings"				,7
	HistoryDUStr,				// "History"				,8
	ActivityDataDUStr,			// "Activity Data"			,9
	UserDUStr,					// "User"					,10
	DeviceDUStr,				// "Device"					,11
	ActivityDUStr,				// "Activity"				,12
};

const XCHAR *StringTableGE[] = {
	PowerOffModeGEStr,			// "PowerOff"				,0
	OptionsGEStr,				// "Options"				,1
	FitnessModeGEStr,			// "Fitness Mode"			,2
	PowerDownGEStr,				// "Power Down"				,3
	WatchModeGEStr,				// "Watch Mode"				,4
	NavigationGEStr,			// "Navigation"				,5
	QuickInfoGEStr,				// "Quick Info"				,6
	SettingsGEStr,				// "Settings"				,7
	HistoryGEStr,				// "History"				,8
	ActivityDataGEStr,			// "Activity Data"			,9
	UserGEStr,					// "User"					,10
	DeviceGEStr,				// "Device"					,11
	ActivityGEStr,				// "Activity"				,12
};

const XCHAR *StringTableDA[] = {
	PowerOffModeDAStr,			// "PowerOff"				,0
	OptionsDAStr,				// "Options"				,1
	FitnessModeDAStr,			// "Fitness Mode"			,2
	PowerDownDAStr,				// "Power Down"				,3
	WatchModeDAStr,				// "Watch Mode"				,4
	NavigationDAStr,			// "Navigation"				,5
	QuickInfoDAStr,				// "Quick Info"				,6
	SettingsDAStr,				// "Settings"				,7
	HistoryDAStr,				// "History"				,8
	ActivityDataDAStr,			// "Activity Data"			,9
	UserDAStr,					// "User"					,10
	DeviceDAStr,				// "Device"					,11
	ActivityDAStr,				// "Activity"				,12
};



static XCHAR **pStringTableList[] = {
	StringTableEN,
	StringTableSP,
	StringTableFR,
	StringTableIT,
	StringTablePO,
	StringTableDU,
	StringTableGE,
	StringTableDA,
};

XCHAR *IdGetMString( WORD wStingID, LANGUAGE_TYPE lang )
{
	XCHAR *pString = NULL;

	pString = (pStringTableList[lang])[wStingID];
	
	return pString;
}

XCHAR *IdGetString( WORD wStingID )
{
	return IdGetMString( wStingID, EN );
}