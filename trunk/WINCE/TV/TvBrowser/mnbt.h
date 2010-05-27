/* MiTAC Int'l Corp. 2001~2006

================================================================================
Description:
    This file contains necessary defines and structures for TV browser.

================================================================================
Release notes

Modifier	        sign	        date		    Description
--------------------------------------------------------------------------------
Pat Kuo                             2006/09/15      first release
*/



#ifndef __MNBT_H__
#define __MNBT_H__


#include <windows.h>

#define MSG_ID_BASE_CNSCMD      2000
#define MSG_ID_BASE_IOCTL       3000

typedef enum 
{
    SYS_UNKNOWN_COMMAND     = -100,
    SYS_MESSAGE_Q_OVERFLOW  = -4,
    SYS_INVALID_PARAM       = -3,
    SYS_DEVICE_NOT_OPENED   = -2,
    SYS_KO                  = -1,
    SYS_OK                  = 0,
    SYS_NO_MESSAGE          = 1,
    SYS_ALREADY_OPENED      = 2
} SYS_ERROR;

// API Message ID
#define MSG_NO_MESSAGE                      0
#define MSG_SYSTEM_CANT_HANDLE_MESSAGE      1


#define MSG_DMB_SET_AUDIO_VOLUME            500
#define MSG_DMB_GET_AUDIO_VOLUME            501
#define SYSTEM_INFO_SIGNAL_STATUS           1000

#define IOCTL_CNSCMD_ACCESS_SVC             MSG_ID_BASE_CNSCMD
#define IOCTL_CNSCMD_READ_TUNER_REG         MSG_ID_BASE_CNSCMD + 1
#define IOCTL_CNSCMD_WRITE_TUNER_REG        MSG_ID_BASE_CNSCMD + 2
#define IOCTL_CNSCMD_SET_VIDEO_601          MSG_ID_BASE_CNSCMD + 3
#define IOCTL_CNSCMD_SET_VIDEO_656          MSG_ID_BASE_CNSCMD + 4
#define IOCTL_CNSCMD_OSD_CTRL               MSG_ID_BASE_CNSCMD + 5
#define IOCTL_CNSCMD_SET_BATTERY_LEVEL      MSG_ID_BASE_CNSCMD + 6
#define IOCTL_CNSCMD_GET_CHECKSUM           MSG_ID_BASE_CNSCMD + 7
#define IOCTL_CNSCMD_RECORD                 MSG_ID_BASE_CNSCMD + 8
#define IOCTL_CNSCMD_PLAYBACK               MSG_ID_BASE_CNSCMD + 9
#define IOCTL_CNSCMD_STILLSHOT              MSG_ID_BASE_CNSCMD + 10
#define IOCTL_CNSCMD_MUTE                   MSG_ID_BASE_CNSCMD + 11
#define IOCTL_CNSCMD_BOOT_INFO              MSG_ID_BASE_CNSCMD + 12
#define IOCTL_CNSCMD_GET_VERSION            MSG_ID_BASE_CNSCMD + 13
#define IOCTL_CNSCMD_BATTERY_STATUS         MSG_ID_BASE_CNSCMD + 14
#define IOCTL_CNSCMD_SET_CONTRAST           MSG_ID_BASE_CNSCMD + 15     ///span1121, add
#define IOCTL_CNSCMD_SET_SATURATION         MSG_ID_BASE_CNSCMD + 16     ///span1121, add
#define IOCTL_CNSCMD_SET_BRIGHTNESS         MSG_ID_BASE_CNSCMD + 17     ///span1121, add
#define IOCTL_CNSCMD_SLS_CHECK              MSG_ID_BASE_CNSCMD + 18     ///span1118, ///add in spi comm spec 1.25
#define IOCTL_CNSCMD_DLS_CHECK              MSG_ID_BASE_CNSCMD + 19     ///span1118, ///add in spi comm spec 1.25
#define IOCTL_CNSCMD_GET_DATASERVICE        MSG_ID_BASE_CNSCMD + 20     ///span1129, ///add for get data service data
#define IOCTL_CNSCMD_CHECK_MODULE           MSG_ID_BASE_CNSCMD + 21     ///span1129, ///add for check module status
#define IOCTL_CNSCMD_SET_FOCUS              MSG_ID_BASE_CNSCMD + 22     ///span1129, ///add for OSD3 focus
#define IOCTL_CNSCMD_GET_FOCUS              MSG_ID_BASE_CNSCMD + 23     ///span1129, ///add for OSD3 focus
#define IOCTL_CNSCMD_GET_SYSINFO            MSG_ID_BASE_CNSCMD + 24     ///span1205, ///add in spi comm spec 1.32
#define IOCTL_CNSCMD_SET_SYSINFO            MSG_ID_BASE_CNSCMD + 25     ///span1205, ///add in spi comm spec 1.32
#define IOCTL_CNSCMD_LOOPBACK_TEST          MSG_ID_BASE_CNSCMD + 26     ///span1215, add for loopback test

