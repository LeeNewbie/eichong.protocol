

#ifndef _PROTOCOL_IEC104_H_
#define _PROTOCOL_IEC104_H_

#include "proshow.h"
#include "ACDecode.h"



//--------------------应用规约控制信息格式--------------------------------------------------------
#define	IEC104_APCI_I		0		//I格式（信息传输格式）
#define	IEC104_APCI_S		0x01	//S格式（计数的监视功能）
#define	IEC104_APCI_U		0x03	//U格式（不计数的控制功能）


//--------------------应用规约控制信息--------------------------------------------------------
#define	APCI_U_STARTDT_ACT	0x07	//启动链路——激活
#define	APCI_U_STARTDT_CON	0x0b	//启动链路——确认
#define	APCI_U_STOPDT_ACT	0x12	//停止链路——激活
#define	APCI_U_STOPDT_CON	0x22	//停止链路——确认
#define	APCI_U_TESTFR_ACT	0x43	//测试链路——激活
#define	APCI_U_TESTFR_CON	0x83	//测试链路——确认



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

#define M_MD_NA_1          132
#define M_RE_NA_1          130
#define M_JC_NA_1          134
#define C_SD_NA_1          133

//--------------------传送原因--------------------------------------------------------
//在控制方向
#define CAUSE_SPONT                 3       //自发
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



class  CProto_IEC104
{
private:
	

	int				SearchAPCI(unsigned char *buffer, int len);					//搜索应用规约控制信息

	int				SearchASDU(unsigned char *buffer, int len);					//搜索应用服务数据单元

	

	void			YC_RxProc(CString strMsg,unsigned char * buffer,int len);			//YC接收处理
	void			YX_RxProc(CString strMsg,unsigned char * buffer,int len);			//YX接收处理

	void			KWH_RxProc(CString strMsg,unsigned char * buffer,int len);			//KWH接收处理

	void            UP_RxProcV1(CString strMsg,unsigned char * buffer,int len);        //接收上行数据V1版本

	void            UP_RxProcV2(CString strMsg,unsigned char * buffer,int len);        //接收上行数据v2版本
	void            UP_RxProcV4(CString strMsg,unsigned char * buffer,int len);        //接收上行数据v2版本

	void	        All_dataProc(CString strMsg,unsigned char * buff,int len);  //处理实时数据整包

	void	        Down_RxProcV1(CString strMsg,unsigned char * buffer,int len);
	void	        Down_RxProcV2(CString strMsg,unsigned char * buffer,int len);
	void	        Down_RxProcV4(CString strMsg,unsigned char * buffer,int len);


	void	       AC_Proc3(CString strMsg,unsigned char * buffer,int len); //记录类型为3

	void	       AC_Proc1(CString strMsg,unsigned char * buffer,int len); //记录类型为1

	void	       DC_Proc2(CString strMsg,unsigned char * buffer,int len); //记录类型为2

	void	       DC_Proc4(CString strMsg,unsigned char * buffer,int len); //记录类型为4

	void	       AC_RxChangeProc(CString strMsg,unsigned char * buffer,int len);


	void	        Pro_one_yx(CString strMsg,unsigned char *buffer,int len);
	void	        Pro_two_yx(CString strMsg,unsigned char *buffer,int len);
	void	        Pro_yc(CString strMsg,unsigned char *buffer,int len);
	void	        Pro_yc2(CString strMsg,unsigned char *buffer,int len);

	CString        errorMsg(CString strMsg,int cos);
	CString        cosMsg(CString strMsg,int cos);
	CString        headMsg(CString strMsg,unsigned char *buffer,CString desc);
	CString        headMsg1(CString strMsg,unsigned char *buffer,CString desc);
	CString        cmdTimeMsg(CString strMsg,CString strErr,unsigned char *buffer,int len);
	CString        crcMsg(CString strMsg,CString strErr,unsigned char *buffer,int len);

