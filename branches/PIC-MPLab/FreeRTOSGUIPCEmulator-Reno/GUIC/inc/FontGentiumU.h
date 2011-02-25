#ifndef __FONTGENTIUMU_H__
#define  __FONTGENTIUMU_H__

#if defined(WIN32)

#define Gentium_Normal25U_SIZE     4580
extern const char Gentium_Normal25U[];
#define Gentium_Normal21U_SIZE     3278
extern const char Gentium_Normal21U[];

#else

#define Gentium_Normal25U_SIZE     4580
extern FONT_FLASH Gentium_Normal25U;
#define Gentium_Normal21U_SIZE     3278
extern FONT_FLASH Gentium_Normal21U;

#endif

#endif