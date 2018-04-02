package com.epcentre.server;

import io.netty.channel.Channel;
import io.netty.channel.ChannelHandlerContext;
import io.netty.handler.codec.ByteToMessageDecoder;
import io.netty.handler.codec.MessageToByteEncoder;

import java.io.IOException;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.epcentre.cache.PhoneClient;
import com.epcentre.service.EpCommClientService;
import com.epcentre.service.PhoneService;
import com.epcentre.utils.DateUtil;
import com.netCore.model.conf.ServerConfig;
import com.netCore.netty.server.AbstractNettyServer;
import com.netCore.util.IPUtil;

public class PhoneNettyServer extends AbstractNettyServer{

	private static final Logger logger = LoggerFactory.getLogger(PhoneNettyServer.class);
	
	public PhoneNettyServer(ServerConfig serverConfig,ByteToMessageDecoder decoder, MessageToByteEncoder encoder,int bossTCount,int workTCount) {
		super(serverConfig, decoder, encoder,bossTCount,workTCount);
	}

	@Override
	public void channelClosed(ChannelHandlerContext ctx) {
		
		//手机断线
		Channel channel = ctx.channel();
	
		PhoneService.offLine(channel);
		
	}

	@Override
	public void channelConnected(ChannelHandlerContext ctx) {
	
		PhoneClient	phoneClient = new PhoneClient();
		
		phoneClient.setChannel(ctx.channel());
	    phoneClient.setLastUseTime(DateUtil.getCurrentSeconds());
	    
	
	    
	    PhoneService.addConnect(phoneClient);
	}

	@Override
	public void exceptionCaught(ChannelHandlerContext ctx, Throwable cause) {
		logger.info("PhoneNettyServer server exception...cause:{},memssage:{}",cause.getCause(),cause.getMessage());
		
		Channel channel = ctx.channel();
		
		PhoneService.offLine(channel);
		
	}

	@Override
	public void messageReceived(ChannelHandlerContext ctx, Object obj) {
		Channel channel = ctx.channel();
		String name = IPUtil.getNameByChannel(channel);
		//logger.info("server receive...");
		
		PhoneMessage message = (PhoneMessage)obj;
		
		/*if(isStop){
			logger.error("服务器已经停止，不再处理消息！忽略来自【{}】的消息:【{}】", new Object[]{ name, message });
			return;
		}*/
		try{
			PhoneMessageHandler.handleMessage(channel, message);
		}
		catch(IOException e)
		{
			logger.error("messageReceived exception");
		}
		
	}
	
	@Override
	public void stop() {
		super.stop();
		logger.info("PhoneNettyServer server stop...");
		
	}

}
