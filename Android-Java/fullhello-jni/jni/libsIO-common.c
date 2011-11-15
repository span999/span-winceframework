/*




*/


#include <stdio.h>
///#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#include "libsIO-i2cchip_list.h"





///#include <linux/i2c-dev.h>
#define I2C_SLAVE			0x0703	/* Use this slave address */
#define I2C_SLAVE_FORCE 	0x0706  /* Use this slave address, even if it
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
static int libsi2cdevsearch( const char *filename, int iMaxNum, int ifunc );
static int libsi2cdevdump( const char *filename, int iMaxNum );
static int libsi2cdevregget( int iBus, int iAddr, int iIdx );
static int libsi2cdevregset( int iBus, int iAddr, int iIdx, int iVal );




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
	return libsi2cdevsearch( DEVI2CFILE_PRIFIX, DEVI2CFILE_MAXNUM, 1 );
}


int libsi2clist( void )
{
	return libsi2cdevsearch( DEVI2CFILE_PRIFIX, DEVI2CFILE_MAXNUM, 0 );
}


int libsi2cdump( void )
{
	return libsi2cdevdump( DEVI2CFILE_PRIFIX, DEVI2CFILE_MAXNUM );
}


int libsi2cget( int iBus, int iAddr, int iIdx )
{
	if( iBus < 0 || iBus > 6 )
		return -1;
	if( iAddr < 0 || iAddr > 0x7f )
		return -2;
	if( iIdx < 0 || iIdx > 100 )
		return -3;

	return libsi2cdevregget( iBus, iAddr, iIdx );
}


int libsi2cset( int iBus, int iAddr, int iIdx, int iVal )
{
	if( iBus < 0 || iBus > 6 )
		return -1;
	if( iAddr < 0 || iAddr > 0x7f )
		return -2;
	if( iIdx < 0 || iIdx > 100 )
		return -3;
	
	libsi2cdevregget( iBus, iAddr, iIdx );
	libsi2cdevregset( iBus, iAddr, iIdx, iVal );
	return libsi2cdevregget( iBus, iAddr, iIdx );
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
///		usleep(1000);
        if (read(fh, &data, count) != count) {
                ///perror("read");
                return -1;
        }

        return (data[1] << 8) + data[0];
}


static int libsi2c_writereg( int fh, char reg, int count, int Val )
{
        char data[3];

        if( fh == 0 )
			return -1;

        if (count < 1 || count > 2)
			return -1;


        data[0] = reg;
        data[1] = Val&0x00ff;
        data[2] = (Val>>8)&0x00ff;
		usleep(1000);
        if (write(fh, &data, 3) != 3) {
                ///perror("write before read");
                return -1;
        }

        return 0;
}


static struct tI2CCHIPDATA* getChipInfobyID( int iID )
{
	struct tI2CCHIPDATA* pRet = 0;
	int iCnt = 0;
	
	while( 0 != chiplistP[iCnt] )
	{
		pRet = (struct tI2CCHIPDATA*)chiplistP[iCnt];
		if( pRet->chipID == iID )
			break;
		else
			pRet = (struct tI2CCHIPDATA*)chiplistP[0];	///set to common chip

		iCnt++;
	}
	
	return pRet;
}


