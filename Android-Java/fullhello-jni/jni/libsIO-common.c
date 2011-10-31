/*




*/


#include <stdio.h>
///#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>



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


struct tCHIPREG {
	int 	regNum;		///register number.
	int 	regWidth;	///register wodth in byte. (8bit=1,16bit=2)
	char* 	regName;	///register name
};


static const struct tCHIPREG chipreglist_HMC5883L[] = {
	{	0x00,	1,	"Configuration Register A" },
	{	0x01,	1,	"Configuration Register B" },
	{	0x02,	1,	"Mode Register" },
	{	0x03,	1,	"Data Output X MSB Register" },
	{	0x04,	1,	"Data Output X LSB Register" },
	{	0x05,	1,	"Data Output Y MSB Register" },
	{	0x06,	1,	"Data Output Y LSB Register" },
	{	0x07,	1,	"Data Output Z MSB Register" },
	{	0x08,	1,	"Data Output Z LSB Register" },
	{	0x09,	1,	"Status Register" },
	{	0x0a,	1,	"Identification Register A" },
	{	0x0b,	1,	"Identification Register B" },
	{	0x0c,	1,	"Identification Register C" },
	{	0xff,	0,	"end of list" },
};


static const struct tCHIPREG chipreglist_KXTF9[] = {
	{	0x00,	1,	"XOUT_HPF_L" },
	{	0x01,	1,	"XOUT_HPF_H" },
	{	0x02,	1,	"YOUT_HPF_L" },
	{	0x03,	1,	"YOUT_HPF_H" },
	{	0x04,	1,	"ZOUT_HPF_L" },
	{	0x05,	1,	"ZOUT_HPF_H" },
	{	0x06,	1,	"XOUT_L" },
	{	0x07,	1,	"XOUT_H" },
	{	0x08,	1,	"YOUT_L" },
	{	0x09,	1,	"YOUT_H" },
	{	0x0a,	1,	"ZOUT_L" },
	{	0x0b,	1,	"ZOUT_H" },
	{	0x0c,	1,	"DCST_RESP" },
	{	0x0d,	1,	"Not Used" },
	{	0x0e,	1,	"Not Used" },
	{	0x0f,	1,	"WHO_AM_I" },
	{	0x10,	1,	"TILT_POS_CUR" },
	{	0x11,	1,	"TILT_POS_PRE" },
	{	0x12,	1,	"Kionix Reserved" },
	{	0x13,	1,	"Kionix Reserved" },
	{	0x14,	1,	"Kionix Reserved" },
	{	0x15,	1,	"INT_SRC_REG1" },
	{	0x16,	1,	"INT_SRC_REG2" },
	{	0x17,	1,	"Not Used" },
	{	0x18,	1,	"STATUS_REG" },
	{	0x19,	1,	"Not Used" },
	{	0x1a,	1,	"INT_REL" },
	{	0x1b,	1,	"CTRL_REG1" },
	{	0x1c,	1,	"CTRL_REG2" },
	{	0x1d,	1,	"CTRL_REG3" },
	{	0x1e,	1,	"INT_CTRL_REG1" },
	{	0x1f,	1,	"INT_CTRL_REG2" },
	{	0x20,	1,	"INT_CTRL_REG3" },
	{	0x21,	1,	"DATA_CTRL_REG" },
	{	0x28,	1,	"TILT_TIMER" },
	{	0x29,	1,	"WUF_TIMER" },
	{	0x2a,	1,	"Not Used" },
	{	0x2b,	1,	"TDT_TIMER" },
	{	0x2c,	1,	"TDT_H_THRESH" },
	{	0x2d,	1,	"TDT_L_THRESH" },
	{	0x2e,	1,	"TDT_TAP_TIMER" },
	{	0x2f,	1,	"TDT_TOTAL_TIMER" },
	{	0x30,	1,	"TDT_LATENCY_TIMER" },
	{	0x31,	1,	"TDT_WINDOW_TIMER" },
	{	0x3a,	1,	"SELF_TEST" },
	{	0x5a,	1,	"WUF_THRESH" },
	{	0x5c,	1,	"TILT_ANGLE" },
	{	0x5f,	1,	"HYST_SET" },
	{	0xff,	0,	"end of list" },
};


