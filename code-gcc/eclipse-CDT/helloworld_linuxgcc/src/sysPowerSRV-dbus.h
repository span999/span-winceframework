#ifndef __SYSPOWERSRV_DBUS_H__
#define __SYSPOWERSRV_DBUS_H__




int sysPowerSRVInit( void );
int sysPowerSRVDeinit( void );

int sPSgetCPUActivatedNum( void );
int sPSsetCPUActivatedNum( int num );
int sPSsetCPUDVFS( int On );
int sPSsetCPUspeed( int nMHz );
int sPSsetCPUsuspend( void );
int sPSsendFakeKey( int keycode );

int sPSloopbackTest( int test );



#endif /* ##ifndef __SYSPOWERSRV_DBUS_H__ */
