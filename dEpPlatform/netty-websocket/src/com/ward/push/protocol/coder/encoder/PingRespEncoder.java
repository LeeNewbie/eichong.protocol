package com.ward.push.protocol.coder.encoder;

import io.netty.buffer.ByteBuf;
import io.netty.channel.ChannelHandlerContext;

import com.ward.push.protocol.coder.DemuxEncoder;
import com.ward.push.protocol.messages.resp.PingRespMessage;

/**
 * 
 * 心跳响应编码器<br>
 * 
 * @author L.Y
 */
public class PingRespEncoder extends DemuxEncoder<PingRespMessage> {

    @Override
    public void encode(ChannelHandlerContext chc, PingRespMessage msg, ByteBuf bb) {
        bb.writeByte(msg.getMessageType() << 4);
        bb.writeByte(msg.getProtocolVersion());
    }

}
