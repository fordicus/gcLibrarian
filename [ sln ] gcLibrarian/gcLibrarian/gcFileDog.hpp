#pragma once
#include <gcDEFINE.hpp>
#include <Windows.h>
#include <afxmt.h>
#include <atlstr.h>
#include <sstream>
#include <strsafe.h>
#include <vector>
using namespace std;


//============================================================================================
// GLOBAL DEFINE
//============================================================================================
#define IDX_tFileDOG		0
#define IDX_tLibrarian		1
#define IDX_tSecretary		2

enum FDogACTION : uint
{
	IDLE				=	0,
	ADDED				=	1,
	REMOVED				=	2,
	MODIFIED			=	3,
	RENAMED_OLD_NAME	=	4,
	RENAMED_NEW_NAME	=	5
};


//============================================================================================
// Global Variable
//============================================================================================
#pragma once
static CString				g_print;


//============================================================================================
// CgcLibrarian
//============================================================================================
class CgcLibrarian
{

public:

	CgcLibrarian()
	{
		/*=====================================================================================

									  THREAD_PRIORITY_TIME_CRITICAL
									  THREAD_PRIORITY_HIGHEST
									  THREAD_PRIORITY_ABOVE_NORMAL
									  THREAD_PRIORITY_NORMAL
									  THREAD_PRIORITY_BELOW_NORMAL
									  THREAD_PRIORITY_LOWEST
									  THREAD_PRIORITY_IDLE

		=====================================================================================*/
		this->FDog_THREAD_PRIORITY	= THREAD_PRIORITY_NORMAL;

		this->FDog_tMS				= 0;	//Determination Equation to FIND ( END of COMPILE )
		this->FDog_nTIMES			= 0;	// = FDog_tMS x FDog_nTIMES

		this->FDog_bVERBOSE			= TRUE;	// TOGGLE ( Reporting in Edit Control )


		this->tLoop					= TRUE;

		this->FDog_ppeKILLS			= NULL;
		this->FDog_peExREQUEST		= NULL;
		this->FDog_peExRETURN		= NULL;
		this->FDog_peBeginCOPY		= NULL;

		this->FDog_pIDX				= NULL;
		this->FDog_lpYARD			= NULL;
		this->FDog_pYARD			= NULL;
		this->FDog_ppEXPORT			= NULL;
		this->FDog_ppIMPORT			= NULL;
		this->FDog_pszSTACK			= NULL;		

		this->FDog_VOID_vUINT.push_back(0);
		this->FDog_VOID_vCSTRING.push_back(L"");

		this->FDog_nCopyFiles		= 0;
		this->FDog_MAXpIDX			= 0;

	};
	~CgcLibrarian(){};
	
	void Begin_THREAD();
	void Release_HEAP();

	#define FDogSummoner			L"FDog.gcRET"
	int								FDog_THREAD_PRIORITY;
	DWORD							FDog_tMS;
	uint							FDog_nTIMES;
	uint							FDog_bVERBOSE;

									//[0]tFileDOG, [1]tLibrarian, and [2]tSecretary.
	const uint						FDog_nThreadMembers = 3;
	
	CWinThread***					FDog_pppThread;
	CWinThread*						T[12];
	uint							tLoop;

	CgcTrayCTRL_MFC*				FDog_pTrayCTRL;

	FILE_NOTIFY_INFORMATION			fInfo[1024];
	FILE_NOTIFY_INFORMATION*		pfInfo = fInfo;

	CEvent**						FDog_ppeKILLS;			//Killing Threads on Program's Exit
	CEvent*							FDog_peExREQUEST;		//Stack Exchange: REQUEST
	CEvent*							FDog_peExRETURN;		//Stack Exchange: ALLOW
	CEvent*							FDog_peBeginCOPY;		//End of Compilation
	CEvent*							FDog_peManualCOPY;		//Manual Copy Operation
	
	uint							FDog_nCopyFiles;		//Number of Files to Copy
	uint							FDog_MAXpIDX;			//Number of Threads to Create
	uint*							FDog_pIDX;				//Each Threads has its own IDX
	LPCWSTR*						FDog_lpYARD;			//PATH: CreateFileW WinAPI
	CString*						FDog_pYARD;				//PATH: EditControl Print
	CString**						FDog_ppEXPORT;			//PATH: SOURCE
	CString**						FDog_ppIMPORT;			//PATH: DEST

