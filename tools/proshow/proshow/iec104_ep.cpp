#include "stdafx.h"
#include "iec104_ep.h"

#include <string.h>
#include <time.h>



#define LoByte(w)		((unsigned char)((unsigned short)(w) % 256))
#define HiByte(w)		((unsigned char)((unsigned short)(w) / 256))




//打印缓冲区的内容
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

	//初始化基本配置参数
	readFeature();
	//初始化基本信息
	info.Next_I_Confirm = I_CON_IDLE; //等待确认的命令
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
 * 函数功能：命令&数据的主处理函数
 * 函数返回：0: 失败　1: 成功
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

			// 判断报文类型,
			// 处理 U 帧和 S 帧
			ret = SearchAPCI();

			if ( ret == IEC104_APCI_I )
			{
				// 处理 I 帧
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
	case	STATE_UNBOUND: //未连接
		//需要主动请求激活链路
		if (bInitAssociation) //主动发起初始化链路标志, false -- 被动等待链接激活, true -- 主动发起链接激活
		{
			//处理“启动链路——激活”
			if(info.T4++ < TIMEOUT_T4)	break;
			if(Tx_APDU_U(APCI_U_STARTDT_ACT) == true)
			{
				info.T4 = 0;
				info.WorkState = STATE_WAIT_STARTDT_CON; //等待启动链路确认
			}
		}
		break;
	case	STATE_IDLE:
		//处理“停止链路——激活”
		if(info.WorkState!=STATE_WAIT_STOPDT_CON && info.Send_U_STOPDT_ACT_Flag==true)  //不ishi等待停止链路确认并且需要发送停止标志
		{
			if(Tx_APDU_U(APCI_U_STOPDT_ACT) == true) //发送停止链路
			{
				info.Send_U_STOPDT_ACT_Flag = false;
				info.WorkState = STATE_WAIT_STOPDT_CON; //等待停止链路确认
			}
		}
		else if(info.WorkState == STATE_FREE) //空闲
		{
			//处理激活链接<确认>
			if (info.Send_U_STARTDT_CON_Flag) //需要发送启动链路
			{
				if(Tx_APDU_U(APCI_U_STARTDT_CON) == true)
				{
					info.Send_U_STARTDT_CON_Flag = false;
				}
				break;
			}
			//处理停止链接<确认>
			else if (info.Send_U_STOPDT_CON_Flag) //需要发送停止链路
			{
				InitInfo();
				if(Tx_APDU_U(APCI_U_STOPDT_CON) == true)
				{
					info.Send_U_STOPDT_CON_Flag = false;
				}
				break;
			}
			//处理测试链接<确认>
			else if (info.Send_U_TESTFR_CON_Flag) //测试命令确认
			{
				if(Tx_APDU_U(APCI_U_TESTFR_CON) == true)
				{
					info.Send_U_TESTFR_CON_Flag = false;
				}
				break;
			}
			//处理“YK命令”
			else if(Tx_CtrlCmd() == true)
			{
				break;
			}
			//处理“设点命令”
			else if(Tx_AnalogOutPut() == true)	
			{
				break;
			}
			//处理“测试链路——激活”
			else if(info.Send_U_TESTFR_ACT_Flag == true)
			{
				if(Tx_APDU_U(APCI_U_TESTFR_ACT) == true)
				{
					info.Send_U_TESTFR_ACT_Flag = false;
					info.WorkState = STATE_WAIT_TESTDT_CON;
				}
			}
			//处理“S帧确认”
			else if(info.Send_S_ACK_Flag == true)
			{
				Tx_APDU_S();
			}
			//处理“数据传输”
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
	info.LinkState = STATE_UNBOUND;//未连接
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
	info.T1++; //等待接收对端应答信息定时器 （超时占用关闭）
	info.T3++; //等待双方进行数据传输定时器（超时启动测试过程）
}

void	CProto_IEC104_C::RxSomeData()
{
	info.T1 = 0; //等待接收对端应答信息定时器 （超时占用关闭）
	info.T3 = 0; //等待双方进行数据传输定时器（超时启动测试过程）
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
	
	//写发送接收命令缓存

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
	if (!bInitAssociation) // 子站
	{
		switch (info.Next_I_Confirm)  //下一个需要确认的命令
		{
		case I_CON_SYSNTIME: //发送时间同步确认
			if (Tx_SyncTimeCon() == true)
				info.Next_I_Confirm = I_CON_IDLE;
			return;
		case I_CON_CALL_ALL_DATA_ACT: //总召唤确认
			if (Tx_CallAllDataCon() == true)
			{
				info.AckYxIndex = 0;
				info.AckYcIndex = 0;
				info.Next_I_Confirm = I_CON_CALL_ALL_YX;
			}
			return;
		case I_CON_CALL_ALL_YX: //发送全遥信
			{
				int ret = Tx_All_YX();
				switch(ret)
				{
				case RET_NO_DAT:
				case RET_TX_GRACE_OVER:// 数据发送过程结束
				case RET_STATE_BUSY: // 当前状态非空闲
					info.Next_I_Confirm = I_CON_CALL_ALL_YC;
					break;
				default:
					break;
				}
			}
			return;
		case I_CON_CALL_ALL_YC://发送全遥测
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
		case I_CON_ALL_DATA_TERM: //发送总召唤终止
			Tx_CallAllDataTerm();
			info.Next_I_Confirm = I_CON_IDLE;
			return;
			// 电度
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
			if (Tx_ChangeYx())//发送全遥信
				return;
			if (Tx_ChangeYc())//发送全遥测
				return;
			//发送换电数据
			 if(Tx_Change())
				 return;
			 //发送充电桩数据
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
//	p[i++] = C_CS_NA_1;					//类型
//	p[i++] = 1;							//结构限定词
//	i += MakeCause(p+i,CAUSE_ACT);		//传送原因
//	i += MakeCommAddr(p+i);				//公共地址（一般为子站站址）
//	i += MakeObjAddr(p+i,0);			//信息体地址
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
//WriteLog("Tx-------> 时间同步 失败：Tx_APDU_I(Len=%d) Return false", i);
//		return false;
//	}
//WriteLog("Tx-------> 时间同步 成功：Tx_APDU_I(Len=%d) Return true", i);
//
//
//
	return true;
}

//----------------------------------------------------------------------------
int	CProto_IEC104_C::SearchAPCI()
{
	getRx(buffer,4);


	//I格式
	if((buffer[0]&0x01) == IEC104_APCI_I)  //I 格式信息报文
	{
		if(RxDataSize <= 4)	return	-1;
		return	IEC104_APCI_I;
	}
	//S格式
	else	if((buffer[0]&0x03) == IEC104_APCI_S) //S 格式信息报文
	{
		if(RxDataSize != 4)	return	-1;
		return	IEC104_APCI_S;
	}
	//U格式
	else	if((buffer[0]&0x03) == IEC104_APCI_U) //U 格式信息报文
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


	// 控制方向报文
	if( !bInitAssociation )
	{
		App_RxAct(AppHead);
	}
	return	true;
}

//----------------------------------------------------------------------------
void	CProto_IEC104_C::DoCallAllDataAck(IEC104_APP_HEAD &head)
{
	//结构限定词
	if(head.SQ!=0 || head.Num!=1)	return;
	//传输原因
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
	//结构限定词
	if(head.SQ!=0 || head.Num!=1)	return;
	//传输原因
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
	//结构限定词
	if(head.SQ!=0 || head.Num!=1)	return;
	//传输原因
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
	buf[num++] = cause & 0x3F;	//T=0，PN=0
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
	// 报文是控制方向
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
	//默认规约特征
	//设置初始值————————开始
	//长度
	if(config.CommAddrLen == 0)	config.CommAddrLen = 2;
	if(config.ObjAddrLen == 0)	config.ObjAddrLen = 3;
	if(config.CotLen == 0)		config.CotLen = 2;
	//起始地址
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

	//设置初始值————————结束
	//计算基本信息————————开始
	config.YcMaxNum = config.Yc_End_Addr - config.Yc_Start_Addr + 1;
	config.YxMaxNum = config.Yx_End_Addr - config.Yx_Start_Addr + 1;
	config.KwhMaxNum = config.Kwh_End_Addr - config.Kwh_Start_Addr + 1;

	//计算基本信息————————结束

	//长度
	config.CommAddrLen = 2; 
	config.ObjAddrLen = 3;
	config.CotLen = 2;

	return	true;
}

/*-----------------------------------------------------------------------*\
接收控制方向报文
\*-----------------------------------------------------------------------*/
void CProto_IEC104_C::App_RxAct(IEC104_APP_HEAD &AppHead)
{
	//logprint(5008,"App_RxAct-1-----------AppHead.Type=%d",AppHead.Type);
	switch(AppHead.Type)
	{
	case C_CS_NA_1:		//时钟同步


		info.Next_I_Confirm = I_CON_SYSNTIME;
		break;
	case C_IC_NA_1:
		{
			unsigned char detail = 0;
			if (AppDataLen > 0) detail = pAppDataBuf[AppDataLen-1];
			else detail = pAppDataBuf[0];
			pAppDataBuf ++;
			if ( detail == 0x14 ) // 总召唤限定词QOI
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
			if ( detail == 0x45 ) // 总召唤电度
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
发送监视方向报文  同步时间<确认>
\*-----------------------------------------------------------------------*/
int CProto_IEC104_C::Tx_SyncTimeCon()
{
	if(info.WorkState != STATE_FREE)
	{
		return	false;
	}
	unsigned char	i = 0;
	unsigned char	*p = buffer + 7;
	p[i++] = C_CS_NA_1;					//类型
	p[i++] = 1;							//结构限定词
	i += MakeCause(p+i,CAUSE_ACT_CON);	//传送原因
	i += MakeCommAddr(p+i);				//公共地址（一般为子站站址）
	i += MakeObjAddr(p+i,0);			//信息体地址

	if(Tx_APDU_I(i) == false)	
	{
		return	false;
	}




	//不改变工作状态
	return	true;
}


/*-----------------------------------------------------------------------
发送监视方向报文  总召唤<确认>
*/
int CProto_IEC104_C::Tx_CallAllDataCon()
{
	if(info.WorkState != STATE_FREE)
	{

		return	false;
	}
	unsigned char	i = 0;
	unsigned char	*p = buffer + 7;
	p[i++] = C_IC_NA_1;					//类型
	p[i++] = 1;							//结构限定词
	i += MakeCause(p+i,CAUSE_ACT_CON);	//传送原因
	i += MakeCommAddr(p+i);				//公共地址（一般为子站站址）
	i += MakeObjAddr(p+i,0);			//信息体地址

	if(Tx_APDU_I(i) == false)	
	{

		return	false;
	}




	return	true;
}


/*-----------------------------------------------------------------------
发送监视方向报文  总召唤数据
*/
int CProto_IEC104_C::Tx_All_YX()
{
	if(info.WorkState != STATE_FREE)
	{

		return	RET_STATE_BUSY;
	}

	unsigned int zfYxNum = config.YxMaxNum;
	// 参数配置及共享内存是否合理 
	if ( zfYxNum <= 0 )
		return	RET_NO_DAT;

	// 确定发送信息体个数,总数-上次发送的数目,
	int ackindex = info.AckYxIndex;
	int txnum = zfYxNum - ackindex;	

	// 超过一帧能发送的最大数目,取最大数目.
	if ( txnum > MAX_YX_NUM_1_GRAM ) 
		txnum = MAX_YX_NUM_1_GRAM;
	else if (txnum <= 0)
		return RET_NO_DAT;

	// 缓冲区指针索引
	short i = 0;
	unsigned char * p = buffer + 7;

	// 不带时标的单点信息
	p[i++] = M_SP_NA_1; 

	// 结构限定词VSQ
	// bit 1234567(遥信个数)  8(顺序传送标志)
	p[i++] = txnum|0x80;

	i += MakeCause (p+i,CAUSE_RESP_CALLALL);	//传送原因
	i += MakeCommAddr (p+i);					//公共地址（一般为子站站址）
	i += MakeObjAddr (p+i,ackindex+config.Yx_Start_Addr);	//信息体地址

	// 信息体
//	DAC_YX_DATA data;
//	DAC_YX* pPara = NULL;
	int n = 0;
	for(n=0;n<txnum;n++,i++)
	{
		unsigned char tmpq = 0;

		//pPara = m_dataInf.yxPara(m_pRoute->group,ackindex+n);
		//if ( pPara == NULL || !pPara->valid || !m_dataInf.yx(m_route,ackindex+n,data) ) 
		//{
		//	tmpq |= 0x80;	//未被激活
		//}
		//else 
		//	tmpq = data.val;


		//if ( BFS_BIT_ENABLED((unsigned int)data.quality,QUALITY_BIT_IV) )
		//	 tmpq |= 0x40;   //无效

		////质量码暂不处理
		////if (pDi->data_active == false)  tmpq |= 0x80;   //未被激活
		////if (pDi->state & 0x01)          tmpq |= 0x40;   //无效
		////if (pDi->data_manset)           tmpq |= 0x20;   //人工

		p[i] = tmpq;
	}

	if(Tx_APDU_I(i) == false)	
	{

		return	RET_TX_DAT_FAIL;
	}


	info.AckYxIndex += txnum;

	//写发送接收命令缓存


	if (txnum < MAX_YX_NUM_1_GRAM)
	{
		return RET_TX_GRACE_OVER;
	}

	return RET_TX_DAT_SUCCESS;
}

/*-----------------------------------------------------------------------
发送监视方向报文  总召唤数据
*/
int CProto_IEC104_C::Tx_All_YC()
{
	if(info.WorkState != STATE_FREE)
	{
		return	RET_STATE_BUSY;
	}

	unsigned int zfYcNum = config.YcMaxNum;
	// 参数配置及共享内存是否合理 
	if( zfYcNum <= 0 )
		return	RET_NO_DAT;

	// 确定发送信息体个数,总数减去上次发送的数目,
	int ackindex = info.AckYcIndex;
	int txnum = zfYcNum - ackindex;	

	// 超过一帧能发送的最大数目,取最大数目.
	if ( txnum > MAX_YC_NUM_1_GRAM ) 
		txnum = MAX_YC_NUM_1_GRAM;
	else if (txnum <= 0)
		return RET_NO_DAT;

	// 缓冲区指针索引
	 short i = 0;
	unsigned char * p = buffer + 7;

	// 不带时标的标度值
	//p[i++] = M_ME_NB_1; 
	// 不带时标的短浮点数
	p[i++] = M_ME_NC_1;

	// 结构限定词VSQ
	// bit 1234567(遥测个数)  8(顺序传送标志)
	p[i++] = txnum|0x80;

	i += MakeCause (p+i,CAUSE_RESP_CALLALL);	//传送原因
	i += MakeCommAddr (p+i);			//公共地址（一般为子站站址）
	unsigned short objaddr = ackindex + config.Yc_Start_Addr;
	i += MakeObjAddr (p+i,objaddr);	//信息体地址

	// 信息体
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
		//	tmpq |= 0x80;	//未被激活
		//}
		//else
		//	fval = data.val;

		//memcpy(p+i, &fval,4);
		//i+=4;

		//if ( BFS_BIT_ENABLED((unsigned int)data.quality,QUALITY_BIT_IV) )
		//	tmpq |= 0x40;   //无效

		////质量码暂不处理
		////if (pAi->data_active == false) tmpq |= 0x80;       //未被激活
		////if (pAi->state & 0x01)         tmpq |= 0x40;       //无效--FROM SCADA
		////if (pAi->data_manset)          tmpq |= 0x20;       //人工
		////if (pAi->data_over)			 tmpq |= 0x01;	   //wfp add at 20080403 for henan

		p[i++] = tmpq;
	}

	if(Tx_APDU_I(i) == false)	
	{
	
		return	RET_TX_DAT_FAIL;
	}


	info.AckYcIndex += txnum;

	//写发送接收命令缓存


	if (txnum < MAX_YC_NUM_1_GRAM)
	{
		return RET_TX_GRACE_OVER;
	}

	return RET_TX_DAT_SUCCESS;
}


/*-----------------------------------------------------------------------
发送监视方向报文  总召唤数据132
*/
int CProto_IEC104_C::Tx_All_YC_132()
{
	if(info.WorkState != STATE_FREE)
	{
		return	RET_STATE_BUSY;
	}

	unsigned int zfYcNum = config.Yc132MaxNum;
	// 参数配置及共享内存是否合理 
	if( zfYcNum <= 0 )
		return	RET_NO_DAT;

	// 确定发送信息体个数,总数减去上次发送的数目,
	int ackindex = info.AckYcIndex;
	int txnum = zfYcNum - ackindex;	

	// 超过一帧能发送的最大数目,取最大数目.
	if ( txnum > MAX_YC_NUM_1_GRAM ) 
		txnum = MAX_YC_NUM_1_GRAM;
	else if (txnum <= 0)
		return RET_NO_DAT;

	// 缓冲区指针索引
	short i = 0;
	unsigned char * p = buffer + 7;

	// 不带时标的标度值
	//p[i++] = M_ME_NB_1; 
	// 不带时标的短浮点数
	p[i++] = M_ME_NC_1;

	// 结构限定词VSQ
	// bit 1234567(遥测个数)  8(顺序传送标志)
	p[i++] = txnum|0x80;

	i += MakeCause (p+i,CAUSE_RESP_CALLALL);	//传送原因
	i += MakeCommAddr (p+i);			//公共地址（一般为子站站址）
	unsigned short objaddr = ackindex + config.Yc_Start_Addr;
	i += MakeObjAddr (p+i,objaddr);	//信息体地址

	// 信息体
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
		//	tmpq |= 0x80;	//未被激活
		//}
		//else
		//	fval = data.val;

		//memcpy(p+i, &fval,4);
		//i+=4;

		//if ( BFS_BIT_ENABLED((unsigned int)data.quality,QUALITY_BIT_IV) )
		//	tmpq |= 0x40;   //无效

		////质量码暂不处理
		////if (pAi->data_active == false) tmpq |= 0x80;       //未被激活
		////if (pAi->state & 0x01)         tmpq |= 0x40;       //无效--FROM SCADA
		////if (pAi->data_manset)          tmpq |= 0x20;       //人工
		////if (pAi->data_over)			 tmpq |= 0x01;	   

		p[i++] = tmpq;
	}

	if(Tx_APDU_I(i) == false)	
	{

		return	RET_TX_DAT_FAIL;
	}



	info.AckYcIndex += txnum;

	//写发送接收命令缓存


	if (txnum < MAX_YC_NUM_1_GRAM)
	{
		return RET_TX_GRACE_OVER;
	}

	return RET_TX_DAT_SUCCESS;
}

/*-----------------------------------------------------------------------
发送监视方向报文  总召唤数据终止
*/
int CProto_IEC104_C::Tx_CallAllDataTerm()
{
	if(info.WorkState != STATE_FREE)
	{
		return	false;
	}
	unsigned char	i = 0;
	unsigned char	*p = buffer + 6;
	p[i++] = C_IC_NA_1;					//类型
	p[i++] = 1;							//结构限定词
	i += MakeCause(p+i,CAUSE_ACT_TERM);	//传送原因
	i += MakeCommAddr(p+i);				//公共地址（一般为子站站址）
	i += MakeObjAddr(p+i,0);			//信息体地址

	if(Tx_APDU_I(i) == false)	
	{
		return	false;
	}


	//写发送接收命令缓存


	//不改变工作状态
	return	true;
}

/*-----------------------------------------------------------------------
变化遥信
*/
int	CProto_IEC104_C::Tx_ChangeYx()
{
	/*if ( !m_dataInf.hasChgYx(m_route) )
		return false;*/

	// 缓冲区指针索引
	short i = 0,txnum=0;
	unsigned char * p = buffer + 7;

	// 不带时标的单点信息
	p[i++] = M_SP_NA_1; 

	i++; // 结构限定词VSQ，后面赋值 

	i += MakeCause (p+i,CAUSE_SPONT);	//传送原因
	i += MakeCommAddr (p+i);			//公共地址（一般为子站站址）

	//DAC_YX_CHG_DATA chgData; 
	for (;;)
	{
	//	if ( !m_dataInf.chgYx(m_route,chgData) ) 
	//		break;

//		i += MakeObjAddr (p+i,chgData.no+config.Yx_Start_Addr);	//信息体地址
		//unsigned char tmpq = chgData.data.val;
        unsigned char tmpq;
		//质量码暂不处理
		
		p[i++]=tmpq;

		txnum++;
		if ( txnum >= MAX_CHG_YX_NUM_1_GRAM ) 
			break;
	}

	if (txnum > 0)
	{
		// 结构限定词VSQ
		// bit 1234567(遥信个数)  8(顺序传送标志)
		p[1] = txnum;
		if(Tx_APDU_I(i) == false)	
		{
			return	false;
		}


		//写发送接收命令缓存


		return true;
	}
	return	false;
}


/*-----------------------------------------------------------------------
发送监视方向报文  变化遥测
*/
int CProto_IEC104_C::Tx_ChangeYc()
{
	//if ( !m_dataInf.hasChgYc(m_route) )
	//	return false;

	// 缓冲区指针索引
	 short i = 0,txnum=0;
	unsigned char * p = buffer + 7;

	// 不带时标的标度值
	//p[i++] = M_ME_NB_1; 
	// 不带时标的短浮点数
	p[i++] = M_ME_NC_1;
	i++; // 结构限定词VSQ，后面赋值 

	i += MakeCause (p+i,CAUSE_SPONT);	//传送原因
	i += MakeCommAddr (p+i);			//公共地址（一般为子站站址）

	//DAC_YC_CHG_DATA chgData;
	for (;;)
	{
		//if ( !m_dataInf.chgYc(m_route,chgData) ) 
		//	break;

		//i += MakeObjAddr (p+i,chgData.no+config.Yc_Start_Addr);	//信息体地址

		//float fval = chgData.data.val;

		float fval;
		memcpy(p+i, &fval,4);
		i+=4;

		unsigned char tmpq = 0;
		//质量码暂不处理
		p[i++] = tmpq;

		txnum++;
		if ( txnum >= MAX_CHG_YC_NUM_1_GRAM ) 
			break;
	}

	if (txnum > 0)
	{
		// 结构限定词VSQ
		// bit 1234567(遥信个数)  8(顺序传送标志)
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
发送监视方向报文  变化遥测
*/
int CProto_IEC104_C::Tx_ChangeYc_132()
{
	//if ( !m_dataInf.hasChgYc(m_route) )
	//	return false;

	// 缓冲区指针索引
	short i = 0,txnum=0;
	unsigned char * p = buffer + 7;


	// 不带时标的短浮点数
	p[i++] = M_MD_NA_1;
	i++; // 结构限定词VSQ，后面赋值 

	i += MakeCause (p+i,CAUSE_SPONT);	//传送原因
	i += MakeCommAddr (p+i);			//公共地址（一般为子站站址）

	//YC_2_CHG_DATA chgData;
	for (;;)
	{
		//if ( chgYc_2(chgData) ) 
		//	break;

		//i += MakeObjAddr (p+i,chgData.no+config.Yc2_Start_Addr);	//信息体地址

		//float fval = chgData.data.val;
		float fval;
		memcpy(p+i, &fval,4);
		i+=4;

		unsigned char tmpq = 0;
		//质量码暂不处理
		p[i++] = tmpq;

		txnum++;
		if ( txnum >= MAX_CHG_YC_NUM_1_GRAM ) 
			break;
	}

	if (txnum > 0)
	{
		// 结构限定词VSQ
		// bit 1234567(遥信个数)  8(顺序传送标志)
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
发送监视方向报文  响应总召唤电度确认
*/
int CProto_IEC104_C::Tx_CallAllKwhCon()
{
	if(info.WorkState != STATE_FREE)
	{

		return	false;
	}
	unsigned char	i = 0;
	unsigned char	*p = buffer + 6;
	p[i++] = C_CI_NA_1;					//类型
	p[i++] = 1;							//结构限定词
	i += MakeCause(p+i,CAUSE_ACT_CON);	//传送原因
	i += MakeCommAddr(p+i);				//公共地址（一般为子站站址）
	i += MakeObjAddr(p+i,0);			//信息体地址

	if(Tx_APDU_I(i) == false)	
	{

		return false;
	}



	return true;
}
/*-----------------------------------------------------------------------
发送监视方向报文  响应总召唤电度数据
*/
int CProto_IEC104_C::Tx_All_Kwh()
{
	if(info.WorkState != STATE_FREE)
	{

		return	RET_STATE_BUSY;
	}

	int zfKwhNum = config.KwhMaxNum;

	// 参数配置及共享内存是否合理 
	if(zfKwhNum <= 0)
		return	RET_NO_DAT;

	// 确定发送信息体个数,总数减去上次发送的数目,
	int ackindex = info.AckKwhIndex;
	int txnum = zfKwhNum - ackindex;	

	// 超过一帧能发送的最大数目,取最大数目.
	if ( txnum > MAX_KWH_NUM_1_GRAM ) 
		txnum = MAX_KWH_NUM_1_GRAM;
	else if (txnum <= 0)
		return RET_NO_DAT;

	// 缓冲区指针索引
     short i = 0;
	unsigned char * p = buffer + 7;

	// 不带时标的累积量
	p[i++] = M_IT_NA_1; 

	// 结构限定词VSQ
	// bit 1234567(遥测个数)  8(顺序传送标志)
	p[i++] = txnum|0x80;

	i += MakeCause (p+i,CAUSE_RESP_CALLALL);	//传送原因
	i += MakeCommAddr (p+i);					//公共地址（一般为子站站址）
	unsigned short objaddr = ackindex + config.Kwh_Start_Addr;
	i += MakeObjAddr (p+i,objaddr);	//信息体地址

	// 信息体
//	DAC_KWH_DATA data;
	int n = 0;
	for(n=0;n<txnum;n++)
	{
		double fval = 0.0;
		unsigned char tmpq = 0;

		//if ( !m_dataInf.kwh(m_route,ackindex+n,data) )
		//{
		//	tmpq |= 0x80;	//未被激活
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
发送监视方向报文  响应总召唤电度结束
*/
int CProto_IEC104_C::Tx_CallAllKwhTerm()
{
	if(info.WorkState != STATE_FREE)
	{

		return	false;
	}
	unsigned char	i = 0;
	unsigned char	*p = buffer + 6;
	p[i++] = C_CI_NA_1;					//类型
	p[i++] = 1;							//结构限定词
	i += MakeCause(p+i,CAUSE_ACT_TERM);	//传送原因
	i += MakeCommAddr(p+i);				//公共地址（一般为子站站址）
	i += MakeObjAddr(p+i,0);			//信息体地址

	if(Tx_APDU_I(i) == false)	
	{

		return	false;
	}


	//不改变工作状态
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
	

	//入口校验
	if ( buf == NULL || len < 0 || len > bufSize ) return -1;
	if ( len == 0 ) return 0;

	unsigned int realLen = 0;
	if ( length() <= len ) //数据不够
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
	

	//入口校验
	if ( buf == NULL || len < 0 || len > bufSize ) return -1;
	if ( len == 0 ) return 0;

	unsigned int realLen = 0;
	if ( length() <= len ) //数据不够
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
	

	//需要环绕一遍
	if ( write + len > bufSize )
	{
		unsigned int hasLen,remainLen;
		hasLen = bufSize - write;
		memcpy( pData+write,buf,hasLen );
		

		sendBuffer->write = (write + hasLen) % bufSize;
			write = sendBuffer->write;

		//剩余没拷贝的字节数
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
	

	//需要环绕一遍
	if ( write + len > bufSize )
	{
		unsigned int hasLen,remainLen;
		hasLen = bufSize - write;
		memcpy( pData+write,buf,hasLen );
		
		
		recvBuffer->write = (write + hasLen) % bufSize;
		write = recvBuffer->write;
		

		//剩余没拷贝的字节数
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
