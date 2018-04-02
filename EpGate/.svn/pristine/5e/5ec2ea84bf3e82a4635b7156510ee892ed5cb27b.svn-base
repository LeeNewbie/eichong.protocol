package com.epcentre.task;

import com.epcentre.epconsumer.ChinaMobileService;



public class ChinaMobileTask  implements Runnable {
	private int tokenType;
	public ChinaMobileTask(int type)
	{
		tokenType = type;
		
	}
	@Override
	public void run() {
		//System.out.print("CheckRateTask-----------------------");
		
		ChinaMobileService.fetchToken(tokenType);
	}

}
