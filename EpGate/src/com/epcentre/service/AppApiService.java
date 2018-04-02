package com.epcentre.service;

import io.netty.channel.Channel;

import java.math.BigDecimal;
import java.nio.ByteBuffer;
import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.epcentre.cache.EpCache;
import com.epcentre.cache.EpGunCache;
import com.epcentre.cache.UserCache;
import com.epcentre.cache.UserOrigin;
import com.epcentre.config.Global;
import com.epcentre.constant.AppManageCmd;
import com.epcentre.constant.EpConstantErrorCode;
import com.epcentre.constant.EpProtocolConstant;
import com.epcentre.constant.EventConstant;
import com.epcentre.protocol.AppServerProtocol;
import com.epcentre.protocol.UtilProtocol;
import com.epcentre.protocol.WmIce104Util;
import com.epcentre.sender.InnerApiMessageSender;
import com.epcentre.utils.NetUtils;
import com.netCore.util.ByteUtil;

/**
 * App后台服务器相关服务
 * @author 
 * 2015-3-18 下午4:59:13
 */
public class AppApiService {

	private static final Logger logger = LoggerFactory.getLogger(AppApiService.class);
	
	/**
	 * 预约充电
	 * @author 
	 * 2015-3-18
	 * @param channel
	 * @param byteBuffer
	 */
	public static void bespoke(Channel channel,ByteBuffer byteBuffer){
		
		logger.debug("bespoke api enter");
		
		
		//预约号	
		String bespNo = ByteUtil.getString(byteBuffer);
		//电桩编号
		int pkEpId = byteBuffer.getInt();
		String epCode = ByteUtil.getString(byteBuffer);
		//预约买断时间,单位分钟
		Short buyOutTime = byteBuffer.getShort();
		//预约开始时间
		long clientBespSt = byteBuffer.getLong();
		
		//续约标致（0：第一次预约，1：续约）
		int redo = byteBuffer.getInt();
		//用户ID
		int userid=byteBuffer.getInt();
		//用户账号	
		String accountNo = ByteUtil.getString(byteBuffer);
		
		//电桩枪口
		int epGunNo=byteBuffer.getInt();
		
		//电桩枪口主键
		long pkEpGunNo=byteBuffer.getLong();
		
		int orgNo = byteBuffer.getInt();
		int payMode = byteBuffer.getInt();
		
		logger.info("bespoke accept accountId:{},bespNo:{},buyOutTime:{},epCode:{},epGunNo:{}",
	    		new Object[]{userid,bespNo,buyOutTime,epCode,epGunNo});
		
	    int errorCode = EpBespokeService.apiBespoke(epCode,epGunNo,pkEpId ,bespNo,buyOutTime,clientBespSt,
				 redo, userid, accountNo, pkEpGunNo,payMode, orgNo,1,NetUtils.getClientIp(channel));
	    
		
	    if(errorCode>0)
		{
	    	logger.info("bespoke apiBespoke errorCode:{},accountId:{},bespNo:{},buyOutTime:{},epCode:{},epGunNo:{}",
	  	    		new Object[]{errorCode,userid,bespNo,buyOutTime,epCode,epGunNo});
	  	 
	    	byte[] msg = AppServerProtocol.bespokeProtocol(bespNo,(short)0,errorCode,userid,(short)redo);
			
			InnerApiMessageSender.gateSendToGame(channel, AppManageCmd.G2A_BESPOKE, (Integer)0,  msg);
			
		}
		
	}
	
	/**
	 * 取消预约 
	 * @author 
	 * 2015-3-18
	 * @param channel
	 * @param byteBuffer
	 */
	public static void cancelBespoke(Channel channel,ByteBuffer byteBuffer){
		
		logger.debug("cancelbespoke api enter");
		
		long pkBespNo = byteBuffer.getLong();
		//预约号	
		String bespNo = ByteUtil.getString(byteBuffer);
		String epCode = ByteUtil.getString(byteBuffer);
		
		int epGunNo = 1;
		
		logger.info("cancelbespoke accept bespNo:{},epCode:{},epGunNo:{}",
	    		new Object[]{bespNo,epCode,epGunNo});
		int errorCode = EpBespokeService.apiStopBespoke(pkBespNo, bespNo, epCode,epGunNo,1,NetUtils.getClientIp(channel));
		
		if(errorCode >0)
		{
			logger.info("cancelbespoke apiStopBespoke errorCode:{},bespNo:{},epCode:{},epGunNo:{}",
		    		new Object[]{errorCode,bespNo,epCode,epGunNo});
			
			byte[] msg = AppServerProtocol.cancelBespokeProtocol(bespNo,(short)0,errorCode);
			InnerApiMessageSender.gateSendToGame(channel, AppManageCmd.G2A_CANCEL_BESPOKE, (Integer)0,  msg);
			
		}
	}
	
	
	