#define IOCTL_VIDEO_ON                      MSG_ID_BASE_IOCTL
#define IOCTL_VIDEO_OFF                     MSG_ID_BASE_IOCTL + 1
#define IOCTL_VIDEO_IN_SIZE                 MSG_ID_BASE_IOCTL + 2
#define IOCTL_VIDEO_OUT_SIZE                MSG_ID_BASE_IOCTL + 3
#define IOCTL_VIDEO_OUT_POS                 MSG_ID_BASE_IOCTL + 4
#define IOCTL_NEPTUNE_PWR_ON                MSG_ID_BASE_IOCTL + 5
#define IOCTL_NEPTUNE_PWR_OFF               MSG_ID_BASE_IOCTL + 6
#define IOCTL_DIRECT_ACCESS                 10001
#define IOCTL_GET_CMD_RESULT                20000


#pragma pack(1)


typedef struct _APIMessage
{
    UINT32          msg;
    UINT32          length;
    UINT8           *pdata;
} APIMessage, *PAPIMessage;

#define API_MSG_SIZE(msg)       (sizeof(APIMessage)+msg->length-1)

typedef struct _CarrierStatus
{
    UINT32  freq;
    UINT8   subch;
    UINT8   lock;
    UINT8   strength;
    UINT32  nSNR;
    UINT32  nBER;
    UINT16  bitrate;
    UINT16  sync_err_rate;
    UINT16  tei_rate;
} CarrierStatus, *PCarrierStatus;

typedef struct _CifSizePos
{
    DWORD x;
    DWORD y;
} CifSizePos, *PCifSizePos;

typedef struct _SVC_INFO
{
	UINT8 	label[16];
	UINT32	srv_id;
	UINT8 	srv_lable[16];
	UINT16	tmid;
	UINT16	asc_dsc;
	UINT16	sub_ch_id;
	UINT16	srv_comp;
	UINT16	packet_addr;
	UINT8	freq_index;
	UINT8	dg_flag;
	UINT32	bitrate;
	UINT32	sub_ch_size;
	UINT32	user_app_type;
} SvcInfo, *PSvcInfo;

///span1129, add for SLS/DLS service information
///SLS:Slide Show Service DLS:Dynamic Label Segment BWS:Broadcast WebSite Service
typedef struct _DATASERVICE_INFO
{
    UINT8   bPackHeader[4];         ///struct header, 
    UINT32  dwPackSize;             ///size of this structure
	UINT32  dwDLSSize;              ///valid data size in byte
	UINT32  dwSLSSize;              ///valid data size in byte
	UINT32  dwSLSxSize;             ///x of resolution
    UINT32  dwSLSySize;             ///y of resolution	
	UINT8   bDLSBuffer[256];        ///valid data buffer
	UINT8   bSLSBuffer[2*640*480];  ///valid data buffer, 640*480 is the max resolution	
} DataServInfo, *PDataServInfo;
///span1129, add for SLS/DLS service information

#pragma pack()


#endif