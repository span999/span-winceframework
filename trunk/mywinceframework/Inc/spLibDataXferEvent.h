#ifndef __SPLIBDATATRANSFEREVENT_H__
#define __SPLIBDATATRANSFEREVENT_H__


typedef BOOL (*PFN_DATATRANSFEREVENT_RECEIVER_CALLBACK)( PVOID pData, DWORD dwByteSize );


DWORD spLibDataXferEvent_Init( DWORD dwParam );
DWORD spLibDataXferEvent_Deinit( DWORD dwParam );
BOOL spLibDataXferEvent_IsHost( void );
BOOL spLibDataXferEvent_SetReceiverCallback( PFN_DATATRANSFEREVENT_RECEIVER_CALLBACK pfnDataTransferReceiverCAllback );
DWORD spLibDataXferEvent_SendData( PVOID pData, DWORD dwByteSize );


#endif	///#ifndef __SPLIBDATATRANSFEREVENT_H__