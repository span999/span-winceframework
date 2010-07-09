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


/* type def for PM module state value */
typedef    unsigned int			xPMMODSTAT;

#define    PMMODSTAT_ACTIVE   (0x0001)
#define    PMMODSTAT_IDLE     (0x0002)


/* type def for PM timeout value */
typedef    unsigned int			xPMSYSSTATTO;




#endif	///#ifndef __MTYPEDEF_H__
