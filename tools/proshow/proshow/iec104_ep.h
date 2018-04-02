

#ifndef _PROTOCOL_IEC104_EP_H_
#define _PROTOCOL_IEC104_EP_H_




#define BUFSIZE_104		4096					//临时缓冲区大小(4k)

//YK类型定义：0——无遥控命令
#define YK_SET_CMD		1			//YK预置
#define YK_EXE_CMD		2			//YK执行
#define YK_DEL_CMD		3			//YK撤消



//--------------------应用规约控制信息格式--------------------------------------------------------
#define	IEC104_APCI_I		0		//I格式（信息传输格式）
#define	IEC104_APCI_S		0x01	//S格式（计数的监视功能）
#define	IEC104_APCI_U		0x03	//U格式（不计数的控制功能）
//--------------------应用规约控制信息--------------------------------------------------------
#define	APCI_U_STARTDT_ACT	0x04	//启动链路——激活
#define	APCI_U_STARTDT_CON	0x08	//启动链路——确认
#define	APCI_U_STOPDT_ACT	0x10	//停止链路——激活
#define	APCI_U_STOPDT_CON	0x20	//停止链路——确认
#define	APCI_U_TESTFR_ACT	0x40	//测试链路——激活
#define	APCI_U_TESTFR_CON	0x80	//测试链路——确认
//--------------------类型标识--------------------------------------------------------
//在监视方向的过程信息（0——44）（类型30——40在IEC 60870-5-101的补充件A1中定义）
//0									//未定义
#define	M_SP_NA_1			1		//单点信息
#define	M_SP_TA_1			2		//带CP24Time2a时标的单点信息
#define	M_DP_NA_1			3		//双点信息
#define	M_DP_TA_1			4		//带CP24Time2a时标的双点信息
#define	M_ST_NA_1			5		//步位置信息
#define	M_ST_TA_1			6		//带CP24Time2a时标的步位置信息
#define	M_BO_NA_1			7		//32比特串
#define	M_BO_TA_1			8		//带CP24Time2a时标的32比特串
#define	M_ME_NA_1			9		//测量值，规一化值
#define	M_ME_TA_1			10		//带CP24Time2a时标的测量值，规一化值
#define	M_ME_NB_1			11		//测量值，标度化值
#define	M_ME_TB_1			12		//带CP24Time2a时标的测量值，称度化值
#define	M_ME_NC_1			13		//测量值，短浮点数
#define	M_ME_TC_1			14		//带CP24Time2a时标的测量值，短浮点数
#define	M_IT_NA_1			15		//累积量
#define	M_IT_TA_1			16		//带CP24Time2a时标的累积量
#define	M_EP_TA_1			17		//带CP24Time2a时标的继电保护装置事件
#define	M_EP_TB_1			18		//带CP24Time2a时标的继电保护装置成组启动事件
#define	M_EP_TC_1			19		//带时标的继电保护装置成组输出电路信息
#define	M_PS_NA_1			20		//带变位检出的成组单点信息
#define	M_ME_ND_1			21		//测量值，不带品质描述词的规一化值
//22——29							//为将来兼容定义保留
#define	M_SP_TB_1			30		//带CP56Time2a时标的单点信息
#define	M_DP_TB_1			31		//带CP56Time2a时标的双点信息
#define	M_ST_TB_1			32		//带CP56Time2a时标的步位置信息
#define	M_BO_TB_1			33		//带CP56Time2a时标的32 比特串
#define	M_ME_TD_1			34		//带CP56Time2a时标的测量值，规一化值
#define	M_ME_TE_1			35		//带CP56Time2a时标的测量值，标度化值
#define	M_ME_TF_1			36		//带CP56Time2a时标的测量值，短浮点数
#define	M_IT_TB_1			37		//带CP56Time2a时标的累积量
#define	M_EP_TD_1			38		//带CP56Time2a时标的继电保拌装置事件
#define	M_EP_TE_1			39		//带CP56Time2a时标的继电保拌装置成组启动事件
#define	M_EP_TF_1			40		//带CP56Time2a时标的继电保拌装置成组输出电路信息
//41——44							//为将来兼容定义保留
//在控制方向的过程信息（45——69）
#define	C_SC_NA_1			45		//单点命令
#define	C_DC_NA_1			46		//双点命令
#define	C_RC_NA_1			47		//调节步命令
#define	C_SE_NA_1			48		//设定值命令，规一化值
#define	C_SE_NB_1			49		//设定值命令，标度化值
#define	C_SE_NC_1			50		//设定值命令，短浮点数
#define	C_BO_NA_1			51		//32 比特串
//52——57							//为将来兼容定义保留
#define	C_SC_TA_1			58		//带CP56Time2a时标的单点命令
#define	C_DC_TA_1			59		//带CP56Time2a时标的双点命令
#define	C_RC_TA_1			60		//带CP56Time2a时标的调节步命令
#define	C_SE_TA_1			61		//带CP56Time2a时标的设定值命令，规一化值
#define	C_SE_TB_1			62		//带CP56Time2a时标的设定值命令，标度化值
#define	C_SE_TC_1			63		//带CP56Time2a时标的设定值命令，短浮点数
#define	C_BO_TA_1			64		//带CP56Time2a时标的32比特串
//65——69							//为将来兼容定义保留
//在监视方向的系统命令（70——99）
#define	M_EI_NA_1			70		//初始化结束
//71——99							//为将来兼容定义保留
//在控制方向的系统命令（100——109）
#define	C_IC_NA_1			100		//总召唤命令
#define	C_CI_NA_1			101		//计数量召唤命令
#define	C_RD_NA_1			102		//读命令
#define	C_CS_NA_1			103		//时钟同步命令
#define	C_TS_NA_1			104		//测试命令
#define	C_RP_NA_1			105		//复位进程命令
#define	C_CD_NA_1			106		//收集传输延时
#define	C_TS_TA_1			107		//带CP56Time2a时标的测试命今
//108——109						//为将来兼容定义保留
//在控制方向的参数命令（110——119）
#define	P_ME_NA_1			110		//测量值参数，规一化值
#define	P_ME_NB_1			111		//测量值参数，标度化值
#define	P_ME_NC_1			112		//测量值参数，短浮点数
#define	P_AC_NA_1			113		//参数激活
//110——119						//为将来兼容定义保留
//文件传输（120——127）
#define	F_FR_NA_1			120		//文件淮备就绪
#define	F_SR_NA_1			121		//节淮备就绪
#define	F_SC_NA_1			122		//召唤目录，选择文件，召唤文件召唤节
#define	F_LS_NA_1			123		//最后的节,最后的段
#define	F_AF_NA_1			124		//认可文件,认可节
#define	F_SG_NA_1			125		//段
#define	F_DR_NA_1			126		//目录
//127								//为将来兼容定义保留
//130--133
#define M_RE_NA_1           130     //充电桩业务数据
#define M_CM_NA_1           131     //换电业务数据
#define M_MD_NA_1           132     //测量值，标度化值，长度大于2 字节


