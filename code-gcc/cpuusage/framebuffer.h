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

struct cpuBarCfg
{
	unsigned long X;
	unsigned long Y;
	unsigned long L;
	unsigned long H;
	unsigned long M;
	unsigned short C;
};

struct cpuBarCfgS
{
	struct cpuBarCfg _af;
	struct cpuBarCfg _ab;
	struct cpuBarCfg _0f;
	struct cpuBarCfg _0b;
	struct cpuBarCfg _1f;
	struct cpuBarCfg _1b;
	struct cpuBarCfg _2f;
	struct cpuBarCfg _2b;
	struct cpuBarCfg _3f;
	struct cpuBarCfg _3b;
};




#endif /* __FRAMEBUFFER_H__ */
