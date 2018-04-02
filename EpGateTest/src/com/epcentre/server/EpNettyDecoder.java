package com.epcentre.server;

import io.netty.buffer.ByteBuf;
import io.netty.channel.ChannelHandlerContext;
import io.netty.handler.codec.ByteToMessageDecoder;

import java.util.List;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.epcentre.protocol.WmIce104Util;
import com.epcentre.protocol.ep.ApciHeader;

/**
 * 收消息，解码
 * 
 * 消息结构：
 * 
 * @author lwz
 * Mar 27, 2015 12:11:06 PM
 */
public class EpNettyDecoder extends ByteToMessageDecoder {
	
	private byte[] lenBytes = new byte[ApciHeader.NUM_LEN_FIELD];
	
	private static final Logger logger = LoggerFactory.getLogger(EpNettyDecoder.class);
	
		
	@Override
	protected void decode(ChannelHandlerContext channelHandlerContext,
			ByteBuf byteBuf, List<Object> list) throws Exception {
		
		String errorMsg="";
		int readableBytes= byteBuf.readableBytes();
		if(readableBytes<7)//如果长度小于APCI长度,不读
		{
			logger.debug("decode readableBytes:{} < apci header 7,channel:{}", readableBytes,channelHandlerContext.channel());
			return;
		}
		byte bFlag104 = 0x68;
		int pos= byteBuf.bytesBefore(bFlag104);//找到0x68的位置
		int discardLen=0;
		if(pos < 0 )//没找到，全部读掉
		{
			discardLen = readableBytes;

			logger.debug("decode not find flag header 0x68,discardLen:{},channel:{}",discardLen,channelHandlerContext.channel());

			
		}
		if(pos>0)
		{
			discardLen = pos;

			logger.debug("decode find flag header 0x68 at pos:{},channel:{}",pos,channelHandlerContext.channel());

		}
		if(discardLen>0)
		{
			byte[] dicardBytes= new byte[discardLen];
			byteBuf.readBytes(dicardBytes);//
			logger.debug("discard msg:{},channel:{}",WmIce104Util.ConvertHex(dicardBytes, 0),channelHandlerContext.channel());
			
			if(discardLen == readableBytes)
			{
				//没有数据可对，还回
				return;
			}
		}
		
		
		readableBytes= byteBuf.readableBytes();
		if(readableBytes<7)
		{
			logger.debug("decode 1 readableBytes:{},channel:{}",readableBytes,channelHandlerContext.channel());
			return;
		}
		
		//1、先标记读索引（必须）
		byteBuf.markReaderIndex();
		byteBuf.readByte();
		
		//byte[] lenBytes = new byte[ApciHeader.NUM_LEN_FIELD];
		byteBuf.readBytes(lenBytes);
		int msg_len= 0;
		for(int i=0;i<ApciHeader.NUM_LEN_FIELD;i++)
		{
			byte tempb = lenBytes[i];
			int nn1=tempb&0x0FF;
			if(nn1==255 && i==1)
			{
				nn1=0;
			}
			if(0 ==i)
			{
				msg_len += nn1;
			}
			else
			{
				msg_len += nn1 <<(i*8);
			}
		}
		int remain_len = byteBuf.readableBytes();
		byte Msg[]=null;
		
		if(remain_len<msg_len )
		{

			logger.debug("ep remain_len<msg_len,remain_len:{},channel:{}",remain_len,channelHandlerContext.channel());
				
			byteBuf.resetReaderIndex();
			return ;
		}

		Msg= new byte[msg_len];
    	byteBuf.readBytes(Msg);
    	
		EpMessage gameMessage = handleIec104Msg(Msg,msg_len);
		
		list.add(gameMessage);
		
	}
		
	private EpMessage handleIec104Msg(byte msg[],int len)
	{
    	int ProtoFlag1 = msg[0]&0x0FF;
    	int ProtoFlag2 = msg[1]&0x0FF;
    	
    	short nFrameType=0;
    	
    	EpMessage gameMessage = new EpMessage();
		
		if (ProtoFlag1 ==255 && (ProtoFlag2==2 || ProtoFlag2 == 3) ) {
			//协议侦
			nFrameType=1;
            	
			
			byte[] bb = new byte[len+ApciHeader.NUM_HEAD+ ApciHeader.NUM_LEN_FIELD];
			bb[0]=0x68;
			bb[ApciHeader.NUM_HEAD]=0x0C;
			System.arraycopy(msg,0,bb,ApciHeader.NUM_HEAD+ ApciHeader.NUM_LEN_FIELD,len);
			
			gameMessage.setBytes(bb);
		} 
		else {
			gameMessage.setBytes(msg);
			byte FormatType = (byte) (msg[0] & 0x03);
			
			
			if (FormatType == 0 || FormatType == 2)
			{
				nFrameType=2; // I
			} 
			else 
			{
				if (FormatType == 1) 
					nFrameType=3;// S
				else {
					
					nFrameType=4;// U	
				}
			}
		}
		gameMessage.setFrameType(nFrameType);

		return gameMessage;
	}

}
