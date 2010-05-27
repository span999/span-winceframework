/* MiTAC Int'l Corp. 2001~2006

================================================================================
Description:

================================================================================
Release notes

Modifier	        sign	        date		    Description
--------------------------------------------------------------------------------

*/

#include <windows.h>
#include "spIPCClass.h"



/// SIPCList ///////////////////////////////////////////////////////////////////
SIPCList::SIPCList()
{
    DWORD dwLoop = 0;
    
    for( dwLoop = 0; dwLoop < IPCLISTMAXSIZE; dwLoop++ )
        m_pIPCInfoList[dwLoop] = NULL;
    
    m_dwCurrentListSize = 0;
}

SIPCList::~SIPCList()
{
    
}

BOOL SIPCList::AddItem( SPIPC_INFO ipcInfo )
{
    BOOL bRet = FALSE;
    SPIPC_INFO *pNewIPCinfo = NULL;
    DWORD dwLoop;
    BOOL bIsClient = FALSE;
    

    for( dwLoop = 0; dwLoop < IPCLISTMAXSIZE; dwLoop++ ) {
        if( m_pIPCInfoList[dwLoop] != NULL ) {  ///find ~
            if( m_pIPCInfoList[dwLoop]->dwIndex == ipcInfo.dwIndex ) {  ///check if there already one host
                ///found host, this is client
                ///need no add item
                bIsClient = TRUE;   ///indicate this is client
                bRet = TRUE;
                IPCDBGMSG( dCLAS, ( L"%s:SIPCList::AddItem client %s\r\n", DBGMSGPREFIX, bRet?L"ok":L"fail") );
                break;
            }
        }
    }
    
    if( bIsClient )
        return bRet;
    else
        IPCDBGMSG( dCLAS, ( L"%s:SIPCList::AddItem not client \r\n", DBGMSGPREFIX) );

    ///it should be host
    for( dwLoop = 0; dwLoop < IPCLISTMAXSIZE; dwLoop++ ) {
        
        if( m_pIPCInfoList[dwLoop] == NULL ) {  ///find a empty one ~
            ///create a new list item
            pNewIPCinfo = new SPIPC_INFO;
            m_pIPCInfoList[dwLoop] = pNewIPCinfo;
            
            if( m_pIPCInfoList[dwLoop] != NULL ) {
                ///init IPCinfo content

                m_pIPCInfoList[dwLoop]->dwIndex = ipcInfo.dwIndex;
                m_pIPCInfoList[dwLoop]->dwBulkSize = ipcInfo.dwBulkSize;
                ///m_pIPCInfoList[dwLoop]->hHostTxDone = ipcInfo.hHostTxDone;
                ///m_pIPCInfoList[dwLoop]->hHostRxDone = ipcInfo.hHostRxDone;
                ///m_pIPCInfoList[dwLoop]->hClientTxDone = ipcInfo.hClientTxDone;
                ///m_pIPCInfoList[dwLoop]->hClientRxDone = ipcInfo.hClientRxDone;
                ///m_pIPCInfoList[dwLoop]->pIPCobject = ipcInfo.pIPCobject;
                m_pIPCInfoList[dwLoop]->hHostTxDone = NULL;
                m_pIPCInfoList[dwLoop]->hHostRxDone = NULL;
                m_pIPCInfoList[dwLoop]->hClientTxDone = NULL;
                m_pIPCInfoList[dwLoop]->hClientRxDone = NULL;
                m_pIPCInfoList[dwLoop]->pIPCobject = NULL;
                
                bRet = TRUE;
            } else {
                bRet = FALSE;
                delete pNewIPCinfo;
                IPCDBGMSG( dFAIL, ( L"%s:SIPCList::AddItem create list fail!\r\n", DBGMSGPREFIX) );
            }
            
            if( bRet ) {
                ///create a new IPC
                ///setup new IPC

                m_pIPCInfoList[dwLoop]->pIPCobject = new SIPC;
                if( NULL != m_pIPCInfoList[dwLoop]->pIPCobject ) {
                    bRet = TRUE;
                } else {
                    IPCDBGMSG( dFAIL, ( L"%s:SIPCList::AddItem create IPC fail!\r\n", DBGMSGPREFIX) );
                    bRet = FALSE;
                }    
            }

            if( bRet ) {
                ///map IPC info into list
                SPIPC_INFO* pIPCinfo = NULL;
                SIPC* pIPC = NULL;
                pIPC = (SIPC*)m_pIPCInfoList[dwLoop]->pIPCobject;
                
                ///get new IPC content into new list
                pIPCinfo = pIPC->GetIPCinfo();
                if( NULL != pIPCinfo ) {
                    m_pIPCInfoList[dwLoop]->hHostTxDone = pIPCinfo->hHostTxDone;
                    m_pIPCInfoList[dwLoop]->hHostRxDone = pIPCinfo->hHostRxDone;
                    m_pIPCInfoList[dwLoop]->hClientTxDone = pIPCinfo->hClientTxDone;
                    m_pIPCInfoList[dwLoop]->hClientRxDone = pIPCinfo->hClientRxDone;
                    ///m_pIPCInfoList[dwLoop]->pIPCobject = pIPCinfo->pIPCobject;
                } else {
                    delete pIPC;
                    IPCDBGMSG( dFAIL, ( L"%s:SIPCList::AddItem map IPC info fail!\r\n", DBGMSGPREFIX) );
                    bRet = FALSE;
                }    
            }
            
            if( bRet )
                m_dwCurrentListSize++;

            IPCDBGMSG( dCLAS, ( L"%s:SIPCList::AddItem host %s\r\n", DBGMSGPREFIX, bRet?L"ok":L"fail") );
            break;
        }
    }   ///for( dwLoop = 0; dwLoop < IPCLISTMAXSIZE; dwLoop++ )

    IPCDBGMSG( dCLAS, ( L"%s:SIPCList::AddItem %s !\r\n", DBGMSGPREFIX, bRet?L"ok":L"fail") );
    return bRet;
}

