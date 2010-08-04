/*
 *
 *
 *
 *
 *
 *
 */



#include <stdlib.h>
#include <string.h>
#include "tools.h"
#include "NMEAparser-def.h"
//#include "NMEAsentence.h"
//#include "NMEAcalculation.h"





unsigned Dump_sentence_field( NMEA_sentence_field_list* psentenceList )
{
	unsigned uRet = 0;
	unsigned uiLoopA = 0;
	unsigned uiLoopB = 0;

	psrprintf( "Dump_sentence_field start parsing 0x%x!!\r\n", psentenceList );

	for( uiLoopA = 0; uiLoopA < psentenceList->vaildfields; uiLoopA++ )
	{
		for( uiLoopB = 0; uiLoopB < psentenceList->Field[uiLoopA].FieldLen; uiLoopB++ )
			psrprintf( "%c", *(psentenceList->Field[uiLoopA].pField + uiLoopB) );
		psrprintf( "/" );
	}

	psrprintf( "\r\n" );
	return uRet;
}


unsigned Dump_sentence_singlefield( NMEA_sentence_field* psentenceField )
{
	unsigned uRet = 0;
	unsigned uiLoopB = 0;

	psrprintf( "Dump_sentence_singlefield start parsing 0x%x!!\r\n", psentenceField );
	if( psentenceField )
	{
		for( uiLoopB = 0; uiLoopB < psentenceField->FieldLen; uiLoopB++ )
			psrprintf( "%c", *(psentenceField->pField + uiLoopB) );

		///	psrprintf( "\r\n" );
	}
	else
		uRet = 1;


	return uRet;
}


#ifndef MPLAB_PIC24_PORT
int Handle_sentence_field_UTCtime( NMEA_sentence_field* psentenceField, gps_NMEA_session* pSession )
{
	int iRet = 0;
	float fRet = 0;

	if( psentenceField && pSession )
	{
		///iRet = atoi( psentenceField->pField );
		iRet = fRet = atof( psentenceField->pField );
		pSession->utc.second = iRet%100+(fRet-iRet);	///0~99
		pSession->utc.minute = (iRet%10000)/100;	///0~99
		pSession->utc.hour = iRet/10000;	///0~99
		pSession->uiIndex = pSession->uiIndex | NMEA_SESSION_UTC_OFFSET;
	}
	else
		iRet = (-1);

///	psrprintf( "Handle_sentence_field_UTCtime:UTC %d->%d:%d:%d\n", iRet, pSession->utc.hour, pSession->utc.minute, pSession->utc.second );
	return iRet;
}
#else
int Handle_sentence_field_UTCtime( NMEA_sentence_field* psentenceField, gps_NMEA_session* pSession )
{
	int iRet = 0;
	float fRet = 0;
	long lRet = 0;

	if( psentenceField && pSession )
	{
		///iRet = atoi( psentenceField->pField );
		lRet = fRet = atof( psentenceField->pField );
		pSession->utc.second = lRet%100+(fRet-lRet);	///0~99
		pSession->utc.minute = (lRet%10000)/100;	///0~99
		pSession->utc.hour = lRet/10000;	///0~99
		pSession->uiIndex = pSession->uiIndex | NMEA_SESSION_UTC_OFFSET;
	}
	else
		iRet = (-1);

///	psrprintf( "Handle_sentence_field_UTCtime:UTC %d->%d:%d:%d\n", iRet, pSession->utc.hour, pSession->utc.minute, pSession->utc.second );
	return iRet;
}
#endif	///#ifndef MPLAB_PIC24_PORT


#ifndef MPLAB_PIC24_PORT
int Handle_sentence_field_UTCdate( NMEA_sentence_field* psentenceField, gps_NMEA_session* pSession )
{
	int iRet = 0;

	if( psentenceField && pSession )
	{
		iRet = atoi( psentenceField->pField );
		pSession->utc.year = iRet%100;	///0~99
		pSession->utc.month = (iRet%10000)/100;	///0~99
		pSession->utc.day = iRet/10000;	///0~99
		pSession->uiIndex = pSession->uiIndex | NMEA_SESSION_UTC_DATE_OFFSET;
	}
	else
		iRet = (-1);

///	psrprintf( "Handle_sentence_field_UTCdate:UTC %d->%d:%d:%d\n", iRet, pSession->utc.year, pSession->utc.month, pSession->utc.day );
	return iRet;
}
#else
int Handle_sentence_field_UTCdate( NMEA_sentence_field* psentenceField, gps_NMEA_session* pSession )
{
	int iRet = 0;
	long lRet = 0;

	if( psentenceField && pSession )
	{
///		char *end;
///		char copyfield[16];
		
///		strncpy( copyfield, psentenceField->pField, psentenceField->FieldLen );
///		strncat( copyfield, "\0", 1 );
		
		lRet = atol( psentenceField->pField );
		///lRet = strtol( psentenceField->pField, &end, 10 );
		///lRet = strtol( copyfield, &end, 10 );
		///psrprintf( "----%d [%s]\n", lRet, end );
		///pSession->utc.year = lRet%100;	///0~99
		///psrprintf( "----%d\n", lRet%100 );
		///pSession->utc.month = (lRet%10000)/100;	///0~99
		///psrprintf( "----%d\n", (lRet%10000)/100 );
		///pSession->utc.day = lRet/10000;	///0~99
		///psrprintf( "----%d\n", lRet/10000 );
		pSession->uiIndex = pSession->uiIndex | NMEA_SESSION_UTC_DATE_OFFSET;
	}
	else
		iRet = (-1);

///	psrprintf( "Handle_sentence_field_UTCdate:UTC %d->%d:%d:%d\n", iRet, pSession->utc.year, pSession->utc.month, pSession->utc.day );
	return iRet;
}
#endif	///#ifndef MPLAB_PIC24_PORT


