#ifndef __FONTKAIU_H__
#define  __FONTKAIU_H__

#if defined(WIN32)

#define kaiu_Norma28_SIZE     5932
extern const char kaiu_Norma28[];
#define kaiu_Norma25_SIZE     5338
extern const char kaiu_Norma25[];
#define kaiu_Normal18_SIZE     3880
extern const char kaiu_Normal18[];

#else

#define kaiu_Norma28_SIZE     5932
extern FONT_FLASH kaiu_Norma28;
#define kaiu_Norma25_SIZE     5338
extern FONT_FLASH kaiu_Norma25;
#define kaiu_Normal18_SIZE     3880
extern FONT_FLASH kaiu_Normal18;

#endif


#endif	/* __FONTKAIU_H__ */