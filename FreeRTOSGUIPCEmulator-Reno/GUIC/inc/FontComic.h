
#ifndef __FONTCOMIC_H__
#define  __FONTCOMIC_H__


#if defined(WIN32)

#define comic_Normal15_SIZE     1952
extern const char comic_Normal15[];
#define comic_Normal19_SIZE     2976
extern const char comic_Normal19[];
#define comic_Normal23_SIZE     3865
extern const char comic_Normal23[];
#define comic_Normal26_SIZE     4916
extern const char comic_Normal26[];
#define comic_Normal24_SIZE     4472
extern const char comic_Normal24[];

#else

#define comic_Normal15_SIZE     1952
extern FONT_FLASH comic_Normal15;
#define comic_Normal19_SIZE     2976
extern FONT_FLASH comic_Normal19;
#define comic_Normal23_SIZE     3865
extern FONT_FLASH comic_Normal23;
#define comic_Normal26_SIZE     4916
extern FONT_FLASH comic_Normal26;
#define comic_Normal24_SIZE     4472
extern FONT_FLASH comic_Normal24;

#endif

#endif	/* __FONTCOMIC_H__ */