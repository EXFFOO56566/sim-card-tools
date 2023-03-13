//  Copyright(C):      Alcor Micro, Corp..
//
//  File:              SLE4432_4442Dlg.pas
//
//  Description:       This sample program outlines the steps on how to
//                     program SLE4432/4442 memory cards using Alcor readers
//                     in PC/SC platform.
//
//  Author:	           Rian.Liu
//
//  Date:	           JAN 14, 2013
//
//  Revision Trail:   (Date/Author/Description)
//
//======================================================================
// SLE4432_4442Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "SLE4432_4442.h"
#include "SLE4432_4442Dlg.h"

// ==========================================================================
// SLE4432_4442Dlg include file

//#include "Winscard.h"

// SLE4432_4442Dlg GlobalVariables
	SCARDCONTEXT			hContext;
	SCARDHANDLE				hCard;
	unsigned long			dwActProtocol;
	LPCBYTE					pbSend;
	DWORD					dwSend, dwRecv, size = 64;
	LPBYTE					pbRecv;
	SCARD_IO_REQUEST		ioRequest;
	int						retCode;
    char					readerName [256];
	DWORD					SendLen,
							RecvLen,
							nBytesRet;
	BYTE					SendBuff[262],
							RecvBuff[262];
	BOOL					connActive;
	CSLE4432_4442Dlg	    *pThis=NULL;
// ==========================================================================

// Alcor SLE4442 API
	PTR_SLE4442CMD_READMAINMEMORY                  pSLE4442Cmd_ReadMainMemory;
	PTR_SLE4442CMD_UPDATEMAINMEMORY                pSLE4442Cmd_UpdateMainMemory;
	PTR_SLE4442CMD_WRITEPROTECTIONMEMORY           pSLE4442Cmd_WriteProtectionMemory;
	PTR_SLE4442CMD_READSECURITYMEMORY              pSLE4442Cmd_ReadSecurityMemory;
	PTR_SLE4442CMD_UPDATESECURITYMEMORY            pSLE4442Cmd_UpdateSecurityMemory;
	PTR_SLE4442CMD_VERIFY        				   pSLE4442Cmd_Verify;
	PTR_Alcor_SwitchCardMode					   pAlcor_SwitchCardMode;
// ==========================================================================

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSLE4432_4442Dlg dialog

CSLE4432_4442Dlg::CSLE4432_4442Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSLE4432_4442Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSLE4432_4442Dlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON1);
	m_hDll = NULL;
}

