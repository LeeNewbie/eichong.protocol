package com.usrlayer.service;


public abstract class ClientService {

	/**
	 * 连接应答（usrGate->phone）
	 */
	public static void sendConnect(String epCode,int epGunNo,int accountId,int ret,int errorCode,int status) {
	}

	/**
	 * 充电应答（usrGate->phone）
	 */
	public static void sendCharge(String epCode,int epGunNo,int accountId,int ret,int errorCode) {
	}

	/**
	 * 充电事件（usrGate->phone）
	 */
	public static void sendChargeEvent(String epCode,int epGunNo,int accountId,int status) {
	}

	/**
	 * 停止充电应答（usrGate->phone）
	 */
	public static void sendStopCharge(String epCode,int epGunNo,int accountId,int ret,int errorCode) {
	}

	/**
	 * 充电实时数据（usrGate->phone）
	 */
	public static void sendChargeReal(String epCode,int epGunNo,int accountId,Object extraData) {
	}

	/**
	 * 消费记录（usrGate->phone）
	 */
	public static void sendConsumeRecord(String epCode,int epGunNo,int accountId,Object extraData) {
	}
}
