/*
 *
 *
 *
 *
 *
 *
 */


///#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tools.h"
#include "NMEAparser-def.h"
#include "NMEAsentence.h"






static int Clean_gps_NMEA_session( gps_NMEA_session* pSession )
{
	return Clean_pointerlength( pSession, sizeof(gps_NMEA_session) );
}


static int Clean_sentence_field_list( NMEA_sentence_field_list* pList )
{
	return Clean_pointerlength( pList, sizeof(NMEA_sentence_field_list) );
}


static unsigned Copy_sentence_field( char* pSentenceNow, unsigned uiSentenceLen, NMEA_sentence_field_list* psentenceList )
{
	unsigned uRet = 0;
	unsigned uiLoopB = 0;
	char commassign = ',';
	char chksumsign = '*';
	char *pSentenceTmp;


	///psrprintf( "Copy_sentence_field start parsing 0x%x %d 0x%x!!\r\n", pSentenceNow, uiSentenceLen, psentenceList );

	///reset field index
	psentenceList->vaildfields = 0;

	///copy first field
	psentenceList->Field[psentenceList->vaildfields].pField = pSentenceNow;
	psentenceList->Field[psentenceList->vaildfields].FieldLen = 0;
	uRet++;

	for( uiLoopB = 0; uiLoopB < uiSentenceLen; uiLoopB++ )
	{
		pSentenceTmp = pSentenceNow + uiLoopB;
		if( commassign != *pSentenceTmp && chksumsign != *pSentenceTmp )
		{
			psentenceList->Field[psentenceList->vaildfields].FieldLen++;
		}
		else	///',' or '*'
		{
			psentenceList->vaildfields++;	///next field
			psentenceList->Field[psentenceList->vaildfields].pField = pSentenceTmp + 1;
			psentenceList->Field[psentenceList->vaildfields].FieldLen = 0;
			uRet++;
		}
	}

	if( 0 != psentenceList->Field[psentenceList->vaildfields].FieldLen )
		psentenceList->vaildfields++;

///	psrprintf( "Copy_sentence_field end copied %d-%d field!!\r\n", uRet, psentenceList->vaildfields );
	return uRet;
}


static int Dump_gps_NMEA_session( gps_NMEA_session* pSession )
{
	int iRet = 0;

	if( pSession )
	{
		int iLoop = 0;

		psrprintf( "============================================================\n" );
		psrprintf( "uiIndex=0x%x\n", pSession->uiIndex );
		psrprintf( "UTC=%d:%d:%d\n", pSession->utc.hour, pSession->utc.minute, pSession->utc.second );
		psrprintf( "Latitude=%d-%f ", pSession->coordinate.Latitude.degree, pSession->coordinate.Latitude.minute );
		if( N == pSession->coordinate.Latitude.direct )
			psrprintf( "[N]\n" );
		else
		if( S == pSession->coordinate.Latitude.direct )
			psrprintf( "[S]\n" );
		else
		if( E == pSession->coordinate.Latitude.direct )
			psrprintf( "[E]\n" );
		else
		if( W == pSession->coordinate.Latitude.direct )
			psrprintf( "[W]\n" );
		else
			psrprintf( "[???]\n" );

		psrprintf( "Longitude=%d-%f ", pSession->coordinate.Longitude.degree, pSession->coordinate.Longitude.minute );
		if( N == pSession->coordinate.Longitude.direct )
			psrprintf( "[N]\n" );
		else
		if( S == pSession->coordinate.Longitude.direct )
			psrprintf( "[S]\n" );
		else
		if( E == pSession->coordinate.Longitude.direct )
			psrprintf( "[E]\n" );
		else
		if( W == pSession->coordinate.Longitude.direct )
			psrprintf( "[W]\n" );
		else
			psrprintf( "[???]\n" );

		psrprintf( "GPSdataStat=%d [%s]\n", pSession->GPSdataStat, (OK == pSession->GPSdataStat)?"OK":"WARNNING" );

		psrprintf( "fixQ=%d ", pSession->fixQ );
		if( Invalid == pSession->fixQ )
			psrprintf( "[Invalid]\n" );
		else
		if( GPS == pSession->fixQ )
			psrprintf( "[GPS]\n" );
		else
		if( DGPS == pSession->fixQ )
			psrprintf( "[DGPS]\n" );
		else
		if( underCalc == pSession->fixQ )
			psrprintf( "[underCalc]\n" );
		else
			psrprintf( "[???]\n" );

		psrprintf( "fixType=%d ", pSession->fixType );
		if( Fix_No == pSession->fixType )
			psrprintf( "[Fix_No]\n" );
		else
		if( Fix_2D == pSession->fixType )
			psrprintf( "[Fix_2D]\n" );
		else
		if( Fix_3D == pSession->fixType )
			psrprintf( "[Fix_3D]\n" );
		else
			psrprintf( "[???]\n" );

		psrprintf( "MfixType=%d ", pSession->MfixType );
		if( Fix_Auto == pSession->MfixType )
			psrprintf( "[Fix_Auto]\n" );
		else
		if( Fix_DGPS == pSession->MfixType )
			psrprintf( "[Fix_DGPS]\n" );
		else
		if( Fix_Emulate == pSession->MfixType )
			psrprintf( "[Fix_Emulate]\n" );
		else
		if( Fix_Invaild == pSession->MfixType )
			psrprintf( "[Fix_Invaild]\n" );
		else
			psrprintf( "[???]\n" );

		psrprintf( "SatelliteNum=%d\n", pSession->SatelliteNum );
		psrprintf( "SatelliteID=" );
		for( iLoop=0; iLoop<12; iLoop++ )
			psrprintf( " %d", pSession->SatelliteIDs[iLoop] );
		psrprintf( "\n" );
		psrprintf( "Altitude=%f\n", pSession->Altitude );
		psrprintf( "GroundSpeed=%f\n", pSession->GroundSpeed );
		psrprintf( "TrackDegree=%f\n", pSession->TrackDegree );

		psrprintf( "============================================================\n" );
	}

	return iRet;
}


