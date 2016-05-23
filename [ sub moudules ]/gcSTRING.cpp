/*==================================================================================

	"gcSTRCONV.CPP"

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

		length()					//length(stringstream)
		clear()						//clear(stringstream)

==================================================================================*/
#pragma once
#include "stdafx.h"
#include "gcSTRING.hpp"


#define DEFAULT_WIDTH 6
#define PRINTABLE_MAX 0xFFU
//==================================================================================
// length: stringstream
//==================================================================================
 inline int length_ss(stringstream& input)
{
	input.seekg(0, ios::end);
	int length = (int)(input.tellg());
	return length;
}


//==================================================================================
// class CgcSTRING: Hidden Symbol
//==================================================================================
CgcSTRING::CgcSTRING() { }
CgcSTRING::~CgcSTRING() { this->FreeMemory(); }			  //Free Dynamic Allocations


//==================================================================================
// NullStatic:				m_carray_static[i] = NULL
// FreeMemory:				delete[]char m_any
//==================================================================================
void CgcSTRING::NullStatic()
{
	register char* ptr = m_carray_static;
	for (register int i = 0; i < PRINTABLE_MAX; ++i)
		*(ptr++) = NULL;
}

//----------------------------------------------------------------------------------
void CgcSTRING::FreeMemory()
{
	this->CLEAN_carray_dynamic();
	this->CLEAN_carray_filePath();
	this->CLEAN_ext();
}

	void CgcSTRING::CLEAN_carray_dynamic()
	{
		if (m_carray_dynamic != NULL)
		{
			delete[]m_carray_dynamic;
			m_carray_dynamic = NULL;
		}
	}

	void CgcSTRING::CLEAN_carray_filePath()
	{
		if (m_carray_filePath != NULL)
		{
			delete[]m_carray_filePath;
			m_carray_filePath = NULL;
		}
	}

	void CgcSTRING::CLEAN_ext()
	{
		if (m_ext != NULL)
		{
			delete[]m_ext;
			m_ext = NULL;
		}
	}


//==================================================================================
// to_carray: string to char[]
//==================================================================================
char* CgcSTRING::to_carray(const string& input)
{
	this->CLEAN_carray_dynamic();

	register int strLen = input.length();
	m_carray_dynamic = new char[strLen + 1];
	strcpy(m_carray_dynamic, input.c_str());

	return m_carray_dynamic;
}


//================================================================================== 
// to_filePTE: path Formatter
//==================================================================================
char* CgcSTRING::to_filePTE
(
	const string& path,
	const char* title,
	const gcEXT& ext
){
	this->CLEAN_carray_filePath();

	switch (ext)
	{
		case gcEXT::DspData:
		{
			this->FormExtension(".gcDSP"); break;
		}
		default:
		{
			this->FormExtension(".unknown"); break;
		}
	}

	int pathLen = path.length();
	pathLen += strlen(title), pathLen += strlen(m_ext);

	m_carray_filePath = new char[pathLen + 1];
	strcpy(m_carray_filePath, path.c_str());
	strcat(m_carray_filePath, title);
	strcat(m_carray_filePath, m_ext);

	return m_carray_filePath;
}

	//------------------------------------------------------------------------------
	// FormExtension: strcpy(m_ext, ".gcDSP")
	//------------------------------------------------------------------------------
	void CgcSTRING::FormExtension(const string& ext)
	{
		this->CLEAN_ext();
		m_ext = new char[ext.length() + 1];
		strcpy(m_ext, ext.c_str());
	}

	void CgcSTRING::FormExtension(const char* ext)
	{
		this->CLEAN_ext();
		m_ext = new char[strlen(ext) + 1];
		strcpy(m_ext, ext);
	}


//==================================================================================
// allignLeft, allignRight: cout, string, stringstream
//==================================================================================
string CgcSTRING::allignLeft(const string& input)
{
	this->m_ss_CLEAR();

	m_ss
		<< setfill(m_setFill)
		<< setw(m_setWidth)
		<< left
		<< input;

	return m_ss.str();
}

string CgcSTRING::allignRight(const string& input)
{
	this->m_ss_CLEAR();

	m_ss
		<< setfill(m_setFill)
		<< setw(m_setWidth)
		<< right
		<< input;

	return m_ss.str();
}