	uint*							FDog_pFirstTICK;		//First Tick				@ACTION
	uint*							FDog_pszSTACK;			//Stack Size Notifier		@ACTION
	vector<uint>					FDog_vPrintCNT;			//Counting Actions			@ACTION
	vector<vector<uint>>			FDog_vvACTION;			//Stack for enum			@ACTION
	vector<vector<uint>>			FDog_vvTICK;			//Stack for Time			@ACTION
	vector<vector<CString>>			FDog_vvWHO;				//Stack for NAME of File	@ACTION

	vector<CString>					FDog_VOID_vCSTRING;		//vector initializer
	vector<uint>					FDog_VOID_vUINT;		//vector initializer		


	//========================================================================================
	// Helpers
	//========================================================================================
	void	to_carray_static	(char* to, CString& from);
	uint	GetTick				();
	CString	ParseAction			(const unsigned int& IDX_ACTION);
	void	VerbosFileAction	(CString& TargetSTR, const uint& myID, const uint& szREPORT);
	void	LibrarianCopiest	(const uint& szREPORT_ACC, const uint& myID, const uint& bCascade);
	
};
#pragma once
static CgcLibrarian	gcFileDOG;


//============================================================================================
// [0] IDX_tFileDOG
//============================================================================================
static uint tFileDOG(LPVOID pParam)
{
	//========================================================================================
	// INITIALIZATION
	//========================================================================================
	uint				myID		=  (uint)pParam;
	LPCWSTR			SearchDIR		=  gcFileDOG.FDog_lpYARD[myID];	
	const WCHAR*	SummonSignalW	=  L"FDog.gcRET";
	char SummonSignal[MAX_PATH];
	{
		/*
			char SummonSignal[]
			to call std::remove()
		*/

		CString CSTR = gcFileDOG.FDog_lpYARD[myID];
		CSTR += FDogSummoner;

		gcFileDOG.to_carray_static(SummonSignal, CSTR);
		
		fstream fClear(SummonSignal);
		if (fClear.is_open())
		{
			fClear.close();
			remove(SummonSignal);
		}
	}

	WCHAR			WHO[MAX_PATH]	= { 0 };
	vector<uint>	tFDog_vACTION;
	vector<uint>	tFDog_vTICK;
	vector<CString> tFDog_vWHO;


	//----------------------------------------------------------------------------------------
	// WinAPI IO: CreateFileW() & ReadDirectoryChangesW()
	//----------------------------------------------------------------------------------------
	HANDLE hDir = CreateFileW(	SearchDIR,		//Directory
		GENERIC_READ,							//Access
		FILE_SHARE_READ | FILE_SHARE_WRITE,		//Share
		0,										//Security
		OPEN_EXISTING,							//Create
		FILE_FLAG_BACKUP_SEMANTICS,				//Flag
		0										//Template
		);

	BOOL bWatchSubtree = FALSE;
	DWORD dwNotifyFilter =
		FILE_NOTIFY_CHANGE_FILE_NAME |
		FILE_NOTIFY_CHANGE_DIR_NAME |
		FILE_NOTIFY_CHANGE_ATTRIBUTES |
		FILE_NOTIFY_CHANGE_SIZE |
		FILE_NOTIFY_CHANGE_LAST_WRITE |
		FILE_NOTIFY_CHANGE_CREATION;

	DWORD szReturnBYTE;
	BOOL fOk = FALSE;


	//========================================================================================
	// EVENT HANDLE
	//========================================================================================
	const uint nCEvent	= 1;
	HANDLE EventLIST[nCEvent];
	EventLIST[0] = (HANDLE)gcFileDOG.FDog_peExREQUEST[myID];


	//========================================================================================
	// START: LOOP
	//========================================================================================
	while (gcFileDOG.tLoop)
	{
		DWORD eSignal
			= WaitForMultipleObjects(nCEvent, EventLIST, FALSE, DELAY_100ms);

		switch (eSignal)
		{
			case WAIT_OBJECT_0:	//FDog_peExREQUEST
			{
				if (gcFileDOG.tLoop)
				{
					gcFileDOG.FDog_peExREQUEST[myID].ResetEvent();
					{
						//TimeTICK @EVENT
						gcFileDOG.FDog_vvTICK[myID].clear();
						gcFileDOG.FDog_vvTICK[myID].assign(
							tFDog_vTICK.begin(), tFDog_vTICK.end());
						tFDog_vTICK.clear();

						//Origin O' EVENT
						gcFileDOG.FDog_vvWHO[myID].clear();
						gcFileDOG.FDog_vvWHO[myID].assign(
							tFDog_vWHO.begin(), tFDog_vWHO.end());
						tFDog_vWHO.clear();

						//Action O' EVENT
						gcFileDOG.FDog_vvACTION[myID].clear();
						gcFileDOG.FDog_vvACTION[myID].assign(
							tFDog_vACTION.begin(), tFDog_vACTION.end());
						tFDog_vACTION.clear();
					}
					gcFileDOG.FDog_peExRETURN[myID].SetEvent();	
				}

				break;
			}
		}
						

		//====================================================================================
		// ReadDirectoryChangesW: Synchronous Call
		//====================================================================================

		fOk = ReadDirectoryChangesW(hDir,
			(LPVOID)gcFileDOG.pfInfo, sizeof(gcFileDOG.fInfo),
			bWatchSubtree, dwNotifyFilter, &szReturnBYTE, 0, 0);

		if (!fOk) break;

		do
		{		
			StringCbCopyNW(WHO, sizeof(WHO),
				gcFileDOG.pfInfo->FileName,
				gcFileDOG.pfInfo->FileNameLength
				);

			if (wcscmp(WHO, SummonSignalW) == NULL)
			{
				goto EXIT;
			}

			switch (gcFileDOG.pfInfo->Action)				//Action O' EVENT
			{
				case FILE_ACTION_ADDED:
				{
					tFDog_vACTION.push_back(FDogACTION::ADDED);
					break;
				}
				case FILE_ACTION_REMOVED:
				{
					tFDog_vACTION.push_back(FDogACTION::REMOVED);
					break;
				}
				case FILE_ACTION_MODIFIED:
				{
					tFDog_vACTION.push_back(FDogACTION::MODIFIED);
					break;
				}
				case FILE_ACTION_RENAMED_OLD_NAME:
				{
					tFDog_vACTION.push_back(FDogACTION::RENAMED_OLD_NAME);
					break;
				}
				case FILE_ACTION_RENAMED_NEW_NAME:
				{
					tFDog_vACTION.push_back(FDogACTION::RENAMED_NEW_NAME);
					break;
				}
			}

			tFDog_vTICK.push_back(gcFileDOG.GetTick());	//Time	 O' EVENT
			tFDog_vWHO.push_back(WHO);					//Origin O' EVENT

			gcFileDOG.pfInfo
				= (FILE_NOTIFY_INFORMATION*)(
					(PBYTE)gcFileDOG.pfInfo +
					gcFileDOG.pfInfo->NextEntryOffset
					);			

		} while (gcFileDOG.pfInfo->NextEntryOffset > 0 && gcFileDOG.tLoop);
	}	
	//========================================================================================
	// END: LOOP
	//========================================================================================

	EXIT:
	{
		remove(SummonSignal);
		*(*(gcFileDOG.FDog_pppThread + myID) + IDX_tFileDOG) = NULL;
		return NULL;
	}
}


