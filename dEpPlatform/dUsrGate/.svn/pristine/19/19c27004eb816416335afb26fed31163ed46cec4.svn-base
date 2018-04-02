package com.usrgate.task;

import com.usrgate.service.CacheService;

public class CheckInstructStatTimeOutTask implements Runnable {

	private int cycle;

	public CheckInstructStatTimeOutTask(int cycle){
		this.cycle = cycle;
	}

	@Override
	public void run() {
		CacheService.checkInstructStatTimeOut(cycle);
	}
}