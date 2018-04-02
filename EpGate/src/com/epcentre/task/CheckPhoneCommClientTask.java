package com.epcentre.task;


import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.epcentre.service.PhoneService;

public class CheckPhoneCommClientTask implements Runnable {
	private static final Logger logger = LoggerFactory.getLogger(CheckPhoneCommClientTask.class);
	@Override
	public void run() {
		try
		{
			PhoneService.checkTimeOut();
		}
		catch(Exception e)
		{
			logger.error("CheckPhoneCommClientTask exception,getStackTrace:{}",e.getStackTrace());
		}
		
		
	}
}
