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

static int libsi2c_readreg( int fh, char reg, int count )
{
        char data[2];

        if( fh == 0 )
			return -1;

        if (count < 1 || count > 2)
			return -1;


        data[0] = reg;
		usleep(1000);
        if (write(fh, &data, 1) != 1) {
                ///perror("write before read");
                return -1;
        }

        data[1] = 0;
		usleep(1000);
        if (read(fh, &data, count) != count) {
                ///perror("read");
                return -1;
        }

        return (data[1] << 8) + data[0];
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

		///try open dev node
		file = open( devname, O_RDWR );
		if( file < 0 )
		{
			printf("Oops!! %s bus is not available !!!\r\n", devname );
		}
		else
		{
			int iLp = 0;
			int ioctlRet = 0;

			printf("Good!! %s is available for access !!!\r\n", devname );

			for( iLp = 0; iLp < 128; iLp++ )
			{
				int addr;
				
				addr = iLp;
				///printf(".");
				///printf("Try %s with chip ID=0x%x !!!\r", devname, addr );
				///printf("Try %s with chip ID=0x%x !!!\r\n", devname, addr );
				/// set slave address
				ioctlRet = ioctl( file, I2C_SLAVE, addr );
				if( ioctlRet < 0 )
				{
					printf(".");
					printf("Oops!! ID=0x%02x(0x%02x) on %s is not controlable err=(%d) !!!\r\n", addr, (addr*2), devname, ioctlRet );
				}
				else
				{
					char buf[10];
					int readRet;
					char reg;

					///printf("Good!! ID=0x%02x on %s is available !!!\r\n", addr, devname );
#if 0
					for( reg = 0; reg < 256; reg++ )
					{
						usleep(200);
						readRet = libsi2c_readreg( file, reg, 2 );
						usleep(200);
						if( readRet < 0 ) {
							/* ERROR HANDLING: i2c transaction failed */
							///printf("Oops!! ID=0x%02x(0x%02x) on %s is not readable (0x%02x=0x%04x)!!!\r\n", addr, (addr*2), devname, reg, readRet );
						} else {
							if( reg == 0 )
								printf("\r\n");
							printf("Good!! ID=0x%02x(0x%02x) on %s is readable (0x%02x=0x%04x)!!!\r\n", addr, (addr*2), devname, reg, readRet );
						} 
						usleep(200);
					}
#else
					///printf("\r\n");
					reg = 0x0;
					readRet = libsi2c_readreg( file, reg, 2 );
					if( readRet < 0 ) {
						/* ERROR HANDLING: i2c transaction failed */
						///printf("Oops!! ID=0x%02x(0x%02x) on %s is not readable (0x%02x=0x%04x)!!!\r\n", addr, (addr*2), devname, reg, readRet );
					} else {
						printf("\r\n");
						printf("Good!! ID=0x%02x(0x%02x) on %s is readable (0x%02x=0x%04x)!!!\r\n", addr, (addr*2), devname, reg, readRet );
					} 
					
					reg = 0x2;
					readRet = libsi2c_readreg( file, reg, 2 );
					if( readRet < 0 ) {
						/* ERROR HANDLING: i2c transaction failed */
						///printf("Oops!! ID=0x%02x(0x%02x) on %s is not readable (0x%02x=0x%04x)!!!\r\n", addr, (addr*2), devname, reg, readRet );
					} else {
						printf("Good!! ID=0x%02x(0x%02x) on %s is readable (0x%02x=0x%04x)!!!\r\n", addr, (addr*2), devname, reg, readRet );
					} 

					reg = 0x4;
					readRet = libsi2c_readreg( file, reg, 2 );
					if( readRet < 0 ) {
						/* ERROR HANDLING: i2c transaction failed */
						///printf("Oops!! ID=0x%02x(0x%02x) on %s is not readable (0x%02x=0x%04x)!!!\r\n", addr, (addr*2), devname, reg, readRet );
					} else {
						printf("Good!! ID=0x%02x(0x%02x) on %s is readable (0x%02x=0x%04x)!!!\r\n", addr, (addr*2), devname, reg, readRet );
					} 

					reg = 0x6;
					readRet = libsi2c_readreg( file, reg, 2 );
					if( readRet < 0 ) {
						/* ERROR HANDLING: i2c transaction failed */
						///printf("Oops!! ID=0x%02x(0x%02x) on %s is not readable (0x%02x=0x%04x)!!!\r\n", addr, (addr*2), devname, reg, readRet );
					} else {
						printf("Good!! ID=0x%02x(0x%02x) on %s is readable (0x%02x=0x%04x)!!!\r\n", addr, (addr*2), devname, reg, readRet );
					}

					reg = 0x8;
					readRet = libsi2c_readreg( file, reg, 2 );
					if( readRet < 0 ) {
						/* ERROR HANDLING: i2c transaction failed */
						///printf("Oops!! ID=0x%02x(0x%02x) on %s is not readable (0x%02x=0x%04x)!!!\r\n", addr, (addr*2), devname, reg, readRet );
					} else {
						printf("Good!! ID=0x%02x(0x%02x) on %s is readable (0x%02x=0x%04x)!!!\r\n", addr, (addr*2), devname, reg, readRet );
					}

					reg = 0xa;
					readRet = libsi2c_readreg( file, reg, 2 );
					if( readRet < 0 ) {
						/* ERROR HANDLING: i2c transaction failed */
						///printf("Oops!! ID=0x%02x(0x%02x) on %s is not readable (0x%02x=0x%04x)!!!\r\n", addr, (addr*2), devname, reg, readRet );
					} else {
						printf("Good!! ID=0x%02x(0x%02x) on %s is readable (0x%02x=0x%04x)!!!\r\n", addr, (addr*2), devname, reg, readRet );
					}

					reg = 0xc;
					readRet = libsi2c_readreg( file, reg, 2 );
					if( readRet < 0 ) {
						/* ERROR HANDLING: i2c transaction failed */
						///printf("Oops!! ID=0x%02x(0x%02x) on %s is not readable (0x%02x=0x%04x)!!!\r\n", addr, (addr*2), devname, reg, readRet );
					} else {
						printf("Good!! ID=0x%02x(0x%02x) on %s is readable (0x%02x=0x%04x)!!!\r\n", addr, (addr*2), devname, reg, readRet );
					}

					reg = 0xe;
					readRet = libsi2c_readreg( file, reg, 2 );
					if( readRet < 0 ) {
						/* ERROR HANDLING: i2c transaction failed */
						///printf("Oops!! ID=0x%02x(0x%02x) on %s is not readable (0x%02x=0x%04x)!!!\r\n", addr, (addr*2), devname, reg, readRet );
					} else {
						printf("Good!! ID=0x%02x(0x%02x) on %s is readable (0x%02x=0x%04x)!!!\r\n", addr, (addr*2), devname, reg, readRet );
					}

					reg = 0x10;
					readRet = libsi2c_readreg( file, reg, 2 );
					if( readRet < 0 ) {
						/* ERROR HANDLING: i2c transaction failed */
						///printf("Oops!! ID=0x%02x(0x%02x) on %s is not readable (0x%02x=0x%04x)!!!\r\n", addr, (addr*2), devname, reg, readRet );
					} else {
						printf("Good!! ID=0x%02x(0x%02x) on %s is readable (0x%02x=0x%04x)!!!\r\n", addr, (addr*2), devname, reg, readRet );
					}

					reg = 0x12;
					readRet = libsi2c_readreg( file, reg, 2 );
					if( readRet < 0 ) {
						/* ERROR HANDLING: i2c transaction failed */
						///printf("Oops!! ID=0x%02x(0x%02x) on %s is not readable (0x%02x=0x%04x)!!!\r\n", addr, (addr*2), devname, reg, readRet );
					} else {
						printf("Good!! ID=0x%02x(0x%02x) on %s is readable (0x%02x=0x%04x)!!!\r\n", addr, (addr*2), devname, reg, readRet );
					}


					reg = 0x14;
					readRet = libsi2c_readreg( file, reg, 2 );
					if( readRet < 0 ) {
						/* ERROR HANDLING: i2c transaction failed */
						///printf("Oops!! ID=0x%02x(0x%02x) on %s is not readable (0x%02x=0x%04x)!!!\r\n", addr, (addr*2), devname, reg, readRet );
					} else {
						printf("Good!! ID=0x%02x(0x%02x) on %s is readable (0x%02x=0x%04x)!!!\r\n", addr, (addr*2), devname, reg, readRet );
					}


					reg = 0x16;
					readRet = libsi2c_readreg( file, reg, 2 );
					if( readRet < 0 ) {
						/* ERROR HANDLING: i2c transaction failed */
						///printf("Oops!! ID=0x%02x(0x%02x) on %s is not readable (0x%02x=0x%04x)!!!\r\n", addr, (addr*2), devname, reg, readRet );
					} else {
						printf("Good!! ID=0x%02x(0x%02x) on %s is readable (0x%02x=0x%04x)!!!\r\n", addr, (addr*2), devname, reg, readRet );
					}


					reg = 0x1c;
					readRet = libsi2c_readreg( file, reg, 2 );
					if( readRet < 0 ) {
						/* ERROR HANDLING: i2c transaction failed */
						///printf("Oops!! ID=0x%02x(0x%02x) on %s is not readable (0x%02x=0x%04x)!!!\r\n", addr, (addr*2), devname, reg, readRet );
					} else {
						printf("Good!! ID=0x%02x(0x%02x) on %s is readable (0x%02x=0x%04x)!!!\r\n", addr, (addr*2), devname, reg, readRet );
					}


					reg = 0x22;
					readRet = libsi2c_readreg( file, reg, 2 );
					if( readRet < 0 ) {
						/* ERROR HANDLING: i2c transaction failed */
						///printf("Oops!! ID=0x%02x(0x%02x) on %s is not readable (0x%02x=0x%04x)!!!\r\n", addr, (addr*2), devname, reg, readRet );
					} else {
						printf("Good!! ID=0x%02x(0x%02x) on %s is readable (0x%02x=0x%04x)!!!\r\n", addr, (addr*2), devname, reg, readRet );
					}


					reg = 0x34;
					readRet = libsi2c_readreg( file, reg, 2 );
					if( readRet < 0 ) {
						/* ERROR HANDLING: i2c transaction failed */
						///printf("Oops!! ID=0x%02x(0x%02x) on %s is not readable (0x%02x=0x%04x)!!!\r\n", addr, (addr*2), devname, reg, readRet );
					} else {
						printf("Good!! ID=0x%02x(0x%02x) on %s is readable (0x%02x=0x%04x)!!!\r\n", addr, (addr*2), devname, reg, readRet );
					}


					reg = 0x36;
					readRet = libsi2c_readreg( file, reg, 2 );
					if( readRet < 0 ) {
						/* ERROR HANDLING: i2c transaction failed */
						///printf("Oops!! ID=0x%02x(0x%02x) on %s is not readable (0x%02x=0x%04x)!!!\r\n", addr, (addr*2), devname, reg, readRet );
					} else {
						printf("Good!! ID=0x%02x(0x%02x) on %s is readable (0x%02x=0x%04x)!!!\r\n", addr, (addr*2), devname, reg, readRet );
					}


					reg = 0x38;
					readRet = libsi2c_readreg( file, reg, 2 );
					if( readRet < 0 ) {
						/* ERROR HANDLING: i2c transaction failed */
						///printf("Oops!! ID=0x%02x(0x%02x) on %s is not readable (0x%02x=0x%04x)!!!\r\n", addr, (addr*2), devname, reg, readRet );
					} else {
						printf("Good!! ID=0x%02x(0x%02x) on %s is readable (0x%02x=0x%04x)!!!\r\n", addr, (addr*2), devname, reg, readRet );
					}


					reg = 0x3a;
					readRet = libsi2c_readreg( file, reg, 2 );
					if( readRet < 0 ) {
						/* ERROR HANDLING: i2c transaction failed */
						///printf("Oops!! ID=0x%02x(0x%02x) on %s is not readable (0x%02x=0x%04x)!!!\r\n", addr, (addr*2), devname, reg, readRet );
					} else {
						printf("Good!! ID=0x%02x(0x%02x) on %s is readable (0x%02x=0x%04x)!!!\r\n", addr, (addr*2), devname, reg, readRet );
					}


					reg = 0x3c;
					readRet = libsi2c_readreg( file, reg, 2 );
					if( readRet < 0 ) {
						/* ERROR HANDLING: i2c transaction failed */
						///printf("Oops!! ID=0x%02x(0x%02x) on %s is not readable (0x%02x=0x%04x)!!!\r\n", addr, (addr*2), devname, reg, readRet );
					} else {
						printf("Good!! ID=0x%02x(0x%02x) on %s is readable (0x%02x=0x%04x)!!!\r\n", addr, (addr*2), devname, reg, readRet );
					}


					reg = 0x3e;
					readRet = libsi2c_readreg( file, reg, 2 );
					if( readRet < 0 ) {
						/* ERROR HANDLING: i2c transaction failed */
						///printf("Oops!! ID=0x%02x(0x%02x) on %s is not readable (0x%02x=0x%04x)!!!\r\n", addr, (addr*2), devname, reg, readRet );
					} else {
						printf("Good!! ID=0x%02x(0x%02x) on %s is readable (0x%02x=0x%04x)!!!\r\n", addr, (addr*2), devname, reg, readRet );
					}

					reg = 0x5a;
					readRet = libsi2c_readreg( file, reg, 2 );
					if( readRet < 0 ) {
						/* ERROR HANDLING: i2c transaction failed */
						///printf("Oops!! ID=0x%02x(0x%02x) on %s is not readable (0x%02x=0x%04x)!!!\r\n", addr, (addr*2), devname, reg, readRet );
					} else {
						printf("Good!! ID=0x%02x(0x%02x) on %s is readable (0x%02x=0x%04x)!!!\r\n", addr, (addr*2), devname, reg, readRet );
					}

					reg = 0x7c;
					readRet = libsi2c_readreg( file, reg, 2 );
					if( readRet < 0 ) {
						/* ERROR HANDLING: i2c transaction failed */
						///printf("Oops!! ID=0x%02x(0x%02x) on %s is not readable (0x%02x=0x%04x)!!!\r\n", addr, (addr*2), devname, reg, readRet );
					} else {
						printf("Good!! ID=0x%02x(0x%02x) on %s is readable (0x%02x=0x%04x)!!!\r\n", addr, (addr*2), devname, reg, readRet );
					}

					reg = 0x7e;
					readRet = libsi2c_readreg( file, reg, 2 );
					if( readRet < 0 ) {
						/* ERROR HANDLING: i2c transaction failed */
						///printf("Oops!! ID=0x%02x(0x%02x) on %s is not readable (0x%02x=0x%04x)!!!\r\n", addr, (addr*2), devname, reg, readRet );
					} else {
						printf("Good!! ID=0x%02x(0x%02x) on %s is readable (0x%02x=0x%04x)!!!\r\n", addr, (addr*2), devname, reg, readRet );
					}
#endif					
				}
				usleep(2000);
				
			}
			printf("\r\n");

			close(file);
		}
		


	}

	return iRet;
}



