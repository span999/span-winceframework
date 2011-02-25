#ifndef __FONTANNABELSCRIPT_H__
#define  __FONTANNABELSCRIPT_H__


#if defined(WIN32)

#define AnnabelScript_Normal13_SIZE     1770
extern const char AnnabelScript_Normal13[];
#define AnnabelScript_Normal16_SIZE     2200
extern const char AnnabelScript_Normal16[];
#define AnnabelScript_Normal18_SIZE     2534
extern const char AnnabelScript_Normal18[];
#define AnnabelScript_Normal20_SIZE     3052
extern const char AnnabelScript_Normal20[];
#define AnnabelScript_Normal23_SIZE     3543
extern const char AnnabelScript_Normal23[];
#define AnnabelScript_Normal26_SIZE     4344
extern const char AnnabelScript_Normal26[];

#else

#define AnnabelScript_Normal13_SIZE     1770
extern FONT_FLASH AnnabelScript_Normal13;
#define AnnabelScript_Normal16_SIZE     2200
extern FONT_FLASH AnnabelScript_Normal16;
#define AnnabelScript_Normal18_SIZE     2534
extern FONT_FLASH AnnabelScript_Normal18;
#define AnnabelScript_Normal20_SIZE     3052
extern FONT_FLASH AnnabelScript_Normal20;
#define AnnabelScript_Normal23_SIZE     3543
extern FONT_FLASH AnnabelScript_Normal23;
#define AnnabelScript_Normal26_SIZE     4344
extern FONT_FLASH AnnabelScript_Normal26;

#endif

#endif	/* __FONTANNABELSCRIPT_H__ */