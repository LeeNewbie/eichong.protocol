package com.epcentre.constant.protocol;

/**
 * 电桩客户端给电桩服务端发送消息的协议号
 */
public class C2S {
    /***********************************************************************************************************************************************/
    /**
     * 电桩--登陆
     */
    public static final short P_101_LOGIN = 101;
    /**
     * 电桩--
     */
    public static final short P_102 = 102;

    /***********************************************************************************************************************************************/
    /**
     * 创建角色
     */
    public static final short P_201_CREATE_PLAYER = 201;
    /**
     * 电桩--查看其他电桩信息
     */
    public static final short P_202_QUERY_OTHER_PLAYER_INFO = 202;

    /***********************************************************************************************************************************************/
    /**
     * 系统--测试命令
     */
    public static final short P_10000_GAME_TEST = 10000;
    /**
     * 系统--获取系统时间
     */
    public static final short P_10001_GET_TIME = 10001;
    /**
     * 系统--获取CFG版本号
     */
    public static final short P_10002_GET_CFG = 10002;
    /**
     */
    public static final short P_10003_CODE = 10003;
}
