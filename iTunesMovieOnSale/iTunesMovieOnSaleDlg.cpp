
// iTunesMovieOnSaleDlg.cpp : implementation file
//

#include "stdafx.h"
#include "iTunesMovieOnSale.h"
#include "iTunesMovieOnSaleDlg.h"
#include "afxdialogex.h"


#include "HttpClient.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


HttpClient m_httpClient;

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	// Implementation
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


// CiTunesMovieOnSaleDlg dialog



CiTunesMovieOnSaleDlg::CiTunesMovieOnSaleDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CiTunesMovieOnSaleDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CiTunesMovieOnSaleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS1, m_myProgressLinker);
	DDX_Control(pDX, IDC_PROGRESS2, m_myProgressPrice);
}

BEGIN_MESSAGE_MAP(CiTunesMovieOnSaleDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_GET, &CiTunesMovieOnSaleDlg::OnBnClickedButtonGet)
	ON_BN_CLICKED(IDC_BUTTON_PRICE, &CiTunesMovieOnSaleDlg::OnBnClickedButtonPrice)
END_MESSAGE_MAP()


// CiTunesMovieOnSaleDlg message handlers

BOOL CiTunesMovieOnSaleDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	Sql.OpenDb(_T("iTunesMovieOnSale.db"));	
	//Sql.Execute(_T("DROP TABLE LINKER;"));
	Sql.Execute(_T("CREATE TABLE LINKER([URL] VARCHAR(255) PRIMARY KEY,[TITLE] VARCHAR(255) ,[PRICE] INTEGER  );"));
	m_myProgressLinker.SetPos(0);
	m_myProgressPrice.SetPos(0);
	strURL.SetSize(16);
	strURL[0] = L"https://itunes.apple.com/tw/genre/dian-ying-dong-zuo-yu-li-xian/id4401?l=zh"; //動作與歷險
	strURL[1] = L"https://itunes.apple.com/tw/genre/dian-ying-jing-song-pian/id4416?l=zh"; //驚悚片
	strURL[2] = L"https://itunes.apple.com/tw/genre/dian-ying-ke-huan-yu-qi-huan/id4413?l=zh"; //科幻與奇幻片
	strURL[3] = L"https://itunes.apple.com/tw/genre/dian-ying-kong-bu-pian/id4408?l=zh"; //恐怖片
	strURL[4] = L"https://itunes.apple.com/tw/genre/dian-ying-ju-qing-pian/id4406?l=zh"; //劇情片
	strURL[5] = L"https://itunes.apple.com/tw/genre/dian-ying-xi-ju/id4404?l=zh"; //喜劇
	strURL[6] = L"https://itunes.apple.com/tw/genre/dian-ying-jing-dian-zuo-pin/id4403?l=zh"; //經典作品
	strURL[7] = L"https://itunes.apple.com/tw/genre/dian-ying-er-tong-yu-jia-ting/id4410?l=zh"; //兒童與家庭片
	strURL[8] = L"https://itunes.apple.com/tw/genre/dian-ying-xi-bu-pian/id4418?l=zh"; //西部片
	strURL[9] = L"https://itunes.apple.com/tw/genre/dian-ying-dou-hui/id4419?l=zh"; //都會
	strURL[10] = L"https://itunes.apple.com/tw/genre/dian-ying-yun-dong/id4417?l=zh"; //運動
	strURL[11] = L"https://itunes.apple.com/tw/genre/dian-ying-te-shu-xing-qu-ying/id4415?l=zh"; //特殊興趣影片
	strURL[12] = L"https://itunes.apple.com/tw/genre/dian-ying-romance/id4412?l=zh"; //Romance
	strURL[13] = L"https://itunes.apple.com/tw/genre/dian-ying-yin-le-ju/id4411?l=zh"; //音樂劇
	strURL[14] = L"https://itunes.apple.com/tw/genre/dian-ying-yin-le-zhu-ti-dian/id4424?l=zh"; //音樂主題電影
	strURL[15] = L"https://itunes.apple.com/tw/genre/dian-ying-du-li-zhi-zuo-ying/id4409?l=zh"; //獨立製作影片

	strAZ.SetSize(28);
	strAZ[0] = L"A";
	strAZ[1] = L"B";
	strAZ[2] = L"C";
	strAZ[3] = L"D";
	strAZ[4] = L"E";
	strAZ[5] = L"F";
	strAZ[6] = L"G";
	strAZ[7] = L"H";
	strAZ[8] = L"I";
	strAZ[9] = L"J";
	strAZ[10] = L"K";
	strAZ[11] = L"L";
	strAZ[12] = L"M";
	strAZ[13] = L"N";
	strAZ[14] = L"O";
	strAZ[15] = L"P";
	strAZ[16] = L"Q";
	strAZ[17] = L"R";
	strAZ[18] = L"S";
	strAZ[19] = L"T";
	strAZ[20] = L"U";
	strAZ[21] = L"V";
	strAZ[22] = L"W";
	strAZ[23] = L"X";
	strAZ[24] = L"Y";
	strAZ[25] = L"Z";
	strAZ[26] = L"*";
	strAZ[27] = L"";


	int iTotalProgress = 0;	
	for (int k = 0; k < strURL.GetSize(); k++)
	{
		for (int j = 0; j < strAZ.GetSize(); j++)
		{
			for (int i = 1; i <= 5; i++)
			{
				iTotalProgress++;
			}
		}
	}
	m_myProgressLinker.SetRange(0, iTotalProgress);

	return TRUE;  // return TRUE  unless you set the focus to a control
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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CiTunesMovieOnSaleDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CiTunesMovieOnSaleDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CiTunesMovieOnSaleDlg::OnBnClickedButtonGet()
{
	GetDlgItem(IDC_BUTTON_GET)->EnableWindow(FALSE);
	LinkerObject linkerObj[500];
	int iPercentage = 0;

	for (int k = 0; k < strURL.GetSize(); k++)
	{
		for (int j = 0; j < strAZ.GetSize(); j++)
		{
			for (int i = 1; i <= 5; i++)
			{
				CString strPage;
				strPage.Format(L"%d", i);
				getLinkerObj(linkerObj, strURL[k], strAZ[j], strPage);
				InsertLinker(linkerObj);

				iPercentage++;
				m_myProgressLinker.SetPos(iPercentage);
			}
		}
	}
	UpdateData(FALSE);
	GetDlgItem(IDC_BUTTON_GET)->EnableWindow(TRUE);


}


