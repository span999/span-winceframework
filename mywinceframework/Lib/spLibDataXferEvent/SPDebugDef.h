#ifndef __SPDEBUGDEF_H__
#define __SPDEBUGDEF_H__


///#define		DBGMSG_LEVEL	(dWARN|dINFO|dDBUG|dFAIL|dCLAS)
///#define		DBGMSG_LEVEL	(dINFO|dWARN|dFAIL)
///#define		DBGMSG_LEVEL	(0)
#define		DBGMSG_LEVEL	( 0xFFFFFFFF )

#define     SPDMSG(level, Message)	((level&DBGMSG_LEVEL)?(NKDbgPrintfW Message):0)

    ///#define     SHMDBGMSG      (void)




#endif	///#ifndef __SPDEBUGDEF_H__