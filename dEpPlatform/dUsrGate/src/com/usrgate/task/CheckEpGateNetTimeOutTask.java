package com.usrgate.task;

import com.usrgate.service.CacheService;

public class CheckEpGateNetTimeOutTask implements Runnable {

	@Override
	public void run() {
		CacheService.checkEpGateTimeOut();
	}
}