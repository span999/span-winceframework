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
// WMPlayerPlugin.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "COMsupport.h"


////////////////////////////////////////////////////////////
// CLSID_WMPPlugin
//
// This is the class ID for our plugin.
////////////////////////////////////////////////////////////

//
// IMPORTANT:
// You must generate your own GUID if you create your own plug-in or unexpected results may occur
// 
// Be sure the GUID in the DllRegisterServer() function at the end of this file matches this GUID.  Also 
// be sure to update the WMPlayerPluginCab installation program with the new GUID under the 
// HKEY_LOCAL_MACHINE\Software\Microsoft\MediaPlay\UIPlugin\{GUID} key
//
// {009B9B8A-5080-4d09-8F74-9BD96A3558D4}
DEFINE_GUID(CLSID_WMPlayerPlugin, 0x9b9b8a, 0x5080, 0x4d09, 0x8f, 0x74, 0x9b, 0xd9, 0x6a, 0x35, 0x58, 0xd4);

////////////////////////////////////////////////////////////
// Global CServer instance
////////////////////////////////////////////////////////////

CServer * g_pServer;

////////////////////////////////////////////////////////////
// Constant strings
////////////////////////////////////////////////////////////

static const TCHAR s_szRegDescription[] = _T("Windows Media Player Plugin - Controlled by WMPStatusPlugin");
static const TCHAR s_szModuleName[] = _T("WMPlayerPlugin.dll");

////////////////////////////////////////////////////////////
// CMediaPlayerPluginClassFactory class
//
// This is the 'class factory' (class implementing IClassFactory)
// for our plugin object.
////////////////////////////////////////////////////////////

class CMediaPlayerPluginClassFactory:
    public IClassFactory
{
public:
    ////////////////////////////////////////
    // Constructor, destructor
    ////////////////////////////////////////
    CMediaPlayerPluginClassFactory()
        : m_cRefs(0)
    {
        g_pServer->Lock();
    }

    ~CMediaPlayerPluginClassFactory()
    {
        g_pServer->Unlock();
    }

    ////////////////////////////////////////
    // IUnknown implementation (for IClassFactory)
    ////////////////////////////////////////
    STDMETHODIMP_(ULONG) AddRef()
    {
        return m_cRefs.Increment();
    }

    STDMETHODIMP_(ULONG) Release()
    {
        ULONG   cRefsNew = m_cRefs.Decrement();

        if (cRefsNew == 0)
            delete this;

        return cRefsNew;
    }

    STDMETHODIMP QueryInterface(REFIID riid, VOID ** ppvObject);

    ////////////////////////////////////////
    // IClassFactory implementation
    ////////////////////////////////////////
    STDMETHODIMP CreateInstance(IUnknown * pUnkOuter, REFIID riid, VOID ** ppvObject);
    STDMETHODIMP LockServer(BOOL fLock);

private:
    ////////////////////////////////////////
    // Data
    ////////////////////////////////////////
    CThreadSafeULONG    m_cRefs;    // Thread-safe reference count
};

////////////////////////////////////////////////////////////
// CMediaPlayerPlugin
//
// This is our actual plugin component which implements
// IWMPPluginUI.
////////////////////////////////////////////////////////////

