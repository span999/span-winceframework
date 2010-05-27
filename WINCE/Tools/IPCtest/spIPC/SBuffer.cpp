// SBuffer.cpp: implementation of the SBuffer class.
//
//////////////////////////////////////////////////////////////////////

///#include "stdafx.h"
#include "windows.h"
#include "SBuffer.h"

////// SBuffer //////////////////////////////////////////////////////
SBuffer::SBuffer()
{
///	pBuffer = NULL;
///	bBufContentValid = FALSE;
///	dwBufContentCount = 0;
///	bInUse = FALSE;
///	
///	///NKDbgPrintfW( L"SBuffer1 dwBufferSizeSet=%d\r\n", dwBufferSizeSet );
//////	if( dwBufferSizeSet == 0 )	///change the setting only when it's not been setting yet
///		dwBufferSizeSet = SBUFFER_SIZE_IN_BYTE;	///buffer size set default
///	
///	///NKDbgPrintfW( L"SBuffer1 dwBufferSizeSet=%d\r\n", dwBufferSizeSet );
///	InitializeCriticalSection(&m_Lock);
///	pBuffer = (PBYTE)LocalAlloc( LPTR, dwBufferSizeSet );
	SBuffer_Initail( SBUFFER_SIZE_IN_BYTE );
}

SBuffer::SBuffer( DWORD dwBufSize )
{
///	pBuffer = NULL;
///	bBufContentValid = FALSE;
///	dwBufContentCount = 0;
///	bInUse = FALSE;
///	///dwBufferSizeSet = 0;
///	///NKDbgPrintfW( L"SBuffer2 dwBufferSizeSet=%d\r\n", dwBufferSizeSet );
///	dwBufferSizeSet = dwBufSize;	///user set buffer size
///	///NKDbgPrintfW( L"SBuffer2 dwBufferSizeSet=%d\r\n", dwBufferSizeSet );
///	
///	InitializeCriticalSection(&m_Lock);
///	pBuffer = (PBYTE)LocalAlloc( LPTR, dwBufferSizeSet );
	SBuffer_Initail( dwBufSize );	
}


SBuffer::~SBuffer()
{
	SBuffer_DeInitail();
}


BOOL SBuffer::SBuffer_Initail( DWORD dwBufSize )
{
	BOOL bRet = FALSE;
	
	///member init
	pBuffer = NULL;
	bBufContentValid = FALSE;
	dwBufContentCount = 0;
	bInUse = FALSE;
	
	///NKDbgPrintfW( L"SBuffer1 dwBufferSizeSet=%d\r\n", dwBufferSizeSet );
	dwBufferSizeSet = dwBufSize;	///buffer size set
	///NKDbgPrintfW( L"SBuffer1 dwBufferSizeSet=%d\r\n", dwBufferSizeSet );
	InitializeCriticalSection(&m_Lock);
	pBuffer = (PBYTE)LocalAlloc( LPTR, dwBufferSizeSet );
	
	if( NULL == pBuffer )
		bRet = FALSE;
	else
		bRet = TRUE;
	
	///NKDbgPrintfW( L"SBuffer::SBuffer_Initail %s\r\n", bRet?L"success":L"failure" );
	return bRet;
}


BOOL SBuffer::SBuffer_DeInitail()
{
	BOOL bRet = FALSE;
	
	DeleteCriticalSection(&m_Lock);
	
	if( NULL == LocalFree( pBuffer ) )
		bRet = TRUE;
	else
		bRet = FALSE;
	
	///NKDbgPrintfW( L"SBuffer::SBuffer_DeInitail %s\r\n", bRet?L"success":L"failure" );
	return bRet;
}

void SBuffer::SetBufDataLen( DWORD dwBufFillLen )
{
	if( dwBufferSizeSet >= dwBufFillLen )
		dwBufContentCount = dwBufFillLen;
	else
		dwBufContentCount = dwBufferSizeSet;
}

void SBuffer::SetDataID( DWORD dwID )
{
	dwDataID = dwID;
}

DWORD SBuffer::GetDataID()
{
	return dwDataID;
}

BOOL SBuffer::IsInUse()
{
	return bInUse;
}

void SBuffer::BufferLock()
{
	EnterCriticalSection(&m_Lock);
	bInUse = TRUE;
}

void SBuffer::BufferUnLock()
{
	bInUse = FALSE;
	LeaveCriticalSection(&m_Lock);
}

BOOL SBuffer::Copy2Buffer( PBYTE pSrc, DWORD dwSrcSize )
{
	BOOL bRet = TRUE;
	
	if( NULL == pSrc || dwSrcSize > dwBufferSizeSet )
		bRet = FALSE;
	
	if( bRet ) {
		memcpy( pBuffer, pSrc, dwSrcSize );
		dwBufContentCount = dwSrcSize;
		bBufContentValid = TRUE;
	}
	
	return bRet;	
}

