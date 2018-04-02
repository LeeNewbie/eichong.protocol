#pragma once


// Crate 对话框

class Crate : public CDialog
{
	DECLARE_DYNAMIC(Crate)

public:
	Crate(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~Crate();

// 对话框数据
	enum { IDD = IDD_RATE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

	
public:
	afx_msg void OnBnClickedOk();

	CString rateid;
};
