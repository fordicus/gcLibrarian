/*==================================================================================

	"gcTrayCTRL.cpp"

	Dependency: Windows x86, MFC, UNICODE.

	HyoSeung Robinus Baskerville Kang.
	May.23.2016.

==================================================================================*/
#include "stdafx.h"
#include "gcTrayCTRL_MFC.hpp"

#ifdef _DEBUG
	#define new DEBUG_NEW
#endif

CWinApp theApp;
using namespace std;

int main()
{
    int nRetCode = 0;

    HMODULE hModule = ::GetModuleHandle(nullptr);

    if (hModule != nullptr)
    {
        if (!AfxWinInit(hModule, nullptr, ::GetCommandLine(), 0))
        {
            wprintf(L"심각한 오류: MFC를 초기화하지 못했습니다.\n");
            nRetCode = 1;
        }
        else
        {
            // TODO: 응용 프로그램의 동작은 여기에서 코딩합니다.
        }
    }
    else
    {
        // TODO: 오류 코드를 필요에 따라 수정합니다.
        wprintf(L"심각한 오류: GetModuleHandle 실패\n");
        nRetCode = 1;
    }

    return nRetCode;
}


//===================================================================================
// INTERFACE: TRAY ICON
//===================================================================================
void CgcTrayCTRL_MFC::Initialize(

	const	CString&		  Tip,
	const	uint&			  CallBackMSG,
	const	HICON&			  Icon,
	const	HWND&			  hMainWnd,
			CWnd*			  pMainWnd,
			uint*			  pbTRAY
){
	m_hMainWnd				= hMainWnd;
	m_pMainWnd				= pMainWnd;
	m_pbTRAY				= pbTRAY;
	*m_pbTRAY				= FALSE;

	ZeroMemory				( &m_nid, sizeof(m_nid));

	m_nid.cbSize			= sizeof(m_nid);
	m_nid.hWnd				= m_hMainWnd;
	m_nid.uID				= 0;
	m_nid.uFlags			= NIF_TIP | NIF_ICON | NIF_MESSAGE | NIF_INFO;
	m_nid.uCallbackMessage	= CallBackMSG;
	m_nid.dwInfoFlags		= NIIF_ICON_MASK;
	m_nid.hIcon				= Icon;

	StringCbCopyNW(
		m_nid.szTip, sizeof(m_nid.szTip),
		Tip, Tip.GetLength() * sizeof(*Tip)	);

	BOOL bRet = ::Shell_NotifyIcon(NIM_ADD, &m_nid);
}


void CgcTrayCTRL_MFC::ToggleSHOW()
{
	if		(*m_pbTRAY == TRUE)		this->SHOW_MainWnd();
	else if (*m_pbTRAY == FALSE)	this->HIDE_MainWnd();
}


void	CgcTrayCTRL_MFC::HIDE_MainWnd()
{
	m_pMainWnd->ShowWindow(SW_HIDE);
	m_pMainWnd->ShowWindow(SW_SHOWMINIMIZED);
	PostMessage(m_hMainWnd, WM_SHOWWINDOW, FALSE, SW_OTHERUNZOOM);	*m_pbTRAY = TRUE;
}


void	CgcTrayCTRL_MFC::SHOW_MainWnd()
{
	PostMessage(m_hMainWnd, WM_SHOWWINDOW, TRUE, SW_OTHERUNZOOM);	*m_pbTRAY = FALSE;
}


//===================================================================================
// INTERFACE: BALLOON
//===================================================================================
void CgcTrayCTRL_MFC::TrayBalloon(
	
	const uint&		ShowPeriod,
	const CString&	Caption,
	const CString&	Text

){
	this->m_qShowPeriod.push(ShowPeriod);

	this->Set_INFO(Caption, Text);
	{
		Shell_NotifyIcon(NIM_MODIFY, &m_nid);
	}
	this->Del_INFO();

	AfxBeginThread(tTrayBalloon, (LPVOID)this, THREAD_PRIORITY_NORMAL);
}


void CgcTrayCTRL_MFC::SafeEXIT()
{
	while (TRUE)
	{
		if (m_qShowPeriod.size() != 0)	Sleep(5);
		else break;
	}
	Shell_NotifyIcon(NIM_DELETE, &m_nid);
}


void CgcTrayCTRL_MFC::Set_INFO(const CString& Caption, const CString& Text)
{
	#define oBufferBYTE(oBuffer) ( sizeof(oBuffer)							)
	#define iBufferBYTE(iBuffer) ( iBuffer.GetLength() * sizeof(*iBuffer)	)
	

	StringCbCopyNW(
		m_nid.szInfoTitle, oBufferBYTE(m_nid.szInfoTitle),
		Caption, iBufferBYTE(Caption));

	StringCbCopyNW(
		m_nid.szInfo, oBufferBYTE(m_nid.szInfo),
		Text, iBufferBYTE(Text));	
}


void CgcTrayCTRL_MFC::Del_INFO()
{	
	StringCbCopyNW(
		m_nid.szInfoTitle, oBufferBYTE(m_nid.szInfoTitle),
		m_VoidSTR, iBufferBYTE(m_VoidSTR));

	StringCbCopyNW(
		m_nid.szInfo, oBufferBYTE(m_nid.szInfo),
		m_VoidSTR, iBufferBYTE(m_VoidSTR));
	

	#undef oBufferBYTE
	#undef iBufferBYTE
}


//===================================================================================
// tTrayBalloon
//===================================================================================
static uint tTrayBalloon(LPVOID pParam)
{
	CgcTrayCTRL_MFC* pCgcTrayCTRL = (CgcTrayCTRL_MFC*)pParam;

	uint t = pCgcTrayCTRL->m_qShowPeriod.front();
	{
		Sleep(t);
	}
	pCgcTrayCTRL->m_qShowPeriod.pop();

	Shell_NotifyIcon(NIM_DELETE, &pCgcTrayCTRL->m_nid);	Sleep(1);
	Shell_NotifyIcon(NIM_ADD, &pCgcTrayCTRL->m_nid);

	return NULL;
}


//==================================================================================
//																		End of Line
//==================================================================================