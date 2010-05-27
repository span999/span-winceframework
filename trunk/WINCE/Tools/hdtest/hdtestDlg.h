// hdtestDlg.h : header file
//

#if !defined(AFX_HDTESTDLG_H__5816431B_DBE9_4A1B_9C1B_FC98A83185DA__INCLUDED_)
#define AFX_HDTESTDLG_H__5816431B_DBE9_4A1B_9C1B_FC98A83185DA__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
#include "diskio.h"
/////////////////////////////////////////////////////////////////////////////
// CHdtestDlg dialog

typedef struct
{
    DWORD dwCmd;
    DWORD dwParm;
} BrightnessCmdParm, *PBrightnessCmdParm;
class CHdtestDlg : public CDialog
{
// Construction
public:
	void Output(DWORD arg1,DWORD arg2,int id);
	void PowerOnHarddisk();
	BOOL SDMMC_Read(SG_REQ *sg);
	BOOL SDMMC_Write(SG_REQ *sg);
	void PowerOffHarddisk();
	HANDLE hRead;
	CHdtestDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CHdtestDlg)
	enum { IDD = IDD_HDTEST_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHdtestDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CHdtestDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnButton1();
	afx_msg void OnButton2();
	afx_msg void OnButton3();
	afx_msg void OnButton4();
	afx_msg void OnButton5();
	afx_msg void OnButton6();
	afx_msg void OnButton7();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft eMbedded Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HDTESTDLG_H__5816431B_DBE9_4A1B_9C1B_FC98A83185DA__INCLUDED_)
