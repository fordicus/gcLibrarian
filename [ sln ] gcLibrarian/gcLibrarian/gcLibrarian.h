
// gcLibrarian.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CgcLibrarianApp:
// �� Ŭ������ ������ ���ؼ��� gcLibrarian.cpp�� �����Ͻʽÿ�.
//

class CgcLibrarianApp : public CWinApp
{
public:
	CgcLibrarianApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CgcLibrarianApp theApp;