static void libsi2c_showchipname ( char* devname, int addr )
{
	
#if 0
	printf("\r\n[I2C]....============================================================" );
	if( 0x66 == addr )
		printf("\r\n[I2C]....Good!! ID=0x%02x(0x%02x) on %s is Maxim MAX8698C (PMIC)!!!", addr, (addr*2), devname );
	else
	if( 0x1a == addr )
		printf("\r\n[I2C]....Good!! ID=0x%02x(0x%02x) on %s is Realtek ALC5621 (Audio Codec)!!!", addr, (addr*2), devname );
	else
	if( 0x1e == addr )
		printf("\r\n[I2C]....Good!! ID=0x%02x(0x%02x) on %s is Honeywell hmc5883l (Compass)!!!", addr, (addr*2), devname );
	else
	if( 0x3c == addr )
		printf("\r\n[I2C]....Good!! ID=0x%02x(0x%02x) on %s is ??? ??? (Back camera)!!!", addr, (addr*2), devname );
	else
	if( 0x62 == addr )
		printf("\r\n[I2C]....Good!! ID=0x%02x(0x%02x) on %s is ??? LM08SS (Front camera)!!!", addr, (addr*2), devname );
	else
	if( 0x0f == addr )
		printf("\r\n[I2C]....Good!! ID=0x%02x(0x%02x) on %s is kionix kxtf9 (3D-accelerometer)!!!", addr, (addr*2), devname );
	else
	if( 0x48 == addr )
		printf("\r\n[I2C]....Good!! ID=0x%02x(0x%02x) on %s is Ti TPS65950 (USB controller)!!!", addr, (addr*2), devname );
	else
	if( 0x49 == addr )
		printf("\r\n[I2C]....Good!! ID=0x%02x(0x%02x) on %s is Ti TPS65950 (AUDIO controller)!!!", addr, (addr*2), devname );
	else
	if( 0x4a == addr )
		printf("\r\n[I2C]....Good!! ID=0x%02x(0x%02x) on %s is Ti TPS65950 (PMIC controller)!!!", addr, (addr*2), devname );
	else
	if( 0x4b == addr )
		printf("\r\n[I2C]....Good!! ID=0x%02x(0x%02x) on %s is Ti TPS65950 (PMIC controller)!!!", addr, (addr*2), devname );
	else
		printf("\r\n[I2C]....Good!! ID=0x%02x(0x%02x) on %s is Unknow ??? ( ??? )!!!", addr, (addr*2), devname );
	
	printf("\r\n[I2C]....============================================================" );	
	printf("\r\n");
#else
	struct tI2CCHIPDATA* pCD = 0;

	printf("\r\n[I2C]....============================================================" );

	pCD = getChipInfobyID( addr );
	printf("\r\n[I2C]....Good!! ID=0x%02x(0x%02x) on %s is %s %s (%s) !!!", addr, (addr*2), devname, pCD->vendorName, pCD->chipName, pCD->description ); 
	
	printf("\r\n[I2C]....============================================================" );	
	printf("\r\n");
#endif
}


static int libsi2c_readreg_ex( char* devname, int file, int addr, char reg, int count )
{
	int readRet = 0;

	readRet = libsi2c_readreg( file, reg, count );
	if( readRet < 0 ) {
		/* ERROR HANDLING: i2c transaction failed */
		printf("\r\n[I2C]....Oops!! ID=0x%02x(0x%02x) on %s, register 0x%02x is not readable err=(0x%x - %s)!!!", addr, (addr*2), devname, reg, errno, strerror(errno) );
	} else {
		if( 1 == count )
			printf("\r\n[I2C]....Good!! ID=0x%02x(0x%02x) on %s is readable (0x%02x=0x%02x)!!!", addr, (addr*2), devname, reg, readRet );
		else
			printf("\r\n[I2C]....Good!! ID=0x%02x(0x%02x) on %s is readable (0x%02x=0x%04x)!!!", addr, (addr*2), devname, reg, readRet );
	} 
	return readRet;
}


