/*




*/



#include <string.h> 
#include <stdio.h>
#include "NMEAparser-def.h"





/*
	private routine for internal use
	prefix with 'static'
*/


static void psrprintf( char* msg )
{
///	UARTprintf( msg );
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
		bRet = bRET_TRUE;
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


static unsigned Copy_sentence_field( char* pSentenceNow, unsigned uiSentenceLen, NMEA_sentence_field_list* psentenceList )
{
	unsigned uRet = 0;
	unsigned uiLoopB = 0;
	char commassign = ',';
	///unsigned uiTemp = 0;

	///reset field index
	psentenceList->vaildfields = 0;
	
	///copy first field
	psentenceList->Field[psentenceList->vaildfields].pField = pSentenceNow;
	psentenceList->Field[psentenceList->vaildfields].FieldLen = 0;
	uRet++;
	
	for( uiLoopB = 0; uiLoopB < uiSentenceLen; uiLoopB++ )
	{
		pSentenceNow = pSentenceNow + uiLoopB;
		if( commassign != *pSentenceNow )
		{
			psentenceList->Field[psentenceList->vaildfields].FieldLen++;
		}
		else	///','
		{
			psentenceList->vaildfields++;	///next field
			psentenceList->Field[psentenceList->vaildfields].pField = pSentenceNow + 1;
			psentenceList->Field[psentenceList->vaildfields].FieldLen = 0;
			uRet++;
		}
	}	
	
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


/*
	public routine for external use
	must export to other
*/


/* parse an NMEA sentence, unpack it into a session structure */
nmeaParseRET NMEAparser(char *pcNMEAsentence, gps_NMEA_session* ptNMEAsession)
{
	nmeaParseRET parserRet = NMEAPARSER_ERR_OK;
	unsigned uiSentenceLen = 0;
	char startsign = '$';
///	char commassign = ',';
	char firstfieldsign = 'G';
	unsigned uiLoopA = 0;
///	unsigned uiLoopB = 0;
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
			if( *pSentenceNow == firstfieldsign )	///'G'
			{
				///start to find and copy all field
				Copy_sentence_field( pSentenceNow, uiSentenceLen, &sentenceList );
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
