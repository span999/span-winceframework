#ifndef __SYSPOWERSRV_DBUS_H__
#define __SYSPOWERSRV_DBUS_H__


#include "sysPowerCOMM.h"




int sysPowerSRVInit( void );
int sysPowerSRVDeinit( void );

int sPSgetCPUActivatedNum( void );
int sPSsetCPUActivatedNum( int num );
int sPSsetCPUDVFS( int num );
int sPSsetCPUspeed( int nMHz );
int sPSsetCPUsuspend( void );

int sPSloopbackTest( int test );



#endif /* ##ifndef __SYSPOWERSRV_DBUS_H__ */
