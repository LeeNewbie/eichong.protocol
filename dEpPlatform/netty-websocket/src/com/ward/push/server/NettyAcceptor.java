package com.ward.push.server;

import io.netty.bootstrap.ServerBootstrap;
import io.netty.buffer.ByteBuf;
import io.netty.channel.ChannelFuture;
import io.netty.channel.ChannelHandlerContext;
import io.netty.channel.ChannelInitializer;
import io.netty.channel.ChannelOption;
import io.netty.channel.ChannelPipeline;
import io.netty.channel.EventLoopGroup;
import io.netty.channel.nio.NioEventLoopGroup;
import io.netty.channel.socket.SocketChannel;
import io.netty.channel.socket.nio.NioServerSocketChannel;
import io.netty.handler.codec.MessageToMessageDecoder;
import io.netty.handler.codec.MessageToMessageEncoder;
import io.netty.handler.codec.http.HttpObjectAggregator;
import io.netty.handler.codec.http.HttpRequestDecoder;
import io.netty.handler.codec.http.HttpResponseEncoder;
import io.netty.handler.codec.http.websocketx.BinaryWebSocketFrame;
import io.netty.handler.codec.http.websocketx.WebSocketServerProtocolHandler;
import io.netty.handler.timeout.IdleStateHandler;

import java.net.BindException;
import java.util.List;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.DisposableBean;
import org.springframework.beans.factory.InitializingBean;

import com.ward.push.exception.NettyServerException;
import com.ward.push.handler.IdleTimeoutHandler;
import com.ward.push.handler.NettyMessageHandler;
import com.ward.push.protocol.MessageDecoder;
import com.ward.push.protocol.MessageEncoder;
import com.ward.push.protocol.ProtocolProcessor;
import com.ward.push.util.Global;
import com.ward.push.util.NamedThreadFactory;

/**
 * 
 * Netty Server<br>
 * 
 * @author L.Y
 */
public class NettyAcceptor implements InitializingBean, DisposableBean {

	private static final Logger logger = LoggerFactory.getLogger(NettyAcceptor.class);

	static class WebSocketFrameToByteBufDecoder extends MessageToMessageDecoder<BinaryWebSocketFrame> {
		@Override
		protected void decode(ChannelHandlerContext chc, BinaryWebSocketFrame frame, List<Object> out) throws Exception {
			ByteBuf bb = frame.content();
			bb.retain();
			out.add(bb);
		}
	}

	static class ByteBufToWebSocketFrameEncoder extends MessageToMessageEncoder<ByteBuf> {
		@Override
		protected void encode(ChannelHandlerContext chc, ByteBuf bb, List<Object> out) throws Exception {
			BinaryWebSocketFrame result = new BinaryWebSocketFrame();
			result.content().writeBytes(bb);
			out.add(result);
		}
	}

	abstract class PipelineInitializer {
		abstract void init(ChannelPipeline pipeline) throws Exception;
	}

	private int port = 9010;
	private int webSocketPort = 9030;

	EventLoopGroup m_bossGroup;
	EventLoopGroup m_workerGroup;

	public void startServer() {
		m_bossGroup = new NioEventLoopGroup(1, new NamedThreadFactory("NettyServer-Boss", true));
		m_workerGroup = new NioEventLoopGroup(0, new NamedThreadFactory("NettyServer-Worker", true));
		ProtocolProcessor processor = new ProtocolProcessor();
		NettyMessageHandler handler = new NettyMessageHandler(processor);
		//initializePlainTCPTransport(handler);
		initializeWebSocketTransport(handler);
	}

	private void initializePlainTCPTransport(final NettyMessageHandler handler) {
		final IdleTimeoutHandler timeoutHandler = new IdleTimeoutHandler();
		initFactory(port, new PipelineInitializer() {
			@Override
			void init(ChannelPipeline pipeline) throws Exception {
				pipeline.addFirst("idleStateHandler", new IdleStateHandler(0, 0, Global.DEFAULT_ALL_IDLE_TIMEOUT));
				pipeline.addAfter("idleStateHandler", "idleEventHandler", timeoutHandler);
				pipeline.addLast("decoder", new MessageDecoder());
				pipeline.addLast("encoder", new MessageEncoder());
				pipeline.addLast("handler", handler);
			}
		});
	}

