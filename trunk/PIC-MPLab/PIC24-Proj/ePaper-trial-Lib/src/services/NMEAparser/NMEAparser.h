/*




*/

#ifndef __NMEAPARSER_H__
#define __NMEAPARSER_H__

#include "mTypeDef.h"



#define NMEA_SENTENCE_MAX	91		/* max length of NMEA sentence */
#define SENTENCE_FIELD_MAX	20

/// nmeaParseRET error code define
#define 	NMEAPARSER_ERR_OK			(0)
#define 	NMEAPARSER_ERR_SENTENCE		(-1)
#define 	NMEAPARSER_ERR_SESSION		(-2)


typedef    unsigned int			nmeaParseRET;


typedef struct {
	BYTE hour;
	BYTE minute;
	BYTE second;
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
	float minute;
	NMEA_GPS_direct direct;
} NMEA_GPS_itude;


typedef struct {
	NMEA_GPS_itude Latitude;
	NMEA_GPS_itude Longitude;
} NMEA_GPS;



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
	unsigned 				Altitude;	/* in meter */
	unsigned 				GroundSpeed;
	unsigned 				TrackDegree;
} gps_NMEA_session;


typedef struct {
	char* pField;
	unsigned FieldLen;
} NMEA_sentence_field;


typedef struct {
	unsigned vaildfields;
	NMEA_sentence_field Field[SENTENCE_FIELD_MAX];	
} NMEA_sentence_field_list;


typedef bRET (*nmea_decoder) ( unsigned count, NMEA_sentence_field_list* psentenceList, gps_NMEA_session* pSession );

typedef struct {
	char *NMEAID;
	unsigned fieldNum;			/* minimum number of fields required to parse */
	nmea_decoder decoder;
} NMEA_sentence_decode_list;


nmeaParseRET NMEAparser(char *pcNMEAsentence, gps_NMEA_session* ptNMEAsession);


#endif	///#ifndef __NMEAPARSER_H__