BOOL SIPCList::DeleteItem( SPIPC_INFO ipcInfo )
{
    BOOL bRet = FALSE;
    SPIPC_INFO *pThisIPCinfo = NULL;
    DWORD dwLoop;
    
    if( ipcInfo.dwIndex != 0x00000000 ) {

        for( dwLoop = 0; dwLoop < IPCLISTMAXSIZE; dwLoop++ ) {
            if( m_pIPCInfoList[dwLoop] != NULL ) {
                if( (m_pIPCInfoList[dwLoop])->dwIndex == ipcInfo.dwIndex ) {
                    pThisIPCinfo = m_pIPCInfoList[dwLoop];
                    m_pIPCInfoList[dwLoop] = NULL;  //clear in list
                    m_dwCurrentListSize--;
                    break;
                }
            }
        }

    } else {
        IPCDBGMSG( dCLAS, ( L"%s:SIPCList::DeleteItem invalid index=0x%08x !\r\n", DBGMSGPREFIX, ipcInfo.dwIndex) );
    }
        
    if( pThisIPCinfo != NULL ) {        
        delete pThisIPCinfo;    ///delete memery
        bRet = TRUE;
    } else {
        IPCDBGMSG( dCLAS, ( L"%s:SIPCList::DeleteItem not found index=0x%08x !\r\n", DBGMSGPREFIX, ipcInfo.dwIndex) );
    }
        
    IPCDBGMSG( dCLAS, ( L"%s:SIPCList::DeleteItem %s\r\n", DBGMSGPREFIX, bRet?L"ok":L"fail") );
    return bRet;
}

SPIPC_INFO* SIPCList::GetInfoWithIndex( DWORD dwIndex )
{
    BOOL bRet = FALSE;
    SPIPC_INFO *pIPCinfo = NULL;
    DWORD dwLoop;
    
    for( dwLoop = 0; dwLoop < IPCLISTMAXSIZE; dwLoop++ ) {
        if( m_pIPCInfoList[dwLoop] != NULL ) {
            if( (m_pIPCInfoList[dwLoop])->dwIndex == dwIndex ) {
                pIPCinfo = m_pIPCInfoList[dwLoop];
                bRet = TRUE;
                break;
            }
        }
    }
    
    IPCDBGMSG( dCLAS, ( L"%s:SIPCList::GetInfoWithIndex %s\r\n", DBGMSGPREFIX, bRet?L"ok":L"fail") );
    return pIPCinfo;
}

