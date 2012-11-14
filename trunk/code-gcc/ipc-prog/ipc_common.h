/*


*/



#ifndef __IPC_COMMON_H__
#define __IPC_COMMON_H__


#define		NAMEDHOSTSHMPATH		("/dev/null")
#define		NAMEDHOSTSEMAVAPATH		("/dev/zero")
#define		NAMEDHOSTSeMRDYPATH		("/dev/rtc")


struct ipcpacket
{
	char			start[4];		/* packet start sign '5''A''5''A' */
	int				userID;			/* user ID */
	int				ipctype;		/* type of IPC, 0 = tcp/ip, 1 = shared memory */
	char			srcip[16];		/* 255.255.255.255 */
	int				srcport;		/* port number */
	char			tarip[16];		/* 255.255.255.255 */
	int				tarport;		/* port number */
	int				srcSHMid;		/* client shared memory pool id */
	int				srcSEMid;		/* client pool data available semaphore id */
	unsigned int	serialnum;		/* serial number, same data pack use same serial number. start with '0' */
	int				packetnum;		/* packet number, used when data zise over packet size. start with '1' */
	int				payloadnum;		/* size in byte for payload data after */
	char			payload[255];	/* payload data, max = 255 */
	int				CRC;
};


struct sysPowerCmd
{
	int				packType;		/* power command packet sign */
	long			packSN;			/* power command packet serial nunber */
	int				cmdID;			/* power command ID */
	int				cmdParam1;		/* the first command paramter */
	int				cmdParam2;		/* the second command paramter */
	long			cmdtimestamp;	/* time stamp */
	int				rspReturn;		/* the return value from response */
	int				rspParam1;		/* the first response paramter */
	int				rspParam2;		/* the second response paramter */
	long			rsptimestamp;	/* time stamp */
};


#endif /* __IPC_COMMON_H__ */

