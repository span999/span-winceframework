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



#endif	/*#ifndef __STRINGID_H__*/
