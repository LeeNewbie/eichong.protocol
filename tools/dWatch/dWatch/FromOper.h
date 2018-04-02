#pragma once
#include "afxcmn.h"



// CFromOper 窗体视图

class CFromOper : public CFormView
{
	DECLARE_EASYSIZE

	DECLARE_DYNCREATE(CFromOper)

protected:
	CFromOper();           // 动态创建所使用的受保护的构造函数
	virtual ~CFromOper();

	

public:
	enum { IDD = IDD_FORMOPER };
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
	CTabCtrl m_tab;


	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
};