void CgcSTRING::m_ss_CLEAR(){	m_ss.str("");	}



//==================================================================================
// Start: Interface
//==================================================================================
CgcSTRING  gcSTRING;


//==================================================================================
// to_string: any >> string
//==================================================================================
inline string to_string(const char* input)
{
	return string(input);
}


//==================================================================================
// to_carray: any >> CArray
//==================================================================================
char* to_carray(const string& input)					{ return gcSTRING.to_carray(input);					}
char* to_carray(const int& input)						{ return gcSTRING.to_carray(to_string(input));		}
char* to_carray(const unsigned int& input)				{ return gcSTRING.to_carray(to_string(input));		}
char* to_carray(const long& input)						{ return gcSTRING.to_carray(to_string(input));		}
char* to_carray(const unsigned long& input)				{ return gcSTRING.to_carray(to_string(input));		}
char* to_carray(const long long& input)					{ return gcSTRING.to_carray(to_string(input));		}
char* to_carray(const unsigned long long& input)		{ return gcSTRING.to_carray(to_string(input));		}
char* to_carray(const float& input)						{ return gcSTRING.to_carray(to_string(input));		}
char* to_carray(const double& input)					{ return gcSTRING.to_carray(to_string(input));		}
char* to_carray(const long double& input)				{ return gcSTRING.to_carray(to_string(input));		}


//==================================================================================
// to_filePTE: { def_path + (enter_title) + def_ext }
//==================================================================================
char* to_filePTE(const string& path, const char* title, const gcEXT& ext) {
	return gcSTRING.to_filePTE(path, title, ext);
}

char* to_filePTE(const char* path, const char* title, const gcEXT& ext) {
	return gcSTRING.to_filePTE(to_string(path), title, ext);
}


//==================================================================================
// allignLeft, allignRight: cout, string, stringstream
//==================================================================================
string allignLeft(const int& input)						{ return gcSTRING.allignLeft(to_string(input));		}
string allignLeft(const unsigned int& input)			{ return gcSTRING.allignLeft(to_string(input));		}
string allignLeft(const long& input)					{ return gcSTRING.allignLeft(to_string(input));		}
string allignLeft(const unsigned long& input)			{ return gcSTRING.allignLeft(to_string(input));		}
string allignLeft(const long long& input)				{ return gcSTRING.allignLeft(to_string(input));		}
string allignLeft(const unsigned long long& input)		{ return gcSTRING.allignLeft(to_string(input));		}
string allignLeft(const float& input)					{ return gcSTRING.allignLeft(to_string(input));		}
string allignLeft(const double& input)					{ return gcSTRING.allignLeft(to_string(input));		}
string allignLeft(const long double& input)				{ return gcSTRING.allignLeft(to_string(input));		}
string allignLeft(const char* input)					{ return gcSTRING.allignLeft(to_string(input));		}
string allignLeft(const string& input)					{ return gcSTRING.allignLeft(input);				}

string allignRight(const int& input)					{ return gcSTRING.allignRight(to_string(input));	}
string allignRight(const unsigned int& input)			{ return gcSTRING.allignRight(to_string(input));	}
string allignRight(const long& input)					{ return gcSTRING.allignRight(to_string(input));	}
string allignRight(const unsigned long& input)			{ return gcSTRING.allignRight(to_string(input));	}
string allignRight(const long long& input)				{ return gcSTRING.allignRight(to_string(input));	}
string allignRight(const unsigned long long& input)		{ return gcSTRING.allignRight(to_string(input));	}
string allignRight(const float& input)					{ return gcSTRING.allignRight(to_string(input));	}
string allignRight(const double& input)					{ return gcSTRING.allignRight(to_string(input));	}
string allignRight(const long double& input)			{ return gcSTRING.allignRight(to_string(input));	}
string allignRight(const char* input)					{ return gcSTRING.allignRight(to_string(input));	}
string allignRight(const string& input)					{ return gcSTRING.allignRight(input);				}


#ifdef PRINTABLE_MAX
#undef PRINTABLE_MAX
#undef DEFAULT_WIDTH
#endif
//==================================================================================
//																		End of Line
//==================================================================================