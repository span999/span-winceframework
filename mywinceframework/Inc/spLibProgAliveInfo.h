#ifndef __SPLIBPROGALIVEINFO_H__
#define __SPLIBPROGALIVEINFO_H__

DWORD spLibProgAliveInfo_Init( DWORD dwParam );
DWORD spLibProgAliveInfo_Deinit( DWORD dwParam );
HANDLE spLibProgAliveInfo_GetCheckEvent( void );
HANDLE spLibProgAliveInfo_GetResponseEvent( void );



#endif	///#ifndef __SPLIBPROGALIVEINFO_H__