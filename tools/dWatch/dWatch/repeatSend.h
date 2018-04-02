#pragma once
#include "afxwin.h"


// CrepeatSend 对话框

class CrepeatSend : public CDialog
{
	DECLARE_DYNAMIC(CrepeatSend)

public:
	CrepeatSend(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CrepeatSend();

// 对话框数据
	enum { IDD = IDD_DIALOG12 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CComboBox m_comboBox;
	CString time;
	CString flag;
	afx_msg void OnCbnSelchangeCombo1();
};
