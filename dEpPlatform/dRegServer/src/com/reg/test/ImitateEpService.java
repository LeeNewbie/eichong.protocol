package com.reg.test;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

public class ImitateEpService {
	
	private static final Logger logger = LoggerFactory
			.getLogger(ImitateEpService.class);
	
	private static boolean startImitate = false;
	
	public static boolean IsStart()
	{
		return startImitate;
	}
		
}
