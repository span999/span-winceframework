
/*
	header of UI common functions
*/

#ifndef _RENOUICOMMON_H_
#define	_RENOUICOMMON_H_


#include "RenoGUI.h"
#include "RenoUIframe.h"

extern SCREEN_STATUS ScrStat;
extern SCREEN_STATUS *pScrStat;


void spClearScreen( void );
void spBlankScreen( void );
void spBlankRect( int x, int y, int xsize, int ysize );
void spBlankRectEx( GUI_RECT* prtTemp );

void spGoAfterFramePage( FRAMEPAGE_HEADER *pGoAfterFP );
int spFrameTimeoutHandling( int iTO );
int spFramePageWait( void );
int spIsMsgKeyStatus( WM_MESSAGE* pMsg, int iTar );




#define		IsMSGUP_press(x)	spIsMsgKeyStatus( x, 'i' )
#define		IsMSGUP_hold(x)		spIsMsgKeyStatus( x, 'o' )
#define		IsMSGDOWN_press(x)	spIsMsgKeyStatus( x, 'k' )
#define		IsMSGDOWN_hold(x)	spIsMsgKeyStatus( x, 'l' )
#define		IsMSGBACK_press(x)	spIsMsgKeyStatus( x, 'u' )
#define		IsMSGBACK_hold(x)	spIsMsgKeyStatus( x, 'y' )
#define		IsMSGENTER_press(x)	spIsMsgKeyStatus( x, 'j' )
#define		IsMSGENTER_hold(x)	spIsMsgKeyStatus( x, 'h' )




#endif	/* _RENOUICOMMON_H_ */