//============================================================================================
// [1] IDX_tLibrarian
//============================================================================================
static uint tLibrarian(LPVOID pParam)
{
	//========================================================================================
	// INITIALIZATION
	//========================================================================================	
	const uint	myID				= (uint)pParam;

	fstream SummonFDog;
	CString SearchDIR				=  gcFileDOG.FDog_pYARD[myID];
			SearchDIR				+= FDogSummoner;

	uint*	pszEventSTACK			=  gcFileDOG.FDog_pszSTACK + myID;
	uint	szREPORT_ACC			=  0;
	uint*	pFirstTICK				=  gcFileDOG.FDog_pFirstTICK + myID;


	//========================================================================================
	// EVENT HANDLE
	//========================================================================================
	const uint	nCEvent	= 3;
	HANDLE EventLIST[nCEvent];
	EventLIST[0] = (HANDLE)gcFileDOG.FDog_peExRETURN[myID];
	EventLIST[1] = (HANDLE)gcFileDOG.FDog_peBeginCOPY[myID];
	EventLIST[2] = (HANDLE)gcFileDOG.FDog_peManualCOPY[myID];
	

	//========================================================================================
	// START: LOOP
	//========================================================================================
	while (gcFileDOG.tLoop)
	{
		
		/*------------------------------------------------------------------------------------

			1. Request tFileDOG to		(Exchange Stack)	: CEvent FDog_peExREQUEST
			2. Wait for					(Returned Stack)	: CEvent FDog_peExRETURN
			3. Manual Copy Operation						: CEvent FDog_peManualCOPY

		------------------------------------------------------------------------------------*/

		gcFileDOG.FDog_peExREQUEST[myID].SetEvent();			//to FileDOG
		
		DWORD eSignal
			= WaitForMultipleObjects(nCEvent, EventLIST, FALSE, DELAY_500ms);

		switch (eSignal)
		{
			case WAIT_OBJECT_0:									//From tFileDOG
			{
				if (gcFileDOG.tLoop)
				{
					gcFileDOG.FDog_peExRETURN[myID].ResetEvent();

					register uint szREPORT
						= gcFileDOG.FDog_vvTICK[myID].size();

					if (szREPORT != 0 )
					{	
						if (gcFileDOG.FDog_bVERBOSE == TRUE)	//[0] PRINT
						{
							if (szREPORT_ACC == 0)
								*pFirstTICK = gcFileDOG.FDog_vvTICK[myID][0];
							
							gcFileDOG.VerbosFileAction(g_print, myID, szREPORT);
						}

						 szREPORT_ACC	+= szREPORT;			//[1] ACCUMULATION
						*pszEventSTACK	 = szREPORT_ACC;		//[2] TELL (tSecretary)
					}
				}

				break;
			}	// END: case WAIT_OBJECT_0:

			case WAIT_OBJECT_0 + 1:								//From tSecretary
			{	
				if (gcFileDOG.tLoop)
				{
					/*------------------------------------------------------------------------
						Clear First
					------------------------------------------------------------------------*/
					gcFileDOG.FDog_peBeginCOPY[myID].ResetEvent();
					*pszEventSTACK = 0, szREPORT_ACC = 0; *pFirstTICK = 0;
					gcFileDOG.FDog_vPrintCNT[myID] = 0;

					gcFileDOG.LibrarianCopiest(					//and Copy
						szREPORT_ACC, myID, FALSE);
				}

				break;

			}	// END: case WAIT_OBJECT_0 + 1:

			case WAIT_OBJECT_0 + 2:								//From Btn_<Manual Copy>
			{
				gcFileDOG.FDog_peManualCOPY[myID].ResetEvent();

				gcFileDOG.LibrarianCopiest(						//if( szREPORT_ACC == 0 )
					szREPORT_ACC, myID, TRUE);
				
				break;

			}	// END: case WAIT_OBJECT_0 + 2:

		}	//END: switch (eSignal)		
	}
	//========================================================================================
	// END: LOOP
	//========================================================================================


	/*----------------------------------------------------------------------------------------

		Temporary File Generation to MAKE ( ReadDirectoryChangesW API ) to RETURN
		inside tFileDOG. This trick is being used to stick to the synchronous implementation.

	----------------------------------------------------------------------------------------*/
	
		SummonFDog.open(SearchDIR, ios::out | ios::trunc);
		if (SummonFDog.is_open()) SummonFDog.close();
	

	*(*(gcFileDOG.FDog_pppThread + myID) + IDX_tLibrarian) = NULL;
	return NULL;
}


