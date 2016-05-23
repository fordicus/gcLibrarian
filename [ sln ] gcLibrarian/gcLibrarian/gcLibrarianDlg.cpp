#pragma once
#include "stdafx.h"
#include "gcLibrarian.h"
#include "gcLibrarianDlg.h"
#include "afxdialogex.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

// �����Դϴ�.
protected:
	DECLARE_MESSAGE_MAP()
};
CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}
void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}
BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


CgcLibrarianDlg::CgcLibrarianDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_GCLIBRARIAN_DIALOG, pParent)
	, m_print(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON1);
}
void CgcLibrarianDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_print);
	DDX_Control(pDX, IDC_EDIT1, m_EditCTRL);
}


BEGIN_MESSAGE_MAP		(CgcLibrarianDlg, CDialogEx)
	ON_WM_SYSCOMMAND	()
	ON_WM_PAINT			()
	ON_WM_QUERYDRAGICON	()
	ON_WM_TIMER			()
	ON_BN_CLICKED		(IDCANCEL, &CgcLibrarianDlg::BtnQUIT)
	ON_BN_CLICKED		(IDOK, &CgcLibrarianDlg::BtnCLEAR)
	ON_WM_ERASEBKGND	()
	ON_BN_CLICKED		(IDC_BUTTON2, &CgcLibrarianDlg::BtnRESET)
	ON_BN_CLICKED		(IDC_BUTTON3, &CgcLibrarianDlg::BtnCOPY)
	ON_MESSAGE			(WM_TRAY_NOTIFICATION, OnTrayNotification)
END_MESSAGE_MAP			()


BOOL CgcLibrarianDlg::OnInitDialog()
{	
	{
		CDialogEx::OnInitDialog();

		ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
		ASSERT(IDM_ABOUTBOX < 0xF000);

		CMenu* pSysMenu = GetSystemMenu(FALSE);
		if (pSysMenu != NULL)
		{
			BOOL bNameValid;
			CString strAboutMenu;
			bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
			ASSERT(bNameValid);
			if (!strAboutMenu.IsEmpty())
			{
				pSysMenu->AppendMenu(MF_SEPARATOR);
				pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
			}
		}

		SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
		SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.
	}

	//========================================================================================
	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	//========================================================================================
	this->TRAY_Install	();
	this->SetTimer		(NULL, DELAY_150ms, 0);
	this->ResetSequence	(gcFileDOG);

	
	return FALSE;
}


//============================================================================================
// UI Functions
//============================================================================================
void CgcLibrarianDlg::BtnQUIT	()
{
	gcFileDOG.Release_HEAP();
	this->Release_HEAP();
	CDialogEx::OnCancel();
}
void CgcLibrarianDlg::BtnRESET	()
{
	this->ResetSequence(gcFileDOG);
}
void CgcLibrarianDlg::BtnCLEAR	()
{
	
	this->ClearEditCTRL(); UpdateData(FALSE);
	//CDialogEx::OnOK();
}
void CgcLibrarianDlg::BtnCOPY	()
{
	for (uint i = 0; i < gcFileDOG.FDog_MAXpIDX; ++i)
		gcFileDOG.FDog_peManualCOPY[i].SetEvent();

	Sleep(DELAY_250ms); g_print += L"\r\n\r\n";
}
void CgcLibrarianDlg::OnTimer	(UINT_PTR nIDEvent)
{
	/*----------------------------------------------------------------------------------------
		Print Changes in Edit Control
	----------------------------------------------------------------------------------------*/
	
		m_print = g_print;

		if (m_print_rsv != m_print)	//On Difference
		{
			m_print_rsv = m_print; UpdateData(FALSE);
			m_EditCTRL.LineScroll(m_EditCTRL.GetLineCount());
		}
	

	CDialogEx::OnTimer(nIDEvent);
}


//============================================================================================
// Untouched MFC
//============================================================================================
HCURSOR CgcLibrarianDlg::OnQueryDragIcon()
{
	// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
	//  �� �Լ��� ȣ���մϴ�.

	return static_cast<HCURSOR>(m_hIcon);
}


//============================================================================================
// Graphics
//============================================================================================
BOOL	CgcLibrarianDlg::OnEraseBkgnd		(CDC* pDC)
{
	return TRUE;
	//return CDialogEx::OnEraseBkgnd(pDC);
}
void	CgcLibrarianDlg::OnPaint			()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ�Դϴ�.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}
void	CgcLibrarianDlg::OnSysCommand		(uint nID, LPARAM lParam)
{
	if (nID == SC_CLOSE)
	{
		m_TrayCTRL.ToggleSHOW();
	}
	else if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}
inline LRESULT CgcLibrarianDlg::OnTrayNotification(WPARAM wParam, LPARAM lParam)
{
	switch (lParam)
	{
		case WM_LBUTTONUP:
		{
			ZeroMemory			( &m_nid, sizeof(m_nid)	);
			
			m_TrayCTRL.ToggleSHOW();
			break;
		}	

		case WM_RBUTTONUP:
		{
			this->BtnCOPY();
			break;
		}
	}

	return TRUE;
}
inline void CgcLibrarianDlg::TRAY_Install()
{
	//------------------------------------------------------------------------------
	// MFC GIVER
	//------------------------------------------------------------------------------
	const	CString		Tip				= gcLibrarian;
	const	uint		CallBackMSG		= WM_TRAY_NOTIFICATION;
	const	HICON		Icon			= AfxGetApp()->LoadIcon(IDI_ICON1);
	const	HWND		hMainWnd		= AfxGetApp()->m_pMainWnd->GetSafeHwnd();
			CWnd*		pMainWnd		= AfxGetApp()->m_pMainWnd;
			uint*		pbTRAY			= &(this->m_bTRAY);


	//------------------------------------------------------------------------------
	// CALL
	//------------------------------------------------------------------------------
	m_TrayCTRL.Initialize(Tip, CallBackMSG, Icon, hMainWnd, pMainWnd, pbTRAY);
	m_TrayCTRL.HIDE_MainWnd();
}