package com.webgate.cache;

import java.math.BigDecimal;

public class UserRealInfo {
	private int id; //
	private String account; //
	private String password;//这可能会随时更新,目前只能从数据库查询
	private int level;//这可能会随时更新,目前只能从数据库查询
	private int status;//这可能会随时更新，目前只能从数据库查询
	BigDecimal money;
	private String deviceid;//手机设备ID号 
	
	public int getId() {
		return id;
	}
	public void setId(int id) {
		this.id = id;
	}
	public String getAccount() {
		return account;
	}
	public void setAccount(String account) {
		this.account = account;
	}
	public String getPassword() {
		return password;
	}
	public void setPassword(String password) {
		this.password = password;
	}
	public int getLevel() {
		return level;
	}
	public void setLevel(int level) {
		this.level = level;
	}
	public int getStatus() {
		return status;
	}
	public void setStatus(int status) {
		this.status = status;
	}
	public BigDecimal getMoney() {
		return money;
	}
	public void setMoney(BigDecimal money) {
		this.money = money;
	}
	public String getDeviceid() {
		return deviceid;
	}
	public void setDeviceid(String deviceid) {
		this.deviceid = deviceid;
	}
	
	
}
