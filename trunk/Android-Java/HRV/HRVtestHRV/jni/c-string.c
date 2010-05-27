
#include "c-common.h"


static char cTagIHRtmp[] 	= "IHR=1234";
static char cTagVERtmp[] 	= "VER=E1.91";
static char cTagTEtmp[] 	= "TE=2345";
static char cTagHRtmp[] 	= "HR=23456";
static char cTagSDtmp[] 	= "SD=23456";
static char cTagRAWtmp[] 	= "RAW=32";
static char cTagRAWDATAtmp[] 	= "12345678901234567890123456789012";
static char cStrTmp[] 	= "1234567890123456789012345678901234567890";


static int SetRandom32( unsigned char *pChar )
{
	int iRet = 0;
	static unsigned char cPrev = 100;
	int iRand;
	unsigned char *pT;
	
	pT = pChar;
	
	DBGMSG("SPD@SetRandom32 +++\n");
	
	for( iRet = 0; iRet < 32; iRet++ )
	{
#if 1
		///*pChar = 33;
		srandom( iRet+cPrev );
		iRand = random();
		if( iRand > 0 )
			cPrev = cPrev + (((short)iRand)&0x03);
		else
			cPrev = cPrev - (((short)iRand)&0x03);
#else
		cPrev = 0x44;
#endif		
		cPrev = abs(cPrev);	
		*pT = abs(cPrev);
		///DBGMSG("%d ", (short)*pT);
		pT++;
	}	
	
	Print32( pChar );
	
	DBGMSG("SPD@SetRandom32 %d---\n", iRet);	
	return iRet;
}



///int GetRandomTagStr( char *pStr )
static int GetRandomTagStr( unsigned *pStr )
{
	int iRet = 0;
	static unsigned uCount = 0;
	char *pChar;
	
	DBGMSG("SPD@GetRandomTagStr +++\n");
	
	///if( NULL != ppStr )
	if( 1 )
	{
		switch( uCount%8 )
		{
			case 0:
				pChar = cTagIHRtmp;
				iRet = 8;	///??get string size
				break;
			case 1:
				pChar = cTagVERtmp;
				iRet = 9;
				break;
			case 2:
				pChar = cTagTEtmp;
				iRet = 7;
				break;
			case 3:
				pChar = cTagHRtmp;
				iRet = 8;
				break;
			case 4:
				pChar = cTagSDtmp;
				iRet = 8;
				break;
			default:
				pChar = cTagIHRtmp;
				iRet = 8;
				break;
		}///switch
		uCount++;
	}
	else
		DBGMSG("SPD@GetRandomTagStr ERROR!!!\n");	
	
	DBGMSG("SPD@GetRandomTagStr %d 0x%08x %s.\n", iRet, (unsigned)pChar, pChar);
	*pStr = (unsigned)pChar;
	DBGMSG("SPD@GetRandomTagStr %d 0x%08x %s.\n", iRet, *pStr, (char *)pStr);
	DBGMSG("SPD@GetRandomTagStr ---\n");
	return iRet;
}

///int GetRandomRAWDATAStr( char **ppStr )
static int GetRandomRAWDATAStr( unsigned *pStr )
{
	int iRet = 0;
	char *pChar;
	
	DBGMSG("SPD@GetRandomRAWDATAStr +++\n");
	
	if( 1 )
	{
		pChar = cTagRAWDATAtmp;
		SetRandom32( pChar );
		iRet = 32;
	}
	else
		DBGMSG("SPD@GetRandomRAWDATAStr ERROR!!!\n");	
	
	///DBGMSG("SPD@GetRandomRAWDATAStr %d 0x%08x %s.\n", iRet, (unsigned)pChar, pChar);
	*pStr = (unsigned)pChar;
	///DBGMSG("SPD@GetRandomRAWDATAStr %d 0x%08x %s.\n", iRet, *pStr, (char *)pStr);
	DBGMSG("SPD@GetRandomRAWDATAStr ---\n");
	return iRet;
}


///int GetRandomStr( char *pStr )
int GetRandomStr( unsigned *pStr )
{
	int iRet = 0;
	static unsigned uCount = 0;
	char *pcTempStr = NULL;
	unsigned	upTemp;
	char *pTemp = NULL;
	
	DBGMSG("SPD@GetRandomStr +++\n");
	if( 1 )
	{
		///pTemp = *ppStr;
		pTemp = cStrTmp;///common buffer
		DBGMSG("SPD@GetRandomStr 000\n");
		
		
		DBGMSG("SPD@GetRandomStr 111\n");
		if( 0 == uCount%8 )
			iRet = GetRandomTagStr( &upTemp );
		else
		{
			///memcpy( pTemp, cTagRAWtmp, 6 );	//hard code
			///pTemp = pTemp + 6;	///shift
			iRet = GetRandomRAWDATAStr( &upTemp );
		}	

		///DBGMSG("SPD@GetRandomStr %d 0x%08x %s.\n", iRet, (unsigned)*ppcTempStr, *ppcTempStr);
		DBGMSG("SPD@GetRandomStr %d 0x%08x %s.\n", iRet, upTemp, (char *)upTemp);
		pcTempStr = (char *)upTemp;
		DBGMSG("SPD@GetRandomStr %d 0x%08x %s.\n", iRet, (unsigned)pcTempStr, pcTempStr);
		///copy random string to 
		memcpy( pTemp, pcTempStr, iRet );
		pTemp = pTemp + iRet;	///shift
		DBGMSG("SPD@GetRandomStr 333\n");
		if( 0 == uCount%8 )
			iRet = iRet + 1;
		else
		{
			memcpy( pTemp, cTagRAWtmp, 6 );	//hard code
			pTemp = pTemp + 6;	///shift

			iRet = iRet + 1 + 6;
		}

		*pTemp = CHECKBYTECODE;	///add "return" code
		DBGMSG("SPD@GetRandomStr 333\n");
		
		pTemp = pTemp + 1;
		*pTemp = '\0';		///add string end
		
		uCount++;
		*pStr = (unsigned)cStrTmp;///common buffer
	}
	else
		DBGMSG("SPD@GetRandomStr ERROR!!!\n");
		
	DBGMSG("SPD@GetRandomStr %d 0x%08x %s.\n", iRet, (unsigned)cStrTmp, cStrTmp);	
	DBGMSG("SPD@GetRandomStr %d 0x%08x %s.\n", iRet, *pStr, (char *)pStr);
	DBGMSG("SPD@GetRandomStr ---\n");
	return iRet;
}


