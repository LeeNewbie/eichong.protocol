package com.third.epconsumer;

import java.math.BigDecimal;
import java.util.ArrayList;
import java.util.Date;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import net.sf.json.JSONObject;

import org.apache.commons.lang.RandomStringUtils;
import org.apache.commons.lang.StringUtils;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.alibaba.fastjson.JSON;
import com.ormcore.dao.DB;
import com.third.config.FieldConfig;
import com.third.config.GameConfig;
import com.third.config.ThirdFieldConfig;
import com.third.constant.BaseConstant;
import com.third.constant.EpProtocolConstant;
import com.third.service.EpGateService;
import com.third.utils.DateUtil;
import com.third.utils.RequestUtil;
import com.third.utils.StringUtil;

public class StopCarOrganService  {
	
	private static final Logger logger = LoggerFactory.getLogger(StopCarOrganService.class.getName() + BaseConstant.SPLIT + GameConfig.serverName);
	

	public static long getTimeStamp()
	{
		return System.currentTimeMillis();
	}
	
    private static final String DateFmt = "yyyy-MM-dd HH:mm:ss";
	
	private static String getFmtTime(long time)
	{
		java.util.Date date = new Date(time*1000);
		
		return DateUtil.toDateFormat(date, DateFmt); 
	}

	/**
	 * 3充电桩充电记录
	 */
	@SuppressWarnings({ "rawtypes", "unchecked" })
	public static void realData(String key)
	{
		Map<String,Object> pointMap = EpGateService.getRealData(key);
		if (pointMap == null || pointMap.size() == 0) return;
		
		String partnerKey = GameConfig.scoCfg.getPartnerKey();
		String partnerId = GameConfig.scoCfg.getPartnerId();
		String baseUri = GameConfig.scoCfg.getBaseUrl()+GameConfig.scoCfg.getStatusMethod();

		//参数转换
		String[] val = key.split(BaseConstant.ESCAPE + BaseConstant.SPLIT);
		String statusTime= getFmtTime((long)pointMap.get(EpProtocolConstant.STATUS_TIME));
		HashMap<String,Object> paramsMap=new HashMap<String,Object>();
		
		String patternString = "\\$\\{(" + StringUtils.join(pointMap.keySet(), "|") + ")\\}";
		Pattern pattern = Pattern.compile(patternString);

		Map<String, ThirdFieldConfig> thirdMap = EpGateService.getThirdConfigs().getThirdField(GameConfig.thirdStation);
		Iterator iter = thirdMap.entrySet().iterator();
		boolean nullFlag = true;
		while (iter.hasNext()) {
			Map.Entry entry = (Map.Entry) iter.next();
			ThirdFieldConfig thirdFieldCfg = (ThirdFieldConfig)entry.getValue();
			String value = thirdFieldCfg.getValue();
			Matcher matcher = pattern.matcher(value);
			StringBuffer sb = new StringBuffer(); 
		    while(matcher.find()) { 
		        matcher.appendReplacement(sb, StringUtil.getString(((FieldConfig)pointMap.get(matcher.group(1))).getValue())); 
		    } 
		    matcher.appendTail(sb);
		    value = sb.toString().replaceAll("@curType", val[2]);
		    value = DB.userInfoDao.executeSql("select " + value);
		    String dataformat = thirdFieldCfg.getDataformat();
		    if (dataformat.indexOf("number") >= 0) {
		    	if (value == null || new BigDecimal(value).compareTo(new BigDecimal(0)) != 0) nullFlag = false;
		    	String len = thirdFieldCfg.getLen();
		    	if (len != null && !"".equals(len)) {
		    		value = new BigDecimal(value).setScale(Integer.valueOf(len), java.math.BigDecimal.ROUND_HALF_UP).toString();
		    	}
		    }

		    paramsMap.put((String)entry.getKey(), value);
		}
		// 当chargepoleStatus为3时，以下参数不能同时为空、不能同时为零
		if ("3".equals(thirdMap.get("chargepoleStatus").getValue()) && nullFlag) {
			logger.debug("chargepoleStatus = 3 and all args is null");
			return;
		}

		//运营服务商标识
		paramsMap.put("operatorId", partnerId);
		// 1.充电桩编码poleId字符串由平台提前注册接入	
		paramsMap.put("equipNo", val[0]);
		// 2.充电枪编码gunNo字符串由平台提前注册接入
		paramsMap.put("gunNo", val[1]);
		// 6.状态时间参照yyyy-MM-dd HH:mm:ss
		paramsMap.put("statusTime", statusTime);
		ArrayList list = new ArrayList();
		list.add(paramsMap);
		HashMap<String,Object> paramsMapTemp=new HashMap<String,Object>();
		paramsMapTemp.put("data", list);
		JSONObject jsonObject = JSONObject.fromObject(paramsMapTemp);
		

		String timestamp = ""+ getTimeStamp();		
		String token= RandomStringUtils.randomAlphanumeric(16);
		String toSign = jsonObject.toString()+"#"+ token + "#" + timestamp;
	    String sign = ApiSecurityUtils.encrypt(toSign, partnerKey, ApiSecurityUtils.HS256);
		
		HashMap<String,Object>  params=new HashMap<String,Object>();
		params.put("partnerId", partnerId);
		params.put("timestamp", timestamp);
		params.put("token", token);
		params.put("input", jsonObject.toString());
		params.put("sign", sign);
		
		logger.debug("realData!baseUri:{},paramsMap:{}",baseUri,params);
		
    	String res = RequestUtil.doPost(baseUri, params);
    	Map<String,Object> ret = (Map<String,Object>)JSON.parse(res);
    	if (!StringUtils.equals("0", (String)ret.get("ret"))) logger.debug("sco realData ret:{}",ret);

    	logger.debug("sco realData res:{}",res);
	}
	
