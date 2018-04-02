#pragma once


// MessageDialog 对话框

class MessageDialog : public CDialog
{
	DECLARE_DYNAMIC(MessageDialog)

public:
	MessageDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~MessageDialog();

// 对话框数据
	enum { IDD = IDD_DIALOG4 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持


	
	DECLARE_MESSAGE_MAP()

public:
	CString msg;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButton1();
};
