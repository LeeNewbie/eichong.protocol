#pragma once
class CEpGateMaintDlg;

// CTestEpGate �Ի���

class CTestEpGate : public CDialog
{
	DECLARE_DYNAMIC(CTestEpGate)

public:
	CTestEpGate(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTestEpGate();

	CEpGateMaintDlg * m_dlg;
// �Ի�������
	enum { IDD = IDD_TESTEPGATE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton7();
	afx_msg void OnBnClickedButton8();
	afx_msg void OnBnClickedButton9();
	afx_msg void OnBnClickedButton10();
	afx_msg void OnBnClickedButton11();
	afx_msg void OnBnClickedButton12();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
};
