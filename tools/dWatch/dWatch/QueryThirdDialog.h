#pragma once


// CQueryThirdDialog 对话框

class CQueryThirdDialog : public CDialog
{
	DECLARE_DYNAMIC(CQueryThirdDialog)

public:
	CQueryThirdDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CQueryThirdDialog();

// 对话框数据
	enum { IDD = IDD_QUERTHIRD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