	public static void startElectricize2(Channel channel,ByteBuffer byteBuffer){
		
		logger.debug("charge api enter");
		//电桩编号
		String epCode = ByteUtil.getString(byteBuffer);
		int epGunNo=byteBuffer.getInt();
		String bespNo= ByteUtil.getString(byteBuffer);
		int accountId=byteBuffer.getInt();
		String account = ByteUtil.getString(byteBuffer);
		short ermFlag = byteBuffer.getShort();
		
		double fronzeAmt =  byteBuffer.getDouble();

		int orgNo = byteBuffer.getInt();
		int payMode = byteBuffer.getInt();//
		
		
		int errorCode=0;
		
		//String epCode,int epGunNo,String bespNo,
		//int accountId,String account,short ermFlag,
		//double fronzeAmt ,int orgNo,int payMode,String clientIp
		
		errorCode = EpChargeService.appApiStartElectric(epCode,epGunNo,bespNo,accountId,account,
				ermFlag,fronzeAmt,orgNo,payMode,NetUtils.getClientIp(channel));
		
		
		if(errorCode>0)
		{
			logger.info("charge startElectricize2 fail errorCode:{} accountId:{},account:{},chargeStyle:{},epCode:{},epGunNo:{} to api",
					new Object[]{errorCode,accountId,account,ermFlag,epCode,epGunNo});
			
			byte[] msg =AppServerProtocol.startElectricizeProtocol(epCode,epGunNo,(short)0,errorCode);
			InnerApiMessageSender.gateSendToGame(channel, AppManageCmd.G2A_START_ELECTRICIZE, (Integer)0,  msg);
		}
	}
	
	/**
	 * 停止充电
	 * @param byteBuffer
	 */
	public static void stopElectricize(Channel channel,ByteBuffer byteBuffer){
		
		logger.debug("stopcharge api enter");
		
		//电桩编号
		String epCode = ByteUtil.getString(byteBuffer);
		int epGunNo=byteBuffer.getInt();
		int userId=byteBuffer.getInt();
		String account = ByteUtil.getString(byteBuffer);
		int orgNo = byteBuffer.getInt();
		logger.info("stopcharge accept accountId:{},epCode:{},epGunNo:{} from api ",
				new Object[]{userId,epCode,epGunNo});
		
		int errorCode = EpChargeService.appApiStopElectric(epCode,epGunNo,orgNo,userId,account,0,NetUtils.getClientIp(channel));
		if(errorCode>0)
		{
			logger.info("appApiStopElectric stopcharge apiStopElectric fail errorCode:{} accountId:{},epCode:{},epGunNo:{} to api",
					new Object[]{errorCode,userId,epCode,epGunNo});
			
			byte[] msg = AppServerProtocol.stopElectricizeProtocol(epCode,epGunNo,(short)0,errorCode);
			InnerApiMessageSender.gateSendToGame(channel, AppManageCmd.G2A_STOP_ELECTRICIZE, (Integer)0,  msg);
		}
	}
	/**
	 * 费率下发
	 * @param byteBuffer
	 */
	public static void RateCmd(Channel channel,ByteBuffer byteBuffer)
	{
		logger.info("费率下发 ..channel是：【{}】", new Object[]{channel} );
		//电桩编号
		String epCodes = ByteUtil.getString(byteBuffer);
		
		int rateId = byteBuffer.getInt();
		
		logger.info("RateCmd,rateId:{},epCodes:{}",rateId,epCodes);
		
		RateService.apiUpdateRateAction(epCodes,rateId);
	}
	
