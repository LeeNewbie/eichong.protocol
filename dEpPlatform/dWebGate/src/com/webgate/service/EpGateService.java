package com.webgate.service;

import io.netty.channel.Channel;

import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.TimeUnit;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.netCore.core.pool.TaskPoolFactory;
import com.netCore.model.conf.ClientConfig;
import com.ormcore.dao.DB;
import com.ormcore.model.TblEpGateConfig;
import com.webgate.cache.ElectricPileCache;
import com.webgate.config.EpGateConfig;
import com.webgate.config.GameConfig;
import com.webgate.constant.BaseConstant;
import com.webgate.constant.CommStatusConstant;
import com.webgate.constant.EpGateConstant;
import com.webgate.net.client.EpGateNetConnect;
import com.webgate.net.codec.EpGateEncoder;
import com.webgate.net.sender.EpGateMessageSender;
import com.webgate.task.ScanEpGateTask;
import com.webgate.utils.DateUtil;
import com.webgate.utils.NetUtils;

public class EpGateService {
	
	private static final Logger logger = LoggerFactory.getLogger(EpGateService.class + BaseConstant.SPLIT + GameConfig.serverName);
	
	private static Map<Integer ,EpGateConfig> epGateConfs = new ConcurrentHashMap<Integer, EpGateConfig>();
	
	public static boolean isValidCmd(int cmd)
	{
		if( 
		cmd == EpGateConstant.EP_LOGIN||
		cmd == EpGateConstant.EP_ACK||
		cmd == EpGateConstant.EP_HEART||
		cmd == EpGateConstant.EP_ONLINE||
		cmd == EpGateConstant.EP_PHONE_ONLINE||
		cmd == EpGateConstant.EP_PHONE_CONNECT_INIT||
		cmd == EpGateConstant.EP_CHARGE||
		cmd == EpGateConstant.EP_CHARGE_EVENT||
		cmd == EpGateConstant.EP_STOP_CHARGE||
		cmd == EpGateConstant.EP_CHARGE_REAL||
		cmd == EpGateConstant.EP_CONSUME_RECORD||
		cmd == EpGateConstant.EP_RE_INSERT_GUN||
		cmd == EpGateConstant.EP_YXYC)
			return true;
		return false;
	}
	
	public static void startScanEpGate(long initDelay) {

		ScanEpGateTask checkTask = new ScanEpGateTask();

		TaskPoolFactory.scheduleAtFixedRate("CHECK_EPGATE_SERVICE_TASK",
				checkTask, initDelay, 30, TimeUnit.SECONDS);
	}
	public static void scanEpGate()
	{
		TblEpGateConfig cfg= new TblEpGateConfig();
		List<TblEpGateConfig> epGateCfgList = DB.epGateCfgDao.find1(cfg);
		
		logger.debug("epGateCfgList size:{}",epGateCfgList.size());
		
		connectAllGate(epGateCfgList);
	}
	
	public static EpGateConfig getEpGateCfg(int gateId)
	{
		return epGateConfs.get(gateId);
	}
	
