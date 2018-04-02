package com.ward.push;

import com.ward.push.server.NettyAcceptor;
import org.springframework.context.support.ClassPathXmlApplicationContext;

public class Launcher {

	public static void main(String[] args) throws Exception {
		/*String[] paths = { "appContext.xml" };
		new ClassPathXmlApplicationContext(paths);
		Thread.sleep(Integer.MAX_VALUE);*/

		final NettyAcceptor server = new NettyAcceptor();
		server.startServer();
		System.out.println("Server started, version 0.9-SNAPSHOT");
		Runtime.getRuntime().addShutdownHook(new Thread() {
			@Override
			public void run() {
				server.stopServer();
			}
		});
	}
}
