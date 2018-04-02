package com.third.net.server;

import io.netty.channel.Channel;
import io.netty.channel.ChannelHandlerContext;
import io.netty.handler.codec.ByteToMessageDecoder;
import io.netty.handler.codec.MessageToByteEncoder;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.netCore.model.conf.ServerConfig;
import com.netCore.netty.server.AbstractNettyServer;
import com.netCore.util.IPUtil;
import com.third.config.GameConfig;
import com.third.constant.BaseConstant;
import com.third.net.client.EpCommClient;
import com.third.net.codec.EpMessage;
import com.third.service.EpGateService;
import com.third.utils.DateUtil;

public class EpGateServer extends AbstractNettyServer{

	private static final Logger logger = LoggerFactory.getLogger(EpGateServer.class.getName() + BaseConstant.SPLIT + GameConfig.serverName);
	
	public EpGateServer(ServerConfig serverConfig,ByteToMessageDecoder decoder, MessageToByteEncoder encoder,int btCount,int wtCount) {
		super(serverConfig, decoder, encoder,btCount,wtCount);
	}

	@Override
	public void channelClosed(ChannelHandlerContext ctx) {
		
		//电桩断线
		Channel channel = ctx.channel();
		
		logger.debug("EpNettyServer server close:{}",channel.toString());
		EpGateService.offLine(channel);
		
	}

	@Override
	public void channelConnected(ChannelHandlerContext ctx) {
		
		EpCommClient commClient = new EpCommClient();
		
		commClient.setChannel(ctx.channel());
		commClient.setLastUseTime(DateUtil.getCurrentSeconds());
		commClient.setStatus(1);
		commClient.setIdentity("");
		EpGateService.addCommClient(commClient);
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
		
		EpGateHandler.handleMessage(channel, message);
		
	}
	
	@Override
	public void stop() {
		super.stop();
		logger.info("EpNettyServer server stop...");
		
	}

}
