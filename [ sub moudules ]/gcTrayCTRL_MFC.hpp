/*==================================================================================

	"gcTrayCTRL.hpp"

	Dependency: Windows x86, MFC, UNICODE.
	
	HyoSeung Robinus Baskerville Kang.
	May.23.2016.

//----------------------------------------------------------------------------------

	>> READ the MANUAL at the BOTTOM of this HEADER <<

==================================================================================*/
#ifdef GCTRAYCTRL_MFC_EXPORTS
	#define GCTRAYCTRL_MFC_API __declspec(dllexport)
#else
	#define GCTRAYCTRL_MFC_API __declspec(dllimport)
#endif


#pragma once
#include <queue>
//===================================================================================
// CgcTrayCTRL_MFC
//===================================================================================
class GCTRAYCTRL_MFC_API CgcTrayCTRL_MFC 
{

public:

	 CgcTrayCTRL_MFC()	{					}
	~CgcTrayCTRL_MFC()	{ this->SafeEXIT(); }
		
	CgcTrayCTRL_MFC
	(
		const CString& Tip, const uint& CallBackMSG,
		const HICON& Icon, const HWND& hMainWnd, CWnd* pMainWnd, uint* pbTRAY		

	){	Initialize(Tip, CallBackMSG, Icon, hMainWnd, pMainWnd, pbTRAY);			}


	//===============================================================================
	// INTERFACE: TRAY ICON
	//===============================================================================
	void Initialize(

		const	CString&	Tip,
		const	uint&		CallBackMSG,
		const	HICON&		Icon,
		const	HWND&		hMainWnd,
				CWnd*		pMainWnd,
				uint*		pbTRAY
		);

	void ToggleSHOW();
	void HIDE_MainWnd();
	void SHOW_MainWnd();


	//===============================================================================
	// INTERFACE: BALLOON
	//===============================================================================
	void TrayBalloon(const uint& ShowPeriod, const CString&	Caption, const CString&	Text );
	void SafeEXIT();

	friend uint tTrayBalloon	(LPVOID pParam);


private:
	NOTIFYICONDATA		m_nid;
	HWND				m_hMainWnd;
	CWnd*				m_pMainWnd;
	uint*				m_pbTRAY;
	CString				m_VoidSTR = L"";
	std::queue<uint>	m_qShowPeriod;


	void Set_INFO(const CString&	Caption, const CString&	Text);
	void Del_INFO();
};


//==================================================================================
// MANUAL: gcTrayCTRL
//==================================================================================
/*

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~	
// 01. yourMFC.h		>> ADD FOLLOWINGS as Member of CyourMFC <<
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
{

public:
	LRESULT			OnTrayNotification(WPARAM wParam, LPARAM lParam);

protected:
	
	CgcTrayCTRL_MFC m_TrayCTRL;
	NOTIFYICONDATA	m_nid;
	UINT			m_bTRAY;

	void			TRAY_Install();

}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// 02. yourMFC.cpp		>> ADD FOLLOWINGS <<
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//----------------------------------------------------------------------------------
// EXAMPLE. MESSAGE MAP
//----------------------------------------------------------------------------------

//BEGIN_MESSAGE_MAP(yourMFC, CDialogEx)

	ON_MESSAGE(WM_TRAY_NOTIFICATION, OnTrayNotification)

//END_MESSAGE_MAP()


//----------------------------------------------------------------------------------
// EXAMPLE. MESSAGE CALLBACK FUNCTION
//----------------------------------------------------------------------------------
inline LRESULT CyourMFC::OnTrayNotification(WPARAM wParam, LPARAM lParam)
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
			m_TrayCTRL.TrayBalloon(1800, L"gcTrayCTRL", L": Hi, there.");
			break;
		}
	}

	return TRUE;
}


//----------------------------------------------------------------------------------
// EXAMPLE. INITIALIZER
//----------------------------------------------------------------------------------
inline void CyourMFC::TRAY_Install()
{
	//------------------------------------------------------------------------------
	// MFC GIVER
	//------------------------------------------------------------------------------
	const	CString		Tip				= gcLibrarian;
	const	uint		CallBackMSG		= WM_TRAY_NOTIFICATION;
	const	HICON		Icon			= AfxGetApp()->LoadIcon(yourICON);
	const	HWND		hMainWnd		= AfxGetApp()->m_pMainWnd->GetSafeHwnd();
			CWnd*		pMainWnd		= AfxGetApp()->m_pMainWnd;
			uint*		pbTRAY			= &(this->m_bTRAY);


	//------------------------------------------------------------------------------
	// CALL
	//------------------------------------------------------------------------------
	m_TrayCTRL.Initialize(Tip, CallBackMSG, Icon, hMainWnd, pMainWnd, pbTRAY);
	m_TrayCTRL.HIDE_MainWnd();
}


//----------------------------------------------------------------------------------
// EXAMPLE. TOGGLE
//----------------------------------------------------------------------------------
void CyourMFC::OnBnClickedOk()
{
	m_TrayCTRL.ToggleSHOW();

	//CDialogEx::OnOK();
}


*/

//==================================================================================
//																		End of Line
//==================================================================================