	CString        timeMsg(CString strMsg,unsigned char *buffer,CString timeName);
	int            getInt4(unsigned char *buffer);
	int            getInt4Small(unsigned char *buffer);
	int            getInt2(unsigned char *buffer);
	int            getInt2Small(unsigned char *buffer);

	

	void	        Pro_UP_1(CString strMsg,unsigned char *buffer,int len);
	void	        Pro_UP_2(CString strMsg,unsigned char *buffer,int len);
	void	        Pro_UP_3(CString strMsg,unsigned char *buffer,int len);
	void	        Pro_UP_4(CString strMsg,unsigned char *buffer,int len);
	void	        Pro_UP_5(CString strMsg,unsigned char *buffer,int len);
	void	        Pro_UP_6(CString strMsg,unsigned char *buffer,int len);
	void	        Pro_UP_7(CString strMsg,unsigned char *buffer,int len);
	void	        Pro_UP_8(CString strMsg,unsigned char *buffer,int len);
	void	        Pro_UP_9(CString strMsg,unsigned char *buffer,int len);
	void	        Pro_UP_10(CString strMsg,unsigned char *buffer,int len);
	void	        Pro_UP_11(CString strMsg,unsigned char *buffer,int len);
	void	        Pro_UP_12(CString strMsg,unsigned char *buffer,int len);
	void	        Pro_UP_13(CString strMsg,unsigned char *buffer,int len);
	void	        Pro_UP_14(CString strMsg,unsigned char *buffer,int len);
	void	        Pro_UP_15(CString strMsg,unsigned char *buffer,int len);
	void	        Pro_UP_16(CString strMsg,unsigned char *buffer,int len);
	void	        Pro_UP_17(CString strMsg,unsigned char *buffer,int len);
	void	        Pro_UP_18(CString strMsg,unsigned char *buffer,int len);
	void	        Pro_UP_19(CString strMsg,unsigned char *buffer,int len);
	void	        Pro_UP_20(CString strMsg,unsigned char *buffer,int len);
	void	        Pro_UP_21(CString strMsg,unsigned char *buffer,int len);
	void	        Pro_UP_22(CString strMsg,unsigned char *buffer,int len);
	void	        Pro_UP_23(CString strMsg,unsigned char *buffer,int len);
	void	        Pro_UP_24(CString strMsg,unsigned char *buffer,int len);
	void	        Pro_UP_25(CString strMsg,unsigned char *buffer,int len);
	void	        Pro_UP_26(CString strMsg,unsigned char *buffer,int len);
	void	        Pro_UP_27(CString strMsg,unsigned char *buffer,int len);
	void	        Pro_UP_28(CString strMsg,unsigned char *buffer,int len);
	void	        Pro_UP_29(CString strMsg,unsigned char *buffer,int len);
	void	        Pro_UP_30(CString strMsg,unsigned char *buffer,int len);
	void	        Pro_UP_31(CString strMsg,unsigned char *buffer,int len);
	void	        Pro_UP_32(CString strMsg,unsigned char *buffer,int len);
	void	        Pro_UP_33(CString strMsg,unsigned char *buffer,int len);

	

	void	        Pro_Down_1(CString strMsg,unsigned char *buffer,int len);

