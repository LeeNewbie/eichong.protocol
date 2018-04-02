#pragma once


// CupdateEp 对话框

class CupdateEp : public CDialog
{
	DECLARE_DYNAMIC(CupdateEp)

public:
	CupdateEp(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CupdateEp();

// 对话框数据
	enum { IDD = IDD_DIALOG3 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	
// 生成的消息映射函数
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();

	int flag;
	CString hardnumber;
	CString hardversion;
	CString softnumber;
	CString softversion;
	CString md5;
	CString station;
};
