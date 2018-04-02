package com.webgate.task;

import com.webgate.service.PhoneService;

public class CheckUsrNetTimeOutTask implements Runnable {

	@Override
	public void run() {
		PhoneService.checkTimeOut();
	}
}
