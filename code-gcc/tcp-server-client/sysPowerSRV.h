#ifndef __SYSPOWERSRV_H__
#define __SYSPOWERSRV_H__


#include "sysPowerCOMM.h"


void PowerCmdDump( struct sysPowerCmd *pCmd );
int getPowerCmdID( struct sysPowerCmd *pCmd );


int getCPUActivatedNum( void );
int setCPUActivatedNum( int num );



#endif /* ##ifndef __SYSPOWERSRV_H__ */
