package com.petmagic.embchat.websocket;

import io.netty.bootstrap.ServerBootstrap;
import io.netty.channel.ChannelFuture;
import io.netty.channel.ChannelOption;
import io.netty.channel.EventLoopGroup;
import io.netty.channel.nio.NioEventLoopGroup;
import io.netty.channel.socket.nio.NioServerSocketChannel;

/**
 * 服务器
 */
public class ChatServer {

    private final int port;

    public ChatServer(final int port) {
        this.port = port;
    }

    public void start() throws Exception {
        EventLoopGroup bossGroup = new NioEventLoopGroup();
        EventLoopGroup workerGroup = new NioEventLoopGroup();
        try {
            ServerBootstrap b = new ServerBootstrap();
            b.group(bossGroup,workerGroup)
                    .channel(NioServerSocketChannel.class);
            // 最大连接?
            // ChannelOption.SO_BACKLOG 英文解释
            // It's a passed through socket option determining the number of connections queued.
            // The maximum queue length for incoming connection indications
            // (a request to connect) is set to the backlog parameter.
            // If a connection indication arrives when the queue is full,
            // the connection is refused.
            b.option(ChannelOption.SO_BACKLOG, 1024)
                    .localAddress(port)
                    .childHandler(new ChatServerChannelInitializer());

            ChannelFuture future = b.bind().sync();
            System.out.println(ChatServer.class.getName() + " 已启动 正在监听... " + future.channel().localAddress());
            System.out.println("打开浏览器 访问:http://localhost:" + port + "/");
            future.channel().closeFuture().sync();
        } finally {
            bossGroup.shutdownGracefully();
            workerGroup.shutdownGracefully();
            System.out.println(ChatServer.class.getName() + " 关闭 停止服务...");
        }
    }

    public static void main(String[] args) throws Exception {

        new ChatServer(8083).start();

        // jvm关闭钩子
        // 在jvm关闭的时候会执行所有通过addShutdownHook添加的钩子
        // 当所有的钩子执行完成 jvm才会关闭
        // 所以这个方法可以再jvm关闭的时候进行内存清理、对象销毁等操作
        /*Runtime.getRuntime().addShutdownHook(new Thread() {

            @Override
            public void run() {
                server.destory();
            }
        });*/
    }
}
