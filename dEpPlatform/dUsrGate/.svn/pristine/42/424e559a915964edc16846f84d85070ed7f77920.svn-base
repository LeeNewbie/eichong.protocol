package com.usrgate.net.client;

import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;

import com.usrgate.cache.FlowStatistics;
import com.usrgate.cache.InstructStatistics;



public class PhoneClient  extends ECTcpClient {
	
	private String  epCode; 
	
	private int epGunNo;

	private int accountId;
	
	private String account;
	
	private String ip;
	private int version;
	private int cmd;
	private boolean connectFlg;

	private Map<Integer, FlowStatistics> flowMap = new ConcurrentHashMap<Integer, FlowStatistics>();	

	private Map<Integer, InstructStatistics> instructMap = new ConcurrentHashMap<Integer, InstructStatistics>();	
	
	public PhoneClient()
	{
		epCode=""; 	
		epGunNo =0;
		accountId =0;
		
		account="";
		
	}
	
	public String getEpCode() {
		return epCode;
	}
	public void setEpCode(String epCode) {
		this.epCode = epCode;
	}
	

	
	
	public int getAccountId() {
		return accountId;
	}
	public void setAccountId(int accountId) {
		this.accountId = accountId;
	}
	public String getAccount() {
		return account;
	}
	public void setAccount(String account) {
		this.account = account;
	}
	public void setEpGunNo(int epGunNo) {
		this.epGunNo = epGunNo;
	}

	public String getIp() {
		return ip;
	}

	public void setIp(String ip) {
		this.ip = ip;
	}

	public int getEpGunNo() {
		return epGunNo;
	}
	
	public int getVersion() {
		return version;
	}

	public void setVersion(int version) {
		this.version = version;
	}

	public int getCmd() {
		return cmd;
	}

	public void setCmd(int cmd) {
		this.cmd = cmd;
	}

	public boolean isConnectFlg() {
		return connectFlg;
	}

	public void setConnectFlg(boolean connectFlg) {
		this.connectFlg = connectFlg;
	}
	
	public FlowStatistics getFlowStat(int cycle) {
		return flowMap.get(cycle);
	}

	public void setFlowStat(FlowStatistics flowStat) {
		flowMap.put(flowStat.getCycle(), flowStat);
	}

	public InstructStatistics getInstructStat(int cycle) {
		return instructMap.get(cycle);
	}

	public void setInstructStat(InstructStatistics instructStat) {
		instructMap.put(instructStat.getCycle(), instructStat);
	}

	@Override
	public String toString() {
		
		final StringBuilder sb = new StringBuilder();
        sb.append("PhoneClient");
        
        sb.append("{epCode=").append(epCode).append("}\n");
        sb.append(",{epGunNo=").append(epGunNo).append("}\n");
        sb.append(",{accountId=").append(accountId).append("}\n");
        
       
        sb.append(",{account=").append(account).append("}\n");
        sb.append(",{ip=").append(ip).append("}\n");
   		return sb.toString();
	}

	

	
}
