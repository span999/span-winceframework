/*
 *
 *
 */
#ifndef __IPCPACKET_H__
#define __IPCPACKET_H__


struct ipcpacket
{
	char			start[4];		/* packet start sign '5''A''5''A' */
	int				userID;			/* user ID */
	int				ipctype;		/* type of IPC, 0 = tcp/ip */
	char			srcip[16];		/* 255.255.255.255 */
	int				srcport;		/* port number */
	char			tarip[16];		/* 255.255.255.255 */
	int				tarport;		/* port number */
	int				serialnum;		/* serial number, same data pack use same serial number. start with '0' */
	int				packetnum;		/* packet number, used when data zise over packet size. start with '1' */
	int				payloadnum;		/* size in byte for payload data after */
	char			payload[255];	/* payload data, max = 255 */
	unsigned short	CRC;
};


void spIPCPacketInit( struct ipcpacket *pPack );
void spIPCPacketCRCsign( struct ipcpacket *pPack );
int spIPCPacketCRCvalid( struct ipcpacket *pPack );
void spIPCPacketDump( struct ipcpacket *pPack );


#endif /* #ifndef __IPCPACKET_H__ */
