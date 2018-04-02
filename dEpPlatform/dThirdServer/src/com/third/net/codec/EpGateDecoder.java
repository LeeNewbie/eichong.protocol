package com.third.net.codec;

import io.netty.buffer.ByteBuf;
import io.netty.channel.Channel;
import io.netty.channel.ChannelHandlerContext;
import io.netty.handler.codec.ByteToMessageDecoder;

import java.nio.ByteBuffer;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.netCore.util.ByteUtil;
import com.third.config.FieldConfig;
import com.third.config.GameConfig;
import com.third.constant.BaseConstant;
import com.third.constant.EpProtocolConstant;
import com.third.net.constant.InnerHead;
import com.third.net.constant.Third2EpGate;
import com.third.protocol.UtilProtocol;
import com.third.protocol.WmIce104Util;
import com.third.service.EpGateService;

/**
 * 收消息，解码
 * 
 * 消息结构：
 * 
 * @author lwz
 * Mar 27, 2015 12:11:06 PM
 */
public class EpGateDecoder extends ByteToMessageDecoder {
	
	
	private static final Logger logger = LoggerFactory.getLogger(EpGateDecoder.class.getName() + BaseConstant.SPLIT + GameConfig.serverName);
	public static final int LEN_READABLE = 6;
		
	@Override
	protected void decode(ChannelHandlerContext channelHandlerContext,
			ByteBuf byteBuf, List<Object> list) throws Exception {
		
		int readableBytes= byteBuf.readableBytes();
		if(readableBytes<LEN_READABLE)//如果长度小于长度,不读
		{
		
			logger.debug("decode 1 readableBytes<{},readableBytes:{},channel:{}", new Object[]{LEN_READABLE,readableBytes,channelHandlerContext.channel()});

			return;
		}
		
		int pos= byteBuf.bytesBefore(InnerHead.HEAD_FLAG1);//找到的位置
		int pos1= byteBuf.bytesBefore(InnerHead.HEAD_FLAG2);//找到的位置
		int discardLen=0;
		if(pos < 0 || pos1<0 || (pos1-pos)!=1)//没找到，全部读掉
		{
			discardLen = readableBytes;
			logger.debug("decode not find flag header 0x45 0x43,readableBytes:{},channel:{}",readableBytes,channelHandlerContext.channel());
		}
		if(pos>0&&(pos1-pos)==1)
		{
			discardLen = pos;	
			logger.debug("decode find flag header 0x45 0x43,pos:{},channel:{}",pos,channelHandlerContext.channel());
		}
		if(discardLen>0)
		{
			byte[] dicardBytes= new byte[discardLen];
			byteBuf.readBytes(dicardBytes);//
			
			logger.debug("discard>0 msg:{},channel:{}",WmIce104Util.ConvertHex(dicardBytes, 0),channelHandlerContext.channel());
			
			if(discardLen == readableBytes)
			{
				//没有数据可对，还回
				return;
			}
		}
		
		readableBytes= byteBuf.readableBytes();
		if(readableBytes<LEN_READABLE)
		{
			logger.debug("decode readableBytes<{} readableBytes:{},channel:{}",new Object[]{LEN_READABLE,readableBytes,channelHandlerContext.channel()});
			
			return;
		}
		
		//1、先标记读索引（必须）
		byteBuf.markReaderIndex();
		
		//读取协议头
		byteBuf.readShort();
			
		int msg_len = byteBuf.readShort();
		int remain_len = byteBuf.readableBytes();

		if(remain_len<msg_len )
		{
			logger.debug("ep remain_len<msg_len,remain_len:{},channel:{}", remain_len, channelHandlerContext.channel());
			
				
			byteBuf.resetReaderIndex();
			return ;
		}
		
		int cmd = byteBuf.readShort();
		
		byte Msg[]= null;
		Msg= new byte[msg_len-2];
    	byteBuf.readBytes(Msg);

    	EpMessage message = new EpMessage();
    			
    	message.setLength(msg_len);
    	message.setCmd(cmd);

    	message.setBytes(Msg);
    			
    	list.add(message);
		
	}

	/**
	 * 心跳
	 */
	public static void decodeHeart(Channel channel,ByteBuffer byteBuffer)
	{
		EpGateService.handleHeart(channel);
	}

