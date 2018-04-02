package com.epcentre.task;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.epcentre.service.NetMessageService;

public class EpMessageTask  implements Runnable {
	private static final Logger logger = LoggerFactory.getLogger(EpMessageTask.class);
	@Override
	public void run() {
		try
		{
			NetMessageService.checkEpReSendMessage();
		}
		catch(Exception e)
		{
			logger.error("EpMessageTask exception,getStackTrace:{}",e.getStackTrace());
		}
	}

}

