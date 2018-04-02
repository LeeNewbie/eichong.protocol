package com.webgate.task;

import com.webgate.service.CacheService;

public class CheckEpGateNetTimeOutTask implements Runnable {

	@Override
	public void run() {
		CacheService.checkEpGateTimeOut();
	}
}