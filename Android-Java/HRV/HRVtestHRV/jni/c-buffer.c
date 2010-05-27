

#include "c-common.h"
#include <pthread.h>


#define RAW_BUF_IDX_BIT_USED	0x1000
#define RAW_BUF_MAX				32
#define RAW_BUF_DATA_MAX		32

struct RAW_buffer_struct {
	long SN;
	unsigned index;
	unsigned length;
	unsigned char data[32];
};


static pthread_mutex_t BufferMutex = PTHREAD_MUTEX_INITIALIZER;

///buffer storage
static struct RAW_buffer_struct raw_buffer_array[32];


static struct RAW_buffer_struct *pArrayHead = NULL;
static struct RAW_buffer_struct *pArrayTail = NULL;
static long HeadCount = 0;
static long TailCount = 0;
static short sArraySize = 0;	///should be (HeadCount-TailCount) 
static long TotalByteCount = 0;

static bool bBufferInited = false;
static short raw_buffer_array_index = 0;	///0~32

static struct RAW_buffer_struct *pNewRequest = NULL;
static struct RAW_buffer_struct *pRemoveRequest = NULL;

int GetCurrArraySize(void)
{
	int iRet = 0;
	iRet = sArraySize;
	return iRet;
}

struct RAW_buffer_struct* RequestArrayAdd( void )
{
	struct RAW_buffer_struct *pNewArray;

	DBGMSG("SPD@RequestArrayAdd +++\n");
	pthread_mutex_lock(&BufferMutex);

	if( NULL == pArrayHead )
	{	///the very first one
		pNewArray = &raw_buffer_array[HeadCount%32];
		pNewRequest = &raw_buffer_array[HeadCount%32];
	}
	else
	{
		pNewArray = &raw_buffer_array[HeadCount%32];
		pNewRequest = &raw_buffer_array[HeadCount%32];
	}

	DBGMSG("SPD@RequestArrayAdd ---\n");
	return pNewArray; 
}


bool RequestArrayAddDone( void )
{
	bool bRet = true;

	DBGMSG("SPD@RequestArrayAddDone +++\n");
	pArrayHead = pNewRequest;

	HeadCount++;
	sArraySize = HeadCount - TailCount;
	DBGMSG("SPD@RequestArrayAddDone HeadCount=%ld pArrayHead=0x%08x pArrayTail=0x%08x \n", HeadCount, (unsigned)pArrayHead, (unsigned)pArrayTail );
	
	TotalByteCount = TotalByteCount + pNewRequest->length;
	pthread_mutex_unlock(&BufferMutex);
	DBGMSG("SPD@RequestArrayAddDone ---\n");
	return bRet; 
}


struct RAW_buffer_struct* RequestArrayRemove( void )
{
	struct RAW_buffer_struct *pTailArray;

	DBGMSG("SPD@RequestArrayRemove +++\n");
	pthread_mutex_lock(&BufferMutex);

	if( NULL == pArrayTail )
	{	///the very first one
		pArrayTail = &raw_buffer_array[TailCount%32];
		pRemoveRequest = &raw_buffer_array[TailCount%32];
	}
	else
	{
		pArrayTail = &raw_buffer_array[TailCount%32];
		pRemoveRequest = &raw_buffer_array[TailCount%32];
	}

	DBGMSG("SPD@RequestArrayRemove ---\n");
	return pArrayTail; 
}


bool RequestArrayRemoveDone( void )
{
	bool bRet = true;

	DBGMSG("SPD@RequestArrayRemoveDone +++\n");
	pArrayTail = pRemoveRequest;

	TailCount++;
	sArraySize = HeadCount - TailCount;
	DBGMSG("SPD@RequestArrayRemoveDone TailCount=%ld pArrayHead=0x%08x pArrayTail=0x%08x \n", TailCount, (unsigned)pArrayHead, (unsigned)pArrayTail );

	pthread_mutex_unlock(&BufferMutex);
	DBGMSG("SPD@RequestArrayRemoveDone ---\n");
	return bRet; 
}


