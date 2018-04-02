package com.ward.push.server;

import io.netty.channel.Channel;
import io.netty.channel.ChannelHandlerContext;
import io.netty.util.Attribute;
import io.netty.util.AttributeKey;

import com.ward.push.util.Global;

/**
 * 保存连接时的参数<br>
 * 
 * @author L.Y
 *
 */
public class NettyUtils {

	private static final AttributeKey<Object> ATTR_KEY_CLIENTID = AttributeKey.valueOf(Global.ATTR_CLIENTID);

	public static Object getAttribute(ChannelHandlerContext ctx, AttributeKey<Object> key) {
		Attribute<Object> attr = ctx.channel().attr(key);
		return attr.get();
	}

	public static void clientID(Channel channel, String clientID) {
		channel.attr(NettyUtils.ATTR_KEY_CLIENTID).set(clientID);
	}

	public static String clientID(Channel channel) {
		return (String) channel.attr(NettyUtils.ATTR_KEY_CLIENTID).get();
	}
}