#define P_PL_NA_1			144		//计划值
#define A_HD_NA_1			145		//历史值--遥测
#define E_HD_NA_1			146		//历史值--电度
//--------------------传送原因--------------------------------------------------------
#define CAUSE_SPONT                 3       //自发
//在控制方向
#define	CAUSE_INIT					4		//初始化
#define	CAUSE_REQ					5		//请求
#define	CAUSE_ACT					6		//激活
#define	CAUSE_STOPACT				8		//停止激活
//在监视方向
#define	CAUSE_ACT_CON				7		//激活确认
#define	CAUSE_STOPACT_CON			9		//停止激活确认
#define	CAUSE_ACT_TERM				10		//激活终止
#define	CAUSE_RESP_CALLALL			20		//响应总召唤
#define	CAUSE_UNKOWN_TYPE			44		//未知的类型标识
#define	CAUSE_UNKOWN_CAUSE			45		//未知的传送原因
#define	CAUSE_UNKOWN_APDUADDR		46		//未知的应用服务数据单元公共地址
#define	CAUSE_UNKOWN_DATAADDR		47		//未知的信息体地址

//在控制方向的传送原因
#define IS_CONTROL_DIR(_CAUSE_)     ((_CAUSE_)==CAUSE_INIT || (_CAUSE_)==CAUSE_REQ || (_CAUSE_)==CAUSE_ACT || (_CAUSE_)==CAUSE_STOPACT)

