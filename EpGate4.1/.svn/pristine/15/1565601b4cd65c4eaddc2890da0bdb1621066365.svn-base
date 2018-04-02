package com.epcentre.task;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.epcentre.service.EpGunService;



public class CheckGunTask  implements Runnable {
	private static final Logger logger = LoggerFactory.getLogger(CheckGunTask.class);
	@Override
	public void run() {
		
		try
		{
			EpGunService.checkTimeout();
		}
		catch(Exception e)
		{
			logger.error("CheckGunTask exception,getStackTrace:{}",e.getStackTrace());
		}
		
		
		
	}

}