BOOL SBuffer::Add2Buffer( PBYTE pSrc, DWORD dwSrcSize )
{
	BOOL bRet = TRUE;
	
	if( NULL == pSrc || (dwSrcSize+dwBufContentCount) > dwBufferSizeSet )
		bRet = FALSE;
	
	if( bRet ) {
		memcpy( (pBuffer+dwBufContentCount) , pSrc, dwSrcSize );
		dwBufContentCount = dwBufContentCount + dwSrcSize;
		bBufContentValid = TRUE;
	}
	
	return bRet;	
}

BOOL SBuffer::CopyBuffer2( PBYTE pDes, DWORD dwDesSize )
{
	BOOL bRet = TRUE;
	
	///if( NULL == pDes || dwDesSize < dwBufferSizeSet )
	if( NULL == pDes || dwDesSize < GetBufferSize() )
		bRet = FALSE;
	
	if( bRet ) {
		///memcpy( pDes, pBuffer, dwDesSize );
		memcpy( pDes, pBuffer, GetBufferSize() );
	}
	
	return bRet;	
}

BOOL SBuffer::IsBufferValid()
{
	return bBufContentValid;
}

void SBuffer::BufferFilled(DWORD dwActualSize)
{
	if( dwBufferSizeSet >= dwActualSize )
		dwBufContentCount = dwActualSize;
	else
		dwBufContentCount = dwBufferSizeSet;

	bBufContentValid = TRUE;
}

PBYTE SBuffer::GetBufferPointer()
{
	return pBuffer;
}

DWORD SBuffer::GetMaxBufferSize()
{
	return dwBufferSizeSet;
}

DWORD SBuffer::GetBufferSize()
{
	return dwBufContentCount;
}

void SBuffer::ClearBuffer()
{
	dwBufContentCount = 0;
	bBufContentValid = FALSE;
	dwDataID = 0;
	memset( pBuffer, 0, dwBufferSizeSet );
}

void SBuffer::DumpBuffer()
{
	DWORD dwLoop = 0;
	PBYTE pTemp = pBuffer;
	
	NKDbgPrintfW( L"DumpBuffer =\r\n" );
	while( dwLoop < dwBufContentCount )
	{
		NKDbgPrintfW( L"%02x ", *pTemp );
		pTemp++;
		dwLoop++;
	}
	NKDbgPrintfW( L"\r\n" );
}


////// SBufferNode //////////////////////////////////////////////////////

SBufferNode::SBufferNode()
	: SBuffer()
{
	pNextNode = NULL;
	pPrivNode = NULL;
	///NKDbgPrintfW( L"SBufferNode1 dwBufferSizeSet=%d\r\n", dwBufferSizeSet );
	///dwBufferSizeSetting = SBUFFER_SIZE_IN_BYTE;	///set default buffer size
	dwBufferSizeSet = SBUFFER_SIZE_IN_BYTE;	///set default buffer size
	///NKDbgPrintfW( L"SBufferNode1 dwBufferSizeSetting=%d\r\n", dwBufferSizeSetting );
	///NKDbgPrintfW( L"SBufferNode1 dwBufferSizeSet=%d\r\n", dwBufferSizeSet );
}

SBufferNode::SBufferNode( DWORD dwSizeSetting )
		: SBuffer( dwSizeSetting )
{
	pNextNode = NULL;
	pPrivNode = NULL;
	///NKDbgPrintfW( L"SBufferNode2 dwBufferSizeSet=%d\r\n", dwBufferSizeSet );
	///dwBufferSizeSetting = dwSizeSetting;	///user set buffer size
	dwBufferSizeSet = dwSizeSetting;	///user set buffer size
	///NKDbgPrintfW( L"SBufferNode2 dwBufferSizeSetting=%d\r\n", dwBufferSizeSetting );
	///NKDbgPrintfW( L"SBufferNode2 dwBufferSizeSet=%d\r\n", dwBufferSizeSet );
}

SBufferNode::~SBufferNode()
{
	///SBuffer_DeInitail();
}

SBufferNode* SBufferNode::GetNextNode()
{
	SBufferNode* pSBuf = NULL;
	
	pSBuf = pNextNode;
	
	return pSBuf;
}

SBufferNode* SBufferNode::GetPrivNode()
{
	SBufferNode* pSBuf = NULL;
	
	pSBuf = pPrivNode;
	
	return pSBuf;
}

BOOL SBufferNode::SetNextNode( SBufferNode *pNewNode )
{
	BOOL bRet = TRUE;
	
	pNextNode = pNewNode;
	
	return bRet;
}
	
