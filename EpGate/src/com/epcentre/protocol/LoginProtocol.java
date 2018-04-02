package com.epcentre.protocol;

import com.epcentre.cache.ElectricPileCache;
import com.netCore.netty.buffer.DynamicByteBuffer;

public class LoginProtocol {

	/**
	 * 通知服务服，服务器登陆成功，并发送gate服的serverId
	 * @author 
	 * 2014-11-27
	 * @param gateServerId
	 * @return
	 */
	public static byte[] gameLoginSuccess(int gateServerId){
		
		DynamicByteBuffer byteBuffer = DynamicByteBuffer.allocate();
		
		byteBuffer.putInt(gateServerId);//gate服的serverId
		
		return byteBuffer.getBytes();
	}
	
	/**
	 * 通知服务服务器，电桩登录成功
	 * @author 
	 * 2014-11-27
	 * @param electricUser
	 * @param isReconnection
	 * @return
	 */
	public static byte[] userLoginSuccess(ElectricPileCache electricUser, boolean isReconnection){
		
		DynamicByteBuffer byteBuffer = DynamicByteBuffer.allocate();
		
		byteBuffer.putLong(electricUser.getPkEpId());//电桩id
		byteBuffer.putBoolean(isReconnection);//是否重连
		byteBuffer.putInt(electricUser.getGateid()); //电桩所在的网关
		
		return byteBuffer.getBytes();
	}
	
	/**
	 * 通知服务服务器，电桩下线
	 * @author 
	 * 2014-11-27
	 * @param electricUser
	 * @return
	 */
	public static byte[] userLoginout(ElectricPileCache electricUser){
		
		DynamicByteBuffer byteBuffer = DynamicByteBuffer.allocate();
		
		byteBuffer.putLong(electricUser.getPkEpId());//电桩id
		
		return byteBuffer.getBytes();
	}
	
}
