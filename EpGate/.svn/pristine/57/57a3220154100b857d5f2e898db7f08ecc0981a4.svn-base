package com.epcentre.task;

import com.epcentre.service.EpCommClientService;
import com.epcentre.service.EpConcentratorService;


public class CheckEpCommClientTask  implements Runnable {

	@Override
	public void run() {
		
		EpCommClientService.checkTimeOut();
		EpConcentratorService.checkTimeOut();
		
	}
}