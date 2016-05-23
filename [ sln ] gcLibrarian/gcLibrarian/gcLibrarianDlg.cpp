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

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
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

		SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
		SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.
	}

	//========================================================================================
	// TODO: 여기에 추가 초기화 작업을 추가합니다.
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
	// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
	//  이 함수를 호출합니다.

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
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
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