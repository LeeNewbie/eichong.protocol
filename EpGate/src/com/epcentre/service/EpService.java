package com.epcentre.service;

import io.netty.channel.Channel;

import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.Vector;
import java.util.concurrent.ConcurrentHashMap;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.epcentre.cache.BespCache;
import com.epcentre.cache.ElectricPileCache;
import com.epcentre.cache.EpCommClient;
import com.epcentre.cache.EpGunCache;
import com.epcentre.cache.INetObject;
import com.epcentre.cache.RealACChargeInfo;
import com.epcentre.cache.RealChargeInfo;
import com.epcentre.config.GameConfig;
import com.epcentre.constant.CommStatusConstant;
import com.epcentre.constant.EpConstant;
import com.epcentre.constant.EpConstantErrorCode;
import com.epcentre.constant.EpProtocolConstant;
import com.epcentre.dao.DB;
import com.epcentre.model.RateInfo;
import com.epcentre.model.TblElectricPile;
import com.epcentre.model.TblElectricPileGun;
import com.epcentre.protocol.AppServerProtocol;
import com.epcentre.protocol.EpEncodeProtocol;
import com.epcentre.protocol.Iec104Constant;
import com.epcentre.protocol.SingleInfo;
import com.epcentre.protocol.WmIce104Util;
import com.epcentre.sender.EpMessageSender;
import com.epcentre.test.ImitateEpService;
import com.epcentre.utils.DateUtil;
import com.netCore.conf.CoreConfig;

public class EpService {
	
	private static final Logger logger = LoggerFactory.getLogger(UserService.class);
	
    
	public static Map<String,ElectricPileCache> mapEpCache = new ConcurrentHashMap<String, ElectricPileCache>();
	
	public static Map<String,ElectricPileCache> getMapEpCache()
	{
		return mapEpCache;
	}
	
	public static String getCacheSize()
	{
		final StringBuilder sb = new StringBuilder();
        sb.append("EpService:\n");
        
        sb.append("mapEpCache count:").append(mapEpCache.size()).append("\n\n");
         
        return sb.toString();
	}
	
	/**
	 * 取最近更新过费率的电桩
	 */
	public static void checkModifyRate()
	{
		List<TblElectricPile> epList = DB.epClientDao.getLastUpdate();
		int size = epList.size();
		logger.info("checkModifyRate,size:{}",size);
		for(int i=0;i<size;i++)
		{
			TblElectricPile epInfo=epList.get(i);
			ElectricPileCache epCache = mapEpCache.get(epInfo.getEpCode());
			epCache.setRateid(epInfo.getRateid());
			epCache.setTypeSpanId(epInfo.getEpTypeSpanId());
		}
		
		
	}
	public static ElectricPileCache convertElectricPileCache(TblElectricPile dbEp)
	{
		ElectricPileCache epCache = new ElectricPileCache();
		epCache.setPkEpId(dbEp.getPkEpId());
		epCache.setCode(dbEp.getEpCode());
		epCache.setName(dbEp.getEpName());
		epCache.setCurrentType(dbEp.getCurrentType());
		epCache.setGunNum(dbEp.getEpGunNum());
		epCache.setConcentratorId(dbEp.getStationId());
		epCache.setStationIndex(dbEp.getStationIndex());
		epCache.setRateid(dbEp.getRateid());
		epCache.setAddress(dbEp.getAddress());
		epCache.setCompany_number(dbEp.getCompany_number());
		epCache.setTypeSpanId(dbEp.getEpTypeSpanId());
        epCache.setOwnCityCode(dbEp.getElPiOwnCityCode());
		epCache.setOwnProvinceCode(dbEp.getElPiOwnProvinceCode());
		epCache.setState(dbEp.getElpiState());
		epCache.setDeleteFlag(dbEp.getDeleteFlag());
		
		return epCache;
	}
	
   
	public static TblElectricPile getDbElectricPile(String epCode)
	{
		TblElectricPile epInfo=null;
		List<TblElectricPile> epList = DB.epClientDao.findResultObject(epCode);
		if(epList ==null || epList.size() !=1)
		{
			//都没有的话断定为无效桩,强制断掉该客户连接
			if(epList ==null || epList.size() ==0)
			{
				logger.info("initConnect fail,epCode:{} have not ElectricPile in DB",epCode);
			}
			else
			{
				logger.info("initConnect fail,epCode:{} have {} ElectricPile in DB",epCode,epList.size());
			}
		}
		else
		{
			epInfo = epList.get(0);
		}
		return epInfo;
	}
	