BOOL SBufferNode::SetPrivNode( SBufferNode *pNewNode )
{
	BOOL bRet = TRUE;
	
	pPrivNode = pNewNode;
	
	return bRet;
}

////// SFinFout //////////////////////////////////////////////////////

SFInFOut::SFInFOut()
{
	SFInFOut_Initail( SBUFFER_SIZE_IN_BYTE );
}

SFInFOut::SFInFOut( DWORD dwEachBuf )
{
	SFInFOut_Initail( dwEachBuf );
}

SFInFOut::~SFInFOut()
{
	SFInFOut_DeInitail();
}

BOOL SFInFOut::SFInFOut_Initail( DWORD dwEachBuf )
{
	BOOL bRet = TRUE;
	
	///member init
	dwCurUsedFIFO = 0;
	dwIDCount = 0;
	InitializeCriticalSection(&m_Lock);
	InitializeCriticalSection(&m_NodeLock);
	
	dwEachBufSize = dwEachBuf;	/// buffer size setting
	
	p2HeadNode = NULL;
	p2TailNode = NULL;
	
	///NKDbgPrintfW( L"SFInFOut::SFInFOut_Initail %s\r\n", bRet?L"success":L"failure" );
	return bRet;
}


BOOL SFInFOut::SFInFOut_DeInitail()
{
	BOOL bRet = TRUE;
	
	DeleteCriticalSection(&m_Lock);
	DeleteCriticalSection(&m_NodeLock);	
	///NKDbgPrintfW( L"SFInFOut::SFInFOut_DeInitail %s\r\n", bRet?L"success":L"failure" );
	return bRet;
}

void SFInFOut::FIFOLock()
{
	EnterCriticalSection(&m_Lock);
}

void SFInFOut::FIFOUnLock()
{
	LeaveCriticalSection(&m_Lock);
}

void SFInFOut::NodeLock()
{
	EnterCriticalSection(&m_NodeLock);
}

void SFInFOut::NodeUnLock()
{
	LeaveCriticalSection(&m_NodeLock);
}

DWORD SFInFOut::GetFIFOMaxDepth()
{
	DWORD dwRet = 0;
	dwRet = SFIFO_DEPTH;
	return dwRet;
}

DWORD SFInFOut::GetCurFIFODepth()
{
	DWORD dwRet = 0;
	dwRet = dwCurUsedFIFO;
	return dwRet;
}

DWORD SFInFOut::GetBufferMaxSize()
{
	DWORD dwRet = 0;

	return dwRet;
}

DWORD SFInFOut::GetCurBufferSize()
{
	DWORD dwRet = 0;
	
	return dwRet;
}	

BOOL SFInFOut::AddNode( SBufferNode *pNewNode )
{
	BOOL bRet = FALSE;
	
	///NKDbgPrintfW( L"SFInFOut::AddNode +++\r\n" );
	NodeLock();
	
	if( (NULL == p2HeadNode) && (NULL == p2TailNode) ) {
		///NKDbgPrintfW( L"SFInFOut::AddNode FIFO first node !\r\n" );
		///empty FIFO, head the same as tail
		p2HeadNode = pNewNode;
		p2TailNode = pNewNode;
		p2HeadNode->SetPrivNode( NULL );
		p2TailNode->SetNextNode( NULL );
		dwIDCount++;
		p2TailNode->SetDataID( dwIDCount );
		dwCurUsedFIFO++;
		bRet = TRUE;
		
	} else if( (NULL == p2HeadNode) || (NULL == p2TailNode) ) {
		NKDbgPrintfW( L"SFInFOut::AddNode FIFO error !\r\n" );
		bRet = FALSE;
	} else {
		///we always add the new one to the tail...
		p2TailNode->SetNextNode( pNewNode );	///set old tail next is new tail
		pNewNode->SetPrivNode( p2TailNode );	///set new priv is old tail
		p2TailNode = pNewNode;					///set new as tail
		p2TailNode->SetNextNode( NULL );		///set tail no next
		dwIDCount++;
		p2TailNode->SetDataID( dwIDCount );
		dwCurUsedFIFO++;
		bRet = TRUE;
	}

	NodeUnLock();
	///NKDbgPrintfW( L"SFInFOut::AddNode --- %d\r\n", dwIDCount );
	
	return bRet;
}

