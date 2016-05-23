/*==================================================================================

	"gcSTRCONV.HPP"

	Dependency: Windows x86

	HyoSeung Robinus Baskerville Kang.
	May.20.2016.

//==================================================================================

	class	CgcSTRING
			CgcSTRING	gcSTRING

		gcSTRING.SetFill()
		gcSTRING.SetWidth()
		gcSTRING.GetWidth()

		to_string()					//any					>> string
		to_carray()					//any					>> CArray
		
		allignLeft()				//left	<< setfill()	<< setw()
		allignRight()				//right	<< setfill()	<< setw()		

		to_filePTE()				//(string)("c:\\_test\\", "file", gcEXT::DspData)

		length_ss()					//length(stringstream)
		clear_ss()					//clear(stringstream)	
	
==================================================================================*/
#ifdef GCSTRING_EXPORTS
	#define GCSTRING_API __declspec(dllexport)
#else
	#define GCSTRING_API __declspec(dllimport)
#endif

#pragma once
#include <gcDEFINE.hpp>
#include <atlstr.h>
#include <stdio.h>	
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
using namespace std;


#define DEFAULT_WIDTH 6
#define PRINTABLE_MAX 0xFFU
//==================================================================================
// class CgcSTRING: Hidden Symbol
//==================================================================================
class CgcSTRING
{

public:
	CgcSTRING();
	~CgcSTRING();


private:

	char						m_carray_static[PRINTABLE_MAX] = { 0 };
	char*						m_carray_dynamic = NULL;
	char*						m_carray_filePath = NULL;
	char*						m_ext = NULL;
	stringstream				m_ss;

	int							m_setWidth = DEFAULT_WIDTH;
	char						m_setFill = ' ';
		
	
	//==============================================================================
	// NullStatic:				m_carray_static[i] = NULL
	// FreeMemory:				delete[]char m_any
	//==============================================================================
	void NullStatic();
	void FreeMemory();						  //delete[]char m_any, calling follows.
		void CLEAN_carray_dynamic();
		void CLEAN_carray_filePath();
		void CLEAN_ext();


	//==============================================================================
	char*	to_carray(const string& input);		//string to CArray


	//==============================================================================
	// Path, Title, and Extension.
	//==============================================================================
	char* to_filePTE(
												
		const string& path,
		const char* title,
		const gcEXT& ext
		);

		//strcpy(m_ext, ".gcDSP")
		void FormExtension(const string& ext);
		void FormExtension(const char* ext);


	//==============================================================================
	// allignLeft, allignRight: cout, string, stringstream
	//==============================================================================
	string	allignLeft(const string& input);
	string	allignRight(const string& input);
	void	m_ss_CLEAR();

	
public:

	void	SetFill(const char& inputFiller)	{ m_setFill = inputFiller;		}
	void	SetWidth(const int& inputWidth)		{ m_setWidth = inputWidth;		}
	int		GetWidth()							{ return m_setWidth;			}
	void	SetWidth()							{ m_setWidth = DEFAULT_WIDTH;	}

	friend	GCSTRING_API inline string		to_string(const char* input);
	
	friend	GCSTRING_API char*				to_carray(const string& input);
	friend	GCSTRING_API char*				to_carray(const int& input);
	friend	GCSTRING_API char*				to_carray(const unsigned int& input);
	friend	GCSTRING_API char*				to_carray(const long& input);
	friend	GCSTRING_API char*				to_carray(const unsigned long& input);
	friend	GCSTRING_API char*				to_carray(const long long& input);
	friend	GCSTRING_API char*				to_carray(const unsigned long long& input);
	friend	GCSTRING_API char*				to_carray(const float& input);
	friend	GCSTRING_API char*				to_carray(const double& input);
	friend	GCSTRING_API char*				to_carray(const long double& input);
	
	friend	GCSTRING_API char*				to_filePTE(const string& path, const char* title, const gcEXT& ext);
	friend	GCSTRING_API char*				to_filePTE(const char* path, const char* title, const gcEXT& ext);

