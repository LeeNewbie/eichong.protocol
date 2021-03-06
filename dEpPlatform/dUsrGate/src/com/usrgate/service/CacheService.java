package com.usrgate.service;

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
import com.usrgate.cache.ElectricPileCache;
import com.usrgate.cache.EpGunCache;
import com.usrgate.cache.FlowStat;
import com.usrgate.cache.FlowStatistics;
import com.usrgate.cache.InstructStat;
import com.usrgate.cache.InstructStatistics;
import com.usrgate.cache.UserCache;
import com.usrgate.cache.UserRealInfo;
import com.usrgate.config.CycleConfigs;
import com.usrgate.config.GameConfig;
import com.usrgate.constant.BaseConstant;
import com.usrgate.net.client.EpGateNetConnect;
import com.usrgate.net.client.PhoneClient;
import com.usrgate.protocol.PhoneConstant;
import com.usrgate.task.CheckEpGateNetTimeOutTask;
import com.usrgate.task.CheckEpInstructStatTimeOutTask;
import com.usrgate.task.CheckEpStatisticsTimeOutTask;
import com.usrgate.task.CheckInstructStatTimeOutTask;
import com.usrgate.task.CheckStatisticsTimeOutTask;
import com.usrgate.utils.DateUtil;

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

	private static CycleConfigs configs;

	public static CycleConfigs getCycleConfigs() {
		return configs;
	}

	public static void setCycleConfigs(CycleConfigs cycleConfigs) {
		configs = cycleConfigs;
	}

	@SuppressWarnings("rawtypes")
	public static void setFlowStat(Channel channel, int msgLen, boolean sendFlag) {
		PhoneClient phoneClient = CacheService.getPhoneClientByChannel(channel);
		if (phoneClient != null) {
			Iterator<Entry<Integer ,Integer>> iter = getCycleConfigs().getFlowcfgs().entrySet().iterator();
			while (iter.hasNext()) {
				Map.Entry entry = (Map.Entry) iter.next();
				int cycle = (int) entry.getKey();
				FlowStatistics flowStat = phoneClient.getFlowStat(cycle);
				if (flowStat == null) flowStat = new FlowStatistics(cycle);
				if (sendFlag) flowStat.addSendFlow(msgLen, phoneClient.getIdentity());
				else flowStat.addReceiveFlow(msgLen, phoneClient.getIdentity());
				phoneClient.setFlowStat(flowStat);
			}
		}
	}

	@SuppressWarnings("rawtypes")
	public static void setInstructStat(PhoneClient phoneClient, int cmd, int version) {
		Iterator<Entry<Integer ,Integer>> iter = getCycleConfigs().getInstructcfgs().entrySet().iterator();
		while (iter.hasNext()) {
			Map.Entry entry = (Map.Entry) iter.next();
			int cycle = (int) entry.getKey();
			InstructStatistics instructStat = phoneClient.getInstructStat(cycle);
			if (instructStat == null) instructStat = new InstructStatistics(cycle);
			if (cmd != PhoneConstant.D_VERSION_CONNET_EP) {
				instructStat.addInstructCount(cmd, phoneClient.getIdentity());
			} else {
				instructStat.addInstructCount(version, cmd, phoneClient.getAccount());
			}
			phoneClient.setInstructStat(instructStat);
		}
	}

	@SuppressWarnings("rawtypes")
	public static void setEpFlowStat(Channel channel, int msgLen, boolean sendFlag) {
		EpGateNetConnect commClient = getEpGateByCh(channel);
		if (commClient != null) {
			Iterator<Entry<Integer ,Integer>> iter = getCycleConfigs().getEpFlowcfgs().entrySet().iterator();
			while (iter.hasNext()) {
				Map.Entry entry = (Map.Entry) iter.next();
				int cycle = (int) entry.getKey();
				FlowStatistics flowStat = commClient.getFlowStat(cycle);
				if (flowStat == null) flowStat = new FlowStatistics(cycle);
				if (sendFlag) flowStat.addSendFlow(msgLen, commClient.getIdentity());
				else flowStat.addReceiveFlow(msgLen, commClient.getIdentity());
				commClient.setFlowStat(flowStat);
			}
		}
	}

	@SuppressWarnings("rawtypes")
	public static void setEpInstructStat(Channel channel, int cmd) {
		EpGateNetConnect commClient = getEpGateByCh(channel);
		if (commClient != null) {
			Iterator<Entry<Integer ,Integer>> iter = getCycleConfigs().getEpInstructcfgs().entrySet().iterator();
			while (iter.hasNext()) {
				Map.Entry entry = (Map.Entry) iter.next();
				int cycle = (int) entry.getKey();
				InstructStatistics instructStat = commClient.getInstructStat(cycle);
				if (instructStat == null) instructStat = new InstructStatistics(cycle);
				instructStat.addInstructCount(cmd, commClient.getIdentity());
				commClient.setInstructStat(instructStat);
			}
		}
	}

	private static Map<Integer, FlowStat> phoneMap = new ConcurrentHashMap<Integer, FlowStat>();
	private static Map<Integer, FlowStat> epMap = new ConcurrentHashMap<Integer, FlowStat>();
	
	public static void startStatisticsTimer(long initDelay, int cycle) {
		if (phoneMap.get(cycle) == null) phoneMap.put(cycle, new FlowStat(cycle));
		CheckStatisticsTimeOutTask checkTask =  new CheckStatisticsTimeOutTask(cycle);
				
		TaskPoolFactory.scheduleAtFixedRate("CHECK_STATISTICS_TIMEOUT_TASK", checkTask, initDelay, cycle, TimeUnit.SECONDS);
	}

	@SuppressWarnings("rawtypes")
	public static void checkStatisticsTimeOut(int cycle)
	{
		Iterator<Entry<Channel ,PhoneClient>> iter = mapCh2PhoneClient.entrySet().iterator();

		FlowStatistics flowMax = new FlowStatistics(cycle);
		FlowStatistics flowTotal = new FlowStatistics(cycle);
		while (iter.hasNext()) {
			Map.Entry entry = (Map.Entry) iter.next();
			
			PhoneClient phoneClient = (PhoneClient) entry.getValue();
			if (phoneClient == null) continue;

			FlowStatistics flowStat = phoneClient.getFlowStat(cycle);
			if (flowStat == null) continue;

			flowMax.setMaxFlow(flowStat);
			flowTotal.addTotalFlow(flowStat);
			flowStat.clearFlow();
		}
		if (flowTotal.getTotalFlow() > 0) {
			logger.info("phone channel max flow:\n{}", flowMax);
			logger.info("allPhone channel max total flow:\n{}", flowTotal);
		}
	
		FlowStat phoneStat = phoneMap.get(cycle);
		if (phoneStat.getFlowTotal().getTotalFlow() > 0 || flowTotal.getTotalFlow() > 0) {
			phoneStat.setFlowMax(flowMax);
			phoneStat.setFlowTotal(flowTotal);
			logger.info("手机流量统计:\n{}", phoneStat);
		}
	}
	
	public static void startEpStatisticsTimer(long initDelay, int cycle) {
		if (epMap.get(cycle) == null) epMap.put(cycle, new FlowStat(cycle));
		CheckEpStatisticsTimeOutTask checkTask =  new CheckEpStatisticsTimeOutTask(cycle);
				
		TaskPoolFactory.scheduleAtFixedRate("CHECK_EPSTATISTICS_TIMEOUT_TASK", checkTask, initDelay, cycle, TimeUnit.SECONDS);
	}

	@SuppressWarnings("rawtypes")
	public static void checkEpStatisticsTimeOut(int cycle)
	{
		Iterator<Entry<Channel ,EpGateNetConnect>> epIt = epGateChannel.entrySet().iterator();

		FlowStatistics flowMax = new FlowStatistics(cycle);
		FlowStatistics flowTotal = new FlowStatistics(cycle);
		while (epIt.hasNext()) {
			Map.Entry entry = (Map.Entry) epIt.next();
			
			EpGateNetConnect commClient = (EpGateNetConnect) entry.getValue();
			if (commClient == null) continue;

			FlowStatistics flowStat = commClient.getFlowStat(cycle);
			if (flowStat == null) continue;

			flowMax.setMaxFlow(flowStat);
			flowTotal.addTotalFlow(flowStat);
			flowStat.clearFlow();
		}
		if (flowTotal.getTotalFlow() > 0) {
			logger.info("electricpile channel max flow:\n{}", flowMax);
			logger.info("allElectricpile channel max total flow:\n{}", flowTotal);
		}
	
		FlowStat epStat = epMap.get(cycle);
		if (epStat.getFlowTotal().getTotalFlow() > 0 || flowTotal.getTotalFlow() > 0) {
			epStat.setFlowMax(flowMax);
			epStat.setFlowTotal(flowTotal);
			logger.info("电桩流量统计:\n{}", epStat);
		}
	}

	private static Map<Integer, InstructStat> phoneInstMap = new ConcurrentHashMap<Integer, InstructStat>();
	private static Map<Integer, InstructStat> epInstMap = new ConcurrentHashMap<Integer, InstructStat>();
	
	public static void startInstructStatTimer(long initDelay, int cycle) {
		if (phoneInstMap.get(cycle) == null) phoneInstMap.put(cycle, new InstructStat(cycle));
		CheckInstructStatTimeOutTask checkTask =  new CheckInstructStatTimeOutTask(cycle);
				
		TaskPoolFactory.scheduleAtFixedRate("CHECK_INSTRUCTSTAT_TIMEOUT_TASK", checkTask, initDelay, cycle, TimeUnit.SECONDS);
	}

	@SuppressWarnings("rawtypes")
	public static void checkInstructStatTimeOut(int cycle)
	{
		Iterator<Entry<Channel ,PhoneClient>> iter = mapCh2PhoneClient.entrySet().iterator();

		InstructStatistics instructMax = new InstructStatistics(cycle);
		InstructStatistics instructTotal = new InstructStatistics(cycle);
		while (iter.hasNext()) {
			Map.Entry entry = (Map.Entry) iter.next();
			
			PhoneClient phoneClient = (PhoneClient) entry.getValue();
			if (phoneClient == null) continue;

			InstructStatistics instructStat = phoneClient.getInstructStat(cycle);	
			if (instructStat == null) continue;

			instructMax.setMaxInstruct(instructStat);
			instructTotal.addTotalInstruct(instructStat);
			instructStat.clearInstruct();
		}
		if (instructTotal.getInstructCount() > 0) {
			logger.info("phone channel max instruct:\n{}", instructMax);
			logger.info("allPhone channel max total instruct:\n{}", instructTotal);
		}

		InstructStat phoneInstruct = phoneInstMap.get(cycle);
		if (phoneInstruct.getInstructTotal().getInstructCount() > 0 || instructTotal.getInstructCount() > 0) {
			phoneInstruct.setInstructMax(instructMax);
			phoneInstruct.setInstructTotal(instructTotal);
			logger.info("手机指令统计:\n{}", phoneInstruct);
		}
	}
	
	public static void startEpInstructStatTimer(long initDelay, int cycle) {
		if (epInstMap.get(cycle) == null) epInstMap.put(cycle, new InstructStat(cycle));
		CheckEpInstructStatTimeOutTask checkTask =  new CheckEpInstructStatTimeOutTask(cycle);
				
		TaskPoolFactory.scheduleAtFixedRate("CHECK_EPINSTRUCTSTAT_TIMEOUT_TASK", checkTask, initDelay, cycle, TimeUnit.SECONDS);
	}

	@SuppressWarnings("rawtypes")
	public static void checkEpInstructStatTimeOut(int cycle)
	{
		Iterator<Entry<Channel ,EpGateNetConnect>> epIt = epGateChannel.entrySet().iterator();

		InstructStatistics instructMax = new InstructStatistics(cycle);
		InstructStatistics instructTotal = new InstructStatistics(cycle);
		while (epIt.hasNext()) {
			Map.Entry entry = (Map.Entry) epIt.next();
			
			EpGateNetConnect commClient = (EpGateNetConnect) entry.getValue();
			if (commClient == null) continue;

			InstructStatistics instructStat = commClient.getInstructStat(cycle);
			if (instructStat == null) continue;

			instructMax.setMaxInstruct(instructStat);
			instructTotal.addTotalInstruct(instructStat);
			instructStat.clearInstruct();
		}
		if (instructTotal.getInstructCount() > 0) {
			logger.info("electricpile channel max instruct:\n{}", instructMax);
			logger.info("allElectricpile channel max total instruct:\n{}", instructTotal);
		}
	
		InstructStat epInstruct = epInstMap.get(cycle);
		if (epInstruct.getInstructTotal().getInstructCount() > 0 || instructTotal.getInstructCount() > 0) {
			epInstruct.setInstructMax(instructMax);
			epInstruct.setInstructTotal(instructTotal);
			logger.info("电桩指令统计:\n{}", epInstruct);
		}
	}
}
