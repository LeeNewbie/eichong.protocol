#pragma once


// CstopBespno 对话框

class CstopBespno : public CDialog
{
	DECLARE_DYNAMIC(CstopBespno)

public:
	CstopBespno(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CstopBespno();

// 对话框数据
	enum { IDD = IDD_DIALOG5 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()


public:
		CString bespno;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButton1();
};