	public static ElectricPileCache loadDiscreteEpConnect(int commVersion,String epCode,int bootStatus)
	{
		boolean bFrist=false;
		ElectricPileCache epCache = mapEpCache.get(epCode);
		if(epCache==null)
		{
			bFrist =true;
			epCache = EpService.init(epCode);
			if(epCache == null )
				return null;
		}
		
		int concentratorId = epCache.getConcentratorId();
		
		if(concentratorId>0)//如果已经绑定集中器，禁止离散连接
		{
			logger.error("initConnect,loadDiscreteEpConnect fail,epCode:{},concentratorId:{}",
					epCode,concentratorId);
			return null;
		}
		
		if(bootStatus==1)//bootLoader状态
		{
			epCache.initGunsBootLoader();
		}
		
		//初始化设备硬件版本信息
		EqVersionService.getEpVersionFromDB(epCache,1);
		
		if(bFrist)
		{
			if(!epCache.init(bootStatus))
				return null;
		}
				
				
		return epCache;
	}
	public static boolean imitateInitDiscreteEp(int commVersion,String epCode)
	{
		ElectricPileCache epCache= loadDiscreteEpConnect(commVersion,epCode,0);
		if(epCache==null)
		{
			//logger.error("imitateInitDiscreteEp epCache=null,epCode:{}",epCode);
			return false;
		}
		int gateId = CoreConfig.gameConfig.getId();
		
		//模拟电桩连接
		EpCommClient epCommClient = new EpCommClient();	
	
		epCommClient.initNetSuccess(epCode, commVersion, EpConstant.COMM_MODE_OF_DISCRETE_POLE);
		
		epCommClient.setBootStatus(2);
		epCommClient.setGateId(gateId);
		epCommClient.setRevINum(0);
		
	    epCache.setEpNetObject(epCommClient);
	    epCache.setGateid(gateId);
	    
	    epCache.setLastUseTime(DateUtil.getCurrentSeconds());
	    
       
	    //更新桩状态

	    EpService.updateEpCommStatusToDb(epCache.getPkEpId(),0,epCache.getGateid());
	    
	    //增加到缓存
	    EpService.addEpCache(epCache);
		
		return true;
		
		
	}
	public static boolean initDiscreteEpConnect(int commVersion,String epCode,EpCommClient epCommClient,int bootStatus)
	{
		
		//1.判断是否是有效桩体
		ElectricPileCache epCache = loadDiscreteEpConnect(commVersion,epCode,bootStatus);//1.判断缓存里时候有
	
		if(epCache == null )
		{
			logger.error("initConnect,epCode:{},fail dbEp == null,close channel",epCode);

			//2.无效桩，强制关闭
			epCommClient.close();
            EpCommClientService.removeEpCommClient(epCommClient);
			return false;
		}
		if(epCache.getConcentratorId()>0)
		{
			logger.error("initConnect,epCode:{},fail epCache.getConcentratorId()>0,concentratorID:{},close channel",epCode,epCache.getConcentratorId());
			//2.无效桩，强制关闭
			epCommClient.close();
            EpCommClientService.removeEpCommClient(epCommClient);
			return false;
		}
		
		int gateId = CoreConfig.gameConfig.getId();
		
		EpCommClientService.handleOldClient(epCommClient, epCache.getCode());
		
		
		epCommClient.initNetSuccess(epCode, commVersion, EpConstant.COMM_MODE_OF_DISCRETE_POLE);
		
		epCommClient.setBootStatus(bootStatus);
		epCommClient.setGateId(gateId);
		epCommClient.setRevINum(0);
		
	    epCache.setEpNetObject(epCommClient);
	    epCache.setGateid(gateId);	
	    epCache.updateNetObject();
	    
        byte[] changeGateData = AppServerProtocol.epIpChange(epCode,epCache.getGateid());
	    AppClientService.notifyEpGate(changeGateData);
	    //更新桩状态

	    EpService.updateEpCommStatusToDb(epCache.getPkEpId(),1,epCache.getGateid());
	    
	    //增加到缓存
	    EpService.addEpCache(epCache);
	    
	    epCache.setLastUseTime(DateUtil.getCurrentSeconds());
	

  		logger.info("initConnect success,epCode:{},getId:{},commVersion:{},boot:{},channel:{}",
  				new Object[]{epCode,epCache.getGateid(),commVersion,bootStatus,epCommClient.getChannel()});
		return true;
	}
	
	
	

	public  static void addEpCache(ElectricPileCache epCache){
		if(epCache!=null)
		{
			String epCode= epCache.getCode();
	
			mapEpCache.put(epCode,epCache);
		}
	}
    
    public static ElectricPileCache getEpByCode(String epCode){
	
		ElectricPileCache electricUser = mapEpCache.get(epCode);
		return electricUser;
	}
    
    public static ElectricPileCache init(String epCode){
    	
		TblElectricPile dbEp = EpService.getDbElectricPile(epCode);
		if(dbEp == null )
			return null;
	
		ElectricPileCache epCache = EpService.convertElectricPileCache(dbEp);
		
		return epCache;
	}

	
	@SuppressWarnings("rawtypes")
	
