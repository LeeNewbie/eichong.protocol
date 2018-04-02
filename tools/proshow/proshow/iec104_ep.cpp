#include "stdafx.h"
#include "iec104_ep.h"

#include <string.h>
#include <time.h>



#define LoByte(w)		((unsigned char)((unsigned short)(w) % 256))
#define HiByte(w)		((unsigned char)((unsigned short)(w) / 256))




//��ӡ������������
static void printBuffer(unsigned char * buf, int size)
{
	for (int i=0;i<size;i++) 
	{
	
	}
}

CProto_IEC104_C::CProto_IEC104_C()
{
	bInitAssociation = false;
	//m_localAddr = localAddr;
	//m_remoteAddr = remoteAddr;
	sendBuffer = new SEND_BUFFER();
	recvBuffer=new RECV_BUFFER();
    m_Rread=0;
	m_Tread=0;

	m_isOpen = false;
}

CProto_IEC104_C::~CProto_IEC104_C()
{
}

bool CProto_IEC104_C::isOpen() const
{
	return m_isOpen;
}

bool CProto_IEC104_C::open()
{
	memset(&config,0,sizeof(IEC104_CONFIG));
	memset(&info,0,sizeof(IEC104_INFO));

	//��ʼ���������ò���
	readFeature();
	//��ʼ��������Ϣ
	info.Next_I_Confirm = I_CON_IDLE; //�ȴ�ȷ�ϵ�����
	info.SendTimeFlag = true; 
	info.CallAllDataFlag = true;
	info.CallAllKwhFlag = true;

	m_isOpen = true;
	return	true;
}

void CProto_IEC104_C::close()
{
	m_isOpen = false;
	
}

/*
 * �������ܣ�����&���ݵ���������
 * �������أ�0: ʧ�ܡ�1: �ɹ�
 */
void CProto_IEC104_C::Rx()
{
	int		ret;
	short	len;
	NoRxAnyData();
	while( (len = length()) >= 6 )
	{
		getRx(buffer,2);


		if(buffer[0] != 0x68)
		{
			back(1);

			continue;
		}
		len -= 2;
		RxDataSize = buffer[1];
		if(len >= RxDataSize)
		{
		//	RxSomeData();

			// �жϱ�������,
			// ���� U ֡�� S ֡
			ret = SearchAPCI();

			if ( ret == IEC104_APCI_I )
			{
				// ���� I ֡
				SearchASDU();
			}
		}
		else
		{
			back(2);
	
			break;
		}
	}//end while

}

void CProto_IEC104_C::Tx()
{
	switch(info.LinkState)
	{
	case	STATE_UNBOUND: //δ����
		//��Ҫ�������󼤻���·
		if (bInitAssociation) //���������ʼ����·��־, false -- �����ȴ����Ӽ���, true -- �����������Ӽ���
		{
			//����������·�������
			if(info.T4++ < TIMEOUT_T4)	break;
			if(Tx_APDU_U(APCI_U_STARTDT_ACT) == true)
			{
				info.T4 = 0;
				info.WorkState = STATE_WAIT_STARTDT_CON; //�ȴ�������·ȷ��
			}
		}
		break;
	case	STATE_IDLE:
		//����ֹͣ��·�������
		if(info.WorkState!=STATE_WAIT_STOPDT_CON && info.Send_U_STOPDT_ACT_Flag==true)  //��ishi�ȴ�ֹͣ��·ȷ�ϲ�����Ҫ����ֹͣ��־
		{
			if(Tx_APDU_U(APCI_U_STOPDT_ACT) == true) //����ֹͣ��·
			{
				info.Send_U_STOPDT_ACT_Flag = false;
				info.WorkState = STATE_WAIT_STOPDT_CON; //�ȴ�ֹͣ��·ȷ��
			}
		}
		else if(info.WorkState == STATE_FREE) //����
		{
			//����������<ȷ��>
			if (info.Send_U_STARTDT_CON_Flag) //��Ҫ����������·
			{
				if(Tx_APDU_U(APCI_U_STARTDT_CON) == true)
				{
					info.Send_U_STARTDT_CON_Flag = false;
				}
				break;
			}
			//����ֹͣ����<ȷ��>
			else if (info.Send_U_STOPDT_CON_Flag) //��Ҫ����ֹͣ��·
			{
				InitInfo();
				if(Tx_APDU_U(APCI_U_STOPDT_CON) == true)
				{
					info.Send_U_STOPDT_CON_Flag = false;
				}
				break;
			}
			//�����������<ȷ��>
			else if (info.Send_U_TESTFR_CON_Flag) //��������ȷ��
			{
				if(Tx_APDU_U(APCI_U_TESTFR_CON) == true)
				{
					info.Send_U_TESTFR_CON_Flag = false;
				}
				break;
			}
			//����YK���
			else if(Tx_CtrlCmd() == true)
			{
				break;
			}
			//����������
			else if(Tx_AnalogOutPut() == true)	
			{
				break;
			}
			//����������·�������
			else if(info.Send_U_TESTFR_ACT_Flag == true)
			{
				if(Tx_APDU_U(APCI_U_TESTFR_ACT) == true)
				{
					info.Send_U_TESTFR_ACT_Flag = false;
					info.WorkState = STATE_WAIT_TESTDT_CON;
				}
			}
			//����S֡ȷ�ϡ�
			else if(info.Send_S_ACK_Flag == true)
			{
				Tx_APDU_S();
			}
			//�������ݴ��䡱
			else if(info.WorkState == STATE_FREE)	
			{
				Tx_DataCmd();
			}
		
		}
		break;
	default:
		break;
	}//end switch

}


void	CProto_IEC104_C::InitInfo()
{
	info.T1 = 0;
	info.T2 = 0;
	info.T3 = 0;
	info.LinkState = STATE_UNBOUND;//δ����
	info.WorkState = STATE_FREE;
	info.NS = 0;
	info.NR = 0;
	info.SendTimeFlag    = true;    
	info.CallAllDataFlag = true;
	info.CallAllKwhFlag =  true;
	info.Send_S_ACK_Flag = false;
	info.Send_U_TESTFR_ACT_Flag = false;
	info.Send_U_STOPDT_ACT_Flag = false;
}

void	CProto_IEC104_C::NoRxAnyData()
{
	info.T1++; //�ȴ����նԶ�Ӧ����Ϣ��ʱ�� ����ʱռ�ùرգ�
	info.T3++; //�ȴ�˫���������ݴ��䶨ʱ������ʱ�������Թ��̣�
}

