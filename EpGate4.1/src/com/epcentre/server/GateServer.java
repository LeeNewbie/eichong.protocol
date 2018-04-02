package com.epcentre.server;

import io.netty.handler.codec.ByteToMessageDecoder;
import io.netty.handler.codec.MessageToByteEncoder;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.epcentre.cache.AnalyzeCommClient;
import com.epcentre.cache.EpCache;
import com.epcentre.cache.GameContext;
import com.epcentre.config.GameConfig;
import com.epcentre.service.AnalyzeService;
import com.epcentre.service.EpCommClientService;
import com.epcentre.service.EpConcentratorService;
import com.epcentre.service.EpGunService;
import com.epcentre.service.EqVersionService;
import com.epcentre.service.PhoneService;
import com.epcentre.service.RateService;
import com.netCore.conf.CoreConfig;
import com.netCore.model.conf.ClientConfig;
import com.netCore.model.conf.ClientConfigs;
import com.netCore.model.conf.ServerConfig;
import com.netCore.model.conf.ServerConfigs;
import com.netCore.netty.httpserver.AbstractHttpServer;
import com.netCore.netty.server.AbstractNettyServer;
import com.netCore.server.impl.AbstractGameServer;

public class GateServer extends AbstractGameServer{
	private static final Logger logger = LoggerFactory.getLogger(GateServer.class);
	
	private static GateServer gameServer;
	
	private static Object lock = new Object();
	
	/**电桩连接的nettyServer*/
	public static AbstractNettyServer nettyEpServer;
	/**后台连接的nettyServer*/
	public static AbstractNettyServer nettyManageServer;
	/**临时的手机长链接服务*/
	public static AbstractNettyServer nettyPhoneServer;
	
	/**临时的手机长链接服务*/
	public static AbstractHttpServer watchHttpServer=null;
	
	/**临时的手机长链接服务*/
	public static AbstractHttpServer thirdHttpServer=null;
	
	/**数据监控分析中连接**/
	public static AnalyzeNettyClient nettyAnalyzeClient;
	

	public GateServer(){
		//super();
	
		
		//ResourceLeakDetector.setLevel(ResourceLeakDetector.Level.ADVANCED);
		
		ServerConfigs serverConfigs = CoreConfig.serverConfigs;
		
		ClientConfigs clientConfigs = CoreConfig.clientConfigs;
		
		if (serverConfigs == null || clientConfigs == null) 
		{
			String errMsg = "【Gate服务器-客户端】缺少【外部】访问配置...服务器强行退出！";
			logger.error(errMsg);
			throw new RuntimeException(errMsg);
		}
		
		//创建netty服务器	
		ServerConfig gateSvrCfg = serverConfigs.get("gate-server");
		//gate服务器
		if (gateSvrCfg !=null) {
			
			ByteToMessageDecoder decoder = new EpNettyDecoder();
			MessageToByteEncoder encoder = new EpNettyEncoder();
			
			EpNettyServer nettyServer = new EpNettyServer(gateSvrCfg, decoder, encoder,0,0);
			nettyServerList.add(nettyServer);
			nettyEpServer = nettyServer;
		}else {
			String errMsg = "【Gate服务器】缺少【外部】访问配置...服务器强行退出！";
			logger.error(errMsg);
			throw new RuntimeException(errMsg);
		}
			
		//App gate服务器
		ServerConfig apiSvrCfg = serverConfigs.get("manage-server");
		if (apiSvrCfg != null) {
			
			ByteToMessageDecoder decoder = new ApiDecoder();
			MessageToByteEncoder encoder = new ApiEncoder();
			
			InnerApiNettyServer nettyServer = new InnerApiNettyServer(apiSvrCfg, decoder, encoder,0,0);
			nettyServerList.add(nettyServer);
			nettyManageServer = nettyServer;
		}else {
			String errMsg = "【Gate服务器】缺少【内部】访问配置...服务器强行退出！";
			logger.error(errMsg);
			throw new RuntimeException(errMsg);
		}
		//内部监控http服务
		ServerConfig watchHttpSvrCfg = serverConfigs.get("gate-monitor-server");
		if (watchHttpSvrCfg != null) {
			
			watchHttpServer= new MonitorHttpServer(watchHttpSvrCfg);
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
			
			PhoneNettyServer nettyServer = new PhoneNettyServer(phoneSvrCfg, decoder, encoder,0,0);
			nettyServerList.add(nettyServer);
			nettyPhoneServer = nettyServer;
		}
		else
		{
			String errMsg = "【Gate服务器】缺少【手机接口】访问配置...服务器强行退出！";
			logger.error(errMsg);
			throw new RuntimeException(errMsg);
		}
		
		//创建netty客户端连接	
		ClientConfig analyzeCfg = clientConfigs.get("analyze-server");
		//数据中心客户端连接
		if (analyzeCfg !=null) {
			
			AnalyzeCommClient commClient = new AnalyzeCommClient(analyzeCfg);
			commClient.connect();
		
			AnalyzeService.setCommClient(commClient);
			
		}else {
			String errMsg = "【Gate服务器-客户端】缺少【外部】访问配置...服务器强行退出！";
			logger.error(errMsg);
			throw new RuntimeException(errMsg);
		}
		
		//初始化升级版本列表，启动定时升级线程
		EqVersionService.init();
		
		logger.info("初始化升级版本列表\n");
		//初始化费率表
		RateService.init();
		
		
		logger.info("初始化费率表\n");
	}
	
	/**
	 * 创建服务端服务器
	 * @author 
	 * 2014-11-28
	 * @return
	 */
	public static GateServer getInstance(){
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
		new GameContext();//初始化spring,加载数据库全局数据
		EpConcentratorService.updateAllCommStatus();//电桩断线更新
		new EpCache();//初始化服务世界
		logger.info("初始化服务成功...");
	}
	
	@Override
	public void start() {
		logger.error("watchHttpServer start");
	
		super.start();
		
		
	}

	@Override
	public void stop() {
		
		//1、停止 netty服务器、停止 netty客户端、关闭线程池、关闭任务池
		super.stop();
		
	}
	
	@Override
	public void startTimerServer() {
		
		//检查枪上的预约和充电超时
		EpGunService.startCheckTimeoutServer(10);
		
		//检查电桩僵尸状态通讯
        EpCommClientService.startCommClientTimeout(5);
        
        //检查手机僵尸状态通讯
        PhoneService.startCommClientTimeout(5);
        
        
        EpGunService.startRepeatSendMessage();
        
        //检查连接监控中心通讯状态
        AnalyzeService.startMonitorCommTimer(10);
        
        EpGunService.startThirdDataTimer();
        
		super.startTimerServer();
		
		logger.info("所有定时任务启动成功!");
		
	}
}
