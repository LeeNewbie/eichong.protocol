package com.usrlayer.net.codec;

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
import com.usrlayer.cache.ChargingInfo;
import com.usrlayer.config.FieldConfig;
import com.usrlayer.config.GameBaseConfig;
import com.usrlayer.constant.BaseConstant;
import com.usrlayer.constant.EpGateConstant;
import com.usrlayer.constant.EpProtocolConstant;
import com.usrlayer.net.client.EpGateMessage;
import com.usrlayer.protocol.UtilProtocol;
import com.usrlayer.protocol.WmIce104Util;
import com.usrlayer.service.EpGateService;

/**
 * 收消息，解码   
 * 
 * 消息结构：2字节协议头+ 2字节长度 (小端)+ 1字节原因+2字节命令类型  + byte[] 数据内容

 */
public class EpGateDecoder extends ByteToMessageDecoder {
	
	private static final Logger logger = LoggerFactory.getLogger(EpGateDecoder.class + BaseConstant.SPLIT + GameBaseConfig.layerName);
	
	@Override
	protected void decode(ChannelHandlerContext channelHandlerContext,
			ByteBuf byteBuf, List<Object> list) throws Exception {
		
		int readableBytes= byteBuf.readableBytes();
		//协议头6个字节,如果没有6个字节不读
		if(readableBytes < 6)
		{
			logger.debug("decode 1 readableBytes<6,readableBytes:{},channel:{}", readableBytes,channelHandlerContext.channel());
			return;
		}
		
		int pos= byteBuf.bytesBefore(EpGateConstant.HEAD_FLAG1);//找到的位置
		int pos1= byteBuf.bytesBefore(EpGateConstant.HEAD_FLAG2);//找到的位置
		int discardLen=0;
		if(pos < 0 || pos1<0 || (pos1-pos)!=1)//没找到，全部读掉
		{
			discardLen = readableBytes;
			logger.debug("decode not find flag header 0x45 0x43,readableBytes:{},channel:{}",readableBytes,channelHandlerContext.channel());
		}
		if(pos>0&&(pos1-pos)==1)
		{
			discardLen = pos;	
			logger.debug("decode  find flag header 0x45 0x43,pos:{},channel:{}",pos,channelHandlerContext.channel());
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
		if(readableBytes<6)
		{
			logger.debug("decode readableBytes<6 readableBytes:{},channel:{}",readableBytes,channelHandlerContext.channel());
			
			return;
		}
		
		//1、先标记读索引（必须）
		byteBuf.markReaderIndex();	
		
		//读取协议头
		byteBuf.readShort();
		//读长度
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
    
    	EpGateMessage message = new EpGateMessage();
    			
    	message.setLength(msg_len);
    	message.setCmd(cmd);

    	message.setBytes(Msg);
    			
    	list.add(message);
	}
	
	/**
	 * 网关登录(连接初始化)
	 */
	public static void decodeLogin(Channel channel,ByteBuffer byteBuffer)
	{
		int h = (int)byteBuffer.get();
		int m = (int)byteBuffer.get();
		int s = (int)byteBuffer.get();
		int usrGateId = byteBuffer.getInt();
		int epGateId = byteBuffer.getInt();
		int ret = (int)byteBuffer.get();
		short errorCode = byteBuffer.getShort();
		EpGateService.handleEpGateLogin(channel, h,m,s,usrGateId,epGateId,ret,errorCode);
	}
	
	/**
	 * 心跳
	 */
	public static void decodeHeart(Channel channel,ByteBuffer byteBuffer)
	{
		EpGateService.handleHeart(channel);
	}
	
	/**
	 * ACK响应
	 */
	public static void decodeAck(Channel channel,ByteBuffer byteBuffer)
	{
		short cmd  = byteBuffer.getShort();
		int h = (int)byteBuffer.get();
		int m = (int)byteBuffer.get();
		int s = (int)byteBuffer.get();
		long usrId = byteBuffer.getLong();
		
		EpGateService.handleAck(channel,cmd,h,m,s,usrId);
	}

	/**
	 * 电桩在线
	 */
	public static void decodeEpOnline(Channel channel,ByteBuffer byteBuffer)
	{
		int h = (int)byteBuffer.get();
		int m = (int)byteBuffer.get();
		int s = (int)byteBuffer.get();
		int online = (int)byteBuffer.get();
		int epNum = byteBuffer.getShort();
		
		String[] epCode = new String[epNum];
		for(int i=0;i<epNum;i++)
		{
			epCode[i] = ByteUtil.getString(byteBuffer);
		}
		
		EpGateService.handleEpOnline(channel, h,m,s,online,epCode);	
	}

	/**
	 * 手机在线
	 */
	public static void decodePhoneOnline(Channel channel,ByteBuffer byteBuffer)
	{
		int h = (int)byteBuffer.get();
		int m = (int)byteBuffer.get();
		int s = (int)byteBuffer.get();
		
		EpGateService.handlePhoneOnline(channel, h,m,s);
	}

	/**
	 * 手机连接初始化
	 */
	public static void decodePhoneConnect(Channel channel,ByteBuffer byteBuffer)
	{
		int h = (int)byteBuffer.get();
		int m = (int)byteBuffer.get();
		int s = (int)byteBuffer.get();
		String epCode = ByteUtil.getString(byteBuffer);
		int epGunNo = (int)byteBuffer.get();
		long usrId = byteBuffer.getLong();

		int ret = (int)byteBuffer.get();
		short errorCode = byteBuffer.getShort();
		int status = (int)byteBuffer.get();

		EpGateService.handlePhoneConnect(channel, h,m,s,epCode,epGunNo,usrId,ret,errorCode,status);
	}

	/**
	 * 充电
	 */
	public static void decodeCharge(Channel channel,ByteBuffer byteBuffer)
	{
		int h = (int)byteBuffer.get();
		int m = (int)byteBuffer.get();
		int s = (int)byteBuffer.get();
		
		String epCode = ByteUtil.getString(byteBuffer);
		int epGunNo = (int)byteBuffer.get();
		long usrId = byteBuffer.getLong();

		int ret = (int)byteBuffer.get();
		short errorCode = byteBuffer.getShort();
		
		EpGateService.handleCharge(channel, h,m,s,epCode,epGunNo,usrId,ret,errorCode);
	}

	/**
	 * 充电事件
	 */
	public static void decodeChargeEvent(Channel channel,ByteBuffer byteBuffer)
	{
		int h = (int)byteBuffer.get();
		int m = (int)byteBuffer.get();
		int s = (int)byteBuffer.get();
		
		String epCode = ByteUtil.getString(byteBuffer);
		int epGunNo = (int)byteBuffer.get();
		long usrId = byteBuffer.getLong();
		
		int status = (int)byteBuffer.get();
		
		EpGateService.handleChargeEvent(channel, h,m,s,epCode,epGunNo,usrId,status);
	}

	/**
	 * 停止充电
	 */
	public static void decodeStopCharge(Channel channel,ByteBuffer byteBuffer)
	{
		int h = (int)byteBuffer.get();
		int m = (int)byteBuffer.get();
		int s = (int)byteBuffer.get();
		
		String epCode = ByteUtil.getString(byteBuffer);
		int epGunNo = (int)byteBuffer.get();
		long usrId = byteBuffer.getLong();

		int ret = (int)byteBuffer.get();
		short errorCode = byteBuffer.getShort();

		EpGateService.handleStopCharge(channel, h,m,s,epCode,epGunNo,usrId,ret,errorCode);
	}

	/**
	 * 充电实时数据
	 */
	public static void decodeChargeReal(Channel channel,ByteBuffer byteBuffer)
	{
		int h = (int)byteBuffer.get();
		int m = (int)byteBuffer.get();
		int s = (int)byteBuffer.get();
		
		String epCode = ByteUtil.getString(byteBuffer);
		int epGunNo = (int)byteBuffer.get();
		long usrId = byteBuffer.getLong();
		
		ChargingInfo chargingInfo = new ChargingInfo();
		chargingInfo.setWorkStatus(byteBuffer.get());
		chargingInfo.setTotalTime(byteBuffer.getShort());
		chargingInfo.setOutVol(byteBuffer.getShort());
		chargingInfo.setOutCurrent(byteBuffer.getShort());
		chargingInfo.setChargeMeterNum(byteBuffer.getInt());
		chargingInfo.setRateInfo(byteBuffer.getShort());
		chargingInfo.setFronzeAmt(byteBuffer.getInt());
		chargingInfo.setChargeAmt(byteBuffer.getInt());
		chargingInfo.setSoc(byteBuffer.get());
		chargingInfo.setDeviceStatus(byteBuffer.getInt());
		chargingInfo.setWarns(byteBuffer.getInt());
		
		EpGateService.handleChargeReal(channel, h,m,s,epCode,epGunNo,usrId,chargingInfo);
	}

	/**
	 * 消费记录
	 */
	public static void decodeConsumeRecord(Channel channel,ByteBuffer byteBuffer)
	{
		int h = (int)byteBuffer.get();
		int m = (int)byteBuffer.get();
		int s = (int)byteBuffer.get();
		
		String epCode = ByteUtil.getString(byteBuffer);
		int epGunNo = (int)byteBuffer.get();
		long usrId = byteBuffer.getLong();

		Map<String, Object> consumeRecordMap = new HashMap<String, Object>();
		consumeRecordMap.put("orderid",ByteUtil.getString(byteBuffer));
		consumeRecordMap.put("st",byteBuffer.getLong());
		consumeRecordMap.put("et",byteBuffer.getLong());
		consumeRecordMap.put("totalMeterNum",byteBuffer.getInt());
		consumeRecordMap.put("totalAmt",byteBuffer.getInt());
		consumeRecordMap.put("serviceAmt",byteBuffer.getInt());
		consumeRecordMap.put("pkEpId",byteBuffer.getInt());

		EpGateService.handleConsumeRecord(channel, h,m,s,epCode,epGunNo,usrId,consumeRecordMap);
	}

	/**
	 * 重新插枪提示
	 */
	public static void decodeReInsertGun(Channel channel,ByteBuffer byteBuffer)
	{
	}

	public static void decodeYxyc(Channel channel,ByteBuffer byteBuffer)
	{
	}

	/**
	 * 实时数据
	 * @param channel
	 * @param byteBuffer
	 */
	public static void decodeThirdData(Channel channel,ByteBuffer byteBuffer)
	{
		Map<String,Object> pointMap = fieldConfigClone(EpGateService.getThirdConfigs().getField(BaseConstant.REAL_FIELDS));

    	int orgNo =(int)byteBuffer.getInt();
	   	pointMap.put(EpProtocolConstant.ORG_NO, orgNo);
		String epCode = ByteUtil.getString(byteBuffer);
		int epGunNo = (int)byteBuffer.get();
		String key = epCode + BaseConstant.SPLIT + epGunNo;

		int epType = (int)byteBuffer.get();
		key += BaseConstant.SPLIT + epType;
		key += BaseConstant.SPLIT + EpGateConstant.EP_THIRD_DATA;
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
	public static void decodeThirdRecord(Channel channel,ByteBuffer byteBuffer)
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
