package com.ward.push.protocol.messages.req;

import com.ward.push.protocol.messages.AbstractMessage;

/**
 * 
 * 心跳检测请求消息<br>
 * 
 * @author L.Y
 */
public class PingReqMessage extends AbstractMessage {
    public PingReqMessage() {
        m_messageType = PINGREQ;
    }
}
