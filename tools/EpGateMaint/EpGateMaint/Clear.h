#pragma once

class CEpGateMaintDlg;

// CClear �Ի���

class CClear : public CDialog
{
	DECLARE_DYNAMIC(CClear)

public:
	CClear(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CClear();

// �Ի�������
	enum { IDD = IDD_CLEAR };



CEpGateMaintDlg * m_dlg;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton14();
	afx_msg void OnBnClickedButton17();
	afx_msg void OnBnClickedButton16();
	afx_msg void OnBnClickedButton19();
};
