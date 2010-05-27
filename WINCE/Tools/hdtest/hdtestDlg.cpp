// hdtestDlg.cpp : implementation file
//

#include "stdafx.h"
#include "hdtest.h"
#include "hdtestDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
HANDLE hRead,hWrite,hContinueRead,g_hStore = NULL;
BOOL beContinueRead = FALSE,beReadTest = FALSE;
unsigned char buffer[512*128];
TCHAR rateBuf[32];
#define READING_TEST 	1
#define WRITTING_TEST 	2
#define CONTINUE_READ 	3
#define POWER_ON_HARDDISK 	4
#define POWER_OFF_HARDDISK 	5
TCHAR bufTest[64];

#define IOCTL_BKL_BRIGHTNESS	100
#define GETBRIGHTNESS 	11
#define SETBRIGHTNESS	12
DWORD brightness;
#define DISK_IOCTL_STANDBY \
    CTL_CODE(FILE_DEVICE_DISK, 0x800, METHOD_BUFFERED, FILE_ANY_ACCESS)
/////////////////////////////////////////////////////////////////////////////
// CHdtestDlg dialog

CHdtestDlg::CHdtestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CHdtestDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CHdtestDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CHdtestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHdtestDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CHdtestDlg, CDialog)
	//{{AFX_MSG_MAP(CHdtestDlg)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	ON_BN_CLICKED(IDC_BUTTON3, OnButton3)
	ON_BN_CLICKED(IDC_BUTTON4, OnButton4)
	ON_BN_CLICKED(IDC_BUTTON5, OnButton5)
	ON_BN_CLICKED(IDC_BUTTON6, OnButton6)
	ON_BN_CLICKED(IDC_BUTTON7, OnButton7)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHdtestDlg message handlers

BOOL CHdtestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	CenterWindow(GetDesktopWindow());	// center to the hpc screen

	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void WriteThread(CHdtestDlg* pDlg)
{
	SG_REQ	sg;
	double time1 = 0,time2 = 0,time3 = 0,TransferRate = 0;
	int  count = 128000;
	TCHAR buf[32];
	int i;
	for(i=0;i<512*128;i++)
	{
		buffer[i] = 'a';

	}
    pDlg->Output(0,0,2);
    time1 = GetTickCount();
	while(1)
	{
		if(count>256000)
		{
			time2  = GetTickCount();
			time3 = (time2 - time1)/1000;
			TransferRate = 64*1024/time3;
			swprintf(buf,_T("The Writing TransferRate is %f"),TransferRate);
			count = 0;
			CloseHandle(g_hStore);
			g_hStore = NULL;
			MessageBox(NULL,buf,_T(""),MB_OK);
			break;
		}
		sg.sr_start = count;
		sg.sr_num_sec = 128;
		sg.sr_num_sg = 1;
		sg.sr_status = ERROR_NOT_SUPPORTED;  // not used by ATADisk
		sg.sr_callback = NULL;
		sg.sr_sglist[0].sb_buf =buffer;
		sg.sr_sglist[0].sb_len = 128*512;
		if(!pDlg->SDMMC_Write(&sg))
			AfxMessageBox(TEXT("WriteTest FAIL"));
			
		count+=128;
	}
	/*
	if(j<5)
	{
		MessageBox(_T("Power off harddisk"));
		PowerOffHarddisk();
	    Sleep(1*60*1000);
	    MessageBox(_T("Next test start"));
	}
	else
	    MessageBox(_T("Test over"));
	}*/
	//pDlg->PowerOffHarddisk();
	pDlg->Output(0,0,7);
	CloseHandle(hWrite);

}

void ReadThread(CHdtestDlg* pDlg)
{
	SG_REQ	sg;
	double time1 = 0,time2 = 0,time3 = 0,TransferRate = 0;
	int  count = 0;
	beReadTest = TRUE;
    //while(beReadTest)
	//{
		pDlg->Output(0,0,1);
		time1 = GetTickCount();
	    while(1)
		{
			//if(!beReadTest)
               //break;
			if(count>128000)
			{
				time2  = GetTickCount();
			    time3 = (time2 - time1)/1000;
			    TransferRate = 64*1024/time3;
			    swprintf(rateBuf,_T("The reading TransferRate is %f"),TransferRate);
			    count = 0;
			    CloseHandle(g_hStore);
			    g_hStore = NULL;
			    MessageBox(NULL,rateBuf,_T(""),MB_OK);
			    break;
			}
		    sg.sr_start = count;
		    sg.sr_num_sec = 128;
		    sg.sr_num_sg = 1;
		    sg.sr_status = ERROR_NOT_SUPPORTED;  // not used by ATADisk
		    sg.sr_callback = NULL;
		    sg.sr_sglist[0].sb_buf =buffer;
		    sg.sr_sglist[0].sb_len = 128*512;
		    if(!pDlg->SDMMC_Read(&sg))
			   AfxMessageBox(TEXT("ReadTest FAIL"));
            //if(!pDlg->SDMMC_Write(&sg))
			   //AfxMessageBox(TEXT("WriteTest FAIL"));
			
		    count+=128;
		}

		//pDlg->PowerOffHarddisk();
	    //Sleep(3.5*60*1000);
		//pDlg->PowerOnHarddisk();

	//}
	pDlg->Output(0,0,7);
	//CloseHandle(g_hStore);
	//g_hStore = NULL;
	CloseHandle(hRead);

}
void CHdtestDlg::OnButton1() 
{
	// TODO: Add your control notification handler code here
    hRead=CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) ReadThread, this, 0, NULL);
	
}

