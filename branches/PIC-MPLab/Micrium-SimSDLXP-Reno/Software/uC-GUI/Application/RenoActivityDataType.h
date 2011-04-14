
/*
	defination of activity data type
*/

#ifndef _RENOACTIVITYDATATYPE_H_
#define	_RENOACTIVITYDATATYPE_H_

/*
	please refer website below for more detail 
	http://magellan.onconfluence.com/display/FITNESS/Activity+Data
*/
#define			ACTDATA_ID_BASE				0x0020
#define			ACTDATA_ID_NONE				(ACTDATA_ID_BASE - 1)
#define			ACTDATA_ID_CADENCE			(ACTDATA_ID_BASE + 1)
#define			ACTDATA_ID_CALORIES			(ACTDATA_ID_BASE + 2)
#define			ACTDATA_ID_DISTANCE			(ACTDATA_ID_BASE + 3)
#define			ACTDATA_ID_ELEVATION		(ACTDATA_ID_BASE + 4)
#define			ACTDATA_ID_GRADE			(ACTDATA_ID_BASE + 5)
#define			ACTDATA_ID_HEARTRATE		(ACTDATA_ID_BASE + 6)
#define			ACTDATA_ID_POWER			(ACTDATA_ID_BASE + 7)
#define			ACTDATA_ID_TEMPERATURE		(ACTDATA_ID_BASE + 8)
#define			ACTDATA_ID_TIME				(ACTDATA_ID_BASE + 9)

/*
	Activity data system
*/
typedef struct
{
	int						iIndex;
	const GUI_ConstString	sDataName;
	const GUI_ConstString	sDataUnit;
} ACTDATA_SYS;

extern ACTDATA_SYS		ActivityDataSys[];




#endif	/* _RENOACTIVITYDATATYPE_H_ */