static bool BufferInit( void )
{
	bool bRet = true;
	struct RAW_buffer_struct *pArray = NULL;
	
	DBGMSG("SPD@BufferInit +++\n");

	if( !bBufferInited )
	{
		int iLoopA = 0;
		pArrayHead = NULL;
		pArrayTail = NULL;
		HeadCount = 0;
		TailCount = 0;
		sArraySize = 0;
		raw_buffer_array_index = 0;
		
		///init pointer
		pArray = &raw_buffer_array[0];
		
		for( iLoopA = 0; iLoopA < RAW_BUF_MAX; iLoopA++ )
		{
			pArray->SN = 0;
			pArray->index = 0;
			pArray->length = 0;
			memset( pArray->data, 0, RAW_BUF_DATA_MAX );
			pArray++;
			///DBGMSG("SPD@BufferInit %d\n", iLoopA);
		}

		bBufferInited = true;		
	}
	
	DBGMSG("SPD@BufferInit ---\n");
	return bRet;
}


static bool SetArrayUsed( unsigned *puIn )
{
	bool bRet = true;
	
	*puIn = *puIn | RAW_BUF_IDX_BIT_USED;

	return bRet;
}


static bool IsArrayUsed( unsigned *puIn )
{
	bool bRet = true;
	
	if( 0 == (*puIn & RAW_BUF_IDX_BIT_USED) )
		bRet = false;

	return bRet;
}


static bool ClrArrayUsed( unsigned *puIn )
{
	bool bRet = true;
	
	*puIn = *puIn & ~RAW_BUF_IDX_BIT_USED;

	return bRet;
}


static long GetSN()
{
	static long SN = 0;
	long lRet = 0;
	
	lRet = SN;
	SN++;

	return lRet;
}


static short GetArrayIndex()
{
	static short ArrayIndex = 0;
	short sRet;
	
	BufferInit();

	sRet = ArrayIndex;

	ArrayIndex++;
	if( ArrayIndex >= 32 )
		ArrayIndex = 0;

	return sRet;
}


int Print32( unsigned char *pChar )
{
	int iRet = 0;
	
	DBGMSG("SPD@Print32 +++\n");
	
	DBGMSG("\n");
	for( iRet = 0; iRet < 32; iRet++ )
	{
		DBGMSG("%d[%x] ", *pChar, *pChar );

		if( 7 == iRet%8 )
			DBGMSG("\n");

		pChar++;
	}	
	
	DBGMSG("\n");	
	DBGMSG("SPD@Print32 %d---\n", iRet);	
	return iRet;
}


int DumpArrayStatus()
{
	int iRet = 0;
	DBGMSG("SPD@DumpArrayStatus: +++");

	INFOMSG(" pArrayHead=0x%08x \n", (unsigned)pArrayHead );
	INFOMSG(" pArrayTail=0x%08x \n", (unsigned)pArrayTail );
	INFOMSG(" HeadCount=%08ld \n", HeadCount );
	INFOMSG(" Tailcount=%08ld \n", TailCount );
	INFOMSG(" sArraySize=%02d \n", sArraySize );
	INFOMSG(" TotalByteCount=%08ld \n", TotalByteCount );

	DBGMSG("SPD@DumpArrayStatus: ---");
	return iRet;
}


