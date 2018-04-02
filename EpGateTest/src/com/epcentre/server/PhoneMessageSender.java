package com.epcentre.server;

import io.netty.channel.Channel;
import io.netty.channel.ChannelFuture;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.epcentre.cache.PhoneClient;
import com.epcentre.service.PhoneService;
/**
 * 发送消息必须通过此类
 * @author 
 * 2014-11-28 上午11:32:54
 */
public class PhoneMessageSender {
	
	private static final Logger logger = LoggerFactory.getLogger(PhoneMessageSender.class);
	
	public static ChannelFuture  sendMessage(Channel channel,Object object) {
		if(channel == null)
			return null;
		 
		if (channel == null || !channel.isWritable()) {
			return null;
		}
		
        PhoneClient phoneClient = PhoneService.getPhoneClientByChannel(channel);
		
        /*if(phoneClient !=null && MsgWhiteList.isOpen())
		{
			ElectricUserInfo u = phoneClient.getUserinfo();
			if(u !=null && MsgWhiteList.find(u.getPhone()))
			{
			    FileUtils.appendLog(u.getPhone()+"_phone.log", WmIce104Util.ConvertHex((byte[])object,3));
			}
		}*/
		ChannelFuture future = channel.writeAndFlush(object);
		return future;
	}
	
	
	

	
}
