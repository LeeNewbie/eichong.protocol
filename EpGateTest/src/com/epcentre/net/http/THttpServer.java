package com.epcentre.net.http;

public class THttpServer  implements Runnable {
	private int port = 0;
	
	public int getPort() {
		return port;
	}

	public void setPort(int port) {
		this.port = port;
	}
	
	public void run() {
		try {
			
			HttpServer outServer= new HttpServer(getPort());
			outServer.run();
			
			System.out.print("启动吉利 http成功【"+getPort()+"】\n");
			
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

}

