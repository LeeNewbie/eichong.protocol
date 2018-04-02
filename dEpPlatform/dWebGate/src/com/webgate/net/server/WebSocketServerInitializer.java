package com.webgate.net.server;

import io.netty.buffer.ByteBuf;
import io.netty.channel.ChannelHandler;
import io.netty.channel.ChannelHandlerContext;
import io.netty.channel.ChannelInitializer;
import io.netty.channel.ChannelPipeline;
import io.netty.channel.socket.SocketChannel;
import io.netty.handler.codec.MessageToMessageDecoder;
import io.netty.handler.codec.MessageToMessageEncoder;
import io.netty.handler.codec.http.HttpObjectAggregator;
import io.netty.handler.codec.http.HttpServerCodec;
import io.netty.handler.codec.http.websocketx.BinaryWebSocketFrame;
import io.netty.handler.codec.http.websocketx.WebSocketServerProtocolHandler;
import io.netty.handler.logging.LoggingHandler;

import java.util.List;

import com.netCore.netty.server.INettyServer;
import com.netCore.netty.server.ServerHandler;

public class WebSocketServerInitializer extends ChannelInitializer<SocketChannel> {
	INettyServer server;

	public WebSocketServerInitializer(INettyServer server) {
		this.server = server;
	}

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

	@Override
    public void initChannel(SocketChannel ch) throws Exception {
        ChannelPipeline pipeline = ch.pipeline();
        
        pipeline.addLast("httpCodec", new HttpServerCodec());
		pipeline.addLast("aggregator", new HttpObjectAggregator(65536));
		pipeline.addLast("webSocketHandler", new WebSocketServerProtocolHandler("/ws"));
        pipeline.addLast("logger", new LoggingHandler());
		pipeline.addLast("ws2bytebufDecoder", new WebSocketFrameToByteBufDecoder());
		pipeline.addLast("bytebuf2wsEncoder", new ByteBufToWebSocketFrameEncoder());
        pipeline.addLast("decoder", (ChannelHandler)server.getDecoder().getClass().newInstance());
        pipeline.addLast("encoder", (ChannelHandler)server.getEncoder().getClass().newInstance());
        pipeline.addLast("handler", new ServerHandler(server));
        
    }
 
}