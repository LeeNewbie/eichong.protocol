package com.petmagic.embchat.websocket;

/**
 * 消息对象
 */
public class Message {

    private int type; // 0:普通消息   1:登录信息   2:关闭链路
    private String name;
    private String message;

    public int getType() {
        return type;
    }

    public void setType(int type) {
        this.type = type;
    }

    public String getMessage() {
        return message;
    }

    public void setMessage(String message) {
        this.message = message;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }
}