static const struct tCHIPREG chipreglist_MAX8698C[] = {
	{	0x00,	1,	"ONOFF1" },
	{	0x01,	1,	"ONOFF2" },
	{	0x02,	1,	"ADISCHG_EN1" },
	{	0x03,	1,	"ADISCHG_EN2" },
	{	0x04,	1,	"Buck1(VCC_ARM)" },
	{	0x05,	1,	"Buck1(VCC_ARM)" },
	{	0x06,	1,	"Buck2(VCC_INTERNAL)" },
	{	0x07,	1,	"Buck3(VCC_MEM)" },
	{	0x08,	1,	"LDO2/LDO3" },
	{	0x09,	1,	"LDO4" },
	{	0x0a,	1,	"LDO5" },
	{	0x0b,	1,	"LDO6" },
	{	0x0c,	1,	"LDO7" },
	{	0x0d,	1,	"LDO8" },
	{	0x0e,	1,	"LDO9" },
	{	0x0f,	1,	"LBCNFG" },
	{	0xff,	0,	"end of list" },
};


static const struct tCHIPREG chipreglist_ALC5621[] = {
	{	0x00,	2,	"Reset" },
	{	0x02,	2,	"Speaker Output Volume" },
	{	0x04,	2,	"Headphone Output Volume" },
	{	0x06,	2,	"MONO_OUT/AUXOUT Volume" },
	{	0x08,	2,	"AUXIN Volume" },
	{	0x0a,	2,	"LINE_IN Volume" },
	{	0x0c,	2,	"STEREO DAC Volume" },
	{	0x0e,	2,	"MIC Volume" },
	{	0x10,	2,	"MIC Routing Control" },
	{	0x12,	2,	"ADC Record Gain" },
	{	0x14,	2,	"ADC Record Mixer Control" },
	{	0x16,	2,	"Avol Soft Volume Control Time" },
	{	0x1c,	2,	"Output Mixer Control" },
	{	0x22,	2,	"Microphone Control" },
	{	0x34,	2,	"Digital Audio Interface Control" },
	{	0x36,	2,	"Stereo AD/DA Clock Control" },
	{	0x38,	2,	"Companding Control" },
	{	0x3a,	2,	"Power Management Addition 1" },
	{	0x3c,	2,	"Power Management Addition 2" },
	{	0x3e,	2,	"Power Management Addition 3" },
	{	0x40,	2,	"Additional Control Register" },
	{	0x42,	2,	"Global Clock Control Register" },
	{	0x44,	2,	"PLL Control Register" },
	{	0x4c,	2,	"GPIO Pin Configuration" },
	{	0x4e,	2,	"GPIO Pin Polarity" },
	{	0x50,	2,	"GPIO Pin Sticky" },
	{	0x52,	2,	"GPIO Pin Wake-Up" },
	{	0x54,	2,	"GPIO Pin Status" },
	{	0x56,	2,	"Pin Sharing" },
	{	0x58,	2,	"Over-Current Status" },
	{	0x5a,	2,	"Jack Detect Control Register" },
	{	0x5e,	2,	"MISC Control" },
	{	0x60,	2,	"Stereo and Spatial Effect Block Control" },
	{	0x62,	2,	"EQ Control" },
	{	0x66,	2,	"EQ Mode Change Enable" },
	{	0x68,	2,	"AVC Control" },
	{	0x6a,	2,	"Index Address" },
	{	0x6c,	2,	"Index Data" },
	{	0x7c,	2,	"VENDOR ID 1" },
	{	0x7e,	2,	"VENDOR ID 2" },
	{	0xff,	0,	"end of list" },
};


struct tCHIPDATA {
	char* chipName;
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
					char buf[10];
					int readRet;
					char reg;

					///printf("..Good!! ID=0x%02x(0x%02x) on %s is available !!!\r\n", addr, (addr*2), devname );
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
	#if 1
///					printf("\r\n");