	public static void updateEpCommStatusToDb(int pkEpId,int commStatus,int gateId)
	{
		TblElectricPile updateEp= new TblElectricPile();
		updateEp.setPkEpId(pkEpId);
		updateEp.setComm_status(commStatus);
		updateEp.setGateid(gateId);

		DB.epClientDao.updateCommStatus(updateEp);
	}
	public static void updateEpsCommStatusToDb(int stationId,int commStatus,int gateId)
	{
		TblElectricPile updateEp= new TblElectricPile();
		updateEp.setComm_status(commStatus);
		updateEp.setStationId(stationId);
		updateEp.setGateid(gateId);

		DB.epClientDao.updateCommStatusByStationId(updateEp);
	}
	

	
	public static int getCurrentType(String epCode)
	{
		ElectricPileCache epClient= getEpByCode(epCode);
		if(	epClient ==null )
		{
			return -1;
		}
		return epClient.getCurrentType();
	}
	public static int getCurrentEpCount()
	{
		return mapEpCache.size();
	}
	
	
	
	
	public static void updateEpsRate(String epcodes,int rateid,RateInfo rateInfo)
	{
		 String[] epCodeArray = epcodes.split(",+"); 
    	 for(String eachEpCode : epCodeArray)
   	     { 
    		 int errorCode = updateEpRate(eachEpCode,rateid,rateInfo);
    		 
    		 logger.info("updateEpRate---epCode:"+eachEpCode
						+",rateId:"+rateid
						+",errorCode(0:success,1:fail):"+errorCode);
    		 
    	 }
	}
	
    public static int updateEpRate(String epCode,int rateid, RateInfo Info)
    {
    	ElectricPileCache  epClient = EpService.getEpByCode(epCode);
		if(epClient == null)
		{
			 return 1;
		}
		epClient.setRateid(rateid);
		
		EpCommClient commClient = (EpCommClient)epClient.getEpNetObject();
		
		if(commClient == null || !commClient.isComm())
		{
			return 2;
		}
   
		byte []cmdTimes = WmIce104Util.timeToByte();
     	
 		 byte[] bRateData= EpEncodeProtocol.do_consume_model(epCode,Info);
 		 
 		if(bRateData ==null)
		{
			logger.error("updateEpRate,do_consume_model exception");
			return 1;
		}
 		
 		EpMessageSender.sendMessage(commClient,0,0,Iec104Constant.C_CONSUME_MODEL, bRateData,cmdTimes,commClient.getVersion());

    	return 0;
    }
    
    public static void updateEpRateToDb(int pkEpId,int rateid)
    {
    	TblElectricPile tblElectricPile = new TblElectricPile();
    	tblElectricPile.setPkEpId(pkEpId);
    	tblElectricPile.setRateid(rateid);
    	
    	DB.epClientDao.updateRateId(tblElectricPile);
    
    }
    
    
    private static void sendRateInfoByGroup(String epCodes,int rateId,RateInfo rateInfo)
    {
    	String[] epCodeArray = epCodes.split(",+"); 
    	for(String epCodeEach : epCodeArray)
    	{ 
    		  ElectricPileCache  epClient = EpService.getEpByCode(epCodeEach);
    		  if(epClient == null){
    			  logger.error("sendRateInfoByGroup find ElectricPile:{}",epCodeEach);
    			   continue;
    		  }
    		  //EpCommClient commClient = EpCommClientService.getCommClient(epClient.getConcentratorId(),epClient.getCode());
    		  EpCommClient commClient = (EpCommClient)epClient.getEpNetObject();
    		  if(commClient==null || !commClient.isComm()) {
    			  logger.error("sendRateInfoByGroup ElectricPile:{} is not comm",epCodeEach);
    			   continue;
    		  }
    		
    		byte[] cmdTimes =  WmIce104Util.timeToByte();
		    byte[] bConsumeModelProtoData= EpEncodeProtocol.do_consume_model(epCodeEach,rateInfo);
		    if(bConsumeModelProtoData != null)
		    {
		    	 logger.error("sendRateInfoByGroup ElectricPile:{},do_consume_model exception",epCodeEach);
  			   continue;
		    }
		    EpMessageSender.sendMessage(commClient,0,0,Iec104Constant.C_CONSUME_MODEL, bConsumeModelProtoData,cmdTimes,commClient.getVersion());
		    epClient.setRateid(rateId);  	     
         }
    }
    
    public static void handleCommClientTimeOut(String clientIdentity,int gateId)
	{
    	if(clientIdentity==null || clientIdentity.length()<1)
		{
			logger.info("epChannelHandler,handleCommClientTimeOut clientIdentity is empty");
			return;
		}
    	
    	ElectricPileCache epCache = getEpByCode( clientIdentity );
    	
    	if(epCache == null)
    	{
    		logger.info("epChannelHandler,handleCommClientTimeOut not find ep:{}",clientIdentity);
    	}
    	else
    	{
    		logger.info("epChannelHandler,handleCommClientTimeOut clientIdentity:{}",clientIdentity);
    		epCache.onNetStatus(0);
    		
    		if(epCache.getEpNetObject()!=null && epCache.getEpNetObject().getStatus()==2)
    		{
	    		epCache.setEpNetObject(null);
	    		EpService.updateEpCommStatusToDb(epCache.getPkEpId(), 0, gateId);
    		}
    	}
		
	}
    public static int  doNearCallEpAction(String epCode,int type,int time,int  accountId,float lng,float lag)
	
