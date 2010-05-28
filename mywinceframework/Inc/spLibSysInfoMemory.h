#ifndef __SPLIBSYSINFOMEMORY_H__
#define __SPLIBSYSINFOMEMORY_H__


typedef struct _LibSysInfoMemoryContent { 
	DWORD dwLength;
	DWORD dwMemoryLoad;
	DWORD dwTotalPhys;
	DWORD dwAvailPhys;
	int iDiffPhys;
	DWORD dwTotalPageFile;
	DWORD dwAvailPageFile;
	int iDiffPageFile;
	DWORD dwTotalVirtual;
	DWORD dwAvailVirtual;
	int iDiffVirtual;
} LibSysInfoMemoryContent, *PLibSysInfoMemoryContent; 



DWORD spLibSysInfoMemory_Init( DWORD dwParam );
DWORD spLibSysInfoMemory_Deinit( DWORD dwParam );
DWORD spLibSysInfoMemory_Get( LibSysInfoMemoryContent *pOut );


#endif	///#ifndef __SPLIBSYSINFOMEMORY_H__