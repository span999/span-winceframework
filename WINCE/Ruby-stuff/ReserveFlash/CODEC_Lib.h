


#ifndef __COEDC_LIB_H__
#define __COEDC_LIB_H__

///define
#define WM_BIT_0	0x0001
#define WM_BIT_1	0x0002
#define WM_BIT_2	0x0004
#define WM_BIT_3	0x0008
#define WM_BIT_4	0x0010
#define WM_BIT_5	0x0020
#define WM_BIT_6	0x0040
#define WM_BIT_7	0x0080
#define WM_BIT_8	0x0100
#define WM_BIT_9	0x0200
#define WM_BIT_10	0x0400
#define WM_BIT_11	0x0800
#define WM_BIT_12	0x1000
#define WM_BIT_13	0x2000
#define WM_BIT_14	0x4000
#define WM_BIT_15	0x8000

#define WM_REGISTER_MAX_ADDRESS		0x80
#define WM_REGISTER_MIN_ADDRESS		0x00

#define WIDM_GET_REGISTER	470
#define WIDM_SET_REGISTER	471

#define WM9713L_CHIP_ID		0x4C13
#define WM9712L_CHIP_ID		0x4C12

typedef struct {
	DWORD dwAddress;	//address of register
	DWORD dwValue;		//value of setting
	DWORD dwResult;		//return value
	DWORD dwSucc;		//success or not(non-0 for no success)
}	AUDIO_TEST, *PAUDIO_TEST;
///span0510, add for audio test


///
DWORD spAccessAudioCodec(BOOL bRead, DWORD* pdwAddr, DWORD* pdwVal, DWORD* pdwRet, DWORD* pdwSuccess );


#endif	///#ifndef __COEDC_LIB_H__