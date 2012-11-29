#ifndef __SYSPOWERSRV_H__
#define __SYSPOWERSRV_H__


#include "sysPowerCOMM.h"


void PowerCmdDump( struct sysPowerCmd *pCmd );
int setPowerCmdID( struct sysPowerCmd *pCmd, int iVal );
int getPowerCmdID( struct sysPowerCmd *pCmd );
int setPowerCmdParam1( struct sysPowerCmd *pCmd, int iVal );
int getPowerCmdParam1( struct sysPowerCmd *pCmd );
int setPowerCmdReturn( struct sysPowerCmd *pCmd, int iVal );
int setPowerCmdRsptime( struct sysPowerCmd *pCmd, long iVal );


int sysPowerSRVInit( void );
int sysPowerSRVDeinit( void );

int sPSgetCPUActivatedNum( void );
int sPSsetCPUActivatedNum( int num );
int sPSsetCPUDVFS( int num );
int sPSsetCPUspeed( int nMHz );

int sPSloopbackTest( int test );



#endif /* ##ifndef __SYSPOWERSRV_H__ */
