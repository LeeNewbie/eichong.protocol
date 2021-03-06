package com.epcentre.service;

import io.netty.channel.Channel;

import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.Vector;
import java.util.concurrent.ConcurrentHashMap;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.epcentre.cache.ElectricPileCache;
import com.epcentre.cache.EpCommClient;
import com.epcentre.cache.EpConcentratorCache;
import com.epcentre.cache.EpGunCache;
import com.epcentre.cache.RealChargeInfo;
import com.epcentre.constant.CommStatusConstant;
import com.epcentre.constant.EpConstant;
import com.epcentre.constant.EpConstantErrorCode;
import com.epcentre.constant.EpProtocolConstant;
import com.epcentre.dao.DB;
import com.epcentre.model.TblElectricPile;
import com.epcentre.protocol.EpEncodeProtocol;
import com.epcentre.protocol.Iec104Constant;
import com.epcentre.protocol.SingleInfo;
import com.epcentre.protocol.WmIce104Util;
import com.epcentre.sender.EpMessageSender;
import com.epcentre.utils.DateUtil;
import com.netCore.conf.CoreConfig;

public class EpConcentratorService {
	
	private static final Logger logger = LoggerFactory.getLogger(EpConcentratorService.class);
	
	private static Map<Integer, EpConcentratorCache> mapStation = new ConcurrentHashMap<Integer,EpConcentratorCache>();
	private static Map<Channel, EpConcentratorCache> mapCh2Station = new ConcurrentHashMap<Channel,EpConcentratorCache>();
	
	
	
	public static Map<Channel, EpConcentratorCache> getMapCh2Station()
	{
		return mapCh2Station;
	}
	
	@SuppressWarnings("null")
	public static boolean InitStation(EpCommClient commClient,int stationId,int commVersion)
	{
		List<TblElectricPile> epList = DB.epClientDao.getEpsByStationId(stationId);
		if(epList==null && epList.size()<=0)
		{
			return false;
		}
		EpConcentratorCache epStation = new EpConcentratorCache();
		
		epStation.setEpCount(epList.size());
		epStation.setPkId(stationId);
		
		try
		{
	
		for(int i=0;i< epList.size();i++)
		{
			TblElectricPile tblEp = epList.get(i);
			
			epStation.setEpGunNum(tblEp.getStationIndex(), tblEp.getEpGunNum());
			
			epStation.addEpCode(tblEp.getStationIndex(), tblEp.getEpCode());
			
			ElectricPileCache epCache = EpService.getElectricPileCache(tblEp);
			epCache.setEpNetObject(commClient);
			epCache.setConcentratorId(stationId);
			
			if(epCache.initGuns())
			{
				EpService.addEpCache(epCache);
				
			}
		}
		}
		catch (Exception e) {
			
			e.printStackTrace();
		}
		
		AddConCentrator(commClient.getChannel(),epStation);
		
		return true;
	}
	public static void AddConCentrator(Channel channel,EpConcentratorCache conCentratorCache)
	{
		if(conCentratorCache!=null)
		{
			mapStation.put(conCentratorCache.getPkId(), conCentratorCache);//这个不移除,以免在重连的时候查数据库
			mapCh2Station.put(channel, conCentratorCache);
		}
	}
	public static void RemoveConCentrator(Channel channel)
	{
		if(channel!=null)
		{
			mapCh2Station.remove(channel);
		}
	}
	public static EpConcentratorCache getConCentrator(Channel channel)
	{
		EpConcentratorCache conCentratorCache = null;
		if(channel!=null)
		{
			conCentratorCache = mapCh2Station.get(channel);
		}
		return conCentratorCache;
	}
	public static EpConcentratorCache getConCentrator(Integer Id)
	{
		EpConcentratorCache conCentratorCache = null;
		if(Id!=0)
		{
			conCentratorCache = mapStation.get(Id);
		}
		return conCentratorCache;
	}
	public static boolean initStationConnect(int commVersion,short nStationId,Channel channel)
	{
		
		boolean isVaildStation = false;
		EpCommClient commClient = EpCommClientService.getCommClient(""+nStationId);
		//
		if(commClient !=null && commClient.getChannel()!=null)
		{
			if( commClient.getChannel()!=channel)
			{
				//和当前内存不一致，关闭前一个连接
				logger.info("old ch:{},new ch:{}",commClient.getChannel(),channel);
				EpCommClientService.removeChannel(commClient.getChannel());
				commClient.getChannel().close();
				isVaildStation =true;
			}
			else
			{
				//和当前内存一样，直接还回
				return true;
			}
		}
		
		//判断数据库中是否有站编号
		if(isVaildStation==false)
		{
			int id = nStationId;
			int count =DB.concentratorDao.findStation(id);
			if(count !=1)
			{
				logger.error("isVaildStation! nStationId:{},count:{}",nStationId,count);
				
				//2.无效站，强制关闭
				channel.close();
				return false;
			}
		}
		//
		if(commClient==null)
		{
			commClient = new EpCommClient();
		}
	
	
		int gateId = CoreConfig.gameConfig.getId();
		
		commClient.setRevINum(0);
		commClient.setChannel(channel);
		commClient.setStatus(CommStatusConstant.INIT_SUCCESS);
		commClient.setIdentity(""+nStationId);
		commClient.setLastUseTime(DateUtil.getCurrentSeconds());
		commClient.setGateId(gateId);
		commClient.setCommVersion(commVersion);
		
		
		commClient.setMode(EpConstant.COMM_MODE_CENTRATOR);
		
		if(InitStation(commClient,nStationId,commVersion)==false)
		{
			logger.error("initStationConnect,InitStation==false, ch:{}",channel);
		
			channel.close();
			return false;
		}
			
			
		EpService.updateEpsCommStatusToDb(nStationId, 1, gateId);
		
		EpCommClientService.addCommClient(""+nStationId, commClient);
		
		//初始化设备硬件版本信息
		EqVersionService.getEquipmentVersionFromDB(null,nStationId,1);
		
		
		return true;	
	}
	