void CHdtestDlg::PowerOffHarddisk()
{

	HANDLE hDev;
	BOOL ret;
	LPVOID lpInBuffer = NULL,lpOutBuffer = NULL;
	DWORD  nInBufferSize = 0,nOutBufferSize = 0;
	LPDWORD  lpBytesReturned = NULL;
	USHORT 	buf[32] = TEXT("In Stand-by mode"), errBuf[32];
	//memcpy(buf,TEXT("%s"),(USHORT)("In Stand-by mode"));
	//MessageBox(NULL,_T("You press the button 'STANDBY'"),MB_OK);
	hDev = CreateFile(_T("DSK1:"),
		              GENERIC_READ|GENERIC_WRITE,
					  FILE_SHARE_WRITE|FILE_SHARE_READ,
					 
					  NULL,
					  OPEN_EXISTING,
					  0,
					  NULL);
	if(!hDev)
	{   
		swprintf(errBuf,_T("Error code: %d"),GetLastError());
	    MessageBox(NULL,errBuf,MB_OK);
		MessageBox(NULL,_T("CreateFile failed"),MB_OK);
	}
	ret = DeviceIoControl(hDev,
		                  DISK_IOCTL_STANDBY,
                          lpInBuffer,
						  nInBufferSize,
						  lpOutBuffer,
						  nOutBufferSize,
						  lpBytesReturned,
						  NULL);

	if(!ret)
	{   

		swprintf(errBuf,_T("Error code: %d"),GetLastError());
	    MessageBox(NULL,errBuf,MB_OK);
		MessageBox(NULL,_T("IOCTL_STANDBY failed"),MB_OK);
	}
	Output(0,0,5);
	CloseHandle(hDev);
}

BOOL CHdtestDlg::SDMMC_Write(SG_REQ *sg)
{
		if(g_hStore==NULL)
	{
		g_hStore=CreateFile(TEXT("DSK1:"),GENERIC_READ|GENERIC_WRITE,0,NULL,OPEN_EXISTING,0,NULL);
		if(g_hStore==INVALID_HANDLE_VALUE)
		{  	
			CloseHandle(g_hStore);
		    return FALSE;
		}
	}

	//AfxMessageBox(L"create file after");
	if (!DeviceIoControl(g_hStore, DISK_IOCTL_WRITE, sg, sizeof(SG_REQ), NULL, 0, NULL, NULL))
	{
              return FALSE;
	}

	return TRUE;


}

BOOL CHdtestDlg::SDMMC_Read(SG_REQ *sg)
{
		if(g_hStore==NULL)
	{
		g_hStore=CreateFile(TEXT("DSK1:"),GENERIC_READ|GENERIC_WRITE,0,NULL,OPEN_EXISTING,0,NULL);
		if(g_hStore==INVALID_HANDLE_VALUE)
		{  	
			AfxMessageBox(L"create file failed");
			CloseHandle(g_hStore);
		    return FALSE;
		}
	}

	//AfxMessageBox(L"create file after");
	if (!DeviceIoControl(g_hStore, DISK_IOCTL_READ, sg, sizeof(SG_REQ), NULL, 0, NULL, NULL))
	{
		AfxMessageBox(L"IO control failed");
              return FALSE;
	}

	return TRUE;


}

void CHdtestDlg::OnButton2() 
{
	// TODO: Add your control notification handler code here
	hWrite=CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) WriteThread, this, 0, NULL);
	
	
}

void CHdtestDlg::OnButton3() 
{
	// TODO: Add your control notification handler code here
	PowerOffHarddisk();
	MessageBox(_T("Disk is powered off"));
}

void CHdtestDlg::OnButton4() 
{
	// TODO: Add your control notification handler code here
	SG_REQ	sg;
	sg.sr_start = 0;
	sg.sr_num_sec = 1;
	sg.sr_num_sg = 1;
	sg.sr_status = ERROR_NOT_SUPPORTED;  // not used by ATADisk
	sg.sr_callback = NULL;
	sg.sr_sglist[0].sb_buf =buffer;
	sg.sr_sglist[0].sb_len = 512;
	if(SDMMC_Read(&sg))
		MessageBox(_T("Harddisk power on"));
	else
		MessageBox(_T("Error power on harddisk"));
}

