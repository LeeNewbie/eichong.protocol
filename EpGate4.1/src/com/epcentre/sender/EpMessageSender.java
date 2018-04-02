package com.epcentre.sender;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import io.netty.channel.Channel;
import io.netty.channel.ChannelFuture;
import io.netty.channel.ChannelFutureListener;

import com.epcentre.cache.EpCommClient;
import com.epcentre.cache.INetObject;
import com.epcentre.config.GameConfig;
import com.epcentre.protocol.EpEncodeProtocol;
import com.epcentre.service.EpCommClientService;
import com.epcentre.service.EpGunService;
import com.epcentre.utils.DateUtil;
import com.netCore.queue.RepeatMessage;

public class EpMessageSender {
	
	private static final Logger logger = LoggerFactory.getLogger(EpMessageSender.class);
	
	public static ChannelFuture  sendMessage(INetObject commClient,int cos,int common_address,int recordType,byte[] msg
			,byte []time,int commVersion) {
		
		EpCommClient  epCommClient= (EpCommClient)commClient;
		int sendINum = epCommClient.getSendINum2();
		int recvINum = epCommClient.getRevINum();
		
		
		byte[] sendData= EpEncodeProtocol.Package(sendINum,recvINum,
				cos,common_address,(byte)recordType,msg,time,commVersion);
		
		
		return EpMessageSender.sendMessage(commClient, sendData);
		
	}
	public static ChannelFuture  sendRepeatMessage(INetObject commClient,String repeatMsgKey,int cos,int common_address,int recordType,byte[] msg
			,byte []time,int commVersion) {
		
		EpCommClient  epCommClient= (EpCommClient)commClient;
		int sendINum = epCommClient.getSendINum2();
		int recvINum = epCommClient.getRevINum();
		
		byte[] sendData= EpEncodeProtocol.Package(sendINum,recvINum,
				cos,common_address,(byte)recordType,msg,time,commVersion);
		
		//1.先立即发送
		sendMessage(epCommClient,sendData);
		
		if(GameConfig.resendEpMsgFlag ==1 && epCommClient.getVersion()>=4)
		{
			RepeatMessage repeatMsg= 
					new RepeatMessage(epCommClient.getChannel(),3,GameConfig.resendEpMsgTime,repeatMsgKey,sendData);
			
			repeatMsg.setLastSendTime( DateUtil.getCurrentSeconds());
			EpGunService.putSendMsg(repeatMsg);
		}
		
		return null;
		
	}
	
	public static ChannelFuture  sendMessage(INetObject commClient,Object object) {
		
		Channel channel = commClient.getChannel();
		if(channel == null)
		{
			logger.info("sendMessage  channel  is null\n");
			return null;
		}
		
		if (!channel.isWritable()) {
			logger.info("sendMessage  is not Writable  channe:{}\n",channel);
			return null;
		}
		
		channel.writeAndFlush(object);
		
		return null;
	}


}
