package com.webgate.service;

import io.netty.channel.Channel;

import java.util.Iterator;
import java.util.Map;
import java.util.Map.Entry;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.TimeUnit;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.netCore.core.pool.TaskPoolFactory;
import com.netCore.queue.RepeatConQueue;
import com.netCore.queue.RepeatMessage;
import com.webgate.cache.ElectricPileCache;
import com.webgate.cache.EpGunCache;
import com.webgate.cache.UserCache;
import com.webgate.cache.UserRealInfo;
import com.webgate.config.GameConfig;
import com.webgate.constant.BaseConstant;
import com.webgate.net.client.EpGateNetConnect;
import com.webgate.net.client.PhoneClient;
import com.webgate.task.CheckEpGateNetTimeOutTask;
import com.webgate.utils.DateUtil;

public class CacheService {
	
	private static final Logger logger = LoggerFactory.getLogger(CacheService.class + BaseConstant.SPLIT + GameConfig.serverName);
	
	public static void startEpGateCommTimer(long initDelay) {
		CheckEpGateNetTimeOutTask checkTask =  new CheckEpGateNetTimeOutTask();
				
		TaskPoolFactory.scheduleAtFixedRate("CHECK_EPGATENET_TIMEOUT_TASK", checkTask, initDelay, 10, TimeUnit.SECONDS);
	}
	
	/**
	 * EpGate缓存
	 */
	private static Map<Integer, EpGateNetConnect> epGateCommClents = new ConcurrentHashMap<Integer, EpGateNetConnect>();
	public static Map<Integer, EpGateNetConnect> getMapEpGate()
	{
		return epGateCommClents;
	}
	
	public static EpGateNetConnect getEpGate(int pkGateId )
	{
		return epGateCommClents.get(pkGateId);
	}
	public static void addEpGate(int pkGateId, EpGateNetConnect epGateClient)
	{
		epGateCommClents.put(pkGateId,epGateClient);
	}
	public static void removeEpGate(int pkGateId)
	{
		epGateCommClents.remove(pkGateId);
	}
	public static EpGateNetConnect getEpGate(String epCode )
	{
		ElectricPileCache epCache = getEpCache(epCode);
		if(epCache==null|| epCache.getGateid()==0)
			return null;
		
		return epGateCommClents.get(epCache.getGateid());
	}

	private static Map<Channel ,EpGateNetConnect> epGateChannel = new ConcurrentHashMap<Channel, EpGateNetConnect>();
	public static EpGateNetConnect getEpGateByCh(Channel channel)
	{
		return epGateChannel.get(channel);
	}
	public static void addEpGateByCh(Channel channel, EpGateNetConnect epGateClient)
	{
		epGateChannel.put(channel, epGateClient);
	}
	public static void removeEpGateByCh(Channel channel)
	{
		epGateChannel.remove(channel);
	}
	
	@SuppressWarnings("unused")
	private static RepeatConQueue phoneReSendMsgQue = new RepeatConQueue();
	
	public static void putPhoneRepeatMsg(RepeatMessage mes)
	{
		logger.debug("putPhoneRepeatMsg,key:{}",mes.getKey());
		logger.debug("putPhoneRepeatMsg phoneReSendMsgQue,{}",phoneReSendMsgQue.count());
		phoneReSendMsgQue.put(mes);
		logger.debug("putRepeatMsg phoneReSendMsgQue,{}",phoneReSendMsgQue.count());
	}
	public static void putPhoneSendMsg(RepeatMessage mes)
	{
		logger.debug("putPhoneRepeatMsg,key:{}",mes.getKey());
		logger.debug("putPhoneRepeatMsg repeatMsgQue,{}",epGateReSendMsgQue.count());
		phoneReSendMsgQue.putSend(mes);
		logger.debug("putPhoneRepeatMsg repeatMsgQue,{}",epGateReSendMsgQue.count());
	}
	public static void removePhoneRepeatMsg(String key)
	{
		logger.debug("removeRepeatMsg,key:{}",key);
		logger.debug("removeRepeatMsg start phoneReSendMsgQue,{}",phoneReSendMsgQue.count());
		phoneReSendMsgQue.remove(key);
		logger.debug("removeRepeatMsg end phoneReSendMsgQue,{}",phoneReSendMsgQue.count());
	}
	
