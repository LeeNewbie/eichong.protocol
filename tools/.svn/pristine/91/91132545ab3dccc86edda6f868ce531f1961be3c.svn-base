// FromServer.cpp : 实现文件
//

#include "stdafx.h"
#include "EpGateMaint.h"
#include "FromServerList.h"


// CFromServer

IMPLEMENT_DYNCREATE(CFromServerList, CFormView)

CFromServerList::CFromServerList()
	: CFormView(CFromServerList::IDD)
{

}

CFromServerList::~CFromServerList()
{
}

void CFromServerList::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CFromServerList, CFormView)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, &CFromServerList::OnTvnSelchangedTree1)
	ON_WM_SIZE()
	ON_WM_SIZING()
	ON_WM_CREATE()
END_MESSAGE_MAP()
BEGIN_EASYSIZE_MAP(CFromServerList)
	EASYSIZE(IDC_TREE1,ES_BORDER,ES_BORDER,ES_BORDER,ES_BORDER,0)

END_EASYSIZE_MAP

// CFromServer 诊断

#ifdef _DEBUG
void CFromServerList::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CFromServerList::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CFromServer 消息处理程序

void CFromServerList::OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}

void CFromServerList::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	UPDATE_EASYSIZE;
}

void CFromServerList::OnSizing(UINT fwSide, LPRECT pRect)
{
	CFormView::OnSizing(fwSide, pRect);

	// TODO: 在此处添加消息处理程序代码
	EASYSIZE_MINSIZE(100,100,fwSide,pRect);
}

int CFromServerList::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFormView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
    INIT_EASYSIZE;
	return 0;
}
