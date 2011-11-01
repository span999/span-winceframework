/*




*/


#include <stdio.h>
///#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#include "libsIO-i2cchip.h"
#include "libsIO-i2cchip_COMMON.h"
#include "libsIO-i2cchip_HMC5883L.h"
#include "libsIO-i2cchip_KXTF9.h"
#include "libsIO-i2cchip_ALC5621.h"
#include "libsIO-i2cchip_MAX8698C.h"





///#include <linux/i2c-dev.h>
#define I2C_SLAVE	0x0703	/* Use this slave address */
#define I2C_SLAVE_FORCE 0x0706  /* Use this slave address, even if it
                                   is already in use by a driver! */

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


static void libsi2c_showchipname ( char* devname, int addr )
{
	printf("\r\n....============================================================" );
	if( 0x66 == addr )
		printf("\r\n....Good!! ID=0x%02x(0x%02x) on %s is Maxim MAX8698C (PMIC)!!!", addr, (addr*2), devname );
	else
	if( 0x1a == addr )
		printf("\r\n....Good!! ID=0x%02x(0x%02x) on %s is Realtek ALC5621 (Audio Codec)!!!", addr, (addr*2), devname );
	else
	if( 0x1e == addr )
		printf("\r\n....Good!! ID=0x%02x(0x%02x) on %s is Honeywell hmc5883l (Compass)!!!", addr, (addr*2), devname );
	else
	if( 0x3c == addr )
		printf("\r\n....Good!! ID=0x%02x(0x%02x) on %s is ??? ??? (Back camera)!!!", addr, (addr*2), devname );
	else
	if( 0x62 == addr )
		printf("\r\n....Good!! ID=0x%02x(0x%02x) on %s is ??? LM08SS (Front camera)!!!", addr, (addr*2), devname );
	else
	if( 0x0f == addr )
		printf("\r\n....Good!! ID=0x%02x(0x%02x) on %s is kionix kxtf9 (3D-accelerometer)!!!", addr, (addr*2), devname );
	else
		printf("\r\n....Good!! ID=0x%02x(0x%02x) on %s is Unknow ??? ( ??? )!!!", addr, (addr*2), devname );

}


static int libsi2c_readreg_ex( char* devname, int file, int addr, char reg, int count )
{
	int readRet = 0;

	readRet = libsi2c_readreg( file, reg, count );
	if( readRet < 0 ) {
		/* ERROR HANDLING: i2c transaction failed */
		printf("\r\n....Oops!! ID=0x%02x(0x%02x) on %s, register 0x%02x is not readable err=(0x%x - %s)!!!", addr, (addr*2), devname, reg, errno, strerror(errno) );
	} else {
		printf("\r\n....Good!! ID=0x%02x(0x%02x) on %s is readable (0x%02x=0x%04x)!!!", addr, (addr*2), devname, reg, readRet );
	} 
	return readRet;
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
	
	/// search each i2c bus id
	for( iLoop = 0; iLoop <= iMaxNum; iLoop++ )
	{
		///load dev name
		strcpy(devname, filename);
		sprintf(devnum,"%d",iLoop);
		strcat(devname, devnum);

		printf("searching %s !!!\r\n", devname );

		///try open dev node
		file = open( devname, O_RDWR|O_NONBLOCK );
		if( file < 0 )
		{
			printf("Oops!! %s bus is not available !!!\r\n", devname );
		}
		else
		{
			int iLp = 0;
			int ioctlRet = 0;

			printf("Good!! bus %s is available for access !!!\r\n", devname );
			///printf("..scan bus %s with address 0-127 !!!\r\n", devname );

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
					printf("..Oops!! ID=0x%02x(0x%02x) on %s is not controlable err=(%s) !!!\r\n", addr, (addr*2), devname, strerror(-ioctlRet) );
					printf("..Oops!! ID=0x%02x(0x%02x) on %s is not controlable errno=(%s) !!!\r\n", addr, (addr*2), devname, strerror(errno) );
					printf("..re-try with FORCE !!"); 
					usleep(1000);
					ioctlRet = ioctl( file, I2C_SLAVE_FORCE, addr );
				}				


				if( ioctlRet < 0 )
				{
					printf("..Oops!! ID=0x%02x(0x%02x) on %s is not controlable err=(%s) !!!\r\n", addr, (addr*2), devname, ioctlRet, strerror(-ioctlRet) );
					printf("..Oops!! ID=0x%02x(0x%02x) on %s is not controlable errno=(%s) !!!\r\n", addr, (addr*2), devname, strerror(errno) );
				}
				else if( (libsi2c_readreg( file, 0x0, 2 ) < 0) && (errno == 0x6) )
				{
					///printf("\r\n....Oops!! ID=0x%02x(0x%02x), No such device or address !!", addr, (addr*2) );
				}
				else
				{
///					char buf[10];
///					int readRet;
///					char reg;

					///printf("..Good!! ID=0x%02x(0x%02x) on %s is available !!!\r\n", addr, (addr*2), devname );


					if( (0x00 <= addr) && (0xff >= addr) )
					{
						int iT = 0;
						struct tI2CCHIPREG *pReglist;

						if(0x66 == addr)
							pReglist = (struct tI2CCHIPREG *)&chipreglist_MAX8698C[0];
						else
						if(0x1a == addr)
							pReglist = (struct tI2CCHIPREG *)&chipreglist_ALC5621[0];
						else
						if(0x1e == addr)
							pReglist = (struct tI2CCHIPREG *)&chipreglist_HMC5883L[0];
						else
						if(0x0f == addr)
							pReglist = (struct tI2CCHIPREG *)&chipreglist_KXTF9[0];
						else
							pReglist = (struct tI2CCHIPREG *)&chipreglist_COMMON[0];
							

						libsi2c_showchipname( devname, addr );
						for( iT=0; iT<0xff; iT++ )
						{							
							if( ((pReglist+iT)->regNum != 0xff) && ((pReglist+iT)->regWidth != 0) )
							{
								printf("\r\n....register <%s>:", (pReglist+iT)->regName );
								libsi2c_readreg_ex( devname, file, addr, (pReglist+iT)->regNum, (pReglist+iT)->regWidth );
							}
							else
							{
								break;
							}
						}
					}
					else
					{
						libsi2c_showchipname( devname, addr );
					}
					printf("\r\n");
				
				
				}
				usleep(2000);
				
			}
			printf("\r\n");

			close(file);
		}
		


	}

	return iRet;
}



