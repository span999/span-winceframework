/* MiTAC Int'l Corp. 2001~2006

================================================================================
Description:
    header file of CCns.cpp

================================================================================
Release notes

Modifier	        sign	        date		    Description
--------------------------------------------------------------------------------
Span Liu                            2006/11/08      first release
*/



#ifndef __DTV_DEF_H__
#define __DTV_DEF_H__


#define MIC_DTV_DRV_REG_KEY						(TEXT("\\Drivers\\BuiltIn\\DTV"))
#define MIC_DTV_DRV_PROJECT_CODE    			0x67333130		///'C''3''1''0'
#define MIC_DTV_DRV_TS_ENCRYPTION_CODE  	    0x454E4300		///'E''N''C''?'

///defines of interface standard command code for Digital TV
///basically, these standard command will only have boolean(TRUE/FALSE) return.


#define NAMED_RECORD_DTAD_AVAILABLE     	(TEXT("named record data available event"))///span70418, add for record data available event for AP layer
#define NAMED_PLAYBACK_DTAD_QUERY     		(TEXT("named playback data query event"))///span70502, add for playback data query event for AP layer
#define NAMED_WARP_NOTIFY_EVENT     		(TEXT("named notify app/dtv event"))

#define MAX_STILLSHOT_SIZE       (320*240*3/2)
#define ISDB_T_TS_PACKET_SIZE       192
#define ISDB_T_TS_DATA_BUFFER_SIZE  (ISDB_T_TS_PACKET_SIZE*10)
#define ISDB_T_TS_DATA_MAX_SIZE     (ISDB_T_TS_DATA_BUFFER_SIZE*4)



///span70509, add for TS file
typedef struct T_MIO_ISDB_TS_FILE_HEADER
{
	BYTE		bFileID[4];			///ID for TS file "TS"
    BYTE 		bCorpName[8];		///Corp name like "mitac"
    BYTE        bTStype[8];			///DTV system name like "ISDB-T"
    DWORD       dwHeaderSize;		///size of this header
    DWORD       dwTSDataOffset;		///the TS data offset from file begin
    DWORD       dwFileSize;			///size of this file
 	SYSTEMTIME	systemTime;			///system time of create this file
    DWORD		dwDuration;			///TS duration in ms
	BYTE		bReserve1[24];		///reserve for further use

	BYTE		bDeviceName[8];		///device name that create this file
	DWORD		dwProjectCode;		///device project code
	DWORD		dwSubProjectCode;	///device sub-project code
	DWORD		dwDrvMajorVer;		///major version of DTV driver on device
	DWORD		dwDrvMinorVer;		///minor version of DTV driver on device
	DWORD		dwTSrate;			///ratio of this TS stream
	DWORD		dwEncrypted;		///TS stream data encrypt
	BYTE		bReserve2[84];		///reserve for further use	

} MIO_ISDB_TS_FILE_HEADER, *PMIO_ISDB_TS_FILE_HEADER;


typedef enum T_DTV_STATUS {

    UNINITIALIZED = 0,
    INITIALIZE,
    POWERON_READY,
    SCAN,
    CHANNELPLAY,
    CHANNELSTANDBY,
    CHANNELREC,
    FILEPLAYBACK,
    PLAYBACKPAUSE,
    SCREENSHOT,
    SHM_ERROR,    

} DTV_STATUS, *PDTV_STATUS;