	public static void updateQRCode(Channel channel,ByteBuffer byteBuffer){
		logger.info("二维码更新 ..channel是：【{}】", new Object[]{channel} );
		//电桩编号
		String epCode = ByteUtil.getString(byteBuffer);
		
		String epGunNo1 = ByteUtil.getString(byteBuffer);
		
		int epGunNo=Integer.parseInt(epGunNo1);
		
		String qrCodeContent = ByteUtil.getString(byteBuffer);
		
		//取枪
		EpGunCache gunCache = EpGunService.getEpGunCache(epCode, epGunNo);
		
		if(gunCache == null)
			return;
		if(!EpCommClientService.enableComm(gunCache.getConcentratorId(),epCode))
			return;
		
		if(gunCache.getRealChargeInfo().getWorkingStatus() == EpProtocolConstant.EP_WORK_STATUS_UPDATE)
		{
		  return;
		  
		}	
		logger.info("updateQRCode epCode:"+epCode+",epGunNo:"+epGunNo+",qrCodeContent:"+qrCodeContent+"\n");
		
		EpCache.updateQRCode(epCode,epGunNo,qrCodeContent);
		
	}
	
	
	public static void updateHexFile(Channel channel,ByteBuffer byteBuffer){
		logger.info("upgrade channel：【{}】", new Object[]{channel} );
	    //产品ID
		try{
		int typeSpanId = byteBuffer.getInt();
		int num = byteBuffer.getInt();
		if(num==0 || typeSpanId==0)
		{
			logger.info("[handleVersion],accept API updateHexFile fail num:0 or typeSpanId:0");
	    	return;
		}
		logger.info("[handleVersion],accept API updateHexFile,typeSpanId:{},num:{}",typeSpanId,num);
		Map<String,BomListInfo> bomMap=new ConcurrentHashMap<String,BomListInfo>();
		
		for(int i=0;i<num;i++)
		{    
			BomListInfo bom=new BomListInfo();
		    
		    String hardwareNumber = ByteUtil.getString(byteBuffer);
		    String hardwareVersion = ByteUtil.getString(byteBuffer);
		    String softNumber = ByteUtil.getString(byteBuffer);
		    String softVersion = ByteUtil.getString(byteBuffer);
		    String updateFlag1 = ByteUtil.getString(byteBuffer);
		    int updateFlag = Integer.parseInt(updateFlag1);
		    String md5Value = ByteUtil.getString(byteBuffer);
		    
		    bom.setSoftNumber(softNumber);
		    bom.setSoftVersion(softVersion);
		    bom.setHardwareNumber(hardwareNumber);
		    bom.setHardwareVersion(hardwareVersion);
		    bom.setFileMd5(md5Value);
		    bom.setForceUpdate(updateFlag);
		    if(bom.splitSoftVersion()>0)
		    {
		    	logger.error("[handleVersion],accept API updateHexFile, bom softVersion error,setSoftNumber:{}softVersion:{}",softNumber,softVersion);
		    	continue;
		    }
		    if(bom.splitHardwareVersion()>0)
		    {
		    	logger.error("[handleVersion],accept API updateHexFile,bom hardVersion error,setHardNumber:{}HardVersion:{}",hardwareNumber,hardwareVersion);
		    	continue;
		    }
		    
		    String key=hardwareNumber+hardwareVersion;
		    bomMap.put(key,bom);
		   
		  }
		  EqVersionService.AddBomList(typeSpanId,bomMap);
		  EqVersionService.queryAllEpByTypeSpanID(typeSpanId,bomMap);
		  EqVersionService.queryAllStaionByTypeSpanID(typeSpanId,bomMap);
		  
		}catch (Exception e) {
			logger.error("[handleVersion],accept API updateHexFile exception,e.getMessage():{}",e.getMessage());
			
		}
		
	}
	
	
	/**
	 * 私有电桩运营时间
	 * @param byteBuffer
	 */
	public static void privateElectricize(Channel channel,ByteBuffer byteBuffer){
		
		/*String businessId = ByteUtil.getString(byteBuffer);//App业务处理Id
		String epCode = ByteUtil.getString(byteBuffer);//获得电桩编号
		short pileStatus = byteBuffer.getShort();//电桩离线标识（0：离线，1：上线）
		String startTime = ByteUtil.getString(byteBuffer);//运营开始时间
		String stopTime = ByteUtil.getString(byteBuffer);//运营结束时间
*/	}
	
	public static void changeEpGate(Channel channel,ByteBuffer byteBuffer)
	{
		logger.info("移桩 ..channel是：【{}】", new Object[]{channel} );
		//电桩编号
		String epCodes = ByteUtil.getString(byteBuffer);
		String newGateIp = ByteUtil.getString(byteBuffer);
		int Port=byteBuffer.getInt();
		
		logger.info("changeEpGate params---"+
				"epCodes:"+epCodes+
				",newGateIp:"+newGateIp+
				",Port:"+Port
		);
	}
	