	//下发计费模型下行数据(尖峰平谷)
	void	        Pro_Down_1_v2(CString strMsg,unsigned char *buffer,int len);
	void	        Pro_Down_3(CString strMsg,unsigned char *buffer,int len);
	void	        Pro_Down_4(CString strMsg,unsigned char *buffer,int len);
	void	        Pro_Down_5(CString strMsg,unsigned char *buffer,int len);
	void	        Pro_Down_6(CString strMsg,unsigned char *buffer,int len);
	void	        Pro_Down_7(CString strMsg,unsigned char *buffer,int len);
	void	        Pro_Down_8(CString strMsg,unsigned char *buffer,int len);
	void	        Pro_Down_9(CString strMsg,unsigned char *buffer,int len);
	void	        Pro_Down_10(CString strMsg,unsigned char *buffer,int len);
	void	        Pro_Down_11(CString strMsg,unsigned char *buffer,int len);
	void	        Pro_Down_12(CString strMsg,unsigned char *buffer,int len);
	void	        Pro_Down_13(CString strMsg,unsigned char *buffer,int len);
	void	        Pro_Down_14(CString strMsg,unsigned char *buffer,int len);
	void	        Pro_Down_17(CString strMsg,unsigned char *buffer,int len);
	void	        Pro_Down_18(CString strMsg,unsigned char *buffer,int len);
	void	        Pro_Down_19(CString strMsg,unsigned char *buffer,int len);
	void	        Pro_Down_20(CString strMsg,unsigned char *buffer,int len);
	void	        Pro_Down_21(CString strMsg,unsigned char *buffer,int len);
	void	        Pro_Down_22(CString strMsg,unsigned char *buffer,int len);
	void	        Pro_Down_23(CString strMsg,unsigned char *buffer,int len);
	void	        Pro_Down_24(CString strMsg,unsigned char *buffer,int len);
	void	        Pro_Down_25(CString strMsg,unsigned char *buffer,int len);
	void	        Pro_Down_26(CString strMsg,unsigned char *buffer,int len);
	void	        Pro_Down_27(CString strMsg,unsigned char *buffer,int len);
	void	        Pro_Down_28(CString strMsg,unsigned char *buffer,int len);
	void	        Pro_Down_29(CString strMsg,unsigned char *buffer,int len);

	void	        Pro_Down_11_v2(CString strMsg,unsigned char *buffer,int len);
	void	        Pro_Down_13_v2(CString strMsg,unsigned char *buffer,int len);
    void	        Pro_Down_31_v2(CString strMsg,unsigned char *buffer,int len);
    void	        Pro_Down_32_v2(CString strMsg,unsigned char *buffer,int len);
    void	        Pro_Down_33_v2(CString strMsg,unsigned char *buffer,int len);
    void	        Pro_Down_41_v2(CString strMsg,unsigned char *buffer,int len);
    void	        Pro_Down_42_v2(CString strMsg,unsigned char *buffer,int len);
    void	        Pro_Down_44_v2(CString strMsg,unsigned char *buffer,int len);
    void	        Pro_Down_46_v2(CString strMsg,unsigned char *buffer,int len);
    void	        Pro_Down_102(CString strMsg,unsigned char *buffer,int len);
	void	        Pro_Down_103(CString strMsg,unsigned char *buffer,int len);
    void	        Pro_Down_141(CString strMsg,unsigned char *buffer,int len);
	void	        Pro_Down_142(CString strMsg,unsigned char *buffer,int len);
	void	        Pro_Down_143(CString strMsg,unsigned char *buffer,int len);
	void	        Pro_Down_144(CString strMsg,unsigned char *buffer,int len);
	void	        Pro_Down_146(CString strMsg,unsigned char *buffer,int len);

    //集中器桩体配置
	void	        Pro_Down_148_v4(CString strMsg,unsigned char *buffer,int len);
	//查询集中器桩体配置
	void	        Pro_Down_149_v4(CString strMsg,unsigned char *buffer,int len);
	//查询远端设备费率
	void	        Pro_Down_150_v4(CString strMsg,unsigned char *buffer,int len);
	//flash ram
	void	        Pro_Down_151_v4(CString strMsg,unsigned char *buffer,int len);
	//充电信息统计下行数据
	void	        Pro_Down_161(CString strMsg,unsigned char *buffer,int len);
	//电桩业务数据查询
	void	        Pro_Down_162(CString strMsg,unsigned char *buffer,int len);

	//查询远端设备通讯信号强度
	void	        Pro_Down_147(CString strMsg,unsigned char *buffer,int len);

	bool 	    IsValide(CString err,unsigned char * buffer,int slen,int dlen);