class CMediaPlayerPlugin:
    public IWMPPluginUI
{
public:
    ////////////////////////////////////////
    // Constructor, destructor
    ////////////////////////////////////////
    CMediaPlayerPlugin()
        : m_cRefs(0),
          m_hwndComm(NULL),
          m_pCore(NULL)
    {
        g_pServer->Lock();
    }

    ~CMediaPlayerPlugin()
    {

        if (m_pCore != NULL)
            m_pCore->Release();

        if (m_hwndComm != NULL)
        {
            DestroyWindow(m_hwndComm);
            UnregisterClass(SZ_MEDIAPLAYERPLUGIN_COMMWINDOW_CLASSNAME, NULL);
        }

        g_pServer->Unlock();
    }

    ////////////////////////////////////////
    // InternalAddRef and InternalRelease
    //
    // Internal versions of AddRef and Release, mimicking ATL.
    //
    // See our class factory's CreateInstance method.
    ////////////////////////////////////////
    VOID InternalAddRef()
    {
        m_cRefs.Increment();
    }

    VOID InternalRelease()
    {
        m_cRefs.Decrement();
    }

    ////////////////////////////////////////
    // FinalConstruct method (mimicking ATL).
    //
    // This is called by our class factory's CreateInstance method to allow
    // us to perform additional initialization and possibly return an error
    // code (which isn't easily done with just the constructor).
    //
    // This is where we create our hidden communication window.
    ////////////////////////////////////////
    HRESULT FinalConstruct();

    ////////////////////////////////////////
    // WndProcs
    //
    // We have a static version that's our official WndProc; this method looks
    // up our 'this' pointer in window data and then calls our instance WndProc.
    ////////////////////////////////////////
    static LRESULT CALLBACK StaticWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    LRESULT WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    ////////////////////////////////////////
    // IUnknown implementation (for IWMPPluginUI)
    ////////////////////////////////////////
    STDMETHODIMP_(ULONG) AddRef()
    {
        return m_cRefs.Increment();
    }

    STDMETHODIMP_(ULONG) Release()
    {
        ULONG   cRefsNew = m_cRefs.Decrement();

        if (cRefsNew == 0)
            delete this;

        return cRefsNew;
    }

    STDMETHODIMP QueryInterface(REFIID riid, VOID ** ppvObject);

    ////////////////////////////////////////
    // IWMPPluginUI implementation
    ////////////////////////////////////////
    STDMETHODIMP Create(HWND hwndParent, HWND * phwndWindow);
    STDMETHODIMP Destroy();
    STDMETHODIMP DisplayPropertyPage(HWND hwndParent);
    STDMETHODIMP GetProperty(LPCWSTR wszName, VARIANT * pvarProperty);
    STDMETHODIMP SetCore(IWMPCore * pCore);
    STDMETHODIMP SetProperty(LPCWSTR wszName, const VARIANT * pvarProperty);
    STDMETHODIMP TranslateAccelerator(MSG * pMsg);


private:
    ////////////////////////////////////////
    // Implementation methods
    ////////////////////////////////////////
    VOID ControlPlayer(UINT uMsg);


    ////////////////////////////////////////
    // Data
    ////////////////////////////////////////
    CThreadSafeULONG    m_cRefs;            // Thread-safe reference count
    HWND                m_hwndComm;         // Our hidden communication window
    IWMPCore *          m_pCore;            // Cached interface pointer for communicating with Windows Media Player
};

////////////////////////////////////////////////////////////
// CMediaPlayerPluginClassFactory::QueryInterface
////////////////////////////////////////////////////////////

HRESULT CMediaPlayerPluginClassFactory::QueryInterface(REFIID riid, VOID ** ppvObject)
{
    *ppvObject = NULL;

    if (IsEqualIID(riid, __uuidof(IUnknown)))
        *ppvObject = static_cast<IUnknown *>(static_cast<IClassFactory *>(this));
    else if (IsEqualIID(riid, __uuidof(IClassFactory)))
        *ppvObject = static_cast<IClassFactory *>(this);

    if (*ppvObject == NULL)
        return E_NOINTERFACE;

    AddRef();

    return S_OK;
}

////////////////////////////////////////////////////////////
// CMediaPlayerPluginClassFactory::CreateInstance
////////////////////////////////////////////////////////////

HRESULT CMediaPlayerPluginClassFactory::CreateInstance(IUnknown * pUnkOuter, REFIID riid, VOID ** ppvObject)
{
    HRESULT                 hr;
    CMediaPlayerPlugin *    pPlugin;

    // Assume failure.
    *ppvObject = NULL;

    // We don't support aggregation.
    if (pUnkOuter != NULL)
        return CLASS_E_NOAGGREGATION;

    // Create the plugin instance.
    if ((pPlugin = new CMediaPlayerPlugin()) == NULL)
        return E_OUTOFMEMORY;

    // Call the plugin's FinalConstruct method to allow it to perform
    // additional initialization.
    //
    // Once again mimicking ATL's behavior, we first artifically increment
    // its ref count before calling FinalConstruct and then artificially
    // decrement it afterward. Why? Because we don't know what the plugin
    // will do in its FinalConstruct method. It could (in theory) hand its
    // interface pointer to some _other_ object, which might AddRef it, use
    // it, and then Release it (according to the standard COM rules). However,
    // because the object currently has a reference count of 0, the hypothetical
    // AddRef would bump it to 1, and the Release would drop it to 0 and destroy
    // the object!
    //
    // When we do this artifical "lifetime support", though, we can't use the
    // IUnknown AddRef and Release methods because we're likely to end up deleting
    // the object for exactly the same reason (its ref count might go to 0 on the
    // Release call). So instead, we use the InternalAddRef and InternalRelease
    // methods. The InternalRelease method, in particular, doesn't check for a
    // resulting ref count of 0 (and thus will never delete the instance).
    pPlugin->InternalAddRef();
    hr = pPlugin->FinalConstruct();
    pPlugin->InternalRelease();

    if (FAILED(hr))
    {
        delete pPlugin;
        return hr;
    }

    // Make sure the plugin object supports the requested interface. Yes, we
    // could probably do this _before_ the FinalConstruct call, thus avoiding
    // the whole InternalAddRef/Release stuff, but only now has the object
    // been properly constructed.
    if (FAILED(hr = pPlugin->QueryInterface(riid, ppvObject)))
    {
        delete pPlugin;
        return hr;
    }

    // The plugin object should have a reference count of at least 1, courtesy
    // of the call to QueryInterface.

    return S_OK;
}