int Handle_sentence_field_Latitude( NMEA_sentence_field* psentenceField, gps_NMEA_session* pSession )
{
	int iRet = 0;
	double dRet = 0.1;
	unsigned uTmp = 0;

	if( psentenceField && pSession )
	{
		dRet = atof( psentenceField->pField );
		uTmp = dRet;
		uTmp = uTmp/100;
		pSession->coordinate.Latitude.degree = uTmp;
		pSession->coordinate.Latitude.minute = (dRet - uTmp*100);
		pSession->uiIndex = pSession->uiIndex | NMEA_SESSION_COORDINATE_OFFSET;
	}
	else
		iRet = (-1);

///	psrprintf( "Handle_sentence_field_Latitude:%f->%d:%f\n", dRet, pSession->coordinate.Latitude.degree, pSession->coordinate.Latitude.minute );
	return iRet;
}


int Handle_sentence_field_Longitude( NMEA_sentence_field* psentenceField, gps_NMEA_session* pSession )
{
	int iRet = 0;
	double dRet = 0.1;
	unsigned uTmp = 0;

	if( psentenceField && pSession )
	{
		dRet = atof( psentenceField->pField );
		uTmp = dRet;
		uTmp = uTmp/100;
		pSession->coordinate.Longitude.degree = uTmp;
		pSession->coordinate.Longitude.minute = (dRet - uTmp*100);
		pSession->uiIndex = pSession->uiIndex | NMEA_SESSION_COORDINATE_OFFSET;
	}
	else
		iRet = (-1);

///	psrprintf( "Handle_sentence_field_Longitude:%f->%d:%f\n", dRet, pSession->coordinate.Longitude.degree, pSession->coordinate.Longitude.minute );
	return iRet;
}


int Handle_sentence_field_Direct( NMEA_sentence_field* psentenceField, gps_NMEA_session* pSession )
{
	int iRet = 0;

	if( psentenceField && pSession )
	{
		if( 'E' ==  *psentenceField->pField )
			pSession->coordinate.Longitude.direct = E;
		else
		if( 'W' ==  *psentenceField->pField )
			pSession->coordinate.Longitude.direct = W;
		else
		if( 'S' ==  *psentenceField->pField )
			pSession->coordinate.Latitude.direct = S;
		else
		if( 'N' ==  *psentenceField->pField )
			pSession->coordinate.Latitude.direct = N;
		else
			;

		pSession->uiIndex = pSession->uiIndex | NMEA_SESSION_COORDINATE_OFFSET;
	}
	else
		iRet = (-1);

///	psrprintf( "Handle_sentence_field_Direct:%d \n", (0 == pSession->coordinate.Longitude.direct)?pSession->coordinate.Latitude.direct:pSession->coordinate.Longitude.direct );
	return iRet;
}


int Handle_sentence_field_DataStatus( NMEA_sentence_field* psentenceField, gps_NMEA_session* pSession )
{
	int iRet = 0;

	if( psentenceField && pSession )
	{
		if( 'A' ==  *psentenceField->pField )
			pSession->GPSdataStat = OK;
		else
		if( 'V' ==  *psentenceField->pField )
			pSession->GPSdataStat = WARNNING;
		else
			;

		pSession->uiIndex = pSession->uiIndex | NMEA_SESSION_GPSSTAT_OFFSET;
	}
	else
		iRet = (-1);

///	psrprintf( "Handle_sentence_field_DataStatus:%d \n", pSession->GPSdataStat );
	return iRet;
}


int Handle_sentence_field_FixQuality( NMEA_sentence_field* psentenceField, gps_NMEA_session* pSession )
{
	int iRet = 0;

	if( psentenceField && pSession )
	{
		pSession->fixQ = atoi( psentenceField->pField );
		pSession->uiIndex = pSession->uiIndex | NMEA_SESSION_FIXQOUAL_OFFSET;
	}
	else
		iRet = (-1);

///	psrprintf( "Handle_sentence_field_FixQuality:%d \n", pSession->fixQ );
	return iRet;
}


