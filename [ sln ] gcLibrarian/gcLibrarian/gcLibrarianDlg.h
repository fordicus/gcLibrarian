#pragma once
//#include <gcSTRING.hpp>
#include <iomanip>
#include "gcFileDog.hpp"
#include "afxwin.h"

class CgcLibrarianDlg : public CDialogEx
{
// 생성입니다.
public:
	CgcLibrarianDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GCLIBRARIAN_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


//============================================================================================
// 구현입니다.
//============================================================================================


/*--------------------------------------------------------------------------------------------
	CRITICAL DEFINES
--------------------------------------------------------------------------------------------*/
#define nDebugRelease				 2
#define nDllLib						 2
#define nCppHpp						 2
#define nCopyFiles					(nDllLib		+ nCppHpp		)
#define nImportPerNAME				(nDebugRelease	* nCopyFiles	)
#define nExportPerNAME				(m_gcNAMES_CNT	* nImportPerNAME)

#define nL1append					 3
#define nL2append					(2				* nL1append		)


/*--------------------------------------------------------------------------------------------
	TRIVIAL DEFINES
--------------------------------------------------------------------------------------------*/
#define ClearMM()					m_message = L""
#define StateLine clear_ss(m_ss);	m_ss << setfill('-') << left << setw(134)
#define WM_TRAY_NOTIFICATION		WM_APP + 1									///Tray ICON


public:

	vector<CString> SecureFolders			(const CString& yard, const CString& MATCH);
	void			ExtrudeLayers			(const uint& Verbose_LEVEL = 0);
	void			ExtrudeLayer_00			(const uint& Verbose_LEVEL = 0);
	void			ExtrudeLayer_Common		(const uint& Verbose_LEVEL = 0);
	void			ExtrudeLayer_01			(const uint& Verbose_LEVEL = 0);	
	void			ExtrudeLayer_CopyFrom	(const uint& Verbose_LEVEL = 0);
	void			ExtrudeLayer_CopyTo		(const uint& Verbose_LEVEL = 0);

	void			Release_HEAP			();
	void			ReadyFDog				(CgcLibrarian& FileDOGW, const int& Verbose_LEVEL = FALSE);
	void			GoFDog					(CgcLibrarian& gcFileDOG);
	void			Load_tINI				(const char* tINI_CODEX_TITLE);
	void			ResetSequence			(CgcLibrarian& gcFileDOG);

	void			ClearEditCTRL			(){ g_print = L""; m_print = L""; }
	LRESULT			OnTrayNotification		(WPARAM wParam, LPARAM lParam);


protected:
	
	const char* tINI_CODEX_TITLE	= "gcLibraryCODEX.gctINI";
	CString tINI_NAME_CONVENTION	= L"gc";
	CString tINI_EXPORT_slnDLL		= L"C:\\``gcLibrary\\``slnDLL\\";
	CString tINI_IMPORT_include		= L"C:\\``gcLibrary\\``include\\";
	uint	tINI_tMS				= 500;
	uint	tINI_nTIMES				= 4;
	uint	tINI_VERBOSE_LEVEL		= 2;
	
	CString m_L1append[nL1append] = {

		L"Debug\\",
		L"Release\\",
		L"" //gcSTRING, ...
	};

	CString m_L2append[nL2append] = {

		L"d.dll",	L"d.lib",	//Debug
		L".dll",	L".lib",	//Release
		L".cpp",	L".hpp"		//Code
	};

	HICON						m_hIcon;
	CString						m_caption;
	CString						m_message;
	stringstream				m_ss;

	CgcTrayCTRL_MFC				m_TrayCTRL;
	NOTIFYICONDATA				m_nid;
	UINT						m_bTRAY;

	void						TRAY_Install();


	fstream						m_gcLibraryCODEX;