//--------------------状态定义--------------------------------------------------------
//链路状态
#define	STATE_UNBOUND				0		//未连接
#define	STATE_IDLE					1		//连接正常
//工作状态（主动链接端）
#define	STATE_FREE					0		//空闲
#define	STATE_WAIT_STARTDT_CON		1		//等待启动链路确认
#define	STATE_WAIT_STOPDT_CON		2		//等待停止链路确认
#define	STATE_WAIT_TESTDT_CON		3		//等待测试链路确认
#define	STATE_WAIT_ALLDATA_CON		11		//等待总召唤确认
#define	STATE_WAIT_ALLKWH_CON		12		//等待计数量召唤确认
#define	STATE_WAIT_TIME_CON			13		//等待时钟同步确认
#define	STATE_WAIT_ALLDATA			14		//等待总召唤的数据
#define	STATE_WAIT_ALLKWH			15		//等待计数量召唤的数据

//服务器端工作状态（等待链接端）
#define	STATE_WAIT_STARTDT_ACK		30		//等待启动链路激活


//------------------------等待确认的命令-------------------------------------------------
#define I_CON_IDLE				  0
#define I_CON_SYSNTIME			  1

#define I_CON_CALL_ALL_DATA_ACT	 11
#define I_CON_CALL_ALL_YX		 12
#define I_CON_CALL_ALL_YC		 13
#define I_CON_ALL_DATA_TERM      14

#define I_CON_CALL_ALL_KWH_ACT   21
#define I_CON_CALL_ALL_KWH		 22
#define I_CON_ALL_KWH_TERM       23

//----------------------------信息对象地址定义------------------------------------------------
#define	IEC104_OBJ_ADDR_UNKOWN				0			//无关的信息对象地址
//2字节信息对象地址
#define	IEC104_OBJ_16_ADDR_YX_START			0x0001		//遥信——起始
#define	IEC104_OBJ_16_ADDR_YX_END			0x1000		//遥信——终止
#define	IEC104_OBJ_16_ADDR_YC_START			0x4001		//遥测——起始
#define	IEC104_OBJ_16_ADDR_YC_END			0x5000		//遥测——终止
#define	IEC104_OBJ_16_ADDR_PARA_START		0x5001		//参数——起始
#define	IEC104_OBJ_16_ADDR_PARA_END			0x6000		//参数——终止
#define	IEC104_OBJ_16_ADDR_SET_START		0x6201		//设定——起始
#define	IEC104_OBJ_16_ADDR_SET_END			0x6400		//设定——终止
#define	IEC104_OBJ_16_ADDR_KWH_START		0x6401		//电能累计量——起始（）
#define	IEC104_OBJ_16_ADDR_KWH_END			0x6600		//电能累计量——终止
#define	IEC104_OBJ_16_ADDR_STEP_START		0x6601		//步位置——起始
#define	IEC104_OBJ_16_ADDR_STEP_END			0x6700		//步位置——终止
#define	IEC104_OBJ_16_ADDR_BIN_START		0x6701		//向被控站传送二进制信息——起始
#define	IEC104_OBJ_16_ADDR_BIN_END			0x6800		//向被控站传送二进制信息——终止
#define	IEC104_OBJ_16_ADDR_RTU_STATE		0x6801		//远东终端状态

//----------------------------------------------------------------------------
//定时器超时定义
#define	TIMEOUT_T1			10*1000    //无数据报文确认的超时时间，默认10秒；
#define	TIMEOUT_T2			15*1000   //发送或测试APDU的超时时间，默认15秒；
#define	TIMEOUT_T3			20*1000   //长期空闲状态下发送测试帧的超时时间，默认20秒。 
#define	TIMEOUT_T4			20*1000   //连接建立的超时时间，默认20秒；

