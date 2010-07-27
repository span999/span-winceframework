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
#include <math.h>
#include "tools.h"
#include "NMEAcalculation.h"



static int nmeaCalcDiffIn2Point( NMEA_GPS *pPoint1, NMEA_GPS *pPoint2, NMEA_GPS *pPointDiff )
{
	int iRet = (-1);

	if( pPoint1 && pPoint2 && pPointDiff )
	{
		iRet = 0;
		if( pPoint1->Latitude.degree > pPoint2->Latitude.degree )
		{
			pPointDiff->Latitude.degree = (pPoint1->Latitude.degree - pPoint2->Latitude.degree);
			if( pPoint1->Latitude.minute > pPoint2->Latitude.minute )
				pPointDiff->Latitude.minute = (pPoint1->Latitude.minute - pPoint2->Latitude.minute);
			else
			{
				pPointDiff->Latitude.degree--;
				pPointDiff->Latitude.minute = ((pPoint1->Latitude.minute+60) - pPoint2->Latitude.minute);
			}
			iRet = 0x1;
		}
		else if( pPoint1->Latitude.degree < pPoint2->Latitude.degree )
		{
			pPointDiff->Latitude.degree = (pPoint2->Latitude.degree - pPoint1->Latitude.degree);
			if( pPoint1->Latitude.minute < pPoint2->Latitude.minute )
				pPointDiff->Latitude.minute = (pPoint2->Latitude.minute - pPoint1->Latitude.minute);
			else
			{
				pPointDiff->Latitude.degree--;
				pPointDiff->Latitude.minute = ((pPoint2->Latitude.minute+60) - pPoint1->Latitude.minute);
			}
			iRet = 0x2;
		}
		else	/// ==
		{
			pPointDiff->Latitude.degree = 0;
			if( pPoint1->Latitude.minute > pPoint2->Latitude.minute )
				pPointDiff->Latitude.minute = (pPoint1->Latitude.minute - pPoint2->Latitude.minute);
			else
				pPointDiff->Latitude.minute = (pPoint2->Latitude.minute - pPoint1->Latitude.minute);
			iRet = 0x4;
		}

		if( pPoint1->Longitude.degree > pPoint2->Longitude.degree )
		{
			pPointDiff->Longitude.degree = (pPoint1->Longitude.degree - pPoint2->Longitude.degree);
			if( pPoint1->Longitude.minute > pPoint2->Longitude.minute )
				pPointDiff->Longitude.minute = (pPoint1->Longitude.minute - pPoint2->Longitude.minute);
			else
			{
				pPointDiff->Longitude.degree--;
				pPointDiff->Longitude.minute = ((pPoint1->Longitude.minute+60) - pPoint2->Longitude.minute);
			}
			iRet = 0x8;
		}
		else if( pPoint1->Longitude.degree < pPoint2->Longitude.degree )
		{
			pPointDiff->Longitude.degree = (pPoint2->Longitude.degree - pPoint1->Longitude.degree);
			if( pPoint1->Longitude.minute < pPoint2->Longitude.minute )
				pPointDiff->Longitude.minute = (pPoint2->Longitude.minute - pPoint1->Longitude.minute);
			else
			{
				pPointDiff->Longitude.degree--;
				pPointDiff->Longitude.minute = ((pPoint2->Longitude.minute+60) - pPoint1->Longitude.minute);
			}
			iRet = 0x10;
		}
		else	/// ==
		{
			pPointDiff->Longitude.degree = 0;
			if( pPoint1->Longitude.minute > pPoint2->Longitude.minute )
				pPointDiff->Longitude.minute = (pPoint1->Longitude.minute - pPoint2->Longitude.minute);
			else
				pPointDiff->Longitude.minute = (pPoint2->Longitude.minute - pPoint1->Longitude.minute);
			iRet = 0x20;
		}

		///iRet = 0;

		if( 0x12 == (iRet&0x12) )
			iRet = 1;
		else
		if( 0x11 == (iRet&0x11) )
			iRet = 2;
		else
		if( 0x09 == (iRet&0x09) )
			iRet = 3;
		else
		if( 0x0A == (iRet&0x0A) )
			iRet = 4;

/*
  4 | 1
 -------
  3 | 2
*/

///		psrprintf( "Latitude=%d-%f ", pPointDiff->Latitude.degree, pPointDiff->Latitude.minute );
///		psrprintf( "Longitude=%d-%f ", pPointDiff->Longitude.degree, pPointDiff->Longitude.minute );
///		psrprintf( "\n" );
	}

	return iRet;

}


