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

#include "fonts.h"



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



/* LCD screen and bitmap image array consants */
#define X_BYTES				106
#define Y_BYTES	      		8
#define SCRN_LEFT			0
#define SCRN_TOP			0
#define SCRN_RIGHT			105
#define SCRN_BOTTOM			55

/* pixel level bit masks for display */
/* this array is setup to map the order */
/* of bits in a byte to the vertical order */
/* of bits at the LCD controller */
const unsigned char l_mask_array[8] =
         {0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};

/* the LCD display image memory */
/* buffer arranged so page memory is sequential in RAM */
unsigned char l_display_array[Y_BYTES][X_BYTES];


void lcd_glyph(unsigned char left, unsigned char top,
			   unsigned char width, unsigned char height,
			   unsigned char *glyph, unsigned char store_width)
{
	unsigned char bit_pos;
	unsigned char byte_offset;
	unsigned char y_bits;
	unsigned char remaining_bits;
	unsigned char mask;
	unsigned char char_mask;
	unsigned char x;
	unsigned char *glyph_scan;
	unsigned char glyph_offset;

  	bit_pos = top & 0x07;		/* get the bit offset into a byte */

	glyph_offset = 0;			/* start at left side of the glyph rasters */
    char_mask = 0x80;			/* initial character glyph mask */

  	for (x = left; x < (left + width); x++)
  	{
    	byte_offset = top >> 3;        	/* get the byte offset into y direction */
		y_bits = height;				/* get length in y direction to write */
		remaining_bits = 8 - bit_pos;	/* number of bits left in byte */
		mask = l_mask_array[bit_pos];	/* get mask for this bit */
		glyph_scan = glyph + glyph_offset;	 /* point to base of the glyph */

    	/* boundary checking here to account for the possibility of  */
    	/* write past the bottom of the screen.                        */
    	while((y_bits) && (byte_offset < Y_BYTES)) /* while there are bits still to write */
    	{
			/* check if the character pixel is set or not */
			if(*glyph_scan & char_mask)
			{
				l_display_array[byte_offset][x] |= mask;	/* set image pixel */
			}
			else
			{
      			l_display_array[byte_offset][x] &= ~mask;	/* clear the image pixel */
			}

			if(l_mask_array[0] & 0x80)
			{
				mask >>= 1;
			}
			else
			{
				mask <<= 1;
			}
			
			y_bits--;
			remaining_bits--;
      		if(remaining_bits == 0)
      		{
				/* just crossed over a byte boundry, reset byte counts */
				remaining_bits = 8;
				byte_offset++;
				mask = l_mask_array[0];
      		}

			/* bump the glyph scan to next raster */
			glyph_scan += store_width;
		}

		/* shift over to next glyph bit */
		char_mask >>= 1;
		if(char_mask == 0)				/* reset for next byte in raster */
		{
			char_mask = 0x80;
			glyph_offset++;
	    }
	}
}


int drawHtext( unsigned long startX, unsigned long startY, unsigned short fcolor, unsigned short bcolor, unsigned char font, char *str )
{
	int iRet = -1;

  	unsigned char x = startX;
  	unsigned char glyph;
  	unsigned char width;
	unsigned char height;
	unsigned char store_width;
	unsigned char *glyph_ptr;

  	while(*str != 0x00)
  	{
    	glyph = (unsigned char)*str;

		/* check to make sure the symbol is a legal one */
		/* if not then just replace it with a "." character */
		if((glyph < ASCII_BOT) || (glyph > ASCII_TOP))
		{
			glyph = '.';
		}

    	/* the fonts start at ASCII_BOT, so to get the index into the font array */
    	/* subtract ASCII_BOT from the glyph                  */
    	glyph -= ASCII_BOT;
    	width = fonts[font].fixed_width;	/* check if it is a fixed width */
		if(width == 0)
		{
			width=fonts[font].width_table[glyph];	/* get the variable width instead */
		}

		height = fonts[font].glyph_height;
		store_width = fonts[font].store_width;

		glyph_ptr = fonts[font].glyph_table + (glyph * store_width * height);

		lcd_glyph(x,startY,width,height,glyph_ptr,store_width);  /* plug symbol into buffer */

		x += width;							/* move right for next character */
		str++;								/* point to next character in string */
	}


	return iRet;
}
