#ifndef __SYSPOWER_LOWLEVEL_H__
#define __SYSPOWER_LOWLEVEL_H__




int setCPUcoreDOWN( int CoreNum );
int setCPUcoreUP( int CoreNum );
int setCPUcoreActivatedNumber( int num );
int getCPUcoreActivatedNumber( void );
int setCPUdvfs( int iOn );
int setCPUspeed( int nSpeed );
int setCPUsuspend( void );






#endif /* ##ifndef __SYSPOWER_LOWLEVEL_H__ */
