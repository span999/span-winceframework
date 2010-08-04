#ifndef __MTYPEDEF_H__
#define __MTYPEDEF_H__

#include "GenericTypeDefs.h"


/* type def for boolean retuen */
#ifdef NO_GENERICTYPEDEFS
	typedef    signed				bRET;
	#define    bRET_TRUE            (1)
	#define    bRET_FALSE           (0)
#else
	typedef    BOOL					bRET;
	#define    bRET_TRUE            TRUE
	#define    bRET_FALSE           FALSE
#endif

/* type def for integer retuen */
#ifdef NO_GENERICTYPEDEFS
	typedef    signed int			iRET;
#else
	typedef    INT					iRET;
#endif

/* type def for PM system state value */
typedef    unsigned int			xPMSYSSTAT;

#define    PMSYSSTAT_ON             (0x0001)
#define    PMSYSSTAT_SAVE          	(0x0002)
#define    PMSYSSTAT_OFF            (0x0004)


/* type def for PM module state value */
typedef    unsigned int			xPMMODSTAT;

#define    PMMODSTAT_ACTIVE   (0x0001)
#define    PMMODSTAT_IDLE     (0x0002)


/* type def for PM timeout value */
typedef    unsigned int			xPMSYSSTATTO;

/* System State Timeout table */
typedef struct PMStTOTBL
{
	BOOL bValid;
    xPMSYSSTATTO xtoSysStat_ON;                /*< Timeour value of system power state 'POWER ON'. */
    xPMSYSSTATTO xtoSysStat_SAVE;             /*< Timeour value of system power state 'POWER SAVE'. */
    xPMSYSSTATTO xtoSysStat_OFF;               /*< Timeour value of system power state 'OFF'. */
} xPMSTATTOTBLE;


///On,OFF define
typedef enum {
    xON = 1,		/*  */
    xOFF,			/*  */
} xOnOff;


#endif	///#ifndef __MTYPEDEF_H__