static int libsi2cdevregget( int iBus, int iAddr, int iIdx )
{
	int iRet = -1;
	char devname[128];
	char devnum[8];
	int file = 0;

	///load dev name
	strcpy(devname, DEVI2CFILE_PRIFIX);
	sprintf(devnum,"%d",iBus);
	strcat(devname, devnum);

	file = open( devname, O_RDWR|O_NONBLOCK );
	if( file < 0 )
	{
		printf("[I2C]Oops!! %s bus is not available !!!\r\n", devname );
	}
	else
	{
		int ioctlRet = 0;

		/// set slave address
		ioctlRet = ioctl( file, I2C_SLAVE, iAddr );
		
		if( ioctlRet < 0 )
		{
			///printf("[I2C]..Oops!! ID=0x%02x(0x%02x) on %s is not controlable err=(%s) !!!\r\n", iAddr, (iAddr*2), devname, strerror(-ioctlRet) );
			///printf("[I2C]..Oops!! ID=0x%02x(0x%02x) on %s is not controlable errno=(%s) !!!\r\n", iAddr, (iAddr*2), devname, strerror(errno) );
			///printf("[I2C]..re-try with FORCE !!");
			usleep(1000);
			ioctlRet = ioctl( file, I2C_SLAVE_FORCE, iAddr );
		}				
		if( ioctlRet < 0 )	///check again
		{
			///printf("[I2C]....Oops!! ID=0x%02x(0x%02x) on %s is not controlable err=(%s) !!!\r\n", iAddr, (iAddr*2), devname, ioctlRet, strerror(-ioctlRet) );
			///printf("[I2C]....Oops!! ID=0x%02x(0x%02x) on %s is not controlable errno=(%s) !!!\r\n", iAddr, (iAddr*2), devname, strerror(errno) );
		}
		else if( libsi2c_readreg( file, 0x0, 2 ) < 0 )	///try to read index 0x0
		{
			if( errno == ENXIO )	///0x6
				printf("\r\n....Oops!! ID=0x%02x(0x%02x), No such device or address !!", iAddr, (iAddr*2) );
			else
				printf("\r\n....Oops!! ID=0x%02x(0x%02x), unknow error=0x%x !!", iAddr, (iAddr*2), errno );
		}
		else
		{
			iRet = libsi2c_readreg( file, iIdx, 2 );
			///iRet = libsi2c_readreg_ex( devname, file, iAddr, iIdx, 2 )
			if( iRet < 0 )
			{
				if( errno == ENXIO )	///0x6
					printf("\r\n....Oops!! get ID=0x%02x(0x%02x), No such device or address !!", iAddr, (iAddr*2) );
				else
					printf("\r\n....Oops!! get ID=0x%02x(0x%02x), unknow error=0x%x !!", iAddr, (iAddr*2), errno );
			}
			else
			{
				
				printf("\r\n[I2C] get ID=0x%02x(0x%02x) on %s, index(0x%02x)=0x%04x\r\n", iAddr, (iAddr*2), devname, iIdx, iRet );
				///libsi2c_showchipname( devname, addr );
			}

		}
		

	}

	return iRet;
}


