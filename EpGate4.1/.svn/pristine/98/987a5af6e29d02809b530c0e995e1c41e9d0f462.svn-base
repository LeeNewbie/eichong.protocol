package com.epcentre.task;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.epcentre.service.EpGunService;

public class ThirdMessageTask  implements Runnable {
	private static final Logger logger = LoggerFactory.getLogger(ThirdMessageTask.class);
	@Override
	public void run() {
	
		try
		{
			EpGunService.handleThirdRealData();
		}
		catch(Exception e)
		{
			logger.error("ThirdMessageTask exception,getStackTrace:{}",e.getStackTrace());
		}
		
	}

}