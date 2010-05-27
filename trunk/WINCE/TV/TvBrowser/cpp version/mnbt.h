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

typedef enum
{
    MSG_NO_MESSAGE              = 0,
    MSG_SYSTEM_CANT_HANDLE_MESSAGE = 1,
    SYSTEM_INFO_SIGNAL_STATUS   = 1000,
    IOCTL_CNSCMD_ACCESS_SVC     = MSG_ID_BASE_CNSCMD,
    IOCTL_VIDEO_ON              = MSG_ID_BASE_IOCTL,
    IOCTL_VIDEO_OFF             = MSG_ID_BASE_IOCTL + 1,
    IOCTL_VIDEO_IN_SIZE         = MSG_ID_BASE_IOCTL + 2,
    IOCTL_VIDEO_OUT_SIZE        = MSG_ID_BASE_IOCTL + 3,
    IOCTL_VIDEO_OUT_POS         = MSG_ID_BASE_IOCTL + 4,
} API_MESSAGE_ID;


#pragma pack(1)


typedef struct _APIMessage
{
    API_MESSAGE_ID  msg;
    UINT16          length;
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



#pragma pack()


#endif