///structrue for module info
typedef struct T_DTV_MODULE_INFO
{
    BOOL        bPowerOn;
    BOOL        bLocked;
    BOOL        bVideoOn;
    BOOL        bAudioOn;
    BOOL        bOSDOn;
    
    DWORD       dwVideoSet;
    DWORD       dwAudioSet;
    DWORD       dwOSDSet;
    DWORD       dwVolumeLevel;    ///span70227, add volume control code
    DWORD       dwVideoInXsize;    ///span70228, add video control code
    DWORD       dwVideoInYsize;    ///span70228, add video control code
    DWORD       dwVideoOutXsize;    ///span70228, add video control code
    DWORD       dwVideoOutYsize;    ///span70228, add video control code
    DWORD       dwVideoOutXpo;    ///span70228, add video control code
    DWORD       dwVideoOutYpo;    ///span70228, add video control code

    HANDLE      hActiveMsgEvent;    ///span70207, add for handle active Msg from module
    HANDLE      hActiveMsgThread;   ///span70418, add for handle active Msg from module
    DWORD       dwActiveMsgThreadID;///span70418, add for handle active Msg from module

} DTV_MODULE_INFO, *PDTV_MODULE_INFO;

///structrue for DTV info
typedef struct T_DTV_CONTENT_INFO
{
    BOOL        bScanDone;
    DWORD       dwTotalChannels;
    DWORD       dwCurrentChannel;
    DWORD       dwDMBChannels;
    DWORD       dwDABChannels;

    DWORD       dwFreqence;
    
    BOOL        bTsDataEncryption;
///    BOOL		bRecording;
///    BOOL		bPlaybacking;
    DTV_STATUS  euDTVstauts;
    
    HANDLE      hRecDataEvent;      ///span70418, add for record data available event
    HANDLE      hPbKDataEvent;      ///span70502, add for playback data query event
    DWORD       dwCurrTSrate;      ///span70508, add for streaming
} DTV_CONTENT_INFO, *PDTV_CONTENT_INFO;


typedef struct T_DTV_INFO
{
    DTV_MODULE_INFO     DtvModuleInfo;
    DTV_CONTENT_INFO    DtvContentInfo;
} DTV_INFO, *PDTV_INFO;


#define DTV_STILLSHOT_FORMAT_RAW       0x00000000
#define DTV_STILLSHOT_FORMAT_BMP       0x00000001
#define DTV_STILLSHOT_FORMAT_JPG       0x00000010


typedef struct  T_DTV_STILLSHOT
{
	DWORD dwFormat;    	        ///format of image
	DWORD dwXsize;              ///image X resolution
	DWORD dwYsize;              ///image Y resolution
	DWORD dwImageSize;          ///total image size in byte
    PBYTE pbStillShotData;      ///pointer of stillshot buffer
	BYTE bStillShotData[MAX_STILLSHOT_SIZE];
	
} DTV_STILLSHOT, *PDTV_STILLSHOT;


///span70411, add for AP layer channel scan for ISDB-T system
typedef struct  T_3DIGIT_CHANNEL_NUM
{
	DWORD dwChannelNum;         ///3-digit channel number
	DWORD dwBranchNum;          ///branch number

} S3DIGIT_CHANNEL_NUM, *PS3DIGIT_CHANNEL_NUM;

typedef  DWORD   CHANNELNUM;		///13~62
typedef  PDWORD  PCHANNELNUM;

///structrue for scan channel
typedef struct  T_CHANNELSCAN
{
	CHANNELNUM dwChannelNum;				///13~62
	DWORD dwScanTimeout;			///max time out value for scan
} CHANNELSCAN, *PCHANNELSCAN;


///structrue for channel info
typedef struct  T_CHANNEL_INFO
{
	S3DIGIT_CHANNEL_NUM ChNum;    ///channel number
	DWORD dwPhyChNum;                 ///physical channel number
	DWORD dwServiceId;                ///service Id, Refer to partial_reception_descriptor in ARIB STD-B10_V4_3-E1.pdf --- Added by Frank Lin@20070312
	DWORD dwAudioMode;                ///audio mode
	DWORD dwTSId;                     ///TS ID, refer to ts_information_descriptor in ARIB STD-B10_V4_3-E1.pdf --- Added by Frank Lin@20070312
	DWORD dwCapLanguageId;            ///closed caption language Id	///span70410, total language number
} CHANNEL_INFO, *PCHANNEL_INFO;
///span70411, add for AP layer channel scan for ISDB-T system


