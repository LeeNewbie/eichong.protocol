package com.webgate.service;

import java.math.BigDecimal;
import java.util.List;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.ormcore.dao.DB;
import com.ormcore.model.RateInfo;
import com.ormcore.model.TblBespoke;
import com.ormcore.model.TblChargingrecord;
import com.ormcore.model.TblElectricPileGun;
import com.webgate.cache.ChargeCache;
import com.webgate.cache.ElectricPileCache;
import com.webgate.cache.EpGunCache;
import com.webgate.cache.UserCache;
import com.webgate.cache.UserOrigin;
import com.webgate.cache.UserRealInfo;
import com.webgate.config.Global;
import com.webgate.constant.EpConstant;
import com.webgate.constant.EpConstantErrorCode;
import com.webgate.net.client.EpGateNetConnect;

public class EpChargeService {
	
	private static final Logger logger = LoggerFactory.getLogger(EpChargeService.class);
	
	private static final String ChargeYearDateFmt = "yyyy-MM-dd HH:mm:ss";
	private static final String ChargeDayShortDateFmt = "MM-dd HH:mm";
	
	public static boolean isValidAddress(int address,int type)
	{
		//logger.debug("into isValidAddress address:{},type:{}",address,type);
		boolean ret= true;
		switch(type)
		{
		case 1:
		{
			if(address>=13 && address <=16)//保留
			{
				ret= false;
			}
			else if(address> 21)//保留
			{
				ret= false;
			}
		}
		break;
		case 3:
		{
			if(address>=5 && address <=8)//保留
			{
				ret= false;
			}
			else if(address> 16)//保留
			{
				ret= false;
			}
		}
		break;
		case 11:
		{
			if(address>8 && address <=16)//保留
			{
				ret= false;
			}
			else if(address>=37 && address <=40)//保留
			{
				ret = false;
			}
			else if(address>=50 && address <=128)//保留
			{
				ret = false;
			}
			
			
		}
		break;
		case 132:
		{
			if(address>=5 && address <=8)//保留
			{
				ret= false;
			}
			else if(address>=11)//保留
			{
				 ret= false;
			}
		}
		break;
		default:
			ret= false;
			break;
		}
		
		//logger.debug("isValidAddress ret:{}",ret);
		return ret;
		
	}
	
	
	public static ChargeCache convertFromDb(TblChargingrecord tblChargeRecord)
	{
		if(tblChargeRecord ==null)
			return null;
		ChargeCache chargeCache = new ChargeCache();
	
		chargeCache.setSt(tblChargeRecord.getChreStartdate().getTime()/1000);
		
		chargeCache.setStatus(tblChargeRecord.getStatus());
		
		
		chargeCache.setAccount(tblChargeRecord.getUserPhone());
		chargeCache.setBespNo(tblChargeRecord.getChreBeginshowsnumber());
		chargeCache.setChargeSerialNo(tblChargeRecord.getChreTransactionnumber());
		
		chargeCache.setChOrCode(tblChargeRecord.getChreCode());
		
		chargeCache.setUserId(tblChargeRecord.getUserId());
		chargeCache.setPkUserCard(tblChargeRecord.getPkUserCard());
		if(chargeCache.getPkUserCard() !=0)
		{
			chargeCache.setStartChargeStyle((short)EpConstant.CHARGE_TYPE_CARD);
		}
		else
		{
			chargeCache.setStartChargeStyle((short)EpConstant.CHARGE_TYPE_QRCODE);
		}
		
		
		BigDecimal value= tblChargeRecord.getFrozenAmt().multiply(Global.DecTime2);
		chargeCache.setFronzeAmt(value.intValue());
		chargeCache.setPayMode(tblChargeRecord.getPayMode());
		
		UserOrigin userOrigin = new UserOrigin(tblChargeRecord.getUserOrgNo(),1,"");
		chargeCache.setUserOrigin(userOrigin);
		
		RateInfo rateInfo = new RateInfo();
		rateInfo.setJ_Rate(tblChargeRecord.getJPrice());
		rateInfo.setF_Rate(tblChargeRecord.getFPrice());
		rateInfo.setP_Rate(tblChargeRecord.getPPrice());
		rateInfo.setG_Rate(tblChargeRecord.getGPrice());
		rateInfo.setG_Rate(tblChargeRecord.getGPrice());
		rateInfo.setQuantumDate(tblChargeRecord.getQuantumDate());
		
		chargeCache.setRateInfo(rateInfo);
		
		return chargeCache;
	}
	
