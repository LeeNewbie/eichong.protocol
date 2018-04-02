package com.third.config;

import com.netCore.model.GameObject;

public class ThirdFieldConfig extends GameObject
{
	private static final long serialVersionUID = 5446782242553572161L;
	private String dataformat;
	private String value;
	private String len;
 
    public ThirdFieldConfig()
    {
    }

	public String getDataformat() {
		return dataformat;
	}

	public void setDataformat(String dataformat) {
		this.dataformat = dataformat;
	}

	public String getValue() {
		return value;
	}

	public void setValue(String value) {
		this.value = value;
	}

	public String getLen() {
		return len;
	}

	public void setLen(String len) {
		this.len = len;
	}

}