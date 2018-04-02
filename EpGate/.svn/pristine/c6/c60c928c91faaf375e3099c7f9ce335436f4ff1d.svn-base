package com.cooperate;

import java.util.List;
import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;

import org.jdom.Element;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.cooperate.elease.EleasePush;
import com.cooperate.Push;
import com.netCore.conf.CoreConfig;

public class real3rdFactory {
	
	private static final Logger logger = LoggerFactory.getLogger(real3rdFactory.class);
	
	
	private static ConcurrentHashMap<Integer,Push>  iReals=new ConcurrentHashMap<Integer,Push>();
	
	private static Map<String,measurePoint> measurePoints= new ConcurrentHashMap<String, measurePoint>();
			
	public static void init()
	{
		
		//1.配置公共测点表
		initPointConfig();
		
		//2.初始化E租网实时库
		EleasePush eLealse= new EleasePush();
		
		if(eLealse.init())
		{
			logger.info("real3rdFactory init orgNo:{} success!",eLealse.getOrgNo());
			iReals.put(eLealse.getOrgNo(), eLealse);
		}
		else
		{
			logger.info("real3rdFactory init orgNo:{} fail!",eLealse.getOrgNo());
		}
	}
	
	
	public static void initPointConfig()
	{
		try
		{
			Element root = CoreConfig.getRootElement("thirdRealData.xml");
			if (root == null) {
				return ;
			}
				
			List<Element> list = root.getChildren("point");
			for (Element e : list) {

				measurePoint pInfo = new measurePoint();
				String addr = e.getAttribute("addr").getValue();
				String type = e.getAttribute("type").getValue();
				pInfo.setAddr(Integer.parseInt(addr));
				pInfo.setType(Integer.parseInt(type));

				measurePoints.put(type + "_" + addr, pInfo);
			}
			logger.info("initPointConfig success,MapThirdRealData.size:{}",measurePoints.size());
		
		} catch (Exception e) {
             logger.error("initPointConfig read thirdRealData.xml fail,cause:{}",e.getMessage());
		}
		
	}
	
	
	public static Push getRealData(int orgNo)
	{
		return iReals.get(orgNo);
	}
	public  static Map<String,measurePoint> getmeasurePoints()
	{
		return measurePoints;
	}

}
