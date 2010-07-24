/*
 *
 *
 *
 *
 *
 *
 *
 *
 */

#ifndef __NMEAPARSER_DEF_H__
#define __NMEAPARSER_DEF_H__

/*




*/


///#include "mTypeDef.h"



#define NMEA_SENTENCE_MAX	91		/* max length of NMEA sentence */
#define SENTENCE_FIELD_MAX	20



/// nmeaParseRET error code define
#define 	NMEAPARSER_ERR_OK			(0)
#define 	NMEAPARSER_ERR_SENTENCE		(-1)
#define 	NMEAPARSER_ERR_SESSION		(-2)



typedef    unsigned int			nmeaParseRET;
typedef    unsigned char		BYTE;



typedef struct {
	BYTE year;
	BYTE month;
	BYTE day;
	BYTE hour;
	BYTE minute;
	///BYTE second;
	float second;
} NMEA_UTC;


typedef enum {
    N = 1,     /*  */
    S = 2,     /*  */
    E = 3,     /*  */
    W = 4,     /*  */
} NMEA_GPS_direct;


typedef enum {
    Invalid = 0,     /*  */
    GPS = 1,     /*  */
    DGPS = 2,     /*  */
    underCalc = 6,     /*  */
} NMEA_fixQ;


typedef enum {
    Fix_No = 1,     /*  */
    Fix_2D = 2,     /*  */
    Fix_3D = 3,     /*  */
} NMEA_fixType;


typedef enum {
    Fix_Auto = 1,     /* auto */
    Fix_DGPS = 2,     /* DGPS fix */
    Fix_Emulate = 3,     /* under calc */
	Fix_Invaild = 4,     /* invaild */
} Mitac_fixType;


typedef enum {
    OK = 1,     /*  */
    WARNNING = 2,     /*  */
} NMEA_GPS_dataState;


typedef struct {
	unsigned degree;
	double minute;
	NMEA_GPS_direct direct;
} NMEA_GPS_itude;


typedef struct {
	NMEA_GPS_itude Latitude;
	NMEA_GPS_itude Longitude;
} NMEA_GPS;


#define 	NMEA_SESSION_OFFSET_BASE				0x1
#define 	NMEA_SESSION_UTC_OFFSET					(NMEA_SESSION_OFFSET_BASE<<1)
#define 	NMEA_SESSION_COORDINATE_OFFSET			(NMEA_SESSION_OFFSET_BASE<<2)
#define 	NMEA_SESSION_GPSSTAT_OFFSET				(NMEA_SESSION_OFFSET_BASE<<3)
#define 	NMEA_SESSION_FIXQOUAL_OFFSET			(NMEA_SESSION_OFFSET_BASE<<4)
#define 	NMEA_SESSION_FIXTYPE_OFFSET				(NMEA_SESSION_OFFSET_BASE<<5)
#define 	NMEA_SESSION_MFIXTYPE_OFFSET			(NMEA_SESSION_OFFSET_BASE<<6)
#define 	NMEA_SESSION_SALLITENUM_OFFSET			(NMEA_SESSION_OFFSET_BASE<<7)
#define 	NMEA_SESSION_SALLITEID_OFFSET			(NMEA_SESSION_OFFSET_BASE<<8)
#define 	NMEA_SESSION_ALTITUDE_OFFSET			(NMEA_SESSION_OFFSET_BASE<<9)
#define 	NMEA_SESSION_GROUNDSPEED_OFFSET			(NMEA_SESSION_OFFSET_BASE<<10)
#define 	NMEA_SESSION_TRACKDEGREE_OFFSET			(NMEA_SESSION_OFFSET_BASE<<11)
#define 	NMEA_SESSION_UTC_DATE_OFFSET			(NMEA_SESSION_OFFSET_BASE<<12)

#define 	NMEA_SESSION_MAX_OFFSET					(NMEA_SESSION_OFFSET_BASE<<15)


///struct for all necessary data report from NMEA
typedef struct {
	unsigned 				uiIndex;
	NMEA_UTC 				utc;
	NMEA_GPS 				coordinate;
	NMEA_GPS_dataState 		GPSdataStat;
	NMEA_fixQ 				fixQ;
	NMEA_fixType 			fixType;
	Mitac_fixType			MfixType;
	BYTE 					SatelliteNum;
	BYTE 					SatelliteIDs[12];
	float 					Altitude;	/* in meter */
	float	 				GroundSpeed;
	float 					TrackDegree;
} gps_NMEA_session;


typedef struct {
	char* pField;
	BYTE FieldLen;
} NMEA_sentence_field;


typedef struct {
	BYTE vaildfields;
	NMEA_sentence_field Field[SENTENCE_FIELD_MAX];
} NMEA_sentence_field_list;


typedef int (*nmea_decoder) ( unsigned count, NMEA_sentence_field_list* psentenceList, gps_NMEA_session* pSession );

typedef struct {
	char *NMEAID;
	BYTE fieldNum;			/* minimum number of fields required to parse */
	nmea_decoder decoder;
} NMEA_sentence_decode_list;

typedef int (*Handle_sentence_fields) ( NMEA_sentence_field* psentenceField, gps_NMEA_session* pSession );

typedef struct {
	BYTE num;
	BYTE valid;			/* field handle assigned */
	Handle_sentence_fields fields_handle;
} NMEA_sentence_handle_list;


#endif	///#ifndef __NMEAPARSER_DEF_H__

