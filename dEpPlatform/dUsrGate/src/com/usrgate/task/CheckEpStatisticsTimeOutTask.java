package com.usrgate.task;

import com.usrgate.service.CacheService;

public class CheckEpStatisticsTimeOutTask implements Runnable {

	private int cycle;

	public CheckEpStatisticsTimeOutTask(int cycle){
		this.cycle = cycle;
	}

	@Override
	public void run() {
		CacheService.checkEpStatisticsTimeOut(cycle);
	}
}