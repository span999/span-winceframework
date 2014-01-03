
/*
	header of tool help functions
*/

#ifndef _RENOUITOOLHELP_H_
#define	_RENOUITOOLHELP_H_

int spGetRandNum( int iBtm, int iTop );
int IsSamekey( int iSrc, int iTar );
void SPPRINTF( char* pString, ... );



#define		IsUP_press(x)		IsSamekey( x, 'i' )
#define		IsUP_hold(x)		IsSamekey( x, 'o' )
#define		IsDOWN_press(x)		IsSamekey( x, 'k' )
#define		IsDOWN_hold(x)		IsSamekey( x, 'l' )
#define		IsBACK_press(x)		IsSamekey( x, 'u' )
#define		IsBACK_hold(x)		IsSamekey( x, 'y' )
#define		IsENTER_press(x)	IsSamekey( x, 'j' )
#define		IsENTER_hold(x)		IsSamekey( x, 'h' )


///#define		PRINTF				void
#define		PRINTF				printf


#endif	/* _RENOUITOOLHELP_H_ */