void* SIPCList::GetIPCWithIndex( DWORD dwIndex )
{
    BOOL bRet = FALSE;
    void *pIPC = NULL;
    DWORD dwLoop;
    
    for( dwLoop = 0; dwLoop < IPCLISTMAXSIZE; dwLoop++ ) {
        if( m_pIPCInfoList[dwLoop] != NULL ) {
            if( (m_pIPCInfoList[dwLoop])->dwIndex == dwIndex ) {
                pIPC = m_pIPCInfoList[dwLoop]->pIPCobject;
                bRet = TRUE;
                break;
            }
        }
    }
    
    IPCDBGMSG( dCLAS, ( L"%s:SIPCList::GetIPCWithIndex %s\r\n", DBGMSGPREFIX, bRet?L"ok":L"fail") );
    return pIPC;
}

DWORD SIPCList::CurrentListSize()
{
    return m_dwCurrentListSize;
}

/// SIPC ///////////////////////////////////////////////////////////////////////
SIPC::SIPC()
{
    SIPCInit( IPCDEFAULTBULKBYTESIZE );
}

SIPC::SIPC( DWORD dwBulkSize )
{
    SIPCInit( dwBulkSize );
}

SIPC::~SIPC()
{
    
}


BOOL SIPC::SIPCInit( DWORD dwBulkSize )
{
    BOOL bRet = FALSE;

    m_IPCinfo.dwBulkSize = dwBulkSize;
    ///init event handle
    m_IPCinfo.hHostTxDone = CreateEvent(NULL, FALSE, FALSE, NULL);    ///handle for host Tx ok, reserved for IPC
    m_IPCinfo.hHostRxDone = CreateEvent(NULL, FALSE, FALSE, NULL);    ///handle for host Rx ok, reserved for host
    m_IPCinfo.hClientTxDone = CreateEvent(NULL, FALSE, FALSE, NULL);  ///handle for client Tx ok, reserved for IPC
    m_IPCinfo.hClientRxDone = CreateEvent(NULL, FALSE, FALSE, NULL);  ///handle for clinet Rx ok, reserved for client
    m_IPCinfo.pIPCobject = this;      ///object pointer
    
    
    if( 
        m_IPCinfo.dwBulkSize == 0 ||
        m_IPCinfo.hHostTxDone == NULL ||
        m_IPCinfo.hHostRxDone == NULL ||
        m_IPCinfo.hClientTxDone == NULL ||
        m_IPCinfo.hClientRxDone == NULL ||
        m_IPCinfo.pIPCobject == NULL
    )
        bRet = FALSE;
    else
        bRet = TRUE;                                    
    
    if( bRet ) {
        ///init FIFO
        pFIFO = NULL;
        pFIFO = new SFInFOut( m_IPCinfo.dwBulkSize );
        hFIFOFilled = CreateEvent(NULL, FALSE, FALSE, NULL);  ///handle for FIFO data filled event
        
        ///init memery map file
        if( NULL != pFIFO ) {
            HINSTANCE hiCoreDll = NULL; 

            gpfnCreateFileMapping = NULL;
            gpfnMapViewOfFile = NULL;
            gpfnUnmapViewOfFile = NULL;
            // get pointers to file-mapping functions
            hiCoreDll = LoadLibrary(_T("coredll.dll"));
            if(hiCoreDll != NULL) {
                gpfnCreateFileMapping = (PFN_CreateFileMapping) GetProcAddress((HMODULE) hiCoreDll, _T("CreateFileMappingW"));
                gpfnMapViewOfFile = (PFN_MapViewOfFile) GetProcAddress((HMODULE) hiCoreDll, _T("MapViewOfFile"));
                gpfnUnmapViewOfFile = (PFN_UnmapViewOfFile) GetProcAddress((HMODULE) hiCoreDll, _T("UnmapViewOfFile"));
            }
        }
        
        if( 
            gpfnCreateFileMapping == NULL ||
            gpfnMapViewOfFile == NULL ||
            gpfnUnmapViewOfFile == NULL
        )
        {
            IPCDBGMSG( dFAIL, ( L"%s:SIPCInit::file-mapping functions 0x%08x 0x%08x 0x%08x fail !\r\n", DBGMSGPREFIX, gpfnCreateFileMapping, gpfnMapViewOfFile, gpfnUnmapViewOfFile) );
        } else {
            hMemFile = NULL;
   	        hMemFile = gpfnCreateFileMapping( (HANDLE)INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, m_IPCinfo.dwBulkSize, NULL );
   	        pbMemFile = NULL;
            pbMemFile = (PBYTE)gpfnMapViewOfFile(hMemFile, FILE_MAP_ALL_ACCESS, 0, 0, m_IPCinfo.dwBulkSize);
        }
        
        
        ///init host Tx FIFO thread
        if( hMemFile != NULL && pbMemFile != NULL ) {
            hHostTxThread = CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)HostTxThreadStub, m_IPCinfo.pIPCobject, 0, NULL);
        } else {
            IPCDBGMSG( dFAIL, ( L"%s:SIPCInit::CreateFileMapping fail !\r\n", DBGMSGPREFIX) );
        }

        
    }
    
    IPCDBGMSG( dCLAS, ( L"%s:SIPC::SIPCInit dwBulkSize=%d hHostTxDone=0x%08x hHostRxDone=0x%08x hClientTxDone=0x%08x hClientRxDone=0x%08x pIPCobject=0x%08x!\r\n"
    , DBGMSGPREFIX, m_IPCinfo.dwBulkSize, m_IPCinfo.hHostTxDone, m_IPCinfo.hHostRxDone, m_IPCinfo.hClientTxDone, m_IPCinfo.hClientRxDone, m_IPCinfo.pIPCobject) );    
    
    return bRet;
}

