package com.petmagic.embchat.netty5;

import io.netty.bootstrap.ServerBootstrap;
import io.netty.channel.ChannelFuture;
import io.netty.channel.ChannelOption;
import io.netty.channel.EventLoopGroup;
import io.netty.channel.nio.NioEventLoopGroup;
import io.netty.channel.socket.nio.NioServerSocketChannel;

/**
 * EmbChat 服务端
 */
public class ChatServer {

    private final int port;

    public ChatServer(int port) {
        this.port = port;
    }

    public void start() throws Exception {
        EventLoopGroup bossGroup = new NioEventLoopGroup();   //处理连接请求
        EventLoopGroup workerGroup = new NioEventLoopGroup(); //处理连接后的具体事情

        try {
            ServerBootstrap b = new ServerBootstrap();

            b.group(bossGroup, workerGroup);
            b.channel(NioServerSocketChannel.class);
            //最大连接?
            // ChannelOption.SO_BACKLOG 英文解释
            // It's a passed through socket option determining the number of connections queued.
            // The maximum queue length for incoming connection indications
            // (a request to connect) is set to the backlog parameter.
            // If a connection indication arrives when the queue is full,
            // the connection is refused.
            b.option(ChannelOption.SO_BACKLOG, 1024);
            //设置地址及端口
            b.localAddress(port);
            b.childHandler(new ChatChannelInitializer());

            //绑定端口 接收连接
            ChannelFuture future = b.bind().sync();
            System.out.println(ChatServer.class.getName() + " 已启动 正在监听... " + future.channel().localAddress());
            //等待服务器socket关闭
            future.channel().closeFuture().sync();
        } finally {
            bossGroup.shutdownGracefully();
            workerGroup.shutdownGracefully();
            System.out.println(ChatServer.class.getName() + " 关闭 停止服务...");
        }

    }

    public static void main(String[] args) throws Exception {
        new ChatServer(65535).start();
    }
}