void	CProto_IEC104_C::RxSomeData()
{
	info.T1 = 0; //�ȴ����նԶ�Ӧ����Ϣ��ʱ�� ����ʱռ�ùرգ�
	info.T3 = 0; //�ȴ�˫���������ݴ��䶨ʱ������ʱ�������Թ��̣�
}

//----------------------------------------------------------------------------
int	CProto_IEC104_C::Tx_APDU_I(unsigned short len)
{

	buffer[0] = 0x68;
	buffer[1] =LoByte(len + 4);
	buffer[2] =HiByte(len + 4)&0x7f;
	buffer[3] = (unsigned char)((info.NS % 128) << 1);
	buffer[4] = (unsigned char)(info.NS / 128);
	buffer[5] = (unsigned char)((info.NR % 128) << 1);
	buffer[6] = (unsigned char)(info.NR / 128);

	putTx(buffer,len+7);


	info.NS = (info.NS + 1) % (unsigned short)0x8000;
	info.Send_S_ACK_Flag = false;
	info.T1 = 0;
	info.T2 = 0;
	info.T3 = 0;

	return	true;
}

int	CProto_IEC104_C::Tx_APDU_S()
{
	buffer[0] = 0x68;
	buffer[1] = 4;
	buffer[2] = 0;
	buffer[3] = 1;
	buffer[4] = 0;
	buffer[5] = (unsigned char)((info.NR % 128) << 1);
	buffer[6] = (unsigned char)(info.NR / 128);


	putTx(buffer,7);


	info.Send_S_ACK_Flag = false;
	info.T1 = 0;
	info.T2 = 0;
	info.T3 = 0;

	return	true;
}

int	CProto_IEC104_C::Tx_APDU_U(unsigned char type)
{

	buffer[0] = 0x68;
	buffer[1] = 4;
	buffer[2] = 0;
	buffer[3] = type | 0x03;
	buffer[4] = 0;
	buffer[5] = 0;
	buffer[6] = 0;
	
	//д���ͽ��������

	putTx(buffer,7);

	info.T1 = 0;
	info.T3 = 0;

	return	true;
}

int		CProto_IEC104_C::Tx_CtrlCmd()
{
	return	false;
}

int		CProto_IEC104_C::Tx_AnalogOutPut()
{
	return	false;
}
//----------------------------------------------------------------------------
void	CProto_IEC104_C::Tx_DataCmd()
{
	if (!bInitAssociation) // ��վ
	{
		switch (info.Next_I_Confirm)  //��һ����Ҫȷ�ϵ�����
		{
		case I_CON_SYSNTIME: //����ʱ��ͬ��ȷ��
			if (Tx_SyncTimeCon() == true)
				info.Next_I_Confirm = I_CON_IDLE;
			return;
		case I_CON_CALL_ALL_DATA_ACT: //���ٻ�ȷ��
			if (Tx_CallAllDataCon() == true)
			{
				info.AckYxIndex = 0;
				info.AckYcIndex = 0;
				info.Next_I_Confirm = I_CON_CALL_ALL_YX;
			}
			return;
		case I_CON_CALL_ALL_YX: //����ȫң��
			{
				int ret = Tx_All_YX();
				switch(ret)
				{
				case RET_NO_DAT:
				case RET_TX_GRACE_OVER:// ���ݷ��͹��̽���
				case RET_STATE_BUSY: // ��ǰ״̬�ǿ���
					info.Next_I_Confirm = I_CON_CALL_ALL_YC;
					break;
				default:
					break;
				}
			}
			return;
		case I_CON_CALL_ALL_YC://����ȫң��
			{
				int ret = Tx_All_YC();
				switch(ret)
				{
				case RET_NO_DAT:
				case RET_TX_GRACE_OVER:
				case RET_STATE_BUSY:
					info.Next_I_Confirm = I_CON_ALL_DATA_TERM;
					break;
				default:
					break;
				}
			}
			return;
		case I_CON_ALL_DATA_TERM: //�������ٻ���ֹ
			Tx_CallAllDataTerm();
			info.Next_I_Confirm = I_CON_IDLE;
			return;
			// ���
		case I_CON_CALL_ALL_KWH_ACT: //
			if (Tx_CallAllKwhCon() == true)
			{
				info.AckKwhIndex = 0;
				info.Next_I_Confirm = I_CON_CALL_ALL_KWH;
			}
			return;
		case I_CON_CALL_ALL_KWH:
			{
				int ret = Tx_All_Kwh();
				switch (ret)
				{
				case RET_NO_DAT:
				case RET_TX_GRACE_OVER:
				case RET_STATE_BUSY:
					info.Next_I_Confirm = I_CON_ALL_KWH_TERM;
					break;
				default:
					break;
				}
			}
			return;
		case I_CON_ALL_KWH_TERM:
			Tx_CallAllKwhTerm();
			info.Next_I_Confirm = I_CON_IDLE;
			return;
		default:
			if (Tx_ChangeYx())//����ȫң��
				return;
			if (Tx_ChangeYc())//����ȫң��
				return;
			//���ͻ�������
			 if(Tx_Change())
				 return;
			 //���ͳ��׮����
			 Tx_Charge();
			return;
			
		}
	}
	
}
int	CProto_IEC104_C::Tx_Change()
{
	return true;
}

int	CProto_IEC104_C::Tx_Charge()
{
	return true;
}

int	CProto_IEC104_C::Tx_SyncTime()
{
//	unsigned char	i = 0;
//	unsigned char	*p = buffer + 6;
//	p[i++] = C_CS_NA_1;					//����
//	p[i++] = 1;							//�ṹ�޶���
//	i += MakeCause(p+i,CAUSE_ACT);		//����ԭ��
//	i += MakeCommAddr(p+i);				//������ַ��һ��Ϊ��վվַ��
//	i += MakeObjAddr(p+i,0);			//��Ϣ���ַ
//
//
//         struct timeval tvl;
//gettimeofday(&tvl, NULL);
//
//	
//	 struct tm *dateTime =localtime( &tvl.tv_sec ); 
//
//	int ms = (int)dateTime->tm_sec*1000 + (int)tvl.tv_usec/1000;
//	p[i++] = LoByte(ms);
//	p[i++] = HiByte(ms);
//	p[i++] = dateTime->tm_min & 0x3F;
//	p[i++] = dateTime->tm_hour & 0x3F;
//	p[i++] = (dateTime->tm_wday<< 5) | (dateTime->tm_mday & 0x1F);
//	p[i++] = (dateTime->tm_mon+1) & 0x0F;
//	p[i++] = ((dateTime->tm_year+1900) % 100) & 0x7F;
//
//	if ( Tx_APDU_I(i) == false )
//	{
//WriteLog("Tx-------> ʱ��ͬ�� ʧ�ܣ�Tx_APDU_I(Len=%d) Return false", i);
//		return false;
//	}
//WriteLog("Tx-------> ʱ��ͬ�� �ɹ���Tx_APDU_I(Len=%d) Return true", i);
//
//
//
	return true;
}

