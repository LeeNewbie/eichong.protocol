package com.petmagic.embchat.websocket;

import io.netty.channel.ChannelInitializer;
import io.netty.channel.ChannelPipeline;
import io.netty.channel.socket.SocketChannel;
import io.netty.handler.codec.http.HttpObjectAggregator;
import io.netty.handler.codec.http.HttpServerCodec;
import io.netty.handler.stream.ChunkedWriteHandler;


/**
 * Created by c on 15/7/19.
 */
public class ChatServerChannelInitializer extends ChannelInitializer<SocketChannel> {

    @Override
    protected void initChannel(SocketChannel ch) throws Exception {
        ChannelPipeline pipeline = ch.pipeline();
        //将请求和响应的消息编码或解码为http消息
        pipeline.addLast("httpCodec", new HttpServerCodec());
        //将http消息的多个部分组合成一条完整的http消息
        pipeline.addLast("aggregator", new HttpObjectAggregator(65536));
        //向客户端发送html5文件
        pipeline.addLast("httpChunked", new ChunkedWriteHandler());
        pipeline.addLast("handler", new ChatWsServerHandler());
    }
}