int Handle_sentence_field_FixMitac( NMEA_sentence_field* psentenceField, gps_NMEA_session* pSession )
{
	int iRet = 0;

	if( psentenceField && pSession )
	{
		if( 'A' ==  *psentenceField->pField )
			pSession->MfixType = Fix_Auto;
		else
		if( 'D' ==  *psentenceField->pField )
			pSession->MfixType = Fix_DGPS;
		else
		if( 'E' ==  *psentenceField->pField )
			pSession->MfixType = Fix_Emulate;
		else
		if( 'N' ==  *psentenceField->pField )
			pSession->MfixType = Fix_Invaild;
		else
			;

		pSession->uiIndex = pSession->uiIndex | NMEA_SESSION_MFIXTYPE_OFFSET;
	}
	else
		iRet = (-1);

///	psrprintf( "Handle_sentence_field_FixMitac:%d \n", pSession->MfixType );
	return iRet;
}


int Handle_sentence_field_FixType( NMEA_sentence_field* psentenceField, gps_NMEA_session* pSession )
{
	int iRet = 0;

	if( psentenceField && pSession )
	{
		if( '1' ==  *psentenceField->pField )
			pSession->fixType = Fix_No;
		else
		if( '2' ==  *psentenceField->pField )
			pSession->fixType = Fix_2D;
		else
		if( '3' ==  *psentenceField->pField )
			pSession->fixType = Fix_3D;
		else
			;

		pSession->uiIndex = pSession->uiIndex | NMEA_SESSION_FIXTYPE_OFFSET;
	}
	else
		iRet =(-1);

///	psrprintf( "Handle_sentence_field_FixType:%d \n", pSession->fixType );
	return iRet;
}


int Handle_sentence_field_SatellitesNum( NMEA_sentence_field* psentenceField, gps_NMEA_session* pSession )
{
	int iRet = 0;

	if( psentenceField && pSession )
	{
		pSession->SatelliteNum = atoi( psentenceField->pField );
		pSession->uiIndex = pSession->uiIndex | NMEA_SESSION_SALLITENUM_OFFSET;
	}
	else
		iRet = (-1);

///	psrprintf( "Handle_sentence_field_SatellitesNum:%d \n", pSession->SatelliteNum );
	return iRet;
}


int Handle_sentence_field_SatellitesID( int iID, NMEA_sentence_field* psentenceField, gps_NMEA_session* pSession )
{
	int iRet = 0;

	if( psentenceField && pSession )
	{
		pSession->SatelliteIDs[iID] = atoi( psentenceField->pField );
		pSession->uiIndex = pSession->uiIndex | NMEA_SESSION_SALLITEID_OFFSET;
	}
	else
		iRet = (-1);

///	psrprintf( "Handle_sentence_field_SatellitesID:%d \n", pSession->SatelliteIDs[iID] );
	return iRet;
}


int Handle_sentence_field_Altitude( NMEA_sentence_field* psentenceField, gps_NMEA_session* pSession )
{
	int iRet = 0;
	float dRet = 0.1;

	if( psentenceField && pSession )
	{
		dRet = atof( psentenceField->pField );

		pSession->Altitude = dRet;
		pSession->uiIndex = pSession->uiIndex | NMEA_SESSION_ALTITUDE_OFFSET;
	}
	else
		iRet = (-1);

///	psrprintf( "Handle_sentence_field_Altitude:%f->%f\n", dRet, pSession->Altitude );
	return iRet;
}


#ifndef MPLAB_PIC24_PORT
int Handle_sentence_field_GroundSpeed( NMEA_sentence_field* psentenceField, gps_NMEA_session* pSession )
{
	int iRet = 0;
	float dRet = 0.1;

	if( psentenceField && pSession )
	{
		dRet = atof( psentenceField->pField );

		pSession->GroundSpeed = dRet;
		pSession->uiIndex = pSession->uiIndex | NMEA_SESSION_GROUNDSPEED_OFFSET;
	}
	else
		iRet = (-1);

///	psrprintf( "Handle_sentence_field_GroundSpeed:%f->%f\n", dRet, pSession->GroundSpeed );
	return iRet;
}
#else
int Handle_sentence_field_GroundSpeed( NMEA_sentence_field* psentenceField, gps_NMEA_session* pSession )
{
	int iRet = 0;
	float dRet = 0.1;

	if( psentenceField && pSession )
	{
		dRet = atof( psentenceField->pField );

		pSession->GroundSpeed = dRet;
		pSession->uiIndex = pSession->uiIndex | NMEA_SESSION_GROUNDSPEED_OFFSET;
	}
	else
		iRet = (-1);

///	psrprintf( "Handle_sentence_field_GroundSpeed:%f->%f\n", dRet, pSession->GroundSpeed );
	return iRet;
}
#endif


int Handle_sentence_field_DegreeInTrack( NMEA_sentence_field* psentenceField, gps_NMEA_session* pSession )
{
	int iRet = 0;
	float dRet = 0.1;

	if( psentenceField && pSession )
	{
		dRet = atof( psentenceField->pField );

		pSession->TrackDegree = dRet;
		pSession->uiIndex = pSession->uiIndex | NMEA_SESSION_TRACKDEGREE_OFFSET;
	}
	else
		iRet = (-1);

///	psrprintf( "Handle_sentence_field_DegreeInTrack:%f->%f\n", dRet, pSession->TrackDegree );
	return iRet;
}