static unsigned Dump_sentence_field( NMEA_sentence_field_list* psentenceList )
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


static unsigned Dump_sentence_singlefield( NMEA_sentence_field* psentenceField )
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


static int Handle_sentence_field_UTCtime( NMEA_sentence_field* psentenceField, gps_NMEA_session* pSession )
{
	int iRet = 0;

	if( psentenceField && pSession )
	{
		iRet = atoi( psentenceField->pField );
		pSession->utc.second = iRet%100;	///0~99
		pSession->utc.minute = (iRet%10000)/100;	///0~99
		pSession->utc.hour = iRet/10000;	///0~99
		pSession->uiIndex = pSession->uiIndex | NMEA_SESSION_UTC_OFFSET;
	}
	else
		iRet = (-1);

///	psrprintf( "Handle_sentence_field_UTCtime:UTC %d->%d:%d:%d\n", iRet, pSession->utc.hour, pSession->utc.minute, pSession->utc.second );
	return iRet;
}


static int Handle_sentence_field_Latitude( NMEA_sentence_field* psentenceField, gps_NMEA_session* pSession )
{
	int iRet = 0;
	float dRet = 0.1;
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


static int Handle_sentence_field_Longitude( NMEA_sentence_field* psentenceField, gps_NMEA_session* pSession )
{
	int iRet = 0;
	float dRet = 0.1;
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


static int Handle_sentence_field_Direct( NMEA_sentence_field* psentenceField, gps_NMEA_session* pSession )
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


static int Handle_sentence_field_DataStatus( NMEA_sentence_field* psentenceField, gps_NMEA_session* pSession )
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


static int Handle_sentence_field_FixQuality( NMEA_sentence_field* psentenceField, gps_NMEA_session* pSession )
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


static int Handle_sentence_field_FixMitac( NMEA_sentence_field* psentenceField, gps_NMEA_session* pSession )
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


static int Handle_sentence_field_FixType( NMEA_sentence_field* psentenceField, gps_NMEA_session* pSession )
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


static int Handle_sentence_field_SatellitesNum( NMEA_sentence_field* psentenceField, gps_NMEA_session* pSession )
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


static int Handle_sentence_field_SatellitesID( int iID, NMEA_sentence_field* psentenceField, gps_NMEA_session* pSession )
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


static int Handle_sentence_field_Altitude( NMEA_sentence_field* psentenceField, gps_NMEA_session* pSession )
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


static int Handle_sentence_field_GroundSpeed( NMEA_sentence_field* psentenceField, gps_NMEA_session* pSession )
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


static int Handle_sentence_field_DegreeInTrack( NMEA_sentence_field* psentenceField, gps_NMEA_session* pSession )
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


static int parseGPGGA( unsigned count, NMEA_sentence_field_list* psentenceList, gps_NMEA_session* pSession )
{
	int iRet = 0;
	unsigned uiLoopA = 0;

///	psrprintf( "parseGPGGA:start parsing %d 0x%x 0x%x!!\r\n", count, psentenceList, pSession );

	if( !psentenceList || !pSession)
		return (-1);

	for( uiLoopA = 0; uiLoopA < count; uiLoopA++ )
	{
		if( psentenceList->vaildfields > uiLoopA )
		{
///			psrprintf( "---%d/", uiLoopA );
///			Dump_sentence_singlefield( &(psentenceList->Field[uiLoopA]) );
///			psrprintf( "\n" );

			if( 0 == uiLoopA )
			{	///ID, ignored

			}
			else
			if( 1 == uiLoopA )
			{	///Time,
				Handle_sentence_field_UTCtime( &(psentenceList->Field[uiLoopA]), pSession );
			}
			else
			if( 2 == uiLoopA )
			{	//Latitude
				Handle_sentence_field_Latitude( &(psentenceList->Field[uiLoopA]), pSession );
			}
			else
			if( 3 == uiLoopA )
			{	//Latitude
				Handle_sentence_field_Direct( &(psentenceList->Field[uiLoopA]), pSession );
			}
			else
			if( 4 == uiLoopA )
			{	//Longitude
				Handle_sentence_field_Longitude( &(psentenceList->Field[uiLoopA]), pSession );
			}
			else
			if( 5 == uiLoopA )
			{	//Longitude
				Handle_sentence_field_Direct( &(psentenceList->Field[uiLoopA]), pSession );
			}
			else
			if( 6 == uiLoopA )
			{	//Fix Quality
				Handle_sentence_field_FixQuality( &(psentenceList->Field[uiLoopA]), pSession );
			}
			else
			if( 7 == uiLoopA )
			{	//Number of satellites
				Handle_sentence_field_SatellitesNum( &(psentenceList->Field[uiLoopA]), pSession );
			}
			else
			if( 8 == uiLoopA )
			{	//Horizontal Dilution of Precision (HDOP)

			}
			else
			if( 9 == uiLoopA )
			{	//Altitude
				Handle_sentence_field_Altitude( &(psentenceList->Field[uiLoopA]), pSession );
			}
			else
			if( 10 == uiLoopA )
			{	//Altitude

			}
			else
			if( 11 == uiLoopA )
			{	//Height of geoid

			}
			else
			if( 12 == uiLoopA )
			{	//Height of geoid

			}
			else
			if( 13 == uiLoopA )
			{	//

			}
			else
			if( 14 == uiLoopA )
			{	//

			}
			else
			if( 15 == uiLoopA )
			{	//

			}

		}
	}

///	psrprintf( "\r\n" );

	return iRet;
}


static int parseGPGSA( unsigned count, NMEA_sentence_field_list* psentenceList, gps_NMEA_session* pSession )
{
	int iRet = 0;
	unsigned uiLoopA = 0;

///	psrprintf( "parseGPGSA:start parsing %d 0x%x 0x%x!!\r\n", count, psentenceList, pSession );

	if( !psentenceList || !pSession)
		return (-1);


	for( uiLoopA = 0; uiLoopA < count; uiLoopA++ )
	{
		if( psentenceList->vaildfields > uiLoopA )
		{
///			psrprintf( "---%d/", uiLoopA );
///			Dump_sentence_singlefield( &(psentenceList->Field[uiLoopA]) );
///			psrprintf( "\n" );

			if( 0 == uiLoopA )
			{	///ID, ignored

			}
			else
			if( 1 == uiLoopA )
			{	///Fix mode,

			}
			else
			if( 2 == uiLoopA )
			{	//Fix type
				Handle_sentence_field_FixType( &(psentenceList->Field[uiLoopA]), pSession );
			}
			else
			if( 3 == uiLoopA )
			{	//ID1
				Handle_sentence_field_SatellitesID( (uiLoopA-3), &(psentenceList->Field[uiLoopA]), pSession );
			}
			else
			if( 4 == uiLoopA )
			{	//ID2
				Handle_sentence_field_SatellitesID( (uiLoopA-3), &(psentenceList->Field[uiLoopA]), pSession );
			}
			else
			if( 5 == uiLoopA )
			{	//ID3
				Handle_sentence_field_SatellitesID( (uiLoopA-3), &(psentenceList->Field[uiLoopA]), pSession );
			}
			else
			if( 6 == uiLoopA )
			{	//ID4
				Handle_sentence_field_SatellitesID( (uiLoopA-3), &(psentenceList->Field[uiLoopA]), pSession );
			}
			else
			if( 7 == uiLoopA )
			{	//ID5
				Handle_sentence_field_SatellitesID( (uiLoopA-3), &(psentenceList->Field[uiLoopA]), pSession );
			}
			else
			if( 8 == uiLoopA )
			{	//ID6
				Handle_sentence_field_SatellitesID( (uiLoopA-3), &(psentenceList->Field[uiLoopA]), pSession );
			}
			else
			if( 9 == uiLoopA )
			{	//ID7
				Handle_sentence_field_SatellitesID( (uiLoopA-3), &(psentenceList->Field[uiLoopA]), pSession );
			}
			else
			if( 10 == uiLoopA )
			{	//ID8
				Handle_sentence_field_SatellitesID( (uiLoopA-3), &(psentenceList->Field[uiLoopA]), pSession );
			}
			else
			if( 11 == uiLoopA )
			{	//ID9
				Handle_sentence_field_SatellitesID( (uiLoopA-3), &(psentenceList->Field[uiLoopA]), pSession );
			}
			else
			if( 12 == uiLoopA )
			{	//ID10
				Handle_sentence_field_SatellitesID( (uiLoopA-3), &(psentenceList->Field[uiLoopA]), pSession );
			}
			else
			if( 13 == uiLoopA )
			{	//ID11
				Handle_sentence_field_SatellitesID( (uiLoopA-3), &(psentenceList->Field[uiLoopA]), pSession );
			}
			else
			if( 14 == uiLoopA )
			{	//ID12
				Handle_sentence_field_SatellitesID( (uiLoopA-3), &(psentenceList->Field[uiLoopA]), pSession );
			}

		}
	}

///	psrprintf( "\r\n" );

	return iRet;
}


static int parseGPRMC( unsigned count, NMEA_sentence_field_list* psentenceList, gps_NMEA_session* pSession )
{
	int iRet = 0;
	unsigned uiLoopA = 0;

///	psrprintf( "parseGPRMC:start parsing %d 0x%x 0x%x!!\r\n", count, psentenceList, pSession );

	if( !psentenceList || !pSession)
		return (-1);


	for( uiLoopA = 0; uiLoopA < count; uiLoopA++ )
	{
		if( psentenceList->vaildfields > uiLoopA )
		{
///			psrprintf( "---%d/", uiLoopA );
///			Dump_sentence_singlefield( &(psentenceList->Field[uiLoopA]) );
///			psrprintf( "\n" );

			if( 0 == uiLoopA )
			{	///ID, ignored

			}
			else
			if( 1 == uiLoopA )
			{	///Time,
				Handle_sentence_field_UTCtime( &(psentenceList->Field[uiLoopA]), pSession );
			}
			else
			if( 2 == uiLoopA )
			{	//data status
				Handle_sentence_field_DataStatus( &(psentenceList->Field[uiLoopA]), pSession );
			}
			else
			if( 3 == uiLoopA )
			{	//Latitude
				Handle_sentence_field_Latitude( &(psentenceList->Field[uiLoopA]), pSession );
			}
			else
			if( 4 == uiLoopA )
			{	//Latitude
				Handle_sentence_field_Direct( &(psentenceList->Field[uiLoopA]), pSession );
			}
			else
			if( 5 == uiLoopA )
			{	//Longitude
				Handle_sentence_field_Longitude( &(psentenceList->Field[uiLoopA]), pSession );
			}
			else
			if( 6 == uiLoopA )
			{	//Longitude
				Handle_sentence_field_Direct( &(psentenceList->Field[uiLoopA]), pSession );
			}
			else
			if( 7 == uiLoopA )
			{	//speed over ground
				Handle_sentence_field_GroundSpeed( &(psentenceList->Field[uiLoopA]), pSession );
			}
			else
			if( 8 == uiLoopA )
			{	//degree in track
				Handle_sentence_field_DegreeInTrack( &(psentenceList->Field[uiLoopA]), pSession );
			}
			else
			if( 9 == uiLoopA )
			{	//UT update

			}
			else
			if( 10 == uiLoopA )
			{	//Magnetic variation degrees

			}
			else
			if( 11 == uiLoopA )
			{	//Magnetic variation degrees

			}
			else
			if( 12 == uiLoopA )
			{	//Mitac fix
				Handle_sentence_field_FixMitac( &(psentenceList->Field[uiLoopA]), pSession );
			}

		}
	}

///	psrprintf( "\r\n" );


	return iRet;
}

#if 0
static NMEA_sentence_decode_list Decoder_list[] = {
	{ "GPGGA", 16, parseGPGGA },
	{ "GPGSA", 19, parseGPGSA },
	{ "GPRMC", 13, parseGPRMC },
	{ "ENDGP",  0, NULL },
};
#else
static NMEA_sentence_decode_list Decoder_list[] = {
	{ "GPGGA", 16, parseGPGGA },
	{ "GPGSA", 19, parseGPGSA },
	{ "GPRMC", 13, parseGPRMC },
	{ "ENDGP",  0, NULL },
};
#endif



#define CHECK_GP_SIGN

/* parse an NMEA sentence, unpack it into a session structure */
nmeaParseRET NMEAparser(char *pcNMEAsentence, gps_NMEA_session* ptNMEAsession)
{
	nmeaParseRET parserRet = NMEAPARSER_ERR_OK;
	unsigned uiSentenceLen = 0;
#ifndef CHECK_GP_SIGN
	char startsign = '$';
	char firstfieldsign = 'G';
#endif
	unsigned uiLoopA = 0;
	char *pSentenceNow = NULL;
	NMEA_sentence_field_list sentenceList;


	///psrprintf( "NMEAparser start parsing 0x%x 0x%x!!\r\n", pcNMEAsentence, ptNMEAsession );


	if( 0 != pointerValid( pcNMEAsentence, "pcNMEAsentence Err!!" ) )
		return NMEAPARSER_ERR_SENTENCE;
	if( 0 != pointerValid( ptNMEAsession, "ptNMEAsession Err!!" ) )
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

	psrprintf( ">>> %s>>>len=%d\n", pcNMEAsentence, uiSentenceLen );

	///check '$' sign
	for( uiLoopA = 0; uiLoopA < uiSentenceLen; uiLoopA++ )
	{
		pSentenceNow = pcNMEAsentence + uiLoopA;
		///psrprintf( "%c-%d", pSentenceNow, uiLoopA );
#ifndef CHECK_GP_SIGN
		if( *pSentenceNow == startsign )	///'$'
		{
			///psrprintf( "got $ sign!!!\r\n" );
			pSentenceNow++;	///move to next
			if( *pSentenceNow == firstfieldsign )	///'G'
			{
				///start to find and copy all field
				Copy_sentence_field( pSentenceNow, (uiSentenceLen - uiLoopA - 1), &sentenceList );
				break;
			}
		}
#else
		if( 0 == IsNMEAsentence( pSentenceNow ) )
		{
			///start to find and copy all field
			Copy_sentence_field( pSentenceNow+1, (uiSentenceLen - uiLoopA - 1), &sentenceList );
			break;
		}
#endif
		///try next char
	}

///	Dump_sentence_field( &sentenceList );


	///parse NMEA sentense with proper decoder
	if( sentenceList.vaildfields > 0 )
	{
		uiLoopA = 0;

		///find if any on decoder list meets the ID in sentence!!
		while( NULL != Decoder_list[uiLoopA].decoder )
		{
			///psrprintf( "%s-%s\r\n", Decoder_list[uiLoopA].NMEAID, sentenceList.Field[0].pField );
			if( strncmp( Decoder_list[uiLoopA].NMEAID, sentenceList.Field[0].pField, 5 ) == 0)
			{
				///psrprintf( "%s-%s\r\n", Decoder_list[uiLoopA].NMEAID, sentenceList.Field[0].pField );
				///analyis field data
				Decoder_list[uiLoopA].decoder( Decoder_list[uiLoopA].fieldNum, &sentenceList, ptNMEAsession );
				break;
			}
			else
			if( strncmp( Decoder_list[uiLoopA].NMEAID, "ENDGP", 5 ) == 0)
			{
				///at end of list
				break;
			}

			uiLoopA++;
		}
	}

///	Dump_gps_NMEA_session( ptNMEAsession );

	return parserRet;
}



static int NMEA_session_Index_update( gps_NMEA_session *psession_new, gps_NMEA_session *psession_local )
{
	int iRet = (-1);
	unsigned uTmp = 0;

	if( psession_new && psession_local )
	{

		do
		{
			if( 0 < ((0x1 << uTmp) & psession_new->uiIndex) )
			{
				if( NMEA_SESSION_UTC_OFFSET == (0x1 << uTmp) )
				{	///update UTC
					psession_local->utc = psession_new->utc;
				}
				else
				if( NMEA_SESSION_COORDINATE_OFFSET == (0x1 << uTmp) )
				{	///update coordinate
					psession_local->coordinate = psession_new->coordinate;
				}
				else
				if( NMEA_SESSION_GPSSTAT_OFFSET == (0x1 << uTmp) )
				{	///update GPS data state
					psession_local->GPSdataStat = psession_new->GPSdataStat;
				}
				else
				if( NMEA_SESSION_FIXQOUAL_OFFSET == (0x1 << uTmp) )
				{	///update GPS fix quality
					psession_local->fixQ = psession_new->fixQ;
				}
				else
				if( NMEA_SESSION_FIXTYPE_OFFSET == (0x1 << uTmp) )
				{	///update GPS fix type
					psession_local->fixType = psession_new->fixType;
				}
				else
				if( NMEA_SESSION_MFIXTYPE_OFFSET == (0x1 << uTmp) )
				{	///update mitac fix type
					psession_local->MfixType = psession_new->MfixType;
				}
				else
				if( NMEA_SESSION_SALLITENUM_OFFSET == (0x1 << uTmp) )
				{	///update fixed satellite number
					psession_local->SatelliteNum = psession_new->SatelliteNum;
				}
				else
				if( NMEA_SESSION_SALLITEID_OFFSET == (0x1 << uTmp) )
				{	///update fixed satellite ID list
					unsigned uA;
					for( uA = 0; uA < 12; uA++ )
						psession_local->SatelliteIDs[uA] = psession_new->SatelliteIDs[uA];
				}
				else
				if( NMEA_SESSION_ALTITUDE_OFFSET == (0x1 << uTmp) )
				{	///update altitude
					psession_local->Altitude = psession_new->Altitude;
				}
				else
				if( NMEA_SESSION_GROUNDSPEED_OFFSET == (0x1 << uTmp) )
				{	///update speed over ground
					psession_local->GroundSpeed = psession_new->GroundSpeed;
				}
				else
				if( NMEA_SESSION_TRACKDEGREE_OFFSET == (0x1 << uTmp) )
				{	///update degree in track
					psession_local->TrackDegree = psession_new->TrackDegree;
				}

			}
			uTmp++;
		}
		while( (NMEA_SESSION_MAX_OFFSET > (0x1 << uTmp)) );


		iRet = 0;
	}

	return iRet;
}



int Handle_NMEA_session( gps_NMEA_session *psession_new )
{
	int iRet = (-1);

	static gps_NMEA_session Last_session;

	iRet = NMEA_session_Index_update( psession_new, &Last_session );

	Dump_gps_NMEA_session( &Last_session );

	return iRet;
}


