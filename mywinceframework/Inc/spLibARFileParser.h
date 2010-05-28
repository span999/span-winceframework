#ifndef __SPLIBARFILEPARSER_H__
#define __SPLIBARFILEPARSER_H__


/*
*/


typedef enum _ARLIBCMDP_RSP
{
	AR_OK = 0,
	AR_EXIT,
	AR_NOCMDFILE,
	AR_CMDFILEERR,
	AR_COPYFILEERR,
	AR_MOVEFILEERR,
	AR_DELETEFILEERR,

} ARLIBCMDPAR_RSP;


ARLIBCMDPAR_RSP spLibCmdFileParser( DWORD dwParam );
BOOL spLibLoadRegCmdSetting( void );



#endif	///#ifndef __SPLIBARFILEPARSER_H__