//============================================================================================
// [2] IDX_tSecretary
//============================================================================================
static uint tSecretary(LPVOID pParam)
{
	//========================================================================================
	// INITIALIZATION
	//========================================================================================
	const uint myID					= (uint)pParam;

	uint*	pszEventSTACK			=  gcFileDOG.FDog_pszSTACK		+ myID;
	CEvent* peBeginCOPY				=  gcFileDOG.FDog_peBeginCOPY	+ myID;
	uint	szEventSTACK_RSV		=  0;
	uint	szEventSTACK_DIFF		=  0;


	//========================================================================================
	// START: LOOP
	//========================================================================================
	while (gcFileDOG.tLoop)
	{
		Sleep(gcFileDOG.FDog_tMS);
		
		if(*pszEventSTACK != 0)
		{
			if (szEventSTACK_DIFF >= gcFileDOG.FDog_nTIMES)	//Compile Finished
			{
				peBeginCOPY->SetEvent();
				szEventSTACK_DIFF	= 0;
				szEventSTACK_RSV	= 0;
			}
			else if (szEventSTACK_RSV != *pszEventSTACK)	//Compile On-going
			{
				szEventSTACK_RSV	= *pszEventSTACK;
				szEventSTACK_DIFF	= 0;
			}
			else if (szEventSTACK_RSV == *pszEventSTACK)	//Is it really finish?
			{
				++szEventSTACK_DIFF;
			}
		}		
	}
	//========================================================================================
	// END: LOOP
	//========================================================================================

	*(*(gcFileDOG.FDog_pppThread + myID) + IDX_tSecretary) = NULL;
	return NULL;
}


