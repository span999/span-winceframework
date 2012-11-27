/*


*/

#ifndef __FRAMEBUFFER_H__
#define __FRAMEBUFFER_H__


#define		_RED_COLOR		0xf800
#define		_PINK_COLOR		(0xf800|0x00e0|0x0007)
#define		_GREEN_COLOR	0x07e0
#define		_BLUE_COLOR		0x001f
#define		_WHITE_COLOR	0xffff
#define		_BLACK_COLOR	0x0000


int getFBinfo( void );
int drawHbar( unsigned long startX, unsigned long startY, unsigned long lenth, unsigned long height, unsigned short color );






#endif /* __FRAMEBUFFER_H__ */
