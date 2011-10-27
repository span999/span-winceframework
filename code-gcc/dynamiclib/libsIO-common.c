/*




*/


#include <stdio.h>




#define X86CPUINFOFILE	"/proc/cpuinfo"
#define X86MEMINFOFILE	"/proc/meminfo"



#if 1
#define CPUINFOFILE	X86CPUINFOFILE
#define MEMINFOFILE	X86MEMINFOFILE
#else

#endif


enum tSYSINFOIDX{
	CPUINFO = 1,
	MEMINFO = 2
};


int libsdumpsysteminfo( enum tSYSINFOIDX index );


int libsdumpcupinfo( void )
{
	return libsdumpsysteminfo( CPUINFO );
}


int libsdumpmeminfo( void )
{
	return libsdumpsysteminfo( MEMINFO );
}


int libsdumpsysteminfo( enum tSYSINFOIDX index )
{
	int iRet = 0;
	FILE *fp = NULL;

	printf("dump system info with index = %d. \r\n", index );


	if( index == CPUINFO )
		fp = fopen(CPUINFOFILE,"r");
	else
	if( index == MEMINFO )
		fp = fopen(MEMINFOFILE,"r");
	else
	{
		printf("Error!! error with system info index (%d)!!!\r\n", index );
		return iRet;
	}
	
	if( fp == NULL )
	{
		printf("Error!! Cannot open file for systen info !!!\r\n");
	}
	else
	{
		char lyne[121];

		printf("open file ok !!!\r\n");
		
		while (fgets(lyne, sizeof(lyne) -1, fp)) {
        		printf("[sysinfo] %s", lyne);
		}

		fclose(fp);
	}

	return iRet;
}




