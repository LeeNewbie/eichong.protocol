package com.epcentre.server;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import io.netty.channel.Channel;
import io.netty.channel.ChannelFuture;
/**
 * 发送消息必须通过此类
 */
public class AnalyzeMessageSender {
	
	private static final Logger logger = LoggerFactory.getLogger(AnalyzeMessageSender.class);
	
	public static ChannelFuture  sendMessage(Channel channel,Object object) {
		if(channel == null)
		{
			logger.error("sendMessage:channel == null");
			return null;
		}
		if (!channel.isWritable()) {
			logger.error("sendMessage:channel.isWritable()=false");
			return null;
		}

		ChannelFuture future = channel.writeAndFlush(object);
		return future;
	}
	
	
	

	
}
