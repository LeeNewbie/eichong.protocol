package com.ward.push.protocol.coder.encoder;

import io.netty.buffer.ByteBuf;
import io.netty.channel.ChannelHandlerContext;

import com.ward.push.protocol.coder.DemuxEncoder;
import com.ward.push.protocol.messages.resp.ConnectAckMessage;

/**
 * 
 * 连接确认<br>
 * 
 * @author L.Y
 */
public class ConnectAckEncoder extends DemuxEncoder<ConnectAckMessage> {

    @Override
    public void encode(ChannelHandlerContext chc, ConnectAckMessage msg, ByteBuf bb) {
        bb.writeByte(msg.getMessageType() << 4);
        bb.writeByte(msg.getProtocolVersion());
        bb.writeByte(msg.getReturnCode());
    }
}
