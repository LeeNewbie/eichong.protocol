package com.epcentre.sender;

import io.netty.channel.Channel;
import io.netty.channel.ChannelFuture;

import com.epcentre.cache.EpCommClient;
import com.epcentre.cache.INetObject;
import com.epcentre.protocol.EpEncodeProtocol;
import com.epcentre.protocol.Iec104Constant;
import com.epcentre.server.InnerApiMessageSender;
import com.epcentre.service.EpCommClientService;
import com.netCore.queue.RepeatMessage;

public class EpMessageSender {
	 
	
	public static ChannelFuture  sendMessage(INetObject commClient,int cos,int common_address,int recordType,byte[] msg
			,byte []time,int commVersion) {
		
		EpCommClient  epCommClient= (EpCommClient)commClient;
		int sendINum = epCommClient.getSendINum2();
		int recvINum = epCommClient.getRevINum();
		
		
		byte[] sendData= EpEncodeProtocol.Package(sendINum,recvINum,
				cos,common_address,(byte)recordType,msg,time,commVersion);
		
		
		return InnerApiMessageSender.sendMessage(commClient.getChannel(), sendData);
		
	}
	public static ChannelFuture  sendRepeatMessage(INetObject commClient,String repeatMsgKey,int cos,int common_address,int recordType,byte[] msg
			,byte []time,int commVersion) {
		
		EpCommClient  epCommClient= (EpCommClient)commClient;
		int sendINum = epCommClient.getSendINum2();
		int recvINum = epCommClient.getRevINum();
		
		
		byte[] sendData= EpEncodeProtocol.Package(sendINum,recvINum,
				cos,common_address,(byte)recordType,msg,time,commVersion);
			
		RepeatMessage chargeMsg= new RepeatMessage(epCommClient.getChannel(),repeatMsgKey,sendData);
		EpCommClientService.putRepeatMsg(chargeMsg);
		return null;
		
	}


}
