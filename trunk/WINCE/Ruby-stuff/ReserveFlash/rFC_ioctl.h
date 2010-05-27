/* MiTAC Int'l Corp. 2001~2005

=========================================================================
Description:
    This file include the defination of the IOCTL code 
    & event naming to system driver.
=========================================================================
Release notes

Modifier	        sign	        date		    Description
-------------------------------------------------------------------------
Span Liu			span0502        05/02/2005		add MPM IOCTL & event name 
Span Liu			span0429        04/29/2005		add MUT event name 
Jawa Chen           jawa01          04/28/2005      Add Flash IOCTL
Span Liu			span0120        01/20/2005		add HDD IOCTL 
Span Liu			span0111        01/11/2005		add backlight IOCTL 
Pat Kuo                                             initial release
*/


#ifndef __RFC_IOCTL_H__
#define __RFC_IOCTL_H__

/// reserverd flash contain command list
#define rFC_READ			0x00000001				/// read record by the record No.
#define rFC_WRITE			0x00000010				/// write record by the record No.
#define rFC_DELET			0x00000100				/// delet record by the record No.
#define rFC_NEW				0x00001000				/// new record by your input return record No.			
#define rFC_LIST			0x00010000				/// list all record
#define rFC_FLUSHMIRROR		0x00020000				/// force re-flush Flash to RAM
#define rFC_SEEK			0x00100000				/// seek record by name
#define rFC_LOCK			0x01000000				/// check out record by record No.
#define rFC_UNLOCK			0x10000000				/// check in record by record No.

#define rFC_TOC_OFFSET		0x00000000
#define rFC_DATA_OFFSET		0x00006000				/// DATA eara start after 24K Byte total 104K Byte

#define rFC_ID_VALID_TAG	0xD0000000
#define rFC_NAME_MAX		11

#define rFC_RECORD_START_TAG	0x36360F0F
#define rFC_RECORD_END_TAG		0x3636F0F0
#define rFC_RECORD_VALID_TAG	0x36363636
#define rFC_RECORD_INVALID_TAG	0x36363939

/// command structure
typedef struct
{
	DWORD 	dwCommand;			///rFC_READ,rFC_WRITE,rFC_NEW ....
	DWORD	dwRecordID;			///Record number
	//LPVOID	lpName;				///data1 income with command
	LPTSTR 	lpName;				///data1 income with command
	DWORD	dwSizeName;			///size of data1
	LPVOID	lpData;				///data2 income with command
	DWORD	dwSizeData;			///size of data2
	LPVOID	lpResponse;			///response after command
	LPDWORD lpBytesResponse;		///size of response
}rFC_COMMAND, *PrFC_COMMAND;

/// rFC record structure
typedef struct
{								///0x3636F0F0 for END, 0x36360F0F for START
	DWORD dwValid;				///0x36363636 for valid, 0x36363939 for involid.
	DWORD dwRecordNum;			///record No. Valid only if dwValid is Valid
	DWORD dwNextNum;			///next realated record No.
	DWORD dwRecordOffset;		///record data offset in Byte form the start of DATA eara
	DWORD dwRecordLength;		///record data length in byte	
	TCHAR cRecordName[(rFC_NAME_MAX+1)];		///record data name MAX 10 char
	DWORD dwData;				///record data
}rFC_RECORD, *PrFC_RECORD;


////// TOC record structure
///typedef struct
///{
///	DWORD dwValid;				///0x36363636 for valid, other for invalid, 0xF0F0F0F0 for end of TOC
///	DWORD dwRecordNum;			///record No.
///	DWORD dwRecordOffset;		///record data offset in Byte form the DATA eara start
///	DWORD dwRecordLength;		///record data length in byte	
///	UCHAR cRecordName[10];		///record data name MAX 10 char
///}rFC_TOC_RECORD, *PrFC_TOC_RECORD;
///
////// DATA record structure
///typedef struct
///{
///	rFC_TOC_RECORD recHeader;
///	BYTE bData[];
///}rFC_DATA_RECORD, *PrFC_DATA_RECORD;

///span0924 end

#endif	///#ifndef __RFC_IOCTL_H__