void ContinueReadThread(CHdtestDlg* pDlg)
{
	SG_REQ	sg;

    beContinueRead = TRUE;
	while(beContinueRead)
	{
		
		sg.sr_num_sec = 128;
		sg.sr_num_sg = 1;
		sg.sr_status = ERROR_NOT_SUPPORTED;  // not used by ATADisk
		sg.sr_callback = NULL;
		sg.sr_sglist[0].sb_buf =buffer;
		sg.sr_sglist[0].sb_len = 128*512;
		//swprintf(bufTest,_T("ContinueReading start: %d\r\nsector number: %d"),sg.sr_start,sg.sr_num_sec);
		pDlg->Output(sg.sr_start,sg.sr_num_sec,3);
		if(!pDlg->SDMMC_Read(&sg))
			AfxMessageBox(TEXT("ReadTest FAIL"));
		if(!pDlg->SDMMC_Write(&sg))
			AfxMessageBox(TEXT("WriteTest FAIL"));
		//NKDbgPrintfW(TEXT("In loop\r\n"));
        sg.sr_start += 128;
		if(sg.sr_start>128000)
			sg.sr_start = 0;
	}
    CloseHandle(g_hStore);
	g_hStore = NULL;
	CloseHandle(hContinueRead);
	

}

void CHdtestDlg::OnButton5() 
{
	// TODO: Add your control notification handler code here
	hContinueRead=CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) ContinueReadThread, this, 0, NULL);

}

void CHdtestDlg::OnButton6() 
{
	// TODO: Add your control notification handler code here
	if(beContinueRead)
		beContinueRead = FALSE;
	if(beReadTest)
	{
		beReadTest = FALSE;
        MessageBox(rateBuf);
	}
}

void CHdtestDlg::PowerOnHarddisk()
{
	SG_REQ	sg;
	sg.sr_start = 0;
	sg.sr_num_sec = 1;
	sg.sr_num_sg = 1;
	sg.sr_status = ERROR_NOT_SUPPORTED;  // not used by ATADisk
	sg.sr_callback = NULL;
	sg.sr_sglist[0].sb_buf =buffer;
	sg.sr_sglist[0].sb_len = 512;
	SDMMC_Read(&sg);
	Output(0,0,4);

}

void CHdtestDlg::OnButton7() 
{
	// TODO: Add your control notification handler code here
	HANDLE hLGT;
	PBrightnessCmdParm pCmdParm;
    pCmdParm = (PBrightnessCmdParm)LocalAlloc(0,sizeof(BrightnessCmdParm));
	pCmdParm->dwCmd = SETBRIGHTNESS;
	NKDbgPrintfW(TEXT("brightness = %d\r\n"),brightness);
    pCmdParm->dwParm = brightness;
	Output(brightness,0,6);
	brightness++;
	if(brightness>5)
		brightness = 0;
	hLGT=CreateFile(TEXT("LGT1:"),GENERIC_READ|GENERIC_WRITE,0,NULL,OPEN_EXISTING,0,NULL);
		if(hLGT==INVALID_HANDLE_VALUE)
		{  	
			AfxMessageBox(L"Open backlight device failed");
			NKDbgPrintfW(TEXT("Open backlight device failed\r\n"));
			CloseHandle(hLGT);
		    //return FALSE;
		}
	
	if (!DeviceIoControl(hLGT, IOCTL_BKL_BRIGHTNESS, pCmdParm, sizeof(BrightnessCmdParm), NULL, 0, NULL, NULL))
	{
		AfxMessageBox(L"IO control failed");
		NKDbgPrintfW(TEXT("IO control failed\r\n"));
             // return FALSE;
	}
	LocalFree(pCmdParm);
	CloseHandle(hLGT);
}



void CHdtestDlg::Output(DWORD arg1, DWORD arg2,int id)
{
	CEdit *pEdit = (CEdit*)GetDlgItem(IDC_EDIT1);
    TCHAR buf[64];
	if(id==1)
    {
		swprintf(buf,_T("In reading test..."));
	}
	else if(id==2)
	{
		swprintf(buf,_T("In Writting test..."));
	}
	else if(id==3)
	{
		//pEdit->SetWindowText(bufTest);
		swprintf(buf,_T("ContinueReading start: %d\r\nsector number: %d"),arg1,arg2);
	}
	else if(id==4)
	{
		swprintf(buf,_T("Harddisk power on"));
	}
	else if(id==5)
	{
		swprintf(buf,_T("Harddisk power off"));
	}
	else if(id==6)
	{
		swprintf(buf,_T("BackLighe level: %d"),arg1);
	}
	else if(id==7)
	{
		swprintf(buf,_T("Test over,thank you!"));
	}



	pEdit->SetWindowText(buf);

}
