package com.epcentre.cache;

import java.util.Iterator;
import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.epcentre.constant.EpConstantErrorCode;
import com.epcentre.constant.UserConstant;
import com.epcentre.service.EpBespokeService;
import com.epcentre.service.EpChargeService;
import com.epcentre.service.EpGunService;
public class UserCache {
	
	private static final Logger logger = LoggerFactory.getLogger(UserCache.class);
	
	
	private int id; //
	private String account; 
	private String name; 
	
	private Map<String, BespCache> bespList = new ConcurrentHashMap<String,BespCache>();
	/**
	 * 因为可以多充，所以对用户来说，没有当前充电.
	 */
	private Map<String, ChargeCache> chargeList = new ConcurrentHashMap<String,ChargeCache>();
	
	
	private int level;
	
	private int newcouponAcStatus;//用户交流新手券状态 -1，未判断 ，0：未使用，1：已使用
	private int newcouponDcStatus;//用户直流新手券状态-1，未判断 ，0：未使用，1：已使用
	
	private String invitePhone;//邀请者号码
	
	private int remainAmtWarnValue;
	private String remainAmtWarnPhone;
	private String remainAmtWarnCPhone;
	private boolean remainAmtWarn;
	
	public UserCache(int userId,String userAccount,String userName,int userLevel,String userInvitePhone)
	{
		id = userId; //
		account =userAccount; //
		level = userLevel;
	
		newcouponAcStatus=-1;//-1，未判断
		newcouponDcStatus=-1;//-1，未判断
		invitePhone=userInvitePhone;
		
		name=userName;
		
		init();
		
		remainAmtWarnValue= 0;
		remainAmtWarnPhone="";
		remainAmtWarnCPhone="";
		remainAmtWarn=false;
	}
	/**
	 * 装载该用户未完成的业务
	 * 1.装载用户未完成的预约
	 * 2.装载用户未完成的充电
	 */
	private void init()
	{
		EpBespokeService.getUnStopBespokeByUserIdFromDb(this);
		EpChargeService.getUnFinishChargeByUserIdFromDb(this);
		
	}
	public void removeBesp(String bespokeNo)
	{
		if(bespokeNo!=null && bespokeNo.length()>0)
		{
			bespList.remove(bespokeNo);
		}
	}
	public void addBesp(BespCache cache)
	{
		bespList.put(cache.getBespNo(), cache);
	}
	public void removeCharge(String chargeSerialNo)
	{
		if(chargeSerialNo!=null && chargeSerialNo.length()>0)
		{
			chargeList.remove(chargeSerialNo);
		}
	}
	public void addCharge(ChargeCache cache)
	{
		chargeList.put(cache.getChargeSerialNo(), cache);
	}
	
	
	public String getInvitePhone() {
		return invitePhone;
	}


	public void setInvitePhone(String invitePhone) {
		this.invitePhone = invitePhone;
	}


	public int getNewcouponAcStatus() {
		return newcouponAcStatus;
	}

	public void setNewcouponAcStatus(int newcouponAcStatus) {
		this.newcouponAcStatus = newcouponAcStatus;
	}

	public int getNewcouponDcStatus() {
		return newcouponDcStatus;
	}
	public void setNewcouponDcStatus(int newcouponDcStatus) {
		this.newcouponDcStatus = newcouponDcStatus;
	}
	


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
	
	
	public int getLevel() {
		return level;
	}
	public void setLevel(int level) {
		this.level = level;
	}
	public void clean()
	{
	}
	
	
	public String getName() {
		return name;
	}
	public void setName(String name) {
		this.name = name;
	}
	public int getRemainAmtWarnValue() {
		return remainAmtWarnValue;
	}
	public void setRemainAmtWarnValue(int remainAmtWarnValue) {
		this.remainAmtWarnValue = remainAmtWarnValue;
	}
	public String getRemainAmtWarnPhone() {
		return remainAmtWarnPhone;
	}
	public void setRemainAmtWarnPhone(String remainAmtWarnPhone) {
		this.remainAmtWarnPhone = remainAmtWarnPhone;
	}
	
	public boolean isRemainAmtWarn() {
		return remainAmtWarn;
	}
	public void setRemainAmtWarn(boolean remainAmtWarn) {
		this.remainAmtWarn = remainAmtWarn;
	}
	
