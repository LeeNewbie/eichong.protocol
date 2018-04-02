#pragma once



// CFromServer 窗体视图

class CFromServerList : public CFormView
{
	DECLARE_EASYSIZE
	DECLARE_DYNCREATE(CFromServerList)

protected:
	CFromServerList();           // 动态创建所使用的受保护的构造函数
	virtual ~CFromServerList();

public:
	enum { IDD = IDD_FORMSERVER };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};


