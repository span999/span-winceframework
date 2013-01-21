#ifndef __SYSPOWER_LOWLEVEL_H__
#define __SYSPOWER_LOWLEVEL_H__




int setCPUcoreDOWN( int CoreNum );
int setCPUcoreUP( int CoreNum );
int setCPUcoreActivatedNumber( int num );
int getCPUcoreActivatedNumber( int ignored );
int setCPUdvfs( int iOn );
int setCPUspeed( int nSpeed );
int setCPUsuspend( int ignored );

int getLoopback( int input );




#endif /* ##ifndef __SYSPOWER_LOWLEVEL_H__ */