///span70411, add for AP layer channel set for ISDB-T system
///structrue for set channel
typedef struct T_ISDB_T_SETCHANNEL
{
    DWORD dwPhyChNum;
    DWORD dwServiceID;
} ISDB_T_SETCHANNEL, *PISDB_T_SETCHANNEL;
///span70411, add for AP layer channel set for ISDB-T system


///span70412, add for AP layer channel set for ISDB-T system
///structrue for RF signal status
typedef struct T_ISDB_T_SIGNALSTATUS
{
	DWORD dwSize;	///size of this structure
	BOOL bOFDMsync;	///OFDM demodulator syncronize
	BOOL bTSsync;	///TS data syncronize
    DWORD dwAGC;	///AGC value from module
    double dbCN;	///Cn value from module
    DWORD dwBEN;	
    DWORD dwBEC;	///BER=BEN/BEC, error num/count num
    DWORD dwSignalLevel;
} ISDB_T_SIGNALSTATUS, *PISDB_T_SIGNALSTATUS;
///span70412, add for AP layer channel set for ISDB-T system

typedef struct T_DTV_PLAYBACK_BUFFERIN
{
    DWORD       Operation;
    PBYTE       pBufIn;
    DWORD       BufInSize;    
} DTV_PLAYBACK_BUFFERIN, *PDTV_PLAYBACK_BUFFERIN;


///span70228, add video control code
typedef struct T_DTV_VIDEO_DIM
{
    DWORD dwX;
    DWORD dwY;
} DTV_VIDEO_DIM, *PDTV_VIDEO_DIM;
///span70228, add video control code

///span70419, add audio routing control
typedef struct T_DTV_AUDIO_ROUTING_CTRL
{
    BOOL bTVaudioRouting;
} DTV_AUDIO_ROUTING_CTRL, *PDTV_AUDIO_ROUTING_CTRL;
///span70419, add audio routing control


///span70327, add for Tuner register access
typedef struct  T_DTV_TUNER_REGISTER
{
	BOOL bRead;                       ///read or write
	BYTE bChipID;					  ///chip ID, 8 bits
	DWORD dwIndex;                    ///register index
	DWORD dwRegRead;                  ///read data
	DWORD dwRegWrite;                 ///Write data
	DWORD dwMask;                     ///bits mask
} DTV_TUNER_REGISTER, *PDTV_TUNER_REGISTER;
///span70327, add for Tuner register access

#define ISDB_ES_AUDIO_MODE_DUAL_MONO        0x00000001
#define ISDB_ES_AUDIO_MODE_MONO             0x00000010
#define ISDB_ES_AUDIO_MODE_STEREO           0x00000100

typedef struct  T_ISDB_ES_AUDIO_MODE
{
	DWORD dwCurrMode;           ///current audio mode in used
	DWORD dwAllModes;           ///all equipped modes in this Audio ES --- Modified by Frank Lin@20070312

} ISDB_ES_AUDIO_MODE, *PISDB_ES_AUDIO_MODE;

typedef struct  T_ISDB_AUDIO_ES_NUM
{
	DWORD dwCurrNum;            ///current audio ES number, start from 0 --- --- Added by Frank Lin@20070312
	DWORD dwTotalNum;           ///total audio ES number

} ISDB_AUDIO_ES_NUM, *PISDB_AUDIO_ES_NUM;


#define ISDB_VIDEO_MODE_4_3             0x00000001
#define ISDB_VIDEO_MODE_16_9            0x00000010

typedef struct  T_ISDB_VIDEO_MODE
{
	DWORD dwCurrNum;            ///current video aspect ratio

} ISDB_VIDEO_MODE, *PISDB_VIDEO_MODE;