#define TIMEOUT_SYNC		60*1000*60
#define TIMEOUT_ALLYC		60*1000*60
#define TIMEOUT_ALLYX		60*1000*60
#define TIMEOUT_ALLKWH		60*1000*60

//在一组报文中最大的发送信息个数
#define MAX_YX_NUM_1_GRAM      127
#define MAX_CHG_YX_NUM_1_GRAM   60	
#define MAX_SOE_NUM_1_GRAM      24
#define MAX_YC_NUM_1_GRAM       48
#define MAX_CHG_YC_NUM_1_GRAM   30
#define MAX_KWH_NUM_1_GRAM      48

//发送全数据结果
#define RET_TX_DAT_SUCCESS      0 // 发送成功,但仍有数据待发
#define RET_TX_GRACE_OVER       1 // 数据发送过程结束
#define RET_NO_DAT              2 // 没有要发送的数据
#define RET_TX_DAT_FAIL         3 // 发送失败,要重发
#define RET_STATE_BUSY          4 // 当前状态非空闲

//计划值
#define PL_FLAG_NODAT			0 // 没有计划值
#define PL_FLAG_TX_DAT			1 // 有计划值需要发送
#define PL_FLAG_TX_TERM			2 // 发送计划值结束


#define RECV_BUFSIZE			8192		//接收缓冲区大小(8k)
#define SEND_BUFSIZE			8192		//发送缓冲区大小(8k)

//----------------------------------------------------------------------------
//应用层头
typedef	struct
{
	unsigned char 			Type;			//类型标识

	unsigned char		Num:7;			//数据个数——6、5、4、3、2、1、0位
	unsigned char		SQ:1;			//单个或者顺序（0——单个，1——顺序）——7位

	//可变结构限定词

	unsigned char		Cause:6;		//传送原因序号——5、4、3、2、1、0位
	unsigned char		PN:1;			//确认（0——肯定确认，1——否定确认）——6位
	unsigned char		T:1;			//测试（0——未试验，1——试验）——7位

	unsigned char		SrcAddr;		//源发站地址
	//传送原因
	unsigned short			CommAddr;		//应用服务数据单元公共地址
}IEC104_APP_HEAD;
//----------------------------------------------------------------------------
typedef	struct
{
	
	//规约特征————————开始
	//长度
	unsigned char			CommAddrLen;			//应用服务数据单元公共地址长度（1——2）
	unsigned char			ObjAddrLen;				//信息对象地址长度（1——3）（一般为2）
	unsigned char			CotLen;					//传送原因长度（1——2）（一般为2）
	//起始地址
	unsigned int			Yx_Start_Addr;			//遥信——起始
	unsigned int			Yx_End_Addr;			//遥信——终止

	unsigned int			Yc_Start_Addr;			//遥测——起始
	unsigned int			Yc_End_Addr;			//遥测——终止

	unsigned int            Yc2_Start_Addr;
    unsigned int            Yc2_End_Addr;

	unsigned int			Para_Start_Addr;		//参数——起始
	unsigned int			Para_End_Addr;			//参数——终止

	unsigned int			Set_Start_Addr;			//设定——起始
	unsigned int			Set_End_Addr;			//设定——终止


	unsigned int			Kwh_Start_Addr;			//电能累计量——起始
	unsigned int			Kwh_End_Addr;			//电能累计量——终止

	unsigned int			Step_Start_Addr;		//步位置——起始
	unsigned int		Step_End_Addr;			//步位置——终止

	unsigned int			Bin_Start_Addr;			//向被控站传送二进制信息——起始
	unsigned int			Bin_End_Addr;			//向被控站传送二进制信息——终止
	unsigned int			Rtu_State_Addr;			//远动终端状态

	//规约特征————————结束
	//固定信息————————开始
	unsigned int		YcMaxNum;				//YC最大个数
	unsigned int			YxMaxNum;				//YX最大个数
	unsigned int			KwhMaxNum;				//KWH最大个数
	unsigned int			YkMaxNum;				//YK最大个数
	unsigned int           Yc132MaxNum;
	//固定信息————————结束
}IEC104_CONFIG;

