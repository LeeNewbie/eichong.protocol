package com.usrlayer.task;

import com.usrlayer.service.EpGateService;



public class CheckThirdPushTask implements Runnable {

	@Override
	public void run() {
		EpGateService.checkThirdPushTimeout();
	}
}