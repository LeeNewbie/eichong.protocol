package com.third.task;

import com.third.service.EpGateService;



public class CheckEpGateCommClientTask  implements Runnable {

	@Override
	public void run() {
		EpGateService.checkCommTimeout();
	}
}