static int libsi2cdevregset( int iBus, int iAddr, int iIdx, int iVal )
{
	int iRet = -1;
	char devname[128];
	char devnum[8];
	int file = 0;

	///load dev name
	strcpy(devname, DEVI2CFILE_PRIFIX);
	sprintf(devnum,"%d",iBus);
	strcat(devname, devnum);

	file = open( devname, O_RDWR|O_NONBLOCK );
	if( file < 0 )
	{
		printf("[I2C]Oops!! %s bus is not available !!!\r\n", devname );
	}
	else
	{
		int ioctlRet = 0;

		/// set slave address
		ioctlRet = ioctl( file, I2C_SLAVE, iAddr );
		
		if( ioctlRet < 0 )
		{
			///printf("[I2C]..Oops!! ID=0x%02x(0x%02x) on %s is not controlable err=(%s) !!!\r\n", iAddr, (iAddr*2), devname, strerror(-ioctlRet) );
			///printf("[I2C]..Oops!! ID=0x%02x(0x%02x) on %s is not controlable errno=(%s) !!!\r\n", iAddr, (iAddr*2), devname, strerror(errno) );
			///printf("[I2C]..re-try with FORCE !!");
			usleep(1000);
			ioctlRet = ioctl( file, I2C_SLAVE_FORCE, iAddr );
		}				
		if( ioctlRet < 0 )	///check again
		{
			///printf("[I2C]....Oops!! ID=0x%02x(0x%02x) on %s is not controlable err=(%s) !!!\r\n", iAddr, (iAddr*2), devname, ioctlRet, strerror(-ioctlRet) );
			///printf("[I2C]....Oops!! ID=0x%02x(0x%02x) on %s is not controlable errno=(%s) !!!\r\n", iAddr, (iAddr*2), devname, strerror(errno) );
		}
		else if( libsi2c_readreg( file, 0x0, 2 ) < 0 )	///try to read index 0x0
		{
			if( errno == ENXIO )	///0x6
				printf("\r\n....Oops!! ID=0x%02x(0x%02x), No such device or address !!", iAddr, (iAddr*2) );
			else
				printf("\r\n....Oops!! ID=0x%02x(0x%02x), unknow error=0x%x !!", iAddr, (iAddr*2), errno );
		}
		else
		{
			///iRet = libsi2c_readreg( file, iIdx, 2 );
			iRet = libsi2c_writereg( file, iIdx, 2, iVal );
			///iRet = libsi2c_readreg_ex( devname, file, iAddr, iIdx, 2 )
			if( iRet < 0 )
			{
				if( errno == ENXIO )	///0x6
					printf("\r\n....Oops!! set ID=0x%02x(0x%02x), No such device or address !!", iAddr, (iAddr*2) );
				else
					printf("\r\n....Oops!! set ID=0x%02x(0x%02x), unknow error=0x%x !!", iAddr, (iAddr*2), errno );
			}
			else
			{
				
				printf("\r\n[I2C] set ID=0x%02x(0x%02x) on %s, index(0x%02x)=0x%04x\r\n", iAddr, (iAddr*2), devname, iIdx, iRet );
				///libsi2c_showchipname( devname, addr );
			}
			usleep(1000);

		}
		

	}

	return iRet;
}


static int libsi2cdevsearch( const char *filename, int iMaxNum, int ifunc )
{
	int iRet = 0;
	int iLoop = 0;
///	FILE *fp = NULL;
	char devname[128];
	char devnum[8];
	int file = 0;

	if( 1 == ifunc )
		printf("[I2C]searching device with %s upto %d device nodes !!!\r\n", filename, iMaxNum );
	
	/// search each i2c bus id
	for( iLoop = 0; iLoop <= iMaxNum; iLoop++ )
	{
		///load dev name
		strcpy(devname, filename);
		sprintf(devnum,"%d",iLoop);
		strcat(devname, devnum);

		if( 1 == ifunc )
			printf("[I2C]searching %s !!!\r\n", devname );

		///try open dev node
		file = open( devname, O_RDWR|O_NONBLOCK );
		if( file < 0 )
		{
			if( 1 == ifunc )
				printf("[I2C]Oops!! %s bus is not available !!!\r\n", devname );
		}
		else
		{
			int iLp = 0;
			int ioctlRet = 0;

			if( 1 == ifunc )
			{
				printf("[I2C]Good!! bus %s is available for access !!!\r\n", devname );
				///printf("..scan bus %s with address 0-127 !!!\r\n", devname );
			}

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
					if( 1 == ifunc )
					{
						printf("[I2C]..Oops!! ID=0x%02x(0x%02x) on %s is not controlable err=(%s) !!!\r\n", addr, (addr*2), devname, strerror(-ioctlRet) );
						printf("[I2C]..Oops!! ID=0x%02x(0x%02x) on %s is not controlable errno=(%s) !!!\r\n", addr, (addr*2), devname, strerror(errno) );
						printf("[I2C]..re-try with FORCE !!");
					} 
					usleep(1000);
					ioctlRet = ioctl( file, I2C_SLAVE_FORCE, addr );
				}				


				if( ioctlRet < 0 )
				{
					if( 1 == ifunc )
					{
						printf("[I2C]....Oops!! ID=0x%02x(0x%02x) on %s is not controlable err=(%s) !!!\r\n", addr, (addr*2), devname, ioctlRet, strerror(-ioctlRet) );
						printf("[I2C]....Oops!! ID=0x%02x(0x%02x) on %s is not controlable errno=(%s) !!!\r\n", addr, (addr*2), devname, strerror(errno) );
					}
				}
				else if( libsi2c_readreg( file, 0x0, 2 ) < 0 )	///try to read index 0x0
				{
					if( errno == ENXIO )	///0x6
						;///printf("\r\n....Oops!! ID=0x%02x(0x%02x), No such device or address !!", addr, (addr*2) );
					else
						;///printf("\r\n....Oops!! ID=0x%02x(0x%02x), unknow error=0x%x !!", addr, (addr*2), errno );
				}
				else
				{

					printf("\r\n[I2C]..Good!! ID=0x%02x(0x%02x) on %s is available !!!\r\n", addr, (addr*2), devname );
					libsi2c_showchipname( devname, addr );
				
				}
				usleep(2000);
				
			}
			printf("\r\n");

			close(file);
		}
		


	}

	return iRet;
}