	public static ChargeCache GetUnFinishChargeCache(String epCode,int epGunNo)
	{
		TblChargingrecord tblQueryChargeRecord= new TblChargingrecord();
		tblQueryChargeRecord.setChreUsingmachinecode(epCode);
		
		tblQueryChargeRecord.setChreChargingnumber(epGunNo);
		
		List<TblChargingrecord> chargeList = DB.chargingrecordDao.getUnFinishedCharge(tblQueryChargeRecord);
		logger.debug("chargeList count:{}",chargeList.size());
		TblChargingrecord tblChargeRecord=null;
		if (chargeList != null && chargeList.size() > 0) {
			tblChargeRecord = chargeList.get(0);
		}
		
		if(tblChargeRecord==null)
			return null;
		
		return convertFromDb(tblChargeRecord);
	}
	
	public static int getChargeOrderStatus(String serialNo)
	{
		String ret = DB.chargeOrderDao.getOrderStatus(serialNo);
		return (ret==null)?-1:Integer.parseInt(ret);
	}
	
	public static ChargeCache GetChargeCacheFromDb(String serialNo)
	{
		TblChargingrecord tblChargeRecord=null;
		String chreTransactionnumber = serialNo;
		List<TblChargingrecord> chargeList = DB.chargingrecordDao.getByTranNumber(chreTransactionnumber);
		logger.debug("chargeList count:{}",chargeList.size());
		if (chargeList != null && chargeList.size() > 0) {
			tblChargeRecord = chargeList.get(0);
		}
		
		if(tblChargeRecord==null)
			return null;
		
		return convertFromDb(tblChargeRecord);
	}
	
	/**
	 * 参数检查
	 * @param epCode
	 * @param account
	 * @return
	 */
	private static int checkElectricParams(String epCode, String account)
	{
		//1.电桩编号长度不对，不能充电
		if(epCode.length() !=16)
		{
			return EpConstantErrorCode.INVALID_EP_CODE;
		}
		//2.用户长度不能充电
		int lenOfAccount=account.length();
		if(lenOfAccount != EpConstant.LEN_ACCOUNT &&  lenOfAccount != EpConstant.LEN_BIG_ACCOUNT)
		{
			logger.info("apiStartElectric,account:{}",account);
			return EpConstantErrorCode.INVALID_ACCOUNT;
		}
		
		return 0;
	}
	
	/**
	 * 参数检查
	 * @param epCode
	 * @param account
	 * @return
	 */
	private static int checkElectricDb(String epCode, int epGunNo, String account, short startChargeStyle)
	{
		//自己的用户需要校验用户的状态,因为用户可能被冻结
		UserRealInfo userRealInfo = UserService.findUserRealInfo(account);
		if(userRealInfo==null)
		{
			logger.info("apiStartElectric,account:{}",account);
			return EpConstantErrorCode.INVALID_ACCOUNT;
		}
		int accountId = userRealInfo.getId();
		
		if(userRealInfo.getStatus() != 1)
		{
			logger.info("apiStartElectric,dbUser.getStatus():{}",userRealInfo.getStatus());
			return EpConstantErrorCode.INVALID_ACCOUNT;
		}
		
		// 取最新的预约中的预约记录
		/*List<TblBespoke> bespList = DB.bespDao.getBespByUser(accountId);
		if (bespList == null || bespList.size() == 0) {
			return EpConstantErrorCode.NOT_HAVE_BESP;
		}*/
		// 取最新的未完成的充电记录
		List<TblChargingrecord> chargeList = DB.chargingrecordDao.getChargeByUser(accountId);
		if(chargeList != null && chargeList.size() > 0)
		{
			return EpConstantErrorCode.EPE_REPEAT_CHARGE;
		}

		//检查电桩
		ElectricPileCache epCache = EpService.getEpCache(epCode);
		if (epCache == null) return EpConstantErrorCode.EP_UNCONNECTED;
		int error = EpService.checkEpCache(epCache);
		if (error > 0) return error;
		EpGateNetConnect commClient = CacheService.getEpGate(epCache.getGateid());
		error = EpService.checkEpGate(commClient);
		if (error > 0) return error;

		if(epGunNo<1|| epGunNo> epCache.getGunNum())
		{
			return EpConstantErrorCode.INVALID_EP_GUN_NO;
		}
		int currentType = epCache.getCurrentType();
		if(currentType!=EpConstant.EP_DC_TYPE && currentType!= EpConstant.EP_AC_TYPE)
		{
			logger.error("init error!invalid current type:{}",currentType);
			return EpConstantErrorCode.EPE_CHARGE_STYLE;
		}

		TblElectricPileGun tblEpGun= EpGunService.getDbEpGun(epCache.getPkEpId(),epGunNo);
		if(tblEpGun==null)
		{
			logger.error("init error!did not find gun,pkEpId:{},epGunNo:{}",epCache.getPkEpId(),epGunNo);
			return EpConstantErrorCode.INVALID_EP_GUN_NO;
		}
		if(tblEpGun.getEpState() ==  EpConstant.EP_GUN_STATUS_SETTING)
		{
			return EpConstantErrorCode.EPE_IN_EP_OPER;//
		}
		if(tblEpGun.getEpState() == EpConstant.EP_GUN_STATUS_EP_OPER && startChargeStyle == EpConstant.CHARGE_TYPE_QRCODE)
		{
			logger.debug("epGunCache.getStatus():{},startChargeStyle:{}",tblEpGun.getEpState(),startChargeStyle);
			return EpConstantErrorCode.EPE_IN_EP_OPER;//
		}
		if(tblEpGun.getEpState() ==  EpConstant.EP_GUN_STATUS_EP_UPGRADE)
		{
			return EpConstantErrorCode.EP_UPDATE;//
		}
		if(tblEpGun.getEpState()>30)
		{
			return EpConstantErrorCode.EPE_GUN_FAULT;//
		}
		//2.取最新的预约中的预约记录
		TblBespoke besp = EpBespokeService.getUnStopBespokeFromDb(epCache.getPkEpId(), tblEpGun.getPkEpGunId());
		if (besp != null && besp.getUserid() != accountId) {
			return EpConstantErrorCode.EPE_OTHER_BESP;
		}
		//3.取最新的未完成的充电记录
		ChargeCache tmpChargeCache=EpChargeService.GetUnFinishChargeCache(epCode, epGunNo);
		if(tmpChargeCache!=null)
		{
			return EpConstantErrorCode.USED_GUN;
		}

		return 0;
	}

