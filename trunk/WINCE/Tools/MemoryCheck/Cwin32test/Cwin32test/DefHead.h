// Cwin32test.cpp : Defines the entry point for the application.
//



#ifndef __DEFHEAD_H__
#define __DEFHEAD_H__




///structure definition
typedef struct T_spPROCENTRYLIST
{
	DWORD 				dwTotalNum;
	DWORD 				dwIndex;
	PROCESSENTRY32		procEntryA[32];
} spPROCENTRYLIST, *pspPROCENTRYLIST;

typedef struct T_spMODENTRYLIST
{
	DWORD 				dwTotalNum;
	DWORD 				dwIndex;
	DWORD				dwProcID;
	MODULEENTRY32		modEntryA[512];
} spMODENTRYLIST, *pspMODENTRYLIST;

typedef struct T_spHEAPENTRYLIST
{
	DWORD 				dwTotalNum;
	DWORD 				dwIndex;
	DWORD				dwProcID;
	HEAPENTRY32			heapEntryA[4096];
} spHEAPENTRYLIST, *pspHEAPENTRYLIST;

typedef struct T_spTHREADLIST
{
	DWORD 				dwTotalNum;
	DWORD 				dwIndex;
	THREADENTRY32		threadEntryA[256];
} spTHREADLIST, *pspTHREADLIST;

typedef struct T_spHEAPINFO
{
	DWORD				dwProcID;
	DWORD				dwHeapID;
	DWORD				dwPartNum;
	DWORD				dwHiAddr;
	DWORD				dwLowAddr;
	DWORD				dwTotalSize;
} spHEAPINFO, *pspHEAPINFO;

typedef struct T_spHEAPLISTLIST
{
	DWORD 				dwTotalNum;
	DWORD 				dwIndex;
	DWORD				dwProcID;
	HEAPLIST32			heapListA[64];
	spHEAPINFO			heapInfo[64];
} spHEAPLISTLIST, *pspHEAPLISTLIST;

typedef struct T_spDLLSUMMARY
{
	DWORD 				dwTotalDllNum;
	DWORD 				dwIndex;
	DWORD				dwDllLow;
	DWORD				dwDllBig;
	DWORD				dwDllLowNK;
	DWORD				dwDllBigNK;
	DWORD				dwDllLowXIP;
	DWORD				dwDllBigXIP;
	DWORD				dwDllLowRES;
	DWORD				dwDllBigRES;
	DWORD				dwDllLow0;
	DWORD				dwDllBig0;
	DWORD				dwDllLowElse;
	DWORD				dwDllBigElse;

} spDLLSUMMARY, *pspDLLSUMMARY;

typedef struct T_spSTACKINFO
{
///	DWORD 				dwFrameNum;
	DWORD 				dwStackAddr;
	DWORD 				dwRetAddr;
} spSTACKINFO, *pspSTACKINFO;

typedef struct T_spSTACKINFOLIST
{
	DWORD 				dwTotalNum;
	DWORD 				dwIndex;
	spSTACKINFO			stackInfo[2048];
} spSTACKINFOLIST, *pspSTACKINFOLIST;


typedef struct T_spVMDLLLOCATEIDX
{
	DWORD 				dw0000;
	DWORD 				dw1000;
	DWORD 				dw2000;
	DWORD 				dw3000;
	DWORD 				dw4000;
	DWORD 				dw5000;
	DWORD 				dw6000;
	DWORD 				dw7000;
	DWORD 				dw8000;
	DWORD 				dw9000;
	DWORD 				dwA000;
	DWORD 				dwB000;
	DWORD 				dwC000;
	DWORD 				dwD000;
	DWORD 				dwE000;
	DWORD 				dwF000;

} spVMDLLLOCATEIDX, *pspVMDLLLOCATEIDX;


#define MEMORX_BINARY_IPAQSTORE_LOCATION		TEXT("\\iPAQ File Store\\PDA_QA_tools\\MemoRx.exe")
#define DEVHEALTH_BINARY_IPAQSTORE_LOCATION		TEXT("\\iPAQ File Store\\PDA_QA_tools\\DevHealth.exe")

#define MEMORX_IPAQSTORE_LOG_FILES				TEXT("\\iPAQ File Store\\memoRx_%04d.log")
#define MEMORX_ROOTSTORE_LOG_FILES				TEXT("\\memoRx_%04d.log")

#endif	///#ifndef __DEFHEAD_H__



