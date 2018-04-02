#pragma once
#include "afxwin.h"


// CStopCarOrgan 对话框

class CStopCarOrgan : public CDialog
{
	DECLARE_DYNAMIC(CStopCarOrgan)

public:
	CStopCarOrgan(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CStopCarOrgan();

// 对话框数据
	enum { IDD = IDD_STOPCARORGAN };

	
	// 生成的消息映射函数
	virtual BOOL OnInitDialog();

	

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CString use;
	CComboBox m_ComboBoxUse;
	CString time;
};