//----------------------------------------------------------------------------
int	CProto_IEC104_C::SearchAPCI()
{
	getRx(buffer,4);


	//I��ʽ
	if((buffer[0]&0x01) == IEC104_APCI_I)  //I ��ʽ��Ϣ����
	{
		if(RxDataSize <= 4)	return	-1;
		return	IEC104_APCI_I;
	}
	//S��ʽ
	else	if((buffer[0]&0x03) == IEC104_APCI_S) //S ��ʽ��Ϣ����
	{
		if(RxDataSize != 4)	return	-1;
		return	IEC104_APCI_S;
	}
	//U��ʽ
	else	if((buffer[0]&0x03) == IEC104_APCI_U) //U ��ʽ��Ϣ����
	{
		if(RxDataSize != 4)	{ return	-1;}
		switch(buffer[0] & 0xFC)
		{
		case	APCI_U_STARTDT_CON:

			if(info.LinkState==STATE_UNBOUND && info.WorkState==STATE_WAIT_STARTDT_CON)
			{
				info.LinkState = STATE_IDLE;
				info.WorkState = STATE_FREE;
			}
			break;
		case	APCI_U_STOPDT_CON:
	
			if(info.LinkState==STATE_IDLE && info.WorkState==STATE_WAIT_STOPDT_CON)
			{
				InitInfo();
			}
			break;
		case	APCI_U_TESTFR_CON:

			if(info.LinkState==STATE_IDLE && info.WorkState==STATE_WAIT_TESTDT_CON)
			{
				info.WorkState = STATE_FREE;
			}
			break;
		case	APCI_U_STARTDT_ACT:
	
			info.Send_U_STARTDT_CON_Flag = true;
			InitInfo();
			info.LinkState = STATE_IDLE;
			info.WorkState = STATE_FREE;
			break;
		case	APCI_U_STOPDT_ACT:
			info.Send_U_STOPDT_CON_Flag = true;
			info.NS = 0;
			if(info.LinkState==STATE_IDLE)//
			{
				InitInfo();
			}
			break;
		case	APCI_U_TESTFR_ACT:
	
			info.Send_U_TESTFR_CON_Flag = true;
			if(info.LinkState==STATE_IDLE)
			{
				info.WorkState = STATE_FREE;
			}
			break;
		default:
			return	-1;
		}
		return	IEC104_APCI_U;
	}
	return	-1;
}

int	CProto_IEC104_C::SearchASDU()
{
	if(info.LinkState == STATE_UNBOUND)	return	false;

	info.NR = (info.NR + 1) % (unsigned short)0x8000;
	AppDataLen = RxDataSize - 4;
	getRx(buffer,AppDataLen);

	pAppDataBuf = buffer;
	IEC104_APP_HEAD		AppHead;

	if ( GetAppHead(&AppHead ) == false)
	{
	

		return	false;
	}


	// ���Ʒ�����
	if( !bInitAssociation )
	{
		App_RxAct(AppHead);
	}
	return	true;
}

//----------------------------------------------------------------------------
void	CProto_IEC104_C::DoCallAllDataAck(IEC104_APP_HEAD &head)
{
	//�ṹ�޶���
	if(head.SQ!=0 || head.Num!=1)	return;
	//����ԭ��
	switch(head.Cause)
	{
	case	CAUSE_ACT_CON:
		if(info.WorkState == STATE_WAIT_ALLDATA_CON)	info.WorkState = STATE_WAIT_ALLDATA;
		break;
	case	CAUSE_STOPACT_CON:
		if(info.WorkState == STATE_WAIT_ALLDATA_CON)	info.WorkState = STATE_FREE;
		break;
	case	CAUSE_ACT_TERM:
		if(info.WorkState == STATE_WAIT_ALLDATA)	info.WorkState = STATE_FREE;
		break;
	default:
		break;
	}
}

void	CProto_IEC104_C::DoCallAllKwhAck(IEC104_APP_HEAD &head)
{
	//�ṹ�޶���
	if(head.SQ!=0 || head.Num!=1)	return;
	//����ԭ��
	switch(head.Cause)
	{
	case	CAUSE_ACT_CON:
		if(info.WorkState == STATE_WAIT_ALLKWH_CON)	info.WorkState = STATE_WAIT_ALLKWH;
		break;
	case	CAUSE_STOPACT_CON:
		if(info.WorkState == STATE_WAIT_ALLKWH_CON)	info.WorkState = STATE_FREE;
		break;
	case	CAUSE_ACT_TERM:
		if(info.WorkState == STATE_WAIT_ALLKWH)	info.WorkState = STATE_FREE;
		break;
	default:
		break;
	}
}

void	CProto_IEC104_C::DoSyncTimeAck(IEC104_APP_HEAD &head)
{
	//�ṹ�޶���
	if(head.SQ!=0 || head.Num!=1)	return;
	//����ԭ��
	if(head.Cause==CAUSE_ACT_CON && info.WorkState==STATE_WAIT_TIME_CON)	info.WorkState = STATE_FREE;
}




//----------------------------------------------------------------------------
int	CProto_IEC104_C::MakeCommAddr(unsigned char *buf)
{
	if(buf == NULL)	return	0;
	int	num = 0;
	if (!bInitAssociation)
	{
		buf[num++] = m_localAddr % 256;
	}
	else
	{
		buf[num++] = m_remoteAddr % 256;
	}
	//if(config.CommAddrLen == 2)	buf[num++] = pRoutePara->remote_addr / 256;

	if(config.CommAddrLen == 2)	buf[num++] = m_localAddr / 256;  
	return	num;
}


int	CProto_IEC104_C::MakeObjAddr(unsigned char *buf,int no)
{
	if(buf == NULL)	return	0;
	int	num = 0;
	buf[num++] = no % 256;
	if(config.ObjAddrLen >= 2)	buf[num++] = (no / 256) % 256;
	if(config.ObjAddrLen >= 3)	buf[num++] = (no / 256) / 256;
	return	num;
}

int	CProto_IEC104_C::MakeCause(unsigned char *buf,unsigned char cause)
{
	if(buf == NULL)	return	0;
	int	num = 0;
	buf[num++] = cause & 0x3F;	//T=0��PN=0
	if(config.CotLen == 2)	
	{
		buf[num++] = (unsigned char)m_localAddr;
	}
	return	num;
}



