package com.usrlayer.task;

import com.usrlayer.service.EpGateService;

public class ScanEpGateTask implements Runnable {

	@Override
	public void run() {
		EpGateService.scanEpGate();
	}
}