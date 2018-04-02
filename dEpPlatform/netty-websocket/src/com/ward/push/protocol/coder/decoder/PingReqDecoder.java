package com.ward.push.protocol.coder.decoder;

import io.netty.buffer.ByteBuf;
import io.netty.util.AttributeMap;

import java.util.List;

import com.ward.push.protocol.coder.DemuxDecoder;
import com.ward.push.protocol.messages.req.PingReqMessage;

/**
 * 
 * 心跳检测解码器<br>
 * 
 * @author L.Y
 */
public class PingReqDecoder extends DemuxDecoder {

    @Override
    public void decode(AttributeMap ctx, ByteBuf in, List<Object> out) throws Exception {
        in.resetReaderIndex();
        PingReqMessage message = new PingReqMessage();
        if (!decodeCommonHeader(message, 0x00, in)) {
            in.resetReaderIndex();
            return;
        }
        out.add(message);
    }
}
