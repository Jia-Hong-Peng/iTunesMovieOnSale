
// iTunesMovieOnSaleDlg.h : ���Y��
//

#pragma once

#define MAX_READ_SIZE 10000

// CiTunesMovieOnSaleDlg ��ܤ��
class CiTunesMovieOnSaleDlg : public CDialogEx
{
// �غc
public:


	CiTunesMovieOnSaleDlg(CWnd* pParent = NULL);	// �зǫغc�禡

// ��ܤ�����
	enum { IDD = IDD_ITUNESMOVIEONSALE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �䴩


// �{���X��@
protected:
	HICON m_hIcon;

	// ���ͪ��T�������禡
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	CString CiTunesMovieOnSaleDlg::UTF8ToUnicode(char* UTF8);
	CString Convert(CString str, int sourceCodepage, int targetCodepage);

};