typedef	struct
{
	//运行时信息
	unsigned int	T1;						//等待接收对端应答信息定时器（超时占用关闭）
	unsigned int	T2;						//等待本端信息处理完成定时器（超时发送S帧）
	unsigned int	T3;						//等待双方进行数据传输定时器（超时启动测试过程）
	unsigned int	T4;						//等待启动链路确认定时器（超时占用关闭）
	unsigned char	LinkState;				//当前链路状态
	unsigned char	WorkState;				//当前工作状态
	unsigned short	NS;						//发送序号
	unsigned short	NR;						//接收序号
	unsigned char	CallAllDataFlag;		//请求全数据（YC、YX）标志
	unsigned char	CallAllKwhFlag;			//请求全KWH标志
	//S_ACT
	unsigned char	Send_S_ACK_Flag;		//发送S帧确认的标志
	//U_ACT
	unsigned char	Send_U_TESTFR_ACT_Flag;	//发送U帧测试链路激活标志
	unsigned char	Send_U_STOPDT_ACT_Flag;	//发送U帧停止链路激活标志
	//U_CON
	unsigned char	Send_U_STARTDT_CON_Flag;//发送U帧激活链路确认标志
	unsigned char	Send_U_TESTFR_CON_Flag; //发送U帧测试链路确认标志
	unsigned char	Send_U_STOPDT_CON_Flag; //发送U帧停止链路确认标志
	//I_ACT
	unsigned char	SendTimeFlag;			//同步时钟标志  
	//I_CON
	unsigned short  Next_I_Confirm;  		//下一个需要确认的命令

	//本次遥信发送的起始地址
	int  AckYxIndex;
	//本次遥测发送的起始地址
	int  AckYcIndex;
	//本次遥测发送的起始地址
	int  AckKwhIndex;

	//DAC_YKBUF		cur_ykbuf;				//当前YK缓冲区
	//PLAN
	//unsigned char	SendPlanFlag;			//有计划值标志  Bruce
	//short			SendPlanSize;			//当前发送的计划值缓冲区位置 Bruce
}IEC104_INFO;



//接收报文缓存
typedef struct recv_buffer_
{ 
	unsigned char			data[RECV_BUFSIZE];		//接收缓冲数据16k
	unsigned int			write;						//接收缓冲写指针  
}RECV_BUFFER;

//发送报文缓存
typedef struct send_buffer_
{
	unsigned char			data[SEND_BUFSIZE];		//发送缓冲数据
	unsigned int			write;						//发送缓冲写指针  
}SEND_BUFFER;
//----------------------------------------------------------------------------

class  CProto_IEC104_C
{
private:
	bool		   bInitAssociation;				//主动发起初始化链路标志, FALSE -- 被动等待链接激活, TRUE -- 主动发起链接激活.

	IEC104_CONFIG	config;
	IEC104_INFO		info;

	unsigned char			*pAppDataBuf;					//应用层数据指针
	unsigned int			AppDataLen;						//应用层数据长度
	unsigned int			RxDataSize;						//收到的数据长度


	int				MakeCommAddr(unsigned char *buf);				//生成公共地址，返回长度
	int				MakeObjAddr(unsigned char *buf,int no);		//生成对象地址，返回长度
	int				MakeCause(unsigned char *buf,unsigned char cause);	//生成传送原因，返回长度
	int				GetAppHead(IEC104_APP_HEAD *head);		//获得数据单元标识符
	int				GetObjAddr();							//获得对象地址
	//----------------------------------------------------------------------------
	void			InitInfo();						//初始化基本信息
	void			NoRxAnyData();					//给一些计数量累加
	void			RxSomeData();					//清除一些计数量
	//void			CheckTimeOut();					//超时处理

	int				Tx_APDU_I(unsigned short len);	//发送I格式的应用规约控制单元
	int				Tx_APDU_S();					//发送S格式的应用规约控制单元
	int				Tx_APDU_U(unsigned char type);	//发送U格式的应用规约控制单元

	int				Tx_CtrlCmd();					//发送控制命令
	int				Tx_AnalogOutPut();				//发送模拟量输出