BOOL SFInFOut::DeleteNode( SBufferNode *pDeleteNode )
{
	BOOL bRet = FALSE;
	
	///NKDbgPrintfW( L"SFInFOut::DeleteNode +++ %d\r\n", pDeleteNode->GetDataID() );
	NodeLock();
	
	if( (NULL == p2HeadNode) && (NULL == p2TailNode) ) {
		///empty FIFO, head the same as tail
		NKDbgPrintfW( L"SFInFOut::DeleteNode FIFO empty !\r\n" );
		bRet = FALSE;
	} else if( (NULL == p2HeadNode) || (NULL == p2TailNode) ) {
		NKDbgPrintfW( L"SFInFOut::DeleteNode FIFO error !\r\n" );
		bRet = FALSE;
	} else {
		///we always delete from head...
		if( pDeleteNode == p2HeadNode ) {
			if( p2HeadNode->GetNextNode() == NULL ) {
				///NKDbgPrintfW( L"SFInFOut::DeleteNode only %d node in FIFO, the last one 0x%08x=0x%08x\r\n", dwCurUsedFIFO, p2HeadNode, p2TailNode );
				delete p2HeadNode;
				p2HeadNode = NULL;
				p2TailNode = NULL;
				bRet = TRUE;
			} else {
				p2HeadNode = p2HeadNode->GetNextNode();	///set new head
				p2HeadNode->SetPrivNode( NULL );		///set new head priv is NULL
				delete pDeleteNode;
				bRet = TRUE;
			}
			dwCurUsedFIFO--;
		} else {
			NKDbgPrintfW( L"SFInFOut::DeleteNode not head !\r\n" );
		}
	}
	
	NodeUnLock();
	///NKDbgPrintfW( L"SFInFOut::DeleteNode ---\r\n" );
	
	return bRet;
}

///data in	
BOOL SFInFOut::Copy2FIFO( PBYTE pSrc, DWORD dwSrcSize )
{
	BOOL bRet = FALSE;
	
	if( (NULL != pSrc) && (dwSrcSize <= dwEachBufSize) ) {
		SBufferNode *pNewBufNode = NULL;
		///FIFOLock();
		pNewBufNode = new SBufferNode( dwEachBufSize );
		
		if( pNewBufNode != NULL ) {
			///copy data to node
			bRet = pNewBufNode->Copy2Buffer( pSrc, dwSrcSize );
			if( bRet ) {
				///pNewBufNode->DumpBuffer();
				///add node to linking list
				bRet = AddNode( pNewBufNode );
				if( !bRet )
					NKDbgPrintfW( L"TVB:: Copy2FIFO AddNode fail\r\n" );
			}
			else
				NKDbgPrintfW( L"TVB:: Copy2FIFO Copy2Buffer fail\r\n" );
		}
		else
			NKDbgPrintfW( L"TVB:: Copy2FIFO new SBufferNode fail\r\n" );
		///FIFOUnLock();			
	}
	else
		NKDbgPrintfW( L"TVB:: Copy2FIFO invalid parameter pSrc=0x%08x dwSrcSize=%d\r\n", pSrc, dwSrcSize );
	
	return bRet;
}
	
///data out
DWORD SFInFOut::FIFOCopy2( PBYTE pDes, DWORD dwDesSize )
{
	BOOL bRet = FALSE;
	DWORD dwRet = 0;
	
	if( (NULL != pDes) && (dwDesSize >= dwEachBufSize) ) {
		SBufferNode *pTempNode = NULL; 
		NodeLock();
		pTempNode = p2HeadNode;
		NodeUnLock();
		///we always release the data of head		
		if( pTempNode != NULL ) {
			///copy from node
			NodeLock();
			///pTempNode->DumpBuffer();
			bRet = pTempNode->CopyBuffer2( pDes, dwDesSize );
			dwRet = pTempNode->GetBufferSize();
			NodeUnLock();
			///NKDbgPrintfW( L"TVB:: FIFOCopy2 did=%d size=%d\r\n", pTempNode->GetDataID(), dwRet );
			if( bRet ) {
				///delete node
				bRet = DeleteNode( pTempNode );
			}
			else
				NKDbgPrintfW( L"TVB:: FIFOCopy2 CopyBuffer2 fail\r\n" );
		}
		else
			NKDbgPrintfW( L"TVB:: FIFOCopy2 no data in FIFO\r\n" );
	}
	else
		NKDbgPrintfW( L"TVB:: FIFOCopy2 invalid parameter pDes=0x%08x dwDesSize=%d\r\n", pDes, dwDesSize );
	
	if( !bRet )
		dwRet = 0;
	
	///NKDbgPrintfW( L"TVB:: FIFOCopy2 size=%d\r\n", dwRet );
	return dwRet;
}

///clear all data
BOOL SFInFOut::ClearFIFO()
{
	BOOL bRet = FALSE;
	PBYTE pTemp;
	
	pTemp = (PBYTE)LocalAlloc( LPTR, dwEachBufSize );
	
	while( GetCurFIFODepth() > 0 ) {
		FIFOCopy2( pTemp, dwEachBufSize );
	}
	
	bRet = ( GetCurFIFODepth() == 0 )?TRUE:FALSE;
	
	LocalFree( pTemp );
	
	return bRet;
}