	private static RepeatConQueue epGateReSendMsgQue = new RepeatConQueue();
	
	public static void putEpGateRepeatMsg(RepeatMessage mes)
	{
		logger.debug("putEpGateRepeatMsg,key:{}",mes.getKey());
		logger.debug("putEpGateRepeatMsg epGateReSendMsgQue,{}",epGateReSendMsgQue.count());
		epGateReSendMsgQue.put(mes);
		logger.debug("putEpGateRepeatMsg epGateReSendMsgQue,{}",epGateReSendMsgQue.count());
	}
	public static void putEpSendMsg(RepeatMessage mes)
	{
		logger.debug("putEpSendMsg,key:{}",mes.getKey());
		logger.debug("putEpSendMsg repeatMsgQue,{}",epGateReSendMsgQue.count());
		epGateReSendMsgQue.putSend(mes);
		logger.debug("putEpSendMsg repeatMsgQue,{}",epGateReSendMsgQue.count());
	}
	public static void removeEpRepeatMsg(String key)
	{
		logger.debug("removeEpRepeatMsg,key:{}",key);
		logger.debug("removeEpRepeatMsg start repeatMsgQue,{}",epGateReSendMsgQue.count());
		epGateReSendMsgQue.remove(key);
		logger.debug("removeEpRepeatMsg end repeatMsgQue,{}",epGateReSendMsgQue.count());
	}
	
	//电桩
	private static Map<String,ElectricPileCache> mapEpCache = new ConcurrentHashMap<String, ElectricPileCache>();
	public static Map<String,ElectricPileCache> getMapEpCache()
	{
		return mapEpCache;
	}
	public static int getCurrentEpCount()
	{
		return mapEpCache.size();
	}
	public  static void addEpCache(ElectricPileCache epCache){
		if(epCache!=null)
		{
			String epCode= epCache.getCode();
	
			mapEpCache.put(epCode,epCache);
		}
	}
    public static ElectricPileCache getEpCache(String epCode){
		ElectricPileCache electricUser = mapEpCache.get(epCode);
		return electricUser;
	}
	
    private static Map<String, EpGunCache> mapGun = new ConcurrentHashMap<String,EpGunCache>();
	public static Map<String, EpGunCache> getMapGun()
	{
		return mapGun;
	}
	public static EpGunCache getEpGunCache(String epCode,int epGunNo)
	{
		String combEpGunNo = epCode+ epGunNo;
		return mapGun.get(combEpGunNo);
	}
	public static void putEpGunCache(String epCode,int epGunNo,EpGunCache cache)
	{
		if(cache !=null && epCode !=null)
		{
			String combEpGunNo = epCode+ epGunNo;
			mapGun.put(combEpGunNo, cache);
			
		}
	}

	private static Map<Channel,PhoneClient> mapCh2PhoneClient = new ConcurrentHashMap<Channel, PhoneClient>();
	public static Map<Channel,PhoneClient> getMapCh2PhoneClient()
	{
		return mapCh2PhoneClient;
	}
	private static Map<Integer,PhoneClient> mapPhoneClients=new ConcurrentHashMap<Integer,PhoneClient>();
	public static Map<Integer,PhoneClient> getMapPhoneClient()
	{
		return mapPhoneClients;
	}
	
	public static int getCh2PhoneClientCount()
	{
		return mapCh2PhoneClient.size();
	}
	
	public static int getPhoneClientCount()
	{
		return mapPhoneClients.size();
	}
	