    {
    	 ElectricPileCache epCache= EpService.getEpByCode(epCode);
         if(epCache==null)
         {
        	 logger.info("not find ElectricPileCache,epCode:{}",epCode);
         	return EpConstantErrorCode.EP_UNCONNECTED;
         	
         }
       //检查电桩
 		int errorCode = epCache.canNearCall(true);
 		if(errorCode > 0)
 		{
 			 logger.info("doNearCallEpAction fail.epCode:{},errorCode",epCode,errorCode);
 			return errorCode;
 		}
    	int hadBesp=0;
        for(int i=0;i<epCache.getGunNum();i++)
        {
        	EpGunCache epGunCache=EpGunService.getEpGunCache(epCode, i+1);
        	if(epGunCache==null)
        		continue;
        	BespCache bespCacheObj =epGunCache.getBespCache();
        	if(bespCacheObj!=null && bespCacheObj.getAccountId()== accountId)
        	{
        		hadBesp=1;
        		break;
        	}
        }
		
    	if(hadBesp == 0)
    	{
    		logger.info("not find BespCache,can not call ep:{}",epCode);
    		return EpConstantErrorCode.CANNOT_REPEAT_SOUNDING_WITHOUT_BESP;
    	}
    	
    	return epCache.callEpAction(type, time,(float)0.0,(float)0.0);
    }
    
    public static void handleEpGateChange(String epCode, int newGateId) {
		byte[] msg = AppServerProtocol.notifyEpGate(epCode, newGateId);

		// 遍历,每个连接上的后台都发一份
		AppClientService.notifyEpGate(msg);
	}
    
	
    
    
    public static int OpenGunLid(String epCode,int epGunNo)
    {
    	int errorCode = 0;
    	ElectricPileCache epCache = EpService.getEpByCode(epCode);//1.判断缓存里时候有
		if(epCache == null )
		{
			logger.info("OpenGunLid did not find ep:" + epCode
					+ "\r\n");
			return EpConstantErrorCode.INVALID_EP_CODE;
		}
		EpGunCache epGunCache = EpGunService.getEpGunCache(epCode, epGunNo);
		if(epGunCache == null)
		{
			return EpConstantErrorCode.INVALID_EP_CODE;
		}
		
		if(!epGunCache.getEpNetObject().isComm())
		{
			return EpConstantErrorCode.EP_UNCONNECTED;
			
		}
		RealACChargeInfo chargeInfo =(RealACChargeInfo)(epGunCache.getRealChargeInfo());
		if(chargeInfo.getGunLidStatus() ==0)
		{
			return EpConstantErrorCode.EPE_GUN_LID_OPEND;
		}
		
		//EpGunService.testChangeGunSignalStatus(epCode,epGunNo,1);
		
    	return 0;
    }
    //////////////////////////////////////////////
    
    public static int DropGroupLock(String epCode,int epGunNo)
    {
    	ElectricPileCache epCache = EpService.getEpByCode(epCode);
		if(epCache == null )
		{
			logger.info("DropGroupLock did not find ep:" + epCode
					+ "\r\n");
			return EpConstantErrorCode.INVALID_EP_CODE;
		}
		
		
    	return 0;
    }
    
    public static int EpStat(String epCode)
	{
		ElectricPileCache epClient= getEpByCode(epCode);
		
		if(epClient == null )
		{
			return EpConstantErrorCode.INVALID_EP_CODE;
		}
		EpCommClient commClient = (EpCommClient)epClient.getEpNetObject();
		  if(commClient==null || !commClient.isComm()) {
			  logger.error("EpStat ElectricPile is not comm,epCode:{}",epCode);
		    return EpConstantErrorCode.EP_UNCONNECTED;
		}
		byte []cmdTimes = WmIce104Util.timeToByte();
		
		byte[] statData= EpEncodeProtocol.do_ep_stat(epCode);
		EpMessageSender.sendMessage(commClient,0,0,Iec104Constant.C_EP_STAT, statData,cmdTimes,commClient.getVersion());
			
		
	    return 0;
    }
    public static int getGunNo(int address,int type)
    {
    	switch(type)
    	{
    	case 1:
    		return (address/128)+1;
    		
    	case 3:
    		return (address/128)+1;
    	case 11:
    		return (address/2500)+1;
    	case 132:
    		return (address/128)+1;
    	default:
    		return 0;
    	}
    }
    