void CiTunesMovieOnSaleDlg::InsertLinker(LinkerObject* n)
{	
	CString sql = L"insert into LINKER( [URL],[TITLE]) values('@URL','@TITLE') ";
	for (int i = 0; i < 500; i++)
	{
		if (!n[i].url.IsEmpty())
		{
			Table tb = Sql.Select(L"LINKER", L"*", L"where URL = '" + n[i].url + L"' ");
			BOOL isExist = (tb.GetRowCount() > 0) ? TRUE : FALSE;
			if (!isExist)
			{

				CString sql_tmp = sql;

				sql_tmp.Replace(L"@URL", n[i].url);
				sql_tmp.Replace(L"@TITLE", n[i].title);			
				Sql.Execute(sql_tmp);
			}
		}
	}
}

void CiTunesMovieOnSaleDlg::getLinkerObj(LinkerObject* linkerObj, CString URL, CString letter, CString page)
{
	CString strTargetUrl = URL + L"&letter=" + letter + L"&page=" + page + L"#page";
	CString strMyHTML = m_httpClient.HTTPGet(strTargetUrl, TRUE, NULL, &m_httpClient.g_cookie);
	CString strLinkHTML = getLinkHTML(strMyHTML);
	
	LPCTSTR slipt = L"><";
	CStringArray links;
	SplitString(links, strLinkHTML, slipt);

	if (links.GetSize() > 2)
	{
		for (int i = 0; i < links.GetSize(); i++)
		{
			CString strLink;
			strLink = links[i];
			if(isRightUrl(strLink))
			{
				linkerObj[i].title = getTitle(strLink);
				linkerObj[i].url = getUrl(strLink);
			}			
		}
	}
}

CString CiTunesMovieOnSaleDlg::getTitle(CString output)
{
	CString strOutput = output;
	strOutput = GetInner(strOutput, L"\">", L"</a");
	strOutput.Replace(L"\">", L"");
	return strOutput;
}

CString CiTunesMovieOnSaleDlg::getUrl(CString output)
{
	CString strOutput = output;
	strOutput = GetInner(strOutput, L"href=\"", L"\">");
	strOutput.Replace(L"href=\"", L"");
	return strOutput;
}

