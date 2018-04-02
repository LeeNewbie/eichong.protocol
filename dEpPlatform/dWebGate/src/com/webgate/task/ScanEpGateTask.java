package com.webgate.task;

import com.webgate.service.EpGateService;

public class ScanEpGateTask implements Runnable {

	@Override
	public void run() {
		EpGateService.scanEpGate();
	}
}