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

#ifndef __NMEACALCULATION_H__
#define __NMEACALCULATION_H__


#include "NMEAparser-def.h"



///typedef    	float		nmeaCalcRET;
typedef     double		nmeaCalcRET;


nmeaCalcRET nmeaCalcDistanceIn2Point( NMEA_GPS *pPoint1, NMEA_GPS *pPoint2 );
nmeaCalcRET Knot2Ms( nmeaCalcRET knot );
nmeaCalcRET Knot2kMh( nmeaCalcRET knot );


#endif	///#ifndef __NMEACALCULATION_H__
