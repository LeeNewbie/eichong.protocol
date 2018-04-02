package com.usrlayer.config;

import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;

import com.netCore.model.GameObject;
public class ThirdConfigs extends GameObject
{
	private static final long serialVersionUID = 8196712018915136998L;

	private ConcurrentHashMap<String, Map<String, FieldConfig>> fieldcfgs;
	private ConcurrentHashMap<String, Map<String, ThirdFieldConfig>> thirdfieldcfgs;

    public ThirdConfigs()
    {
    	fieldcfgs = new ConcurrentHashMap<String, Map<String, FieldConfig>>();
    	thirdfieldcfgs = new ConcurrentHashMap<String, Map<String, ThirdFieldConfig>>();
    }
    public void putField(String key, Map<String, FieldConfig> cfg)
    {
    	fieldcfgs.put(key, cfg);
    }
    public Map<String, FieldConfig> getField(String key)
    {
    	return (Map<String, FieldConfig>)fieldcfgs.get(key);
    }
    public void putThirdField(String key, Map<String, ThirdFieldConfig> cfg)
    {
    	thirdfieldcfgs.put(key, cfg);
    }
    public Map<String, ThirdFieldConfig> getThirdField(String key)
    {
    	return (Map<String, ThirdFieldConfig>)thirdfieldcfgs.get(key);
    }
}