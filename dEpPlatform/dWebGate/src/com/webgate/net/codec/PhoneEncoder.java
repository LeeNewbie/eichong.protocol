package com.webgate.net.codec;

import io.netty.buffer.ByteBuf;
import io.netty.channel.ChannelHandlerContext;
import io.netty.handler.codec.MessageToByteEncoder;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.webgate.config.GameConfig;
import com.webgate.constant.BaseConstant;
import com.webgate.net.server.PhoneMessage;

/**
 * 发消息，编码
 * 
 * 消息结构：byte混淆码1 + byte混淆吗2 + int长度  + short协议号  + byte是否压缩  + byte[] 数据内容 + byte混淆码3 + byte混淆码4
 * 
 * @author haojian
 * Mar 27, 2013 4:11:15 PM
 */
public class PhoneEncoder extends MessageToByteEncoder<PhoneMessage>{


	private static final Logger logger = LoggerFactory.getLogger(PhoneEncoder.class + BaseConstant.SPLIT + GameConfig.serverName);
	
	
	/**
	 * 不管channel.write(arg0)发送的是什么类型，
	 * 最终都要组装成 ByteBuf 发送,
	 * 所以encode需要返回 ByteBuf 类型的对象
	 
	 * @param chc
	 * @param bb   (Message)
	 * @param byteBuf   (Byte)
	 * @return
	 * @throws Exception
	 */
	@Override
	protected void encode(ChannelHandlerContext chc, PhoneMessage msg, ByteBuf byteBuf)
			throws Exception {

		if(msg instanceof PhoneMessage){
			
			byteBuf.writeByte(msg.getCmd());
			byteBuf.writeByte(msg.getCos());
			byteBuf.writeBytes(msg.getBytes());
			
		}else{
			
			logger.debug("未知的消息类型:{}",chc.channel().toString());
			
		}
		
		
	}




}
