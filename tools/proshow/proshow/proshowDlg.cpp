
// proshowDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "proshow.h"
#include "proshowDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif



class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CproshowDlg �Ի���




CproshowDlg::CproshowDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CproshowDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CproshowDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	
	DDX_Control(pDX, IDC_TAB1, m_tabCtrl);
}

BEGIN_MESSAGE_MAP(CproshowDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP


	
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CproshowDlg::OnTcnSelchangeTab)
END_MESSAGE_MAP()


// CproshowDlg ��Ϣ�������

BOOL CproshowDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

    // TODO: �ڴ���Ӷ���ĳ�ʼ������
	 //��ʼ��
//	AfxSocketInit();
	m_tabCtrl.InsertItem(0,"ͨѶЭ�����");

//	m_tabCtrl.InsertItem(1,"ģ���׮����");

	m_decodeDialog.Create(IDD_DIALOG_AC_DECODE,GetDlgItem(IDC_TAB1));
	//m_eptestDialog.Create(IDD_DIALOG_EPTEST,GetDlgItem(IDC_TAB1));

	 CRect rs;
    m_tabCtrl.GetClientRect(&rs);
    //�����ӶԻ����ڸ������е�λ��
    rs.top += 20;
    rs.bottom -= 20;
    rs.left += 1;
    rs.right -= 2;
    //�����ӶԻ���ߴ粢�ƶ���ָ��λ��
    m_decodeDialog.MoveWindow(&rs);
	//m_eptestDialog.MoveWindow(&rs);
    
    //�ֱ��������غ���ʾ
    m_decodeDialog.ShowWindow(1);
	//m_eptestDialog.ShowWindow(0);
  
    //����Ĭ�ϵ�ѡ�
    m_tabCtrl.SetCurSel(0);

   	 //��ʼ��
	AfxSocketInit();

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CproshowDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CproshowDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CproshowDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CproshowDlg::OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;

	int CurSel = m_tabCtrl.GetCurSel();
 
    switch(CurSel)
    {
     case 0:
       m_decodeDialog.ShowWindow(true);
     //  m_eptestDialog.ShowWindow(false);
     break;
     case 1:
       m_decodeDialog.ShowWindow(false);
    //   m_eptestDialog.ShowWindow(true);
     break;
     default:
     break;
    }
}
