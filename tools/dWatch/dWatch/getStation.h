#pragma once


// CgetStation 对话框

class CgetStation : public CDialog
{
	DECLARE_DYNAMIC(CgetStation)

public:
	CgetStation(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CgetStation();

// 对话框数据
	enum { IDD = IDD_DIALOG8 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CString stationAddr;
};
