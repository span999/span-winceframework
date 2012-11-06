#ifndef __SYSPOWERCOMM_H__
#define __SYSPOWERCOMM_H__




typedef enum {
	NONEPWRCMD = 0,
	GETCPUACTIVATEDNUM,
	SETCPUACTIVATEDNUM,
} tPWRCMDID;

#define		POWERMGRPACKSIGN		5647


struct sysPowerCmd
{
	int				packType;		/* power command packet sign */
	int				cmdID;			/* power command ID */
	int				cmdParam1;		/* the first command paramter */
	int				cmdParam2;		/* the second command paramter */
#if 0	
	int				cmdtimestamp;	/* time stamp */
#else
	long			cmdtimestamp;	/* time stamp */
#endif
	int				rspReturn;		/* the return value from response */
	int				rspParam1;		/* the first response paramter */
	int				rspParam2;		/* the second response paramter */
#if 0
	int				rsptimestamp;	/* time stamp */
#else
	long			rsptimestamp;	/* time stamp */
#endif
};





#endif /* ##ifndef __SYSPOWERCOMM_H__ */