int	CProto_IEC104_C::GetAppHead(IEC104_APP_HEAD *head)
{
	if(pAppDataBuf==NULL || AppDataLen==0)	return	false;
	int headLen = 0;
	if(config.CotLen == 2)
	{
		memcpy(head,pAppDataBuf,4);
		pAppDataBuf += 4;
		AppDataLen -= 4;
		headLen+=4;
	}
	else
	{
		memcpy(head,pAppDataBuf,3);
		pAppDataBuf += 3;
		AppDataLen -= 3;
		headLen+=3;
	}
	unsigned short	CommAddr = 0;
	if(config.CommAddrLen == 2)
	{
		memcpy(&CommAddr,pAppDataBuf,2);
		pAppDataBuf += 2;
		AppDataLen -= 2;
		headLen+=2;
	}
	else
	{
		memcpy(&CommAddr,pAppDataBuf,1);
		pAppDataBuf += 1;
		AppDataLen -= 1;
		headLen+=1;
	}
	head->CommAddr = CommAddr;
	// �����ǿ��Ʒ���
	if (!IS_CONTROL_DIR(head->Cause))
	{
		if(head->CommAddr != (unsigned short)m_remoteAddr)	
		{
			return	false;
		}
	}
	return	true;
}

int	CProto_IEC104_C::GetObjAddr()
{
	unsigned int	addr;
	memset(&addr,0,sizeof(unsigned int));
	memcpy(&addr,pAppDataBuf,config.ObjAddrLen);
	pAppDataBuf += config.ObjAddrLen;
	return	(int)addr;
}

bool CProto_IEC104_C::readFeature()
{
	//Ĭ�Ϲ�Լ����
	//���ó�ʼֵ������������������ʼ
	//����
	if(config.CommAddrLen == 0)	config.CommAddrLen = 2;
	if(config.ObjAddrLen == 0)	config.ObjAddrLen = 3;
	if(config.CotLen == 0)		config.CotLen = 2;
	//��ʼ��ַ
	if(config.Yx_Start_Addr == 0)	config.Yx_Start_Addr = IEC104_OBJ_16_ADDR_YX_START;
	if(config.Yx_End_Addr == 0)	config.Yx_End_Addr = IEC104_OBJ_16_ADDR_YX_END;


	if(config.Yc_Start_Addr == 0)	config.Yc_Start_Addr = IEC104_OBJ_16_ADDR_YC_START;
	if(config.Yc_End_Addr == 0)	config.Yc_End_Addr = IEC104_OBJ_16_ADDR_YC_END;
	if(config.Para_Start_Addr == 0)	config.Para_Start_Addr = IEC104_OBJ_16_ADDR_PARA_START;
	if(config.Para_End_Addr == 0)	config.Para_End_Addr = IEC104_OBJ_16_ADDR_PARA_END;


	if(config.Set_Start_Addr == 0)	config.Set_Start_Addr = IEC104_OBJ_16_ADDR_SET_START;
	if(config.Set_End_Addr == 0)	config.Set_End_Addr = IEC104_OBJ_16_ADDR_SET_END;
	if(config.Kwh_Start_Addr == 0)	config.Kwh_Start_Addr = IEC104_OBJ_16_ADDR_KWH_START;
	if(config.Kwh_End_Addr == 0)	config.Kwh_End_Addr = IEC104_OBJ_16_ADDR_KWH_END;
	if(config.Step_Start_Addr == 0)	config.Step_Start_Addr = IEC104_OBJ_16_ADDR_STEP_START;
	if(config.Step_End_Addr == 0)	config.Step_End_Addr = IEC104_OBJ_16_ADDR_STEP_END;
	if(config.Bin_Start_Addr == 0)	config.Bin_Start_Addr = IEC104_OBJ_16_ADDR_BIN_START;
	if(config.Bin_End_Addr == 0)	config.Bin_End_Addr = IEC104_OBJ_16_ADDR_BIN_END;
	if(config.Rtu_State_Addr == 0)	config.Rtu_State_Addr = IEC104_OBJ_16_ADDR_RTU_STATE;

	//���ó�ʼֵ��������������������
	//���������Ϣ������������������ʼ
	config.YcMaxNum = config.Yc_End_Addr - config.Yc_Start_Addr + 1;
	config.YxMaxNum = config.Yx_End_Addr - config.Yx_Start_Addr + 1;
	config.KwhMaxNum = config.Kwh_End_Addr - config.Kwh_Start_Addr + 1;

	//���������Ϣ��������������������

	//����
	config.CommAddrLen = 2; 
	config.ObjAddrLen = 3;
	config.CotLen = 2;

	return	true;
}

/*-----------------------------------------------------------------------*\
���տ��Ʒ�����
\*-----------------------------------------------------------------------*/
void CProto_IEC104_C::App_RxAct(IEC104_APP_HEAD &AppHead)
{
	//logprint(5008,"App_RxAct-1-----------AppHead.Type=%d",AppHead.Type);
	switch(AppHead.Type)
	{
	case C_CS_NA_1:		//ʱ��ͬ��


		info.Next_I_Confirm = I_CON_SYSNTIME;
		break;
	case C_IC_NA_1:
		{
			unsigned char detail = 0;
			if (AppDataLen > 0) detail = pAppDataBuf[AppDataLen-1];
			else detail = pAppDataBuf[0];
			pAppDataBuf ++;
			if ( detail == 0x14 ) // ���ٻ��޶���QOI
			{
	
				info.Next_I_Confirm = I_CON_CALL_ALL_DATA_ACT;

			}
			else 
			{

			}
		}
		break;
	case C_CI_NA_1:
		{
			unsigned char detail = 0;
			if (AppDataLen > 0) detail = pAppDataBuf[AppDataLen-1];
			else detail = pAppDataBuf[0];
			pAppDataBuf ++;
			if ( detail == 0x45 ) // ���ٻ����
			{
				info.Next_I_Confirm = I_CON_CALL_ALL_KWH_ACT; // 

			}
			else 
			{


			}
		}
		break;
	}
}

/*-----------------------------------------------------------------------*\
���ͼ��ӷ�����  ͬ��ʱ��<ȷ��>
\*-----------------------------------------------------------------------*/
int CProto_IEC104_C::Tx_SyncTimeCon()
{
	if(info.WorkState != STATE_FREE)
	{
		return	false;
	}
	unsigned char	i = 0;
	unsigned char	*p = buffer + 7;
	p[i++] = C_CS_NA_1;					//����
	p[i++] = 1;							//�ṹ�޶���
	i += MakeCause(p+i,CAUSE_ACT_CON);	//����ԭ��
	i += MakeCommAddr(p+i);				//������ַ��һ��Ϊ��վվַ��
	i += MakeObjAddr(p+i,0);			//��Ϣ���ַ

	if(Tx_APDU_I(i) == false)	
	{
		return	false;
	}




	//���ı乤��״̬
	return	true;
}


