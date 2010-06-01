//
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
//
// Use of this sample source code is subject to the terms of the Microsoft
// license agreement under which you licensed this sample source code. If
// you did not accept the terms of the license agreement, you are not
// authorized to use this sample source code. For the terms of the license,
// please see the license agreement between you and Microsoft or, if applicable,
// see the LICENSE.RTF on your install media or the root of your tools installation.
// THE SAMPLE SOURCE CODE IS PROVIDED "AS IS", WITH NO WARRANTIES.
//
#include "spOS.h"
#include "spPlatform.h"
#include "spCommon.h"
#include "SPDebugDef.h"
#include "spLibNumPadParser.h"

///#define		DD_DEBUG_SHOW
#ifdef DD_DEBUG_SHOW
#include "spLibDDrawClass.h"
#endif





#define		SPPREFIX			TEXT("NPPLib:")
#define		LIBMSGFLAG			(dDOUT|0x0FFFFFFF)		///diag msg only
///#define		LIBMSGFLAG			(dNOUT|0x0FFFFFFF)		///nk msg only


#define		SEQSTRING_LEN		5

///phototype

///                                                           2, 3, 5, 7, 0
///static TOUCHHOOKEXT_AREA_SET	SeqString[SEQSTRING_LEN] = {AREA_TWO, AREA_THREE, AREA_FIVE, AREA_SEVEN, AREA_ZERO};
///                                                           *, #, 0, 6, #
static TOUCHHOOKEXT_AREA_SET	SeqString[SEQSTRING_LEN] = {AREA_STAR, AREA_POUND, AREA_ZERO, AREA_SIX, AREA_POUND};


static int 			iScreenX = 0;
static int 			iScreenY = 0;
static DWORD 		dwPreTickCount = 0;
#ifdef DD_DEBUG_SHOW
static spLibDDraw	*pmyDD = NULL;
#endif




/// *****************************************
/// interface functions
/// *****************************************
BOOL spLibParseAreaInit( void )
{
	BOOL bRet = FALSE;
	
	iScreenX = GetSystemMetrics( SM_CXSCREEN );
	iScreenX = iScreenX * 4;
	iScreenX = iScreenX/3;
	
	iScreenY = GetSystemMetrics( SM_CYSCREEN );
	iScreenY = iScreenY * 4;
	iScreenY = iScreenY/4;
	dwPreTickCount = GetTickCount();
	
#ifdef DD_DEBUG_SHOW	
	pmyDD = new spLibDDraw();
	pmyDD->spLibInitDDraw();
#endif
	
	return bRet;
}


BOOL spLibParseAreaDeInit( void )
{
	BOOL bRet = TRUE;

#ifdef DD_DEBUG_SHOW	
	delete pmyDD;
#endif
	
	return bRet;
}


TOUCHHOOKEXT_AREA_SET spLibParseAreaCondition( INT X, INT Y )
{
	TOUCHHOOKEXT_AREA_SET tasRet = AREA_ONE;
	
	if( (X > 0) && (X < (iScreenX)) )
	{
		if( (Y > 0) && (Y < (iScreenY)) )
		{	///"1"
			tasRet = AREA_ONE;
		}
		else
		if( (Y > (iScreenY)) && (Y < (iScreenY*2)) )
		{	///"4"
			tasRet = AREA_FOUR;
		}
		else
		if( (Y > (iScreenY*2)) && (Y < (iScreenY*3)) )
		{	///"7"
			tasRet = AREA_SEVEN;
		}
		else
		{	///"*"
			tasRet = AREA_STAR;
		}
	}
	else
	if( (X > (iScreenX)) && (X < (iScreenX*2)) )
	{
		if( (Y > 0) && (Y < (iScreenY)) )
		{	///"2"
			tasRet = AREA_TWO;
		}
		else
		if( (Y > (iScreenY)) && (Y < (iScreenY*2)) )
		{	///"5"
			tasRet = AREA_FIVE;
		}
		else
		if( (Y > (iScreenY*2)) && (Y < (iScreenY*3)) )
		{	///"8"
			tasRet = AREA_EIGHT;		
		}
		else
		{	///"0"
			tasRet = AREA_ZERO;
		}
	}
	else
	{
		if( (Y > 0) && (Y < (iScreenY)) )
		{	///"3"
			tasRet = AREA_THREE;
		}
		else
		if( (Y > (iScreenY)) && (Y < (iScreenY*2)) )
		{	///"6"
			tasRet = AREA_SIX;
		}
		else
		if( (Y > (iScreenY*2)) && (Y < (iScreenY*3)) )
		{	///"9"
			tasRet = AREA_NINE;
		}
		else
		{	///"#"
			tasRet = AREA_POUND;
		}
	}

#ifdef DD_DEBUG_SHOW	
	pmyDD->spLibTextDraw( 200, 170, TEXT("HIT =>%4d, [%d,%d]"), tasRet, iScreenX, iScreenY );
#endif
	
	return tasRet;
}



DWORD spLibParseSequenceCondition( TOUCHHOOKEXT_AREA_SET ThisHit )
{
	DWORD dwRet = (-1);
	
	static DWORD dwCount = 0;
	static BOOL bHitAll = FALSE;
	
	if( !bHitAll )
	{
		if( ThisHit == SeqString[dwCount] )
		{
			dwCount++;
#ifdef DD_DEBUG_SHOW
			pmyDD->spLibTextDraw( 200, 200, TEXT("HIT %d!!!"), ThisHit );
#endif
		}
		else
		{
			dwCount = 0;
#ifdef DD_DEBUG_SHOW
			pmyDD->spLibTextDraw( 200, 200, TEXT("Clear!!!") );
#endif
		}
	
		if( dwCount == SEQSTRING_LEN )
		{
			dwRet = 99;
			bHitAll = TRUE;
			dwCount = 0;
#ifdef DD_DEBUG_SHOW
			pmyDD->spLibTextDraw( 200, 200, TEXT("All HIT!!!") );
#endif
		}
	}
	else
	{
		bHitAll = FALSE;
		dwCount = 0;
		
		if( ThisHit == AREA_ZERO )
			dwRet = 0;
		else
		if( ThisHit == AREA_ONE )
			dwRet = 1;
		else
		if( ThisHit == AREA_TWO )
			dwRet = 2;
		else
		if( ThisHit == AREA_THREE )
			dwRet = 3;
		else
		if( ThisHit == AREA_FOUR )
			dwRet = 4;
		else
		if( ThisHit == AREA_FIVE )
			dwRet = 5;
		else
		if( ThisHit == AREA_SIX )
			dwRet = 6;
		else
		if( ThisHit == AREA_SEVEN )
			dwRet = 7;
		else
		if( ThisHit == AREA_EIGHT )
			dwRet = 8;
		else
		if( ThisHit == AREA_NINE )
			dwRet = 9;
		else
		if( ThisHit == AREA_STAR )
			dwRet = 10;
		else
		if( ThisHit == AREA_POUND )
			dwRet = 11;
			
	}
	
	return dwRet;
}
