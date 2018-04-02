package com.ward.push.protocol;

import io.netty.buffer.ByteBuf;

import com.ward.push.protocol.messages.AbstractMessage;

public class Utils {

    static byte readMessageType(ByteBuf in) {
        byte h1 = in.readByte();
        byte messageType = (byte) ((h1 & 0xF0) >> 4);
        return messageType;
    }

    public static String msgType2String(int type) {
        switch (type) {
            case AbstractMessage.CONNECT:
                return "CONNECT";
            case AbstractMessage.CONNACK:
                return "CONNACK";
            case AbstractMessage.PUBLISH:
                return "PUBLISH";
            case AbstractMessage.PUBACK:
                return "PUBACK";
            case AbstractMessage.SUBSCRIBE:
                return "SUBSCRIBE";
            case AbstractMessage.SUBACK:
                return "SUBACK";
            case AbstractMessage.UNSUBSCRIBE:
                return "UNSUBSCRIBE";
            case AbstractMessage.UNSUBACK:
                return "UNSUBACK";
            case AbstractMessage.PINGREQ:
                return "PINGREQ";
            case AbstractMessage.PINGRESP:
                return "PINGRESP";
            case AbstractMessage.DISCONNECT:
                return "DISCONNECT";
            default:
                throw new RuntimeException("Can't decode message type " + type);
        }
    }
    
    public static void main(String[] args) {
        System.out.println((12 & 0x0F) << 4);
        System.out.println((192 & 0xF0) >> 4);
    }
}
