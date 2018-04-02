package com.reg.server;

import io.netty.channel.Channel;
import io.netty.channel.ChannelHandlerContext;
import io.netty.handler.codec.ByteToMessageDecoder;
import io.netty.handler.codec.MessageToByteEncoder;

import java.io.IOException;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.netCore.model.conf.ServerConfig;
import com.netCore.netty.server.AbstractNettyServer;
import com.netCore.util.IPUtil;
import com.reg.cache.GateClient;
import com.reg.utils.DateUtil;

public class GateManageServer extends AbstractNettyServer{

	private static final Logger logger = LoggerFactory.getLogger(GateManageServer.class);
	
	public GateManageServer(ServerConfig serverConfig,ByteToMessageDecoder decoder, MessageToByteEncoder encoder) {
		super(serverConfig, decoder, encoder);
	}

	@Override
	public void channelClosed(ChannelHandlerContext ctx) {
		
		//手机断线
		Channel channel = ctx.channel();
	
		//EpGateService.offLine(channel);
		
	}

	@Override
	public void channelConnected(ChannelHandlerContext ctx) {
	
		GateClient	phoneClient = new GateClient();
		
		phoneClient.setChannel(ctx.channel());
	    phoneClient.setLastUseTime(DateUtil.getCurrentSeconds());
	
	    
	    //EpGateService.addPhoneClient(phoneClient);
	}

	@Override
	public void exceptionCaught(ChannelHandlerContext ctx, Throwable cause) {
		logger.debug("server exception...cause:{},memssage:{}",cause.getCause(),cause.getMessage());
		
		Channel channel = ctx.channel();
		///PhoneService.offLine(channel);
		
	}

	@Override
	public void messageReceived(ChannelHandlerContext ctx, Object obj) {
		Channel channel = ctx.channel();
		String name = IPUtil.getNameByChannel(channel);
		//logger.info("server receive...");
		
		GateManageMessage message = (GateManageMessage)obj;
		
		/*if(isStop){
			logger.error("服务器已经停止，不再处理消息！忽略来自【{}】的消息:【{}】", new Object[]{ name, message });
			return;
		}*/
		try{
			GateManageMessageHandler.handleMessage(channel, message);
		}
		catch(IOException e)
		{
			
		}
		
	}
	
	@Override
	public void stop() {
		super.stop();
		logger.info("PhoneNettyServer server stop...");
		
	}

}
