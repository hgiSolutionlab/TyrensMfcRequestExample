
// mfcRequestDlg.h : header file
//

#pragma once


// CmfcRequestDlg dialog
class CmfcRequestDlg : public CDialogEx
{
	// Construction
public:
	CmfcRequestDlg(CWnd* pParent = nullptr);	// standard constructor
	~CmfcRequestDlg();

	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCREQUEST_DIALOG };
#endif

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
	ApiWrapper* Api;
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnLbnSelchangeList1();
	CListBox projectsListBox;
};
