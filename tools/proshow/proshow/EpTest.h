#pragma once




// CEpTest �Ի���

class CEpTest : public CDialog
{
	DECLARE_DYNAMIC(CEpTest)

public:
	CEpTest(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CEpTest();

// �Ի�������
	enum { IDD = IDD_DIALOG_EPTEST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedConect();

	 CString m_strIP;
	 int m_nPort;
	
};
