package com.third.server;

import java.util.Iterator;
import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.TimeUnit;

import com.netCore.core.pool.TaskPoolFactory;
import com.third.cache.SCOEp;
import com.third.epconsumer.StopCarOrganService;
import com.third.task.TestTask;
import com.third.utils.DateUtil;

public class SCOService {
	
	private static ConcurrentHashMap<String, SCOEp> eps= new ConcurrentHashMap<String, SCOEp>();
	
	public static void addEp(SCOEp ep)
	{
		eps.put(ep.getEpCode(), ep);
	}
	
	public static ConcurrentHashMap<String, SCOEp> getEps() {
		return eps;
	}
	
	public static void startThirdPushTimeout(long initDelay) {
		
		TestTask checkTask = new TestTask();
				
		TaskPoolFactory.scheduleAtFixedRate("CHECK_THIRDPUSH_TIMEOUT_TASK", checkTask, initDelay, 5, TimeUnit.SECONDS);
	}

	public static void check()
	{
		Iterator iter = eps.entrySet().iterator();
		while (iter.hasNext()) {
		
			Map.Entry entry = (Map.Entry) iter.next();
			if(entry==null)
			{
				break;	
			}
			
			SCOEp ep=(SCOEp) entry.getValue();
			
			if(ep==null)
			{
				continue;	
			}
			
			long now= DateUtil.getCurrentSeconds();
			if(now>ep.getlStartTime() && ep.getWorkStatus() == 2)//空闲
			{
				ep.gotoCharge();
			}
			if(now>ep.getlEndTime() && ep.getWorkStatus() == 3)//充电状态
			{
				ep.fixMeterNum();
				StopCarOrganService.chargeRecord(ep);
				
				ep.setSendScoLastTime(now);
				ep.gotoIdle();
			}
			
			if(now>ep.getlStartTime() && now<ep.getlEndTime() && ep.getWorkStatus()==3)//
			{
				ep.gotoStepCharge();
			}
					
			
			if(now>(ep.getSendScoLastTime()+60))
			{
				ep.setSendScoLastTime(now);
				//发送实时数据
				StopCarOrganService.realData(ep);
			}
		}
	}

}
