#ifndef __SYSPOWER_LOWLEVEL_H__
#define __SYSPOWER_LOWLEVEL_H__




void setCPUcoreDOWN( int CoreNum );
void setCPUcoreUP( int CoreNum );
int setCPUcoreActivatedNumber( int num );
int setCPUdvfs( int iOn );
int setCPUspeed( int nSpeed );
int setCPUsuspend( void );






#endif /* ##ifndef __SYSPOWER_LOWLEVEL_H__ */
