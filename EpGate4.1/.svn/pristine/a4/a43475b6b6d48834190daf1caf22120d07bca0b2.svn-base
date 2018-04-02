package com.epcentre.utils;

import io.netty.channel.Channel;

import java.net.SocketAddress;
import java.util.concurrent.TimeUnit;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.epcentre.service.AppApiService;

public class NetUtils {
	private static final Logger logger = LoggerFactory.getLogger(NetUtils.class);
	 public static String getClientIp(Channel channel){
		 SocketAddress address = channel.remoteAddress();
		 String ip = "";
		 if (address != null) {
			 ip = address.toString().trim();
			 int index = ip.lastIndexOf(':');
			 if (index < 1) {
				 index = ip.length();
			 }
			 ip = ip.substring(1, index);
		 }
		 if (ip.length() > 15) {
			 ip = ip.substring(Math.max(ip.indexOf("/") + 1, ip.length() - 15));
		 }
		 return ip;
	 };
	 /**
	  * 
	  * @param time  毫秒
	  */
	 public static void sleep(long time)
	 {
		 try{
			 TimeUnit.MILLISECONDS.sleep(10);
		 }catch (Exception e) {
			 logger.error("sleep exception,e.getMessage():{}",e.getMessage());
		 }
	 }
}
