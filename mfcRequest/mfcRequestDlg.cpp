
// mfcRequestDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "mfcRequest.h"
#include "mfcRequestDlg.h"
#include "afxdialogex.h"
#include "ApiWrapper.h"

// Define API credentials

#define TYRENS_API_ENDPOINT  ""
#define TYRENS_API_AUTH_TOKEN ""

using json = nlohmann::json;

namespace Models {

	// Model

	struct Project {
		int projectNumber;
		std::string name;
		std::string status;
		std::string ua;
		std::string uaFullName;
		bool isTemplate;
		bool isVisibleForEmployees;
	};

	// Mapping functions

	void from_json(const json& j, Project& p) {
		p.projectNumber = std::stoi(j["projectNumber"].get<std::string>());
		j.at("name").get_to(p.name);
		j.at("status").get_to(p.status);
		j.at("ua").get_to(p.ua);
		j.at("uaFullName").get_to(p.uaFullName);
		j.at("isTemplate").get_to(p.isTemplate);
		j.at("isVisibleForEmployees").get_to(p.isVisibleForEmployees);
	}
}


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CAboutDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CmfcRequestDlg dialog



CmfcRequestDlg::CmfcRequestDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCREQUEST_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	// Initializing API wrapper

	Api = new ApiWrapper(TYRENS_API_ENDPOINT, TYRENS_API_AUTH_TOKEN);
}

CmfcRequestDlg::~CmfcRequestDlg() {
	delete Api;
}

void CmfcRequestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, projectsListBox);
}

BEGIN_MESSAGE_MAP(CmfcRequestDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CmfcRequestDlg::OnBnClickedButton1)
	ON_LBN_SELCHANGE(IDC_LIST1, &CmfcRequestDlg::OnLbnSelchangeList1)
END_MESSAGE_MAP()


// CmfcRequestDlg message handlers

BOOL CmfcRequestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
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

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CmfcRequestDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CmfcRequestDlg::OnPaint()
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
HCURSOR CmfcRequestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CAboutDlg::OnBnClickedButton1()
{

}


void CmfcRequestDlg::OnBnClickedButton1()
{

	// Calling API

	auto resBuffer = Api->request("/projects");

	// Parsing string json response as vector of project models

	auto resJson = json::parse(*resBuffer);

	int resJsonSize = resJson.size();
	std::vector<Models::Project> projects;
	projects.reserve(resJsonSize);

	for (auto item : resJson) {
		projects.emplace_back(item.get<Models::Project>());
	}

	// Adding project names to list box

	std::for_each(projects.begin(), projects.end(), [&](const Models::Project& p) {
		projectsListBox.AddString(CString(p.name.c_str()));
	});
}


void CmfcRequestDlg::OnLbnSelchangeList1()
{
	// TODO: Add your control notification handler code here
}