					if( (0x1a == addr) || (0x66 == addr) || (0x1e == addr) || (0x0f == addr) )
					{
						int iT = 0;
						struct tCHIPREG *pReglist;

						if(0x66 == addr)
							pReglist = (struct tCHIPREG *)&chipreglist_MAX8698C[0];
						else
						if(0x1a == addr)
							pReglist = (struct tCHIPREG *)&chipreglist_ALC5621[0];
						else
						if(0x1e == addr)
							pReglist = (struct tCHIPREG *)&chipreglist_HMC5883L[0];
						else
						if(0x0f == addr)
							pReglist = (struct tCHIPREG *)&chipreglist_KXTF9[0];

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
						libsi2c_readreg_ex( devname, file, addr, 0x00, 2 );
						libsi2c_readreg_ex( devname, file, addr, 0x02, 2 );
						libsi2c_readreg_ex( devname, file, addr, 0x04, 2 );
						libsi2c_readreg_ex( devname, file, addr, 0x06, 2 );
						libsi2c_readreg_ex( devname, file, addr, 0x08, 2 );
						libsi2c_readreg_ex( devname, file, addr, 0x0a, 2 );
						libsi2c_readreg_ex( devname, file, addr, 0x0c, 2 );
						libsi2c_readreg_ex( devname, file, addr, 0x0e, 2 );
						libsi2c_readreg_ex( devname, file, addr, 0x10, 2 );
						libsi2c_readreg_ex( devname, file, addr, 0x12, 2 );
						libsi2c_readreg_ex( devname, file, addr, 0x14, 2 );
						libsi2c_readreg_ex( devname, file, addr, 0x16, 2 );
						libsi2c_readreg_ex( devname, file, addr, 0x18, 2 );
						libsi2c_readreg_ex( devname, file, addr, 0x1a, 2 );
						libsi2c_readreg_ex( devname, file, addr, 0x1c, 2 );
						libsi2c_readreg_ex( devname, file, addr, 0x1e, 2 );
						libsi2c_readreg_ex( devname, file, addr, 0x20, 2 );
						libsi2c_readreg_ex( devname, file, addr, 0x22, 2 );
						libsi2c_readreg_ex( devname, file, addr, 0x32, 2 );
						libsi2c_readreg_ex( devname, file, addr, 0x34, 2 );
						libsi2c_readreg_ex( devname, file, addr, 0x36, 2 );
						libsi2c_readreg_ex( devname, file, addr, 0x38, 2 );
						libsi2c_readreg_ex( devname, file, addr, 0x3a, 2 );
						libsi2c_readreg_ex( devname, file, addr, 0x3c, 2 );
						libsi2c_readreg_ex( devname, file, addr, 0x3e, 2 );
						libsi2c_readreg_ex( devname, file, addr, 0x5a, 2 );
						libsi2c_readreg_ex( devname, file, addr, 0x7c, 2 );
						libsi2c_readreg_ex( devname, file, addr, 0x7e, 2 );
					}
					printf("\r\n");
	#else
					///printf("\r\n");
					reg = 0x0;
					readRet = libsi2c_readreg( file, reg, 2 );
					if( readRet < 0 ) {
						/* ERROR HANDLING: i2c transaction failed */
						///printf("....Oops!! ID=0x%02x(0x%02x) on %s, register 0x%02x is not readable err=(0x%x - %s)!!!\r\n", addr, (addr*2), devname, reg, errno, strerror(errno) );
					} else {
						printf("\r\n");
						printf("....Good!! ID=0x%02x(0x%02x) on %s is readable (0x%02x=0x%04x)!!!\r\n", addr, (addr*2), devname, reg, readRet );
					} 
					
					reg = 0x2;
					readRet = libsi2c_readreg( file, reg, 2 );
					if( readRet < 0 ) {
						/* ERROR HANDLING: i2c transaction failed */
						///printf("Oops!! ID=0x%02x(0x%02x) on %s is not readable (0x%02x=0x%04x)!!!\r\n", addr, (addr*2), devname, reg, readRet );
					} else {
						printf("....Good!! ID=0x%02x(0x%02x) on %s is readable (0x%02x=0x%04x)!!!\r\n", addr, (addr*2), devname, reg, readRet );
					} 