SPIPC_INFO* SIPC::GetIPCinfo()
{
    return &m_IPCinfo;
}

BOOL SIPC::IPCHostTx( PBYTE pbDataTx, DWORD dwDataLenght )
{
    BOOL bRet = FALSE;
    
    ///put it FIFO
    if( NULL != pFIFO ) {
        bRet = pFIFO->Copy2FIFO( pbDataTx, dwDataLenght );
    }
    
    IPCDBGMSG( dDBUG, ( L"%s:IPCHostTx:: %d data %s !\r\n", DBGMSGPREFIX, dwDataLenght, (bRet)?L"ok":L"fail") );
    return bRet;
}

DWORD SIPC::IPCHostRx( PBYTE pbBufferRx, DWORD dwBufferLenght )
{
    DWORD dwRet = 0;
    
    IPCDBGMSG( dDBUG, ( L"%s:IPCHostRx:: %d data %s !\r\n", DBGMSGPREFIX, dwRet, (dwRet>0)?L"ok":L"fail") );
    return dwRet;
}

BOOL SIPC::IPCClientTx( PBYTE pbDataTx, DWORD dwDataLenght )
{
    BOOL bRet = FALSE;
    
    IPCDBGMSG( dDBUG, ( L"%s:IPCClientTx:: %d data %s !\r\n", DBGMSGPREFIX, dwDataLenght, (bRet)?L"ok":L"fail") );
    return bRet;
}

DWORD SIPC::IPCClientRx( PBYTE pbBufferRx, DWORD dwBufferLenght )
{
    BOOL bRet = FALSE;
    DWORD dwRet = 0;
    
    ///get from memery map file
    bRet = MemMapFile2Out( pbBufferRx, dwBufferLenght );
    
    ///signal done 
    if( bRet )
        SetEvent( m_IPCinfo.hClientRxDone );
    else
        dwRet = m_IPCinfo.dwBulkSize;
    
    IPCDBGMSG( dDBUG, ( L"%s:IPCClientRx:: %d data %s !\r\n", DBGMSGPREFIX, dwRet, (dwRet>0)?L"ok":L"fail") );
    return dwRet;
}

DWORD SIPC::HostTxThreadStub( LPVOID lpvParam )
{
    SIPC *pIPC = (SIPC *)lpvParam;
    DWORD dwStatus = pIPC->HostTxThread();
    return dwStatus;
}

