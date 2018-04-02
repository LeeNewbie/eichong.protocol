package com.usrlayer.cache;

import com.usrlayer.utils.ObjectCacheMap;

public class UserCache {
	
	private int id; //
	private String account; //
	//private String password;
	
	private ObjectCacheMap bespHisoryList;//预约历史表
	private ObjectCacheMap chargeHisoryList;////充电历史表
	
	private int useGunStaus;//0:空闲;3：预约中；6：充电中;10：占用
	private String useGun;
	private int level;//TODO,移动到充电开始的时候
	
	private ChargeCardCache  card;
	
	public UserCache(int userId,String userAccount,int userLevel)
	{
		id = userId; //
		account =userAccount; //
		level = userLevel;
		useGunStaus=0;//0:空闲;3：预约中；6：充电中;10：占用
		useGun="";
		
		init();
	}
	public ChargeCardCache getCard() {
		return card;
	}
	public void setCard(ChargeCardCache card) {
		this.card = card;
	}
	public int getId() {
		return id;
	}
	public void setId(int id) {
		this.id = id;
	}
	public String getUseGun() {
		return useGun;
	}
	public void setUseGun(String useGun) {
		this.useGun = useGun;
	}
	public int getUseGunStaus() {
		return useGunStaus;
	}
	public void setUseGunStaus(int useGunStaus) {
		this.useGunStaus = useGunStaus;
	}
	public String getAccount() {
		return account;
	}
	public void setAccount(String account) {
		this.account = account;
	}
	
	
	public int getLevel() {
		return level;
	}
	public void setLevel(int level) {
		this.level = level;
	}
	public void clean()
	{
		useGunStaus=0;
		useGun="";
        card=null;
        level =0;
	}
	/**
	 * 装载该用户未完成的业务
	 */
	private void init()
	{
		
	}
	
	@Override
	public String toString() {
		
	
		final StringBuilder sb = new StringBuilder();
        sb.append("UserCache");
        sb.append("{id=").append(id).append("}\n");
        sb.append(",{account=").append(account).append("}\n");
          
        sb.append(",{useGun=").append(useGun).append("}\n");   
        sb.append(",{useGunStaus=").append(useGunStaus).append("}\n");
        
        sb.append(",{card=").append(card).append("}\n");
        
    	
   		return sb.toString();
	}
	
	
	
}