	public String getRemainAmtWarnCPhone() {
		return remainAmtWarnCPhone;
	}
	public void setRemainAmtWarnCPhone(String remainAmtWarnCPhone) {
		this.remainAmtWarnCPhone = remainAmtWarnCPhone;
	}
	@Override
	public String toString() {
		
	
		final StringBuilder sb = new StringBuilder();
        sb.append("UserCache");
        sb.append("{id=").append(id).append("}\n");
        sb.append(",{account=").append(account).append("}\n");
          
        int bespokeCount = bespList.size();
        if(bespokeCount>0)
        {
	        sb.append("bespoke list!count").append(bespList.size()).append(":\n");
	        Iterator iter = bespList.entrySet().iterator();
			
			while (iter.hasNext()) {
				Map.Entry entry = (Map.Entry) iter.next();
				BespCache o=(BespCache) entry.getValue();
				if(o==null)
					continue;
				sb.append(o.toString());
			}
        }
		
		int chargeCount = chargeList.size();
		if(chargeCount>0)
		{
			sb.append("charge list!count").append(chargeList.size()).append(":\n");
			Iterator iter = chargeList.entrySet().iterator();
			
			while (iter.hasNext()) {
				Map.Entry entry = (Map.Entry) iter.next();
				ChargeCache o=(ChargeCache) entry.getValue();
				if(o==null)
					continue;
				sb.append(o.toString());
			}
		}
    	
   		return sb.toString();
	}
	public int canBespoke(String usingGun)
	{
		
		Iterator iter = bespList.entrySet().iterator();
		
		while (iter.hasNext()) {
			Map.Entry entry = (Map.Entry) iter.next();
			BespCache o=(BespCache) entry.getValue();
			if(o==null)
				continue;
			String epGunNo = o.getEpCode()+o.getEpGunNo();
			if(epGunNo.compareTo(usingGun)!=0)
				return EpConstantErrorCode.SELF_HAVED_BESP;
				
		}
		iter = chargeList.entrySet().iterator();
		
		while (iter.hasNext()) {
			Map.Entry entry = (Map.Entry) iter.next();
			
			ChargeCache o=(ChargeCache) entry.getValue();
			if(o==null)
				continue;
			String epGunNo = o.getEpCode()+o.getEpGunNo();
			if(epGunNo.compareTo(usingGun)!=0)
				return EpConstantErrorCode.CAN_NOT_BESP_IN_ELE;;	
		}
		return 0;
	}
	/**
	 * 只有电桩失联并且预冻结的充电
	 * @return
	 */
	public ChargeCache getHistoryCharge(String epGunNo)
	{
		Iterator iter = chargeList.entrySet().iterator();
		
		while (iter.hasNext()) {
			Map.Entry entry = (Map.Entry) iter.next();
			ChargeCache o=(ChargeCache) entry.getValue();
			EpGunCache epGun = EpGunService.getEpGunCache(o.getEpCode(), o.getEpGunNo());
			if(epGun==null)
				continue;
			String s= epGun.getEpCode() + epGun.getEpGunNo();
			if(s.compareTo(epGunNo)==0)
				continue;
			INetObject netObject = epGun.getEpNetObject();
			if(netObject!=null && !netObject.isComm())
				return o;	
		}
		return null;
	}
	private int canPauseStatOldCharge(String chargedEpCode,int chargedEpGunNo,String usingGun)
	{
		String sChargedEpGunNo = chargedEpCode + chargedEpGunNo;
		
		if(sChargedEpGunNo.compareTo(usingGun)==0)
			return 0;
		
		EpGunCache epGun = EpGunService.getEpGunCache(chargedEpCode, chargedEpGunNo);
		
		if(epGun==null)
			return 0;
		INetObject netObject = epGun.getEpNetObject();
		if(netObject.isComm())
		{
			return EpConstantErrorCode.EPE_REPEAT_CHARGE;
		}
		
		return 0;
		
	}
	/**
	 * 
	 * @param usingGun
	 * @return
	 */
	public int canCharge(String usingGun,int chargingUsrId,int chargingUsrOrgNo,String chargingUsrIdentity,int pkCard,boolean allowMutliCharge)
	{
		Iterator iter = bespList.entrySet().iterator();
		
		while (iter.hasNext()) {
			Map.Entry entry = (Map.Entry) iter.next();
			BespCache o=(BespCache) entry.getValue();
			if(o==null)
				continue;
			String epGunNo = o.getEpCode()+o.getEpGunNo();
			if(epGunNo.compareTo(usingGun)!=0)
				return EpConstantErrorCode.CANNOT_OTHER_OPER;
				
		}
		
		if(chargeList.size()<=0)
		{
			return 0;
		}
		
		if(pkCard>0 && allowMutliCharge)
			return 0;
		
	
		iter = chargeList.entrySet().iterator();
		
		String epCode = "";
		int epGunNo = 0;
		int chargedUsrId = 0;
		String chargedUsrIdentity = "";
		
		int errorCode=0;
		while (iter.hasNext()) {
			Map.Entry entry = (Map.Entry) iter.next();
			ChargeCache o=(ChargeCache) entry.getValue();
			epCode = o.getEpCode();
			epGunNo = o.getEpGunNo();
			chargedUsrId = o.getUserId();
			chargedUsrIdentity = o.getThirdUsrIdentity();
			
			if(chargingUsrOrgNo == UserConstant.ORG_I_CHARGE ||
					(chargingUsrOrgNo != UserConstant.ORG_I_CHARGE&& chargingUsrIdentity.equals(chargedUsrIdentity)))
			{
				errorCode= canPauseStatOldCharge(epCode,epGunNo,usingGun);
				if(errorCode>0)
				{
					break;
				}
			}
			
		}
		
		logger.debug("canCharge,errorCode:{},usingGun:{},chargingUsrId:{},orgNo:{},chargingUsrIdentity:{},pkCard:{},allowMutliCharge:{}",
				new Object[]{errorCode,usingGun,chargingUsrId,chargingUsrOrgNo,chargingUsrIdentity, pkCard,allowMutliCharge,
				epCode,epGunNo,chargedUsrId,chargedUsrIdentity});
		
		return errorCode;
		
	}
}
