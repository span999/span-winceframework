#include <stdio.h>

#include <FreeRTOS.h>
#include <task.h>


/// include this for PIC graphic lib
#include "PICgraphic_set.h"

#include "StringID.h"
#include "fonts.h"

LANGUAGE_TYPE	gLanguage = 0;


/*
a= 0x0061, 0x0041,
b= 0x0062, 
c= 0x0063, 
d= 0x0064, 
e= 0x0065, 
f= 0x0066, 
g= 0x0067, 
h= 0x0068, 
i= 0x0069, 
j= 0x006A, 
k= 0x006B, 
l= 0x006C, 
m= 0x006D, 
n= 0x006E, 
o= 0x006F, 
p= 0x0070, 
q= 0x0071, 0x0051,
r= 0x0072, 
s= 0x0073, 
t= 0x0074, 
u= 0x0075, 
v= 0x0076, 
w= 0x0077, 
x= 0x0078, 
y= 0x0079, 
z= 0x007A,
*/
const XCHAR Func01Str[] = {	0x006C, 0x0061, 0x0070, 0x0020, 0x0061, 0x0076, 0x0067, 0x0020, 0x0070, 0x0061, 0x0063, 0x0065, // lap avg pace
							0x0000};  
const XCHAR Func02Str[] = {	0x0065, 0x006C, 0x0061, 0x0070, 0x0073, 0x0065, 0x0064, 0x0020, 0x0074, 0x0069, 0x006D, 0x0065, // elapsed time
							0x0000};  
const XCHAR Func03Str[] = {	0x006C, 0x0061, 0x0070, 0x0020, 0x006D, 0x0061, 0x0078, 0x0020, 0x002D, 0x0020, 0x0067, 0x0072, 0x0064, // lap max - grd
							0x0000};
const XCHAR Func04Str[] = {	0x006C, 0x0061, 0x0070, 0x0020, 0x0061, 0x0076, 0x0067, 0x0020, 0x0068, 0x0072, // lap avg hr
							0x0000};  
const XCHAR Func05Str[] = {	0x0064, 0x0069, 0x0073, 0x0074, 0x0061, 0x006E, 0x0063, 0x0065, // distance
							0x0000};  
const XCHAR Func06Str[] = {	0x0065, 0x006C, 0x0065, 0x0076, 0x0061, 0x0074, 0x0069, 0x006F, 0x006E, // elevation
							0x0000};  

							
const XCHAR Unit01Str[] = {	0x002F, 0x006D, 0x0069, // /mi
							0x0000};  
const XCHAR Unit02Str[] = {	0x0025, // %
							0x0000};  
const XCHAR Unit03Str[] = {	0x0062, 0x0070, 0x006D, // bpm
							0x0000};  
const XCHAR Unit04Str[] = {	0x0066, 0x0074, // ft
							0x0000};  

						
/*
/= 0x002F, 
:= 0x003A,
%= 0x0025,
.= 0x002E,
-= 0x002D,
<= 0x003C,
>= 0x003E,
,= 0x002C,
0x0030, 0x0031, 0x0032, 0x0033, 0x0034, 0x0035, 0x0036, 0x0037, 0x0038, 0x0039,
*/							
const XCHAR Data01Str[] = {	0x0031, 0x0030, 0x003A, 0x0032, 0x0030, // 10:20
							0x0000};  
///const XCHAR Data02Str[] = {	0x0032, 0x0030, 0x003A, 0x0031, 0x0039, 0x003A, 0x0030, 0x0031, // 20:19:01
///							0x0000};  
const XCHAR Data02Str[] = {	0x0031, 0x0039, 0x003A, 0x0030, 0x0031, 0x0000, 0x0000, 0x0000,// 19:01
							0x0000};  
const XCHAR Data03Str[] = {	0x0031, 0x0035, // 15
							0x0000};  
const XCHAR Data04Str[] = {	0x0031, 0x0036, 0x0030, // 160
							0x0000};  
const XCHAR Data05Str[] = {	0x0033, 0x002E, 0x0030, 0x0030, // 3.00
							0x0000};  
