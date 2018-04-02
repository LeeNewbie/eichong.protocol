package com.epcentre.cache;

import io.netty.handler.codec.ByteToMessageDecoder;
import io.netty.handler.codec.MessageToByteEncoder;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.epcentre.server.AnalyzeNettyClient;
import com.epcentre.server.PhoneDecoder;
import com.epcentre.server.PhoneEncoder;
import com.epcentre.utils.DateUtil;
import com.netCore.model.conf.ClientConfig;


public class AnalyzeCommClient extends NetObject{
	
	private static final Logger logger = LoggerFactory.getLogger(AnalyzeCommClient.class);
	
	public int connetTimes;//连接次数
	public long lastConnectTime=0;//最后连接动作的时间
	protected long lastSendTime;
	
	public int revDataNum;

	AnalyzeNettyClient nettyClient;
	
	ClientConfig clientConfig = null;
	
	public AnalyzeCommClient(ClientConfig clientConfigPara)
	{	
		connetTimes=0;
		lastConnectTime =0;
		lastUseTime = 0;	
		
		identity = "Analyze";
		nettyClient=null;
		
		clientConfig =  clientConfigPara;
		lastSendTime=0;
		revDataNum = 0;
	}
	
	
	public AnalyzeNettyClient getNettyClient() {
		return nettyClient;
	}

	public void setNettyClient(AnalyzeNettyClient nettyClient) {
		this.nettyClient = nettyClient;
	}

	
	
	public int getRevDataNum() {
		return revDataNum;
	}


	public void setRevDataNum(int revDataNum) {
		this.revDataNum = revDataNum;
	}


	public long getLastConnectTime() {
		return lastConnectTime;
	}


	public void setLastConnectTime(long lastConnectTime) {
		this.lastConnectTime = lastConnectTime;
	}


	public int getConnetTimes() {
		return connetTimes;
	}
	public void clearConnecTtimes()
	{
		connetTimes=0;
	}

	
	public long getLastSendTime() {
		return lastSendTime;
	}


	public void setLastSendTime(long lastSendTime) {
		this.lastSendTime = lastSendTime;
	}


	public AnalyzeNettyClient connect()
	{
		//连接的时候增加
		connetTimes++;
		lastConnectTime = DateUtil.getCurrentSeconds();
		
		logger.debug("AnalyzeCommClient connect,connetTime:{},lastConnectTime:{}",connetTimes,lastConnectTime);
		
		
		ByteToMessageDecoder decoder = new PhoneDecoder();
		MessageToByteEncoder<?> encoder = new PhoneEncoder();
	
		AnalyzeNettyClient nettyClient = new AnalyzeNettyClient(clientConfig, decoder, encoder);
		
		//logger.debug("AnalyzeCommClient connect,address:{}",clientConfig.getIpAndPort());
		nettyClient.start();
		this.nettyClient = nettyClient;
		return nettyClient;
	}

	
	
	public int reconnect(boolean forceReconnect)
	{
		logger.debug("AnalyzeCommClient reconnect,forceReconnect:{}",forceReconnect);
	
		close();
		connect();
		
		return 1;
		
	}
	public void close()
	{
		if(channel!=null)
		{
			channel.close();
		}
		status=0;
	}
	
	
}