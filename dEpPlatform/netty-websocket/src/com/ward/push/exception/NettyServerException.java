package com.ward.push.exception;

/**
 * 
 * NettyServer Exception<br>
 * 
 * @author L.Y
 */
public class NettyServerException extends RuntimeException {

    private static final long serialVersionUID = 4922299169829689148L;

    public NettyServerException() {
        super();
    }

    public NettyServerException(String message) {
        super(message);
    }

    public NettyServerException(String message, Throwable cause) {
        super(message, cause);
    }

    public NettyServerException(Throwable cause) {
        super(cause);
    }
}