	public static void flashLed(Channel channel,ByteBuffer byteBuffer){
		
		logger.info("flashLed 【{}】", new Object[]{channel} );
		//电桩编号
		String epCode = ByteUtil.getString(byteBuffer);
		
		short type = byteBuffer.getShort();
		int continueTime = byteBuffer.getInt();//分钟
		int  accountId = byteBuffer.getInt();
		float lng = byteBuffer.getFloat();
		float lag = byteBuffer.getFloat();
		
		if(continueTime==0)
		{
			continueTime = 2;
		}
		
		//取枪
		int errorCode = EpService.doNearCallEpAction(epCode,type,continueTime*60, accountId,lng,lag);
		
		logger.info("flashLed,errorCode:{}",errorCode);
	}
	public static void dropCarPlaceLock(Channel channel,ByteBuffer byteBuffer)
	{
		logger.info("移桩 ..channel是：【{}】", new Object[]{channel} );
		//电桩编号
		String epCode = ByteUtil.getString(byteBuffer);
		int  epGunNo = byteBuffer.getShort();
		String carPlaceNo = ByteUtil.getString(byteBuffer);//车位号
		int  accountId = byteBuffer.getInt();
		float lng = byteBuffer.getFloat();
		float lag = byteBuffer.getFloat();
		
		
		logger.info("dropCarPlaceLock params---"+
				"epCodes:"+epCode+
				",epGunNo:"+epGunNo
		);
		
		//取枪
		int errorCode = EpGunService.dropCarPlaceLockAction(epCode, epGunNo, accountId,lng,lag);
		
		logger.info("dropCarPlaceLock,errorCode:{}",errorCode);
	}
	
