package com.usrgate.task;

import com.usrgate.service.CacheService;

public class CheckStatisticsTimeOutTask implements Runnable {

	private int cycle;

	public CheckStatisticsTimeOutTask(int cycle){
		this.cycle = cycle;
	}

	@Override
	public void run() {
		CacheService.checkStatisticsTimeOut(cycle);
	}
}