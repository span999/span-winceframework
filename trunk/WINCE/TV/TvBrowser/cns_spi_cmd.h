/* MiTAC Int'l Corp. 2001~2006

================================================================================
Description:
    header file of CCns.cpp

================================================================================
Release notes

Modifier	        sign	        date		    Description
--------------------------------------------------------------------------------
Span Liu                            2006/11/07      first release
Span Liu            span1215        2006/12/15      Add IOCTL_CNSCMD_LOOPBACK_TEST IO code.
*/



#ifndef __CNS_SPI_CMD_H__
#define __CNS_SPI_CMD_H__



///define of CNS SPI communication frame

typedef UINT8 CNS_SPI_FRAME_ATTRIBUTE;
typedef UINT8 CNS_SPI_FRAME_ID;
typedef UINT16 CNS_SPI_FRAME_PAYLOAD_LENGTH;

typedef struct T_CNS_SPI_FRAME_HEADER
{
    CNS_SPI_FRAME_ATTRIBUTE             aAttribute;
    CNS_SPI_FRAME_ID                    idID;
    CNS_SPI_FRAME_PAYLOAD_LENGTH        plPayLoadLength;    

} CNS_SPI_FRAME_HEADER, *PCNS_SPI_FRAME_HEADER;

typedef UINT32* CNS_SPI_FRAME_PAYLOAD_POINTER;

typedef struct T_CNS_SPI_FRAME_PACK
{
    CNS_SPI_FRAME_HEADER                hHeader;
    CNS_SPI_FRAME_PAYLOAD_POINTER       pPayload;

} CNS_SPI_FRAME_PACK, *PCNS_SPI_FRAME_PACK;


#define MAX_PAYLOAD_SIZE_IN_BYTE                    (4096)

///define of ID value for CNS API frame 
    ///attribute
#define CNS_SPI_FRAME_ATTRIBUTE_TYPE_MASK           0xF0
#define CNS_SPI_FRAME_ATTRIBUTE_SEQNUM_MASK         0x0F

#define CNS_SPI_FRAME_TYPE_HSOT_COMMAND             0x1
#define CNS_SPI_FRAME_TYPE_NORMAL_RSP               0x2
#define CNS_SPI_FRAME_TYPE_ABNORMAL_RSP             0x3
#define CNS_SPI_FRAME_TYPE_UNKNOW                   0x4
#define CNS_SPI_FRAME_TYPE_STREAM_RSP               0x5

#define CNS_SPI_FRAME_ATTRIBUTE_TYPE_OFFSET         4

///#define CNS_SPI_FRAME_ATTRIBUTE_HSOT_COMMAND            (CNS_SPI_FRAME_TYPE_HSOT_COMMAND<<CNS_SPI_FRAME_ATTRIBUTE_TYPE_OFFSET)
///#define CNS_SPI_FRAME_ATTRIBUTE_NORMAL_RSP              (CNS_SPI_FRAME_TYPE_HSOT_COMMAND<<CNS_SPI_FRAME_ATTRIBUTE_TYPE_OFFSET)
///#define CNS_SPI_FRAME_ATTRIBUTE_ABNORMAL_RSP            (CNS_SPI_FRAME_TYPE_HSOT_COMMAND<<CNS_SPI_FRAME_ATTRIBUTE_TYPE_OFFSET)
///#define CNS_SPI_FRAME_ATTRIBUTE_STREAM_RSP              (CNS_SPI_FRAME_TYPE_HSOT_COMMAND<<CNS_SPI_FRAME_ATTRIBUTE_TYPE_OFFSET)
#define CNS_SPI_FRAME_ATTRIBUTE_HSOT_COMMAND            0x10
#define CNS_SPI_FRAME_ATTRIBUTE_NORMAL_RSP              0x20
#define CNS_SPI_FRAME_ATTRIBUTE_ABNORMAL_RSP            0x30
#define CNS_SPI_FRAME_ATTRIBUTE_STREAM_RSP              0x50

    ///Frame ID
#define CNS_SPI_FRAME_ID_CATEGORY_MASK              0xF0
#define CNS_SPI_FRAME_ID_INDEXCODE_MASK             0x0F

#define CNS_SPI_FRAME_ID_CATEGORY_CHANNEL_CTRL      0x0
#define CNS_SPI_FRAME_ID_CATEGORY_CHANNEL_INFO      0x1
#define CNS_SPI_FRAME_ID_CATEGORY_VIDEO_CTRL        0x2
#define CNS_SPI_FRAME_ID_CATEGORY_UNDEF1            0x3
#define CNS_SPI_FRAME_ID_CATEGORY_AUDIO_CTRL        0x4
#define CNS_SPI_FRAME_ID_CATEGORY_UNDEF2            0x5
#define CNS_SPI_FRAME_ID_CATEGORY_STREAM_CTRL       0x6
#define CNS_SPI_FRAME_ID_CATEGORY_SYSTEM_RELATE     0x7
#define CNS_SPI_FRAME_ID_CATEGORY_OSD_CTRL          0x8