	void			Tx_DataCmd();					//发送数据命令
	int				Tx_CallAllData();				//总召唤
	int				Tx_CallAllKwh();				//计数量召唤
	int				Tx_SyncTime();					//同步时钟

	int				SearchAPCI();					//搜索应用规约控制信息
	int				SearchASDU();					//搜索应用服务数据单元

	void			DoCallAllDataAck(IEC104_APP_HEAD &head);	//处理总召唤应答
	void			DoCallAllKwhAck(IEC104_APP_HEAD &head);		//处理召唤全电度应答
	void			DoSyncTimeAck(IEC104_APP_HEAD &head);		//处理时钟同步应答
	void			DoYkAck(IEC104_APP_HEAD &head);				//处理YK命令
    


	/*-----------------------------------------------------------------------
	接收控制方向报文
	*/
	void            App_RxAct(IEC104_APP_HEAD &);


	/*-----------------------------------------------------------------------
	发送监视方向报文  同步时间<确认>
	*/
	int             Tx_SyncTimeCon();
	/*-----------------------------------------------------------------------
	发送监视方向报文  总召唤<确认>
	*/
	int             Tx_CallAllDataCon();
	/*-----------------------------------------------------------------------
	发送监视方向报文  总召唤遥信
	*/
	int             Tx_All_YX();
	/*-----------------------------------------------------------------------
	发送监视方向报文  变化遥信
	*/
	int	            Tx_ChangeYx();
	/*-----------------------------------------------------------------------
	发送监视方向报文  总召唤遥测
	*/
	int             Tx_All_YC();
	/*-----------------------------------------------------------------------
	发送监视方向报文
	*/ 
	int             Tx_All_YC_132(); 
	/*-----------------------------------------------------------------------
	
	发送监视方向报文  变化遥测
	*/
	int	            Tx_ChangeYc();
	int             Tx_ChangeYc_132();
	
	/*-----------------------------------------------------------------------
	发送监视方向报文  总召唤数据终止
	*/
	int             Tx_CallAllDataTerm();
	/*-----------------------------------------------------------------------
	发送监视方向报文  响应总召唤电度确认
	*/
	int             Tx_CallAllKwhCon();
	/*-----------------------------------------------------------------------
	发送监视方向报文  响应总召唤电度数据
	*/
	int             Tx_All_Kwh();
	/*-----------------------------------------------------------------------
	发送监视方向报文  响应总召唤电度结束
	*/
	int             Tx_CallAllKwhTerm();

	/*-----------------------------------------------------------------------
	发送监视方向报文  I帧
	*/
	int             Tx_App_I_Frame();
	/*-----------------------------------------------------------------------
	发送监视方向报文  换电桩数据
	*/
	int	            Tx_Change(); 
	/*-----------------------------------------------------------------------
	发送监视方向报文 充电桩数据
	*/
	int	            Tx_Charge();

	

public:
	CProto_IEC104_C();
	virtual ~CProto_IEC104_C();

	//是否打开
	bool	isOpen() const;
	//打开规约
	 bool	open();
	//关闭规约
	 void	close();
	//接收处理入口
	 void	Rx();
	//发送处理入口
	 void	Tx();
	 //接收到的数据发送到缓存
	unsigned int getRx(unsigned char *buf,unsigned int len);
	unsigned int getTx(unsigned char *buf,unsigned int len);

	bool back(unsigned int num);
	bool putTx(const unsigned char *buf,unsigned int len);
	bool putRx(const unsigned char *buf,unsigned int len);
unsigned int	length()	const;
protected:
	
	 bool	readFeature();


public:
	

	unsigned char	buffer[BUFSIZE_104];				
	bool			m_isOpen;
	unsigned char m_localAddr;
	unsigned char m_remoteAddr;

    RECV_BUFFER		*recvBuffer;	//接收报文缓冲区
	SEND_BUFFER		 *sendBuffer;	//发送报文缓冲区
	int    m_Rread;
	int    m_Tread;
};

#endif //_PROTOCOL_IEC104_ICC_H_
