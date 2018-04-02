package com.epcentre.task;

import com.epcentre.service.EpGunService;



public class CheckGunTask  implements Runnable {
	
	@Override
	public void run() {
		
		
		EpGunService.checkTimeout();
	}

}