	public static void sendEvent(String Identity,short protocolNum, int senderId, byte[] bb)
	{
		logger.debug("sendEvent,Identity:{}",Identity);
		if(Identity!=null && Identity.length()>0)
		{
			Channel channel = AppClientService.getAppChannel(Identity);
			if(channel!=null)
			{
				logger.info("sendEvent,channel:{},protocolNum:{},senderId:{}",
						new Object[]{channel,protocolNum,senderId});
				InnerApiMessageSender.gateSendToGame(channel,protocolNum, senderId, bb);
			}
			else
			{
				logger.error("sendEvent,not found channel of Identity:{}",Identity);
			}
		}
		else
		{
			logger.info("sendEvent,broadcastMsg,protocolNum:{}",protocolNum);
			AppClientService.broadcastMsg(protocolNum,senderId, bb);
		}
		
	}
	public static void sendStopChargeByPhoneDisconnect(String epCode,int epGunNo,int userId,int ret,int casuse,int chargeTime)
	{
		logger.debug("sendstopchargeByPhoneDisconnect,epCode:{},epGunNo:{},userId:{},casuse:{},chargeTime:{}",new Object[]{epCode,epGunNo,userId,casuse,chargeTime});
		
		byte[] msg = AppServerProtocol.stopElectricizeProtocol(epCode, epGunNo, (short)ret,casuse);
		sendEvent("",AppManageCmd.G2A_STOP_ELECTRICIZE, (Integer) 0, msg);
			
		byte[] msgEvent = AppServerProtocol.stopElectricizeEventProtocol(chargeTime, (short)ret, casuse, userId);
		sendEvent("",AppManageCmd.G2A_STOP_ELECTRICIZE_EVENT, (Integer) 0, msgEvent);
		
	}
	@SuppressWarnings("unchecked")
	public static void onEvent(int type,UserOrigin userOrigin,int respRet,int cause,Object srcParams,Object extraData)
	{
		try{
			logger.info("api onEvent,type:{},userOrigin:{}",type,userOrigin);
		if(extraData==null)
		{
			logger.info("AppApiService onEvent error,extraData==null");
			return;
		}
		String actionIdentity="";
		if(userOrigin!=null)
			actionIdentity = userOrigin.getCmdChIdentity();
		switch(type)
		{
			case EventConstant.EVENT_BESPOKE:
			{
				
				Map<String, Object> paramsMap = (Map<String, Object>)extraData;
				logger.debug("AppApiService onEvent!paramsMap:{}",paramsMap);
				
				int userId  = (int)paramsMap.get("userId");
				int redo  = (int)paramsMap.get("redo");
				String bespNo  = (String)paramsMap.get("bespNo");
				
				byte[] msg = AppServerProtocol.bespokeProtocol(bespNo, (short)respRet, cause, userId, (short)redo);
				
				logger.info("EventConstant.EVENT_BESPOKE,bespNo:{}",bespNo);
				sendEvent(actionIdentity,AppManageCmd.G2A_BESPOKE, (Integer) 0, msg);
				
				
			}
			break;
			case EventConstant.EVENT_CANNEL_BESPOKE:
			{
				Map<String, Object> paramsMap = (Map<String, Object>)extraData;
				logger.debug("AppApiService onEvent!paramsMap:{}",paramsMap);
				String bespokeNo  = (String)paramsMap.get("bespNo");
				
				byte[] msg = AppServerProtocol.cancelBespokeProtocol(bespokeNo, (short)respRet, cause);
				logger.info("EVENT_CANNEL_BESPOKE respRet:{}",respRet);
				sendEvent(actionIdentity,AppManageCmd.G2A_CANCEL_BESPOKE, (Integer) 0, msg);
					
					
				logger.info("respRet:{}",respRet);
				if(respRet ==1)
				{
					double amt = (double)paramsMap.get("amt");
					double userRemainAmt = (double)paramsMap.get("remainAmt");
					String account = (String)paramsMap.get("account");
					
					logger.info("amt:{},account:{}",amt,account);
					
					byte[] bespEventData = AppServerProtocol.bespokeRespEvent(amt, userRemainAmt, account);
					
					sendEvent(actionIdentity,AppManageCmd.G2A_BESP_EVENT, (Integer) 0, bespEventData);
				
				}
			}
			break;
			case EventConstant.EVENT_CHARGE:
			{
				Map<String, Object> paramsMap = (Map<String, Object>)extraData;
				logger.debug("AppApiService onEvent!paramsMap:{}",paramsMap);
				
				String epCode = (String)paramsMap.get("epcode");
				int epGunNo = (int)paramsMap.get("epgunno");
				
				logger.info("AppServerProtocol.startElectricizeProtocol,epCode:{},epGunNo:{}",epCode,epGunNo);
				
				byte[] msg = AppServerProtocol.startElectricizeProtocol(epCode,epGunNo,(short)respRet,cause);
				sendEvent(actionIdentity,AppManageCmd.G2A_START_ELECTRICIZE, (Integer) 0, msg);
				
			}
			break;
			case EventConstant.EVENT_STOP_CHARGE:
			{
				Map<String, Object> paramsMap = (Map<String, Object>)extraData;
				logger.debug("AppApiService onEvent!paramsMap:{}",paramsMap);
				String epCode = (String)paramsMap.get("epcode");
				int epGunNo = (int)paramsMap.get("epgunno");
				
				logger.info("AppServerProtocol.stopElectricizeProtocol,epCode:{},epGunNo:{}",epCode,epGunNo);
				
				byte[] msg = AppServerProtocol.stopElectricizeProtocol(epCode, epGunNo, (short)respRet,cause);
				sendEvent(actionIdentity,AppManageCmd.G2A_STOP_ELECTRICIZE, (Integer) 0, msg);
			}
			break;
			
			case EventConstant.EVENT_CONSUME_RECORD:
			{
				Map<String, Object> paramsMap = (Map<String, Object>)extraData;
				logger.debug("AppApiService onEvent!paramsMap:{}",paramsMap);
				int pkUserId  = (int)paramsMap.get("userId");
				String userPhone  = (String)paramsMap.get("account");
				long st = (long)paramsMap.get("st");
				long et = (long)paramsMap.get("et");
				
				int totalAmt = (int)paramsMap.get("totalAmt");
				
				short chargeTime = (short)((et-st)/60);
				int couPonAmt = (int)paramsMap.get("realCouPonAmt");
				
				byte[] msg = AppServerProtocol.consumeRecordProtocol(pkUserId, userPhone, totalAmt/100, chargeTime, (short)respRet, cause,couPonAmt/100);
				
				sendEvent(actionIdentity,AppManageCmd.G2A_ELECTRIC_CONSUME_RECORD, (Integer) 0, msg);
			}
			break;
			case EventConstant.EVENT_START_CHARGE_EVENT:
			{
				Map<String, Object> paramsMap = (Map<String, Object>)extraData;
				logger.debug("AppApiService onEvent EVENT_START_CHARGE_EVENT!paramsMap:{}",paramsMap);
				
				String Account = (String)paramsMap.get("account");
				int userId = (int)paramsMap.get("userId");
				
				logger.info("AppServerProtocol.startElectricizeEventProtocol,Account:{}",Account);
				
				byte[] msg = AppServerProtocol.startElectricizeEventProtocol(userId, Account);
				sendEvent(actionIdentity,AppManageCmd.G2A_START_ELECTRICIZE_EVENT, (Integer) 0, msg);
	
			}
		   break;
		default:
			logger.error("AppApiService onEvent!invalid type:{}",type);
			break;
		  }
	   }
		catch (Exception e) 
		{
			logger.error("onEvent exception,e.getMessage():{}",e.getMessage());
			
			return ;
		}
	}
	
	public static void  concentratorConfig(Channel channel,ByteBuffer byteBuffer){
		logger.info("concentratorConfig channel：【{}】", new Object[]{channel} );
	  
			int concentratorId = byteBuffer.getInt();
			
			EpConcentratorService.concentratorConfig(concentratorId);
		
	}
}
