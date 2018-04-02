#pragma once


// CgetEpType 对话框

class CgetEpType : public CDialog
{
	DECLARE_DYNAMIC(CgetEpType)

public:
	CgetEpType(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CgetEpType();

// 对话框数据
	enum { IDD = IDD_DIALOG15 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();

	CString type;
};
