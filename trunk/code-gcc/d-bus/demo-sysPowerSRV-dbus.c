#include <stdio.h>
#include <stdlib.h>

///include power service
#include "sysPowerSRV-dbus.h"





int main(int argc, char** argv)
{
	int iCnt = 0;
	int Return = 0;
	
	/// init power service
	sysPowerSRVInit();
	
	while( iCnt++ < 1000 )
	{
		///do power service call whatever
	#if 0	
		Return = sPSgetCPUActivatedNum();
		Return = sPSsetCPUActivatedNum(1);
		Return = sPSgetCPUActivatedNum();
		Return = sPSsetCPUActivatedNum(2);
		Return = sPSgetCPUActivatedNum();
		Return = sPSsetCPUActivatedNum(3);
		Return = sPSgetCPUActivatedNum();
		Return = sPSsetCPUActivatedNum(4);
		///Return = sPSsetCPUDVFS(1);
	#else
		Return = sPSloopbackTest(iCnt);
		if( Return == iCnt )
			printf("sysPowerSRV-dbus loopback test pass! [%d/%d]\n", iCnt, Return);
		else
			printf("sysPowerSRV-dbus loopback test failed! [%d/%d\n]", iCnt, Return);
	#endif
	}
	
	/// de-init power service
	sysPowerSRVDeinit();
	return 0;	
	
}