	public static void addEpGateCfg(int gateId,EpGateConfig object)
	{
		if(gateId==0 || object==null)
		{
			logger.info("addGateConnectObject fail");
			return;
		}
		epGateConfs.put(gateId,object);
	}
	public static void removeEpGateCfg(int gateId)
	{
		epGateConfs.remove(gateId);
		
	}
	public static int getSize()
	{
		return epGateConfs.size();
	}
	public static void EpGateConnect(TblEpGateConfig tblEpGateCfg)
	{
		int gateId = tblEpGateCfg.getPkGateid();
		int gateState = tblEpGateCfg.getGateState();
		
		logger.debug("EpGateConnect ,gateId:{},gateState:{}",gateId,gateState);
		
		if( gateState==1 ){//创建一个Channel
			
			EpGateConfig  epGateCfg= getEpGateCfg(gateId);
			//内存中没有的话，加上
			if(epGateCfg==null)
			{
				//log.info("changeGateChannelCache gateId:"+gateId+"gateConnectionObject==null");
				epGateCfg=new EpGateConfig();
				
				epGateCfg.setEpGateId(gateId);
				epGateCfg.setIp(tblEpGateCfg.getGtseGateip());
				epGateCfg.setPort(tblEpGateCfg.getGtseGateport());
				//epGateCfg.setState(tblEpGateCfg.getGateState());
				
				addEpGateCfg(gateId, epGateCfg);
			}
			//log.info("changeGateChannelCache connect times:"+gateConnectionObject.getConnTimes());
			//失败次数大于6次不重连
			EpGateNetConnect epGateCommClient= CacheService.getEpGate(gateId);
			if(epGateCommClient == null  )
			{
	
				ClientConfig clrCfg = new ClientConfig();
				clrCfg.setIp(epGateCfg.getIp());
				clrCfg.setPort(epGateCfg.getPort());
				epGateCommClient=  EpGateNetConnect.getNewInstance(clrCfg);
				
				logger.debug("connectAllGate ,ip:{},port:{}",clrCfg.getIp(),clrCfg.getPort());
				
				epGateCommClient.start();
				
				CacheService.addEpGate(epGateCfg.getEpGateId(),epGateCommClient);
			}
			//在连接函数中增加到列表里
		}
		else//所有不等于1的，都认为关闭
		//if(gateState == 2)
		{
			EpGateNetConnect  epGateCommClient= CacheService.getEpGate(gateId);
			if(epGateCommClient!=null)
			{
				//log.info("changeGateChannelCache gateChannel:"+gateChannel);
				
				//epGateCommClient.close();//关闭连接
				CacheService.removeEpGate(gateId);//移除MAP数据
			}
			else
			{
				//log.info("changeGateChannelCache GATE_TO_CHANNEL_INFO do not find gateId:"+gateId);
			}
			//
			EpGateConfig  epGateCfg= epGateConfs.get(gateId);
			if(epGateCfg!=null)
			{
				//log.info("changeGateChannelCache connect times:"+gateConnectionObject.getConnTimes());
				epGateConfs.remove(gateId);
			}
		}
		
	}
	public static void connectAllGate(List<TblEpGateConfig> gateList)
	{
		//遍历内存在中有，但数据库中没有的
    	syncDb(gateList);
    	
    	int count = gateList.size();
    	
    	//判断数据库中增加和删除
		for (int i=0;i<count;i++) {//限制次数
			try {
				TblEpGateConfig tblEpGateCfg= gateList.get(i); 
				EpGateConnect(tblEpGateCfg);
				
			} catch (Exception e) {
				e.printStackTrace();
				logger.error("更新GATE-Channel异常：", e);
			}
		}
	}
	
	public static void syncDb(List<TblEpGateConfig> gateDbList)
	{
		logger.info("syncDb 1 gateDbList size:{}",gateDbList.size());
		@SuppressWarnings("rawtypes")
		Iterator iter = epGateConfs.entrySet().iterator();
		while (iter.hasNext()) {
			@SuppressWarnings("rawtypes")
			Map.Entry entry = (Map.Entry) iter.next();
			logger.info("syncDb 2 gateDbList size:{}",gateDbList.size());
			EpGateConfig gateObject=(EpGateConfig) entry.getValue();
			if(null == gateObject)
			{
				continue;
			}
			int  gateId1 = (int)entry.getKey();
			boolean find=false;
			for(TblEpGateConfig gate:gateDbList){
				int gateId2 = gate.getPkGateid();
				if(gateId1== gateId2)
				{
					find=true;
					break;
				}
			}
			logger.info("syncDb 3 gateDbList size:{}",gateDbList.size());
			if(find==false)//移除掉
			{
				EpGateNetConnect epGateCommClient =  CacheService.getEpGate(gateId1);
				if(epGateCommClient!=null)
				{
					//epGateCommClient.close();
				}
				CacheService.removeEpGate(gateId1);//移除MAP数据
				iter.remove();
				epGateConfs.remove(gateId1);      
			}
		}
		
	}

	/**
	 * 网关登录请求（webgate->EPGate）
	 */
	public static void sendEpGateLogin(Channel channel,int usrGateId){
		logger.info("send epGateLogin usrGateId:{}",usrGateId);
		
		byte[] hmsTime = NetUtils.timeToByte();
		byte[] reqData = EpGateEncoder.login(usrGateId,hmsTime);

		String messagekey = usrGateId + NetUtils.timeToString(hmsTime);
		EpGateMessageSender.sendRepeatMessage(channel, reqData, messagekey);
	}

	/**
	 * 网关登录应答（EPGate->webgate）
	 */
	public static void handleEpGateLogin(Channel channel,int h,int m,int s,int usrGateId,int epGateId,int ret,int errorCode)
	{
		logger.info("receive epGateLogin usrGateId:{},epGateId:{},ret:{},errorCode:{}",new Object[]{usrGateId,epGateId,ret,errorCode});
		
		EpGateNetConnect epGateCommClient = CacheService.getEpGate(epGateId);
		if(epGateCommClient==null)
		{
			logger.info("receive epGateLogin epGateId:{} not connected.",epGateId);
			return;
		}
		epGateCommClient.setStatus(CommStatusConstant.INIT_SUCCESS);
		epGateCommClient.setLastUseTime(DateUtil.getCurrentSeconds());
		CacheService.addEpGateByCh(channel, epGateCommClient);
		
		if(ret==1)
		{
			String key = usrGateId + NetUtils.timeToString(h, m, s);
			CacheService.removeEpRepeatMsg(key);
		}
	}