////////////////////////////////////////////////////////////
// CMediaPlayerPluginClassFactory::LockServer
////////////////////////////////////////////////////////////

HRESULT CMediaPlayerPluginClassFactory::LockServer(BOOL fLock)
{
    if (fLock)
        g_pServer->Lock();
    else
        g_pServer->Unlock();

    return S_OK;
}

////////////////////////////////////////////////////////////
// CMediaPlayerPlugin::QueryInterface
////////////////////////////////////////////////////////////

HRESULT CMediaPlayerPlugin::QueryInterface(REFIID riid, VOID ** ppvObject)
{
    *ppvObject = NULL;

    if (IsEqualIID(riid, __uuidof(IUnknown)))
        *ppvObject = static_cast<IUnknown *>(static_cast<IWMPPluginUI *>(this));
    else if (IsEqualIID(riid, __uuidof(IWMPPluginUI)))
        *ppvObject = static_cast<IWMPPluginUI *>(this);

    if (*ppvObject == NULL)
        return E_NOINTERFACE;

    AddRef();

    return S_OK;
}

////////////////////////////////////////////////////////////
// CMediaPlayerPlugin::FinalConstruct
////////////////////////////////////////////////////////////

HRESULT CMediaPlayerPlugin::FinalConstruct()
{
    WNDCLASS    wc;

    // As stated in the class declaration, here's where we create our
    // hidden communication window.
    //
    // First, of course, we have to register our window class.
    wc.style = 0;
    wc.lpfnWndProc = StaticWndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = NULL;
    wc.hIcon = NULL;
    wc.hCursor = NULL;
    wc.hbrBackground = NULL;
    wc.lpszMenuName = NULL;
    wc.lpszClassName = SZ_MEDIAPLAYERPLUGIN_COMMWINDOW_CLASSNAME;

    if (RegisterClass(&wc) == 0)
        return HRESULT_FROM_WIN32(GetLastError());

    // Now create the window.
    m_hwndComm = CreateWindow(  SZ_MEDIAPLAYERPLUGIN_COMMWINDOW_CLASSNAME,
                                SZ_MEDIAPLAYERPLUGIN_COMMWINDOW_WINDOWNAME,
                                WS_OVERLAPPED,      // style (note: does NOT include WS_VISIBLE)
                                0,                  // x
                                0,                  // y
                                1,                  // width
                                1,                  // height
                                NULL,               // parent window handle
                                NULL,               // menu
                                NULL,               // instance handle
                                NULL);              // parameter

    if (m_hwndComm == NULL)
    {
        UnregisterClass(SZ_MEDIAPLAYERPLUGIN_COMMWINDOW_CLASSNAME, NULL);
        return HRESULT_FROM_WIN32(GetLastError());
    }

    // Add our 'this' pointer to the window data.
    SetWindowLong(m_hwndComm, GWL_USERDATA, reinterpret_cast<LONG>(this));

    return S_OK;
}

////////////////////////////////////////////////////////////
// CMediaPlayerPlugin::StaticWndProc
////////////////////////////////////////////////////////////

/*static*/ LRESULT CALLBACK CMediaPlayerPlugin::StaticWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    CMediaPlayerPlugin *   pThis;

    // Fetch our 'this' pointer from window data.
    //
    // NOTE: Since we don't SET this value until AFTER the window is created
    // (see FinalConstruct), we're not guaranteed to have this value yet. That
    // is, AS the window is being constructed, we'll get some set of messages
    // here and we won't yet have our 'this' pointer. Therefore, just punt those
    // to DefWindowProc.
    if ((pThis = reinterpret_cast<CMediaPlayerPlugin *>(GetWindowLong(hwnd, GWL_USERDATA))) == NULL)
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    else
        return pThis->WndProc(hwnd, uMsg, wParam, lParam);
}

////////////////////////////////////////////////////////////
// CMediaPlayerPlugin::WndProc
////////////////////////////////////////////////////////////

LRESULT CMediaPlayerPlugin::WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        // we have received the toggle message from the Today Screen plug-in
        case WM_WMPTOGGLE:
            ControlPlayer(uMsg);
            break;

        default:
            break;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}


////////////////////////////////////////////////////////////
// CMediaPlayerPlugin::Create
////////////////////////////////////////////////////////////

HRESULT CMediaPlayerPlugin::Create(HWND hwndParent, HWND * phwndWindow)
{
    // As per the IWMPPluginUI documentation, this method will never be called
    // because we're a background plugin (our registry Capabilities flags include
    // PLUGIN_TYPE_BACKGROUND = 0x00000001).

    return E_NOTIMPL;
}

