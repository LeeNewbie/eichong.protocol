package com.third.service;

import java.io.File;
import java.io.FileOutputStream;
import java.util.Iterator;
import java.util.List;

import org.jdom.Document;
import org.jdom.Element;
import org.jdom.input.SAXBuilder;
import org.jdom.output.XMLOutputter;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.netCore.conf.CoreConfig;
import com.third.cache.SCOEp;
import com.third.cache.StopCarOrganConfig;
import com.third.config.GameConfig;
import com.third.constant.BaseConstant;
import com.third.server.SCOService;
import com.third.utils.DateUtil;

public class ThirdConfigService
{
    private static final Logger initConfigLog = LoggerFactory.getLogger(ThirdConfigService.class.getName() + BaseConstant.SPLIT + GameConfig.serverName);

    private static SCOEp initEp(String epCode, int type,int power,double initMeterNum,int i)
    {
    	long now= DateUtil.getCurrentSeconds();
    	
    	int count=5;
    	long lastSendTime =now+ (i+1)*(60/count);
    	
    	long nextChargeTime =now+200+ (i+1)*(60/count);
    	
    	SCOEp ep = new SCOEp(epCode, type,lastSendTime,nextChargeTime,power,initMeterNum);
    	
    	
    	return ep;
    }

    public static void initThirdConfigs()
    {
        initConfigLog.info("开始初始化第三方服务配置...");

        try {
	        Element root = CoreConfig.getRootElement("electricpile-config.xml");
	
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
	                if(key != null && key.equals("electricpile"))
	                {
	                	initThirdConfig(e);
	                }
	            }
	        } while(true);
       	} catch(Exception e) {
			String errMsg = "【Third服务器】缺少【第三方接口】配置信息...服务器强行退出！";
			initConfigLog.error(errMsg);
			throw new RuntimeException(errMsg);
       	}

        initScoConfig();
	}

    private static void initThirdConfig(Element e)
    {
        List<?> elementList = e.getChildren();
        Iterator<?> i$ = elementList.iterator();
    	int i=0;
        do
        {
            if(!i$.hasNext())
                break;
            Element child = (Element)i$.next();
            if(child != null)
            {
            	String id = child.getName();
            	String epCode = "3301021010000005";
            	int type = 5;
            	int power = 60;
            	double meter = 0.1;
                if(id != null && id.equals("epcode")) {
                   	epCode = child.getValue();
                } else if(id != null && id.equals("type"))  {
                	type = Integer.valueOf(child.getValue());
                } else if(id != null && id.equals("power"))  {
                	power = Integer.valueOf(child.getValue());
                } else if(id != null && id.equals("meter"))  {
                	meter = Double.parseDouble(child.getValue());
                }
            	SCOEp ep = initEp(epCode, type, power, meter, i);
            	SCOService.addEp(ep);
            	i++;
            }
        } while(true);
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

    public static void writeMeter(String epCode, int type, double meter)
    {
    	String stype = String.valueOf(type);
        String smeter = Double.toString(meter);

        try {
	        String confPath = (new StringBuilder()).append(System.getProperty("user.dir")).append(System.getProperty("file.separator")).append("conf").append(System.getProperty("file.separator")).toString();
	        String fileFullName = (new StringBuilder()).append(confPath).append("electricpile-config.xml").toString();
	        SAXBuilder sb = new SAXBuilder();
	        Document doc = sb.build(new File(fileFullName));
	        Element root = doc.getRootElement();
	
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
	                if(key != null && key.equals("electricpile"))
	                {
	                	if (epCode.equals(((Element)e.getChildren().get(0)).getValue())
	                			&& stype.equals(((Element)e.getChildren().get(1)).getValue())) {
	                		((Element)e.getChildren().get(3)).setText(smeter);
	                		break;
	                	}
	                }
	            }
	        } while(true);

	        XMLOutputter printDoc = new XMLOutputter();
	        FileOutputStream fos=new FileOutputStream(new File(fileFullName));
	        printDoc.output(doc, fos);
	        fos.flush();
	        fos.close();
       	} catch(Exception e) {
			String errMsg = "【第三方接口】配置信息写错误...服务器强行退出！";
			initConfigLog.error(errMsg);
			throw new RuntimeException(errMsg);
       	}
	}
}