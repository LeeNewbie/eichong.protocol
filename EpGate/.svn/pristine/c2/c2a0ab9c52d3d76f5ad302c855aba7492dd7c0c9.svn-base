package com.epcentre.epconsumer;

import java.math.BigDecimal;
import java.text.MessageFormat;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.TimeUnit;

import net.sf.json.JSONObject;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.epcentre.cache.EpPartnerCache;
import com.epcentre.cache.UserOrigin;
import com.epcentre.config.Global;
import com.epcentre.constant.EventConstant;
import com.epcentre.service.ChargingInfo;
import com.epcentre.service.EpGunService;
import com.epcentre.task.ChinaMobileTask;
import com.epcentre.utils.DateUtil;
import com.netCore.core.pool.TaskPoolFactory;

public class ChinaMobileService {
	
	 
	private static final Logger logger = LoggerFactory.getLogger(ChinaMobileService.class);
	
	
	public static String httpsChargingUrl = "https://dev03.virtueit.net/services-ext/charging/cp";
	public static String httpsChargingsUrl = httpsChargingUrl+"l";
	public static String httpsUrlAccess = "https://dev03.virtueit.net/services-ext/chargingBill/access";
	public static String httpsConsumeRecordUrl = "https://dev03.virtueit.net/services-ext/chargingBill/upload";
	
	
	public static String realRefresh_token="";
	public static String realAccess_token="";
	
	public static String payRefresh_token="";
	public static String payAccess_token="";
	
	public static long nextTime = 28800;
	
	public static void startUpdateToken(int tokenType,long initDelay) {
		
		ChinaMobileTask fetchKeyTask = new ChinaMobileTask(tokenType);
		TaskPoolFactory.scheduleAtFixedRate("CM_FETCH_TOKEN_TASK", fetchKeyTask, initDelay, TimeUnit.SECONDS);
		
	}
	public static void getToken(String scope)
	{
		String httpsUrl = "https://dev03.virtueit.net/oauth2/token";
		
	    String formStr = "grant_type=password&username=CARDAN/wanma&password=DP3AdON7V6XvEIT6WkX4&scope="+scope;
	           
	    String content = HttpsPost.GetAuthToken(httpsUrl, formStr);
	    
        int pos=content.indexOf('{');
        if(pos==0)
        {
            JSONObject jb = JSONObject.fromObject(content.substring(pos));
   		 
   		 	if(jb.has("refresh_token"))
   		 	{
   		 		if(scope.compareTo("charging")==0)
   		 			realRefresh_token = jb.getString("refresh_token");
   		 		else
   		 			payRefresh_token= jb.getString("refresh_token");
   		 			
   		 	}
   		 	if(jb.has("access_token"))
   		 	{
   		 		if(scope.compareTo("charging")==0)
   		 			realAccess_token = jb.getString("access_token");
		 		else
		 			payAccess_token= jb.getString("access_token");
   		 	}
   		 	if(jb.has("expires_in"))
   		 	{
   		 		nextTime = jb.getInt("expires_in");
   		 	}
   		 	else
   		 	{
   		 		nextTime = 28800;
   		 	}
        }
	}

	public static void init(EpPartnerCache partner)
	{
		fetchToken(1);
		fetchToken(2);
	}
	
	public static void fetchToken(int tokenType)
	{
		if(tokenType ==1)
		{
			getToken("charging");
		}
		else
		{
			getToken("chargingBill");
		}
		
		logger.debug("ChinaMobileService nextTime:{}",nextTime);
		
		startUpdateToken(tokenType,nextTime);
	}
	
