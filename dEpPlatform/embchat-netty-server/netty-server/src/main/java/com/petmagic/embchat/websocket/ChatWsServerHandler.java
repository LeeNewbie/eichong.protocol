package com.petmagic.embchat.websocket;

import com.alibaba.fastjson.JSON;
import io.netty.buffer.ByteBuf;
import io.netty.buffer.Unpooled;
import io.netty.channel.ChannelFuture;
import io.netty.channel.ChannelFutureListener;
import io.netty.channel.ChannelHandlerContext;
import io.netty.channel.SimpleChannelInboundHandler;
import io.netty.handler.codec.http.*;
import io.netty.handler.codec.http.websocketx.*;
import io.netty.util.CharsetUtil;

import java.util.logging.Level;
import java.util.logging.Logger;

public class ChatWsServerHandler extends SimpleChannelInboundHandler<Object> {

    private static final Logger LOGGER = Logger.getLogger(ChatWsServerHandler.class.getName());
    private WebSocketServerHandshaker handshaker;

    /*protected void channelRead0(ChannelHandlerContext ctx, Object msg) throws Exception {
        //传统Http接入
        if(msg instanceof FullHttpRequest) {
            handleHttpRequest(ctx, (FullHttpRequest) msg);
        }
        //webSocket接入
        else if (msg instanceof WebSocketFrame) {
            handleWebSocketFrame(ctx, (WebSocketFrame) msg);
        }
    }*/
    //netty5的方法，netty4中用上面的channelRead0
    @Override
    protected void messageReceived(ChannelHandlerContext ctx, Object msg) throws Exception {
        //传统Http接入
        if(msg instanceof FullHttpRequest) {
            handleHttpRequest(ctx, (FullHttpRequest) msg);
        }
        //webSocket接入
        else if (msg instanceof WebSocketFrame) {
            handleWebSocketFrame(ctx, (WebSocketFrame) msg);
        }
    }

    /*@Override
    public void channelReadComplete(ChannelHandlerContext ctx) throws Exception {
        ctx.flush();
    }*/

    private void handleHttpRequest(ChannelHandlerContext ctx, FullHttpRequest request) throws Exception {
        //如果http解码失败，返回http异常
        if(!request.decoderResult().isSuccess()
                || ! "websocket".equals(request.headers().get("Upgrade"))) {
                sendHttpResponse(ctx, request, new DefaultFullHttpResponse(HttpVersion.HTTP_1_1, HttpResponseStatus.BAD_REQUEST));
            return;
        }

        //构造握手响应返回，本机测试
        WebSocketServerHandshakerFactory wsFactory = new WebSocketServerHandshakerFactory("ws://localhost:8083/websocket",null,false);
        handshaker = wsFactory.newHandshaker(request);
        if(handshaker == null) {  //不支持
            WebSocketServerHandshakerFactory.sendUnsupportedVersionResponse(ctx.channel());
        } else {
            // 构造握手响应消息返回给客户端
            // 动态添加编码和解码类到ChannelPipeline
            handshaker.handshake(ctx.channel(), request);

            //集中管理channel //每次登就会有一个channel
            ChatServerService.addChannel(ctx.channel());
//            ChatServerService.userMap.put("", ctx);
            System.out.println("已建立连接...");
        }
    }

    private void handleWebSocketFrame(ChannelHandlerContext ctx, WebSocketFrame frame) {

        // 判断是否是关闭链路指令
        if(frame instanceof CloseWebSocketFrame) {
            // 关闭socket连接
            handshaker.close(ctx.channel(), (CloseWebSocketFrame) frame.retain());
            ChatServerService.removeChannel(ctx.channel());
            //删除登录用户
            Message user = ChatServerService.removeOnlineUser(ctx.channel().id().toString());
            //关闭链路 下线
            user.setType(2);
            //通知 用户下线
            ChatServerService.writeMessage(new TextWebSocketFrame(JSON.toJSONString(user)));

            System.out.println(user.getName() + " 已下线...");
            return;
        }

        // 判断是否是ping消息
        if(frame instanceof PingWebSocketFrame) {
            //构造Pong消息返回
            ctx.channel().write(new PongWebSocketFrame(frame.content().retain()));
            return;
        }

        // 本例程仅支持文本消息，不支持二进制消息
        // 如果不是文本消息 抛出异常
        if(!(frame instanceof TextWebSocketFrame)) {
            throw new UnsupportedOperationException(String.format(
                    "%s frame types not supported", frame.getClass().getName()
            ));
        }

        //收到的消息
        String requestMsg = ((TextWebSocketFrame) frame).text();
        Message message = JSON.parseObject(requestMsg, Message.class);
        //记录日志
        if(LOGGER.isLoggable(Level.FINE)) {
            LOGGER.fine(String.format("%s received %s", ctx.channel(), requestMsg));
        }

        //返回应答消息
        if(message.getType() == 1) { //登录
            ctx.channel().writeAndFlush(new TextWebSocketFrame(ChatServerService.getOnlineUsersJson()));
            //增加在线用户
            ChatServerService.putOnlineUser(ctx.channel().id().toString(), message);
            System.out.println(message.getName() + " 已登录...");
        }
        ChatServerService.writeMessage(new TextWebSocketFrame(requestMsg));
        //ctx.channel().write(new TextWebSocketFrame(requestMsg));
    }

    private static void sendHttpResponse(ChannelHandlerContext ctx, FullHttpRequest request, FullHttpResponse response) {
        //返回应答给客户端
        if(response.status().code() != 200) {
            ByteBuf buf = Unpooled.copiedBuffer(response.status().toString(), CharsetUtil.UTF_8);
            response.content().writeBytes(buf);
            buf.release();
            response.headers().set(HttpHeaderNames.CONTENT_LENGTH, String.valueOf(response.content().readableBytes()));
            response.headers().set(HttpHeaderNames.CONNECTION, HttpHeaderValues.KEEP_ALIVE);
        }

        //如果是非Keep-Alive,关闭连接
        ChannelFuture f = ctx.channel().writeAndFlush(response);
        boolean isKeepAlive = HttpHeaderUtil.isKeepAlive(request);
        if(!isKeepAlive || response.status().code() != 200) {
            f.addListener(ChannelFutureListener.CLOSE);
        }
    }

    @Override
    public void channelActive(ChannelHandlerContext ctx) throws Exception {
        System.out.println("channel: " + ctx.channel().id() + " 活跃...");
        super.channelActive(ctx);
    }

    @Override
    public void channelInactive(ChannelHandlerContext ctx) throws Exception {
        System.out.println("channel: " + ctx.channel().id() + " 已闲置...");
        super.channelInactive(ctx);
    }

    @Override
    public void exceptionCaught(ChannelHandlerContext ctx, Throwable cause) throws Exception {
        cause.printStackTrace();
        ctx.close();
    }


}
