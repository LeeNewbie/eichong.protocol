// EpTest.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "proshow.h"
#include "EpTest.h"
#include "tcpSocket.h"
#include "iec104_ep.h"

// CEpTest �Ի���


 CProto_IEC104_C  * g_iec104_ep = new CProto_IEC104_C();

CtcpSocket  * m_tcpSocket;

UINT ThreadFunc(LPVOID)
{
   while(1)
   {
	   Sleep(500);

	   unsigned char	buffer[BUFSIZE_104];
	   unsigned int leng =  g_iec104_ep->getTx(buffer,BUFSIZE_104);
	   if(leng>0)
	   {
	      m_tcpSocket->Send(buffer,leng);
	   }

	   leng =  m_tcpSocket->Receive((void *)buffer, BUFSIZE_104);

	   g_iec104_ep->putRx(buffer,leng);
	  
		
   }

	return 0;
}


IMPLEMENT_DYNAMIC(CEpTest, CDialog)

CEpTest::CEpTest(CWnd* pParent /*=NULL*/)
	: CDialog(CEpTest::IDD, pParent)
{
  
}

CEpTest::~CEpTest()
{
	delete g_iec104_ep;
	g_iec104_ep = NULL;

}

void CEpTest::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CEpTest, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CEpTest::OnBnClickedConect)
END_MESSAGE_MAP()


// CEpTest ��Ϣ�������

void CEpTest::OnBnClickedConect()
{
	this->GetDlgItem(IDC_EDIT1)->SetWindowText("2406");
    this->GetDlgItem(IDC_IPADDRESS1)->SetWindowText("127.0.0.1");

	m_tcpSocket = new CtcpSocket();
	//��ʼ�� CSocket ����, ��Ϊ�ͻ��˲���Ҫ���κζ˿ں͵�ַ, ������Ĭ�ϲ�������
	if(!m_tcpSocket->Create())
	{
		char szMsg[1024] = {0};
		sprintf(szMsg, "create socket faild: %d", m_tcpSocket->GetLastError());
		AfxMessageBox(szMsg);
		return;
	}

	this->GetDlgItem(IDC_IPADDRESS1)->GetWindowText(m_strIP);
	CString strPort;
	this->GetDlgItem(IDC_EDIT1)->GetWindowText(strPort);
	//ת����Ҫ���ӵĶ˿���������
	m_nPort = atoi(strPort);

	//����ָ���ĵ�ַ�Ͷ˿�	
	if(!m_tcpSocket->Connect(m_strIP, m_nPort))
	{
		char szMsg[1024] = {0};
		sprintf(szMsg, "create faild: %d", m_tcpSocket->GetLastError());
		AfxMessageBox(szMsg);
        return;
	}

	AfxBeginThread(ThreadFunc, 0,THREAD_PRIORITY_NORMAL,0,0,NULL);
                 
	
}



