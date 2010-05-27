// hdtest.h : main header file for the HDTEST application
//

#if !defined(AFX_HDTEST_H__82557A2D_0559_46AE_8987_6B2E2781907E__INCLUDED_)
#define AFX_HDTEST_H__82557A2D_0559_46AE_8987_6B2E2781907E__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CHdtestApp:
// See hdtest.cpp for the implementation of this class
//

class CHdtestApp : public CWinApp
{
public:
	CHdtestApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHdtestApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CHdtestApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft eMbedded Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HDTEST_H__82557A2D_0559_46AE_8987_6B2E2781907E__INCLUDED_)
