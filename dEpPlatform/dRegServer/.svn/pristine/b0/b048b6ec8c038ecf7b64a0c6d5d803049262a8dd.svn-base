package com.reg.server;

import io.netty.channel.Channel;

import java.io.ByteArrayInputStream;
import java.io.IOException;
import java.io.InputStream;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.reg.cache.GateClient;
import com.reg.protocol.StreamUtil;
import com.reg.service.GateManageService;
import com.reg.utils.DateUtil;


/**
 * 接受客户端数据并处理
 * @author lwz
 * 2015.10.11
 */
public class GateManageMessageHandler{
	
	private static final Logger logger = LoggerFactory.getLogger(GateManageMessageHandler.class);
	
	
	public static void handleMessage(Channel channel, GateManageMessage message) throws  IOException
	{
		GateClient phoneClient = GateManageService.getClientByChannel(channel);
		if(phoneClient==null)
		{
			logger.error("handleMessage error! not find PhoneClient:"+channel.toString());
			return ;
		}
   	    byte[] msg = message.getBytes();
   	 
		int msgLen = msg.length;

		InputStream in = new ByteArrayInputStream(msg);
		
		int cos = StreamUtil.read(in);
		byte typel = StreamUtil.read(in);
		byte typeh = StreamUtil.read(in);
		int type1 = typel + typeh * 0x100;
		
		int type = type1 & 0xffff;
		
	
		switch (type) 
		{
			case 1:// 1 连接充电桩
			
		
            break;
             
			 default:
			 break;
		  }
	}
}
   

