#ifndef __SPLIBSYSHALIOCTL_H__
#define __SPLIBSYSHALIOCTL_H__


DWORD spLibSysHALIoctl_Init( DWORD dwParam );
DWORD spLibSysHALIoctl_Deinit( DWORD dwParam );
DWORD spLibSysHALIoctl_Reboot( void );

///Kenel IO backlight access
DWORD spLibSysHALIoctl_BacklightSet( DWORD dwValue );
DWORD spLibSysHALIoctl_BacklightGet( void );
DWORD spLibSysHALIoctl_BacklightUp( void );
DWORD spLibSysHALIoctl_BacklightDown( void );

#endif	///#ifndef __SPLIBSYSHALIOCTL_H__