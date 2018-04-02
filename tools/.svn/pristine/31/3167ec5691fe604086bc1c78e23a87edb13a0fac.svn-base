#pragma once
#include "afxwin.h"


// CBespno 对话框

class CBespno : public CDialog
{
	DECLARE_DYNAMIC(CBespno)

public:
	CBespno(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CBespno();

// 对话框数据
	enum { IDD = IDD_BESPNO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();

	CString redo;
	CString timet;
	afx_msg void OnBnClickedOk();

	int flag;
	CString bespno;
	CString  payType;
	CString pw;

	afx_msg void OnBnClickedButton1();
	CComboBox m_payMode;
	CComboBox m_BespnoTime;
};