	public static Map<String, Object> do_real_data(String epCode,ChargingInfo chargingInfo)
	{	
		BigDecimal bdValue= new BigDecimal(0.0);
		
		Map<String, Object> resp= new ConcurrentHashMap<String, Object>();
		//"{\"pileCode\":\"1003\",
		resp.put("pileCode", epCode);
		
		//\"power\":32.1, 
		resp.put("power", 32.1);
		//\"pileState\":\"3\", 
		resp.put("pileState", chargingInfo.getWorkStatus());
		//\"switchSignal\":\"2\", 
		resp.put("switchSignal", "2");
		//\"electricity\":88.2, 
		bdValue = bdValue.add(new BigDecimal(chargingInfo.getChargeMeterNum()).multiply(Global.Dec2));
    	bdValue = bdValue.setScale(1,BigDecimal.ROUND_HALF_UP);
		resp.put("electricity", bdValue.doubleValue());
		//\"voltage\":36,
		bdValue = bdValue.add(new BigDecimal(chargingInfo.getOutVol()).multiply(Global.Dec1));
    	bdValue = bdValue.setScale(1,BigDecimal.ROUND_HALF_UP);
		
		//SingleInfo info = pointMap.get(key);
    	resp.put("voltage", 36);
		resp.put("voltage", bdValue.doubleValue());
		//\"temperature\":77.6, 
		resp.put("temperature", 77.6);
		//\"pileFault\":\"0\", 
		//resp.put("pileFault", "0");
		//\"time\":\"2015-12-21 14:18:19\",
		SimpleDateFormat dateFormat = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
		
		resp.put("time", dateFormat.format(new Date()));
		
		//\"quantity\":67, 
		resp.put("quantity", 67);
		//\"expenses\":2380.11}";
		bdValue =bdValue.add(new BigDecimal(chargingInfo.getChargeAmt()).multiply(Global.Dec1));
    	bdValue = bdValue.setScale(1,BigDecimal.ROUND_HALF_UP);
		
		resp.put("expenses", bdValue.doubleValue());
		
		return resp;
	}
	public static Map<String, Object> do_access(String epCode,int epGunNo, String cardId,String cardNumber)
	{
		Map<String, Object> reqMap= new ConcurrentHashMap<String, Object>();
		reqMap.put("cardId", cardId);
		reqMap.put("cardNumber", cardNumber);
		reqMap.put("chargingTime", DateUtil.getCurrentSeconds());
		reqMap.put("chargingPileNumber",epCode);
		
		return reqMap;
		
		
	}
	public static Map<String, Object> do_consume_record(String epCode,String serialNumber,String cardId, String cardNumber,
			double chargStartEnergy,double chargEndEnergy,long chargBeginTime,long chargEndTime,double chargFee,double chargAmount)
	{
		Map<String, Object> reqMap= new ConcurrentHashMap<String, Object>();
		
		reqMap.put("serialNumber", serialNumber);
		reqMap.put("cardId", "");
		reqMap.put("cardNumber", cardNumber);
		reqMap.put("chargPileNumber", epCode);
		reqMap.put("chargStartEnergy", chargStartEnergy);
		reqMap.put("chargEndEnergy", chargEndEnergy);
		reqMap.put("chargBeginTime", chargBeginTime);
		reqMap.put("chargEndTime", chargEndTime);
		reqMap.put("chargFee", chargFee);
		reqMap.put("chargAmount", chargAmount);
		
		return reqMap;
	}
	
	public static int handleAuthReslt(String content)
	{
		 if(content == null)
			 return 1;
        // System.out.print(content+"\n");
         int pos=content.indexOf('{');
     
         int errorCode=1;
         
         if(pos!=0)
         {
        	 return 4;
         }
        JSONObject jb = JSONObject.fromObject(content.substring(pos));
        
       
        boolean success= false;
        boolean enableCharge = false;
	 	
	 	
	 	if(jb.has("success"))
	 	{
	 		success =  jb.getBoolean("success");
	 	}
	 	if(jb.has("data"))
	 	{
	 		enableCharge = jb.getBoolean("data");
	 	}
	 	
	 	if(success==true )
	 	{
	 		if(enableCharge == true)
	 			errorCode =0;
	 		else
	 			errorCode= 6;
	 	}
	 	else
	 	{
	 		errorCode= 5;
	 	}
	 	
	 	return errorCode;
	}
	public static int handleConsumeRecordReslt(String content)
	{
		if(content==null)
			return 1;
		 logger.debug("content:{}",content);
	      
         int pos=content.indexOf('{');
         
         
         if(pos==0)
         {
            JSONObject jb = JSONObject.fromObject(content.substring(pos));
            boolean success= false;
   		 	if(jb.has("success"))
   		 	{
   		 		success =  jb.getBoolean("success");
   		 	}
   		 	return  0;
         }
         else
         {
        	 return 3;
         }
	}

