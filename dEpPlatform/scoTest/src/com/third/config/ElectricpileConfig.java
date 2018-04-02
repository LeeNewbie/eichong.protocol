package com.third.config;

import com.netCore.model.GameObject;

public class ElectricpileConfig extends GameObject
{
	private static final long serialVersionUID = 5446782242553572161L;
	private String epcode;
	private String type;
	private String addr;
	private int value;
 
    public ElectricpileConfig()
    {
    }

	public String getEpcode() {
		return epcode;
	}

	public void setEpcode(String epcode) {
		this.epcode = epcode;
	}

	public String getType() {
		return type;
	}

	public void setType(String type) {
		this.type = type;
	}

	public String getAddr() {
		return addr;
	}

	public void setAddr(String addr) {
		this.addr = addr;
	}

	public int getValue() {
		return value;
	}

	public void setValue(int value) {
		this.value = value;
	}

}