#ifndef __SPLIBDBGMSG_H__
#define __SPLIBDBGMSG_H__



DWORD spLibDbgMsg_Init( DWORD dwParam );
DWORD spLibDbgMsg_Deinit( DWORD dwParam );
DWORD spLibDbgMsg( DWORD dwFlag, LPCTSTR szError,... );
DWORD spLibDbgMsg_Dlg( LPCTSTR szError,... );
DWORD spLibDbgMsg_NK( LPCTSTR szError,... );
DWORD spLibDbgMsg_Log( LPCTSTR szError,... );


#endif	///#ifndef __SPLIBDBGMSG_H__