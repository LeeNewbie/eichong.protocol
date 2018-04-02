#pragma once
class CEpGateMaintDlg;

// CTestEpGate 对话框

class CTestEpGate : public CDialog
{
	DECLARE_DYNAMIC(CTestEpGate)

public:
	CTestEpGate(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTestEpGate();

	CEpGateMaintDlg * m_dlg;
// 对话框数据
	enum { IDD = IDD_TESTEPGATE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

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