	friend	GCSTRING_API string				allignLeft(const int& input);
	friend	GCSTRING_API string				allignLeft(const unsigned int& input);
	friend	GCSTRING_API string				allignLeft(const long& input);
	friend	GCSTRING_API string				allignLeft(const unsigned long& input);
	friend	GCSTRING_API string				allignLeft(const long long& input);
	friend	GCSTRING_API string				allignLeft(const unsigned long long& input);
	friend	GCSTRING_API string				allignLeft(const float& input);
	friend	GCSTRING_API string				allignLeft(const double& input);
	friend	GCSTRING_API string				allignLeft(const long double& input);
	friend	GCSTRING_API string				allignLeft(const char* input);
	friend	GCSTRING_API string				allignLeft(const string& input);
	
	friend	GCSTRING_API string				allignRight(const int& input);
	friend	GCSTRING_API string				allignRight(const unsigned int& input);
	friend	GCSTRING_API string				allignRight(const long& input);
	friend	GCSTRING_API string				allignRight(const unsigned long& input);
	friend	GCSTRING_API string				allignRight(const long long& input);
	friend	GCSTRING_API string				allignRight(const unsigned long long& input);
	friend	GCSTRING_API string				allignRight(const float& input);
	friend	GCSTRING_API string				allignRight(const double& input);
	friend	GCSTRING_API string				allignRight(const long double& input);
	friend	GCSTRING_API string				allignRight(const char* input);
	friend	GCSTRING_API string				allignRight(const string& input);
};


//==================================================================================
// Start Exporting
//==================================================================================
#define										clear_ss(input)		input.str("")
#define										to_cstring(input)	(CString)(to_carray(input))


GCSTRING_API inline int						length_ss(stringstream& input);
GCSTRING_API inline string					to_string(const char* input);

GCSTRING_API inline void					to_carray_staticA(char* to, CString& from)
{
	wchar_t* tmpW = from.GetBuffer(from.GetLength());
	int      len = WideCharToMultiByte(CP_ACP, 0, tmpW, -1, NULL, 0, NULL, NULL);

	char* tmp = new char[len];
	WideCharToMultiByte(CP_ACP, 0, tmpW, -1, tmp, len, 0, 0);

	char* pTo = to;
	char* pTmp = tmp;
	for (register int i= 0; i < len; ++i)
	{
		*pTo++ = *pTmp++;
	}
	*pTo = NULL;
	delete[]tmp;
}
GCSTRING_API char*							to_carray(const string& input);
GCSTRING_API char*							to_carray(const int& input);
GCSTRING_API char*							to_carray(const unsigned int& input);
GCSTRING_API char*							to_carray(const long& input);
GCSTRING_API char*							to_carray(const unsigned long& input);
GCSTRING_API char*							to_carray(const long long& input);
GCSTRING_API char*							to_carray(const unsigned long long& input);
GCSTRING_API char*							to_carray(const float& input);
GCSTRING_API char*							to_carray(const double& input);
GCSTRING_API char*							to_carray(const long double& input);

GCSTRING_API char*							to_filePTE(const string& path, const char* title, const gcEXT& ext);
GCSTRING_API char*							to_filePTE(const char* path, const char* title, const gcEXT& ext);

GCSTRING_API string							allignLeft(const int& input);
GCSTRING_API string							allignLeft(const unsigned int& input);
GCSTRING_API string							allignLeft(const long& input);
GCSTRING_API string							allignLeft(const unsigned long& input);
GCSTRING_API string							allignLeft(const long long& input);
GCSTRING_API string							allignLeft(const unsigned long long& input);
GCSTRING_API string							allignLeft(const float& input);
GCSTRING_API string							allignLeft(const double& input);
GCSTRING_API string							allignLeft(const long double& input);
GCSTRING_API string							allignLeft(const char* input);
GCSTRING_API string							allignLeft(const string& input);

GCSTRING_API string							allignRight(const int& input);
GCSTRING_API string							allignRight(const unsigned int& input);
GCSTRING_API string							allignRight(const long& input);
GCSTRING_API string							allignRight(const unsigned long& input);
GCSTRING_API string							allignRight(const long long& input);
GCSTRING_API string							allignRight(const unsigned long long& input);
GCSTRING_API string							allignRight(const float& input);
GCSTRING_API string							allignRight(const double& input);
GCSTRING_API string							allignRight(const long double& input);
GCSTRING_API string							allignRight(const char* input);
GCSTRING_API string							allignRight(const string& input);

extern GCSTRING_API CgcSTRING				gcSTRING;

#ifdef PRINTABLE_MAX
	#undef PRINTABLE_MAX
	#undef DEFAULT_WIDTH
#endif
//==================================================================================
//																		End of Line
//==================================================================================