	/**
	 * 心跳（webgate->EPGate）
	 */
	public static void sendHeart(Channel channel){
		logger.info("send heart channel:{}",channel);
		
		byte[] reqData = EpGateEncoder.heart();

		EpGateMessageSender.sendMessage(channel, reqData);
	}

	/**
	 * 心跳（EPGate->webgate）
	 */
	public static void handleHeart(Channel channel){
		logger.info("receive heart");
		
		setLastUseTime(channel);
	}

	/**
	 * ACK应答（webgate->EPGate）
	 */
	private static void sendAck(Channel channel,int cmd,long accountId){
		logger.info("send ack channel:{}",channel);
		setLastSendTime(channel);
		
		byte[] hmsTime = NetUtils.timeToByte();
		byte[] reqData = EpGateEncoder.ack(cmd,hmsTime,accountId);

		EpGateMessageSender.sendMessage(channel, reqData);
	}

	/**
	 * ACK应答（EPGate->webgate）
	 */
	public static void handleAck(Channel channel,int cmd,int h,int m,int s,long accountId){
		String key = accountId + NetUtils.timeToString(h, m, s);
		CacheService.removeEpRepeatMsg(key);
		logger.info("receive ack key:{}",key);
		setLastUseTime(channel);
	}

	/**
	 * 电桩在线通知（EPGate->webgate）
	 */
	public static void handleEpOnline(Channel channel,int h,int m,int s,int online,String[] epCode){
		logger.info("receive epOnline channel:{},epCode[]:{}",channel,epCode);
		setLastUseTime(channel);

		for (String code : epCode)
		{
			ElectricPileCache epCache = EpService.getEpCache(code);
			epCache.setState(online);
		}

		byte[] hmsTime = NetUtils.timeToByte();
		byte[] reqData = EpGateEncoder.epOnline(hmsTime);

		setLastSendTime(channel);
		EpGateMessageSender.sendMessage(channel, reqData);
	}

	/**
	 * 手机链路在线请求（webgate->EPGate）
	 */
	public static void sendPhoneOnline(Channel channel,int accountId,int online){
		logger.info("send phoneOnline accountId:{},online:{}",accountId,online);
		setLastSendTime(channel);

		byte[] hmsTime = NetUtils.timeToByte();
		byte[] reqData = EpGateEncoder.phoneOnline(hmsTime, accountId, online);

		EpGateMessageSender.sendMessage(channel, reqData);
	}

	/**
	 * 手机链路在线应答（EPGate->webgate）
	 */
	public static void handlePhoneOnline(Channel channel,int h,int m,int s){
		logger.info("receive phoneOnline channel:{}",channel);

		setLastUseTime(channel);
	}

	/**
	 * 手机连接请求（webgate->EPGate）
	 */
	public static void sendPhoneConnect(Channel channel,String epCode,int epGunNo,int accountId)
	{
		logger.info("send phoneConnect epCode:{},epGunNo:{},accountId:{}",new Object[]{epCode,epGunNo,accountId});

		byte[] hmsTime = NetUtils.timeToByte();
		byte[] reqData = EpGateEncoder.phoneConnect(hmsTime, epCode, epGunNo, accountId);

		String messagekey = accountId + NetUtils.timeToString(hmsTime);
		EpGateMessageSender.sendRepeatMessage(channel, reqData, messagekey);
	}

	/**
	 * 手机连接应答（EPGate->webgate）
	 */
	public static void handlePhoneConnect(Channel channel,int h,int m,int s,String epCode,int epGunNo,long accountId,int ret,int errorCode,int status){
		logger.info("receive phoneConnect epCode:{},epGunNo:{},accountId:{},ret:{},errorCode:{}",new Object[]{epCode,epGunNo,accountId,ret,errorCode});
		setLastUseTime(channel);
		
		if(ret==1)
		{
			String key = accountId + NetUtils.timeToString(h, m, s);
			CacheService.removeEpRepeatMsg(key);
		}

		PhoneService.sendPhoneConnect(epCode, epGunNo, (int)accountId, ret, errorCode, status);
	}

