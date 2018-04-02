#pragma once


// m_space 对话框

class m_space : public CDialog
{
	DECLARE_DYNAMIC(m_space)

public:
	m_space(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~m_space();

// 对话框数据
	enum { IDD = IDD_DIALOG16 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
