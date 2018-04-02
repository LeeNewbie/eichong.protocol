package com.usrgate.cache;

import com.usrgate.utils.DateUtil;

public class ProtocolVersion {
	
	private int version;//版本号
	private int count;//次数
	private int accountId;//用户ID
	private long loginTime;//最后登录时间

	public ProtocolVersion(int version, int accountId){
		this.version = version;
		this.count = 1;
		this.accountId = accountId;
		loginTime = DateUtil.getCurrentSeconds();
	}
	
	public int getVersion() {
		return version;
	}

	public void setVersion(int version) {
		this.version = version;
	}

	public int getCount() {
		return count;
	}

	public void setCount(int count) {
		this.count = count;
	}

	public void addCount(int accountId) {
		this.count += 1;
		this.accountId = accountId;
		loginTime = DateUtil.getCurrentSeconds();
	}

	public void minusCount() {
		this.count -= 1;
	}

	public int getAccountId() {
		return accountId;
	}

	public void setAccountId(int accountId) {
		this.accountId = accountId;
	}

	public long getLoginTime() {
		return loginTime;
	}

	public void setLoginTime(long loginTime) {
		this.loginTime = loginTime;
	}

	@Override
	public String toString() {
		
		final StringBuilder sb = new StringBuilder();
        sb.append("ProtocolVersion");
      
        sb.append(",{版本号 = ").append(version).append("}\n");
		sb.append(",{连接数 = ").append(count).append("}\n");
		sb.append(",{最后登录用户ID = ").append(accountId).append("}\n");
        String sTime= DateUtil.StringYourDate(DateUtil.toDate(loginTime*1000));
        sb.append("最后登录时间 = ").append(sTime).append("\n");
        
   		return sb.toString();
	}

}
