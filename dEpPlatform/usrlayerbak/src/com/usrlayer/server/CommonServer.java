package com.usrlayer.server;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

public class CommonServer {
	private static final Logger logger = LoggerFactory.getLogger(CommonServer.class);

	/**
	 * 开始充电
	 * @param epCode
	 * @param epGunNo
	 * @param accountId
	 * @param account
	 * @param bespNo
	 * @param ermFlag
	 * @param appClientIp
	 * @param frozenAmt
	 * @param source,但来自于爱充的用户需要收费，来自于其他合作伙伴有可能不冻结钱.只记录充电和消费记录
	 * @return
	 */
	public static int startChange(String epCode,int epGunNo,String account,String bespNo,
			short startChargeStyle,int pkUserCard,
			int frozenAmt,int payMode,
			int orgNo,int fromSource,String actionIdentity)
	{
		return 0;
	}

	/**
	 * 停止充电
	 * @param epCode
	 * @param epGunNo
	 * @param userId
	 * @param source
	 * @param apiClientIp
	 * @return
	 */
	public static int stopChange(String epCode,int epGunNo,int userId,int source,String apiClientIp)
	{
		return 0;
	}

	/**
	 * 开始充电反馈
	 * @param epCode
	 * @param epGunNo
	 * @param accountId
	 * @param ret
	 * @param errorCode
	 */
	public static void startChangeBack(String epCode,int epGunNo,int accountId,int ret,int errorCode) {
		
	}

	/**
	 * 停止充电反馈
	 * @param epCode
	 * @param epGunNo
	 * @param accountId
	 * @param ret
	 * @param errorCode
	 */
	public static void stopChangeBack(String epCode,int epGunNo,int accountId,int ret,int errorCode) {
		
	}

	/**
	 * 实时数据
	 * @param epCode
	 * @param epGunNo
	 * @param accountId
	 * @param extraData
	 */
	public static void chargeReal(String epCode,int epGunNo,int accountId,Object extraData) {
		
	}

	/**
	 * 消费记录
	 * @param epCode
	 * @param epGunNo
	 * @param accountId
	 * @param extraData
	 */
	public static void consumeRecord(String epCode,int epGunNo,int accountId,Object extraData) {
		
	}
}
