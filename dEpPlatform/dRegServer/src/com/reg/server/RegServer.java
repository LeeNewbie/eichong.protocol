package com.reg.server;

import io.netty.handler.codec.ByteToMessageDecoder;
import io.netty.handler.codec.MessageToByteEncoder;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.netCore.conf.CoreConfig;
import com.netCore.model.conf.ServerConfig;
import com.netCore.model.conf.ServerConfigs;
import com.netCore.netty.httpserver.AbstractHttpServer;
import com.netCore.netty.server.AbstractNettyServer;
import com.netCore.server.impl.AbstractGameServer;
import com.reg.cache.GameContext;
import com.reg.config.GameConfig;

public class RegServer extends AbstractGameServer{
	
	private static final Logger logger = LoggerFactory.getLogger(RegServer.class);
	
	private static RegServer gameServer;
	
	private static Object lock = new Object();
	
	/**电桩连接的nettyServer*/
	public static AbstractNettyServer innerTcpServer;
	
	/**
	 * 1.dEpGate分配
	 * 2.dUsrGate分配
	 */
	public static AbstractHttpServer httpServer=null;
	
	
	/**临时的手机长链接服务*/
	public static AbstractHttpServer watchHttpServer=null;
	
	
	

	public RegServer(){
		//super();
	
		//创建netty服务器
		ServerConfigs serverConfigs = CoreConfig.serverConfigs;
		if (serverConfigs != null) {
			
			ServerConfig regSvrCfg = serverConfigs.get("reg-server");
			//gate服务器
			if (regSvrCfg !=null) {
				
				
				ByteToMessageDecoder decoder = new GateManageDecoder();
				MessageToByteEncoder encoder = new GateManageEncoder();
				
				GateManageServer nettyServer = new GateManageServer(regSvrCfg, decoder, encoder);
				nettyServerList.add(nettyServer);
				innerTcpServer = nettyServer;
			}else {
				String errMsg = "【RegServer】缺少【外部】访问配置...服务器强行退出！";
				logger.error(errMsg);
				throw new RuntimeException(errMsg);
			}
			
			ServerConfig regHttpSvrCfg = serverConfigs.get("reg-http-server");
			if (regHttpSvrCfg != null) {
				httpServer= new RegHttpServer(regHttpSvrCfg);
				nettyHttpServerList.add(httpServer);
			}
			else
			{
				String errMsg = "【RegServer】缺少【gate负债】访问配置...服务器强行退出！";
				logger.error(errMsg);
				throw new RuntimeException(errMsg);
			}
			ServerConfig watchHttpSvrCfg = serverConfigs.get("gate-monitor-server");
			if (watchHttpSvrCfg != null) {
				watchHttpServer= new WatchHttpServer(watchHttpSvrCfg);
				nettyHttpServerList.add(watchHttpServer);
			}
			else
			{
				String errMsg = "【RegServer】缺少【内部监控】访问配置...服务器强行退出！";
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
	public static RegServer getInstance(){
		synchronized(lock){
			if(gameServer==null){
				gameServer = new RegServer();
			}
		}
		return gameServer;
	}
	
	public void init(){
		super.init();
		new GameConfig();//初始化服务器基础配置
		//new GameContext();//初始化spring,加载数据库全局数据
		
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
	
		super.startTimerServer();
		
		logger.info("所有定时任务启动成功!");
		
	}
}
