package com.usrgate.service;

import java.util.Iterator;
import java.util.List;
import java.util.concurrent.ConcurrentHashMap;

import org.jdom.Element;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.netCore.conf.CoreConfig;
import com.usrgate.config.CycleConfigs;
import com.usrgate.config.GameConfig;
import com.usrgate.constant.BaseConstant;
import com.usrgate.utils.JudgeNullUtils;

public class CycleConfigService
{
    private static final Logger initConfigLog = LoggerFactory.getLogger(CycleConfigService.class.getName() + BaseConstant.SPLIT + GameConfig.serverName);

    public CycleConfigService()
    {
    }
    public static CycleConfigs initConfigs()
    {
        CycleConfigs configs = new CycleConfigs();
        try {
	        Element root = CoreConfig.getRootElement("StatisticsCycle.xml");
	
	        List<?> elementList = root.getChildren();
	        Iterator<?> i$ = elementList.iterator();
	        do
	        {
	            if(!i$.hasNext())
	                break;
	            Element e = (Element)i$.next();
	            if(e != null)
	            {
	
	                String key = e.getName();
	                if (key != null && key.equals("phone-flow-cycle"))
	                {
	                	configs.setFlowcfgs(initConfig(e));
	                }
	                else if (key != null && key.equals("phone-instruct-cycle"))
	                {
	                	configs.setInstructcfgs(initConfig(e));
	                }
	                else if (key != null && key.equals("ep-flow-cycle"))
	                {
	                	configs.setEpFlowcfgs(initConfig(e));
	                }
	                else if (key != null && key.equals("ep-instruct-cycle"))
	                {
	                	configs.setEpInstructcfgs(initConfig(e));
	                }
	            }
	        } while(true);
       	} catch(Exception e) {
			String errMsg = "【服务器】缺少【统计周期】配置信息...";
			initConfigLog.debug(errMsg);
       	}

        return configs;
	}

    private static ConcurrentHashMap<Integer, Integer> initConfig(Element e)
    {
    	ConcurrentHashMap<Integer, Integer> map = new ConcurrentHashMap<Integer, Integer>();
        List<?> elementList = e.getChildren();
        Iterator<?> i$ = elementList.iterator();
        do
        {
            if(!i$.hasNext())
                break;

            Element child = (Element)i$.next();
            if(child != null)
            {
            	String id = child.getName();
            	String val = child.getValue();
                if(id != null && id.equals("cycle")) {
                    if (!JudgeNullUtils.isEmpty(val))
                    	map.put(Integer.valueOf(val.trim()), Integer.valueOf(val.trim()));
                }
            }
        } while(true);

        return map;
	}
}