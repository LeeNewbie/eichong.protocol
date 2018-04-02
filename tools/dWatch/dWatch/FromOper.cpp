// FromOper.cpp : ʵ���ļ�
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

// CFromOper ���

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
   //����tabCtrl�ؼ�  
    m_tab.Create(TCS_TABS|WS_CHILD|WS_VISIBLE,rec,this,IDC_TAB1);  
	
	m_tab.InsertItem(0,"  �鿴  ");
	m_tab.InsertItem(1,"  �鿴1  ");

	// TODO:  �ڴ������ר�õĴ�������
//	m_epServerDialog = new CepServerDialog(this);

//	m_epServerDialog.Create(IDD_DIALOG9,GetDlgItem(IDC_TAB1));
	
  
	CRect rs;
   m_tab.GetClientRect(&rs);
    //�����ӶԻ����ڸ������е�λ��
   rs.top += 60;
   rs.bottom -= 20;
   rs.left += 100;
   rs.right -= 2;
    //�����ӶԻ���ߴ粢�ƶ���ָ��λ��
//    m_epServerDialog.MoveWindow(&rs);
    //�ֱ��������غ���ʾ
 //   m_epServerDialog.ShowWindow(1);
  
    //����Ĭ�ϵ�ѡ�
    m_tab.SetCurSel(0);
	INIT_EASYSIZE;

	return 0;
}

void CFromOper::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	
	UPDATE_EASYSIZE;
}

void CFromOper::OnSizing(UINT fwSide, LPRECT pRect)
{
	CFormView::OnSizing(fwSide, pRect);

	// TODO: �ڴ˴������Ϣ����������
	EASYSIZE_MINSIZE(280,250,fwSide,pRect);
}

void CFromOper::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
}