////////////////////////////////////////////////////////////
// CMediaPlayerPlugin::Destroy
////////////////////////////////////////////////////////////

HRESULT CMediaPlayerPlugin::Destroy()
{
    return S_OK;
}

////////////////////////////////////////////////////////////
// CMediaPlayerPlugin::DisplayPropertyPage
////////////////////////////////////////////////////////////

HRESULT CMediaPlayerPlugin::DisplayPropertyPage(HWND hwndParent)
{
    // We provide no property pages, and therefore don't need to do anything here.

    return E_NOTIMPL;
}

////////////////////////////////////////////////////////////
// CMediaPlayerPlugin::GetProperty
////////////////////////////////////////////////////////////

HRESULT CMediaPlayerPlugin::GetProperty(LPCWSTR wszName, VARIANT * pvarProperty)
{
    return S_OK;
}

////////////////////////////////////////////////////////////
// CMediaPlayerPlugin::SetCore
////////////////////////////////////////////////////////////

HRESULT CMediaPlayerPlugin::SetCore(IWMPCore * pCore)
{
    // This method is called shortly after this instance is created, and allows
    // us to hook up to Windows Media Player. Save the interface pointer.
    //
    // When WMP is shutting down, it calls SetCore(NULL) so we have to handle that
    // as well.
    if (m_pCore != NULL)
    {
        m_pCore->Release();
    }

    m_pCore = pCore;

    if (m_pCore != NULL)
    {
        m_pCore->AddRef();
    }

    return S_OK;
}

////////////////////////////////////////////////////////////
// CMediaPlayerPlugin::SetProperty
////////////////////////////////////////////////////////////

HRESULT CMediaPlayerPlugin::SetProperty(LPCWSTR wszName, const VARIANT * pvarProperty)
{
    return S_OK;
}

////////////////////////////////////////////////////////////
// CMediaPlayerPlugin::TranslateAccelerator
////////////////////////////////////////////////////////////

HRESULT CMediaPlayerPlugin::TranslateAccelerator(MSG * pMsg)
{
    // We have no reason to want to respond to keystroke events.

    return S_FALSE;
}


VOID CMediaPlayerPlugin::ControlPlayer(UINT uMsg)
{
    IWMPControls *  pControls;

    // Get the 'IWMPControls' interface.
    if (SUCCEEDED(m_pCore->get_controls(&pControls)))
    {
        switch (uMsg)
        {
            // for the toggle message
            case WM_WMPTOGGLE:
                WMPPlayState wmpps;

                // get the current state
                if (SUCCEEDED(m_pCore->get_playState(&wmpps)))
                {
                  // if it playing a track, pause it
                  if (wmpps == wmppsPlaying)
                  {
                    pControls->pause();
                  }
                  // otherwise try to play the track
                  else
                  {
                    pControls->play();
                  }

                }
                break;

            default:
                break;
        }

        pControls->Release();
    }
}

////////////////////////////////////////////////////////////
// DLL-level implementation
//
// DllGetClassObject, DllCanUnloadNow and DllRegisterServer
// are exported courtesy of our .DEF file.
////////////////////////////////////////////////////////////

BOOL WINAPI DllMain(HANDLE hModule, DWORD ulReason, VOID * pvReserved)
{
    if (ulReason == DLL_PROCESS_ATTACH)
    {
        if ((g_pServer = new CServer) == NULL)
            return FALSE;
    }
    else if (ulReason == DLL_PROCESS_DETACH)
    {
        delete g_pServer;
    }

    return TRUE;
}

STDAPI DllGetClassObject(REFCLSID clsid, REFIID riid, VOID ** ppv)
{
    HRESULT                             hr;
    CMediaPlayerPluginClassFactory *    pcf;

    *ppv = NULL;

    if ((pcf = new CMediaPlayerPluginClassFactory()) == NULL)
        return E_OUTOFMEMORY;

    if (FAILED(hr = pcf->QueryInterface(riid, ppv)))
    {
        delete pcf;
        return hr;
    }

    return S_OK;
}

STDAPI DllCanUnloadNow()
{
    if (g_pServer->CanUnload())
        return S_OK;
    else
        return S_FALSE;
}

STDAPI DllRegisterServer()
{
    TCHAR   szModulePath[MAX_PATH];

    // Get our module path.
    if (!GetModuleFileName(GetModuleHandle(s_szModuleName), szModulePath, MAX_PATH))
        return E_FAIL;

    szModulePath[MAX_PATH - 1] = _T('\0');

    // register this COM object.  
    // 
    // IMPORTANT:
    // be sure to update this GUID with another if you create another plug-in.  Each plug-in must have a unique GUID.
    return DllRegisterServerImplementation(_T("{009B9B8A-5080-4d09-8F74-9BD96A3558D4}"), s_szRegDescription, szModulePath, _T("Free"));
}
