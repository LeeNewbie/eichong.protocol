package com.epcentre.cache;

import io.netty.channel.Channel;

import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.epcentre.constant.EpProtocolConstant;
import com.epcentre.service.EpGunService;

/**
 * 世界
 * @author 
 * 2014-11-27 上午11:34:48
 */
public class EpCache {
	private static final Logger logger = LoggerFactory.getLogger(EpCache.class);
	
	
	//充电桩二进制程序缓存，KEY={公司名字_version_sectionindex}
	public static Map<String, char[]> mapEpExeBin=new ConcurrentHashMap<String,char[]>();
	
	//充电桩二进制程序缓存，epcode 2 版本
	public static Map<String, ExeCacheObject> mapEpExeVersion=new ConcurrentHashMap<String,ExeCacheObject>();
	
	
	
	public static Map<String,EpFault> mapEpFault=new ConcurrentHashMap<String,EpFault >();

	
	
		
	 static {
	        //初始化加载版本号
	 }
	 
	//server服的连接
	public static Channel gameServerChannel = null;
	
	//app后台服的连接
	//public static Channel appServerChannel = null;
	
	/**
	 * 通过serverId 保存 服的chennel
	 * @author 
	 * 2014-11-27
	 * @param channel
	 */
	public synchronized static void addGameToGameWorld(Channel channel){
		if (channel != null) {
			gameServerChannel = channel;
		}
	}
	
	
	
	
	
	
	
	
	
	/**
	 * 通过serverId 删除 服的chennel
	 * @author 
	 * 2014-11-27
	 */
	public synchronized static void removeGameFromGameWorld(){
		gameServerChannel = null;
		
	}
	
	
	
	
	

	/**
	 * 将user添加到世界
	 * @author 
	 * 2014-11-27
	 * @param channel
	 * @param electricUser
	 */
	
	
	

	
	
	public static String ToString(){
		final StringBuilder sb = new StringBuilder();
		
		sb.append("在线电桩数量=");
		//.append(epClinetMapKeyChannel.size());
		
		return sb.toString();
	}
	
	/**
	 * 通过channel获取user
	 * @author 
	 * 2014-11-27
	 * @param channel
	 * @return
	 */
	
	
    
	
	public static String getVersion() {
        return version;
    }

    public static void setVersion(String version) {
        EpCache.version = version;
    }
    
   
    
    
    
  
    private static String version;
    
    /**
	 * 通过EpCode
	 * @author lwz
	 * 2015-5-13
	 * @param epCode,obj
	 */
	public synchronized static void addEpExeVersion(String epCode,ExeCacheObject obj){
		if (epCode != null && obj!=null) {
			
			mapEpExeVersion.put(epCode, obj);
		}
	}
	public synchronized static ExeCacheObject getEpExeVersion(String epCode){
		
		return mapEpExeVersion.get(epCode);
		
	}
	public synchronized static void addEpExeBinary(String cachekey,char[] data){
		if (cachekey != null && data!=null) {
			
			mapEpExeBin.put(cachekey, data);
		}
	}
	public synchronized static char[] getEpExeBinary(String cachekey){
		return mapEpExeBin.get(cachekey);
	}
	
	public synchronized static void updateQRCode(String epCode,Integer epGunNo,String Content){
		
		EpGunCache epGunCache = EpGunService.getEpGunCache(epCode, epGunNo);//1.�жϻ�����ʱ����
		
		if(epGunCache == null)
			return;
			
		INetObject commClient = epGunCache.getEpNetObject();
		if(commClient==null || !commClient.isComm())
			return;
		
		
		
		//EpMessageHandler.handleQrcodeSumaryRet(commClient, epCode,epGunNo);//,Content);
		
		
	}
	
	
	public synchronized static void addFault(String epCode,EpFault fault)
	{
		if (epCode!=null && epCode.length() == EpProtocolConstant.LEN_CODE && fault!=null) {
			
			mapEpFault.put(epCode, fault);
		}
		
	}
	public synchronized static void removeFault(String epCode)
	{
		mapEpFault.remove(epCode);
	}
	
	
}
