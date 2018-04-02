#pragma once
class CEpGateMaintDlg;

// CTestEpDialog 对话框

class CTestEpDialog : public CDialog
{
	DECLARE_DYNAMIC(CTestEpDialog)

public:
	CTestEpDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTestEpDialog();

	CEpGateMaintDlg * m_dlg;
// 对话框数据
	enum { IDD = IDD_TESTEP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton20();
	afx_msg void OnBnClickedButton21();
	afx_msg void OnBnClickedButton22();
	afx_msg void OnBnClickedButton23();
};
