package com.phonegate.config;


import java.util.Properties;

import com.usrlayer.config.GameBaseConfig;
import com.usrlayer.service.CacheService;

public class GameConfig extends GameBaseConfig{
	
	/** 端口号 */
	public static int port;
	/**服务名称*/
	public static String serverName;

	/**手机连接超时*/
	public static int phoneConnectTimeout;

	static {
		GameConfig.loadGameConfig();
	}
	
	/**
	 * 加载GameConfig.properties文件
	 * @author 
	 * 2014-11-26
	 */
	public static void loadGameConfig(){
		Properties p = getProperties(fileName);
		
		port = Integer.valueOf(p.getProperty("port","8888").trim());
		serverName = p.getProperty("serverName","usrGate");
		
		CacheService.setUsrGateId(Integer.valueOf(p.getProperty("usrGateId","1").trim()));
		phoneConnectTimeout = Integer.valueOf(p.getProperty("phoneConnectTimeout","60").trim());
	}

}
