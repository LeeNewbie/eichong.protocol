package com.epcentre.net.http;

import io.netty.bootstrap.Bootstrap;
import io.netty.channel.Channel;
import io.netty.channel.EventLoopGroup;
import io.netty.channel.nio.NioEventLoopGroup;
import io.netty.channel.socket.nio.NioSocketChannel;
import io.netty.handler.codec.http.DefaultFullHttpRequest;
import io.netty.handler.codec.http.DefaultHttpRequest;
import io.netty.handler.codec.http.HttpMethod;
import io.netty.handler.codec.http.HttpRequest;
import io.netty.handler.codec.http.HttpVersion;
import io.netty.handler.codec.http.multipart.DefaultHttpDataFactory;
import io.netty.handler.codec.http.multipart.HttpDataFactory;
import io.netty.handler.codec.http.multipart.HttpPostRequestEncoder;
import io.netty.handler.codec.http.multipart.HttpPostRequestEncoder.ErrorDataEncoderException;
import io.netty.handler.codec.http.multipart.InterfaceHttpData;

import java.net.URI;
import java.net.URISyntaxException;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;



public class HttpPostClient {
	
	
	private  HttpPostReq req;
    public HttpPostClient(HttpPostReq req) {
        this.req = req;
    }

	public void run() throws Exception {
		URI uri= new URI(req.getBaseUri());
		String scheme = uri.getScheme() == null? "http" : uri.getScheme();
	    String host = uri.getHost() == null? "localhost" : uri.getHost();
        int port = uri.getPort();
        if (port == -1) {
            if ("http".equalsIgnoreCase(scheme)) {
                port = 80;
            } else if ("https".equalsIgnoreCase(scheme)) {
                port = 443;
            }
        }

        if (!"http".equalsIgnoreCase(scheme) && !"https".equalsIgnoreCase(scheme)) {
            System.err.println("Only HTTP(S) is supported.");
            return;
        }

        boolean ssl = "https".equalsIgnoreCase(scheme);
	 
	    // Configure the client.
	    EventLoopGroup group = new NioEventLoopGroup();

	        // setup the factory: here using a mixed memory/disk based on size threshold
	    HttpDataFactory factory = new DefaultHttpDataFactory(DefaultHttpDataFactory.MINSIZE); // Disk if MINSIZE exceed

	       
	        try {
	            Bootstrap b = new Bootstrap();
	            b.group(group).channel(NioSocketChannel.class).handler(new HttpPostClientIntializer(this.req,ssl));
	            //b.group(group).channel(NioSocketChannel.class).handler(new HttpPostClientIntializer());
	            // Simple Post form: factory used for big attributes
	            List<InterfaceHttpData> bodylist = Post(b, host, port, uri,  factory, this.req.getParams());
	            if (bodylist == null) {
	                factory.cleanAllHttpDatas();
	                return;
	            }

	           } finally {
	            // Shut down executor threads to exit.
	            //group.shutdownGracefully();

	            // Really clean all temporary files if they still exist
	            //factory.cleanAllHttpDatas();
	        }
	    }

	    
	   
	    private static List<InterfaceHttpData> Post(Bootstrap bootstrap, String host, int port, URI uriSimple,
	             HttpDataFactory factory, HashMap<String,Object> params) throws Exception {

	        // Start the connection attempt
	        Channel channel = bootstrap.connect(host, port).sync().channel();

	        // Prepare the HTTP request.
	        HttpRequest request =
	                new DefaultHttpRequest(HttpVersion.HTTP_1_1, HttpMethod.POST, uriSimple.toASCIIString());

	        // Use the PostBody encoder
	        HttpPostRequestEncoder bodyRequestEncoder = null;
	        try {
	            bodyRequestEncoder = new HttpPostRequestEncoder(factory, request, false); // false not multipart
	        } catch (NullPointerException e) {
	            // should not be since args are not null
	            e.printStackTrace();
	        } catch (ErrorDataEncoderException e) {
	            // test if getMethod is a POST getMethod
	            e.printStackTrace();
	        }
	       
	        // add Form attribute
	        try {
	        	System.out.print("params:"+params+"");
	        	Iterator iter = params.entrySet().iterator();
	    		while (iter.hasNext()) {
	    			Map.Entry entry = (Map.Entry) iter.next();
	    			bodyRequestEncoder.addBodyAttribute(entry.getKey().toString(), entry.getValue().toString());	
	    		}
	           
	        } catch (NullPointerException e) {
	            // should not be since not null args
	            e.printStackTrace();
	        } catch (ErrorDataEncoderException e) {
	            // if an encoding error occurs
	            e.printStackTrace();
	        }

	        // finalize request
	        try {
	            request = bodyRequestEncoder.finalizeRequest();
	        } catch (ErrorDataEncoderException e) {
	            // if an encoding error occurs
	            e.printStackTrace();
	        }

	        // Create the bodylist to be reused on the last version with Multipart support
	        List<InterfaceHttpData> bodylist = bodyRequestEncoder.getBodyListAttributes();

	        // send request
	        channel.write(request);
	        
	        channel.writeAndFlush(bodyRequestEncoder);
	        //绑定
	        // Wait for the server to close the connection.
	        channel.closeFuture().sync();

	        return bodylist;
	    }

	    /**
	     * Multipart example
	     */
	    private static void formPostMultipart(Bootstrap bootstrap, String host, int port, URI uriFile,
	            HttpDataFactory factory, List<Entry<String, String>> headers, List<InterfaceHttpData> bodylist)
	            throws Exception {

	        // Start the connection attempt
	        Channel channel = bootstrap.connect(host, port).sync().channel();

	        // Prepare the HTTP request.
	        HttpRequest request =
	                new DefaultFullHttpRequest(HttpVersion.HTTP_1_1, HttpMethod.POST, uriFile.toASCIIString());

	        // Use the PostBody encoder
	        HttpPostRequestEncoder bodyRequestEncoder = null;
	        try {
	            bodyRequestEncoder = new HttpPostRequestEncoder(factory, request, true); // true => multipart
	        } catch (NullPointerException e) {
	            // should not be since no null args
	            e.printStackTrace();
	        } catch (ErrorDataEncoderException e) {
	            // test if getMethod is a POST getMethod
	            e.printStackTrace();
	        }

	        // it is legal to add directly header or cookie into the request until finalize
	        for (Entry<String, String> entry : headers) {
	            request.headers().set(entry.getKey(), entry.getValue());
	        }

	        // add Form attribute from previous request in formpost()
	        try {
	            bodyRequestEncoder.setBodyHttpDatas(bodylist);
	        } catch (NullPointerException e1) {
	            // should not be since previously created
	            e1.printStackTrace();
	        } catch (ErrorDataEncoderException e1) {
	            // again should not be since previously encoded (except if an error
	            // occurs previously)
	            e1.printStackTrace();
	        }

	        // finalize request
	        try {
	            request = bodyRequestEncoder.finalizeRequest();
	        } catch (ErrorDataEncoderException e) {
	            // if an encoding error occurs
	            e.printStackTrace();
	        }

	        // send request
	        channel.write(request);

	        // test if request was chunked and if so, finish the write
	        if (bodyRequestEncoder.isChunked()) {
	            channel.writeAndFlush(bodyRequestEncoder).awaitUninterruptibly();
	        } else {
	            channel.flush();
	        }

	        // Now no more use of file representation (and list of HttpData)
	        bodyRequestEncoder.cleanFiles();

	        // Wait for the server to close the connection.
	        channel.closeFuture().sync();
	    }


}
