#pragma once
#include "afxwin.h"


// Copen_white 对话框

class Copen_white : public CDialog
{
	DECLARE_DYNAMIC(Copen_white)

public:
	Copen_white(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~Copen_white();

// 对话框数据
	enum { IDD = IDD_OPEN_WHITE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_ComBoxOpen;
	afx_msg void OnBnClickedOk();

	CString open;
};