					reg = 0x4;
					readRet = libsi2c_readreg( file, reg, 2 );
					if( readRet < 0 ) {
						/* ERROR HANDLING: i2c transaction failed */
						///printf("Oops!! ID=0x%02x(0x%02x) on %s is not readable (0x%02x=0x%04x)!!!\r\n", addr, (addr*2), devname, reg, readRet );
					} else {
						printf("....Good!! ID=0x%02x(0x%02x) on %s is readable (0x%02x=0x%04x)!!!\r\n", addr, (addr*2), devname, reg, readRet );
					} 

					reg = 0x6;
					readRet = libsi2c_readreg( file, reg, 2 );
					if( readRet < 0 ) {
						/* ERROR HANDLING: i2c transaction failed */
						///printf("Oops!! ID=0x%02x(0x%02x) on %s is not readable (0x%02x=0x%04x)!!!\r\n", addr, (addr*2), devname, reg, readRet );
					} else {
						printf("....Good!! ID=0x%02x(0x%02x) on %s is readable (0x%02x=0x%04x)!!!\r\n", addr, (addr*2), devname, reg, readRet );
					}

					reg = 0x8;
					readRet = libsi2c_readreg( file, reg, 2 );
					if( readRet < 0 ) {
						/* ERROR HANDLING: i2c transaction failed */
						///printf("Oops!! ID=0x%02x(0x%02x) on %s is not readable (0x%02x=0x%04x)!!!\r\n", addr, (addr*2), devname, reg, readRet );
					} else {
						printf("....Good!! ID=0x%02x(0x%02x) on %s is readable (0x%02x=0x%04x)!!!\r\n", addr, (addr*2), devname, reg, readRet );
					}

					reg = 0xa;
					readRet = libsi2c_readreg( file, reg, 2 );
					if( readRet < 0 ) {
						/* ERROR HANDLING: i2c transaction failed */
						///printf("Oops!! ID=0x%02x(0x%02x) on %s is not readable (0x%02x=0x%04x)!!!\r\n", addr, (addr*2), devname, reg, readRet );
					} else {
						printf("....Good!! ID=0x%02x(0x%02x) on %s is readable (0x%02x=0x%04x)!!!\r\n", addr, (addr*2), devname, reg, readRet );
					}

					reg = 0xc;
					readRet = libsi2c_readreg( file, reg, 2 );
					if( readRet < 0 ) {
						/* ERROR HANDLING: i2c transaction failed */
						///printf("Oops!! ID=0x%02x(0x%02x) on %s is not readable (0x%02x=0x%04x)!!!\r\n", addr, (addr*2), devname, reg, readRet );
					} else {
						printf("....Good!! ID=0x%02x(0x%02x) on %s is readable (0x%02x=0x%04x)!!!\r\n", addr, (addr*2), devname, reg, readRet );
					}

					reg = 0xe;
					readRet = libsi2c_readreg( file, reg, 2 );
					if( readRet < 0 ) {
						/* ERROR HANDLING: i2c transaction failed */
						///printf("Oops!! ID=0x%02x(0x%02x) on %s is not readable (0x%02x=0x%04x)!!!\r\n", addr, (addr*2), devname, reg, readRet );
					} else {
						printf("....Good!! ID=0x%02x(0x%02x) on %s is readable (0x%02x=0x%04x)!!!\r\n", addr, (addr*2), devname, reg, readRet );
					}

					reg = 0x10;
					readRet = libsi2c_readreg( file, reg, 2 );
					if( readRet < 0 ) {
						/* ERROR HANDLING: i2c transaction failed */
						///printf("Oops!! ID=0x%02x(0x%02x) on %s is not readable (0x%02x=0x%04x)!!!\r\n", addr, (addr*2), devname, reg, readRet );
					} else {
						printf("....Good!! ID=0x%02x(0x%02x) on %s is readable (0x%02x=0x%04x)!!!\r\n", addr, (addr*2), devname, reg, readRet );
					}