	uint						m_Threads_CNT;
	uint						m_gcNAMES_CNT;
	vector<CString>				m_gcNAMES;				
	vector<CString>				m_EXPORT_gcL0;
	vector<CString>				m_EXPORT_gcL1_DR;
	vector<vector<CString>>		m_EXPORT_gcL1;
	CString**					m_CommonLayer		= NULL;
	CString**					m_EXPORT_CopyFrom	= NULL;
	CString**					m_EXPORT_CopyTo		= NULL;
	
		
	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(uint nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()


public:

	CString m_print, m_print_rsv;
	CEdit	m_EditCTRL;

	afx_msg void BtnQUIT		();						//Terminate Program
	afx_msg void BtnRESET		();
	afx_msg void BtnCLEAR		();						//Sweep Edit Control

	afx_msg void OnTimer		(UINT_PTR nIDEvent);	//Redraw Edit Control
	
	afx_msg void BtnCOPY();
};


//============================================================================================
// Initialization
//============================================================================================
inline vector<CString>	CgcLibrarianDlg::SecureFolders			(const CString& yard, const CString& MATCH)
{
	vector<CString> LIST;

	CFileFind dog;
	int bTry = dog.FindFile(yard);

	while (bTry)
	{
		bTry = dog.FindNextFile();
		if (dog.IsDirectory() == TRUE)
		{
			CString _DIR = dog.GetFileName();

			if (_DIR == _T(".") ||
				_DIR == _T("..")
				) continue;

			CString DIR = dog.GetFileTitle();
			if (DIR.Find(MATCH) != -1) LIST.push_back(DIR);
		}
	}

	return LIST;
}
inline void				CgcLibrarianDlg::Load_tINI				(const char* tINI_CODEX_TITLE)
{
	m_gcLibraryCODEX.open(tINI_CODEX_TITLE, ios::in);

	if (!m_gcLibraryCODEX.is_open())
	{
		MessageBox(L"Unable to Find CODEX.", gcLibrarian, 0);
		OnCancel();
	}
	else
	{
		{
#define GetLine()	m_gcLibraryCODEX >> FileReader

			string FileReader;

			GetLine();			tINI_NAME_CONVENTION	= to_cstring(FileReader);
			GetLine();			tINI_EXPORT_slnDLL		= to_cstring(FileReader);
			GetLine();			tINI_IMPORT_include		= to_cstring(FileReader);
			m_gcLibraryCODEX >> tINI_tMS;
			m_gcLibraryCODEX >> tINI_nTIMES;
			m_gcLibraryCODEX >> tINI_VERBOSE_LEVEL;

			m_gcLibraryCODEX.close();

#undef GetLine
		}

		SetCaption("CODEX Secured.");
	}
}
inline void				CgcLibrarianDlg::ExtrudeLayers			(const uint& Verbose_LEVEL)
{
	this->ExtrudeLayer_00(Verbose_LEVEL);		SetCaption("ExtrudeLayer_00 Secured.");
	this->ExtrudeLayer_Common(Verbose_LEVEL);	SetCaption("ExtrudeLayer_Common Secured.");
	this->ExtrudeLayer_01(Verbose_LEVEL);		SetCaption("ExtrudeLayer_01 Secured.");
	this->ExtrudeLayer_CopyFrom(Verbose_LEVEL);	SetCaption("ExtrudeLayer_CopyFrom Secured.");
	this->ExtrudeLayer_CopyTo(Verbose_LEVEL);	SetCaption("ExtrudeLayer_CopyTo Secured.");
}
inline void				CgcLibrarianDlg::ExtrudeLayer_00		(const uint& Verbose_LEVEL)
{
	CString yard = tINI_EXPORT_slnDLL; yard += L"*.*";
	m_gcNAMES = this->SecureFolders(yard, tINI_NAME_CONVENTION);
	m_gcNAMES_CNT = m_gcNAMES.size();
	m_Threads_CNT = nDebugRelease * m_gcNAMES_CNT;

	for (register uint i = 0; i < m_gcNAMES_CNT; ++i)
	{
		CString L1(tINI_EXPORT_slnDLL); L1 += m_gcNAMES[i]; L1 += L"\\";
		m_EXPORT_gcL0.push_back(L1);
	}	

	if( Verbose_LEVEL == 1 || Verbose_LEVEL == 2)
	{
		ClearMM(); StateLine << "<Source Layer_00>";
		m_message += to_cstring(m_ss.str()); m_message += L"\r\n\r\n";

		for (register uint i = 0; i < m_gcNAMES.size(); ++i)
		{
			m_message += L"\t";
			m_message += m_EXPORT_gcL0[i];
			m_message += L"\r\n";
		}

		m_message += L"\r\n\r\n";
		g_print += m_message;
	}
}
inline void				CgcLibrarianDlg::ExtrudeLayer_Common	(const uint& Verbose_LEVEL)
{
	m_CommonLayer = new CString*[m_Threads_CNT];

	CString**	ppCommonLayer = m_CommonLayer;
	{
		for (uint i = 0; i < m_Threads_CNT; ++i)
			*ppCommonLayer++ = new CString[nCopyFiles];
	}
	ppCommonLayer = m_CommonLayer;

	{
		CString*	pNAME	= new CString[m_Threads_CNT];
		CString*	pEXT	= new CString[nImportPerNAME];
		CString*	ptrNAME	= pNAME;
		CString*	ptrEXT	= pEXT;

	//----------------------------------------------------------------------------------------	
	// STAGE: NAME
	//----------------------------------------------------------------------------------------
		{
			vector<CString>::iterator i;

			for (i = m_gcNAMES.begin(); i != m_gcNAMES.end(); ++i)
			{
				for (uint j = 0; j < nDebugRelease; ++j)
					*ptrNAME++ = *i;

			}
			ptrNAME = pNAME;

			if (Verbose_LEVEL == 2)
			{

				CString toConsole; uint itrLimit = m_gcNAMES_CNT * nDebugRelease;
				for (uint i = 0; i < itrLimit; ++i)
				{
					toConsole += *ptrNAME++; toConsole += L"\r\n";
				}
				ptrNAME = pNAME;
				//MessageBox(toConsole, gcLibrarian, 0); toConsole = L"";
			}
		}
		//------------------------------------------------------------------------------------	
		// STAGE: EXT
		//------------------------------------------------------------------------------------
		{
			CString* pm_L2append = m_L2append;
			CString* CPP = m_L2append + 4;
			CString* HPP = m_L2append + 5;
			for (register int i = 0; i < nDebugRelease; ++i)
			{
				*ptrEXT++ = *pm_L2append++;
				*ptrEXT++ = *pm_L2append++;
				*ptrEXT++ = *CPP;
				*ptrEXT++ = *HPP;
			}
			ptrEXT = pEXT;

			if (Verbose_LEVEL == 2)
			{

				CString toConsole; uint itrLimit = nImportPerNAME;
				for (uint i = 0; i < itrLimit; ++i)
				{
					toConsole += *ptrEXT++; toConsole += L"\r\n";
				}
				ptrEXT = pEXT;

				//MessageBox(toConsole, gcLibrarian, 0); toConsole = L"";
			}
		}
		//------------------------------------------------------------------------------------
		// STAGE: DR + NAME + EXT
		//------------------------------------------------------------------------------------
		{
			CString toConsole;
			CString* pm_L1append = m_L1append;
			for (uint i = 0; i < m_gcNAMES_CNT; ++i)
			{
				for (uint j = 0; j < nDebugRelease; ++j)
				{
					CString* sp = *ppCommonLayer++;
					for (uint k = 0; k < nCopyFiles; ++k)
					{
						switch (k)
						{
							case 0:	
							case 1: 
							{
								*sp = *pm_L1append;
								*sp += *ptrNAME;
								*sp += *ptrEXT++;
								break;
							}
							default:
							{
								*sp = *ptrNAME;
								*sp += *ptrEXT++;
								break;
							}
						}					

						if (Verbose_LEVEL == 2){
							toConsole += L"\t";
							toConsole += *sp;
							toConsole += L"\r\n";
						}
						++sp;
					}
					++pm_L1append;
					++ptrNAME;

					if (Verbose_LEVEL == 2) toConsole += L"\r\n";
				}

				//MessageBox(toConsole, gcLibrarian, 0); toConsole = L"";
				pm_L1append = m_L1append;
				ptrEXT = pEXT;
			}
			ptrNAME = pNAME;
			ppCommonLayer = m_CommonLayer;

			if (Verbose_LEVEL == 2)
			{
				ClearMM(); StateLine << "<Common_Layer>";
				m_message += to_cstring(m_ss.str()); m_message += L"\r\n\r\n";
				m_message += toConsole; m_message += L"\r\n";
				g_print += m_message;
			}
		}

		delete[]pNAME;
		delete[]pEXT;
	}
}
inline void				CgcLibrarianDlg::ExtrudeLayer_01		(const uint& Verbose_LEVEL)
{
	for (uint i = 0; i < m_gcNAMES_CNT; ++i)
	{
		m_L1append[nL1append-1] = m_gcNAMES[i];
		m_L1append[nL1append-1] += L"\\";								//"gcGENERIC\\"

		for (uint j = 0; j < nDebugRelease; ++j)						//Debug & Release Directories to WATCH
		{
			CString DRforThreads = m_EXPORT_gcL0[i] + m_L1append[j];
			m_EXPORT_gcL1_DR.push_back(DRforThreads);
		}
	}

	for (uint i = 0; i < m_gcNAMES_CNT; ++i)
	{
		for (uint j = 0; j < nDebugRelease; ++j)
		{
			m_L1append[nL1append - 1] = m_gcNAMES[i];
			m_L1append[nL1append - 1] += L"\\";
			
			vector<CString> ForSingleThread;
			CString Line = m_EXPORT_gcL0[i];

			for (uint k1 = 0; k1 < nDllLib; ++k1)
				ForSingleThread.push_back(Line);

			Line += m_gcNAMES[i]; Line += L"\\";

			for (uint k2 = 0; k2 < nCppHpp; ++k2)				
				ForSingleThread.push_back(Line);

			m_EXPORT_gcL1.push_back(ForSingleThread);
		}		
	}

	if (Verbose_LEVEL == 2)
	{
		ClearMM(); StateLine << "<Source Layer_01>";
		m_message += to_cstring(m_ss.str()); m_message += L"\r\n\r\n";

		for (register uint i = 0; i < m_Threads_CNT; ++i)
		{
			for (register int j = 0; j < nCopyFiles; ++j)
			{
				m_message += L"\t";
				m_message += m_EXPORT_gcL1[i][j];
				m_message += L"\r\n";
	
			}	m_message += L"\r\n";
	
		}
		m_message += L"\r\n";
		g_print += m_message;
	}

}
inline void				CgcLibrarianDlg::ExtrudeLayer_CopyFrom	(const uint& Verbose_LEVEL)
{
	m_EXPORT_CopyFrom = new CString*[m_Threads_CNT];

	CString**	ppm_EXPORT_CopyFrom = m_EXPORT_CopyFrom;
	{
		for (uint i = 0; i < m_Threads_CNT; ++i)
			*ppm_EXPORT_CopyFrom++ = new CString[nCopyFiles];
	}
		
	ppm_EXPORT_CopyFrom = m_EXPORT_CopyFrom;
	CString** ppCommonLayer =	m_CommonLayer;

	CString toConsole;

	for (uint i = 0; i < m_Threads_CNT; ++i)
	{	
		CString* pm_EXPORT_CopyFrom = *ppm_EXPORT_CopyFrom++;
		CString* pm_COMMON			= *ppCommonLayer++;

		for (uint j = 0; j < nCopyFiles; ++j)
		{
			*pm_EXPORT_CopyFrom		=  m_EXPORT_gcL1[i][j];
			*pm_EXPORT_CopyFrom		+= *pm_COMMON++;

			toConsole += L"\t";
			toConsole += *pm_EXPORT_CopyFrom++;
			toConsole += L"\r\n";
		}
		toConsole += L"\r\n";
	}
	//MessageBox(toConsole, gcLibrarian, 0); toConsole = L"";

	ppm_EXPORT_CopyFrom = m_EXPORT_CopyFrom;
	ppCommonLayer = m_CommonLayer;

	if (Verbose_LEVEL == 2)
	{
		ClearMM(); StateLine << "<Copy From>";
		m_message += to_cstring(m_ss.str()); m_message += L"\r\n\r\n";
		m_message += toConsole; m_message += L"\r\n";
		g_print += m_message;
	}
}
inline void				CgcLibrarianDlg::ExtrudeLayer_CopyTo	(const uint& Verbose_LEVEL)
{

	CString**	ppCommonLayer			= m_CommonLayer;

	m_EXPORT_CopyTo					= new CString*[m_Threads_CNT];
	CString**	ppm_EXPORT_CopyTo	= m_EXPORT_CopyTo;
	{
		for (uint i = 0; i < m_Threads_CNT; ++i)
			*ppm_EXPORT_CopyTo++ = new CString[nCopyFiles];

	}
	ppm_EXPORT_CopyTo				= m_EXPORT_CopyTo;

	
	CString toConsole;

	for (uint i = 0; i < m_Threads_CNT; ++i)
	{
		CString* pm_COMMON			= *ppCommonLayer++;
		CString* pm_EXPORT_CopyTo	= *ppm_EXPORT_CopyTo++;

		for (uint j = 0; j < nCopyFiles; ++j)
		{
			*pm_EXPORT_CopyTo =	  tINI_IMPORT_include;
			*pm_EXPORT_CopyTo += *pm_COMMON++;

			toConsole += L"\t";
			toConsole += *pm_EXPORT_CopyTo++;
			toConsole += L"\r\n";
		}

		toConsole += L"\r\n";
	}

	ppCommonLayer		= m_CommonLayer;
	ppm_EXPORT_CopyTo	= m_EXPORT_CopyTo;

	//MessageBox(toConsole, gcLibrarian, 0); toConsole = L"";	

	if (Verbose_LEVEL == 2)
	{
		ClearMM(); StateLine << "<Copy To>";
		m_message += to_cstring(m_ss.str()); m_message += L"\r\n\r\n";
		m_message += toConsole;	m_message += L"\r\n";
		g_print += m_message;
	}
}

inline void				CgcLibrarianDlg::Release_HEAP			()
{
	if (m_CommonLayer		!= NULL)
	{
		CString** ppCommonLayer = m_CommonLayer;

		for (uint i = 0; i < m_Threads_CNT; ++i)
		{
			CString* pm_COMMON = *ppCommonLayer++;

			if ((pm_COMMON) != NULL)
			{
				delete[]pm_COMMON;
				*(m_CommonLayer + i) = NULL;
			}
		}

		delete[]m_CommonLayer;
		m_CommonLayer = NULL;
	}
	if (m_EXPORT_CopyFrom	!= NULL)
	{
		CString** ppCommonLayer = m_EXPORT_CopyFrom;

		for (uint i = 0; i < m_Threads_CNT; ++i)
		{
			CString* pm_EXPORT_gcL2 = *ppCommonLayer++;
			if ((pm_EXPORT_gcL2) != NULL)
			{
				delete[]pm_EXPORT_gcL2;
				*(m_EXPORT_CopyFrom + i) = NULL;
			}
		}

		delete[]m_EXPORT_CopyFrom;
		m_EXPORT_CopyFrom = NULL;
	}
	if (m_EXPORT_CopyTo		!= NULL)
	{
		CString** ppm_EXPORT_CopyTo = m_EXPORT_CopyTo;

		for (uint i = 0; i < m_Threads_CNT; ++i)
		{
			CString* pm_EXPORT_CopyTo = *ppm_EXPORT_CopyTo++;
			if ((pm_EXPORT_CopyTo) != NULL)
			{
				delete[]pm_EXPORT_CopyTo;
				*(m_EXPORT_CopyTo + i) = NULL;
			}
		}

		delete[]m_EXPORT_CopyTo;
		m_EXPORT_CopyTo = NULL;
	}
}
inline void				CgcLibrarianDlg::ReadyFDog				(CgcLibrarian& gcFileDOG, const int& Verbose_LEVEL)
{

	gcFileDOG.FDog_tMS				= tINI_tMS;
	gcFileDOG.FDog_nTIMES			= tINI_nTIMES;
	gcFileDOG.FDog_MAXpIDX			= m_Threads_CNT;
	gcFileDOG.FDog_nCopyFiles		= nCopyFiles;
	gcFileDOG.FDog_pTrayCTRL		= &m_TrayCTRL;

	//----------------------------------------------------------------------------------------
	// Heap Allocation
	//----------------------------------------------------------------------------------------
	gcFileDOG.FDog_peExREQUEST		=	new CEvent	[gcFileDOG.FDog_MAXpIDX];
	gcFileDOG.FDog_peExRETURN		=	new CEvent	[gcFileDOG.FDog_MAXpIDX];
	gcFileDOG.FDog_peBeginCOPY		=	new CEvent	[gcFileDOG.FDog_MAXpIDX];
	gcFileDOG.FDog_peManualCOPY		=	new CEvent	[gcFileDOG.FDog_MAXpIDX];
	gcFileDOG.FDog_pIDX				=	new uint	[gcFileDOG.FDog_MAXpIDX];
	gcFileDOG.FDog_lpYARD			=	new LPCWSTR	[gcFileDOG.FDog_MAXpIDX];
	gcFileDOG.FDog_pYARD			=	new CString	[gcFileDOG.FDog_MAXpIDX];
	gcFileDOG.FDog_ppEXPORT			=	new CString*[gcFileDOG.FDog_MAXpIDX];
	gcFileDOG.FDog_ppIMPORT			=	new CString*[gcFileDOG.FDog_MAXpIDX];
	gcFileDOG.FDog_pszSTACK			=	new uint	[nL2append];   ///What??
	

	//----------------------------------------------------------------------------------------
	// [ FDog_pFirstTICK ]
	//----------------------------------------------------------------------------------------
	{
		gcFileDOG.FDog_pFirstTICK	=	new uint	[gcFileDOG.FDog_MAXpIDX];
		uint* pFirstTICK			=	gcFileDOG.FDog_pFirstTICK;

		for (uint i = 0; i < gcFileDOG.FDog_MAXpIDX; ++i)
			*pFirstTICK++ = 0;

	}


	//----------------------------------------------------------------------------------------
	// [ FDog_pppThread ]
	//----------------------------------------------------------------------------------------
	{
		gcFileDOG.FDog_pppThread	=	new CWinThread**[gcFileDOG.FDog_MAXpIDX];
		CWinThread*** pppThread		=	gcFileDOG.FDog_pppThread;

		for (uint i = 0; i < gcFileDOG.FDog_MAXpIDX; ++i)
		{
			*pppThread++ = new CWinThread*[gcFileDOG.FDog_nThreadMembers];
		}
		pppThread = gcFileDOG.FDog_pppThread;
	

		for (uint i = 0; i < gcFileDOG.FDog_MAXpIDX; ++i)
		{
			CWinThread**ppThread = *pppThread++;

			for (uint j = 0; j < gcFileDOG.FDog_nThreadMembers; ++j)
				*ppThread++ = NULL;
		}
	}	

	//----------------------------------------------------------------------------------------
	// [ FDog_vvACTION, FDog_vvTICK, FDog_vvWHO ]
	//----------------------------------------------------------------------------------------
	{
		uint* pFileDOG_nEVENT_STACK = gcFileDOG.FDog_pszSTACK;
		for (uint i = 0; i < gcFileDOG.FDog_MAXpIDX; ++i)
		{
			*(pFileDOG_nEVENT_STACK++) = 0;
			gcFileDOG.FDog_vPrintCNT.push_back(0);

			gcFileDOG.FDog_vvACTION.push_back	(gcFileDOG.FDog_VOID_vUINT);
			gcFileDOG.FDog_vvTICK.push_back		(gcFileDOG.FDog_VOID_vUINT);
			gcFileDOG.FDog_vvWHO.push_back		(gcFileDOG.FDog_VOID_vCSTRING);		
		}
	}

	//----------------------------------------------------------------------------------------
	// [ FDog_ppEXPORT, FDog_ppIMPORT ]
	//----------------------------------------------------------------------------------------
	{
		for (uint i = 0; i < gcFileDOG.FDog_MAXpIDX; ++i)
		{
			*(gcFileDOG.FDog_ppEXPORT + i) = NULL;
			*(gcFileDOG.FDog_ppIMPORT + i) = NULL;
		}

		CString** ppEXPORT = gcFileDOG.FDog_ppEXPORT;
		CString** ppIMPORT = gcFileDOG.FDog_ppIMPORT;
		{
			for (uint i = 0; i < gcFileDOG.FDog_MAXpIDX; ++i)
			{
				*ppEXPORT++ = new CString[nCopyFiles];
				*ppIMPORT++	= new CString[nCopyFiles];
			}
		}
		ppEXPORT = gcFileDOG.FDog_ppEXPORT;
		ppIMPORT = gcFileDOG.FDog_ppIMPORT;


		CString** ppm_EXPORT_CopyFrom	= m_EXPORT_CopyFrom;
		CString** ppm_EXPORT_CopyTo		= m_EXPORT_CopyTo;
	
		CString toConsoleE, toConsoleI;

		for (uint i = 0; i < gcFileDOG.FDog_MAXpIDX; ++i)
		{
			CString* pEXPORT			= *ppEXPORT++;
			CString* pIMPORT			= *ppIMPORT++;
			CString* pm_EXPORT_CopyFrom	= *ppm_EXPORT_CopyFrom++;
			CString* pm_EXPORT_CopyTo	= *ppm_EXPORT_CopyTo++;

			for (uint j = 0; j < nCopyFiles; ++j)
			{
				*pEXPORT = *pm_EXPORT_CopyFrom++;
			
				toConsoleE += L"\t";
				toConsoleE += *pEXPORT++;
				toConsoleE += L"\r\n";
			

				*pIMPORT = *pm_EXPORT_CopyTo++;

				toConsoleI += L"\t";
				toConsoleI += *pIMPORT++;
				toConsoleI += L"\r\n";
			}

			toConsoleE += L"\r\n";
			toConsoleI += L"\r\n";
		}	
		toConsoleE += L"\r\n";
		toConsoleI += L"\r\n";
	
	
		if (Verbose_LEVEL == 2)
		{
			ClearMM(); StateLine << "<fDog EXPORT>";
			m_message += to_cstring(m_ss.str()); m_message += L"\r\n\r\n";
			m_message += toConsoleE;

			StateLine << "<fDog IMPORT>";
			m_message += to_cstring(m_ss.str()); m_message += L"\r\n\r\n";
			m_message += toConsoleI;

			g_print += m_message;
		}
	}

	//----------------------------------------------------------------------------------------
	// [ pIDX, lpYARD, pYARD ]
	//----------------------------------------------------------------------------------------	
	{
		uint*		pIDX		= gcFileDOG.FDog_pIDX;
		LPCWSTR*	lpYARD		= gcFileDOG.FDog_lpYARD;
		CString*	pYARD		= gcFileDOG.FDog_pYARD;

		for (register uint i = 0; i < (nL1append - 1) * m_gcNAMES_CNT; ++i)
		{
			*pIDX++				= i;
			*lpYARD++			= m_EXPORT_gcL1_DR[i];
			*pYARD++			= m_EXPORT_gcL1_DR[i];
		}
	}

	SetCaption("Grabbed WHISTLE.");
}
inline void				CgcLibrarianDlg::GoFDog					(CgcLibrarian& gcFileDOG)
{	
	gcFileDOG.Begin_THREAD();

	SetCaption("FileDOG Released.");
	ClearMM(); StateLine << "<Begin FileDog Sequence>";
	m_message += to_cstring(m_ss.str()); m_message += L"\r\n\r\n";
	g_print += m_message;
}
inline void				CgcLibrarianDlg::ResetSequence			(CgcLibrarian& gcFileDOG)
{
	this->Release_HEAP		();
	gcFileDOG.Release_HEAP	();
	this->Load_tINI			(this->tINI_CODEX_TITLE);
	this->ExtrudeLayers		(this->tINI_VERBOSE_LEVEL);

	this->ReadyFDog			(gcFileDOG, this->tINI_VERBOSE_LEVEL);
	this->GoFDog			(gcFileDOG);
}
