package com.ward.push.protocol.messages.resp;

import com.ward.push.protocol.messages.AbstractMessage;

/**
 * 
 * server->client连接确认消息<br>
 * 
 * @author L.Y
 */
public class ConnectAckMessage extends AbstractMessage {

	public static final byte CONNECTION_ACCEPTED = 0x00;

	public static final byte UNNACEPTABLE_PROTOCOL_VERSION = 0x01;

	public static final byte IDENTIFIER_REJECTED = 0x02;

	public ConnectAckMessage() {
		m_messageType = CONNACK;
		protocolVersion = VERSION;
	}

	private byte returnCode;

	public byte getReturnCode() {
		return returnCode;
	}

	public void setReturnCode(byte returnCode) {
		this.returnCode = returnCode;
	}

}