#define CNS_SPI_FRAME_ID_CATEGORY_OFFSET            4

#define CNS_SPI_FRAME_ID_CHANNEL_CTRL               0x00
#define CNS_SPI_FRAME_ID_CHANNEL_INFO               0x10
#define CNS_SPI_FRAME_ID_VIDEO_CTRL                 0x20
#define CNS_SPI_FRAME_ID_UNDEF1                     0x30
#define CNS_SPI_FRAME_ID_AUDIO_CTRL                 0x40
#define CNS_SPI_FRAME_ID_UNDEF2                     0x50
#define CNS_SPI_FRAME_ID_STREAM_CTRL                0x60
#define CNS_SPI_FRAME_ID_SYSTEM_RELATE              0x70
#define CNS_SPI_FRAME_ID_OSD_CTRL                   0x80
#define CNS_SPI_FRAME_ID_DLS_RELATE                 0xA0  ///add in spi comm spec 1.25
#define CNS_SPI_FRAME_ID_TUNER_RELATE               0xB0


///CNS SPI command list
#define CNS_SPI_CMD_ACCESS_SVC                      (CNS_SPI_FRAME_ID_CHANNEL_CTRL|0x01)
#define CNS_SPI_CMD_SVC_SCAN_ON                     (CNS_SPI_FRAME_ID_CHANNEL_CTRL|0x04)

#define CNS_SPI_CMD_GET_SVC_LIST                    (CNS_SPI_FRAME_ID_CHANNEL_INFO|0x00)
#define CNS_SPI_CMD_GET_SVC_INFO                    (CNS_SPI_FRAME_ID_CHANNEL_INFO|0x01)
#define CNS_SPI_CMD_GET_SIG_STAT                    (CNS_SPI_FRAME_ID_CHANNEL_INFO|0x03)
#define CNS_SPI_CMD_GET_SIG_BER                     (CNS_SPI_FRAME_ID_CHANNEL_INFO|0x04)
#define CNS_SPI_CMD_GET_SIG_SNR                     (CNS_SPI_FRAME_ID_CHANNEL_INFO|0x05)

#define CNS_SPI_CMD_DISP_MODE                       (CNS_SPI_FRAME_ID_VIDEO_CTRL|0x00)
#define CNS_SPI_CMD_DISP_ON                         (CNS_SPI_FRAME_ID_VIDEO_CTRL|0x01)
#define CNS_SPI_CMD_SET_CONTRAST                    (CNS_SPI_FRAME_ID_VIDEO_CTRL|0x02)
#define CNS_SPI_CMD_SET_SATURATION                  (CNS_SPI_FRAME_ID_VIDEO_CTRL|0x03)
#define CNS_SPI_CMD_SET_BRIGHTNESS                  (CNS_SPI_FRAME_ID_VIDEO_CTRL|0x04)
#define CNS_SPI_CMD_GET_STILLSHOT                   (CNS_SPI_FRAME_ID_VIDEO_CTRL|0x05)
#define CNS_SPI_CMD_START_RECORD                    (CNS_SPI_FRAME_ID_VIDEO_CTRL|0x06)
#define CNS_SPI_CMD_START_PLAYBACK                  (CNS_SPI_FRAME_ID_VIDEO_CTRL|0x07)
#define CNS_SPI_CMD_SLS_CHECK                       (CNS_SPI_FRAME_ID_VIDEO_CTRL|0x08)  ///add in spi comm spec 1.25

#define CNS_SPI_CMD_SET_VOL                         (CNS_SPI_FRAME_ID_AUDIO_CTRL|0x00)
#define CNS_SPI_CMD_MUTE_ON                         (CNS_SPI_FRAME_ID_AUDIO_CTRL|0x01)

#define CNS_SPI_CMD_STRM_PLAYBACK                   (CNS_SPI_FRAME_ID_STREAM_CTRL|0x00)
#define CNS_SPI_CMD_STRM_RECORD                     (CNS_SPI_FRAME_ID_STREAM_CTRL|0x01)
#define CNS_SPI_CMD_STRM_STOP                       (CNS_SPI_FRAME_ID_STREAM_CTRL|0x02)
#define CNS_SPI_CMD_STRM_RESUME                     (CNS_SPI_FRAME_ID_STREAM_CTRL|0x03)  ///add in spi comm spec 1.23
#define CNS_SPI_CMD_STRM_PAUSE                      (CNS_SPI_FRAME_ID_STREAM_CTRL|0x04)  ///add in spi comm spec 1.23

