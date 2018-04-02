#pragma once


// CVinCode 对话框

class CVinCode : public CDialog
{
	DECLARE_DYNAMIC(CVinCode)

public:
	CVinCode(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CVinCode();

// 对话框数据
	enum { IDD = IDD_DIALOG9 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CString vinCode;
};
