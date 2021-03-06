
// BTFishDlg.h : 头文件
//

#pragma once

#include "BTClient.h"
#include "afxcmn.h"
#include "DownloadInterface.h"

// CBTFishDlg 对话框
class CBTFishDlg : public CDialog
{
// 构造
public:
	CBTFishDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_BTFISH_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


private:
	//获取界面设置的配置信息
	void GetPeerInfoFormUI();

	//设置UI初始化值
	void InitUI();

    bool InitDownload();

	void LoadConfig();

	void SaveConfig();

private:
	//helper Function
	void EableWnd(int iID, BOOL bTrue = TRUE);

private:
	CBTClientChannel* m_pClient;
// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedBtnStop();
private:
	int m_Port;
	CString m_InfoHash;
	CString m_InfoPeerConfig;
	CIPAddressCtrl m_IPCCtrl;
	CString	m_PeerIP;
	UINT m_PieceCount;
public:
    UINT m_ulPieceSize;
    IDownLoad*  m_IDownload;
    ULONGLONG m_i64FileSize;
    TASK_HANDLE m_hDownloadTask;
    CString m_strSavePath;
	afx_msg void OnClose();
};
