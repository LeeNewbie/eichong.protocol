package com.phonegate.task;

import com.phonegate.service.PhoneService;

public class CheckUsrNetTimeOutTask implements Runnable {

	@Override
	public void run() {
		PhoneService.checkTimeOut();
	}
}
