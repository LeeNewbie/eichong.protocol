package com.ward.push.protocol.coder;

import io.netty.buffer.ByteBuf;
import io.netty.util.AttributeMap;

import java.util.List;

import com.ward.push.protocol.messages.AbstractMessage;

/**
 * 
 * 多路复用解码器<br>
 * 
 * @author L.Y
 */
public abstract class DemuxDecoder {

    public abstract void decode(AttributeMap ctx, ByteBuf in, List<Object> out) throws Exception;

    protected boolean decodeCommonHeader(AbstractMessage message, int expectedFlags, ByteBuf in) {
        return genericDecodeCommonHeader(message, expectedFlags, in);
    }

    private boolean genericDecodeCommonHeader(AbstractMessage message, Integer expectedFlagsOpt, ByteBuf in) {
        if (in.readableBytes() < 2) {
            return false;
        }
        // 读取messageType
        byte byte1 = in.readByte();
        byte messageType = (byte) ((byte1 & 0x00F0) >> 4);
        message.setMessageType(messageType);
        // 读取版本
        byte byte2 = in.readByte();
        message.setProtocolVersion(byte2);
        if (message.getProtocolVersion() == AbstractMessage.VERSION) {

        }
        return true;
    }
}
