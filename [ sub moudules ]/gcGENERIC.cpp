/*====================================================================================

	"gcGENERIC.CPP"

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
#pragma once
#include "stdafx.h"
#include "gcGENERIC.hpp"
#include <gcSTRING.hpp>


extern inline string PUSH2(const string& unit);
extern inline string PUSH3(const string& unit);
//==================================================================================
// SetCaptionTime: 
//==================================================================================
GCGENERIC_API inline string SetCaptionTime(const string& input)
{	
	stringstream ss;
	ss << "( " << gcQueryLocalTime(TRUE, FALSE) << " ) " << input;
	
	return ss.str();
}


//==================================================================================
// gcQueryDesktopPath: C:\\Users\\gcQueryWinUserName()\\Desktop\\
//==================================================================================
extern GCGENERIC_API string gcQueryWinUserName();
GCGENERIC_API string gcQueryDesktopPath()
{
	stringstream ss;
	ss << "C:\\Users\\" << gcQueryWinUserName() << "\\Desktop\\";

	return ss.str();
}

	GCGENERIC_API string gcQueryWinUserName()
	{
		DWORD dwCnt = 0;
		LPTSTR pStr = NULL;

		WTSQuerySessionInformation(
			WTS_CURRENT_SERVER_HANDLE,
			WTS_CURRENT_SESSION,
			WTSUserName,
			&pStr,
			&dwCnt
			);

		int length = sizeof(pStr) / sizeof(*pStr);
		int depth = _tcslen(pStr);
		LPTSTR pT = pStr;

		char* conv = new char[depth + 1];
		char* pConv = conv;

		if (length == 2)
		{
			for (int i = 0; i < length * depth; ++i)
				if (i % 2 == 0) *pConv++ = (char)*pT++;
		}
		else if (length == 1)
		{
			for (int i = 0; i < depth; ++i) *pConv++ = (char)*pT++;

		}	*pConv = NULL;

		WTSFreeMemory(pStr);
		string res = to_string(conv);
		delete[]conv;

		return res;
	}
	

//==================================================================================
// gcQueryLocalTime: May.07.2016. 20:32:48
//==================================================================================
GCGENERIC_API inline char* gcQueryLocalTime(const int& showMS, const int& showDAY)
{
	//--------------------------------------------------------------------------------
	SYSTEMTIME		systime; GetLocalTime(&systime);
	stringstream	ss;
	string			toPush	= "";
	string			zero	= "0";

	//--------------------------------------------------------------------------------
	switch (systime.wMonth)
	{
		case 1:		ss << "Jan."; break;
		case 2:		ss << "Feb."; break;
		case 3:		ss << "Mar."; break;
		case 4:		ss << "Apr."; break;
		case 5:		ss << "May."; break;
		case 6:		ss << "Jun."; break;
		case 7:		ss << "Jul."; break;
		case 8:		ss << "Aug."; break;
		case 9:		ss << "Sep."; break;
		case 10:	ss << "Oct."; break;
		case 11:	ss << "Nov."; break;
		case 12:	ss << "Dec."; break;
	}

	//--------------------------------------------------------------------------------
	string	unit = to_string(systime.wDay);		ss << PUSH2(unit);
	ss << "." << to_string(systime.wYear) << " ";


	//--------------------------------------------------------------------------------
	if (showDAY)
	{
		string day;
		switch (systime.wDayOfWeek)
		{
			case 0:	day = "Sun";	break;
			case 1:	day = "Mon";	break;
			case 2:	day = "Tue";	break;
			case 3:	day = "Wed";	break;
			case 4:	day = "Thu";	break;
			case 5:	day = "Fri";	break;
			case 6:	day = "Sat";	break;
		}	ss << "(" << day << ") ";
	}

	//--------------------------------------------------------------------------------
	ss << "`";
	unit = to_string(systime.wHour);	ss << PUSH2(unit) << ":";
	unit = to_string(systime.wMinute);	ss << PUSH2(unit) << ":";
	unit = to_string(systime.wSecond);	ss << PUSH2(unit);
		
	if (showMS)
	{
		ss << " (";
		unit = to_string(systime.wMilliseconds);
		ss << PUSH3(unit) << ")";
	}

	unit = ss.str();
	return to_carray(unit);
}


//==================================================================================
// gcQueryLocalTimeCODE: SATMAY072016473
//==================================================================================
GCGENERIC_API inline char* gcQueryLocalTimeCODE()
{
	//--------------------------------------------------------------------------------
	SYSTEMTIME		systime; GetLocalTime(&systime);
	stringstream	ss;

	//--------------------------------------------------------------------------------
	switch (systime.wDayOfWeek)
	{
		case 0:		ss << "SUN"; break;
		case 1:		ss << "MON"; break;
		case 2:		ss << "TUE"; break;
		case 3:		ss << "WED"; break;
		case 4:		ss << "THU"; break;
		case 5:		ss << "FRI"; break;
		case 6:		ss << "SAT"; break;
	}

	//--------------------------------------------------------------------------------
	switch (systime.wMonth)
	{
		case 1:		ss << "JAN"; break;
		case 2:		ss << "FEB"; break;
		case 3:		ss << "MAR"; break;
		case 4:		ss << "APR"; break;
		case 5:		ss << "MAY"; break;
		case 6:		ss << "JUN"; break;
		case 7:		ss << "JUL"; break;
		case 8:		ss << "AUG"; break;
		case 9:		ss << "SEP"; break;
		case 10:	ss << "OCT"; break;
		case 11:	ss << "NOV"; break;
		case 12:	ss << "DEC"; break;
	}

	//--------------------------------------------------------------------------------
	string	unit = to_string(systime.wDay);		ss << PUSH2(unit);
	ss << to_string(systime.wYear);
	unit = to_string(systime.wHour);			ss << PUSH2(unit);
	unit = to_string(systime.wMinute);			ss << PUSH2(unit);
	unit = to_string(systime.wSecond);			ss << PUSH2(unit);
	unit = to_string(systime.wMilliseconds);	ss << PUSH3(unit);
		
	unit = ss.str();
	return to_carray(unit);
}


//==================================================================================
// gcTimeScaleUS: [min], [s], [ms], [us]
//==================================================================================
GCGENERIC_API inline char* gcTimeScaleUS(const double& us)
{
	double timeCONV;
	stringstream ss;

	if (us > 1000000.0)
	{
		timeCONV = (us / 1000000.0);

		if (timeCONV > 60)
		{
			timeCONV /= 60.0;
			ss << setprecision(4)
				<< timeCONV << "[min]";
		}
		else
		{
			ss << setprecision(4)
				<< timeCONV << "[s]";
		}

	}
	else if (us > 1000.0)
	{
		timeCONV = (us / 1000.0);
		ss << std::setprecision(4)
			<< timeCONV << "[ms]";
	}
	else
	{
		timeCONV = us;
		ss << setprecision(4)
			<< timeCONV << "[us]";
	}

	return to_carray(ss.str());
}


//==================================================================================
// Internal Helper
//==================================================================================
inline string PUSH2(const string& unit)
{
	UINT length		= unit.length();
	string toPush	= "";
	string zero		= "0";
	stringstream ss;

	switch (length)
	{
		case 1:
			toPush = zero;
			toPush += unit;
			ss << toPush;
			break;
		case 2:
			ss << unit;
			break;
	}

	return ss.str();
}

inline string PUSH3(const string& unit)
{
	UINT length = unit.length();
	string toPush = "";
	string zero = "0";
	stringstream ss;

		switch (length)
		{
			case 1:
				toPush = zero; toPush += zero;
				toPush += unit;
				ss << toPush;
				break;
			case 2:
				toPush = zero;
				toPush += unit;
				ss << toPush;
				break;
			case 3:
				ss << unit;
				break;
		}

	return ss.str();
}


//==================================================================================
// gcRand: Random Positive Integer											 [0:max]
//==================================================================================
GCGENERIC_API inline UINT gcRand(const UINT& MaxVal)
{
	auto seed = chrono::high_resolution_clock::now().time_since_epoch().count();
	mt19937 Mersenne((UINT)seed);
	uniform_real_distribution<> Rand(-0.5, MaxVal + 0.5);
	
	return (int)(round(Rand(Mersenne)));
}


//==================================================================================
// gcRand_I_Uniform: Random Integer										   [min:max]
//==================================================================================
GCGENERIC_API inline int gcRand_I_Uniform(const int& MinVal, const int& MaxVal)
{
	auto seed = chrono::high_resolution_clock::now().time_since_epoch().count();
	mt19937 Mersenne((UINT)seed);
	uniform_real_distribution<> Rand(MinVal - 0.5, MaxVal + 0.5);

	return (int)(round(Rand(Mersenne)));
}


//==================================================================================
// gcRand_I_Gaussian: Random Integer								(mean, Variance)
//==================================================================================
GCGENERIC_API inline int gcRand_I_Gaussian(const int& MeanVal, const int& VarianceVal)
{
	auto seed = chrono::high_resolution_clock::now().time_since_epoch().count();
	mt19937 Mersenne((UINT)seed);
	normal_distribution<> Rand(MeanVal, VarianceVal);

	return (int)(round(Rand(Mersenne)));
}


//==================================================================================
// gcRand_R_Uniform: Random Real Number									   [min:max]
//==================================================================================
//GCGENERIC_API inline UINT gcRand_R_Uniform()
//{
//
//}