const XCHAR Data06Str[] = {	0x0031, 0x0035, 0x0035, // 155
							0x0000};  



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
	VerticalSpeedENStr,			// "Vertical Speed"			,63	
	TimeENStr,					// "Time"					,64	
	SpeedPaceENStr,				// "Speed/Pace"				,65	
	GradeENStr,					// "Grade"					,66	
	CaloriesENStr,				// "Calories"				,67	
	CadenceENStr,				// "Cadence"				,68	
	NavigatetoLocationENStr,	// "Navigate to Location"	,69	
	MapBreadcrumbENStr,			// "Map & Breadcrumb"		,70	
	PowerAlertENStr,			// "Power Alert"			,71	
	CadenceAlertENStr,			// "Cadence Alert"			,72	
	HeartRateAlertENStr,		// "Heart Rate Alert"		,73	
	SpeedPaceAlertENStr,		// "Speed Pace Alert"		,74	
	TimeAlertENStr,				// "Time Alert"				,75	
	DistanceAlertENStr,			// "Distance Alert"			,76	
	CaloriesAlertENStr,			// "Calories Alert"			,77	
	TrainingAlertsENStr,		// "Training Alerts"		,78	
	PowerZonesENStr,			// "Power Zones"			,79	
	HeartRateZonesENStr,		// "Heart Rate Zones"		,80	
	TrainingZonesENStr,			// "Training Zones"			,81	
	ActivityPacerScreenENStr,	// "Activity Pacer Screen"	,82	
	ActivityDataScreensENStr,	// "Activity Data Screens"	,83	
	AutoScrollENStr,			// "Auto Scroll"			,84	
	DataScreensENStr,			// "Data Screens"			,85	
	AutoPauseENStr,				// "Auto Pause"				,86	
	AutoLapENStr,				// "Auto Lap"				,87	
	AutoPauseENStr,				// "Auto Pause"				,88	
	RecordingPreferencesENStr,	// "Recording Preferences"	,89	
	SpeedPacePreferenceENStr,	// "Speed Pace Preference"	,90	
	ProfileNameENStr,			// "Profile Name"			,91	
	MultiSportSetupENStr,		// "MultiSport Setup"		,92	
	
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
	VerticalSpeedSPStr,			// "Vertical Speed"			,63	
	TimeSPStr,					// "Time"					,64	
	SpeedPaceSPStr,				// "Speed/Pace"				,65	
	GradeSPStr,					// "Grade"					,66	
	CaloriesSPStr,				// "Calories"				,67	
	CadenceSPStr,				// "Cadence"				,68	
	NavigatetoLocationSPStr,	// "Navigate to Location"	,69	
	MapBreadcrumbSPStr,			// "Map & Breadcrumb"		,70	
	PowerAlertSPStr,			// "Power Alert"			,71	
	CadenceAlertSPStr,			// "Cadence Alert"			,72	
	HeartRateAlertSPStr,		// "Heart Rate Alert"		,73	
	SpeedPaceAlertSPStr,		// "Speed Pace Alert"		,74	
	TimeAlertSPStr,				// "Time Alert"				,75	
	DistanceAlertSPStr,			// "Distance Alert"			,76	
	CaloriesAlertSPStr,			// "Calories Alert"			,77	
	TrainingAlertsSPStr,		// "Training Alerts"		,78	
	PowerZonesSPStr,			// "Power Zones"			,79	
	HeartRateZonesSPStr,		// "Heart Rate Zones"		,80	
	TrainingZonesSPStr,			// "Training Zones"			,81	
	ActivityPacerScreenSPStr,	// "Activity Pacer Screen"	,82	
	ActivityDataScreensSPStr,	// "Activity Data Screens"	,83	
	AutoScrollSPStr,			// "Auto Scroll"			,84	
	DataScreensSPStr,			// "Data Screens"			,85	
	AutoPauseSPStr,				// "Auto Pause"				,86	
	AutoLapSPStr,				// "Auto Lap"				,87	
	AutoPauseSPStr,				// "Auto Pause"				,88	
	RecordingPreferencesSPStr,	// "Recording Preferences"	,89	
	SpeedPacePreferenceSPStr,	// "Speed Pace Preference"	,90	
	ProfileNameSPStr,			// "Profile Name"			,91	
	MultiSportSetupSPStr,		// "MultiSport Setup"		,92	

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
	VerticalSpeedFRStr,			// "Vertical Speed"			,63	
	TimeFRStr,					// "Time"					,64	
	SpeedPaceFRStr,				// "Speed/Pace"				,65	
	GradeFRStr,					// "Grade"					,66	
	CaloriesFRStr,				// "Calories"				,67	
	CadenceFRStr,				// "Cadence"				,68	
	NavigatetoLocationFRStr,	// "Navigate to Location"	,69	
	MapBreadcrumbFRStr,			// "Map & Breadcrumb"		,70	
	PowerAlertFRStr,			// "Power Alert"			,71	
	CadenceAlertFRStr,			// "Cadence Alert"			,72	
	HeartRateAlertFRStr,		// "Heart Rate Alert"		,73	
	SpeedPaceAlertFRStr,		// "Speed Pace Alert"		,74	
	TimeAlertFRStr,				// "Time Alert"				,75	
	DistanceAlertFRStr,			// "Distance Alert"			,76	
	CaloriesAlertFRStr,			// "Calories Alert"			,77	
	TrainingAlertsFRStr,		// "Training Alerts"		,78	
	PowerZonesFRStr,			// "Power Zones"			,79	
	HeartRateZonesFRStr,		// "Heart Rate Zones"		,80	
	TrainingZonesFRStr,			// "Training Zones"			,81	
	ActivityPacerScreenFRStr,	// "Activity Pacer Screen"	,82	
	ActivityDataScreensFRStr,	// "Activity Data Screens"	,83	
	AutoScrollFRStr,			// "Auto Scroll"			,84	
	DataScreensFRStr,			// "Data Screens"			,85	
	AutoPauseFRStr,				// "Auto Pause"				,86	
	AutoLapFRStr,				// "Auto Lap"				,87	
	AutoPauseFRStr,				// "Auto Pause"				,88	
	RecordingPreferencesFRStr,	// "Recording Preferences"	,89	
	SpeedPacePreferenceFRStr,	// "Speed Pace Preference"	,90	
	ProfileNameFRStr,			// "Profile Name"			,91	
	MultiSportSetupFRStr,		// "MultiSport Setup"		,92	

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
	VerticalSpeedITStr,			// "Vertical Speed"			,63	
	TimeITStr,					// "Time"					,64	
	SpeedPaceITStr,				// "Speed/Pace"				,65	
	GradeITStr,					// "Grade"					,66	
	CaloriesITStr,				// "Calories"				,67	
	CadenceITStr,				// "Cadence"				,68	
	NavigatetoLocationITStr,	// "Navigate to Location"	,69	
	MapBreadcrumbITStr,			// "Map & Breadcrumb"		,70	
	PowerAlertITStr,			// "Power Alert"			,71	
	CadenceAlertITStr,			// "Cadence Alert"			,72	
	HeartRateAlertITStr,		// "Heart Rate Alert"		,73	
	SpeedPaceAlertITStr,		// "Speed Pace Alert"		,74	
	TimeAlertITStr,				// "Time Alert"				,75	
	DistanceAlertITStr,			// "Distance Alert"			,76	
	CaloriesAlertITStr,			// "Calories Alert"			,77	
	TrainingAlertsITStr,		// "Training Alerts"		,78	
	PowerZonesITStr,			// "Power Zones"			,79	
	HeartRateZonesITStr,		// "Heart Rate Zones"		,80	
	TrainingZonesITStr,			// "Training Zones"			,81	
	ActivityPacerScreenITStr,	// "Activity Pacer Screen"	,82	
	ActivityDataScreensITStr,	// "Activity Data Screens"	,83	
	AutoScrollITStr,			// "Auto Scroll"			,84	
	DataScreensITStr,			// "Data Screens"			,85	
	AutoPauseITStr,				// "Auto Pause"				,86	
	AutoLapITStr,				// "Auto Lap"				,87	
	AutoPauseITStr,				// "Auto Pause"				,88	
	RecordingPreferencesITStr,	// "Recording Preferences"	,89	
	SpeedPacePreferenceITStr,	// "Speed Pace Preference"	,90	
	ProfileNameITStr,			// "Profile Name"			,91	
	MultiSportSetupITStr,		// "MultiSport Setup"		,92	

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
	VerticalSpeedPOStr,			// "Vertical Speed"			,63	
	TimePOStr,					// "Time"					,64	
	SpeedPacePOStr,				// "Speed/Pace"				,65	
	GradePOStr,					// "Grade"					,66	
	CaloriesPOStr,				// "Calories"				,67	
	CadencePOStr,				// "Cadence"				,68	
	NavigatetoLocationPOStr,	// "Navigate to Location"	,69	
	MapBreadcrumbPOStr,			// "Map & Breadcrumb"		,70	
	PowerAlertPOStr,			// "Power Alert"			,71	
	CadenceAlertPOStr,			// "Cadence Alert"			,72	
	HeartRateAlertPOStr,		// "Heart Rate Alert"		,73	
	SpeedPaceAlertPOStr,		// "Speed Pace Alert"		,74	
	TimeAlertPOStr,				// "Time Alert"				,75	
	DistanceAlertPOStr,			// "Distance Alert"			,76	
	CaloriesAlertPOStr,			// "Calories Alert"			,77	
	TrainingAlertsPOStr,		// "Training Alerts"		,78	
	PowerZonesPOStr,			// "Power Zones"			,79	
	HeartRateZonesPOStr,		// "Heart Rate Zones"		,80	
	TrainingZonesPOStr,			// "Training Zones"			,81	
	ActivityPacerScreenPOStr,	// "Activity Pacer Screen"	,82	
	ActivityDataScreensPOStr,	// "Activity Data Screens"	,83	
	AutoScrollPOStr,			// "Auto Scroll"			,84	
	DataScreensPOStr,			// "Data Screens"			,85	
	AutoPausePOStr,				// "Auto Pause"				,86	
	AutoLapPOStr,				// "Auto Lap"				,87	
	AutoPausePOStr,				// "Auto Pause"				,88	
	RecordingPreferencesPOStr,	// "Recording Preferences"	,89	
	SpeedPacePreferencePOStr,	// "Speed Pace Preference"	,90	
	ProfileNamePOStr,			// "Profile Name"			,91	
	MultiSportSetupPOStr,		// "MultiSport Setup"		,92	

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
	VerticalSpeedDUStr,			// "Vertical Speed"			,63	
	TimeDUStr,					// "Time"					,64	
	SpeedPaceDUStr,				// "Speed/Pace"				,65	
	GradeDUStr,					// "Grade"					,66	
	CaloriesDUStr,				// "Calories"				,67	
	CadenceDUStr,				// "Cadence"				,68	
	NavigatetoLocationDUStr,	// "Navigate to Location"	,69	
	MapBreadcrumbDUStr,			// "Map & Breadcrumb"		,70	
	PowerAlertDUStr,			// "Power Alert"			,71	
	CadenceAlertDUStr,			// "Cadence Alert"			,72	
	HeartRateAlertDUStr,		// "Heart Rate Alert"		,73	
	SpeedPaceAlertDUStr,		// "Speed Pace Alert"		,74	
	TimeAlertDUStr,				// "Time Alert"				,75	
	DistanceAlertDUStr,			// "Distance Alert"			,76	
	CaloriesAlertDUStr,			// "Calories Alert"			,77	
	TrainingAlertsDUStr,		// "Training Alerts"		,78	
	PowerZonesDUStr,			// "Power Zones"			,79	
	HeartRateZonesDUStr,		// "Heart Rate Zones"		,80	
	TrainingZonesDUStr,			// "Training Zones"			,81	
	ActivityPacerScreenDUStr,	// "Activity Pacer Screen"	,82	
	ActivityDataScreensDUStr,	// "Activity Data Screens"	,83	
	AutoScrollDUStr,			// "Auto Scroll"			,84	
	DataScreensDUStr,			// "Data Screens"			,85	
	AutoPauseDUStr,				// "Auto Pause"				,86	
	AutoLapDUStr,				// "Auto Lap"				,87	
	AutoPauseDUStr,				// "Auto Pause"				,88	
	RecordingPreferencesDUStr,	// "Recording Preferences"	,89	
	SpeedPacePreferenceDUStr,	// "Speed Pace Preference"	,90	
	ProfileNameDUStr,			// "Profile Name"			,91	
	MultiSportSetupDUStr,		// "MultiSport Setup"		,92	

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
	VerticalSpeedGEStr,			// "Vertical Speed"			,63	
	TimeGEStr,					// "Time"					,64	
	SpeedPaceGEStr,				// "Speed/Pace"				,65	
	GradeGEStr,					// "Grade"					,66	
	CaloriesGEStr,				// "Calories"				,67	
	CadenceGEStr,				// "Cadence"				,68	
	NavigatetoLocationGEStr,	// "Navigate to Location"	,69	
	MapBreadcrumbGEStr,			// "Map & Breadcrumb"		,70	
	PowerAlertGEStr,			// "Power Alert"			,71	
	CadenceAlertGEStr,			// "Cadence Alert"			,72	
	HeartRateAlertGEStr,		// "Heart Rate Alert"		,73	
	SpeedPaceAlertGEStr,		// "Speed Pace Alert"		,74	
	TimeAlertGEStr,				// "Time Alert"				,75	
	DistanceAlertGEStr,			// "Distance Alert"			,76	
	CaloriesAlertGEStr,			// "Calories Alert"			,77	
	TrainingAlertsGEStr,		// "Training Alerts"		,78	
	PowerZonesGEStr,			// "Power Zones"			,79	
	HeartRateZonesGEStr,		// "Heart Rate Zones"		,80	
	TrainingZonesGEStr,			// "Training Zones"			,81	
	ActivityPacerScreenGEStr,	// "Activity Pacer Screen"	,82	
	ActivityDataScreensGEStr,	// "Activity Data Screens"	,83	
	AutoScrollGEStr,			// "Auto Scroll"			,84	
	DataScreensGEStr,			// "Data Screens"			,85	
	AutoPauseGEStr,				// "Auto Pause"				,86	
	AutoLapGEStr,				// "Auto Lap"				,87	
	AutoPauseGEStr,				// "Auto Pause"				,88	
	RecordingPreferencesGEStr,	// "Recording Preferences"	,89	
	SpeedPacePreferenceGEStr,	// "Speed Pace Preference"	,90	
	ProfileNameGEStr,			// "Profile Name"			,91	
	MultiSportSetupGEStr,		// "MultiSport Setup"		,92	

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
	VerticalSpeedDAStr,			// "Vertical Speed"			,63	
	TimeDAStr,					// "Time"					,64	
	SpeedPaceDAStr,				// "Speed/Pace"				,65	
	GradeDAStr,					// "Grade"					,66	
	CaloriesDAStr,				// "Calories"				,67	
	CadenceDAStr,				// "Cadence"				,68	
	NavigatetoLocationDAStr,	// "Navigate to Location"	,69	
	MapBreadcrumbDAStr,			// "Map & Breadcrumb"		,70	
	PowerAlertDAStr,			// "Power Alert"			,71	
	CadenceAlertDAStr,			// "Cadence Alert"			,72	
	HeartRateAlertDAStr,		// "Heart Rate Alert"		,73	
	SpeedPaceAlertDAStr,		// "Speed Pace Alert"		,74	
	TimeAlertDAStr,				// "Time Alert"				,75	
	DistanceAlertDAStr,			// "Distance Alert"			,76	
	CaloriesAlertDAStr,			// "Calories Alert"			,77	
	TrainingAlertsDAStr,		// "Training Alerts"		,78	
	PowerZonesDAStr,			// "Power Zones"			,79	
	HeartRateZonesDAStr,		// "Heart Rate Zones"		,80	
	TrainingZonesDAStr,			// "Training Zones"			,81	
	ActivityPacerScreenDAStr,	// "Activity Pacer Screen"	,82	
	ActivityDataScreensDAStr,	// "Activity Data Screens"	,83	
	AutoScrollDAStr,			// "Auto Scroll"			,84	
	DataScreensDAStr,			// "Data Screens"			,85	
	AutoPauseDAStr,				// "Auto Pause"				,86	
	AutoLapDAStr,				// "Auto Lap"				,87	
	AutoPauseDAStr,				// "Auto Pause"				,88	
	RecordingPreferencesDAStr,	// "Recording Preferences"	,89	
	SpeedPacePreferenceDAStr,	// "Speed Pace Preference"	,90	
	ProfileNameDAStr,			// "Profile Name"			,91	
	MultiSportSetupDAStr,		// "MultiSport Setup"		,92	

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