int DumpArray()
{
	unsigned iLoopA = 0;
	unsigned iLoopB = 0;
	
	for( iLoopA = 0; iLoopA < 32; iLoopA++ )
	{
		if( IsArrayUsed( &raw_buffer_array[iLoopA].index ) )
		{
			INFOMSG("Array=%04d ", iLoopA );
			INFOMSG("SN=%08ld ", raw_buffer_array[iLoopA].SN );
			INFOMSG("index=0x%04x ", raw_buffer_array[iLoopA].index );
			INFOMSG("length=%02d\n", raw_buffer_array[iLoopA].length );
			for( iLoopB = 0; iLoopB < raw_buffer_array[iLoopA].length; iLoopB++ )
				INFOMSG("%c[%d] ", raw_buffer_array[iLoopA].data[iLoopB], raw_buffer_array[iLoopA].data[iLoopB] );
			INFOMSG("\n");
			///ClrArrayUsed( &raw_buffer_array[iLoopA].index );
		}
	}

	return iLoopA;
}


int DumpSingleArray( struct RAW_buffer_struct *pArray )
{
	///int iLoopA = 0;
	unsigned iLoopB = 0;
	
	if( NULL != pArray )
	{
		INFOMSG("SN=%08ld ", pArray->SN );
		INFOMSG("index=0x%04x ", pArray->index );
		INFOMSG("length=%02d\n", pArray->length );
		for( iLoopB = 0; iLoopB < pArray->length; iLoopB++ )
			INFOMSG("%c[%d] ", pArray->data[iLoopB], pArray->data[iLoopB] );
		INFOMSG("\n");
	}

	return iLoopB;
}

/*
	DataCarry
	func: to get stored data from buffer
	In:
	Out:
*/
int DataCarry()
{
	int iRet = 0;
	struct RAW_buffer_struct Array;
	struct RAW_buffer_struct *pArray = NULL;
	
	DBGMSG("SPD@DataCarry: +++\n");
	///prepare a holder
	pArray = &Array;

	///DumpArrayStatus();
	if( sArraySize > 0 )
	{ 
		///get stored data for data storage
		pArray = RequestArrayRemove();
		DumpSingleArray( pArray );
		WriteLog( pArray->data, pArray->length );
		iRet = pArray->length;

		///clear buffer
		pArray->SN = 0;
		ClrArrayUsed( &(pArray->index) );
		pArray->length = 0;
		memset( pArray->data, 0, RAW_BUF_DATA_MAX );  
		///confirm data remove
		RequestArrayRemoveDone();
	}

	///DumpArrayStatus();
	DBGMSG("SPD@DataCarry: ---\n");
	return iRet;	
}


/*
	DataStore
	func: to store the RAW data from string to a buffer array.
	In: string pointer
	Out: number of data stored
*/
int DataStore( unsigned char *upChar )
{
	int iRet = 0;
	int iLoop = 0;
	unsigned char *upT;
	unsigned char ucThisChar;
	struct RAW_buffer_struct *pArray = NULL;
	
	///make a copy of data string
	upT = upChar;

	DBGMSG("SPD@DataStore: +++\n");
	
	///check each char untill we find 0x0D
	for( iLoop = 0; iLoop < 50; iLoop++ )
	{	
		///get current char
		ucThisChar = *upT;
		///check 
		if( CHECKBYTECODE == ucThisChar )
			break;

		///INFOMSG( "%c ", *upT );
		upT++;
	}

	///INFOMSG("\n");
	
	///counter of char before CHECKBYTECODE
	iRet = iLoop + 1;
	INFOMSG("SPD@DataStore::size=%d => %s\n", iRet, (iRet>32)?"RAW data":"Tag info" );
	
	///Note: we drop data after 32 bytes RAW data, usually drop "RAW=32"
	///get new array for data store
	pArray = RequestArrayAdd();
	pArray->SN = GetSN();
	SetArrayUsed( &(pArray->index) );
	pArray->length = (iRet>RAW_BUF_DATA_MAX)?RAW_BUF_DATA_MAX:iRet;
	memcpy( pArray->data, upChar, (iRet>RAW_BUF_DATA_MAX)?RAW_BUF_DATA_MAX:iRet ); 
	///confirm data store
	RequestArrayAddDone();

	DBGMSG("SPD@DataStore: ---\n");

	return iRet;
}


