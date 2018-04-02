package com.webgate.net.client;

import io.netty.channel.Channel;

import java.io.ByteArrayInputStream;
import java.io.IOException;
import java.io.InputStream;

import com.webgate.constant.RegConstant;
import com.webgate.protocol.StreamUtil;

public class RegMessageHandler {/**
	 * 接受数据中心数据并处理
	 * @param channel
	 * @param message
	 * @throws IOException 
	 */
     public static void handleMessage(Channel channel, RegMessage message) {
		
    	 byte[] msg = message.getBytes();
    	
    	 
    	 try {
    	     processFrame(channel,msg);
    	 }catch (IOException e) {
				e.printStackTrace();
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
		case RegConstant.D_HEART:// 1
        {
        	int revNum = StreamUtil.readUB2(in);
        	
        	
        	/*RegCommClient commClient = RegCommClient.getCommClient();
        	if(commClient != null)
        	{
        	    commClient.setLastUseTime(DateUtil.getCurrentSeconds());
        	    commClient.setRevDataNum(revNum);
        	}*/
		}
		break;

		default:
		break;
		}

	}
}

