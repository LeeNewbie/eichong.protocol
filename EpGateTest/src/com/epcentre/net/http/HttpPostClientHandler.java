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
package com.epcentre.net.http;

import io.netty.channel.ChannelHandlerContext;
import io.netty.channel.SimpleChannelInboundHandler;
import io.netty.handler.codec.http.HttpContent;
import io.netty.handler.codec.http.HttpObject;
import io.netty.handler.codec.http.HttpResponse;

import java.util.logging.Logger;

import com.epcentre.epconsumer.StopCarOrganService;

/**
 * Handler that just dumps the contents of the response from the server
 */
public class HttpPostClientHandler extends SimpleChannelInboundHandler<HttpObject> {

    private static final Logger logger = Logger.getLogger(HttpPostClientHandler.class.getName());
    
    private boolean readingChunks;
    private  HttpPostReq req;
    public HttpPostClientHandler(HttpPostReq req)
    {
    	this.req = req;
    }

    
    public void messageReceived(ChannelHandlerContext ctx, HttpObject msg) throws Exception {
    	if (msg instanceof HttpResponse) {
            HttpResponse response = (HttpResponse) msg; 
        }
        if (msg instanceof HttpContent) {
        	
            HttpContent content = (HttpContent) msg;
            
            StopCarOrganService.handHttpContent(content,req);
           // ThirdEpController.handHttpContent(content,req);
           
            
        }
    }

    @Override
    public void exceptionCaught(ChannelHandlerContext ctx, Throwable cause) throws Exception {
        cause.printStackTrace();
        ctx.channel().close();
    }

	@Override
	protected void channelRead0(ChannelHandlerContext arg0, HttpObject arg1)
			throws Exception {
		messageReceived(arg0,arg1);
		
	}
}
