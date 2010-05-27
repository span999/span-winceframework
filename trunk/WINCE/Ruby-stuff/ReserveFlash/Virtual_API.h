///
///
#ifndef __VIRTAUL_API_H__
#define __VIRTAUL_API_H__


/// =============================================================================
/// =============================================================================

#define VDBG_DRIVER_HANDLE				TEXT("DBG1:")
#define VDBG_MESSAGE_MAX_SIZE			128
#define MAX_PORT_LIST					6

#define IOCTL_OEM_VDBG_PORT_INIT		CTL_CODE(FILE_DEVICE_HAL,4230,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define IOCTL_OEM_VDBG_PORT_INIT_NAME	CTL_CODE(FILE_DEVICE_HAL,4231,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define IOCTL_OEM_VDBG_PORT_DEINIT		CTL_CODE(FILE_DEVICE_HAL,4232,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define IOCTL_OEM_VDBG_PORT_OPEN		CTL_CODE(FILE_DEVICE_HAL,4233,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define IOCTL_OEM_VDBG_PORT_LIST		CTL_CODE(FILE_DEVICE_HAL,4234,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define IOCTL_OEM_VDBG_PORT_GETDATA		CTL_CODE(FILE_DEVICE_HAL,4235,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define IOCTL_OEM_VDBG_PORT_GETIDNAME	CTL_CODE(FILE_DEVICE_HAL,4236,METHOD_BUFFERED,FILE_ANY_ACCESS)
/// =============================================================================
/// =============================================================================

typedef struct {
    DWORD PortContext;
    BOOL  (*pfnWrite)(DWORD dwPort, TCHAR* pData);
    DWORD (*pfnRead)(DWORD dwPort, TCHAR* pData);
} DEVICE_IFC_DBG;




#endif ///#ifndef __VIRTAUL_API_H__


