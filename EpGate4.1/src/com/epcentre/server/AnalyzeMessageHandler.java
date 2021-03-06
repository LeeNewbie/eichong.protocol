package com.epcentre.server;

import io.netty.channel.Channel;

import java.io.ByteArrayInputStream;
import java.io.IOException;
import java.io.InputStream;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.epcentre.cache.AnalyzeCommClient;
import com.epcentre.protocol.AnalyzeConstant;
import com.epcentre.protocol.StreamUtil;
import com.epcentre.service.AnalyzeService;
import com.epcentre.utils.DateUtil;



/**
 * 接受数据中心数据并处理
 */
public class AnalyzeMessageHandler{

	private static final Logger logger = LoggerFactory.getLogger(AnalyzeMessageHandler.class);

    /**
	 * 接受数据中心数据并处理
	 * @param channel
	 * @param message
	 * @throws IOException 
	 */
     public static void handleMessage(Channel channel, PhoneMessage message) {
		
    	 byte[] msg = message.getBytes();
    	
    	 
    	 try {
    	     processFrame(channel,msg);
    	 }catch (IOException e) {
    		 logger.error("handleMessage exception,e.StackTrace:{}",e.getStackTrace());
			}
	}

	public static void processFrame(Channel channel, byte[] msg)
			throws IOException {

        int msgLen = msg.length;
        
        InputStream in = new ByteArrayInputStream(msg);
        
        int cos = StreamUtil.read(in);
        int type = StreamUtil.readUB2(in);
	    
		boolean logMsg = true;

		switch (type & 0xffff) 
		{
		case AnalyzeConstant.D_HEART:// 1
        {
        	int revNum = StreamUtil.readUB2(in);
        	
        	
        	AnalyzeCommClient commClient = AnalyzeService.getCommClient();
        	if(commClient != null)
        	{
        	    commClient.setLastUseTime(DateUtil.getCurrentSeconds());
        	    commClient.setRevDataNum(revNum);
        	}
		}
		break;

		default:
		break;
		}

	}


	
}
