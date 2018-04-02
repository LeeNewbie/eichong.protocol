package com.petmagic.embchat.websocket;

import com.alibaba.fastjson.JSON;
import io.netty.channel.Channel;
import io.netty.channel.ChannelHandlerContext;
import io.netty.channel.group.ChannelGroup;
import io.netty.channel.group.DefaultChannelGroup;
import io.netty.handler.codec.http.websocketx.WebSocketFrame;
import io.netty.util.concurrent.GlobalEventExecutor;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

/**
 * 服务类
 */
public class ChatServerService {

    //DefaultChannelGroup 用于同一管理所有channel
    private static ChannelGroup channelGroup = new DefaultChannelGroup(GlobalEventExecutor.INSTANCE);

    //用于管理所有channelHandleContext 程序只有这一个channelHandler所以一个对应一个用户
    private static Map<String, ChannelHandlerContext> channelMap = new HashMap<String, ChannelHandlerContext>();

    //当前在线用户列表
    private static Map<String, Message> onlineUser = new HashMap<String, Message>();

    //public static Map<String,InformationPacket.Group.User> userList = new HashMap<String,InformationPacket.Group.User>();

    public static void addChannel(Channel channel) {
        channelGroup.add(channel);
    }

    public static void removeChannel(Channel channel) {
        channelGroup.remove(channel);
    }

    public static void writeMessage(WebSocketFrame frame) {
        channelGroup.writeAndFlush(frame);
    }

    public static void addChannelMap(String channelId, ChannelHandlerContext context) {
        channelMap.put(channelId, context);
    }

    public static void removeChannelMap(String channelId) {
        channelMap.remove(channelId);
    }

    public static void putOnlineUser(String channelId, Message message) {
        onlineUser.put(channelId, message);
    }

    public static Map<String, Message> getOnlineUser() {
        return onlineUser;
    }

    public static Message removeOnlineUser(String channelId) {
        return onlineUser.remove(channelId);
    }

    public static String getOnlineUsersJson() {
        return JSON.toJSON(onlineUser.values()).toString();
    }
}