typedef struct  T_ISDB_VIDEO_ES_NUM
{
	DWORD dwCurrNum;            ///current video ES number
	DWORD dwTotalNum;           ///total video ES number

} ISDB_VIDEO_ES_NUM, *PISDB_VIDEO_ES_NUM;

typedef struct  T_ISDB_CAPTION_LANGUAGE
{
	DWORD dwCurrLanguageNum;    ///current language ID, start from 0 --- Modified by Frank Lin@20070312
	DWORD dwTotalLanguageNum;   ///total language number

} ISDB_CAPTION_LANGUAGE, *PISDB_CAPTION_LANGUAGE;

///span70501, add for copy protect 
typedef struct  T_ISDB_COPY_PROTECT_CONTENT
{
	DWORD dwCopyCtlType;        ///copy control type, 2 bits defined
	DWORD dwRecCtlData;         ///digital recording control data, 2 bits defined
	DWORD dwEncryptMode;        ///encyption mode, 1 bit defined
} ISDB_COPY_PROTECT_CONTENT, *PISDB_COPY_PROTECT_CONTENT;
///span70501, add for copy protect 

///structrue for channel service info, not used in warpper layer
typedef struct  T_ISDB_T_CH_SERVICE_INFO
{
	DWORD dwPhyChNum;                       ///physical channel number ex:13~62
	DWORD dwServiceId;                      ///service Id 
	S3DIGIT_CHANNEL_NUM ChNum;              ///3-digit channel number ex:611
	ISDB_ES_AUDIO_MODE AudioModes;          ///audio supported mode
	ISDB_AUDIO_ES_NUM AudioESnums;          ///total audio ES numbers
	ISDB_VIDEO_MODE VideoMode;              ///video supported mode
	ISDB_VIDEO_ES_NUM VideoESnums;          ///total video ES numbers
	ISDB_CAPTION_LANGUAGE LanguageNums;     ///total caption language numbers
	ISDB_COPY_PROTECT_CONTENT CopyProtects;  ///copy protect info
} ISDB_T_CH_SERVICE_INFO, *PISDB_T_CH_SERVICE_INFO;
///span70423, add for AP layer channel service info


typedef struct  T_ISDB_SDT
{
	DWORD dwSDTSize;          	///SDT size in byte
	BYTE SDT[256];        		///current service name, reside in SDT
} ISDB_SDT, *PISDB_SDT;

typedef struct  T_ISDB_NIT
{
	DWORD dwNITSize;          	///NIT size in byte
	BYTE NIT[256];        		///NIT data
} ISDB_NIT, *PISDB_NIT;

typedef struct  T_ISDB_PMT
{
	DWORD dwPMTSize;          	///PMT size in byte
	BYTE PMT[256];        		///MPT data
} ISDB_PMT, *PISDB_PMT;

///structrue for channel detail info, not used in warpper layer
typedef struct  T_ISDB_T_CH_DETAIL_INFO
{
	ISDB_NIT NITcontent;
	ISDB_SDT SDTcontent;
	ISDB_PMT PMTcontent;
} ISDB_T_CH_DETAIL_INFO, *PISDB_T_CH_DETAIL_INFO;
///span70423, add for AP layer channel detail info

///span70501, start
typedef struct  T_ISDB_EWS_INFO
{
	DWORD dwServiceId;          ///service id
	DWORD dwStartEndFlag;       ///start end flag
	DWORD dwSignalLevel;        ///Signal level
	///DWORD dwAreaCodeLen;        ///area code length
	DWORD dwAreaCodeNumber;        ///area code number, max 128
	///BYTE AreaCode[53];           ///arar code, 12-bit --- Modified by Frank Lin@20070312
	DWORD dwAreaCode[128];			///area code

} ISDB_EWS_INFO, *PISDB_EWS_INFO;

typedef struct  T_ISDB_PROGCLKREF
{
	DWORD dwHiNum;      ///bit 63~ bit32 PCR
	DWORD dwLoNum;      ///bit 31~ bit0 PCR

} ISDB_PROGCLKREF, *PISDB_PROGCLKREF;

