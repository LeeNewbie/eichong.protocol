package com.usrgate.server;

import io.netty.handler.codec.ByteToMessageDecoder;
import io.netty.handler.codec.MessageToByteEncoder;

import java.util.Iterator;
import java.util.Map;
import java.util.Map.Entry;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.netCore.conf.CoreConfig;
import com.netCore.model.conf.ServerConfig;
import com.netCore.model.conf.ServerConfigs;
import com.netCore.netty.httpserver.AbstractHttpServer;
import com.netCore.netty.server.AbstractNettyServer;
import com.netCore.server.impl.AbstractGameServer;
import com.ormcore.cache.GameContext;
import com.usrgate.config.GameConfig;
import com.usrgate.net.codec.PhoneDecoder;
import com.usrgate.net.codec.PhoneEncoder;
import com.usrgate.net.server.PhoneServer;
import com.usrgate.net.server.WatchHttpServer;
import com.usrgate.service.CacheService;
import com.usrgate.service.CycleConfigService;
import com.usrgate.service.EpGateService;
import com.usrgate.service.PhoneService;

public class GateServer extends AbstractGameServer{
	private static final Logger logger = LoggerFactory.getLogger(GateServer.class);
	
	private static GateServer gameServer;
	
	private static Object lock = new Object();
	
	
	/**临时的手机长链接服务*/
	public static AbstractNettyServer nettyPhoneServer;
	
	/**临时的手机长链接服务*/
	public static AbstractHttpServer watchHttpServer=null;
	

	public GateServer() {
		//super();
	
		//创建netty服务器
		ServerConfigs serverConfigs = CoreConfig.serverConfigs;
		if (serverConfigs != null) {
			
			//内部监控http服务
			ServerConfig watchHttpSvrCfg = serverConfigs.get("gate-monitor-server");
			if (watchHttpSvrCfg != null) {
				
				watchHttpServer= new WatchHttpServer(watchHttpSvrCfg);
				nettyHttpServerList.add(watchHttpServer);
				
			}
			else
			{
				String errMsg = "【Gate服务器】缺少【内部监控】访问配置...服务器强行退出！";
				logger.error(errMsg);
				throw new RuntimeException(errMsg);
				
			}
			
			ServerConfig phoneSvrCfg = serverConfigs.get("phone-server");
			//
			if (phoneSvrCfg != null) {
				
				ByteToMessageDecoder decoder = new PhoneDecoder();
				MessageToByteEncoder encoder = new PhoneEncoder();
				
				PhoneServer nettyServer = new PhoneServer(phoneSvrCfg, decoder, encoder,0,0);
				nettyServerList.add(nettyServer);
				nettyPhoneServer = nettyServer;
			}
			else
			{
				String errMsg = "【Gate服务器】缺少【手机接口】访问配置...服务器强行退出！";
				logger.error(errMsg);
				throw new RuntimeException(errMsg);
			}
		}
	}
	
	/**
	 * 创建服务端服务器
	 * @author 
	 * 2014-11-28
	 * @return
	 */
	public static GateServer getInstance() {
		synchronized(lock){
			if(gameServer==null){
				gameServer = new GateServer();
			}
		}
		return gameServer;
	}
	public void init(){
		super.init();
		new GameConfig();//初始化服务器基础配置
		CacheService.setCycleConfigs(CycleConfigService.initConfigs());
		new GameContext(null);//初始化spring,加载数据库全局数据
		
		logger.info("初始化服务成功...");
	}
	
	@Override
	public void start() {
	
		super.start();
		
	}

	@Override
	public void stop() {
		
		//1、停止 netty服务器、停止 netty客户端、关闭线程池、关闭任务池
		super.stop();
		
	}
	
	@SuppressWarnings("rawtypes")
	@Override
	public void startTimerServer() {
		   
        //检查手机僵尸状态通讯
        PhoneService.startCommClientTimeout(5);
        
		super.startTimerServer();
		
		EpGateService.startScanEpGate(10);
        
		Iterator<Entry<Integer ,Integer>> iter = CacheService.getCycleConfigs().getFlowcfgs().entrySet().iterator();
		while (iter.hasNext()) {
			Map.Entry entry = (Map.Entry) iter.next();
			int cycle = (int) entry.getKey();
			CacheService.startStatisticsTimer(10, cycle);
		}
		Iterator<Entry<Integer ,Integer>> inIt = CacheService.getCycleConfigs().getInstructcfgs().entrySet().iterator();
		while (inIt.hasNext()) {
			Map.Entry entry = (Map.Entry) inIt.next();
			int cycle = (int) entry.getKey();
	        CacheService.startInstructStatTimer(10, cycle);
		}
		Iterator<Entry<Integer ,Integer>> epiter = CacheService.getCycleConfigs().getEpFlowcfgs().entrySet().iterator();
		while (epiter.hasNext()) {
			Map.Entry entry = (Map.Entry) epiter.next();
			int cycle = (int) entry.getKey();
			CacheService.startEpStatisticsTimer(10, cycle);
		}
		Iterator<Entry<Integer ,Integer>> epinIt = CacheService.getCycleConfigs().getEpInstructcfgs().entrySet().iterator();
		while (epinIt.hasNext()) {
			Map.Entry entry = (Map.Entry) epinIt.next();
			int cycle = (int) entry.getKey();
	        CacheService.startEpInstructStatTimer(10, cycle);
		}

		//检查连接监控中心通讯状态
        CacheService.startEpGateCommTimer(10);
		
		logger.info("所有定时任务启动成功!");
		
	}
}