	/**
	 * api开始充电
	 * @param epCode
	 * @param epGunNo
	 * @param accountId
	 * @param account
	 * @param bespNo
	 * @param ermFlag
	 * @param appClientIp
	 * @param frozenAmt
	 * @param source,但来自于爱充的用户需要收费，来自于其他合作伙伴有可能不冻结钱.只记录充电和消费记录
	 * @return
	 */
	public static int apiStartElectric(String epCode,int epGunNo,String account,String bespNo,
			short startChargeStyle,int pkUserCard,
			int frozenAmt,int payMode,
			int orgNo,int fromSource,String actionIdentity)
	{
		int error = checkElectricParams(epCode, account);
		if (error > 0) return error;
		
		error = checkElectricDb(epCode, epGunNo, account, startChargeStyle);
		if (error > 0) return error;

		UserCache memUserInfo= UserService.getUserCache(account);
		if (memUserInfo == null) return EpConstantErrorCode.INVALID_ACCOUNT;

		EpGunCache epGunCache = CacheService.getEpGunCache(epCode, epGunNo);
		int errorCode = epGunCache.startChargeAction(epCode, epGunNo, memUserInfo.getId(), bespNo, startChargeStyle, frozenAmt,
				payMode, orgNo,fromSource,actionIdentity);
		
		if (errorCode>0) return errorCode;
		memUserInfo.setUseGun(epCode + epGunNo);
		//epGunCache.getChargeCache().setPkUserCard(pkUserCard);
		
		return 0;
	}
	public static int apiStopElectric(String epCode,int epGunNo,int userId,int source,String apiClientIp)
	{
		if(epCode.length() !=16)
		{
			return EpConstantErrorCode.INVALID_EP_CODE;
		}
		
		//检查电桩
		ElectricPileCache epCache = CacheService.getEpCache(epCode);
		if(epCache == null)
		{
			return EpConstantErrorCode.EP_UNCONNECTED;
		}

		EpGateNetConnect commClient = CacheService.getEpGate(epCache.getGateid());
		int error = EpService.checkEpGate(commClient);
		if (error > 0) return error;
		
		if(epGunNo<1|| epGunNo> epCache.getGunNum())
		{
			return EpConstantErrorCode.INVALID_EP_GUN_NO;
		}
		
		EpGunCache epGunCache= CacheService.getEpGunCache(epCode, epGunNo);
		//桩断线，不能结束充电
 		if(epGunCache ==null )
		{
			
			return EpConstantErrorCode.EP_UNCONNECTED;
		}
		
		return epGunCache.stopChargeAction(userId,commClient);
	}
	
}
	
