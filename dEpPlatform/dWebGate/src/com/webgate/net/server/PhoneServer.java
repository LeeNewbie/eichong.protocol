package com.webgate.net.server;

import io.netty.bootstrap.ServerBootstrap;
import io.netty.buffer.PooledByteBufAllocator;
import io.netty.channel.Channel;
import io.netty.channel.ChannelFuture;
import io.netty.channel.ChannelFutureListener;
import io.netty.channel.ChannelHandlerContext;
import io.netty.channel.ChannelOption;
import io.netty.channel.EventLoopGroup;
import io.netty.channel.nio.NioEventLoopGroup;
import io.netty.channel.socket.nio.NioServerSocketChannel;
import io.netty.handler.codec.ByteToMessageDecoder;
import io.netty.handler.codec.MessageToByteEncoder;

import java.io.IOException;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.netCore.model.conf.ServerConfig;
import com.netCore.netty.server.AbstractNettyServer;
import com.netCore.util.IPUtil;
import com.webgate.config.GameConfig;
import com.webgate.constant.BaseConstant;
import com.webgate.net.client.PhoneClient;
import com.webgate.service.CacheService;
import com.webgate.service.PhoneService;
import com.webgate.utils.DateUtil;

public class PhoneServer extends AbstractNettyServer{

	private static final Logger logger = LoggerFactory.getLogger(PhoneServer.class + BaseConstant.SPLIT + GameConfig.serverName);
	
	public PhoneServer(ServerConfig serverConfig,ByteToMessageDecoder decoder, MessageToByteEncoder<PhoneMessage> encoder,int btCount,int wtCount) {
		super(serverConfig, decoder, encoder,btCount,wtCount);
	}

	@Override
	public void start() {
	   init();

	   EventLoopGroup bossGroup = new NioEventLoopGroup();
	   EventLoopGroup workerGroup = new NioEventLoopGroup();

	   try
	   {
	        ServerBootstrap bootstrap = new ServerBootstrap();
	        bootstrap.group(bossGroup, workerGroup)
	            .channel(NioServerSocketChannel.class)
	            .option(ChannelOption.SO_BACKLOG, Integer.valueOf(1024))
	            .option(ChannelOption.TCP_NODELAY, Boolean.valueOf(true))
	            .option(ChannelOption.SO_KEEPALIVE, Boolean.valueOf(true))
	            .option(ChannelOption.SO_REUSEADDR, Boolean.valueOf(true))
	            .option(ChannelOption.ALLOCATOR, PooledByteBufAllocator.DEFAULT)
	            .childOption(ChannelOption.ALLOCATOR, PooledByteBufAllocator.DEFAULT)
	            .childHandler(new WebSocketServerInitializer(this));

			System.out.println("WebsocketChatServer 启动了");
		    ChannelFuture f = bootstrap.bind(serverConfig.getPort());
			f.addListener(new ChannelFutureListener() {
					public void operationComplete(ChannelFuture future) throws Exception {
	                    if(future.isSuccess()) {
	                    } else {
	                        System.exit(0);
	                    }
	                }
	            }
			);
        } catch(Exception e) {
	            logger.error("websocket 启动失败,memssage:{}", new Object[] {e.getMessage()});
	            e.printStackTrace();
        }
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
	    
	    CacheService.addPhoneClient(phoneClient);
	}

	@Override
	public void exceptionCaught(ChannelHandlerContext ctx, Throwable cause) {
		logger.debug("server exception...cause:{},memssage:{}",cause.getCause(),cause.getMessage());
		
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
			
		}
		
	}
	
	@Override
	public void stop() {
		super.stop();
		logger.info("PhoneNettyServer server stop...");
		
	}

}
