package com.webgate.service;

import java.math.BigDecimal;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.ormcore.dao.DB;
import com.ormcore.model.TblChargeCard;
import com.ormcore.model.TblUserInfo;
import com.webgate.cache.ChargeCardCache;
import com.webgate.cache.ElectricPileCache;
import com.webgate.cache.RateInfoCache;
import com.webgate.cache.UserCache;
import com.webgate.cache.UserRealInfo;
import com.webgate.config.Global;
import com.webgate.constant.EpConstant;
import com.webgate.constant.EpConstantErrorCode;
import com.webgate.protocol.CardAuthResp;
import com.webgate.protocol.WmIce104Util;

public class UserService {
	
	private static final Logger logger = LoggerFactory.getLogger(UserService.class);
	
	public static UserCache getUserCache(String account)
	{
		UserCache userInfo = CacheService.getMapEpUserInfo().get(account);
		if(userInfo!=null)
		{
			return userInfo;
		}
		UserRealInfo userRealInfo =  findUserRealInfo(account);
		if(userRealInfo==null)
			return null;
		
		return CacheService.convertToCache(userRealInfo);
	}
	public static UserCache getUserCache(int accountId)
	{
		UserCache userInfo = CacheService.getMapEpUserInfo2().get(accountId);
		if(userInfo!=null)
		{
			return userInfo;
		}
		UserRealInfo userRealInfo =  findUserRealInfo(accountId);
		if(userRealInfo==null)
			return null;
		
		return CacheService.convertToCache(userRealInfo);
	}
	public static int checkUserGun(int accountId, String chargeGun)
	{
		UserCache userInfo = CacheService.getMapEpUserInfo().get(accountId);
		if(userInfo!=null)
		{
			if (userInfo.getUseGun().length() > 0
					&& userInfo.getUseGun().compareTo(chargeGun) != 0) {
				if (userInfo.getUseGunStaus() == 6) {
					return EpConstantErrorCode.EPE_REPEAT_CHARGE;
				} else if (userInfo.getUseGunStaus() == 3) {
					return EpConstantErrorCode.CANNOT_OTHER_OPER;
				} else {
					return EpConstantErrorCode.USER_OPER_IN_OTHER_EP;
				}
			}
		}
		
		return 0;
	}
	
	public static ChargeCardCache convertCardToCache(TblChargeCard tabCardInfo)
	{
		if(tabCardInfo == null)
			return null;
		
		int payMode =1;
		if(tabCardInfo.getUc_pay_mode()==12)
		{
			payMode=2;
		}
		ChargeCardCache card= new ChargeCardCache(
				tabCardInfo.getPk_UserCard(),tabCardInfo.getUc_InternalCardNumber(),
				tabCardInfo.getUc_ExternalCardNumber(),tabCardInfo.getUc_CompanyNumber(),
				tabCardInfo.getUc_UserId(),tabCardInfo.getUc_Status(),payMode
				);
	
		
		return card;
	}