	/**
	 * 充电请求（webgate->EPGate）
	 */
	public static void sendCharge(Channel channel,String epCode,int epGunNo,int accountId,int frozenAmt,int changeMode,int showPrice)
	{
		logger.info("send charge epCode:{},epGunNo:{},accountId:{}",new Object[]{epCode,epGunNo,accountId});

		byte[] hmsTime = NetUtils.timeToByte();
		byte[] reqData = EpGateEncoder.charge(hmsTime, epCode, epGunNo, accountId, frozenAmt, changeMode, showPrice);

		String messagekey = accountId + NetUtils.timeToString(hmsTime);
		EpGateMessageSender.sendRepeatMessage(channel, reqData, messagekey);
	}

	/**
	 * 充电应答（EPGate->webgate）
	 */
	public static void handleCharge(Channel channel,int h,int m,int s,String epCode,int epGunNo,long accountId,int ret,int errorCode)
	{
		logger.info("receive charge epCode:{},epGunNo:{},accountId:{},ret:{},errorCode:{}",new Object[]{epCode,epGunNo,accountId,ret,errorCode});
		setLastUseTime(channel);

		PhoneService.sendCharge(epCode, epGunNo, (int)accountId, ret, errorCode);
	}

	/**
	 * 充电事件（EPGate->webgate）
	 */
	public static void handleChargeEvent(Channel channel,int h,int m,int s,String epCode,int epGunNo,long accountId,int status)
	{
		logger.info("receive chargeEvent epCode:{},epGunNo:{},accountId:{},status:{}",new Object[]{epCode,epGunNo,accountId,status});
		setLastUseTime(channel);

		sendAck(channel, EpGateConstant.EP_CHARGE_EVENT, accountId);
		PhoneService.sendChargeEvent(epCode, epGunNo, (int)accountId, status);
	}

	/**
	 * 停止充电请求（webgate->EPGate）
	 */
	public static void sendStopCharge(Channel channel,String epCode,int epGunNo,int accountId)
	{
		logger.info("send stopCharge epCode:{},epGunNo:{},accountId:{}",new Object[]{epCode,epGunNo,accountId});

		byte[] hmsTime = NetUtils.timeToByte();
		byte[] reqData = EpGateEncoder.stopCharge(hmsTime, epCode, epGunNo, accountId);

		String messagekey = accountId + NetUtils.timeToString(hmsTime);
		EpGateMessageSender.sendRepeatMessage(channel, reqData, messagekey);
	}

	/**
	 * 停止充电应答（EPGate->webgate）
	 */
	public static void handleStopCharge(Channel channel,int h,int m,int s,String epCode,int epGunNo,long accountId,int ret,int errorCode){
		logger.info("receive stopCharge epCode:{},epGunNo:{},accountId:{},ret:{},errorCode:{}",new Object[]{epCode,epGunNo,accountId,ret,errorCode});
		setLastUseTime(channel);

		PhoneService.sendStopCharge(epCode, epGunNo, (int)accountId, ret, errorCode);
	}

	/**
	 * 充电实时数据（EPGate->webgate）
	 */
	public static void handleChargeReal(Channel channel,int h,int m,int s,String epCode,int epGunNo,long accountId,ChargingInfo chargingInfo){
		logger.info("receive charge realData epCode:{},epGunNo:{},accountId:{},chargingInfo:{}",new Object[]{epCode,epGunNo,accountId,chargingInfo});
		setLastUseTime(channel);

		PhoneService.sendChargeReal(epCode, epGunNo, (int)accountId, chargingInfo);
	}

	/**
	 * 消费记录（EPGate->webgate）
	 */
	public static void handleConsumeRecord(Channel channel,int h,int m,int s,String epCode,int epGunNo,long accountId,Map<String, Object> consumeRecordMap){
		logger.info("receive charge consumeRecord epCode:{},epGunNo:{},accountId:{},consumeRecordMap:{}",new Object[]{epCode,epGunNo,accountId,consumeRecordMap});
		setLastUseTime(channel);

		sendAck(channel, EpGateConstant.EP_CONSUME_RECORD, accountId);
		PhoneService.sendConsumeRecord(epCode, epGunNo, (int)accountId, consumeRecordMap);
	}

	/**
	 * EPGate最新响应时间设置
	 */
	private static void setLastUseTime(Channel channel){
		EpGateNetConnect epGateClient = CacheService.getEpGateByCh(channel);
		if (epGateClient == null) return;

		epGateClient.setLastUseTime(DateUtil.getCurrentSeconds());
	}

	/**
	 * usrGate最新发送时间设置
	 */
	private static void setLastSendTime(Channel channel){
		EpGateNetConnect epGateClient = CacheService.getEpGateByCh(channel);
		if (epGateClient == null) return;

		epGateClient.setLastSendTime(DateUtil.getCurrentSeconds());
	}
}