	public static void handleOneBitYxInfo(Channel ch,Vector<SingleInfo> singleInfos)
	{
		EpConcentratorCache stationCache = mapCh2Station.get(ch);
		
		if(stationCache==null)
		{
			logger.error("handleYxChangeInfo not find EpStationCache");
			return ;
		}
		Map<String, GunPointMap> pointMaps = new ConcurrentHashMap<String,GunPointMap>();
		
		for(int i=0;i< singleInfos.size(); i++ )
		{
			SingleInfo info = singleInfos.get(i);
			int address = info.getAddress();//紧凑排列
			
			int pos = stationCache.getGunNo(address,1);
			logger.debug("pos:{}",pos);
			if(pos==0)
				continue;
			
			int nStationIndex = pos/100;
			int epGunNo = pos % 100;
			String epCode = stationCache.getEpCode(nStationIndex);
			
			
			int yxPos = address % 128;
			if(!EpChargeService.isValidAddress(yxPos,1))
			{
				logger.debug("handleOneBitYxInfo static pkId:{},",stationCache.getPkId());
				logger.debug("handleOneBitYxInfo invalid address:{},ycPos:{}",address,yxPos);

				continue;
			}

			
			GunPointMap gunPointMap= pointMaps.get(epCode+epGunNo);
			if(gunPointMap==null)
			{
				gunPointMap = new GunPointMap(epCode,epGunNo);
				pointMaps.put(epCode+epGunNo, gunPointMap);
			}
			
			info.setAddress(yxPos);
			RealChargeInfo.AddPoint(gunPointMap.getPointMap(), info);
		}
		disptachPointToGun(1,pointMaps);
	}
	public static void handleOneBitYxInfo_v4(String epCode,int epGunNo,Channel ch,Vector<SingleInfo> singleInfos)
	{
		EpConcentratorCache stationCache = mapCh2Station.get(ch);
		
		if(stationCache==null)
		{
			logger.error("handleOneBitYxInfo_v4 not find EpStationCache");
			return ;
		}
		GunPointMap gunPointMap= new GunPointMap(epCode,epGunNo);
		
		for(int i=0;i< singleInfos.size(); i++ )
		{
			SingleInfo info = singleInfos.get(i);
			int address = info.getAddress();//紧凑排列
			int yxPos = address % 128;
			if(!EpChargeService.isValidAddress(yxPos,1))
			{
				logger.debug("handleOneBitYxInfo_v4 static pkId:{},",stationCache.getPkId());
				logger.debug("handleOneBitYxInfo_v4 invalid address:{},ycPos:{}",address,yxPos);

				continue;
			}
		}
		EpGunCache epGunCache =  EpGunService.getEpGunCache(epCode, epGunNo);
		if(epGunCache !=null)
		{
			epGunCache.onRealDataChange(gunPointMap.getPointMap(), 1);
		}
	}
	public static void handleTwoBitYxInfo(Channel ch,Vector<SingleInfo> singleInfos)
	{
		EpConcentratorCache stationCache = mapCh2Station.get(ch);
		
		if(stationCache==null)
		{
			logger.error("handleYxChangeInfo not find EpStationCache");
			return ;
			
		}
		
		Map<String, GunPointMap> pointMaps = new ConcurrentHashMap<String,GunPointMap>();
		
		for(int i=0;i< singleInfos.size(); i++ )
		{
			SingleInfo info = singleInfos.get(i);
			int address = info.getAddress();//紧凑排列
			
			int pos = stationCache.getGunNo(address,3);
			logger.debug("pos:{}",pos);
			if(pos==0)
				continue;
			
			int nStationIndex = pos/100;
			int epGunNo = pos % 100;
			String epCode = stationCache.getEpCode(nStationIndex);
			
			
			int yxPos = address % 128;
			if(!EpChargeService.isValidAddress(yxPos,3))
			{
				logger.debug("handleTwoBitYxInfo static pkId:{},",stationCache.getPkId());
				logger.debug("handleTwoBitYxInfo invalid address:{},yxPos:{}",address,yxPos);

				continue;
			}
			
			GunPointMap gunPointMap= pointMaps.get(epCode+epGunNo);
			if(gunPointMap==null)
			{
				gunPointMap = new GunPointMap(epCode,epGunNo);
				pointMaps.put(epCode+epGunNo, gunPointMap);
			}
			
			info.setAddress(yxPos+EpProtocolConstant.YX_2_START_POS);
			RealChargeInfo.AddPoint(gunPointMap.getPointMap(), info);
		}
		disptachPointToGun(1,pointMaps);
	}
	public static void handleTwoBitYxInfo_v4(String epCode,int epGunNo,Channel ch,Vector<SingleInfo> singleInfos)
	{
		EpConcentratorCache stationCache = mapCh2Station.get(ch);
		
		if(stationCache==null)
		{
			logger.error("handleTwoBitYxInfo_v4 not find EpStationCache");
			return ;
			
		}
		
		GunPointMap gunPointMap = new GunPointMap(epCode,epGunNo);
		
		for(int i=0;i< singleInfos.size(); i++ )
		{
			SingleInfo info = singleInfos.get(i);
			int address = info.getAddress();//紧凑排列
			int yxPos = address % 128;
			if(!EpChargeService.isValidAddress(yxPos,3))
			{
				logger.debug("handleTwoBitYxInfo_v4 static pkId:{},",stationCache.getPkId());
				logger.debug("handleTwoBitYxInfo_v4 invalid address:{},yxPos:{}",address,yxPos);

				continue;
			}
			info.setAddress(yxPos+EpProtocolConstant.YX_2_START_POS);
			RealChargeInfo.AddPoint(gunPointMap.getPointMap(), info);
		}
		EpGunCache epGunCache =  EpGunService.getEpGunCache(epCode, epGunNo);
		if(epGunCache !=null)
		{
			epGunCache.onRealDataChange(gunPointMap.getPointMap(), 3);
		}
	}
	public static void handleYcInfo(Channel ch,Vector<SingleInfo> singleInfos)
	{
		EpConcentratorCache stationCache = mapCh2Station.get(ch);
		if(stationCache==null)
		{
			logger.error("handleYcChangeInfo not find EpStationCache");
			return ;
			
		}
		Map<String, GunPointMap> pointMaps = new ConcurrentHashMap<String,GunPointMap>();
		
		for(int i=0;i< singleInfos.size(); i++ )
		{
			SingleInfo info = singleInfos.get(i);
			int address = info.getAddress();//紧凑排列
			
			int pos = stationCache.getGunNo(address,11);
			
			if(pos==0)
			{
				logger.debug("address:{},pos:{}",address,pos);
				continue;
			}
			
			int nStationIndex = pos/100;
			int epGunNo = pos%100;
			
			String epCode = stationCache.getEpCode(nStationIndex);
			
			int ycPos  = address % 2500;
	
			if(!EpChargeService.isValidAddress(ycPos,11))
			{
				logger.debug("handleYcInfo static pkId:{},",stationCache.getPkId());
				logger.debug("handleYcInfo invalid address:{},ycPos:{}",address,ycPos);

				continue;
			}
			
			
			
			GunPointMap gunPointMap= pointMaps.get(epCode+epGunNo);
			if(gunPointMap==null)
			{
				gunPointMap = new GunPointMap(epCode,epGunNo);
				pointMaps.put(epCode+epGunNo, gunPointMap);
			}
			
			
			logger.debug("handleYcChangeInfo,stationCache.getPkId():"+stationCache.getPkId()+",nStationIndex:"+nStationIndex+",address:"+address+",pos:"+pos +",nGunNo:"+epGunNo+",ycPos:"+ycPos+",value:"+info.getIntValue() +"\r\n");
			
			info.setAddress(ycPos+EpProtocolConstant.YC_START_POS);
			
			RealChargeInfo.AddPoint(gunPointMap.getPointMap(), info);
		}
		disptachPointToGun(11,pointMaps);
	}
	
