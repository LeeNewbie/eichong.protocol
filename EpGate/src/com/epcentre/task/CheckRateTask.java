package com.epcentre.task;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.epcentre.service.EpService;
import com.epcentre.service.EqVersionService;
import com.epcentre.service.RateService;

public class CheckRateTask implements Runnable {
	private static final Logger logger = LoggerFactory.getLogger(CheckRateTask.class);
	@Override
	public void run() {
		//System.out.print("CheckRateTask-----------------------");
		
		try
		{
			RateService.checkModifyRate();
			
			EpService.checkModifyRate();
			
			EqVersionService.onTimerUpdateHexFile();
		}
		catch(Exception e)
		{
			logger.error("CheckRateTask exception,getStackTrace:{}",e.getStackTrace());
		}
	}

}
