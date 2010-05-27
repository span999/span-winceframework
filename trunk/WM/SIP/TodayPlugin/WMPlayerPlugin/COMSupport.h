////////////////////////////////////////////////////////////
// Windows Media Player / Homescreen Plugins for Windows CE
//
//
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
//
// Use of this sample source code is subject to the terms of the Microsoft 
// license agreement under which you licensed this sample source code and is 
// provided AS-IS.  If you did not accept the terms of the license agreement, you 
// are not authorized to use this sample source code.  For the terms of the 
// license, please see the license agreement between you and Microsoft.
//
//
////////////////////////////////////////////////////////////

#ifndef _WMPPLUGINCOMSUPPORT_H_
#define _WMPPLUGINCOMSUPPORT_H_

////////////////////////////////////////////////////////////
// CThreadSafeULONG class
//
// This class simply 'wraps' a ULONG value so that it can be incremented and
// decremented in a thread-safe manner, since our components are free-threaded.
////////////////////////////////////////////////////////////

class CThreadSafeULONG
{
public:
    CThreadSafeULONG(ULONG ulInitialValue) { m_ulValue = ulInitialValue; }

    ULONG Increment() { return static_cast<ULONG>(InterlockedIncrement(reinterpret_cast<LONG *>(&m_ulValue))); }
    ULONG Decrement() { return static_cast<ULONG>(InterlockedDecrement(reinterpret_cast<LONG *>(&m_ulValue))); }
    ULONG CurrentValue() { return m_ulValue; }

private:
    ULONG m_ulValue;
};

////////////////////////////////////////////////////////////
// CServer class
//
// This class represents a COM server as a whole, though it's
// really only concerned with server lifetime according to the
// COM rules.
//
// This class could be subclassed to add additional functionality
// if desired.
////////////////////////////////////////////////////////////

class CServer
{
public:
    ////////////////////////////////////////
    // Constructor
    ////////////////////////////////////////
    CServer()
        : m_cLocks(0)
    {
    }

    ////////////////////////////////////////
    // The Lock and Unlock methods are used to increment or decrement
    // the server lock count so that we can properly implement the
    // exported function DllCanUnloadNow.
    ////////////////////////////////////////
    VOID Lock() { m_cLocks.Increment(); }
    VOID Unlock() { m_cLocks.Decrement(); }

    ////////////////////////////////////////
    // CanUnload returns TRUE if the server can unload (lock count == 0)
    // or FALSE if it can't (lock count > 0). Again, this is used to
    // support DllCanUnloadNow.
    ////////////////////////////////////////
    BOOL CanUnload() { return m_cLocks.CurrentValue() == 0; }

private:
    ////////////////////////////////////////
    // Data
    ////////////////////////////////////////
    CThreadSafeULONG    m_cLocks;   // Thread-safe lock count
};


////////////////////////////////////////////////////////////
// DllRegisterServerImplementation
//
// A straightforward implementation of DllRegisterServer --
// just pass in your CLSID string, a description string, the
// absolute path of your module, and the ThreadingModel string.
// If you don't want a ThreadingModel entry, pass NULL for
// that argument.
////////////////////////////////////////////////////////////

HRESULT DllRegisterServerImplementation(LPCTSTR szCLSID, LPCTSTR szDescription, LPCTSTR szModulePath, LPCTSTR szThreadingModel)
{
    HRESULT     hr = E_FAIL;
    DWORD       dwDisposition;
    HKEY        hkeyCLSID = NULL, hkeyCLSIDClass = NULL, hkeyInprocServer = NULL;

    // Open the CLSID key.
    if (RegOpenKeyEx(HKEY_CLASSES_ROOT, _T("CLSID"), 0, 0, &hkeyCLSID) != ERROR_SUCCESS)
        goto Cleanup;

    // Create our subkey.
    if (RegCreateKeyEx( hkeyCLSID,
                        szCLSID,
                        0,
                        NULL,
                        0,
                        0,
                        NULL,
                        &hkeyCLSIDClass,
                        &dwDisposition) != ERROR_SUCCESS)
        goto Cleanup;

    // Set its default value (a description of this component).
    if (RegSetValueEx(  hkeyCLSIDClass,
                        _T(""),
                        0,
                        REG_SZ,
                        (BYTE *)szDescription,
                        sizeof(TCHAR) * (_tcslen(szDescription) + 1)) != ERROR_SUCCESS)
        goto Cleanup;

    // Create its InprocServer32 subkey.
    if (RegCreateKeyEx( hkeyCLSIDClass,
                        _T("InprocServer32"),
                        0,
                        NULL,
                        0,
                        0,
                        NULL,
                        &hkeyInprocServer,
                        &dwDisposition) != ERROR_SUCCESS)
        goto Cleanup;

    // Set the default value for the InprocServer32 subkey.
    if (RegSetValueEx(  hkeyInprocServer,
                        _T(""),
                        0,
                        REG_SZ,
                        (BYTE *)szModulePath,
                        sizeof(TCHAR) * (_tcslen(szModulePath) + 1)) != ERROR_SUCCESS)
        goto Cleanup;

    // Set the value of its "ThreadingModel" subkey.
    if (szThreadingModel != NULL)
    {
        if (RegSetValueEx(  hkeyInprocServer,
                            _T("ThreadingModel"),
                            0,
                            REG_SZ,
                            (BYTE *)szThreadingModel,
                            sizeof(TCHAR) * (_tcslen(szThreadingModel) + 1)) != ERROR_SUCCESS)
            goto Cleanup;
    }

    // Success.
    hr = S_OK;    

Cleanup:
    if (hkeyInprocServer != NULL)
        RegCloseKey(hkeyInprocServer);

    if (hkeyCLSIDClass != NULL)
        RegCloseKey(hkeyCLSIDClass);

    if (hkeyCLSID != NULL)
        RegCloseKey(hkeyCLSID);

    return(hr);
}

#endif // _WMPPLUGINCOMSUPPORT_H_
