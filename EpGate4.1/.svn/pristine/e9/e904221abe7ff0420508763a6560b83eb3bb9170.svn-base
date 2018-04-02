package com.epcentre.service;


import java.util.Map;
import java.util.concurrent.TimeUnit;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.epcentre.cache.AnalyzeCommClient;
import com.epcentre.cache.UserOrigin;
import com.epcentre.config.GameConfig;
import com.epcentre.constant.EventConstant;
import com.epcentre.protocol.AnalyzeConstant;
import com.epcentre.protocol.AnalyzeProtocol;
import com.epcentre.protocol.SingleInfo;
import com.epcentre.server.AnalyzeMessageSender;
import com.epcentre.task.CheckMonitorCommTask;
import com.epcentre.utils.DateUtil;
import com.netCore.core.pool.TaskPoolFactory;



public class AnalyzeService{

	private static final Logger logger = LoggerFactory.getLogger(AnalyzeService.class);
	
	public static AnalyzeCommClient  commClient;
	
	public static void startMonitorCommTimer(long initDelay) {
		
		CheckMonitorCommTask checkTask =  new CheckMonitorCommTask();
				
		TaskPoolFactory.scheduleAtFixedRate("CHECK_MONITOR_TIMEOUT_TASK", checkTask, initDelay, 10, TimeUnit.SECONDS);
		
	}

	
	

	public static void setCommClient(AnalyzeCommClient commClient) {
		AnalyzeService.commClient = commClient;
	}
	/**
	 * 
	 * @return
	 */
	public static AnalyzeCommClient getCommClient() {
		return commClient;
	}
	/**
	 * 
	 * @param type
	 * @param userOrigin
	 * @param ret
	 * @param cause
	 * @param srcParams
	 * @param extraData
	 * @return
	 */
	public static int onEvent(int type,UserOrigin userOrigin,int ret,int cause, Object srcParams,Object extraData)
	{
		logger.debug("onEvent,realData,type:{},UserOrigin:{}",type,userOrigin);
		if(commClient==null)
		{
			logger.error("onEvent,realData,commClient==null");
			return 0;
		}
		if(commClient.getChannel()==null)
		{
			logger.error("onEvent,realData,commClient.getChannel()==null");
			return 0;
		}
		try
		{
		if(type==EventConstant.EVENT_ONE_BIT_YX ||
				type==EventConstant.EVENT_TWO_BIT_YX || 
				type==EventConstant.EVENT_YC ||
				type==EventConstant.EVENT_VAR_YC)
		{
			
			if(srcParams==null)
			{
				logger.error("onEvent,realData,srcParams==null");
				return 0 ;
			}
			Map<String, Object> paramsMap = (Map<String, Object>) srcParams;
			
			String epCode  = (String)paramsMap.get("epcode");
			int epGunNo  = (int)paramsMap.get("epgunno");
			int  currentType = (int)paramsMap.get("currenttype");
			
			Map<Integer, SingleInfo> pointMap = (Map<Integer, SingleInfo>) extraData;
			
			//logger.debug("onEvent,realData,epCode:{},epGunNo:{},type:{},pointMap.size():{}",
				//	new Object[]{epCode,epGunNo,type,pointMap.size()});
			switch(type)
			{
			case EventConstant.EVENT_ONE_BIT_YX:
			{
				byte[] msg =AnalyzeProtocol.do_one_bit_yx(epCode, epGunNo, currentType, pointMap);
				AnalyzeMessageSender.sendMessage(commClient.getChannel(), msg);
			}
			break;
			case EventConstant.EVENT_TWO_BIT_YX:
			{
				byte[] msg =AnalyzeProtocol.do_two_bit_yx(epCode, epGunNo, currentType, pointMap);
				AnalyzeMessageSender.sendMessage(commClient.getChannel(), msg);
			}
			break;
			case EventConstant.EVENT_YC:
			{
				byte[] msg =AnalyzeProtocol.do_yc(epCode, epGunNo, currentType, pointMap);
				AnalyzeMessageSender.sendMessage(commClient.getChannel(), msg);
			}
			break;
			case EventConstant.EVENT_VAR_YC:
			{
				byte[] msg =AnalyzeProtocol.do_var_yc(epCode, epGunNo, currentType, pointMap);
				AnalyzeMessageSender.sendMessage(commClient.getChannel(), msg);
			}
			break;
			default:
				break;
			}
		}
		else
		{
			logger.error("onEvent,realData,ep invalid type:{}",type);
		}
		}
		catch (Exception e) {
			logger.error("onEvent exception,type:{},e.getMessage():{}",type,e.getMessage());
		}
		return 0;

	}
	
	@SuppressWarnings("rawtypes")
	public synchronized static void checkTimeOut() {
		
		if(commClient==null)
		{
			logger.error("AnalyzeChannelHandler, checkTimeOut commClient==null");
			return ;
		}
		
		//需要连接
		boolean bNeedReConnect=false;
		//检查连接，是否需要重连
		int  commStatus = commClient.getStatus();
		if(commStatus == 0 || commStatus == 1)
		{
			//检查
			long connectDiff = DateUtil.getCurrentSeconds() - commClient.getLastConnectTime();
			int times = (commClient.getConnetTimes() / 6) +1;
			long interval= times * GameConfig.analyzeReConnectInterval;
			if(connectDiff> interval)
			{
				logger.debug("AnalyzeCommClient checkTimeOut commStatus:{},connectDiff:{},interval:{}",
						new Object[]{commStatus,connectDiff,interval});
				bNeedReConnect  = true;
			}
		}
		else
		{
			long connectDiff = DateUtil.getCurrentSeconds() - commClient.getLastUseTime();
			
			long interval= GameConfig.analyzeKeepLiveInterval;
			if(connectDiff> interval)
			{
				logger.debug("AnalyzeCommClient checkTimeOut commStatus:{},connectDiff:{},interval:{}",
						new Object[]{commStatus,connectDiff,interval});
				bNeedReConnect  = true;
			}
			
		}
		
		if(bNeedReConnect)
		{
			commClient.reconnect(false);
		}
		
		
		long now = DateUtil.getCurrentSeconds();
		//1.检查连接是否活动,不活动的话发送心跳侦
		long activeDiff = now - commClient.getLastSendTime();
		if (activeDiff >= GameConfig.analyzeHeartInterval) {
			byte[] msg = AnalyzeProtocol.do_heart();
			AnalyzeMessageSender.sendMessage(commClient.getChannel(), msg);
			commClient.setLastUseTime(now);
			commClient.setLastSendTime(now);
		}	
		
	}
	
	public static String stat()
	{
		
		
		if(commClient == null)
			return "commClient = null";
		
		final StringBuilder sb = new StringBuilder();
		
		long connectTime = commClient.getLastConnectTime();
		
    	sb.append("最后连接动作的时间:").append(DateUtil.StringYourDate(DateUtil.toDate(connectTime*1000))).append(" \n");
    	
    	sb.append("重连次数：").append(commClient.getConnetTimes()).append(" \n");
    	
    	sb.append("发送到实时监控系统数据条数：").append(commClient.getRevDataNum()).append(" \n");
    	
    	long useTime = commClient.getLastUseTime();
    	
    	sb.append("最后接收数据时间：").append(DateUtil.StringYourDate(DateUtil.toDate(useTime*1000))).append(" \n");
    	
    	
		return sb.toString();
	}
}
