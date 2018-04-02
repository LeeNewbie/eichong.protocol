// FromOper.cpp : 实现文件
//

#include "stdafx.h"
#include "EpGateMaint.h"
#include "FromOper.h"


// CFromOper

IMPLEMENT_DYNCREATE(CFromOper, CFormView)

CFromOper::CFromOper()
	: CFormView(CFromOper::IDD)
{
	

}

CFromOper::~CFromOper()
{
}

void CFromOper::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_tab);
}

BEGIN_MESSAGE_MAP(CFromOper, CFormView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_SIZING()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CFromOper::OnTcnSelchangeTab1)
END_MESSAGE_MAP()

BEGIN_EASYSIZE_MAP(CFromOper)
	EASYSIZE(IDC_TAB1,ES_BORDER,ES_BORDER,ES_BORDER,ES_BORDER,0)

END_EASYSIZE_MAP

// CFromOper 诊断

#ifdef _DEBUG
void CFromOper::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CFromOper::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


int CFromOper::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFormView::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rec;  
   GetClientRect(&rec);  
   //创建tabCtrl控件  
    m_tab.Create(TCS_TABS|WS_CHILD|WS_VISIBLE,rec,this,IDC_TAB1);  
	
	m_tab.InsertItem(0,"  查看  ");
	m_tab.InsertItem(1,"  查看1  ");

	// TODO:  在此添加您专用的创建代码
//	m_epServerDialog = new CepServerDialog(this);

//	m_epServerDialog.Create(IDD_DIALOG9,GetDlgItem(IDC_TAB1));
	
  
	CRect rs;
   m_tab.GetClientRect(&rs);
    //调整子对话框在父窗口中的位置
   rs.top += 60;
   rs.bottom -= 20;
   rs.left += 100;
   rs.right -= 2;
    //设置子对话框尺寸并移动到指定位置
//    m_epServerDialog.MoveWindow(&rs);
    //分别设置隐藏和显示
 //   m_epServerDialog.ShowWindow(1);
  
    //设置默认的选项卡
    m_tab.SetCurSel(0);
	INIT_EASYSIZE;

	return 0;
}

void CFromOper::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	
	UPDATE_EASYSIZE;
}

void CFromOper::OnSizing(UINT fwSide, LPRECT pRect)
{
	CFormView::OnSizing(fwSide, pRect);

	// TODO: 在此处添加消息处理程序代码
	EASYSIZE_MINSIZE(280,250,fwSide,pRect);
}

void CFromOper::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}
