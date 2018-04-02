package com.epcentre.task;

import com.epcentre.service.EpService;
import com.epcentre.service.EqVersionService;
import com.epcentre.service.RateService;

public class CheckRateTask implements Runnable {
	@Override
	public void run() {
		System.out.print("CheckRateTask-----------------------");
		EpService.checkModifyRate();
		RateService.checkModifyRate();
		EqVersionService.onTimerUpdateHexFile();
	}

}
