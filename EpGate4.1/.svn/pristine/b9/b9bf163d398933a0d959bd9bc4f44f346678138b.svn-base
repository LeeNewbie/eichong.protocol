package com.epcentre.task;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.epcentre.service.AnalyzeService;
import com.epcentre.service.EpCommClientService;
import com.epcentre.service.EpConcentratorService;
import com.epcentre.service.EpGunService;


public class CheckEpCommClientTask  implements Runnable {
    
	private static final Logger logger = LoggerFactory.getLogger(CheckEpCommClientTask.class);
	@Override
	public void run() {
		
		try
		{
			EpCommClientService.checkTimeOut();
		}
		catch(Exception e)
		{
			logger.error("CheckEpCommClientTask exception,getStackTrace:{}",e.getStackTrace());
		}
		
	}
}