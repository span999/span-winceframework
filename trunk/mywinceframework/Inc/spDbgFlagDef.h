#ifndef __SPDBGFLAGDEF_H__
#define __SPDBGFLAGDEF_H__

#define		dDBGFLAGCOMMMASK		0x0000FFFF
#define		dDBGFLAGSPECMASK		0x0FFF0000
#define		dDBGFLAGOUTPMASK		0xF0000000

///common
#define		dINIT		( 0x00000001 & dDBGFLAGCOMMMASK )
#define		dWARN		( 0x00000002 & dDBGFLAGCOMMMASK )
#define		dINFO		( 0x00000004 & dDBGFLAGCOMMMASK )
#define		dDBUG		( 0x00000008 & dDBGFLAGCOMMMASK )
#define		dFAIL		( 0x00000010 & dDBGFLAGCOMMMASK )

///special
#define		dCLAS		( 0x00010000 & dDBGFLAGSPECMASK )
#define		dLOAD		( 0x00020000 & dDBGFLAGSPECMASK )
#define		dFBUS		( 0x00040000 & dDBGFLAGSPECMASK )
#define		dMFRM		( 0x00080000 & dDBGFLAGSPECMASK )
#define		dINTR		( 0x00100000 & dDBGFLAGSPECMASK )
#define		dSMAP		( 0x00200000 & dDBGFLAGSPECMASK )

//output
#define		dFOUT		( 0x10000000 & dDBGFLAGOUTPMASK )	///file out
#define		dNOUT		( 0x20000000 & dDBGFLAGOUTPMASK )	///NKDbgPrintfW out
#define		dDOUT		( 0x40000000 & dDBGFLAGOUTPMASK )	///dialogue out




#endif	///#ifndef __SPDBGFLAGDEF_H__