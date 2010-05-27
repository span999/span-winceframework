// SBuffer.h: implementation of the SBuffer class.
//
//////////////////////////////////////////////////////////////////////


#ifndef __SBUFFER_H__
#define __SBUFFER_H__



///windows BMP file header define
typedef struct _WIN_BMP_FILE_HEADER
{
	///UINT8 	Signature0;					///must be 0x42 hex 
	///UINT8 	Signature1;					///must be 0x4D hex 	
	UINT32	Signature;					///must be 0x4D424D42 hex
	UINT32	FileSize;					///headers size + image data size
	UINT16 	reserved0;					///must be zero
	UINT16 	reserved1;					///must be zero
	UINT32	ImageDataOffset;			///usually 0x36 (54 bytes)
} WINBMPFILEHEADER, *PWINBMPFILEHEADER;

///windows BMP info header define
typedef struct _WIN_BMP_INFO_HEADER
{
	UINT32	BITMAPINFOHEADERSize;		///must be 0x28 (40 bytes)
	UINT32	ImageWidth;					///x resolution
	UINT32	ImageHeight;				///y resolution
	UINT16	PlanesNumber;				///must be 1
	UINT16	BitsPerPixel;				///must be (1, 4, 8, or 24)
	UINT32	CompressType;				///compression type (0=none, 1=RLE-8, 2=RLE-4)
	UINT32	ImageDataSize;				///size of image data in bytes (including padding)
	UINT32	HpixelPerMeter;				///0, horizontal resolution in pixels per meter (unreliable)
	UINT32	VpixelPerMeter;				///0, vertical resolution in pixels per meter (unreliable)	
	UINT32	ColorNumber;				///0, number of colors in image, or zero
	UINT32	VIPColorNumber;				///0, number of important colors in image, or zero
} WINBMPINFOHEADER, *PWINBMPINFOHEADER;


#define		SBUFFER_SIZE_IN_BYTE	(4*1024)



//////////// class ///////////////////////////////////////////////////
class SBuffer
{
public:
	SBuffer();
	SBuffer( DWORD dwBufSize );
	~SBuffer();
	
	void BufferLock();
	void BufferUnLock();
	///data in
	BOOL Copy2Buffer( PBYTE pSrc, DWORD dwSrcSize );
	BOOL Add2Buffer( PBYTE pSrc, DWORD dwSrcSize );
	///data out
	BOOL CopyBuffer2( PBYTE pDes, DWORD dwDesSize );
	DWORD GetBufferSize();
	DWORD GetMaxBufferSize();
	BOOL IsBufferValid();
	void ClearBuffer();
	PBYTE GetBufferPointer();
	void BufferFilled(DWORD dwActualSize);
	void DumpBuffer();
	DWORD GetDataID();
	BOOL IsInUse();
	void SetDataID(DWORD dwID);
	void SetBufDataLen(DWORD dwBufFillLen);

protected:
	BOOL SBuffer_Initail( DWORD dwBufSize );
	BOOL SBuffer_DeInitail();
	
protected:
	DWORD dwDataID;
	BOOL bInUse;
	PBYTE pBuffer;
	PBYTE pCurBufStart;
	DWORD dwBufferSizeSet;
	BOOL bBufContentValid;
	DWORD dwBufContentCount;
	CRITICAL_SECTION m_Lock;

};

//////////// class ///////////////////////////////////////////////////
class SBufferNode : public SBuffer
{
public:
	SBufferNode();
	SBufferNode( DWORD dwBufferSizeSetting );
	~SBufferNode();
	
	SBufferNode* GetNextNode();
	SBufferNode* GetPrivNode();
	BOOL SetNextNode( SBufferNode *pNewNode );
	BOOL SetPrivNode( SBufferNode *pNewNode );
	
protected:
	SBufferNode *pNextNode;
	SBufferNode *pPrivNode;
		
};




#define		SFIFO_DEPTH		16

//////////// class ///////////////////////////////////////////////////
class SFInFOut
{
public:
	SFInFOut();
	SFInFOut( DWORD dwEachBuf );
	~SFInFOut();
	DWORD GetFIFOMaxDepth();			///get FIFO Max depth
	DWORD GetCurFIFODepth();			///get current FIFO depth
	DWORD GetBufferMaxSize();			///get max buffer size

	///data in	
	BOOL Copy2FIFO( PBYTE pSrc, DWORD dwSrcSize );
	
	///data out
	DWORD FIFOCopy2( PBYTE pDes, DWORD dwDesSize );	
	
	BOOL ClearFIFO();					///clear all FIFO data

protected:
	BOOL SFInFOut_Initail( DWORD dwEachBufSize );
	BOOL SFInFOut_DeInitail();
	void FIFOLock();					///lock whole FIFO(in/out)
	void FIFOUnLock();					///unlock whole FIFO(in/out)
	void NodeLock();
	void NodeUnLock();
	DWORD GetCurBufferSize();			///get current buffer size
	BOOL AddNode( SBufferNode *pNewNode );
	BOOL DeleteNode( SBufferNode *pDeleteNode );
			
protected:
	CRITICAL_SECTION m_Lock;
	CRITICAL_SECTION m_NodeLock;
	SBufferNode	*p2HeadNode;
	SBufferNode	*p2TailNode;
	DWORD dwCurUsedFIFO;
	DWORD dwIDCount;
	DWORD dwEachBufSize;
	
};





#endif	///#ifndef __SBUFFER_H__