/*-----------------------------------------------------------------------
���ͼ��ӷ�����  ���ٻ�<ȷ��>
*/
int CProto_IEC104_C::Tx_CallAllDataCon()
{
	if(info.WorkState != STATE_FREE)
	{

		return	false;
	}
	unsigned char	i = 0;
	unsigned char	*p = buffer + 7;
	p[i++] = C_IC_NA_1;					//����
	p[i++] = 1;							//�ṹ�޶���
	i += MakeCause(p+i,CAUSE_ACT_CON);	//����ԭ��
	i += MakeCommAddr(p+i);				//������ַ��һ��Ϊ��վվַ��
	i += MakeObjAddr(p+i,0);			//��Ϣ���ַ

	if(Tx_APDU_I(i) == false)	
	{

		return	false;
	}




	return	true;
}


/*-----------------------------------------------------------------------
���ͼ��ӷ�����  ���ٻ�����
*/
int CProto_IEC104_C::Tx_All_YX()
{
	if(info.WorkState != STATE_FREE)
	{

		return	RET_STATE_BUSY;
	}

	unsigned int zfYxNum = config.YxMaxNum;
	// �������ü������ڴ��Ƿ���� 
	if ( zfYxNum <= 0 )
		return	RET_NO_DAT;

	// ȷ��������Ϣ�����,����-�ϴη��͵���Ŀ,
	int ackindex = info.AckYxIndex;
	int txnum = zfYxNum - ackindex;	

	// ����һ֡�ܷ��͵������Ŀ,ȡ�����Ŀ.
	if ( txnum > MAX_YX_NUM_1_GRAM ) 
		txnum = MAX_YX_NUM_1_GRAM;
	else if (txnum <= 0)
		return RET_NO_DAT;

	// ������ָ������
	short i = 0;
	unsigned char * p = buffer + 7;

	// ����ʱ��ĵ�����Ϣ
	p[i++] = M_SP_NA_1; 

	// �ṹ�޶���VSQ
	// bit 1234567(ң�Ÿ���)  8(˳���ͱ�־)
	p[i++] = txnum|0x80;

	i += MakeCause (p+i,CAUSE_RESP_CALLALL);	//����ԭ��
	i += MakeCommAddr (p+i);					//������ַ��һ��Ϊ��վվַ��
	i += MakeObjAddr (p+i,ackindex+config.Yx_Start_Addr);	//��Ϣ���ַ

	// ��Ϣ��
//	DAC_YX_DATA data;
//	DAC_YX* pPara = NULL;
	int n = 0;
	for(n=0;n<txnum;n++,i++)
	{
		unsigned char tmpq = 0;

		//pPara = m_dataInf.yxPara(m_pRoute->group,ackindex+n);
		//if ( pPara == NULL || !pPara->valid || !m_dataInf.yx(m_route,ackindex+n,data) ) 
		//{
		//	tmpq |= 0x80;	//δ������
		//}
		//else 
		//	tmpq = data.val;


		//if ( BFS_BIT_ENABLED((unsigned int)data.quality,QUALITY_BIT_IV) )
		//	 tmpq |= 0x40;   //��Ч

		////�������ݲ�����
		////if (pDi->data_active == false)  tmpq |= 0x80;   //δ������
		////if (pDi->state & 0x01)          tmpq |= 0x40;   //��Ч
		////if (pDi->data_manset)           tmpq |= 0x20;   //�˹�

		p[i] = tmpq;
	}

	if(Tx_APDU_I(i) == false)	
	{

		return	RET_TX_DAT_FAIL;
	}


	info.AckYxIndex += txnum;

	//д���ͽ��������


	if (txnum < MAX_YX_NUM_1_GRAM)
	{
		return RET_TX_GRACE_OVER;
	}

	return RET_TX_DAT_SUCCESS;
}

/*-----------------------------------------------------------------------
���ͼ��ӷ�����  ���ٻ�����
*/
int CProto_IEC104_C::Tx_All_YC()
{
	if(info.WorkState != STATE_FREE)
	{
		return	RET_STATE_BUSY;
	}

	unsigned int zfYcNum = config.YcMaxNum;
	// �������ü������ڴ��Ƿ���� 
	if( zfYcNum <= 0 )
		return	RET_NO_DAT;

	// ȷ��������Ϣ�����,������ȥ�ϴη��͵���Ŀ,
	int ackindex = info.AckYcIndex;
	int txnum = zfYcNum - ackindex;	

	// ����һ֡�ܷ��͵������Ŀ,ȡ�����Ŀ.
	if ( txnum > MAX_YC_NUM_1_GRAM ) 
		txnum = MAX_YC_NUM_1_GRAM;
	else if (txnum <= 0)
		return RET_NO_DAT;

	// ������ָ������
	 short i = 0;
	unsigned char * p = buffer + 7;

	// ����ʱ��ı��ֵ
	//p[i++] = M_ME_NB_1; 
	// ����ʱ��Ķ̸�����
	p[i++] = M_ME_NC_1;

	// �ṹ�޶���VSQ
	// bit 1234567(ң�����)  8(˳���ͱ�־)
	p[i++] = txnum|0x80;

	i += MakeCause (p+i,CAUSE_RESP_CALLALL);	//����ԭ��
	i += MakeCommAddr (p+i);			//������ַ��һ��Ϊ��վվַ��
	unsigned short objaddr = ackindex + config.Yc_Start_Addr;
	i += MakeObjAddr (p+i,objaddr);	//��Ϣ���ַ

	// ��Ϣ��
	/*DAC_YC_DATA data;
	DAC_YC* pPara = NULL;*/
	int n = 0;
	for(n=0;n<txnum;n++)
	{
		unsigned char tmpq = 0;
		float fval = 0.0;

		//pPara = m_dataInf.ycPara(m_pRoute->group,ackindex+n);
		//if ( pPara == NULL || !pPara->valid || !m_dataInf.yc(m_route,ackindex+n,data) ) 
		//{
		//	tmpq |= 0x80;	//δ������
		//}
		//else
		//	fval = data.val;

		//memcpy(p+i, &fval,4);
		//i+=4;

		//if ( BFS_BIT_ENABLED((unsigned int)data.quality,QUALITY_BIT_IV) )
		//	tmpq |= 0x40;   //��Ч

		////�������ݲ�����
		////if (pAi->data_active == false) tmpq |= 0x80;       //δ������
		////if (pAi->state & 0x01)         tmpq |= 0x40;       //��Ч--FROM SCADA
		////if (pAi->data_manset)          tmpq |= 0x20;       //�˹�
		////if (pAi->data_over)			 tmpq |= 0x01;	   //wfp add at 20080403 for henan

		p[i++] = tmpq;
	}

	if(Tx_APDU_I(i) == false)	
	{
	
		return	RET_TX_DAT_FAIL;
	}


	info.AckYcIndex += txnum;

	//д���ͽ��������


	if (txnum < MAX_YC_NUM_1_GRAM)
	{
		return RET_TX_GRACE_OVER;
	}

	return RET_TX_DAT_SUCCESS;
}


