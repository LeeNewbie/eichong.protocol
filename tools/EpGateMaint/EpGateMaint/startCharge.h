#pragma once
#include "afxwin.h"


// CstartCharge 对话框

class CstartCharge : public CDialog
{
	DECLARE_DYNAMIC(CstartCharge)

public:
	CstartCharge(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CstartCharge();

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();

// 对话框数据
	enum { IDD = IDD_DIALOG6 };

	CString froneAmt;
	CString  chargeType;
	CString  payType;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CComboBox m_comboBox;
	CComboBox m_payMode;

	
};
