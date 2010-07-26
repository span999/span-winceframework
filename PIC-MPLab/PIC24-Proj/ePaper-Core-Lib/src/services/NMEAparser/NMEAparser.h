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

#ifndef __NMEAPARSER_H__
#define __NMEAPARSER_H__

#include "NMEAparser-def.h"

nmeaParseRET NMEAparser(char *pcNMEAsentence, gps_NMEA_session* ptNMEAsession);
int Handle_NMEA_session( gps_NMEA_session *psession_new );


#endif	///#ifndef __NMEAPARSER_H__