static int libsi2cdevdump( const char *filename, int iMaxNum )
{
	int iRet = 0;
	int iLoop = 0;
///	FILE *fp = NULL;
	char devname[128];
	char devnum[8];
	int file = 0;

	printf("[I2C]searching device with %s upto %d device nodes !!!\r\n", filename, iMaxNum );
	
	/// search each i2c bus id
	for( iLoop = 0; iLoop <= iMaxNum; iLoop++ )
	{
		///load dev name
		strcpy(devname, filename);
		sprintf(devnum,"%d",iLoop);
		strcat(devname, devnum);

		printf("[I2C]searching %s !!!\r\n", devname );

		///try open dev node
		file = open( devname, O_RDWR|O_NONBLOCK );
		if( file < 0 )
		{
			printf("[I2C]Oops!! %s bus is not available !!!\r\n", devname );
		}
		else
		{
			int iLp = 0;
			int ioctlRet = 0;

			printf("[I2C]Good!! bus %s is available for access !!!\r\n", devname );
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
					printf("[I2C]..Oops!! ID=0x%02x(0x%02x) on %s is not controlable err=(%s) !!!\r\n", addr, (addr*2), devname, strerror(-ioctlRet) );
					printf("[I2C]..Oops!! ID=0x%02x(0x%02x) on %s is not controlable errno=(%s) !!!\r\n", addr, (addr*2), devname, strerror(errno) );
					printf("[I2C]..re-try with FORCE !!"); 
					usleep(1000);
					ioctlRet = ioctl( file, I2C_SLAVE_FORCE, addr );
				}				


				if( ioctlRet < 0 )
				{
					printf("[I2C]....Oops!! ID=0x%02x(0x%02x) on %s is not controlable err=(%s) !!!\r\n", addr, (addr*2), devname, ioctlRet, strerror(-ioctlRet) );
					printf("[I2C]....Oops!! ID=0x%02x(0x%02x) on %s is not controlable errno=(%s) !!!\r\n", addr, (addr*2), devname, strerror(errno) );
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
						struct tI2CCHIPDATA *pChipInfo;

#if 0
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
							
						printf("\r\n[I2C]..addr=0x%x.pChipInfo=0x%08x.pReglist=0x%08x", addr, pChipInfo, pReglist );
#else
						pChipInfo = getChipInfobyID( addr );
						pReglist = (struct tI2CCHIPREG *)pChipInfo->ppchipReg;
						
						///printf("\r\n[I2C]..addr=0x%x.pChipInfo=0x%08x.pReglist=0x%08x", addr, pChipInfo, pReglist );
#endif						

						libsi2c_showchipname( devname, addr );
						for( iT=0; iT<0xff; iT++ )
						{							
							if( ((pReglist+iT)->regNum != 0xff) && ((pReglist+iT)->regWidth != 0) )
							{
								printf("\r\n[I2C]....register <%s>:", (pReglist+iT)->regName );
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

