
// BTFishDlg.h : ͷ�ļ�
//

#pragma once

#include "BTClient.h"
#include "afxcmn.h"

// CBTFishDlg �Ի���
class CBTFishDlg : public CDialog
{
// ����
public:
	CBTFishDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_BTFISH_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


private:
	//��ȡ�������õ�������Ϣ
	void GetPeerInfoFormUI();

	//����UI��ʼ��ֵ
	void InitUI();


private:
	CBTClientChannel* m_pClient;
// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
};
