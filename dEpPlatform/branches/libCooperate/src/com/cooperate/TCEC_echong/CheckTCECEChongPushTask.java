package com.cooperate.TCEC_echong;


public class CheckTCECEChongPushTask implements Runnable {

	@Override
	public void run() {
		TCECEChongService.checkPushTimeout();
	}
}