    public static void handleOneBitYxInfo(String epCode,Vector<SingleInfo> singleInfos)
	{
    	ElectricPileCache epCache = EpService.getEpByCode(epCode);
    
		if(epCache==null)
		{
			logger.error("handleOneBitYxInfo,realData dataType:1=oneBitYx,epCode:{},not find ElectricPileCache",epCode);
			return ;
		}
		Map<String, GunPointMap> pointMaps = new ConcurrentHashMap<String,GunPointMap>();
		for(int i=0;i< singleInfos.size(); i++ )
		{
			SingleInfo info = singleInfos.get(i);
			int address = info.getAddress();//紧凑排列
			int yxAddress = address%128;
			
			if(!EpChargeService.isValidAddress(yxAddress, 1))
			{
				logger.debug("handleOneBitYxInfo,realData dataType:1=oneBitYx,epCode:{},address:{}, isValid yxAddress:{}",
						new Object[]{epCode,address,yxAddress});
				continue;
			}
				
			
			int epGunNo = EpService.getGunNo(address,1);
			if(epGunNo<0)
			{
				logger.error("handleOneBitYxInfo,realData dataType:1=oneBitYx,epCode:{},epGunNo:{},address:{} invalid gun no",
						new Object[]{epCode,epGunNo,address});
				continue;
			}
			GunPointMap gunPointMap= pointMaps.get(epCode+epGunNo);
			if(gunPointMap==null)
			{
				gunPointMap = new GunPointMap(epCode,epGunNo);
				pointMaps.put(epCode+epGunNo, gunPointMap);
			}
			
			
			info.setAddress(yxAddress);
			RealChargeInfo.AddPoint(gunPointMap.getPointMap(), info);
		}
		
		
		disptachPointToGun(1,pointMaps);
	}
    
    public static void handleOneBitYxInfo_v4(String epCode,int epGunNo,Vector<SingleInfo> singleInfos)
	{
    	if(epGunNo<0)
		{
			logger.error("handleOneBitYxInfo_v4,realData dataType:1=oneBitYx,epCode:{},epGunNo:{},invalid gun no",epCode,epGunNo);
			return ;
		}
    	ElectricPileCache epCache = EpService.getEpByCode(epCode);
		if(epCache==null)
		{
			logger.error("handleOneBitYxInfo_v4,realData dataType:1=oneBitYx,epCode:{},epGunNo:{},not find ElectricPileCache",epCode,epGunNo);
			return ;
		}
		GunPointMap gunPointMap=new GunPointMap(epCode,epGunNo);
		for(int i=0;i< singleInfos.size(); i++ )
		{
			SingleInfo info = singleInfos.get(i);
			int address = info.getAddress();//紧凑排列
			int yxAddress = address%128;
			
			if(!EpChargeService.isValidAddress(yxAddress, 1))
			{
				logger.debug("handleOneBitYxInfo_v4,realData dataType:1=oneBitYx,epCode:{},epGunNo:{},isValid address:{},yxAddress:{}",
						new Object[]{epCode,epGunNo,address,yxAddress});
				continue;
			}
			info.setAddress(yxAddress);
			RealChargeInfo.AddPoint(gunPointMap.getPointMap(), info);
		}
		
		EpGunCache epGunCache =  EpGunService.getEpGunCache(epCode, epGunNo);
		if(epGunCache !=null)
		{
			epGunCache.onRealDataChange(gunPointMap.getPointMap(), 1);
		}
	}
    
	public static void handleTwoBitYxInfo(String epCode,Vector<SingleInfo> singleInfos)
	{
		ElectricPileCache epCache = EpService.getEpByCode(epCode);
	    
		if(epCache==null)
		{
			logger.error("handleTwoBitYxInfo,realData dataType:2=twoBitYx,epCode:{},not find ElectricPileCache",epCode);
			return ;
		}
		
		Map<String, GunPointMap> pointMaps = new ConcurrentHashMap<String,GunPointMap>();
		
		for(int i=0;i< singleInfos.size(); i++ )
		{
			SingleInfo info = singleInfos.get(i);
			int address = info.getAddress();//紧凑排列
			int yxAddress = address%128;
			if(!EpChargeService.isValidAddress(yxAddress, 3))
			{
				logger.debug("handleTwoBitYxInfo,realData dataType:2=twoBitYx,epCode:{},address:{}, isValid yxAddress:{}",
						new Object[]{epCode,address,yxAddress});
				continue;
			}
			
			int epGunNo = EpService.getGunNo(address,3);
			if(epGunNo<0)
			{
				logger.error("handleTwoBitYxInfo,realData dataType:2=twoBitYx,epCode:{},epGunNo:{},address:{} invalid gun no",
						new Object[]{epCode,epGunNo,address});
				continue;
			}
			
			GunPointMap gunPointMap= pointMaps.get(epCode+epGunNo);
			if(gunPointMap==null)
			{
				gunPointMap = new GunPointMap(epCode,epGunNo);
				pointMaps.put(epCode+epGunNo, gunPointMap);
			}
			
			info.setAddress(yxAddress+EpProtocolConstant.YX_2_START_POS);
			RealChargeInfo.AddPoint(gunPointMap.getPointMap(), info);
		}
		disptachPointToGun(1,pointMaps);
	}
	
