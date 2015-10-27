
// BT FishDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "BTFish.h"
#include "BTFishDlg.h"
#include "CommBuffer.h"
#include "BTTask.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CBTFishDlg 对话框




CBTFishDlg::CBTFishDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBTFishDlg::IDD, pParent)
	, m_pClient(NULL)
	, m_Port(0)
	, m_InfoHash(_T(""))
	, m_InfoPeerConfig(_T(""))
	, m_PieceCount(1)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CBTFishDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_PORT, m_Port);
	DDX_Text(pDX, IDC_EDIT_INFOHASH, m_InfoHash);
	DDX_Text(pDX, IDC_EDIT_PEERCONFIG, m_InfoPeerConfig);
	DDX_Control(pDX, IDC_IPADDRESS_PEER, m_IPCCtrl);
	DDX_Text(pDX, IDC_EDIT_PIECECOUNT, m_PieceCount);
}

BEGIN_MESSAGE_MAP(CBTFishDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CBTFishDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BTN_STOP, &CBTFishDlg::OnBnClickedBtnStop)
END_MESSAGE_MAP()


// CBTFishDlg 消息处理程序

BOOL CBTFishDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	InitUI();
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CBTFishDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CBTFishDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CBTFishDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CBTFishDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	GetPeerInfoFormUI();
	std::string ip = CW2A(m_PeerIP);
	std::string InfoHash = CW2A(m_InfoHash);

	//先测试阻塞
	CBTTask::BTInfo info;
	info.FileSize = 48201;
	info.InfoHash = InfoHash;
	info.PieceSize = 16384*2;
	CBTTask* pTask = new CBTTask(info);
	pTask->Start();
}

void CBTFishDlg::GetPeerInfoFormUI()
{
	UpdateData();
	//配置文件的解析
	m_IPCCtrl.GetWindowText(m_PeerIP);
	
}


void CBTFishDlg::InitUI()
{
	m_IPCCtrl.SetWindowText(_T("127.0.0.1"));
	m_Port = 11038;
	m_InfoHash = _T("b628a353e19abfd3e7671140bdb9dd06ff987f12");
	UpdateData(FALSE);
	GetDlgItem(IDC_BTN_STOP)->EnableWindow(FALSE);
}

void CBTFishDlg::OnBnClickedBtnStop()
{
	// TODO: 在此添加控件通知处理程序代码
	//做关闭处理
}
