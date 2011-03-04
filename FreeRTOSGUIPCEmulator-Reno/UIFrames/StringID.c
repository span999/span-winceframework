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
	SettingsMenuENStr,			// "Settings Menu"			,13
	HistoryMenuENStr,			// "History Menu"			,14
	ActivityHistoryENStr,		// "Activity History"		,15
	ViewENStr,					// "View"					,16
	DeleteENStr,				// "Delete"					,17
	AllActivitiesENStr,			// "All Activities"			,18
	ActivityTypeENStr,			// "ActivityType"			,19
	IndividualActivityENStr,	// "Individual Activity"	,20
	SessionENStr,				// "Session"				,21
	LapENStr,					// "Lap"					,22
	ActivityTotalsENStr,		// "Activity Totals"		,23
	ClearENStr,					// "Clear"					,24
	LocationHistoryENStr,		// "Location History"		,25
	IndividualLocationENStr,	// "Individual Location"	,26
	AllLocationsENStr,			// "All Locations"			,27
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
	SettingsMenuSPStr,			// "Settings Menu"			,13
	HistoryMenuSPStr,			// "History Menu"			,14
	ActivityHistorySPStr,		// "Activity History"		,15
	ViewSPStr,					// "View"					,16
	DeleteSPStr,				// "Delete"					,17
	AllActivitiesSPStr,			// "All Activities"			,18
	ActivityTypeSPStr,			// "ActivityType"			,19
	IndividualActivitySPStr,	// "Individual Activity"	,20
	SessionSPStr,				// "Session"				,21
	LapSPStr,					// "Lap"					,22
	ActivityTotalsSPStr,		// "Activity Totals"		,23
	ClearSPStr,					// "Clear"					,24
	LocationHistorySPStr,		// "Location History"		,25
	IndividualLocationSPStr,	// "Individual Location"	,26
	AllLocationsSPStr,			// "All Locations"			,27
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
	SettingsMenuFRStr,			// "Settings Menu"			,13
	HistoryMenuFRStr,			// "History Menu"			,14
	ActivityHistoryFRStr,		// "Activity History"		,15
	ViewFRStr,					// "View"					,16
	DeleteFRStr,				// "Delete"					,17
	AllActivitiesFRStr,			// "All Activities"			,18
	ActivityTypeFRStr,			// "ActivityType"			,19
	IndividualActivityFRStr,	// "Individual Activity"	,20
	SessionFRStr,				// "Session"				,21
	LapFRStr,					// "Lap"					,22
	ActivityTotalsFRStr,		// "Activity Totals"		,23
	ClearFRStr,					// "Clear"					,24
	LocationHistoryFRStr,		// "Location History"		,25
	IndividualLocationFRStr,	// "Individual Location"	,26
	AllLocationsFRStr,			// "All Locations"			,27
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
	SettingsMenuITStr,			// "Settings Menu"			,13
	HistoryMenuITStr,			// "History Menu"			,14
	ActivityHistoryITStr,		// "Activity History"		,15
	ViewITStr,					// "View"					,16
	DeleteITStr,				// "Delete"					,17
	AllActivitiesITStr,			// "All Activities"			,18
	ActivityTypeITStr,			// "ActivityType"			,19
	IndividualActivityITStr,	// "Individual Activity"	,20
	SessionITStr,				// "Session"				,21
	LapITStr,					// "Lap"					,22
	ActivityTotalsITStr,		// "Activity Totals"		,23
	ClearITStr,					// "Clear"					,24
	LocationHistoryITStr,		// "Location History"		,25
	IndividualLocationITStr,	// "Individual Location"	,26
	AllLocationsITStr,			// "All Locations"			,27
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
	SettingsMenuPOStr,			// "Settings Menu"			,13
	HistoryMenuPOStr,			// "History Menu"			,14
	ActivityHistoryPOStr,		// "Activity History"		,15
	ViewPOStr,					// "View"					,16
	DeletePOStr,				// "Delete"					,17
	AllActivitiesPOStr,			// "All Activities"			,18
	ActivityTypePOStr,			// "ActivityType"			,19
	IndividualActivityPOStr,	// "Individual Activity"	,20
	SessionPOStr,				// "Session"				,21
	LapPOStr,					// "Lap"					,22
	ActivityTotalsPOStr,		// "Activity Totals"		,23
	ClearPOStr,					// "Clear"					,24
	LocationHistoryPOStr,		// "Location History"		,25
	IndividualLocationPOStr,	// "Individual Location"	,26
	AllLocationsPOStr,			// "All Locations"			,27
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
	SettingsMenuDUStr,			// "Settings Menu"			,13
	HistoryMenuDUStr,			// "History Menu"			,14
	ActivityHistoryDUStr,		// "Activity History"		,15
	ViewDUStr,					// "View"					,16
	DeleteDUStr,				// "Delete"					,17
	AllActivitiesDUStr,			// "All Activities"			,18
	ActivityTypeDUStr,			// "ActivityType"			,19
	IndividualActivityDUStr,	// "Individual Activity"	,20
	SessionDUStr,				// "Session"				,21
	LapDUStr,					// "Lap"					,22
	ActivityTotalsDUStr,		// "Activity Totals"		,23
	ClearDUStr,					// "Clear"					,24
	LocationHistoryDUStr,		// "Location History"		,25
	IndividualLocationDUStr,	// "Individual Location"	,26
	AllLocationsDUStr,			// "All Locations"			,27
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
	SettingsMenuGEStr,			// "Settings Menu"			,13
	HistoryMenuGEStr,			// "History Menu"			,14
	ActivityHistoryGEStr,		// "Activity History"		,15
	ViewGEStr,					// "View"					,16
	DeleteGEStr,				// "Delete"					,17
	AllActivitiesGEStr,			// "All Activities"			,18
	ActivityTypeGEStr,			// "ActivityType"			,19
	IndividualActivityGEStr,	// "Individual Activity"	,20
	SessionGEStr,				// "Session"				,21
	LapGEStr,					// "Lap"					,22
	ActivityTotalsGEStr,		// "Activity Totals"		,23
	ClearGEStr,					// "Clear"					,24
	LocationHistoryGEStr,		// "Location History"		,25
	IndividualLocationGEStr,	// "Individual Location"	,26
	AllLocationsGEStr,			// "All Locations"			,27
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
	SettingsMenuDAStr,			// "Settings Menu"			,13
	HistoryMenuDAStr,			// "History Menu"			,14
	ActivityHistoryDAStr,		// "Activity History"		,15
	ViewDAStr,					// "View"					,16
	DeleteDAStr,				// "Delete"					,17
	AllActivitiesDAStr,			// "All Activities"			,18
	ActivityTypeDAStr,			// "ActivityType"			,19
	IndividualActivityDAStr,	// "Individual Activity"	,20
	SessionDAStr,				// "Session"				,21
	LapDAStr,					// "Lap"					,22
	ActivityTotalsDAStr,		// "Activity Totals"		,23
	ClearDAStr,					// "Clear"					,24
	LocationHistoryDAStr,		// "Location History"		,25
	IndividualLocationDAStr,	// "Individual Location"	,26
	AllLocationsDAStr,			// "All Locations"			,27
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