static nmeaCalcRET nmeaCalcGetLatDecimalDegree( NMEA_GPS *pPoint )
{
	nmeaCalcRET xRet = 0;

	xRet = ((pPoint->Latitude.minute)*100)/60;
	xRet = xRet + pPoint->Latitude.degree;

///	psrprintf( "nmeaCalcGetLatDecimalDegree=%f\n", xRet );
	return xRet;
}


static nmeaCalcRET nmeaCalcGetLonDecimalDegree( NMEA_GPS *pPoint )
{
	nmeaCalcRET xRet = 0;

	xRet = ((pPoint->Longitude.minute)*100)/60;
	xRet = xRet + pPoint->Longitude.degree;

///	psrprintf( "nmeaCalcGetLonDecimalDegree=%f\n", xRet );
	return xRet;
}


#define		xPI		(3.141592653589793)


static nmeaCalcRET nmeaCalcDegree2Radian( nmeaCalcRET xIn )
{
	nmeaCalcRET xRet = 0;

	xRet = xIn * xPI / 180;
///	psrprintf( "nmeaCalcDegree2Radian=%f\n", xRet );
	return xRet;
}


#define 	Rads(x)		nmeaCalcDegree2Radian(x)
#define 	LatD(x)		nmeaCalcGetLatDecimalDegree(x)
#define 	LonD(x)		nmeaCalcGetLonDecimalDegree(x)

#define 	LatDr(x)		Rads(LatD(x))
#define 	LonDr(x)		Rads(LonD(x))


#define		xRADIO		(6378.16)


nmeaCalcRET nmeaCalcDistanceIn2Point( NMEA_GPS *pPoint1, NMEA_GPS *pPoint2 )
{
	nmeaCalcRET xRet = 0;
	nmeaCalcRET xTmp = 0;
	nmeaCalcRET xAngle = 0;

	if( pPoint1 && pPoint2 )
	{
		NMEA_GPS nmeaTmp;
		NMEA_GPS *pT = &nmeaTmp;

		psrprintf( ">>> P1.lat=%f P1.lon=%f P2.lat=%f P2.lon=%f <<<\n", LatD(pPoint1), LonD(pPoint1), LatD(pPoint2), LonD(pPoint2) );
		if( -1 != nmeaCalcDiffIn2Point( pPoint1, pPoint2, pT ) )
		{
			xTmp = ( sin(LatDr(pT)/2)*sin(LatDr(pT)/2) ) + (cos(LatDr(pPoint1)) * cos(LatDr(pPoint2))) * ( sin(LonDr(pT)/2) * sin(LonDr(pT)/2) );
			xAngle = 2 * atan2( sqrt(xTmp), sqrt(1-xTmp) );
			xRet = xAngle * xRADIO;
			psrprintf( "nmeaCalcDistanceIn2Point %f [%s] !!!\n", xRet, (0!=xRet)?"OK":"Fail" );
		}
		else
			psrprintf( "nmeaCalcDiffIn2Point fail !!!\n" );
	}
	else
		psrprintf( "nmeaCalcDistanceIn2Point %f [null] !!!\n", xRet );

	return xRet;
}


nmeaCalcRET nmeaCalcAngleInNorth2Point( NMEA_GPS *pPointOLD, NMEA_GPS *pPointNEW )
{
	nmeaCalcRET xRet = 0;
	nmeaCalcRET xTmp = 0;
	nmeaCalcRET xAngle = 0;

	if( pPointOLD && pPointNEW )
	{
		NMEA_GPS nmeaTmp;
		NMEA_GPS *pT = &nmeaTmp;

		psrprintf( ">>> P1.lat=%f P1.lon=%f P2.lat=%f P2.lon=%f <<<\n", LatD(pPointOLD), LonD(pPointOLD), LatD(pPointNEW), LonD(pPointNEW) );
		if( -1 != nmeaCalcDiffIn2Point( pPointOLD, pPointNEW, pT ) )
		{
			psrprintf( "nmeaCalcAngleInNorth2Point %f [%s] !!!\n", xRet, (0!=xRet)?"OK":"Fail" );
		}
		else
			psrprintf( "nmeaCalcAngleInNorth2Point fail !!!\n" );
	}
	else
		psrprintf( "nmeaCalcAngleInNorth2Point %f [null] !!!\n", xRet );

	return xRet;
}


nmeaCalcRET Knot2Ms( nmeaCalcRET knot )
{
	return (knot * (0.514444));
}


nmeaCalcRET Knot2kMh( nmeaCalcRET knot )
{
	return (knot * (1.852));
}