	public static CardAuthResp checkUserCard(String epCode,String inCardNo,String userPasswordMd5)
	{
		CardAuthResp ret= new CardAuthResp();
	
		String uc_InternalCardNumber= inCardNo;
		
		logger.debug("checkUserCard,inCardNo:{},len:{}",inCardNo,inCardNo.length());
		List<TblChargeCard> cardList= DB.chargeCardDao.findCard(uc_InternalCardNumber);
		if(cardList==null || cardList.size()!=1)
		{
			logger.debug("cardList:{}",cardList);
			ret.setErrorCode(6);
			return ret;
		}
		
		TblChargeCard cardInDb =cardList.get(0);
		ret.setInnerNo(inCardNo);
		ret.setOuterNo(cardInDb.getUc_ExternalCardNumber());
		ret.setCardStatus((cardInDb.getUc_Status()==0)?1:2);
		if(cardInDb.getUc_Status()!=0)
		{
			ret.setCardStatus(2);
			ret.setErrorCode(EpConstant.E_CARD_LOSS);
			return ret;
		}
		ret.setCardStatus(1);
		
		//查用户
		int userId= cardInDb.getUc_UserId();
		logger.debug("userId:{}",userId);
		UserRealInfo u= findUserRealInfo(userId);
		
		if(u==null)
		{
			ret.setErrorCode(EpConstant.E_CARD_NOT_BIND_USER);
			return ret;
		}
		if(u.getStatus() !=1)//不是正常用户
		{
			ret.setErrorCode(EpConstant.E_CARD_NO_RIGHT);
			return ret;
		}
		int remainAmt = u.getMoney().multiply(Global.DecTime2).intValue();
		ret.setRemainAmt(remainAmt);
		
		//查电桩
		ElectricPileCache epCache = EpService.getEpCache(epCode);
		if(epCache==null)
		{
			ret.setErrorCode(EpConstant.E_CARD_NOT_FIND_EP);
			return ret;
		}
		int error = EpService.checkEpCache(epCache);
 		if(error > 0)
 		{
 			ret.setErrorCode(error);
			return ret;
 		}
	
		if(u.getPassword().compareTo(userPasswordMd5)!=0)
		{
			ret.setErrorCode(EpConstant.E_CARD_AUTH_ERROR_PW);
			return ret;
		}
		if(cardInDb.getUc_CompanyNumber()==0)
		{
			if(epCache.getCompany_number()>0)
			{
				//普通卡不能刷带公司标识的桩
				ret.setErrorCode(EpConstant.E_CARD_NO_RIGHT);
				return ret;
			}
			else
			{
				ret.setIsFrozenAmt(1);
				
			}
		}
		else
		{
			if(epCache.getCompany_number() != cardInDb.getUc_CompanyNumber())
			{
				ret.setErrorCode(EpConstant.E_CARD_NO_RIGHT);
				return ret;
			}
			if(cardInDb.getUc_pay_mode()!= EpConstant.CARD_ABNORMAL_PRE_PAY && 
					cardInDb.getUc_pay_mode()!=EpConstant.CARD_ABNORMAL_FRONZE)
			{
				ret.setErrorCode(EpConstant.E_CARD_INVALID);
				return ret;
			}
			if(cardInDb.getUc_pay_mode() == 11)
			{
				ret.setIsFrozenAmt(1);
			
			}
			else
			{
				ret.setIsFrozenAmt(2);
				ret.setRemainAmt(0);
			}
			
		}
		return ret;
	}
	

	
	public static  UserCache getCardUser(String innerCardNo)
	{
		UserCache userCache =null;
		
		String uc_InternalCardNumber=innerCardNo;
		List<TblChargeCard> cardList= DB.chargeCardDao.findCard(uc_InternalCardNumber);
		if(cardList==null || cardList.size()!=1)
		{
			logger.info("cardList:{}",cardList);
			return userCache;
		}
		
		TblChargeCard cardInDb = cardList.get(0);
		ChargeCardCache cardCache = UserService.convertCardToCache(cardInDb);
	
		int userId= cardInDb.getUc_UserId();
		userCache=UserService.getUserCache(userId);
		
		if(userCache != null)
		{
			userCache.setCard(cardCache);
		}
		else
		{
			logger.error("not find card:{} user",innerCardNo);
			return null;
		}
		return userCache;
		
		
	}
	
	public static BigDecimal getRemainBalance(int accountId)
	{	
		UserRealInfo u = findUserRealInfo(accountId);
		//假设数据
		if(null != u)
		{
			return u.getMoney();
		}
		return new BigDecimal(0.0);
	}
	/***
	 * 
	 * @param Account
	 * @param passwordMd5
	 * @return 0:成功,1:没找到用户;2:账户被冻结;3:密码错误;4:桩没绑定费率;5:有未支付的订单;6:余额不够
	 */
	public static int checkUser(String epCode,String account, byte[] passwordMd5)
	{
		
		int ret=0;
		UserRealInfo  userRealInfo = findUserRealInfo(account);
		if(null == userRealInfo)
			return 1;
		if(1!= userRealInfo.getStatus())
		{
			return 2;
		}
		
		String sPw=new String(passwordMd5);
		if(sPw.compareTo(userRealInfo.getPassword())!=0 )
		{
			return 3;
		}
			
		RateInfoCache rateInfo = RateService.getRateInfo(epCode);
		
		if(rateInfo == null)
		{
			return 4;
		}
		int userId = userRealInfo.getId();
		int unpayNum = DB.chargeOrderDao.getUnpayOrder(userId);
		if(unpayNum>0)
		{
			return 5;		
		}
		BigDecimal minAmt = new BigDecimal(1.0); 
		if(userRealInfo.getMoney().compareTo( rateInfo.getRateInfo().getMinFreezingMoney())<0 ||userRealInfo.getMoney().compareTo(minAmt)<0)
		{
			return 6;
		}
		int num = DB.chargeOrderDao.getUnpayOrder(userId);
		if(num>0)
		{
			return 8;
		}
		
		return 0;
	
	}
	