					reg = 0x12;
					readRet = libsi2c_readreg( file, reg, 2 );
					if( readRet < 0 ) {
						/* ERROR HANDLING: i2c transaction failed */
						///printf("Oops!! ID=0x%02x(0x%02x) on %s is not readable (0x%02x=0x%04x)!!!\r\n", addr, (addr*2), devname, reg, readRet );
					} else {
						printf("....Good!! ID=0x%02x(0x%02x) on %s is readable (0x%02x=0x%04x)!!!\r\n", addr, (addr*2), devname, reg, readRet );
					}


					reg = 0x14;
					readRet = libsi2c_readreg( file, reg, 2 );
					if( readRet < 0 ) {
						/* ERROR HANDLING: i2c transaction failed */
						///printf("Oops!! ID=0x%02x(0x%02x) on %s is not readable (0x%02x=0x%04x)!!!\r\n", addr, (addr*2), devname, reg, readRet );
					} else {
						printf("....Good!! ID=0x%02x(0x%02x) on %s is readable (0x%02x=0x%04x)!!!\r\n", addr, (addr*2), devname, reg, readRet );
					}


					reg = 0x16;
					readRet = libsi2c_readreg( file, reg, 2 );
					if( readRet < 0 ) {
						/* ERROR HANDLING: i2c transaction failed */
						///printf("Oops!! ID=0x%02x(0x%02x) on %s is not readable (0x%02x=0x%04x)!!!\r\n", addr, (addr*2), devname, reg, readRet );
					} else {
						printf("....Good!! ID=0x%02x(0x%02x) on %s is readable (0x%02x=0x%04x)!!!\r\n", addr, (addr*2), devname, reg, readRet );
					}


					reg = 0x1c;
					readRet = libsi2c_readreg( file, reg, 2 );
					if( readRet < 0 ) {
						/* ERROR HANDLING: i2c transaction failed */
						///printf("Oops!! ID=0x%02x(0x%02x) on %s is not readable (0x%02x=0x%04x)!!!\r\n", addr, (addr*2), devname, reg, readRet );
					} else {
						printf("....Good!! ID=0x%02x(0x%02x) on %s is readable (0x%02x=0x%04x)!!!\r\n", addr, (addr*2), devname, reg, readRet );
					}


					reg = 0x22;
					readRet = libsi2c_readreg( file, reg, 2 );
					if( readRet < 0 ) {
						/* ERROR HANDLING: i2c transaction failed */
						///printf("Oops!! ID=0x%02x(0x%02x) on %s is not readable (0x%02x=0x%04x)!!!\r\n", addr, (addr*2), devname, reg, readRet );
					} else {
						printf("....Good!! ID=0x%02x(0x%02x) on %s is readable (0x%02x=0x%04x)!!!\r\n", addr, (addr*2), devname, reg, readRet );
					}


					reg = 0x34;
					readRet = libsi2c_readreg( file, reg, 2 );
					if( readRet < 0 ) {
						/* ERROR HANDLING: i2c transaction failed */
						///printf("Oops!! ID=0x%02x(0x%02x) on %s is not readable (0x%02x=0x%04x)!!!\r\n", addr, (addr*2), devname, reg, readRet );
					} else {
						printf("....Good!! ID=0x%02x(0x%02x) on %s is readable (0x%02x=0x%04x)!!!\r\n", addr, (addr*2), devname, reg, readRet );
					}


					reg = 0x36;
					readRet = libsi2c_readreg( file, reg, 2 );
					if( readRet < 0 ) {
						/* ERROR HANDLING: i2c transaction failed */
						///printf("Oops!! ID=0x%02x(0x%02x) on %s is not readable (0x%02x=0x%04x)!!!\r\n", addr, (addr*2), devname, reg, readRet );
					} else {
						printf("....Good!! ID=0x%02x(0x%02x) on %s is readable (0x%02x=0x%04x)!!!\r\n", addr, (addr*2), devname, reg, readRet );
					}


