package com.usrgate.task;

import com.usrgate.service.PhoneService;

public class CheckUsrNetTimeOutTask implements Runnable {

	@Override
	public void run() {
		PhoneService.checkTimeOut();
	}
}