	public static void handleTwoBitYxInfo_v4(String epCode,int epGunNo,Vector<SingleInfo> singleInfos)
	{
		if(epGunNo<0)
		{
			logger.error("handleTwoBitYxInfo_v4,realData dataType:2=twoBitYx,epCode:{},epGunNo:{},invalid gun no",epCode,epGunNo);
			return ;
		}
    	ElectricPileCache epCache = EpService.getEpByCode(epCode);
		if(epCache==null)
		{
			logger.error("handleTwoBitYxInfo_v4,realData dataType:2=twoBitYx,epCode:{},epGunNo:{},not find ElectricPileCache",epCode,epGunNo);
			return ;
		}
		GunPointMap gunPointMap=new GunPointMap(epCode,epGunNo);
		
		for(int i=0;i< singleInfos.size(); i++ )
		{
			SingleInfo info = singleInfos.get(i);
			int address = info.getAddress();//紧凑排列
			int yxAddress = address%128;
			if(!EpChargeService.isValidAddress(yxAddress, 3))
			{
				logger.debug("handleTwoBitYxInfo_v4,realData dataType:2=twoBitYx,epCode:{},epGunNo:{},isValid address:{},yxAddress:{}",
						new Object[]{epCode,epGunNo,address,yxAddress});
				continue;
			}
			
			info.setAddress(yxAddress+EpProtocolConstant.YX_2_START_POS);
			RealChargeInfo.AddPoint(gunPointMap.getPointMap(), info);
		}
		
		EpGunCache epGunCache =  EpGunService.getEpGunCache(epCode, epGunNo);
		if(epGunCache !=null)
		{
			epGunCache.onRealDataChange(gunPointMap.getPointMap(), 3);
		}
	}
	
	public static void handleYcInfo(String epCode,Vector<SingleInfo> singleInfos)
	{
		ElectricPileCache epCache = EpService.getEpByCode(epCode);
	    
		if(epCache==null)
		{
			logger.error("handleYcInfo,realData dataType:3=yc,epCode:{}, not find ElectricPileCache",epCode);
			return ;
		}
		
		Map<String, GunPointMap> pointMaps = new ConcurrentHashMap<String,GunPointMap>();
		
		for(int i=0;i< singleInfos.size(); i++ )
		{
			SingleInfo info = singleInfos.get(i);
			int address = info.getAddress();//紧凑排列
			int ycAddress = address%2500;
	
			if(!EpChargeService.isValidAddress(ycAddress,11))
			{
				logger.debug("handleYcInfo,realData dataType:3=yc,epCode:{},address:{}, isValid ycAddress:{}",
						new Object[]{epCode,address,ycAddress});
				continue;
			}
				
			
			int epGunNo = EpService.getGunNo(address,11);
			if(epGunNo<0)
			{
				logger.error("handleYcInfo,realData dataType:3=yc,epCode:{},epGunNo:{},address:{} invalid gun no",
						new Object[]{epCode,epGunNo,address});
				continue;
			}
			
			
			GunPointMap gunPointMap= pointMaps.get(epCode+epGunNo);
			if(gunPointMap==null)
			{
				gunPointMap = new GunPointMap(epCode,epGunNo);
				pointMaps.put(epCode+epGunNo, gunPointMap);
			}
				
			info.setAddress(ycAddress+EpProtocolConstant.YC_START_POS);
			
			RealChargeInfo.AddPoint(gunPointMap.getPointMap(), info);
		}
		disptachPointToGun(11,pointMaps);
	}
	public static void handleYcInfo_v4(String epCode,int epGunNo,Vector<SingleInfo> singleInfos)
	{
		if(epGunNo<0)
		{
			logger.error("handleYcInfo_v4,realData dataType:3=yc,epCode:{},epGunNo:{},invalid gun no",epCode,epGunNo);
			return ;
		}
    	ElectricPileCache epCache = EpService.getEpByCode(epCode);
		if(epCache==null)
		{
			logger.error("handleYcInfo_v4,realData dataType:3=yc,epCode:{},epGunNo:{},not find ElectricPileCache",epCode,epGunNo);
			return ;
		}
		GunPointMap gunPointMap= new GunPointMap(epCode,epGunNo);
		
		for(int i=0;i< singleInfos.size(); i++ )
		{
			SingleInfo info = singleInfos.get(i);
			int address = info.getAddress();//紧凑排列
			int ycAddress = address%2500;
	
			if(!EpChargeService.isValidAddress(ycAddress,11))
			{
				logger.debug("handleYcInfo_v4,realData dataType:3=yc,epCode:{},epGunNo:{},isValid address:{},ycAddress:{}",
						new Object[]{epCode,epGunNo,address,ycAddress});
				continue;
			}
			info.setAddress(ycAddress+EpProtocolConstant.YC_START_POS);
			
			RealChargeInfo.AddPoint(gunPointMap.getPointMap(), info);
		}
		EpGunCache epGunCache =  EpGunService.getEpGunCache(epCode, epGunNo);
		if(epGunCache !=null)
		{
			epGunCache.onRealDataChange(gunPointMap.getPointMap(), 11);
		}
	}
	public static void handleVarYcInfo(String epCode,Vector<SingleInfo> singleInfos)
	{
		ElectricPileCache epCache = EpService.getEpByCode(epCode);
	    
		if(epCache==null)
		{
			logger.error("handleVarYcInfo,realData dataType:4=varYc,epCode:{}, not find ElectricPileCache",epCode);
			return ;
		}
		
		Map<String, GunPointMap> pointMaps = new ConcurrentHashMap<String,GunPointMap>();
		
		for(int i=0;i< singleInfos.size(); i++ )
		{
			SingleInfo info = singleInfos.get(i);
			int address = info.getAddress();//紧凑排列
			int ycAddress = address%128; 
			int epGunNo = EpService.getGunNo(address,132);
			if(epGunNo<0)
			{
				logger.error("handleVarYcInfo,realData dataType:4=varYc,epCode:{},epGunNo:{},address:{} invalid gun no",
						new Object[]{epCode,epGunNo,address});
				continue;
			}
		
			GunPointMap gunPointMap= pointMaps.get(epCode+epGunNo);
			if(gunPointMap==null)
			{
				gunPointMap = new GunPointMap(epCode,epGunNo);
				pointMaps.put(epCode+epGunNo, gunPointMap);
			}
			info.setAddress(ycAddress+EpProtocolConstant.YC_VAR_START_POS);
			
			RealChargeInfo.AddPoint(gunPointMap.getPointMap(), info);
		}
		
		disptachPointToGun(132,pointMaps);
	}
	
