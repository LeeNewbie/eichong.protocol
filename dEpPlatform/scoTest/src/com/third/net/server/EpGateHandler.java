package com.third.net.server;

import io.netty.channel.Channel;

import java.nio.ByteBuffer;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.third.net.codec.EpGateDecoder;
import com.third.net.codec.EpMessage;
import com.third.net.constant.Third2EpGate;
import com.third.service.EpGateService;

/**
 * 接受电桩客户端数据并处理
 * 
 * @author 2014-12-1 下午2:58:22
 */
public class EpGateHandler {

	private static final Logger logger = LoggerFactory
			.getLogger(EpGateHandler.class);

	
	//private static final Logger log = LoggerFactory.getLogger("FlashRamLog");
	/**
	 * 接受电桩发送的消息进行处理
	 * 
	 * @author lwz 2015-3-19
	 * @param channel
	 * @param message
	 */
	public static void handleMessage(Channel channel, EpMessage message) {

		int cmd = message.getCmd();
		if(!EpGateService.isValidCmd(cmd))
		{
			logger.error("handleMessage error! invalid cmd:{},{}",cmd,channel);
			return ;
		}

		ByteBuffer byteBuffer = ByteBuffer.wrap(message.getBytes());
		
		switch (cmd) 
		{
		case Third2EpGate.EP_HEART:
			EpGateDecoder.decodeHeart(channel, byteBuffer);
			break;
		case Third2EpGate.EP_REAL_DATA:
			EpGateDecoder.decodeRealData(channel, byteBuffer);
			break;
		case Third2EpGate.EP_CONSUME_RECORD:
			EpGateDecoder.decodeConsumeRecord(channel, byteBuffer);
			break;
		}
		
		message = null;
	}
	


}
