
// iTunesMovieOnSaleDlg.h : header file
//

#pragma once
#include "SqlOperator.h"
#include "afxcmn.h"



struct LinkerObject
{	
	CString title;	
	CString url;	
};

// CiTunesMovieOnSaleDlg dialog
class CiTunesMovieOnSaleDlg : public CDialogEx
{
// Construction
public:
	SqlOperator Sql;
	CiTunesMovieOnSaleDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_ITUNESMOVIEONSALE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonGet();
	void SplitString(CStringArray& dst, const CString& src, LPCTSTR slipt);
	virtual BOOL DestroyWindow();
	CString CiTunesMovieOnSaleDlg::GetInner(CString htmls, CString ff, CString ll);
	CStringArray strURL;
	CStringArray strAZ;
	CString CiTunesMovieOnSaleDlg::getLinkHTML(CString output);
	CString CiTunesMovieOnSaleDlg::getTitle(CString output);
	CString CiTunesMovieOnSaleDlg::getUrl(CString output);
	BOOL CiTunesMovieOnSaleDlg::isRightUrl(CString output);
	void CiTunesMovieOnSaleDlg::getLinkerObj(LinkerObject* linkerObj, CString URL, CString letter, CString page);
	void CiTunesMovieOnSaleDlg::InsertLinker(LinkerObject* n);
	CProgressCtrl m_myProgressLinker;
	afx_msg void OnBnClickedButtonPrice();
	CProgressCtrl m_myProgressPrice;
};
