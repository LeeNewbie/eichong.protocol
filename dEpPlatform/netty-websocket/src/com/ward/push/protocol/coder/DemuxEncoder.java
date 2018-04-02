package com.ward.push.protocol.coder;

import io.netty.buffer.ByteBuf;
import io.netty.channel.ChannelHandlerContext;

import com.ward.push.protocol.messages.AbstractMessage;

/**
 * 
 * 多路复用编码器<br>
 * 
 * @author L.Y
 */
public abstract class DemuxEncoder<T extends AbstractMessage> {

    public abstract void encode(ChannelHandlerContext chc, T msg, ByteBuf bb);

}
