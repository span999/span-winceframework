#ifndef __SYSPOWERSRV_H__
#define __SYSPOWERSRV_H__


#include "sysPowerCOMM.h"


void PowerCmdDump( struct sysPowerCmd *pCmd );
int getPowerCmdID( struct sysPowerCmd *pCmd );
int getPowerCmdID( struct sysPowerCmd *pCmd );
int setPowerCmdParam1( struct sysPowerCmd *pCmd, int iVal );
int getPowerCmdParam1( struct sysPowerCmd *pCmd );
int setPowerCmdReturn( struct sysPowerCmd *pCmd, int iVal );
int setPowerCmdRsptime( struct sysPowerCmd *pCmd, long iVal );


int getCPUActivatedNum( void );
int setCPUActivatedNum( int num );

int loopbackTest( int test );



#endif /* ##ifndef __SYSPOWERSRV_H__ */
