
// Gobang_FiveChess.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CGobang_FiveChessApp:
// �йش����ʵ�֣������ Gobang_FiveChess.cpp
//

class CGobang_FiveChessApp : public CWinApp
{
public:
	CGobang_FiveChessApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CGobang_FiveChessApp theApp;