package com.ward.push.protocol.coder.decoder;

import io.netty.buffer.ByteBuf;
import io.netty.util.AttributeMap;

import java.util.List;

import com.ward.push.protocol.coder.DemuxDecoder;
import com.ward.push.protocol.messages.req.ConnectReqMessage;

/**
 * 
 * 连接解码器<br>
 * 
 * @author L.Y
 */
public class ConnectDecoder extends DemuxDecoder {

    @Override
    public void decode(AttributeMap ctx, ByteBuf in, List<Object> out) throws Exception {
        in.resetReaderIndex();
        ConnectReqMessage message = new ConnectReqMessage();
        // 读取头
        if (!decodeCommonHeader(message, 0x00, in)) {
            in.resetReaderIndex();
            return;
        }
        // 读取clientId长度
        int clientIdLen = in.readUnsignedShort();
        byte[] clientIdBuffer = new byte[clientIdLen];
        in.readBytes(clientIdBuffer);
        String clientId = new String(clientIdBuffer, "UTF-8");
        message.setClientID(clientId);
        // 读取token长度
        int tokenLen = in.readUnsignedShort();
        byte[] tokenBuffer = new byte[tokenLen];
        in.readBytes(tokenBuffer);
        String token = new String(tokenBuffer, "UTF-8");
        message.setToken(token);
        out.add(message);
    }
}