	private void initializeWebSocketTransport(final NettyMessageHandler handler) {
		final IdleTimeoutHandler timeoutHandler = new IdleTimeoutHandler();
		initFactory(webSocketPort, new PipelineInitializer() {
			@Override
			void init(ChannelPipeline pipeline) throws Exception {
				pipeline.addFirst("idleStateHandler", new IdleStateHandler(0, 0, Global.DEFAULT_ALL_IDLE_TIMEOUT));
				pipeline.addAfter("idleStateHandler", "idleEventHandler", timeoutHandler);
				pipeline.addLast("httpEncoder", new HttpResponseEncoder());
				pipeline.addLast("httpDecoder", new HttpRequestDecoder());
				pipeline.addLast("aggregator", new HttpObjectAggregator(65536));
				pipeline.addLast("webSocketHandler", new WebSocketServerProtocolHandler("/ws"));
				pipeline.addLast("ws2bytebufDecoder", new WebSocketFrameToByteBufDecoder());
				pipeline.addLast("bytebuf2wsEncoder", new ByteBufToWebSocketFrameEncoder());
				pipeline.addLast("decoder", new MessageDecoder());
				pipeline.addLast("encoder", new MessageEncoder());
				pipeline.addLast("handler", handler);

			}
		});
	}

	private void initFactory(int port, final PipelineInitializer pipeliner) {
		int startingPort = port;
		ServerBootstrap serverBootstrap = new ServerBootstrap();
		serverBootstrap.group(m_bossGroup, m_workerGroup).channel(NioServerSocketChannel.class)
				.childHandler(new ChannelInitializer<SocketChannel>() {
					@Override
					public void initChannel(SocketChannel ch) throws Exception {
						ChannelPipeline pipeline = ch.pipeline();
						try {
							pipeliner.init(pipeline);
						} catch (Throwable th) {
							logger.error("Severe error during pipeline creation", th);
						}
					}
				}).option(ChannelOption.SO_BACKLOG, 128)
				// 设置no_delay
				.option(ChannelOption.TCP_NODELAY, true)
				// 设置channel no_delay
				.childOption(ChannelOption.TCP_NODELAY, true)
				// 设置可以重用time_wait的socket
				.childOption(ChannelOption.SO_REUSEADDR, true)
				// 是否保活
				.childOption(ChannelOption.SO_KEEPALIVE, false)
				// 设置SO_LINGER为2秒
				.childOption(ChannelOption.SO_LINGER, 2);
		while (true) {
			try {
				ChannelFuture future = serverBootstrap.bind(startingPort).sync();
				logger.info(String.format("Netty server started at [%s]", startingPort));
				future.channel().closeFuture().sync();
				break;
			} catch (InterruptedException ex) {
				logger.error(String.format("Netty server failed to start at [%s]", startingPort), ex);
				Thread.currentThread().interrupt();
				throw new NettyServerException("failed to start netty server", ex);
			} catch (Exception ex) {
				if (ex instanceof BindException) {
					// 端口被占用，用于一台服务器启动一个应用的多个JVM的场景
					logger.warn("NettyServer init fail,will try other port.", ex);
					startingPort++;
				}
			}
		}
	}

	public void stopServer() {
		// 关闭所有相关链接
		m_bossGroup.shutdownGracefully();
		m_workerGroup.shutdownGracefully();
	}

	public int getPort() {
		return port;
	}

	public void setPort(int port) {
		this.port = port;
	}

	public int getWebSocketPort() {
		return webSocketPort;
	}

	public void setWebSocketPort(int webSocketPort) {
		this.webSocketPort = webSocketPort;
	}

	@Override
	public void destroy() throws Exception {
		stopServer();
	}

	@Override
	public void afterPropertiesSet() throws Exception {
		startServer();
	}

}