	public static void handleVarYcInfo_v4(String epCode,int epGunNo,Vector<SingleInfo> singleInfos)
	{
		if(epGunNo<0)
		{
			logger.error("handleVarYcInfo_v4,realData dataType:4=varYc,epCode:{},epGunNo:{},invalid gun no",epCode,epGunNo);
			return ;
		}
    	ElectricPileCache epCache = EpService.getEpByCode(epCode);
		if(epCache==null)
		{
			logger.error("handleVarYcInfo_v4,realData dataType:4=varYc,epCode:{},epGunNo:{},not find ElectricPileCache",epCode,epGunNo);
			return ;
		}
		
		
		GunPointMap gunPointMap = new GunPointMap(epCode,epGunNo);
		for(int i=0;i< singleInfos.size(); i++ )
		{
			SingleInfo info = singleInfos.get(i);
			int address = info.getAddress();
			
			info.setAddress(address+EpProtocolConstant.YC_VAR_START_POS);
			RealChargeInfo.AddPoint(gunPointMap.getPointMap(), info);
		}
		EpGunCache epGunCache =  EpGunService.getEpGunCache(epCode, epGunNo);
		if(epGunCache !=null)
		{
			epGunCache.onRealDataChange(gunPointMap.getPointMap(), 132);
		}
	}
	
	public static void disptachPointToGun(int type,Map<String, GunPointMap> pointMaps)
	{
		Iterator iter = pointMaps.entrySet().iterator();
		
		while (iter.hasNext()) {
			Map.Entry entry = (Map.Entry) iter.next();
			
			GunPointMap gunPointMap=(GunPointMap) entry.getValue();
			
			if(gunPointMap==null)
			{
				continue;	
			}
			EpGunCache epGunCache =  EpGunService.getEpGunCache(gunPointMap.getEpCode(), gunPointMap.getEpGunNo());
			if(epGunCache !=null)
			{
				epGunCache.onRealDataChange(gunPointMap.getPointMap(), type);
			}
		}
	}
   
	public static int queryConsumeRecord(String epCode,int startPos,int enteryNum)
	{
		ElectricPileCache epClient= getEpByCode(epCode);
		
		if(epClient == null )
		{
			return EpConstantErrorCode.INVALID_EP_CODE;
		}
		
		EpCommClient commClient = (EpCommClient)epClient.getEpNetObject();
		  if(commClient==null || !commClient.isComm()) 
		  {
			  logger.error("queryConsumeRecord ElectricPile is not comm,epCode:{}",epCode);
		    return EpConstantErrorCode.EP_UNCONNECTED;
		}
		byte []cmdTimes = WmIce104Util.timeToByte();
		
		
		 
		
		 byte[] statData= EpEncodeProtocol.do_query_consume_record(epCode,startPos,(short)enteryNum);
		 EpMessageSender.sendMessage(commClient,0,0,Iec104Constant.C_QUERY_CONSUME_RECORD, statData,cmdTimes,commClient.getVersion());
			
		
	    return 0;
    }
	
