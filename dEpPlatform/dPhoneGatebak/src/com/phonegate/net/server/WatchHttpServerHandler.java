/*
 * Copyright 2012 The Netty Project
 *
 * The Netty Project licenses this file to you under the Apache License,
 * version 2.0 (the "License"); you may not use this file except in compliance
 * with the License. You may obtain a copy of the License at:
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations
 * under the License.
 */
package com.phonegate.net.server;



import java.io.IOException;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.phonegate.test.ImitateConsumeService;

public class WatchHttpServerHandler
{
	private static final Logger logger = LoggerFactory.getLogger(WatchHttpServerHandler.class);
	


    /** Buffer that stores the response content */
   
    public static String handleGetMessage( String method,Map<String, List<String>> params) throws  IOException
	{
    	StringBuilder buf = new StringBuilder();

    	switch(method)
        {
        case "/testCallEp":
        {
        	String retDesc=ImitateConsumeService.testCallEp(params);
        	if(retDesc!=null)
        		buf.append(retDesc);
        }
        	break;
       
        case "/getRate":
        {  
        	String retDesc=ImitateConsumeService.getRatebyId(params);
        	if(retDesc!=null)
        		buf.append(retDesc);
        }
        break;
        case "/testStartBespoke":
        {
        	String retDesc=ImitateConsumeService.testStartBespoke(params);
        	if(retDesc!=null)
        		buf.append(retDesc);

        }
        break;  
        case "/testStartBespoke2":
        {
        	String retDesc=ImitateConsumeService.testStartBespoke2(params);
        	if(retDesc!=null)
        		buf.append(retDesc);

        }
        break;
        
        case "/testStopBespoke":
        {
        	
        	String retDesc=ImitateConsumeService.testStopBespoke(params);
        	if(retDesc!=null)
        		buf.append(retDesc);
        }
        break;
        case "/testStopBespoke2":
        {
        	
        	String retDesc=ImitateConsumeService.testStopBespoke2(params);
        	if(retDesc!=null)
        		buf.append(retDesc);
        }
        break;
        case "/testStartCharge":
        {
        	String retDesc=ImitateConsumeService.testStartCharge(params);
        	if(retDesc!=null)
        		buf.append(retDesc);
        }
        break; 	
        case "/testStartCharge2":
        {
        	String retDesc=ImitateConsumeService.testStartCharge2(params);
        	if(retDesc!=null)
        		buf.append(retDesc);
        }
        break;
        case "/testStopCharge":
        {
        	
        	String stopDesc=ImitateConsumeService.testStopCharge(params);
        	if(stopDesc!=null)
        		buf.append(stopDesc);
        	
        }
        break;
        case "/testStopCharge2":
        {
        	
        	String stopDesc=ImitateConsumeService.testStopCharge2(params);
        	if(stopDesc!=null)
        		buf.append(stopDesc);
        	
        }
        break;
        case "/user":
        {
        	
        	String stopDesc=ImitateConsumeService.findUser(params);
        	if(stopDesc!=null)
        		buf.append(stopDesc);
        	
        }
        break;
        case "/stat":
        {
        	String stopDesc=ImitateConsumeService.stat(params);
        	if(stopDesc!=null)
        		buf.append(stopDesc);
       
        }
        break;
       
       
       
       
        case "/gun_restore":
        {
        	String stopDesc=ImitateConsumeService.gun_restore(params);
        	if(stopDesc!=null)
        		buf.append(stopDesc);
        }
        break;
        case "/getChNum":
        {
        	String stopDesc=ImitateConsumeService.getChNum(params);
        	if(stopDesc!=null)
        		buf.append(stopDesc);
        }
        	
        break;
        case "/getepdetail":
        {
        	String stopDesc=ImitateConsumeService.getEpDetail(params);
        	if(stopDesc!=null)
        		buf.append(stopDesc);
        }
        	
        break;
       
        case "/getReal":
        {
        	String stopDesc=ImitateConsumeService.getRealData(params);
        	if(stopDesc!=null)
        		buf.append(stopDesc);
        }
        	
        break;
        case "/getLastConsumeRecord":
        {
        	String stopDesc=ImitateConsumeService.getLastConsumeRecord(params);
        	if(stopDesc!=null)
        		buf.append(stopDesc);
        }
        	
        break;
       
        
        
        case "/getgundetail":
        {
        	String stopDesc=ImitateConsumeService.getgundetail(params);
        	if(stopDesc!=null)
        		buf.append(stopDesc);
        }
        	
        break;
        case "/removeCharge":
        {
        	String stopDesc=ImitateConsumeService.removeCharge(params);
        	if(stopDesc!=null)
        		buf.append(stopDesc);
        }
        break;
        case "/removebesp":
        {
        	String stopDesc=ImitateConsumeService.removeBespoke(params);
        	if(stopDesc!=null)
        		buf.append(stopDesc);
        }
        	break;
        case "/cleanuser":
        {
        	String stopDesc=ImitateConsumeService.cleanUser(params);
        	if(stopDesc!=null)
        		buf.append(stopDesc);
        }
        	break;
        
        
       
        
        default:
        	break;
        
        };
        
        return buf.toString();
	}
    
    public static String handlePostMessage(String method,HashMap<String,Object> params) throws  IOException
   	{
    	return "";
   	}
}
