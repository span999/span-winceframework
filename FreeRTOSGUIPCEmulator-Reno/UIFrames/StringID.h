#ifndef __STRINGID_H__
#define  __STRINGID_H__



typedef enum
{
    EN  = 0,
    SP,
    FR,
    IT,
    PO,
    DU,
    GE,
    DA,
} LANGUAGE_TYPE;


extern LANGUAGE_TYPE	gLanguage;


extern XCHAR *IdGetMString( WORD wStingID, LANGUAGE_TYPE lang );
extern XCHAR *IdGetString( WORD wStingID );





extern const XCHAR Func01Str[];
extern const XCHAR Func02Str[];  
extern const XCHAR Func03Str[];
extern const XCHAR Func04Str[]; 
extern const XCHAR Func05Str[];
extern const XCHAR Func06Str[]; 
							
extern const XCHAR Unit01Str[]; 
extern const XCHAR Unit02Str[];
extern const XCHAR Unit03Str[]; 
extern const XCHAR Unit04Str[];

extern const XCHAR Data01Str[];  
extern const XCHAR Data02Str[];  
extern const XCHAR Data03Str[]; 
extern const XCHAR Data04Str[];
extern const XCHAR Data05Str[]; 
extern const XCHAR Data06Str[];



#endif	/*#ifndef __STRINGID_H__*/