/*-----------------------------------------------------------------------
���ͼ��ӷ�����  ���ٻ�����132
*/
int CProto_IEC104_C::Tx_All_YC_132()
{
	if(info.WorkState != STATE_FREE)
	{
		return	RET_STATE_BUSY;
	}

	unsigned int zfYcNum = config.Yc132MaxNum;
	// �������ü������ڴ��Ƿ���� 
	if( zfYcNum <= 0 )
		return	RET_NO_DAT;

	// ȷ��������Ϣ�����,������ȥ�ϴη��͵���Ŀ,
	int ackindex = info.AckYcIndex;
	int txnum = zfYcNum - ackindex;	

	// ����һ֡�ܷ��͵������Ŀ,ȡ�����Ŀ.
	if ( txnum > MAX_YC_NUM_1_GRAM ) 
		txnum = MAX_YC_NUM_1_GRAM;
	else if (txnum <= 0)
		return RET_NO_DAT;

	// ������ָ������
	short i = 0;
	unsigned char * p = buffer + 7;

	// ����ʱ��ı��ֵ
	//p[i++] = M_ME_NB_1; 
	// ����ʱ��Ķ̸�����
	p[i++] = M_ME_NC_1;

	// �ṹ�޶���VSQ
	// bit 1234567(ң�����)  8(˳���ͱ�־)
	p[i++] = txnum|0x80;

	i += MakeCause (p+i,CAUSE_RESP_CALLALL);	//����ԭ��
	i += MakeCommAddr (p+i);			//������ַ��һ��Ϊ��վվַ��
	unsigned short objaddr = ackindex + config.Yc_Start_Addr;
	i += MakeObjAddr (p+i,objaddr);	//��Ϣ���ַ

	// ��Ϣ��
	/*DAC_YC_DATA data;
	DAC_YC* pPara = NULL;*/
	int n = 0;
	for(n=0;n<txnum;n++)
	{
		unsigned char tmpq = 0;
		unsigned int fval = 0.0;

		//pPara = m_dataInf.ycPara(m_pRoute->group,ackindex+n);
		//if ( pPara == NULL || !pPara->valid || !m_dataInf.yc(m_route,ackindex+n,data) ) 
		//{
		//	tmpq |= 0x80;	//δ������
		//}
		//else
		//	fval = data.val;

		//memcpy(p+i, &fval,4);
		//i+=4;

		//if ( BFS_BIT_ENABLED((unsigned int)data.quality,QUALITY_BIT_IV) )
		//	tmpq |= 0x40;   //��Ч

		////�������ݲ�����
		////if (pAi->data_active == false) tmpq |= 0x80;       //δ������
		////if (pAi->state & 0x01)         tmpq |= 0x40;       //��Ч--FROM SCADA
		////if (pAi->data_manset)          tmpq |= 0x20;       //�˹�
		////if (pAi->data_over)			 tmpq |= 0x01;	   

		p[i++] = tmpq;
	}

	if(Tx_APDU_I(i) == false)	
	{

		return	RET_TX_DAT_FAIL;
	}



	info.AckYcIndex += txnum;

	//д���ͽ��������


	if (txnum < MAX_YC_NUM_1_GRAM)
	{
		return RET_TX_GRACE_OVER;
	}

	return RET_TX_DAT_SUCCESS;
}

/*-----------------------------------------------------------------------
���ͼ��ӷ�����  ���ٻ�������ֹ
*/
int CProto_IEC104_C::Tx_CallAllDataTerm()
{
	if(info.WorkState != STATE_FREE)
	{
		return	false;
	}
	unsigned char	i = 0;
	unsigned char	*p = buffer + 6;
	p[i++] = C_IC_NA_1;					//����
	p[i++] = 1;							//�ṹ�޶���
	i += MakeCause(p+i,CAUSE_ACT_TERM);	//����ԭ��
	i += MakeCommAddr(p+i);				//������ַ��һ��Ϊ��վվַ��
	i += MakeObjAddr(p+i,0);			//��Ϣ���ַ

	if(Tx_APDU_I(i) == false)	
	{
		return	false;
	}


	//д���ͽ��������


	//���ı乤��״̬
	return	true;
}

/*-----------------------------------------------------------------------
�仯ң��
*/
int	CProto_IEC104_C::Tx_ChangeYx()
{
	/*if ( !m_dataInf.hasChgYx(m_route) )
		return false;*/

	// ������ָ������
	short i = 0,txnum=0;
	unsigned char * p = buffer + 7;

	// ����ʱ��ĵ�����Ϣ
	p[i++] = M_SP_NA_1; 

	i++; // �ṹ�޶���VSQ�����渳ֵ 

	i += MakeCause (p+i,CAUSE_SPONT);	//����ԭ��
	i += MakeCommAddr (p+i);			//������ַ��һ��Ϊ��վվַ��

	//DAC_YX_CHG_DATA chgData; 
	for (;;)
	{
	//	if ( !m_dataInf.chgYx(m_route,chgData) ) 
	//		break;

//		i += MakeObjAddr (p+i,chgData.no+config.Yx_Start_Addr);	//��Ϣ���ַ
		//unsigned char tmpq = chgData.data.val;
        unsigned char tmpq;
		//�������ݲ�����
		
		p[i++]=tmpq;

		txnum++;
		if ( txnum >= MAX_CHG_YX_NUM_1_GRAM ) 
			break;
	}

	if (txnum > 0)
	{
		// �ṹ�޶���VSQ
		// bit 1234567(ң�Ÿ���)  8(˳���ͱ�־)
		p[1] = txnum;
		if(Tx_APDU_I(i) == false)	
		{
			return	false;
		}


		//д���ͽ��������


		return true;
	}
	return	false;
}


