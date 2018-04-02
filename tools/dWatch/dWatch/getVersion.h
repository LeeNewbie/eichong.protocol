#pragma once


// CgetVersion 对话框

class CgetVersion : public CDialog
{
	DECLARE_DYNAMIC(CgetVersion)

public:
	CgetVersion(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CgetVersion();

// 对话框数据
	enum { IDD = IDD_GETVER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedOk();

	int flag;
	CString stationAddr;
};
