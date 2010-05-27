#ifndef __SPDEBUGDEF_H__
#define __SPDEBUGDEF_H__


#define		dINIT		0x00000001
#define		dWARN		0x00000010
#define		dINFO		0x00000100
#define		dDBUG		0x00001000
#define		dFAIL		0x00010000
#define		dCLAS		0x00100000
#define		dLOAD		0x01000000
#define		dFBUS		0x02000000
#define		dMFRM		0x04000000
#define		dINTR		0x08000000
#define		dSMAP		0x10000000

///#define		DBGMSG_LEVEL	(dWARN|dINFO|dDBUG|dFAIL|dCLAS)
///#define		DBGMSG_LEVEL	(dINFO|dWARN|dFAIL)
#define		DBGMSG_LEVEL	(dWARN|dFAIL)
///#define		DBGMSG_LEVEL	(0)
///#define		DBGMSG_LEVEL	( 0xFFFFFFFF )

#define     SPDMSG(level, Message)	((level&DBGMSG_LEVEL)?(NKDbgPrintfW Message):0)

    ///#define     SHMDBGMSG      (void)




#endif	///#ifndef __SPDEBUGDEF_H__