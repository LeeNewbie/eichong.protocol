#pragma once
#include "afxwin.h"



// CmanagerEpcode 对话框

class CmanagerEpcode : public CDialog
{
	DECLARE_DYNAMIC(CmanagerEpcode)

public:
	CmanagerEpcode(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CmanagerEpcode();

// 对话框数据
	enum { IDD = IDD_DIALOG_EP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedOk();
	CListBox m_epCodeList;

	  CString epCodes;
	  void setParam(CString configName,CString itemName);

	  CString m_configName;
	  CString m_itemName;
};