typedef struct  T_ISDB_TSTOT
{
	DWORD dwHiNum;      ///bit 63~ bit32 TOT
	DWORD dwLoNum;      ///bit 31~ bit0 TOT

} ISDB_TSTOT, *PISDB_TSTOT;

//ColorIndex: 
//			0 --- BACKGROUND
//			1 --- RED
//			2 --- GREEN
//			3 --- YELLOW
//			4 --- BLUE
//			5 --- MAGENTA
//			6 --- CYAN
//			7 --- WHITE
//RGB565: 16-bit, R use first 5 bits, G use following 6 bits, B use final 5 bits.
typedef struct  T_ISDB_CCSETCOLOR
{
	DWORD dwColorIndex;
	DWORD dwRGB565;
} ISDB_CCSETCOLOR, *PISDB_CCSETCOLOR;


///        dwCC_WindowID: Closed Caption indicator
///			0 --- Closed Cpation Font Window
///			1 --- Closed Cpation Area Window
///        dwBlendingRatio: blending ratio number
typedef struct  T_ISDB_CCALPHABLENDRATIO
{
	DWORD dwCC_WindowID;
	DWORD dwBlendingRatio;
} ISDB_CCALPHABLENDRATIO, *PISDB_CCALPHABLENDRATIO;


/// --------------EPG---------------------------
typedef struct  T_ISDB_EPG_PRG_TIME
{
	BYTE bProgramDate[5];    ///program Date in EPG, 40 bits
	DWORD dwProgramDuration;    ///program duration in EPG, 24 bits
} ISDB_EPG_PRG_TIME, *PISDB_EPG_PRG_TIME;

///span70425, modified
typedef struct  T_ISDB_EPG_PRG_NAME
{
	DWORD dwProgramNameLength;	///program name length in EPG
	BYTE bProgramName[255];    	///program name in EPG, 96 bytes usually
} ISDB_EPG_PRG_NAME, *PISDB_EPG_PRG_NAME;

///span70425, modified
typedef struct  T_ISDB_EPG_PRG_INFO
{
	DWORD dwProgramInfoLength;	///program info length in EPG
	BYTE bProgramInfo[255];    	///program info in EPG, 192 bytes usually
} ISDB_EPG_PRG_INFO, *PISDB_EPG_PRG_INFO;

///span70425, modified
typedef struct  T_ISDB_EPG_PRG_CONTENT
{
	ISDB_EPG_PRG_TIME ProgTime;
	ISDB_EPG_PRG_NAME ProgName;
	ISDB_EPG_PRG_INFO ProgInfo;
} ISDB_EPG_PRG_CONTENT, *PISDB_EPG_PRG_CONTENT;

///span70425, modified
typedef struct  T_ISDB_EPG_PRG
{
	ISDB_EPG_PRG_CONTENT ThisProgram;	///Program info of this.
	BOOL bHasNextProgram;				///Is there next EPG info? TRUE for yes, FALSE for no.
} ISDB_EPG_INFO, *PISDB_EPG_INFO;
/// --------------EPG---------------------------

typedef struct  T_ISDB_NETWORK_NAME
{
	DWORD dwNetworkNameLength;	///Network Name length
	BYTE bNetworkName[255];    	///Network Name, 255 is Max in NIT, 20 bytes usually
} ISDB_NETWORK_NAME, *PISDB_NETWORK_NAME;
///span70501, end

///span70509, add function
typedef struct  T_ISDB_TS_NAME
{
	DWORD dwTSNameLength;	///TS Name length
	BYTE bTSName[255];    	///TS Name
} ISDB_TS_NAME, *PISDB_TS_NAME;
///span70509, add function







#define             DTV_STD_CODE_BASE                           0x00010000
#define             DTV_ISDB_CODE_BASE                          (DTV_STD_CODE_BASE+0x00001000)	///span70424, ISDB specified code base