	public static void handleYcInfo_v4(String epCode,int epGunNo, Channel ch,Vector<SingleInfo> singleInfos)
	{
		EpConcentratorCache stationCache = mapCh2Station.get(ch);
		if(stationCache==null)
		{
			logger.error("handleYcInfo_v4 not find EpStationCache");
			return ;
			
		}
		GunPointMap gunPointMap= new GunPointMap(epCode,epGunNo);
		
		for(int i=0;i< singleInfos.size(); i++ )
		{
			SingleInfo info = singleInfos.get(i);
			int address = info.getAddress();//紧凑排列
			
			int ycPos  = address % 2500;
	
			if(!EpChargeService.isValidAddress(ycPos,11))
			{
				logger.debug("handleYcInfo_v4 static pkId:{},",stationCache.getPkId());
				logger.debug("handleYcInfo_v4 invalid address:{},ycPos:{}",address,ycPos);

				continue;
			}
			logger.debug("handleYcInfo_v4,stationCache.getPkId():{},address:{},pos:{},epCode:{},nGunNo:{},value:{}",
					new Object[]{stationCache.getPkId(),address,ycPos,epCode,epGunNo,info.getIntValue()});
			
			info.setAddress(ycPos+EpProtocolConstant.YC_START_POS);
			
			RealChargeInfo.AddPoint(gunPointMap.getPointMap(), info);
		}
		EpGunCache epGunCache =  EpGunService.getEpGunCache(epCode, epGunNo);
		if(epGunCache !=null)
		{
			epGunCache.onRealDataChange(gunPointMap.getPointMap(), 11);
		}
	}
	public static void handleVarYcInfo(Channel ch,Vector<SingleInfo> singleInfos)
	{
		EpConcentratorCache stationCache = mapCh2Station.get(ch);
		if(stationCache==null)
		{
			logger.error("handleYc2ChangeInfo not find EpStationCache");
			return ;
		}
			
		Map<String, GunPointMap> pointMaps = new ConcurrentHashMap<String,GunPointMap>();
		
		for(int i=0;i< singleInfos.size(); i++ )
		{
			SingleInfo info = singleInfos.get(i);
			int address = info.getAddress();//紧凑排列
			
			int pos = stationCache.getGunNo(address,132);
			logger.debug("handleVarYcInfo pos:{}",pos);
			if(pos==0)
				continue;
			
			int nStationIndex = pos/100;
			int epGunNo = pos%100;
			
			String epCode = stationCache.getEpCode(nStationIndex);
			logger.debug("handleYc2ChangeInfo,epCode:{},stationCache:{}",epCode,stationCache);
			
			GunPointMap gunPointMap= pointMaps.get(epCode+epGunNo);
			if(gunPointMap==null)
			{
				gunPointMap = new GunPointMap(epCode,epGunNo);
				pointMaps.put(epCode+epGunNo, gunPointMap);
			}
			int yc2Pos  = address % 128;
			
			if(!EpChargeService.isValidAddress(yc2Pos,132))
			{
				logger.debug("handleVarYcInfo isValidAddress yc2Pos:{},nStationIndex:{}",yc2Pos,stationCache.getPkId());
				continue;
			}
			
			
			
			logger.debug("handleYc2ChangeInfo,address:"+address+",pos:"+pos+",nStationIndex:"+nStationIndex+",nGunNo:"+epGunNo+",yc2Pos:"+yc2Pos+",value:"+info.getIntValue() +"\r\n");
			info.setAddress(yc2Pos+EpProtocolConstant.YC_VAR_START_POS);
			RealChargeInfo.AddPoint(gunPointMap.getPointMap(), info);
		}
		
		disptachPointToGun(132,pointMaps);
	}
	
