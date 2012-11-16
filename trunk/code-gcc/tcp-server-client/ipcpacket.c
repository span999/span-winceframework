/*
 *
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "ipcpacket.h"
#include "toolhelps.h"


/* debug flag sets */
#define	dDBG			0x00001000
#define	dINFO			0x00000100
#define	dERR			0x00010000
/* #define	DBGFSET		(dDBG|dINFO|dERR) */
#define	DBGFSET		(dINFO|dERR)
#define	dF(x)		(DBGFSET&x)



static unsigned short spGetCRC( unsigned char *pPack, int DataCnt )
{
	int cCnt = 0;
	unsigned short crc = 0;

	for( cCnt = 0; cCnt<(DataCnt); cCnt++ )
	{
		crc = (unsigned char)(crc >> 8) | (crc << 8);
		crc ^= pPack[cCnt];
		spMSG( dF(dDBG), "[0x%02x]", pPack[cCnt] );
		crc ^= (unsigned char)(crc & 0xff) >> 4;
		crc ^= (crc << 8) << 4;
		crc ^= ((crc & 0xff) << 4) << 1;
	}

	/* DBGMS( DBGFLAG_ERR, "SDP@MMcmdgetCRC %d[0x%08X] \r\n", crc, crc ); */
	spMSG( dF(dDBG), "%s:%s: spGetCRC: 0x%04x\n", __FILE__, __FUNCTION__, crc );
	return crc;
}


void spIPCPacketInit( struct ipcpacket *pPack )
{
	static unsigned int SN = 0; 
	
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
		pPack->serialnum = SN;
		pPack->packetnum = 1;
		pPack->payloadnum = 0;
		memset( pPack->payload, 0, 255 );
		pPack->CRC = 0;
		
		SN++;
	}

	return;
}


void spIPCPacketDump( struct ipcpacket *pPack )
{
	int iLoop = 0;
	
	if( pPack )
	{
		spQMSG( "%s:%s: \n", __FILE__, __FUNCTION__ );
		spQMSG( "start sign [%c%c%c%c]\n", pPack->start[0], pPack->start[1], pPack->start[2], pPack->start[3] );
		spQMSG( "user ID: %d\n", pPack->userID );
		spQMSG( "IPC type: %d\n", pPack->ipctype );
		spQMSG( "SRC IP: %s[%d]\n", pPack->srcip, pPack->srcport );
		spQMSG( "DES IP: %s[%d]\n", pPack->tarip, pPack->tarport );
		spQMSG( "SHM ID: %d[%d]\n", pPack->srcSHMid, pPack->srcSHMid );
		spQMSG( "SEM ID: %d[%d]\n", pPack->srcSEMid, pPack->srcSEMid );
		spQMSG( "serial number: %d\n", pPack->serialnum );
		spQMSG( "packet number: %d\n", pPack->packetnum );
		spQMSG( "payload size: %d\n", pPack->payloadnum );
		for( ; iLoop < pPack->payloadnum; iLoop++ )
		{
			spQMSG( "[0x%02x]", pPack->payload[iLoop] );
		}
		spQMSG( "\n" );
		spQMSG( "CRC: 0x%04x\n", pPack->CRC );
	}

	return;
}


void spIPCPacketCRCsign( struct ipcpacket *pPack )
{
	if( pPack )
	{
		unsigned short crc = 0;
	#if 0
 		crc = spGetCRC( (unsigned char *)pPack, sizeof(struct ipcpacket)-sizeof(unsigned short) );
 	#else
		crc = spGetCRC( (unsigned char *)pPack, sizeof(struct ipcpacket)-sizeof(int) );
 	#endif
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
	#if 0
 		crc = spGetCRC( (unsigned char *)pPack, sizeof(struct ipcpacket)-sizeof(unsigned short) );
 	#else
		crc = spGetCRC( (unsigned char *)pPack, sizeof(struct ipcpacket)-sizeof(int) );
 	#endif
		if( pPack->CRC == crc )
			iRet = 0;
		else
		{
			spMSG( dF(dERR), "%s:%s: ERROR!! crc:0x%04x while CRC:0x%04x\n", __FILE__, __FUNCTION__, crc, pPack->CRC );
		}
	}

	return iRet;
}


int spIPCPacketDataSize( struct ipcpacket *pPack )
{
	int iRet = -1;
	
	if( pPack )
	{
		iRet = pPack->payloadnum;
	}

	return iRet;
}


int spIPCPacketDataQuery( struct ipcpacket *pPack, char *pData, int *piSize )
{
	int iRet = -1;

	if( pPack && pData && piSize && (*piSize >= spIPCPacketDataSize(pPack) ) )
	{
		iRet = spIPCPacketDataSize(pPack);
		memcpy( pData, pPack->payload, iRet );
	}
	
	return iRet;
}
