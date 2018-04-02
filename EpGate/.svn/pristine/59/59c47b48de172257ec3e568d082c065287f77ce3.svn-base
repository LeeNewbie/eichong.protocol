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

import static io.netty.buffer.Unpooled.copiedBuffer;
import static io.netty.handler.codec.http.HttpHeaders.Names.CONNECTION;
import static io.netty.handler.codec.http.HttpHeaders.Names.CONTENT_LENGTH;
import static io.netty.handler.codec.http.HttpHeaders.Names.CONTENT_TYPE;
import io.netty.buffer.ByteBuf;
import io.netty.channel.Channel;
import io.netty.channel.ChannelFuture;
import io.netty.channel.ChannelFutureListener;
import io.netty.channel.ChannelHandlerContext;
import io.netty.channel.SimpleChannelInboundHandler;
import io.netty.handler.codec.http.DefaultFullHttpResponse;
import io.netty.handler.codec.http.FullHttpResponse;
import io.netty.handler.codec.http.HttpContent;
import io.netty.handler.codec.http.HttpHeaders;
import io.netty.handler.codec.http.HttpMethod;
import io.netty.handler.codec.http.HttpObject;
import io.netty.handler.codec.http.HttpRequest;
import io.netty.handler.codec.http.HttpResponseStatus;
import io.netty.handler.codec.http.HttpVersion;
import io.netty.handler.codec.http.LastHttpContent;
import io.netty.handler.codec.http.QueryStringDecoder;
import io.netty.handler.codec.http.multipart.Attribute;
import io.netty.handler.codec.http.multipart.DefaultHttpDataFactory;
import io.netty.handler.codec.http.multipart.HttpDataFactory;
import io.netty.handler.codec.http.multipart.HttpPostRequestDecoder;
import io.netty.handler.codec.http.multipart.HttpPostRequestDecoder.EndOfDataDecoderException;
import io.netty.handler.codec.http.multipart.HttpPostRequestDecoder.ErrorDataDecoderException;
import io.netty.handler.codec.http.multipart.InterfaceHttpData;
import io.netty.handler.codec.http.multipart.InterfaceHttpData.HttpDataType;
import io.netty.util.CharsetUtil;

import java.io.IOException;
import java.util.HashMap;
import java.util.logging.Level;
import java.util.logging.Logger;



  
public class HttpServerHandler extends SimpleChannelInboundHandler<HttpObject> {
  
    private static final Logger logger = Logger.getLogger(HttpServerHandler.class.getName());
  
    private HttpRequest fullHttpRequest;
  
    private boolean readingChunks;
  
    private final StringBuilder responseContent = new StringBuilder();
  
    private static final HttpDataFactory factory = new DefaultHttpDataFactory(DefaultHttpDataFactory.MINSIZE); //Disk
  
    private HttpPostRequestDecoder decoder;
    private String postMethod="";
  
    @Override
    public void channelInactive(ChannelHandlerContext ctx) throws Exception {
        if (decoder != null) {
            decoder.cleanFiles();
        }
    }
    public HttpRequest getHttpReq()
    {
    	return fullHttpRequest;
    }
    public StringBuilder getRespSB()
    {
    	return responseContent;
    }
  
    public void messageReceived(ChannelHandlerContext ctx, HttpObject msg) throws Exception {
    	
    	if (msg instanceof HttpRequest) {
      
    	     fullHttpRequest = (HttpRequest) msg;
        /**
         * 服务器端返回信息
         */
            responseContent.setLength(0);
        
            System.out.print("GeelyHttpServerHandler:fullHttpRequest.getMethod():"+fullHttpRequest.getMethod()+"\r\n");
  
            if (fullHttpRequest.getMethod().equals(HttpMethod.GET)) {
               return;
            } 
            else if (fullHttpRequest.getMethod().equals(HttpMethod.POST)) {
               try {
                /**
                 * 通过HttpDataFactory和request构造解码器
                 */
                decoder = new HttpPostRequestDecoder(factory, fullHttpRequest);
               } catch (ErrorDataDecoderException e1) {
                e1.printStackTrace();
                responseContent.append(e1.getMessage());
                writeResponse(ctx.channel());
                ctx.channel().close();
                return;
               }
        	   QueryStringDecoder queryStringDecoder = new QueryStringDecoder(fullHttpRequest.getUri());
               String path = queryStringDecoder.path();
               System.out.print("path:"+path+"\n");
               switch(path)
               {
	               case "/getEpList":
	               {
	            	postMethod = "/getEpList";
	               }  	 
	               break;
                   case "/register":
	               {
	            	postMethod = "/register";
	               }
            	   break;
                   case "/srvcontrol":
	               {
	            	postMethod = "/srvcontrol";
	               }
	               break;
                   case "/ep/srvcontrol":
	               {
	            	   postMethod = "/ep/srvcontrol";
	               }
	               break;
                   case "/ep/geteplists":
	               {
	            	   postMethod = "/ep/geteplists";
	               }
	               break;
	               default:
	               break;
                 }
             }
    	  }
    
    	 if (decoder != null) {
            if (msg instanceof  HttpContent) {
                // New chunk is received
                HttpContent chunk = (HttpContent) msg;
                try {
                    decoder.offer(chunk);
                } catch (ErrorDataDecoderException e1) {
                    e1.printStackTrace();
                    responseContent.append(e1.getMessage());
                    writeResponse(ctx.channel());
                    ctx.channel().close();
                    return;
                }
              //得到参数列表
                HashMap<String,Object> paramsMap= getPostParams(decoder);
                switch(postMethod)
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
                if(postMethod.equals("/ep/srvcontrol"))
                {
                	readingChunks = false;
                    reset();
                    return;
                }
                // example of reading only if at the end
                if (chunk instanceof LastHttpContent ) {
                    writeResponse(ctx.channel());
                    readingChunks = false;
                    reset();
                }
            }
    	 }
 
    }
  
