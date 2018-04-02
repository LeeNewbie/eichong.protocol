package com.ward.push.protocol.messages.req;

import com.ward.push.protocol.messages.AbstractMessage;

/**
 * 
 * client->server 连接消息<br>
 * 
 * @author L.Y
 */
public class ConnectReqMessage extends AbstractMessage {

    public ConnectReqMessage() {
        m_messageType = CONNECT;
    }
    
    protected String clientID;
    
    protected String token;

    public String getClientID() {
        return clientID;
    }

    public void setClientID(String clientID) {
        this.clientID = clientID;
    }

    public String getToken() {
        return token;
    }

    public void setToken(String token) {
        this.token = token;
    }

}
