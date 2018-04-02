package com.third.task;

import com.third.server.SCOService;




public class TestTask  implements Runnable {

	@Override
	public void run() {
		SCOService.check();
	}
}