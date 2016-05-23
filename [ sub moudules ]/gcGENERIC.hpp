/*====================================================================================

	"gcGENERIC.HPP"

	Dependency: Windows x86

	HyoSeung Robinus Baskerville Kang.
	May.13.2016.

//==================================================================================

	gcQueryDesktopPath()
	gcQueryWinUserName()	
	
	gcQueryLocalTime()
	gcQueryLocalTimeCODE()
	gcTimeScaleUS()

	gcRand()
	gcRand_I_Uniform()
	gcRand_I_Gaussian()
	
====================================================================================*/
#ifdef GCGENERIC_EXPORTS
	#define GCGENERIC_API __declspec(dllexport)
#else
	#define GCGENERIC_API __declspec(dllimport)
#endif

#pragma once
#include <gcDEFINE.hpp>
#include <WtsApi32.h>					//gcQueryWinUserName()
#pragma comment(lib, "Wtsapi32.lib")	//gcQueryWinUserName()
#include <atlstr.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <chrono>
#include <random>
using namespace std;


//==================================================================================
// Start Exporting
//----------------------------------------------------------------------------------
// <random> https://msdn.microsoft.com/ko-kr/library/bb982398.aspx
//==================================================================================
GCGENERIC_API string			gcQueryDesktopPath();
GCGENERIC_API string			gcQueryWinUserName();

GCGENERIC_API inline string		SetCaptionTime			(const string& input);
#define SetCaption(input)		SetWindowText			(to_cstring(SetCaptionTime(input)))

GCGENERIC_API inline char*		gcQueryLocalTime		(const int& showMS = FALSE, const int& showDAY = FALSE);
GCGENERIC_API inline char*		gcQueryLocalTimeCODE	(void);
GCGENERIC_API inline char*		gcTimeScaleUS			(const double& elapse);

GCGENERIC_API inline UINT		gcRand					(const UINT& MaxVal = 0xFFU);
GCGENERIC_API inline int		gcRand_I_Uniform		(const int& MinVal, const int& max);
GCGENERIC_API inline int		gcRand_I_Gaussian		(const int& MeanVal, const int& VarianceVal);