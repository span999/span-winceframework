#ifndef __MTYPEDEF_H__
#define __MTYPEDEF_H__


/* type def for boolean retuen */
typedef    signed				bRET;
#define    bRET_TRUE             (1)
#define    bRET_FALSE            (0)

/* type def for integer retuen */
typedef    signed int			iRET;


/* type def for PM system state value */
typedef    unsigned int			xPMSYSSTAT;


/* type def for PM module state value */
typedef    unsigned int			xPMMODSTAT;

#define    PMMODSTAT_ACTIVE   (0x0001)
#define    PMMODSTAT_IDLE     (0x0002)


/* type def for PM timeout value */
typedef    unsigned int			xPMSYSSTATTO;




#endif	///#ifndef __MTYPEDEF_H__