    private void reset() {
        //fullHttpRequest = null;
        // destroy the decoder to release all resources
        decoder.destroy();
        decoder = null;
    }
    
    private HashMap<String,Object> getPostParams(HttpPostRequestDecoder decoder)
    {
    	HashMap<String,Object>  paramsMap=new HashMap<String,Object>();
        try {
        	
            while (decoder.hasNext()) {
                InterfaceHttpData data = decoder.next();
                if (data != null) {
                    try {
                    	if (data.getHttpDataType() == HttpDataType.Attribute) 
                    	{
                    		Attribute attribute = (Attribute) data;
                            String value;
                            try {
                                value = attribute.getValue();
                                paramsMap.put(attribute.getName(), value);
                            } catch (IOException e1) {
                                e1.printStackTrace();
                            }
                    		
                    	}
                        
                    } finally {
                        data.release();
                    }
                }
            }
            
        } catch (EndOfDataDecoderException e1) {
            //responseContent.append("\r\n\r\nEND OF CONTENT CHUNK BY CHUNK\r\n\r\n");
        }
        return paramsMap;
    	
    }
  
    private void writeHttpData(InterfaceHttpData data) {
  
        /**
         * HttpDataType有三种类型
         * Attribute, FileUpload, InternalAttribute
         */
        if (data.getHttpDataType() == HttpDataType.Attribute) {
            Attribute attribute = (Attribute) data;
            String value;
            try {
                value = attribute.getValue();
            } catch (IOException e1) {
                e1.printStackTrace();
                responseContent.append("\r\nBODY Attribute: " + attribute.getHttpDataType().name() + ":"
                        + attribute.getName() + " Error while reading value: " + e1.getMessage() + "\r\n");
                return;
            }
            if (value.length() > 100) {
                responseContent.append("\r\nBODY Attribute: " + attribute.getHttpDataType().name() + ":"
                        + attribute.getName() + " data too long\r\n");
            } else {
                responseContent.append("\r\nBODY Attribute: " + attribute.getHttpDataType().name() + ":"
                        + attribute.toString() + "\r\n");
            }
        }
    }
  
   
    
    public void writeResponse(Channel channel) {
        // Convert the response content to a ChannelBuffer.
        ByteBuf buf = copiedBuffer(responseContent.toString(), CharsetUtil.UTF_8);
        responseContent.setLength(0);
  
        // Decide whether to close the connection or not.
        boolean close = fullHttpRequest.headers().contains(CONNECTION, HttpHeaders.Values.CLOSE, true)
                || fullHttpRequest.getProtocolVersion().equals(HttpVersion.HTTP_1_0)
                && !fullHttpRequest.headers().contains(CONNECTION, HttpHeaders.Values.KEEP_ALIVE, true);
  
        // Build the response object.
        FullHttpResponse response = new DefaultFullHttpResponse(HttpVersion.HTTP_1_1, HttpResponseStatus.OK, buf);
        response.headers().set(CONTENT_TYPE, "text/plain; charset=UTF-8");
  
        if (!close) {
            // There's no need to add 'Content-Length' header
            // if this is the last response.
            response.headers().set(CONTENT_LENGTH, buf.readableBytes());
        }
 
        // Write the response.
        ChannelFuture future = channel.writeAndFlush(response);
        // Close the connection after the write operation is done if necessary.
        if (close) {
            future.addListener(ChannelFutureListener.CLOSE);
        }
    }
  
    @Override
    public void exceptionCaught(ChannelHandlerContext ctx, Throwable cause) throws Exception {
        logger.log(Level.WARNING, responseContent.toString(), cause);
        ctx.channel().close();
    }
  
    

	@Override
	public boolean acceptInboundMessage(Object msg) throws Exception {
		return super.acceptInboundMessage(msg);
	}

	@Override
	public void channelRead(ChannelHandlerContext ctx, Object msg)
			throws Exception {
		//super.channelRead(ctx, msg);
		messageReceived(ctx,(HttpObject) msg);
	}
	@Override
	protected void channelRead0(ChannelHandlerContext arg0, HttpObject arg1)
			throws Exception {
		messageReceived(arg0,arg1);
		
	}
}
 



