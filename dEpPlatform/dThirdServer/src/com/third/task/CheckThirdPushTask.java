package com.third.task;

import com.third.service.EpGateService;



public class CheckThirdPushTask implements Runnable {

	@Override
	public void run() {
		EpGateService.checkThirdPushTimeout();
	}
}