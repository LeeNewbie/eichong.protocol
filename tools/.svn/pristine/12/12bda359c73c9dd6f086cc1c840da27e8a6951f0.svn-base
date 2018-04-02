#pragma once
class CEpGateMaintDlg;

// CQueryDialog 对话框

class CQueryDialog : public CDialog
{
	DECLARE_DYNAMIC(CQueryDialog)

public:
	CQueryDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CQueryDialog();


	// 生成的消息映射函数
	virtual BOOL OnInitDialog();

// 对话框数据
	enum { IDD = IDD_VIEW };

	CEpGateMaintDlg * m_dlg;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	afx_msg void OnTimer(UINT nIDEvent);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton25();
	afx_msg void OnBnClickedButton24();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnBnClickedButton6();
	int state;
    void SetCheckFalse();
	CString sec;
	afx_msg void OnBnClickedButton26();
	afx_msg void OnBnClickedButton29();
};