					reg = 0x38;
					readRet = libsi2c_readreg( file, reg, 2 );
					if( readRet < 0 ) {
						/* ERROR HANDLING: i2c transaction failed */
						///printf("Oops!! ID=0x%02x(0x%02x) on %s is not readable (0x%02x=0x%04x)!!!\r\n", addr, (addr*2), devname, reg, readRet );
					} else {
						printf("....Good!! ID=0x%02x(0x%02x) on %s is readable (0x%02x=0x%04x)!!!\r\n", addr, (addr*2), devname, reg, readRet );
					}


					reg = 0x3a;
					readRet = libsi2c_readreg( file, reg, 2 );
					if( readRet < 0 ) {
						/* ERROR HANDLING: i2c transaction failed */
						///printf("Oops!! ID=0x%02x(0x%02x) on %s is not readable (0x%02x=0x%04x)!!!\r\n", addr, (addr*2), devname, reg, readRet );
					} else {
						printf("....Good!! ID=0x%02x(0x%02x) on %s is readable (0x%02x=0x%04x)!!!\r\n", addr, (addr*2), devname, reg, readRet );
					}


					reg = 0x3c;
					readRet = libsi2c_readreg( file, reg, 2 );
					if( readRet < 0 ) {
						/* ERROR HANDLING: i2c transaction failed */
						///printf("Oops!! ID=0x%02x(0x%02x) on %s is not readable (0x%02x=0x%04x)!!!\r\n", addr, (addr*2), devname, reg, readRet );
					} else {
						printf("....Good!! ID=0x%02x(0x%02x) on %s is readable (0x%02x=0x%04x)!!!\r\n", addr, (addr*2), devname, reg, readRet );
					}


					reg = 0x3e;
					readRet = libsi2c_readreg( file, reg, 2 );
					if( readRet < 0 ) {
						/* ERROR HANDLING: i2c transaction failed */
						///printf("Oops!! ID=0x%02x(0x%02x) on %s is not readable (0x%02x=0x%04x)!!!\r\n", addr, (addr*2), devname, reg, readRet );
					} else {
						printf("....Good!! ID=0x%02x(0x%02x) on %s is readable (0x%02x=0x%04x)!!!\r\n", addr, (addr*2), devname, reg, readRet );
					}

					reg = 0x5a;
					readRet = libsi2c_readreg( file, reg, 2 );
					if( readRet < 0 ) {
						/* ERROR HANDLING: i2c transaction failed */
						///printf("Oops!! ID=0x%02x(0x%02x) on %s is not readable (0x%02x=0x%04x)!!!\r\n", addr, (addr*2), devname, reg, readRet );
					} else {
						printf("....Good!! ID=0x%02x(0x%02x) on %s is readable (0x%02x=0x%04x)!!!\r\n", addr, (addr*2), devname, reg, readRet );
					}

					reg = 0x7c;
					readRet = libsi2c_readreg( file, reg, 2 );
					if( readRet < 0 ) {
						/* ERROR HANDLING: i2c transaction failed */
						///printf("Oops!! ID=0x%02x(0x%02x) on %s is not readable (0x%02x=0x%04x)!!!\r\n", addr, (addr*2), devname, reg, readRet );
					} else {
						printf("....Good!! ID=0x%02x(0x%02x) on %s is readable (0x%02x=0x%04x)!!!\r\n", addr, (addr*2), devname, reg, readRet );
					}

					reg = 0x7e;
					readRet = libsi2c_readreg( file, reg, 2 );
					if( readRet < 0 ) {
						/* ERROR HANDLING: i2c transaction failed */
						///printf("Oops!! ID=0x%02x(0x%02x) on %s is not readable (0x%02x=0x%04x)!!!\r\n", addr, (addr*2), devname, reg, readRet );
					} else {
						printf("....Good!! ID=0x%02x(0x%02x) on %s is readable (0x%02x=0x%04x)!!!\r\n", addr, (addr*2), devname, reg, readRet );
					}
	#endif
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



