package com.epcentre.task;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.epcentre.service.AnalyzeService;

public class CheckMonitorCommTask implements Runnable {
	
	private static final Logger logger = LoggerFactory.getLogger(CheckMonitorCommTask.class);
	@Override
	public void run() {
		
		try
		{
			AnalyzeService.checkTimeOut();
		}
		catch(Exception e)
		{
			logger.error("CheckMonitorCommTask exception,getStackTrace:{}",e.getStackTrace());
		}
	
		
	}

}
