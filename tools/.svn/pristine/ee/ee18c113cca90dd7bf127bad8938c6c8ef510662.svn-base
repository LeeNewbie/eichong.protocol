#pragma once
#include "afxwin.h"


// CCallEp 对话框

class CCallEp : public CDialog
{
	DECLARE_DYNAMIC(CCallEp)

public:
	CCallEp(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCallEp();

// 对话框数据
	enum { IDD = IDD_CALLEP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CComboBox m_comboBox;
	CString type;
	CString time;
	afx_msg void OnCbnSelchangeCombo1();
};
