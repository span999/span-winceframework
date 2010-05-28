#ifndef __SPDEBUGDEF_H__
#define __SPDEBUGDEF_H__


#include "spDbgFlagDef.h"


///#define		DBGMSG_LEVEL	( (dFOUT|dNOUT|dDOUT) | (dSMAP|dINTR|dMFRM|dLOAD|dCLAS) | (dINIT|dWARN|dINFO|dDBUG|dFAIL) )

///#define		DBGMSG_LEVEL	(dINFO|dWARN|dFAIL)
///#define		DBGMSG_LEVEL	(0)
///#define		DBGMSG_LEVEL	( 0xFFFFFFFF )
#define		DBGMSG_LEVEL	( dDBGFLAGOUTPMASK | dDBGFLAGSPECMASK | dDBGFLAGCOMMMASK )


#define     SPDNKMSG(level, Message)	((level&DBGMSG_LEVEL)?(NKDbgPrintfW Message):0)


#endif	///#ifndef __SPDEBUGDEF_H__