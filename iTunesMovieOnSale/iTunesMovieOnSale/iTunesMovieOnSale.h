
// iTunesMovieOnSale.h : PROJECT_NAME ���ε{�����D�n���Y��
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�� PCH �]�t���ɮ׫e���]�t 'stdafx.h'"
#endif

#include "resource.h"		// �D�n�Ÿ�


// CiTunesMovieOnSaleApp: 
// �аѾ\��@�����O�� iTunesMovieOnSale.cpp
//

class CiTunesMovieOnSaleApp : public CWinApp
{
public:
	CiTunesMovieOnSaleApp();

// �мg
public:
	virtual BOOL InitInstance();

// �{���X��@

	DECLARE_MESSAGE_MAP()
};

extern CiTunesMovieOnSaleApp theApp;