package com.ward.push.server;

import io.netty.channel.Channel;

/**
 * 
 * 存储链接<br>
 * 
 * @author L.Y
 */
public class ConnectionDescriptor {
    public final String clientID;
    public final Channel channel;

    public ConnectionDescriptor(String clientID, Channel channel) {
        this.clientID = clientID;
        this.channel = channel;
    }

    @Override
    public String toString() {
        return "ConnectionDescriptor{" + "clientID=" + clientID + '}';
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) {
            return true;
        }
        if (o == null || getClass() != o.getClass()) {
            return false;
        }
        ConnectionDescriptor that = (ConnectionDescriptor) o;
        if (clientID != null ? !clientID.equals(that.clientID) : that.clientID != null) {
            return false;
        }
        return !(channel != null ? !channel.equals(that.channel) : that.channel != null);

    }

    @Override
    public int hashCode() {
        int result = clientID != null ? clientID.hashCode() : 0;
        result = 31 * result + (channel != null ? channel.hashCode() : 0);
        return result;
    }
}
