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
	ActivityTypeENStr,			// "Activity Type"			,19
	IndividualActivityENStr,	// "Individual Activity"	,20
	SessionENStr,				// "Session"				,21
	LapENStr,					// "Lap"					,22
	ActivityTotalsENStr,		// "Activity Totals"		,23
	ClearENStr,					// "Clear"					,24
	LocationHistoryENStr,		// "Location History"		,25
	IndividualLocationENStr,	// "Individual Location"	,26
	AllLocationsENStr,			// "All Locations"			,27
	PhysicalInformationENStr,	// "Physical Information"	,28
	ContactInformationENStr,	// "Contact Information"	,29
	EmergencyInformationENStr,	// "Emergency Information"	,30
	GPSENStr,					// "GPS"					,31
	EnableDisableENStr,			// "Enable/Disable"			,32
	SatellitesENStr,			// "Satellites"				,33
	LanguageENStr,				// "Language"				,34
	DateTimeENStr,				// "Date Time"				,35
	DateFormatENStr,			// "Date Format"			,36
	TimeFormatENStr,			// "Time Format"			,37
	TimeZoneENStr,				// "Time Zone"				,38
	DaylightSavingsTimeENStr,	// "Daylight Savings Time"	,39
	UnitsofMeasurementENStr,	// "Units of Measurement"	,40
	DistanceENStr,				// "Distance"				,41
	ElevationENStr,				// "Elevation"				,42
	TemperatureENStr,			// "Temperature"			,43
	HeartRateENStr,				// "HeartRate"				,44
	PowerENStr,					// "Power"					,45
	DisplayENStr,				// "Display"				,46
	BacklightENStr,				// "Backlight"				,47
	RecordingENStr,				// "Recording"				,48
	StartActivityReminderENStr,	// "Start Activity Reminder"	,49
	DataRecordingFrequencyENStr,	// "Data Recording Frequency"		,50
	PowerAveragingRecordingENStr,	// "Power Averaging Recording"	,51
	CadenceAveragingRecordingENStr,	// "Cadence Averaging Recording"	,52
	AutoPowerDownENStr,			// "Auto Power Down"			,53
	FeedbackENStr,				// "Feedback"				,54
	PhysicalButtonPressesENStr,	// "Physical Button Presses"	,55
	DigitalButtonPressesENStr,	// "Digital Button Presses"	,56
	NotificationsENStr,			// "Notifications"			,57
	AlertsENStr,				// "Alerts"					,58
	EquipmentENStr,				// "Equipment"				,59
	DeviceInformationENStr,		// "Device Information"		,60	
	CurrentActivityProfileENStr,	// "Current Activity Profile"		,61
	ActivityProfileSettingsENStr,	// "Activity Profile Settings"	,62	
	
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
	PhysicalInformationSPStr,	// "Physical Information"	,28
	ContactInformationSPStr,	// "Contact Information"	,29
	EmergencyInformationSPStr,	// "Emergency Information"	,30
	GPSSPStr,					// "GPS"					,31
	EnableDisableSPStr,			// "Enable/Disable"			,32
	SatellitesSPStr,			// "Satellites"				,33
	LanguageSPStr,				// "Language"				,34
	DateTimeSPStr,				// "Date Time"				,35
	DateFormatSPStr,			// "Date Format"			,36
	TimeFormatSPStr,			// "Time Format"			,37
	TimeZoneSPStr,				// "Time Zone"				,38
	DaylightSavingsTimeSPStr,	// "Daylight Savings Time"	,39
	UnitsofMeasurementSPStr,	// "Units of Measurement"	,40
	DistanceSPStr,				// "Distance"				,41
	ElevationSPStr,				// "Elevation"				,42
	TemperatureSPStr,			// "Temperature"			,43
	HeartRateSPStr,				// "HeartRate"				,44
	PowerSPStr,					// "Power"					,45
	DisplaySPStr,				// "Display"				,46
	BacklightSPStr,				// "Backlight"				,47
	RecordingSPStr,				// "Recording"				,48
	StartActivityReminderSPStr,	// "Start Activity Reminder"	,49
	DataRecordingFrequencySPStr,	// "Data Recording Frequency"		,50
	PowerAveragingRecordingSPStr,	// "Power Averaging Recording"	,51
	CadenceAveragingRecordingSPStr,	// "Cadence Averaging Recording"	,52
	AutoPowerDownSPStr,			// "Auto Power Down"			,53
	FeedbackSPStr,				// "Feedback"				,54
	PhysicalButtonPressesSPStr,	// "Physical Button Presses"	,55
	DigitalButtonPressesSPStr,	// "Digital Button Presses"	,56
	NotificationsSPStr,			// "Notifications"			,57
	AlertsSPStr,				// "Alerts"					,58
	EquipmentSPStr,				// "Equipment"				,59
	DeviceInformationSPStr,		// "Device Information"		,60	
	CurrentActivityProfileSPStr,	// "Current Activity Profile"		,61
	ActivityProfileSettingsSPStr,	// "Activity Profile Settings"	,62	

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
	PhysicalInformationFRStr,	// "Physical Information"	,28
	ContactInformationFRStr,	// "Contact Information"	,29
	EmergencyInformationFRStr,	// "Emergency Information"	,30
	GPSFRStr,					// "GPS"					,31
	EnableDisableFRStr,			// "Enable/Disable"			,32
	SatellitesFRStr,			// "Satellites"				,33
	LanguageFRStr,				// "Language"				,34
	DateTimeFRStr,				// "Date Time"				,35
	DateFormatFRStr,			// "Date Format"			,36
	TimeFormatFRStr,			// "Time Format"			,37
	TimeZoneFRStr,				// "Time Zone"				,38
	DaylightSavingsTimeFRStr,	// "Daylight Savings Time"	,39
	UnitsofMeasurementFRStr,	// "Units of Measurement"	,40
	DistanceFRStr,				// "Distance"				,41
	ElevationFRStr,				// "Elevation"				,42
	TemperatureFRStr,			// "Temperature"			,43
	HeartRateFRStr,				// "HeartRate"				,44
	PowerFRStr,					// "Power"					,45
	DisplayFRStr,				// "Display"				,46
	BacklightFRStr,				// "Backlight"				,47
	RecordingFRStr,				// "Recording"				,48
	StartActivityReminderFRStr,	// "Start Activity Reminder"	,49
	DataRecordingFrequencyFRStr,	// "Data Recording Frequency"		,50
	PowerAveragingRecordingFRStr,	// "Power Averaging Recording"	,51
	CadenceAveragingRecordingFRStr,	// "Cadence Averaging Recording"	,52
	AutoPowerDownFRStr,			// "Auto Power Down"			,53
	FeedbackFRStr,				// "Feedback"				,54
	PhysicalButtonPressesFRStr,	// "Physical Button Presses"	,55
	DigitalButtonPressesFRStr,	// "Digital Button Presses"	,56
	NotificationsFRStr,			// "Notifications"			,57
	AlertsFRStr,				// "Alerts"					,58
	EquipmentFRStr,				// "Equipment"				,59
	DeviceInformationFRStr,		// "Device Information"		,60	
	CurrentActivityProfileFRStr,	// "Current Activity Profile"		,61
	ActivityProfileSettingsFRStr,	// "Activity Profile Settings"	,62	

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
	PhysicalInformationITStr,	// "Physical Information"	,28
	ContactInformationITStr,	// "Contact Information"	,29
	EmergencyInformationITStr,	// "Emergency Information"	,30
	GPSITStr,					// "GPS"					,31
	EnableDisableITStr,			// "Enable/Disable"			,32
	SatellitesITStr,			// "Satellites"				,33
	LanguageITStr,				// "Language"				,34
	DateTimeITStr,				// "Date Time"				,35
	DateFormatITStr,			// "Date Format"			,36
	TimeFormatITStr,			// "Time Format"			,37
	TimeZoneITStr,				// "Time Zone"				,38
	DaylightSavingsTimeITStr,	// "Daylight Savings Time"	,39
	UnitsofMeasurementITStr,	// "Units of Measurement"	,40
	DistanceITStr,				// "Distance"				,41
	ElevationITStr,				// "Elevation"				,42
	TemperatureITStr,			// "Temperature"			,43
	HeartRateITStr,				// "HeartRate"				,44
	PowerITStr,					// "Power"					,45
	DisplayITStr,				// "Display"				,46
	BacklightITStr,				// "Backlight"				,47
	RecordingITStr,				// "Recording"				,48
	StartActivityReminderITStr,	// "Start Activity Reminder"	,49
	DataRecordingFrequencyITStr,	// "Data Recording Frequency"		,50
	PowerAveragingRecordingITStr,	// "Power Averaging Recording"	,51
	CadenceAveragingRecordingITStr,	// "Cadence Averaging Recording"	,52
	AutoPowerDownITStr,			// "Auto Power Down"			,53
	FeedbackITStr,				// "Feedback"				,54
	PhysicalButtonPressesITStr,	// "Physical Button Presses"	,55
	DigitalButtonPressesITStr,	// "Digital Button Presses"	,56
	NotificationsITStr,			// "Notifications"			,57
	AlertsITStr,				// "Alerts"					,58
	EquipmentITStr,				// "Equipment"				,59
	DeviceInformationITStr,		// "Device Information"		,60	
	CurrentActivityProfileITStr,	// "Current Activity Profile"		,61
	ActivityProfileSettingsITStr,	// "Activity Profile Settings"	,62	

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
	PhysicalInformationPOStr,	// "Physical Information"	,28
	ContactInformationPOStr,	// "Contact Information"	,29
	EmergencyInformationPOStr,	// "Emergency Information"	,30
	GPSPOStr,					// "GPS"					,31
	EnableDisablePOStr,			// "Enable/Disable"			,32
	SatellitesPOStr,			// "Satellites"				,33
	LanguagePOStr,				// "Language"				,34
	DateTimePOStr,				// "Date Time"				,35
	DateFormatPOStr,			// "Date Format"			,36
	TimeFormatPOStr,			// "Time Format"			,37
	TimeZonePOStr,				// "Time Zone"				,38
	DaylightSavingsTimePOStr,	// "Daylight Savings Time"	,39
	UnitsofMeasurementPOStr,	// "Units of Measurement"	,40
	DistancePOStr,				// "Distance"				,41
	ElevationPOStr,				// "Elevation"				,42
	TemperaturePOStr,			// "Temperature"			,43
	HeartRatePOStr,				// "HeartRate"				,44
	PowerPOStr,					// "Power"					,45
	DisplayPOStr,				// "Display"				,46
	BacklightPOStr,				// "Backlight"				,47
	RecordingPOStr,				// "Recording"				,48
	StartActivityReminderPOStr,	// "Start Activity Reminder"	,49
	DataRecordingFrequencyPOStr,	// "Data Recording Frequency"		,50
	PowerAveragingRecordingPOStr,	// "Power Averaging Recording"	,51
	CadenceAveragingRecordingPOStr,	// "Cadence Averaging Recording"	,52
	AutoPowerDownPOStr,			// "Auto Power Down"			,53
	FeedbackPOStr,				// "Feedback"				,54
	PhysicalButtonPressesPOStr,	// "Physical Button Presses"	,55
	DigitalButtonPressesPOStr,	// "Digital Button Presses"	,56
	NotificationsPOStr,			// "Notifications"			,57
	AlertsPOStr,				// "Alerts"					,58
	EquipmentPOStr,				// "Equipment"				,59
	DeviceInformationPOStr,		// "Device Information"		,60	
	CurrentActivityProfilePOStr,	// "Current Activity Profile"		,61
	ActivityProfileSettingsPOStr,	// "Activity Profile Settings"	,62	

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
	PhysicalInformationDUStr,	// "Physical Information"	,28
	ContactInformationDUStr,	// "Contact Information"	,29
	EmergencyInformationDUStr,	// "Emergency Information"	,30
	GPSDUStr,					// "GPS"					,31
	EnableDisableDUStr,			// "Enable/Disable"			,32
	SatellitesDUStr,			// "Satellites"				,33
	LanguageDUStr,				// "Language"				,34
	DateTimeDUStr,				// "Date Time"				,35
	DateFormatDUStr,			// "Date Format"			,36
	TimeFormatDUStr,			// "Time Format"			,37
	TimeZoneDUStr,				// "Time Zone"				,38
	DaylightSavingsTimeDUStr,	// "Daylight Savings Time"	,39
	UnitsofMeasurementDUStr,	// "Units of Measurement"	,40
	DistanceDUStr,				// "Distance"				,41
	ElevationDUStr,				// "Elevation"				,42
	TemperatureDUStr,			// "Temperature"			,43
	HeartRateDUStr,				// "HeartRate"				,44
	PowerDUStr,					// "Power"					,45
	DisplayDUStr,				// "Display"				,46
	BacklightDUStr,				// "Backlight"				,47
	RecordingDUStr,				// "Recording"				,48
	StartActivityReminderDUStr,	// "Start Activity Reminder"	,49
	DataRecordingFrequencyDUStr,	// "Data Recording Frequency"		,50
	PowerAveragingRecordingDUStr,	// "Power Averaging Recording"	,51
	CadenceAveragingRecordingDUStr,	// "Cadence Averaging Recording"	,52
	AutoPowerDownDUStr,			// "Auto Power Down"			,53
	FeedbackDUStr,				// "Feedback"				,54
	PhysicalButtonPressesDUStr,	// "Physical Button Presses"	,55
	DigitalButtonPressesDUStr,	// "Digital Button Presses"	,56
	NotificationsDUStr,			// "Notifications"			,57
	AlertsDUStr,				// "Alerts"					,58
	EquipmentDUStr,				// "Equipment"				,59
	DeviceInformationDUStr,		// "Device Information"		,60	
	CurrentActivityProfileDUStr,	// "Current Activity Profile"		,61
	ActivityProfileSettingsDUStr,	// "Activity Profile Settings"	,62	

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
	PhysicalInformationGEStr,	// "Physical Information"	,28
	ContactInformationGEStr,	// "Contact Information"	,29
	EmergencyInformationGEStr,	// "Emergency Information"	,30
	GPSGEStr,					// "GPS"					,31
	EnableDisableGEStr,			// "Enable/Disable"			,32
	SatellitesGEStr,			// "Satellites"				,33
	LanguageGEStr,				// "Language"				,34
	DateTimeGEStr,				// "Date Time"				,35
	DateFormatGEStr,			// "Date Format"			,36
	TimeFormatGEStr,			// "Time Format"			,37
	TimeZoneGEStr,				// "Time Zone"				,38
	DaylightSavingsTimeGEStr,	// "Daylight Savings Time"	,39
	UnitsofMeasurementGEStr,	// "Units of Measurement"	,40
	DistanceGEStr,				// "Distance"				,41
	ElevationGEStr,				// "Elevation"				,42
	TemperatureGEStr,			// "Temperature"			,43
	HeartRateGEStr,				// "HeartRate"				,44
	PowerGEStr,					// "Power"					,45
	DisplayGEStr,				// "Display"				,46
	BacklightGEStr,				// "Backlight"				,47
	RecordingGEStr,				// "Recording"				,48
	StartActivityReminderGEStr,	// "Start Activity Reminder"	,49
	DataRecordingFrequencyGEStr,	// "Data Recording Frequency"		,50
	PowerAveragingRecordingGEStr,	// "Power Averaging Recording"	,51
	CadenceAveragingRecordingGEStr,	// "Cadence Averaging Recording"	,52
	AutoPowerDownGEStr,			// "Auto Power Down"			,53
	FeedbackGEStr,				// "Feedback"				,54
	PhysicalButtonPressesGEStr,	// "Physical Button Presses"	,55
	DigitalButtonPressesGEStr,	// "Digital Button Presses"	,56
	NotificationsGEStr,			// "Notifications"			,57
	AlertsGEStr,				// "Alerts"					,58
	EquipmentGEStr,				// "Equipment"				,59
	DeviceInformationGEStr,		// "Device Information"		,60	
	CurrentActivityProfileGEStr,	// "Current Activity Profile"		,61
	ActivityProfileSettingsGEStr,	// "Activity Profile Settings"	,62	

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
	PhysicalInformationDAStr,	// "Physical Information"	,28
	ContactInformationDAStr,	// "Contact Information"	,29
	EmergencyInformationDAStr,	// "Emergency Information"	,30
	GPSDAStr,					// "GPS"					,31
	EnableDisableDAStr,			// "Enable/Disable"			,32
	SatellitesDAStr,			// "Satellites"				,33
	LanguageDAStr,				// "Language"				,34
	DateTimeDAStr,				// "Date Time"				,35
	DateFormatDAStr,			// "Date Format"			,36
	TimeFormatDAStr,			// "Time Format"			,37
	TimeZoneDAStr,				// "Time Zone"				,38
	DaylightSavingsTimeDAStr,	// "Daylight Savings Time"	,39
	UnitsofMeasurementDAStr,	// "Units of Measurement"	,40
	DistanceDAStr,				// "Distance"				,41
	ElevationDAStr,				// "Elevation"				,42
	TemperatureDAStr,			// "Temperature"			,43
	HeartRateDAStr,				// "HeartRate"				,44
	PowerDAStr,					// "Power"					,45
	DisplayDAStr,				// "Display"				,46
	BacklightDAStr,				// "Backlight"				,47
	RecordingDAStr,				// "Recording"				,48
	StartActivityReminderDAStr,	// "Start Activity Reminder"	,49
	DataRecordingFrequencyDAStr,	// "Data Recording Frequency"		,50
	PowerAveragingRecordingDAStr,	// "Power Averaging Recording"	,51
	CadenceAveragingRecordingDAStr,	// "Cadence Averaging Recording"	,52
	AutoPowerDownDAStr,			// "Auto Power Down"			,53
	FeedbackDAStr,				// "Feedback"				,54
	PhysicalButtonPressesDAStr,	// "Physical Button Presses"	,55
	DigitalButtonPressesDAStr,	// "Digital Button Presses"	,56
	NotificationsDAStr,			// "Notifications"			,57
	AlertsDAStr,				// "Alerts"					,58
	EquipmentDAStr,				// "Equipment"				,59
	DeviceInformationDAStr,		// "Device Information"		,60	
	CurrentActivityProfileDAStr,	// "Current Activity Profile"		,61
	ActivityProfileSettingsDAStr,	// "Activity Profile Settings"	,62	

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