#define             IOCTL_DTV_MODULE_POWER_ON                (DTV_STD_CODE_BASE+1)        ///power on the DTV module
#define             IOCTL_DTV_MODULE_POWER_OFF               (DTV_STD_CODE_BASE+2)        ///power off the DTV module
#define             IOCTL_DTV_ACCESS_SERVICE                 (DTV_STD_CODE_BASE+3)        ///try to access service
#define             IOCTL_DTV_CHANNEL_SCAN                   (DTV_STD_CODE_BASE+4)        ///channel scan
#define             IOCTL_DTV_CURRENT_CHANNEL_LOCKED         (DTV_STD_CODE_BASE+5)        ///channel locked
#define             IOCTL_DTV_MODULE_VIDEO_ON                (DTV_STD_CODE_BASE+6)        ///enable module video output
#define             IOCTL_DTV_MODULE_VIDEO_OFF               (DTV_STD_CODE_BASE+7)        ///disable module video output
#define             IOCTL_DTV_MODULE_AUDIO_ON                (DTV_STD_CODE_BASE+8)        ///enable module audio output
#define             IOCTL_DTV_MODULE_AUDIO_OFF               (DTV_STD_CODE_BASE+9)        ///disable module audio output
#define             IOCTL_DTV_MODULE_OSD_ON                  (DTV_STD_CODE_BASE+10)       ///enable module OSD output
#define             IOCTL_DTV_MODULE_OSD_OFF                 (DTV_STD_CODE_BASE+11)       ///disable module OSD output
#define             IOCTL_DTV_CHANNEL_UP                     (DTV_STD_CODE_BASE+12)       ///channel up
#define             IOCTL_DTV_CHANNEL_DOWN                   (DTV_STD_CODE_BASE+13)       ///channel down
#define             IOCTL_DTV_MODULE_VOLUME_UP               (DTV_STD_CODE_BASE+14)       ///module audio up
#define             IOCTL_DTV_MODULE_VOLUME_DOWN             (DTV_STD_CODE_BASE+15)       ///module audio down
#define             IOCTL_DTV_GET_INFO                       (DTV_STD_CODE_BASE+16)       ///get module / service info
#define             IOCTL_DTV_SET_VIDEO_OUT_POSTION          (DTV_STD_CODE_BASE+17)       ///set camera i/f output video position
#define             IOCTL_DTV_SET_VIDEO_OUT_SIZE             (DTV_STD_CODE_BASE+18)       ///set camera i/f output video size
#define             IOCTL_DTV_SET_VIDEO_IN_SIZE              (DTV_STD_CODE_BASE+19)       ///set camera i/f input video size
#define             IOCTL_DTV_MODULE_VOLUME_SET              (DTV_STD_CODE_BASE+20)       ///module audio volume set
#define             IOCTL_DTV_TUNER_REGISTER_ACCESS          (DTV_STD_CODE_BASE+21)       ///Tuner register access
#define             IOCTL_DTV_CHANNEL_SET                    (DTV_STD_CODE_BASE+22)       ///set channel
#define             IOCTL_DTV_MODULE_OSD_CTRL                (DTV_STD_CODE_BASE+23)       ///control module OSD output
#define             IOCTL_DTV_SYS_BATTERY_STATUS             (DTV_STD_CODE_BASE+24)       ///system battery status
#define             IOCTL_DTV_MODULE_OSD_SIGNAL_LEVEL        (DTV_STD_CODE_BASE+25)       ///set OSD signal level
#define             IOCTL_DTV_MODULE_IO_LOOPBACK_TEST        (DTV_STD_CODE_BASE+26)       ///test module IO
#define             IOCTL_DTV_MODULE_SUBTITLE_ON	         (DTV_STD_CODE_BASE+27)       ///set sub-title on
#define             IOCTL_DTV_MODULE_SUBTITLE_OFF	         (DTV_STD_CODE_BASE+28)       ///set sub-title off
#define             IOCTL_DTV_MODULE_SIGNAL_STATUS	         (DTV_STD_CODE_BASE+29)       ///get module signal status
#define             IOCTL_DTV_MODULE_RECORD_OPERATION        (DTV_STD_CODE_BASE+30)       ///commmond for TV record operation
#define             IOCTL_DTV_MODULE_PLAYBACK_OPERATION      (DTV_STD_CODE_BASE+31)       ///commmond for TV playback operation
#define             IOCTL_DTV_AUDIO_ROUTING_CTRL             (DTV_STD_CODE_BASE+32)       ///DTV audio routing control
#define             IOCTL_DTV_MODULE_EVENT_HANDLE_QUERY      (DTV_STD_CODE_BASE+33)       ///query module event handle
#define             IOCTL_DTV_MODULE_VIDEO_STILLSHOT         (DTV_STD_CODE_BASE+34)       ///get current module TV video output stillshot
#define             IOCTL_DTV_VIDEO_STILLSHOT         		 (DTV_STD_CODE_BASE+35)       ///get current TV screnn stillshot
#define             IOCTL_DTV_MODULE_FW_VERSION       		 (DTV_STD_CODE_BASE+36)       ///get TV module version
#define             IOCTL_DTV_TS_DATA_ENCRYPT_ENABLE   		 (DTV_STD_CODE_BASE+37)       ///enable the encryption function on TS data
#define             IOCTL_DTV_TS_DATA_ENCRYPT_DISABLE  		 (DTV_STD_CODE_BASE+38)       ///disable the encryption function on TS data

