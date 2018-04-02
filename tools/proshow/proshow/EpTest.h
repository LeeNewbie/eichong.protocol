#pragma once




// CEpTest 对话框

class CEpTest : public CDialog
{
	DECLARE_DYNAMIC(CEpTest)

public:
	CEpTest(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CEpTest();

// 对话框数据
	enum { IDD = IDD_DIALOG_EPTEST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedConect();

	 CString m_strIP;
	 int m_nPort;
	
};
