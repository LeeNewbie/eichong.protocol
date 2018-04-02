package com.epcentre.test;

import java.io.IOException;
import java.text.SimpleDateFormat;
import java.util.Date;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.epcentre.cache.ChargeCache;
import com.epcentre.cache.ElectricPileCache;
import com.epcentre.cache.EpCommClient;
import com.epcentre.protocol.ChargeCmdResp;
import com.epcentre.protocol.ChargeEvent;
import com.epcentre.protocol.EpBespResp;
import com.epcentre.protocol.EpCancelBespResp;
import com.epcentre.protocol.NoCardConsumeRecord;
import com.epcentre.protocol.WmIce104Util;
import com.epcentre.service.EpBespokeService;
import com.epcentre.service.EpChargeService;
import com.epcentre.service.EpService;
import com.epcentre.utils.DateUtil;
import com.epcentre.utils.StringUtil;

public class ImitateEpService {
	
	private static final Logger logger = LoggerFactory
			.getLogger(ImitateEpService.class);
	
	private static boolean startImitate = false;
	
	public static boolean IsStart()
	{
		return startImitate;
	}
	public static void init()
	{
		startImitate =true;
		EpService.imitateInitDiscreteEp(3, "3301021010000008");	
	}
    public static void ImitateBespoke(String epCode, int epGunNo,int nRedo,String OrderNo)
     {
	    EpBespResp bespResp = new EpBespResp(epCode,epGunNo,nRedo,OrderNo,1,0);
	    byte[] cmdTimes = WmIce104Util.timeToByte();

	    EpBespokeService.handleEpBespRet(null,bespResp,cmdTimes);
	     
    }
    
	public static void ImitateCancelBespoke(String epCode, int epGunNo,String bespokeNo)
	{
		byte successFlag = 1;
		// {{电桩没对时，以服务器时间为准
		java.util.Date dtEt = new Date();
		long et = dtEt.getTime() / 1000;
		// }}
		
		EpCancelBespResp cancelBespResp = new EpCancelBespResp(epCode, epGunNo,
				bespokeNo, (short)1, successFlag, et,0);
		  byte[] cmdTimes = WmIce104Util.timeToByte();

		EpBespokeService.onEpCancelBespRet(null,cancelBespResp,cmdTimes);
	}
	
	
	public static void startCharge(String epCode, int epGunNo,String Account)
	{
		ChargeCmdResp chargeCmdResp= new ChargeCmdResp(epCode, epGunNo, 1, (short)0);
		
		 byte[] cmdTimes = WmIce104Util.timeToByte();
		 
		EpChargeService.handEpStartChargeResp(null, chargeCmdResp,cmdTimes);
		
		
		ImitateChargeEvent(epCode,epGunNo,Account);
	}
	
