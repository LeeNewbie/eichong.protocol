#pragma once
#include "afxwin.h"


// CPrintMsg 对话框

class CPrintMsg : public CDialog
{
	DECLARE_DYNAMIC(CPrintMsg)

public:
	CPrintMsg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPrintMsg();

	virtual BOOL OnInitDialog();
// 对话框数据
	enum { IDD = IDD_DIALOG14 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnBnClickedOk();
	CComboBox m_PrintMsg;

	CString isPrint;
};