	public static void onEvent(int type,UserOrigin userOrigin,int ret,int cause,Object srcParams, Object extraData)
	{
		switch(type)
		{
			case EventConstant.EVENT_CONSUME_RECORD:
			{
				Map<String, Object> consumeRecordMap = (Map<String, Object>)extraData;
				
				String chargeOrder = (String)consumeRecordMap.get("orderid");
				long lst= (long)consumeRecordMap.get("st");
				long let= (long)consumeRecordMap.get("et");
				
				
				int st = (int)lst;
				int et = (int)let;
			
				int totalMeterNum = (int)consumeRecordMap.get("totalMeterNum");
				int totalAmt = (int)consumeRecordMap.get("totalAmt");
				int serviceAmt = (int)consumeRecordMap.get("serviceAmt");
				int pkEpId = (int)consumeRecordMap.get("pkEpId");
				String epCode = (String)consumeRecordMap.get("epCode");
				String serialNo = (String)consumeRecordMap.get("serialNo");
				String cardNumber = (String)consumeRecordMap.get("account");
				int chargStartEnergy = (int)consumeRecordMap.get("chargStartEnergy");
				int chargEndEnergy = (int)consumeRecordMap.get("chargEndEnergy");
				
				BigDecimal bdStartEnergy = new BigDecimal(0.0);
				bdStartEnergy =bdStartEnergy.add(new BigDecimal(chargStartEnergy).multiply(Global.Dec1));
				bdStartEnergy = bdStartEnergy.setScale(1,BigDecimal.ROUND_HALF_UP);
				
				BigDecimal bdEndEnergy = new BigDecimal(0.0);
				bdEndEnergy = bdEndEnergy.add(new BigDecimal(chargEndEnergy).multiply(Global.Dec1));
				bdEndEnergy = bdEndEnergy.setScale(1,BigDecimal.ROUND_HALF_UP);
				
				BigDecimal bdChargFee = new BigDecimal(0.0);
				bdChargFee = bdChargFee.add(new BigDecimal(totalAmt).multiply(Global.Dec1));
				bdChargFee = bdChargFee.setScale(1,BigDecimal.ROUND_HALF_UP);
				
				BigDecimal bdChargAmount = new BigDecimal(0.0);
				bdChargAmount = bdChargAmount.add(new BigDecimal(totalMeterNum).multiply(Global.Dec2));
				bdChargAmount = bdChargAmount.setScale(1,BigDecimal.ROUND_HALF_UP);
				
				Map<String ,Object> reqMap = do_consume_record(epCode,serialNo,"",cardNumber,
						bdStartEnergy.doubleValue(),
						bdEndEnergy.doubleValue(),
						lst*1000,let*1000,
						bdChargFee.doubleValue(),
						bdChargAmount.doubleValue());
				
				JSONObject jsonObject = JSONObject.fromObject(reqMap);
				String data= jsonObject.toString();
				
				String content = HttpsPost.PostPileInfo(httpsConsumeRecordUrl, data, payAccess_token);
				
				int errorCode = handleConsumeRecordReslt(content);
				
				String msg=MessageFormat.format("EventConstant.EVENT_CONSUME_RECORD,code:{0},serialNo:{1},ChargFee:{2},totalMeterNum:{3},errorCode:{4}",
						epCode,serialNo,bdChargFee.doubleValue(),bdChargAmount.doubleValue(),errorCode);
				
				logger.info(msg);
			}
			break;
			case EventConstant.EVENT_CARD_AUTH:
			{
				Map<String ,Object> respMap = (Map<String ,Object>)extraData;
				String epCode = (String)respMap.get("epcode");
				int epGunNo = (int)respMap.get("epgunno");
				String innerno = (String)respMap.get("innerno");
				String outno = (String)respMap.get("outno");
				byte[] cmdTimes =(byte[]) respMap.get("cmdTimes");
				Map<String ,Object> reqMap = do_access(epCode,epGunNo,innerno,outno);
				
				JSONObject jsonObject = JSONObject.fromObject(reqMap);
				String data= jsonObject.toString();
				String content = HttpsPost.PostPileInfo(httpsUrlAccess, data, payAccess_token);
	
				String msg = MessageFormat.format("EventConstant.EVENT_CARD_AUTH,epCode:{0},gunno:{1},innerno:{2},outno:{3}",
						epCode,epGunNo,innerno,outno);
				logger.debug(msg);
				 
				 int result=1;
				 
				 int errorCode =handleAuthReslt(content);
				 if(errorCode>0)
				 {
					 result=0;
				 }
				 EpGunService.onAuthResp(epCode, epGunNo, innerno,outno,result, errorCode,cmdTimes);
		          
			}
				
			break;
			case EventConstant.EVENT_REAL_CHARGING:
			{
				if(srcParams ==null)
				{
					logger.debug("ChinaMobileService onEvent error,srcParams==null");
					return;
				}
				if(extraData==null)
				{
					logger.debug("ChinaMobileService onEvent error,extraData==null");
					return ;
				}
				Map<String, Object> respMap = (Map<String, Object>)srcParams;
			    String epCode = (String)respMap.get("epcode");
			     int epGunNo = (int)respMap.get("epgunno");
				ChargingInfo chargingInfo = (ChargingInfo)extraData;
				
				Map<String, Object> resp= do_real_data(epCode,chargingInfo);
				
				JSONObject jsonObject = JSONObject.fromObject(resp);
				String data= jsonObject.toString();
				String result = HttpsPost.PostPileInfo(httpsChargingUrl, data, realAccess_token);
				String msg = MessageFormat.format("httpsUrl,httpsChargingUrl:{0},result:{1},epcode:{2},epgunno:{3}",
						httpsChargingUrl,result,epCode,epGunNo);
				logger.debug(msg);
			
			}
			break;
			default:
			break;
		}
		
	} 

}
