/*
 * Copyright (C), 2002-2016, 苏宁易购电子商务有限公司
 * FileName: PingRespMessage.java
 * Author:   Administrator
 * Date:     2016-10-1 上午10:59:32
 * Description: //模块目的、功能描述      
 * History: //修改记录
 * <author>      <time>      <version>    <desc>
 * 修改人姓名             修改时间            版本号                  描述
 */
package com.ward.push.protocol.messages.resp;

import com.ward.push.protocol.messages.AbstractMessage;

/**
 * 心跳检测响应消息<br>
 * 
 * @author L.Y
 */
public class PingRespMessage extends AbstractMessage {
    public PingRespMessage() {
        m_messageType = PINGRESP;
        protocolVersion = VERSION;
    }
}
