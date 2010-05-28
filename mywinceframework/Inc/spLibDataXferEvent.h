#ifndef __SPLIBDATATRANSFEREVENT_H__
#define __SPLIBDATATRANSFEREVENT_H__


typedef BOOL (*PFN_DATATRANSFEREVENT_RECEIVER_CALLBACK)( PVOID pData, DWORD dwByteSize );


DWORD spLibDataTransferEvent_Init( DWORD dwParam );
DWORD spLibDataTransferEvent_Deinit( DWORD dwParam );
BOOL spLibDataTransferEvent_IsHost( void );
BOOL spLibDataTransferEvent_SetReceiverCallback( PFN_DATATRANSFEREVENT_RECEIVER_CALLBACK pfnDataTransferReceiverCAllback );
DWORD spLibDataTransferEvent_SendData( PVOID pData, DWORD dwByteSize );


#endif	///#ifndef __SPLIBDATATRANSFEREVENT_H__