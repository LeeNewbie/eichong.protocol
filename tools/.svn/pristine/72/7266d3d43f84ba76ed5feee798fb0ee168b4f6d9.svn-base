#pragma once


// CconsumeRecord 对话框

class CconsumeRecord : public CDialog
{
	DECLARE_DYNAMIC(CconsumeRecord)

public:
	CconsumeRecord(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CconsumeRecord();

		// 生成的消息映射函数
	virtual BOOL OnInitDialog();
// 对话框数据
	enum { IDD = IDD_DIALOG7 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();

	CString startNum;
	CString entryNum;
};
