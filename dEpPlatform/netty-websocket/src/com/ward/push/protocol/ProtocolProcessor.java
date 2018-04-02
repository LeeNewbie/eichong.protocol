package com.ward.push.protocol;

import io.netty.channel.Channel;

import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ConcurrentMap;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.ward.push.protocol.messages.req.ConnectReqMessage;
import com.ward.push.protocol.messages.resp.ConnectAckMessage;
import com.ward.push.server.ConnectionDescriptor;
import com.ward.push.server.NettyUtils;

/**
 * 
 * 协议处理器<br>
 * 
 * @author L.Y
 */
public class ProtocolProcessor {

	private static final Logger logger = LoggerFactory.getLogger(ProtocolProcessor.class);

	protected ConcurrentMap<String, ConnectionDescriptor> m_clientIDs;

	public ProtocolProcessor() {
		this.m_clientIDs = new ConcurrentHashMap<String, ConnectionDescriptor>();
	}

	/**
	 * 
	 * 处理Connect消息 <br>
	 * 
	 * @param channel
	 */
	public void processConnect(Channel channel, ConnectReqMessage msg) {
		logger.debug("clientId:{}准备建立连接!", msg.getClientID());
		// 不符合版本号
		if (msg.getProtocolVersion() != ConnectReqMessage.VERSION) {
			ConnectAckMessage badProto = new ConnectAckMessage();
			badProto.setReturnCode(ConnectAckMessage.UNNACEPTABLE_PROTOCOL_VERSION);
			logger.warn("clientId:{}建立连接消息不满足协议,协议中未带版本号!", msg.getClientID());
			channel.writeAndFlush(badProto);
			channel.close();
			return;
		}

		// 判断是否符合要求
		if (msg.getClientID() == null || msg.getClientID().length() == 0) {
			ConnectAckMessage okResp = new ConnectAckMessage();
			okResp.setReturnCode(ConnectAckMessage.IDENTIFIER_REJECTED);
			channel.writeAndFlush(okResp);
			return;
		}

		if (m_clientIDs.containsKey(msg.getClientID())) {
			logger.info("clientId:{}从缓存中发现存在的老连接,准备关闭!", msg.getClientID());
			Channel oldChannel = m_clientIDs.get(msg.getClientID()).channel;
			oldChannel.close();
			logger.debug("clientId:{}缓存中已经存在的老连接关闭完成!", msg.getClientID());
		}
		NettyUtils.clientID(channel, msg.getClientID());

		ConnectionDescriptor connDescr = new ConnectionDescriptor(msg.getClientID(), channel);
		m_clientIDs.put(msg.getClientID(), connDescr);
		ConnectAckMessage okResp = new ConnectAckMessage();
		okResp.setReturnCode(ConnectAckMessage.CONNECTION_ACCEPTED);
		channel.writeAndFlush(okResp);

		logger.debug("clientId:{}连接建立完成!", msg.getClientID());
	}
}
