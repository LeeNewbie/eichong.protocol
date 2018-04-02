package com.ward.push.handler;

import io.netty.channel.ChannelHandler.Sharable;
import io.netty.channel.ChannelHandlerContext;
import io.netty.channel.ChannelInboundHandlerAdapter;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.ward.push.protocol.ProtocolProcessor;
import com.ward.push.protocol.messages.AbstractMessage;
import com.ward.push.protocol.messages.req.ConnectReqMessage;
import com.ward.push.protocol.messages.resp.PingRespMessage;

/**
 * 
 * 消息处理Handler<br>
 * 
 * @author L.Y
 */
@Sharable
public class NettyMessageHandler extends ChannelInboundHandlerAdapter {

	private static final Logger logger = LoggerFactory.getLogger(NettyMessageHandler.class);

	private final ProtocolProcessor processor;

	public NettyMessageHandler(ProtocolProcessor processor) {
		this.processor = processor;
	}

	@Override
	public void channelRead(ChannelHandlerContext ctx, Object message) throws Exception {
		AbstractMessage msg = (AbstractMessage) message;
		// logger.debug("Received a message of type {}",
		// Utils.msgType2String(msg.getMessageType()));
		switch (msg.getMessageType()) {
		case AbstractMessage.CONNECT:
			processor.processConnect(ctx.channel(), (ConnectReqMessage) msg);
			break;
		case AbstractMessage.PINGREQ:
			PingRespMessage pingResp = new PingRespMessage();
			ctx.writeAndFlush(pingResp);
			break;
		default:
			break;
		}
		super.channelRead(ctx, msg);
	}
}
