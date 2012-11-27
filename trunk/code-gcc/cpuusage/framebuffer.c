/*


*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>



static unsigned long g_xres = 0;
static unsigned long g_yres = 0;
static unsigned long g_bpp = 0;
static unsigned long g_linelen = 0;


int getFBinfo( void )
{
	int iRet = 0;

	int fp = 0;
	struct fb_var_screeninfo vinfo;
	struct fb_fix_screeninfo finfo;

#ifndef __ARM_CODE__
	goto _ERREXIT;
#endif

	fp = open( "/dev/fb0", O_RDWR );

	if( fp < 0 )
	{
		printf("Error : Can not open framebuffer device\n");
		iRet = -1;
		goto _ERREXIT;
	}

	if( ioctl(fp,FBIOGET_FSCREENINFO,&finfo) )
	{
		printf("Error reading fixed information\n");
		iRet = -2;
		close (fp);
		goto _ERREXIT;
	}

	if( ioctl(fp,FBIOGET_VSCREENINFO,&vinfo) )
	{
		printf("Error reading variable information\n");
		iRet = -3;
		close (fp);
		goto _ERREXIT;
	}

	printf("The mem is :%d\n", finfo.smem_len );
	printf("The line_length is :%d\n", finfo.line_length );
	printf("The xres is :%d\n", vinfo.xres );
	printf("The yres is :%d\n", vinfo.yres );
	printf("bits_per_pixel is :%d\n", vinfo.bits_per_pixel );
	close (fp);
	
	g_xres = vinfo.xres;
	g_yres = vinfo.yres;
	g_bpp = vinfo.bits_per_pixel;
	g_linelen = finfo.line_length;
	
	goto _OKEXIT;

_ERREXIT:

_OKEXIT:

	return iRet;
}




int drawHbar( unsigned long startX, unsigned long startY, unsigned long lenth, unsigned long height, unsigned short color )
{
	int iRet = 0;
	long screensize=0;
	int fp = 0;
	char *fbp = 0;
	long x = 0, y = 0;
	long l = 0, h = 0;
	long location = 0;

#ifndef __ARM_CODE__
	goto _ERREXIT;
#endif

	if( (0==g_xres) || (0==g_yres) || (0==g_bpp) || (0==g_linelen) )
	{
		iRet = getFBinfo();
		if( 0 != iRet )
			goto _ERREXIT;
	}
	
	fp = open( "/dev/fb0", O_RDWR );

	if( fp < 0 )
	{
		printf("Error : Can not open framebuffer device\n");
		iRet = -1;
		goto _ERREXIT;
	}

	screensize = g_xres * g_yres * g_bpp / 8;

	fbp = (char *)mmap( 0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fp, 0 );
	if( (int)fbp == -1 )
	{
		printf ("Error: failed to map framebuffer device to memory.\n");
		iRet = -2;
		close( fp );
		goto _ERREXIT;
	}

	/* location = x * (g_bpp / 8) + y * g_linelen; */
	x = startX;
	y = startY;
	l = 0;
	h = 0;
	
	while( h++ < height )
	{
		while( l++ < lenth )
		{
			location = ((x+l)*(g_bpp/8)) + ((y+h)*g_linelen);
			*(unsigned short *)(fbp+location) = color;
		}	/* while */
		l = 0;
	}	/* while */
	
	

_OKEXIT:
	munmap(fbp, screensize ); 
	close( fp );
_ERREXIT:

	return iRet;
}



