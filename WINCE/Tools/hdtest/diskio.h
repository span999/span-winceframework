//
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
//
// Use of this source code is subject to the terms of the Microsoft end-user
// license agreement (EULA) under which you licensed this SOFTWARE PRODUCT.
// If you did not accept the terms of the EULA, you are not authorized to use
// this source code. For a copy of the EULA, please see the LICENSE.RTF on your
// install media.
//
/*++
THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
PARTICULAR PURPOSE.

Module Name:  

    diskio.h

Abstract:

    This module contains the constant, type and structure definitions for the
    WINCE disk driver interface.

Notes: 


--*/
#ifndef _DISKIO_H_
#define _DISKIO_H_

//
// Backward compatible IOControlCode values
//
#define DISK_IOCTL_GETINFO      1
#define DISK_IOCTL_SETINFO      5
#define DISK_IOCTL_READ         2
#define DISK_IOCTL_WRITE        3
#define DISK_IOCTL_INITIALIZED  4
#define DISK_IOCTL_FORMAT_MEDIA 6
#define DISK_IOCTL_GETNAME      9

#define CTL_CODE(DeviceType, Function, Method, Access) \
                (((DeviceType) << 16) | ((Access) << 14) | ((Function) << 2) | (Method))
#define METHOD_BUFFERED                 0
#define FILE_ANY_ACCESS                 0
#define FILE_DEVICE_DISK                0x00000007
#define FILE_READ_ACCESS          ( 0x0001 )    // file & pipe
#define FILE_WRITE_ACCESS         ( 0x0002 )    // file & pipe


// New IOControlCode values

#define IOCTL_DISK_BASE FILE_DEVICE_DISK

#define IOCTL_DISK_DEVICE_INFO \
    CTL_CODE(IOCTL_DISK_BASE, 0x600, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_DISK_GETINFO         \
    CTL_CODE(IOCTL_DISK_BASE, 0x700, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_DISK_SETINFO         \
    CTL_CODE(IOCTL_DISK_BASE, 0x701, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_DISK_READ            \
    CTL_CODE(IOCTL_DISK_BASE, 0x702, METHOD_BUFFERED, FILE_READ_ACCESS)

#define IOCTL_DISK_WRITE           \
    CTL_CODE(IOCTL_DISK_BASE, 0x703, METHOD_BUFFERED, FILE_WRITE_ACCESS)

#define IOCTL_DISK_INITIALIZED     \
    CTL_CODE(IOCTL_DISK_BASE, 0x704, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_DISK_FORMAT_MEDIA    \
    CTL_CODE(IOCTL_DISK_BASE, 0x705, METHOD_BUFFERED, FILE_WRITE_ACCESS)

#define IOCTL_DISK_SET_STANDBY_TIMER     \
    CTL_CODE(IOCTL_DISK_BASE, 0x706, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_DISK_STANDBY_NOW     \
    CTL_CODE(IOCTL_DISK_BASE, 0x707, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_DISK_GETNAME         \
    CTL_CODE(IOCTL_DISK_BASE, 0x708, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_DISK_GET_STORAGEID   \
    CTL_CODE(IOCTL_DISK_BASE, 0x709, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_DISK_DELETE_CLUSTER	\
    CTL_CODE(IOCTL_DISK_BASE, 0x710, METHOD_BUFFERED, FILE_ANY_ACCESS) // Obsolete !!!

#define IOCTL_DISK_READ_CDROM \
    CTL_CODE(IOCTL_DISK_BASE, 0x711, METHOD_BUFFERED, FILE_ANY_ACCESS)
    
#define IOCTL_DISK_WRITE_CDROM \
    CTL_CODE(IOCTL_DISK_BASE, 0x712, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_DISK_DELETE_SECTORS \
    CTL_CODE(IOCTL_DISK_BASE, 0x713, METHOD_BUFFERED, FILE_ANY_ACCESS)


// Specified so that Block drivers can implement their own ioctl's without
// fear of overlapping with OS defined codes.
#define IOCTL_DISK_USER_START 0x7E0
#define IOCTL_DISK_USER_END   0x7FF
#define IOCTL_DISK_USER(Function) \
    CTL_CODE( IOCTL_DISK_BASE, IOCTL_DISK_USER_START+Function, METHOD_BUFFERED, FILE_ANY_ACCESS)


// IOCTLS to request that FATFS format or scan a volume. 
#define IOCTL_DISK_FORMAT_VOLUME                CTL_CODE(IOCTL_DISK_BASE, 0x0088, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_DISK_SCAN_VOLUME                  CTL_CODE(IOCTL_DISK_BASE, 0x0089, METHOD_BUFFERED, FILE_ANY_ACCESS)

typedef struct _DELETE_SECTOR_INFO {
    DWORD cbSize; // Size of structure
    DWORD startsector;
    DWORD numsectors;
} DELETE_SECTOR_INFO, *PDELETE_SECTOR_INFO;    

//
// Disk info structure for DISK_IOCTL_GETINFO and DISK_IOCTL_SETINFO
//
typedef struct _DISK_INFO {
    ULONG di_total_sectors;
    ULONG di_bytes_per_sect;
    ULONG di_cylinders;
    ULONG di_heads;
    ULONG di_sectors;
    ULONG di_flags;       
} DISK_INFO, * PDISK_INFO;

//
// DISK_INFO.di_flags bit definitions
//
#define DISK_INFO_FLAG_MBR           0x00000001 // device type has/needs an MBR
#define DISK_INFO_FLAG_CHS_UNCERTAIN 0x00000002 // driver generated CHS values are suspect
#define DISK_INFO_FLAG_UNFORMATTED   0x00000004 // low level format (DISK_IOCTL_FORMAT_MEDIA) needed
#define DISK_INFO_FLAG_PAGEABLE      0x00000008 // device can be used for demand paging

#define MAX_SG_BUF 8

//
// Request completion callback function prototype
//
typedef DWORD (* PFN_REQDONE)(struct _SG_REQ *);

// 
// Scatter/gather structures for read and write ioctls
//
typedef struct _SG_BUF {
	PUCHAR sb_buf;        // pointer to buffer
	DWORD  sb_len;        // length of buffer
} SG_BUF, *PSG_BUF;

typedef struct _SG_REQ {
    DWORD sr_start;     // starting sector number
	DWORD sr_num_sec;   // number of sectors
	DWORD sr_num_sg;    // number of scatter/gather buffers
    DWORD sr_status;    // request status
    PFN_REQDONE sr_callback;  // request completion callback function
	SG_BUF sr_sglist[1];   // first scatter/gather buffer
} SG_REQ, * PSG_REQ;

//
//
//



//
// Removeable media error conditions.
//
#define DISK_DEAD_ERROR     ERROR_NOT_READY
#define DISK_REMOVED_ERROR  ERROR_BAD_UNIT

//
// Structure for use with IOCTL_DISK_GET_STORAGEID
//
typedef struct _STORAGE_IDENTIFICATION {
    DWORD dwSize;
    DWORD dwFlags;
    DWORD dwManufactureIDOffset;
    DWORD dwSerialNumOffset;
} STORAGE_IDENTIFICATION, *PSTORAGE_IDENTIFICATION;
//
// And bits for the dwFlags field:
#define MANUFACTUREID_INVALID   0x01
#define SERIALNUM_INVALID       0x02


#endif // _DISKIO_H_

