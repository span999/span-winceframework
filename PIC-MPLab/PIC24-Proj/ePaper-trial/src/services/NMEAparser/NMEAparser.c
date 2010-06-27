/*




*/


#include "SystemProfile.h"
#include "Compiler.h"
#include "GenericTypeDefs.h"
#include "mFreeRTOSDef.h"
#include "mTypeDef.h"
#include "mDrvIncludes.h"


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









static void psrprintf( char* msg )
{
	UARTprintf( msg );
}


static bRET pointerValid( void* pPointer, char* pcErrorMsg )
{
	bRET bRet = bRET_TRUE;

	if( NULL == pPointer )
	{
		psrprintf( pcErrorMsg );
		bRet = bRET_FALSE;
	}	
		
	return bRet;
}


#if defined(_RAW_FUNCTION_)

#else
static bRET Clean_pointerlength( void* pPointer, unsigned uLen )
{
	bRET bRet = bRET_FALSE;
	
	if( NULL != pPointer )
	{
		memset( pPointer, 0, uLen );
		bRET bRet = bRET_TRUE;
	}
	
	return bRet;
}
#endif


static bRET Clean_gps_NMEA_session( gps_NMEA_session* pSession )
{
#if defined(_RAW_FUNCTION_)
	bRET bRet = bRET_FALSE;
	
	if( NULL != pSession )
	{
		memset( pSession, 0, sizeof(gps_NMEA_session) );
		bRET bRet = bRET_TRUE;
	}
	
	return bRet;
#else	
	return Clean_pointerlength( pSession, sizeof(gps_NMEA_session) );
#endif
}


static bRET Clean_sentence_field_list( NMEA_sentence_field_list* pList )
{
#if defined(_RAW_FUNCTION_)
	bRET bRet = bRET_FALSE;
	
	if( NULL != pList )
	{
		memset( pList, 0, sizeof(NMEA_sentence_field_list) );
		bRET bRet = bRET_TRUE;
	}
	
	return bRet;
#else
	return Clean_pointerlength( pList, sizeof(NMEA_sentence_field_list) );
#endif	
}


static unsigned Copy_sentence_field( char* pSentenceNow, NMEA_sentence_field_list* psentenceList )
{
	unsigned uRet = 0;
	unsigned uiLoopB = 0;
	char commassign = ',';
	
	psentenceList->Field[psentenceList->vaildfields].pField = pSentenceNow;
	psentenceList->Field[psentenceList->vaildfields].FieldLen = 1;
	for( uiLoopB = 0; uiLoopB < NMEA_SENTENCE_MAX; uiLoopB++ )
	{
		pSentenceNow++;
		if( commassign != *pSentenceNow )
			psentenceList->Field[psentenceList->vaildfields].FieldLen++;
		else	///','
		{
			uRet = 	uiLoopB + 1;
			break;
		}
	}
	psentenceList->vaildfields++;
	
	return uRet;
}

static bRET parseGPGGA( unsigned count, NMEA_sentence_field_list* psentenceList, gps_NMEA_session* pSession )
{
	bRET bRet = bRET_TRUE;
	
	return bRet;
}


static bRET parseGPGSA( unsigned count, NMEA_sentence_field_list* psentenceList, gps_NMEA_session* pSession )
{
	bRET bRet = bRET_TRUE;
	
	return bRet;
}


static bRET parseGPRMC( unsigned count, NMEA_sentence_field_list* psentenceList, gps_NMEA_session* pSession )
{
	bRET bRet = bRET_TRUE;
	
	return bRet;
}


static NMEA_sentence_decode_list Decoder_list[] = {
	{ "GPGGA", 16, parseGPGGA },
	{ "GPGSA", 18, parseGPGSA },
	{ "GPRMC", 13, parseGPRMC },
	{ "ENDGP",  0, NULL },
};



/* parse an NMEA sentence, unpack it into a session structure */
nmeaParseRET NMEAparser(char *pcNMEAsentence, gps_NMEA_session* ptNMEAsession)
{
	nmeaParseRET parserRet = NMEAPARSER_ERR_OK;
	unsigned uiSentenceLen = 0;
	char startsign = '$';
///	char commassign = ',';
	char firstfieldsign = 'G';
	unsigned uiLoopA = 0;
	unsigned uiLoopB = 0;
	char *pSentenceNow = NULL;
	NMEA_sentence_field_list sentenceList;

	
	if( !pointerValid( pcNMEAsentence, "pcNMEAsentence Err!!" ) )
		return NMEAPARSER_ERR_SENTENCE;
	if( !pointerValid( ptNMEAsession, "ptNMEAsession Err!!" ) )
		return NMEAPARSER_ERR_SESSION;

	///get sentence length
	uiSentenceLen = strlen(pcNMEAsentence);
    if( uiSentenceLen > NMEA_SENTENCE_MAX )
	{
		psrprintf( "Overlong packet rejected!!\r\n" );
		return NMEAPARSER_ERR_SENTENCE;
    }
		
	Clean_gps_NMEA_session( ptNMEAsession );
	Clean_sentence_field_list( &sentenceList );

	///check '$' sign
	for( uiLoopA = 0; uiLoopA < uiSentenceLen; uiLoopA++ )
	{
		pSentenceNow = pcNMEAsentence + uiLoopA;
		if( *pSentenceNow == startsign )	///'$'
		{
			pSentenceNow++;	///move to next
			///reset field index
			sentenceList.vaildfields = 0;
			if( *pSentenceNow == firstfieldsign )	///'G'
			{
				///start to find all field
				unsigned uiTemp = 0;
				for( uiLoopB = 0; uiLoopB < uiSentenceLen; uiLoopB++ )
				{
					pSentenceNow = pSentenceNow + uiTemp;
					uiTemp = Copy_sentence_field( pSentenceNow, &sentenceList );
					uiLoopB = uiLoopB + uiTemp;
				}
				break;
			}
		}
		///try next char
	}
	
	
	///parse NMEA sentense with proper decoder 
	if( sentenceList.vaildfields > 0 )
	{
		uiLoopA = 0;
		while( NULL != Decoder_list[uiLoopA].decoder )
		{
			if( strcmp( Decoder_list[uiLoopA].NMEAID, sentenceList.Field[0].pField ) == 0)
			{
				///analyis field data
				Decoder_list[uiLoopA].decoder( Decoder_list[uiLoopA].fieldNum, &sentenceList, ptNMEAsession );
				break;
			}
			uiLoopA++;
		}
	}
	
	return parserRet;
}