//============================================================================================
// Critical Section
//============================================================================================
inline void		CgcLibrarian::Begin_THREAD()
{
	this->tLoop = TRUE;
	uint* pIDX = this->FDog_pIDX;
	CWinThread*** pppThread = gcFileDOG.FDog_pppThread;

	for (uint i = 0; i < this->FDog_MAXpIDX; ++i)
	{
		CWinThread**ppThread = *pppThread++;
		this->FDog_THREAD_PRIORITY;
		*ppThread++ = AfxBeginThread(tFileDOG, (LPVOID)(*pIDX), FDog_THREAD_PRIORITY);
		*ppThread++ = AfxBeginThread(tLibrarian, (LPVOID)(*pIDX), FDog_THREAD_PRIORITY);
		*ppThread++ = AfxBeginThread(tSecretary, (LPVOID)(*pIDX++), FDog_THREAD_PRIORITY);

	}	pppThread = gcFileDOG.FDog_pppThread;

#ifdef _DEBUG
	//CString ADDR;
	//CString ADDR_ACC;
	//for (uint i = 0; i < this->FDog_MAXpIDX; ++i)
	//{
	//	CWinThread**ppThread = *pppThread++;

	//	for (uint j = 0; j < this->FDog_nThreadMembers; ++j)
	//	{
	//		ADDR.Format(L"Thread[%d][%d] @0x%X\r\n", i, j, (int)*ppThread++);
	//		ADDR_ACC += ADDR;
	//	}
	//}
	//MessageBox(NULL, ADDR_ACC, gcLibrarian, 0);
#endif
}
inline void		CgcLibrarian::Release_HEAP()
{
	//----------------------------------------------------------------------------------------
	// Finish Threads First
	//----------------------------------------------------------------------------------------
	this->tLoop = FALSE;
	CWinThread*** pppThread = this->FDog_pppThread;
	{
		register int pThread_ACC = NULL;

		while (TRUE)
		{
			Sleep(5);

			for (uint i = 0; i < this->FDog_MAXpIDX; ++i)
			{
				CWinThread**ppThread = *pppThread++;

				for (uint j = 0; j < gcFileDOG.FDog_nThreadMembers; ++j)
					pThread_ACC += (int)*ppThread++;
			}

#ifdef _DEBUG
			//{
			//	CString to_print, conv;
			//	CWinThread*** pppThread = gcFileDOG.FDog_pppThread;

			//	for (uint i = 0; i < gcFileDOG.FDog_MAXpIDX; ++i)
			//	{
			//		CWinThread**ppThread = *pppThread++;

			//		for (uint j = 0; j < gcFileDOG.FDog_nThreadMembers; ++j)
			//		{
			//			conv.Format(L"\r\n\t\t\tThread[%d][%d] 0x%X", i, j, (int)*ppThread++);
			//			to_print += conv;
			//		}
			//	}
			//	MessageBox(NULL, to_print, gcLibrarian, 0);
			//}
#endif

			if (pThread_ACC == NULL) break;
			else
			{
				pThread_ACC = NULL;
				pppThread = this->FDog_pppThread;
			}

		}	// All the Threads RETURNED

	}	  pppThread = this->FDog_pppThread;
	

	//----------------------------------------------------------------------------------------
	// Release Heaps
	//----------------------------------------------------------------------------------------
	if (FDog_peExREQUEST	!= NULL) { delete[]FDog_peExREQUEST;	FDog_peExREQUEST	= NULL; }
	if (FDog_peExRETURN		!= NULL) { delete[]FDog_peExRETURN;		FDog_peExRETURN		= NULL; }
	if (FDog_peBeginCOPY	!= NULL) { delete[]FDog_peBeginCOPY;	FDog_peBeginCOPY	= NULL; }
	if (FDog_peManualCOPY	!= NULL) { delete[]FDog_peManualCOPY;	FDog_peManualCOPY	= NULL; }
	
	if (FDog_pIDX			!= NULL) { delete[]FDog_pIDX;			FDog_pIDX			= NULL; }
	if (FDog_lpYARD			!= NULL) { delete[]FDog_lpYARD;			FDog_lpYARD			= NULL; }
	if (FDog_pYARD			!= NULL) { delete[]FDog_pYARD;			FDog_pYARD			= NULL; }
	if (FDog_pszSTACK		!= NULL) { delete[]FDog_pszSTACK;		FDog_pszSTACK		= NULL; }

	if (FDog_pFirstTICK		!= NULL) { delete[]FDog_pFirstTICK;		FDog_pFirstTICK		= NULL; }

	if (FDog_ppEXPORT		!= NULL)
	{
		CString** ppEXPORT = FDog_ppEXPORT;

		for (uint i = 0; i < FDog_MAXpIDX; ++i)
		{
			CString* pEXPORT = *ppEXPORT++;

			if ((pEXPORT)	!= NULL)
			{
				delete[]pEXPORT;
				*(FDog_ppEXPORT + i) = NULL;
			}
		}

		delete[]FDog_ppEXPORT;
		FDog_ppEXPORT = NULL;
	}
	if (FDog_ppIMPORT		!= NULL)
	{
		CString** ppIMPORT = FDog_ppIMPORT;

		for (uint i = 0; i < FDog_MAXpIDX; ++i)
		{
			CString* pIMPORT = *ppIMPORT++;

			if ((pIMPORT)	!= NULL)
			{
				delete[]pIMPORT;
				*(FDog_ppIMPORT + i) = NULL;
			}
		}

		delete[]FDog_ppIMPORT;
		FDog_ppIMPORT = NULL;
	}
	if (FDog_ppeKILLS		!= NULL)
	{
		CEvent** ppeKILLS = FDog_ppeKILLS;

		for (uint i = 0; i < FDog_MAXpIDX; ++i)
		{
			CEvent* peKILLS = *ppeKILLS++;

			if ((peKILLS)	!= NULL)
			{
				delete[]peKILLS;
				*(FDog_ppeKILLS + i) = NULL;
			}
		}

		delete[]FDog_ppeKILLS;
		FDog_ppeKILLS = NULL;
	}


	//----------------------------------------------------------------------------------------
	// Quench Thread Pointers
	//----------------------------------------------------------------------------------------
	for (register uint i = 0; i < this->FDog_MAXpIDX; ++i)
	{
		CWinThread**ppThread = *pppThread++;
		delete[]ppThread;
	}

	delete[]this->FDog_pppThread;
	this->FDog_pppThread = NULL;

}


