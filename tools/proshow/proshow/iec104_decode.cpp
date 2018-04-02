#include "stdafx.h"
#include "iec104_decode.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//----------------------------------------------------------------------------
CProto_IEC104::CProto_IEC104(CACDecode *dlg)
{
	m_Dlg = dlg;
}

CProto_IEC104::~CProto_IEC104()
{
}


void CProto_IEC104::ParaData(unsigned char *buff,int len)
{
	int DataSize = 0;
	if(buff==0)
		return;
	if(len<4)
	{
		CString str("报文长度错误，最少4个字节");
		m_Dlg->ProDecodeShow(str);
	       return;
	}
	else
	{
		if(buff[0] == 0x68)
		{
			buff = buff+3;
			len = len-3;
		}
		int ret = SearchAPCI(buff,len);

		if(ret == IEC104_APCI_I)
		{
			SearchASDU(buff,len);
		}
		

	}
}


//----------------------------------------------------------------------------
int	CProto_IEC104::SearchAPCI(unsigned char *buffer, int len)
{
	CString str;
	//I格式
	if((buffer[0]&0x01) == IEC104_APCI_I)
	{
		if(len <= 4)
		{
			str="(第1个字节）控制域八位位组1判读出I帧数据，但长度错误，应大于4个字节";
			m_Dlg->ProDecodeShow(str);
			return	-1;
		}
		return	IEC104_APCI_I;
	}
	//S格式
	else	if((buffer[0]&0x03) == IEC104_APCI_S)
	{
		if(len != 4)
		{
			str="(第1个字节）控制域八位位组1判读出S帧数据，但长度错误，应等于4个字节";
			m_Dlg->ProDecodeShow(str);
			return	-1;
		}
        str.Format("S帧：%02x %02x（接收序号：%d）",buffer[2],buffer[3],(buffer[2]+buffer[3]*0x100)>>1);
		m_Dlg->ProDecodeShow(str);
		return	IEC104_APCI_S;
	}
	//U格式
	else	if(((buffer[0]&0x03) == IEC104_APCI_U) && (buffer[0]==0) && (buffer[1] == 0) && (buffer[2]==0))
	{
		if(len != 4)
		{
			str="(第1个字节）控制域八位位组1判读出U帧数据，但长度错误，应等于4个字节";
			m_Dlg->ProDecodeShow(str);
			return	-1;
		}
		switch(buffer[0] & 0xFF)
		{
		case	APCI_U_STARTDT_ACT:	
			{
				 str="U帧激活帧";
		    	m_Dlg->ProDecodeShow(str);
			}
			break;
		case	APCI_U_STARTDT_CON:
			{
				 str="U帧激活确认帧";
		    	m_Dlg->ProDecodeShow(str);
			}
			
			break;
		case	APCI_U_STOPDT_ACT:
			{
				 str="U帧停止帧";
		    	m_Dlg->ProDecodeShow(str);
			}
			
			break;
		case	APCI_U_STOPDT_CON:
			{
				str="U帧停止确认帧";
		    	m_Dlg->ProDecodeShow(str);
			}
			
			break;
		case	APCI_U_TESTFR_CON:
			{
				str="U帧测试确认帧";
		    	m_Dlg->ProDecodeShow(str);
			}
			
			break;
		case	APCI_U_TESTFR_ACT:
			{
				 str="U帧测试激活帧";
		    	m_Dlg->ProDecodeShow(str);
			}
			
			break;
		default:
			{
                str="U帧报文错误";
		    	m_Dlg->ProDecodeShow(str);
			}
			return	-1;
		}
		return	IEC104_APCI_U;
	}
	else
	{
		str="U帧报文错误";
		m_Dlg->ProDecodeShow(str);
	}
	return	-1;
}

CString   CProto_IEC104::headMsg(CString strMsg,unsigned char *buffer,CString desc)
{
	
	CString str;
	str.Format("\r\n%02x(ASDU 类型标识:%s:%d)\r\n\r\n%02x(可变结构限定词:%d)\r\n",
		buffer[0],desc,buffer[0],
		buffer[1],buffer[1]);
	strMsg=strMsg+str;

	str.Format("%02x %02x(传送原因:",buffer[2],buffer[3]);
	strMsg=strMsg+str;

	strMsg=cosMsg(strMsg,buffer[2]);

	str.Format("%02x %02x(公共地址:%d)\r\n%02x %02x %02x(信息对象地址:%d)\r\n%02x(%s限定词QOI:%d)\r\n",
		buffer[4],buffer[5],buffer[4]+buffer[5]*256,
		buffer[6],buffer[6],buffer[8],buffer[6]+buffer[7]*256+buffer[8]*256*256,
		buffer[9],desc,buffer[9]);
	strMsg=strMsg+str;
	return strMsg;
}

CString   CProto_IEC104::headMsg1(CString strMsg,unsigned char *buffer,CString desc)
{
	
	CString str;
	str.Format("\r\n%02x(ASDU 类型标识:%s:%d)\r\n\r\n%02x(可变结构限定词:%d)\r\n",
		buffer[0],desc,buffer[0],
		buffer[1],buffer[1]);
	strMsg=strMsg+str;

	str.Format("%02x %02x(传送原因:",buffer[2],buffer[3]);
	strMsg=strMsg+str;

	strMsg=cosMsg(strMsg,buffer[2]);

	str.Format("%02x %02x(公共地址:%d)\r\n%02x %02x %02x(信息对象地址:%d)\r\n",
		buffer[4],buffer[5],buffer[4]+buffer[5]*256,
		buffer[6],buffer[6],buffer[8],buffer[6]+buffer[7]*256+buffer[8]*256*256);
	strMsg=strMsg+str;
	return strMsg;
}

CString   CProto_IEC104::cmdTimeMsg(CString strMsg,CString strErr,unsigned char *buffer,int len)
{
	CString str1;
	if(m_Dlg->getProVersion()>=40)
	{
		if(IsValide(strErr,buffer,len,3))
		{
			

			str1.Format("%02x 命令发送时间：时：%d)\r\n",buffer[0],buffer[0]);
			strMsg=strMsg+str1;

			str1.Format("%02x 命令发送时间：分：%d)\r\n",buffer[1],buffer[1]);
			strMsg=strMsg+str1;

			str1.Format("%02x 命令发送时间：秒：%d)\r\n",buffer[2],buffer[2]);
			strMsg=strMsg+str1;
		}
		else
		{
			str1.Format("(命令发送时间长度错误)\r\n");
			strMsg = strMsg+strErr+str1;
		}
	}
	
	return strMsg;
}

CString   CProto_IEC104::crcMsg(CString strMsg,CString strErr,unsigned char *buffer,int len)
{
	CString str1;
	if(m_Dlg->getProVersion()>=40)
	{
		if(IsValide(strErr,buffer,len,2))
		{
			

			str1.Format("%02x %02x(crc校验码：%d)\r\n",buffer[0],buffer[1],buffer[0]+buffer[1]*0x100);
			strMsg=strMsg+str1;

		}
		else
		{
			str1.Format("(crc校验码长度错误)\r\n");
			strMsg = strMsg+strErr+str1;
		}
	}
	
	return strMsg;
}


CString   CProto_IEC104::timeMsg(CString strMsg,unsigned char *buffer,CString timeName)
{
	int i=0;
	CString str1;
	str1.Format("%02x %02x (%s秒：%d 毫秒：%d)\r\n",buffer[i],buffer[i+1],timeName,(buffer[i]+buffer[i+1]*256)/1000,(buffer[i]+buffer[i+1]*256)%1000);
	strMsg=strMsg+str1;

	i=i+2;
	str1.Format("%02x %s分：%d)\r\n",buffer[i],timeName,buffer[i]& 0x3F);
	strMsg=strMsg+str1;

	i++;

	str1.Format("%02x %s时：%d)\r\n",buffer[i],timeName,buffer[i]& 0x1F);
	strMsg=strMsg+str1;

	i++;
	str1.Format("%02x %s日：%d)\r\n",buffer[i],timeName,buffer[i]& 0x1F);
	strMsg=strMsg+str1;

	i++;
	str1.Format("%02x %s月：%d)\r\n",buffer[i],timeName,buffer[i]& 0x0F);
	strMsg=strMsg+str1;

	i++;
	str1.Format("%02x %s年：%d)\r\n",buffer[i],timeName,(buffer[i]&0x7F) + 2000);
	strMsg=strMsg+str1;
	return strMsg;
}
int	CProto_IEC104::SearchASDU(unsigned char *buffer, int len)
{
	
	CString str,str1;

	if( buffer+4==NULL || len - 4==0)
	{
		str="I帧报文错误,无ASDU数据";
		m_Dlg->ProDecodeShow(str);
		return	false;
	}
	str.Format("%02x %02x(控制域八位位组1和2,I帧发送序列号%d)\r\n%02x %02x(控制域八位位组3和4,I帧接收序列号%d)\r\n",
		buffer[0],buffer[1],(buffer[0]>>1)&0x7f+buffer[1]*0x100,buffer[2],buffer[3],(buffer[2]>>1)&0x7f+buffer[3]*0x100);
 
	switch(buffer[4])
	{
	case	C_IC_NA_1:		//总召唤
		{
			if(m_Dlg->getProVersion() <40)
			{
				if(len !=14)
				{
					str1="总召唤错误，应该为14字节";
					str = str+str1;
					m_Dlg->ProDecodeShow(str);
				}
				else
				{
					str= headMsg(str,buffer+4,"总召唤");
					m_Dlg->ProDecodeShow(str);
				}
			}
			else
			{
				if(len !=14+5)
				{
					str1="总召唤错误，应该为19字节";
					str = str+str1;
					m_Dlg->ProDecodeShow(str);
				}
				else
				{
					str= headMsg(str,buffer+4,"总召唤");
					int i=i+14;
					str= cmdTimeMsg(str,str1,buffer+i,5);
					i=i+3;
					str= crcMsg(str,str1,buffer+i,2);
					m_Dlg->ProDecodeShow(str);
				}
			}
		}
		break;
	case	C_CI_NA_1:		//计数量召唤
		{
			if(m_Dlg->getProVersion() <40)
			{
				if(len !=14)
				{
					str1="计数量召唤错误，应该为14字节";
					str = str+str1;
					m_Dlg->ProDecodeShow(str);
				}
				else
				{
					str= headMsg(str,buffer+4,"计数量召唤");
					m_Dlg->ProDecodeShow(str);
				}
			}
			else
			{
				if(len !=14+5)
				{
					str1="计数量召唤错误，应该为19字节";
					str = str+str1;
					m_Dlg->ProDecodeShow(str);
				}
				else
				{
					str= headMsg(str,buffer+4,"计数量召唤");
					int i=i+14;
					str= cmdTimeMsg(str,str1,buffer+i,5);
					i=i+3;
					str= crcMsg(str,str1,buffer+i,2);
					m_Dlg->ProDecodeShow(str);
				}
			}
		}
		break;
	case	C_CS_NA_1:		//时钟同步
		{
			if(m_Dlg->getProVersion() <40)
			{
				if(len !=23-3)
				{
					str1="时钟同步错误，应该为23字节";
					str = str+str1;
					m_Dlg->ProDecodeShow(str);
				}
				else
				{
					str= headMsg1(str,buffer+4,"时间同步");
					int i=16-3;
					str= timeMsg(str,buffer+i,"时间");
					i=i+7;
					str= cmdTimeMsg(str,str1,buffer+i,5);
					i=i+3;
					str= crcMsg(str,str1,buffer+i,2);
					m_Dlg->ProDecodeShow(str);
				}
			}
			else
			{
				if(len !=23-3+5)
				{
					str1="时钟同步错误，应该为25字节";
					str = str+str1;
					m_Dlg->ProDecodeShow(str);
				}
				else
				{
					str= headMsg1(str,buffer+4,"时间同步");
					int i=16-3;
					str= timeMsg(str,buffer+i,"时间");
					i=i+7;
					str= cmdTimeMsg(str,str1,buffer+i,5);
					i=i+3;
					str= crcMsg(str,str1,buffer+i,2);
					m_Dlg->ProDecodeShow(str);
				}
			}
		}
		break;
	case	M_SP_NA_1:		//单点信息
		Pro_one_yx(str,buffer,len);
		break;
	case	M_DP_NA_1:		//双点信息
		Pro_two_yx(str,buffer,len);
		break;
	case	M_ME_NB_1:		//测量值，规一化值
		Pro_yc(str,buffer,len);
		break;
	case	M_MD_NA_1:
		Pro_yc2(str,buffer,len);
		break;
	case	M_IT_NA_1:		//累积量
		break;
	case	M_RE_NA_1:		//上行业务数据
		{
			if(m_Dlg->getProVersion() <30)
	        {
		       UP_RxProcV1(str,buffer,len);
			}
			else if(m_Dlg->getProVersion() ==30)
			{
               UP_RxProcV2(str,buffer,len);
			}
			else if(m_Dlg->getProVersion() >=40)
			{
               UP_RxProcV4(str,buffer,len);
			}

		}
		break;
	case	C_SD_NA_1:		//下行行业务数据
		if(m_Dlg->getProVersion() <30)
	    {
		     Down_RxProcV1(str,buffer,len );
		}
		else if(m_Dlg->getProVersion() ==30)
		{
             Down_RxProcV2(str,buffer,len);
		}
		else if(m_Dlg->getProVersion() >=40)
		{
             Down_RxProcV4(str,buffer,len);
		}
		break;
	case	M_JC_NA_1:		//交流充电桩实时监测数据项M_
		All_dataProc(str,buffer,len);
		break;
	default:
		break;
	}

	return	true;
}
void	CProto_IEC104::Pro_one_yx(CString strMsg,unsigned char *buffer,int len)
{
   CString str;
   CString strErr;

   int ss=buffer[5];
	int num = ss&0x7f;
	int sq = (buffer[5]>>7)&0x01;

	CString str1;
	str1.Format("\r\n%02x(ASDU 类型标识:单点遥信:%d)\r\n\r\n%02x(可变结构限定词:信息体个数%d,顺序%d)\r\n",
		buffer[4],buffer[4],
		buffer[5],num,sq);
	str=str+str1;

	str1.Format("%02x %02x(传送原因:",buffer[6],buffer[7]);
	str=str+str1;
	str=cosMsg(str,buffer[6]);
	str1.Format("%02x %02x(公共地址:%d)\r\n",buffer[11-3],buffer[12-3],buffer[11-3]+buffer[12-3]*256);

	str=str+str1;

	

	int  i=13-3;

	strMsg = strMsg+str;
	strMsg=timeMsg(strMsg,buffer+i,"时间");

	buffer=buffer+i;
	int j=10;

	if(sq==0)
		{
			for(int i=0;i<num;i++)
			{
                 if(IsValide(strErr,buffer+j,len-j,4))
	            {
					str.Format("%02x %02x %02x(信息对象地址:%d,值%d)\r\n",buffer[j],buffer[j+1],buffer[j+2],buffer[j]+buffer[j+1]*0x100+buffer[j+2]*0x10000,buffer[j+3]);
	             	strMsg = strMsg+str;
	            }
				 else
	            {
		            str.Format("(信息对象地址长度错误)\r\n");
		           strMsg = strMsg+str;
		            m_Dlg->ProDecodeShow(strMsg);
		                return;
	               
				 }
				 j=j+4;
				
			 }
		}
		else
		{
			int addr = buffer[j]+buffer[j+1]*0x100+buffer[j+2]*0x10000;
			 if(IsValide(strErr,buffer+j,len-j,3))
	          {
					str.Format("%02x %02x %02x(信息对象地址:%d)\r\n",buffer[j],buffer[j+1],buffer[j+2],addr);
	             	strMsg = strMsg+str;
	          }
			  else
	           {
		            str.Format("(信息对象地址长度错误)\r\n");
		           strMsg = strMsg+str;
		            m_Dlg->ProDecodeShow(strMsg);
		                return;
	               
				 }
				 j=j+3;

			for(int i=0;i<num;i++)
			{
				 if(IsValide(strErr,buffer+j,len-j,1))
	             {
		            str.Format("%02x(地址%d，值:%d)\r\n",buffer[j],addr+i,buffer[j]);
	             	strMsg = strMsg+str;
	             }
				 else
	             {
		            str.Format("(值长度错误)\r\n");
		           strMsg = strMsg+str;
		            m_Dlg->ProDecodeShow(strMsg);
		                return;
	               
				 }
				 j=j+1;
			}
		}
						
						
		
     strMsg = cmdTimeMsg(strMsg,strErr,buffer+j,len-j);
     strMsg = crcMsg(strMsg,strErr,buffer+j+3,len-j-3);
		m_Dlg->ProDecodeShow(strMsg);

	
}

void	CProto_IEC104::Pro_two_yx(CString strMsg,unsigned char *buffer,int len)
{
	  CString str;
   CString strErr;

   int ss=buffer[5];
	int num = ss&0x7f;
	int sq = (buffer[5]>>7)&0x01;

	CString str1;
	str1.Format("\r\n%02x(ASDU 类型标识:双点遥信:%d)\r\n\r\n%02x(可变结构限定词:信息体个数%d,顺序%d)\r\n",
		buffer[4],buffer[4],
		buffer[5],num,sq);
	str=str+str1;

	str1.Format("%02x %02x(传送原因:",buffer[6],buffer[7]);
	str=str+str1;
	str=cosMsg(str,buffer[6]);
	str1.Format("%02x %02x(公共地址:%d)\r\n",buffer[11-3],buffer[12-3],buffer[11-3]+buffer[12-3]*256);

	str=str+str1;

	int  i=13-3;
	strMsg = strMsg+str;
	strMsg=timeMsg(strMsg,buffer+i,"时间");

	buffer=buffer+i;
	int j=10;

	if(sq==0)
		{
			for(int i=0;i<num;i++)
			{
                 if(IsValide(strErr,buffer+j,len-j,4))
	            {
					str.Format("%02x %02x %02x(信息对象地址:%d,值%d)\r\n",buffer[j],buffer[j+1],buffer[j+2],buffer[j]+buffer[j+1]*0x100+buffer[j+2]*0x10000,buffer[j+3]);
	             	strMsg = strMsg+str;
	            }
				 else
	            {
		            str.Format("(信息对象地址长度错误)\r\n");
		           strMsg = strMsg+str;
		            m_Dlg->ProDecodeShow(strMsg);
		                return;
	               
				 }
				 j=j+4;
				
			 }
		}
		else
		{
			int addr = buffer[j]+buffer[j+1]*0x100+buffer[j+2]*0x10000;
			 if(IsValide(strErr,buffer+j,len-j,3))
	          {
					str.Format("%02x %02x %02x(信息对象地址:%d)\r\n",buffer[j],buffer[j+1],buffer[j+2],addr);
	             	strMsg = strMsg+str;
	          }
			  else
	           {
		            str.Format("(信息对象地址长度错误)\r\n");
		           strMsg = strMsg+str;
		            m_Dlg->ProDecodeShow(strMsg);
		                return;
	               
				 }
				 j=j+3;

			for(int i=0;i<num;i++)
			{
				 if(IsValide(strErr,buffer+j,len-j,1))
	             {
		            str.Format("%02x(地址%d，值:%d)\r\n",buffer[j],addr+i,buffer[j]);
	             	strMsg = strMsg+str;
	             }
				 else
	             {
		            str.Format("(值长度错误)\r\n");
		           strMsg = strMsg+str;
		            m_Dlg->ProDecodeShow(strMsg);
		                return;
	               
				 }
				 j=j+1;
			}
		}
						
	strMsg = cmdTimeMsg(strMsg,strErr,buffer+j,len-j);
     strMsg = crcMsg(strMsg,strErr,buffer+j+3,len-j-3);					
				
		m_Dlg->ProDecodeShow(strMsg);
  
	 
}

void	CProto_IEC104::Pro_yc(CString strMsg,unsigned char *buffer,int len)
{
	 CString str;
   CString strErr;

   int ss=buffer[5];
	int num = ss&0x7f;
	int sq = (buffer[5]>>7)&0x01;

	CString str1;
	str1.Format("\r\n%02x(ASDU 类型标识:遥测:%d)\r\n\r\n%02x(可变结构限定词:信息体个数%d,顺序%d)\r\n",
		buffer[4],buffer[4],
		buffer[5],num,sq);
	str=str+str1;

	str1.Format("%02x %02x(传送原因:",buffer[6],buffer[7]);
	str=str+str1;
	str=cosMsg(str,buffer[6]);
	str1.Format("%02x %02x(公共地址:%d)\r\n",buffer[11-3],buffer[12-3],buffer[11-3]+buffer[12-3]*256);

	str=str+str1;

	int  i=13-3;
	strMsg = strMsg+str;
	strMsg=timeMsg(strMsg,buffer+i,"时间");

		buffer=buffer+i;
	int j=10;

	if(sq==0)
		{
			for(int i=0;i<num;i++)
			{
                 if(IsValide(strErr,buffer+j,len-j,6))
	            {
					str.Format("%02x %02x %02x(信息对象地址:%d,值%d)\r\n",buffer[j],buffer[j+1],buffer[j+2],buffer[j]+buffer[j+1]*0x100+buffer[j+2]*0x10000,buffer[j+3]+buffer[j+4]*0x100);
	             	strMsg = strMsg+str;
	            }
				 else
	            {
		            str.Format("(信息对象地址长度错误)\r\n");
		           strMsg = strMsg+str;
		            m_Dlg->ProDecodeShow(strMsg);
		                return;
	               
				 }
				 j=j+6;
				
			 }
		}
		else
		{
			int addr = buffer[j]+buffer[j+1]*0x100+buffer[j+2]*0x10000;
			 if(IsValide(strErr,buffer+j,len-j,3))
	          {
					str.Format("%02x %02x %02x(信息对象地址:%d)\r\n",buffer[j],buffer[j+1],buffer[j+2],addr);
	             	strMsg = strMsg+str;
	          }
			  else
	           {
		            str.Format("(信息对象地址长度错误)\r\n");
		           strMsg = strMsg+str;
		            m_Dlg->ProDecodeShow(strMsg);
		                return;
	               
				 }
				 j=j+3;

			for(int i=0;i<num;i++)
			{
				 if(IsValide(strErr,buffer+j,len-j,3))
	             {
		            str.Format("%02x(地址%d，值:%d)\r\n",buffer[j],addr+i,buffer[j]+buffer[j+1]*0x100);
	             	strMsg = strMsg+str;
	             }
				 else
	             {
		            str.Format("(值长度错误)\r\n");
		           strMsg = strMsg+str;
		            m_Dlg->ProDecodeShow(strMsg);
		                return;
	               
				 }
				 j=j+3;
			}
		}
						
	strMsg = cmdTimeMsg(strMsg,strErr,buffer+j,len-j);
     strMsg = crcMsg(strMsg,strErr,buffer+j+3,len-j-3);					
				
		m_Dlg->ProDecodeShow(strMsg);
	 
}


void	CProto_IEC104::Pro_yc2(CString strMsg,unsigned char *buffer,int len)
{
	 CString str;
   CString strErr;

   int ss=buffer[5];
	int num = ss&0x7f;
	int sq = (buffer[5]>>7)&0x01;

	CString str1;
	str1.Format("\r\n%02x(ASDU 类型标识:变长遥测:%d)\r\n\r\n%02x(可变结构限定词:信息体个数%d,顺序%d)\r\n",
		buffer[4],buffer[4],
		buffer[5],num,sq);
	str=str+str1;

	str1.Format("%02x %02x(传送原因:",buffer[6],buffer[7]);
	str=str+str1;
	str=cosMsg(str,buffer[6]);
	str1.Format("%02x %02x(公共地址:%d)\r\n",buffer[11-3],buffer[12-3],buffer[11-3]+buffer[12-3]*256);

	str=str+str1;

	int  i=13-3;
	strMsg = strMsg+str;
	strMsg=timeMsg(strMsg,buffer+i,"时间");

		buffer=buffer+i;
	int j=10;

	if(sq==0)
		{
			for(int i=0;i<num;i++)
			{
                 if(IsValide(strErr,buffer+j,len-j,3))
	            {
					str.Format("%02x %02x %02x(信息对象地址:%d,值:",buffer[j],buffer[j+1],buffer[j+2]);
	             	strMsg = strMsg+str;
	            }
				 else
	            {
		            str.Format("(信息对象地址长度错误)\r\n");
		           strMsg = strMsg+str;
		            m_Dlg->ProDecodeShow(strMsg);
		                return;
	               
				 }
				 j=j+3;
				 int templen=buffer[j];
				 j++;
				if(IsValide(strErr,buffer+j,len-j,templen))
	            {
					if(i==0||i==1||i==2|i==3)
					{
						int value = buffer[j]+buffer[j+1]*0x100+buffer[j+2]*0x10000+buffer[j+3]*0x1000000;
						str.Format("%d,",value);
						j=j+templen;
					}
					else
					{
						for(int n=0;n<templen;n++)
					   {
						   str.Format("%d ",buffer[j]);
	             	       strMsg = strMsg+str;
						   j++;
						}
					 
					}
	            }
				else
	            {
		            str.Format("(值长度错误)\r\n");
		           strMsg = strMsg+str;
		            m_Dlg->ProDecodeShow(strMsg);
		                return;
	               
				 }

				
			 }
		}
		else
		{
			int addr = buffer[j]+buffer[j+1]*0x100+buffer[j+2]*0x10000;
			 if(IsValide(strErr,buffer+j,len-j,3))
	          {
					str.Format("%02x %02x %02x(信息对象地址:%d)\r\n",buffer[j],buffer[j+1],buffer[j+2],addr);
	             	strMsg = strMsg+str;
	          }
			  else
	           {
		            str.Format("(信息对象地址长度错误)\r\n");
		           strMsg = strMsg+str;
		            m_Dlg->ProDecodeShow(strMsg);
		                return;
	               
				 }
				 j=j+3;

			for(int i=0;i<num;i++)
			{
				 int templen=buffer[j];
				 j++;
				if(IsValide(strErr,buffer+j,len-j,templen))
	            {
					if(i==0||i==1||i==2|i==3)
					{
						int value = buffer[j]+buffer[j+1]*0x100+buffer[j+2]*0x10000+buffer[j+3]*0x1000000;
						str.Format("%d,",value);
						j=j+templen;
					}
					else
					{
						for(int n=0;n<templen;n++)
					   {
						   str.Format("%d ",buffer[j]);
	             	       strMsg = strMsg+str;
						   j++;
						}
					 
					}
	            }
				else
	            {
		            str.Format("(值长度错误)\r\n");
		           strMsg = strMsg+str;
		            m_Dlg->ProDecodeShow(strMsg);
		                return;
	               
				 }
			}
		}
						
		strMsg = cmdTimeMsg(strMsg,strErr,buffer+j,len-j);
     strMsg = crcMsg(strMsg,strErr,buffer+j+3,len-j-3);				
				
		m_Dlg->ProDecodeShow(strMsg);
     
	 
}

//解释上行数据
void	CProto_IEC104::UP_RxProcV2(CString strMsg,unsigned char *buffer,int len)
{
	   CString str1;
		
		strMsg=headMsg1(strMsg,buffer+4,"上行业务数据");
		int i=16-3;
		switch(buffer[i])
	   {
	   case 1:
           str1.Format("\r\n%02x(计费模型请求上行数据%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_UP_1(strMsg,buffer+i+1,len-i-1);

		   break;
	   case 2:
		   str1.Format("\r\n%02x(下发计费模型结果数据%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_UP_2_v2(strMsg,buffer+i+1,len-i-1);
		   break;
	   case 3:
		    str1.Format("\r\n%02x(私有充电桩下发充电桩运营时间%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_UP_3(strMsg,buffer+i+1,len-i-1);
		   break;
	    case 11:
		   str1.Format("\r\n%02x(无卡用户名密码鉴权上行数据%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_UP_11_v2(strMsg,buffer+i+1,len-i-1);
		   break;
	   case 12:
		   str1.Format("\r\n%02x(无卡验证码鉴权上行数据%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_UP_12_v2(strMsg,buffer+i+1,len-i-1);
		   break;
	   case 13:
		   str1.Format("\r\n%02x(不圈存卡鉴权上行数据%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_UP_13_v2(strMsg,buffer+i+1,len-i-1);
		   break;
	   case 31:
		   str1.Format("\r\n%02x(预约锁定上行数据%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_UP_31_v2(strMsg,buffer+i+1,len-i-1);
		   break;
	   case 32:
		    str1.Format("\r\n%02x(电桩应答预约报文的确认%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_UP_32_v2(strMsg,buffer+i+1,len-i-1);
		   break;
	   case 33:
		    str1.Format("\r\n%02x(取消预约上行应答数据%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_UP_33_v2(strMsg,buffer+i+1,len-i-1);
		   break;
	   case 41:
		    str1.Format("\r\n%02x(启动充电应答上行据%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_UP_41_v2(strMsg,buffer+i+1,len-i-1);
		   break;
	   case 42:
		   str1.Format("\r\n%02x(充电事件上行数据%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_UP_42_v2(strMsg,buffer+i+1,len-i-1);
		   break;
	    case 43:
			str1.Format("\r\n%02x(用户停止充电应答上行数据%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_UP_43_v2(strMsg,buffer+i+1,len-i-1);
		   break;
	    case 45:
			str1.Format("\r\n%02x(结束充电事件上行数据%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_UP_45_v2(strMsg,buffer+i+1,len-i-1);
		   break;
	    case 44:
			str1.Format("\r\n%02x(不圈存卡充电冻结金额上行数据%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_UP_44_v2(strMsg,buffer+i+1,len-i-1);
		   break;
	    case 46:
			str1.Format("\r\n%02x(充电消费记录上行数据%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_UP_46_v2(strMsg,buffer+i+1,len-i-1);
		   break;
	    case 47:
			str1.Format("\r\n%02x(消费余额告警信息%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_UP_47_v2(strMsg,buffer+i+1,len-i-1);
		   break;
	    case 101:
		   str1.Format("\r\n%02x(电桩设备支持报告上行数据%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_UP_101(strMsg,buffer+i+1,len-i-1);
		   break;
	    case 104:
			 str1.Format("\r\n%02x(打开枪锁装置应答上行数据%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_UP_104(strMsg,buffer+i+1,len-i-1);
		   break;
	    case 105:
			str1.Format("\r\n%02x(锁枪失败告警上行数据%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_UP_105(strMsg,buffer+i+1,len-i-1);
		   break;
	    case 141:
		str1.Format("\r\n%02x(产品信息查询应答%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_UP_141(strMsg,buffer+i+1,len-i-1);
		   break;
	    case 143:
			str1.Format("\r\n%02x(远端设备二进制程序文件下载概要信息请求%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_UP_143(strMsg,buffer+i+1,len-i-1);
		   break;
	    case 144:
			str1.Format("\r\n%02x(远端设备二进制程序文件分段下载请求%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_UP_144(strMsg,buffer+i+1,len-i-1);
		   break;
	   
	   case 145:
		   str1.Format("\r\n%02x(远端设备二进制程序文件更新确认应答%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_UP_145(strMsg,buffer+i+1,len-i-1);
		   break;
	   case 161:
		   str1.Format("\r\n%02x(充电信息统计上新数据%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_UP_161(strMsg,buffer+i+1,len-i-1);
		   break;
	   case 162:
		   str1.Format("\r\n%02x(电桩业务数据查询应答报文%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_UP_162(strMsg,buffer+i+1,len-i-1);
		   break;
	   case 147:
		   str1.Format("\r\n%02x(远端设备通讯信号强度应答%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_UP_147(strMsg,buffer+i+1,len-i-1);
		   break;
	   default:
		    str1.Format("记录类型错误");
		   strMsg =strMsg+str1;
		   m_Dlg->ProDecodeShow(strMsg);
		   break;
	   }

       

}
//解释上行数据
void	CProto_IEC104::UP_RxProcV1(CString strMsg,unsigned char *buffer,int len)
{
	CString str1;
	strMsg=headMsg1(strMsg,buffer+4,"上行业务数据");
		int i=16-3;
		switch(buffer[i])
	   {
	   case 1:
           str1.Format("\r\n%02x(计费模型请求上行数据%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_UP_1(strMsg,buffer+i+1,len-i-1);
		   break;
	   case 2:
		   str1.Format("\r\n%02x(下发计费模型结果数据%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_UP_2(strMsg,buffer+i+1,len-i-1);
		   break;
	   case 3:
		    str1.Format("\r\n%02x(私有充电桩下发充电桩运营时间%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_UP_3(strMsg,buffer+i+1,len-i-1);
		   break;
	   case 4:
		   str1.Format("\r\n%02x(有卡支付鉴权%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_UP_4(strMsg,buffer+i+1,len-i-1);
		   break;
	   case 5:
		   str1.Format("\r\n%02x(无卡支付用户密码鉴权%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_UP_5(strMsg,buffer+i+1,len-i-1);
		   break;
	   case 6:
		    str1.Format("\r\n%02x(无卡支付验证码鉴权%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_UP_6(strMsg,buffer+i+1,len-i-1);
		   break;
	   case 7:
		   str1.Format("\r\n%02x(预约锁定上行数据%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_UP_7(strMsg,buffer+i+1,len-i-1);
		   break;
	   case 8:
		    str1.Format("\r\n%02x(取消预约上行数据%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_UP_8(strMsg,buffer+i+1,len-i-1);
		   break;
	   case 9:
		    str1.Format("\r\n%02x(开始充电事件上行数据%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_UP_9(strMsg,buffer+i+1,len-i-1);
		   break;
	   case 10:
		   str1.Format("\r\n%02x(结束充电事件上行数据%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_UP_10(strMsg,buffer+i+1,len-i-1);
		   break;
	    case 11:
			str1.Format("\r\n%02x(无卡充电消费记录上行数据%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_UP_11(strMsg,buffer+i+1,len-i-1);
		   break;
	    case 12:
			str1.Format("\r\n%02x(用户开始充电上行数据%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_UP_12(strMsg,buffer+i+1,len-i-1);
		   break;
	    case 13:
			str1.Format("\r\n%02x(有卡在线停止充电时上传充电记录数%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_UP_13(strMsg,buffer+i+1,len-i-1);
		   break;
	    case 14:
			str1.Format("\r\n%02x(有卡离线停止充电时上传充电记录数%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_UP_14(strMsg,buffer+i+1,len-i-1);
		   break;
	    case 15:
			str1.Format("\r\n%02x(异桩解扣上行数据%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_UP_15(strMsg,buffer+i+1,len-i-1);
		   break;
	    case 16:
		   str1.Format("\r\n%02x(异桩解扣结果上行数据%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_UP_16(strMsg,buffer+i+1,len-i-1);
		   break;
	    case 17:
			 str1.Format("\r\n%02x(充电桩鉴权%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_UP_17(strMsg,buffer+i+1,len-i-1);
		   break;
	    case 18:
			str1.Format("\r\n%02x(密钥更新过程中确认指令%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_UP_18(strMsg,buffer+i+1,len-i-1);
		   break;
	    case 19:
		str1.Format("\r\n%02x(密钥更新过程中确认指令%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_UP_19(strMsg,buffer+i+1,len-i-1);
		   break;
	    case 20:
			str1.Format("\r\n%02x(黑名单下发数据请求数据%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_UP_20(strMsg,buffer+i+1,len-i-1);
		   break;
	    case 21:
			str1.Format("\r\n%02x(黑名单下发数据结果上行数据%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_UP_21(strMsg,buffer+i+1,len-i-1);
		   break;
	    case 22:
			str1.Format("\r\n%02x(请求二维码文件概要信息%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_UP_22(strMsg,buffer+i+1,len-i-1);
		   break;
	    case 23:
			str1.Format("\r\n%02x(请求二维码文件数据%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_UP_23(strMsg,buffer+i+1,len-i-1);
		   break;
		case 24:
			str1.Format("\r\n%02x(消费余额告警信息%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_UP_24(strMsg,buffer+i+1,len-i-1);
		   break;
	   case 25:
		   str1.Format("\r\n%02x(充电桩程序文件概要信息请求%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_UP_25(strMsg,buffer+i+1,len-i-1);
		   break;
	   case 26:
		   str1.Format("\r\n%02x(充电桩程序文件分段信息请求%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_UP_26(strMsg,buffer+i+1,len-i-1);
		   break;
	   case 27:
		   str1.Format("\r\n%02x(用户停止充电上行数据%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_UP_27(strMsg,buffer+i+1,len-i-1);
		   break;
	   case 28:
		   str1.Format("\r\n%02x(充电桩充电统计信息上行数据%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_UP_28(strMsg,buffer+i+1,len-i-1);
		   break;
	   case 29:
		   str1.Format("\r\n%02x(直流桩自检完成%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_UP_29(strMsg,buffer+i+1,len-i-1);
		   break;
	   case 30:
		   str1.Format("\r\n%02x(锁枪失败报警上行数据%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_UP_30(strMsg,buffer+i+1,len-i-1);
		   break;
	   case 31:
		   str1.Format("\r\n%02x(电桩设备支持报告上行数据%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_UP_31(strMsg,buffer+i+1,len-i-1);
		   break;
	   case 32:
		   str1.Format("\r\n%02x(联网充值请求上行数据%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_UP_32(strMsg,buffer+i+1,len-i-1);
		   break;
	   case 33:
		   str1.Format("\r\n%02x(打开枪锁装置应答上行数据%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_UP_33(strMsg,buffer+i+1,len-i-1);
		   break;
	   default:
		    str1.Format("记录类型错误");
		   strMsg =strMsg+str1;
		   m_Dlg->ProDecodeShow(strMsg);
		   break;
	   }

       

}

CString   CProto_IEC104::getEpCode(CString strMsg,unsigned char *buffer)
{
	CString str;
	str.Format("%02x%02x%02x%02x%02x%02x%02x%02x(终端机器编码)\r\n",buffer[0],buffer[1],buffer[2],buffer[3],buffer[4],buffer[5],buffer[6],buffer[7]);
   strMsg = strMsg+str;
   return strMsg;
}

CString   CProto_IEC104::getPhone(CString strMsg,unsigned char *buffer)
{
	CString str;
	str.Format("%02x%02x%02x%02x%02x%02x(账号(手机号))\r\n",buffer[0],buffer[1],buffer[2],buffer[3],buffer[4],buffer[5]);
	strMsg = strMsg+str;
   return strMsg;
}
//请求费率数据上行数据
void	CProto_IEC104::Pro_UP_1(CString strMsg,unsigned char *buffer,int len)
{
	CString str,strErr;
	if(buffer==0 || len==0)
	{
		str="无数据内容";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	if(IsValide(strErr,buffer,len,8))
	{
		strMsg = getEpCode(strMsg,buffer);
	}
	else
	{
		str.Format("(终端机器编码长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return ;
	}
	int i=8;

	strMsg = cmdTimeMsg(strMsg,strErr,buffer+i,len-i);
     strMsg = crcMsg(strMsg,strErr,buffer+i+3,len-i-3);
	m_Dlg->ProDecodeShow(strMsg);

	 
}
//
void	CProto_IEC104::Pro_UP_2(CString strMsg,unsigned char *buffer,int len)
{
	CString str,strErr;
	if(buffer==0 || len==0)
	{
		str="无数据内容";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	int i=0;

	if(IsValide(strErr,buffer,len,8))
	{
		strMsg = getEpCode(strMsg,buffer);
	}
	else
	{
		str.Format("(终端机器编码长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=8;
    if(IsValide(strErr,buffer+i,len-i,1))
	{
		str.Format("%02x(充电接口标识)\r\n",buffer[8]);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("充电接口标识错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	
	i++;
    if(IsValide(strErr,buffer+i,len-i,8))
	{

		str.Format("%02x %02x %02x %02x %02x %02x %02x %02x(计费模型ID：%d)\r\n",buffer[9],buffer[10],buffer[11],buffer[12],buffer[13],buffer[14],buffer[15],buffer[16],
			buffer[16]*0x100000000000000+buffer[15]*0x1000000000000+buffer[14]*0x10000000000+buffer[13]*0x100000000+buffer[12]*0x1000000+buffer[11]*0x10000+buffer[10]*0x100+buffer[9]);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(计费模型ID长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+8;
	if(IsValide(strErr,buffer+i,len-i,1))
	{
		str.Format("%02x(成功标识：",buffer[17]);
		strMsg = strMsg+str;
		if(buffer[17]==0)
		{
			str.Format("成功)\r\n");
		    strMsg = strMsg+str;
		}
		else if(buffer[17]==1)
		{
			str.Format("失败)\r\n");
		    strMsg = strMsg+str;
		}
		else
		{
			str.Format("未知)\r\n");
		    strMsg = strMsg+str;
		}
	}
	else
	{
		str.Format("成功标识错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
   i=i+1;
	
   strMsg = cmdTimeMsg(strMsg,strErr,buffer+i,len-i);
	i=i+3;
     strMsg = crcMsg(strMsg,strErr,buffer+i,len-i);
		
	m_Dlg->ProDecodeShow(strMsg);
   
	 
}

//下发计费模型结果数据
void	CProto_IEC104::Pro_UP_2_v2(CString strMsg,unsigned char *buffer,int len)
{
	CString str,strErr;
	if(buffer==0 || len==0)
	{
		str="无数据内容";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	int i=0;

	if(IsValide(strErr,buffer,len,8))
	{
		strMsg = getEpCode(strMsg,buffer);
	}
	else
	{
		str.Format("(终端机器编码长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=8;
    if(IsValide(strErr,buffer+i,len-i,1))
	{
		str.Format("%02x(充电接口标识)\r\n",buffer[8]);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("充电接口标识错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	
	i++;
    if(IsValide(strErr,buffer+i,len-i,8))
	{

		str.Format("%02x %02x %02x %02x %02x %02x %02x %02x(计费模型ID：%d)\r\n",buffer[9],buffer[10],buffer[11],buffer[12],buffer[13],buffer[14],buffer[15],buffer[16],
			buffer[16]*0x100000000000000+buffer[15]*0x1000000000000+buffer[14]*0x10000000000+buffer[13]*0x100000000+buffer[12]*0x1000000+buffer[11]*0x10000+buffer[10]*0x100+buffer[9]);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(计费模型ID长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+8;
	if(IsValide(strErr,buffer+i,len-i,1))
	{
		str.Format("%02x(成功标识：",buffer[17]);
		strMsg = strMsg+str;
		if(buffer[17]==0)
		{
			str.Format("成功)\r\n");
		    strMsg = strMsg+str;
		}
		else if(buffer[17]==1)
		{
			str.Format("失败)\r\n");
		    strMsg = strMsg+str;
		}
		else
		{
			str.Format("未知)\r\n");
		    strMsg = strMsg+str;
		}
	}
	else
	{
		str.Format("成功标识错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+1;
	if(IsValide(strErr,buffer+i,len-i,2))
	{
		str.Format("%02x %02x(错误代码：",buffer[i],buffer[i+1]);

		strMsg =strMsg+str;

		strMsg = errorMsg(strMsg,buffer[i]+buffer[i+1]*0x100);
		
	}
	else
	{
		str.Format("(错误代码长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+2;
	
	strMsg = cmdTimeMsg(strMsg,strErr,buffer+i,len-i);
	i=i+3;
    strMsg = crcMsg(strMsg,strErr,buffer+i,len-i);
		
	m_Dlg->ProDecodeShow(strMsg);

	 
}

//私有充电桩下发充电桩运营时间上行数据

void	CProto_IEC104::Pro_UP_3(CString strMsg,unsigned char *buffer,int len)
{
	CString str,strErr;
	if(buffer==0 || len==0)
	{
		str="无数据内容";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	if(IsValide(strErr,buffer,len,8))
	{
		strMsg = getEpCode(strMsg,buffer);
	}
	else
	{
		str.Format("(终端机器编码长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	int i=8;
	if(IsValide(strErr,buffer+i,len-i,1))
	{
		str.Format("%02x(离线标志：",buffer[8]);
		strMsg = strMsg+str;

		switch(buffer[8])
		{
		case 0:
			str.Format("离线)\r\n");
		    strMsg = strMsg+str;
			break;
		case 1:
			str.Format("运营)\r\n");
		    strMsg = strMsg+str;
			break;
		default:
            str.Format("未知)\r\n");
		    strMsg = strMsg+str;
			break;
		}
	}
	else
	{
		str.Format("离线标志错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i++;
	if(IsValide(strErr,buffer+i,len-i,1))
	{
		str.Format("%02x(标识：",buffer[9]);
		strMsg = strMsg+str;

		switch(buffer[9])
		{
		case 0:
			str.Format("成功)\r\n");
		    strMsg = strMsg+str;
			break;
		default:
            str.Format("失败)\r\n");
		    strMsg = strMsg+str;
			break;
		}
	}
	else
	{
		str.Format("标识错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
    i=i+1;
	
	strMsg = cmdTimeMsg(strMsg,strErr,buffer+i,len-i);
	i=i+3;
    strMsg = crcMsg(strMsg,strErr,buffer+i,len-i);
	m_Dlg->ProDecodeShow(strMsg);

	 
}
void	CProto_IEC104::Pro_UP_4(CString strMsg,unsigned char *buffer,int len)
{
	CString str,strErr;
	if(buffer==0 || len==0)
	{
		str="无数据内容";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	if(IsValide(strErr,buffer,len,8))
	{
		strMsg = getEpCode(strMsg,buffer);
	}
	else
	{
		str.Format("(终端机器编码长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	int i=8;
    if(IsValide(strErr,buffer+i,len-i,1))
	{
		str.Format("%02x(充电桩接口标识)\r\n",buffer[8]);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("充电桩接口标识错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	 i++;
    if(IsValide(strErr,buffer+i,len-i,6))
	{
		strMsg = getPhone(strMsg,buffer+i);
	}
	else
	{
		str.Format("(账号(手机号)长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+6;
    if(IsValide(strErr,buffer+i,len-i,32))
	{
		CString s,s1;
		for(int j=15;j<47;j++)
		{
			str.Format("%02x ",buffer[j]);
			strMsg = strMsg+str;

			s1.Format("%c",buffer[j]);
			s = s+s1;
		}
		str.Format("(密码MD5-32：%s)",s);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("密码MD5-32长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+32;
	if(IsValide(strErr,buffer+i,len-i,8))
	{
		str.Format("%02x%02x%02x%02x%02x%02x%02x%02x(物理卡号)\r\n",buffer[47],buffer[48],buffer[49],buffer[50],buffer[51],buffer[52],buffer[53],buffer[54]);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("物理卡号长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+8;
	if(IsValide(strErr,buffer+i,len-i,8))
	{
		str.Format("%02x%02x%02x%02x%02x%02x%02x%02x(支付卡号)\r\n",buffer[55],buffer[56],buffer[57],buffer[58],buffer[59],buffer[60],buffer[61],buffer[62]);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("支付卡号长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+8;
	if(IsValide(strErr,buffer+i,len-i,8))
	{
		str.Format("%02x%02x%02x%02x%02x%02x%02x%02x(认证支付卡数据)\r\n",buffer[63],buffer[64],buffer[65],buffer[66],buffer[67],buffer[68],buffer[69],buffer[70]);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("认证支付卡数据长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	 i=i+8;
	
	strMsg = cmdTimeMsg(strMsg,strErr,buffer+i,len-i);
	i=i+3;
    strMsg = crcMsg(strMsg,strErr,buffer+i,len-i);
    m_Dlg->ProDecodeShow(strMsg);

	 
}

void	CProto_IEC104::Pro_UP_11_v2(CString strMsg,unsigned char *buffer,int len)
{
	CString str,strErr;
	if(buffer==0 || len==0)
	{
		str="无数据内容";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	if(IsValide(strErr,buffer,len,8))
	{
		strMsg = getEpCode(strMsg,buffer);
	}
	else
	{
		str.Format("(终端机器编码长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	int i=8;
    if(IsValide(strErr,buffer+i,len-i,1))
	{
		str.Format("%02x(充电桩接口标识)\r\n",buffer[8]);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("充电桩接口标识错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	 i++;
    if(IsValide(strErr,buffer+i,len-i,6))
	{
		strMsg = getPhone(strMsg,buffer+i);
	}
	else
	{
		str.Format("(账号(手机号)长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+6;
    if(IsValide(strErr,buffer+i,len-i,32))
	{
		CString s,s1;
		for(int j=15;j<47;j++)
		{
			str.Format("%02x ",buffer[j]);
			strMsg = strMsg+str;

			s1.Format("%c",buffer[j]);
			s = s+s1;
		}
		str.Format("(密码MD5-32：%s)",s);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("密码MD5-32长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	 i=i+32;
	
	strMsg = cmdTimeMsg(strMsg,strErr,buffer+i,len-i);
	i=i+3;
    strMsg = crcMsg(strMsg,strErr,buffer+i,len-i);
	
	m_Dlg->ProDecodeShow(strMsg);

	 
}

void	CProto_IEC104::Pro_UP_5(CString strMsg,unsigned char *buffer,int len)
{
	CString str,strErr;
	if(buffer==0 || len==0)
	{
		str="无数据内容";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	if(IsValide(strErr,buffer,len,8))
	{
		strMsg = getEpCode(strMsg,buffer);
	}
	else
	{
		str.Format("(终端机器编码长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	int i=8;
	if(IsValide(strErr,buffer+i,len-i,1))
	{
		str.Format("%02x(充电桩接口标识)\r\n",buffer[8]);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("充电桩接口标识错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+1;
	if(IsValide(strErr,buffer+i,len-i,6))
	{
		strMsg = getPhone(strMsg,buffer+i);
	}
	else
	{
		str.Format("(账号(手机号)长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+6;
	if(IsValide(strErr,buffer+i,len-i,32))
	{
		CString s,s1;
		for(int j=15;j<47;j++)
		{
			str.Format("%02x ",buffer[j]);
			strMsg = strMsg+str;

			s1.Format("%c",buffer[j]);
			s = s+s1;
		}
		str.Format("(密码MD5-32：%s)",s);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(密码MD5-32长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	 i=i+32;
	
	strMsg = cmdTimeMsg(strMsg,strErr,buffer+i,len-i);
	i=i+3;
    strMsg = crcMsg(strMsg,strErr,buffer+i,len-i);
	m_Dlg->ProDecodeShow(strMsg);

	 
}

void	CProto_IEC104::Pro_UP_6(CString strMsg,unsigned char *buffer,int len)
{
	CString str,strErr;
	if(buffer==0 || len==0)
	{
		str="无数据内容";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	if(IsValide(strErr,buffer,len,8))
	{
		strMsg = getEpCode(strMsg,buffer);
	}
	else
	{
		str.Format("(终端机器编码长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	int i=8;
	if(IsValide(strErr,buffer+i,len-i,1))
	{
		str.Format("%02x(充电桩接口标识)\r\n",buffer[8]);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("充电桩接口标识错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	 i=i+1;
	if(IsValide(strErr,buffer+i,len-i,8))
	{
		str.Format("%02x%02x%02x%02x%02x%02x%02x%02x(验证码)\r\n",buffer[9],buffer[10],buffer[11],buffer[12],buffer[13],buffer[14],buffer[15],buffer[16]);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(验证码长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+8;
	
	strMsg = cmdTimeMsg(strMsg,strErr,buffer+i,len-i);
	i=i+3;
    strMsg = crcMsg(strMsg,strErr,buffer+i,len-i);
	m_Dlg->ProDecodeShow(strMsg);

	 
}

void	CProto_IEC104::Pro_UP_12_v2(CString strMsg,unsigned char *buffer,int len)
{
	CString str,strErr;
	if(buffer==0 || len==0)
	{
		str="无数据内容";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	if(IsValide(strErr,buffer,len,8))
	{
		strMsg = getEpCode(strMsg,buffer);
	}
	else
	{
		str.Format("(终端机器编码长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	int i=8;
	if(IsValide(strErr,buffer+i,len-i,1))
	{
		str.Format("%02x(充电桩接口标识)\r\n",buffer[8]);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("充电桩接口标识错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+1;
	if(IsValide(strErr,buffer+i,len-i,8))
	{
		str.Format("%02x%02x%02x%02x%02x%02x%02x%02x(验证码)\r\n",buffer[9],buffer[10],buffer[11],buffer[12],buffer[13],buffer[14],buffer[15],buffer[16]);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(验证码长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+8;
	
	strMsg = cmdTimeMsg(strMsg,strErr,buffer+i,len-i);
	i=i+3;
    strMsg = crcMsg(strMsg,strErr,buffer+i,len-i);
	m_Dlg->ProDecodeShow(strMsg);

	 
}
void	CProto_IEC104::Pro_UP_13_v2(CString strMsg,unsigned char *buffer,int len)
{
	CString str,strErr;
	if(buffer==0 || len==0)
	{
		str="无数据内容";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	if(IsValide(strErr,buffer,len,8))
	{
		strMsg = getEpCode(strMsg,buffer);
	}
	else
	{
		str.Format("(终端机器编码长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	int i=8;
	if(IsValide(strErr,buffer+i,len-i,1))
	{
		str.Format("%02x(充电桩接口标识)\r\n",buffer[8]);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("充电桩接口标识错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+1;
	
	if(IsValide(strErr,buffer+i,len-i,32))
	{
		CString s,s1;
		for(int j=15;j<47;j++)
		{
			str.Format("%02x ",buffer[j]);
			strMsg = strMsg+str;

			s1.Format("%c",buffer[j]);
			s = s+s1;
		}
		str.Format("(内卡号：%s)",s);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(内卡号长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+32;
	if(IsValide(strErr,buffer+i,len-i,32))
	{
		CString s,s1;
		for(int j=i;j<i+32;j++)
		{
			str.Format("%02x ",buffer[j]);
			strMsg = strMsg+str;

			s1.Format("%c",buffer[j]);
			s = s+s1;
		}
		str.Format("(卡密码：%s)",s);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(卡密码长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	 i=i+32;
	
	strMsg = cmdTimeMsg(strMsg,strErr,buffer+i,len-i);
	i=i+3;
    strMsg = crcMsg(strMsg,strErr,buffer+i,len-i);
	m_Dlg->ProDecodeShow(strMsg);

	 
}

void	CProto_IEC104::Pro_UP_7(CString strMsg,unsigned char *buffer,int len)
{
	CString str,strErr;
	if(buffer==0 || len==0)
	{
		str="无数据内容";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	if(IsValide(strErr,buffer,len,8))
	{
		strMsg = getEpCode(strMsg,buffer);
	}
	else
	{
		str.Format("(终端机器编码长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	int i=8;
	if(IsValide(strErr,buffer+i,len-i,1))
	{
		str.Format("%02x(枪口编号)\r\n",buffer[8]);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("枪口编号错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+1;
	if(IsValide(strErr,buffer+i,len-i,1))
	{
		str.Format("%02x(续约标识：",buffer[9]);
		strMsg = strMsg+str;

		switch(buffer[9])
		{
		case 1:
			str.Format("再续约)\r\n");
		    strMsg = strMsg+str;
			break;
		default:
            str.Format("其它)\r\n");
		    strMsg = strMsg+str;
			break;
		}
	}
	else
	{
		str.Format("续约标识错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+1;
	if(IsValide(strErr,buffer+i,len-i,6))
	{
		for(int j=10;j<16;j++)
		{
			str.Format("%02x",buffer[j]);
			strMsg = strMsg+str;

		}
		str.Format("(预约编号：");
		strMsg = strMsg+str;
		strMsg = strMsg+")\r\n";
	}
	else
	{
		str.Format("(预约编号长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+6;
    if(IsValide(strErr,buffer+i,len-i,1))
	{    
		str.Format("%02x(执行预约的结果：",buffer[i]);
		strMsg = strMsg+str;
	
		switch(buffer[i])
		{
		case 0:
			str.Format("成功)\r\n");
		    strMsg = strMsg+str;
			break;
		default:
            str.Format("其它)\r\n");
		    strMsg = strMsg+str;
			break;
		}
	}
    else
	{
		str.Format("执行预约的结果错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+1;
    if(IsValide(strErr,buffer+i,len-i,2))
	{    
		str.Format("%02x%02x(错误原因：",buffer[i],buffer[i+1]);

		strMsg =strMsg+str;

		strMsg = errorMsg(strMsg,buffer[i]+buffer[i+1]*0x100);
	}
    else
	{
		str.Format("错误原因错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+2;
	
	strMsg = cmdTimeMsg(strMsg,strErr,buffer+i,len-i);
	i=i+3;
    strMsg = crcMsg(strMsg,strErr,buffer+i,len-i);

	m_Dlg->ProDecodeShow(strMsg);

	 
}



void	CProto_IEC104::Pro_UP_31_v2(CString strMsg,unsigned char *buffer,int len)
{
	CString str,strErr;
	if(buffer==0 || len==0)
	{
		str="无数据内容";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	if(IsValide(strErr,buffer,len,8))
	{
		strMsg = getEpCode(strMsg,buffer);
	}
	else
	{
		str.Format("(终端机器编码长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	int i=8;
	if(IsValide(strErr,buffer+i,len-i,1))
	{
		str.Format("%02x(枪口编号)\r\n",buffer[8]);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("枪口编号错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+1;
	if(IsValide(strErr,buffer+i,len-i,1))
	{
		str.Format("%02x(续约标识：",buffer[9]);
		strMsg = strMsg+str;

		switch(buffer[9])
		{
		case 1:
			str.Format("再续约)\r\n");
		    strMsg = strMsg+str;
			break;
		default:
            str.Format("其它)\r\n");
		    strMsg = strMsg+str;
			break;
		}
	}
	else
	{
		str.Format("续约标识错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+1;
	if(IsValide(strErr,buffer+i,len-i,6))
	{
		for(int j=10;j<16;j++)
		{
			str.Format("%02x",buffer[j]);
			strMsg = strMsg+str;

		}
		str.Format("(预约编号：");
		strMsg = strMsg+str;
		strMsg = strMsg+")\r\n";
	}
	else
	{
		str.Format("(预约编号长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+6;
    if(IsValide(strErr,buffer+i,len-i,1))
	{    
		str.Format("%02x(执行预约的结果：",buffer[i]);
		strMsg = strMsg+str;
	
		switch(buffer[i])
		{
		case 0:
			str.Format("成功)\r\n");
		    strMsg = strMsg+str;
			break;
		default:
            str.Format("其它)\r\n");
		    strMsg = strMsg+str;
			break;
		}
	}
    else
	{
		str.Format("执行预约的结果错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+1;
    if(IsValide(strErr,buffer+i,len-i,2))
	{    
		str.Format("%02x%02x(错误原因：",buffer[i],buffer[i+1]);

		strMsg =strMsg+str;

		strMsg = errorMsg(strMsg,buffer[i]+buffer[i+1]*0x100);
	}
    else
	{
		str.Format("错误原因结果错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+2;
	
	strMsg = cmdTimeMsg(strMsg,strErr,buffer+i,len-i);
	i=i+3;
    strMsg = crcMsg(strMsg,strErr,buffer+i,len-i);

	m_Dlg->ProDecodeShow(strMsg);

	 
}

void	CProto_IEC104::Pro_UP_32_v2(CString strMsg,unsigned char *buffer,int len)
{
	CString str,strErr;
	if(buffer==0 || len==0)
	{
		str="无数据内容";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	if(IsValide(strErr,buffer,len,8))
	{
		strMsg = getEpCode(strMsg,buffer);
	}
	else
	{
		str.Format("(终端机器编码长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	int i=8;
	if(IsValide(strErr,buffer+i,len-i,1))
	{
		str.Format("%02x(枪口编号)\r\n",buffer[i]);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("枪口编号错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+1;
	if(IsValide(strErr,buffer+i,len-i,1))
	{
		str.Format("%02x(续约标识：",buffer[i]);
		strMsg = strMsg+str;

		switch(buffer[i])
		{
		case 1:
			str.Format("再续约)\r\n");
		    strMsg = strMsg+str;
			break;
		default:
            str.Format("其它)\r\n");
		    strMsg = strMsg+str;
			break;
		}
	}
	else
	{
		str.Format("续约标识错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+1;
	
    if(IsValide(strErr,buffer+i,len-i,1))
	{    
		str.Format("%02x(执行预约的结果：",buffer[i]);
		strMsg = strMsg+str;
	
		switch(buffer[i])
		{
		case 1:
			str.Format("处理成功)\r\n");
		    strMsg = strMsg+str;
			break;
		case 2:
			str.Format("数据不存在)\r\n");
		    strMsg = strMsg+str;
			break;
		case 3:
			str.Format("已经处理)\r\n");
		    strMsg = strMsg+str;
			break;
		case 4:
			str.Format("桩编号不存在)\r\n");
		    strMsg = strMsg+str;
			break;
		default:
            str.Format("其它)\r\n");
		    strMsg = strMsg+str;
			break;
		}
	}
    else
	{
		str.Format("执行预约的结果错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+1;
	
	strMsg = cmdTimeMsg(strMsg,strErr,buffer+i,len-i);
	i=i+3;
    strMsg = crcMsg(strMsg,strErr,buffer+i,len-i);

	m_Dlg->ProDecodeShow(strMsg);

	 
}


void	CProto_IEC104::Pro_UP_33_v2(CString strMsg,unsigned char *buffer,int len)
{
	CString str,strErr;
	if(buffer==0 || len==0)
	{
		str="无数据内容";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	if(IsValide(strErr,buffer,len,8))
	{
		strMsg = getEpCode(strMsg,buffer);
	}
	else
	{
		str.Format("(终端机器编码长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	int i=8;
	if(IsValide(strErr,buffer+i,len-i,1))
	{
		str.Format("%02x(枪口编号)\r\n",buffer[8]);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("枪口编号错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+1;
	if(IsValide(strErr,buffer+i,len-i,6))
	{

		strMsg = getPhone(strMsg,buffer+i);
	}
	else
	{
		str.Format("(账号(手机号长度错误))\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+6;
	if(IsValide(strErr,buffer+i,len-i,1))
	{

		str.Format("%02x(标识：",buffer[i]);
		strMsg = strMsg+str;
      
		if(buffer[i]==0)
		{
			str.Format("电桩接受了运营系统的指令)\r\n");
		    strMsg = strMsg+str;
		}
		else if(buffer[i]==1)
		{
			str.Format("解锁，下面的预约号为明文预约号)\r\n");
		    strMsg = strMsg+str;
		}
		else
		{
			str.Format("未知)\r\n");
		    strMsg = strMsg+str;
		}
	}
	else
	{
		str.Format("标识错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+1;
	if(IsValide(strErr,buffer+i,len-i,6))
	{
		strMsg = strMsg+("(预约编号:");
		for(int j=i;j<i+6;j++)
		{
			str.Format("%02x",buffer[j]);
			strMsg = strMsg+str;
			
		}
		strMsg = strMsg+str+")\r\n";
	}
	else
	{
		
		str.Format("(预约编号长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+6;
	if(IsValide(strErr,buffer+i,len-i,1))
	{
		str.Format("%02x(执行取消预约的结果：",buffer[i]);
		strMsg = strMsg+str;

		if(buffer[i]==1)
		{
			str.Format("取消预约成功)\r\n");
			strMsg = strMsg+str;
		}
		else
		{
			str.Format("失败)\r\n");
			strMsg = strMsg+str;
		}
	}
	else
	{
		str.Format("执行取消预约的结果错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+1;
	if(IsValide(strErr,buffer+i,len-i,7))
	{
         strMsg=timeMsg(strMsg,buffer+i,"时间");
	}
	else
	{

		str.Format("(时间年长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+7;
	if(IsValide(strErr,buffer+i,len-i,2))
	{ 
		str.Format("%02x %02x(执行取消错误描述：",buffer[i],buffer[i+1]);

		strMsg =strMsg+str;

		strMsg = errorMsg(strMsg,buffer[i]+buffer[i+1]*0x100);
		
	}
	else
	{
		str.Format("(执行取消错误描述错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
    i=i+2;
	
	strMsg = cmdTimeMsg(strMsg,strErr,buffer+i,len-i);
	i=i+3;
    strMsg = crcMsg(strMsg,strErr,buffer+i,len-i);
	m_Dlg->ProDecodeShow(strMsg);

	 
}


void	CProto_IEC104::Pro_UP_8(CString strMsg,unsigned char *buffer,int len)
{
	CString str,strErr;
	if(buffer==0 || len==0)
	{
		str="无数据内容";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	if(IsValide(strErr,buffer,len,8))
	{
		strMsg = getEpCode(strMsg,buffer);
	}
	else
	{
		str.Format("(终端机器编码长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	int i=8;
	if(IsValide(strErr,buffer+i,len-i,1))
	{
		str.Format("%02x(枪口编号)\r\n",buffer[8]);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("枪口编号错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+1;
	if(IsValide(strErr,buffer+i,len-i,6))
	{

		strMsg = getPhone(strMsg,buffer+i);
	}
	else
	{
		str.Format("(账号(手机号长度错误))\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+6;
	if(IsValide(strErr,buffer+i,len-i,1))
	{

		str.Format("%02x(标识：",buffer[i]);
		strMsg = strMsg+str;
      
		if(buffer[i]==0)
		{
			str.Format("电桩接受了运营系统的指令)\r\n");
		    strMsg = strMsg+str;
		}
		else if(buffer[i]==1)
		{
			str.Format("解锁，下面的预约号为明文预约号)\r\n");
		    strMsg = strMsg+str;
		}
		else
		{
			str.Format("未知)\r\n");
		    strMsg = strMsg+str;
		}
	}
	else
	{
		str.Format("标识错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+1;
	if(IsValide(strErr,buffer+i,len-i,6))
	{
		strMsg = strMsg+("(预约编号:");
		for(int j=i;j<i+6;j++)
		{
			str.Format("%02x",buffer[j]);
			strMsg = strMsg+str;
			
		}
		strMsg = strMsg+str+")\r\n";
	}
	else
	{
		
		str.Format("(预约编号长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+6;
	if(IsValide(strErr,buffer+i,len-i,1))
	{
		str.Format("%02x(执行取消预约的结果：",buffer[i]);
		strMsg = strMsg+str;

		if(buffer[i]==1)
		{
			str.Format("取消预约成功)\r\n");
			strMsg = strMsg+str;
		}
		else
		{
			str.Format("失败)\r\n");
			strMsg = strMsg+str;
		}
	}
	else
	{
		str.Format("执行取消预约的结果错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+1;
	if(IsValide(strErr,buffer+i,len-i,7))
	{

		strMsg=timeMsg(strMsg,buffer+i,"时间");
	}
	else
	{

		str.Format("(时间年长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+7;
	if(IsValide(strErr,buffer+i,len-i,2))
	{ 
		str.Format("%02x %02x(执行取消错误描述：",buffer[i],buffer[i+1]);

		strMsg =strMsg+str;

		strMsg = errorMsg(strMsg,buffer[i]+buffer[i+1]*0x100);
		
	}
	else
	{
		str.Format("(执行取消错误描述错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}

	i=i+2;
	
	strMsg = cmdTimeMsg(strMsg,strErr,buffer+i,len-i);
	i=i+3;
    strMsg = crcMsg(strMsg,strErr,buffer+i,len-i);
	m_Dlg->ProDecodeShow(strMsg);

	 
}
void	CProto_IEC104::Pro_UP_41_v2(CString strMsg,unsigned char *buffer,int len)
{
	CString str,strErr;
	if(buffer==0 || len==0)
	{
		str="无数据内容";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	
	if(IsValide(strErr,buffer,len,8))
	{
		strMsg = getEpCode(strMsg,buffer);
	}
	else
	{
		str.Format("(终端机器编码长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	int i=8;
	if(IsValide(strErr,buffer+i,len-i,1))
	{
		str.Format("%02x(枪口编号)\r\n",buffer[i]);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("枪口编号错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+1;
	if(IsValide(strErr,buffer+i,len-i,1))
	{
		str.Format("%02x(动作结果：",buffer[i]);
		strMsg = strMsg+str;

		if(buffer[i]==1)
		{
			str.Format("成功)\r\n");
			strMsg = strMsg+str;
		}
		else
		{
			str.Format("失败)\r\n");
			strMsg = strMsg+str;
		}
	}
	else
	{
		str.Format("动作结果错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i++;
	if(IsValide(strErr,buffer+i,len-i,4))
	{

		int v=getInt4Small(buffer+i);;
		str.Format("%02x %02x %02x %02x(预充金额：%0.02f）\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],v*0.01);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(预充金额长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+4;
	if(IsValide(strErr,buffer+i,len-i,2))
	{ 
		str.Format("%02x %02x(错误原因：",buffer[i],buffer[i+1]);

		strMsg =strMsg+str;

		strMsg = errorMsg(strMsg,buffer[i]+buffer[i+1]*0x100);
		
	}
	else
	{
		str.Format("(错误原因错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+2;
	
	strMsg = cmdTimeMsg(strMsg,strErr,buffer+i,len-i);
	i=i+3;
    strMsg = crcMsg(strMsg,strErr,buffer+i,len-i);
	m_Dlg->ProDecodeShow(strMsg);
}

void	CProto_IEC104::Pro_UP_9(CString strMsg,unsigned char *buffer,int len)
{
	CString str,strErr;
	if(buffer==0 || len==0)
	{
		str="无数据内容";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	if(IsValide(strErr,buffer,len,8))
	{
		strMsg = getEpCode(strMsg,buffer);
	}
	else
	{
		str.Format("(终端机器编码长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	int i=8;
	if(IsValide(strErr,buffer+i,len-i,16))
	{

		for(int j=8;j<24;j++)
		{
			str.Format("%02x",buffer[j]);
			strMsg = strMsg+str;
		}
		str.Format("(交易流水号:终端机器编码16+序列号16)\r\n");
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(交易流水号长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+16;
	unsigned int v =0;
	if(IsValide(strErr,buffer+i,len-i,4))
	{

		v=getInt4Small(buffer+i);
		str.Format("%02x %02x %02x %02x(表低示数：%0.02f度）\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],v*0.01);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(表低示数长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+4;
	if(IsValide(strErr,buffer+i,len-i,6))
	{
		str.Format("%02x%02x%02x%02x%02x%02x(预约号))\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],buffer[i+4],buffer[i+5]);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(预约号长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+6;
	if(IsValide(strErr,buffer+i,len-i,7))
	{

		strMsg=timeMsg(strMsg,buffer+i,"充电开始时间");
    }
	else
	{
		str.Format("(充电开始时间年长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+7;
	if(IsValide(strErr,buffer+i,len-i,1))
	{

		str.Format("%02x(充电枪编号)\r\n",buffer[i]);
		strMsg = strMsg+str;
    }
	else
	{
		str.Format("(充电枪编号长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+1;
	if(IsValide(strErr,buffer+i,len-i,1))
	{

		str.Format("%02x(充电方式：",buffer[i]);
		strMsg = strMsg+str;
		switch(buffer[42])
		{
		case 1:
			str.Format("电量)\r\n");
		    strMsg = strMsg+str;
			break;
		case 2:
			str.Format("金额)\r\n");
		    strMsg = strMsg+str;
			break;
		case 3:
			str.Format("时间)\r\n");
		    strMsg = strMsg+str;
			break;
		case 4:
			str.Format("自动充满)\r\n");
		    strMsg = strMsg+str;
			break;
		default:
            str.Format("未知)\r\n");
		    strMsg = strMsg+str;
			break;
		}
    }
	else
	{
		str.Format("充电方式错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+1;
	if(IsValide(strErr,buffer+i,len-i,4))
	{
		str.Format("%02x %02x %02x %02x(充满电剩余秒：%d)\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],getInt4Small(buffer+i));
		strMsg = strMsg+str;
	 }
	else
	{
		str.Format("(充满电剩余秒长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+4;
	if(IsValide(strErr,buffer+i,len-i,6))
	{

		str.Format("%02x%02x%02x%02x%02x%02x(帐号)\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],buffer[i+4],buffer[i+5]);
		strMsg = strMsg+str;
	 }
	else
	{
		str.Format("(充满电剩余秒长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}

	if(m_Dlg->getProVersion() >=12)
	{
		i=i+6;
		if(IsValide(strErr,buffer+i,len-i,4))
		{

			str.Format("%02x %02x %02x %02x(预充电金额秒：%d)\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],getInt4Small(buffer+i));
			strMsg = strMsg+str;
		}
		else
		{
			str.Format("(预充电金额秒长度错误)\r\n");
			strMsg = strMsg+strErr+str;
			m_Dlg->ProDecodeShow(strMsg);
			return;
		}

		i=i+4;
		if(IsValide(strErr,buffer+i,len-i,1))
		{

			str.Format("%02x(发起停电方式：",buffer[i]);
			strMsg = strMsg+str;
			switch(buffer[i])
			{
			case 1:
				str.Format("服务端停止充电)\r\n");
				strMsg = strMsg+str;
				break;
			case 2:
				str.Format("用户名密码鉴权充电)\r\n");
				strMsg = strMsg+str;
				break;
			case 3:
				str.Format("卡充电)\r\n");
				strMsg = strMsg+str;
				break;
			default:
				str.Format("未知)\r\n");
				strMsg = strMsg+str;
				break;
			}
		}
		else
		{
			str.Format("发起停电方式错误)\r\n");
			strMsg = strMsg+strErr+str;
			m_Dlg->ProDecodeShow(strMsg);
			return;
		}
		i=i+1;
		if(IsValide(strErr,buffer+i,len-i,1))
		{

			str.Format("%02x(离线标识：",buffer[i]);
			strMsg = strMsg+str;
			switch(buffer[i])
			{
			case 1:
				str.Format("在线)\r\n");
				strMsg = strMsg+str;
				break;
			case 0:
				str.Format("离线)\r\n");
				strMsg = strMsg+str;
				break;
			default:
				str.Format("未知)\r\n");
				strMsg = strMsg+str;
				break;
			}
		}
		else
		{
			str.Format("离线标识错误)\r\n");
			strMsg = strMsg+strErr+str;
			m_Dlg->ProDecodeShow(strMsg);
			return;
		}
		i=i+1;
		if(IsValide(strErr,buffer+i,len-i,1))
		{

			str.Format("%02x(成功标识：",buffer[i]);
			strMsg = strMsg+str;
			switch(buffer[i])
			{
			case 1:
				str.Format("成功)\r\n");
				strMsg = strMsg+str;
				break;
			default:
				str.Format("失败)\r\n");
				strMsg = strMsg+str;
				break;
			}
		}
		else
		{
			str.Format("成功标识错误)\r\n");
			strMsg = strMsg+strErr+str;
			m_Dlg->ProDecodeShow(strMsg);
			return;
		}
		i=i+1;

		strMsg = cmdTimeMsg(strMsg,strErr,buffer+i,len-i);
		i=i+3;
		strMsg = crcMsg(strMsg,strErr,buffer+i,len-i);
	}
	m_Dlg->ProDecodeShow(strMsg);

	 
}
void	CProto_IEC104::Pro_UP_42_v2(CString strMsg,unsigned char *buffer,int len)
{
	CString str,strErr;
	if(buffer==0 || len==0)
	{
		str="无数据内容";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	if(IsValide(strErr,buffer,len,8))
	{
		strMsg = getEpCode(strMsg,buffer);
	}
	else
	{
		str.Format("(终端机器编码长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	int i=8;
	if(IsValide(strErr,buffer+i,len-i,1))
	{

		str.Format("%02x(充电枪编号)\r\n",buffer[i]);
		strMsg = strMsg+str;
    }
	else
	{
		str.Format("(充电枪编号长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+1;
	if(IsValide(strErr,buffer+i,len-i,16))
	{

		for(int j=i;j<i+16;j++)
		{
			str.Format("%02x",buffer[j]);
			strMsg = strMsg+str;
		}
		str.Format("(交易流水号:终端机器编码16+序列号16)\r\n");
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(交易流水号长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+16;
	unsigned int v =0;
	if(IsValide(strErr,buffer+i,len-i,4))
	{

		v=getInt4Small(buffer+i);
		str.Format("%02x %02x %02x %02x(表低示数：%0.02f度）\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],v*0.01);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(表低示数长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+4;
	
	if(IsValide(strErr,buffer+i,len-i,7))
	{
       strMsg=timeMsg(strMsg,buffer+i,"充电开始时间");
    }
	else
	{
		str.Format("(充电开始时间年长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+7;

	if(IsValide(strErr,buffer+i,len-i,4))
	{
		str.Format("%02x %02x %02x %02x(充满电剩余秒：%d)\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],getInt4Small(buffer+i));
		strMsg = strMsg+str;
	 }
	else
	{
		str.Format("(充满电剩余秒长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+4;
	
	if(IsValide(strErr,buffer+i,len-i,1))
	{

		str.Format("%02x(标识：",buffer[i]);
		strMsg = strMsg+str;
		switch(buffer[i])
		{
		case 1:
			str.Format("成功充电)\r\n");
		    strMsg = strMsg+str;
			break;
		case 0:
			str.Format("失败)\r\n");
		    strMsg = strMsg+str;
			break;
		
		default:
            str.Format("未知)\r\n");
		    strMsg = strMsg+str;
			break;
		}
    }
	else
	{
		str.Format("标识错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+1;

    if(IsValide(strErr,buffer+i,len-i,2))
	{
		str.Format("%02x %02x(错误码：",buffer[i],buffer[i+1]);

		strMsg =strMsg+str;

		strMsg = errorMsg(strMsg,buffer[i]+buffer[i+1]*0x100);
		
	}
	else
	{
		str.Format("(错误码错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+2;
	
	strMsg = cmdTimeMsg(strMsg,strErr,buffer+i,len-i);
	i=i+3;
    strMsg = crcMsg(strMsg,strErr,buffer+i,len-i);

	
	m_Dlg->ProDecodeShow(strMsg);

	 
}
void	CProto_IEC104::Pro_UP_43_v2(CString strMsg,unsigned char *buffer,int len)
{
	CString str,strErr;
	if(buffer==0 || len==0)
	{
		str="无数据内容";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	if(IsValide(strErr,buffer,len,8))
	{
		strMsg = getEpCode(strMsg,buffer);
	}
	else
	{
		str.Format("(终端机器编码长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	int i=8;
	if(IsValide(strErr,buffer+i,len-i,1))
	{
		str.Format("%02x(枪口编号)\r\n",buffer[i]);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("枪口编号错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+1;
	if(IsValide(strErr,buffer+i,len-i,1))
	{
		str.Format("%02x(动作结果：",buffer[i]);
		strMsg = strMsg+str;

		if(buffer[i]==0)
		{
			str.Format("成功)\r\n");
			strMsg = strMsg+str;
		}
		else
		{
			str.Format("失败)\r\n");
			strMsg = strMsg+str;
		}
	}
	else
	{
		str.Format("动作结果错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+1;
	
	strMsg = cmdTimeMsg(strMsg,strErr,buffer+i,len-i);
	i=i+3;
    strMsg = crcMsg(strMsg,strErr,buffer+i,len-i);

	m_Dlg->ProDecodeShow(strMsg);
}

void	CProto_IEC104::Pro_UP_10(CString strMsg,unsigned char *buffer,int len)
{
	CString str,strErr;
	if(buffer==0 || len==0)
	{
		str="无数据内容";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	if(IsValide(strErr,buffer,len,8))
	{
		strMsg = getEpCode(strMsg,buffer);
	}
	else
	{
		str.Format("(终端机器编码长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	int i=8;
	if(IsValide(strErr,buffer+i,len-i,4))
	{

		double v =(buffer[11]*0x1000000+buffer[10]*0x10000+buffer[9]*0x100+buffer[8])/1000.0;
		str.Format("%02x %02x %02x %02x(表低示数：%0.3f)\r\n",buffer[8],buffer[9],buffer[10],buffer[11],v);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(表低示数长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
    i=i+4;
	if(IsValide(strErr,buffer+i,len-i,16))
	{
		for(int j=12;j<28;j++)
		{
			str.Format("%02x",buffer[j]);
			strMsg = strMsg+str;
		}
		str.Format("(交易流水号：终端机器编码16+序列号16)\r\n");
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(交易流水号长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	 i=i+16;
	if(IsValide(strErr,buffer+i,len-i,7))
	{	
		strMsg=timeMsg(strMsg,buffer+i,"结束开始时间");
    }
	else
	{
		str.Format("(结束充电时间年长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+7;
	if(IsValide(strErr,buffer+i,len-i,1))
	{
		
		str.Format("%02x (充电抢编号)\r\n",buffer[35]);
		strMsg = strMsg+str;
     }
	else
	{
		str.Format("(充电抢编号错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+1;
	if(IsValide(strErr,buffer+i,len-i,2))
	{
		str.Format("%02x %02x(停止充电原因：",buffer[36],buffer[37]);

		strMsg =strMsg+str;

		strMsg = errorMsg(strMsg,buffer[36]+buffer[37]*0x100);
		
	}
	else
	{
		str.Format("(停止充电原因错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	if(m_Dlg->getProVersion() >=12)
	{
		i=i+2;
		if(IsValide(strErr,buffer+i,len-i,1))
		{

			str.Format("%02x(发起停电方式：",buffer[i]);
			strMsg = strMsg+str;
			switch(buffer[i])
			{
			case 1:
				str.Format("服务端停止充电)\r\n");
				strMsg = strMsg+str;
				break;
			case 2:
				str.Format("用户名密码鉴权充电)\r\n");
				strMsg = strMsg+str;
				break;
			case 3:
				str.Format("卡充电)\r\n");
				strMsg = strMsg+str;
				break;
			default:
				str.Format("未知)\r\n");
				strMsg = strMsg+str;
				break;
			}
		}
		else
		{
			str.Format("发起停电方式错误)\r\n");
			strMsg = strMsg+strErr+str;
			m_Dlg->ProDecodeShow(strMsg);
			return;
		}
		i=i+1;
		if(IsValide(strErr,buffer+i,len-i,1))
		{

			str.Format("%02x(离线标识：",buffer[i]);
			strMsg = strMsg+str;
			switch(buffer[i])
			{
			case 1:
				str.Format("在线)\r\n");
				strMsg = strMsg+str;
				break;
			case 0:
				str.Format("离线)\r\n");
				strMsg = strMsg+str;
				break;
			default:
				str.Format("未知)\r\n");
				strMsg = strMsg+str;
				break;
			}
		}
		else
		{
			str.Format("离线标识错误)\r\n");
			strMsg = strMsg+strErr+str;
			m_Dlg->ProDecodeShow(strMsg);
			return;
		}
		i=i+1;
		if(IsValide(strErr,buffer+i,len-i,1))
		{

			str.Format("%02x(成功标识：",buffer[i]);
			strMsg = strMsg+str;
			switch(buffer[i])
			{
			case 1:
				str.Format("成功)\r\n");
				strMsg = strMsg+str;
				break;
			default:
				str.Format("失败)\r\n");
				strMsg = strMsg+str;
				break;
			}
		}
		else
		{
			str.Format("成功标识错误)\r\n");
			strMsg = strMsg+strErr+str;
			m_Dlg->ProDecodeShow(strMsg);
			return;
		}
		i=i+1;

		strMsg = cmdTimeMsg(strMsg,strErr,buffer+i,len-i);
		i=i+3;
		strMsg = crcMsg(strMsg,strErr,buffer+i,len-i);

	}
	m_Dlg->ProDecodeShow(strMsg);

	 
}

void	CProto_IEC104::Pro_UP_45_v2(CString strMsg,unsigned char *buffer,int len)
{
	CString str,strErr;
	if(buffer==0 || len==0)
	{
		str="无数据内容";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	if(IsValide(strErr,buffer,len,8))
	{
		strMsg = getEpCode(strMsg,buffer);
	}
	else
	{
		str.Format("(终端机器编码长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	int i=8;
	if(IsValide(strErr,buffer+i,len-i,4))
	{

		double v =(buffer[11]*0x1000000+buffer[10]*0x10000+buffer[9]*0x100+buffer[8])/1000.0;
		str.Format("%02x %02x %02x %02x(表低示数：%0.3f)\r\n",buffer[8],buffer[9],buffer[10],buffer[11],v);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(表低示数长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
    i=i+4;
	if(IsValide(strErr,buffer+i,len-i,16))
	{
		for(int j=12;j<28;j++)
		{
			str.Format("%02x",buffer[j]);
			strMsg = strMsg+str;
		}
		str.Format("(交易流水号：终端机器编码16+序列号16)\r\n");
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(交易流水号长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	 i=i+16;
	if(IsValide(strErr,buffer+i,len-i,7))
	{	
		strMsg=timeMsg(strMsg,buffer+i,"结束充电时间");
    }
	else
	{
		str.Format("(结束充电时间年长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+7;
	if(IsValide(strErr,buffer+i,len-i,1))
	{
		
		str.Format("%02x (充电抢编号)\r\n",buffer[35]);
		strMsg = strMsg+str;
     }
	else
	{
		str.Format("(充电抢编号错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+1;
	if(IsValide(strErr,buffer+i,len-i,2))
	{
		str.Format("%02x %02x(停止充电原因：",buffer[36],buffer[37]);

		strMsg =strMsg+str;

		strMsg = errorMsg(strMsg,buffer[36]+buffer[37]*0x100);
		
	}
	else
	{
		str.Format("(停止充电原因错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	
	i=i+2;
	if(IsValide(strErr,buffer+i,len-i,1))
	{

		str.Format("%02x(发起停电方式：",buffer[i]);
		strMsg = strMsg+str;
		switch(buffer[i])
		{
		case 1:
			str.Format("服务端停止充电)\r\n");
		    strMsg = strMsg+str;
			break;
		case 2:
			str.Format("用户名密码鉴权充电)\r\n");
		    strMsg = strMsg+str;
			break;
		case 3:
			str.Format("卡充电)\r\n");
		    strMsg = strMsg+str;
			break;
		default:
            str.Format("未知)\r\n");
		    strMsg = strMsg+str;
			break;
		}
	}
	else
	{
		str.Format("发起停电方式错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+1;
	if(IsValide(strErr,buffer+i,len-i,1))
	{

		str.Format("%02x(离线标识：",buffer[i]);
		strMsg = strMsg+str;
		switch(buffer[i])
		{
		case 1:
			str.Format("在线)\r\n");
		    strMsg = strMsg+str;
			break;
		case 0:
			str.Format("离线)\r\n");
		    strMsg = strMsg+str;
			break;
		default:
            str.Format("未知)\r\n");
		    strMsg = strMsg+str;
			break;
		}
    }
	else
	{
		str.Format("离线标识错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+1;
	if(IsValide(strErr,buffer+i,len-i,1))
	{

		str.Format("%02x(成功标识：",buffer[i]);
		strMsg = strMsg+str;
		switch(buffer[i])
		{
		case 1:
			str.Format("成功)\r\n");
		    strMsg = strMsg+str;
			break;
		default:
            str.Format("失败)\r\n");
		    strMsg = strMsg+str;
			break;
		}
    }
	else
	{
		str.Format("成功标识错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+1;
	
	strMsg = cmdTimeMsg(strMsg,strErr,buffer+i,len-i);
	i=i+3;
    strMsg = crcMsg(strMsg,strErr,buffer+i,len-i);

	m_Dlg->ProDecodeShow(strMsg);

	 
}

void	CProto_IEC104::Pro_UP_44_v2(CString strMsg,unsigned char *buffer,int len)
{
	CString str,strErr;
	if(buffer==0 || len==0)
	{
		str="无数据内容";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}

	if(IsValide(strErr,buffer,len,8))
	{
		strMsg = getEpCode(strMsg,buffer);
	}
	else
	{
		str.Format("(终端机器编码长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	int i=8;
	if(IsValide(strErr,buffer+i,len-i,1))
	{
		
		str.Format("%02x (充电抢编号)\r\n",buffer[35]);
		strMsg = strMsg+str;
     }
	else
	{
		str.Format("(充电抢编号错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+1;
	if(IsValide(strErr,buffer+i,len-i,32))
	{
		CString s,s1;
		for(int j=i;j<i+32;j++)
		{
			str.Format("%02x ",buffer[j]);
			strMsg = strMsg+str;

			s1.Format("%c",buffer[j]);
			s = s+s1;
		}
		str.Format("(内卡号：%s)",s);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("内卡号长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+32;
	int v=0;
	if(IsValide(strErr,buffer+i,len-i,4))
	{
		v =(getInt4Small(buffer+i));
		str.Format("%02x %02x %02x %02x (预冻金额：%f)\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],v*0.01);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(预冻金额错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}

	i=i+4;
	
	strMsg = cmdTimeMsg(strMsg,strErr,buffer+i,len-i);
	i=i+3;
    strMsg = crcMsg(strMsg,strErr,buffer+i,len-i);
	m_Dlg->ProDecodeShow(strMsg);
}

CString	CProto_IEC104::cosMsg(CString strMsg,int cos)
{
	CString str1;
	switch(cos)
	{
	case 0:
		str1.Format("未用)\r\n");
		strMsg=strMsg+str1;
		break;
	case 2:
		str1.Format("周期,循环)\r\n");
		strMsg=strMsg+str1;
		break;
	case 3:
		str1.Format("背景扫描)\r\n");
		strMsg=strMsg+str1;
		break;
	case 4:
		str1.Format("初始化)\r\n");
		strMsg=strMsg+str1;
		break;
	case 5:
		str1.Format("请求或被请求)\r\n");
		strMsg=strMsg+str1;
		break;
	case 6:
		str1.Format("激活)\r\n");
		strMsg=strMsg+str1;
		break;
	case 7:
		str1.Format("激活确认)\r\n");
		strMsg=strMsg+str1;
		break;
	case 20:
		str1.Format("响应占总招)\r\n");
		strMsg=strMsg+str1;
		break;
	case 44:
		str1.Format("未知的类型标识)\r\n");
		strMsg=strMsg+str1;
		break;
	case 45:
		str1.Format("未知的传送原因)\r\n");
		strMsg=strMsg+str1;
		break;
	case 46:
		str1.Format("未知的应用数据单元公共地址)\r\n");
		strMsg=strMsg+str1;
		break;
	case 47:
		str1.Format("未知的信息对象地址)\r\n");
		strMsg=strMsg+str1;
		break;
	default:
		str1.Format("未定义)\r\n");
		strMsg=strMsg+str1;
		break;
	}

	return strMsg;
}



CString	CProto_IEC104::errorMsg(CString strMsg,int cos)
{
	CString str;
	switch(cos)
	{
	case 1:
		str.Format("正常)\r\n");
		strMsg = strMsg+str;
		break;
	case 2:
		str.Format("用户强制结束)\r\n");
		strMsg = strMsg+str;
		break;
	case 3:
		str.Format("急停)\r\n");
		strMsg = strMsg+str;
		break;
	case 4:
		str.Format("连接线断掉)\r\n");
		strMsg = strMsg+str;
		break;
	case 5:
		str.Format("电表异常)\r\n");
		strMsg = strMsg+str;
		break;
	case 6:
		str.Format("过流停止)\r\n");
		strMsg = strMsg+str;
		break;
	case 7:
		str.Format("过压停止)\r\n");
		strMsg = strMsg+str;
		break;
	case 8:
		str.Format("防雷器故障)\r\n");
		strMsg = strMsg+str;
		break;
	case 9:
		str.Format("接触器故障)\r\n");
		strMsg = strMsg+str;
		break;
	case 10:
		str.Format("充电金额不足)\r\n");
		strMsg = strMsg+str;
		break;
	case 11:
		str.Format("漏电保护器)\r\n");
		strMsg = strMsg+str;
		break;
	case 12:
		str.Format("自动充满)\r\n");
		strMsg = strMsg+str;
		break;
	case 13:
		str.Format("BMS通信异常故障)\r\n");
		strMsg = strMsg+str;
		break;
	case 14:
		str.Format("违规拔枪)\r\n");
		strMsg = strMsg+str;
		break;
	case 15:
		str.Format("电桩断电)\r\n");
		strMsg = strMsg+str;
		break;
	case 16:
		str.Format("ccu故障)\r\n");
		strMsg = strMsg+str;
		break;
	case 17:
		str.Format("显示屏故障)\r\n");
		strMsg = strMsg+str;
		break;
	case 18:
		str.Format("电源模块故障)\r\n");
		strMsg = strMsg+str;
		break;
	case 19:
		str.Format("车主动停止)\r\n");
		strMsg = strMsg+str;
		break;
	case 21:
		str.Format("用户app 上停止充电)\r\n");
		strMsg = strMsg+str;
		break;
	case 22:
		str.Format("二维码方式充电桩上结束充电)\r\n");
		strMsg = strMsg+str;
		break;
	case 23:
		str.Format("用户名密码用户输入密码结束充电)\r\n");
		strMsg = strMsg+str;
		break;
	case 24:
		str.Format("验证码方式用户输入验证码结束充电)\r\n");
		strMsg = strMsg+str;
		break;
	case 25:
		str.Format("充电桩检测汽车充电充满信号结束充电)\r\n");
		strMsg = strMsg+str;
		break;
	case 26:
		str.Format("按时间充时间达到设定值)\r\n");
		strMsg = strMsg+str;
		break;
	case 27:
		str.Format("按电量充电量达到设定值)\r\n");
		strMsg = strMsg+str;
		break;
	case 28:
		str.Format("按金额充金额达到设定值)\r\n");
		strMsg = strMsg+str;
		break;
	case 29:
		str.Format("自动充满方式)\r\n");
		strMsg = strMsg+str;
		break;
	case 30:
		str.Format("第5 点充电桩检测汽车充电充满信号结束充电)\r\n");
		strMsg = strMsg+str;
		break;
	case 31:
		str.Format("电流小于1A 超过规定时间(1分钟和10 分钟)结束充电)\r\n");
		strMsg = strMsg+str;
		break;
	default:
		str.Format("未知)\r\n");
		strMsg = strMsg+str;
		break;
	}
	return strMsg;
}

void	CProto_IEC104::Pro_UP_11(CString strMsg,unsigned char *buffer,int len)
{
	CString str,strErr;
	if(buffer==0 || len==0)
	{
		str="无数据内容";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	if(IsValide(strErr,buffer,len,8))
	{
		strMsg = getEpCode(strMsg,buffer);
	}
	else
	{
		str.Format("(终端机器编码长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	int i=8;
	if(IsValide(strErr,buffer+i,len-i,16))
	{
		for(int j=8;j<24;j++)
		{
			str.Format("%02x",buffer[j]);
			strMsg = strMsg+str;
		}
		str.Format("(交易流水号:终端机器编码16+序列号16)\r\n");
		strMsg = strMsg+str;
	}
    else
	{
		str.Format("(交易流水号长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+16;
	if(IsValide(strErr,buffer+i,len-i,8))
	{
		for(int j=24;j<32;j++)
		{
			if(buffer[j] == 0xff)
				continue;
			str.Format("%02x",buffer[j]);
			strMsg = strMsg+str;
		}
		str.Format("(用户编号)\r\n");
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(用户编号长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+8;
	if(IsValide(strErr,buffer+i,len-i,1))
	{
	
		str.Format("%02x (离线交易类型)\r\n",buffer[32]);
		strMsg = strMsg+str;
    }
	else
	{
		str.Format("(离线交易类型长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+1;
	if(IsValide(strErr,buffer+i,len-i,7))
	{
		strMsg=timeMsg(strMsg,buffer+i,"开始时间");
    }
	else
	{
		str.Format("(开始时间年长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+7;
	if(IsValide(strErr,buffer+i,len-i,7))
	{
		strMsg=timeMsg(strMsg,buffer+i,"结束时间");
     }
	else
	{
		str.Format("(结束时间年长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+7;
	
	double v=0;
	float rate=1000.0;
	if(IsValide(strErr,buffer+i,len-i,4))
	{
		v = (getInt4Small(buffer+i))/rate;
		 str.Format("%02x %02x %02x %02x (尖起示值：%0.3f)\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],v);
		strMsg = strMsg+str;
     }
	else
	{
		str.Format("(尖起示值长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+4;
	if(IsValide(strErr,buffer+i,len-i,4))
	{
		v = (getInt4Small(buffer+i))/rate;
		 str.Format("%02x %02x %02x %02x (尖止示值：%0.3f)\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],v);
		strMsg = strMsg+str;
   }
	else
	{
		str.Format("(尖止示值长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+4;
	if(IsValide(strErr,buffer+i,len-i,4))
	{
		v = (getInt4Small(buffer+i))/rate;
		 str.Format("%02x %02x %02x %02x (峰起示值：%0.3f)\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],v);
		strMsg = strMsg+str;
    }
	else
	{
		str.Format("(峰起示值长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+4;
	if(IsValide(strErr,buffer+i,len-i,4))
	{
		v = (getInt4Small(buffer+i))/rate;
		 str.Format("%02x %02x %02x %02x (峰止示值：%0.3f)\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],v);
		strMsg = strMsg+str;
     }
	else
	{
		str.Format("(峰止示值长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+4;
	if(IsValide(strErr,buffer+i,len-i,4))
	{
		 v = (getInt4Small(buffer+i))/rate;
		 str.Format("%02x %02x %02x %02x (平起示值：%0.3f)\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],v);
		strMsg = strMsg+str;
     }
	else
	{
		str.Format("(平起示值长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+4;
	if(IsValide(strErr,buffer+i,len-i,4))
	{
		v = (getInt4Small(buffer+i))/rate;
		 str.Format("%02x %02x %02x %02x (平止示值：%0.3f)\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],v);
		strMsg = strMsg+str;
    }
	else
	{
		str.Format("(平止示值长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+4;
	if(IsValide(strErr,buffer+i,len-i,4))
	{
		v = (getInt4Small(buffer+i))/rate;
		 str.Format("%02x %02x %02x %02x (谷起示值：%0.3f)\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],v);
		strMsg = strMsg+str;
    }
	else
	{
		str.Format("(谷起示值长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+4;
	if(IsValide(strErr,buffer+i,len-i,4))
	{
		 v = (getInt4Small(buffer+i))/rate;
		 str.Format("%02x %02x %02x %02x (谷止示值：%0.3f)\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],v);
		strMsg = strMsg+str;
    }
	else
	{
		str.Format("(谷止示值长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+4;
	if(IsValide(strErr,buffer+i,len-i,1))
	{
		str.Format("%02x (计量类型:",buffer[i]);
		strMsg = strMsg+str;
		switch(buffer[i])
		{
		case 1:
			str.Format("里程)\r\n");
		    strMsg = strMsg+str;
		   break;
	   case 2:
			str.Format("充电量)\r\n");
		    strMsg = strMsg+str;
		   break;
	   case 3:
			str.Format("放电量)\r\n");
		    strMsg = strMsg+str;
		   break;
	   default:
			str.Format("未知)\r\n");
		    strMsg = strMsg+str;
			break;
		}
    }
	else
	{
		str.Format("(计量类型错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+1;
	if(IsValide(strErr,buffer+i,len-i,4))
	{
		 v = (getInt4Small(buffer+i))/rate;
		 str.Format("%02x %02x %02x %02x (本次计量示数：%0.3f)\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],v);
		strMsg = strMsg+str;
     }
	else
	{
		str.Format("(本次计量示数长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+4;
	if(IsValide(strErr,buffer+i,len-i,4))
	{
		v = (getInt4Small(buffer+i))/rate;
		 str.Format("%02x %02x %02x %02x (上次计量示数：%0.3f)\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],v);
		strMsg = strMsg+str;
     }
	else
	{
		str.Format("(上次计量示数长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+4;
	if(IsValide(strErr,buffer+i,len-i,4))
	{
		v = (getInt4Small(buffer+i))/100.0;
		 str.Format("%02x %02x %02x %02x (尖单价：%0.2f)\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],v);
		strMsg = strMsg+str;
    }
	else
	{
		str.Format("(尖单价长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+4;
	if(IsValide(strErr,buffer+i,len-i,4))
	{
		v = (getInt4Small(buffer+i))/rate;
		 str.Format("%02x %02x %02x %02x (尖电量：%0.3f)\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],v);
		strMsg = strMsg+str;
    }
	else
	{
		str.Format("(尖电量长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+4;
	if(IsValide(strErr,buffer+i,len-i,4))
	{
		v = (getInt4Small(buffer+i))/100.0;
		 str.Format("%02x %02x %02x %02x (尖金额：%0.2f)\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],v);
		strMsg = strMsg+str;
    }
	else
	{
		str.Format("(尖金额长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+4;
	if(IsValide(strErr,buffer+i,len-i,4))
	{
		v = (getInt4Small(buffer+i))/100.0;
		 str.Format("%02x %02x %02x %02x (峰单价：%0.2f)\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],v);
		strMsg = strMsg+str;
    }
	else
	{
		str.Format("(峰单价长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+4;
	if(IsValide(strErr,buffer+i,len-i,4))
	{
		v = (getInt4Small(buffer+i))/rate;
		 str.Format("%02x %02x %02x %02x (峰电量：%0.3f)\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],v);
		strMsg = strMsg+str;
    }
	else
	{
		str.Format("(峰电量长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+4;
	if(IsValide(strErr,buffer+i,len-i,4))
	{
		v = (getInt4Small(buffer+i))/100.0;
		 str.Format("%02x %02x %02x %02x (峰金额：%0.2f)\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],v);
		strMsg = strMsg+str;
     }
	else
	{
		str.Format("(峰金额长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+4;
	if(IsValide(strErr,buffer+i,len-i,4))
	{
		v = (getInt4Small(buffer+i))/100.0;
		 str.Format("%02x %02x %02x %02x (平单价：%0.2f)\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],v);
		strMsg = strMsg+str;
    }
	else
	{
		str.Format("(平单价长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+4;
	if(IsValide(strErr,buffer+i,len-i,4))
	{
		v = (getInt4Small(buffer+i))/rate;
		 str.Format("%02x %02x %02x %02x (平电量：%0.3f)\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],v);
		strMsg = strMsg+str;
    }
	else
	{
		str.Format("(平电量长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+4;
	if(IsValide(strErr,buffer+i,len-i,4))
	{
		v = (getInt4Small(buffer+i))/100.0;
		 str.Format("%02x %02x %02x %02x (平金额：%0.2f)\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],v);
		strMsg = strMsg+str;
    }
	else
	{
		str.Format("(平金额长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+4;
	if(IsValide(strErr,buffer+i,len-i,4))
	{
		v = (getInt4Small(buffer+i))/100.0;
		 str.Format("%02x %02x %02x %02x (谷单价：%0.2f)\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],v);
		strMsg = strMsg+str;
    }
	else
	{
		str.Format("(谷单价长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+4;
	if(IsValide(strErr,buffer+i,len-i,4))
	{
		v = (getInt4Small(buffer+i))/rate;
		 str.Format("%02x %02x %02x %02x (谷电量：%0.3f)\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],v);
		strMsg = strMsg+str;
     }
	else
	{
		str.Format("(谷电量长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+4;
	if(IsValide(strErr,buffer+i,len-i,4))
	{
		v = (getInt4Small(buffer+i))/100.0;
		 str.Format("%02x %02x %02x %02x (谷金额：%0.2f)\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],v);
		strMsg = strMsg+str;
    }
	else
	{
		str.Format("(谷金额长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+4;
	if(IsValide(strErr,buffer+i,len-i,4))
	{
		v = (getInt4Small(buffer+i))/rate;
		 str.Format("%02x %02x %02x %02x (总电量：%0.3f)\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],v);
		strMsg = strMsg+str;
     }
	else
	{
		str.Format("(总电量长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+4;
	if(IsValide(strErr,buffer+i,len-i,2))
	{
		
		 str.Format("%02x%02x(业务类型)\r\n",buffer[i],buffer[i+1]);
		strMsg = strMsg+str;
     }
	else
	{
		str.Format("(业务类型长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+2;
	if(IsValide(strErr,buffer+i,len-i,4))
	{
		v = (getInt4Small(buffer+i))/100.0;
		 str.Format("%02x %02x %02x %02x (消费数值：%0.2f)\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],v);
		strMsg = strMsg+str;
      }
	else
	{
		str.Format("(消费数值长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+4;
	if(IsValide(strErr,buffer+i,len-i,4))
	{
	v = (getInt4Small(buffer+i))/100.0;
		 str.Format("%02x %02x %02x %02x (消费单价：%0.2f)\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],v);
		strMsg = strMsg+str;
     }
	else
	{
		str.Format("(消费单价长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+4;
	if(IsValide(strErr,buffer+i,len-i,4))
	{
		v = (getInt4Small(buffer+i))/100.0;
		str.Format("%02x %02x %02x %02x (消费金额：%0.2f)\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],v);
		strMsg = strMsg+str;
      }
	else
	{
		str.Format("(消费金额长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+4;
	if(IsValide(strErr,buffer+i,len-i,32))
	{
		CString s,s1;
		for(;i<186;i++)
		{
		   str.Format("%02x ",buffer[i]);
		   strMsg = strMsg+str;
		   s1.Format("%c",buffer[i]);
		   s=s+s1;
		}
		str.Format("(电动汽车唯一标识：%s)\r\n",s);
		strMsg = strMsg+str;
		
	}
	else
	{
		str.Format("(电动汽车唯一标识长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=186;
	if(IsValide(strErr,buffer+i,len-i,1))
	{

		 str.Format("%02x(充电桩接口标识)\r\n",buffer[i]);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(充电桩接口标识错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+1;
	
	strMsg = cmdTimeMsg(strMsg,strErr,buffer+i,len-i);
	i=i+3;
    strMsg = crcMsg(strMsg,strErr,buffer+i,len-i);
    m_Dlg->ProDecodeShow(strMsg);

}

void	CProto_IEC104::Pro_UP_49_v4(CString strMsg,unsigned char *buffer,int len)
{
	CString str,strErr;
	if(buffer==0 || len==0)
	{
		str="无数据内容";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	if(IsValide(strErr,buffer,len,8))
	{
		strMsg = getEpCode(strMsg,buffer);
	}
	else
	{
		str.Format("(终端机器编码长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	int i=8;
	if(IsValide(strErr,buffer+i,len-i,1))
	{
		str.Format("%02x(充电接口标识)\r\n",buffer[8]);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("充电接口标识错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+1;
	
	strMsg = cmdTimeMsg(strMsg,strErr,buffer+i,len-i);
	i=i+3;
    strMsg = crcMsg(strMsg,strErr,buffer+i,len-i);
    m_Dlg->ProDecodeShow(strMsg);

}




void	CProto_IEC104::Pro_UP_47_v2(CString strMsg,unsigned char *buffer,int len)
{
	CString str,strErr;
	if(buffer==0 || len==0)
	{
		str="无数据内容";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	if(IsValide(strErr,buffer,len,8))
	{
		strMsg = getEpCode(strMsg,buffer);
	}
	else
	{
		str.Format("(终端机器编码长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	int i=8;
	if(IsValide(strErr,buffer+i,len-i,1))
	{
		str.Format("%02x(充电接口标识)\r\n",buffer[8]);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("充电接口标识错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i++;
	if(IsValide(strErr,buffer+i,len-i,6))
	{
		for(int j=8;j<14;j++)
		{
			str.Format("%02x",buffer[j]);
			strMsg = strMsg+str;
		}
		str.Format("(用户账号)\r\n");
		strMsg = strMsg+str;
	}
    else
	{
		str.Format("(用户账号错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+6;
	
	double v=0;
	float rate=10.0;
	
	if(IsValide(strErr,buffer+i,len-i,4))
	{
		v = (getInt4Small(buffer+i))/rate;
		 str.Format("%02x %02x %02x %02x (用户余额：%0.1f)\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],v);
		strMsg = strMsg+str;
    }
	else
	{
		str.Format("(用户余额长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+4;
	if(IsValide(strErr,buffer+i,len-i,4))
	{
		v = (getInt4Small(buffer+i))/10.0;
		 str.Format("%02x %02x %02x %02x (告警金额：%0.2f)\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],v);
		strMsg = strMsg+str;
    }
	else
	{
		str.Format("(告警金额长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+4;
	
	strMsg = cmdTimeMsg(strMsg,strErr,buffer+i,len-i);
	i=i+3;
    strMsg = crcMsg(strMsg,strErr,buffer+i,len-i);
    m_Dlg->ProDecodeShow(strMsg);

}

CString	CProto_IEC104::Pro_UP_46_v2(CString strMsg,unsigned char *buffer,int len)
{
	CString str,strErr;
	if(buffer==0 || len==0)
	{
		str="无数据内容";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return strMsg;
	}
	if(IsValide(strErr,buffer,len,8))
	{
		strMsg = getEpCode(strMsg,buffer);
	}
	else
	{
		str.Format("(终端机器编码长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return strMsg;
	}
	int i=8;
	if(IsValide(strErr,buffer+i,len-i,1))
	{
		str.Format("%02x(充电接口标识)\r\n",buffer[i]);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("充电接口标识错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return strMsg;
	}
	i++;
	if(IsValide(strErr,buffer+i,len-i,16))
	{
		for(int j=i;j<i+16;j++)
		{
			str.Format("%02x",buffer[j]);
			strMsg = strMsg+str;
		}
		str.Format("(交易流水号:终端机器编码16+序列号16)\r\n");
		strMsg = strMsg+str;
	}
    else
	{
		str.Format("(交易流水号长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return strMsg;
	}
	i=i+16;
	if(IsValide(strErr,buffer+i,len-i,1))
	{
		str.Format("%02x(帐号类型:",buffer[i]);
		strMsg = strMsg+str;
		switch(buffer[i])
		{
		case 1:
			str.Format("帐号)\r\n");
		    strMsg = strMsg+str;
			break;
		case 2:
			str.Format("普通卡)\r\n");
		    strMsg = strMsg+str;
			break;
		case 3:
			str.Format("特殊卡)\r\n");
		    strMsg = strMsg+str;
			break;
		default:
            str.Format("未知)\r\n");
		    strMsg = strMsg+str;
			break;
		}
	}
	else
	{
		str.Format("帐号类型错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return strMsg;
	}
     i=i+1;
	if(IsValide(strErr,buffer+i,len-i,2))
	{
		str.Format("%02x %02x(用户来源:",buffer[i],buffer[i+1]);
		strMsg = strMsg+str;
		switch(buffer[i]+buffer[i+1]*0x100)
		{
		case 0:
			str.Format("爱充)\r\n");
		    strMsg = strMsg+str;
			break;
		case 1:
			str.Format("富士康)\r\n");
		    strMsg = strMsg+str;
			break;
		case 2:
			str.Format("吉利)\r\n");
		    strMsg = strMsg+str;
			break;
		case 3:
			str.Format("绿地)\r\n");
		    strMsg = strMsg+str;
			break;
		case 4:
			str.Format("浙誉)\r\n");
		    strMsg = strMsg+str;
			break;
		case 5:
			str.Format("北汽出行)\r\n");
		    strMsg = strMsg+str;
			break;
		default:
            str.Format("未知)\r\n");
		    strMsg = strMsg+str;
			break;
		}
	}
	else
	{
		str.Format("用户来源错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return strMsg;
	}
     i=i+2;
	if(IsValide(strErr,buffer+i,len-i,32))
	{
		for(int j=i;j<i+32;j++)
		{
			if(buffer[j] == 0xff)
				continue;
			str.Format("%02x",buffer[j]);
			strMsg = strMsg+str;
		}
		str.Format("(用户编号)\r\n");
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(用户编号长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return strMsg;
	}
	i=i+32;
	if(IsValide(strErr,buffer+i,len-i,1))
	{
	
		str.Format("%02x (离线交易类型)\r\n",buffer[i]);
		strMsg = strMsg+str;
		switch(buffer[i])
		{
		case 0:
			str.Format("离线，临时充电之前的版本)\r\n");
		    strMsg = strMsg+str;
			break;
		case 1:
			str.Format("在线)\r\n");
		    strMsg = strMsg+str;
			break;
		case 2:
			str.Format("离线）\r\n");
		    strMsg = strMsg+str;
			break;
		default:
            str.Format("未知)\r\n");
		    strMsg = strMsg+str;
			break;
		}
    }
	else
	{
		str.Format("(离线交易类型长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return strMsg;
	}
	i=i+1;
	if(IsValide(strErr,buffer+i,len-i,7))
	{
		strMsg=timeMsg(strMsg,buffer+i,"开始时间");
    }
	else
	{
		str.Format("(开始时间年长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return strMsg;
	}
	i=i+7;
	if(IsValide(strErr,buffer+i,len-i,7))
	{
		strMsg=timeMsg(strMsg,buffer+i,"结束时间");
     }
	else
	{
		str.Format("(结束时间年长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return strMsg;
	}
	i=i+7;
	
	double v=0;
	float rate=1000.0;
	
	if(IsValide(strErr,buffer+i,len-i,4))
	{
		v = getInt4Small(buffer+i)/rate;
		 str.Format("%02x %02x %02x %02x (尖电量：%0.3f)\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],v);
		strMsg = strMsg+str;
    }
	else
	{
		str.Format("(尖电量长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return strMsg;
	}
	i=i+4;
	if(IsValide(strErr,buffer+i,len-i,4))
	{
		v = getInt4Small(buffer+i)/100.0;
		 str.Format("%02x %02x %02x %02x (尖金额：%0.2f)\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],v);
		strMsg = strMsg+str;
    }
	else
	{
		str.Format("(尖金额长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return strMsg;
	}
	i=i+4;
	
	if(IsValide(strErr,buffer+i,len-i,4))
	{
		v = getInt4Small(buffer+i)/rate;
		 str.Format("%02x %02x %02x %02x (峰电量：%0.3f)\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],v);
		strMsg = strMsg+str;
    }
	else
	{
		str.Format("(峰电量长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return strMsg;
	}
	i=i+4;
	if(IsValide(strErr,buffer+i,len-i,4))
	{
		v = getInt4Small(buffer+i)/100.0;
		 str.Format("%02x %02x %02x %02x (峰金额：%0.2f)\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],v);
		strMsg = strMsg+str;
     }
	else
	{
		str.Format("(峰金额长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return strMsg;
	}
	i=i+4;
	
	if(IsValide(strErr,buffer+i,len-i,4))
	{
		v = getInt4Small(buffer+i)/rate;
		 str.Format("%02x %02x %02x %02x (平电量：%0.3f)\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],v);
		strMsg = strMsg+str;
    }
	else
	{
		str.Format("(平电量长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return strMsg;
	}
	i=i+4;
	if(IsValide(strErr,buffer+i,len-i,4))
	{
		v =getInt4Small(buffer+i)/100.0;
		 str.Format("%02x %02x %02x %02x (平金额：%0.2f)\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],v);
		strMsg = strMsg+str;
    }
	else
	{
		str.Format("(平金额长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return strMsg;
	}
	i=i+4;
	
	if(IsValide(strErr,buffer+i,len-i,4))
	{
		v = getInt4Small(buffer+i)/rate;
		 str.Format("%02x %02x %02x %02x (谷电量：%0.3f)\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],v);
		strMsg = strMsg+str;
     }
	else
	{
		str.Format("(谷电量长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return strMsg;
	}
	i=i+4;
	if(IsValide(strErr,buffer+i,len-i,4))
	{
		v = getInt4Small(buffer+i)/100.0;
		 str.Format("%02x %02x %02x %02x (谷金额：%0.2f)\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],v);
		strMsg = strMsg+str;
    }
	else
	{
		str.Format("(谷金额长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return strMsg;
	}
	i=i+4;
	if(IsValide(strErr,buffer+i,len-i,4))
	{
		v = getInt4Small(buffer+i)/rate;
		 str.Format("%02x %02x %02x %02x (总电量：%0.3f)\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],v);
		strMsg = strMsg+str;
     }
	else
	{
		str.Format("(总电量长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return strMsg;
	}
	i=i+4;
	if(IsValide(strErr,buffer+i,len-i,4))
	{
		v = getInt4Small(buffer+i)/100.0;
		 str.Format("%02x %02x %02x %02x (总充电金额：%0.2f)\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],v);
		strMsg = strMsg+str;
     }
	else
	{
		str.Format("(总充电金额长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return strMsg;
	}
	i=i+4;
	if(IsValide(strErr,buffer+i,len-i,4))
	{
		v =getInt4Small(buffer+i)/100.0;
		 str.Format("%02x %02x %02x %02x (服务费：%0.2f)\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],v);
		strMsg = strMsg+str;
      }
	else
	{
		str.Format("(服务费长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return strMsg;
	}
	i=i+4;
	if(IsValide(strErr,buffer+i,len-i,4))
	{
	v = getInt4Small(buffer+i)/rate;
		 str.Format("%02x %02x %02x %02x (开始充电总电量：%0.3f)\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],v);
		strMsg = strMsg+str;
     }
	else
	{
		str.Format("(开始充电总电量长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return strMsg;
	}
	i=i+4;
	if(IsValide(strErr,buffer+i,len-i,4))
	{
		v =getInt4Small(buffer+i)/rate;
		str.Format("%02x %02x %02x %02x (结束充电总电量：%0.3f)\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],v);
		strMsg = strMsg+str;
      }
	else
	{
		str.Format("(结束充电总电量长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return strMsg;
	}
	i=i+4;
	if(IsValide(strErr,buffer+i,len-i,2))
	{
		str.Format("%02x %02x(停止充电原因：",buffer[i],buffer[i+1]);

		strMsg =strMsg+str;

		strMsg = errorMsg(strMsg,buffer[i]+buffer[i+1]*0x100);
		
	}
	else
	{
		str.Format("(停止充电原因错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return strMsg;
	}
	i=i+2;
	
	strMsg = cmdTimeMsg(strMsg,strErr,buffer+i,len-i);
	i=i+3;
    strMsg = crcMsg(strMsg,strErr,buffer+i,len-i);
	
    m_Dlg->ProDecodeShow(strMsg);
     return strMsg;
}

void	CProto_IEC104::Pro_UP_12(CString strMsg,unsigned char *buffer,int len)
{
	CString str,strErr;
	if(buffer==0 || len==0)
	{
		str="无数据内容";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	
	if(IsValide(strErr,buffer,len,8))
	{
		strMsg = getEpCode(strMsg,buffer);
	}
	else
	{
		str.Format("(终端机器编码长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	int i=8;
	if(IsValide(strErr,buffer+i,len-i,1))
	{
	str.Format("%02x(充电接口)\r\n",buffer[8]);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(充电桩接口错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+1;
	if(IsValide(strErr,buffer+i,len-i,1))
	{
		str.Format("%02x(动作结果：",buffer[9]);
		strMsg = strMsg+str;
		if(buffer[9]==0)
		{
			str.Format("成功)\r\n");
			strMsg = strMsg+str;
		}
		else
		{
			str.Format("失败)\r\n");
			strMsg = strMsg+str;
		}
	}
	else
	{
		str.Format("(动作结果错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i++;
	if(m_Dlg->getProVersion()==13)
	{
		if(IsValide(strErr,buffer+i,len-i,4))
		{
			double v =(getInt4Small(buffer+i));
			str.Format("%02x %02x %02x %02x (预充金额：%d)\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],v*0.01);
			strMsg = strMsg+str;
		}
		else
		{
			str.Format("(预充金额错误)\r\n");
			strMsg = strMsg+strErr+str;
			m_Dlg->ProDecodeShow(strMsg);
			return;
		}
		i=i+4;
	}
	strMsg = cmdTimeMsg(strMsg,strErr,buffer+i,len-i);
	i=i+3;
    strMsg = crcMsg(strMsg,strErr,buffer+i,len-i);
    	m_Dlg->ProDecodeShow(strMsg);
}
void	CProto_IEC104::Pro_UP_13(CString strMsg,unsigned char *buffer,int len)
{
	CString str,strErr;
	if(buffer==0 || len==0)
	{
		str="无数据内容";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	if(IsValide(strErr,buffer,len,8))
	{
		strMsg = getEpCode(strMsg,buffer);
	}
	else
	{
		str.Format("(终端机器编码长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	int i=8;
	if(IsValide(strErr,buffer+i,len-i,1))
	{
	
		str.Format("%02x (充电桩接口标识)\r\n",buffer[i]);
		strMsg = strMsg+str;
    }
	else
	{
		str.Format("(充电桩接口标识长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	if(IsValide(strErr,buffer+i,len-i,16))
	{
		for(int j=i;j<i+16;j++)
		{
			str.Format("%02x",buffer[j]);
			strMsg = strMsg+str;
		}
		str.Format("(交易流水号:终端机器编码16+序列号16)\r\n");
		strMsg = strMsg+str;
	}
    else
	{
		str.Format("(交易流水号长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+16;
	if(IsValide(strErr,buffer+i,len-i,1))
	{
		str.Format("%02x (交易标识：)\r\n",buffer[i]);
		strMsg = strMsg+str;
		if(buffer[i]==1)
			str.Format("CPU 成功)\r\n");
		else if(buffer[i]==2)
			str.Format("CPU 失败(灰锁))\r\n");
		else if(buffer[i]==3)
			str.Format("M1 成功)\r\n");
		else if(buffer[i]==4)
			str.Format("M1 失败(灰锁))\r\n");
		strMsg = strMsg+str;
             
    }
	else
	{
		str.Format("(交易标识长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
    i++;
	if(IsValide(strErr,buffer+i,len-i,16))
	{
		for(int j=i;j<i+16;j++)
		{
			str.Format("%02x",buffer[j]);
			strMsg = strMsg+str;
		}
		str.Format("(内卡号:芯片卡号,位数不足用0xff 补齐)\r\n");
		strMsg = strMsg+str;
	}
    else
	{
		str.Format("(内卡号长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+16;
	if(IsValide(strErr,buffer+i,len-i,7))
	{
		strMsg=timeMsg(strMsg,buffer+i,"开始时间");
    }
	else
	{
		str.Format("(开始时间长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+7;
	if(IsValide(strErr,buffer+i,len-i,7))
	{
		strMsg=timeMsg(strMsg,buffer+i,"结束时间");
     }
	else
	{
		str.Format("(结束时间年长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+7;
	
	double v=0;
	if(IsValide(strErr,buffer+i,len-i,4))
	{
		v = (getInt4Small(buffer+i))/100.0;
		 str.Format("%02x %02x %02x %02x (尖起示值：%0.2f)\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],v);
		strMsg = strMsg+str;
     }
	else
	{
		str.Format("(尖起示值长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+4;
	if(IsValide(strErr,buffer+i,len-i,4))
	{
		v = (getInt4Small(buffer+i))/100.0;
		 str.Format("%02x %02x %02x %02x (尖止示值：%0.2f)\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],v);
		strMsg = strMsg+str;
   }
	else
	{
		str.Format("(尖止示值长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+4;
	if(IsValide(strErr,buffer+i,len-i,4))
	{
		v = (getInt4Small(buffer+i))/100.0;
		 str.Format("%02x %02x %02x %02x (峰起示值：%0.2f)\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],v);
		strMsg = strMsg+str;
    }
	else
	{
		str.Format("(峰起示值长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+4;
	if(IsValide(strErr,buffer+i,len-i,4))
	{
		v = (getInt4Small(buffer+i))/100.0;
		 str.Format("%02x %02x %02x %02x (峰止示值：%0.2f)\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],v);
		strMsg = strMsg+str;
     }
	else
	{
		str.Format("(峰止示值长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+4;
	if(IsValide(strErr,buffer+i,len-i,4))
	{
		 v = (getInt4Small(buffer+i))/100.0;
		 str.Format("%02x %02x %02x %02x (平起示值：%0.2f)\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],v);
		strMsg = strMsg+str;
     }
	else
	{
		str.Format("(平起示值长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+4;
	if(IsValide(strErr,buffer+i,len-i,4))
	{
		v = (getInt4Small(buffer+i))/100.0;
		 str.Format("%02x %02x %02x %02x (平止示值：%0.2f)\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],v);
		strMsg = strMsg+str;
    }
	else
	{
		str.Format("(平止示值长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+4;
	if(IsValide(strErr,buffer+i,len-i,4))
	{
		v = (getInt4Small(buffer+i))/100.0;
		 str.Format("%02x %02x %02x %02x (谷起示值：%0.2f)\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],v);
		strMsg = strMsg+str;
    }
	else
	{
		str.Format("(谷起示值长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+4;
	if(IsValide(strErr,buffer+i,len-i,4))
	{
		 v = (getInt4Small(buffer+i))/100.0;
		 str.Format("%02x %02x %02x %02x (谷止示值：%0.2f)\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],v);
		strMsg = strMsg+str;
    }
	else
	{
		str.Format("(谷止示值长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+4;
	if(IsValide(strErr,buffer+i,len-i,4))
	{
		v = (getInt4Small(buffer+i))/100.0;
		 str.Format("%02x %02x %02x %02x (尖电量：%0.2f)\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],v);
		strMsg = strMsg+str;
    }
	else
	{
		str.Format("(尖电量长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+4;
	if(IsValide(strErr,buffer+i,len-i,4))
	{
		v = (getInt4Small(buffer+i))/100.0;
		 str.Format("%02x %02x %02x %02x (峰电量：%0.2f)\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],v);
		strMsg = strMsg+str;
    }
	else
	{
		str.Format("(峰电量长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+4;
	
	if(IsValide(strErr,buffer+i,len-i,4))
	{
		v = (getInt4Small(buffer+i))/100.0;
		 str.Format("%02x %02x %02x %02x (平电量：%0.2f)\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],v);
		strMsg = strMsg+str;
    }
	else
	{
		str.Format("(平电量长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+4;
	if(IsValide(strErr,buffer+i,len-i,4))
	{
		v = (getInt4Small(buffer+i))/100.0;
		 str.Format("%02x %02x %02x %02x (谷电量：%0.2f)\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],v);
		strMsg = strMsg+str;
     }
	else
	{
		str.Format("(谷电量长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+4;
	if(IsValide(strErr,buffer+i,len-i,4))
	{
		v = (getInt4Small(buffer+i))/100.0;
		 str.Format("%02x %02x %02x %02x (总电量：%0.2f)\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],v);
		strMsg = strMsg+str;
     }
	else
	{
		str.Format("(总电量长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+4;
	if(IsValide(strErr,buffer+i,len-i,4))
	{
		v = (getInt4Small(buffer+i))/100.0;
		 str.Format("%02x %02x %02x %02x (扣款前卡余额：%0.2f)\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],v);
		strMsg = strMsg+str;
     }
	else
	{
		str.Format("(扣款前卡余额长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+4;
	if(IsValide(strErr,buffer+i,len-i,4))
	{
		v = (getInt4Small(buffer+i))/100.0;
		 str.Format("%02x %02x %02x %02x (预冻结金额：%0.2f)\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],v);
		strMsg = strMsg+str;
     }
	else
	{
		str.Format("(预冻结金额长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+4;
	if(IsValide(strErr,buffer+i,len-i,4))
	{
		v = (getInt4Small(buffer+i))/100.0;
		 str.Format("%02x %02x %02x %02x (消费金额：%0.2f)\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],v);
		strMsg = strMsg+str;
     }
	else
	{
		str.Format("(消费金额长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+4;
	if(IsValide(strErr,buffer+i,len-i,32))
	{
		CString s,s1;
		for(int j=i;j<i+32;j++)
		{
		   str.Format("%02x ",buffer[j]);
		   strMsg = strMsg+str;
		   s1.Format("%c",buffer[j]);
		   s=s+s1;
		}
		str.Format("(电动汽车唯一标识：%s)\r\n",s);
		strMsg = strMsg+str;
		
	}
	else
	{
		str.Format("(电动汽车唯一标识长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+32;
	if(IsValide(strErr,buffer+i,len-i,6))
	{
		str.Format("%02x%02x%02x%02x%02x%02x(读卡器编号)\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],buffer[i+4],buffer[i+5]);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(读卡器编号长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+6;
	if(IsValide(strErr,buffer+i,len-i,2))
	{
		int v = (buffer[i+1]*0x100+buffer[i]);
		 str.Format("%02x %02x  (交易产生时卡中的序号：%d)\r\n",buffer[i],buffer[i+1],v);
		strMsg = strMsg+str;
     }
	else
	{
		str.Format("(交易产生时卡中的序号长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+2;
	if(IsValide(strErr,buffer+i,len-i,4))
	{
		long v = (getInt4Small(buffer+i));
		 str.Format("%02x %02x %02x %02x  (SAMTAC：%d)\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],v);
		strMsg = strMsg+str;
     }
	else
	{
		str.Format("(SAMTAC长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+4;
	if(IsValide(strErr,buffer+i,len-i,1))
	{
		 str.Format("%02x (TAC)\r\n",buffer[i]);
		strMsg = strMsg+str;
     }
	else
	{
		str.Format("(TAC长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+1;
	if(IsValide(strErr,buffer+i,len-i,1))
	{
		 str.Format("%02x (密钥版本号)\r\n",buffer[i]);
		strMsg = strMsg+str;
     }
	else
	{
		str.Format("(密钥版本号长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+1;
	if(IsValide(strErr,buffer+i,len-i,1))
	{
		 str.Format("%02x (密钥版本号)\r\n",buffer[i]);
		strMsg = strMsg+str;
     }
	else
	{
		str.Format("(密钥版本号长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+1;
    if(IsValide(strErr,buffer+i,len-i,1))
	{
		 str.Format("%02x (停止原因)\r\n",buffer[i]);
		strMsg = strMsg+str;
     }
	else
	{
		str.Format("(停止原因长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+1;
	
	strMsg = cmdTimeMsg(strMsg,strErr,buffer+i,len-i);
	i=i+3;
    strMsg = crcMsg(strMsg,strErr,buffer+i,len-i);
	
    m_Dlg->ProDecodeShow(strMsg);

}
void	CProto_IEC104::Pro_UP_14(CString strMsg,unsigned char *buffer,int len)
{
		CString str,strErr;
	if(m_Dlg->getProVersion() ==13)
	{
		str="版本错误";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}

	if(buffer==0 || len==0)
	{
		str="无数据内容";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}

	int i=0;
	
	strMsg = cmdTimeMsg(strMsg,strErr,buffer+i,len-i);
	i=i+3;
    strMsg = crcMsg(strMsg,strErr,buffer+i,len-i);
	m_Dlg->ProDecodeShow(strMsg);
}
void	CProto_IEC104::Pro_UP_15(CString strMsg,unsigned char *buffer,int len)
{
	CString str,strErr;
	if(buffer==0 || len==0)
	{
		str="无数据内容";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	if(IsValide(strErr,buffer,len,8))
	{
		strMsg = getEpCode(strMsg,buffer);
	}
	else
	{
		str.Format("(终端机器编码长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	int i=8;
	if(IsValide(strErr,buffer+i,len-i,1))
	{
		str.Format("%02x(充电接口标识)\r\n",buffer[i]);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(充电接口标识错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=9;
	if(IsValide(strErr,buffer+i,len-i,8))
	{

		for(i=9;i<17;i++)
		{
			str.Format("%02x",buffer[i]);
			strMsg = strMsg+str;
		}

		str.Format("（物理卡号)\r\n");
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(物理卡号长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}

	i=17;

	if(len != buffer[17]*12+18)
	{
		str.Format("CPU 卡的灰锁记录数据错误，数据长度不对"); 
		strMsg = strMsg+str;
		/*m_Dlg->ProDecodeShow(strMsg);
		return;*/
	}


	for(int j=0;j<buffer[17];j++)
	{
		i=18+j*12;
		if(IsValide(strErr,buffer+i,len-i,2))
		{
			str.Format("%02x %02x(用户卡脱机交易序号：%d)\r\n",buffer[i],buffer[i+1],buffer[i+1]*0x100+buffer[i]); 
			strMsg = strMsg+str;
		}
		else
		{
			str.Format("(用户卡脱机交易序号长度错误)\r\n");
			strMsg = strMsg+strErr+str;
			m_Dlg->ProDecodeShow(strMsg);
			return;
		}

		i=20+j*12;
		if(IsValide(strErr,buffer+i,len-i,6))
		{
			str.Format("%02x %02x %02x %02x %02x %02x(终端机编号：%d)\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],buffer[i+4],buffer[i+5],buffer[i+5]*0x10000000000+buffer[i+4]*0x100000000+buffer[i+3]*0x1000000+buffer[i+2]*10000+buffer[i+1]*0x100+buffer[i]); 
			strMsg = strMsg+str;
		}
		else
		{
			str.Format("(终端机编号长度错误)\r\n");
			strMsg = strMsg+strErr+str;
			m_Dlg->ProDecodeShow(strMsg);
			return;
		}

		i=26+j*12;
		if(IsValide(strErr,buffer+i,len-i,4))
		{
			str.Format("%02x %02x %02x %02x (终端机交易序列号：%d)\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],buffer[i+3]*0x1000000+buffer[i+2]*10000+buffer[i+1]*0x100+buffer[i]); 
			strMsg = strMsg+str;
		}
		else
		{
			str.Format("(端机交易序列号长度错误)\r\n");
			strMsg = strMsg+strErr+str;
			m_Dlg->ProDecodeShow(strMsg);
			return;
		}
		i=i+4;
	}
	
	strMsg = cmdTimeMsg(strMsg,strErr,buffer+i,len-i);
	i=i+3;
    strMsg = crcMsg(strMsg,strErr,buffer+i,len-i);
	m_Dlg->ProDecodeShow(strMsg);

}
void	CProto_IEC104::Pro_UP_16(CString strMsg,unsigned char *buffer,int len)
{
	CString str,strErr;
	if(buffer==0 || len==0)
	{
		str="无数据内容";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	if(IsValide(strErr,buffer,len,8))
	{
		strMsg = getEpCode(strMsg,buffer);
	}
	else
	{
		str.Format("(终端机器编码长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	int i=8;
	if(IsValide(strErr,buffer+i,len-i,1))
	{
		str.Format("%02x(充电接口标识)\r\n",buffer[i]);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(充电接口标识错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+1;
	
	strMsg = cmdTimeMsg(strMsg,strErr,buffer+i,len-i);
	i=i+3;
    strMsg = crcMsg(strMsg,strErr,buffer+i,len-i);
  
	m_Dlg->ProDecodeShow(strMsg);
}
void	CProto_IEC104::Pro_UP_17(CString strMsg,unsigned char *buffer,int len)
{
	CString str,strErr;
	if(buffer==0 || len==0)
	{
		str="无数据内容";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	m_Dlg->ProDecodeShow(strMsg);
}
void	CProto_IEC104::Pro_UP_18(CString strMsg,unsigned char *buffer,int len)
{
	CString str,strErr;
	if(buffer==0 || len==0)
	{
		str="无数据内容";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	if(IsValide(strErr,buffer,len,8))
	{
		strMsg = getEpCode(strMsg,buffer);
	}
	else
	{
		str.Format("(终端机器编码长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	int i=8;
	if(IsValide(strErr,buffer+i,len-i,16))
	{
		
		for(int j=i;j<i+16;j++)
		{
		    str.Format("%02x ",buffer[j]);
		    strMsg = strMsg+str;
		}
		str.Format("(终端机器编码的原密钥的MD5-16 数据)\r\n");
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(终端机器编码的原密钥的MD5-16 数据长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+16;
	
	strMsg = cmdTimeMsg(strMsg,strErr,buffer+i,len-i);
	i=i+3;
    strMsg = crcMsg(strMsg,strErr,buffer+i,len-i);
  
	m_Dlg->ProDecodeShow(strMsg);
}
void	CProto_IEC104::Pro_UP_19(CString strMsg,unsigned char *buffer,int len)
{
	CString str,strErr;
	if(buffer==0 || len==0)
	{
		str="无数据内容";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	if(IsValide(strErr,buffer,len,8))
	{
		strMsg = getEpCode(strMsg,buffer);
	}
	else
	{
		str.Format("(终端机器编码长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	int i=8;
	if(IsValide(strErr,buffer+i,len-i,110))
	{
		for(int j=i;j<i+110;j++)
		{
		    str.Format("%02x ",buffer[j]);
		    strMsg = strMsg+str;
		}
		str.Format("前8 位为密钥,后面为无效混淆数据)\r\n");
		 strMsg = strMsg+str;
	}
	else
	{
		str.Format("(密钥数据长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
  
	i=i+110;
	strMsg = cmdTimeMsg(strMsg,strErr,buffer+i,len-i);
	i=i+3;
    strMsg = crcMsg(strMsg,strErr,buffer+i,len-i);
	m_Dlg->ProDecodeShow(strMsg);
}
void	CProto_IEC104::Pro_UP_20(CString strMsg,unsigned char *buffer,int len)
{
	
	CString str,strErr;
	if(buffer==0 || len==0)
	{
		str="无数据内容";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	if(IsValide(strErr,buffer,len,8))
	{
		strMsg = getEpCode(strMsg,buffer);
	}
	else
	{
		str.Format("(终端机器编码长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	int i=8;
	if(IsValide(strErr,buffer+i,len-i,1))
	{

		str.Format("%02x(充电接口标识)\r\n",buffer[8]);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(充电接口标识错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
    i=9;
	if(IsValide(strErr,buffer+i,len-i,9))
	{
		for(i=9;i<18;i++)
		{
			str.Format("%02x",buffer[i]);
		}
		str.Format("(时间戳序号)\r\n");
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(时间戳序号长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}

	i=18;
	if(IsValide(strErr,buffer+i,len-i,1))
	{
		str.Format("%02x(成功标识：",buffer[i]);
		strMsg = strMsg+str;

		switch(buffer[i])
		{
		case 0:
			str.Format("成功)\r\n");
			strMsg = strMsg+str;
			break;
		case 1:
			str.Format("失败)\r\n");
			strMsg = strMsg+str;
			break;
		default:
			str.Format("未知)\r\n");
			strMsg = strMsg+str;
			break;
		}
	}
	else
	{
		str.Format("(成功标识错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+1;
	strMsg = cmdTimeMsg(strMsg,strErr,buffer+i,len-i);
	i=i+3;
    strMsg = crcMsg(strMsg,strErr,buffer+i,len-i);
	m_Dlg->ProDecodeShow(strMsg);


}
void	CProto_IEC104::Pro_UP_21(CString strMsg,unsigned char *buffer,int len)
{
	CString str,strErr;
	if(buffer==0 || len==0)
	{
		str="无数据内容";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	if(IsValide(strErr,buffer,len,8))
	{
		strMsg = getEpCode(strMsg,buffer);
	}
	else
	{
		str.Format("(终端机器编码长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	int i=8;
	if(IsValide(strErr,buffer+i,len-i,1))
	{

		str.Format("%02x(充电接口标识)\r\n",buffer[8]);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(充电接口标识错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
    i=9;
	if(IsValide(strErr,buffer+i,len-i,9))
	{
		for(i=9;i<18;i++)
		{
			str.Format("%02x",buffer[i]);
			strMsg = strMsg+str;
			
		}
	
		str.Format("(时间戳序号)\r\n");
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(时间戳序号长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}

	i=18;
	if(IsValide(strErr,buffer+i,len-i,1))
	{
		str.Format("%02x(成功标识：",buffer[i]);
		strMsg = strMsg+str;

		switch(buffer[i])
		{
		case 0:
			str.Format("成功)\r\n");
			strMsg = strMsg+str;
			break;
		case 1:
			str.Format("失败)\r\n");
			strMsg = strMsg+str;
			break;
		default:
			str.Format("未知)\r\n");
			strMsg = strMsg+str;
			break;
		}
	}
	else
	{
		str.Format("(成功标识错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}

	i=i+1;
	strMsg = cmdTimeMsg(strMsg,strErr,buffer+i,len-i);
	i=i+3;
    strMsg = crcMsg(strMsg,strErr,buffer+i,len-i);
	m_Dlg->ProDecodeShow(strMsg);
}
void	CProto_IEC104::Pro_UP_22(CString strMsg,unsigned char *buffer,int len)
{
	CString str,strErr;
	if(buffer==0 || len==0)
	{
		str="无数据内容";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	
	if(IsValide(strErr,buffer,len,8))
	{
		strMsg = getEpCode(strMsg,buffer);
	}
	else
	{
		str.Format("(终端机器编码长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	int i=8;
	if(m_Dlg->getProVersion()==13)
	{
		if(IsValide(strErr,buffer+8,len-8,1))
		{

			str.Format("%02x(枪口编号)\r\n",buffer[8]);
			strMsg = strMsg+str;
		}
		else
		{
			str.Format("(枪口编号错误)\r\n");
			strMsg = strMsg+strErr+str;
			m_Dlg->ProDecodeShow(strMsg);
			return;
		}
		i++;
	}
	strMsg = cmdTimeMsg(strMsg,strErr,buffer+i,len-i);
	i=i+3;
    strMsg = crcMsg(strMsg,strErr,buffer+i,len-i);
	
	m_Dlg->ProDecodeShow(strMsg);
}
void	CProto_IEC104::Pro_UP_23(CString strMsg,unsigned char *buffer,int len)
{
	CString str,strErr;
	if(buffer==0 || len==0)
	{
		str="无数据内容";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	
	if(IsValide(strErr,buffer,len,8))
	{
		strMsg = getEpCode(strMsg,buffer);
	}
	else
	{
		str.Format("(终端机器编码长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	int i = 8;
	if(m_Dlg->getProVersion()==13)
	{
		if(IsValide(strErr,buffer+8,len-8,1))
		{

			str.Format("%02x(枪口编号)\r\n",buffer[8]);
			strMsg = strMsg+str;
		}
		else
		{
			str.Format("(枪口编号错误)\r\n");
			strMsg = strMsg+strErr+str;
			m_Dlg->ProDecodeShow(strMsg);
			return;
		}
		i++;
	}	
	if(IsValide(strErr,buffer+i,len-i,2))
	{
		str.Format("%02x %02x(段索引：%d)",buffer[i],buffer[i+1],buffer[i+1]*0x100+buffer[i]);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(段索引长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+2;
	strMsg = cmdTimeMsg(strMsg,strErr,buffer+i,len-i);
	i=i+3;
    strMsg = crcMsg(strMsg,strErr,buffer+i,len-i);
	m_Dlg->ProDecodeShow(strMsg);
	
	
}
void	CProto_IEC104::Pro_UP_24(CString strMsg,unsigned char *buffer,int len)
{
	CString str,strErr;
	if(buffer==0 || len==0)
	{
		str="无数据内容";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	if(IsValide(strErr,buffer,len,8))
	{
		strMsg = getEpCode(strMsg,buffer);
	}
	else
	{
		str.Format("(终端机器编码长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	int i=8;
	if(IsValide(strErr,buffer+i,len-i,6))
	{
		for(i=8;i<14;i++)
		{
			str.Format("%02x",buffer[i]);
		}
		str.Format("(用户账号)\r\n");
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(用户账号长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=14;
	double v=0;
	if(IsValide(strErr,buffer+i,len-i,4))
	{
		v=getInt4Small(buffer+i)*0.01;
		str.Format("%02x %02x %02x %02x(用户余额：%0.2f)\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],v);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(用户余额长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}

	i=18;
	
	if(IsValide(strErr,buffer+i,len-i,4))
	{
		v=getInt4Small(buffer+i)*0.01;
		str.Format("%02x %02x %02x %02x(告警金额：:%0.2f)\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],v);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(告警金额长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}

	i=i+4;
	strMsg = cmdTimeMsg(strMsg,strErr,buffer+i,len-i);
	i=i+3;
    strMsg = crcMsg(strMsg,strErr,buffer+i,len-i);
	m_Dlg->ProDecodeShow(strMsg);
		
}
void	CProto_IEC104::Pro_UP_25(CString strMsg,unsigned char *buffer,int len)
{
	CString str,strErr;
	if(buffer==0 || len==0)
	{
		str="无数据内容";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	if(IsValide(strErr,buffer,len,8))
	{
		strMsg = getEpCode(strMsg,buffer);
	}
	else
	{
		str.Format("(终端机器编码长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	int i=8;
	if(IsValide(strErr,buffer+i,len-i,1))
	{

		str.Format("%02x(型号)\r\n",buffer[i]);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(型号错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=9;
	if(IsValide(strErr,buffer+i,len-i,2))
	{
		str.Format("%02x %02x(版本编号：%d)",buffer[i],buffer[i+1],getInt2(buffer+i));
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(版本编号长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+2;
	if(IsValide(strErr,buffer+i,len-i,1))
	{

		str.Format("%02x(更新标识)\r\n",buffer[i]);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(更新标识错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+1;
	strMsg = cmdTimeMsg(strMsg,strErr,buffer+i,len-i);
	i=i+3;
    strMsg = crcMsg(strMsg,strErr,buffer+i,len-i);
	m_Dlg->ProDecodeShow(strMsg);
}
void	CProto_IEC104::Pro_UP_26(CString strMsg,unsigned char *buffer,int len)
{
	CString str,strErr;
	if(buffer==0 || len==0)
	{
		str="无数据内容";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	if(IsValide(strErr,buffer,len,8))
	{
		strMsg = getEpCode(strMsg,buffer);
	}
	else
	{
		str.Format("(终端机器编码长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	int i=8;
	if(IsValide(strErr,buffer+i,len-i,2))
	{

		str.Format("%02x %02x(段索引：%d)",buffer[8],buffer[9],getInt2(buffer+i));
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(段索引长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=10;
	if(IsValide(strErr,buffer+i,len-i,1))
	{
		str.Format("%02x (型号)",buffer[10]);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(型号错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
    i=i+1;
	if(IsValide(strErr,buffer+i,len-i,2))
	{

		str.Format("%02x %02x(版本编号：%d)",buffer[i],buffer[i+1],getInt2(buffer+i));
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(版本编号长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+2;
	
	strMsg = cmdTimeMsg(strMsg,strErr,buffer+i,len-i);
	i=i+3;
    strMsg = crcMsg(strMsg,strErr,buffer+i,len-i);
	m_Dlg->ProDecodeShow(strMsg);
}
void	CProto_IEC104::Pro_UP_27(CString strMsg,unsigned char *buffer,int len)
{
	CString str,strErr;
	if(buffer==0 || len==0)
	{
		str="无数据内容";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	if(IsValide(strErr,buffer,len,8))
	{
		strMsg = getEpCode(strMsg,buffer);
	}
	else
	{
		str.Format("(终端机器编码长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	int i=8;
	if(IsValide(strErr,buffer+i,len-i,1))
	{

		str.Format("%02x (充电接口)\r\n",buffer[8]);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(充电接口错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
    i=9;
	if(IsValide(strErr,buffer+i,len-i,1))
	{

		str.Format("%02x (动作结果：",buffer[i]);
		strMsg = strMsg+str;
		if(buffer[i]==0)
		{
			str.Format("成功)\r\n");
		    strMsg = strMsg+str;
		}
		else 
		{
			str.Format("失败)\r\n");
		    strMsg = strMsg+str;
		}
	}
	else
	{
		str.Format("(动作结果错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+1;
	
	strMsg = cmdTimeMsg(strMsg,strErr,buffer+i,len-i);
	i=i+3;
    strMsg = crcMsg(strMsg,strErr,buffer+i,len-i);
		
    m_Dlg->ProDecodeShow(strMsg);

}
void	CProto_IEC104::Pro_UP_28(CString strMsg,unsigned char *buffer,int len)
{
	CString str,strErr;
	if(m_Dlg->getProVersion() <12)
	{
		str="请选择V1.2以上版本";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}

	if(buffer==0 || len==0)
	{
		str="无数据内容";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	
	if(IsValide(strErr,buffer,len,8))
	{
		strMsg = getEpCode(strMsg,buffer);
	}
	else
	{
		str.Format("(终端机器编码长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	int i=8;
	if(IsValide(strErr,buffer+i,len-i,1))
	{
		str.Format("%02x (充电接口)\r\n",buffer[8]);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(充电接口错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=9;
	unsigned int v=0;
	if(IsValide(strErr,buffer+i,len-i,4))
	{
		v=getInt4Small(buffer+i);
		str.Format("%02x %02x %02x %02x(充电总时间：%d分钟)\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],v);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(充电总时间长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}

	i=13;
	if(IsValide(strErr,buffer+i,len-i,4))
	{
		v=getInt4Small(buffer+i);
		str.Format("%02x %02x %02x %02x(总充电次数：%d)\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],v);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(总充电次数长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=17;
	if(IsValide(strErr,buffer+i,len-i,4))
	{
		double v1=getInt4Small(buffer+i)*0.01;
		str.Format("%02x %02x %02x %02x(总充电度数：%0.2f)\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],v1);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(总充电度数长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
    
	i=i+4;
	
	strMsg = cmdTimeMsg(strMsg,strErr,buffer+i,len-i);
	i=i+3;
    strMsg = crcMsg(strMsg,strErr,buffer+i,len-i);
	m_Dlg->ProDecodeShow(strMsg);
}
void	CProto_IEC104::Pro_UP_29(CString strMsg,unsigned char *buffer,int len)
{
	CString str,strErr;
	if(m_Dlg->getProVersion() <12)
	{
		str="请选择V1.2以上版本";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}

	if(buffer==0 || len==0)
	{
		str="无数据内容";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	if(IsValide(strErr,buffer,len,8))
	{
		strMsg = getEpCode(strMsg,buffer);
	}
	else
	{
		str.Format("(终端机器编码长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	int i=8;
	if(IsValide(strErr,buffer+i,len-i,1))
	{

		str.Format("%02x (枪口编号)\r\n",buffer[8]);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(枪口编号错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=9;
	if(IsValide(strErr,buffer+i,len-i,1))
	{
		str.Format("%02x (自检完成标识：",buffer[9]);
		strMsg = strMsg+str;
		if(buffer[9]==0)
		{
			str.Format("成功)\r\n");
			strMsg = strMsg+str;
		}
		else if(buffer[9]==1) 
		{
			str.Format("失败)\r\n");
			strMsg = strMsg+str;
		}
		else
		{
			str.Format("未知)\r\n");
			strMsg = strMsg+str;
		}
		m_Dlg->ProDecodeShow(strMsg);
	}
	else
	{
		str.Format("(自检完成标识错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+1;
	
	strMsg = cmdTimeMsg(strMsg,strErr,buffer+i,len-i);
	i=i+3;
    strMsg = crcMsg(strMsg,strErr,buffer+i,len-i);
	m_Dlg->ProDecodeShow(strMsg);
}
void	CProto_IEC104::Pro_UP_30(CString strMsg,unsigned char *buffer,int len)
{
	CString str,strErr;
	if(m_Dlg->getProVersion() <13)
	{
		str="请选择V1.3以上版本";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}

	if(buffer==0 || len==0)
	{
		str="无数据内容";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	if(IsValide(strErr,buffer,len,8))
	{
		strMsg = getEpCode(strMsg,buffer);
	}
	else
	{
		str.Format("(终端机器编码长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	int i=8;
	if(IsValide(strErr,buffer+i,len-i,1))
	{

		str.Format("%02x (枪口编号)\r\n",buffer[8]);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(枪口编号错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=9;
	if(IsValide(strErr,buffer+i,len-i,1))
	{
		str.Format("%02x (失败原因)",buffer[9]);
		strMsg = strMsg+str;
		/*if(buffer[9]==0)
		{
			str.Format("成功)\r\n");
			strMsg = strMsg+str;
		}
		else if(buffer[9]==1) 
		{
			str.Format("失败)\r\n");
			strMsg = strMsg+str;
		}
		else
		{
			str.Format("未知)\r\n");
			strMsg = strMsg+str;
		}*/
		m_Dlg->ProDecodeShow(strMsg);
	}
	else
	{
		str.Format("(失败原因错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+1;
	
	strMsg = cmdTimeMsg(strMsg,strErr,buffer+i,len-i);
	i=i+3;
    strMsg = crcMsg(strMsg,strErr,buffer+i,len-i);
	m_Dlg->ProDecodeShow(strMsg);
}
void	CProto_IEC104::Pro_UP_31(CString strMsg,unsigned char *buffer,int len)
{
	CString str,strErr;
	if(m_Dlg->getProVersion() <13)
	{
		str="请选择V1.3以上版本";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}

	if(buffer==0 || len==0)
	{
		str="无数据内容";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	if(IsValide(strErr,buffer,len,8))
	{
		strMsg = getEpCode(strMsg,buffer);
	}
	else
	{
		str.Format("(终端机器编码长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	int i=8;
	if(IsValide(strErr,buffer+i,len-i,1))
	{
		str.Format("%02x (枪锁：",buffer[i]);
		strMsg = strMsg+str;
		if(buffer[i]==0)
		{
			str.Format("没安装)\r\n");
			strMsg = strMsg+str;
		}
		else if(buffer[i]==1) 
		{
			str.Format("安装)\r\n");
			strMsg = strMsg+str;
		}
		else
		{
			str.Format("未知)\r\n");
			strMsg = strMsg+str;
		}
		m_Dlg->ProDecodeShow(strMsg);
	}
	else
	{
		str.Format("(枪锁错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=9;
	if(IsValide(strErr,buffer+i,len-i,1))
	{
		str.Format("%02x (枪座信号：",buffer[i]);
		strMsg = strMsg+str;
		if(buffer[i]==0)
		{
			str.Format("没安装)\r\n");
			strMsg = strMsg+str;
		}
		else if(buffer[i]==1) 
		{
			str.Format("安装)\r\n");
			strMsg = strMsg+str;
		}
		else
		{
			str.Format("未知)\r\n");
			strMsg = strMsg+str;
		}
		m_Dlg->ProDecodeShow(strMsg);
	}
	else
	{
		str.Format("(枪座信号错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=10;
	if(IsValide(strErr,buffer+i,len-i,1))
	{
		str.Format("%02x (BMS 通讯：",buffer[i]);
		strMsg = strMsg+str;
		if(buffer[i]==0)
		{
			str.Format("不支持)\r\n");
			strMsg = strMsg+str;
		}
		else if(buffer[i]==1) 
		{
			str.Format("支持)\r\n");
			strMsg = strMsg+str;
		}
		else
		{
			str.Format("未知)\r\n");
			strMsg = strMsg+str;
		}
		m_Dlg->ProDecodeShow(strMsg);
	}
	else
	{
		str.Format("(BMS 通讯错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=11;
	if(IsValide(strErr,buffer+i,len-i,1))
	{
		str.Format("%02x (地锁：",buffer[i]);
		strMsg = strMsg+str;
		if(buffer[i]==0)
		{
			str.Format("不支持)\r\n");
			strMsg = strMsg+str;
		}
		else if(buffer[i]==1) 
		{
			str.Format("支持)\r\n");
			strMsg = strMsg+str;
		}
		else
		{
			str.Format("未知)\r\n");
			strMsg = strMsg+str;
		}
		m_Dlg->ProDecodeShow(strMsg);
	}
	else
	{
		str.Format("(地锁错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=12;
	if(IsValide(strErr,buffer+i,len-i,1))
	{
		str.Format("%02x (雷达探测车位：",buffer[i]);
		strMsg = strMsg+str;
		if(buffer[i]==0)
		{
			str.Format("不支持)\r\n");
			strMsg = strMsg+str;
		}
		else if(buffer[i]==1) 
		{
			str.Format("支持)\r\n");
			strMsg = strMsg+str;
		}
		else
		{
			str.Format("未知)\r\n");
			strMsg = strMsg+str;
		}
		m_Dlg->ProDecodeShow(strMsg);
	}
	else
	{
		str.Format("(雷达探测车位错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+1;
	
	strMsg = cmdTimeMsg(strMsg,strErr,buffer+i,len-i);
	i=i+3;
    strMsg = crcMsg(strMsg,strErr,buffer+i,len-i);
	m_Dlg->ProDecodeShow(strMsg);
}
	
void	CProto_IEC104::Pro_UP_32(CString strMsg,unsigned char *buffer,int len)
{
	CString str,strErr;
	if(m_Dlg->getProVersion() <13)
	{
		str="请选择V1.3以上版本";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}

	if(buffer==0 || len==0)
	{
		str="无数据内容";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	if(IsValide(strErr,buffer,len,8))
	{
		strMsg = getEpCode(strMsg,buffer);
	}
	else
	{
		str.Format("(终端机器编码长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	int i=8;
	if(IsValide(strErr,buffer+i,len-i,1))
	{
		str.Format("%02x (外卡号）\r\n",buffer[i]);
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
	}
	else
	{
		str.Format("(外卡号错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=9;
	if(IsValide(strErr,buffer+i,len-i,1))
	{
		str.Format("%02x (内卡号）\r\n",buffer[i]);
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
	}
	else
	{
		str.Format("(内卡号错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=10;
	if(IsValide(strErr,buffer+i,len-i,1))
	{
		str.Format("%02x (交易随机数）\r\n",buffer[i]);
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
	}
	else
	{
		str.Format("(交易随机数错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=11;
	if(IsValide(strErr,buffer+i,len-i,1))
	{
		str.Format("%02x (TMAC)\r\n",buffer[i]);
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
	}
	else
	{
		str.Format("(TMAC错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=12;
	if(IsValide(strErr,buffer+i,len-i,1))
	{
		str.Format("%02x (雷达探测车位)\r\n",buffer[i]);
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
	}
	else
	{
		str.Format("(雷达探测车位错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+1;
	
	strMsg = cmdTimeMsg(strMsg,strErr,buffer+i,len-i);
	i=i+3;
    strMsg = crcMsg(strMsg,strErr,buffer+i,len-i);
	m_Dlg->ProDecodeShow(strMsg);
}
void	CProto_IEC104::Pro_UP_33(CString strMsg,unsigned char *buffer,int len)
{
	CString str,strErr;
	if(m_Dlg->getProVersion() <13)
	{
		str="请选择V1.3以上版本";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}

	if(buffer==0 || len==0)
	{
		str="无数据内容";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	if(IsValide(strErr,buffer,len,8))
	{
		strMsg = getEpCode(strMsg,buffer);
	}
	else
	{
		str.Format("(终端机器编码长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	int i=8;
	if(IsValide(strErr,buffer+i,len-i,1))
	{

		str.Format("%02x (枪口编号)\r\n",buffer[8]);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(枪口编号错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=9;
	if(IsValide(strErr,buffer+i,len-i,1))
	{
		str.Format("%02x (操作结果：",buffer[9]);
		strMsg = strMsg+str;
		if(buffer[i]==0)
		{
			str.Format("失败)\r\n");
			strMsg = strMsg+str;
		}
		else if(buffer[i]==1) 
		{
			str.Format("成功)\r\n");
			strMsg = strMsg+str;
		}
		else
		{
			str.Format("未知)\r\n");
			strMsg = strMsg+str;
		}
		m_Dlg->ProDecodeShow(strMsg);
	}
	else
	{
		str.Format("(操作结果错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=10;
	if(IsValide(strErr,buffer+i,len-i,1))
	{
		str.Format("%02x (枪锁装置状态：",buffer[i]);
		strMsg = strMsg+str;
		if(buffer[i]==0)
		{
			str.Format("未打开)\r\n");
			strMsg = strMsg+str;
		}
		else if(buffer[i]==1) 
		{
			str.Format("打开)\r\n");
			strMsg = strMsg+str;
		}
		else
		{
			str.Format("未知)\r\n");
			strMsg = strMsg+str;
		}
		m_Dlg->ProDecodeShow(strMsg);
	}
	else
	{
		str.Format("(枪锁装置状态错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+1;
	
	strMsg = cmdTimeMsg(strMsg,strErr,buffer+i,len-i);
	i=i+3;
    strMsg = crcMsg(strMsg,strErr,buffer+i,len-i);
	m_Dlg->ProDecodeShow(strMsg);
}


void	CProto_IEC104::Pro_UP_101(CString strMsg,unsigned char *buffer,int len)
{
	CString str,strErr;
	if(m_Dlg->getProVersion() <20)
	{
		str="请选择V2.0以上版本";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}

	if(buffer==0 || len==0)
	{
		str="无数据内容";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	if(IsValide(strErr,buffer,len,8))
	{
		strMsg = getEpCode(strMsg,buffer);
	}
	else
	{
		str.Format("(终端机器编码长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	int i=8;
	if(IsValide(strErr,buffer+i,len-i,1))
	{

		str.Format("%02x ( 枪数量)\r\n",buffer[i]);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(枪数量错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	
	i++;
	int num = buffer[8];
	for(int i=0;i<num;i++)
	{
		if(IsValide(strErr,buffer+i,len-i,1))
		{
			str.Format("%02x (枪锁：",buffer[i]);
			strMsg = strMsg+str;
			if(buffer[i]==0)
			{
				str.Format("没安装)\r\n");
				strMsg = strMsg+str;
			}
			else if(buffer[i]==1) 
			{
				str.Format("安装)\r\n");
				strMsg = strMsg+str;
			}
			else
			{
				str.Format("未知)\r\n");
				strMsg = strMsg+str;
			}
			m_Dlg->ProDecodeShow(strMsg);
		}
		else
		{
			str.Format("(枪锁错误)\r\n");
			strMsg = strMsg+strErr+str;
			m_Dlg->ProDecodeShow(strMsg);
			return;
		}
		i++;
		if(IsValide(strErr,buffer+i,len-i,1))
		{
			str.Format("%02x (枪座信号：",buffer[i]);
			strMsg = strMsg+str;
			if(buffer[i]==0)
			{
				str.Format("没安装)\r\n");
				strMsg = strMsg+str;
			}
			else if(buffer[i]==1) 
			{
				str.Format("安装)\r\n");
				strMsg = strMsg+str;
			}
			else
			{
				str.Format("未知)\r\n");
				strMsg = strMsg+str;
			}
			m_Dlg->ProDecodeShow(strMsg);
		}
		else
		{
			str.Format("(枪座信号错误)\r\n");
			strMsg = strMsg+strErr+str;
			m_Dlg->ProDecodeShow(strMsg);
			return;
		}
		i++;
		if(IsValide(strErr,buffer+i,len-i,1))
		{
			str.Format("%02x (BMS通讯：",buffer[i]);
			strMsg = strMsg+str;
			if(buffer[i]==0)
			{
				str.Format("没支持)\r\n");
				strMsg = strMsg+str;
			}
			else if(buffer[i]==1) 
			{
				str.Format("支持)\r\n");
				strMsg = strMsg+str;
			}
			else
			{
				str.Format("未知)\r\n");
				strMsg = strMsg+str;
			}
			m_Dlg->ProDecodeShow(strMsg);
		}
		else
		{
			str.Format("(BMS通讯错误)\r\n");
			strMsg = strMsg+strErr+str;
			m_Dlg->ProDecodeShow(strMsg);
			return;
		}
		i++;
		if(IsValide(strErr,buffer+i,len-i,1))
		{
			str.Format("%02x (车位检查：",buffer[i]);
			strMsg = strMsg+str;
			if(buffer[i]==0)
			{
				str.Format("没支持)\r\n");
				strMsg = strMsg+str;
			}
			else if(buffer[i]==1) 
			{
				str.Format("支持)\r\n");
				strMsg = strMsg+str;
			}
			else
			{
				str.Format("未知)\r\n");
				strMsg = strMsg+str;
			}
			m_Dlg->ProDecodeShow(strMsg);
		}
		else
		{
			str.Format("(车位检查错误)\r\n");
			strMsg = strMsg+strErr+str;
			m_Dlg->ProDecodeShow(strMsg);
			return;
		}
		i++;
		if(IsValide(strErr,buffer+i,len-i,1))
		{
			str.Format("%02x (LED灯：",buffer[i]);
			strMsg = strMsg+str;
			if(buffer[i]==0)
			{
				str.Format("没支持)\r\n");
				strMsg = strMsg+str;
			}
			else if(buffer[i]==1) 
			{
				str.Format("支持)\r\n");
				strMsg = strMsg+str;
			}
			else
			{
				str.Format("未知)\r\n");
				strMsg = strMsg+str;
			}
			m_Dlg->ProDecodeShow(strMsg);
		}
		else
		{
			str.Format("(LED灯错误)\r\n");
			strMsg = strMsg+strErr+str;
			m_Dlg->ProDecodeShow(strMsg);
			return;
		}

		i++;
	}
	
	
	strMsg = cmdTimeMsg(strMsg,strErr,buffer+i,len-i);
	i=i+3;
    strMsg = crcMsg(strMsg,strErr,buffer+i,len-i);
	m_Dlg->ProDecodeShow(strMsg);
}


void	CProto_IEC104::Pro_UP_104(CString strMsg,unsigned char *buffer,int len)
{
	CString str,strErr;
	if(m_Dlg->getProVersion() <20)
	{
		str="请选择V2.0以上版本";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}

	if(buffer==0 || len==0)
	{
		str="无数据内容";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	if(IsValide(strErr,buffer,len,8))
	{
		strMsg = getEpCode(strMsg,buffer);
	}
	else
	{
		str.Format("(终端机器编码长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	int i=8;
	if(IsValide(strErr,buffer+i,len-i,1))
	{

		str.Format("%02x ( 枪口编号)\r\n",buffer[i]);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(枪口编号错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i++;
	if(IsValide(strErr,buffer+i,len-i,1))
	{
		str.Format("%02x (操作结果：",buffer[i]);
		strMsg = strMsg+str;
		if(buffer[i]==0)
		{
			str.Format("失败)\r\n");
			strMsg = strMsg+str;
		}
		else if(buffer[i]==1) 
		{
			str.Format("成功)\r\n");
			strMsg = strMsg+str;
		}
		else
		{
			str.Format("未知)\r\n");
			strMsg = strMsg+str;
		}
		m_Dlg->ProDecodeShow(strMsg);
	}
	else
	{
		str.Format("(操作结果错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i++;
	if(IsValide(strErr,buffer+i,len-i,1))
	{
		str.Format("%02x (枪锁装置状态：",buffer[i]);
		strMsg = strMsg+str;
		if(buffer[i]==0)
		{
			str.Format("未打开)\r\n");
			strMsg = strMsg+str;
		}
		else if(buffer[i]==1) 
		{
			str.Format("打开)\r\n");
			strMsg = strMsg+str;
		}
		else
		{
			str.Format("未知)\r\n");
			strMsg = strMsg+str;
		}
		m_Dlg->ProDecodeShow(strMsg);
	}
	else
	{
		str.Format("(枪锁装置状态错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+1;
	strMsg = cmdTimeMsg(strMsg,strErr,buffer+i,len-i);
	i=i+3;
	strMsg = crcMsg(strMsg,strErr,buffer+i,len-i);

	m_Dlg->ProDecodeShow(strMsg);
}


void	CProto_IEC104::Pro_UP_105(CString strMsg,unsigned char *buffer,int len)
{
	CString str,strErr;
	if(m_Dlg->getProVersion() <20)
	{
		str="请选择V2.0以上版本";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}

	if(buffer==0 || len==0)
	{
		str="无数据内容";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	if(IsValide(strErr,buffer,len,8))
	{
		strMsg = getEpCode(strMsg,buffer);
	}
	else
	{
		str.Format("(终端机器编码长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	int i=8;
	if(IsValide(strErr,buffer+i,len-i,1))
	{

		str.Format("%02x ( 枪口编号)\r\n",buffer[i]);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(枪口编号错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i++;
	if(IsValide(strErr,buffer+i,len-i,1))
	{
		str.Format("%02x (失败原因：",buffer[i]);
		strMsg = strMsg+str;
		if(buffer[i]==0)
		{
			str.Format("失败)\r\n");
			strMsg = strMsg+str;
		}
		else if(buffer[i]==1) 
		{
			str.Format("成功)\r\n");
			strMsg = strMsg+str;
		}
		else
		{
			str.Format("未知)\r\n");
			strMsg = strMsg+str;
		}
		m_Dlg->ProDecodeShow(strMsg);
	}
	else
	{
		str.Format("(失败原因错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}

	i=i+1;
	strMsg = cmdTimeMsg(strMsg,strErr,buffer+i,len-i);
	i=i+3;
	strMsg = crcMsg(strMsg,strErr,buffer+i,len-i);

	m_Dlg->ProDecodeShow(strMsg);
}

//解释交流实时打包数据
void	CProto_IEC104::AC_RxChangeProc(CString strMsg,unsigned char * buffer,int len)
{
	CString str,strErr;
	
	if(buffer==0 || len==0)
	{
		str="无数据内容";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return ;
	}
	
	if(IsValide(strErr,buffer,len,8))
	{
		strMsg = getEpCode(strMsg,buffer);
	}
	else
	{
		str.Format("(终端机器编码长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	int i=8;
	if(IsValide(strErr,buffer+i,len-i,1))
	{

		str.Format("%02x (枪口编号)\r\n",buffer[8]);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(枪口编号错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	 i=9;
	if(IsValide(strErr,buffer+i,len-i,1))
	{

		str.Format("%02x (变化数据量)\r\n",buffer[9]);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(变化数据量错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i++;
	for(int j=0;j<buffer[9];j++)
	{	
		int index = buffer[i];
		i++;
		switch(index)
		{
		case 3:
			{
				if(IsValide(strErr,buffer+i,len-i,1))
				{
					str.Format("%02x (车与桩连接确认开关状态：",buffer[i]);
					strMsg = strMsg+str;
					if(buffer[i]==0)
					{
						str.Format("关)\r\n");
						strMsg = strMsg+str;
					}
					else if(buffer[i]==1)
					{
						str.Format("开)\r\n");
						strMsg = strMsg+str;
					}
					else
					{
						str.Format(")\r\n");
						strMsg = strMsg+str;
					}
				}
				else
				{
					str.Format("(车与桩连接确认开关状态错误)\r\n");
					strMsg = strMsg+strErr+str;
					m_Dlg->ProDecodeShow(strMsg);
					return;
				}
				i++;
			}
			break;
		case 4:
			{
				if(IsValide(strErr,buffer+i,len-i,1))
				{
					str.Format("%02x (工作状态：",buffer[i]);
					strMsg = strMsg+str;
					if(buffer[i]==0)
					{
						str.Format("离线)\r\n");
						strMsg = strMsg+str;
					}
					else if(buffer[i]==1)
					{
						str.Format("故障)\r\n");
						strMsg = strMsg+str;
					}
					else if(buffer[i]==2)
					{
						str.Format("待机)\r\n");
						strMsg = strMsg+str;
					}
					else if(buffer[i]==3)
					{
						str.Format("工作)\r\n");
						strMsg = strMsg+str;
					}
					else if(buffer[i]==4)
					{
						str.Format("欠压故障)\r\n");
						strMsg = strMsg+str;
					}
					else if(buffer[i]==5)
					{
						str.Format("过压故障)\r\n");
						strMsg = strMsg+str;
					}
					else if(buffer[i]==6)
					{
						str.Format("过电流故障)\r\n");
						strMsg = strMsg+str;
					}
					else
					{
						str.Format(")\r\n");
						strMsg = strMsg+str;
					}
				}
				else
				{
					str.Format("(工作状态错误)\r\n");
					strMsg = strMsg+strErr+str;
					m_Dlg->ProDecodeShow(strMsg);
					return;
				}
				i++;
			}
			break;
		case 5:
			{
				if(IsValide(strErr,buffer+i,len-i,1))
				{
					str.Format("%02x (枪座状态：",buffer[i]);
					strMsg = strMsg+str;
					if(buffer[i]==0)
					{
						str.Format("未收枪)\r\n");
						strMsg = strMsg+str;
					}
					else if(buffer[i]==1)
					{
						str.Format("收枪)\r\n");
						strMsg = strMsg+str;
					}
					else
					{
						str.Format(")\r\n");
						strMsg = strMsg+str;
					}
				}
				else
				{
					str.Format("(枪座状态错误)\r\n");
					strMsg = strMsg+strErr+str;
					m_Dlg->ProDecodeShow(strMsg);
					return;
				}
				i++;
			}
			break;
		case 6:
			{
				if(IsValide(strErr,buffer+i,len-i,1))
				{
					str.Format("%02x (充电枪盖状态：",buffer[i]);
					strMsg = strMsg+str;
					if(buffer[i]==0)
					{
						str.Format("开)\r\n");
						strMsg = strMsg+str;
					}
					else if(buffer[i]==1)
					{
						str.Format("关闭)\r\n");
						strMsg = strMsg+str;
					}
					else
					{
						str.Format(")\r\n");
						strMsg = strMsg+str;
					}
				}
				else
				{
					str.Format("(充电枪盖状态错误)\r\n");
					strMsg = strMsg+strErr+str;
					m_Dlg->ProDecodeShow(strMsg);
					return;
				}
				i++;
			}
			break;
		case 7:
			{
				if(IsValide(strErr,buffer+i,len-i,1))
				{
					str.Format("%02x (车与桩建立通信信号：",buffer[i]);
					strMsg = strMsg+str;
					if(buffer[i]==0)
					{
						str.Format("未建立通讯)\r\n");
						strMsg = strMsg+str;
					}
					else if(buffer[i]==1)
					{
						str.Format("建立通讯)\r\n");
						strMsg = strMsg+str;
					}
					else
					{
						str.Format(")\r\n");
						strMsg = strMsg+str;
					}
				}
				else
				{
					str.Format("(车与桩建立通信信号错误)\r\n");
					strMsg = strMsg+strErr+str;
					m_Dlg->ProDecodeShow(strMsg);
					return;
				}
				i++;
			}
			break;
		case 8:
			{
				if(IsValide(strErr,buffer+i,len-i,1))
				{
					str.Format("%02x (交流输入过压告警：",buffer[i]);
					strMsg = strMsg+str;
					if(buffer[i]==0)
					{
						str.Format("不过压)\r\n");
						strMsg = strMsg+str;
					}
					else if(buffer[i]==1)
					{
						str.Format("过压)\r\n");
						strMsg = strMsg+str;
					}
					else
					{
						str.Format(")\r\n");
						strMsg = strMsg+str;
					}
				}
				else
				{
					str.Format("(交流输入过压告警错误)\r\n");
					strMsg = strMsg+strErr+str;
					m_Dlg->ProDecodeShow(strMsg);
					return;
				}
				i++;
			}
			break;
		case 9:
			{
				if(IsValide(strErr,buffer+i,len-i,1))
				{
					str.Format("%02x (交流输入欠压告警：",buffer[i]);
					strMsg = strMsg+str;
					if(buffer[i]==0)
					{
						str.Format("不欠压)\r\n");
						strMsg = strMsg+str;
					}
					else if(buffer[i]==1)
					{
						str.Format("欠压)\r\n");
						strMsg = strMsg+str;
					}
					else
					{
						str.Format(")\r\n");
						strMsg = strMsg+str;
					}
				}
				else
				{
					str.Format("(交流输入欠压告警错误)\r\n");
					strMsg = strMsg+strErr+str;
					m_Dlg->ProDecodeShow(strMsg);
					return;
				}
				i++;
			}
			break;
		case 10:
			{
				if(IsValide(strErr,buffer+i,len-i,1))
				{
					str.Format("%02x (交流电流过负荷告警：",buffer[i]);
					strMsg = strMsg+str;
					if(buffer[i]==0)
					{
						str.Format("不过负荷)\r\n");
						strMsg = strMsg+str;
					}
					else if(buffer[i]==1)
					{
						str.Format("过负荷)\r\n");
						strMsg = strMsg+str;
					}
					else
					{
						str.Format(")\r\n");
						strMsg = strMsg+str;
					}
				}
				else
				{
					str.Format("(交流电流过负荷告警错误)\r\n");
					strMsg = strMsg+strErr+str;
					m_Dlg->ProDecodeShow(strMsg);
					return;
				}
				i++;
			}
			break;
		case 11:
			{
				if(IsValide(strErr,buffer+i,len-i,2))
				{
					int v = getInt2(buffer+i);;
					str.Format("%02x %02x (充电输出电压%0.1f)\r\n",buffer[i],buffer[i+1],v*0.1);
					strMsg = strMsg+str;
				}
				else
				{
					str.Format("(充电输出电压长度错误)\r\n");
					strMsg = strMsg+strErr+str;
					m_Dlg->ProDecodeShow(strMsg);
					return;
				}
				i=i+2;
			}
			break;
		case 12:
			{
				double v=0;
				if(IsValide(strErr,buffer+i,len-i,2))
				{
					v = getInt2(buffer+i);;
					str.Format("%02x %02x (充电输出电流%0.2f)\r\n",buffer[i],buffer[i+1],v*0.01);
					strMsg = strMsg+str;
				}
				else
				{
					str.Format("(充电输出电流长度错误)\r\n");
					strMsg = strMsg+strErr+str;
					m_Dlg->ProDecodeShow(strMsg);
					return;
				}
				i=i+2;
			}
			break;
		case 13:
			{
				if(IsValide(strErr,buffer+i,len-i,1))
				{
					str.Format("%02x (输出继电器状态：",buffer[i]);
					strMsg = strMsg+str;
					if(buffer[i]==0)
					{
						str.Format("关)\r\n");
						strMsg = strMsg+str;
					}
					else if(buffer[i]==1)
					{
						str.Format("开)\r\n");
						strMsg = strMsg+str;
					}
					else
					{
						str.Format(")\r\n");
						strMsg = strMsg+str;
					}
				}
				else
				{
					str.Format("(输出继电器状态错误)\r\n");
					strMsg = strMsg+strErr+str;
					m_Dlg->ProDecodeShow(strMsg);
					return;
				}
				i++;
			}
			break;
		case 14:
			{
				if(IsValide(strErr,buffer+i,len-i,4))
				{
					double v =getInt4Small(buffer+i);
					str.Format("%02x %02x %02x %02x (有功总电度：%0.2f)\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],v*0.01);
					strMsg = strMsg+str;
				}
				else
				{
					str.Format("(有功总电度长度错误)\r\n");
					strMsg = strMsg+strErr+str;
					m_Dlg->ProDecodeShow(strMsg);
					return;
				}
                i=i+4;			  
			}
             break;
		case 15:
			{
				if(IsValide(strErr,buffer+i,len-i,2))
				{
					unsigned short  v1 =getInt2(buffer+i);;
					str.Format("%02x %02x (累计充电时间分：%d)\r\n",buffer[i],buffer[i+1],v1);
					strMsg = strMsg+str;
				}
				else
				{
					str.Format("(累计充电时间分长度错误)\r\n");
					strMsg = strMsg+strErr+str;
					m_Dlg->ProDecodeShow(strMsg);
					return;
				}
				i=i+2;
			}
			break;
		case 16:
			{
				if(IsValide(strErr,buffer+i,len-i,1))
				{
					str.Format("%02x (车位占用状态)",buffer[i]);
					strMsg = strMsg+str;
					m_Dlg->ProDecodeShow(strMsg);
				}
				else
				{
					str.Format("(车位占用状态错误)\r\n");
					strMsg = strMsg+strErr+str;
					m_Dlg->ProDecodeShow(strMsg);
					return;
				}
				i++;
			}
			break;
		default:
			break;

			}
	}
	strMsg = cmdTimeMsg(strMsg,strErr,buffer+i,len-i);
	i=i+3;
	strMsg = crcMsg(strMsg,strErr,buffer+i,len-i);
	m_Dlg->ProDecodeShow(strMsg);
}

CString	CProto_IEC104::workStatusMsg(CString strMsg,unsigned char *buffer)
{
	int i=0;
	CString str;
	str.Format("%02x (工作状态：",buffer[i]);
	strMsg = strMsg+str;
	if(buffer[i]==0)
	{
		str.Format("离线)\r\n");
		strMsg = strMsg+str;
	}
	else if(buffer[i]==1)
	{
		str.Format("故障)\r\n");
		strMsg = strMsg+str;
	}
	else if(buffer[i]==2)
	{
		str.Format("待机)\r\n");
		strMsg = strMsg+str;
	}
	else if(buffer[i]==3)
	{
		str.Format("工作)\r\n");
		strMsg = strMsg+str;
	}
	else if(buffer[i]==4)
	{
		str.Format("欠压故障)\r\n");
		strMsg = strMsg+str;
	}
	else if(buffer[i]==5)
	{
		str.Format("过压故障)\r\n");
		strMsg = strMsg+str;
	}
	else if(buffer[i]==6)
	{
		str.Format("过电流故障)\r\n");
		strMsg = strMsg+str;
	}
	else if(buffer[i]==8)
	{
		str.Format("预约)\r\n");
		strMsg = strMsg+str;
	}
	else if(buffer[i]==9)
	{
		str.Format("在线升级)\r\n");
		strMsg = strMsg+str;
	}
	else if(buffer[i]==10)
	{
		str.Format("操作中)\r\n");
		strMsg = strMsg+str;
	}
	else if(buffer[i]==11)
	{
		str.Format("设置状态)\r\n");
		strMsg = strMsg+str;
	}
	else if(buffer[i]==12)
	{
		str.Format("充电模式选择)\r\n");
		strMsg = strMsg+str;
	}
	else if(buffer[i]==31)
	{
		str.Format("欠压故障)\r\n");
		strMsg = strMsg+str;
	}
	else if(buffer[i]==32)
	{
		str.Format("过压故障)\r\n");
		strMsg = strMsg+str;
	}
	else if(buffer[i]==33)
	{
		str.Format("过电流故障)\r\n");
		strMsg = strMsg+str;
	}
	else if(buffer[i]==34)
	{
		str.Format("防雷器故障)\r\n");
		strMsg = strMsg+str;
	}
	else if(buffer[i]==35)
	{
		str.Format("电表故障)\r\n");
		strMsg = strMsg+str;
	}
	else
	{
		str.Format(")\r\n");
		strMsg = strMsg+str;
	}
	return strMsg;

}

CString    CProto_IEC104::lockStatusMsg(CString strMsg,unsigned char *buffer)
{
	int i=0;
	CString str;
	str.Format("%02x (地锁状态：",buffer[i]);
	strMsg = strMsg+str;
	switch(buffer[i])
	{
	case 0:
		str.Format("地锁硬件无法找到)\r\n");
		strMsg = strMsg+str;
		break;
	case 1:
		str.Format("降下)\r\n");
		strMsg = strMsg+str;
		break;
	case 2:
		str.Format("升起)\r\n");
		strMsg = strMsg+str;
		break;
	case 3:
		str.Format("运动中)\r\n");
		strMsg = strMsg+str;
		break;
	case 4:
		str.Format("故障)\r\n");
		strMsg = strMsg+str;
		break;
	default:
		str.Format("未知)\r\n");
		strMsg = strMsg+str;
		break;
	}
	return strMsg;
}


void	CProto_IEC104::AC_Proc1(CString strMsg,unsigned char * buffer,int len)
{
	CString str,strErr;

	int i=0;
	if(IsValide(strErr,buffer+i,len-i,8))
	{
		strMsg = getEpCode(strMsg,buffer+i);
	}
	else
	{
		str.Format("(终端机器编码长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+8;
	if(IsValide(strErr,buffer+i,len-i,1))
	{
		str.Format("%02x (充电接口标识)\r\n",buffer[i]);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(充电接口标识错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i++;
	if(IsValide(strErr,buffer+i,len-i,1))
	{
		strMsg =linkCarMsg(strMsg,buffer+i);
	}
	else
	{
		str.Format("(车与桩连接确认开关状态错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i++;
	if(IsValide(strErr,buffer+i,len-i,1))
	{
		strMsg=workStatusMsg(strMsg,buffer+i);
	}
	else
	{
		str.Format("(工作状态错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i++;
	if(IsValide(strErr,buffer+i,len-i,1))
	{
		str.Format("%02x (枪座状态：",buffer[i]);
		strMsg = strMsg+str;
		if(buffer[i]==0)
		{
			str.Format("未收枪)\r\n");
			strMsg = strMsg+str;
		}
		else if(buffer[i]==1)
		{
			str.Format("收枪)\r\n");
			strMsg = strMsg+str;
		}
		else
		{
			str.Format(")\r\n");
			strMsg = strMsg+str;
		}
	}
	else
	{
		str.Format("(枪座状态错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i++;
	if(IsValide(strErr,buffer+i,len-i,1))
	{
		str.Format("%02x (充电枪盖状态：",buffer[i]);
		strMsg = strMsg+str;
		if(buffer[i]==0)
		{
			str.Format("开)\r\n");
			strMsg = strMsg+str;
		}
		else if(buffer[i]==1)
		{
			str.Format("关闭)\r\n");
			strMsg = strMsg+str;
		}
		else
		{
			str.Format(")\r\n");
			strMsg = strMsg+str;
		}
	}
	else
	{
		str.Format("(充电枪盖状态错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i++;
	if(IsValide(strErr,buffer+i,len-i,1))
	{
		str.Format("%02x (车与桩建立通信信号：",buffer[i]);
		strMsg = strMsg+str;
		if(buffer[i]==0)
		{
			str.Format("未建立通讯)\r\n");
			strMsg = strMsg+str;
		}
		else if(buffer[i]==1)
		{
			str.Format("建立通讯)\r\n");
			strMsg = strMsg+str;
		}
		else
		{
			str.Format(")\r\n");
			strMsg = strMsg+str;
		}
	}
	else
	{
		str.Format("(车与桩建立通信信号错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i++;
	if(IsValide(strErr,buffer+i,len-i,1))
	{
		str.Format("%02x (交流输入过压告警：",buffer[i]);
		strMsg = strMsg+str;
		if(buffer[i]==0)
		{
			str.Format("不过压)\r\n");
			strMsg = strMsg+str;
		}
		else if(buffer[i]==1)
		{
			str.Format("过压)\r\n");
			strMsg = strMsg+str;
		}
		else
		{
			str.Format(")\r\n");
			strMsg = strMsg+str;
		}
	}
	else
	{
		str.Format("(交流输入过压告警错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i++;
	if(IsValide(strErr,buffer+i,len-i,1))
	{
		str.Format("%02x (交流输入欠压告警：",buffer[i]);
		strMsg = strMsg+str;
		if(buffer[i]==0)
		{
			str.Format("不欠压)\r\n");
			strMsg = strMsg+str;
		}
		else if(buffer[i]==1)
		{
			str.Format("欠压)\r\n");
			strMsg = strMsg+str;
		}
		else
		{
			str.Format(")\r\n");
			strMsg = strMsg+str;
		}
	}
	else
	{
		str.Format("(交流输入欠压告警错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}

	i++;
	if(IsValide(strErr,buffer+i,len-i,1))
	{
		str.Format("%02x (交流电流过负荷告警：",buffer[i]);
		strMsg = strMsg+str;
		if(buffer[i]==0)
		{
			str.Format("不过负荷)\r\n");
			strMsg = strMsg+str;
		}
		else if(buffer[i]==1)
		{
			str.Format("过负荷)\r\n");
			strMsg = strMsg+str;
		}
		else
		{
			str.Format(")\r\n");
			strMsg = strMsg+str;
		}
	}
	else
	{
		str.Format("(交流电流过负荷告警错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i++;
	if(IsValide(strErr,buffer+i,len-i,2))
	{
		int v = getInt2(buffer+i);
		str.Format("%02x %02x (充电输出电压%0.1f)\r\n",buffer[i],buffer[i+1],v*0.1);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(充电输出电压长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+2;
	double v=0;
	if(IsValide(strErr,buffer+i,len-i,2))
	{
		v = getInt2(buffer+i);
		str.Format("%02x %02x (充电输出电流%0.2f)\r\n",buffer[i],buffer[i+1],v*0.01);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(充电输出电流长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+2;
	if(IsValide(strErr,buffer+i,len-i,1))
	{
		str.Format("%02x (输出继电器状态：",buffer[i]);
		strMsg = strMsg+str;
		if(buffer[i]==0)
		{
			str.Format("关)\r\n");
			strMsg = strMsg+str;
		}
		else if(buffer[i]==1)
		{
			str.Format("开)\r\n");
			strMsg = strMsg+str;
		}
		else
		{
			str.Format(")\r\n");
			strMsg = strMsg+str;
		}
	}
	else
	{
		str.Format("(输出继电器状态错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i++;
	if(IsValide(strErr,buffer+i,len-i,4))
	{
		v = getInt4Small(buffer+i);
		str.Format("%02x %02x %02x %02x (有功总电度：%0.3f)\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],v*0.001);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(有功总电度长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+4;
	if(IsValide(strErr,buffer+i,len-i,2))
	{
		unsigned short  v1 =getInt2(buffer+i);
		str.Format("%02x %02x (累计充电时间分：%d)\r\n",buffer[i],buffer[i+1],v1);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(累计充电时间分长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+2;
	if(m_Dlg->getProVersion() >= 13)
	{ 
		if(IsValide(strErr,buffer+i,len-i,1))
		{
			str.Format("%02x (车位占用状态：",buffer[i]);
			strMsg = strMsg+str;
	    	if(buffer[i]==0)
		    {
			   str.Format("关)\r\n");
			   strMsg = strMsg+str;
		    }
		    else if(buffer[i]==1)
		    {
			   str.Format("开)\r\n");
			   strMsg = strMsg+str;
	    	}
		    else
		    {
		 	    str.Format("未知)\r\n");
			    strMsg = strMsg+str;
		     }

		}
		else
		{
			str.Format("(车位占用状态错误)\r\n");
			strMsg = strMsg+strErr+str;
			m_Dlg->ProDecodeShow(strMsg);
			return;
		}
		i=i+1;

		if(IsValide(strErr,buffer+i,len-i,4))
		{
			double	v =getInt4Small(buffer+i);
			str.Format("%02x %02x %02x %02x (已充金额：%0.2f)\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],v*0.01);
			strMsg = strMsg+str;
		}
		else
		{
			str.Format("(已充金额错误)\r\n");
			strMsg = strMsg+strErr+str;
			m_Dlg->ProDecodeShow(strMsg);
			return;
		}
		i=i+4;
		if(IsValide(strErr,buffer+i,len-i,4))
		{
			double	v =getInt4Small(buffer+i);
			str.Format("%02x %02x %02x %02x (电价：%0.2f)\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],v*0.01);
			strMsg = strMsg+str;
		}
		else
		{
			str.Format("(电价错误)\r\n");
			strMsg = strMsg+strErr+str;
			m_Dlg->ProDecodeShow(strMsg);
			return;
		}
		i=i+4;
		if(IsValide(strErr,buffer+i,len-i,4))
		{
			double	v =getInt4Small(buffer+i);
			str.Format("%02x %02x %02x %02x (已充总度数：%0.3f)\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],v*0.001);
			strMsg = strMsg+str;
		}
		else
		{
			str.Format("(已充总度数错误)\r\n");
			strMsg = strMsg+strErr+str;
			m_Dlg->ProDecodeShow(strMsg);
			return;
		}
		i=i+4;
		if(IsValide(strErr,buffer+i,len-i,1))
		{
			strMsg=lockStatusMsg(strMsg,buffer+i);
		}
		else
		{
			str.Format("(地锁状态错误)\r\n");
			strMsg = strMsg+strErr+str;
			m_Dlg->ProDecodeShow(strMsg);
			return;
		}
		i=i+1;
	}

	strMsg = cmdTimeMsg(strMsg,strErr,buffer+i,len-i);
	i=i+3;
	strMsg = crcMsg(strMsg,strErr,buffer+i,len-i);

	m_Dlg->ProDecodeShow(strMsg);
}
CString  CProto_IEC104::  linkCarMsg(CString strMsg,unsigned char *buffer)
{
	int i=0;
	CString str;
	str.Format("%02x (车与桩连接确认开关状态：",buffer[i]);
	strMsg = strMsg+str;
	if(buffer[i]==0)
	{
		str.Format("关)\r\n");
		strMsg = strMsg+str;
	}
	else if(buffer[i]==1)
	{
		str.Format("开)\r\n");
		strMsg = strMsg+str;
	}
	else
	{
		str.Format(")\r\n");
		strMsg = strMsg+str;
	}
	return strMsg;
}
CString  CProto_IEC104:: linkBatteryMsg(CString strMsg,unsigned char *buffer)
{
	int i=0;
	CString str;
	str.Format("%02x (是否连接电池：",buffer[i]);
	strMsg = strMsg+str;
	if(buffer[i]==0)
	{
		str.Format("未连接)\r\n");
		strMsg = strMsg+str;
	}
	else if(buffer[i]==1)
	{
		str.Format("连接)\r\n");
		strMsg = strMsg+str;
	}
	else
	{
		str.Format(")\r\n");
		strMsg = strMsg+str;
	}
	return strMsg;
}
void	 CProto_IEC104::AC_Proc3(CString strMsg,unsigned char * buffer,int len)
{

	CString str,strErr;

	int i=0;
	if(IsValide(strErr,buffer+i,len-i,8))
	{
		strMsg = getEpCode(strMsg,buffer+i);
	}
	else
	{
		str.Format("(终端机器编码长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+8;
	if(IsValide(strErr,buffer+i,len-i,1))
	{
		str.Format("%02x (充电接口标识)\r\n",buffer[i]);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(充电接口标识错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i++;
	if(IsValide(strErr,buffer+i,len-i,2))
	{
		int v = getInt2(buffer+i);
		str.Format("%02x %02x (充电输出电压%0.1f)\r\n",buffer[i],buffer[i+1],v*0.1);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(充电输出电压长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+2;
	double v=0;
	if(IsValide(strErr,buffer+i,len-i,2))
	{
		v = getInt2(buffer+i);
		str.Format("%02x %02x (充电输出电流%0.2f)\r\n",buffer[i],buffer[i+1],v*0.01);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(充电输出电流长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+2;
	if(IsValide(strErr,buffer+i,len-i,1))
	{
		strMsg=workStatusMsg(strMsg,buffer+i);
	}
	else
	{
		str.Format("(工作状态错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i++;
	if(IsValide(strErr,buffer+i,len-i,1))
	{
		strMsg=lockStatusMsg(strMsg,buffer+i);
	}
	else
	{
		str.Format("(地锁状态错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i++;
	if(IsValide(strErr,buffer+i,len-i,4))
	{
		v = getInt4Small(buffer+i);
		str.Format("%02x %02x %02x %02x (有功总电度：%0.3f)\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],v*0.001);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(有功总电度长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+4;

	if(IsValide(strErr,buffer+i,len-i,4))
	{
		double	v =getInt4Small(buffer+i);
		str.Format("%02x %02x %02x %02x (已充金额：%0.2f)\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],v*0.01);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(已充金额错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+4;
	if(IsValide(strErr,buffer+i,len-i,4))
	{
		double	v =getInt4Small(buffer+i);
		str.Format("%02x %02x %02x %02x (电价：%0.2f)\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],v*0.01);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(电价错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+4;
	if(IsValide(strErr,buffer+i,len-i,4))
	{
		double	v =getInt4Small(buffer+i);
		str.Format("%02x %02x %02x %02x (已充总度数：%0.3f)\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],v*0.001);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(已充总度数错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+4;
	if(IsValide(strErr,buffer+i,len-i,2))
	{
		unsigned short  v1 =getInt2(buffer+i);
		str.Format("%02x %02x (累计充电时间分：%d)\r\n",buffer[i],buffer[i+1],v1);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(累计充电时间分长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+2;
	if(IsValide(strErr,buffer+i,len-i,1))
	{
		unsigned char value = buffer[i]&0x01;
		strMsg=linkBatteryMsg(strMsg,&value);
		value = (buffer[i]>>1)&0x01;
		str.Format("%02x (枪座状态：",value);
		strMsg = strMsg+str;
		if(value==0)
		{
			str.Format("未收枪)\r\n");
			strMsg = strMsg+str;
		}
		else if(value==1)
		{
			str.Format("收枪)\r\n");
			strMsg = strMsg+str;
		}
		else
		{
			str.Format(")\r\n");
			strMsg = strMsg+str;
		}

		value = (buffer[i]>>2)&0x01;
		str.Format("%02x (充电枪盖状态：",value);
		strMsg = strMsg+str;
		if(value==0)
		{
			str.Format("开)\r\n");
			strMsg = strMsg+str;
		}
		else if(value==1)
		{
			str.Format("关闭)\r\n");
			strMsg = strMsg+str;
		}
		else
		{
			str.Format(")\r\n");
			strMsg = strMsg+str;
		}
		value = (buffer[i]>>3)&0x01;
		str.Format("%02x (车与桩建立通信信号：",value);
		strMsg = strMsg+str;
		if(value==0)
		{
			str.Format("未建立通讯)\r\n");
			strMsg = strMsg+str;
		}
		else if(value==1)
		{
			str.Format("建立通讯)\r\n");
			strMsg = strMsg+str;
		}
		else
		{
			str.Format(")\r\n");
			strMsg = strMsg+str;
		}
		value = (buffer[i]>>4)&0x01;
		str.Format("%02x (车位占用状态：",value);
		strMsg = strMsg+str;
		if(value==0)
		{
			str.Format("未占用)\r\n");
			strMsg = strMsg+str;
		}
		else if(value==1)
		{
			str.Format("已占用)\r\n");
			strMsg = strMsg+str;
		}
		else
		{
			str.Format(")\r\n");
			strMsg = strMsg+str;
		}

	}
	else
	{
		str.Format("(是否连接电池错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}


	i++;
	if(IsValide(strErr,buffer+i,len-i,1))
	{
		unsigned char value = buffer[i]&0x01;
		str.Format("%02x (读卡器通讯故障：",value);
		strMsg = strMsg+str;
		if(value==0)
		{
			str.Format("正常)\r\n");
			strMsg = strMsg+str;
		}
		else if(value==1)
		{
			str.Format("故障)\r\n");
			strMsg = strMsg+str;
		}
		else
		{
			str.Format(")\r\n");
			strMsg = strMsg+str;
		}
		value = (buffer[i]>>1)&0x01;
		str.Format("%02x (急停按钮动作故障：",value);
		strMsg = strMsg+str;
		if(value==0)
		{
			str.Format("正常)\r\n");
			strMsg = strMsg+str;
		}
		else if(value==1)
		{
			str.Format("故障)\r\n");
			strMsg = strMsg+str;
		}
		else
		{
			str.Format(")\r\n");
			strMsg = strMsg+str;
		}
		value = (buffer[i]>>2)&0x01;
		str.Format("%02x (避雷器故障：",value);
		strMsg = strMsg+str;
		if(value==0)
		{
			str.Format("正常)\r\n");
			strMsg = strMsg+str;
		}
		else if(value==1)
		{
			str.Format("故障)\r\n");
			strMsg = strMsg+str;
		}
		else
		{
			str.Format(")\r\n");
			strMsg = strMsg+str;
		}
		value = (buffer[i]>>3)&0x01;
		str.Format("%02x (绝缘检测故障：",value);
		strMsg = strMsg+str;
		if(value==0)
		{
			str.Format("正常)\r\n");
			strMsg = strMsg+str;
		}
		else if(value==1)
		{
			str.Format("故障)\r\n");
			strMsg = strMsg+str;
		}
		else
		{
			str.Format(")\r\n");
			strMsg = strMsg+str;
		}
		value = (buffer[i]>>4)&0x01;
		str.Format("%02x (充电枪未连接告警：",value);
		strMsg = strMsg+str;
		if(value==0)
		{
			str.Format("正常)\r\n");
			strMsg = strMsg+str;
		}
		else if(value==1)
		{
			str.Format("告警)\r\n");
			strMsg = strMsg+str;
		}
		else
		{
			str.Format(")\r\n");
			strMsg = strMsg+str;
		}
		value = (buffer[i]>>5)&0x01;
		str.Format("%02x (交易记录已满告警：",value);
		strMsg = strMsg+str;
		if(value==0)
		{
			str.Format("正常)\r\n");
			strMsg = strMsg+str;
		}
		else if(value==1)
		{
			str.Format("告警)\r\n");
			strMsg = strMsg+str;
		}
		else
		{
			str.Format(")\r\n");
			strMsg = strMsg+str;
		}
		value = (buffer[i]>>6)&0x01;
		str.Format("%02x (电度表异常：",value);
		strMsg = strMsg+str;
		if(value==0)
		{
			str.Format("正常)\r\n");
			strMsg = strMsg+str;
		}
		else if(value==1)
		{
			str.Format("异常)\r\n");
			strMsg = strMsg+str;
		}
		else
		{
			str.Format(")\r\n");
			strMsg = strMsg+str;
		}
	}
	else
	{
		str.Format("(读卡器通讯故障错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}


	i++;

	if(IsValide(strErr,buffer+i,len-i,1))
	{
		unsigned char value = buffer[i]&0x03;
		str.Format("%02x (交流输入过压\欠压告警：",value);
		strMsg = strMsg+str;
		if(value==0)
		{
			str.Format("正常)\r\n");
			strMsg = strMsg+str;
		}
		else if(value==1)
		{
			str.Format("过压)\r\n");
			strMsg = strMsg+str;
		}
		else if(value==2)
		{
			str.Format("欠压)\r\n");
			strMsg = strMsg+str;
		}
		else
		{
			str.Format(")\r\n");
			strMsg = strMsg+str;
		}
		value = (buffer[i]>>2)&0x03;
		str.Format("%02x (充电机过温故障：",value);
		strMsg = strMsg+str;
		if(value==0)
		{
			str.Format("正常)\r\n");
			strMsg = strMsg+str;
		}
		else if(value==1)
		{
			str.Format("故障)\r\n");
			strMsg = strMsg+str;
		}
		else
		{
			str.Format(")\r\n");
			strMsg = strMsg+str;
		}
		value = (buffer[i]>>4)&0x03;
		str.Format("%02x (交流电流过负荷告警：",value);
		strMsg = strMsg+str;
		if(value==0)
		{
			str.Format("正常)\r\n");
			strMsg = strMsg+str;
		}
		else if(value==1)
		{
			str.Format("告警)\r\n");
			strMsg = strMsg+str;
		}
		else
		{
			str.Format(")\r\n");
			strMsg = strMsg+str;
		}
		value = (buffer[i]>>6)&0x03;
		str.Format("%02x (输出继电器状态：",value);
		strMsg = strMsg+str;
		if(value==0)
		{
			str.Format("关)\r\n");
			strMsg = strMsg+str;
		}
		else if(value==1)
		{
			str.Format("开)\r\n");
			strMsg = strMsg+str;
		}
		else
		{
			str.Format(")\r\n");
			strMsg = strMsg+str;
		}
	}
	else
	{
		str.Format("(交流输入过压\欠压告警错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}

	i=i+1;
	strMsg = cmdTimeMsg(strMsg,strErr,buffer+i,len-i);
	i=i+3;
	strMsg = crcMsg(strMsg,strErr,buffer+i,len-i);
	m_Dlg->ProDecodeShow(strMsg);
}
void	CProto_IEC104::DC_Proc2(CString strMsg,unsigned char * buffer,int len)
{
		CString str,strErr;
	
		int i=1;
		if(IsValide(strErr,buffer+i,len-i,8))
		{
			strMsg = getEpCode(strMsg,buffer+i);
		}
		else
		{
			str.Format("(终端机器编码长度错误)\r\n");
			strMsg = strMsg+strErr+str;
			m_Dlg->ProDecodeShow(strMsg);
			return;
		}

		i=9;
		if(IsValide(strErr,buffer+i,len-i,1))
		{
			str.Format("%02x (充电接口标识)\r\n",buffer[i]);
			strMsg = strMsg+str;
		}
		else
		{
			str.Format("(充电接口标识错误)\r\n");
		strMsg = strMsg+strErr+str;
			m_Dlg->ProDecodeShow(strMsg);
			return;
		}
		i++;
		double v = 0;
		if(IsValide(strErr,buffer+i,len-i,2))
		{
			v =getInt2(buffer+i)*0.1;
			str.Format("%02x %02x (充电机输出电压：%0.1f)\r\n",buffer[i],buffer[i+1],v);
			strMsg = strMsg+str;
		}
		else
		{
			str.Format("(充电机输出电压长度错误)\r\n");
			strMsg = strMsg+strErr+str;
			m_Dlg->ProDecodeShow(strMsg);
			return;
		}

		i=i+2;
		if(IsValide(strErr,buffer+i,len-i,2))
		{
			v =getInt2(buffer+i)*0.01;
			str.Format("%02x %02x (充电机输出电流：%0.2f)\r\n",buffer[i],buffer[i+1],v);
			strMsg = strMsg+str;
		}
		else
		{
			str.Format("(充电机输出电流长度错误)\r\n");
			strMsg = strMsg+strErr+str;
			m_Dlg->ProDecodeShow(strMsg);
			return;
		}

		i=i+2;
		unsigned short  v1=0;
		if(IsValide(strErr,buffer+i,len-i,2))
		{
			v1=getInt2(buffer+i);
			str.Format("%02x %02x (SOC：%d)\r\n",buffer[i],buffer[i+1],v1);
			strMsg = strMsg+str;
		}
		else
		{
			str.Format("(SOC长度错误)\r\n");
			strMsg = strMsg+strErr+str;
			m_Dlg->ProDecodeShow(strMsg);
			return;
		}

	//	if(ver<12)
		{
			i=i+2;
			if(IsValide(strErr,buffer+i,len-i,2))
			{
				v1 =getInt2(buffer+i);
				str.Format("%02x %02x (电池组最低温度：%0.1f)\r\n",buffer[i],buffer[i+1],v1*0.1);
				strMsg = strMsg+str;
			}
			else
			{
				str.Format("(电池组最低温度长度错误)\r\n");
				strMsg = strMsg+strErr+str;
				m_Dlg->ProDecodeShow(strMsg);
				return;
			}
			i=i+2;
			if(IsValide(strErr,buffer+i,len-i,2))
			{
				v1 =getInt2(buffer+i);
				str.Format("%02x %02x (电池组最高温度：%0.1f)\r\n",buffer[i],buffer[i+1],v1*0.1);
				strMsg = strMsg+str;
			}
			else
			{
				str.Format("(电池组最高温度长度错误)\r\n");
				strMsg = strMsg+strErr+str;
				m_Dlg->ProDecodeShow(strMsg);
				return;
			}
		}

		i=i+2;
		if(IsValide(strErr,buffer+i,len-i,2))
		{
			v1 =getInt2(buffer+i);
			str.Format("%02x %02x (累计充电时间分：%d)\r\n",buffer[i],buffer[i+1],v1);
			strMsg = strMsg+str;
		}
		else
		{
			str.Format("(累计充电时间分长度错误)\r\n");
			strMsg = strMsg+strErr+str;
			m_Dlg->ProDecodeShow(strMsg);
			return;
		}
		i=i+2;

		if(IsValide(strErr,buffer+i,len-i,1))
		{
			str.Format("%02x (充电机状态：",buffer[i],buffer[i+1]);
			strMsg = strMsg+str;
			switch(buffer[i])
			{
			case 0:
				str.Format("离线)\r\n");
				strMsg = strMsg+str;
				break;
			case 2:
				str.Format("待机)\r\n");
				strMsg = strMsg+str;
				break;
			case 3:
				str.Format("工作)\r\n");
				strMsg = strMsg+str;
				break;
			case 4:
				str.Format("离线)\r\n");
				strMsg = strMsg+str;
				break;
			case 5:
				str.Format("完成)\r\n");
				strMsg = strMsg+str;
				break;
			case 8:
				str.Format("预约)\r\n");
				strMsg = strMsg+str;
				break;
			case 9:
				str.Format("在线升级)\r\n");
				strMsg = strMsg+str;
				break;
			case 10:
				str.Format("在线升级)\r\n");
				strMsg = strMsg+str;
				break;
			default:
				str.Format("操作中)\r\n");
				strMsg = strMsg+str;
				break;
			}
		}
		else
		{
			str.Format("(充电机状态错误)\r\n");
		strMsg = strMsg+strErr+str;
			m_Dlg->ProDecodeShow(strMsg);
			return;
		}
		i=i+1;
		if(IsValide(strErr,buffer+i,len-i,1))
		{
			str.Format("%02x (BMS 通信异常：",buffer[i]);
			strMsg = strMsg+str;
			switch(buffer[i])
			{
			case 0:
				str.Format("正常)\r\n");
				strMsg = strMsg+str;
				break;
			case 1:
				str.Format("异常)\r\n");
				strMsg = strMsg+str;
				break;
			default:
				str.Format("未知)\r\n");
				strMsg = strMsg+str;
				break;
			}
		}
		else
		{
			str.Format("(BMS 通信异常错误)\r\n");
		strMsg = strMsg+strErr+str;
			m_Dlg->ProDecodeShow(strMsg);
			return;
		}
		i=i+1;
		if(IsValide(strErr,buffer+i,len-i,1))
		{
			str.Format("%02x (直流母线输出过压：",buffer[i]);
			strMsg = strMsg+str;
			switch(buffer[i])
			{
			case 0:
				str.Format("不过压)\r\n");
				strMsg = strMsg+str;
				break;
			case 1:
				str.Format("过压)\r\n");
				strMsg = strMsg+str;
				break;
			default:
				str.Format("未知)\r\n");
				strMsg = strMsg+str;
				break;
			}
		}
		else
		{
			str.Format("(直流母线输出过压错误)\r\n");
		strMsg = strMsg+strErr+str;
			m_Dlg->ProDecodeShow(strMsg);
			return;
		}
		i=i+1;
		if(IsValide(strErr,buffer+i,len-i,1))
		{
			str.Format("%02x (直流母线输出欠压：",buffer[i]);
			strMsg = strMsg+str;
			switch(buffer[i])
			{
			case 0:
				str.Format("不欠压)\r\n");
				strMsg = strMsg+str;
				break;
			case 1:
				str.Format("欠压)\r\n");
				strMsg = strMsg+str;
				break;
			default:
				str.Format("未知)\r\n");
				strMsg = strMsg+str;
				break;
			}
		}
		else
		{
			str.Format("(直流母线输出欠压错误)\r\n");
		strMsg = strMsg+strErr+str;
			m_Dlg->ProDecodeShow(strMsg);
			return;
		}
	//	if(ver<12)
		{
			i=i+1;
			if(IsValide(strErr,buffer+i,len-i,1))
			{
				str.Format("%02x (蓄电池充电过流告警：",buffer[i]);
				strMsg = strMsg+str;
				switch(buffer[i])
				{
				case 0:
					str.Format("不过流)\r\n");
					strMsg = strMsg+str;
					break;
				case 1:
					str.Format("过流)\r\n");
					strMsg = strMsg+str;
					break;
				default:
					str.Format("未知)\r\n");
					strMsg = strMsg+str;
					break;
				}
			}
			else
			{
				str.Format("(蓄电池充电过流告警错误)\r\n");
		strMsg = strMsg+strErr+str;
				m_Dlg->ProDecodeShow(strMsg);
				return;
			}
			i=i+1;
			if(IsValide(strErr,buffer+i,len-i,1))
			{
				str.Format("%02x (蓄电池模块采样点过温告警：",buffer[i]);
				strMsg = strMsg+str;
				switch(buffer[i])
				{
				case 0:
					str.Format("不过温)\r\n");
					strMsg = strMsg+str;
					break;
				case 1:
					str.Format("过温)\r\n");
					strMsg = strMsg+str;
					break;
				default:
					str.Format("未知)\r\n");
					strMsg = strMsg+str;
					break;
				}
			}
			else
			{
				str.Format("(蓄电池模块采样点过温告警错误)\r\n");
		strMsg = strMsg+strErr+str;
				m_Dlg->ProDecodeShow(strMsg);
				return;
			}
		}
		i=i+1;
		unsigned int v2=0;
		if(IsValide(strErr,buffer+i,len-i,4))
		{
			 v2 = getInt4Small(buffer+i);
			str.Format("%02x %02x %02x %02x (有功总电度：%0.1f)\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],v2*0.1);
			strMsg = strMsg+str;
		}
		else
		{
			str.Format("(有功总电度长度错误)\r\n");
			strMsg = strMsg+strErr+str;
			m_Dlg->ProDecodeShow(strMsg);
			return;
		}

		i=i+4;
		if(IsValide(strErr,buffer+i,len-i,1))
		{
			str.Format("%02x (是否连接电池车辆)\r\n",buffer[i]);
			strMsg = strMsg+str;
		}
		else
		{
			str.Format("(是否连接电池车辆错误)\r\n");
		strMsg = strMsg+strErr+str;
			m_Dlg->ProDecodeShow(strMsg);
			return;
		}
		i=i+1;
		if(IsValide(strErr,buffer+i,len-i,2))
		{
			v2 = getInt2(buffer+i);
			str.Format("%02x %02x (单体电池最高电压：%0.3f)\r\n",buffer[i],buffer[i+1],v2*0.001);
			strMsg = strMsg+str;
		}
		else
		{
			str.Format("(单体电池最高电压长度错误)\r\n");
			strMsg = strMsg+strErr+str;
			m_Dlg->ProDecodeShow(strMsg);
			return;
		}

		//if(ver<12)
		{
			i=i+2;
			if(IsValide(strErr,buffer+i,len-i,2))
			{
				v2 = getInt2(buffer+i);
				str.Format("%02x %02x (单体电池最低电压：%0.3f)\r\n",buffer[i],buffer[i+1],v2*0.001);
				strMsg = strMsg+str;
			}
			else
			{
				str.Format("(单体电池最低电压长度错误)\r\n");
				strMsg = strMsg+strErr+str;
				m_Dlg->ProDecodeShow(strMsg);
				return;
			}
		}
		//else
		{
			i=i+2;
			if(IsValide(strErr,buffer+i,len-i,1))
			{
				str.Format("%02x (枪座状态：",buffer[i]);
				strMsg = strMsg+str;
				switch(buffer[i])
				{
				case 0:
					str.Format("未收枪)\r\n");
					strMsg = strMsg+str;
					break;
				case 1:
					str.Format("收枪)\r\n");
					strMsg = strMsg+str;
					break;
				default:
					str.Format("未知)\r\n");
					strMsg = strMsg+str;
					break;
				}
			}
			else
			{
				str.Format("(枪座状态错误)\r\n");
		strMsg = strMsg+strErr+str;
				m_Dlg->ProDecodeShow(strMsg);
				return;
			}
			i=i+1;
			if(IsValide(strErr,buffer+i,len-i,1))
			{
				str.Format("%02x (充电枪盖状态：",buffer[i]);
				strMsg = strMsg+str;
				switch(buffer[i])
				{
				case 0:
					str.Format("开)\r\n");
					strMsg = strMsg+str;
					break;
				case 1:
					str.Format("关闭)\r\n");
					strMsg = strMsg+str;
					break;
				default:
					str.Format("未知)\r\n");
					strMsg = strMsg+str;
					break;
				}
			}
			else
			{
				str.Format("(充电枪盖状态错误)\r\n");
		strMsg = strMsg+strErr+str;
				m_Dlg->ProDecodeShow(strMsg);
				return;
			}
			i=i+1;
			if(IsValide(strErr,buffer+i,len-i,1))
			{
				str.Format("%02x (车与桩建立通信信号：",buffer[i]);
				strMsg = strMsg+str;
				switch(buffer[i])
				{
				case 0:
					str.Format("未建立通讯)\r\n");
					strMsg = strMsg+str;
					break;
				case 1:
					str.Format("建立通讯)\r\n");
					strMsg = strMsg+str;
					break;
				default:
					str.Format("未知)\r\n");
					strMsg = strMsg+str;
					break;
				}
			}
			else
			{
				str.Format("(车与桩建立通信信号错误)\r\n");
		        strMsg = strMsg+strErr+str;
				m_Dlg->ProDecodeShow(strMsg);
				return;
			}
			i=i+1;
			if(IsValide(strErr,buffer+i,len-i,1))
			{
				str.Format("%02x (车位占用状态：",buffer[i]);
				strMsg = strMsg+str;
				switch(buffer[i])
				{
				case 0:
					str.Format("未占用)\r\n");
					strMsg = strMsg+str;
					break;
				case 1:
					str.Format("占用)\r\n");
					strMsg = strMsg+str;
					break;
				default:
					str.Format("未知)\r\n");
					strMsg = strMsg+str;
					break;
				}
			}
			else
			{
				str.Format("(车位占用状态错误)\r\n");
		        strMsg = strMsg+strErr+str;
				m_Dlg->ProDecodeShow(strMsg);
				return;
			}
			i=i+1;
			if(IsValide(strErr,buffer+i,len-i,1))
			{
				str.Format("%02x (交易记录已满告警：",buffer[i]);
				strMsg = strMsg+str;
				switch(buffer[i])
				{
				case 0:
					str.Format("未告警)\r\n");
					strMsg = strMsg+str;
					break;
				case 1:
					str.Format("交易记录已满告警)\r\n");
					strMsg = strMsg+str;
					break;
				default:
					str.Format("未知)\r\n");
					strMsg = strMsg+str;
					break;
				}
			}
			else
			{
				str.Format("(交易记录已满告警错误)\r\n");
		        strMsg = strMsg+strErr+str;
				m_Dlg->ProDecodeShow(strMsg);
				return;
			}
				i=i+1;
			if(IsValide(strErr,buffer+i,len-i,1))
			{
				str.Format("%02x (读卡器通讯异常：",buffer[i]);
				strMsg = strMsg+str;
				switch(buffer[i])
				{
				case 0:
					str.Format("正常)\r\n");
					strMsg = strMsg+str;
					break;
				case 1:
					str.Format("异常)\r\n");
					strMsg = strMsg+str;
					break;
				default:
					str.Format("未知)\r\n");
					strMsg = strMsg+str;
					break;
				}
			}
			else
			{
				str.Format("(读卡器通讯异常错误)\r\n");
		        strMsg = strMsg+strErr+str;
				m_Dlg->ProDecodeShow(strMsg);
				return;
			}
				i=i+1;
			if(IsValide(strErr,buffer+i,len-i,1))
			{
				str.Format("%02x (电度表异常：",buffer[i]);
				strMsg = strMsg+str;
				switch(buffer[i])
				{
				case 0:
					str.Format("正常)\r\n");
					strMsg = strMsg+str;
					break;
				case 1:
					str.Format("异常)\r\n");
					strMsg = strMsg+str;
					break;
				default:
					str.Format("未知)\r\n");
					strMsg = strMsg+str;
					break;
				}
			}
			else
			{
				str.Format("(电度表异常错误)\r\n");
		        strMsg = strMsg+strErr+str;
				m_Dlg->ProDecodeShow(strMsg);
				return;
			}
			i=i+1;
			
			if(IsValide(strErr,buffer+i,len-i,4))
		     {
		             double	v =getInt4Small(buffer+i);
		 	         str.Format("%02x %02x %02x %02x (已充金额：%f)\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],v*0.01);
			          strMsg = strMsg+str;
		    }
		    else
		   {
			       str.Format("(已充金额错误)\r\n");
			       strMsg = strMsg+strErr+str;
		        	m_Dlg->ProDecodeShow(strMsg);
		      	return;
	    	}
            i=i+4;
			if(IsValide(strErr,buffer+i,len-i,4))
		     {
		             double	v =getInt4Small(buffer+i);
		 	         str.Format("%02x %02x %02x %02x (电价：%f)\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],v*0.01);
			          strMsg = strMsg+str;
		    }
		    else
		   {
			       str.Format("(电价错误)\r\n");
			       strMsg = strMsg+strErr+str;
		        	m_Dlg->ProDecodeShow(strMsg);
		      	return;
	    	}
			 i=i+4;
			if(IsValide(strErr,buffer+i,len-i,4))
		     {
		             double	v =getInt4Small(buffer+i);
		 	         str.Format("%02x %02x %02x %02x (已充总度数：%f)\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],v*0.01);
			          strMsg = strMsg+str;
		    }
		    else
		   {
			       str.Format("(已充总度数错误)\r\n");
			       strMsg = strMsg+strErr+str;
		        	m_Dlg->ProDecodeShow(strMsg);
		      	return;
	    	}
			i=i+4;
			if(IsValide(strErr,buffer+i,len-i,1))
			{
				str.Format("%02x (地锁状态：",buffer[i]);
				strMsg = strMsg+str;
				switch(buffer[i])
				{
				case 0:
					str.Format("地锁硬件无法找到)\r\n");
					strMsg = strMsg+str;
					break;
				case 1:
					str.Format("降下)\r\n");
					strMsg = strMsg+str;
					break;
				case 2:
					str.Format("升起)\r\n");
					strMsg = strMsg+str;
					break;
				case 3:
					str.Format("运动中)\r\n");
					strMsg = strMsg+str;
					break;
				case 4:
					str.Format("故障)\r\n");
					strMsg = strMsg+str;
					break;
				default:
					str.Format("未知)\r\n");
					strMsg = strMsg+str;
					break;
				}
			}
			else
			{
				str.Format("(地锁状态错误)\r\n");
		        strMsg = strMsg+strErr+str;
				m_Dlg->ProDecodeShow(strMsg);
				return;
			}
			i=i+1;
		strMsg = cmdTimeMsg(strMsg,strErr,buffer+i,len-i);
		i=i+3;
        strMsg = crcMsg(strMsg,strErr,buffer+i,len-i);

		}
		
		m_Dlg->ProDecodeShow(strMsg);
}
void	CProto_IEC104::DC_Proc4(CString strMsg,unsigned char * buffer,int len)
{
	CString str,strErr;

	int i=0;
	if(IsValide(strErr,buffer+i,len-i,8))
	{
		strMsg = getEpCode(strMsg,buffer+i);
	}
	else
	{
		str.Format("(终端机器编码长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+8;
	if(IsValide(strErr,buffer+i,len-i,1))
	{
		str.Format("%02x (充电接口标识)\r\n",buffer[i]);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(充电接口标识错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i++;
	if(IsValide(strErr,buffer+i,len-i,2))
	{
		int v =getInt2(buffer+i);
		str.Format("%02x %02x (充电输出电压%0.1f)\r\n",buffer[i],buffer[i+1],v*0.1);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(充电输出电压长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+2;
	double v=0;
	if(IsValide(strErr,buffer+i,len-i,2))
	{
		v =getInt2(buffer+i);
		str.Format("%02x %02x (充电输出电流%0.2f)\r\n",buffer[i],buffer[i+1],v*0.01);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(充电输出电流长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+2;
	if(IsValide(strErr,buffer+i,len-i,1))
	{
		strMsg=workStatusMsg(strMsg,buffer+i);
	}
	else
	{
		str.Format("(工作状态错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i++;
	if(IsValide(strErr,buffer+i,len-i,1))
	{
		strMsg=lockStatusMsg(strMsg,buffer+i);
	}
	else
	{
		str.Format("(地锁状态错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i++;
	if(IsValide(strErr,buffer+i,len-i,4))
	{
		v = getInt4Small(buffer+i);
		str.Format("%02x %02x %02x %02x (有功总电度：%0.2f)\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],v*0.001);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(有功总电度长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+4;

	if(IsValide(strErr,buffer+i,len-i,4))
	{
		double	v =getInt4Small(buffer+i);
		str.Format("%02x %02x %02x %02x (已充金额：%0.2f)\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],v*0.01);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(已充金额错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+4;
	if(IsValide(strErr,buffer+i,len-i,4))
	{
		double	v =getInt4Small(buffer+i);
		str.Format("%02x %02x %02x %02x (电价：%0.2f)\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],v*0.001);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(电价错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+4;
	if(IsValide(strErr,buffer+i,len-i,4))
	{
		double	v =getInt4Small(buffer+i);
		str.Format("%02x %02x %02x %02x (已充总度数：%0.2f)\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],v*0.001);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(已充总度数错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+4;
	if(IsValide(strErr,buffer+i,len-i,2))
	{
		unsigned short  v1 =getInt2(buffer+i);
		str.Format("%02x %02x (累计充电时间分：%d)\r\n",buffer[i],buffer[i+1],v1);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(累计充电时间分长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+2;
	if(IsValide(strErr,buffer+i,len-i,1))
	{
		unsigned char value = buffer[i]&0x01;
		strMsg=linkBatteryMsg(strMsg,&value);
		value = (buffer[i]>>1)&0x01;
		str.Format("%02x (枪座状态：",value);
		strMsg = strMsg+str;
		if(value==0)
		{
			str.Format("未收枪)\r\n");
			strMsg = strMsg+str;
		}
		else if(value==1)
		{
			str.Format("收枪)\r\n");
			strMsg = strMsg+str;
		}
		else
		{
			str.Format(")\r\n");
			strMsg = strMsg+str;
		}

		value = (buffer[i]>>2)&0x01;
		str.Format("%02x (充电枪盖状态：",value);
		strMsg = strMsg+str;
		if(value==0)
		{
			str.Format("开)\r\n");
			strMsg = strMsg+str;
		}
		else if(value==1)
		{
			str.Format("关闭)\r\n");
			strMsg = strMsg+str;
		}
		else
		{
			str.Format(")\r\n");
			strMsg = strMsg+str;
		}
		value = (buffer[i]>>3)&0x01;
		str.Format("%02x (车与桩建立通信信号：",value);
		strMsg = strMsg+str;
		if(value==0)
		{
			str.Format("未建立通讯)\r\n");
			strMsg = strMsg+str;
		}
		else if(value==1)
		{
			str.Format("建立通讯)\r\n");
			strMsg = strMsg+str;
		}
		else
		{
			str.Format(")\r\n");
			strMsg = strMsg+str;
		}
		value = (buffer[i]>>4)&0x01;
		str.Format("%02x (车位占用状态：",value);
		strMsg = strMsg+str;
		if(value==0)
		{
			str.Format("未占用)\r\n");
			strMsg = strMsg+str;
		}
		else if(value==1)
		{
			str.Format("已占用)\r\n");
			strMsg = strMsg+str;
		}
		else
		{
			str.Format(")\r\n");
			strMsg = strMsg+str;
		}

	}
	else
	{
		str.Format("(是否连接电池错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}


	i++;
	if(IsValide(strErr,buffer+i,len-i,1))
	{
		unsigned char value = buffer[i]&0x01;
		str.Format("%02x (读卡器通讯故障：",value);
		strMsg = strMsg+str;
		if(value==0)
		{
			str.Format("正常)\r\n");
			strMsg = strMsg+str;
		}
		else if(value==1)
		{
			str.Format("故障)\r\n");
			strMsg = strMsg+str;
		}
		else
		{
			str.Format(")\r\n");
			strMsg = strMsg+str;
		}
		value = (buffer[i]>>1)&0x01;
		str.Format("%02x (急停按钮动作故障：",value);
		strMsg = strMsg+str;
		if(value==0)
		{
			str.Format("正常)\r\n");
			strMsg = strMsg+str;
		}
		else if(value==1)
		{
			str.Format("故障)\r\n");
			strMsg = strMsg+str;
		}
		else
		{
			str.Format(")\r\n");
			strMsg = strMsg+str;
		}
		value = (buffer[i]>>2)&0x01;
		str.Format("%02x (避雷器故障：",value);
		strMsg = strMsg+str;
		if(value==0)
		{
			str.Format("正常)\r\n");
			strMsg = strMsg+str;
		}
		else if(value==1)
		{
			str.Format("故障)\r\n");
			strMsg = strMsg+str;
		}
		else
		{
			str.Format(")\r\n");
			strMsg = strMsg+str;
		}
		value = (buffer[i]>>3)&0x01;
		str.Format("%02x (绝缘检测故障：",value);
		strMsg = strMsg+str;
		if(value==0)
		{
			str.Format("正常)\r\n");
			strMsg = strMsg+str;
		}
		else if(value==1)
		{
			str.Format("故障)\r\n");
			strMsg = strMsg+str;
		}
		else
		{
			str.Format(")\r\n");
			strMsg = strMsg+str;
		}
		value = (buffer[i]>>4)&0x01;
		str.Format("%02x (充电枪未连接告警：",value);
		strMsg = strMsg+str;
		if(value==0)
		{
			str.Format("正常)\r\n");
			strMsg = strMsg+str;
		}
		else if(value==1)
		{
			str.Format("告警)\r\n");
			strMsg = strMsg+str;
		}
		else
		{
			str.Format(")\r\n");
			strMsg = strMsg+str;
		}
		value = (buffer[i]>>5)&0x01;
		str.Format("%02x (交易记录已满告警：",value);
		strMsg = strMsg+str;
		if(value==0)
		{
			str.Format("正常)\r\n");
			strMsg = strMsg+str;
		}
		else if(value==1)
		{
			str.Format("告警)\r\n");
			strMsg = strMsg+str;
		}
		else
		{
			str.Format(")\r\n");
			strMsg = strMsg+str;
		}
		value = (buffer[i]>>6)&0x01;
		str.Format("%02x (电度表异常：",value);
		strMsg = strMsg+str;
		if(value==0)
		{
			str.Format("正常)\r\n");
			strMsg = strMsg+str;
		}
		else if(value==1)
		{
			str.Format("异常)\r\n");
			strMsg = strMsg+str;
		}
		else
		{
			str.Format(")\r\n");
			strMsg = strMsg+str;
		}
	}
	else
	{
		str.Format("(读卡器通讯故障错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}


	i++;

	if(IsValide(strErr,buffer+i,len-i,1))
	{
		unsigned char value = buffer[i]&0x03;
		str.Format("%02x (交流输入过压\欠压告警：",value);
		strMsg = strMsg+str;
		if(value==0)
		{
			str.Format("正常)\r\n");
			strMsg = strMsg+str;
		}
		else if(value==1)
		{
			str.Format("过压)\r\n");
			strMsg = strMsg+str;
		}
		else if(value==2)
		{
			str.Format("欠压)\r\n");
			strMsg = strMsg+str;
		}
		else
		{
			str.Format(")\r\n");
			strMsg = strMsg+str;
		}
		value = (buffer[i]>>2)&0x03;
		str.Format("%02x (充电机过温故障：",value);
		strMsg = strMsg+str;
		if(value==0)
		{
			str.Format("正常)\r\n");
			strMsg = strMsg+str;
		}
		else if(value==1)
		{
			str.Format("故障)\r\n");
			strMsg = strMsg+str;
		}
		else
		{
			str.Format(")\r\n");
			strMsg = strMsg+str;
		}
		value = (buffer[i]>>4)&0x03;
		str.Format("%02x (交流电流过负荷告警：",value);
		strMsg = strMsg+str;
		if(value==0)
		{
			str.Format("正常)\r\n");
			strMsg = strMsg+str;
		}
		else if(value==1)
		{
			str.Format("告警)\r\n");
			strMsg = strMsg+str;
		}
		else
		{
			str.Format(")\r\n");
			strMsg = strMsg+str;
		}
		value = (buffer[i]>>6)&0x03;
		str.Format("%02x (输出继电器状态：",value);
		strMsg = strMsg+str;
		if(value==0)
		{
			str.Format("关)\r\n");
			strMsg = strMsg+str;
		}
		else if(value==1)
		{
			str.Format("开)\r\n");
			strMsg = strMsg+str;
		}
		else
		{
			str.Format(")\r\n");
			strMsg = strMsg+str;
		}
	}
	else
	{
		str.Format("(交流输入过压\欠压告警错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i++;
	if(IsValide(strErr,buffer+i,len-i,1))
	{
		str.Format("%02x (SOC: %d\r\n)",buffer[i],buffer[i]);
	}
	else
	{
		str.Format("(SOC错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i++;
	if(IsValide(strErr,buffer+i,len-i,2))
	{
		int v =getInt2(buffer+i);
		str.Format("%02x %02x(电池组最低温度: %.1f\r\n)",buffer[i],buffer[i+1],v*0.1);
	}
	else
	{
		str.Format("(电池组最低温度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+2;
	if(IsValide(strErr,buffer+i,len-i,2))
	{
		int v = getInt2(buffer+i);
		str.Format("%02x %02x(电池组最高温度: %.1f\r\n)",buffer[i],buffer[i+1],v*0.1);
	}
	else
	{
		str.Format("(电池组最高温度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+2;
	if(IsValide(strErr,buffer+i,len-i,1))
	{
		unsigned char value=buffer[i]&0x01;
		str.Format("%02x (电池反接故障: ",value);
		strMsg = strMsg+str;
		if(value==0)
		{
			str.Format("正常)\r\n");
			strMsg = strMsg+str;
		}
		else if(value==1)
		{
			str.Format("故障)\r\n");
			strMsg = strMsg+str;
		}
		else
		{
			str.Format(")\r\n");
			strMsg = strMsg+str;
		}
		value=(buffer[i]>>1)&0x01;
		str.Format("%02x (烟雾报警故障: ",value);
		strMsg = strMsg+str;
		if(value==0)
		{
			str.Format("正常)\r\n");
			strMsg = strMsg+str;
		}
		else if(value==1)
		{
			str.Format("故障)\r\n");
			strMsg = strMsg+str;
		}
		else
		{
			str.Format(")\r\n");
			strMsg = strMsg+str;
		}
		value=(buffer[i]>>2)&0x01;
		str.Format("%02x (BMS 通信异常: ",value);
		strMsg = strMsg+str;
		if(value==0)
		{
			str.Format("正常)\r\n");
			strMsg = strMsg+str;
		}
		else if(value==1)
		{
			str.Format("异常)\r\n");
			strMsg = strMsg+str;
		}
		else
		{
			str.Format(")\r\n");
			strMsg = strMsg+str;
		}
		value=(buffer[i]>>3)&0x01;
		str.Format("%02x (直流电度表异常: ",value);
		strMsg = strMsg+str;
		if(value==0)
		{
			str.Format("正常)\r\n");
			strMsg = strMsg+str;
		}
		else if(value==1)
		{
			str.Format("异常)\r\n");
			strMsg = strMsg+str;
		}
		else
		{
			str.Format(")\r\n");
			strMsg = strMsg+str;
		}
		value=(buffer[i]>>4)&0x01;
		str.Format("%02x (直流输出过流告警: ",value);
		strMsg = strMsg+str;
		if(value==0)
		{
			str.Format("正常)\r\n");
			strMsg = strMsg+str;
		}
		else if(value==1)
		{
			str.Format("告警)\r\n");
			strMsg = strMsg+str;
		}
		else
		{
			str.Format(")\r\n");
			strMsg = strMsg+str;
		}

	}
	else
	{
		str.Format("(电池反接故障错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i++;
	if(IsValide(strErr,buffer+i,len-i,1))
	{
		unsigned char value = buffer[i]&0x03;
		str.Format("%02x (充电模式: ",value);
		strMsg = strMsg+str;
		if(value==0)
		{
			str.Format("不可信)\r\n");
			strMsg = strMsg+str;
		}
		else if(value==1)
		{
			str.Format("恒压)\r\n");
			strMsg = strMsg+str;
		}
		else if(value==2)
		{
			str.Format("恒流)\r\n");
			strMsg = strMsg+str;
		}
		else
		{
			str.Format(")\r\n");
			strMsg = strMsg+str;
		}
		value = (buffer[i]>>2)&0x03;
		str.Format("%02x (整车动力蓄电池SOC告警: ",value);
		strMsg = strMsg+str;
		if(value==0)
		{
			str.Format("不可信)\r\n");
			strMsg = strMsg+str;
		}
		else if(value==1)
		{
			str.Format("恒压)\r\n");
			strMsg = strMsg+str;
		}
		else if(value==2)
		{
			str.Format("恒流)\r\n");
			strMsg = strMsg+str;
		}
		else
		{
			str.Format(")\r\n");
			strMsg = strMsg+str;
		}
		value = (buffer[i]>>4)&0x03;
		str.Format("%02x (蓄电池模块采样点过温告警: ",value);
		strMsg = strMsg+str;
		if(value==0)
		{
			str.Format("不可信)\r\n");
			strMsg = strMsg+str;
		}
		else if(value==1)
		{
			str.Format("恒压)\r\n");
			strMsg = strMsg+str;
		}
		else if(value==2)
		{
			str.Format("恒流)\r\n");
			strMsg = strMsg+str;
		}
		else
		{
			str.Format(")\r\n");
			strMsg = strMsg+str;
		}
		value = (buffer[i]>>6)&0x03;
		str.Format("%02x (输出连接器过温: ",value);
		strMsg = strMsg+str;
		if(value==0)
		{
			str.Format("不可信)\r\n");
			strMsg = strMsg+str;
		}
		else if(value==1)
		{
			str.Format("恒压)\r\n");
			strMsg = strMsg+str;
		}
		else if(value==2)
		{
			str.Format("恒流)\r\n");
			strMsg = strMsg+str;
		}
		else
		{
			str.Format(")\r\n");
			strMsg = strMsg+str;
		}
	}
	else
	{
		str.Format("(充电模式错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i++;

	if(IsValide(strErr,buffer+i,len-i,1))
	{
		unsigned char value = buffer[i]&0x03;
		str.Format("%02x (整车动力蓄电池组输出连接器连接状态: ",value);
		strMsg = strMsg+str;
		if(value==0)
		{
			str.Format("不可信)\r\n");
			strMsg = strMsg+str;
		}
		else if(value==1)
		{
			str.Format("恒压)\r\n");
			strMsg = strMsg+str;
		}
		else if(value==2)
		{
			str.Format("恒流)\r\n");
			strMsg = strMsg+str;
		}
		else
		{
			str.Format(")\r\n");
			strMsg = strMsg+str;
		}
		value =( buffer[i]>>2)&0x03;
		str.Format("%02x (整车蓄电池充电过流告警: ",value);
		strMsg = strMsg+str;
		if(value==0)
		{
			str.Format("不可信)\r\n");
			strMsg = strMsg+str;
		}
		else if(value==1)
		{
			str.Format("恒压)\r\n");
			strMsg = strMsg+str;
		}
		else if(value==2)
		{
			str.Format("恒流)\r\n");
			strMsg = strMsg+str;
		}
		else
		{
			str.Format(")\r\n");
			strMsg = strMsg+str;
		}
		value =( buffer[i]>>4)&0x03;
		str.Format("%02x (直流母线输出过压/欠压: ",value);
		strMsg = strMsg+str;
		if(value==0)
		{
			str.Format("不可信)\r\n");
			strMsg = strMsg+str;
		}
		else if(value==1)
		{
			str.Format("恒压)\r\n");
			strMsg = strMsg+str;
		}
		else if(value==2)
		{
			str.Format("恒流)\r\n");
			strMsg = strMsg+str;
		}
		else
		{
			str.Format(")\r\n");
			strMsg = strMsg+str;
		}
		value =( buffer[i]>>6)&0x03;
		str.Format("%02x (BMS过压/欠压告警: ",value);
		strMsg = strMsg+str;
		if(value==0)
		{
			str.Format("不可信)\r\n");
			strMsg = strMsg+str;
		}
		else if(value==1)
		{
			str.Format("恒压)\r\n");
			strMsg = strMsg+str;
		}
		else if(value==2)
		{
			str.Format("恒流)\r\n");
			strMsg = strMsg+str;
		}
		else
		{
			str.Format(")\r\n");
			strMsg = strMsg+str;
		}
	}
	else
	{
		str.Format("(整车动力蓄电池组输出连接器连接状态错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+1;
	strMsg = cmdTimeMsg(strMsg,strErr,buffer+i,len-i);
	i=i+3;
	strMsg = crcMsg(strMsg,strErr,buffer+i,len-i);
	m_Dlg->ProDecodeShow(strMsg);
}
void	CProto_IEC104::All_dataProc(CString strMsg,unsigned char * buff,int len)
{
	CString str,strErr;
	unsigned char * buffer = buff;
	if(buffer==0 || len==0)
	{
		str="无数据内容";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}


	strMsg=headMsg1(strMsg,buffer+4,"交流充电桩实时监测数据");

	len = len-16+3;

	buffer=buffer+16-3;

	str.Format("%02x (记录类型）\r\n",buffer[0]);
	strMsg=strMsg+str;

	
	if(buffer[0] == 3)
	{
		AC_Proc3(strMsg,buffer+1,len-1);
	}
	else if(buffer[0] == 1)
	{
		AC_Proc1(strMsg,buffer+1,len-1);
	}
	else if(buffer[0]==2)
	{
		DC_Proc2(strMsg,buffer+1,len-1);
	}
	else if(buffer[0]==4)
	{
		DC_Proc4(strMsg,buffer+1,len-1);
	}
	else
	{
		strMsg=strMsg+"记录类型错误";
		m_Dlg->ProDecodeShow(strMsg);
	}

}
//解释下行数据
void	CProto_IEC104::Down_RxProcV1(CString strMsg,unsigned char *buffer,int len)
{
	CString str1,strErr;
		strMsg=headMsg1(strMsg,buffer+4,"业务下行数据");
		int i=16-3;
		switch(buffer[i])
	   {
	   case 1:
           str1.Format("\r\n%02x(黑名单下发时下行数据：%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_Down_1(strMsg,buffer+i+1,len-i-1);
		   break;
	   case 2:
		   str1.Format("\r\n%02x(下发计费模型下行数据：%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_Down_1_v2(strMsg,buffer+i+1,len-i-1);
		   break;
	   case 3:
		    str1.Format("\r\n%02x(私有充电桩下发充电桩运营时间：%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_Down_3(strMsg,buffer+i+1,len-i-1);
		   break;
	   case 4:
		   str1.Format("\r\n%02x(无卡验鉴权结果下行数据：%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_Down_4(strMsg,buffer+i+1,len-i-1);
		   break;
	   case 5:
		   str1.Format("\r\n%02x(无卡充电扣款后下行数据：%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_Down_5(strMsg,buffer+i+1,len-i-1);
		   break;
	   case 6:
		    str1.Format("\r\n%02x(预约：%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_Down_6(strMsg,buffer+i+1,len-i-1);
		   break;
	   case 7:
		   str1.Format("\r\n%02x(取消预约：%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_Down_7(strMsg,buffer+i+1,len-i-1);
		   break;
	   case 8:
		    str1.Format("\r\n%02x(启动充电：%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_Down_8(strMsg,buffer+i+1,len-i-1);
		   break;
	   case 9:
		    str1.Format("\r\n%02x(有卡鉴权下行数据：%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_Down_9(strMsg,buffer+i+1,len-i-1);
		   break;
	   case 10:
		   str1.Format("\r\n%02x(有卡充电扣款后下行数据：%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_Down_10(strMsg,buffer+i+1,len-i-1);
		   break;
	    case 11:
			str1.Format("\r\n%02x(交易取随机数下行数据：%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_Down_11(strMsg,buffer+i+1,len-i-1);
		   break;
	    case 12:
			str1.Format("\r\n%02x(离线交易包下行数据：%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_Down_12(strMsg,buffer+i+1,len-i-1);
		   break;
	    case 13:
			str1.Format("\r\n%02x(异桩解扣下行数据%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_Down_13(strMsg,buffer+i+1,len-i-1);
		   break;
	    case 14:
			str1.Format("\r\n%02x(异桩解扣结果下行数据：%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_Down_14(strMsg,buffer+i+1,len-i-1);
		   break;
	    case 17:
			 str1.Format("\r\n%02x(密钥升级指令：%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_Down_17(strMsg,buffer+i+1,len-i-1);
		   break;
	    case 18:
			str1.Format("\r\n%02x(密钥更新：%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_Down_18(strMsg,buffer+i+1,len-i-1);
		   break;
	    case 19:
		str1.Format("\r\n%02x(二维码文件概要信息下行数据：%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_Down_19(strMsg,buffer+i+1,len-i-1);
		   break;
	    case 20:
			str1.Format("\r\n%02x(二维码分段数据：%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_Down_20(strMsg,buffer+i+1,len-i-1);
		   break;
	    case 21:
			str1.Format("\r\n%02x(充电桩文件概要信息下行数据：%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_Down_21(strMsg,buffer+i+1,len-i-1);
		   break;
	    case 22:
			str1.Format("\r\n%02x(充电桩文件分段下行数据：%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_Down_22(strMsg,buffer+i+1,len-i-1);
		   break;
	    case 23:
			str1.Format("\r\n%02x(停止充电：%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_Down_23(strMsg,buffer+i+1,len-i-1);
		   break;
		case 24:
			str1.Format("\r\n%02x(业务确认：%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_Down_24(strMsg,buffer+i+1,len-i-1);
		   break;
	   case 25:
		   str1.Format("\r\n%02x(充电桩统计信息下行数据：%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_Down_25(strMsg,buffer+i+1,len-i-1);
		   break;
	   case 26:
		   str1.Format("\r\n%02x(更新充电桩后台服务器IP 下行数据：%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_Down_26(strMsg,buffer+i+1,len-i-1);
		   break;
	    case 27:
		   str1.Format("\r\n%02x(近场呼叫：%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_Down_27(strMsg,buffer+i+1,len-i-1);
		   break;
	    case 28:
		   str1.Format("\r\n%02x(打开锁枪装置下行数据：%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_Down_28(strMsg,buffer+i+1,len-i-1);
		   break;
	    case 29:
		   str1.Format("\r\n%02x(联网充值下行应答数据：%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_Down_29(strMsg,buffer+i+1,len-i-1);
		   break;
	   default:
		    str1.Format("记录类型错误");
		   strMsg =strMsg+str1;
		   m_Dlg->ProDecodeShow(strMsg);
		   break;
	   }

}

//解释下行数据
void	CProto_IEC104::Down_RxProcV2(CString strMsg,unsigned char *buffer,int len)
{
	CString str1,strErr;
	strMsg=headMsg1(strMsg,buffer+4,"业务下行数据");
   
		int i=16-3;
		switch(buffer[i])
	   {
	   case 1:
		   str1.Format("\r\n%02x(下发计费模型下行数据：%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_Down_1_v2(strMsg,buffer+i+1,len-i-1);
		   break;
	   case 3:
		    str1.Format("\r\n%02x(私有充电桩下发充电桩运营时间：%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_Down_3(strMsg,buffer+i+1,len-i-1);
		   break;
	   case 11:
		   str1.Format("\r\n%02x(无卡鉴权下行数据：%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_Down_11_v2(strMsg,buffer+i+1,len-i-1);
		   break;
	   case 13:
		   str1.Format("\r\n%02x(不圈存卡鉴权下行应答数据：%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_Down_13_v2(strMsg,buffer+i+1,len-i-1);
		   break;
	   case 31:
		    str1.Format("\r\n%02x(预约：%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_Down_31_v2(strMsg,buffer+i+1,len-i-1);
		   break;
	   case 32:
		    str1.Format("\r\n%02x(电桩应答预约报文的确认：%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_Down_32_v2(strMsg,buffer+i+1,len-i-1);
		   break;
	   case 33:
		   str1.Format("\r\n%02x(取消预约：%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_Down_33_v2(strMsg,buffer+i+1,len-i-1);
		   break;
	   case 41:
		    str1.Format("\r\n%02x(启动充电：%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_Down_41_v2(strMsg,buffer+i+1,len-i-1);
		   break;
	    case 42:
		    str1.Format("\r\n%02x(充电事件的确认报文：%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_Down_42_v2(strMsg,buffer+i+1,len-i-1);
		   break;
	   case 43:
			str1.Format("\r\n%02x(停止充电：%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_Down_23(strMsg,buffer+i+1,len-i-1);
		   break;
	   case 44:
			str1.Format("\r\n%02x(不圈存卡充电冻结金额下行数据：%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_Down_44_v2(strMsg,buffer+i+1,len-i-1);
		   break;
	   case 46:
			str1.Format("\r\n%02x(消费记录确认下行数据：%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_Down_46_v2(strMsg,buffer+i+1,len-i-1);
		   break;
	   case 49:
			str1.Format("\r\n%02x(电桩识别码应答下行数据：%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		    Pro_Down_49_v2(strMsg,buffer+i+1,len-i-1);
		   break;
	  
	     case 102:
		   str1.Format("\r\n%02x(近场呼叫：%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_Down_102(strMsg,buffer+i+1,len-i-1);
		   break;
	    case 103:
		   str1.Format("\r\n%02x(降地锁：%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_Down_103(strMsg,buffer+i+1,len-i-1);
		   break;
	    case 104:
		   str1.Format("\r\n%02x(打开锁枪装置下行数据：%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_Down_28(strMsg,buffer+i+1,len-i-1);
		   break;
	    case 141:
			str1.Format("\r\n%02x(产品信息查询：%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_Down_141(strMsg,buffer+i+1,len-i-1);
		   break;
	    case 142:
			str1.Format("\r\n%02x(强制更新远端设备软件下行数据：%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_Down_142(strMsg,buffer+i+1,len-i-1);
		   break;
	    case 143:
			str1.Format("\r\n%02x(充电桩文件概要信息下行数据：%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_Down_143(strMsg,buffer+i+1,len-i-1);
		   break;
	    case 144:
			str1.Format("\r\n%02x(充电桩文件分段下行数据：%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_Down_144(strMsg,buffer+i+1,len-i-1);
		   break;
	    
	
	   case 161:
		   str1.Format("\r\n%02x(充电桩统计信息下行数据：%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_Down_161(strMsg,buffer+i+1,len-i-1);
		   break;
	    case 162:
		   str1.Format("\r\n%02x(电桩业务数据查询：%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_Down_162(strMsg,buffer+i+1,len-i-1);
		   break;
	   case 146:
		   str1.Format("\r\n%02x(更新充电桩后台服务器IP 下行数据：%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_Down_26(strMsg,buffer+i+1,len-i-1);
		   break;
	   case 147:
		   str1.Format("\r\n%02x(查询远端设备通讯信号强度：%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_Down_147(strMsg,buffer+i+1,len-i-1);
		   break;
	    
	   default:
		    str1.Format("记录类型错误");
		   strMsg =strMsg+str1;
		   m_Dlg->ProDecodeShow(strMsg);
		   break;
	   }

}

//下发计费模型下行数据(尖峰平谷)
void	CProto_IEC104::Pro_Down_1(CString strMsg,unsigned char *buffer,int len)
{
	
	CString str,strErr;
	if(buffer==0 || len==0)
	{
		str="无数据内容";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	
	int i=0;
	if(IsValide(strErr,buffer,len,8))
	{
		strMsg = getEpCode(strMsg,buffer);
	}
	else
	{
		str.Format("(终端机器编码长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=8;
	if(IsValide(strErr,buffer+i,len-i,9))
	{

		str.Format("%02x%02x%02x%02x%02x%02x%02x%02x%02x(时间戳序号)\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],buffer[i+4],buffer[i+5],buffer[i+6],buffer[i+7],buffer[i+8]);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(时间戳序号长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}

	i=17;
	if(IsValide(strErr,buffer+i,len-i,1))
	{
		str.Format("%02x (黑名单数量：%d)\r\n",buffer[i],buffer[i]);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(黑名单数量错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	if(len != buffer[17]*9+12)
	{
		str.Format("黑名单记录数据错误，数据长度不对"); 
		strMsg = strMsg+str;
		/*m_Dlg->ProDecodeShow(strMsg);
		return;*/
	}


	for(int j=0;j<buffer[17];j++)
	{
		i=12+j*9;
		if(IsValide(strErr,buffer+i,len-i,8))
		{
			for(i=12+j*9;i<j*9+20;i++)
			{
				str.Format("%02x",buffer[i]);
				strMsg = strMsg+str;
			}
			str.Format("(物理卡号)\r\n");
		}
		else
		{
			str.Format("(物理卡号长度错误)\r\n");
			strMsg = strMsg+strErr+str;
			m_Dlg->ProDecodeShow(strMsg);
			return;
		}
		i=20+j*9;
		if(IsValide(strErr,buffer+i,len-i,1))
		{
			str.Format("%02x(状态：",buffer[i]);
			strMsg = strMsg+str;
			if(buffer[i]==1)
			{
				str.Format("挂失)\r\n");
				strMsg = strMsg+str;
			}
			else if(buffer[i]==2)
			{
				str.Format("解挂)\r\n");
				strMsg = strMsg+str;
			}
			else
			{
				str.Format("未知)\r\n");
				strMsg = strMsg+str;
			}
		}
		else
		{
			str.Format("(状态错误)\r\n");
			strMsg = strMsg+strErr+str;
			m_Dlg->ProDecodeShow(strMsg);
			return;
		}
		i=i+1;
	}
	strMsg = cmdTimeMsg(strMsg,strErr,buffer+i,len-i);
	i=i+3;
    strMsg = crcMsg(strMsg,strErr,buffer+i,len-i);

	m_Dlg->ProDecodeShow(strMsg);


}
void	CProto_IEC104::Pro_Down_1_v2(CString strMsg,unsigned char *buffer,int len)
{
	CString str,strErr;
	if(buffer==0 || len==0)
	{
		str="无数据内容";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	
	if(IsValide(strErr,buffer,len,8))
	{
		strMsg = getEpCode(strMsg,buffer);
	}
	else
	{
		str.Format("(终端机器编码长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
    int	i=8;
	if(IsValide(strErr,buffer+i,len-i,8))
	{

		for(int i=8;i<16;i++)
		{
			str.Format("%02x ",buffer[i]);
			strMsg = strMsg+str;
		}
		str.Format("(计费模型ID)\r\n");
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(计费模型ID长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=16;
	if(IsValide(strErr,buffer+i,len-i,7))
	{
		strMsg=timeMsg(strMsg,buffer+i,"生效日期");
	}
	else
	{
		str.Format("(生效日期年错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}	
	i=i+7;
	if(IsValide(strErr,buffer+i,len-i,7))
	{
	    strMsg=timeMsg(strMsg,buffer+i,"失效日期");
	}
	else
	{
		str.Format("(失效日期年错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}	
	i=i+7;
	unsigned int v =0;
	float fvalue=0.0;
	if(IsValide(strErr,buffer+i,len-i,2))
	{
		v=(buffer[i+1]*0x100+buffer[i]);
         fvalue=v*0.01;
		str.Format("%02x %02x(预冻结金额：%0.2f)\r\n",buffer[i],buffer[i+1],fvalue);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(预冻结金额错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}	
	i=i+2;
	if(IsValide(strErr,buffer+i,len-i,2))
	{
		v =(buffer[i+1]*0x100+buffer[i]);
		fvalue=v*0.01;
		str.Format("%02x %02x(最小冻结金额：%0.2f)\r\n",buffer[i],buffer[i+1],fvalue);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(最小冻结金额错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+2;
	if(IsValide(strErr,buffer+i,len-i,2))
	{
		str.Format("%02x(时段数：%d\r\n",buffer[i],buffer[i]);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(时段数错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	//if(len != buffer[i]*9+35+28)
	//{
	//	str.Format("时段记录数据错误，数据长度不对"); 
	//	strMsg = strMsg+str;
		/*m_Dlg->ProDecodeShow(strMsg);
		return;*/
	//}
	int j=0;
	for(j=0;j<buffer[34];j++)
	{
		i=j*9+35;
		if(IsValide(strErr,buffer+i,len-i,4))
		{
			v =getInt4Small(buffer+i);
			str.Format("%02x %02x %02x %02x (开始时间：%d分)\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],v);
			strMsg = strMsg+str;
		}
		else
		{
			str.Format("(开始时间错误)\r\n");
			strMsg = strMsg+strErr+str;
			m_Dlg->ProDecodeShow(strMsg);
			return;
		}
		i=i+4;
		if(IsValide(strErr,buffer+i,len-i,4))
		{
			v =getInt4Small(buffer+i);
			str.Format("%02x %02x %02x %02x (结束时间：%d分)\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],v);
			strMsg = strMsg+str;
		}
		else
		{
			str.Format("(结束时间错误)\r\n");
			strMsg = strMsg+strErr+str;
			m_Dlg->ProDecodeShow(strMsg);
			return;
		}
		i=i+4;
		if(IsValide(strErr,buffer+i,len-i,1))
		{
			str.Format("%02x(标志：",buffer[i]);
			strMsg = strMsg+str;
			if(buffer[i]==1)
			{
				str.Format("尖时段)\r\n");
				strMsg = strMsg+str;
		    }
			else if(buffer[i]==2)
		 {
			 str.Format("峰时段)\r\n");
			 strMsg = strMsg+str;
		 }
			else if(buffer[i]==3)
		 {
			 str.Format("平时段)\r\n");
			 strMsg = strMsg+str;
		 }
			else if(buffer[i]==4)
		 {
			 str.Format("谷时段)\r\n");
			 strMsg = strMsg+str;
		 }
			else
			{
				str.Format("未知)\r\n");
				strMsg = strMsg+str;
			}
		}
		else
		{
			str.Format("(标志错误)\r\n");
			strMsg = strMsg+strErr+str;
			m_Dlg->ProDecodeShow(strMsg);
			return;
		}
		i++;
	}

	if(IsValide(strErr,buffer+i,len-i,4))
	{
		v =getInt4Small(buffer+i);
		fvalue=v*0.001;
		str.Format("%02x %02x %02x %02x (尖时段电价：%0.3f)\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],fvalue);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(尖时段电价错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+4;
	if(IsValide(strErr,buffer+i,len-i,4))
	{
		v =getInt4Small(buffer+i);
		fvalue=v*0.001;
		str.Format("%02x %02x %02x %02x (峰时段电价：%0.3f)\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],fvalue);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(峰时段电价错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+4;
	if(IsValide(strErr,buffer+i,len-i,4))
	{
		v =getInt4Small(buffer+i);
		fvalue=v*0.001;
		str.Format("%02x %02x %02x %02x (平时段电价：%0.3f)\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],fvalue);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(平时段电价错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+4;
	if(IsValide(strErr,buffer+i,len-i,4))
	{
		v =getInt4Small(buffer+i);
		fvalue=v*0.001;
		str.Format("%02x %02x %02x %02x (谷时段电价：%0.3f)\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],fvalue);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(谷时段电价错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+4;
	if(IsValide(strErr,buffer+i,len-i,4))
	{
		v =getInt4Small(buffer+i);
		fvalue=v*0.001;
		str.Format("%02x %02x %02x %02x (预约费率：%0.3f)\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],fvalue);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(预约费率错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+4;
	if(IsValide(strErr,buffer+i,len-i,4))
	{
		v =getInt4Small(buffer+i);
		fvalue=v*0.001;
		str.Format("%02x %02x %02x %02x (服务费：%0.3f)\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],fvalue);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(服务费错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+4;
	if(IsValide(strErr,buffer+i,len-i,4))
	{
		v =getInt4Small(buffer+i);
		fvalue=v*0.01;
		str.Format("%02x %02x %02x %02x(((告警金额：%0.2f)\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],fvalue);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(告警金额错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
    i=i+4;
    strMsg = cmdTimeMsg(strMsg,strErr,buffer+i,len-i);
	i=i+3;
    strMsg = crcMsg(strMsg,strErr,buffer+i,len-i);
    m_Dlg->ProDecodeShow(strMsg);
}

//私有充电桩下发充电桩运营时间下行数据
void	CProto_IEC104::Pro_Down_3(CString strMsg,unsigned char *buffer,int len)
{
	CString str,strErr;
	if(buffer==0 || len==0)
	{
		str="无数据内容";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	if(IsValide(strErr,buffer,len,8))
	{
		strMsg = getEpCode(strMsg,buffer);
	}
	else
	{
		str.Format("(终端机器编码长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
    int	i=8;
	if(IsValide(strErr,buffer+i,len-i,1))
	{
		str.Format("%02x(离线标志：",buffer[8]);
		strMsg = strMsg+str;
		if(buffer[8]==0)
		{
			str.Format("离线)\r\n");
			strMsg = strMsg+str;
		}
		else if(buffer[8]==1)
		{
			str.Format("运营)\r\n");
			strMsg = strMsg+str;
		}
		else 
		{
			str.Format("未知)\r\n");
			strMsg = strMsg+str;
		}
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(离线标志错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=9;
	unsigned int v =0;
	if(IsValide(strErr,buffer+i,len-i,4))
	{
		v=getInt4Small(buffer+i);
		str.Format("%02x %02x %02x %02x (开始时间：%d秒)\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],v);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(开始时间错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=13;
	if(IsValide(strErr,buffer+i,len-i,4))
	{
		v =getInt4Small(buffer+i);
		str.Format("%02x %02x %02x %02x (结束时间：%d秒)\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],v);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(结束时间错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+4;
	strMsg = cmdTimeMsg(strMsg,strErr,buffer+i,len-i);
	i=i+3;
    strMsg = crcMsg(strMsg,strErr,buffer+i,len-i);
	m_Dlg->ProDecodeShow(strMsg);

}
void	CProto_IEC104::Pro_Down_4(CString strMsg,unsigned char *buffer,int len)
{
	CString str,strErr;
	if(buffer==0 || len==0)
	{
		str="无数据内容";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	if(IsValide(strErr,buffer,len,8))
	{
		strMsg = getEpCode(strMsg,buffer);
	}
	else
	{
		str.Format("(终端机器编码长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
    int	i=8;
	if(IsValide(strErr,buffer+i,len-i,1))
	{
		str.Format("%02x(充电桩接口标识)\r\n",buffer[8]);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(充电桩接口标识错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=9;
	if(IsValide(strErr,buffer+i,len-i,1))
	{
		str.Format("%02x(鉴权成功标识：",buffer[9]);
		strMsg = strMsg+str;
		switch(buffer[8])
		{
		case 1:
		 {
			 str.Format("鉴权成功标识)\r\n");
			 strMsg = strMsg+str;
		 }
		 break;
		case 0:
		 {
			 str.Format("鉴权失败)\r\n");
			 strMsg = strMsg+str;
		 }
		 break;
		default:
		 {
			 str.Format("未知)\r\n");
			 strMsg = strMsg+str;
		 }
		 break;
		}
	}
	else
	{
		str.Format("(鉴权成功标识错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=10;
	if(IsValide(strErr,buffer+i,len-i,2))
	{
	
		str.Format("%02x%02x(鉴权失败原因)\r\n",buffer[i],buffer[i+1]);
		strMsg = strMsg+str;
	}

	i=12;
	unsigned int v =0;
	if(IsValide(strErr,buffer+i,len-i,4))
	{
		v=getInt4Small(buffer+i);
		str.Format("%02x %02x %02x %02x (剩余金额：%0.2f)\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],v*0.01);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(剩余金额错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=14;
	if(IsValide(strErr,buffer+i,len-i,6))
	{
		str.Format("%02x%02x%02x%02x%02x%02x(用户帐号)\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],buffer[i+4],buffer[i+5]);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(用户帐号错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}

	i=i+6;
	strMsg = cmdTimeMsg(strMsg,strErr,buffer+i,len-i);
	i=i+3;
    strMsg = crcMsg(strMsg,strErr,buffer+i,len-i);

	m_Dlg->ProDecodeShow(strMsg);
}

void	CProto_IEC104::Pro_Down_11_v2(CString strMsg,unsigned char *buffer,int len)
{
	CString str,strErr;
	if(buffer==0 || len==0)
	{
		str="无数据内容";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	if(IsValide(strErr,buffer,len,8))
	{
		strMsg = getEpCode(strMsg,buffer);
	}
	else
	{
		str.Format("(终端机器编码长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
    int	i=8;
	if(IsValide(strErr,buffer+i,len-i,1))
	{
		str.Format("%02x(充电桩接口标识)\r\n",buffer[8]);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(充电桩接口标识错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i++;
	if(IsValide(strErr,buffer+i,len-i,1))
	{
		str.Format("%02x(鉴权成功标识：",buffer[i]);
		strMsg = strMsg+str;
		switch(buffer[i])
		{
		case 1:
		 {
			 str.Format("鉴权成功标识)\r\n");
			 strMsg = strMsg+str;
		 }
		 break;
		case 0:
		 {
			 str.Format("鉴权失败)\r\n");
			 strMsg = strMsg+str;
		 }
		 break;
		default:
		 {
			 str.Format("未知)\r\n");
			 strMsg = strMsg+str;
		 }
		 break;
		}
	}
	else
	{
		str.Format("(鉴权成功标识错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i++;
	if(IsValide(strErr,buffer+i,len-i,2))
	{
	
		str.Format("%02x%02x(鉴权失败原因)\r\n",buffer[i],buffer[i+1]);
		strMsg = strMsg+str;
	}

	i=i+2;
	unsigned int v =0;
	if(IsValide(strErr,buffer+i,len-i,4))
	{
		v=getInt4Small(buffer+i);
		str.Format("%02x %02x %02x %02x (剩余金额：%0.2f)\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],v*0.01);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(剩余金额错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+4;
	if(IsValide(strErr,buffer+i,len-i,6))
	{
		str.Format("%02x%02x%02x%02x%02x%02x(用户帐号)\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],buffer[i+4],buffer[i+5]);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(用户帐号错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	
	}
	i=i+6;
	if(IsValide(strErr,buffer+i,len-i,32))
	{
		CString s,s1;
		for(int j=i;j<i+32;j++)
		{
			str.Format("%02x ",buffer[j]);
			strMsg = strMsg+str;

			s1.Format("%c",buffer[j]);
			s = s+s1;
		}
		str.Format("(用户密码：%s)",s);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(用户密码长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+32;
	if(IsValide(strErr,buffer+i,len-i,32))
	{
		CString s,s1;
		for(int j=i;j<i+32;j++)
		{
			str.Format("%02x ",buffer[j]);
			strMsg = strMsg+str;

			s1.Format("%c",buffer[j]);
			s = s+s1;
		}
		str.Format("(CPU卡号：%s)",s);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(CPU卡号长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+32;
    if(IsValide(strErr,buffer+i,len-i,2))
	{
		str.Format("%02x%02x(合格电桩使用伙伴Id:%d)\r\n",buffer[i],buffer[i+1],getInt2(buffer+i));
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(合格电桩使用伙伴Id错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	
	}
	i=i+2;
	strMsg = cmdTimeMsg(strMsg,strErr,buffer+i,len-i);
	i=i+3;
    strMsg = crcMsg(strMsg,strErr,buffer+i,len-i);

	m_Dlg->ProDecodeShow(strMsg);
}


void	CProto_IEC104::Pro_Down_13_v2(CString strMsg,unsigned char *buffer,int len)
{
	CString str,strErr;
	if(buffer==0 || len==0)
	{
		str="无数据内容";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	if(IsValide(strErr,buffer,len,8))
	{
		strMsg = getEpCode(strMsg,buffer);
	}
	else
	{
		str.Format("(终端机器编码长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
    int	i=8;
	if(IsValide(strErr,buffer+i,len-i,1))
	{
		str.Format("%02x(充电桩接口标识)\r\n",buffer[8]);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(充电桩接口标识错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i++;
	
	if(IsValide(strErr,buffer+i,len-i,32))
	{
		CString s,s1;
		for(int j=i;j<i+32;j++)
		{
			str.Format("%02x ",buffer[j]);
			strMsg = strMsg+str;

			s1.Format("%c",buffer[j]);
			s = s+s1;
		}
		str.Format("(内卡号：%s)",s);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(内卡号长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+32;
	if(IsValide(strErr,buffer+i,len-i,20))
	{
		CString s,s1;
		for(int j=i;j<i+20;j++)
		{
			str.Format("%02x ",buffer[j]);
			strMsg = strMsg+str;

			s1.Format("%c",buffer[j]);
			s = s+s1;
		}
		str.Format("(外卡号：%s)",s);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(外卡号长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+20;
	if(IsValide(strErr,buffer+i,len-i,1))
	{
		str.Format("%02x(卡状态)\r\n",buffer[i]);
		strMsg = strMsg+str;

		switch(buffer[i])
		{
		case 1:
		 {
			 str.Format("正常)\r\n");
			 strMsg = strMsg+str;
		 }
		 break;
		case 2:
		 {
			 str.Format("挂失)\r\n");
			 strMsg = strMsg+str;
		 }
		 break;
		default:
		 {
			 str.Format("未知)\r\n");
			 strMsg = strMsg+str;
		 }
		 break;
		}

	}
	else
	{
		str.Format("(卡状态错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i++;
	unsigned int v =0;
	if(IsValide(strErr,buffer+i,len-i,4))
	{
		v=getInt4Small(buffer+i);
		str.Format("%02x %02x %02x %02x (余额：%0.2f)\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],v*0.01);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(余额错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+4;
	if(IsValide(strErr,buffer+i,len-i,1))
	{
		str.Format("%02x(成功标识)\r\n",buffer[i]);
		strMsg = strMsg+str;

		switch(buffer[i])
		{
		case 1:
		 {
			 str.Format("成功)\r\n");
			 strMsg = strMsg+str;
		 }
		 break;
		case 0:
		 {
			 str.Format("失败)\r\n");
			 strMsg = strMsg+str;
		 }
		 break;
		default:
		 {
			 str.Format("未知)\r\n");
			 strMsg = strMsg+str;
		 }
		 break;
		}

	}
	else
	{
		str.Format("(成功标识错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i++;
    if(IsValide(strErr,buffer+i,len-i,2))
	{
		str.Format("%02x(错误编码:%d)\r\n",buffer[i],buffer[i+1],getInt2(buffer+i));
		strMsg = strMsg+str;

		switch(buffer[i])
		{
		case 1:
		 {
			 str.Format("平台无法连接，请求失败)\r\n");
			 strMsg = strMsg+str;
		 }
		 break;
		case 2:
		 {
			 str.Format("请求超时)\r\n");
			 strMsg = strMsg+str;
		 }
		 break;
	 case 3:
		 {
			 str.Format("没找到桩)\r\n");
			 strMsg = strMsg+str;
		 }
		 break;
	 case 4:
		 {
			 str.Format("请求失败)\r\n");
			 strMsg = strMsg+str;
		 }
		 break;
	 case 5:
		 {
			 str.Format("没有权限)\r\n");
			 strMsg = strMsg+str;
		 }
		 break;
	 case 6:
		 {
			 str.Format("无效卡)\r\n");
			 strMsg = strMsg+str;
		 }
		 break;
	 case 7:
		 {
			 str.Format("挂失卡)\r\n");
			 strMsg = strMsg+str;
		 }
		 break;
	 case 8:
		 {
			 str.Format("没有绑定用户)\r\n");
			 strMsg = strMsg+str;
		 }
		 break;
	 case 9:
		 {
			 str.Format("密码不对)\r\n");
			 strMsg = strMsg+str;
		 }
		 break;
		default:
		 {
			 str.Format("未知)\r\n");
			 strMsg = strMsg+str;
		 }
		 break;
		}

	}
	else
	{
		str.Format("(错误编码错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}

	i=i+2;
	strMsg = cmdTimeMsg(strMsg,strErr,buffer+i,len-i);
	i=i+3;
    strMsg = crcMsg(strMsg,strErr,buffer+i,len-i);

	m_Dlg->ProDecodeShow(strMsg);
}

void	CProto_IEC104::Pro_Down_5(CString strMsg,unsigned char *buffer,int len)
{
	CString str,strErr;
	if(buffer==0 || len==0)
	{
		str="无数据内容";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	
	if(IsValide(strErr,buffer,len,8))
	{
		strMsg = getEpCode(strMsg,buffer);
	}
	else
	{
		str.Format("(终端机器编码长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
   int	i=8;
	if(IsValide(strErr,buffer+i,len-i,1))
	{
		str.Format("%02x(充电桩接口标识)\r\n",buffer[8]);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(充电桩接口标识错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i++;
	unsigned int v =0;
	if(IsValide(strErr,buffer+i,len-i,4))
	{
		v=getInt4Small(buffer+i);
		str.Format("%02x %02x %02x %02x (扣款金额：%0.2f)\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],v*0.01);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(扣款金额错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+4;
	if(IsValide(strErr,buffer+i,len-i,4))
	{
		v=getInt4Small(buffer+i);
		str.Format("%02x %02x %02x %02x (账户余额：%0.2f)\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],v*0.01);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(账户余额错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+4;
	if(IsValide(strErr,buffer+i,len-i,1))
	{
		str.Format("%02x(扣款成功标志：",buffer[i]);
		strMsg = strMsg+str;
		switch(buffer[i])
		{
		case 1:
			{
				str.Format("成功)\r\n");
				strMsg = strMsg+str;
			}
			break;
		case 0:
			{
				str.Format("失败)\r\n");
				strMsg = strMsg+str;
			}
			break;
		default:
			{
				str.Format("其它)\r\n");
				strMsg = strMsg+str;
			}
			break;
		}
	}
	else
	{
		str.Format("(扣款成功标志错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i++;
    if(IsValide(strErr,buffer+i,len-i,1))
	{
		str.Format("%02x(扣款失败原因)",buffer[i]);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(扣款成功标志错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}

	i=i+1;
	strMsg = cmdTimeMsg(strMsg,strErr,buffer+i,len-i);
	i=i+3;
    strMsg = crcMsg(strMsg,strErr,buffer+i,len-i);
	m_Dlg->ProDecodeShow(strMsg);
}
void	CProto_IEC104::Pro_Down_6(CString strMsg,unsigned char *buffer,int len)
{
	CString str,strErr;
	if(buffer==0 || len==0)
	{
		str="无数据内容";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}

	if(IsValide(strErr,buffer,len,8))
	{
		strMsg = getEpCode(strMsg,buffer);
	}
	else
	{
		str.Format("(终端机器编码长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
   int	i=8;
	if(IsValide(strErr,buffer+i,len-i,1))
	{
		str.Format("%02x(充电桩接口标识)\r\n",buffer[8]);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(充电桩接口标识错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i++;
	if(IsValide(strErr,buffer+i,len-i,1))
	{
		str.Format("%02x(续约标识)\r\n",buffer[i]);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(续约标识错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i++;
    if(IsValide(strErr,buffer+i,len-i,7))
	{	
		strMsg = timeMsg(strMsg,buffer+i,"预约开始时间");
    }
	else
	{
		str.Format("(预约开始时间长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+7;
	if(IsValide(strErr,buffer+i,len-i,2))
	{
		int v=(buffer[i+1]*0x100+buffer[i]);
		str.Format("%02x %02x(预约等待时间：%d)\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],v);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(预约等待时间错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
    i=i+2;
	if(IsValide(strErr,buffer+i,len-i,6))
	{
        strMsg = getPhone(strMsg,buffer+i);
	}
	else
	{
		str.Format("(账号(手机号长度错误))\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
    i=i+6;
	if(IsValide(strErr,buffer+i,len-i,8))
	{

		str.Format("%02x%02x%02x%02x%02x%02x%02x%02x((卡号)\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],buffer[i+4],buffer[i+5],buffer[i+6],buffer[i+7]);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(卡号长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+8;
	if(IsValide(strErr,buffer+i,len-i,32))
	{
		CString s,s1;
		for(int j=i;j<i+32;j++)
		{
			str.Format("%02x ",buffer[j]);
			strMsg = strMsg+str;

			s1.Format("%c",buffer[j]);
			s = s+s1;
		}
		str.Format("(预约号MD5-32：%s)",s);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("预约号MD5-32长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+32;
	if(IsValide(strErr,buffer+i,len-i,16))
	{
		CString s,s1;
		for(int j=i;j<i+16;j++)
		{
			str.Format("%02x ",buffer[j]);
			strMsg = strMsg+str;

			s1.Format("%c",buffer[j]);
			s = s+s1;
		}
		str.Format("(车牌号：%s)",s);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("车牌号长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}

	i=i+16;
	strMsg = cmdTimeMsg(strMsg,strErr,buffer+i,len-i);
	i=i+3;
    strMsg = crcMsg(strMsg,strErr,buffer+i,len-i);
	m_Dlg->ProDecodeShow(strMsg);
}


void	CProto_IEC104::Pro_Down_32_v2(CString strMsg,unsigned char *buffer,int len)
{
	CString str,strErr;
	if(buffer==0 || len==0)
	{
		str="无数据内容";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}

	if(IsValide(strErr,buffer,len,8))
	{
		strMsg = getEpCode(strMsg,buffer);
	}
	else
	{
		str.Format("(终端机器编码长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
    int	i=8;
	if(IsValide(strErr,buffer+i,len-i,1))
	{
		str.Format("%02x(充电桩接口标识)\r\n",buffer[i]);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(充电桩接口标识错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i++;
	if(IsValide(strErr,buffer+i,len-i,6))
	{

		str.Format("%02x%02x%02x%02x%02x%02x((预约号)\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],buffer[i+4],buffer[i+5]);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(预约号长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+6;
	if(IsValide(strErr,buffer+i,len-i,1))
	{
		str.Format("%02x(预约标识)\r\n",buffer[i]);
		strMsg = strMsg+str;
		switch(buffer[i])
		{
		case 1:
			{
				str.Format("续约)\r\n");
				strMsg = strMsg+str;
			}
			break;
		case 0:
			{
				str.Format("预约)\r\n");
				strMsg = strMsg+str;
			}
			break;
		default:
			{
				str.Format("其它)\r\n");
				strMsg = strMsg+str;
			}
			break;
		}
	}
	else
	{
		str.Format("(预约标识错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i++;
	if(IsValide(strErr,buffer+i,len-i,1))
	{
		str.Format("%02x(处理结果)\r\n",buffer[i]);
		strMsg = strMsg+str;
		switch(buffer[i])
		{
		case 1:
			{
				str.Format("处理成功)\r\n");
				strMsg = strMsg+str;
			}
			break;
		case 2:
			{
				str.Format("数据不存在)\r\n");
				strMsg = strMsg+str;
			}
			break;
		case 3:
			{
				str.Format("已经处理)\r\n");
				strMsg = strMsg+str;
			}
			break;
		case 4:
			{
				str.Format("桩编号不存在)\r\n");
				strMsg = strMsg+str;
			}
			break;
		default:
			{
				str.Format("其它)\r\n");
				strMsg = strMsg+str;
			}
			break;
		}
	}
	else
	{
		str.Format("(处理结果错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+1;
	strMsg = cmdTimeMsg(strMsg,strErr,buffer+i,len-i);
	i=i+3;
    strMsg = crcMsg(strMsg,strErr,buffer+i,len-i);
	m_Dlg->ProDecodeShow(strMsg);
}


void	CProto_IEC104::Pro_Down_31_v2(CString strMsg,unsigned char *buffer,int len)
{
	CString str,strErr;
	if(buffer==0 || len==0)
	{
		str="无数据内容";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}

	if(IsValide(strErr,buffer,len,8))
	{
		strMsg = getEpCode(strMsg,buffer);
	}
	else
	{
		str.Format("(终端机器编码长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
   int	i=8;
	if(IsValide(strErr,buffer+i,len-i,1))
	{
		str.Format("%02x(充电桩接口标识)\r\n",buffer[8]);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(充电桩接口标识错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i++;
	if(IsValide(strErr,buffer+i,len-i,1))
	{
		str.Format("%02x(续约标识)\r\n",buffer[i]);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(续约标识错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i++;
    if(IsValide(strErr,buffer+i,len-i,7))
	{	
		strMsg = timeMsg(strMsg,buffer+i,"预约开始时间");
    }
	else
	{
		str.Format("(预约开始时间长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+7;
	if(IsValide(strErr,buffer+i,len-i,2))
	{
		int v=getInt2(buffer+i);
		str.Format("%02x %02x(预约等待时间：%d)\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],v);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(预约等待时间错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
    i=i+2;
	if(IsValide(strErr,buffer+i,len-i,6))
	{

		strMsg = getPhone(strMsg,buffer+i);
	}
	else
	{
		str.Format("(账号(手机号长度错误))\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
    i=i+6;
	if(IsValide(strErr,buffer+i,len-i,8))
	{

		str.Format("%02x%02x%02x%02x%02x%02x%02x%02x((卡号)\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],buffer[i+4],buffer[i+5],buffer[i+6],buffer[i+7]);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(卡号长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+8;
	if(IsValide(strErr,buffer+i,len-i,6))
	{

		str.Format("%02x%02x%02x%02x%02x%02x((预约号)\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],buffer[i+4],buffer[i+5]);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(预约号长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+6;
	if(IsValide(strErr,buffer+i,len-i,16))
	{
		CString s,s1;
		for(int j=i;j<i+16;j++)
		{
			str.Format("%02x ",buffer[j]);
			strMsg = strMsg+str;

			s1.Format("%c",buffer[j]);
			s = s+s1;
		}
		str.Format("(车牌号：%s)",s);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("车牌号长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+16;
	strMsg = cmdTimeMsg(strMsg,strErr,buffer+i,len-i);
	i=i+3;
    strMsg = crcMsg(strMsg,strErr,buffer+i,len-i);
	m_Dlg->ProDecodeShow(strMsg);
}

void	CProto_IEC104::Pro_Down_7(CString strMsg,unsigned char *buffer,int len)
{
	CString str,strErr;
	if(buffer==0 || len==0)
	{
		str="无数据内容";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	if(IsValide(strErr,buffer,len,8))
	{
		strMsg = getEpCode(strMsg,buffer);
	}
	else
	{
		str.Format("(终端机器编码长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
    int	i=8;
	if(IsValide(strErr,buffer+i,len-i,1))
	{
		str.Format("%02x(充电桩接口标识)\r\n",buffer[8]);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(充电桩接口标识错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i++;
	if(IsValide(strErr,buffer+i,len-i,32))
	{
		CString s,s1;
		for(int j=i;j<i+32;j++)
		{
			str.Format("%02x ",buffer[j]);
			strMsg = strMsg+str;

			s1.Format("%c",buffer[j]);
			s = s+s1;
		}
		str.Format("(预约号MD5-32：%s)",s);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("预约号MD5-32长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}

	i=i+32;
	strMsg = cmdTimeMsg(strMsg,strErr,buffer+i,len-i);
	i=i+3;
    strMsg = crcMsg(strMsg,strErr,buffer+i,len-i);
	m_Dlg->ProDecodeShow(strMsg);
}

void	CProto_IEC104::Pro_Down_33_v2(CString strMsg,unsigned char *buffer,int len)
{
	CString str,strErr;
	if(buffer==0 || len==0)
	{
		str="无数据内容";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	if(IsValide(strErr,buffer,len,8))
	{
		strMsg = getEpCode(strMsg,buffer);
	}
	else
	{
		str.Format("(终端机器编码长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
    int	i=8;
	if(IsValide(strErr,buffer+i,len-i,1))
	{
		str.Format("%02x(充电桩接口标识)\r\n",buffer[8]);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(充电桩接口标识错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i++;
	if(IsValide(strErr,buffer+i,len-i,6))
	{

		str.Format("%02x%02x%02x%02x%02x%02x((预约号)\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],buffer[i+4],buffer[i+5]);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(预约号长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}

	i=i+6;
	strMsg = cmdTimeMsg(strMsg,strErr,buffer+i,len-i);
	i=i+3;
    strMsg = crcMsg(strMsg,strErr,buffer+i,len-i);
	m_Dlg->ProDecodeShow(strMsg);
}

void	CProto_IEC104::Pro_Down_8(CString strMsg,unsigned char *buffer,int len)
{
	CString str,strErr;
	if(buffer==0 || len==0)
	{
		str="无数据内容";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	
	if(IsValide(strErr,buffer,len,8))
	{
		strMsg = getEpCode(strMsg,buffer);
	}
	else
	{
		str.Format("(终端机器编码长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
    int	i=8;
	if(IsValide(strErr,buffer+i,len-i,1))
	{
		str.Format("%02x(充电接口)\r\n",buffer[8]);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(充电接口错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=9;
	if(IsValide(strErr,buffer+i,len-i,6))
	{
		strMsg = getPhone(strMsg,buffer+i);
	}
	else
	{
		str.Format("(手机号长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}

	i=15;
	unsigned int v =0;
	if(IsValide(strErr,buffer+i,len-i,4))
	{
		v=getInt4Small(buffer+i);
		str.Format("%02x %02x %02x %02x (余额：%d)\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],v);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(余额错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=19;
	if(IsValide(strErr,buffer+i,len-i,4))
	{
		v =getInt4Small(buffer+i);
		str.Format("%02x %02x %02x %02x (最小充电金额：%d)\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],v);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(最小充电金额错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=23;
	if(IsValide(strErr,buffer+i,len-i,1))
	{
		str.Format("%02x(二维码方式：",buffer[i]);
		strMsg = strMsg+str;
		switch(buffer[i])
		{
		case 1:
			{
				str.Format("是二维码充电)\r\n");
				strMsg = strMsg+str;
			}
			break;
		default:
			{
				str.Format("其它)\r\n");
				strMsg = strMsg+str;
			}
			break;
		}
	}
	else
	{
		str.Format("(二维码方式错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i++;
	if(m_Dlg->getProVersion()==13)
	{
		if(IsValide(strErr,buffer+i,len-i,4))
		{
		double	v =getInt4Small(buffer+i);
			str.Format("%02x %02x %02x %02x (预充金额：%f)\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],v*0.01);
			strMsg = strMsg+str;
		}
		else
		{
			str.Format("(预充金额错误)\r\n");
			strMsg = strMsg+strErr+str;
			m_Dlg->ProDecodeShow(strMsg);
			return;
		}
		i=i+4;
		if(IsValide(strErr,buffer+i,len-i,32))
		{
			for(int k=0;k<32;k++)
			{
				str.Format("%02x",buffer[i+k]); 
				strMsg = strMsg+str;
			}

			str.Format("(用户密码)\r\n"); 
			strMsg = strMsg+str;
		}
			i=i+32;
	}
	
	strMsg = cmdTimeMsg(strMsg,strErr,buffer+i,len-i);
	i=i+3;
    strMsg = crcMsg(strMsg,strErr,buffer+i,len-i);
	m_Dlg->ProDecodeShow(strMsg);
}

void	CProto_IEC104::Pro_Down_41_v2(CString strMsg,unsigned char *buffer,int len)
{
	CString str,strErr;
	if(buffer==0 || len==0)
	{
		str="无数据内容";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}

	if(IsValide(strErr,buffer,len,8))
	{
		strMsg = getEpCode(strMsg,buffer);
	}
	else
	{
		str.Format("(终端机器编码长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
int	i=8;
	if(IsValide(strErr,buffer+i,len-i,1))
	{
		str.Format("%02x(充电接口)\r\n",buffer[8]);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(充电接口错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=9;
	if(IsValide(strErr,buffer+i,len-i,6))
	{
		strMsg = getPhone(strMsg,buffer+i);
	}
	else
	{
		str.Format("(手机号长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}

	i=15;
	unsigned int v =0;
	if(IsValide(strErr,buffer+i,len-i,4))
	{
		v=getInt4Small(buffer+i);
		str.Format("%02x %02x %02x %02x (余额：%d)\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],v);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(余额错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=19;
	if(IsValide(strErr,buffer+i,len-i,4))
	{
		v=getInt4Small(buffer+i);
		str.Format("%02x %02x %02x %02x (最小充电金额：%d)\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],v);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(最小充电金额错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=23;
	if(IsValide(strErr,buffer+i,len-i,1))
	{
		str.Format("%02x(二维码方式：",buffer[i]);
		strMsg = strMsg+str;
		switch(buffer[i])
		{
		case 1:
			{
				str.Format("是二维码充电)\r\n");
				strMsg = strMsg+str;
			}
			break;
		default:
			{
				str.Format("其它)\r\n");
				strMsg = strMsg+str;
			}
			break;
		}
	}
	else
	{
		str.Format("(二维码方式错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i++;
	if(IsValide(strErr,buffer+i,len-i,1))
	{
		str.Format("%02x(扣费方式：",buffer[i]);
		strMsg = strMsg+str;
		switch(buffer[i])
		{
		case 1:
			{
				str.Format("预冻结)\r\n");
				strMsg = strMsg+str;
			}
			break;
		case 2:
			{
				str.Format("后付费)\r\n");
				strMsg = strMsg+str;
			}
			break;
		default:
			{
				str.Format("其它)\r\n");
				strMsg = strMsg+str;
			}
			break;
		}
	}
	else
	{
		str.Format("(二维码方式错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i++;
	if(IsValide(strErr,buffer+i,len-i,4))
	{
		   double	v=getInt4Small(buffer+i);

			str.Format("%02x %02x %02x %02x (预充金额：%f)\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],v*0.01);
			strMsg = strMsg+str;
	}
	else
	{
			str.Format("(预充金额错误)\r\n");
			strMsg = strMsg+strErr+str;
			m_Dlg->ProDecodeShow(strMsg);
			return;
	}
	i=i+4;
	if(IsValide(strErr,buffer+i,len-i,32))
	{
			CString s,s1;
			for(int k=0;k<32;k++)
			{
				str.Format("%02x",buffer[i+k]); 
				strMsg = strMsg+str;
			    s1.Format("%c",buffer[i+k]);
			    s = s+s1;
		   }
		   str.Format("用户密码MD5-32：%s)",s);
		   strMsg = strMsg+str;
		
	}
	else
	{
			str.Format("(用户密码MD5-32错误)\r\n");
			strMsg = strMsg+strErr+str;
			m_Dlg->ProDecodeShow(strMsg);
			return;
	}
	i=i+32;
	if(IsValide(strErr,buffer+i,len-i,16))
	{
			for(int k=0;k<16;k++)
			{
				str.Format("%02x",buffer[i+k]); 
				strMsg = strMsg+str;
			}

			str.Format("(流水号)\r\n"); 
			strMsg = strMsg+str;
	}
	else
	{
			str.Format("(流水号错误)\r\n");
			strMsg = strMsg+strErr+str;
			m_Dlg->ProDecodeShow(strMsg);
			return;
	}
	i=i+16;
	strMsg = cmdTimeMsg(strMsg,strErr,buffer+i,len-i);
	i=i+3;
    strMsg = crcMsg(strMsg,strErr,buffer+i,len-i);
	m_Dlg->ProDecodeShow(strMsg);
}

void	CProto_IEC104::Pro_Down_42_v2(CString strMsg,unsigned char *buffer,int len)
{
	CString str,strErr;
	if(buffer==0 || len==0)
	{
		str="无数据内容";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	

	if(IsValide(strErr,buffer,len,8))
	{
		strMsg = getEpCode(strMsg,buffer);
	}
	else
	{
		str.Format("(终端机器编码长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
    int	i=8;
	if(IsValide(strErr,buffer+i,len-i,1))
	{
		str.Format("%02x(充电接口)\r\n",buffer[8]);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(充电接口错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=9;
	
	if(IsValide(strErr,buffer+i,len-i,16))
	{
			for(int k=0;k<16;k++)
			{
				str.Format("%02x",buffer[i+k]); 
				strMsg = strMsg+str;
			}

			str.Format("(流水号)\r\n"); 
			strMsg = strMsg+str;
	}
	i=i+16;
	if(IsValide(strErr,buffer+i,len-i,2))
	{
		str.Format("%02x%02x(处理结果：%d)",buffer[i],buffer[i+1],buffer[i]+buffer[i+1]*0x100);
		strMsg = strMsg+str;
		switch(buffer[i])
		{
		case 1:
			{
				str.Format("处理成功)\r\n");
				strMsg = strMsg+str;
			}
			break;
		case 2:
			{
				str.Format("已经处理)\r\n");
				strMsg = strMsg+str;
			}
			break;
		case 3:
			{
				str.Format("不存在)\r\n");
				strMsg = strMsg+str;
			}
			break;
		default:
			{
				str.Format("未知)\r\n");
				strMsg = strMsg+str;
			}
			break;
		}
	}
	else
	{
		str.Format("(处理结果错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}

	i=i+2;
	strMsg = cmdTimeMsg(strMsg,strErr,buffer+i,len-i);
	i=i+3;
    strMsg = crcMsg(strMsg,strErr,buffer+i,len-i);
	
	m_Dlg->ProDecodeShow(strMsg);
}


void	CProto_IEC104::Pro_Down_9(CString strMsg,unsigned char *buffer,int len)
{

	CString str,strErr;
	if(m_Dlg->getProVersion()==13)
	{
		str="版本选择错误";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	if(buffer==0 || len==0)
	{
		str="无数据内容";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	m_Dlg->ProDecodeShow(strMsg);
}
void	CProto_IEC104::Pro_Down_10(CString strMsg,unsigned char *buffer,int len)
{
	CString str,strErr;
	if(buffer==0 || len==0)
	{
		str="无数据内容";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	m_Dlg->ProDecodeShow(strMsg);
}
void	CProto_IEC104::Pro_Down_11(CString strMsg,unsigned char *buffer,int len)
{
	CString str,strErr;
	if(buffer==0 || len==0)
	{
		str="无数据内容";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	m_Dlg->ProDecodeShow(strMsg);
}
void	CProto_IEC104::Pro_Down_12(CString strMsg,unsigned char *buffer,int len)
{
	CString str,strErr;
	if(buffer==0 || len==0)
	{
		str="无数据内容";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	m_Dlg->ProDecodeShow(strMsg);
}
void	CProto_IEC104::Pro_Down_13(CString strMsg,unsigned char *buffer,int len)
{
		CString str,strErr;
	if(buffer==0 || len==0)
	{
		str="无数据内容";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	m_Dlg->ProDecodeShow(strMsg);
}
void	CProto_IEC104::Pro_Down_14(CString strMsg,unsigned char *buffer,int len)
{
	CString str,strErr;
	if(buffer==0 || len==0)
	{
		str="无数据内容";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	
	if(IsValide(strErr,buffer,len,8))
	{
		strMsg = getEpCode(strMsg,buffer);
	}
	else
	{
		str.Format("(终端机器编码长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	int	i=8;
	if(IsValide(strErr,buffer+i,len-i,1))
	{
		str.Format("%02x (充电接口标识)\r\n",buffer[8]);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(充电接口标识错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=9;
	if(IsValide(strErr,buffer+i,len-i,8))
	{
		str.Format("%02x%02x%02x%02x%02x%02x%02x%02x(物理卡号)\r\n",buffer[9],buffer[10],buffer[11],buffer[12],buffer[13],buffer[14],buffer[15],buffer[16]);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(物理卡号错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=17;
	if(IsValide(strErr,buffer+i,len-i,1))
	{
		str.Format("%02x (记录条数)\r\n",buffer[17]);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(记录条数错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}

	int j=0;

	for(j=0;j<buffer[17];j++)
	{
		i=j*50+18;
		unsigned int  v =0;
		if(IsValide(strErr,buffer+i,len-i,2))
		{
			v=(buffer[i+1]*0x100+buffer[i]);
			str.Format("%02x %02x(用户卡脱机交易序号：%d)\r\n",buffer[i],buffer[i+1],v);
			strMsg = strMsg+str;
		}
		else
		{
			str.Format("(用户卡脱机交易序号错误)\r\n");
			strMsg = strMsg+strErr+str;
			m_Dlg->ProDecodeShow(strMsg);
			return;
		}

		i=i+2;

		if(IsValide(strErr,buffer+i,len-i,16))
		{

			for(int k=0;k<16;k++)
		 {
			 str.Format("%02x",buffer[i+k]); 
			 strMsg = strMsg+str;

		 }

			str.Format("(交易流水号)\r\n"); 
			strMsg = strMsg+str;
		}
		else
		{
			str.Format("(交易流水号错误)\r\n");
			strMsg = strMsg+strErr+str;
			m_Dlg->ProDecodeShow(strMsg);
			return;
		}
		i=i+16;
		if(IsValide(strErr,buffer+i,len-i,4))
		{
			v=getInt4Small(buffer+i);
			str.Format("%02x %02x %02x %02x (交易金额：%d)\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],v);
			strMsg = strMsg+str;
		}
		else
		{
			str.Format("(交易金额错误)\r\n");
			strMsg = strMsg+strErr+str;
			m_Dlg->ProDecodeShow(strMsg);
			return;
		}
		i=i+4;
		if(IsValide(strErr,buffer+i,len-i,6))
		{
			str.Format("%02x%02x%02x%02x%02x%02x(终端机编号)\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],buffer[i+4],buffer[i+5]);
			strMsg = strMsg+str;
		}
		else
		{
			str.Format("(终端机编号错误)\r\n");
			strMsg = strMsg+strErr+str;
			m_Dlg->ProDecodeShow(strMsg);
			return;
		}
		i=i+6;
		if(IsValide(strErr,buffer+i,len-i,4))
		{
			v=getInt4Small(buffer+i);
			str.Format("%02x%02x%02x%02x(终端机交易序列号：%d)\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],v);
			strMsg = strMsg+str;
		}
		else
		{
			str.Format("(终端机交易序列号错误)\r\n");
			strMsg = strMsg+strErr+str;
			m_Dlg->ProDecodeShow(strMsg);
			return;
		}
		i=i+4;
		if(IsValide(strErr,buffer+i,len-i,7))
		{
			strMsg = timeMsg(strMsg,buffer+i,"生效日期");
		}
		else
		{
			str.Format("(生效日期年错误)\r\n");
			strMsg = strMsg+strErr+str;
			m_Dlg->ProDecodeShow(strMsg);
			return;
		}		
		i=i+7;
		if(IsValide(strErr,buffer+i,len-i,7))
		{
			strMsg = timeMsg(strMsg,buffer+i,"失效日期");
		}
		else
		{
			str.Format("(失效日期年错误)\r\n");
			strMsg = strMsg+strErr+str;
			m_Dlg->ProDecodeShow(strMsg);
			return;
		}		

		i=i+7;
		if(IsValide(strErr,buffer+i,len-i,4))
		{
			v=getInt4Small(buffer+i);
			str.Format("%02x %02x %02 x%02x(GMAC：%d)\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],v);
			strMsg = strMsg+str;
		}
		else
		{
			str.Format("(GMAC错误)\r\n");
			strMsg = strMsg+strErr+str;
			m_Dlg->ProDecodeShow(strMsg);
			return;
		}
		i=i+4;
	}

	
	strMsg = cmdTimeMsg(strMsg,strErr,buffer+i,len-i);
	i=i+3;
    strMsg = crcMsg(strMsg,strErr,buffer+i,len-i);

	m_Dlg->ProDecodeShow(strMsg);
	}
void	CProto_IEC104::Pro_Down_17(CString strMsg,unsigned char *buffer,int len)
{
	CString str,strErr;
	if(buffer==0 || len==0)
	{
		str="无数据内容";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	m_Dlg->ProDecodeShow(strMsg);
}
void	CProto_IEC104::Pro_Down_18(CString strMsg,unsigned char *buffer,int len)
{
	CString str,strErr;
	if(buffer==0 || len==0)
	{
		str="无数据内容";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	if(IsValide(strErr,buffer,len,8))
	{
		strMsg = getEpCode(strMsg,buffer);
	}
	else
	{
		str.Format("(终端机器编码长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	int i=8;
	if(IsValide(strErr,buffer+i,len-i,1))
	{
		str.Format("%02x (充电接口标识)\r\n",buffer[8]);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(充电接口标识错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=9;
	if(IsValide(strErr,buffer+i,len-i,1))
	{
		str.Format("%02x (条数)\r\n",buffer[9]);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(条数错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	if(len != buffer[9]*17+10)
	{
		str.Format("灰锁记录数据错误，数据长度不对"); 
		strMsg = strMsg+str;
		/*m_Dlg->ProDecodeShow(strMsg);
		return;*/
	}
	i++;

	int j=0;
	for(j=0;j<buffer[9];j++)
	{
		i=j*17+10;
		if(IsValide(strErr,buffer+i,len-i,16))
		{
			for(int k=0;k<16;k++)
			{
				str.Format("%02x",buffer[i+k]); 
				strMsg = strMsg+str;
			}

			str.Format("(交易流水号)\r\n"); 
			strMsg = strMsg+str;
		}
		else
		{
			str.Format("(交易流水号长度错误)\r\n");
			strMsg = strMsg+strErr+str;
			m_Dlg->ProDecodeShow(strMsg);
			return;
		}
		i=i+16;
		if(IsValide(strErr,buffer+i,len-i,1))
		{
			str.Format("%02x(成功标识：",buffer[i]); 
			strMsg = strMsg+str;
			switch(buffer[i])
			{
			case 0:
				str.Format("成功)\r\n");
				strMsg = strMsg+str;
				break;
			case 1:
				str.Format("失败)\r\n");
				strMsg = strMsg+str;
				break;
			default:
				str.Format("未知)\r\n");
				strMsg = strMsg+str;
				break;
			}
		}
		else
		{
			str.Format("(成功标识错误)\r\n");
			strMsg = strMsg+strErr+str;
			m_Dlg->ProDecodeShow(strMsg);
			return;
		}
		i=i+1;

	}
	
	strMsg = cmdTimeMsg(strMsg,strErr,buffer+i,len-i);
	i=i+3;
    strMsg = crcMsg(strMsg,strErr,buffer+i,len-i);


	m_Dlg->ProDecodeShow(strMsg);
}
void	CProto_IEC104::Pro_Down_19(CString strMsg,unsigned char *buffer,int len)
{
	CString str,strErr;
	if(buffer==0 || len==0)
	{
		str="无数据内容";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	
	if(IsValide(strErr,buffer,len,8))
	{
		strMsg = getEpCode(strMsg,buffer);
	}
	else
	{
		str.Format("(终端机器编码长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	int i=8;
	if(m_Dlg->getProVersion()==13)
	{
		if(IsValide(strErr,buffer+8,len-8,1))
		{

			str.Format("%02x(枪口编号)\r\n",buffer[8]);
			strMsg = strMsg+str;
		}
		else
		{
			str.Format("(枪口编号错误)\r\n");
			strMsg = strMsg+strErr+str;
			m_Dlg->ProDecodeShow(strMsg);
			return;
		}
		i++;
	}

	if(IsValide(strErr,buffer+i,len-i,1))
	{
		str.Format("%02x(二维码文件找到标识：",buffer[i]);
		strMsg = strMsg+str;
		switch(buffer[i])
		{
		case 1:
			{
				str.Format("成功)\r\n");
				strMsg = strMsg+str;
			}
			break;
		case 0:
			{
				str.Format("失败)\r\n");
				strMsg = strMsg+str;
			}
			break;
		default:
			{
				str.Format("未知)\r\n");
				strMsg = strMsg+str;
			}
			break;
		}
	}
	else
	{
		str.Format("(二维码文件找到标识错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i++;
	unsigned int v =0;
	if(IsValide(strErr,buffer+i,len-i,4))
	{
		v=getInt4Small(buffer+i);
		str.Format("%02x %02x %02x %02x (字节数：%d\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],v);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(字节数长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+4;
	if(IsValide(strErr,buffer+i,len-i,2))
	{
		v =getInt2(buffer+i);
		str.Format("%02x %02x(分段数：%d)\r\n",buffer[i],buffer[i+1],v);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(分段数长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	CString s,s1;
	i=i+2;
	if(IsValide(strErr,buffer+i,len-i,32))
	{
		for(;i<47;i++)
		{
			str.Format("%02x ",buffer[i]);
			strMsg = strMsg+str;

			s1.Format("%c",buffer[i]);
			s = s+s1;
		}

		str.Format("(文件信息MD5 验证码：%c)\r\n",s);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(文件信息MD5 验证码长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}

	i=i+32;
	strMsg = cmdTimeMsg(strMsg,strErr,buffer+i,len-i);
	i=i+3;
    strMsg = crcMsg(strMsg,strErr,buffer+i,len-i);
	m_Dlg->ProDecodeShow(strMsg);
}
void	CProto_IEC104::Pro_Down_20(CString strMsg,unsigned char *buffer,int len)
{
	CString str,strErr;
	if(buffer==0 || len==0)
	{
		str="无数据内容";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}

	if(IsValide(strErr,buffer,len,8))
	{
		strMsg = getEpCode(strMsg,buffer);
	}
	else
	{
		str.Format("(终端机器编码长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	int i=8;
	unsigned int  v=0;
	if(IsValide(strErr,buffer+i,len-i,2))
	{

		v =getInt2(buffer+i);
		str.Format("%02x %02x(段索引：%d)\r\n",buffer[8],buffer[9],v);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(段索引长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=10;
	if(IsValide(strErr,buffer+i,len-i,1))
	{
		str.Format("%02x(成功标识：",buffer[10]);
		strMsg = strMsg+str;

		switch(buffer[10])
		{
		case 1:
			{
				str.Format("请求段索引不在文件段范围内)\r\n");
				strMsg = strMsg+str;
			}
			break;
		case 0:
			{
				str.Format("成功)\r\n");
				strMsg = strMsg+str;
			}
			break;
		default:
			{
				str.Format("未知)\r\n");
				strMsg = strMsg+str;
			}
			break;
		}
	}
	else
	{
		str.Format("(成功标识错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=11;
	if(IsValide(strErr,buffer+i,len-i,2))
	{
		v =getInt2(buffer+i);
		str.Format("%02x %02x(段大小：%d\r\n",buffer[i],buffer[i+1],v);
		strMsg = strMsg+str;

	}
	else
	{
		str.Format("(段大小长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	for(i=13;i<v+13;i++)
	{
		str.Format("%02x ",buffer[i]);
	}
	strMsg = strMsg+str;
	str.Format("(段数据)\r\n");
	strMsg = strMsg+str;

	if(m_Dlg->getProVersion()==13)
	{
		if(IsValide(strErr,buffer+v+13,len-v-13,1))
		{

			str.Format("%02x(枪口编号)\r\n",buffer[v+13]);
			strMsg = strMsg+str;
		}
		else
		{
			str.Format("(枪口编号错误)\r\n");
			strMsg = strMsg+strErr+str;
			m_Dlg->ProDecodeShow(strMsg);
			return;
		}
		i++;

	}
    strMsg = cmdTimeMsg(strMsg,strErr,buffer+i,len-i);
	i=i+3;
    strMsg = crcMsg(strMsg,strErr,buffer+i,len-i);
	m_Dlg->ProDecodeShow(strMsg);
}
void	CProto_IEC104::Pro_Down_21(CString strMsg,unsigned char *buffer,int len)
{
	CString str,strErr;
	if(buffer==0 || len==0)
	{
		str="无数据内容";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	if(IsValide(strErr,buffer,len,8))
	{
		strMsg = getEpCode(strMsg,buffer);
	}
	else
	{
		str.Format("(终端机器编码长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	int i=8;
	if(IsValide(strErr,buffer+i,len-i,1))
	{
		str.Format("%02x(充电桩程序文件是否存在：",buffer[8]);
		strMsg = strMsg+str;
		switch(buffer[8])
		{
		case 1:
			{
				str.Format("成功)\r\n");
				strMsg = strMsg+str;
			}
			break;
		case 0:
			{
				str.Format("失败)\r\n");
				strMsg = strMsg+str;
			}
			break;
		default:
			{
				str.Format("未知)\r\n");
				strMsg = strMsg+str;
			}
			break;
		}
	}
	else
	{
		str.Format("(充电桩程序文件是否存在错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=9;
	unsigned int v =0;
	if(IsValide(strErr,buffer+i,len-i,4))
	{
		v=getInt4Small(buffer+i);
		str.Format("%02x %02x %02x %02x(字节数：%d\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],v);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(字节数错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=13;
	if(IsValide(strErr,buffer+i,len-i,2))
	{
		v =getInt2(buffer+i);
		str.Format("%02x %02x(分段数：%d\r\n",buffer[i],buffer[i+1],v);
		strMsg = strMsg+str;
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(分段数错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=15;
	if(IsValide(strErr,buffer+i,len-i,32))
	{
		CString s,s1;
		for(;i<47;i++)
		{
			str.Format("%02x ",buffer[i]);
			strMsg = strMsg+str;
			s1.Format("%0c",buffer[i]);
			s = s+s1;
		}
		str.Format("(文件信息MD5 验证码：%s)\r\n",s);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(文件信息MD5 验证码长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=47;
	if(IsValide(strErr,buffer+i,len-i,2))
	{
		v =getInt2(buffer+i);
		str.Format("%02x %02x(版本编号：%d\r\n",buffer[i],buffer[i+1],v);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(版本编号长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}

	i=i+2;
	strMsg = cmdTimeMsg(strMsg,strErr,buffer+i,len-i);
	i=i+3;
    strMsg = crcMsg(strMsg,strErr,buffer+i,len-i);

	m_Dlg->ProDecodeShow(strMsg);
}
void	CProto_IEC104::Pro_Down_22(CString strMsg,unsigned char *buffer,int len)
{
	CString str,strErr;
	if(buffer==0 || len==0)
	{
		str="无数据内容";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	
	if(IsValide(strErr,buffer,len,8))
	{
		strMsg = getEpCode(strMsg,buffer);
	}
	else
	{
		str.Format("(终端机器编码长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	int i=8;
	unsigned int  v=0;
	if(IsValide(strErr,buffer+i,len-i,2))
	{
		v=(buffer[9]*0x100+buffer[8]);
		str.Format("%02x %02x(段索引：%d)\r\n",buffer[8],buffer[9],v);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(段索引长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=10;
	if(IsValide(strErr,buffer+i,len-i,1))
	{
		str.Format("%02x(成功标识：",buffer[10]);
		strMsg = strMsg+str;
		switch(buffer[10])
		{
			case 1:
				{
					str.Format("请求段索引不在文件段范围内)\r\n");
					strMsg = strMsg+str;
				}
				break;
			case 0:
				{
					str.Format("成功)\r\n");
					strMsg = strMsg+str;
				}
				break;
			default:
				{
					str.Format("未知)\r\n");
					strMsg = strMsg+str;
				}
				break;
		}
	}
	else
	{
			str.Format("(成功标识错误)\r\n");
			strMsg = strMsg+strErr+str;
			m_Dlg->ProDecodeShow(strMsg);
			return;
	}
	i=11;
	if(IsValide(strErr,buffer+i,len-i,32))
	{
			for(i=11;i<11+32;i++)
		    {
			   str.Format("%02x ",buffer[i]);
			   strMsg = strMsg+str;
		    }
			str.Format("(文件信息MD5验证码)\r\n");
			strMsg = strMsg+strErr+str;
	}
	else
	{
			str.Format("(文件信息MD5验证码错误)\r\n");
			strMsg = strMsg+strErr+str;
			m_Dlg->ProDecodeShow(strMsg);
			return;
	}
		
	if(IsValide(strErr,buffer+i,len-i,2))
	{
			v =(buffer[i+1]*0x100+buffer[i]);
			str.Format("%02x %02x(段大小：%d\r\n",buffer[i],buffer[i+1],v);
			strMsg = strMsg+str;
	}
	else
	{
			str.Format("(段大小错误)\r\n");
			strMsg = strMsg+strErr+str;
			m_Dlg->ProDecodeShow(strMsg);
			return;
	}
	i=i+2;

	for(int j=0;j<v;j++)
	{
			str.Format("%02x ",buffer[i]);
			strMsg = strMsg+str;
			i++;
	}	
	str.Format("(段数据)\r\n");
	strMsg = strMsg+str;

		
	strMsg = cmdTimeMsg(strMsg,strErr,buffer+i,len-i);
	i=i+3;
    strMsg = crcMsg(strMsg,strErr,buffer+i,len-i);

	m_Dlg->ProDecodeShow(strMsg);

}
void	CProto_IEC104::Pro_Down_23(CString strMsg,unsigned char *buffer,int len)
{
	CString str,strErr;
	if(buffer==0 || len==0)
	{
		str="无数据内容";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	if(IsValide(strErr,buffer,len,8))
	{
		strMsg = getEpCode(strMsg,buffer);
	}
	else
	{
		str.Format("(终端机器编码长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	int i=8;

	if(IsValide(strErr,buffer+i,len-i,1))
	{
		str.Format("%02x(枪口编号）\r\n",buffer[8]); 
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(枪口编号错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}

	i=i+1;
	strMsg = cmdTimeMsg(strMsg,strErr,buffer+i,len-i);
	i=i+3;
    strMsg = crcMsg(strMsg,strErr,buffer+i,len-i);
	m_Dlg->ProDecodeShow(strMsg);
}
void	CProto_IEC104::Pro_Down_44_v2(CString strMsg,unsigned char *buffer,int len)
{
	CString str,strErr;
	if(buffer==0 || len==0)
	{
		str="无数据内容";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	if(IsValide(strErr,buffer,len,8))
	{
		strMsg = getEpCode(strMsg,buffer);
	}
	else
	{
		str.Format("(终端机器编码长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	int i=8;

	if(IsValide(strErr,buffer+i,len-i,1))
	{
		str.Format("%02x(枪口编号）\r\n",buffer[i]); 
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(枪口编号错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	
	if(IsValide(strErr,buffer+i,len-i,32))
	{
		CString s,s1;
		for(int j=i;j<i+32;j++)
		{
			str.Format("%02x ",buffer[j]);
			strMsg = strMsg+str;

			s1.Format("%c",buffer[j]);
			s = s+s1;
		}
		str.Format("(内卡号：%s)",s);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(内卡号长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+32;
	if(IsValide(strErr,buffer+i,len-i,1))
	{
		str.Format("%02x(成功标识)\r\n",buffer[i]);
		strMsg = strMsg+str;

		switch(buffer[i])
		{
		case 1:
		 {
			 str.Format("成功)\r\n");
			 strMsg = strMsg+str;
		 }
		 break;
		case 0:
		 {
			 str.Format("失败)\r\n");
			 strMsg = strMsg+str;
		 }
		 break;
		default:
		 {
			 str.Format("未知)\r\n");
			 strMsg = strMsg+str;
		 }
		 break;
		}

	}
	else
	{
		str.Format("(成功标识错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i++;
    if(IsValide(strErr,buffer+i,len-i,2))
	{
		str.Format("%02x(错误编码:%d)\r\n",buffer[i],buffer[i+1],getInt2(buffer+i));
		strMsg = strMsg+str;

		switch(buffer[i])
		{
		case 1:
		 {
			 str.Format("金额不足)\r\n");
			 strMsg = strMsg+str;
		 }
		 break;
		case 2:
		 {
			 str.Format("没找到桩)\r\n");
			 strMsg = strMsg+str;
		 }
		 break;
	 case 3:
		 {
			 str.Format("无效卡)\r\n");
			 strMsg = strMsg+str;
		 }
		 break;
	 case 4:
		 {
			 str.Format("在其他桩上使用，不能充电)\r\n");
			 strMsg = strMsg+str;
		 }
		 break;
		default:
		 {
			 str.Format("未知)\r\n");
		 }
		 break;
	  }
	}
	i=i+2;
	strMsg = cmdTimeMsg(strMsg,strErr,buffer+i,len-i);
	i=i+3;
    strMsg = crcMsg(strMsg,strErr,buffer+i,len-i);

	m_Dlg->ProDecodeShow(strMsg);
}

void	CProto_IEC104::Pro_Down_46_v2(CString strMsg,unsigned char *buffer,int len)
{
	CString str,strErr;
	if(buffer==0 || len==0)
	{
		str="无数据内容";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	
	if(IsValide(strErr,buffer,len,8))
	{
		strMsg = getEpCode(strMsg,buffer);
	}
	else
	{
		str.Format("(终端机器编码长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	int i=8;

	if(IsValide(strErr,buffer+i,len-i,1))
	{
		str.Format("%02x(枪口编号）\r\n",buffer[i]); 
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(枪口编号错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i++;
	if(IsValide(strErr,buffer+i,len-i,16))
	{
		for(int k=0;k<16;k++)
		{
			str.Format("%02x",buffer[i+k]); 
				strMsg = strMsg+str;
		}

			str.Format("(流水号)\r\n"); 
			strMsg = strMsg+str;
	}
	i=i+16;
	if(IsValide(strErr,buffer+i,len-i,1))
	{
		str.Format("%02x(处理结果：",buffer[i]);
		strMsg = strMsg+str;
		switch(buffer[i])
		{
		case 1:
			{
				str.Format("处理成功)\r\n");
				strMsg = strMsg+str;
			}
			break;
		case 2:
			{
				str.Format("数据不存在)\r\n");
				strMsg = strMsg+str;
			}
			break;
		case 3:
			{
				str.Format("已经处理)\r\n");
				strMsg = strMsg+str;
			}
			break;
		default:
			{
				str.Format("未知)\r\n");
				strMsg = strMsg+str;
			}
			break;
		}
	}
	else
	{
		str.Format("(处理结果错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
    i=i+1;
	strMsg = cmdTimeMsg(strMsg,strErr,buffer+i,len-i);
	i=i+3;
    strMsg = crcMsg(strMsg,strErr,buffer+i,len-i);

	m_Dlg->ProDecodeShow(strMsg);
}

void CProto_IEC104::Pro_Down_49_v4(CString strMsg,unsigned char *buffer,int len)
{
	CString str,strErr;
	if(buffer==0 || len==0)
	{
		str="无数据内容";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	if(IsValide(strErr,buffer,len,8))
	{
		strMsg = getEpCode(strMsg,buffer);
	}
	else
	{
		str.Format("(终端机器编码长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	int i=8;
	if(IsValide(strErr,buffer+i,len-i,1))
	{
		str.Format("%02x(充电接口标识)\r\n",buffer[8]);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("充电接口标识错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+1;
	if(IsValide(strErr,buffer+i,len-i,10))
	{
		for(int z=0;z<10;z++)
		{
		   str.Format("%02x ",buffer[i+z]);
		   strMsg = strMsg+str;
		}
		
	}
	else
	{
		str.Format("识别码标识错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	strMsg = strMsg+"(识别码)\r\n";
	i=i+10;
	strMsg=timeMsg(strMsg,buffer+i,"识别码生成时间");
	i=i+7;
	strMsg = cmdTimeMsg(strMsg,strErr,buffer+i,len-i);
	i=i+3;
    strMsg = crcMsg(strMsg,strErr,buffer+i,len-i);
    m_Dlg->ProDecodeShow(strMsg);

}

void CProto_IEC104::Pro_Down_49_v2(CString strMsg,unsigned char *buffer,int len)
{
	CString str,strErr;
	if(buffer==0 || len==0)
	{
		str="无数据内容";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	if(IsValide(strErr,buffer,len,8))
	{
		strMsg = getEpCode(strMsg,buffer);
	}
	else
	{
		str.Format("(终端机器编码长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	int i=8;
	if(IsValide(strErr,buffer+i,len-i,1))
	{
		str.Format("%02x(充电接口标识)\r\n",buffer[8]);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("充电接口标识错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+1;
	if(IsValide(strErr,buffer+i,len-i,1))
	{
		str.Format("%d(时)\r\n",buffer[i]);
		strMsg = strMsg+str;
		
	}
	else
	{
		str.Format("时错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+1;
	if(IsValide(strErr,buffer+i,len-i,1))
	{
		str.Format("%d(分)\r\n",buffer[i]);
		strMsg = strMsg+str;
		
	}
	else
	{
		str.Format("分错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+1;
	if(IsValide(strErr,buffer+i,len-i,1))
	{
		str.Format("%d(秒)\r\n",buffer[i]);
		strMsg = strMsg+str;
		
	}
	else
	{
		str.Format("秒错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
    i=i+1;
	if(IsValide(strErr,buffer+i,len-i,10))
	{
		for(int z=0;z<10;z++)
		{
		   str.Format("%02x ",buffer[i+z]);
		   strMsg = strMsg+str;
		}
		
	}
	else
	{
		str.Format("识别码标识错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	strMsg = strMsg+"(识别码)\r\n";
	i=i+10;
	strMsg=timeMsg(strMsg,buffer+i,"识别码生成时间");
	
	
    m_Dlg->ProDecodeShow(strMsg);

}

void	CProto_IEC104::Pro_Down_24(CString strMsg,unsigned char *buffer,int len)
{
	CString str,strErr;
	if(m_Dlg->getProVersion() <12)
	{
		str="请选择V1.2以上版本";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	if(buffer==0 || len==0)
	{
		str="无数据内容";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	if(IsValide(strErr,buffer,len,8))
	{
		strMsg = getEpCode(strMsg,buffer);
	}
	else
	{
		str.Format("(终端机器编码长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	int i=8;

	if(IsValide(strErr,buffer+i,len-i,1))
	{
	str.Format("%02x(枪口编号）\r\n",buffer[8]); 
	strMsg = strMsg+str;
	}
	else
	{
		str.Format("(枪口编号错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i++;
   if(IsValide(strErr,buffer+i,len-i,1))
	{
	 str.Format("%02x(业务指令：",buffer[9]);
		 strMsg = strMsg+str;
		switch(buffer[9])
		{
		case 7:
		 {
			 str.Format("预约应答)\r\n");
			 strMsg = strMsg+str;
		 }
		 break;
		case 8:
		 {
			 str.Format("取消预约)\r\n");
			 strMsg = strMsg+str;
		 }
		break;
		case 12:
		 {
			 str.Format("开始充电)\r\n");
			 strMsg = strMsg+str;
		 }
		break;
		case 27:
		 {
			 str.Format("停止充电)\r\n");
			 strMsg = strMsg+str;
		 }
		break;
		case 9:
		 {
			 str.Format("开始充电事件)\r\n");
			 strMsg = strMsg+str;
		 }
		break;
		case 10:
		 {
			 str.Format("停止充电时间)\r\n");
			 strMsg = strMsg+str;
		 }
		break;
		case 11:
		 {
			 str.Format("消费记录上报)\r\n");
			 strMsg = strMsg+str;
		 }
		break;
	    case 3:
		 {
			 str.Format("私有充电桩下发充电桩运营时间上行结果数据)\r\n");
			 strMsg = strMsg+str;
		 }
		break;
		default:
		 {
			 str.Format("未知)\r\n");
			 strMsg = strMsg+str;
		 }
		 break;
		}
}
	else
	{
		str.Format("(业务指令错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
   i=i+1;
    if(IsValide(strErr,buffer+i,len-i,2))
	{
		unsigned int v =(buffer[11]*0x100+buffer[10]);
		 str.Format("%02x %02x(业务数据帧编号：%d\r\n",buffer[10],buffer[11],v);
		 strMsg = strMsg+str;
    }
	else
	{
		str.Format("(业务数据帧编号错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	 i=i+2;
    if(IsValide(strErr,buffer+i,len-i,1))
	{
		 str.Format("%02x(离线标识：%d)\r\n",buffer[i],buffer[i]);
		 strMsg = strMsg+str;
    }
	else
	{
		str.Format("(离线标识错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	 i=i+1;
    if(IsValide(strErr,buffer+i,len-i,1))
	{
		 str.Format("%02x(业务超时：%d)\r\n",buffer[i],buffer[i]);
		 strMsg = strMsg+str;
    }
	else
	{
		str.Format("(业务超时错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	 i=i+1;
    if(IsValide(strErr,buffer+i,len-i,7))
	{

		strMsg = timeMsg(strMsg,buffer+i,"充电开始时间");
    }
	else
	{
		str.Format("(充电开始时间年长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}

	i=i+7;
	strMsg = cmdTimeMsg(strMsg,strErr,buffer+i,len-i);
	i=i+3;
    strMsg = crcMsg(strMsg,strErr,buffer+i,len-i);

	m_Dlg->ProDecodeShow(strMsg);


}
void	CProto_IEC104::Pro_Down_25(CString strMsg,unsigned char *buffer,int len)
{
	CString str,strErr;
	if(m_Dlg->getProVersion() <12)
	{
		str="请选择V1.2以上版本";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	if(buffer==0 || len==0)
	{
		str="无数据内容";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	if(IsValide(strErr,buffer,len,8))
	{
		strMsg = getEpCode(strMsg,buffer);
	}
	else
	{
		str.Format("(终端机器编码长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	int i=8;

	if(IsValide(strErr,buffer+i,len-i,1))
	{
	    str.Format("%02x(枪口编号）\r\n",buffer[8]); 
	    strMsg = strMsg+str;
	}
	else
	{
		str.Format("(枪口编号错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+1;
	strMsg = cmdTimeMsg(strMsg,strErr,buffer+i,len-i);
	i=i+3;
    strMsg = crcMsg(strMsg,strErr,buffer+i,len-i);
	m_Dlg->ProDecodeShow(strMsg);
}
void	CProto_IEC104::Pro_Down_26(CString strMsg,unsigned char *buffer,int len)
{
	CString str,strErr;
	if(m_Dlg->getProVersion() <12)
	{
		str="请选择V1.2以上版本";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	if(buffer==0 || len==0)
	{
		str="无数据内容";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	if(IsValide(strErr,buffer,len,8))
	{
		strMsg = getEpCode(strMsg,buffer);
	}
	else
	{
		str.Format("(终端机器编码长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	int i=8;
	if(IsValide(strErr,buffer+i,len-i,1))
	{
		str.Format("%02x(服务器IP 第一段%d）\r\n",buffer[i],buffer[i]); 
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(服务器IP 第一段错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+1;
	if(IsValide(strErr,buffer+i,len-i,1))
	{
		str.Format("%02x(服务器IP 第二段%d）\r\n",buffer[i],buffer[i]); 
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(服务器IP 第二段错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+1;
	if(IsValide(strErr,buffer+i,len-i,1))
	{
		str.Format("%02x(服务器IP 第三段%d）\r\n",buffer[i],buffer[i]); 
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(服务器IP 第三段错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+1;
	if(IsValide(strErr,buffer+i,len-i,1))
	{
		str.Format("%02x(服务器IP 第四段%d）\r\n",buffer[i],buffer[i]); 
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(服务器IP 第四段错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+1;
	if(IsValide(strErr,buffer+i,len-i,1))
	{
		str.Format("%02x(服务器IP 第五段%d）\r\n",buffer[i],buffer[i]); 
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(服务器IP 第五段错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+1;
	if(IsValide(strErr,buffer+i,len-i,1))
	{
		str.Format("%02x(服务器IP 第六段%d）\r\n",buffer[i],buffer[i]); 
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(服务器IP 第六段错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}

	i=i+1;
	if(IsValide(strErr,buffer+i,len-i,2))
	{
		str.Format("%02x %02x(端口号：%d）\r\n",buffer[i],buffer[i+1],getInt2(buffer+i)); 
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(端口号错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}

	i=i+2;
	strMsg = cmdTimeMsg(strMsg,strErr,buffer+i,len-i);
	i=i+3;
    strMsg = crcMsg(strMsg,strErr,buffer+i,len-i);

	m_Dlg->ProDecodeShow(strMsg);
}

void	CProto_IEC104::Pro_Down_102(CString strMsg,unsigned char *buffer,int len)
{
	CString str,strErr;
	if(m_Dlg->getProVersion() <20)
	{
		str="请选择V2.0以上版本";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	if(buffer==0 || len==0)
	{
		str="无数据内容";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	if(IsValide(strErr,buffer,len,8))
	{
		strMsg = getEpCode(strMsg,buffer);
	}
	else
	{
		str.Format("(终端机器编码长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	int i=8;
	if(IsValide(strErr,buffer+i,len-i,1))
	{
		str.Format("%02x(操作标识)\r\n",buffer[i]);
		strMsg = strMsg+str;

		switch(buffer[i])
		{
		case 1:
		 {
			 str.Format("启)\r\n");
			 strMsg = strMsg+str;
		 }
		 break;
		case 0:
		 {
			 str.Format("停)\r\n");
			 strMsg = strMsg+str;
		 }
		 break;
		default:
		 {
			 str.Format("未知)\r\n");
			 strMsg = strMsg+str;
		 }
		 break;
		}

	}
	else
	{
		str.Format("(操作标识错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i++;
	if(IsValide(strErr,buffer+i,len-i,1))
	{
		str.Format("%02x(呼叫类型)\r\n",buffer[i]);
		strMsg = strMsg+str;

		switch(buffer[i])
		{
		case 1:
		 {
			 str.Format("LED灯)\r\n");
			 strMsg = strMsg+str;
		 }
		 break;
		case 2:
		 {
			 str.Format("声音)\r\n");
			 strMsg = strMsg+str;
		 }
		 break;
	 case 3:
		 {
			 str.Format("LED和声音)\r\n");
			 strMsg = strMsg+str;
		 }
		 break;
		default:
		 {
			 str.Format("未知)\r\n");
			 strMsg = strMsg+str;
		 }
		 break;
		}

	}
	else
	{
		str.Format("(呼叫类型错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i++;
	if(IsValide(strErr,buffer+i,len-i,2))
	{
		str.Format("%02x %02x(时间：%d）\r\n",buffer[i],buffer[i+1],getInt2(buffer+i)); 
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(时间错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}

	i=i+2;
	strMsg = cmdTimeMsg(strMsg,strErr,buffer+i,len-i);
	i=i+3;
    strMsg = crcMsg(strMsg,strErr,buffer+i,len-i);
	m_Dlg->ProDecodeShow(strMsg);
}

void	CProto_IEC104::Pro_Down_103(CString strMsg,unsigned char *buffer,int len)
{
	CString str,strErr;
	if(m_Dlg->getProVersion() <13)
	{
		str="请选择V1.3以上版本";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	if(buffer==0 || len==0)
	{
		str="无数据内容";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	if(IsValide(strErr,buffer,len,8))
	{
		strMsg = getEpCode(strMsg,buffer);
	}
	else
	{
		str.Format("(终端机器编码长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	int i=8;
	
	if(IsValide(strErr,buffer+i,len-i,1))
	{
	str.Format("%02x(枪口编号）\r\n",buffer[i]); 
	strMsg = strMsg+str;
	}
	else
	{
		str.Format("(枪口编号错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}

	i=i+1;
	strMsg = cmdTimeMsg(strMsg,strErr,buffer+i,len-i);
	i=i+3;
    strMsg = crcMsg(strMsg,strErr,buffer+i,len-i);
	m_Dlg->ProDecodeShow(strMsg);
}

void	CProto_IEC104::Pro_Down_27(CString strMsg,unsigned char *buffer,int len)
{
	CString str,strErr;
	if(m_Dlg->getProVersion() <13)
	{
		str="请选择V1.3以上版本";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	if(buffer==0 || len==0)
	{
		str="无数据内容";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	if(IsValide(strErr,buffer,len,8))
	{
		strMsg = getEpCode(strMsg,buffer);
	}
	else
	{
		str.Format("(终端机器编码长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}

	int i=8;
	strMsg = cmdTimeMsg(strMsg,strErr,buffer+i,len-i);
	i=i+3;
    strMsg = crcMsg(strMsg,strErr,buffer+i,len-i);
	m_Dlg->ProDecodeShow(strMsg);
}
void	CProto_IEC104::Pro_Down_28(CString strMsg,unsigned char *buffer,int len)
{
	CString str,strErr;
	if(m_Dlg->getProVersion() <13)
	{
		str="请选择V1.3以上版本";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	if(buffer==0 || len==0)
	{
		str="无数据内容";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	if(IsValide(strErr,buffer,len,8))
	{
		strMsg = getEpCode(strMsg,buffer);
	}
	else
	{
		str.Format("(终端机器编码长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	int i=8;
	if(IsValide(strErr,buffer+i,len-i,1))
	{
		str.Format("%02x(枪口编号)\r\n",buffer[i]);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(枪口编号长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i++;
	if(IsValide(strErr,buffer+i,len-i,4))
	{
			int v=getInt4Small(buffer+i);
			str.Format("%02x %02x %02x %02x (预充金额：%f)\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],v*0.01);
			strMsg = strMsg+str;
	}
	else
	{
			str.Format("(预充金额错误)\r\n");
			strMsg = strMsg+strErr+str;
			m_Dlg->ProDecodeShow(strMsg);
			return;
	}
	i=i+4;
	strMsg = cmdTimeMsg(strMsg,strErr,buffer+i,len-i);
	i=i+3;
    strMsg = crcMsg(strMsg,strErr,buffer+i,len-i);

	m_Dlg->ProDecodeShow(strMsg);
}
void	CProto_IEC104::Pro_Down_29(CString strMsg,unsigned char *buffer,int len)
{
	CString str,strErr;
	if(m_Dlg->getProVersion() <13)
	{
		str="请选择V1.3以上版本";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	if(buffer==0 || len==0)
	{
		str="无数据内容";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	if(IsValide(strErr,buffer,len,8))
	{
		strMsg = getEpCode(strMsg,buffer);
	}
	else
	{
		str.Format("(终端机器编码长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	int i=8;
	if(IsValide(strErr,buffer+i,len-i,1))
	{
		str.Format("%02x(枪口编号)\r\n",buffer[i]);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(枪口编号长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i++;
	if(IsValide(strErr,buffer+i,len-i,2))
	{
		str.Format("%02x %02x(版本编号%d)\r\n",buffer[i],buffer[i+1],getInt2(buffer+i));
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(版本编号长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+2;
	strMsg = cmdTimeMsg(strMsg,strErr,buffer+i,len-i);
	i=i+3;
    strMsg = crcMsg(strMsg,strErr,buffer+i,len-i);

	m_Dlg->ProDecodeShow(strMsg);
}

bool 	CProto_IEC104::IsValide(CString err,unsigned char * buffer,int slen,int dlen)
{
	if(slen>=dlen)
		return true;
	else
	{
		for(int i=0;i<slen;i++)
		{
		     err.Format("%02x",buffer[i]);
		}
		return false;
	}
}

void	CProto_IEC104::YC_RxProc(CString strMsg,unsigned char * buffer,int len)
{
}

void	CProto_IEC104::YX_RxProc(CString strMsg,unsigned char * buffer,int len)
{
	
}

void	CProto_IEC104::KWH_RxProc(CString strMsg,unsigned char * buffer,int len)
{
	
}



void	CProto_IEC104::Pro_UP_143(CString strMsg,unsigned char *buffer,int len)
{
	CString str,strErr;
	if(m_Dlg->getProVersion() <20)
	{
		str="请选择V2.0以上版本";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}

	if(buffer==0 || len==0)
	{
		str="无数据内容";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	if(IsValide(strErr,buffer,len,8))
	{
		strMsg = getEpCode(strMsg,buffer);
	}
	else
	{
		str.Format("(终端机器编码长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	int i=8;
	if(IsValide(strErr,buffer+i,len-i,2))
	{

		str.Format("%02x %02x ( 站地址:%d)\r\n",buffer[i],buffer[i+1],getInt2Small(buffer+i));
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(站地址错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+2;
	if(IsValide(strErr,buffer+i,len-i,10))
	{
		CString s,s1;
		for(int j=i;j<i+10;j++)
		{
			str.Format("%02x ",buffer[j]);
			strMsg = strMsg+str;

			s1.Format("%c",buffer[j]);
			s = s+s1;
		}
		str.Format("(硬件型号：%s)\r\n",s);
		strMsg = strMsg+str;

	}
	else
	{
		str.Format("(硬件型号长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+10;
	if(IsValide(strErr,buffer+i,len-i,1))
	{
			str.Format("%02x (硬件主版本号：%d)\r\n",buffer[i],buffer[i]);
			strMsg = strMsg+str;
			
	}
	else
	{
			str.Format("(硬件主版本号错误)\r\n");
			strMsg = strMsg+strErr+str;
			m_Dlg->ProDecodeShow(strMsg);
			return;
	}
	i++;
    if(IsValide(strErr,buffer+i,len-i,1))
	{
			str.Format("%02x (硬件子版本号：%d)\r\n",buffer[i],buffer[i]);
			strMsg = strMsg+str;
		
	}
	else
	{
			str.Format("(硬件子版本号错误)\r\n");
			strMsg = strMsg+strErr+str;
			m_Dlg->ProDecodeShow(strMsg);
			return;
	}
    i++;
    if(IsValide(strErr,buffer+i,len-i,2))
	{
			str.Format("%02x %02x (分段字节大小：%d)\r\n",buffer[i],buffer[i+1],getInt2(buffer+i));
			strMsg = strMsg+str;
			
	}
	else
	{
			str.Format("(分段字节大小错误)\r\n");
			strMsg = strMsg+strErr+str;
			m_Dlg->ProDecodeShow(strMsg);
			return;
	}
	i=i+2;
	strMsg = cmdTimeMsg(strMsg,strErr,buffer+i,len-i);
	i=i+3;
    strMsg = crcMsg(strMsg,strErr,buffer+i,len-i);

	m_Dlg->ProDecodeShow(strMsg);
}



void	CProto_IEC104::Pro_UP_144(CString strMsg,unsigned char *buffer,int len)
{
	CString str,strErr;
	if(m_Dlg->getProVersion() <20)
	{
		str="请选择V2.0以上版本";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}

	if(buffer==0 || len==0)
	{
		str="无数据内容";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	
	if(IsValide(strErr,buffer,len,8))
	{
		strMsg = getEpCode(strMsg,buffer);
	}
	else
	{
		str.Format("(终端机器编码长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	int i=8;
	if(IsValide(strErr,buffer+i,len-i,2))
	{

		str.Format("%02x %02x ( 站地址:%d)\r\n",buffer[i],buffer[i+1],getInt2(buffer+i));
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(站地址错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+2;
	if(IsValide(strErr,buffer+i,len-i,8))
	{
		CString s,s1;
		for(int j=i;j<i+8;j++)
		{
			str.Format("%02x ",buffer[j]);
			strMsg = strMsg+str;

			s1.Format("%c",buffer[j]);
			s = s+s1;
		}
		str.Format("(固件型号：%s)\r\n",s);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(固件型号长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+8;
	if(IsValide(strErr,buffer+i,len-i,1))
	{
			str.Format("%02x (固件主版本号：%d)\r\n",buffer[i],buffer[i]);
			strMsg = strMsg+str;
			
	}
	else
	{
			str.Format("(固件主版本号错误)\r\n");
			strMsg = strMsg+strErr+str;
			m_Dlg->ProDecodeShow(strMsg);
			return;
	}
	i++;
    if(IsValide(strErr,buffer+i,len-i,1))
	{
			str.Format("%02x (固件副版本号：%d)\r\n",buffer[i],buffer[i]);
			strMsg = strMsg+str;
		
	}
	else
	{
			str.Format("(固件副版本号错误)\r\n");
			strMsg = strMsg+strErr+str;
			m_Dlg->ProDecodeShow(strMsg);
			return;
	}
    i++;
    if(IsValide(strErr,buffer+i,len-i,2))
	{
			str.Format("%02x %02x(固件编译版本号：%d)\r\n",buffer[i],buffer[i+1],getInt2(buffer+i));
			strMsg = strMsg+str;
			
	}
	else
	{
			str.Format("(固件编译版本号错误)\r\n");
			strMsg = strMsg+strErr+str;
			m_Dlg->ProDecodeShow(strMsg);
			return;
	}
	i=i+2;;
    if(IsValide(strErr,buffer+i,len-i,2))
	{
			str.Format("%02x %02x (段索引：%d)\r\n",buffer[i],buffer[i+1],getInt2(buffer+i));
			strMsg = strMsg+str;
			
	}
	else
	{
			str.Format("(段索引错误)\r\n");
			strMsg = strMsg+strErr+str;
			m_Dlg->ProDecodeShow(strMsg);
			return;
	}
	i=i+2;;
    if(IsValide(strErr,buffer+i,len-i,2))
	{
			str.Format("%02x %02x(分段字节大小：%d)\r\n",buffer[i],buffer[i+1],getInt2(buffer+i));
			strMsg = strMsg+str;
			
	}
	else
	{
			str.Format("(分段字节大小错误)\r\n");
			strMsg = strMsg+strErr+str;
			m_Dlg->ProDecodeShow(strMsg);
			return;
	}
    i=i+2;
	strMsg = cmdTimeMsg(strMsg,strErr,buffer+i,len-i);
	i=i+3;
    strMsg = crcMsg(strMsg,strErr,buffer+i,len-i);
	m_Dlg->ProDecodeShow(strMsg);
}


void	CProto_IEC104::Pro_UP_145(CString strMsg,unsigned char *buffer,int len)
{
	CString str,strErr;
	if(m_Dlg->getProVersion() <20)
	{
		str="请选择V2.0以上版本";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}

	if(buffer==0 || len==0)
	{
		str="无数据内容";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	if(IsValide(strErr,buffer,len,8))
	{
		strMsg = getEpCode(strMsg,buffer);
	}
	else
	{
		str.Format("(终端机器编码长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	int i=8;
	if(IsValide(strErr,buffer+i,len-i,1))
	{

		str.Format("%02x%02x ( 站地址)\r\n",buffer[i],buffer[i+1]);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(站地址错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+2;
	if(IsValide(strErr,buffer+i,len-i,10))
	{
		CString s,s1;
		for(int j=i;j<i+10;j++)
		{
			str.Format("%02x ",buffer[j]);
			strMsg = strMsg+str;

			s1.Format("%c",buffer[j]);
			s = s+s1;
		}
		str.Format("(硬件1型号：%s)\r\n",s);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(硬件长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+10;
	if(IsValide(strErr,buffer+i,len-i,1))
	{
			str.Format("%02x (硬件主版本号：%d)\r\n",buffer[i],buffer[i]);
			strMsg = strMsg+str;
			
	}
	else
	{
			str.Format("(硬件主版本号错误)\r\n");
			strMsg = strMsg+strErr+str;
			m_Dlg->ProDecodeShow(strMsg);
			return;
	}
	i++;
    if(IsValide(strErr,buffer+i,len-i,1))
	{
			str.Format("%02x (硬件子版本号：%d)\r\n",buffer[i],buffer[i]);
			strMsg = strMsg+str;
			
	}
	else
	{
			str.Format("(硬件子版本号错误)\r\n");
			strMsg = strMsg+strErr+str;
			m_Dlg->ProDecodeShow(strMsg);
			return;
	}
    i++;

	if(IsValide(strErr,buffer+i,len-i,8))
	{
		CString s,s1;
		for(int j=i;j<i+8;j++)
		{
			str.Format("%02x ",buffer[j]);
			strMsg = strMsg+str;

			s1.Format("%c",buffer[j]);
			s = s+s1;
		}
		str.Format("(固件型号：%s)\r\n",s);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(固件长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+8;
	
	if(IsValide(strErr,buffer+i,len-i,1))
	{
			str.Format("%02x (固件主版本号：%d)\r\n",buffer[i],buffer[i]);
			strMsg = strMsg+str;
			
	}
	else
	{
			str.Format("(固件主版本号错误)\r\n");
			strMsg = strMsg+strErr+str;
			m_Dlg->ProDecodeShow(strMsg);
			return;
	}
	i++;
    if(IsValide(strErr,buffer+i,len-i,1))
	{
			str.Format("%02x (固件副版本号：%d)\r\n",buffer[i],buffer[i]);
			strMsg = strMsg+str;
			
	}
	else
	{
			str.Format("(固件副版本号错误)\r\n");
			strMsg = strMsg+strErr+str;
			m_Dlg->ProDecodeShow(strMsg);
			return;
	}
    i++;
    if(IsValide(strErr,buffer+i,len-i,2))
	{
			str.Format("%02x %02x (固件编译版本号：%d)\r\n",buffer[i],buffer[i+1],getInt2(buffer+i));
			strMsg = strMsg+str;
			
	}
	else
	{
			str.Format("(固件编译版本号错误)\r\n");
			strMsg = strMsg+strErr+str;
			m_Dlg->ProDecodeShow(strMsg);
			return;
	}
	i=i+2;
	if(IsValide(strErr,buffer+i,len-i,1))
	{
		str.Format("%02x(更新成功标识：",buffer[i]);
		strMsg = strMsg+str;

		if(buffer[i]==0)
		{
			str.Format("失败)\r\n");
			strMsg = strMsg+str;
		}
		else
		{
			str.Format("成功)\r\n");
			strMsg = strMsg+str;
		}
	}
	else
	{
		str.Format("更新成功标识错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}

	i=i+1;
	strMsg = cmdTimeMsg(strMsg,strErr,buffer+i,len-i);
	i=i+3;
    strMsg = crcMsg(strMsg,strErr,buffer+i,len-i);
     

	m_Dlg->ProDecodeShow(strMsg);
}




void	CProto_IEC104::Pro_UP_141(CString strMsg,unsigned char *buffer,int len)
{
	CString str,strErr;
	if(m_Dlg->getProVersion() <20)
	{
		str="请选择V2.0以上版本";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}

	if(buffer==0 || len==0)
	{
		str="无数据内容";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	
	if(IsValide(strErr,buffer,len,8))
	{
		strMsg = getEpCode(strMsg,buffer);
	}
	else
	{
		str.Format("(终端机器编码长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	int i=8;
	if(IsValide(strErr,buffer+i,len-i,2))
	{

		str.Format("%02x %02x ( 站地址)\r\n",buffer[i],buffer[i+1]);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(站地址错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+2;
	if(IsValide(strErr,buffer+i,len-i,20))
	{
		CString s,s1;
		for(int j=i;j<i+20;j++)
		{
			str.Format("%02x ",buffer[j]);
			strMsg = strMsg+str;

			s1.Format("%c",buffer[j]);
			s = s+s1;
		}
		str.Format("(产品型号：%s)\r\n",s);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(产品型号长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+20;
	if(IsValide(strErr,buffer+i,len-i,1))
	{
			str.Format("%02x (硬件/固件数量：%d)\r\n",buffer[i],buffer[i]);
			strMsg = strMsg+str;
			
	}
	else
	{
			str.Format("(硬件/固件数量错误)\r\n");
			strMsg = strMsg+strErr+str;
			m_Dlg->ProDecodeShow(strMsg);
			return;
	}
	int num = buffer[i];
    i++;
	for(int k=0;k<num;k++)
	{
	   if(IsValide(strErr,buffer+i,len-i,10))
	   {
		   CString s,s1;
	 	   for(int j=i;j<i+10;j++)
		   {
			  str.Format("%02x ",buffer[j]);
			  strMsg = strMsg+str;

			  s1.Format("%c",buffer[j]);
			   s = s+s1;
		    }
		    str.Format("(硬件型号：%s)\r\n",s);
		    strMsg = strMsg+str;
	    }
	   else
	   {
		    str.Format("(硬件型号长度错误)\r\n");
		    strMsg = strMsg+strErr+str;
		    m_Dlg->ProDecodeShow(strMsg);
		    return;
	    }
	    i=i+10;
	    if(IsValide(strErr,buffer+i,len-i,1))
		{
			str.Format("%02x (硬件主版本号：%d)\r\n",buffer[i],buffer[i]);
			strMsg = strMsg+str;
			
		}
		else
		{
			str.Format("(硬件主版本号错误)\r\n");
			strMsg = strMsg+strErr+str;
			m_Dlg->ProDecodeShow(strMsg);
			return;
		}
	    i++;
        if(IsValide(strErr,buffer+i,len-i,1))
		{
			str.Format("%02x (硬件子版本号：%d)\r\n",buffer[i],buffer[i]);
			strMsg = strMsg+str;
			
		}
		else
		{
			str.Format("(硬件子版本号错误)\r\n");
			strMsg = strMsg+strErr+str;
			m_Dlg->ProDecodeShow(strMsg);
			return;
		}
        i++;
	   if(IsValide(strErr,buffer+i,len-i,8))
	   {
		   CString s,s1;
		   for(int j=i;j<i+8;j++)
		   {
			  str.Format("%02x ",buffer[j]);
			  strMsg = strMsg+str;

			   s1.Format("%c",buffer[j]);
			   s = s+s1;
		     }
		      str.Format("(固件型号：%s)\r\n",s);
	 	      strMsg = strMsg+str;
	     }
	     else
	     {
		    str.Format("(固件型号长度错误)\r\n");
		    strMsg = strMsg+strErr+str;
		    m_Dlg->ProDecodeShow(strMsg);
		    return;
	     }
	     i=i+8;
	    if(IsValide(strErr,buffer+i,len-i,1))
	    {
			str.Format("%02x (固件主版本号：%d)\r\n",buffer[i],buffer[i]);
			strMsg = strMsg+str;
			
	    }
	    else
	    {
			str.Format("(固件主版本号错误)\r\n");
			strMsg = strMsg+strErr+str;
			m_Dlg->ProDecodeShow(strMsg);
			return;
	    }
	    i++;
        if(IsValide(strErr,buffer+i,len-i,1))
		{
			str.Format("%02x (固件副版本号：%d)\r\n",buffer[i],buffer[i]);
			strMsg = strMsg+str;
			
		}
		else
		{
			str.Format("(固件副版本号错误)\r\n");
			strMsg = strMsg+strErr+str;
			m_Dlg->ProDecodeShow(strMsg);
			return;
		}
        i++;
       if(IsValide(strErr,buffer+i,len-i,2))
	   {
			str.Format("%02x %02x(固件编译版本号：%d)\r\n",buffer[i],buffer[i+1],getInt2(buffer+i));
			strMsg = strMsg+str;
			
	   }
	   else
	   {
			str.Format("(固件编译版本号错误)\r\n");
			strMsg = strMsg+strErr+str;
			m_Dlg->ProDecodeShow(strMsg);
			return;
	    }
	    i=i+2;
      
	}
	strMsg = cmdTimeMsg(strMsg,strErr,buffer+i,len-i);
	i=i+3;
    strMsg = crcMsg(strMsg,strErr,buffer+i,len-i);
	m_Dlg->ProDecodeShow(strMsg);
}

void	CProto_IEC104::Pro_UP_161(CString strMsg,unsigned char *buffer,int len)
{
	CString str,strErr;
	if(m_Dlg->getProVersion() <20)
	{
		str="请选择V2.0以上版本";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}

	if(buffer==0 || len==0)
	{
		str="无数据内容";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	if(IsValide(strErr,buffer,len,8))
	{
		strMsg = getEpCode(strMsg,buffer);
	}
	else
	{
		str.Format("(终端机器编码长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	int i=8;
	if(IsValide(strErr,buffer+i,len-i,1))
	{

		str.Format("%02x ( 充电接口数量)\r\n",buffer[i]);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(充电接口数量错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i++;
	int num = buffer[i];
	for(int i=0;i<num;i++)
	{
		if(IsValide(strErr,buffer+i,len-i,4))
		{
			int v=getInt4Small(buffer+i);
			str.Format("%02x %02x %02x %02x (充电总时间：%d)\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],v);
			strMsg = strMsg+str;
		}
		else
		{
			str.Format("(充电总时间错误)\r\n");
			strMsg = strMsg+strErr+str;
			m_Dlg->ProDecodeShow(strMsg);
			return;
		}
		i=i+4;

		if(IsValide(strErr,buffer+i,len-i,4))
		{
			int	v=getInt4Small(buffer+i);
			str.Format("%02x %02x %02x %02x (总充电次数：%d)\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],v);
			strMsg = strMsg+str;
		}
		else
		{
			str.Format("(总充电次数错误)\r\n");
			strMsg = strMsg+strErr+str;
			m_Dlg->ProDecodeShow(strMsg);
			return;
		}
		i=i+4;

		if(IsValide(strErr,buffer+i,len-i,4))
		{
			int	v=getInt4Small(buffer+i);
			str.Format("%02x %02x %02x %02x (总充电度数：%f)\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],v*0.001);
			strMsg = strMsg+str;
		}
		else
		{
			str.Format("(总充电度数错误)\r\n");
			strMsg = strMsg+strErr+str;
			m_Dlg->ProDecodeShow(strMsg);
			return;
		}
		i=i+4;

		if(IsValide(strErr,buffer+i,len-i,4))
		{
			int	v=getInt4Small(buffer+i);
			str.Format("%02x %02x %02x %02x (总充电度数：%f)\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],v*0.001);
			strMsg = strMsg+str;
		}
		else
		{
			str.Format("(总充电次数错误)\r\n");
			strMsg = strMsg+strErr+str;
			m_Dlg->ProDecodeShow(strMsg);
			return;
		}
		i=i+4;
	}
	strMsg = cmdTimeMsg(strMsg,strErr,buffer+i,len-i);
	i=i+3;
    strMsg = crcMsg(strMsg,strErr,buffer+i,len-i);
	m_Dlg->ProDecodeShow(strMsg);
}

void	CProto_IEC104::Pro_UP_162(CString strMsg,unsigned char *buffer,int len)
{
	CString str,strErr;
	if(m_Dlg->getProVersion() <20)
	{
		str="请选择V2.0以上版本";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}

	if(buffer==0 || len==0)
	{
		str="无数据内容";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	if(IsValide(strErr,buffer,len,8))
	{
		strMsg = getEpCode(strMsg,buffer);
	}
	else
	{
		str.Format("(终端机器编码长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	int i=8;
	if(IsValide(strErr,buffer+i,len-i,1))
	{

		str.Format("%02x ( 充电接口数量)\r\n",buffer[i]);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(充电接口数量错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}

	i++;
    int EntryNum=0;
	if(IsValide(strErr,buffer+i,len-i,4))
	{
        EntryNum=getInt4(buffer+i);
		str.Format("%02x (总条数)\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],EntryNum);
		
	}
	else
	{
		str.Format("(总条数错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}

	i=i+4;

	for(int j=0;j<EntryNum;j++)
	{
		strMsg=Pro_UP_46_v2(strMsg,buffer+i,len-i);
		i=i+129;
	}

	strMsg = cmdTimeMsg(strMsg,strErr,buffer+i,len-i);
	i=i+3;
    strMsg = crcMsg(strMsg,strErr,buffer+i,len-i);
	
	
	m_Dlg->ProDecodeShow(strMsg);
}



void	CProto_IEC104::Pro_Down_141(CString strMsg,unsigned char *buffer,int len)
{
	CString str,strErr;
	if(m_Dlg->getProVersion() <20)
	{
		str="请选择V2.0以上版本";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}

	if(buffer==0 || len==0)
	{
		str="无数据内容";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	if(IsValide(strErr,buffer,len,8))
	{
		strMsg = getEpCode(strMsg,buffer);
	}
	else
	{
		str.Format("(终端机器编码长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	int i=8;
	if(IsValide(strErr,buffer+i,len-i,2))
	{

		str.Format("%02x %02x ( 站地址:%d)\r\n",buffer[i],buffer[i+1],getInt2Small(buffer+i));
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(站地址错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	
	i=i+2;
	strMsg = cmdTimeMsg(strMsg,strErr,buffer+i,len-i);
	i=i+3;
    strMsg = crcMsg(strMsg,strErr,buffer+i,len-i);
	m_Dlg->ProDecodeShow(strMsg);
}


void	CProto_IEC104::Pro_Down_142(CString strMsg,unsigned char *buffer,int len)
{
	CString str,strErr;
	if(m_Dlg->getProVersion() <20)
	{
		str="请选择V2.0以上版本";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}

	if(buffer==0 || len==0)
	{
		str="无数据内容";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	int i=0;
	if(IsValide(strErr,buffer+i,len-i,2))
	{

		str.Format("%02x %02x ( 站地址:%d)\r\n",buffer[i],buffer[i+1],getInt2Small(buffer+i));
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(站地址错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
     i=i+2;
	if(IsValide(strErr,buffer,len-i,8))
	{
		strMsg = getEpCode(strMsg,buffer);
	}
	else
	{
		str.Format("(终端机器编码长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+8;
	if(IsValide(strErr,buffer+i,len-i,10))
	{
		CString s,s1;
		for(int j=i;j<i+10;j++)
		{
			str.Format("%02x ",buffer[j]);
			strMsg = strMsg+str;

			s1.Format("%c",buffer[j]);
			s = s+s1;
		}
		str.Format("(硬件型号：%s)\r\n",s);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(硬件型号长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+10;
	if(IsValide(strErr,buffer+i,len-i,1))
	{
		str.Format("%02x (硬件主版本号：%d)\r\n",buffer[i],buffer[i]);
		strMsg = strMsg+str;
			
	}
	else
	{
		str.Format("(硬件主版本号错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i++;
    if(IsValide(strErr,buffer+i,len-i,1))
	{
		str.Format("%02x (硬件子版本号：%d)",buffer[i],buffer[i]);
		strMsg = strMsg+str;
			
	}
	else
	{
		str.Format("(硬件子版本号错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}

	i=i+1;
	strMsg = cmdTimeMsg(strMsg,strErr,buffer+i,len-i);
	i=i+3;
    strMsg = crcMsg(strMsg,strErr,buffer+i,len-i);

	m_Dlg->ProDecodeShow(strMsg);
}

void	CProto_IEC104::Pro_Down_143(CString strMsg,unsigned char *buffer,int len)
{
	CString str,strErr;
	if(m_Dlg->getProVersion() <20)
	{
		str="请选择V2.0以上版本";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}

	if(buffer==0 || len==0)
	{
		str="无数据内容";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	int i=0;
	if(IsValide(strErr,buffer,len-i,8))
	{
		strMsg = getEpCode(strMsg,buffer);
	}
	else
	{
		str.Format("(终端机器编码长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+8;
	if(IsValide(strErr,buffer+i,len-i,2))
	{

		str.Format("%02x %02x ( 站地址:%d)\r\n",buffer[i],buffer[i+1],getInt2Small(buffer+i));
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(站地址错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
    i=i+2;
	if(IsValide(strErr,buffer+i,len-i,1))
	{

		str.Format("%02x ( 强制更新标识:%d)\r\n",buffer[i],buffer[i]);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(强制更新标识错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
     i=i+1;
	
	if(IsValide(strErr,buffer+i,len-i,10))
	{
		CString s,s1;
		for(int j=i;j<i+10;j++)
		{
			str.Format("%02x ",buffer[j]);
			strMsg = strMsg+str;

			s1.Format("%c",buffer[j]);
			s = s+s1;
		}
		str.Format("(硬件型号：%s)\r\n",s);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(硬件型号长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+10;
	if(IsValide(strErr,buffer+i,len-i,1))
	{
			str.Format("%02x (硬件主版本号：%d)\r\n",buffer[i],buffer[i]);
			strMsg = strMsg+str;
			
	}
	else
	{
			str.Format("(硬件主版本号错误)\r\n");
			strMsg = strMsg+strErr+str;
			m_Dlg->ProDecodeShow(strMsg);
			return;
	}
	i++;
    if(IsValide(strErr,buffer+i,len-i,1))
	{
			str.Format("%02x (硬件子版本号：%d)\r\n",buffer[i],buffer[i]);
			strMsg = strMsg+str;
			
	}
	else
	{
			str.Format("(硬件子版本号错误)\r\n");
			strMsg = strMsg+strErr+str;
			m_Dlg->ProDecodeShow(strMsg);
			return;
	}
	i++;
	

	if(IsValide(strErr,buffer+i,len-i,8))
	{
		CString s,s1;
		for(int j=i;j<i+8;j++)
		{
			str.Format("%02x ",buffer[j]);
			strMsg = strMsg+str;

			s1.Format("%c",buffer[j]);
			s = s+s1;
		}
		str.Format("(固件型号：%s)\r\n",s);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(固件型号长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+8;
	if(IsValide(strErr,buffer+i,len-i,1))
	{
			str.Format("%02x (固件主版本号：%d)\r\n",buffer[i],buffer[i]);
			strMsg = strMsg+str;
			
	}
	else
	{
			str.Format("(固件主版本号错误)\r\n");
			strMsg = strMsg+strErr+str;
			m_Dlg->ProDecodeShow(strMsg);
			return;
	}
	i++;
    if(IsValide(strErr,buffer+i,len-i,1))
	{
		str.Format("%02x (固件副版本号：%d)\r\n",buffer[i],buffer[i]);
		strMsg = strMsg+str;
			
	}
	else
	{
		str.Format("(固件副版本号错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
    i++;
    if(IsValide(strErr,buffer+i,len-i,2))
	{
		str.Format("%02x %02x(固件编译版本号：%d)\r\n",buffer[i],buffer[i+1],getInt2(buffer+i));
		strMsg = strMsg+str;
			
	}
	else
	{
		str.Format("(固件编译版本号错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+2;
	if(IsValide(strErr,buffer+i,len-i,1))
	{
		str.Format("%02x(充电桩程序文件是否存在:",buffer[i]);
		strMsg = strMsg+str;
		switch(buffer[i])
		{
		case 1:
			{
				str.Format("文件存在)\r\n");
				strMsg = strMsg+str;
			}
			break;
		case 0:
			{
				str.Format("文件不存在)\r\n");
				strMsg = strMsg+str;
			}
			break;
		default:
			{
				str.Format("其它)\r\n");
				strMsg = strMsg+str;
			}
			break;
		}
	}
	else
	{
		str.Format("(充电桩程序文件是否存在错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i++;
	if(IsValide(strErr,buffer+i,len-i,4))
	{
		str.Format("%02x %02x %02x %02x(文件总字节数:%d)\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],getInt4(buffer+i));
		strMsg = strMsg+str;
		
	}
	else
	{
		str.Format("(文件总字节数错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+4;
	if(IsValide(strErr,buffer+i,len-i,2))
	{
		str.Format("%02x %02x (分段数)\r\n",buffer[i],buffer[i+1],getInt2(buffer+i));
		strMsg = strMsg+str;
		
	}
	else
	{
		str.Format("(分段数错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+2;
	if(IsValide(strErr,buffer+i,len-i,32))
	{
		CString s,s1;
		for(int j=0;j<32;j++)
		{
			str.Format("%02x ",buffer[i+j]);
			strMsg = strMsg+str;

			s1.Format("%c",buffer[i+j]);
			s = s+s1;
		}
		str.Format("(密码MD5-32：%s)",s);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(密码MD5-32长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}

	i=i+32;
	strMsg = cmdTimeMsg(strMsg,strErr,buffer+i,len-i);
	i=i+3;
    strMsg = crcMsg(strMsg,strErr,buffer+i,len-i);
	m_Dlg->ProDecodeShow(strMsg);
}


void	CProto_IEC104::Pro_Down_144(CString strMsg,unsigned char *buffer,int len)
{
	CString str,strErr;
	if(m_Dlg->getProVersion() <20)
	{
		str="请选择V2.0以上版本";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}

	if(buffer==0 || len==0)
	{
		str="无数据内容";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	int i=0;
	if(IsValide(strErr,buffer,len-i,8))
	{
		strMsg = getEpCode(strMsg,buffer);
	}
	else
	{
		str.Format("(终端机器编码长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+8;
	if(IsValide(strErr,buffer+i,len-i,2))
	{

		str.Format("%02x %02x ( 站地址:%d)\r\n",buffer[i],buffer[i+1],getInt2Small(buffer+i));
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(站地址错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
    i=i+2;
	
	

	if(IsValide(strErr,buffer+i,len-i,8))
	{
		CString s,s1;
		for(int j=i;j<i+8;j++)
		{
			str.Format("%02x ",buffer[j]);
			strMsg = strMsg+str;

			s1.Format("%c",buffer[j]);
			s = s+s1;
		}
		str.Format("(固件型号：%s)\r\n",s);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(固件型号长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+8;
	if(IsValide(strErr,buffer+i,len-i,1))
	{
			str.Format("%02x (固件主版本号：%d)\r\n",buffer[i],buffer[i]);
			strMsg = strMsg+str;
			
	}
	else
	{
			str.Format("(固件主版本号错误)\r\n");
			strMsg = strMsg+strErr+str;
			m_Dlg->ProDecodeShow(strMsg);
			return;
	}
	i++;
    if(IsValide(strErr,buffer+i,len-i,1))
	{
			str.Format("%02x (固件副版本号：%d)\r\n",buffer[i],buffer[i]);
			strMsg = strMsg+str;
			
	}
	else
	{
			str.Format("(固件副版本号错误)\r\n");
			strMsg = strMsg+strErr+str;
			m_Dlg->ProDecodeShow(strMsg);
			return;
	}
    i++;
    if(IsValide(strErr,buffer+i,len-i,2))
	{
			str.Format("%02x %02x (固件编译版本号：%d)\r\n",buffer[i],buffer[i+1],getInt2(buffer+i));
			strMsg = strMsg+str;
			
	}
	else
	{
			str.Format("(固件编译版本号错误)\r\n");
			strMsg = strMsg+strErr+str;
			m_Dlg->ProDecodeShow(strMsg);
	     return;
     }
	 i=i+2;
			
	
	if(IsValide(strErr,buffer+i,len-i,2))
	{
		str.Format("%02x %02x (段索引)\r\n",buffer[i],buffer[i+1],getInt2(buffer+i));
		strMsg = strMsg+str;
		
	}
	else
	{
		str.Format("(段索引错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+2;
	if(IsValide(strErr,buffer+i,len-i,1))
	{
		str.Format("%02x(成功标识:",buffer[i]);
		strMsg = strMsg+str;
		switch(buffer[i])
		{
		case 1:
			{
				str.Format("成功)\r\n");
				strMsg = strMsg+str;
			}
			break;
		case 0:
			{
				str.Format("请求段索引不在文件段范围内)\r\n");
				strMsg = strMsg+str;
			}
			break;
		default:
			{
				str.Format("其它)\r\n");
				strMsg = strMsg+str;
			}
			break;
		}
	}
	else
	{
		str.Format("(成功标识错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i++;
	if(IsValide(strErr,buffer+i,len-i,32))
	{
		CString s,s1;
		for(int j=0;j<32;j++)
		{
			str.Format("%02x",buffer[i+j]);
			strMsg = strMsg+str;

		s1.Format("%c",buffer[i+j]);
			s = s+s1;
		}
		str.Format("(文件信息MD5验证码：%s)\r\n",s);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(文件信息MD5验证码错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+32;
	int datalen =0;
	if(IsValide(strErr,buffer+i,len-i,2))
	{
		datalen=getInt2(buffer+i);
		str.Format("%02x %02x (段大小:%d)\r\n",buffer[i],buffer[i+1],datalen);
		strMsg = strMsg+str;
		
	}
	else
	{
		str.Format("(段大小)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+2;
	if(IsValide(strErr,buffer+i,len-i,datalen))
	{
		CString s,s1;
		for(int j=0;j<len-i+1;j++)
		{
			str.Format("%02x",buffer[i+j]);
			strMsg = strMsg+str;
		}
		str.Format("(段数据)");
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(段数据错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}

	i=i+datalen;
	strMsg = cmdTimeMsg(strMsg,strErr,buffer+i,len-i);
	i=i+3;
    strMsg = crcMsg(strMsg,strErr,buffer+i,len-i);
	
	m_Dlg->ProDecodeShow(strMsg);
}




void	CProto_IEC104::Pro_Down_161(CString strMsg,unsigned char *buffer,int len)
{
	CString str,strErr;
	if(m_Dlg->getProVersion() <20)
	{
		str="请选择V2.0以上版本";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}

	if(buffer==0 || len==0)
	{
		str="无数据内容";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	
	if(IsValide(strErr,buffer,len,8))
	{
		strMsg = getEpCode(strMsg,buffer);
	}
	else
	{
		str.Format("(终端机器编码长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}

	int i=8;
	strMsg = cmdTimeMsg(strMsg,strErr,buffer+i,len-i);
	i=i+3;
    strMsg = crcMsg(strMsg,strErr,buffer+i,len-i);
	
	m_Dlg->ProDecodeShow(strMsg);
}

void	CProto_IEC104::Pro_Down_162(CString strMsg,unsigned char *buffer,int len)
{
	CString str,strErr;
	if(m_Dlg->getProVersion() <20)
	{
		str="请选择V2.0以上版本";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}

	if(buffer==0 || len==0)
	{
		str="无数据内容";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	
	if(IsValide(strErr,buffer,len,8))
	{
		strMsg = getEpCode(strMsg,buffer);
	}
	else
	{
		str.Format("(终端机器编码长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	int i=8;
	if(IsValide(strErr,buffer+i,len-i,1))
	{

		str.Format("%02x ( 充电接口数量)\r\n",buffer[i]);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(充电接口数量错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	 i++;
	if(IsValide(strErr,buffer+i,len-i,4))
	{

		str.Format("%02x %02x %02x %02x (起始位置%d)\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],getInt4(buffer+i));
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(起始位置错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}

	i=i+4;
	strMsg = cmdTimeMsg(strMsg,strErr,buffer+i,len-i);
	i=i+3;
    strMsg = crcMsg(strMsg,strErr,buffer+i,len-i);
	
	
	m_Dlg->ProDecodeShow(strMsg);
}

int   CProto_IEC104::getInt2Small(unsigned char *buffer)
{
	int value= buffer[0]*0x100+buffer[1];
	return value;
}


int   CProto_IEC104::getInt2(unsigned char *buffer)
{
	int value= buffer[1]*0x100+buffer[0];
	return value;
}


int   CProto_IEC104::getInt4(unsigned char *buffer)
{
	int value= buffer[0]*0x1000000+buffer[1]*0x10000+buffer[2]*0x100+buffer[3];
	return value;
}


int   CProto_IEC104::getInt4Small(unsigned char *buffer)
{
	int value= buffer[3]*0x1000000+buffer[2]*0x10000+buffer[1]*0x100+buffer[0];
	return value;
}

void CProto_IEC104::Pro_Down_147(CString strMsg,unsigned char *buffer,int len)
{
	CString str,strErr;
	if(m_Dlg->getProVersion() <20)
	{
		str="请选择V2.0以上版本";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}

	if(buffer==0 || len==0)
	{
		str="无数据内容";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	
	if(IsValide(strErr,buffer,len,8))
	{
		strMsg = getEpCode(strMsg,buffer);
	}
	else
	{
		str.Format("(终端机器编码长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	int i=8;
	if(IsValide(strErr,buffer+i,len-i,2))
	{

		str.Format("%02x %02x(站地址%d)\r\n",buffer[i],buffer[i+1],getInt2(buffer+i));
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(站地址错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}

	i=i+2;
	strMsg = cmdTimeMsg(strMsg,strErr,buffer+i,len-i);
	i=i+3;
    strMsg = crcMsg(strMsg,strErr,buffer+i,len-i);
	m_Dlg->ProDecodeShow(strMsg);
}

void	CProto_IEC104::Pro_UP_147(CString strMsg,unsigned char *buffer,int len)
{
	CString str,strErr;
	if(m_Dlg->getProVersion() <20)
	{
		str="请选择V2.0以上版本";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}

	if(buffer==0 || len==0)
	{
		str="无数据内容";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	
	if(IsValide(strErr,buffer,len,8))
	{
		strMsg = getEpCode(strMsg,buffer);
	}
	else
	{
		str.Format("(终端机器编码长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	int i=8;
	if(IsValide(strErr,buffer+i,len-i,2))
	{

		str.Format("%02x %02x(站地址%d)\r\n",buffer[i],buffer[i+1],getInt2(buffer+i));
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(站地址错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+2;
	if(IsValide(strErr,buffer+i,len-i,1))
	{

		str.Format("%02x(信号强度值%d)\r\n",buffer[i],buffer[i]);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(信号强度值错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}

	i=i+1;
	timeMsg(strMsg,buffer+i,"系统时间");
	i=i+7;
	strMsg = cmdTimeMsg(strMsg,strErr,buffer+i,len-i);
	i=i+3;
    strMsg = crcMsg(strMsg,strErr,buffer+i,len-i);
	
	m_Dlg->ProDecodeShow(strMsg);
}

void	CProto_IEC104::Pro_Down_148_v4(CString strMsg,unsigned char *buffer,int len)
{
	CString str,strErr;
	if(m_Dlg->getProVersion() <40)
	{
		str="请选择V4.0以上版本";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}

	if(buffer==0 || len==0)
	{
		str="无数据内容";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	
	int i=0;
	if(IsValide(strErr,buffer+i,len-i,2))
	{

		str.Format("%02x %02x(站地址%d)\r\n",buffer[i],buffer[i+1],getInt2(buffer+i));
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(站地址错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+2;
	int epNum =0;
	if(IsValide(strErr,buffer+i,len-i,2))
	{
        epNum = getInt2(buffer+i);
		str.Format("%02x %02x(电桩个数%d)\r\n",buffer[i],buffer[i+1],epNum);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(电桩个数错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+2;
	for(int j=0;j<epNum;j++)
	{
		if(IsValide(strErr,buffer+i,len-i,8))
		{
			strMsg = getEpCode(strMsg,buffer);
		}
		else
		{
			str.Format("(终端机器编码长度错误)\r\n");
			strMsg = strMsg+strErr+str;
			m_Dlg->ProDecodeShow(strMsg);
			return;
		}
		i=i+8;
	}
	strMsg = cmdTimeMsg(strMsg,strErr,buffer+i,len-i);
	i=i+3;
    strMsg = crcMsg(strMsg,strErr,buffer+i,len-i);
	
	m_Dlg->ProDecodeShow(strMsg);
}

void	CProto_IEC104::Pro_UP_148_v4(CString strMsg,unsigned char *buffer,int len)
{
	CString str,strErr;
	if(m_Dlg->getProVersion() <40)
	{
		str="请选择V4.0以上版本";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}

	if(buffer==0 || len==0)
	{
		str="无数据内容";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	
	int i=0;
	if(IsValide(strErr,buffer+i,len-i,2))
	{

		str.Format("%02x %02x(站地址%d)\r\n",buffer[i],buffer[i+1],getInt2(buffer+i));
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(站地址错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+2;
	if(IsValide(strErr,buffer+i,len-i,1))
	{
		str.Format("%02x(成功标识:",buffer[i]);
		strMsg = strMsg+str;
		switch(buffer[i])
		{
		case 0:
			{
				str.Format("失败)\r\n");
				strMsg = strMsg+str;
			}
			break;
		default:
			{
				str.Format("成功)\r\n");
				strMsg = strMsg+str;
			}
			break;
		}
	}
	else
	{
		str.Format("(成功标识错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i++;
	if(IsValide(strErr,buffer+i,len-i,1))
	{
		str.Format("%02x (错误原因%d)\r\n",buffer[i],buffer[i]);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(错误原因错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i++;
	strMsg = cmdTimeMsg(strMsg,strErr,buffer+i,len-i);
	i=i+3;
    strMsg = crcMsg(strMsg,strErr,buffer+i,len-i);
	
	m_Dlg->ProDecodeShow(strMsg);
}

void	CProto_IEC104::Pro_Down_41_v4(CString strMsg,unsigned char *buffer,int len)
{
	CString str,strErr;
	if(buffer==0 || len==0)
	{
		str="无数据内容";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}

	if(IsValide(strErr,buffer,len,8))
	{
		strMsg = getEpCode(strMsg,buffer);
	}
	else
	{
		str.Format("(终端机器编码长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
    int	i=8;
	if(IsValide(strErr,buffer+i,len-i,1))
	{
		str.Format("%02x(充电接口)\r\n",buffer[8]);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(充电接口错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=9;
	if(IsValide(strErr,buffer+i,len-i,6))
	{
		strMsg = getPhone(strMsg,buffer+i);
	}
	else
	{
		str.Format("(手机号长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}

	i=15;
	unsigned int v =0;
	if(IsValide(strErr,buffer+i,len-i,4))
	{
		v=getInt4Small(buffer+i);
		str.Format("%02x %02x %02x %02x (余额：%d)\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],v*0.01);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(余额错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=19;
	if(IsValide(strErr,buffer+i,len-i,4))
	{
		v=getInt4Small(buffer+i);
		str.Format("%02x %02x %02x %02x (最小充电金额：%d)\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],v);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(最小充电金额错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=23;
	if(IsValide(strErr,buffer+i,len-i,1))
	{
		str.Format("%02x(二维码方式：",buffer[i]);
		strMsg = strMsg+str;
		switch(buffer[i])
		{
		case 1:
			{
				str.Format("是二维码充电)\r\n");
				strMsg = strMsg+str;
			}
			break;
		default:
			{
				str.Format("其它)\r\n");
				strMsg = strMsg+str;
			}
			break;
		}
	}
	else
	{
		str.Format("(二维码方式错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i++;
	if(IsValide(strErr,buffer+i,len-i,1))
	{
		str.Format("%02x(扣费方式：",buffer[i]);
		strMsg = strMsg+str;
		switch(buffer[i])
		{
		case 1:
			{
				str.Format("预冻结)\r\n");
				strMsg = strMsg+str;
			}
			break;
		case 2:
			{
				str.Format("后付费)\r\n");
				strMsg = strMsg+str;
			}
			break;
		default:
			{
				str.Format("其它)\r\n");
				strMsg = strMsg+str;
			}
			break;
		}
	}
	else
	{
		str.Format("(扣费方式错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i++;
	if(IsValide(strErr,buffer+i,len-i,4))
	{
		   double	v=getInt4Small(buffer+i);

			str.Format("%02x %02x %02x %02x (预充金额：%f)\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],v*0.01);
			strMsg = strMsg+str;
	}
	else
	{
			str.Format("(预充金额错误)\r\n");
			strMsg = strMsg+strErr+str;
			m_Dlg->ProDecodeShow(strMsg);
			return;
	}
	i=i+4;
	if(IsValide(strErr,buffer+i,len-i,32))
	{
			CString s,s1;
			for(int k=0;k<32;k++)
			{
				str.Format("%02x",buffer[i+k]); 
				strMsg = strMsg+str;
			    s1.Format("%c",buffer[i+k]);
			    s = s+s1;
		   }
		   str.Format("用户密码MD5-32：%s)",s);
		   strMsg = strMsg+str;
		
	}
	else
	{
			str.Format("(用户密码MD5-32错误)\r\n");
			strMsg = strMsg+strErr+str;
			m_Dlg->ProDecodeShow(strMsg);
			return;
	}
	i=i+32;
	if(IsValide(strErr,buffer+i,len-i,16))
	{
			for(int k=0;k<16;k++)
			{
				str.Format("%02x",buffer[i+k]); 
				strMsg = strMsg+str;
			}

			str.Format("(流水号)\r\n"); 
			strMsg = strMsg+str;
	}
	else
	{
			str.Format("(流水号错误)\r\n");
			strMsg = strMsg+strErr+str;
			m_Dlg->ProDecodeShow(strMsg);
			return;
	}
	i=i+16;
	if(IsValide(strErr,buffer+i,len-i,1))
	{
		str.Format("%02x(显示电价：",buffer[i]);
		strMsg = strMsg+str;
		switch(buffer[i])
		{
		case 1:
			{
				str.Format("显示)\r\n");
				strMsg = strMsg+str;
			}
			break;
		default:
			{
				str.Format("不显示)\r\n");
				strMsg = strMsg+str;
			}
			break;
		}
	}
	else
	{
		str.Format("(显示电价错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i++;
	if(IsValide(strErr,buffer+i,len-i,4))
	{
		double	v=getInt4Small(buffer+i);

		str.Format("%02x %02x %02x %02x (尖时刻电价：%f)\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],v*0.001);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(尖时刻电价错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+4;
	if(IsValide(strErr,buffer+i,len-i,4))
	{
		double	v=getInt4Small(buffer+i);

		str.Format("%02x %02x %02x %02x (峰时刻电价：%f)\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],v*0.001);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(峰时刻电价错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+4;
	if(IsValide(strErr,buffer+i,len-i,4))
	{
		double	v=getInt4Small(buffer+i);

		str.Format("%02x %02x %02x %02x (平时刻电价：%f)\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],v*0.001);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(平时刻电价错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+4;
	if(IsValide(strErr,buffer+i,len-i,4))
	{
		double	v=getInt4Small(buffer+i);

		str.Format("%02x %02x %02x %02x (谷时刻电价：%f)\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],v*0.001);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(谷时刻电价错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+4;
	if(IsValide(strErr,buffer+i,len-i,4))
	{
		double	v=getInt4Small(buffer+i);

		str.Format("%02x %02x %02x %02x (服务费：%f)\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],v*0.001);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(服务费错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+4;
	
	strMsg = cmdTimeMsg(strMsg,strErr,buffer+i,len-i);
	i=i+3;
    strMsg = crcMsg(strMsg,strErr,buffer+i,len-i);
	m_Dlg->ProDecodeShow(strMsg);
}

//解释上行数据
void	CProto_IEC104::UP_RxProcV4(CString strMsg,unsigned char *buffer,int len)
{
	   CString str1;
		
		strMsg=headMsg1(strMsg,buffer+4,"上行业务数据");
		int i=16-3;
		switch(buffer[i])
	   {
	   case 1:
           str1.Format("\r\n%02x(计费模型请求上行数据%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_UP_1(strMsg,buffer+i+1,len-i-1);

		   break;
	   case 2:
		   str1.Format("\r\n%02x(下发计费模型结果数据%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_UP_2_v2(strMsg,buffer+i+1,len-i-1);
		   break;
	   case 3:
		    str1.Format("\r\n%02x(私有充电桩下发充电桩运营时间%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_UP_3(strMsg,buffer+i+1,len-i-1);
		   break;
	    case 11:
		   str1.Format("\r\n%02x(无卡用户名密码鉴权上行数据%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_UP_11_v2(strMsg,buffer+i+1,len-i-1);
		   break;
	   case 12:
		   str1.Format("\r\n%02x(无卡验证码鉴权上行数据%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_UP_12_v2(strMsg,buffer+i+1,len-i-1);
		   break;
	   case 13:
		   str1.Format("\r\n%02x(不圈存卡鉴权上行数据%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_UP_13_v2(strMsg,buffer+i+1,len-i-1);
		   break;
	   case 31:
		   str1.Format("\r\n%02x(预约锁定上行数据%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_UP_31_v2(strMsg,buffer+i+1,len-i-1);
		   break;
	   case 32:
		    str1.Format("\r\n%02x(电桩应答预约报文的确认%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_UP_32_v2(strMsg,buffer+i+1,len-i-1);
		   break;
	   case 33:
		    str1.Format("\r\n%02x(取消预约上行应答数据%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_UP_33_v2(strMsg,buffer+i+1,len-i-1);
		   break;
	   case 41:
		    str1.Format("\r\n%02x(启动充电应答上行据%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_UP_41_v2(strMsg,buffer+i+1,len-i-1);
		   break;
	   case 42:
		   str1.Format("\r\n%02x(充电事件上行数据%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_UP_42_v2(strMsg,buffer+i+1,len-i-1);
		   break;
	    case 43:
			str1.Format("\r\n%02x(用户停止充电应答上行数据%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_UP_43_v2(strMsg,buffer+i+1,len-i-1);
		   break;
	    case 45:
			str1.Format("\r\n%02x(结束充电事件上行数据%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_UP_45_v2(strMsg,buffer+i+1,len-i-1);
		   break;
	    case 44:
			str1.Format("\r\n%02x(不圈存卡充电冻结金额上行数据%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_UP_44_v2(strMsg,buffer+i+1,len-i-1);
		   break;
	    case 46:
			str1.Format("\r\n%02x(充电消费记录上行数据%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_UP_46_v2(strMsg,buffer+i+1,len-i-1);
		   break;
	    case 47:
			str1.Format("\r\n%02x(消费余额告警信息%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_UP_47_v2(strMsg,buffer+i+1,len-i-1);
		   break;
	   case 49:
			str1.Format("\r\n%02x(电桩识别码请求上行数据%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_UP_49_v4(strMsg,buffer+i+1,len-i-1);
		   break;
	    case 101:
		   str1.Format("\r\n%02x(电桩设备支持报告上行数据%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_UP_101(strMsg,buffer+i+1,len-i-1);
		   break;
	    case 104:
			 str1.Format("\r\n%02x(打开枪锁装置应答上行数据%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_UP_104(strMsg,buffer+i+1,len-i-1);
		   break;
	    case 105:
			str1.Format("\r\n%02x(锁枪失败告警上行数据%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_UP_105(strMsg,buffer+i+1,len-i-1);
		   break;
	    case 141:
		str1.Format("\r\n%02x(产品信息查询应答%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_UP_141(strMsg,buffer+i+1,len-i-1);
		   break;
	    case 143:
			str1.Format("\r\n%02x(远端设备二进制程序文件下载概要信息请求%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_UP_143(strMsg,buffer+i+1,len-i-1);
		   break;
	    case 144:
			str1.Format("\r\n%02x(远端设备二进制程序文件分段下载请求%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_UP_144(strMsg,buffer+i+1,len-i-1);
		   break;
	   
	   case 145:
		   str1.Format("\r\n%02x(远端设备二进制程序文件更新确认应答%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_UP_145(strMsg,buffer+i+1,len-i-1);
		   break;
	   case 161:
		   str1.Format("\r\n%02x(充电信息统计上新数据%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_UP_161(strMsg,buffer+i+1,len-i-1);
		   break;
	   case 162:
		   str1.Format("\r\n%02x(电桩业务数据查询应答报文%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_UP_162(strMsg,buffer+i+1,len-i-1);
		   break;
	   case 147:
		   str1.Format("\r\n%02x(远端设备通讯信号强度应答%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_UP_147(strMsg,buffer+i+1,len-i-1);
		   break;
	    case 148:
		   str1.Format("\r\n%02x(集中器桩体配置应答%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_UP_148_v4(strMsg,buffer+i+1,len-i-1);
		   break;
	    case 149:
		   str1.Format("\r\n%02x(查询集中器桩体配置应答%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_UP_149_v4(strMsg,buffer+i+1,len-i-1);
		   break;
	    case 150:
		   str1.Format("\r\n%02x(查询远端设备费率应答%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_UP_150_v4(strMsg,buffer+i+1,len-i-1);
		   break;
	    case 151:
		   str1.Format("\r\n%02x(查询远端flash ram应答%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_UP_151_v4(strMsg,buffer+i+1,len-i-1);
		   break;
	    case 152:
		   str1.Format("\r\n%02x(查询临时充电次数应答应答%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_UP_152_v4(strMsg,buffer+i+1,len-i-1);
		   break;
	    case 153:
		   str1.Format("\r\n%02x(设置临时充电次数应答应答%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_UP_153_v4(strMsg,buffer+i+1,len-i-1);
		   break;
	   default:
		    str1.Format("记录类型错误");
		   strMsg =strMsg+str1;
		   m_Dlg->ProDecodeShow(strMsg);
		   break;
	   }

       

}


//解释下行数据
void	CProto_IEC104::Down_RxProcV4(CString strMsg,unsigned char *buffer,int len)
{
	CString str1,strErr;
	strMsg=headMsg1(strMsg,buffer+4,"业务下行数据");
   
		int i=16-3;
		switch(buffer[i])
	   {
	   case 1:
		   str1.Format("\r\n%02x(下发计费模型下行数据：%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_Down_1_v2(strMsg,buffer+i+1,len-i-1);
		   break;
	   case 3:
		    str1.Format("\r\n%02x(私有充电桩下发充电桩运营时间：%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_Down_3(strMsg,buffer+i+1,len-i-1);
		   break;
	   case 11:
		   str1.Format("\r\n%02x(无卡鉴权下行数据：%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_Down_11_v2(strMsg,buffer+i+1,len-i-1);
		   break;
	   case 13:
		   str1.Format("\r\n%02x(不圈存卡鉴权下行应答数据：%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_Down_13_v2(strMsg,buffer+i+1,len-i-1);
		   break;
	   case 31:
		    str1.Format("\r\n%02x(预约：%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_Down_31_v2(strMsg,buffer+i+1,len-i-1);
		   break;
	   case 32:
		    str1.Format("\r\n%02x(电桩应答预约报文的确认：%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_Down_32_v2(strMsg,buffer+i+1,len-i-1);
		   break;
	   case 33:
		   str1.Format("\r\n%02x(取消预约：%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_Down_33_v2(strMsg,buffer+i+1,len-i-1);
		   break;
	   case 41:
		    str1.Format("\r\n%02x(启动充电：%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_Down_41_v4(strMsg,buffer+i+1,len-i-1);
		   break;
	    case 42:
		    str1.Format("\r\n%02x(充电事件的确认报文：%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_Down_42_v2(strMsg,buffer+i+1,len-i-1);
		   break;
	   case 43:
			str1.Format("\r\n%02x(停止充电：%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_Down_23(strMsg,buffer+i+1,len-i-1);
		   break;
	   case 44:
			str1.Format("\r\n%02x(不圈存卡充电冻结金额下行数据：%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_Down_44_v2(strMsg,buffer+i+1,len-i-1);
		   break;
	   case 46:
			str1.Format("\r\n%02x(消费记录确认下行数据：%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_Down_46_v2(strMsg,buffer+i+1,len-i-1);
		   break;
	   case 49:
			str1.Format("\r\n%02x(电桩识别码应答下行数据.：%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_Down_49_v4(strMsg,buffer+i+1,len-i-1);
		   break;

	   case 102:
		   str1.Format("\r\n%02x(近场呼叫：%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_Down_102(strMsg,buffer+i+1,len-i-1);
		   break;
	    case 103:
		   str1.Format("\r\n%02x(降地锁：%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_Down_103(strMsg,buffer+i+1,len-i-1);
		   break;
	    case 104:
		   str1.Format("\r\n%02x(打开锁枪装置下行数据：%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_Down_28(strMsg,buffer+i+1,len-i-1);
		   break;
	    case 141:
			str1.Format("\r\n%02x(产品信息查询：%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_Down_141(strMsg,buffer+i+1,len-i-1);
		   break;
	    case 142:
			str1.Format("\r\n%02x(强制更新远端设备软件下行数据：%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_Down_142(strMsg,buffer+i+1,len-i-1);
		   break;
	    case 143:
			str1.Format("\r\n%02x(充电桩文件概要信息下行数据：%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_Down_143(strMsg,buffer+i+1,len-i-1);
		   break;
	    case 144:
			str1.Format("\r\n%02x(充电桩文件分段下行数据：%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_Down_144(strMsg,buffer+i+1,len-i-1);
		   break;
	   
	    
	
	   case 161:
		   str1.Format("\r\n%02x(充电桩统计信息下行数据：%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_Down_161(strMsg,buffer+i+1,len-i-1);
		   break;
	    case 162:
		   str1.Format("\r\n%02x(电桩业务数据查询：%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_Down_162(strMsg,buffer+i+1,len-i-1);
		   break;
	   case 146:
		   str1.Format("\r\n%02x(更新充电桩后台服务器IP 下行数据：%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_Down_26(strMsg,buffer+i+1,len-i-1);
		   break;
	   case 147:
		   str1.Format("\r\n%02x(查询远端设备通讯信号强度：%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_Down_147(strMsg,buffer+i+1,len-i-1);
		   break;
	    case 148:
		   str1.Format("\r\n%02x(集中器桩体配置：%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_Down_148_v4(strMsg,buffer+i+1,len-i-1);
		   break;
	    case 151:
		   str1.Format("\r\n%02x(查询远端flash ram：%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_Down_151_v4(strMsg,buffer+i+1,len-i-1);
		   break;
	    case 149:
		   str1.Format("\r\n%02x(查询集中器桩体配置：%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_Down_149_v4(strMsg,buffer+i+1,len-i-1);
		   break;
	    case 150:
		   str1.Format("\r\n%02x(查询远端设备费率：%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_Down_150_v4(strMsg,buffer+i+1,len-i-1);
		   break;
	    case 152:
		   str1.Format("\r\n%02x(查询临时充电次数：%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_Down_152_v4(strMsg,buffer+i+1,len-i-1);
		   break;
	    case 153:
		   str1.Format("\r\n%02x(设置临时充电次数：%d)\r\n\r\n",buffer[i],buffer[i]);
		   strMsg =strMsg+str1;
		   Pro_Down_153_v4(strMsg,buffer+i+1,len-i-1);
		   break;
	    
	   default:
		    str1.Format("记录类型错误");
		   strMsg =strMsg+str1;
		   m_Dlg->ProDecodeShow(strMsg);
		   break;
	   }

}

void	CProto_IEC104::Pro_Down_149_v4(CString strMsg,unsigned char *buffer,int len)
{
	CString str,strErr;
	if(m_Dlg->getProVersion() <40)
	{
		str="请选择V4.0以上版本";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}

	if(buffer==0 || len==0)
	{
		str="无数据内容";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	
	int i=0;
	if(IsValide(strErr,buffer+i,len-i,2))
	{

		str.Format("%02x %02x(站地址%d)\r\n",buffer[i],buffer[i+1],getInt2(buffer+i));
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(站地址错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+2;
	strMsg = cmdTimeMsg(strMsg,strErr,buffer+i,len-i);
	i=i+3;
    strMsg = crcMsg(strMsg,strErr,buffer+i,len-i);
	
	m_Dlg->ProDecodeShow(strMsg);
}

void	CProto_IEC104::Pro_UP_149_v4(CString strMsg,unsigned char *buffer,int len)
{
	CString str,strErr;
	if(m_Dlg->getProVersion() <40)
	{
		str="请选择V4.0以上版本";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}

	if(buffer==0 || len==0)
	{
		str="无数据内容";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	
	int i=0;
	if(IsValide(strErr,buffer+i,len-i,2))
	{

		str.Format("%02x %02x(站地址%d)\r\n",buffer[i],buffer[i+1],getInt2(buffer+i));
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(站地址错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+2;
	
	int num=0;
	if(IsValide(strErr,buffer+i,len-i,2))
	{
		num = getInt2(buffer+i);
		str.Format("%02x %02x (电桩个数%d)\r\n",buffer[i],buffer[i+1],num);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(电桩个数错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+2;
	for(int j=0;j<num;j++)
	{
		if(IsValide(strErr,buffer+i,len-i,8))
	    {
	    	strMsg = getEpCode(strMsg,buffer);
    	}
    	else
    	{
		     str.Format("(终端机器编码长度错误)\r\n");
		    strMsg = strMsg+strErr+str;
		    m_Dlg->ProDecodeShow(strMsg);
		    break;
	     }
		i=i+8;
	}

	strMsg = cmdTimeMsg(strMsg,strErr,buffer+i,len-i);
	i=i+3;
    strMsg = crcMsg(strMsg,strErr,buffer+i,len-i);
	
	m_Dlg->ProDecodeShow(strMsg);
}



void	CProto_IEC104::Pro_Down_150_v4(CString strMsg,unsigned char *buffer,int len)
{
	CString str,strErr;
	if(m_Dlg->getProVersion() <40)
	{
		str="请选择V4.0以上版本";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}

	if(buffer==0 || len==0)
	{
		str="无数据内容";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	
	int i=0;
	if(IsValide(strErr,buffer+i,len-i,8))
	{
	   strMsg = getEpCode(strMsg,buffer);
    }
    else
    {
		str.Format("(终端机器编码长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		 m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+8;

	if(IsValide(strErr,buffer+i,len-i,2))
	{

		str.Format("%02x %02x(站地址%d)\r\n",buffer[i],buffer[i+1],getInt2(buffer+i));
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(站地址错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+2;


	strMsg = cmdTimeMsg(strMsg,strErr,buffer+i,len-i);
	i=i+3;
    strMsg = crcMsg(strMsg,strErr,buffer+i,len-i);
	
	m_Dlg->ProDecodeShow(strMsg);
}

void	CProto_IEC104::Pro_UP_150_v4(CString strMsg,unsigned char *buffer,int len)
{
	CString str,strErr;
	if(m_Dlg->getProVersion() <40)
	{
		str="请选择V4.0以上版本";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}

	if(buffer==0 || len==0)
	{
		str="无数据内容";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	
	int i=0;
	if(IsValide(strErr,buffer+i,len-i,8))
	{
	   strMsg = getEpCode(strMsg,buffer);
    }
    else
    {
		str.Format("(终端机器编码长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		 m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+8;

	if(IsValide(strErr,buffer+i,len-i,8))
	{

		str.Format("%02x %02x %02x %02x %02x %02x %02x %02x(计费模型ID)\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],buffer[i+4],buffer[i+5],buffer[i+6],buffer[i+7]);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(计费模型ID错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	
	i=i+8;
    timeMsg(strMsg,buffer+i,"生效日期");
	i=i+7;
	timeMsg(strMsg,buffer+i,"失效日期");
	i=i+7;
	if(IsValide(strErr,buffer+i,len-i,2))
	{

		str.Format("%02x %02x(预冻结金额:%0.2f)\r\n",buffer[i],buffer[i+1],getInt2(buffer+i)*0.01);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(预冻结金额错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+2;

	if(IsValide(strErr,buffer+i,len-i,2))
	{

		str.Format("%02x %02x(最小冻结金额:%0.2f)\r\n",buffer[i],buffer[i+1],getInt2(buffer+i)*0.01);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(最小冻结金额错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+2;
	int num=0;
	if(IsValide(strErr,buffer+i,len-i,1))
	{

		num = buffer[i];
		str.Format("%02x(时段数)\r\n",buffer[i]);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(时段数错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+1;
	for(int j=0;j<num;j++)
	{
     	if(IsValide(strErr,buffer+i,len-i,4))
	    {

		    str.Format("%02x %02x %02x %02x(时段%d开始时间:%d)\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],j,getInt4(buffer+i));
		    strMsg = strMsg+str;
	    }
	    else
	    {
		    str.Format("(结束时间错误)\r\n");
		    strMsg = strMsg+strErr+str;
		    m_Dlg->ProDecodeShow(strMsg);
		    return;
	     }
	     i=i+4;
	     if(IsValide(strErr,buffer+i,len-i,4))
	     {

		     str.Format("%02x %02x %02x %02x(时段%d结束时间:%d)\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],j,getInt4(buffer+i));
		     strMsg = strMsg+str;
	     }
	     else
	     {
	        	str.Format("(结束时间错误)\r\n");
	      	strMsg = strMsg+strErr+str;
	     	m_Dlg->ProDecodeShow(strMsg);
	     	return;
	     }
	     i=i+4;
	     if(IsValide(strErr,buffer+i,len-i,1))
	     {

		     str.Format("%02x (时段%d标志\r\n",buffer[i],j);
		     strMsg = strMsg+str;
     	 }
	     else
	     {
	      	str.Format("(标志错误)\r\n");
	    	strMsg = strMsg+strErr+str;
	        	m_Dlg->ProDecodeShow(strMsg);
		      return;
	     }
	     i=i+1;
	}

	 if(IsValide(strErr,buffer+i,len-i,4))
	  {

		    str.Format("%02x %02x %02x %02x(尖时段电价:%0.3f)\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],getInt4(buffer+i)*0.001);
		    strMsg = strMsg+str;
	    }
	    else
	    {
		    str.Format("(尖时段电价错误)\r\n");
		    strMsg = strMsg+strErr+str;
		    m_Dlg->ProDecodeShow(strMsg);
		    return;
	     }
	     i=i+4;
	  if(IsValide(strErr,buffer+i,len-i,4))
	  {

		    str.Format("%02x %02x %02x %02x(峰时段电价:%0.3f)\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],getInt4(buffer+i)*0.001);
		    strMsg = strMsg+str;
	  }
	  else
	  {
		    str.Format("(峰时段电价错误)\r\n");
		    strMsg = strMsg+strErr+str;
		    m_Dlg->ProDecodeShow(strMsg);
		    return;
	   }
	  i=i+4;
    if(IsValide(strErr,buffer+i,len-i,4))
	  {

		    str.Format("%02x %02x %02x %02x(平时段电价:%0.3f)\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],getInt4(buffer+i)*0.001);
		    strMsg = strMsg+str;
	  }
	  else
	  {
		    str.Format("(平时段电价错误)\r\n");
		    strMsg = strMsg+strErr+str;
		    m_Dlg->ProDecodeShow(strMsg);
		    return;
	   }
	  i=i+4;
  if(IsValide(strErr,buffer+i,len-i,4))
	  {

		    str.Format("%02x %02x %02x %02x(谷时段电价:%0.3f)\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],getInt4(buffer+i)*0.001);
		    strMsg = strMsg+str;
	  }
	  else
	  {
		    str.Format("(谷时段电价错误)\r\n");
		    strMsg = strMsg+strErr+str;
		    m_Dlg->ProDecodeShow(strMsg);
		    return;
	   }
	  i=i+4;
  if(IsValide(strErr,buffer+i,len-i,4))
	  {

		    str.Format("%02x %02x %02x %02x(预约费率:%0.3f)\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],getInt4(buffer+i)*0.001);
		    strMsg = strMsg+str;
	  }
	  else
	  {
		    str.Format("(预约费率错误)\r\n");
		    strMsg = strMsg+strErr+str;
		    m_Dlg->ProDecodeShow(strMsg);
		    return;
	   }
	  i=i+4;
  if(IsValide(strErr,buffer+i,len-i,4))
	  {

		    str.Format("%02x %02x %02x %02x(服务费:%0.3f)\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],getInt4(buffer+i)*0.001);
		    strMsg = strMsg+str;
	  }
	  else
	  {
		    str.Format("(服务费错误)\r\n");
		    strMsg = strMsg+strErr+str;
		    m_Dlg->ProDecodeShow(strMsg);
		    return;
	   }
	  i=i+4;
	   if(IsValide(strErr,buffer+i,len-i,4))
	  {

		    str.Format("%02x %02x %02x %02x(告警金额:%0.2f)\r\n",buffer[i],buffer[i+1],buffer[i+2],buffer[i+3],getInt4(buffer+i)*0.01);
		    strMsg = strMsg+str;
	  }
	  else
	  {
		    str.Format("(告警金额错误)\r\n");
		    strMsg = strMsg+strErr+str;
		    m_Dlg->ProDecodeShow(strMsg);
		    return;
	   }
	  i=i+4;
	strMsg = cmdTimeMsg(strMsg,strErr,buffer+i,len-i);
	i=i+3;
    strMsg = crcMsg(strMsg,strErr,buffer+i,len-i);
	
	m_Dlg->ProDecodeShow(strMsg);
}



void	CProto_IEC104::Pro_Down_151_v4(CString strMsg,unsigned char *buffer,int len)
{
	CString str,strErr;
	if(m_Dlg->getProVersion() <40)
	{
		str="请选择V4.0以上版本";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}

	if(buffer==0 || len==0)
	{
		str="无数据内容";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	
	int i=0;
	if(IsValide(strErr,buffer+i,len-i,8))
	{
	   strMsg = getEpCode(strMsg,buffer);
    }
    else
    {
		str.Format("(终端机器编码长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		 m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+8;

	if(IsValide(strErr,buffer+i,len-i,2))
	{

		str.Format("%02x %02x(站地址%d)\r\n",buffer[i],buffer[i+1],getInt2(buffer+i));
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(站地址错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+2;
	if(IsValide(strErr,buffer+i,len-i,2))
	{

		str.Format("%02x %02x(类型：",buffer[i],buffer[i+1]);
		int type = getInt2(buffer+i);
		switch(type)
		{
		case 1:
				str.Format("外部flash)\r\n");
			break;
        case 2:
				str.Format("内部flash)\r\n");
			break;
		 case 3:
				str.Format("ram)\r\n");
			break;
		 default:
             str.Format("未知)\r\n");
		}
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(类型错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+2;
	if(IsValide(strErr,buffer+i,len-i,2))
	{

		str.Format("%02x %02x(开始未知：%d",buffer[i],buffer[i+1],getInt2(buffer+i));
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(开始未知错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+2;
	if(IsValide(strErr,buffer+i,len-i,2))
	{

		str.Format("%02x %02x(长度：%d",buffer[i],buffer[i+1],getInt2(buffer+i));
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+2;


	strMsg = cmdTimeMsg(strMsg,strErr,buffer+i,len-i);
	i=i+3;
    strMsg = crcMsg(strMsg,strErr,buffer+i,len-i);
	
	m_Dlg->ProDecodeShow(strMsg);
}

void	CProto_IEC104::Pro_UP_151_v4(CString strMsg,unsigned char *buffer,int len)
{
	CString str,strErr;
	if(m_Dlg->getProVersion() <40)
	{
		str="请选择V4.0以上版本";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}

	if(buffer==0 || len==0)
	{
		str="无数据内容";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	
	int i=0;
	if(IsValide(strErr,buffer+i,len-i,8))
	{
	   strMsg = getEpCode(strMsg,buffer);
    }
    else
    {
		str.Format("(终端机器编码长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		 m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+8;

	if(IsValide(strErr,buffer+i,len-i,2))
	{

		str.Format("%02x %02x(站地址%d)\r\n",buffer[i],buffer[i+1],getInt2(buffer+i));
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(站地址错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+2;
	if(IsValide(strErr,buffer+i,len-i,2))
	{

		str.Format("%02x %02x(类型：",buffer[i],buffer[i+1]);
		int type = getInt2(buffer+i);
		switch(type)
		{
		case 1:
				str.Format("外部flash)\r\n");
			break;
        case 2:
				str.Format("内部flash)\r\n");
			break;
		 case 3:
				str.Format("ram)\r\n");
			break;
		 default:
             str.Format("未知)\r\n");
		}
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(类型错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+2;
	if(IsValide(strErr,buffer+i,len-i,2))
	{

		str.Format("%02x %02x(开始未知：%d",buffer[i],buffer[i+1],getInt2(buffer+i));
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(开始未知错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+2;
	if(IsValide(strErr,buffer+i,len-i,2))
	{

		str.Format("%02x %02x(长度：%d",buffer[i],buffer[i+1],getInt2(buffer+i));
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+2;
    int k=0;
	for(int j=i;j<len-5;j++)
	{
		str.Format("%02x ",buffer[i]);
		strMsg = strMsg+str;
		k++;
	}
    i=i+k;

	strMsg = cmdTimeMsg(strMsg,strErr,buffer+i,len-i);
	i=i+3;
    strMsg = crcMsg(strMsg,strErr,buffer+i,len-i);
	
	m_Dlg->ProDecodeShow(strMsg);
}
void	CProto_IEC104::Pro_Down_152_v4(CString strMsg,unsigned char *buffer,int len)
{
	CString str,strErr;
	if(m_Dlg->getProVersion() <40)
	{
		str="请选择V4.0以上版本";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}

	if(buffer==0 || len==0)
	{
		str="无数据内容";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	
	int i=0;
	if(IsValide(strErr,buffer+i,len-i,8))
	{
	   strMsg = getEpCode(strMsg,buffer);
    }
    else
    {
		str.Format("(终端机器编码长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		 m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+8;
	strMsg = cmdTimeMsg(strMsg,strErr,buffer+i,len-i);
	i=i+3;
    strMsg = crcMsg(strMsg,strErr,buffer+i,len-i);
	
	m_Dlg->ProDecodeShow(strMsg);
}
void	CProto_IEC104::Pro_UP_152_v4(CString strMsg,unsigned char *buffer,int len)
{
	CString str,strErr;
	if(m_Dlg->getProVersion() <40)
	{
		str="请选择V4.0以上版本";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}

	if(buffer==0 || len==0)
	{
		str="无数据内容";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	
	int i=0;
	if(IsValide(strErr,buffer+i,len-i,8))
	{
	   strMsg = getEpCode(strMsg,buffer);
    }
    else
    {
		str.Format("(终端机器编码长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		 m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+8;

	if(IsValide(strErr,buffer+i,len-i,1))
	{

		str.Format("%02x（临时充电次数：%d)\r\n",buffer[i],buffer[i]);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(临时充电次数错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	
    i=i+1;
	strMsg = cmdTimeMsg(strMsg,strErr,buffer+i,len-i);
	i=i+3;
    strMsg = crcMsg(strMsg,strErr,buffer+i,len-i);
	
	m_Dlg->ProDecodeShow(strMsg);
}

void	CProto_IEC104::Pro_Down_153_v4(CString strMsg,unsigned char *buffer,int len)
{
	CString str,strErr;
	if(m_Dlg->getProVersion() <40)
	{
		str="请选择V4.0以上版本";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}

	if(buffer==0 || len==0)
	{
		str="无数据内容";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	
	int i=0;
	if(IsValide(strErr,buffer+i,len-i,8))
	{
	   strMsg = getEpCode(strMsg,buffer);
    }
    else
    {
		str.Format("(终端机器编码长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		 m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+8;
	
	if(IsValide(strErr,buffer+i,len-i,1))
	{

		str.Format("%02x（临时充电次数：%d)\r\n",buffer[i],buffer[i]);
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(临时充电次数错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	
    i=i+1;
	strMsg = cmdTimeMsg(strMsg,strErr,buffer+i,len-i);
	i=i+3;
    strMsg = crcMsg(strMsg,strErr,buffer+i,len-i);
	
	m_Dlg->ProDecodeShow(strMsg);
}

void	CProto_IEC104::Pro_UP_153_v4(CString strMsg,unsigned char *buffer,int len)
{
	CString str,strErr;
	if(m_Dlg->getProVersion() <40)
	{
		str="请选择V4.0以上版本";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}

	if(buffer==0 || len==0)
	{
		str="无数据内容";
		strMsg = strMsg+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	
	int i=0;
	if(IsValide(strErr,buffer+i,len-i,8))
	{
	   strMsg = getEpCode(strMsg,buffer);
    }
    else
    {
		str.Format("(终端机器编码长度错误)\r\n");
		strMsg = strMsg+strErr+str;
		 m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	i=i+8;

	if(IsValide(strErr,buffer+i,len-i,1))
	{

		str.Format("%02x(成功标识：",buffer[i]);
		strMsg = strMsg+str;
		switch(buffer[i])
		{
		case 1:
				str.Format("成功)\r\n");
			break;
		 default:
             str.Format("失败)\r\n");
		}
		strMsg = strMsg+str;
	}
	else
	{
		str.Format("(成功标识错误)\r\n");
		strMsg = strMsg+strErr+str;
		m_Dlg->ProDecodeShow(strMsg);
		return;
	}
	
    i=i+1;
	strMsg = cmdTimeMsg(strMsg,strErr,buffer+i,len-i);
	i=i+3;
    strMsg = crcMsg(strMsg,strErr,buffer+i,len-i);
	
	m_Dlg->ProDecodeShow(strMsg);
}