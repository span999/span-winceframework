#ifndef __SYSPOWERCOMM_H__
#define __SYSPOWERCOMM_H__




typedef enum {
	NONEPWRCMD = 0,
	GETCPUACTIVATEDNUM,
} tPWRCMDID;


struct sysPowerCmd
{
	int				cmdID;			/* power command ID */
	int				cmdParam1;		/* the first command paramter */
	int				cmdParam2;		/* the second command paramter */
	int				cmdtimestamp;	/* time stamp */
	int				rspReturn;		/* the return value from response */
	int				rspParam1;		/* the first response paramter */
	int				rspParam2;		/* the second response paramter */
	int				rsptimestamp;	/* time stamp */
};





#endif /* ##ifndef __SYSPOWERCOMM_H__ */