	/**
	 * 实时数据
	 * @param channel
	 * @param byteBuffer
	 */
	public static void decodeRealData(Channel channel,ByteBuffer byteBuffer)
	{
		Map<String,Object> pointMap = fieldConfigClone(EpGateService.getThirdConfigs().getField(BaseConstant.REAL_FIELDS));

    	int orgNo =(int)byteBuffer.getInt();
	   	pointMap.put(EpProtocolConstant.ORG_NO, orgNo);
		String epCode = ByteUtil.getString(byteBuffer);
		int epGunNo = (int)byteBuffer.get();
		String key = epCode + BaseConstant.SPLIT + epGunNo;

		int epType = (int)byteBuffer.get();
		key += BaseConstant.SPLIT + epType;
		key += BaseConstant.SPLIT + Third2EpGate.EP_REAL_DATA;
		Map<String,Object> oldMap = EpGateService.getRealData(key);
		if (oldMap != null && oldMap.size() > 0 && oldMap.get(EpProtocolConstant.STATUS_TIME) != null) {
			String oldKey = key + BaseConstant.SPLIT + BaseConstant.OLD;
			EpGateService.removeRealData(oldKey);
			EpGateService.addRealData(oldKey, oldMap);
			EpGateService.removeRealData(key);
		}

		int num = (int)byteBuffer.getShort();
		short addr;
		int type,value;
		FieldConfig fieldConfig;

		for(int i=0;i<num;i++)
	    {
	        addr = byteBuffer.getShort();
	        type =(int)byteBuffer.get();
	        if (type == EpProtocolConstant.YC) {
	        	value =(int)byteBuffer.getShort();
	        } else if (type == EpProtocolConstant.YC_VAR) {
	        	value =(int)byteBuffer.getInt();
	        } else {
	        	value =(int)byteBuffer.get();
	        }
	        fieldConfig = (FieldConfig)pointMap.get(type + BaseConstant.SPLIT + addr);
	        if (fieldConfig != null) {
	        	fieldConfig.setValue(value);
		        if (EpProtocolConstant.YC == type && EpProtocolConstant.YC_WORKSTATUS == addr) pointMap.put(type + BaseConstant.SPLIT + addr, value);
		        logger.debug("realData key:{},type:{},addr:{},value:{}",new Object[]{key,type,addr,value});
	        }
	    }

		EpGateService.addRealData(key, pointMap);
	}

	/**
	 * 消费记录
	 * @param channel
	 * @param byteBuffer
	 */
	public static void decodeConsumeRecord(Channel channel,ByteBuffer byteBuffer)
	{
		Map<String,Object> pointMap = new HashMap<String,Object>();

	   	int orgNo =(int)byteBuffer.getInt();
	   	pointMap.put(EpProtocolConstant.ORG_NO, orgNo);
		String epCode = ByteUtil.getString(byteBuffer);
		int epGunNo = (int)byteBuffer.get();
		String key = epCode + BaseConstant.SPLIT + epGunNo;

		pointMap.put(EpProtocolConstant.SERIALNO, ByteUtil.getString(byteBuffer));
		pointMap.put(EpProtocolConstant.STARTTIME, new Long(byteBuffer.getInt()));
		pointMap.put(EpProtocolConstant.ENDTIME, new Long(byteBuffer.getInt()));
		if ((long)pointMap.get(EpProtocolConstant.STARTTIME) >= (long)pointMap.get(EpProtocolConstant.ENDTIME)) {
			logger.debug("ConsumeRecord key:{} pointMap:{}",new Object[]{key,pointMap});
			return;
		}
		int value = byteBuffer.getInt();
		if (value == 0) {
			logger.debug("ConsumeRecord pq == 0");
			return;
		}
    	pointMap.put(EpProtocolConstant.PQ, UtilProtocol.intToBigDecimal3(value));

		logger.debug("ConsumeRecord key:{} pointMap:{}",new Object[]{key,pointMap});
		EpGateService.addRealData(key, pointMap);
	}
	
	private static Map<String,Object> fieldConfigClone(Map<String, FieldConfig> fieldCfg)
	{
		Map<String,Object> pointMap = new HashMap<String,Object>();

		for(String dataKey : fieldCfg.keySet()) {
			if (!dataKey.contains(BaseConstant.SPLIT)) {
				FieldConfig fieldConfig = fieldCfg.get(dataKey);
	            String type = fieldConfig.getType();
	            String addr = fieldConfig.getAddr();

	            FieldConfig epgateConfig = new FieldConfig();
	            epgateConfig.setType(type);
	            epgateConfig.setAddr(addr);

	            pointMap.put(dataKey, epgateConfig);
	            pointMap.put(type + BaseConstant.SPLIT + addr, epgateConfig);
			}
		}
		
		return pointMap;
	}
}
