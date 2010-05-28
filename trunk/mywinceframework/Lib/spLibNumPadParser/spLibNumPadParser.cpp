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
#include "spLibDDrawClass.h"
#include "spLibNumPadParser.h"




#define		SPPREFIX			TEXT("NPPLib:")
#define		LIBMSGFLAG			(dDOUT|0x0FFFFFFF)		///diag msg only
///#define		LIBMSGFLAG			(dNOUT|0x0FFFFFFF)		///nk msg only

#define		SEQSTRING_LEN		5

///phototype


static TOUCHHOOKEXT_AREA_SET	SeqString[SEQSTRING_LEN] = {AREA_TWO, AREA_THREE, AREA_FIVE, AREA_SEVEN, AREA_ZERO};

static int 			iScreenX = 0;
static int 			iScreenY = 0;
static DWORD 		dwPreTickCount = 0;
static spLibDDraw	*pmyDD = NULL;





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
	
	pmyDD = new spLibDDraw();
	pmyDD->spLibInitDDraw();

	return bRet;
}


BOOL spLibParseAreaDeInit( void )
{
	BOOL bRet = TRUE;
	
	delete pmyDD;
	
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
	
	pmyDD->spLibTextDraw( 200, 170, TEXT("HIT =>%4d, [%d,%d]"), tasRet, iScreenX, iScreenY );
	
	return tasRet;
}



BOOL spLibParseSequenceCondition( TOUCHHOOKEXT_AREA_SET ThisHit )
{
	BOOL bRet = FALSE;
	
	static DWORD dwCount = 0;
	
	if( ThisHit == SeqString[dwCount] )
	{
		dwCount++;
		pmyDD->spLibTextDraw( 200, 200, TEXT("HIT %d!!!"), ThisHit );
	}
	else
	{
		dwCount = 0;
		pmyDD->spLibTextDraw( 200, 200, TEXT("Clear!!!") );
	}
	
	if( dwCount == SEQSTRING_LEN )
	{
		bRet = TRUE;
		dwCount = 0;
		pmyDD->spLibTextDraw( 200, 200, TEXT("All HIT!!!") );
	}
	
	return bRet;
}
