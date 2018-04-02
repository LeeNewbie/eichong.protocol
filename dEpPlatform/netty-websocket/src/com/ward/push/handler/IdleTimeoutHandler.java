package com.ward.push.handler;

import io.netty.channel.ChannelDuplexHandler;
import io.netty.channel.ChannelHandler.Sharable;
import io.netty.channel.ChannelHandlerContext;
import io.netty.handler.timeout.IdleState;
import io.netty.handler.timeout.IdleStateEvent;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.ward.push.server.NettyUtils;

/**
 * 
 * 空闲检测时间<br>
 * 
 * @author L.Y
 */
@Sharable
public class IdleTimeoutHandler extends ChannelDuplexHandler {
	private static final Logger logger = LoggerFactory.getLogger(IdleTimeoutHandler.class);

	@Override
	public void userEventTriggered(ChannelHandlerContext ctx, Object evt) throws Exception {
		if (evt instanceof IdleStateEvent) {
			String clientId = NettyUtils.clientID(ctx.channel());
			logger.info("检测到当前clientId:{}处于空间状态", new Object[] { clientId });
			IdleState e = ((IdleStateEvent) evt).state();
			if (e == IdleState.ALL_IDLE) {
				ctx.fireChannelInactive();
				ctx.close();
			}
		} else {
			super.userEventTriggered(ctx, evt);
		}
	}
}
