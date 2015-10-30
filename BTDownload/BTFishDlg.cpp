
// BT FishDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "BTFish.h"
#include "BTFishDlg.h"
#include "CommBuffer.h"
#include "BTTask.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CBTFishDlg �Ի���




CBTFishDlg::CBTFishDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBTFishDlg::IDD, pParent)
	, m_pClient(NULL)
	, m_Port(0)
	, m_InfoHash(_T(""))
	, m_InfoPeerConfig(_T(""))
	, m_PieceCount(1)
    , m_ulPieceSize(0)
    , m_IDownload(NULL)
    , m_i64FileSize(0)
    , m_strSavePath(_T(""))
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
    DDX_Text(pDX, IDC_EDIT1, m_ulPieceSize);
    DDX_Text(pDX, IDC_EDIT2, m_i64FileSize);
    DDX_Text(pDX, IDC_EDIT3, m_strSavePath);
}

BEGIN_MESSAGE_MAP(CBTFishDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CBTFishDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BTN_STOP, &CBTFishDlg::OnBnClickedBtnStop)
END_MESSAGE_MAP()


// CBTFishDlg ��Ϣ�������

BOOL CBTFishDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	InitUI();
    
    //���س��ڳ�ʼ��
    InitDownload();
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CBTFishDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CBTFishDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CBTFishDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	GetPeerInfoFormUI();
	std::string ip = CW2A(m_PeerIP);
	std::string InfoHash = CW2A(m_InfoHash);
    std::string SavePath = CW2A(m_strSavePath);

    m_IDownload->Init(ip,m_Port);
    m_IDownload->CreateBTTask(InfoHash,m_PieceCount,m_ulPieceSize,m_i64FileSize,SavePath,&m_hDownloadTask);
    m_IDownload->StartTask(m_hDownloadTask);
    
}

void CBTFishDlg::GetPeerInfoFormUI()
{
	UpdateData();
	//�����ļ��Ľ���
	m_IPCCtrl.GetWindowText(m_PeerIP);
	
}


void CBTFishDlg::InitUI()
{
	m_IPCCtrl.SetWindowText(_T("127.0.0.1"));
	m_Port = 11038;
	m_InfoHash = _T("b628a353e19abfd3e7671140bdb9dd06ff987f12");
    m_PieceCount = 2;
    m_ulPieceSize = 16384 * 2;
    m_i64FileSize = 48201;
    m_strSavePath = _T("D:\\MyTest.xml");
	UpdateData(FALSE);
	GetDlgItem(IDC_BTN_STOP)->EnableWindow(FALSE);
}

void CBTFishDlg::OnBnClickedBtnStop()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//���رմ���
}

bool CBTFishDlg::InitDownload()
{
    m_IDownload = GetIDownLoad();
    return true;
}
