#pragma once


// CTempChargeNum 对话框

class CTempChargeNum : public CDialog
{
	DECLARE_DYNAMIC(CTempChargeNum)

public:
	CTempChargeNum(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTempChargeNum();

// 对话框数据
	enum { IDD = IDD_DIALOG17 };


	CString num;
	CString time;
	CString flag;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};
