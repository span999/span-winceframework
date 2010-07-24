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

#ifndef __NMEADENTENCEFIELD_H__
#define __NMEADENTENCEFIELD_H__

/*




*/
#include "NMEAparser-def.h"


unsigned Dump_sentence_field( NMEA_sentence_field_list* psentenceList );
unsigned Dump_sentence_singlefield( NMEA_sentence_field* psentenceField );
int Handle_sentence_field_UTCtime( NMEA_sentence_field* psentenceField, gps_NMEA_session* pSession );
int Handle_sentence_field_UTCdate( NMEA_sentence_field* psentenceField, gps_NMEA_session* pSession );
int Handle_sentence_field_Latitude( NMEA_sentence_field* psentenceField, gps_NMEA_session* pSession );
int Handle_sentence_field_Longitude( NMEA_sentence_field* psentenceField, gps_NMEA_session* pSession );
int Handle_sentence_field_Direct( NMEA_sentence_field* psentenceField, gps_NMEA_session* pSession );
int Handle_sentence_field_DataStatus( NMEA_sentence_field* psentenceField, gps_NMEA_session* pSession );
int Handle_sentence_field_FixQuality( NMEA_sentence_field* psentenceField, gps_NMEA_session* pSession );
int Handle_sentence_field_FixMitac( NMEA_sentence_field* psentenceField, gps_NMEA_session* pSession );
int Handle_sentence_field_FixType( NMEA_sentence_field* psentenceField, gps_NMEA_session* pSession );
int Handle_sentence_field_SatellitesNum( NMEA_sentence_field* psentenceField, gps_NMEA_session* pSession );
int Handle_sentence_field_SatellitesID( int iID, NMEA_sentence_field* psentenceField, gps_NMEA_session* pSession );
int Handle_sentence_field_Altitude( NMEA_sentence_field* psentenceField, gps_NMEA_session* pSession );
int Handle_sentence_field_GroundSpeed( NMEA_sentence_field* psentenceField, gps_NMEA_session* pSession );
int Handle_sentence_field_DegreeInTrack( NMEA_sentence_field* psentenceField, gps_NMEA_session* pSession );








#endif	///__NMEADENTENCEFIELD_H__