	private static int chargeIndex=0;
	public static void ImitateChargeEvent(String epCode, int epGunNo,String Account)
	{
		chargeIndex = chargeIndex+1;
		SimpleDateFormat dateFormat = new SimpleDateFormat("yyMMddHH");

    
    	String pattern="00000000";
    	java.text.DecimalFormat df = new java.text.DecimalFormat(pattern);
    	
    	Date now = new Date();  
		// 2 交易流水号 BCD码 10Byte 16位交易代码
		String SerialNo = epCode + dateFormat.format( now ) + df.format(chargeIndex);
		// 3 表低示数 精确到小数点后两位，单位度
		int meterNum = 10000;
		// 4 预约号 BCD码 6Byte 有预约号填预约号；无预约号都为0x00
		String OrderNo = StringUtil.repeat("0", 12);
		// 5 充电开始时间 CP56Time2a
		//byte[] bStartTimes = StreamUtil.readCP56Time2a(in);
		long st = DateUtil.getCurrentSeconds();//WmIce104Util.getP56Time2aTime(bStartTimes);
		// 6 充电枪编号 BIN码 1Byte
		//int epGunNo = StreamUtil.read(in);
		// 7 充电方式 BIN码 1Byte 1:电量
		// 2:金额
		// 3:时间
		// 4:自动充满
		byte b_cd_style = 1;
		// 8 充满电剩余时间 BIN码 4Byte 精确到秒数

		int remainSeconds = 0;

		//String Account = StreamUtil.readBCDWithLength(in, 6);
		// 10 预充电金额
		int momeny = 0;

		byte offstates = 1;

		//byte flag = StreamUtil.read(in);

		byte successflag = 1;
		
		
		ChargeEvent chargeEvent = 
				new ChargeEvent(epCode,epGunNo,SerialNo,meterNum,(int)st,remainSeconds,successflag,0);

		
		int commVer=0;
		ElectricPileCache epClient = EpService.getEpByCode(epCode);
		if(epClient!=null)
		{
			EpCommClient commClient = (EpCommClient)epClient.getEpNetObject();
		    if(commClient!=null)
		    {
		    	commVer = commClient.getCommVersion();
		    }
		}
		byte[] cmdTimes = WmIce104Util.timeToByte();
		
		    EpChargeService.handleStartElectricizeEventV3(
				null, chargeEvent,cmdTimes);
		
	}
	
	
	public static void ImitateStopCharge(String epCode, int epGunNo,String account,ChargeCache chargeCacheObj)
	{
		ChargeCmdResp chargeCmdResp= new ChargeCmdResp(epCode, epGunNo, 1, (short)0);
		
		byte[] cmdTimes = WmIce104Util.timeToByte();
		EpChargeService.handEpStartChargeResp(null, chargeCmdResp,cmdTimes);
		
		ImitateChargeConsumeRecord(epCode,epGunNo,account,chargeCacheObj);
	}
	
	
    public static void ImitateChargeConsumeRecord(String epCode, int epGunNo,String account,ChargeCache chargeCacheObj)
	{
		NoCardConsumeRecord consumeRecord = new NoCardConsumeRecord();
		
		consumeRecord.setEpCode(epCode);
		consumeRecord.setEpGunNo(epGunNo);

		// 2 交易流水号 BCD码 10Byte 16位交易代码
	     consumeRecord.setSerialNo(chargeCacheObj.getChargeSerialNo());
		// 3 用户编号 BCD码 8Byte 16位设备编码
	     consumeRecord.setAccountType(1);
		consumeRecord.setEpUserAccount(account);
//		// 4 离线交易类型 BIN码 1Byte 0:
		consumeRecord.setTransType(0);
//		// 5 开始时间 BIN码 7Byte CP56Time2a
		consumeRecord.setStartTime(chargeCacheObj.getSt());
		// 6 结束时间 BIN码 7Byte CP56Time2a
		consumeRecord.setEndTime(chargeCacheObj.getEt());		
//		// 7 尖度量
		consumeRecord.setjDl(2);
//		// 8 尖金额
		consumeRecord.setjAmt(2);
//		
//		// 9 峰度量
		consumeRecord.setfDl(2);
//		// 10 峰金额
		consumeRecord.setfAmt(2);
//		
//		// 11平度量
    	consumeRecord.setpDl(2);
//		// 12 平金额
		consumeRecord.setpAmt(2);
//		
//		// 13谷度量
		consumeRecord.setgDl(2);
//		// 14 谷金额
		consumeRecord.setgAmt(2);
//		
//		// 15 谷金额
		consumeRecord.setTotalDl(2);
//		
//		// 16谷度量
		consumeRecord.setTotalAmt(2);
	
//		// 17谷度量
		consumeRecord.setStartMeterNum(2);
//		// 18 谷金额
		consumeRecord.setEndMeterNum(2);
//		//19停止充电原因
		consumeRecord.setStopCause(chargeCacheObj.getStopCause());
	
//		
		byte[] cmdTimes = WmIce104Util.timeToByte();
		try {
			EpChargeService.handleConsumeRecord(null,consumeRecord,cmdTimes);
		} catch (IOException e) {
			e.printStackTrace();
		}
		
     }
    
  
	

	
}
