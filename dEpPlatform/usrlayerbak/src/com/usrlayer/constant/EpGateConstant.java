package com.usrlayer.constant;

public class EpGateConstant {

	public static final byte  HEAD_FLAG1 = 0x45;
	public static final byte  HEAD_FLAG2 = 0x43;
	
	public static final int EP_LOGIN= 101;//网关登录程序
	public static final int EP_ACK= 102;//102	ACK响应
	public static final int EP_HEART= 103;//103	心跳
	public static final int EP_ONLINE= 202;//202	电桩在线
	public static final int EP_PHONE_ONLINE= 203;//203	手机在线
	public static final int EP_PHONE_CONNECT_INIT= 1001;//1001	手机连接初始化(带部分充电逻辑)
	public static final int EP_CHARGE= 1002;//1002	充电
	public static final int EP_CHARGE_EVENT= 1003;//1003	充电事件
	public static final int EP_STOP_CHARGE= 1004;//1004	停止充电
	public static final int EP_CHARGE_REAL= 1005;//1005	充电实时数据
	public static final int EP_CONSUME_RECORD= 1006;//1006	消费记录
	public static final int EP_RE_INSERT_GUN= 1100;//1100	重新插枪提示
	public static final int EP_YXYC= 2001;//2001	部分遥信遥测值

	// 第三方服务
	public static final int EP_THIRD_DATA = 3001;
	public static final int EP_THIRD_RECORD = 3005;

}
