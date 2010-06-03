/*
	header file for spLog2txt
	spCElogEx.h will be main file.
*/

#ifndef __SPCELOG2TXT_H__
#define __SPCELOG2TXT_H__


#define SPCELOG_POSTFILE_NAME			TEXT("spCElog2txt.txt")
#define SPCELOG_OUTLINE_FLAG			TEXT("--------------------------------")


BOOL MakeLog2txt();
BOOL MakeCopyLog2txt();

#endif	///#ifndef __SPCELOG2TXT_H__