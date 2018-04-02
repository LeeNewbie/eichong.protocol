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
package com.reg.server;

import java.io.IOException;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.logging.Logger;



  
public class RegHttpServerHandler {
  
    private static final Logger logger = Logger.getLogger(RegHttpServerHandler.class.getName());
  
    public static String handleGetMessage( String method,Map<String, List<String>> params) throws  IOException
   	{
    	return "";
   	}
    public static String handlePostMessage(String method,HashMap<String,Object> params) throws  IOException
   	{
    	switch(method)
        {
               case "/getEpList":
            {
            	//ThirdEpController ctrl= new ThirdEpController();
            	//responseContent.append(FoxconnService.getEpList(paramsMap));
            }
            
            break;
        case "/register":
            {
            	//ThirdEpController ctrl= new ThirdEpController();
            	//responseContent.append(FoxconnService.register(paramsMap));
            }
        	break;
        case "/srvcontrol":
            {
            	//ThirdEpController ctrl= new ThirdEpController();
            	//responseContent.append(FoxconnService.epController(paramsMap));
            }
            break;
      
      
        case "/ep/srvcontrol":
            { 	
            	//GeelyEpController ctrl= new GeelyEpController();
            	//ctrl.epController(paramsMap,ctx,this);
	            //responseContent.append();
            }
            break;
        case "/ep/geteplists":
        { 	
        	// ThirdEpController ctrl= new ThirdEpController();
           // responseContent.append(ctrl.epController(paramsMap));
        }
        break;
        default:
         break;
        }
    	
    	return "";
   	}
   
   
   
   
  
    
  
    
   
  
    

	

}
 



