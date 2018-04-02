package com.cooperate.elease;

//import com.google.common.base.Strings;

import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.InputStream;
import java.util.Map;
import java.util.Properties;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.cooperate.elease.constant.Consts;
import com.cooperate.elease.constant.KeyConsts;
import com.cooperate.elease.utils.Strings;
import com.cooperate.ThirdFieldConfig;

/**
 * Created by zangyaoyi on 2016/12/30.
 */
public class ConfigManager {
    //todo:是test环境与实际环境参数不同，正式发布更改 IS_TEST 为false
   
    private static final Logger logger = LoggerFactory.getLogger(ConfigManager.class);
    //系统配置文件
    private static Properties eChongProperties = null;

    private static ConfigManager propertiesManager = null;
    
    public static String path = System.getProperty("file.separator")
			+System.getProperty("user.dir")
			+System.getProperty("file.separator") + "conf"
			+System.getProperty("file.separator");
    
    public static Properties getProperties(String fileName) {
		InputStream is = null;
		try {
			is = new FileInputStream(fileName);
		} catch (FileNotFoundException e1) {
			e1.printStackTrace();
		}
		Properties properties = new Properties();
		try {
			properties.load(is);
			if(is!=null)is.close();
		} catch (Exception e) {
			e.printStackTrace();
			throw new RuntimeException(e);
		}
		
		return properties;
	}
   
    public boolean init()
    {
    	try {
    		Properties p = getProperties(path+KeyConsts.FILE_SETTING);
        	
    		Consts.E_APP_ID = p.getProperty(KeyConsts.E_APP_ID);
    		Consts.E_APP_KEY = p.getProperty(KeyConsts.E_APP_KEY);
    		Consts.E_REAL_DATA_URL = p.getProperty(KeyConsts.E_REAL_DATA_URL);
    		Consts.E_ORDER_URL = p.getProperty(KeyConsts.E_ORDER_URL);
            
    		Consts.orgNo = Integer.valueOf(p.getProperty(KeyConsts.E_ORG_NO,"1010").trim());
    		Consts.mode = Integer.valueOf(p.getProperty(KeyConsts.E_REAL_DATA_MODE,"2").trim());
    		Consts.period = Integer.valueOf(p.getProperty(KeyConsts.E_REAL_DATA_PERIOD,"30").trim());
    		
			return true;
		} catch (Exception e) {
			e.printStackTrace();
		}
    	
    	return false;
    }
    
    public static Map<String, ThirdFieldConfig> loadConfig()
	{
		return null;
	}

   

    /**
     * 初始化Manager
     */
    public static ConfigManager getMessageManager() {
        ConfigManager manager;
        if (propertiesManager == null) {
            manager = new ConfigManager();
        } else {
            manager = propertiesManager;
        }

        return manager;

    }

   
    public static String getEChongProperties(String messageKey) {
        if (Strings.isNullOrEmpty(messageKey)) {
            return null;
        }
       
        return eChongProperties.getProperty(messageKey);

    }

    public static void main(String[] args) {
        String a = getEChongProperties("app_id");
        System.out.println(a);
    }
}
