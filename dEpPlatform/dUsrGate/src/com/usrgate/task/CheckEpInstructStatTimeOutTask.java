package com.usrgate.task;

import com.usrgate.service.CacheService;

public class CheckEpInstructStatTimeOutTask implements Runnable {

	private int cycle;

	public CheckEpInstructStatTimeOutTask(int cycle){
		this.cycle = cycle;
	}

	@Override
	public void run() {
		CacheService.checkEpInstructStatTimeOut(cycle);
	}
}