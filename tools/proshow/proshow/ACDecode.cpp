// ACDecode.cpp : 实现文件
//

#include "stdafx.h"
#include "proshow.h"
#include "ACDecode.h"
#include "iec104_decode.h"


CProto_IEC104  *g_iec104=NULL;
// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

// CACDecode 对话框

IMPLEMENT_DYNAMIC(CACDecode, CDialog)

CACDecode::CACDecode(CWnd* pParent /*=NULL*/)
	: CDialog(CACDecode::IDD, pParent)
{

}

CACDecode::~CACDecode()
{
}

void CACDecode::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_com);
}


BEGIN_MESSAGE_MAP(CACDecode, CDialog)
	ON_BN_CLICKED(IDC_cleanMsg, &CACDecode::OnBnClickedcleanmsg)
	ON_BN_CLICKED(IDC_cleanPro, &CACDecode::OnBnClickedcleanpro)
	ON_BN_CLICKED(IDC_parse, &CACDecode::OnBnClickedparse)

END_MESSAGE_MAP()

	

// CACDecode 消息处理程序

BOOL CACDecode::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_com.AddString("V1.2");
	m_com.AddString("V1.3");
	m_com.AddString("V3.0");
	m_com.AddString("V4.0");

	m_com.SetCurSel(2);
	UpdateData(false); 


	// TODO: 在此添加额外的初始化代码
	g_iec104 = new CProto_IEC104(this);
	return true;
}


void CACDecode::OnBnClickedcleanmsg()
{
	// TODO: 在此添加控件通知处理程序代码
	((CEdit *)GetDlgItem(IDC_Msg))->SetWindowText("");//
}

void CACDecode::OnBnClickedcleanpro()
{
	// TODO: 在此添加控件通知处理程序代码
	((CEdit *)GetDlgItem(IDC_Pro))->SetWindowText("");//
}

void CACDecode::OnBnClickedparse()
{
	// TODO: 在此添加控件通知处理程序代码
	getMsg();
	
    g_iec104->ParaData(m_dataMsg,m_leng);
	
}

void CACDecode::ProDecodeShow(CString str)
{
	// TODO: 在此添加控件通知处理程序代码
    ((CEdit *)GetDlgItem(IDC_Pro))->SetWindowText(str);//

	
}
int CACDecode::getProVersion()
{
	int version=12;

	switch(m_com.GetCurSel())
	{
	case 0:
		version=12;
		break;
	case 1:
		version=13;
		break;
	case 2:
		version=30;
		break;
	case 3:
		version=40;
		break;
	default:
		break;
	}
	return version;
}

void CACDecode::getMsg()
{

	m_leng=0;
   CString Msg;
    GetDlgItem(IDC_Msg)->GetWindowText(Msg);
	if(Msg==" ")
	{ 
		AfxMessageBox("请输入数据");
		return;
	}
	else 
	{
		Msg.TrimLeft(" ");
        Msg.TrimRight(" ");
		int k=0;
		unsigned char *data=new unsigned char[Msg.GetLength()];
		for(int i=0;i<Msg.GetLength();i++)
		{
		   if(Msg[i]==' '|| (Msg[i]==0x0d && Msg[i+1]==0x0a) || Msg[i] == 0x0a)
		   {}
		   else
		   {
			   data[k]=Msg[i];
			   k++;
		   }
		}
		if((k%2)!=0)
		{
			   CString message1,message2,message3;
			   message1.Format("%d",k);
			   message2="字符数为";
			   message2=message2+message1;
			   message3="不是2的倍数,请检查输入的数据";
			   message3=message2+message3;
			   
			   AfxMessageBox( message3);
			   delete data;
			   return;
		}
		else
		{
			for(int h=0;h<k;h++)
			{
				if( data[h]<=57&& data[h]>=48)
				{
		            data[h]= data[h]-48;
				}
	            else if( data[h]<=70&& data[h]>=65)
				{
		            data[h]= data[h]-55;
				}
	            else if( data[h]<=102&& data[h]>=97)
				{
		            data[h]= data[h]-87;
				} 
				else
				{
				    AfxMessageBox("发现错误字符");
					 delete data;
					return;
				}
			}
		   
			int g=0;
		    for(int j=0;j<k/2;j++)
			{
                m_dataMsg[j]=data[g]*16+data[g+1];
				g=g+2;
			}
			m_leng=k/2;

		   
			delete data;
		}
 	}
}