/*-----------------------------------------------------------------------
���ͼ��ӷ�����  �仯ң��
*/
int CProto_IEC104_C::Tx_ChangeYc()
{
	//if ( !m_dataInf.hasChgYc(m_route) )
	//	return false;

	// ������ָ������
	 short i = 0,txnum=0;
	unsigned char * p = buffer + 7;

	// ����ʱ��ı��ֵ
	//p[i++] = M_ME_NB_1; 
	// ����ʱ��Ķ̸�����
	p[i++] = M_ME_NC_1;
	i++; // �ṹ�޶���VSQ�����渳ֵ 

	i += MakeCause (p+i,CAUSE_SPONT);	//����ԭ��
	i += MakeCommAddr (p+i);			//������ַ��һ��Ϊ��վվַ��

	//DAC_YC_CHG_DATA chgData;
	for (;;)
	{
		//if ( !m_dataInf.chgYc(m_route,chgData) ) 
		//	break;

		//i += MakeObjAddr (p+i,chgData.no+config.Yc_Start_Addr);	//��Ϣ���ַ

		//float fval = chgData.data.val;

		float fval;
		memcpy(p+i, &fval,4);
		i+=4;

		unsigned char tmpq = 0;
		//�������ݲ�����
		p[i++] = tmpq;

		txnum++;
		if ( txnum >= MAX_CHG_YC_NUM_1_GRAM ) 
			break;
	}

	if (txnum > 0)
	{
		// �ṹ�޶���VSQ
		// bit 1234567(ң�Ÿ���)  8(˳���ͱ�־)
		p[1] = txnum;
		if(Tx_APDU_I(i) == false)	
		{
			return	false;
		}


	
		return true;
	}
	return	false;
}


/*-----------------------------------------------------------------------
���ͼ��ӷ�����  �仯ң��
*/
int CProto_IEC104_C::Tx_ChangeYc_132()
{
	//if ( !m_dataInf.hasChgYc(m_route) )
	//	return false;

	// ������ָ������
	short i = 0,txnum=0;
	unsigned char * p = buffer + 7;


	// ����ʱ��Ķ̸�����
	p[i++] = M_MD_NA_1;
	i++; // �ṹ�޶���VSQ�����渳ֵ 

	i += MakeCause (p+i,CAUSE_SPONT);	//����ԭ��
	i += MakeCommAddr (p+i);			//������ַ��һ��Ϊ��վվַ��

	//YC_2_CHG_DATA chgData;
	for (;;)
	{
		//if ( chgYc_2(chgData) ) 
		//	break;

		//i += MakeObjAddr (p+i,chgData.no+config.Yc2_Start_Addr);	//��Ϣ���ַ

		//float fval = chgData.data.val;
		float fval;
		memcpy(p+i, &fval,4);
		i+=4;

		unsigned char tmpq = 0;
		//�������ݲ�����
		p[i++] = tmpq;

		txnum++;
		if ( txnum >= MAX_CHG_YC_NUM_1_GRAM ) 
			break;
	}

	if (txnum > 0)
	{
		// �ṹ�޶���VSQ
		// bit 1234567(ң�Ÿ���)  8(˳���ͱ�־)
		p[1] = txnum;
		if(Tx_APDU_I(i) == false)	
		{
	
			return	false;
		}




	
		return true;
	}
	return	false;
}


/*-----------------------------------------------------------------------
���ͼ��ӷ�����  ��Ӧ���ٻ����ȷ��
*/
int CProto_IEC104_C::Tx_CallAllKwhCon()
{
	if(info.WorkState != STATE_FREE)
	{

		return	false;
	}
	unsigned char	i = 0;
	unsigned char	*p = buffer + 6;
	p[i++] = C_CI_NA_1;					//����
	p[i++] = 1;							//�ṹ�޶���
	i += MakeCause(p+i,CAUSE_ACT_CON);	//����ԭ��
	i += MakeCommAddr(p+i);				//������ַ��һ��Ϊ��վվַ��
	i += MakeObjAddr(p+i,0);			//��Ϣ���ַ

	if(Tx_APDU_I(i) == false)	
	{

		return false;
	}



	return true;
}
/*-----------------------------------------------------------------------
���ͼ��ӷ�����  ��Ӧ���ٻ��������
*/
int CProto_IEC104_C::Tx_All_Kwh()
{
	if(info.WorkState != STATE_FREE)
	{

		return	RET_STATE_BUSY;
	}

	int zfKwhNum = config.KwhMaxNum;

	// �������ü������ڴ��Ƿ���� 
	if(zfKwhNum <= 0)
		return	RET_NO_DAT;

	// ȷ��������Ϣ�����,������ȥ�ϴη��͵���Ŀ,
	int ackindex = info.AckKwhIndex;
	int txnum = zfKwhNum - ackindex;	

	// ����һ֡�ܷ��͵������Ŀ,ȡ�����Ŀ.
	if ( txnum > MAX_KWH_NUM_1_GRAM ) 
		txnum = MAX_KWH_NUM_1_GRAM;
	else if (txnum <= 0)
		return RET_NO_DAT;

	// ������ָ������
     short i = 0;
	unsigned char * p = buffer + 7;

	// ����ʱ����ۻ���
	p[i++] = M_IT_NA_1; 

	// �ṹ�޶���VSQ
	// bit 1234567(ң�����)  8(˳���ͱ�־)
	p[i++] = txnum|0x80;

	i += MakeCause (p+i,CAUSE_RESP_CALLALL);	//����ԭ��
	i += MakeCommAddr (p+i);					//������ַ��һ��Ϊ��վվַ��
	unsigned short objaddr = ackindex + config.Kwh_Start_Addr;
	i += MakeObjAddr (p+i,objaddr);	//��Ϣ���ַ

	// ��Ϣ��
//	DAC_KWH_DATA data;
	int n = 0;
	for(n=0;n<txnum;n++)
	{
		double fval = 0.0;
		unsigned char tmpq = 0;

		//if ( !m_dataInf.kwh(m_route,ackindex+n,data) )
		//{
		//	tmpq |= 0x80;	//δ������
		//}
		//else 
		//	fval = data.val;

		long scalar_val = 0; //64
		if (fval >=0 ) scalar_val = (long)(fval+0.5);
		else scalar_val = (long)(fval-0.5);

		p[i++]=scalar_val%256;
		p[i++]=(scalar_val/256)%256;
		p[i++]=((scalar_val/256)/256)%256;
		p[i++]=((scalar_val/256)/256)/256;
		p[i++] = tmpq;
	}

	if(Tx_APDU_I(i) == false)	
	{

		return	RET_TX_DAT_FAIL;
	}



	info.AckKwhIndex += txnum;



	if (txnum < MAX_KWH_NUM_1_GRAM)
	{
		return RET_TX_GRACE_OVER;
	}

	return RET_TX_DAT_SUCCESS;

	return 0;
}

