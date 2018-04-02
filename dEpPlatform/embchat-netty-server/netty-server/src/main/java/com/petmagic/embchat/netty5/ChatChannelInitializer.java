package com.petmagic.embchat.netty5;

import com.petmagic.embchat.netty5.bean.InformationPacket;
import com.petmagic.embchat.netty5.message.AgreementMsg;
import io.netty.channel.ChannelInitializer;
import io.netty.channel.socket.SocketChannel;
import io.netty.handler.codec.json.JsonObjectDecoder;
import io.netty.handler.codec.protobuf.ProtobufDecoder;
import io.netty.handler.codec.protobuf.ProtobufEncoder;
import io.netty.handler.codec.protobuf.ProtobufVarint32FrameDecoder;
import io.netty.handler.codec.protobuf.ProtobufVarint32LengthFieldPrepender;

/**
 * 设置handler
 */
public class ChatChannelInitializer extends ChannelInitializer<SocketChannel> {
    @Override
    protected void initChannel(SocketChannel channel) throws Exception {

        //添加解码器
        channel.pipeline().addLast("frameDecoder", new ProtobufVarint32FrameDecoder());
        channel.pipeline().addLast("protobufDecoder", new ProtobufDecoder(InformationPacket.Group.getDefaultInstance()));

        //添加编码器
        channel.pipeline().addLast("frameEncoder", new ProtobufVarint32LengthFieldPrepender());
        channel.pipeline().addLast("protobufEncoder", new ProtobufEncoder());
        //添加handler

        channel.pipeline().addLast(new ChatServerChannelHandler());

        //发送连接成功包给客户端
        AgreementMsg msg = new AgreementMsg(true);
        channel.writeAndFlush(msg.doGetConnectServerInfoPacket(InformationPacket.Group.ServerConnectEnum.Success));
        System.out.println(" send connection success msg to client...");
    }
}
