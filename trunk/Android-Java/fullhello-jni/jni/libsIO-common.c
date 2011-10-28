/*




*/


#include <stdio.h>
///#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

///#include <linux/i2c-dev.h>
#define I2C_SLAVE	0x0703	/* Use this slave address */

#define X86CPUINFOFILE		"/proc/cpuinfo"
#define X86MEMINFOFILE		"/proc/meminfo"

#define ARMCPUINFOFILE		"/proc/cpuinfo"
#define ARMMEMINFOFILE		"/proc/meminfo"


/* prefix for i2c device */
#define DEVI2CFILE_PRIFIX	"/dev/i2c-"
/* maxima support i2c device number */
#define DEVI2CFILE_MAXNUM	5


#if 1
#define CPUINFOFILE	X86CPUINFOFILE
#define MEMINFOFILE	X86MEMINFOFILE
#else
#define CPUINFOFILE	ARMCPUINFOFILE
#define MEMINFOFILE	ARMMEMINFOFILE
#endif



enum tSYSINFOIDX{
	CPUINFO = 1,
	MEMINFO = 2
};


static int libsdumpsysteminfo( enum tSYSINFOIDX index );
static int libsi2cdevsearch( const char *filename, int iMaxNum );








int libsdumpcupinfo( void )
{
	return libsdumpsysteminfo( CPUINFO );
}


int libsdumpmeminfo( void )
{
	return libsdumpsysteminfo( MEMINFO );
}


static int libsdumpsysteminfo( enum tSYSINFOIDX index )
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


int libsi2csearch( void )
{
	return libsi2cdevsearch( DEVI2CFILE_PRIFIX, DEVI2CFILE_MAXNUM );
}


static int libsi2cdevsearch( const char *filename, int iMaxNum )
{
	int iRet = 0;
	int iLoop = 0;
	FILE *fp = NULL;
	char devname[128];
	char devnum[8];
	int file = 0;

	printf("searching device with %s upto %d device nodes !!!\r\n", filename, iMaxNum );
	
	for( iLoop = 0; iLoop <= iMaxNum; iLoop++ )
	{
		///load dev name
		strcpy(devname, filename);
		sprintf(devnum,"%d",iLoop);
		strcat(devname, devnum);

		printf("searching %s !!!\r\n", devname );

#if 0
		fp = fopen(devname,"r");
		if( fp == NULL )
		{
			printf("Oops!! %s is not available !!!\r\n", devname );
		}
		else
		{
			printf("Good!! %s is available for access !!!\r\n", devname );
		}

		if( fp != NULL )	///close only opend
			fclose(fp);
#endif

		file = open( devname, O_RDWR );
		if( file < 0 )
		{
			printf("Oops!! %s is not available !!!\r\n", devname );
		}
		else
		{
			int iLp = 0;
			///char index = 0;

			printf("Good!! %s is available for access !!!\r\n", devname );

			for( iLp = 0; iLp < 256; iLp++ )
			{
				int addr;
				
				addr = iLp;
				///printf("Try %s with chip ID=0x%x !!!\r\n", devname, addr );
				/// set slave address
				if( ioctl( file, I2C_SLAVE, addr ) < 0 )
				{
					///printf("Oops!! ID=0x%02x on %s is not available !!!\r\n", addr, devname );
				}
				else
				{
					char buf[10];
					int readret;

					///printf("Good!! ID=0x%02x on %s is available !!!\r\n", addr, devname );
					
					buf[0] = 0x0;	///register 0x0
					readret = read(file, buf, 1);
					
					if( readret != 1)
						readret = read(file, buf, 1);

					if( readret != 1)
						readret = read(file, buf, 1);

					if( readret != 1)
						readret = read(file, buf, 1);

					if( readret != 1) {
						/* ERROR HANDLING: i2c transaction failed */
						///printf("Oops!! ID=0x%02x on %s is not readable 0x00(0x%x)!!!\r\n", addr, devname, buf[0] );
					} else {
				    	/* buf[0] contains the read byte */
						printf("Good!! ID=0x%02x on %s is readable 0x00(0x%x)!!!\r\n", addr, devname, buf[0] );

						buf[0] = 0x1;	///register 0x1
						if(read(file, buf, 1) == 1) {
							printf("Good!! ID=0x%02x on %s is readable 0x01(0x%x)!!!\r\n", addr, devname, buf[0] );
						}
					}
					
				}
			}

			close(file);
		}
		


	}

	return iRet;
}