#define             IOCTL_DTV_ISDB_CURRENT_SERVICE_INFO      (DTV_ISDB_CODE_BASE+1)       ///DTV current service info
#define             IOCTL_DTV_ISDB_CURRENT_CH_DETAIL_INFO    (DTV_ISDB_CODE_BASE+2)       ///DTV current channel detail info
#define             IOCTL_DTV_ISDB_CURRENT_COPY_PROTECT_INFO (DTV_ISDB_CODE_BASE+3)       ///DTV current channel copy protect info
#define             IOCTL_DTV_ISDB_CURRENT_EWS_INFO          (DTV_ISDB_CODE_BASE+4)       ///DTV current EWS info
#define             IOCTL_DTV_ISDB_LASTEST_PCR               (DTV_ISDB_CODE_BASE+5)       ///DTV latest PCR
#define             IOCTL_DTV_ISDB_LASTEST_TOT               (DTV_ISDB_CODE_BASE+6)       ///DTV latest TOT
#define             IOCTL_DTV_ISDB_CLOSEDCAPTION_Y_OFFSET    (DTV_ISDB_CODE_BASE+7)       ///DTV closed caption y offset
#define             IOCTL_DTV_ISDB_CLOSEDCAPTION_COLOR_SET   (DTV_ISDB_CODE_BASE+8)       ///DTV closed caption set color define
#define             IOCTL_DTV_ISDB_CLOSEDCAPTION_ALPHABLEND  (DTV_ISDB_CODE_BASE+9)       ///DTV closed caption alpha blend ratio
#define             IOCTL_DTV_ISDB_CURRENT_EPG               (DTV_ISDB_CODE_BASE+10)      ///DTV current EPG 
#define             IOCTL_DTV_ISDB_CURRENT_NETWORK_NAME      (DTV_ISDB_CODE_BASE+11)      ///DTV current network name
#define             IOCTL_DTV_ISDB_SET_AUDIO_ES_NUM      	 (DTV_ISDB_CODE_BASE+12)      ///DTV set audio ES num
#define             IOCTL_DTV_ISDB_SET_AUDIO_MODE      	     (DTV_ISDB_CODE_BASE+13)      ///DTV set audio mode
#define             IOCTL_DTV_ISDB_SET_VIDEO_ES_NUM      	 (DTV_ISDB_CODE_BASE+14)      ///DTV set video ES num
#define             IOCTL_DTV_ISDB_SET_CC_LANGUAGE_NUM    	 (DTV_ISDB_CODE_BASE+15)      ///DTV set Closed caption language
#define             IOCTL_DTV_ISDB_GET_TS_STREAM_RATE    	 (DTV_ISDB_CODE_BASE+16)      ///get current TS recording stream rate
#define             IOCTL_DTV_ISDB_SET_TS_STREAM_RATE    	 (DTV_ISDB_CODE_BASE+17)      ///set current TS playback stream rate
#define             IOCTL_DTV_ISDB_CURRENT_TS_NAME           (DTV_ISDB_CODE_BASE+18)      ///DTV current TS name







