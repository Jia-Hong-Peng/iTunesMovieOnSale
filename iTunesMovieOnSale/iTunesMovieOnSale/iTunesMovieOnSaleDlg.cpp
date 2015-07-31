
// iTunesMovieOnSaleDlg.cpp : 實作檔
//

#include "stdafx.h"
#include "iTunesMovieOnSale.h"
#include "iTunesMovieOnSaleDlg.h"
#include "afxdialogex.h"
#include <afxinet.h>
#include "resource.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 對 App About 使用 CAboutDlg 對話方塊

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 對話方塊資料
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支援

// 程式碼實作
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CiTunesMovieOnSaleDlg 對話方塊



CiTunesMovieOnSaleDlg::CiTunesMovieOnSaleDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CiTunesMovieOnSaleDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CiTunesMovieOnSaleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CiTunesMovieOnSaleDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CiTunesMovieOnSaleDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CiTunesMovieOnSaleDlg 訊息處理常式

BOOL CiTunesMovieOnSaleDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 將 [關於...] 功能表加入系統功能表。

	// IDM_ABOUTBOX 必須在系統命令範圍之中。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 設定此對話方塊的圖示。當應用程式的主視窗不是對話方塊時，
	// 框架會自動從事此作業
	SetIcon(m_hIcon, TRUE);			// 設定大圖示
	SetIcon(m_hIcon, FALSE);		// 設定小圖示

	// TODO:  在此加入額外的初始設定

	return TRUE;  // 傳回 TRUE，除非您對控制項設定焦點
}

void CiTunesMovieOnSaleDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果將最小化按鈕加入您的對話方塊，您需要下列的程式碼，
// 以便繪製圖示。對於使用文件/檢視模式的 MFC 應用程式，
// 框架會自動完成此作業。

void CiTunesMovieOnSaleDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 繪製的裝置內容

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 將圖示置中於用戶端矩形
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 描繪圖示
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 當使用者拖曳最小化視窗時，
// 系統呼叫這個功能取得游標顯示。
HCURSOR CiTunesMovieOnSaleDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CiTunesMovieOnSaleDlg::OnBnClickedButton1()
{
	//All the work of the application happens here
	// We will get the raw data of the information into the thing and have people
	// that you can get the HTTP pages raw data
	//
	// Copyright 2003 by Sam Contapay
	CInternetSession ISession;
	CHttpConnection* pConnection = NULL;
	CString strURL;
	CString Header, Data;
	string Temp;

	Temp.Empty(); Data.Empty();
	//Clear the edit box if anything is in there
	SetDlgItemText(IDC_RESULT, L"");

	//Get the URL string
	GetDlgItemText(IDC_URL_TEXT, strURL);

	pConnection = ISession.GetHttpConnection((LPCTSTR)strURL);
	if (pConnection == NULL)
	{
		//Unable to open the damn connection to the server
		//what the hell
		AfxMessageBox(L"Unable to Open connection to server. Please check your internet connection and the URL.", MB_ICONERROR);
		return;
	}

	//We got this far so something is open all we need todo is retrieve
	//the information and put it in the damn box
	CHttpFile* pURLFile = pConnection->OpenRequest(L"GET", L"/");

	pURLFile->SendRequest();

	pURLFile->SetReadBufferSize(MAX_READ_SIZE);

	
	while (pURLFile->ReadString(Temp))
	{
		Data += Temp + L"\r\n";
	}
	//int TotalBytes = pURLFile->Read(Buffer, MAX_READ_SIZE);
	pURLFile->QueryInfo(HTTP_QUERY_RAW_HEADERS_CRLF, Header);

	//Data.SetString(Buffer, TotalBytes);
	//Get rid of the weird square thing
	//Data.Replace("\n", "\r\n");

	//char *pch;
	//pch = new char[Data.GetLength() * sizeof(TCHAR) + 1];
	//memcpy(pch, Data, Data.GetLength() * sizeof(TCHAR));
	//Data = UTF8ToUnicode(pch);




	SetDlgItemText(IDC_DATA, Data);
	SetDlgItemText(IDC_RESULT, Header);

	pURLFile->Close();
	delete pURLFile;
	pConnection->Close();
	delete pConnection;
	ISession.Close();

}



CString CiTunesMovieOnSaleDlg::UTF8ToUnicode(char* UTF8)
{

	DWORD dwUnicodeLen;
	TCHAR *pwText;
	CString strUnicode;
	dwUnicodeLen = MultiByteToWideChar(CP_UTF8, 0, UTF8, -1, NULL, 0);
	pwText = new TCHAR[dwUnicodeLen];
	if (!pwText)
	{
		return strUnicode;
	}	
	MultiByteToWideChar(CP_UTF8, 0, UTF8, -1, pwText, dwUnicodeLen);	
	strUnicode.Format(_T("%s"), pwText);

	delete[]pwText;	
	return strUnicode;
}


CString CiTunesMovieOnSaleDlg::Convert(CString str, int sourceCodepage, int targetCodepage)
{
	int len = str.GetLength();
	CStringA strA(str);
	int unicodeLen = MultiByteToWideChar(sourceCodepage, 0, strA, -1, NULL, 0);

	wchar_t *pUnicode;
	pUnicode = new wchar_t[unicodeLen + 1];
	memset(pUnicode, 0, (unicodeLen + 1)*sizeof(wchar_t));
	MultiByteToWideChar(sourceCodepage, 0, strA, -1, (LPWSTR)pUnicode, unicodeLen);

	BYTE *pTargetData = NULL;
	int targetLen = WideCharToMultiByte(targetCodepage, 0, (LPWSTR)pUnicode, -1, (char*)pTargetData, 0, NULL, NULL);

	pTargetData = new BYTE[targetLen + 1];
	memset(pTargetData, 0, targetLen + 1);

	WideCharToMultiByte(targetCodepage, 0, (LPWSTR)pUnicode, -1, (char*)pTargetData, targetLen, NULL, NULL);

	CString rt;
	rt.Format(L"%s", pTargetData);

	delete[]pUnicode;
	delete[]pTargetData;


	return rt;

}