package com.epcentre.server;

import io.netty.channel.Channel;
import io.netty.channel.ChannelHandlerContext;
import io.netty.handler.codec.ByteToMessageDecoder;
import io.netty.handler.codec.MessageToByteEncoder;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.epcentre.cache.EpCommClient;
import com.epcentre.service.EpCommClientService;
import com.epcentre.utils.DateUtil;
import com.netCore.model.conf.ServerConfig;
import com.netCore.netty.server.AbstractNettyServer;
import com.netCore.util.IPUtil;

public class EpNettyServer extends AbstractNettyServer{

	private static final Logger logger = LoggerFactory.getLogger(EpNettyServer.class);
	
	public EpNettyServer(ServerConfig serverConfig,ByteToMessageDecoder decoder, MessageToByteEncoder encoder) {
		super(serverConfig, decoder, encoder);
	}

	@Override
	public void channelClosed(ChannelHandlerContext ctx) {
		
		//电桩断线
		Channel channel = ctx.channel();
		
		logger.debug("EpNettyServer server close:{}",channel.toString());
		EpCommClientService.offLine(channel);
		
	}

	@Override
	public void channelConnected(ChannelHandlerContext ctx) {
	
		
		EpCommClient commClient = new EpCommClient();
		
		commClient.setChannel(ctx.channel());
		commClient.setLastUseTime(DateUtil.getCurrentSeconds());
		commClient.setStatus(1);
		commClient.setIdentity("");
		EpCommClientService.addCommClient(commClient);
	}

	@Override
	public void exceptionCaught(ChannelHandlerContext ctx, Throwable cause) {
		
		
		Channel channel = ctx.channel();
		
		logger.debug("server exception:{}",channel.toString());
		//TODO
		//EpCommClientService.offLine(channel);
		
	}

	@Override
	public void messageReceived(ChannelHandlerContext ctx, Object obj) {
		Channel channel = ctx.channel();
		String name = IPUtil.getNameByChannel(channel);
		
		
		EpMessage message = (EpMessage)obj;
		
		if(isStop){
			logger.error("服务器已经停止，不再处理消息！忽略来自【{}】的消息:【{}】", new Object[]{ name, message });
			return;
		}
		
		EpMessageHandler.handleMessage(channel, message);
		
	}
	
	@Override
	public void stop() {
		super.stop();
		logger.info("EpNettyServer server stop...");
		
	}

}
