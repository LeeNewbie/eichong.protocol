package com.petmagic.embchat.netty5;

import com.petmagic.embchat.netty5.bean.InformationPacket;
import io.netty.channel.ChannelHandlerContext;
import io.netty.channel.group.ChannelGroup;
import io.netty.channel.group.DefaultChannelGroup;
import io.netty.util.concurrent.GlobalEventExecutor;

import java.util.HashMap;
import java.util.Map;

/**
 * Created by c on 15/7/18.
 */
public class MsgHandleService {

    //DefaultChannelGroup 用于管理所有channel
    public static ChannelGroup channelGroup = new DefaultChannelGroup(GlobalEventExecutor.INSTANCE);

    //用于管理所有channelHandleContext 程序只有这一个channelHandler所以一个对应一个用户
    public static Map<String, ChannelHandlerContext> userMap = new HashMap<String, ChannelHandlerContext>();

    public static Map<String,InformationPacket.Group.User> userList = new HashMap<String,InformationPacket.Group.User>();
}
