package com.reg.test;


import java.util.ArrayList;
import java.util.Collection;
import java.util.Collections;
import java.util.List;
import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.reg.protocol.WmIce104Util;

public class ImitateConsumeService {
	
	private static final Logger logger = LoggerFactory
			.getLogger(ImitateConsumeService.class);
	
	public static Map<String,String> userKeyMap = new ConcurrentHashMap<String, String>();

	
	public static String getConnetUserKey(String userName)
	{
		String userKey=null;
		userKey = userKeyMap.get(userName);
		
		return userKey;
	}
	public static void putConnetUserKey(String userName,String userKey)
	{
		userKeyMap.put(userName, userKey);
	}
	static public int checkSign(Map<String, List<String>> params)
	{
			int errorCode=0; 
			Object oUser = params.get("user");
			Object oSign = params.get("sign");
			if(oUser == null || oSign == null)
			{
				errorCode=1;
				return errorCode;
			}
			
			String src="";
			String sign="";
			String userName="";
			
			Collection<String> keyset= params.keySet();
			List<String> list = new ArrayList<String>(keyset);  
			   
			//对key键值按字典升序排序  
			Collections.sort(list);  
			            
		    for (int i = 0; i < list.size(); i++) {
		    	 String key = list.get(i);
		    	 if(key.compareTo("sign")==0)
		    	 {
		    		 sign = (String)params.get(key).get(0);
		    	 }
		    	 else
		    	 {
		    		 if(key.compareTo("user")==0)
		    		 {
		    			 userName= (String) params.get(key).get(0);
		    		 }
		    		 if(src.length()!=0)
		    			 src =src +"&";
		    		 src +=list.get(i)+"="+params.get(key).get(0);
		    	 } 
		     }
		    
		    String userKey = getConnetUserKey(userName);
		    if(userKey ==null)
		    {
		    	return 2;
		    }
		    src += userKey ;
		   
		    String calcSign = WmIce104Util.MD5Encode(src.getBytes());
		    if(calcSign.compareTo(sign)!=0)
		    {
		    	return 3;
		    }
		    	
		    
		    return 0;
	}
	
		
}
	
