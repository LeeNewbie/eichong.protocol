package com.ward.push.protocol;

import io.netty.buffer.ByteBuf;
import io.netty.channel.ChannelHandlerContext;
import io.netty.handler.codec.CorruptedFrameException;
import io.netty.handler.codec.MessageToByteEncoder;

import java.util.HashMap;
import java.util.Map;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.ward.push.protocol.coder.DemuxEncoder;
import com.ward.push.protocol.coder.encoder.ConnectAckEncoder;
import com.ward.push.protocol.coder.encoder.PingRespEncoder;
import com.ward.push.protocol.messages.AbstractMessage;

/**
 * 
 * 消息编码器<br>
 * 
 * @author Administrator
 */
public class MessageEncoder extends MessageToByteEncoder<AbstractMessage> {

	private static final Logger logger = LoggerFactory.getLogger(MessageEncoder.class);

	@SuppressWarnings("rawtypes")
	private final Map<Byte, DemuxEncoder> m_encoderMap = new HashMap<Byte, DemuxEncoder>();

	public MessageEncoder() {
		m_encoderMap.put(AbstractMessage.CONNACK, new ConnectAckEncoder());
		m_encoderMap.put(AbstractMessage.PINGRESP, new PingRespEncoder());
	}

	@SuppressWarnings("unchecked")
	@Override
	protected void encode(ChannelHandlerContext ctx, AbstractMessage msg, ByteBuf out) throws Exception {
		byte messageType = msg.getMessageType();
		logger.debug("Send a message of type {}", Utils.msgType2String(messageType));
		DemuxEncoder<AbstractMessage> encoder = m_encoderMap.get(messageType);
		if (encoder == null) {
			throw new CorruptedFrameException("Can't find any message encoder for message type: "
					+ msg.getMessageType());
		}
		encoder.encode(ctx, msg, out);
	}
}