BOOL CiTunesMovieOnSaleDlg::isRightUrl(CString output)
{
	CString strOutput = output;
	strOutput = GetInner(strOutput, L"href=\"", L"\">");
	strOutput.Replace(L"href=\"", L"");

	if (strOutput.Right(4) == L"l=zh") return TRUE;
	else return FALSE;
}



void CiTunesMovieOnSaleDlg::SplitString(CStringArray& dst, const CString& src, LPCTSTR slipt)
{
	dst.RemoveAll();
	const int len = _tcslen(slipt);

	int start = 0, end = 0;
	while ((end = src.Find(slipt, end)) != -1)
	{
		dst.Add(src.Mid(start, end - start));
		start = end += len;
	}
	dst.Add(src.Right(src.GetLength() - start));
}


CString CiTunesMovieOnSaleDlg::getLinkHTML(CString output)
{
	CString strOutput = output;
	strOutput = GetInner(strOutput, L"<div class=\"column first\">", L"<div id=\"genre-nav\"");
	strOutput.Replace(L" = ", L"=");
	strOutput.Replace(L"<div class=\"column first\">", L"");
	strOutput.Replace(L"<div class=\"column\">", L"");
	strOutput.Replace(L"<div class=\"column last\">", L"");
	strOutput.Replace(L"<ul>", L"");
	strOutput.Replace(L"</ul>", L"");
	strOutput.Replace(L"<li>", L"");
	strOutput.Replace(L"</li>", L"");
	strOutput.Replace(L"<div>", L"");
	strOutput.Replace(L"</div>", L"");
	strOutput.Replace(L"\r\n", L"");
	strOutput.Replace(L"\n\r", L"");
	strOutput.Replace(L"\n", L"");
	strOutput.Replace(L"\r", L"");
	strOutput.Replace(L"  ", L" ");
	strOutput.Replace(L"  ", L"");
	strOutput.Replace(L"&nbsp;", L"");
	//strOutput.Replace(L"&amp;", L"&");
	return strOutput;
}


CString CiTunesMovieOnSaleDlg::GetInner(CString htmls, CString ff, CString ll)
{
	int first = htmls.Find(ff);
	int last = htmls.Find(ll);
	CString output = htmls.Mid(first, last - first);
	return output;
}


BOOL CiTunesMovieOnSaleDlg::DestroyWindow()
{
	Sql.CloseDb();
	return CDialogEx::DestroyWindow();
}


void CiTunesMovieOnSaleDlg::OnBnClickedButtonPrice()
{
	int iTotalPriceCount = 0;
	Table tPrice = Sql.Select(L"LINKER",L"*", L"PRICE IS NULL");
	iTotalPriceCount = tPrice.GetRowCount();
	m_myProgressPrice.SetRange(0, iTotalPriceCount);
	int iPercentagePrice = 0;
	for (int i = 0; i < iTotalPriceCount; i++)
	{
		tPrice.GoRow(i);
		CString strTargetUrl = tPrice.GetValue(0);
		CString strMyHTML = m_httpClient.HTTPGet(strTargetUrl, TRUE, NULL, &m_httpClient.g_cookie);
		CString  thePrice = getPrice(strMyHTML);
		Sql.Update(L"LINKER", L"PRICE=" + thePrice, L"URL='" + strTargetUrl + L"'");

		iPercentagePrice++;
		m_myProgressPrice.SetPos(iPercentagePrice);
	}

}


CString CiTunesMovieOnSaleDlg::getPrice(CString output)
{
	CString strOutput = output;
	strOutput.Replace(L" = ", L"=");
	strOutput.Replace(L"<ul>", L"");
	strOutput.Replace(L"</ul>", L"");
	strOutput.Replace(L"<li>", L"");
	strOutput.Replace(L"</li>", L"");
	strOutput.Replace(L"<div>", L"");
	strOutput.Replace(L"</div>", L"");
	strOutput.Replace(L"\r\n", L"");
	strOutput.Replace(L"\n\r", L"");
	strOutput.Replace(L"\n", L"");
	strOutput.Replace(L"\r", L"");
	strOutput.Replace(L"  ", L" ");
	strOutput.Replace(L"  ", L"");
	strOutput.Replace(L"&nbsp;", L"");

	strOutput = GetInner(strOutput, L"<div id=\"left-stack\">", L"客戶評分");
	strOutput = GetInner(strOutput, L"class=\"price\">", L"</span><li ");
	strOutput.Replace(L"class=\"price\">", L"");
	strOutput.Replace(L"NT$ ", L"");
	return strOutput;
}