	/**
	 * 4充电桩充电结算记录
	 */
	@SuppressWarnings({ "rawtypes", "unchecked" })
	public static void chargeRecord(String key)
	{
		Map<String,Object> pointMap = EpGateService.getRealData(key);
		if (pointMap == null || pointMap.size() == 0) return;

		String partnerKey = GameConfig.scoCfg.getPartnerKey();
		String partnerId = GameConfig.scoCfg.getPartnerId();
		String baseUri = GameConfig.scoCfg.getBaseUrl()+GameConfig.scoCfg.getChargeMethod();
	    
		//参数转换
		String[] val = key.split(BaseConstant.ESCAPE + BaseConstant.SPLIT);
		String startTime= getFmtTime((long)pointMap.get(EpProtocolConstant.STARTTIME));
		String endTime =getFmtTime((long)pointMap.get(EpProtocolConstant.ENDTIME));
	
		HashMap<String,Object>  paramsMap=new HashMap<String,Object>();
        // 运营服务商标识
		paramsMap.put("operatorId",partnerId);
		// 充电记录编号	
		paramsMap.put(EpProtocolConstant.SERIALNO, pointMap.get(EpProtocolConstant.SERIALNO));
		// 1.充电桩编码
		paramsMap.put("equipNo", val[0]);
		// 2.充电枪编码gunNo字符串由平台提前注册接入
		paramsMap.put("gunNo", val[1]);
		// 3.充电量meterNumFLOAT单位度 (小数点后3位 )	
		paramsMap.put(EpProtocolConstant.PQ, pointMap.get(EpProtocolConstant.PQ));
		// 4.开始时间startTime字符串参照yyyy-MM-dd HH:mm:ss
		paramsMap.put("startTime", startTime);
		// 5.结束时间endTime字符串参照yyyy-MM-dd HH:mm:ss
		paramsMap.put("endTime", endTime);
		ArrayList list = new ArrayList();
		list.add(paramsMap);
		HashMap<String,Object>  paramsMapTemp=new HashMap<String,Object>();
		paramsMapTemp.put("data", list);
		JSONObject jsonObject = JSONObject.fromObject(paramsMapTemp);
		
		
		String timestamp = ""+ getTimeStamp();
		String token= RandomStringUtils.randomAlphanumeric(16);
		logger.debug("input:{}",jsonObject.toString());
		
		String toSign =jsonObject.toString()+"#"+ token + "#" + timestamp;	
		String sign = ApiSecurityUtils.encrypt(toSign, partnerKey, ApiSecurityUtils.HS256);
	
		HashMap<String,Object>  params=new HashMap<String,Object>();
		params.put("partnerId", partnerId);
		params.put("timestamp", timestamp);
		params.put("token", token);
		params.put("input", jsonObject.toString());
		params.put("sign", sign);

	    logger.debug("chargeRecord!baseUri:{},paramsMap:{}",baseUri,params);
    	String res = RequestUtil.doPost(baseUri, params);
    	Map<String,Object> ret = (Map<String,Object>)JSON.parse(res);
    	if (!StringUtils.equals("0", (String)ret.get("ret"))) {
    		logger.debug("sco realData ret:{}",ret);
    		EpGateService.removeRealData(key);
    	}
    
    	logger.debug("sco chargeRecord res:{}",res);
	}
	
	public static void main(String[] args) {
	/*	valid=1;
		serverIp="121.40.18.116";
		//serverIp="hzskynet.i91pv.com";
		//serverPort=55555;
		serverPort=8888;
		partnerId="310000002";
		partnerKey="tlDYWr1WtNvIyDFI";
		sendRealDataCyc=60;
		baseUrl = "https://"+serverIp+":"+serverPort;
		statusMethod="/api/pipe/status";
		chargeMethod="/api/pipe/charge-records";
		
		StopCarOrganService.realData("3301021010000009", 1, 2, 0, 0, 1470817277, 
		new BigDecimal(220.1), new BigDecimal(220.2), new BigDecimal(220.3), new BigDecimal(1.1), 
		new BigDecimal(1.2),new BigDecimal(1.3), new BigDecimal(50), new BigDecimal(380.0), 
		new BigDecimal(40.0), 65, 50);
		
		
		StopCarOrganService.chargeRecord("3301021010000009", 1,new BigDecimal(111.2),
				1467012163,1467012178);
		*/
		
	}
	
}
