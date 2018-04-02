#pragma once
#include "afxwin.h"


// Cflashram 对话框

class Cflashram : public CDialog
{
	DECLARE_DYNAMIC(Cflashram)

public:
	Cflashram(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~Cflashram();

// 对话框数据
	enum { IDD = IDD_DIALOG10 };

		// 生成的消息映射函数
	virtual BOOL OnInitDialog();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnCbnSelchangeCombo1();
	CComboBox m_ComBoxType;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();

	CString flag;
	CString type;
	CString startPos;
	CString len;
	CString stationAddr;
};
