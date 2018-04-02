package com.ward.push.protocol.messages;

/**
 * 
 * Basic Message<br>
 * 
 * @author L.Y
 */
public abstract class AbstractMessage {

    public static final byte VERSION = 0x03;// 协议版本

    public static final byte CONNECT = 1; // Client Connect Server

    public static final byte CONNACK = 2; // Connect Server ACK

    public static final byte PUBLISH = 3; // Publish message

    public static final byte PUBACK = 4; // Publish message ACK

    public static final byte SUBSCRIBE = 8; // Client Subscribe

    public static final byte SUBACK = 9; // Subscribe ACK

    public static final byte UNSUBSCRIBE = 10; // Client Unsubscribe request

    public static final byte UNSUBACK = 11; // Unsubscribe ACK

    public static final byte PINGREQ = 12; // PING Request

    public static final byte PINGRESP = 13; // PING Response

    public static final byte DISCONNECT = 14; // Client is Disconnecting

    protected byte m_messageType; // 消息类型

    protected byte protocolVersion;// 版本

    public byte getMessageType() {
        return m_messageType;
    }

    public void setMessageType(byte m_messageType) {
        this.m_messageType = m_messageType;
    }

    public byte getProtocolVersion() {
        return protocolVersion;
    }

    public void setProtocolVersion(byte protocolVersion) {
        this.protocolVersion = protocolVersion;
    }
}
