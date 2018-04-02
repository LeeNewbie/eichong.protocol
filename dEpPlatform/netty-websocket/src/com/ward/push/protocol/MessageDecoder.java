package com.ward.push.protocol;

import io.netty.buffer.ByteBuf;
import io.netty.channel.ChannelHandlerContext;
import io.netty.handler.codec.ByteToMessageDecoder;
import io.netty.handler.codec.CorruptedFrameException;

import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.ward.push.protocol.coder.DemuxDecoder;
import com.ward.push.protocol.coder.decoder.ConnectDecoder;
import com.ward.push.protocol.coder.decoder.PingReqDecoder;
import com.ward.push.protocol.messages.AbstractMessage;

/**
 * 
 * 消息解码器<br>
 * 
 * @author L.Y
 */
public class MessageDecoder extends ByteToMessageDecoder {

	private static final Logger logger = LoggerFactory.getLogger(MessageEncoder.class);

	private final Map<Byte, DemuxDecoder> m_decoderMap = new HashMap<Byte, DemuxDecoder>();

	public MessageDecoder() {
		m_decoderMap.put(AbstractMessage.CONNECT, new ConnectDecoder());
		m_decoderMap.put(AbstractMessage.PINGREQ, new PingReqDecoder());
	}

	@Override
	protected void decode(ChannelHandlerContext ctx, ByteBuf in, List<Object> out) throws Exception {
		in.markReaderIndex();
		in.resetReaderIndex();
		byte messageType = Utils.readMessageType(in);
		logger.debug("Received a message of type {}", Utils.msgType2String(messageType));
		DemuxDecoder decoder = m_decoderMap.get(messageType);
		if (decoder == null) {
			throw new CorruptedFrameException("Can't find any message decoder for message type: " + messageType);
		}
		decoder.decode(ctx, in, out);
	}

}
