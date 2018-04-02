package com.third.service;

import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;

import org.jdom.Element;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.netCore.conf.CoreConfig;
import com.third.cache.StopCarOrganConfig;
import com.third.config.FieldConfig;
import com.third.config.GameConfig;
import com.third.config.ThirdConfigs;
import com.third.config.ThirdFieldConfig;
import com.third.constant.BaseConstant;

public class ThirdConfigService
{
    private static final Logger initConfigLog = LoggerFactory.getLogger(ThirdConfigService.class.getName() + BaseConstant.SPLIT + GameConfig.serverName);

    public ThirdConfigService()
    {
    }
    public static ThirdConfigs initThirdConfigs()
    {
        initConfigLog.info("开始初始化第三方服务配置...");

        ThirdConfigs thirdConfigs = new ThirdConfigs();
        try {
	        Element root = CoreConfig.getRootElement("third-config.xml");
	
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
	                if(key != null && key.equals(BaseConstant.REAL_FIELDS))
	                {
	                	Map<String, FieldConfig> mapEpGate = initEpGateConfig(e);
	                	thirdConfigs.putField(key, mapEpGate);
	                }
	                else if(key != null && key.equals(GameConfig.thirdStation))
	                {
	                	Map<String, ThirdFieldConfig> mapThird = initThirdConfig(e);
	                	thirdConfigs.putThirdField(key, mapThird);
	                }
	            }
	        } while(true);
       	} catch(Exception e) {
			String errMsg = "【Third服务器】缺少【第三方接口】配置信息...服务器强行退出！";
			initConfigLog.error(errMsg);
			throw new RuntimeException(errMsg);
       	}

        initConfigLog.info("初始化第三方服务配置结束...配置信息:【{}】", new Object[] {thirdConfigs.toString()});
        return thirdConfigs;
	}

    private static Map<String, ThirdFieldConfig> initThirdConfig(Element e)
    {
    	Map<String, ThirdFieldConfig> mapthird = new ConcurrentHashMap<String, ThirdFieldConfig>();
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
                if(id != null && id.equals("sendConfig")) {
                   	initScoConfig();
                	initsendConfig(child);
                 } else if(id != null && id.equals("Fields"))  {

                	initThirdConfigDetail(child, mapthird);
                }
            }
        } while(true);

    	initConfigLog.info((new StringBuilder()).append("初始化第三方服务配置:【").append(mapthird.toString()).append("】").toString());

        return mapthird;
	}

    public static void initScoConfig()
    {
       	StopCarOrganConfig scoCfg = GameConfig.scoCfg;
       	Element root = CoreConfig.getRootElement(GameConfig.thirdStation + ".xml");

        String serverIp = null;
        if (root.getChild("ip") != null) serverIp = root.getChild("ip").getValue().trim();
        if (serverIp == null) serverIp = "121.40.18.116";
        scoCfg.setServerIp(serverIp);

        String serverPort = null;
        if (root.getChild("port") != null) serverPort = root.getChild("port").getValue().trim();
        if (serverPort == null) serverPort = "8888";
        scoCfg.setServerPort(Integer.valueOf(serverPort));

        scoCfg.setBaseUrl();

        String partnerId = null;
        if (root.getChild("partner-id") != null) partnerId = root.getChild("partner-id").getValue().trim();
        if (partnerId == null) partnerId = "310000002";
        scoCfg.setPartnerId(partnerId);

        String partnerKey = null;
        if (root.getChild("key") != null) partnerKey = root.getChild("key").getValue().trim();
        if (partnerKey == null) partnerKey = "tlDYWr1WtNvIyDFI";
        scoCfg.setPartnerKey(partnerKey);

        String statusMethod = null;
        if (root.getChild("status-method") != null) statusMethod = root.getChild("status-method").getValue().trim();
        if (statusMethod == null) statusMethod = "/api/pipe/v1.1/status";
        scoCfg.setStatusMethod(statusMethod);

        String chargeMethod = null;
        if (root.getChild("charge-method") != null) chargeMethod = root.getChild("charge-method").getValue().trim();
        if (chargeMethod == null) chargeMethod = "/api/pipe/v1.1/charge-records";
        scoCfg.setChargeMethod(chargeMethod);
    }

    private static void initsendConfig(Element e)
    {
    	StopCarOrganConfig scoCfg = GameConfig.scoCfg;

    	if(e != null) {
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
		            if (id == null || id.length() == 0) break;
	
		            if (id.equals("Valid")) {
		            	scoCfg.setValid(Integer.valueOf(child.getValue().trim()));
		            } else if (id.equals("SendCycle")) {
		            	scoCfg.setSendRealDataCyc(Long.valueOf(child.getValue().trim()));
		            }
	            }
	        } while(true);
    	}

		if (scoCfg.getValid() == 0) scoCfg.setValid(1);
		if (scoCfg.getSendRealDataCyc() == 0) scoCfg.setSendRealDataCyc(60);
    }

    private static void initThirdConfigDetail(Element e, Map<String, ThirdFieldConfig> mapthird)
    {
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
                if(id != null && id.length() > 0)  {

		            ThirdFieldConfig thirdConfig = new ThirdFieldConfig();
		            String dataformat = child.getChild("dataformat").getValue();
			    	String[] str = dataformat.split(",");
			    	if (str.length > 1) {
			    		String len = str[1].trim().replace(")", "");
			    		thirdConfig.setLen(len);
			    	}
		            String value = child.getChild("value").getValue();
	
		            thirdConfig.setDataformat(dataformat);
		            thirdConfig.setValue(value);

                	mapthird.put(id, thirdConfig);
                }
            }
        } while(true);
	}

    private static Map<String, FieldConfig> initEpGateConfig(Element e)
    {
    	Map<String, FieldConfig> map = new ConcurrentHashMap<String, FieldConfig>();
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
	            if (id != null && id.length() > 0 && (id == "dataformat" || id == "value")) break;

	            FieldConfig epgateConfig = new FieldConfig();
	            String type = child.getChild("type").getValue();
	            String addr = child.getChild("addr").getValue();

	            epgateConfig.setType(type);
	            epgateConfig.setAddr(addr);

            	map.put(id, epgateConfig);
            	map.put(type + BaseConstant.SPLIT + addr, epgateConfig);
            }
        } while(true);

        initConfigLog.info((new StringBuilder()).append("初始化第三方服务配置:【").append(map.toString()).append("】").toString());
        return map;
	}
}