///span70312, add notify index list
#define NOTIFY_FUNC_MASK            0xFFFF0000
#define NOTIFY_ITEM_MASK            0x0000FFFF

#define NOTIFY_PROG_SET             0x00010000
#define NOTIFY_NETWORK_SET          0x00020000
#define NOTIFY_SERVICE_SET          0x00040000

#define NOTIFY_SHM_SET              0x01000000
#define NOTIFY_MFI_SET              0x02000000



///program class
#define NOTIFY_PROG_AV_ES_CHG           (NOTIFY_PROG_SET|0x0001)        ///notify index for Audio/Video ES changed
#define NOTIFY_PROG_AUDIO_MODE_CHG      (NOTIFY_PROG_SET|0x0002)        ///notify index for Audio mode changed
#define NOTIFY_PROG_EIT_CHG             (NOTIFY_PROG_SET|0x0004)        ///notify index for EIT changed
#define NOTIFY_PROG_COPY_INFO_CHG       (NOTIFY_PROG_SET|0x0008)        ///notify index for copy protection info changed
#define NOTIFY_PROG_TEXT_MODE_CHG       (NOTIFY_PROG_SET|0x0010)        ///notify index for Text mode changed
#define NOTIFY_PROG_VIDEO_MODE_CHG      (NOTIFY_PROG_SET|0x0020)        ///notify index for Video mode changed
#define NOTIFY_PROG_CLOSECAPTION_CHG    (NOTIFY_PROG_SET|0x0040)        ///notify index for closed caption mode changed

///network class
#define NOTIFY_NETWORK_CHG              (NOTIFY_NETWORK_SET|0x0001)     ///notify index for NIT changed
#define NOTIFY_TOT_CHG              	(NOTIFY_NETWORK_SET|0x0002)     ///notify index for TOT changed
#define NOTIFY_SDT_CHG              	(NOTIFY_NETWORK_SET|0x0004)     ///notify index for SDT changed

///service class
#define NOTIFY_SERVICE_SDTT             (NOTIFY_SERVICE_SET|0x0001)     ///notify index for SDTT display & mail changed
#define NOTIFY_SERVICE_EWS              (NOTIFY_SERVICE_SET|0x0002)     ///notify index for EWS changed


///SHM class
#define NOTIFY_SHM_ERROR                (NOTIFY_SHM_SET|0x0001)         ///notify index for SHM driver error
#define NOTIFY_SHM_INIT_READY           (NOTIFY_SHM_SET|0x0002)         ///notify index for SHM driver init ready
#define NOTIFY_SHM_RING_EMPTY           (NOTIFY_SHM_SET|0x0004)         ///notify index for ring buffer empty
#define NOTIFY_SHM_RING_FULL            (NOTIFY_SHM_SET|0x0008)         ///notify index for ring buffer full
#define NOTIFY_SHM_DATA_READYL          (NOTIFY_SHM_SET|0x0010)         ///notify index for data ready for read
///span70312, add notify index list







#endif  ///#ifndef  __DTV_DEF_H__