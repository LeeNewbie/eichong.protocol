package com.epcentre.cache;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.epcentre.constant.CommStatusConstant;
import com.epcentre.service.EpCommClientService;
import com.epcentre.service.EpGunService;
import com.epcentre.service.PhoneService;
import com.epcentre.service.StatService;
import com.epcentre.utils.DateUtil;


public class PhoneClient  extends NetObject {
	
	private static final Logger logger = LoggerFactory.getLogger(EpCommClient.class);
	
	private String  epCode; 
	
	private int epGunNo;

	private int accountId;
	
	private String account;
	
	private String ip;
	
	
	
	public PhoneClient()
	{
		epCode=""; 	
		epGunNo =0;
		accountId =0;
		
		account="";
		version =1;
		
	}
	
	
	
	public int getVersion() {
		return version;
	}



	public void setVersion(int version) {
		this.version = version;
	}



	public String getEpCode() {
		return epCode;
	}
	public void setEpCode(String epCode) {
		this.epCode = epCode;
	}
	
	public int getStatus() {
		return status;
	}
	public void setStatus(int status) {
		this.status = status;
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
	
	public void initSuccess(int usrId,String usrAccount,String epCode,int epGunNo)
	{
		super.initNetSuccess(usrAccount, getVersion());
		
		setAccountId(usrId);
		setAccount(usrAccount);
		setEpCode(epCode);
		setEpGunNo(epGunNo);
		
		
		PhoneService.addClient( this);
	}
	private void cleanPhoneAuthUser()
	{
		EpGunCache epGunCache =  EpGunService.getEpGunCache(getEpCode(), getEpGunNo());
		if(epGunCache==null)
			return ;
		AuthUserCache authUser= epGunCache.getAuthCache();
		if(authUser==null)
			return ;
		if(authUser.getStyle()==1 && authUser.getUsrId()==getAccountId() )
			epGunCache.setAuthCache(null);
	}
	
	
	public void close()
	{
		super.close();
		logger.debug("close, identity:{},channel:{}",this.identity,this.channel);
		
		StatService.unRegProtocolVersion(1,version);
	}
	
	public void handleNetTimeOut() {
		
		logger.debug("handleNetTimeOut, identity:{},channel:{}",this.identity,this.channel);
		super.handleNetTimeOut();
		cleanPhoneAuthUser();
		
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