/*-----------------------------------------------------------------------
���ͼ��ӷ�����  ��Ӧ���ٻ���Ƚ���
*/
int CProto_IEC104_C::Tx_CallAllKwhTerm()
{
	if(info.WorkState != STATE_FREE)
	{

		return	false;
	}
	unsigned char	i = 0;
	unsigned char	*p = buffer + 6;
	p[i++] = C_CI_NA_1;					//����
	p[i++] = 1;							//�ṹ�޶���
	i += MakeCause(p+i,CAUSE_ACT_TERM);	//����ԭ��
	i += MakeCommAddr(p+i);				//������ַ��һ��Ϊ��վվַ��
	i += MakeObjAddr(p+i,0);			//��Ϣ���ַ

	if(Tx_APDU_I(i) == false)	
	{

		return	false;
	}


	//���ı乤��״̬
	return	true;
}




unsigned int	CProto_IEC104_C::getRx(unsigned char *buf,unsigned int len)
{
	unsigned int bufSize = 0;
	unsigned int write = 0;
	unsigned char* pData = NULL;
		
		bufSize = RECV_BUFSIZE;
		write = recvBuffer->write;
		pData = recvBuffer->data;
	

	//���У��
	if ( buf == NULL || len < 0 || len > bufSize ) return -1;
	if ( len == 0 ) return 0;

	unsigned int realLen = 0;
	if ( length() <= len ) //���ݲ���
		realLen = length();
	else
		realLen = len;

	if ( m_Rread > write )
	{
		if ( (m_Rread + realLen) >= bufSize )
		{
			unsigned int hasLen,remainLen;
			hasLen = bufSize - m_Rread;
			memcpy( buf,pData+m_Rread,hasLen);
			remainLen = realLen - hasLen;
			memcpy( buf + hasLen,pData,remainLen );

			m_Rread = (m_Rread + hasLen + remainLen) % bufSize;

			return hasLen + remainLen;
		}
		else
		{
			memcpy(buf,pData+m_Rread,realLen);
			m_Rread =( m_Rread + realLen ) % bufSize;

			return realLen;
		}
	}
	else
	{
		memcpy(buf,pData+m_Rread,realLen );
		m_Rread = (m_Rread + realLen ) % bufSize;

		return realLen;
	}
}

unsigned int	CProto_IEC104_C::getTx(unsigned char *buf,unsigned int len)
{
       unsigned int bufSize = 0;
	unsigned int write = 0;
	unsigned char* pData = NULL;
		
	bufSize = SEND_BUFSIZE;
		write = sendBuffer->write;
		pData = sendBuffer->data;
	

	//���У��
	if ( buf == NULL || len < 0 || len > bufSize ) return -1;
	if ( len == 0 ) return 0;

	unsigned int realLen = 0;
	if ( length() <= len ) //���ݲ���
		realLen = length();
	else
		realLen = len;

	if ( m_Tread > write )
	{
		if ( (m_Tread + realLen) >= bufSize )
		{
			unsigned int hasLen,remainLen;
			hasLen = bufSize - m_Tread;
			memcpy( buf,pData+m_Tread,hasLen);
			remainLen = realLen - hasLen;
			memcpy( buf + hasLen,pData,remainLen );

			m_Tread = (m_Tread + hasLen + remainLen) % bufSize;

			return hasLen + remainLen;
		}
		else
		{
			memcpy(buf,pData+m_Tread,realLen);
			m_Tread =( m_Tread + realLen ) % bufSize;

			return realLen;
		}
	}
	else
	{
		memcpy(buf,pData+m_Tread,realLen );
		m_Tread = (m_Tread + realLen ) % bufSize;

		return realLen;
	}
}

bool CProto_IEC104_C::back(unsigned int num)
{
	if ( num <= 0 ) return false;

	m_Rread = ( m_Rread + RECV_BUFSIZE - num ) % RECV_BUFSIZE;
	
	return true;
}



bool CProto_IEC104_C::putTx(const unsigned char *buf,unsigned int len)
{

	if ( buf == NULL || len <= 0 ) return false;

	unsigned int bufSize = 0;
	unsigned int write = 0;
	unsigned char* pData = NULL;
	
		bufSize = SEND_BUFSIZE;
		write = sendBuffer->write;
		pData = sendBuffer->data;
	

	//��Ҫ����һ��
	if ( write + len > bufSize )
	{
		unsigned int hasLen,remainLen;
		hasLen = bufSize - write;
		memcpy( pData+write,buf,hasLen );
		

		sendBuffer->write = (write + hasLen) % bufSize;
			write = sendBuffer->write;

		//ʣ��û�������ֽ���
		remainLen = (len - hasLen >= bufSize - write ? bufSize - write : len - hasLen);
		memcpy( pData+write,buf+hasLen,remainLen );

		sendBuffer->write = (write + remainLen) % bufSize;
	
	}
	else
	{
		memcpy( pData+write,buf,len );
	
		sendBuffer->write = (write + len) % bufSize;
		
	}
	return true;
}

bool CProto_IEC104_C::putRx(const unsigned char *buf,unsigned int len )
{
	if ( buf == NULL || len <= 0 ) return false;

	unsigned int bufSize = 0;
	unsigned int write = 0;
	unsigned char* pData = NULL;
		
	bufSize = RECV_BUFSIZE;
	write = recvBuffer->write;
	pData = recvBuffer->data;
	

	//��Ҫ����һ��
	if ( write + len > bufSize )
	{
		unsigned int hasLen,remainLen;
		hasLen = bufSize - write;
		memcpy( pData+write,buf,hasLen );
		
		
		recvBuffer->write = (write + hasLen) % bufSize;
		write = recvBuffer->write;
		

		//ʣ��û�������ֽ���
		remainLen = (len - hasLen >= bufSize - write ? bufSize - write : len - hasLen);
		memcpy( pData+write,buf+hasLen,remainLen );

		recvBuffer->write = (write + remainLen) % bufSize;
		
	}
	else
	{
		memcpy( pData+write,buf,len );
		
		recvBuffer->write = (write + len) % bufSize;
	
	}
	return true;
}


unsigned int	CProto_IEC104_C::length()	const
{

	unsigned int bufSize = 0;
	unsigned int write = 0;
		
	bufSize = RECV_BUFSIZE;
	write = recvBuffer->write;
	

	if( write >= m_Rread )
		return write - m_Rread;
	else
		return write + bufSize - m_Rread;
}