	void	        Pro_Down_49_v4(CString strMsg,unsigned char *buffer,int len);
	void	        Pro_Down_49_v2(CString strMsg,unsigned char *buffer,int len);
	void	        Pro_Down_152_v4(CString strMsg,unsigned char *buffer,int len);
	void	        Pro_Down_153_v4(CString strMsg,unsigned char *buffer,int len);


	CString	   Pro_UP_46_v2(CString strMsg,unsigned char *buffer,int len);
	void	       Pro_UP_2_v2(CString strMsg,unsigned char *buffer,int len);
	void	       Pro_UP_11_v2(CString strMsg,unsigned char *buffer,int len);
	void	       Pro_UP_12_v2(CString strMsg,unsigned char *buffer,int len);
	void	       Pro_UP_13_v2(CString strMsg,unsigned char *buffer,int len);
	void	       Pro_UP_31_v2(CString strMsg,unsigned char *buffer,int len);
	void	       Pro_UP_32_v2(CString strMsg,unsigned char *buffer,int len);
	void	       Pro_UP_33_v2(CString strMsg,unsigned char *buffer,int len);
    void	       Pro_UP_45_v2(CString strMsg,unsigned char *buffer,int len);
	void	       Pro_UP_43_v2(CString strMsg,unsigned char *buffer,int len);
	void	       Pro_UP_42_v2(CString strMsg,unsigned char *buffer,int len);
	void	       Pro_UP_41_v2(CString strMsg,unsigned char *buffer,int len);
	void	       Pro_UP_44_v2(CString strMsg,unsigned char *buffer,int len);
	void	       Pro_UP_47_v2(CString strMsg,unsigned char *buffer,int len);


	void	       Pro_UP_141(CString strMsg,unsigned char *buffer,int len);
	void	       Pro_UP_145(CString strMsg,unsigned char *buffer,int len);
	void	       Pro_UP_144(CString strMsg,unsigned char *buffer,int len);
	void	       Pro_UP_143(CString strMsg,unsigned char *buffer,int len);

	void	       Pro_UP_101(CString strMsg,unsigned char *buffer,int len);
	void	       Pro_UP_104(CString strMsg,unsigned char *buffer,int len);
	void	       Pro_UP_105(CString strMsg,unsigned char *buffer,int len);

	void	       Pro_UP_161(CString strMsg,unsigned char *buffer,int len);
    //
	void	       Pro_UP_162(CString strMsg,unsigned char *buffer,int len);
	//远端设备通讯信号强度应答
	void	       Pro_UP_147(CString strMsg,unsigned char *buffer,int len); 
   //集中器桩体配置应答
	void	       Pro_UP_148_v4(CString strMsg,unsigned char *buffer,int len);
	   //查询集中器桩体配置应答
	void	       Pro_UP_149_v4(CString strMsg,unsigned char *buffer,int len);
	 //远端设备费率应答
	void	       Pro_UP_150_v4(CString strMsg,unsigned char *buffer,int len);
	 //flashram
	void	       Pro_UP_151_v4(CString strMsg,unsigned char *buffer,int len);
   //开始充电下行
	void	        Pro_Down_41_v4(CString strMsg,unsigned char *buffer,int len);

	void	       Pro_UP_49_v4(CString strMsg,unsigned char *buffer,int len);

	void	       Pro_UP_152_v4(CString strMsg,unsigned char *buffer,int len);
	void	       Pro_UP_153_v4(CString strMsg,unsigned char *buffer,int len);

	CString        workStatusMsg(CString strMsg,unsigned char *buffer);
	CString        lockStatusMsg(CString strMsg,unsigned char *buffer);
	CString        linkCarMsg(CString strMsg,unsigned char *buffer);
	CString        linkBatteryMsg(CString strMsg,unsigned char *buffer);

	CString        getEpCode(CString strMsg,unsigned char *buffer);
	CString        getPhone(CString strMsg,unsigned char *buffer);
	
public:
	CProto_IEC104(CACDecode *dlg);
	virtual ~CProto_IEC104();

	void            ParaData(unsigned char *buff,int len );

	CACDecode *m_Dlg;

			
	
	

};

#endif