	public static int insertPartnerUser(String account,int partnerId)
	{
		int norm_origin = 0;
		
		Map<String,Object> userMap =  new HashMap<String,Object> ();
		
		userMap.put("user_account",account);
		//userMap.put("norm_origin",partnerId);
		userMap.put("user_password",WmIce104Util.MD5Encode("123456".getBytes()));
		
		userMap.put("user_leval",6);
		userMap.put("user_status",1);
		
		int user_id = DB.userInfoDao.insertUser(userMap);
	    
		Map<String,Object> normalUser =  new HashMap<String,Object> ();
		normalUser.put("user_id",user_id);
		normalUser.put("norm_origin",norm_origin);
		DB.userInfoDao.insertNormalUser(normalUser);
		
		return 0;
	}
	
    public static  ChargeCardCache getChargeCardCache(int pkUserCard)
	 {
		 ChargeCardCache cardCache=null;
		 List<TblChargeCard> chargeCardList = DB.chargeCardDao.findCardById(pkUserCard);
		 if(chargeCardList!=null && chargeCardList.size()>=1)
		 {
	    	TblChargeCard cardInDb = chargeCardList.get(0);
			cardCache = UserService.convertCardToCache(cardInDb);
		 }
		 return cardCache;
	}
    
	public static UserRealInfo convertUsrRealInfo(TblUserInfo tblUserInfo)
	{
		if(tblUserInfo == null)
			return null;
		UserRealInfo u= new UserRealInfo();
		u.setAccount(tblUserInfo.getPhone());
		u.setId(tblUserInfo.getId());
		u.setPassword(tblUserInfo.getPassword());
		u.setStatus(tblUserInfo.getStatus());
		u.setDeviceid(tblUserInfo.getDeviceId());
		
		u.setMoney(tblUserInfo.getBalance());
		
		return u;
	}
    
    public static UserRealInfo findUserRealInfo(int userId)
    {
    	UserRealInfo userInfo=null;
    	
    	List<TblUserInfo> userInfoList=DB.userInfoDao.findUserInfoById(userId);
		if(null != userInfoList && userInfoList.size()!=1)
		{
			return userInfo;
		}
		
		TblUserInfo dbUser = userInfoList.get(0);
		
		return convertUsrRealInfo(dbUser);
    }
    public static UserRealInfo findUserRealInfo(String userAccount)
    {
    	UserRealInfo userInfo=null;
    	
    	List<TblUserInfo> userInfoList = DB.userInfoDao.findUserInfoByPhone(userAccount);
		if(null != userInfoList && userInfoList.size()!=1)
		{
			return userInfo;
		}
		
		TblUserInfo dbUser = userInfoList.get(0);
		
		return convertUsrRealInfo(dbUser);
    }
    
    public static int checkUserRealInfo(int userId)
    {
    	List<TblUserInfo> userInfoList=DB.userInfoDao.findUserInfoById(userId);
		if(null != userInfoList && userInfoList.size()!=1)
		{
			return EpConstantErrorCode.INVALID_ACCOUNT;
		}
		
		TblUserInfo dbUser = userInfoList.get(0);
		if (dbUser.getStatus() != 1) {
			return EpConstantErrorCode.INVALID_ACCOUNT_STATUS;
		}
		
		return 0;
    }
}