#define CNS_SPI_CMD_BOOT_SUCCESS_INFO               (CNS_SPI_FRAME_ID_SYSTEM_RELATE|0x00)
#define CNS_SPI_CMD_GET_VERSION                     (CNS_SPI_FRAME_ID_SYSTEM_RELATE|0x01)
#define CNS_SPI_CMD_BATTERY_STATUS                  (CNS_SPI_FRAME_ID_SYSTEM_RELATE|0x02)
#define CNS_SPI_CMD_MODULE_CHECK                    (CNS_SPI_FRAME_ID_SYSTEM_RELATE|0x03)  ///add in spi comm spec 1.3
#define CNS_SPI_CMD_SYSTEMINFO_LOAD                 (CNS_SPI_FRAME_ID_SYSTEM_RELATE|0x04)  ///add in spi comm spec 1.32
#define CNS_SPI_CMD_SYSTEMINFO_SAVE                 (CNS_SPI_FRAME_ID_SYSTEM_RELATE|0x05)  ///add in spi comm spec 1.32

#define CNS_SPI_CMD_OSD_CONTENTS1                   (CNS_SPI_FRAME_ID_OSD_CTRL|0x00)
#define CNS_SPI_CMD_OSD_CONTENTS2                   (CNS_SPI_FRAME_ID_OSD_CTRL|0x01)
#define CNS_SPI_CMD_OSD_CONTENTS3                   (CNS_SPI_FRAME_ID_OSD_CTRL|0x02)
#define CNS_SPI_CMD_OSD_SET_FOCUS                   (CNS_SPI_FRAME_ID_OSD_CTRL|0x03)        ///add in spi comm spec 1.3
#define CNS_SPI_CMD_OSD_GET_FOCUS                   (CNS_SPI_FRAME_ID_OSD_CTRL|0x04)        ///add in spi comm spec 1.3
#define CNS_SPI_CMD_OSD_VOLUME_SET                  (CNS_SPI_FRAME_ID_OSD_CTRL|0x05)        ///span70125, add function for OSD volume show

#define CNS_SPI_CMD_DLS_CHECK                       (CNS_SPI_FRAME_ID_DLS_RELATE|0x04) ///add in spi comm spec 1.25

#define CNS_SPI_CMD_TUNER_READ_REG                  (CNS_SPI_FRAME_ID_TUNER_RELATE|0x00)
#define CNS_SPI_CMD_TUNER_WRITE_REG                 (CNS_SPI_FRAME_ID_TUNER_RELATE|0x01)
#define CNS_SPI_CMD_TUNER_GET_CKS                   (CNS_SPI_FRAME_ID_TUNER_RELATE|0x02)
#define CNS_SPI_CMD_LOOPBACK_TEST                   (CNS_SPI_FRAME_ID_TUNER_RELATE|0x03)    ///span1215, add for loopback test



///command table
typedef struct T_CNS_SPI_CMD_HEADER_TABLE
{
    CNS_SPI_FRAME_ATTRIBUTE         index;
    CNS_SPI_FRAME_ATTRIBUTE         attribute;
    CNS_SPI_FRAME_ID                id;
    CNS_SPI_FRAME_PAYLOAD_LENGTH    payloadlen;

} CNS_SPI_CMD_HEADER_TABLE, *PCNS_SPI_CMD_HEADER_TABLE;

#define END_OF_HEADER_TABLE                         0xFF



///Command error code define
#define CNS_SPI_CMD_ERROR_INVALID_PARAMETER         0xF0000001
#define CNS_SPI_CMD_ERROR_NOT_FOUND                 0xF0000002
#define CNS_SPI_CMD_ERROR_NOT_ACTIVE                0xF0000003
#define CNS_SPI_CMD_ERROR_NOT_DEFINE1               0xF0000004
#define CNS_SPI_CMD_ERROR_COMMAND_FAIL              0xF0000005
#define CNS_SPI_CMD_ERROR_NOT_DEFINE2               0xF0000006
#define CNS_SPI_CMD_ERROR_NOT_STREAM                0xF0000007
#define CNS_SPI_CMD_ERROR_OVERFLOW_STREAM           0xF0000008
#define CNS_SPI_CMD_ERROR_NOT_DETECT                0xF0000009  ///add in spi comm spec 1.25





#endif  ///#ifndef  __CNS_SPI_CMD_H__