	public static int queryCommSignal(String epCode,short stationAddr)
	{
		ElectricPileCache epClient= getEpByCode(epCode);
		
		if(epClient == null )
		{
			return EpConstantErrorCode.INVALID_EP_CODE;
		}
		
		EpCommClient commClient = (EpCommClient)epClient.getEpNetObject();
		if(commClient==null || !commClient.isComm()) 
		{
			  logger.error("queryCommSignal ElectricPile is not comm,epCode:{}",epCode);
		    return EpConstantErrorCode.EP_UNCONNECTED;
		}
		byte []cmdTimes = WmIce104Util.timeToByte();
		
		byte[] statData= EpEncodeProtocol.do_query_comm_signal(epCode, stationAddr);
		EpMessageSender.sendMessage(commClient,0,0,Iec104Constant.C_QUERY_COMM_SIGNAL, statData,cmdTimes,commClient.getVersion());
			
		
	    return 0;
    }
	
	public static void handleCommSignal(String epCode,int signal)
	{
         ElectricPileCache epClient= getEpByCode(epCode);
		
		if(epClient == null )
		{   
			logger.error("handleCommSignal epClient:{} is null",epCode);
			return ;
		}
		EpCommClient commClient =(EpCommClient)epClient.getEpNetObject();
		if(commClient == null )
		{   
			logger.error("handleCommSignal epClient.getEpNetObject() is null,epCode:{}",epCode);
			return ;
		}
		
		commClient.setCommSignal(signal);
	}
	
	
	
	public static int handleEpIdentyCodeQuery(EpCommClient commClient,String epCode,int gunno,int hour,int min,int sec)
	{
		EpGunCache epGunCache = EpGunService.getEpGunCache(epCode, gunno);
		 if(epGunCache == null)
		 {
			 logger.error("handleEpIdentyCodeQuery not find EpGun,epCode:{},gunno:{}",epCode,gunno);
			 return EpConstantErrorCode.EP_UNCONNECTED;
		 }
		 
		 long createTime = epGunCache.getCreateIdentyCodeTime();
		 String identyCode = epGunCache.getIdentyCode();
		 
		 long now = DateUtil.getCurrentSeconds();
		 
		 boolean bIsCreateICode =false;
		 long diff = now-createTime;
		 if(diff>=GameConfig.identycodeTimeout1 || identyCode.length()<6)
		 {
			 String number=createIdentyCode(now,epGunCache.getPkEpGunId());
			 
			
			 epGunCache.setIdentyCode(number);
			 epGunCache.setCreateIdentyCodeTime(now);
			 createTime = now;
			 
			 bIsCreateICode= true;
			 
			 identyCode =  number;
			 logger.debug("handleEpIdentyCodeQuery: updateQR,epCode:{},epGunNo:{},identyCode:{}",
					 new Object[]{epCode,gunno,identyCode});
		 }
		 int errorCode=0;
		 if(commClient!=null && commClient.isComm()) 
		 {
			 byte[] data= EpEncodeProtocol.do_ep_identyCode(epCode, (byte)gunno,identyCode, 
						createTime,(byte)hour,(byte)min,(byte)sec,commClient.getVersion());
				byte[] cmdTimes = WmIce104Util.timeToByte(hour, min, sec);
				EpMessageSender.sendMessage(commClient,0,0,Iec104Constant.M_EP_IDENTYCODE, data,cmdTimes,commClient.getVersion());
				
				
			  logger.info("handleEpIdentyCodeQuery send identyCode:{},epCode:{},gunno:{}",
					  new Object[]{identyCode,epCode,gunno});
		  }
		 else
		 {
			 logger.error("handleEpIdentyCodeQuery commClient:{} is not comm",commClient);
			 errorCode= EpConstantErrorCode.EP_UNCONNECTED;
		 }
		
		
		if(bIsCreateICode)
		{
			logger.info("handleEpIdentyCodeQuery,save db,epCode:{},epGunNo:{},identyCode:{}",
					new Object[]{epCode,gunno,identyCode});
			
			 TblElectricPileGun tblGun=new TblElectricPileGun();
			 
			 tblGun.setPkEpGunId(epGunCache.getPkEpGunId());
			 tblGun.setQrdate(now+GameConfig.identycodeTimeout2);
			 
			 tblGun.setQr_codes(identyCode);
			 //保存到数据库
			 DB.epGunDao.updateQR(tblGun);
		}
		
		return errorCode;
	}
	
	public static String createIdentyCode(long dt,int pkEpId)
	{
		
		long t1= dt % 960;
		long t2 = pkEpId % 100;
		long t3 = (dt % pkEpId)%10;
		
		return String.format("%03d%02d%d",t1,t2,t3);
	}
	
	public static ElectricPileCache getEpRealStatus(ElectricPileCache epCache)
	{
		if(epCache == null )
		{ 
			 return null;
		}
		int ret =0;
		List<TblElectricPile> epList = DB.epClientDao.findResultObject(epCache.getCode());
		if(epList ==null || epList.size() !=1)
		{
			return null;
		}
		TblElectricPile dbEp = epList.get(0);
		
		 epCache.setCompany_number(dbEp.getCompany_number());
		 epCache.setState(dbEp.getElpiState());
		 epCache.setDeleteFlag(dbEp.getDeleteFlag());
		
		 //更新
		addEpCache(epCache);
		 
		return epCache;
	}
	
	
	

	
}
