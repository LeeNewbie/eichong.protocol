#pragma once
class CEpGateMaintDlg;

// CTestEpDialog �Ի���

class CTestEpDialog : public CDialog
{
	DECLARE_DYNAMIC(CTestEpDialog)

public:
	CTestEpDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTestEpDialog();

	CEpGateMaintDlg * m_dlg;
// �Ի�������
	enum { IDD = IDD_TESTEP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton20();
	afx_msg void OnBnClickedButton21();
	afx_msg void OnBnClickedButton22();
	afx_msg void OnBnClickedButton23();
};