void CSLE4432_4442Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSLE4432_4442Dlg)
	DDX_Control(pDX, IDC_BUTTON4, bChange);
	DDX_Control(pDX, IDC_BUTTON6, bWriteProt);
	DDX_Control(pDX, IDC_EDIT1, tAdd);
	DDX_Control(pDX, IDC_RADIO1, rbSLE4432);
	DDX_Control(pDX, IDC_RADIO2, rbSLE4442);
	DDX_Control(pDX, IDC_COMBO1, cbReader);
	DDX_Control(pDX, IDC_LIST1, mMsg);
	DDX_Control(pDX, IDC_EDIT4, tData);
	DDX_Control(pDX, IDC_EDIT3, tLen);
	DDX_Control(pDX, IDC_BUTTON9, bReset);
	DDX_Control(pDX, IDC_BUTTON8, bErrCtr);
	DDX_Control(pDX, IDC_BUTTON7, bSubmit);
	DDX_Control(pDX, IDC_BUTTON5, bWrite);
	DDX_Control(pDX, IDC_BUTTON3, bRead);
	DDX_Control(pDX, IDC_BUTTON2, bConnect);
	DDX_Control(pDX, IDC_BUTTON10, bQuit);
	DDX_Control(pDX, IDC_BUTTON1, bInit);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSLE4432_4442Dlg, CDialog)
	//{{AFX_MSG_MAP(CSLE4432_4442Dlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, OnInit)
	ON_BN_CLICKED(IDC_BUTTON9, OnReset)
	ON_BN_CLICKED(IDC_BUTTON10, OnQuit)
	ON_BN_CLICKED(IDC_BUTTON2, OnConnect)
	ON_BN_CLICKED(IDC_RADIO1, OnSLE4432)
	ON_CBN_EDITCHANGE(IDC_COMBO1, OnEditchangeCombo1)
	ON_BN_CLICKED(IDC_RADIO2, OnSLE4442)
	ON_BN_CLICKED(IDC_BUTTON3, OnRead)
	ON_BN_CLICKED(IDC_BUTTON4, OnChangeCode)
	ON_BN_CLICKED(IDC_BUTTON5, OnWrite)
	ON_BN_CLICKED(IDC_BUTTON7, OnSubmit)
	ON_BN_CLICKED(IDC_BUTTON8, OnErrCtr)
	ON_BN_CLICKED(IDC_BUTTON6, OnWriteProt)
	ON_CBN_SELCHANGE(IDC_COMBO1, OnSelchangeCombo1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// ==========================================================================
// SLE4432_4442Dlg internal routines

static CString GetScardErrMsg(int code)
{
	switch(code)
	{
	// Smartcard Reader interface errors
	case SCARD_E_CANCELLED:
		return ("The action was canceled by an SCardCancel request.");
		break;
	case SCARD_E_CANT_DISPOSE:
		return ("The system could not dispose of the media in the requested manner.");
		break;
	case SCARD_E_CARD_UNSUPPORTED:
		return ("The smart card does not meet minimal requirements for support.");
		break;
	case SCARD_E_DUPLICATE_READER:
		return ("The reader driver didn't produce a unique reader name.");
		break;
	case SCARD_E_INSUFFICIENT_BUFFER:
		return ("The data buffer for returned data is too small for the returned data.");
		break;
	case SCARD_E_INVALID_ATR:
		return ("An ATR string obtained from the registry is not a valid ATR string.");
		break;
	case SCARD_E_INVALID_HANDLE:
		return ("The supplied handle was invalid.");
		break;
	case SCARD_E_INVALID_PARAMETER:
		return ("One or more of the supplied parameters could not be properly interpreted.");
		break;
	case SCARD_E_INVALID_TARGET:
		return ("Registry startup information is missing or invalid.");
		break;
	case SCARD_E_INVALID_VALUE:
		return ("One or more of the supplied parameter values could not be properly interpreted.");
		break;
	case SCARD_E_NOT_READY:
		return ("The reader or card is not ready to accept commands.");
		break;
	case SCARD_E_NOT_TRANSACTED:
		return ("An attempt was made to end a non-existent transaction.");
		break;
	case SCARD_E_NO_MEMORY:
		return ("Not enough memory available to complete this command.");
		break;
	case SCARD_E_NO_SERVICE:
		return ("The smart card resource manager is not running.");
		break;
	case SCARD_E_NO_SMARTCARD:
		return ("The operation requires a smart card, but no smart card is currently in the device.");
		break;
	case SCARD_E_PCI_TOO_SMALL:
		return ("The PCI receive buffer was too small.");
		break;
	case SCARD_E_PROTO_MISMATCH:
		return ("The requested protocols are incompatible with the protocol currently in use with the card.");
		break;
	case SCARD_E_READER_UNAVAILABLE:
		return ("The specified reader is not currently available for use.");
		break;
	case SCARD_E_READER_UNSUPPORTED:
		return ("The reader driver does not meet minimal requirements for support.");
		break;
	case SCARD_E_SERVICE_STOPPED:
		return ("The smart card resource manager has shut down.");
		break;
	case SCARD_E_SHARING_VIOLATION:
		return ("The smart card cannot be accessed because of other outstanding connections.");
		break;
	case SCARD_E_SYSTEM_CANCELLED:
		return ("The action was canceled by the system, presumably to log off or shut down.");
		break;
	case SCARD_E_TIMEOUT:
		return ("The user-specified timeout value has expired.");
		break;
	case SCARD_E_UNKNOWN_CARD:
		return ("The specified smart card name is not recognized.");
		break;
	case SCARD_E_UNKNOWN_READER:
		return ("The specified reader name is not recognized.");
		break;
	case SCARD_F_COMM_ERROR:
		return ("An internal communications error has been detected.");
		break;
	case SCARD_F_INTERNAL_ERROR:
		return ("An internal consistency check failed.");
		break;
	case SCARD_F_UNKNOWN_ERROR:
		return ("An internal error has been detected, but the source is unknown.");
		break;
	case SCARD_F_WAITED_TOO_LONG:
		return ("An internal consistency timer has expired.");
		break;
	case SCARD_W_REMOVED_CARD:
		return ("The smart card has been removed and no further communication is possible.");
		break;
	case SCARD_W_RESET_CARD:
		return ("The smart card has been reset, so any shared state information is invalid.");
		break;
	case SCARD_W_UNPOWERED_CARD:
		return ("Power has been removed from the smart card and no further communication is possible.");
		break;
	case SCARD_W_UNRESPONSIVE_CARD:
		return ("The smart card is not responding to a reset.");
		break;
	case SCARD_W_UNSUPPORTED_CARD:
		return ("The reader cannot communicate with the card due to ATR string configuration conflicts.");
		break;
	case NO_READER_INSTALLED:
		return ("The smartcard reader is not installed in your system.");
		break;
	case ERR_SLE4442_CARD_LOCKED:
        return ("The smart card is locked.");
		break;
	}
	return ("Error is not documented.");
}

void ClearBuffers()
{
	int indx;
    for (indx = 0;indx<263;indx++)
    {
      SendBuff[indx] = 0x00;
      RecvBuff[indx] = 0x00;
	}
}

void ClearFields()
{

	pThis->tAdd.SetWindowText("");
	pThis->tLen.SetWindowText("");
	pThis->tData.SetWindowText("");

}

void EnableFields()
{
	pThis->tAdd.EnableWindow(TRUE);
	pThis->tAdd.SetLimitText(2);
	pThis->tLen.EnableWindow(TRUE);
	pThis->tLen.SetLimitText(2);
	pThis->tData.EnableWindow(TRUE);
}

void DisableFields()
{
	pThis->tAdd.EnableWindow(FALSE);
	pThis->tLen.EnableWindow(FALSE);
	pThis->tData.EnableWindow(FALSE);
}

void InitMenu()
{
	pThis->cbReader.ResetContent();
    pThis->mMsg.ResetContent();
	pThis->rbSLE4432.SetCheck(0);
	pThis->cbReader.EnableWindow(FALSE);
	pThis->bInit.EnableWindow(TRUE);
	pThis->bConnect.EnableWindow(FALSE);
	pThis->bErrCtr.EnableWindow(FALSE);
	pThis->bRead.EnableWindow(FALSE);
	pThis->bWrite.EnableWindow(FALSE);
	pThis->bWriteProt.EnableWindow(FALSE);
	pThis->bReset.EnableWindow(FALSE);
	pThis->bSubmit.EnableWindow(FALSE);
	pThis->bChange.EnableWindow(FALSE);
	ClearFields();
	DisableFields();
}

void DisplayOut(int errType, int retVal, CString prtText)
{
  int indx, strLen;
  CString strTxt;

  strLen = prtText.GetLength();

  switch (errType){
  case 0:                          // Notifications
	  pThis->mMsg.AddString(prtText);
	  break;
  case 1:                          // Error Messages
	  pThis->mMsg.AddString(GetScardErrMsg(retVal));
	  break;
  case 2:						   // Input data
	  for (indx=0; indx<strLen; indx+=48)
	  {
		  strTxt = "< " + prtText.Mid(indx, 48);
		  pThis->mMsg.AddString(strTxt);
	  }	  	 
	  break;
  case 3:                          // Output data
	  for (indx=0; indx<strLen; indx+=48)
	  {
		 strTxt = "> " + prtText.Mid(indx, 48);
		 pThis->mMsg.AddString(strTxt);
	  }	 
	  break;
  }
}

void AddButtons()
{
	pThis->bInit.EnableWindow(FALSE);
	pThis->bConnect.EnableWindow(TRUE);
	pThis->bRead.EnableWindow(TRUE);
	pThis->bWrite.EnableWindow(TRUE);
	pThis->bWriteProt.EnableWindow(TRUE);
	pThis->bReset.EnableWindow(TRUE);
    if (pThis->rbSLE4432.GetCheck() == 1){
 		pThis->bSubmit.EnableWindow(FALSE);
    	pThis->bChange.EnableWindow(FALSE);
		pThis->bErrCtr.EnableWindow(FALSE);
	}
	else{
 		pThis->bSubmit.EnableWindow(TRUE);
    	pThis->bChange.EnableWindow(TRUE);
		pThis->bErrCtr.EnableWindow(TRUE);
	}
	
}

BOOL InputOK(int checkType)
{
	char tmpStr[257];
	int tmpLen;

	switch(checkType){
	case 0:               // for Read function
		tmpLen = pThis->tAdd.GetWindowText(tmpStr, 3);
		if  (tmpLen != 2){
			pThis->tAdd.SetFocus();
			return 0;
		}
		tmpLen = pThis->tLen.GetWindowText(tmpStr, 3);
		if  (tmpLen != 2){
			pThis->tLen.SetFocus();
			return 0;
		}
		break;
	case 1:               // for Write function
		tmpLen = pThis->tAdd.GetWindowText(tmpStr, 3);
		if  (tmpLen != 2){
			pThis->tAdd.SetFocus();
			return 0;
		}
		tmpLen = pThis->tLen.GetWindowText(tmpStr, 3);
		if  (tmpLen != 2){
			pThis->tLen.SetFocus();
			return 0;
		}
		tmpLen = pThis->tData.GetWindowText(tmpStr, 257);
		if  (tmpStr[0] == 0){
			pThis->tData.SetFocus();
			return 0;
		}
		break;
	case 2:               // for Verify function
        pThis->tAdd.Clear();
        pThis->tLen.Clear();
		tmpLen = pThis->tData.GetWindowText(tmpStr, 257);
		if  (tmpLen < 4){
			pThis->tData.SetFocus();
			return 0;
		}
		break;
	}

	return 1;

}

/////////////////////////////////////////////////////////////////////////////
// CSLE4432_4442Dlg message handlers

BOOL CSLE4432_4442Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	pThis = this;
	InitMenu();

	InitAlcorAPI();//Load Alcor SLE4442 API Dll

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSLE4432_4442Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSLE4432_4442Dlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSLE4432_4442Dlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CSLE4432_4442Dlg::OnInit() 
{
	int nLength = 64;
	
  // 1. Initialize SC reader
  //  1.1. Establish context
 	retCode = SCardEstablishContext (SCARD_SCOPE_USER,
								NULL,
								NULL,
								&hContext);
	if (retCode != SCARD_S_SUCCESS)
	{
        DisplayOut(1, retCode, "");
        return;
	}

  //  1.2. List PC/SC readers
	size = 256;
	retCode = SCardListReaders (hContext,
							NULL,
							readerName,
							&size);
	if (retCode != SCARD_S_SUCCESS)
	{
        DisplayOut(1, retCode, "");
        return;
	}
	if (readerName == NULL)
	{
        DisplayOut(0, retCode, "No PC/SC reader is detected in your system.");
    	return;
	}
	
	cbReader.ResetContent();
	char *p = readerName;
	while (*p)
	{
    	int i;
		for (i=0;p[i];i++);
	      i++;
	    if (*p != 0)
		{
     		cbReader.AddString(p);
		}
		p = &p[i];
	}
	cbReader.SetCurSel(0);
    DisplayOut(0, retCode, "Select Reader and Card to connect.");
    cbReader.EnableWindow(TRUE);
	rbSLE4432.SetCheck(0);
	rbSLE4442.SetCheck(1);
	bConnect.EnableWindow(TRUE);
	bInit.EnableWindow(FALSE);
	bReset.EnableWindow(TRUE);
	
}

void CSLE4432_4442Dlg::OnReset() 
{

	ClearFields();

    // Close SC reader
	if (connActive)
		retCode = SCardDisconnect(hCard, SCARD_UNPOWER_CARD);
    retCode = SCardReleaseContext(hContext);
    connActive = FALSE;
	InitMenu();
	
}

void CSLE4432_4442Dlg::OnQuit() 
{

    int tmpResult = 0;

    // Close SC reader
	if (connActive)
		retCode = SCardDisconnect(hCard, SCARD_UNPOWER_CARD);
	retCode = SCardReleaseContext(hContext);
    this->EndDialog(tmpResult);
	
}

void CSLE4432_4442Dlg::InitAlcorAPI(void)
{
	m_hDll = LoadLibrary( _T("AlcorSLE4442API.dll") );
	if ( m_hDll == NULL )
	{	
		AfxMessageBox(_T("Load Alcor SLE4442 API Dll Fail!"));
		return;
	}

	pSLE4442Cmd_ReadMainMemory =                 (PTR_SLE4442CMD_READMAINMEMORY) GetProcAddress(m_hDll, _T("SLE4442Cmd_ReadMainMemory"));
	pSLE4442Cmd_UpdateMainMemory =               (PTR_SLE4442CMD_UPDATEMAINMEMORY) GetProcAddress(m_hDll, _T("SLE4442Cmd_UpdateMainMemory"));
	pSLE4442Cmd_WriteProtectionMemory =          (PTR_SLE4442CMD_WRITEPROTECTIONMEMORY) GetProcAddress(m_hDll, _T("SLE4442Cmd_WriteProtectionMemory"));
	pSLE4442Cmd_ReadSecurityMemory =             (PTR_SLE4442CMD_READSECURITYMEMORY) GetProcAddress(m_hDll, _T("SLE4442Cmd_ReadSecurityMemory"));
	pSLE4442Cmd_UpdateSecurityMemory =           (PTR_SLE4442CMD_UPDATESECURITYMEMORY) GetProcAddress(m_hDll, _T("SLE4442Cmd_UpdateSecurityMemory"));
	pSLE4442Cmd_Verify =						 (PTR_SLE4442CMD_VERIFY) GetProcAddress(m_hDll, _T("SLE4442Cmd_Verify"));
	pAlcor_SwitchCardMode =						 (PTR_Alcor_SwitchCardMode) GetProcAddress(m_hDll, _T("Alcor_SwitchCardMode"));
}

void CSLE4432_4442Dlg::OnConnect() 
{
	int tmpInt = cbReader.GetCurSel();
	CString rName,cardType;
	DWORD dwReaderLen;
	char buffer[200];
	char tmpStr[128]; 

	if (connActive){
		DisplayOut(0, 0, "Connection is already active.");
		return;
	}
	cbReader.GetLBText(tmpInt, rName);

  // 1. Direct Connection
	retCode = SCardConnect(hContext,
						rName,
						SCARD_SHARE_DIRECT,
						0,
						&hCard,
						&dwActProtocol);
	if (retCode != SCARD_S_SUCCESS)
	{
        DisplayOut(1, retCode, "");
        return;
	}

    // 2. Select Card Type
	ClearBuffers();

    if (rbSLE4432.GetCheck() == 1){
		// Card Type for SLE4432
		cardType = "SLE 4432";
	}
	else{
		// Card Type for SLE4442
		cardType = "SLE 4442";
	}

	retCode = pAlcor_SwitchCardMode(hCard, 0, 0x04);//0x04, SLE4442 Card Type
	if (retCode != SCARD_S_SUCCESS)
	{
        DisplayOut(1, retCode, "");
        return;
	}

	sprintf(buffer, "%s is selected.", cardType);
	DisplayOut(0, 0, buffer);

    // 3. Reconnect using SCARD_SHARE_SHARED and
    //    SCARD_PROTOCOL_T0 parameters
    retCode = SCardDisconnect(hCard, SCARD_UNPOWER_CARD);
	if (retCode != SCARD_S_SUCCESS)
	{
        DisplayOut(1, retCode, "");
        return;
	}

	retCode = SCardConnect(hContext,
						rName,
						SCARD_SHARE_SHARED,
						SCARD_PROTOCOL_T0,
						&hCard,
						&dwActProtocol);
	if (retCode != SCARD_S_SUCCESS)
	{
        DisplayOut(1, retCode, "");
		connActive = FALSE;
        return;
	}
	sprintf(buffer, "Connected to %s.", rName);
	DisplayOut(0, 0, buffer);

	// 4. Get ATR
	DisplayOut(0, 0, "Get ATR");
	ClearBuffers();

	memset(buffer, '\0', 200);
	sprintf(buffer, "%s.", rName);
	dwReaderLen = sizeof(buffer);
	RecvLen = 262;
	retCode = SCardStatus(hCard, /*NULL*/ buffer, &dwReaderLen, NULL, NULL,RecvBuff, &RecvLen);
	if (retCode != SCARD_S_SUCCESS)
	{
		DisplayOut(1, retCode, "");
		return;
	}

    strcpy(tmpStr, "");
	for(int i=0; i<RecvLen-1; i++){
		sprintf(buffer, "%02X ", RecvBuff[i] & 0x00FF);
		strcat(tmpStr, buffer);
	}
	DisplayOut(3, 0, tmpStr);

	connActive = TRUE;
	AddButtons();
	EnableFields();

}

void CSLE4432_4442Dlg::OnSLE4432() 
{
	ClearFields();
	DisableFields();
	bRead.EnableWindow(FALSE);
	bWrite.EnableWindow(FALSE);
	bChange.EnableWindow(FALSE);
	bWriteProt.EnableWindow(FALSE);
	bSubmit.EnableWindow(FALSE);
	bErrCtr.EnableWindow(FALSE);
	
	if (connActive)
		retCode = SCardDisconnect(hCard, SCARD_UNPOWER_CARD);
	connActive = FALSE;

}

void CSLE4432_4442Dlg::OnEditchangeCombo1() 
{
	ClearFields();
	DisableFields();
	bRead.EnableWindow(FALSE);
	bWrite.EnableWindow(FALSE);
	bChange.EnableWindow(FALSE);
	bWriteProt.EnableWindow(FALSE);
	bSubmit.EnableWindow(FALSE);
	bErrCtr.EnableWindow(FALSE);
	
	if (connActive)
		retCode = SCardDisconnect(hCard, SCARD_UNPOWER_CARD);
	connActive = FALSE;

}

void CSLE4432_4442Dlg::OnSLE4442() 
{
	ClearFields();
	DisableFields();
	bRead.EnableWindow(FALSE);
	bWrite.EnableWindow(FALSE);
	bChange.EnableWindow(FALSE);
	bWriteProt.EnableWindow(FALSE);
	bSubmit.EnableWindow(FALSE);
	bErrCtr.EnableWindow(FALSE);
	
	if (connActive)
		retCode = SCardDisconnect(hCard, SCARD_UNPOWER_CARD);
	connActive = FALSE;

}

void CSLE4432_4442Dlg::OnRead() 
{
	int tmpInt, indx;
	BYTE abAddress,abDataLen;
	char buffer[256], tmpStr[256];
	CString strText,strTemp;

	// 1. Check for all input fields
	if (!InputOK(0))
		return;
    DisplayOut(0, 0, "Read Main Memory");

	// 2. Read input fields and pass data to card
	tData.Clear();
	ClearBuffers();

	tmpInt = 0;
	tAdd.GetWindowText(buffer, 3);
	sscanf(buffer, "%02X", &tmpInt);
	abAddress = tmpInt;

	tmpInt = 0;
	tLen.GetWindowText(buffer, 3);
	sscanf(buffer, "%02X", &tmpInt);
	abDataLen = tmpInt;

	if (abDataLen > (0xFF - abAddress))
	{
		DisplayOut(3, 0, "Read Data Length isn't proper ( length <= 0xFF-Address )");
		return;
    }
	
	retCode = pSLE4442Cmd_ReadMainMemory(
		hCard,
		0,
		abAddress,
		abDataLen,
		RecvBuff,
		&RecvLen
		);	

	if (retCode != SCARD_S_SUCCESS)
	{
		DisplayOut(1, retCode, "");
        return;
	}

	// 3. Display data read from card into Data textbox
    strcpy(tmpStr, "");
	for(indx=0; indx<abDataLen; indx++){
 		sprintf(buffer, "%c", RecvBuff[indx] & 0xFF);
		strcat(tmpStr, buffer);
	}
	tData.SetWindowText(tmpStr);

	for(indx = 0; indx <= RecvLen -1; indx++)
	{
		strTemp.Format("%02X ", RecvBuff[indx]);
		strText += strTemp;
	}	
    DisplayOut(3, 0, strText);
}

void CSLE4432_4442Dlg::OnChangeCode() 
{
	int  indx;
	char buffer[256], tmpByte[2];
	CString strText,strTemp;

	// 1. Check for all input fields
	if (!InputOK(2))
		return;
	DisplayOut(0, 0, "Change PIN Number");
   
	// 2. Read input fields and pass data to card
	// 2. Read input fields and pass data to card
	ClearBuffers();

	tData.GetWindowText(buffer, 7);
	memcpy(tmpByte, buffer, 2);
	sscanf(tmpByte, "%02X", &SendBuff[0]);
	memcpy(tmpByte, buffer+2, 2);
	sscanf(tmpByte, "%02X", &SendBuff[1]);
	memcpy(tmpByte, buffer+4, 2);
	sscanf(tmpByte, "%02X", &SendBuff[2]);

	for(indx = 0; indx <= 2; indx++)
	{
		strTemp.Format("%02X ", SendBuff[indx]);
		strText += strTemp;
	}	
    DisplayOut(2, 0, strText);

	for (indx = 0; indx <= 2; indx++)
	{
		retCode = pSLE4442Cmd_UpdateSecurityMemory(hCard, 0, indx+1, SendBuff[indx]);
    }

	if (retCode != SCARD_S_SUCCESS)
        return;

	tData.SetWindowText("");

}

void CSLE4432_4442Dlg::OnWrite() 
{
	int tmpInt, indx;
	char buffer[256];
	BYTE abAddress, abWriteLen;
	CString strText, strTemp;

	// 1. Check for all input fields
	if (!InputOK(1))
		return;
	DisplayOut(0, 0, "Write Main Memory");
   
	// 2. Read input fields and pass data to card
	ClearBuffers();

	tmpInt = 0;
	tAdd.GetWindowText(buffer, 3);
	sscanf(buffer, "%02X", &tmpInt);
	abAddress = tmpInt;

	tmpInt = 0;
	tLen.GetWindowText(buffer, 3);
	sscanf(buffer, "%02X", &tmpInt);
	abWriteLen = tmpInt;
	
	if (abWriteLen > (0xFF - abAddress))
	{
		DisplayOut(3, 0, "Wrotten Data Length isn't proper ( length <= 0xFF-Address )");
		return;
    }

	tData.GetWindowText(buffer, abWriteLen+1);
	for (indx =0;indx<abWriteLen;indx++)
		SendBuff[indx] = buffer[indx];

	for(indx = 0; indx < abWriteLen; indx++)
	{
		strTemp.Format("%02X ", SendBuff[indx]);
		strText += strTemp;
	}	
    DisplayOut(2, 0, strText);

	for (indx=0; indx<abWriteLen; indx++)
	{
		retCode = pSLE4442Cmd_UpdateMainMemory(
			hCard,
			0,
			abAddress+indx,
			SendBuff[indx]
		);
	}

	if (retCode != SCARD_S_SUCCESS)
        return;

	tData.SetWindowText("");	
}

void CSLE4432_4442Dlg::OnSubmit() 
{
	int  indx;
	char buffer[256], tmpByte[2];
	CString strText,strTemp;

	// 1. Check for all input fields
	if (!InputOK(2))
		return;
	DisplayOut(0, 0, "Verify PIN Number");
   
	// 2. Read input fields and pass data to card
	ClearBuffers();

	tData.GetWindowText(buffer, 7);
	memcpy(tmpByte, buffer, 2);
	sscanf(tmpByte, "%02X", &SendBuff[0]);
	memcpy(tmpByte, buffer+2, 2);
	sscanf(tmpByte, "%02X", &SendBuff[1]);
	memcpy(tmpByte, buffer+4, 2);
	sscanf(tmpByte, "%02X", &SendBuff[2]);

	for(indx = 0; indx <= 2; indx++)
	{
		strTemp.Format("%02X ", SendBuff[indx]);
		strText += strTemp;
	}	
    DisplayOut(2, 0, strText);

	retCode = pSLE4442Cmd_Verify(
		hCard,
		0,
		3,
		SendBuff
		);
	if (retCode != SCARD_S_SUCCESS)
	{
		DisplayOut(1, retCode, "");
        return;
	}

	DisplayOut(3, 0, "Verify PIN Number Successfully!");
	tData.SetWindowText("");
}

void CSLE4432_4442Dlg::OnErrCtr() 
{

	int indx;
	CString strText, strTemp;

	// 1. Clear all input fields
	ClearFields();
	DisplayOut(0, 0, "Read Security Memory");
   
  // 2. Send all inputs to the card
	ClearBuffers();

	retCode = pSLE4442Cmd_ReadSecurityMemory(
		hCard,
		0,
		4,
		RecvBuff,
		&RecvLen
		);
	if (retCode != SCARD_S_SUCCESS)
	{
		 DisplayOut(1, retCode, "");
        return;
	}

	for(indx = 0; indx <= RecvLen -1; indx++)
	{
		strTemp.Format("%02X ", RecvBuff[indx]);
		strText += strTemp;
	}	
    DisplayOut(3, 0, strText);
}

void CSLE4432_4442Dlg::OnWriteProt() 
{
	int tmpInt, indx;
	char buffer[256];
	BYTE abAddress, abWriteLen;
	CString strText, strTemp;

	// 1. Check for all input fields
	if (!InputOK(1))
		return;
	DisplayOut(0, 0, "Write Protection Memory");
   
	// 2. Read input fields and pass data to card
	ClearBuffers();

	tmpInt = 0;
	tAdd.GetWindowText(buffer, 3);
	sscanf(buffer, "%02X", &tmpInt);
	abAddress = tmpInt;
	if (abAddress > 0x1F)    
	{
		DisplayOut(3, 0, "Wrotten Data Address isn't proper ( Address < 0x20 )");
		return;
	}

	tmpInt = 0;
	tLen.GetWindowText(buffer, 3);
	sscanf(buffer, "%02X", &tmpInt);
	abWriteLen = tmpInt;
	if (abWriteLen > (0x20 - abAddress))
	{
		DisplayOut(3, 0, "Wrotten Data Length isn't proper ( length <= 0x20-Address )");
		return;
    }
	
	tData.GetWindowText(buffer, abWriteLen+1);
	for (indx =0; indx<abWriteLen; indx++)
		SendBuff[indx] = buffer[indx];

	for(indx = 0; indx < abWriteLen; indx++)
	{
		strTemp.Format("%02X ", SendBuff[indx]);
		strText += strTemp;
	}	
    DisplayOut(2, 0, strText);

	for (indx=0; indx<abWriteLen; indx++)
	{
		retCode = pSLE4442Cmd_WriteProtectionMemory(
			hCard,
			0,
			abAddress+indx,
			SendBuff[indx]
		);
	}

	if (retCode != SCARD_S_SUCCESS)
        return;

	tData.SetWindowText("");
	
}

void CSLE4432_4442Dlg::OnSelchangeCombo1() 
{
	ClearFields();
	DisableFields();
	bRead.EnableWindow(FALSE);
	bWrite.EnableWindow(FALSE);
	bChange.EnableWindow(FALSE);
	bWriteProt.EnableWindow(FALSE);
	bSubmit.EnableWindow(FALSE);
	bErrCtr.EnableWindow(FALSE);
	
	if (connActive)
		retCode = SCardDisconnect(hCard, SCARD_UNPOWER_CARD);
	connActive = FALSE;

}