	public static void handleVarYcInfo_v4(String epCode,int epGunNo, Channel ch,Vector<SingleInfo> singleInfos)
	{
		EpConcentratorCache stationCache = mapCh2Station.get(ch);
		if(stationCache==null)
		{
			logger.error("handleVarYcInfo_v4 not find EpStationCache");
			return ;
		}
			
		GunPointMap gunPointMap = new GunPointMap(epCode,epGunNo);
		
		for(int i=0;i< singleInfos.size(); i++ )
		{
			SingleInfo info = singleInfos.get(i);
			int address = info.getAddress();//紧凑排列
			int yc2Pos  = address % 128;
			
			if(!EpChargeService.isValidAddress(yc2Pos,132))
			{
				logger.debug("handleVarYcInfo_v4 isValidAddress yc2Pos:{},nStationIndex:{}",yc2Pos,stationCache.getPkId());
				continue;
			}
			
			logger.debug("handleVarYcInfo_v4,stationCache.getPkId():{},address:{},pos:{},epCode:{},nGunNo:{},value:{}",
					new Object[]{stationCache.getPkId(),address,yc2Pos,epCode,epGunNo,info.getIntValue()});
		
			info.setAddress(yc2Pos+EpProtocolConstant.YC_VAR_START_POS);
			RealChargeInfo.AddPoint(gunPointMap.getPointMap(), info);
		}
		EpGunCache epGunCache =  EpGunService.getEpGunCache(epCode, epGunNo);
		if(epGunCache !=null)
		{
			epGunCache.onRealDataChange(gunPointMap.getPointMap(), 11);
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
	
	public static void checkTimeOut()
	{
		Iterator iter = mapCh2Station.entrySet().iterator();
		
		while (iter.hasNext()) {
			
			Map.Entry entry = (Map.Entry) iter.next();
			
			EpCommClient commClient=(EpCommClient) entry.getValue();
			
			if(commClient==null)
			{
				continue;	
			}
			long now = DateUtil.getCurrentSeconds();
			long lastTime = commClient.getLastUseTime();
			long diff = now - lastTime;
			
			if(commClient.getStatus() < 2  )//只有链接的，没有初始化，10秒钟算超时
			{
				
				if( diff > 10)
				{
					logger.debug("commClient.getStatus():{},diff:{}",commClient.getStatus(),diff);
					Channel channel= commClient.getChannel();
					if(channel!=null)
					{
						channel.close();
					}
					iter.remove();
				}
			
			}
			else
			{
				if(diff<EpConstant.EP_CONNECT_TIMEOUT)//40秒后更新状态
				{
					continue;
				}
				logger.info("check comm client({}),diff:{}",commClient.getIdentity(),diff);
				
				commClient.setStatus(0);
				
				String clientIdentity = commClient.getIdentity();
				
				Channel channel= commClient.getChannel();
				if(channel==null)
				{
					iter.remove();
				}
				else
				{
					handleCommClientTimeOut(channel,clientIdentity,0);
					
					logger.info("check station client({}:{})timeout!",commClient.getIdentity(),channel.toString());
					
					channel.close();
					iter.remove();
				}
					
									
			}
		
		}
		
	}
	
	public static void handleCommClientTimeOut(Channel channel,String clientIdentity,int gateId)
	{
		if(clientIdentity==null || clientIdentity.length()<1)
		{
			logger.info("handleCommClientTimeOut clientIdentity is empty");
			return;
		}

		int pkConcentratorId = Integer.parseInt(clientIdentity);
		
		EpConcentratorCache conCentrator= mapStation.get(pkConcentratorId);
		if(conCentrator!=null)
		{
			conCentrator.onEpCommClientDelete();
		}
		RemoveConCentrator(channel);
		logger.info("handleCommClientTimeOut,clientIdentity:{}",clientIdentity);
		EpService.updateEpsCommStatusToDb(pkConcentratorId, 0, gateId);
		
		
		
		
	}
	
	public static void concentratorConfig(int concentratorId)
	{
		EpConcentratorCache epStation = getConCentrator(concentratorId);
		if(epStation == null)
		{
			logger.error("concentratorConfig,epStation is null,stationId:{}",concentratorId);
		}
		
		List<TblElectricPile> epList = DB.epClientDao.getEpsByStatus(concentratorId);

		//做协议逻辑
		EpCommClient commClient = EpCommClientService.getCommClient(""+concentratorId);
		String epCodes="";
		if(epList==null || epList.size()<=0)
		{
			epStation.setEpCount(0);
		}
		else
		{
		   epStation.setEpCount(epList.size());
		   try{
		   for(int i=0;i< epList.size();i++)
		      {
			   TblElectricPile tblEp = epList.get(i);
			   epStation.setEpGunNum(tblEp.getStationIndex(), tblEp.getEpGunNum());
			   epStation.addEpCode(tblEp.getStationIndex(), tblEp.getEpCode());
			   ElectricPileCache epCache = EpService.getEpByCode(tblEp.getEpCode());
			   if(epCache == null)
			   {
			      epCache = EpService.getElectricPileCache(tblEp);
			      if(epCache.initGuns())
			      {
				     EpService.addEpCache(epCache);
				  }
			   }
			   epCache.setEpNetObject(commClient);
			   epCache.setConcentratorId(concentratorId);
			   
			   epCodes=epCodes+tblEp.getEpCode()+",";
		     }
			}catch (Exception e) {e.printStackTrace();}
		}
		if(commClient ==null || !commClient.isComm())
		{
			logger.error("concentratorConfig,commClient is close,stationId:{}",concentratorId);	
			return;
		}
		int epCount = epStation.getEpCount();
		byte[]data = EpEncodeProtocol.do_concentroter_setep((short)concentratorId,(short)epCount, epCodes); 
		byte[] cmdTimes = WmIce104Util.timeToByte();
		EpMessageSender.sendMessage(commClient,0,0, Iec104Constant.C_CONCENTROTER_SET_EP,data,cmdTimes,commClient.getCommVersion());
	
	}
	
	public static void updateAllCommStatus()
	{
		DB.epClientDao.updateAllCommStatus();
	}

}