	public static void addPhoneClient(PhoneClient commClient)
	{
		 if(commClient==null || commClient.getChannel()==null)
			return;
		 mapCh2PhoneClient.put(commClient.getChannel(), commClient);
	}
	public static void addPhoneClientByAccount(PhoneClient commClient)
	{
		PhoneClient commClient1 = mapPhoneClients.get(commClient.getAccountId());
		if(commClient1!=null)
		{
			mapCh2PhoneClient.remove(commClient1.getChannel());
		}
		
		PhoneClient commClient2 = mapCh2PhoneClient.get(commClient.getChannel());
		 if(commClient2==null)
			return;
		 mapPhoneClients.put(commClient.getAccountId(), commClient);
	}
	public static PhoneClient getPhoneClientByChannel(Channel ch)
	{
		 return mapCh2PhoneClient.get(ch);
	}
	public static PhoneClient getPhoneClientByAccountId(int accountId)
	{
		 return mapPhoneClients.get(accountId);
	}
	public static PhoneClient removePhoneClientByAccountId(int accountId)
	{
		 return mapPhoneClients.remove(accountId);
	}
	public static void removePhoneClientByChannel(Channel ch)
	{
		mapCh2PhoneClient.remove(ch);
	}
	
	private static Map<String, UserCache> epUserInfoMap = new ConcurrentHashMap<String,UserCache>();
	private static Map<Integer, UserCache> epUserInfo2Map = new ConcurrentHashMap<Integer,UserCache>();
	public static Map<String, UserCache> getMapEpUserInfo()
	{
		return epUserInfoMap;
	}
	public static Map<Integer, UserCache> getMapEpUserInfo2()
	{
		return epUserInfo2Map;
	}
	
	public static void putUserCache(UserCache userCache)
	{
		if(userCache!=null)
		{
			epUserInfoMap.put(userCache.getAccount(), userCache);
			epUserInfo2Map.put(userCache.getId(), userCache);
		}
		
	}
	public static UserCache convertToCache(UserRealInfo userRealInfo)
	{
		if(userRealInfo == null)
			return null;
		
		String userAccount=  userRealInfo.getAccount();
		int userId = userRealInfo.getId();
		
		UserCache u= new UserCache(userId,userAccount,userRealInfo.getLevel());
		
		if(epUserInfoMap.get(userAccount)==null)
		{
			epUserInfoMap.put(userAccount, u);
		}
		if(epUserInfo2Map.get(userId)==null)
		{
			epUserInfo2Map.put(userId, u);
		}
		
		return u;
	}

	public static void checkEpGateTimeOut()
	{
		Iterator<Entry<Integer ,EpGateNetConnect>> iter = epGateCommClents.entrySet().iterator();
		
		while (iter.hasNext()) {
			Map.Entry entry = (Map.Entry) iter.next();
			
			EpGateNetConnect commClient=(EpGateNetConnect) entry.getValue();
			if(commClient==null)
			{
				logger.error("EpGateNetConnect checkTimeOut commClient==null");
				continue ;
			}
		
			//需要连接
			boolean bNeedReConnect=false;
			//检查
			long connectDiff = DateUtil.getCurrentSeconds() - commClient.getLastUseTime();
			//检查连接，是否需要重连
			int commStatus = commClient.getStatus();
			if(commStatus == 0 || commStatus == 1)
			{
				int times = (commClient.getConnectTimes() / 6) + 1;
				if (connectDiff>(times * GameConfig.reconnectInterval))
				{
					bNeedReConnect  = true;
				}
			}
			else
			{
				if(connectDiff>GameConfig.netKeepLiveInterval)
				{
					bNeedReConnect  = true;
				}
				
			}
		
			if(bNeedReConnect)
			{
				commClient.reconnection();
			}
			
			long now = DateUtil.getCurrentSeconds();
			//1.检查连接是否活动,不活动的话发送心跳侦
			long activeDiff = now - commClient.getLastSendTime();
			if (activeDiff >= GameConfig.heartInterval) {
				commClient.setLastSendTime(now);
				if(commClient.getChannel() != null)
				{
					EpGateService.sendHeart(commClient.getChannel());
				}
			}
		}
	}
}
