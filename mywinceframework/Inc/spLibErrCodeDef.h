#ifndef __SPLIBERRCODEDEF_H__
#define __SPLIBERRCODEDEF_H__


typedef 	DWORD 	DWSPERRCODE;

#define 		SPERRCODE_LIB_SET			0x00010000
#define 		SPERRCODE_DLL_SET			0x00020000
#define 		SPERRCODE_EXE_SET			0x00040000

#define 		SPERRCODE_OFFSET			0x00000010
#define 		SPLIBERRCODE_BASE			( SPERRCODE_LIB_SET | SPERRCODE_OFFSET )

#define 		SPLIBERRCODE_TRUE			( SPLIBERRCODE_BASE + 0 )
#define 		SPLIBERRCODE_FALSE			( SPLIBERRCODE_BASE + 1 )


#endif	///#ifndef __SPLIBERRCODEDEF_H__