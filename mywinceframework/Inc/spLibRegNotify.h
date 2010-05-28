#ifndef __SPLIBREGNOTIFY_H__
#define __SPLIBREGNOTIFY_H__


/*
*/


typedef BOOL (*PFN_REG_NOTIFY_LIB_CALLBACK)( DWORD dwData );

DWORD spLibRegNotify_Init( DWORD dwParam );
DWORD spLibRegNotify_Deinit( DWORD dwParam );
HANDLE spLibRegNotify_GetNotifyHandle( void );
BOOL spLibRegNotify_SetCallback( PFN_REG_NOTIFY_LIB_CALLBACK pfnRegNotifyLibCB );



#endif	///#ifndef __SPLIBREGNOTIFY_H__