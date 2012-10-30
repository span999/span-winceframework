/*
 *
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "ipcpacket.h"




static unsigned short spGetCRC( unsigned char* pPack, int DataCnt )
{
	int cCnt = 0;
	unsigned short crc = 0;

	for( cCnt = 0; cCnt<(DataCnt); cCnt++ )
	{
		crc = (unsigned char)(crc >> 8) | (crc << 8);
		crc ^= pPack[cCnt];
		crc ^= (unsigned char)(crc & 0xff) >> 4;
		crc ^= (crc << 8) << 4;
		crc ^= ((crc & 0xff) << 4) << 1;
	}

	/* DBGMS( DBGFLAG_ERR, "SDP@MMcmdgetCRC %d[0x%08X] \r\n", crc, crc ); */
	return crc;
}


void spIPCPacketInit( struct ipcpacket *pPack )
{
	if( pPack )
	{
		pPack->start[0] = '5';
		pPack->start[1] = 'A';
		pPack->start[2] = '5';
		pPack->start[3] = 'A';
		pPack->userID = 0;
		pPack->ipctype = 0;
		memcpy( pPack->srcip, "255.255.255.255", 16 ); 
		pPack->srcport = 77777;
		memcpy( pPack->tarip, "255.255.255.255", 16 ); 
		pPack->tarport = 77777;
		pPack->serialnum = 0;
		pPack->packetnum = 1;
		pPack->payloadnum = 0;
		memset( pPack->payload, 0, 255 );
		pPack->CRC = 0;
	}

	return;
}


void spIPCPacketCRCsign( struct ipcpacket *pPack )
{
	if( pPack )
	{
		unsigned short crc = 0;
 		crc = spGetCRC( (unsigned char *)pPack, sizeof(struct ipcpacket)-sizeof(unsigned short) );
		pPack->CRC = crc;
	}

	return;
}


int spIPCPacketCRCvalid( struct ipcpacket *pPack )
{
	int iRet = (-1);

	if( pPack )
	{
		unsigned short crc = 0;
 		crc = spGetCRC( (unsigned char *)pPack, sizeof(struct ipcpacket)-sizeof(unsigned short) );
		if( pPack->CRC == crc )
			iRet = 0;
	}

	return iRet;
}



