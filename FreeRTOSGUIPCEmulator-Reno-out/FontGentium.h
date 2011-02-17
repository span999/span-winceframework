#ifndef FONTGENTIUM_H
#define  FONTGENTIUM_H

#if defined(WIN32)
extern const char Gentium_Bold25[];
#define Gentium_Italic33_SIZE     8840
extern const char Gentium_Italic33[];
#define Gentium_Italic25_SIZE     5192
extern const char Gentium_Italic25[];
#define Gentium_Normal33_SIZE     6893
extern const char Gentium_Normal33[];
#define Gentium_Normal25_SIZE     4667
extern const char Gentium_Normal25[];
#define Gentium_Normal11_SIZE     1459
extern const char Gentium_Normal11[];
#define Gentium_Normal15_SIZE     1922
extern const char Gentium_Normal15[];
#define Gentium_Bold33_SIZE     7157
extern const char Gentium_Bold33[];
#define Gentium_Bold25_SIZE     4767
extern const char Gentium_Bold25[];

#else

#define Gentium_Italic33_SIZE     8840
extern FONT_FLASH Gentium_Italic33;
#define Gentium_Italic25_SIZE     5192
extern FONT_FLASH Gentium_Italic25;
#define Gentium_Normal33_SIZE     6893
extern FONT_FLASH Gentium_Normal33;
#define Gentium_Normal25_SIZE     4667
extern FONT_FLASH Gentium_Normal25;
#define Gentium_Normal11_SIZE     1459
extern FONT_FLASH Gentium_Normal11;
#define Gentium_Normal15_SIZE     1922
extern FONT_FLASH Gentium_Normal15;
#define Gentium_Bold33_SIZE     7157
extern FONT_FLASH Gentium_Bold33;
#define Gentium_Bold25_SIZE     4767
extern FONT_FLASH Gentium_Bold25;

#endif

#endif