DWORD SIPC::HostTxThread( void )
{
    
#define     IPC_Tx_CONTROL_EVENTS_NUM   2

    DWORD dwRet = 0;
    BOOL bRet = FALSE;
    BOOL bMemFileFilled = FALSE;
    HANDLE hTxHandles[IPC_Tx_CONTROL_EVENTS_NUM];
    DWORD dwWaitRet = 0;
    ///DWORD dwTimeout = INFINITE;
    DWORD dwTimeout = 2999;     ///check every 2.999 sec, backup !


    hTxHandles[0] = hFIFOFilled;
    hTxHandles[1] = m_IPCinfo.hClientRxDone;


    IPCDBGMSG( dCLAS, ( L"%s:SIPC::HostTxThread start +++ \r\n", DBGMSGPREFIX) );
    
    ///this thrad will take care the data transfer form Tx FIFO to Memery map file, and signal event
    while( 1 ) {
        
        dwWaitRet = WaitForMultipleObjects( IPC_Tx_CONTROL_EVENTS_NUM, hTxHandles, FALSE, dwTimeout );

        switch ( dwWaitRet )
        {
            case (WAIT_OBJECT_0 + 0):
                IPCDBGMSG( dCLAS, ( L"%s:SIPC::HostTxThread 1\r\n", DBGMSGPREFIX) );
                ///new data in FIFO, move to memery file
                if( bMemFileFilled )
                    break;
                    
                dwRet = FIFO2MemMapFile();
                if( 0xFFFFFFFF != dwRet ) {
                    bMemFileFilled = TRUE;
                    SetEvent( m_IPCinfo.hHostTxDone );
                } else {
                    ///FIFO error !
                    bMemFileFilled = FALSE;
                }
                
                break;
            case (WAIT_OBJECT_0 + 1):
                IPCDBGMSG( dCLAS, ( L"%s:SIPC::HostTxThread 2\r\n", DBGMSGPREFIX) );
                ///Client Rx done
                dwRet = FIFO2MemMapFile();
                if( 0xFFFFFFFF != dwRet ) {
                    bMemFileFilled = TRUE;
                    SetEvent( m_IPCinfo.hHostTxDone );
                } else {
                    ///FIFO empty !
                    bMemFileFilled = FALSE;
                }
                
                break;
            case (WAIT_TIMEOUT):
                IPCDBGMSG( dCLAS, ( L"%s:SIPC::HostTxThread 3\r\n", DBGMSGPREFIX) );
                ///check if data in FIFO
                if( pFIFO->GetCurFIFODepth() > 0 ) {
                    dwRet = FIFO2MemMapFile();
                    if( 0xFFFFFFFF != dwRet ) {
                        bMemFileFilled = TRUE;
                        SetEvent( m_IPCinfo.hHostTxDone );
                    } else {
                        ///FIFO error !
                        bMemFileFilled = FALSE;
                    }              
                } else
                    bMemFileFilled = FALSE;
                    
                break;
            default : 
                IPCDBGMSG( dFAIL, ( L"%s:SIPC::HostTxThread switch default !!!\r\n", DBGMSGPREFIX) );
                
                break;                  
        }   ///switch
        
        
    }
    
    IPCDBGMSG( dCLAS, ( L"%s:SIPC::HostTxThread end ---\r\n", DBGMSGPREFIX) );
    return dwRet;
}


DWORD SIPC::FIFO2MemMapFile()
{
    BOOL bRet = FALSE;
    DWORD dwRet = 0;
    
    if( NULL != pFIFO ) {
        ///copy FIFO to memery file        
        dwRet = pFIFO->FIFOCopy2( pbMemFile, m_IPCinfo.dwBulkSize );

        if( 0 == dwRet ) {
            IPCDBGMSG( dFAIL, ( L"%s:SIPC::FIFO2MemMapFile FIFOCopy2 fail! \r\n", DBGMSGPREFIX) );
            bRet = FALSE;
         } else {
            bRet = TRUE;
            dwRet = pFIFO->GetCurFIFODepth();
        }    
    }
    
    if( !bRet )
        dwRet = 0xFFFFFFFF;

    return dwRet;
}


BOOL SIPC::MemMapFile2Out( PBYTE pBuffer, DWORD dwBufferSize )
{
    BOOL bRet = FALSE;
    
    if( dwBufferSize >= m_IPCinfo.dwBulkSize ) {
        memcpy( pBuffer, pbMemFile, m_IPCinfo.dwBulkSize );
        bRet = TRUE;
    } else {
        IPCDBGMSG( dFAIL, ( L"%s:SIPC::MemMapFile2Out invalid buffer size! \r\n", DBGMSGPREFIX) );   
    }
    
    return bRet;    
}