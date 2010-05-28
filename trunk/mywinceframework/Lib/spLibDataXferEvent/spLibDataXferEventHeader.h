#ifndef __SPLIBDATATRANSFEREVENTHEADWE_H__
#define __SPLIBDATATRANSFEREVENTHEADWE_H__


#define 	SPLIB_DATATRANSFER_EVENT_1_NAME 		TEXT("named data transfer event 1")
#define 	SPLIB_DATATRANSFER_EVENT_2_NAME 		TEXT("named data transfer event 2")
#define 	SPLIB_DATATRANSFER_MUTEX_NAME 			TEXT("named data transfer mutex")
#define		SPLIB_DATATRANSFER_1_ACK_SIGN			0xFF00FF00
#define		SPLIB_DATATRANSFER_2_ACK_SIGN			0x00FF00FF
#define		SPLIB_DATATRANSFER_PACK_SIGN			0x00FF0FFF
#define		SPLIB_DATATRANSFER_PACKSTART_SIGN		0x00FF0F0F
#define		SPLIB_DATATRANSFER_PACKEND_SIGN			0x00FF0FF0
#define		SPLIB_DATATRANSFER_EVENTDATA_CLEAR		0xFFFFFFFF


/*
+-----------+-----------+------------+---------+---------+----------+-----------+
| Pack Sign | Pack Size | Pack Start | ...data | ...data | Pack End | check sum |
+-----------+-----------+------------+---------+---------+----------+-----------+
| DWORD     | DWORD     | 4 bytes    | 4 bytes | 4 bytes | DWORD    | DWORD     | 
+-----------+-----------+------------+---------+---------+----------+-----------+
*/

typedef enum _DATATRANSFER_EVENT_STATE
{
	INIT_STATE = 0,
	WAIT_STATE,
	RECV_STATE,
	SEND_STATE,
} DATATRANSFER_EVENT_STATE;


typedef enum _DATATRANSFER_PACK_STATE
{
	NONE_DATA_STATE = 0,
	PACK_SIGN_STATE,
	PACK_SIZE_STATE,
	PACK_START_STATE,
	PACK_DATA_STATE,
	PACK_END_STATE,
	PACK_CHKSUN_STATE,
} DATATRANSFER_PACK_STATE;


///
typedef struct _LibDataTransferEventContent
{
	HANDLE 										hDataTransEvent1;	///event for host send
	HANDLE 										hDataTransEvent2;	///event for client send
///	HANDLE 										hTouchEventUp;
	HANDLE 										hDataTransEventControl;
	HANDLE										hDataTransMutex;
	PFN_DATATRANSFEREVENT_RECEIVER_CALLBACK 	pfnDataTransferReceiverCallback;
	HANDLE										hMonitorThread;
	DWORD										dwInitStep;
	DATATRANSFER_EVENT_STATE					eState;
	DATATRANSFER_PACK_STATE						pState;
	DWORD										dwPacketSize;
	DWORD										dwPacketChksum;
	DWORD										dwPacketChksumCount;
	PVOID										pRevData;
	DWORD										*pRevDataCurr;
	BOOL										bIsHost;
} LibDataTransferEventContent;

/*
typedef struct TOUCH_EVENT_STRUC{
	TOUCH_EVENT_TYPE	teType;
	INT					tiX;
	INT					tiY;
}TOUCH_EVENT_DATA, *PTOUCH_EVENT_DATA;
*/


#endif	///#ifndef __SPLIBDATATRANSFEREVENTHEADWE_H__