//============================================================================================
// Helpers
//============================================================================================
inline void		CgcLibrarian::to_carray_static	(char* to, CString& from)
{
	wchar_t* tmpW = from.GetBuffer(from.GetLength());
	int      len = WideCharToMultiByte(CP_ACP, 0, tmpW, -1, NULL, 0, NULL, NULL);

	char* tmp = new char[len];
	WideCharToMultiByte(CP_ACP, 0, tmpW, -1, tmp, len, 0, 0);

	char* pTo = to;
	char* pTmp = tmp;
	for (register int i = 0; i < len; ++i)
	{
		*pTo++ = *pTmp++;
	}
	*pTo = NULL;
	delete[]tmp;
}
inline uint		CgcLibrarian::GetTick				()
{
	LARGE_INTEGER q, w; double Tick;
	::QueryPerformanceCounter(&q);
	::QueryPerformanceFrequency(&w);

	Tick = (q.QuadPart*1000000.0) / (w.QuadPart);
	return (uint)Tick;
}
inline CString	CgcLibrarian::ParseAction			(const unsigned int& IDX_ACTION)
{
	CString res;
	switch (IDX_ACTION)
	{
		case FDogACTION::IDLE:				res = L"[ IDLE\t\t] "; break;
		case FDogACTION::ADDED:				res = L"[ ADDED\t\t] "; break;
		case FDogACTION::REMOVED:			res = L"[ REMOVED\t] "; break;
		case FDogACTION::MODIFIED:			res = L"[ MODIFIED\t] "; break;
		case FDogACTION::RENAMED_OLD_NAME:	res = L"[ RENAMED_OLD\t] "; break;
		case FDogACTION::RENAMED_NEW_NAME:	res = L"[ RENAMED_NEW\t] "; break;

	}	return res;
}
inline void		CgcLibrarian::VerbosFileAction	(CString& TargetSTR, const uint& myID, const uint& szREPORT)
{
	#define Alligned9		ss << "[ " << right << setw(9) << setfill('_')
	#define TimeOffset		to_string( (FDog_vvTICK[myID][i]  - FirstTICK) / 1000 ) << "ms ]"
	#define TimeStrGEN()	Alligned9 << TimeOffset

	#define Alligned4		ss << "[ " << right << setw(4) << setfill('_')
	#define ActionCNT		to_string( ++FDog_vPrintCNT[myID] ) << " ] "
	#define CntStrGEN()		Alligned4 << ActionCNT

	#define PushSTREAM()	TargetSTR += to_cstring(ss.str()); clear_ss(ss)
	

	stringstream ss;
	register uint FirstTICK = *(gcFileDOG.FDog_pFirstTICK + myID);	
	
	for (register uint i = 0; i < szREPORT; ++i)
	{	

		TargetSTR += ParseAction(FDog_vvACTION[myID][i]);		//< ACTION		>			
		TimeStrGEN(),	PushSTREAM();							//< TICK		>
		CntStrGEN(),	PushSTREAM();							//< OFFSET		>
		TargetSTR +=											//< YARD + WHO	>
			(	*(this->FDog_pYARD + myID)
				+ this->FDog_vvWHO[myID][i]						+ L"\r\n"
			);
	}

	#undef Alligned9
	#undef TimeOffset
	#undef TimeStrGEN
	#undef Alligned4
	#undef ActionCNT
	#undef CntStrGEN
	#undef PushSTREAM
}
inline void		CgcLibrarian::LibrarianCopiest	(const uint& szREPORT_ACC, const uint& myID, const uint& bCascade)
{
	/*------------------------------------------------------------------------
		COPY FILES ( .dll || .lib || .cpp || .hpp )
	------------------------------------------------------------------------*/
	if(szREPORT_ACC == 0)
	{
		CString*	pEXPORT = *(this->FDog_ppEXPORT + myID);
		CString*	pIMPORT = *(this->FDog_ppIMPORT + myID);

		for (register uint i = 0; i < this->FDog_nCopyFiles; ++i)
			CopyFile(*pEXPORT++, *pIMPORT++, FALSE);
		
		CString common		= L"COMMIT ~¢¾\t";
		CString to_print	= (common + this->FDog_pYARD[myID]);
		this->FDog_pTrayCTRL->TrayBalloon(1800, gcLibrarian, common);

		if(bCascade) g_print += (to_print + L"\r\n");
		else		 g_print += (L"\r\n" + to_print + L"\r\n\r\n\r\n");
	}
}
#undef IDX_tFileDOG
#undef IDX_tLibrarian
#undef IDX_tSecretary
#undef FDogSummoner