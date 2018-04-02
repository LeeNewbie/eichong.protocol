package com.epcentre.service;

import java.io.IOException;
import java.math.BigDecimal;
import java.text.MessageFormat;
import java.util.Date;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.Random;

import net.sf.json.JSONObject;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.epcentre.cache.AuthUserCache;
import com.epcentre.cache.ChargeCache;
import com.epcentre.cache.ChargeCarInfo;
import com.epcentre.cache.ChargeCardCache;
import com.epcentre.cache.ChargePowerModInfo;
import com.epcentre.cache.ElectricPileCache;
import com.epcentre.cache.EpCommClient;
import com.epcentre.cache.EpGunCache;
import com.epcentre.cache.UserCache;
import com.epcentre.cache.UserOrigin;
import com.epcentre.cache.UserRealInfo;
import com.epcentre.config.GameConfig;
import com.epcentre.config.Global;
import com.epcentre.constant.ChargeStatus;
import com.epcentre.constant.EpConstant;
import com.epcentre.constant.EpConstantErrorCode;
import com.epcentre.constant.EpProtocolConstant;
import com.epcentre.constant.UserConstant;
import com.epcentre.dao.DB;
import com.epcentre.model.RateInfo;
import com.epcentre.model.TblCarVin;
import com.epcentre.model.TblChargingOrder;
import com.epcentre.model.TblChargingfaultrecord;
import com.epcentre.model.TblChargingrecord;
import com.epcentre.model.TblCoupon;
import com.epcentre.model.TblElectricPileGun;
import com.epcentre.model.TblJpush;
import com.epcentre.model.TblPowerModule;
import com.epcentre.model.TblVehicleBattery;
import com.epcentre.net.message.AliSMS;
import com.epcentre.net.message.JPushUtil;
import com.epcentre.net.message.MobiCommon;
import com.epcentre.protocol.ChargeCmdResp;
import com.epcentre.protocol.ChargeEvent;
import com.epcentre.protocol.EpEncodeProtocol;
import com.epcentre.protocol.Iec104Constant;
import com.epcentre.protocol.NoCardConsumeRecord;
import com.epcentre.protocol.UtilProtocol;
import com.epcentre.protocol.WmIce104Util;
import com.epcentre.sender.EpMessageSender;
import com.epcentre.test.ImitateEpService;
import com.epcentre.utils.DateUtil;
import com.epcentre.utils.StringUtil;

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
	
	
	public static ChargeCache convertFromDb(TblChargingrecord tcr)
	{
		if(tcr ==null)
			return null;
		
		ChargeCache chargeCache = new ChargeCache();
	
		chargeCache.setSt(tcr.getChreStartdate().getTime()/1000);
		chargeCache.setStatus(tcr.getStatus());
		chargeCache.setAccount(tcr.getUserPhone());
		chargeCache.setBespNo(tcr.getChreBeginshowsnumber());
		chargeCache.setChargeSerialNo(tcr.getChreTransactionnumber());
		
		chargeCache.setChOrCode(tcr.getChreCode());
		
		chargeCache.setUserId(tcr.getUserId());
		chargeCache.setThirdUsrIdentity(tcr.getThirdUsrIdentity());
		//怎么加载userLevel
		if(tcr.getPkUserCard()>0)
		{
			ChargeCardCache cardCache=UserService.getChargeCardCache(tcr.getPkUserCard());
			chargeCache.setCard(cardCache);
			chargeCache.setStartChargeStyle((short)EpConstant.CHARGE_TYPE_CARD);
		}
		else{
			chargeCache.setStartChargeStyle((short)EpConstant.CHARGE_TYPE_QRCODE);
		}
		
		
		BigDecimal value= tcr.getFrozenAmt().multiply(Global.DecTime2);
		chargeCache.setFronzeAmt(value.intValue());
		chargeCache.setPayMode(tcr.getPayMode());
		
		UserOrigin userOrigin = new UserOrigin(tcr.getUserOrgNo(),2,"");
		chargeCache.setUserOrigin(userOrigin);
		
		RateInfo rateInfo = new RateInfo();
		rateInfo.setJ_Rate(tcr.getJPrice());
		rateInfo.setF_Rate(tcr.getFPrice());
		rateInfo.setP_Rate(tcr.getPPrice());
		rateInfo.setG_Rate(tcr.getGPrice());
		rateInfo.setServiceRate(tcr.getServicePrice());
		rateInfo.setQuantumDate(tcr.getQuantumDate());
		
		chargeCache.setRateInfo(rateInfo);
		
		
		String vinCode=null;
		int thirdCode=0;
		if(tcr.getThirdType()==ChargeStatus.CHARGEORDER_THIRDTYPE_VIN)
		{
			TblCarVin carVin=getCarVinById(tcr.getThirdCode());
		    if (carVin != null )
		    {
		    	vinCode=carVin.getVinCode();
		    	thirdCode = tcr.getThirdCode();
		    }
		}
		chargeCache.setCarVin(vinCode);
		chargeCache.setThirdCode(thirdCode);
		chargeCache.setThirdType(tcr.getThirdType());
		
		return chargeCache;
	}
	
	public static int getOrderId(String chorTransactionnumber)
	{
		List<TblChargingOrder> orderList = DB.chargeOrderDao.findOrderId(chorTransactionnumber);
		if(orderList==null || orderList.size()<=0)
			return 0;
		return Integer.parseInt(orderList.get(0).getPkChargingorder());
	}
	
	
	
	
	public static ChargeCache getUnFinishChargeFromDb(String epCode,int epGunNo)
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
	public static void getUnFinishChargeByUserIdFromDb(UserCache u)
	{
		TblChargingrecord tblQueryChargeRecord= new TblChargingrecord();
		tblQueryChargeRecord.setUserId(u.getId());
		
		List<TblChargingrecord> chargeList = DB.chargingrecordDao.getUnFinishedChargeByUsrId(tblQueryChargeRecord);

		if (chargeList == null)
			return ;
		int size = chargeList.size();
		logger.debug("chargeList count:{}",size);
		if(size<=0)
			return ;
		for(int i=0;i<size;i++)
		{
			TblChargingrecord tblChargeRecord = chargeList.get(i);
			ChargeCache cache = convertFromDb(tblChargeRecord);
			u.addCharge(cache);
		}
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
	public static int checkChargeParams(String epCode,String usrAccount)
	{
		//1.电桩编号长度不对，不能充电
		if(epCode==null ||epCode.length() !=16)
		{
			return EpConstantErrorCode.INVALID_EP_CODE;
		}
		if(usrAccount==null)
		{
			return EpConstantErrorCode.INVALID_ACCOUNT;
		}
		//2.用户长度不能充电
		int lenOfAccount=usrAccount.length();
		if(lenOfAccount != EpProtocolConstant.LEN_ACCOUNT &&  lenOfAccount != EpProtocolConstant.LEN_BIG_ACCOUNT)
		{
			return EpConstantErrorCode.INVALID_ACCOUNT;
		}
		return 0;
		
	}
	public static int appApiStartElectric(String epCode,int epGunNo,String bespNo,
			int accountId,String account,short ermFlag,
			double fronzeAmt ,int orgNo,int payMode,String clientIp)
	{
		logger.info("appApiStartElectric charge accept orgNo:{},accountId:{},account:{},chargeStyle:{},epCode:{},epGunNo:{},fronzeAmt:{} from api",
				new Object[]{orgNo,accountId,account,ermFlag,epCode,epGunNo,fronzeAmt});
		
		if(orgNo!=UserConstant.ORG_I_CHARGE && payMode==EpConstant.P_M_FIRST)
			return  EpConstantErrorCode.INVALID_ACCOUNT;
		UserCache u=null;
		
		if(orgNo !=UserConstant.ORG_I_CHARGE)
		{
			int user_id=UserService.getUserIdByOrgNo(orgNo);
			
			if(user_id>0)
			{
				u= UserService.getUserCache(user_id);
				accountId= user_id;
			}
			else
			{
				return EpConstantErrorCode.INVALID_ACCOUNT;
			
			}
		}
		else
		{
			u = UserService.getMemUser(accountId,account);
		}
		
		if(u==null)
		{
			return EpConstantErrorCode.INVALID_ACCOUNT;
		}
		
		BigDecimal bdFronzeAmt = new BigDecimal(fronzeAmt);
		bdFronzeAmt = bdFronzeAmt.multiply(Global.DecTime2);
		int nFronzeamt = UtilProtocol.BigDecimal2ToInt(bdFronzeAmt);
		
		byte[] cmdTimes = WmIce104Util.timeToByte();
		
		logger.info("appApiStartElectric charge accept orgNo:{},accountId:{},account:{},chargeStyle:{},epCode:{},epGunNo:{},fronzeAmt:{},nFronzeamt:{} from api",
				new Object[]{orgNo,accountId,account,ermFlag,epCode,epGunNo,fronzeAmt,nFronzeamt});
		
		
		return EpChargeService.apiStartElectric(epCode,epGunNo,u,account,null,bespNo,ermFlag,
				nFronzeamt,payMode,orgNo,1,clientIp,cmdTimes);
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
	public static int apiStartElectric(String epCode,int epGunNo,UserCache u,String thirdUsrIdentity,ChargeCardCache card,String bespNo,
			short startChargeStyle,
			int frozenAmt,int payMode,
			int orgNo,int fromSource,String actionIdentity,byte [] cmdTimes)
	{
		if(u==null)
		{
			return EpConstantErrorCode.INVALID_ACCOUNT;
		}
		
		int errorCode = checkChargeParams(epCode,u.getAccount());
		if(errorCode>0)
			return errorCode;
		
		//检查电桩
		ElectricPileCache epCache =  EpService.getEpByCode(epCode);
		if(epCache == null)
		{
			logger.error("charge apiStartElectric fail,dont find ElectricPileCache,epCode:{}",epCode);
			return EpConstantErrorCode.EP_UNCONNECTED;
		}
		int pkUserCard = (card!=null)? card.getId():0;
		
		boolean allowMutliCharge = (card!=null)? card.isAllowMutliCharge():false;
		int cardType= (card!=null)? card.getCardType():0;
		int cardOrgNo= (card!=null)? card.getCompanyNumber():0;
		int error = epCache.canCharge(true,pkUserCard,cardType,cardOrgNo,epGunNo);
		if(error>0)
		{
			logger.error("charge apiStartElectric fail, canCharge,epCode:{},error",epCode,error);
			
			return error;
		}
		
				
		int rateInfoId = epCache.getRateid();
		
		RateInfo rateInfo= RateService.getRateById(rateInfoId);
		if(rateInfo == null)
		{
			logger.error("charge apiStartElectric fail,rateInfo is null,epCode:{},rateInfoId:{}",epCode,rateInfoId);
			return EpConstantErrorCode.INVALID_RATE;
		}
		
		String epChargeGun = epCode + epGunNo;
		//所有合作商户在我们这儿都有用户体系，都需要做判断
		UserRealInfo userRealInfo = UserService.findUserRealInfo(u.getId());
		
		if(userRealInfo==null)
		{
			logger.error("charge apiStartElectric fail,invalid account,epCode:{},accountId:{}",epCode,u.getId());
			return EpConstantErrorCode.INVALID_ACCOUNT;
		}
		
		errorCode = userRealInfo.canCharge();
		
		if(errorCode>0)
		{
			return errorCode;
		}
		
		errorCode = u.canCharge(epChargeGun,u.getId(),orgNo,thirdUsrIdentity,pkUserCard,allowMutliCharge);
		
		if(errorCode>0)
		{
			return errorCode;
		}
		
		
		EpGunCache epGunCache = EpGunService.getEpGunCache(epCode, epGunNo);
		
		errorCode = epGunCache.canCharge(startChargeStyle,u.getId(),true);
		if(errorCode>0)
		{
			return errorCode;
		}
		
		errorCode = epGunCache.startChargeAction(u, thirdUsrIdentity,card,rateInfo, bespNo, startChargeStyle, frozenAmt,
				payMode, orgNo,fromSource,actionIdentity,cmdTimes);
		
		
		return errorCode;
	}
	public static int appApiStopElectric(String epCode,int epGunNo,int orgNo,int userId,String account,int source,String apiClientIp)
	{
		
		if(orgNo !=UserConstant.ORG_I_CHARGE)
		{
			userId=UserService.getUserIdByOrgNo(orgNo);
			if(userId==0)
			{
				return EpConstantErrorCode.EP_UNCONNECTED;
			}
		}
		return apiStopElectric(epCode, epGunNo,orgNo, userId,account,source,apiClientIp);
		
	}
	/**
	 * 
	 * @param epCode
	 * @param epGunNo
	 * @param orgNo
	 * @param userId
	 * @param account，当orgNo!=UserConstant.ORG_I_CHARGE时，account为第三方用户标识
	 * @param source
	 * @param apiClientIp
	 * @return
	 */
	public static int apiStopElectric(String epCode,int epGunNo,int orgNo,int userId,String account,int source,String apiClientIp)
	{
		if(epCode.length() !=16)
		{
			return EpConstantErrorCode.INVALID_EP_CODE;//
		}
		
		//检查电桩
		ElectricPileCache epCache =  EpService.getEpByCode(epCode);
		if(epCache == null)
		{
			logger.error("charge fail,dont find ElectricPileCache:{}",epCode);
			return EpConstantErrorCode.EP_UNCONNECTED;
		}
		
		
		if(epGunNo<1|| epGunNo> epCache.getGunNum())
		{
			return EpConstantErrorCode.INVALID_EP_GUN_NO;//
		}
		
		EpGunCache epGunCache= EpGunService.getEpGunCache(epCode, epGunNo);
		//桩断线，不能结束充电
 		if(epGunCache ==null )
		{
			return EpConstantErrorCode.EP_UNCONNECTED;//
		}
		
		return epGunCache.stopChargeAction(orgNo,userId,account,source,apiClientIp);
	}

	public static void handEpStartChargeResp(EpCommClient epCommClient,ChargeCmdResp chargeCmdResp,byte []cmdTimes )
	{
		logger.info("enter handEpStartchargeResp,epCode:{},epGunNo:{},cmdTime:{},ChargeCmdResp:{}",
				new Object[]{chargeCmdResp.getEpCode(),chargeCmdResp.getEpGunNo(),cmdTimes,chargeCmdResp});
	
		EpGunCache epGunCache = EpGunService.getEpGunCache(chargeCmdResp.getEpCode(),chargeCmdResp.getEpGunNo());
		
		if(epGunCache!=null)
		{
			epGunCache.onEpStartCharge(chargeCmdResp);
		}
	}
	/**
	 * 
	 * @param chargeEvent
	 * @return 4:参数错误
	 */
	private static int checkRespChargeEventParams(ChargeEvent chargeEvent)
	{
		if(chargeEvent==null)
		{
			logger.info("checkRespChargeEventParams chargeEvent==null");
			return 4;
		}
		String epCode = chargeEvent.getEpCode();
		if(epCode==null || epCode.length()!=16)
		{
			logger.info("checkRespChargeEventParams invalid epCode:{}",epCode);
			return 4;
		}
		int epGunNo = chargeEvent.getEpGunNo();
		
		if(chargeEvent.getSuccessFlag()!=0&&  chargeEvent.getSuccessFlag()!=1)
		{
			logger.info("checkRespChargeEventParams invalid successFlag:{}",chargeEvent.getSuccessFlag());
			return 4;
		}
		
        String serialNo = chargeEvent.getSerialNo();		
		if(serialNo==null || serialNo.length()!=32)
		{
			logger.info("checkRespChargeEventParams not find serialno:{}",serialNo);
			return 5;
		}
		String zeroSerialNo= StringUtil.repeat("0", 32);
		if( serialNo.compareTo(zeroSerialNo)==0)
		{
			 logger.error("checkRespChargeEventParams invalid SerialNo:{}",serialNo);
			 return 6;
		 }

		EpGunCache epGunCache = EpGunService.getEpGunCache(epCode,epGunNo);
		if(epGunCache == null)
		{
			logger.info("checkRespChargeEventParams not find epGunCache,epCode:{},epGunNo:{}",epCode,epGunNo);
			return 7;
		}
		return 0;
		
	}
	public static  int  handleStartElectricizeEventV3(EpCommClient epCommClient,ChargeEvent chargeEvent,byte []cdmTimes) 
	{
		logger.info("enter onEpStartChargeEvent,chargeEvent:{}",chargeEvent);
		
		String retEpCode= null;
		int retEpGunNo = 0;
		String serialNo = null;
		
		//1.检查充电时间参数
		int retCode = checkRespChargeEventParams(chargeEvent);
		if(retCode==0)
		{
			 retEpCode= chargeEvent.getEpCode();
			 retEpGunNo = chargeEvent.getEpGunNo();
			 serialNo = chargeEvent.getSerialNo();
			
			EpGunCache epGunCache = EpGunService.getEpGunCache(retEpCode,retEpGunNo);
		
			retCode = epGunCache.handleStartChargeEvent(chargeEvent);
		}
		else
		{
			logger.error("handle charge event fail,retCode:{}",retCode);
			
			if(chargeEvent.getEpCode() !=null && chargeEvent.getEpCode().length()==16)
			{
				retEpCode= chargeEvent.getEpCode();
			}
			else
			{
				retEpCode = StringUtil.repeat("0", 16);
			}
			if(retCode==5)
			{
				serialNo = StringUtil.repeat("0", 32);
			}
			else
			{
				serialNo = chargeEvent.getSerialNo();
			}
			
		}
		
		if(!ImitateEpService.IsStart() )
		{
			byte[] confirmdata = EpEncodeProtocol.do_charge_event_confirm(chargeEvent.getEpCode(),chargeEvent.getEpGunNo(),chargeEvent.getSerialNo(),retCode);
				
			if(confirmdata==null)
			{
				logger.error("handleStartElectricizeEventV3 do_charge_event_confirm exception");
				return retCode;
			}
			
			EpMessageSender.sendMessage(epCommClient, 0,0,(byte)Iec104Constant.C_CHARGE_EVENT_CONFIRM,confirmdata,cdmTimes,epCommClient.getVersion());
			
			logger.info("charge event confirm to ep retCode:{},serialNo:{},epCode:{},epGunNo:{}",
					new Object[]{retCode,chargeEvent.getSerialNo(),chargeEvent.getEpCode(),chargeEvent.getEpGunNo()});
		}
		return retCode;
		
	}
	public static void updateBeginRecordToDb(int chargeUserId,int chorType,
			String chargeAccount,int pkUserCard,int userOrigin,int pkEpId,String epCode,
			int epGunNo,int chargingmethod,String bespokeNo,String chOrCode, int frozenAmt,long st,
			String chargeSerialNo,int startMeterNum,RateInfo rateInfo,int status,int thirdCode,int thirdType)
	{
		TblChargingrecord record = new TblChargingrecord();
		
		BigDecimal bdMeterNum = UtilProtocol.intToBigDecimal3(startMeterNum);
		// 开始充电表低示数
		String beginShowsNumber = String.valueOf(bdMeterNum);
		record.setChreBeginshowsnumber(beginShowsNumber);
		
		// 充电开始时间
		Date startDate = DateUtil.toDate(st * 1000);
		record.setChreStartdate(startDate);
		record.setChreEnddate(startDate);
		record.setChreUsingmachinecode(epCode);
		record.setChreChargingnumber(epGunNo);
		record.setChreReservationnumber(bespokeNo);
		record.setChreResttime(0);
		record.setChreTransactionnumber(chargeSerialNo);
	    record.setUserId(chargeUserId);
	    record.setUserPhone(chargeAccount);
		record.setChreElectricpileid(pkEpId);
	
		record.setChreChargingmethod(chargingmethod);
		
		record.setChreEndshowsnumber("0");
		record.setChreCode(chOrCode);
		record.setStatus(status);
		record.setJPrice(rateInfo.getJ_Rate());
		record.setFPrice(rateInfo.getF_Rate());
		record.setPPrice(rateInfo.getP_Rate());
		record.setGPrice(rateInfo.getG_Rate());
		record.setQuantumDate(rateInfo.getQuantumDate());
		record.setPkUserCard(pkUserCard);
		record.setUserOrigin(userOrigin);
		BigDecimal bdFrozenAmt = UtilProtocol.intToBigDecimal2(frozenAmt);
		
		record.setFrozenAmt(bdFrozenAmt);
		
		record.setThirdCode(thirdCode);
		record.setThirdType(thirdType);
		// 新增开始充电记录
		DB.chargingrecordDao.updateBeginRecord(record);
	}
	
	public static void updateFailRecordToDb(String serialNo,int status)
	{
		TblChargingrecord record = new TblChargingrecord();
	
		record.setChreTransactionnumber(serialNo);
		record.setStatus(status);
		// 新增开始充电记录
		DB.chargingrecordDao.updateFailChargeRecord(record);
	}
	
	public static void insertChargeRecordToDb(int chargeUserId,int chorType,
			String chargeAccount,int pkUserCard,int userOrigin,int pkEpId,String epCode,
			int epGunNo,int chargingmethod,String bespokeNo,String chOrCode, int frozenAmt,int payMode,int userOrgNo,
			ChargeEvent chargeEvent
			, RateInfo rateInfo,int status,String thirdUsrIdentity)
	{
		TblChargingrecord record = new TblChargingrecord();
		
		BigDecimal bdMeterNum = UtilProtocol.intToBigDecimal3(chargeEvent.getStartMeterNum());
		// 开始充电表低示数
		String beginShowsNumber = String.valueOf(bdMeterNum);
		record.setChreBeginshowsnumber(beginShowsNumber);
		
		// 充电开始时间
		Date startDate = DateUtil.toDate(chargeEvent.getStartChargeTime() * 1000);
		record.setChreStartdate(startDate);
		record.setChreEnddate(startDate);
		record.setChreUsingmachinecode(chargeEvent.getEpCode());
		record.setChreChargingnumber(chargeEvent.getEpGunNo());
		record.setChreReservationnumber(chargeEvent.getBespokeNo());
		record.setChreResttime(chargeEvent.getRemainTime());
		record.setChreTransactionnumber(chargeEvent.getSerialNo());
        record.setUserId(chargeUserId);
	    record.setUserPhone(chargeAccount);
		record.setChreElectricpileid(pkEpId);
		
		record.setChreChargingmethod(chargingmethod);
		
		record.setChreEndshowsnumber("0");
		record.setChreCode(chOrCode);
	
		
		record.setStatus(status);
		record.setJPrice(rateInfo.getJ_Rate());
		record.setFPrice(rateInfo.getF_Rate());
		record.setPPrice(rateInfo.getP_Rate());
		record.setGPrice(rateInfo.getG_Rate());
		record.setServicePrice(rateInfo.getServiceRate());
		record.setQuantumDate(rateInfo.getQuantumDate());
		record.setPkUserCard(pkUserCard);
		record.setUserOrigin(0);
		
		BigDecimal bdFrozenAmt = UtilProtocol.intToBigDecimal2(frozenAmt);
		record.setFrozenAmt(bdFrozenAmt);
		record.setUserOrgNo(userOrgNo);
		record.setPayMode(payMode);
		record.setThirdUsrIdentity(thirdUsrIdentity);
		// 新增开始充电记录
		DB.chargingrecordDao.insertBeginRecord(record);
		
	}
	public static int insertChargeOrderToDb(int chargeUserId,int chorType,
			int pkUserCard,int userOrigin,int pkEpId,String epCode,
			int epGunNo,int chargingmethod,String bespokeNo,String chOrCode, int frozenAmt,
			int payMode,int userOrgNo,long st,String chargeSerialNo
			, RateInfo rateInfo,String thirdUsrIdentity)
	{
		//订单编号，赞借用流水号
		Date date = new Date();
		
		Date dtStart = new Date(st *1000);
		// 计算总电量
		String beginTime = DateUtil.toDateFormat(dtStart, "MM-dd HH:mm");
		
		BigDecimal  bdZero = new BigDecimal(0.0);
		
		BigDecimal  chargeAmt = new BigDecimal(0.0);
		BigDecimal  serviceAmt =new BigDecimal(0.0);
		// 充电消费订单
		 TblChargingOrder order = new TblChargingOrder();
		 order.setChorCode(chOrCode);
		
		order.setChorAppointmencode(bespokeNo);
		
		 order.setChorPilenumber(epCode);
		 order.setChorUserid(""+chargeUserId);
		
		 order.setChorType(chorType);
		 //order.setChorType(0);
		 
		 order.setChorMoeny(chargeAmt.add(serviceAmt) + "");
		 order.setChOr_tipPower(bdZero);
		 order.setChOr_peakPower(bdZero);
		 order.setChOr_usualPower(bdZero);
		 order.setChOr_valleyPower(bdZero);
		 order.setChorQuantityelectricity(bdZero );
		 order.setChorTimequantum(beginTime + " - ");
		 order.setChorMuzzle(epGunNo);
		 order.setChorChargingstatus(EpConstant.ORDER_STATUS_WAIT + "");
		 order.setChorTranstype("1");
		
		 order.setChorCreatedate(new Date());
		 order.setChorUpdatedate(new Date());
		 //order.setChorUsername(userInfo.getUsinFacticityname() == null ? "":userInfo.getUsinFacticityname());
		 order.setChorUsername("");
		 order.setChorTransactionnumber(chargeSerialNo);
		 order.setChorChargemoney(chargeAmt);
		 order.setChorServicemoney(serviceAmt);
		 order.setChorOrdertype(0);
		 beginTime = DateUtil.toDateFormat(dtStart, ChargeYearDateFmt);
		
		 order.setChargeBegintime(beginTime);
		 order.setChargeEndtime(beginTime);
		 order.setPkUserCard(pkUserCard);
		 order.setUserOrigin(0);
		 order.setUserOrgNo(userOrgNo);
		 //order.setPayMode(payMode);
		 order.setThirdUsrIdentity(thirdUsrIdentity);
		 // 新增充电消费订单
		 return DB.chargeOrderDao.insertChargeOrder(order);
	}
	public static String getMeterNum(int nMeterNum)
	{
		BigDecimal bdMeterNum = UtilProtocol.intToBigDecimal3(nMeterNum);

		String strMeterNum = String.valueOf(bdMeterNum);
		return strMeterNum;
	}
	
	public static void insertFullChargeOrder(int chargeUserId,int chorType,
			String chargeAccount,int pkUserCard,int userOrigin,int pkEpId,String epCode,int epGunNo,int chargingmethod,
			String bespokeNo,String chOrCode,int payMode,NoCardConsumeRecord consumeRecord
			, RateInfo rateInfo,int orderStatus)
	{
		// 尖时段用电度数
		BigDecimal tipPower = UtilProtocol.intToBigDecimal3(consumeRecord.getjDl());
		// 峰时段用电度数
		BigDecimal peakPower = UtilProtocol.intToBigDecimal3(consumeRecord.getfDl());
		// 平时段用电度数
		BigDecimal usualPower = UtilProtocol.intToBigDecimal3(consumeRecord.getpDl());
		// 谷时段用电度数
		BigDecimal valleyPower = UtilProtocol.intToBigDecimal3(consumeRecord.getgDl()); 
		// 时间段
		
		BigDecimal totalPower = UtilProtocol.intToBigDecimal3(consumeRecord.getTotalDl());
		
		
		Date dtStart = new Date(consumeRecord.getStartTime()*1000);
		Date dtEnd = new Date(consumeRecord.getEndTime()*1000);
		// 计算总电量
		String beginTime = DateUtil.toDateFormat(dtStart, "MM-dd HH:mm");
		String endTime = DateUtil.toDateFormat(dtEnd,"MM-dd HH:mm");
		
		
		BigDecimal  chargeAmt =  UtilProtocol.intToBigDecimal2(consumeRecord.getTotalChargeAmt());
		BigDecimal  serviceAmt = UtilProtocol.intToBigDecimal2(consumeRecord.getServiceAmt());
	    String thirdUsrIdentity="";
		if(userOrigin == EpConstant.CPYNUMBER_BQ)
		 {
			thirdUsrIdentity = consumeRecord.getEpUserAccount();
		 }
		// 充电消费订单
		 TblChargingOrder order = new TblChargingOrder();
		 order.setChorCode(chOrCode);
		
		order.setChorAppointmencode(bespokeNo);
		
		 order.setChorPilenumber(epCode);
		 order.setChorUserid(""+chargeUserId);
		
		 order.setChorType(chorType);
		 
		 order.setChorMoeny(chargeAmt.add(serviceAmt) + "");
		 order.setChOr_tipPower(tipPower);
		 order.setChOr_peakPower(peakPower);
		 order.setChOr_usualPower(usualPower);
		 order.setChOr_valleyPower(valleyPower);
		 order.setChorQuantityelectricity(totalPower );
		 order.setChorTimequantum(beginTime + " - " + endTime);
		 order.setChorMuzzle(epGunNo);
		 order.setChorChargingstatus(""+orderStatus);
		 order.setChorTranstype(""+consumeRecord.getTransType());
		
		 order.setChorCreatedate(new Date());
		 order.setChorUpdatedate(new Date());
		 order.setChorUsername("");
		 order.setChorTransactionnumber(consumeRecord.getSerialNo());
		 order.setChorChargemoney(chargeAmt);
		 order.setChorServicemoney(serviceAmt);
		 order.setChorOrdertype(0);
		 beginTime = DateUtil.toDateFormat(dtStart, ChargeYearDateFmt);
		 endTime = DateUtil.toDateFormat(dtEnd,ChargeYearDateFmt);
		
		 order.setChargeBegintime(beginTime);
		 order.setChargeEndtime(endTime);
		 order.setPkUserCard(pkUserCard);
		 order.setUserOrgNo(userOrigin);
		 order.setUserOrigin(userOrigin);
		 order.setThirdUsrIdentity(thirdUsrIdentity);
		 
		 // 新增充电消费订单
		 DB.chargeOrderDao.insertFullChargeOrder(order);
		
	}
	public static void insertFullChargeRecord(int chargeUserId,int chorType,
			String chargeAccount,int pkUserCard,int userOrigin,int pkEpId,String epCode,int epGunNo,int chargingmethod,
			String bespokeNo,String chOrCode,int payMode,NoCardConsumeRecord consumeRecord
			, RateInfo rateInfo)
	{
		Date dtStart = new Date(consumeRecord.getStartTime()*1000);
		Date dtEnd = new Date(consumeRecord.getEndTime()*1000);
		 String thirdUsrIdentity="";
		if(userOrigin == EpConstant.CPYNUMBER_BQ)
		 {
			thirdUsrIdentity = consumeRecord.getEpUserAccount();
		 }
		
		TblChargingrecord record = new TblChargingrecord();
		// 开始充电表低示数
		String beginShowsNumber = getMeterNum(consumeRecord.getStartMeterNum());
		String endShowsnumber = getMeterNum(consumeRecord.getEndMeterNum());
		
		// 充电开始时间
		
        record.setUserId(chargeUserId);
	    record.setUserPhone(chargeAccount);
		record.setChreElectricpileid(pkEpId);
		record.setChreUsingmachinecode(epCode);

		record.setChreTransactionnumber(consumeRecord.getSerialNo());
		
		record.setChreStartdate(dtStart);
		record.setChreChargingnumber(epGunNo);
		record.setChreChargingmethod(chargingmethod);
		record.setChreReservationnumber("");
		
		record.setChreBeginshowsnumber(beginShowsNumber);
		record.setChreEndshowsnumber(endShowsnumber);
		
		record.setChreCode(chOrCode);
		
		record.setChreEnddate(dtEnd);
		record.setChreResttime(0);
		record.setStatus(1);
		record.setJPrice(rateInfo.getJ_Rate());
		record.setFPrice(rateInfo.getF_Rate());
		record.setPPrice(rateInfo.getP_Rate());
		record.setGPrice(rateInfo.getG_Rate());
		record.setQuantumDate(rateInfo.getQuantumDate());
		record.setPkUserCard(pkUserCard);
		record.setUserOrigin(userOrigin);
		record.setUserOrgNo(userOrigin);
		record.setPayMode(payMode);
		record.setServicePrice(rateInfo.getServiceRate());
		record.setThirdUsrIdentity(thirdUsrIdentity);
		//record.setThirdExtraData("");
		// 新增开始充电记录
		DB.chargingrecordDao.insertFullChargeRecord(record);
		
	}
	
	public static void insertChargeWithConsumeRecord(int chargeUserId,int chorType,
			String chargeAccount,int pkUserCard,int userOrigin,int pkEpId,String epCode,int epGunNo,int chargingmethod,
			String bespokeNo,String chOrCode,int payMode,NoCardConsumeRecord consumeRecord
			, RateInfo rateInfo)
	{
		// 尖时段用电度数
		BigDecimal tipPower = UtilProtocol.intToBigDecimal3(consumeRecord.getjDl());
		// 峰时段用电度数
		BigDecimal peakPower = UtilProtocol.intToBigDecimal3(consumeRecord.getfDl());
		// 平时段用电度数
		BigDecimal usualPower = UtilProtocol.intToBigDecimal3(consumeRecord.getpDl());
		// 谷时段用电度数
		BigDecimal valleyPower = UtilProtocol.intToBigDecimal3(consumeRecord.getgDl()); 
		// 时间段
		BigDecimal totalPower = UtilProtocol.intToBigDecimal3(consumeRecord.getTotalDl());
		
		
		Date dtStart = new Date(consumeRecord.getStartTime()*1000);
		Date dtEnd = new Date(consumeRecord.getEndTime()*1000);
		// 计算总电量
		String beginTime = DateUtil.toDateFormat(dtStart, "MM-dd HH:mm");
		String endTime = DateUtil.toDateFormat(dtEnd,"MM-dd HH:mm");
		
		
		BigDecimal  chargeAmt =  UtilProtocol.intToBigDecimal2(consumeRecord.getTotalChargeAmt());
		BigDecimal  serviceAmt = UtilProtocol.intToBigDecimal2(consumeRecord.getServiceAmt());
	    String thirdUsrIdentity="";
		if(userOrigin == EpConstant.CPYNUMBER_BQ)
		 {
			thirdUsrIdentity = consumeRecord.getEpUserAccount();
		 }
		// 充电消费订单
		 TblChargingOrder order = new TblChargingOrder();
		 order.setChorCode(chOrCode);
		
		order.setChorAppointmencode(bespokeNo);
		
		 order.setChorPilenumber(epCode);
		 order.setChorUserid(""+chargeUserId);
		
		 order.setChorType(chorType);
		 
		 order.setChorMoeny(chargeAmt.add(serviceAmt) + "");
		 order.setChOr_tipPower(tipPower);
		 order.setChOr_peakPower(peakPower);
		 order.setChOr_usualPower(usualPower);
		 order.setChOr_valleyPower(valleyPower);
		 order.setChorQuantityelectricity(totalPower );
		 order.setChorTimequantum(beginTime + " - " + endTime);
		 order.setChorMuzzle(epGunNo);
		 order.setChorChargingstatus(EpConstant.ORDER_STATUS_DONE + "");
		 order.setChorTranstype(""+consumeRecord.getTransType());
		
		 order.setChorCreatedate(new Date());
		 order.setChorUpdatedate(new Date());
		 order.setChorUsername("");
		 order.setChorTransactionnumber(consumeRecord.getSerialNo());
		 order.setChorChargemoney(chargeAmt);
		 order.setChorServicemoney(serviceAmt);
		 order.setChorOrdertype(0);
		 beginTime = DateUtil.toDateFormat(dtStart, ChargeYearDateFmt);
		 endTime = DateUtil.toDateFormat(dtEnd,ChargeYearDateFmt);
		
		 order.setChargeBegintime(beginTime);
		 order.setChargeEndtime(endTime);
		 order.setPkUserCard(pkUserCard);
		 order.setUserOrgNo(userOrigin);
		 order.setUserOrigin(userOrigin);
		 
		order.setThirdUsrIdentity(thirdUsrIdentity);
		 // 新增充电消费订单
		 DB.chargeOrderDao.insertFullChargeOrder(order);
			 
	 
		
		TblChargingrecord record = new TblChargingrecord();
		// 开始充电表低示数
		String beginShowsNumber = getMeterNum(consumeRecord.getStartMeterNum());
		String endShowsnumber = getMeterNum(consumeRecord.getEndMeterNum());
		
		// 充电开始时间
		
        record.setUserId(chargeUserId);
	    record.setUserPhone(chargeAccount);
		record.setChreElectricpileid(pkEpId);
		record.setChreUsingmachinecode(epCode);

		record.setChreTransactionnumber(consumeRecord.getSerialNo());
		
		record.setChreStartdate(dtStart);
		record.setChreChargingnumber(epGunNo);
		record.setChreChargingmethod(chargingmethod);
		record.setChreReservationnumber("");
		
		record.setChreBeginshowsnumber(beginShowsNumber);
		record.setChreEndshowsnumber(endShowsnumber);
		
		record.setChreCode(chOrCode);
		
		record.setChreEnddate(dtEnd);
		record.setChreResttime(0);
		record.setStatus(1);
		record.setJPrice(rateInfo.getJ_Rate());
		record.setFPrice(rateInfo.getF_Rate());
		record.setPPrice(rateInfo.getP_Rate());
		record.setGPrice(rateInfo.getG_Rate());
		record.setQuantumDate(rateInfo.getQuantumDate());
		record.setPkUserCard(pkUserCard);
		record.setUserOrigin(userOrigin);
		record.setUserOrgNo(userOrigin);
		record.setPayMode(payMode);
		record.setServicePrice(rateInfo.getServiceRate());
		record.setThirdUsrIdentity(thirdUsrIdentity);
		// 新增开始充电记录
		DB.chargingrecordDao.insertFullChargeRecord(record);
		
	}
	
	
	public static void updateChargeToDb(EpGunCache epGunCache,ChargeCache chargeCache,NoCardConsumeRecord consumeRecord,
			boolean exceptionData,BigDecimal couPonAmt,int pkCouPon,int thirdType,BigDecimal servicePrice)
	{
		String epCode =  epGunCache.getEpCode();
		int epGunNo =  epGunCache.getEpGunNo();
		int pkEpId = epGunCache.getPkEpGunId();
		
		int payMode = chargeCache.getPayMode();
		String chargeUserAccount = chargeCache.getAccount();
		int userId = chargeCache.getUserId();
		RateInfo rateInfo = chargeCache.getRateInfo();
		String chOrCode = chargeCache.getChOrCode();
		
		
		// 尖时段用电度数
		BigDecimal tipPower = UtilProtocol.intToBigDecimal3(consumeRecord.getjDl());
		// 峰时段用电度数
		BigDecimal peakPower = UtilProtocol.intToBigDecimal3(consumeRecord.getfDl());
		// 平时段用电度数
		BigDecimal usualPower = UtilProtocol.intToBigDecimal3(consumeRecord.getpDl());
		// 谷时段用电度数
		BigDecimal valleyPower = UtilProtocol.intToBigDecimal3(consumeRecord.getgDl()); 
		// 时间段
		
		BigDecimal totalPower = UtilProtocol.intToBigDecimal3(consumeRecord.getTotalDl());
				
			
		Date dtStart = new Date(consumeRecord.getStartTime()*1000);
		Date dtEnd = new Date(consumeRecord.getEndTime()*1000);
		// 计算总电量
		String beginTime = DateUtil.toDateFormat(dtStart, "MM-dd HH:mm");
		String endTime = DateUtil.toDateFormat(dtEnd,"MM-dd HH:mm");
		
		BigDecimal  chargeAmt =  UtilProtocol.intToBigDecimal2(consumeRecord.getTotalChargeAmt());
		BigDecimal  serviceAmt = UtilProtocol.intToBigDecimal2(consumeRecord.getServiceAmt());
		BigDecimal  chargeCost = UtilProtocol.intToBigDecimal2(consumeRecord.getTotalAmt());
		// 充电消费订单
		 TblChargingOrder order = new TblChargingOrder();
		 order.setChorCode(chOrCode);
		
		order.setChorAppointmencode("000000000000");
		
		order.setChorPilenumber(epCode);
		 
		 
		 order.setChorMoeny(chargeCost + "");
		 logger.debug("updateChargeToDb,consumeRecord.getTotalChargeAmt():{},chargeAmt:{}",consumeRecord.getTotalChargeAmt(),chargeAmt);
		 order.setChorChargemoney(chargeAmt);
		 order.setChorServicemoney(serviceAmt);
		 
		 order.setChOr_tipPower(tipPower);
		 order.setChOr_peakPower(peakPower);
		 order.setChOr_usualPower(usualPower);
		 order.setChOr_valleyPower(valleyPower);
		 order.setChorQuantityelectricity(totalPower );
		 order.setChorTimequantum(beginTime + " - " + endTime);
		 order.setChorTransactionnumber(consumeRecord.getSerialNo());
		 order.setChorMuzzle(epGunNo);
		 order.setPkCoupon(pkCouPon);
		 order.setCouPonAmt(couPonAmt);
		 order.setThirdType(thirdType);
		 
		 if(!exceptionData)
		 {
		 
			 if(payMode == EpConstant.P_M_FIRST)
			 {
				 order.setChorChargingstatus(EpConstant.ORDER_STATUS_SUCCESS + "");
			 }
			 else
			 {
				 order.setChorChargingstatus(EpConstant.ORDER_STATUS_DONE + "");
			 }
		 }
		 else
		 {
			 order.setChorChargingstatus(EpConstant.ORDER_STATUS_EXCEPTION+"");
		 }
		 
		 
		 order.setChorTranstype(consumeRecord.getTransType() + "");
		 String bespokeNo= StringUtil.repeat("0", 12);
		 order.setChorAppointmencode(bespokeNo);
		 
		 order.setChorUpdatedate(new Date());
		
		 order.setChorUsername("");
		 order.setChorTransactionnumber(consumeRecord.getSerialNo());
		
		 
		 beginTime = DateUtil.toDateFormat(dtStart, ChargeYearDateFmt);
		 endTime = DateUtil.toDateFormat(dtEnd,ChargeYearDateFmt);
		 order.setChargeBegintime(beginTime);
		 order.setChargeEndtime(endTime);
		 // 新增充电消费订单
		 DB.chargeOrderDao.updateOrder(order);
				 
		 // 根据交易流水号更新充电订单编号
		TblChargingrecord record = new TblChargingrecord();
		
		String beginShowsNumber = getMeterNum(consumeRecord.getStartMeterNum());
		String endShowsnumber = getMeterNum(consumeRecord.getEndMeterNum());
		
		// 开始充电表低示数
	
		// 充电开始时间
		record.setUserId(userId);
	    record.setUserPhone(chargeUserAccount);
	   
		record.setChreElectricpileid(pkEpId);
		record.setChreUsingmachinecode(epCode);

		record.setChreTransactionnumber(consumeRecord.getSerialNo());
		record.setChreBeginshowsnumber(beginShowsNumber);
		record.setChreEndshowsnumber(endShowsnumber);
		record.setChreStartdate(dtStart);
		record.setChreEnddate(dtEnd);
		record.setChreChargingnumber(epGunNo);
		record.setChreChargingmethod(1);
		record.setChreReservationnumber(bespokeNo);

		record.setChreResttime(0);
		
		record.setJPrice(rateInfo.getJ_Rate());
		record.setFPrice(rateInfo.getF_Rate());
		record.setPPrice(rateInfo.getP_Rate());
		record.setGPrice(rateInfo.getG_Rate());
		record.setQuantumDate(rateInfo.getQuantumDate());
		record.setChreCode(order.getChorCode());
		record.setServicePrice(servicePrice);
		
		record.setStatus(1);
		record.setThirdCode(pkCouPon);
		record.setThirdType(thirdType);
		
		// 新增开始充电记录
		DB.chargingrecordDao.insertEndRecord(record);
		
	}

	public static String getFaultDesc(int nStopRet)
	{
		String faultCause = "";
		switch(nStopRet)
		{
		case 2:
			faultCause = "用户强制结束";
			break;
		case 3:
			faultCause = "急停";
			break;
		case 4:
			faultCause = "连接线断掉";
			break;
		case 5:
			faultCause = "电表异常";
			break;
		case 6:
			faultCause = "过流停止";
			break;
		case 7:
			faultCause = "过压停止";
			break;
		case 8:
			faultCause = "防雷器故障";
			break;
		case 9:
			faultCause = "接触器故障";
			break;
		case 10:
			faultCause = "充电金额不足";
			break;
		case 11:
			faultCause = "漏电保护器";
			break;
		case 13:
			faultCause = "BMS通信异常故障";
			break;
		case 14:
			faultCause = "违规拔枪";
			break;
		case 15:
			faultCause = "电桩断电";
			break;
		default:
			faultCause = "电桩保护,nStopRet:"+nStopRet;
			break;
		}
		return faultCause;
	}
	public static String getStopChargeDesc(int nCause)
	{
		String faultCause = "";
		switch(nCause)
		{
		case 3:
			faultCause = "急停";
			break;
		case 4:
			faultCause = "连接线断掉";
			break;
		case 5:
		case 6:
		case 7:
		case 8:
		case 9:
		case 11:
		case 13:
			faultCause = "电桩保护(原因"+nCause+")";
			break;
		case 10:
			faultCause = "充电金额不足";
			break;
		case 12:
			faultCause = "自动充满";
			break;
		case 14:
			faultCause = "违规拔枪";
			break;
		case 15:
			faultCause = "电桩断电";
			break;
		case 16:
		case 17:
		case 18:
			faultCause = "电桩保护(原因"+nCause+")";
			break;
		case 19:
			faultCause = "车BMS主动停止";
			break;
		default:
			faultCause = "电桩保护(原因"+nCause+")";
			break;
		}
		return faultCause;
	}
	
	public static  int handleStopElectricizeEvent(EpCommClient epCommClient,String epCode,int epGunNo,String serialNo,long et,
			short nStopRet,int nDbValue,byte ChargeStye,byte offstates,byte successflag,byte[]time,byte []cmdTimes) throws IOException
	{
		
		short isException=0;
		int exceptionCode=0;
	
		//1.
		EpGunCache epGunCache = EpGunService.getEpGunCache(epCode, epGunNo);
		if(epGunCache==null)
		{
			logger.error("stopcharge Event error epGunCache==null,epCode:{},epGunNo:{}",epCode,epGunNo);			
			return  1;
		}
		short nTimeOut =0;
		ChargeCache chargeCache = epGunCache.getChargeCache();
			//在线二维码停止充电超时
		
		if (chargeCache == null )
		{
			logger.error("stopcharge event error not find ElectricCache,epCode:{},epGunNo:{}",
					epCode ,epGunNo );
			return  2;
		}
		if(chargeCache.getStatus() == ChargeStatus.CS_CHARGE_END)
		{
			logger.error("stopcharge event error had been stop,epCode:{},epGunNo:{}",
					epCode ,epGunNo );
			return 3;
		}
		
		//桩停止充电不成功		
		if (successflag != 1)
		{
			logger.error("stopcharge fail,epCode:{},epGunNo:{},successflag:{}",
					new Object[]{epCode,epGunNo,successflag});
			return 3;
		}
		chargeCache.setEt(et);

		if (nStopRet > 12) {
				nStopRet = 1;
		}
		if (nStopRet == 1 || nStopRet == 2 || nStopRet == 3 || nStopRet == 10) {
			isException = 1;
		} 
		else {
			exceptionCode = nStopRet;
		}
		short nnStopRet = nStopRet;
		if (nStopRet > 11)// 充电桩正常退出
		{
			nnStopRet = nStopRet;
		}
		chargeCache.setStopCause(nStopRet);
		
		endChargeRecord(chargeCache.getChargeSerialNo(),et ,0,chargeCache.getRateInfo().getServiceRate());
        
		chargeCache.setStatus(ChargeStatus.CS_CHARGE_END);
		
		return 0;
	}
	public static void endChargeRecord(String serialNo,long et,int endMeterNum,BigDecimal servicePrice)
	{
		TblChargingrecord record = new TblChargingrecord();
		record.setChreTransactionnumber(serialNo);
		record.setStatus( ChargeStatus.CS_CHARGE_END);
		record.setServicePrice(servicePrice);
		
		BigDecimal bdEndMeterNum = UtilProtocol.intToBigDecimal3(endMeterNum);
		String endShowsnumber = String.valueOf(bdEndMeterNum);
		
		record.setChreEndshowsnumber(endShowsnumber);
		Date endDate = DateUtil.toDate(et * 1000);
		
		
		record.setChreEnddate(endDate);
		
		// 更新充电记录 记录结束时间和用电度数
		DB.chargingrecordDao.insertEndRecord(record);
	}
	public static void insertFaultRecord(int stopCause,String epCode,int pkEpId,int epGunNo,String serialNo,Date faultDate)
	{
		if(stopCause<=2 || stopCause==10)
			return ;
		try
		{
			
			String faultCause = ""+stopCause;//getFaultDesc(stopCause);
			
            TblChargingfaultrecord faultrecord = new TblChargingfaultrecord();
			faultrecord.setCfreUsingmachinecode(epCode);
			faultrecord.setCfreElectricpileid(pkEpId);
			faultrecord.setCfreElectricpilename("");
			
			faultrecord.setcFRe_EphNo(epGunNo);
				
			
			faultrecord.setCfreChargingsarttime(faultDate);
			faultrecord.setCfreFaultcause(faultCause);
			faultrecord.setCfreTransactionnumber(serialNo);
			// 新增故障记录
			DB.chargingfaultrecordDao.insertFaultRecord(faultrecord);
			
		}catch(Exception e)
		{
			logger.error("insertFaultRecord,exception stack trace:{}",e.getStackTrace());
		}
		
	}
	private static int convertChargeCodeToAuthCode(int code)
	{
		switch(code)
		{
		case 1002:
		{				
			return 1;	
		}
		
		case EpConstantErrorCode.EP_UNCONNECTED:
		case EpConstantErrorCode.INVALID_EP_CODE:
		case EpConstantErrorCode.INVALID_EP_GUN_NO:
		case EpConstantErrorCode.INVALID_RATE:
		{
			return 2;	
		}
		
		case 6800:
		case EpConstantErrorCode.CANNOT_OTHER_OPER:
		{
			return 4;
		}
		
		case EpConstantErrorCode.INVALID_ACCOUNT:
		{
			return 3;
		}
		
		default:
			break;
		}
		return code;
		
	}
	public static int getCardPayMode(int cardPayMode)
	{
		if(cardPayMode==EpConstant.CARD_CO_CREDIT)
		{
			return 2;
		}
		return 1;
	}
	/**
	 * 
	 * @param epCode
	 * @param epGunNo
	 * @param innerCardNo
	 * @param fronzeAmt
	 * @param cmdTimes
	 * @return 1：金额不足
				2：没找到桩
				3：无效卡
				4：在其他桩上使用，不能充电

	 */
	private static int fronzeCardAmt(String epCode,int epGunNo,String innerCardNo,int fronzeAmt,byte []cmdTimes )
	{
		try
		{
			ElectricPileCache epCache=EpService.getEpByCode(epCode);
			if(epCache==null)
			{
				return 2;
			}
			int errorCode=0;
			
			EpGunCache epGunCache = EpGunService.getEpGunCache(epCode, epGunNo);
			if(epGunCache == null)
				return  2;
	
			ChargeCardCache cardCache =  UserService.getCard(innerCardNo);
			if(cardCache == null)
				return 3;
			
			int payMode= getCardPayMode(cardCache.getCardType());
			
			
			errorCode = epCache.canCharge(true,cardCache.getId(),payMode,cardCache.getCompanyNumber(),epGunNo);
			if(errorCode>0)
			{
				return 3;
			}
			
			int usrId = cardCache.getUserId();
			UserCache userCache= UserService.getUserCache(usrId);
			if(userCache==null)
				return 3;
			
			AuthUserCache authCache = epGunCache.getAuthCache();
			long now =DateUtil.getCurrentSeconds();
			
			if(authCache!=null)
			{
				long authTime = authCache.getLastTime();
				int curUsrId = epGunCache.getCurUserId();
				if(!cardCache.isAllowMutliCharge() && 
						(now- authTime)<GameConfig.userAuthTimeOut && 
						curUsrId>0 && curUsrId!=userCache.getId())
				{	
					errorCode = 4;	
				}
				else
				{
					authCache.setLastTime(now);
				}
			}
			else
			{
				authCache = new AuthUserCache(userCache.getId(),userCache.getAccount(),now,(short)3);
				epGunCache.setAuthCache(authCache);
			}
			
			if(errorCode==0)
			{
				int pkCardId = cardCache.getId();
				logger.info("charge card fronze amt success errorCode:{},accountId:{},account:{},innerCardNo:{},cardPayMode:{},epCode:{},epGunNo:{}",
						new Object[]{errorCode,usrId,userCache.getAccount(),innerCardNo,payMode,epCode,epGunNo});
				
				errorCode = apiStartElectric(
						epCode,epGunNo,userCache,"",cardCache,"",
						(short)EpConstant.CHARGE_TYPE_CARD,
						fronzeAmt,payMode,cardCache.getCompanyNumber(),2,userCache.getAccount(),cmdTimes);
				
				return convertChargeCodeToAuthCode(errorCode);
			}
			return errorCode;
		}
		catch(Exception e)
		{
			logger.debug("checkCardAuth,e.getMessage():{}",e.getMessage());
			return 3;
		}
	}

	public static  void handleCardFronzeAmt(EpCommClient epCommClient,String epCode,int epGunNo,String innerCardNo,int fronzeAmt,byte []cmdTimes) throws IOException
	{
		int accountId=0;
		String account="";
		
		int errorCode= fronzeCardAmt(epCode, epGunNo,innerCardNo,fronzeAmt,cmdTimes);
		
		if(errorCode>0)
		{
			logger.info("charge card fronze amt errorCode:{} accountId:{},account:{},innerCardNo:{},epCode:{},epGunNo:{}",
					
					new Object[]{errorCode,accountId,account,innerCardNo,epCode,epGunNo});
			
			
			logger.info("charge card fronze amt response errorCode:{} accountId:{},account:{},innerCardNo:{},epCode:{},epGunNo:{}",
					new Object[]{errorCode,accountId,account,innerCardNo,epCode,epGunNo});
			
			byte[] data = EpEncodeProtocol.do_card_frozen_amt(epCode,epGunNo,innerCardNo,0,errorCode);
			EpMessageSender.sendMessage(epCommClient,0,0,Iec104Constant.M_CARD_FRONZE_AMT_RET,data,cmdTimes,epCommClient.getVersion());
			
		}
		
		
	}
	/**
	 * 检查订单数据库状态
	 */
	public static int checkDBOrderStatus(String chargeSerialNo)
	{
		int ret=1;
		int orderStatus = EpChargeService.getChargeOrderStatus(chargeSerialNo);
		if(orderStatus==2|| orderStatus==3)//
		{
			logger.info("checkOrderStatus,serialNo:{},orderStatus:{}",chargeSerialNo,orderStatus);
			ret= 3;
		}
		else
			ret = orderStatus;
		
		return ret;
		
	}
	
	/**
	 * 检查参数和订单状态
	 * @param consumeRecord
	 * @return
	 */
	
	private static int checkConsumeRecordParams(NoCardConsumeRecord consumeRecord)
	{
		//1.检查流水号是否合法
	   if(consumeRecord==null)
	   {
		   logger.error("endcharge invalid consumeRecord:{}",consumeRecord);
		   return 4;
	   }
	   
	   String epCode = consumeRecord.getEpCode();
		int epGunNo = consumeRecord.getEpGunNo();
		  
	   String consumeSerialNo = consumeRecord.getSerialNo();
	   String zeroSerialNo= StringUtil.repeat("0", 32);
	   
	   if(consumeSerialNo==null || consumeSerialNo.length()!=32)
	   {
		   logger.error("endcharge invalid consumeSerialNo:{},consumeRecord:{}",consumeSerialNo,consumeRecord);
		   return 5;
	   }
	   if( consumeSerialNo.compareTo(zeroSerialNo)==0)
	   {
		   logger.error("endcharge invalid consumeSerialNo:{},consumeRecord:{}",consumeSerialNo,consumeRecord);
		   return 6;
	   }
		   
		EpGunCache  epGunCache = EpGunService.getEpGunCache(epCode, epGunNo);
		
		
		if(epGunCache == null)
		{
			logger.error("endcharge do not find EpGunCache,epCode:{},epGunNo:{},consumeRecord:{}", new Object[]{epCode,epGunNo,consumeRecord});
			return 7;
		}
		
		
		return 0;
		
	}
	public  static int isNewCouponStatus(int currentType,int newCouponAcStatus,int newCouponDcStatus)
	{
		if(     (newCouponAcStatus==0&&currentType==EpConstant.EP_AC_TYPE)||
				(newCouponDcStatus==0&&currentType==EpConstant.EP_DC_TYPE))
		{
			return 1;//该用户首次使用(分交直流)
		}
		return 0;
	}
	/**
	 * 消费记录电桩编号和枪口编号一定要准确，错了就发到别的地方
	 * @param channel
	 * @param consumeRecord
	 * @throws IOException
	 */
	public static  void handleConsumeRecord(EpCommClient epCommClient,NoCardConsumeRecord consumeRecord,byte []cmdTimes) throws IOException
	{
		logger.debug("endcharge enter handleNoCardConsumeRecord,consumeRecord:{}",consumeRecord);
		
		//1.检查订单参数
		int retCode= checkConsumeRecordParams(consumeRecord);
		
		if(retCode != 0 && retCode!=8)
		{
			logger.error("[endcharge] checkConsumeRecord error,retCode:{}",retCode);
			
			
			String retEpCode = StringUtil.repeat("0", 16);
			if(consumeRecord.getEpCode()!=null && consumeRecord.getEpCode().length()==16)
			{
				retEpCode = consumeRecord.getEpCode();
			}
			
			int retEpGunNo = consumeRecord.getEpGunNo();
				
			byte[] confirmdata=null;
			String serialNo = consumeRecord.getSerialNo();
			if(retCode==5)
			{
				serialNo = StringUtil.repeat("0", 32);
			}
			
			confirmdata = EpEncodeProtocol.do_consumerecord_confirm(retEpCode,retEpGunNo,serialNo,retCode);
			EpMessageSender.sendMessage(epCommClient,0,0,Iec104Constant.C_CONSUME_RECORD_CONFIRM, confirmdata,cmdTimes,epCommClient.getVersion());	
			
			return ;
		}
		//检查订单状态
		
		
		String epCode = consumeRecord.getEpCode();
		int epGunNo = consumeRecord.getEpGunNo();
		
		EpGunCache  epGunCache = EpGunService.getEpGunCache(epCode, epGunNo);
		if(epGunCache==null)
		{
			logger.error("[endcharge]handleConsumeRecord did not find gun,epCode:{},epGunNo:{}",
					new Object[]{epCode,epGunNo,consumeRecord});
			return ;
		}
		int retOrderCode = 0;
		retCode =-1;
		if(consumeRecord.getAccountType()==3 )
		{
			//检查订单状态
			retOrderCode = checkDBOrderStatus(consumeRecord.getSerialNo());
			if(retOrderCode==EpConstant.ORDER_STATUS_NO)//无订单
			{
				if(consumeRecord.getUserOrgin() == EpConstant.CPYNUMBER_BQ)
				{
					retCode = epGunCache.endBQConsumeRecord(consumeRecord);
				}
				else if(consumeRecord.getUserOrgin() == 5)
				{
					logger.info("[endcharge] handleNoCardConsumeRecord ,epCode:{},epGunNo:{},discard,CPYNUMBER_BQ old order:{} ",
								new Object[]{epCode,epGunNo,consumeRecord.getSerialNo()});
					retCode = 1;
				}
				else
				{
					retCode = epGunCache.endCreditConsumeRecord(consumeRecord);
				}
			}
		}
		//萍乡特殊的消息记录，即没有卡号，也没有账号
		else if(consumeRecord.getAccountType() == 4 && consumeRecord.getUserOrgin() == EpConstant.CPYNUMBER_PXGJ )
		{
			retOrderCode = checkDBOrderStatus(consumeRecord.getSerialNo());
			if(retOrderCode==EpConstant.ORDER_STATUS_NO)//无订单
			{
				retCode = epGunCache.endPXConsumeRecord(consumeRecord);
			}
		}
		else
		{
			retOrderCode= epGunCache.checkConsumeRecord(consumeRecord.getSerialNo(), consumeRecord.getEpUserAccount());
			//爱充正常用户充电记录
			if(retOrderCode==1)
			{
				retCode = epGunCache.endChargeWithConsumeRecord(consumeRecord);	
			}
		}
		
		
        logger.info("[endcharge] handleNoCardConsumeRecord over,epCode:{},epGunNo:{},retCode:{} ",
				new Object[]{epCode,epGunNo,retOrderCode});
        
        //当成数据不存在，确认!!!!!!!!!!!!!!!!!!!!!!!
        if(retOrderCode<0||retOrderCode>=4)
        {
        	retOrderCode=2;
        }
	
		if(retOrderCode>=0 && retOrderCode <=3)
		{	
				
			byte[] confirmdata=null;
			
			confirmdata = EpEncodeProtocol.do_consumerecord_confirm(epCode,epGunNo,consumeRecord.getSerialNo(),retOrderCode);
			EpMessageSender.sendMessage(epCommClient,0,0,Iec104Constant.C_CONSUME_RECORD_CONFIRM, confirmdata,cmdTimes,epCommClient.getVersion());
		}
		if(retCode!=1 && retCode!=-1)
		{
			logger.error("[endcharge] handleNoCardConsumeRecord error,retCode:{},epCode:{},epGunNo:{}", new Object[]{retCode,epCode,epGunNo});
			
		}
		
	}
	
	
	public static  void handleHisConsumeRecord(EpCommClient epCommClient,NoCardConsumeRecord consumeRecord) throws IOException
	{
		Logger log = LoggerFactory.getLogger("epRamLog");
		
		log.info("consumeRecord:{}",consumeRecord);
	}
	
	/**
	 * 该函数用来测试强制移除充电业务
	 * @param epCode
	 * @param epGunNo
	 */
	static public String forceRemoveCharge(String epCode,int epGunNo)
	{
		if(epCode!=null && epCode.length() != EpProtocolConstant.LEN_CODE )
        {
			return "remove charge cache! code invalid! \n";
        }
    	ElectricPileCache epClient =  EpService.getEpByCode(epCode);
    	if(epClient==null)
    	{
    		return "remove charge cache! did not find ep"+ epCode+" \n";
    	}
        		
        int nGunNum=epClient.getGunNum();
            	
            	
    	if(epGunNo<1 ||epGunNo >nGunNum)
    	{
    		return "remove charge cache! epGunNo invalid"+epGunNo+" \n";
    	}
            	
		//String epGunNoKey = epCode +epGunNo;
		//int currentType = epClient.getCurrentType();
        //ChargeCache chargeCache=null;
        
        EpGunCache epGunCache = EpGunService.getEpGunCache(epCode, epGunNo);
        if(epGunCache !=null)
        {
        	if(epGunCache.getChargeCache()!=null)
        	{
        		long et = DateUtil.getCurrentSeconds();
        		endChargeRecord(epGunCache.getChargeCache().getChargeSerialNo(),et ,0,epGunCache.getChargeCache().getRateInfo().getServiceRate());
        		logger.info("forceRemovecharge,endChargeRecord,epCode:{},epGunNo:{}",epCode,epGunNo);
        	}
        	
        	//TODO://如果处理数据库
        	epGunCache.cleanChargeInfo();
        	
    		BigDecimal bdZero = new BigDecimal(0.0);
    		EpGunService.updateChargeInfoToDbByEpCode(epClient.getCurrentType(),epCode,epGunNo,
    				bdZero,"",bdZero,0,0);
    		logger.info("forceRemovecharge,epCode:{},epGunNo:{}",epCode,epGunNo);
        }
        return "";
	}

	/**
	 * 该函数用来测试强制移除充电业务
	 * @param epCode
	 * @param epGunNo
	 */
	static public String forceRemoveBespoke(String epCode,int epGunNo)
	{
		if(epCode!=null && epCode.length() != EpProtocolConstant.LEN_CODE )
        {
			return "remove bespoke cache! code invalid! \n";
        }
    	ElectricPileCache epClient =  EpService.getEpByCode(epCode);
    	if(epClient==null)
    	{
    		return "remove bespoke cache! did not find ep"+ epCode+" \n";
    	}
        		
        int nGunNum=epClient.getGunNum();
            	
            	
    	if(epGunNo<1 ||epGunNo >nGunNum)
    	{
    		return "remove bespoke cache! epGunNo invalid"+epGunNo+" \n";
    	}
            
        EpGunCache epGunCache = EpGunService.getEpGunCache(epCode, epGunNo);
        if(epGunCache !=null)
        {
        	epGunCache.cleanBespokeInfo();
        	logger.info("forceRemovebespoke,epCode:{},epGunNo:{}",epCode,epGunNo);
        }
        return "";
	}
	public static String makeSerialNo()
	{
		Date now = new Date();
		Random random = new Random();
		
		return DateUtil.toDateFormat(now, Global.serialSecFormat) + "1"+ String.format("%03d",Math.abs(random.nextLong()%1000)); 
	}
	
	private static String getFixLenthString(int strLength) {  
	      
	    Random rm = new Random();  
	      
	    // 获得随机数  
	    double a= rm.nextDouble();
	  
	    // 将获得的获得随机数转化为字符串  
	    String fixLenthString = String.valueOf(a);  
	  
	    // 返回固定的长度的随机数  
	    return fixLenthString.substring(2, strLength + 2);  
	} 
	public static String makeChargeOrderNo(long pkGunId,long usrId)
	{
		long now = DateUtil.getCurrentSeconds();
		
		long l1= pkGunId%100000;
		long l2= pkGunId%1000000;
		
		String chOrCode = String.format("%d%05d%06d", now,l1,l2);

		return chOrCode;
	}
	
	public static void addChargeStat(int pkEpGunId,int chargeMeterNum,int chargeTime,int chargeAmt)	
	{

		TblElectricPileGun info= new TblElectricPileGun();
		info.setPkEpGunId(pkEpGunId);
		
		info.setTotalChargeMeter(UtilProtocol.intToBigDecimal3(chargeMeterNum));
		info.setTotalChargeTime(chargeTime);
		info.setTotalChargeAmt(UtilProtocol.intToBigDecimal2(chargeAmt));
	    
		DB.epGunDao.addChargeStat(info);
	}
	
	public static int getOrType(int level)
	{
		int chorType=1;
		if(level==6)
			chorType=1;
		else if(level == 5)
			chorType=2;
		else
			chorType=3;
		
		return chorType;
	}
	
	private static TblPowerModule converPowerModule(ChargePowerModInfo info)
	{
		TblPowerModule powerModule = new TblPowerModule();
		
		
		powerModule.setPkPowerModuleid( info.getPkId());
		//1.输出电压
		powerModule.setOutput_voltage((new BigDecimal(info.getOutVol())).multiply(Global.Dec1));
		
		//2.输出电流
		powerModule.setOutput_current((new BigDecimal(info.getOutCurrent())).multiply(Global.Dec2));
		
		//3.A相电压
		powerModule.setA_voltage((new BigDecimal(info.getaVol())).multiply(Global.Dec1));
		
		//4.B相电压
		powerModule.setB_voltage((new BigDecimal(info.getbVol())).multiply(Global.Dec1));
		
		//5.C相电压
		powerModule.setC_voltage((new BigDecimal(info.getcVol())).multiply(Global.Dec1));
		
		//6.A相电流
		powerModule.setA_current((new BigDecimal(info.getaCurrent())).multiply(Global.Dec2));
		
		//7.B相电流
		powerModule.setB_current((new BigDecimal(info.getbCurrent())).multiply(Global.Dec2));
		
		//8.C相电流
		powerModule.setC_current((new BigDecimal(info.getcCurrent())).multiply(Global.Dec2));
		
		//9.模块温度
		powerModule.setTemperature((new BigDecimal(info.getTemp())).multiply(Global.Dec1));
		
		return powerModule;
	
	}
	
	public static void updatePowerInfoToDB(ChargePowerModInfo info){
		
		if(info==null )
		{
			logger.error("updatePowerInfoToDB error!ChargePowerModInfo:{}",info);
			return ;
		}
		
		TblPowerModule dbInfo = converPowerModule(info);
		
		dbInfo.setPkPowerModuleid(info.getPkId());

		DB.powerModuleDao.update(dbInfo);
		
	}
	public static void insertPowerInfoDB(ChargePowerModInfo info,String chargeSerialNo)
	{
		if(info==null || chargeSerialNo==null || chargeSerialNo.length()!=32)
		{
			logger.error("insertPowerInfoDB error!ChargePowerModInfo:{},chargeSerialNo:{}",info,chargeSerialNo);
			return ;
		}
		
		TblPowerModule dbInfo = converPowerModule(info);
		dbInfo.setChargeSerialNo(chargeSerialNo);
		
		dbInfo.setPowerModuleName("电源模块_"+info.getModId());
		
	
		int pkId = DB.powerModuleDao.insert(dbInfo);
		
		info.setPkId(pkId);
		
	}
	
	public  static void savePowerModule( Map<Integer,ChargePowerModInfo> mapPowerModule,String chargeSerialNo)
	{
		logger.debug("savePowerModule");
		if( mapPowerModule.size()== 0) //没有数据需要更新
		{
			return;
		}
        Iterator iter = mapPowerModule.entrySet().iterator();
        
		while (iter.hasNext()) {
			
			Map.Entry entry = (Map.Entry) iter.next();
			
			ChargePowerModInfo powerModInfo = (ChargePowerModInfo)entry.getValue();
			if(powerModInfo==null || powerModInfo.getChange()!=1  )
				continue;
			
			if(powerModInfo.getPkId()==0)
				insertPowerInfoDB(powerModInfo,chargeSerialNo);
			else
			{
				updatePowerInfoToDB(powerModInfo);
			}
			
			powerModInfo.setChange(0);
		}
	}
	
	private static TblVehicleBattery convertChargeCarInfo(ChargeCarInfo info)
	{
		TblVehicleBattery battery = new TblVehicleBattery();
		
		
		battery.setBattery_manufacturers( info.getBatteryManufacturer());
		
		battery.setBattery_rated_capacity((int)info.getTotalBattryCapacity());
		battery.setBattery_type((int)info.getBattryType());
		battery.setCycle_count((int)info.getBattryChargeTimes());
		battery.setMax_current((new BigDecimal(info.getSignleAllowableHighCurrent())).multiply(Global.Dec2));
		battery.setMax_temperature((new BigDecimal(info.getAllowableHighTotalTemp())).multiply(Global.Dec1));
		battery.setSingle_max_vol((new BigDecimal(info.getSignleAllowableHighVol())).multiply(Global.Dec1));
		battery.setTotal_energy((new BigDecimal(info.getTotalBattryPower())).multiply(Global.Dec1));
		battery.setVin(info.getCarIdenty());
		battery.setTotal_rated_voltage((new BigDecimal(info.getAllowableHighTotalVol())).multiply(Global.Dec1));
		
		int year=info.getBattryMadeYear();
		int day = info.getBattryMadeDay();
		
		String dateString=String.format("%04d%02d%02d",year,day/100,day%100);		
		battery.setProduction_date(DateUtil.parse(dateString));
		
		
		return battery;
	}
	
	public static void updateChargeCarInfoToDB(ChargeCarInfo info){
		
		if(info==null )
		{
			logger.error("updatePowerInfoToDB error!ChargePowerModInfo:{}",info);
			return ;
		}
		
		TblVehicleBattery dbInfo = convertChargeCarInfo(info);
		
		dbInfo.setPk_VehicleBattery(info.getPkId());
		
		DB.vehicleBatteryDao.update(dbInfo);
		
	}
	public static void insertChargeCarInfoToDB(ChargeCarInfo info,String chargeSerialNo)
	{
		if(info==null || chargeSerialNo==null || chargeSerialNo.length()!=32)
		{
			logger.error("insertChargeCarInfoToDB error!ChargePowerModInfo:{},chargeSerialNo:{}",info,chargeSerialNo);
			return ;
		}
		
		TblVehicleBattery dbInfo = convertChargeCarInfo(info);
		dbInfo.setChargeSerialNo(chargeSerialNo);
		
		int pkId =DB.vehicleBatteryDao.insert(dbInfo);
	
		info.setPkId(pkId);
		
	}
	
	public static void onChargeFail(int userId,ChargeCache chargeCache)
	{
		if(chargeCache.getStatus() == ChargeStatus.CS_CHARGE_FAIL)
		{
			logger.error("onChargeFail had handle,chargeCache:{}",chargeCache);
			return ;
		}
		chargeCache.setStatus(ChargeStatus.CS_CHARGE_FAIL);
		
		try{
		
			TblChargingrecord record = new TblChargingrecord();
			record.setChreTransactionnumber(chargeCache.getChargeSerialNo());
			record.setStatus(ChargeStatus.CS_CHARGE_FAIL);
			record.setThirdCode(chargeCache.getThirdCode());
			record.setThirdType(chargeCache.getThirdType());
			DB.chargingrecordDao.updateBeginRecordStatus(record);
		}
		catch(Exception e)
		{
			logger.error("updateBeginRecordStatus exception.accountId:{},chargeCache:{},getStackTrace:{}",
					new Object[]{userId,chargeCache,e.getStackTrace()});
		}
		
		
		//2.退钱
		BigDecimal bdFronzeAmt = UtilProtocol.intToBigDecimal2(chargeCache.getFronzeAmt());
		logger.info("onChargeFail,return accountId:{},fronze amt:{}",userId,bdFronzeAmt);
		UserService.addAmt(userId, bdFronzeAmt);
	}
	
	public static void saveVehicleBatteryToDb(ChargeCarInfo info,String chargeSerialNo)
	{
		logger.debug("saveVehicleBatteryToDb");
		if(info==null)
		{
			return ;
		}
		if(info.getPkId()==0)//第一次保存
		{
			insertChargeCarInfoToDB(info,chargeSerialNo);
		}
		else//有数据更新
		{
			updateChargeCarInfoToDB(info);
		}
	}
	
	//查
	//注册消费
	public static void insertUserCoupon(int userId)
	{
		/*TblCoupon coupon= new TblCoupon();
		//4.
		coupon.setPkActivity(pkActivity);
		coupon.setPkCouponvariety(pkCouponvariety);
	    
		DB.couponDao.insertCoupon(coupon);*/
	}
	public static TblCoupon queryCoupon(int cpUserid,int cpLimitation,int consemeAmt)
	{
		return queryCoupon(cpUserid,cpLimitation,0,consemeAmt);
	}
	public static TblCoupon queryCoupon(int cpUserid,int cpLimitation,int actActivityrule,int consemeAmt)
	{
		Map<String,Object> map =  new HashMap<String,Object> ();

		map.put("cpUserid", cpUserid);
		map.put("cpLimitation", cpLimitation);
		map.put("actActivityrule", actActivityrule);
		map.put("consemeAmt", consemeAmt);
	    
		List<TblCoupon> list = DB.couponDao.queryCoupon(map);
		if (list != null && list.size() > 0)
		{
			if ("0".equals(list.get(0).getCpRate())) return null;
			return list.get(0);
		} else {
			return null;
		}
	}
	public static void updateCoupon(int pkCoupon)
	{
		DB.couponDao.updateCoupon(pkCoupon);
	}
	public static void insertInviteCoupon(int cpUserid)
	{
		List<Integer> list = DB.couponDao.queryActivity(4);
		if (list != null && list.size() == 1)
		{
			Map<String,Object> map =  new HashMap<String,Object> ();

			map.put("cpUserid", cpUserid);
			map.put("pkActivity", list.get(0));
		    
			DB.couponDao.insertInviteCoupon(map);
		}
	}
	
	public static void statChargeAmt(int userId,int userLevel,int fronzeAmt,int payMode,
			NoCardConsumeRecord consumeRecord,int couPonAmt,int pkOrderId,String orderNo,boolean isPauseStat)
	{
		
		int totalAmt = consumeRecord.getTotalAmt();
		int realCouPonAmt = EpChargeService.calcRealCouPonAmt(totalAmt,couPonAmt);//实际优惠金额
		int realAmt = totalAmt - realCouPonAmt;//抵扣后多出的消费

		logger.info("endcharge stat accountId:{},fronzeAmt:{},totalAmt:{},couPonAmt:{},realCouPonAmt:{},isPauseStat:{}",
				new Object[]{userId,fronzeAmt,totalAmt,couPonAmt,realCouPonAmt,isPauseStat});

		
	
		//先付费的结算资金
		if(payMode != EpConstant.P_M_FIRST)
		{
			logger.info("endcharge stat 1 accountId:{}",userId);
			return;
		}
		int remainAmt = fronzeAmt- realAmt; //剩余金额
		logger.info("endcharge stat userId:{},remainAmt:{}",userId,remainAmt);
		if(remainAmt<=0)
		{
			logger.info("endcharge stat 2 accountId:{}",userId);
			return ;
		}
		
		BigDecimal bdRemainAmt= UtilProtocol.intToBigDecimal2(remainAmt);
		UserService.addAmt(userId, bdRemainAmt);
		
		logger.info("endcharge stat accountId:{},remainAmt:{}",userId,remainAmt);
		
		if(isPauseStat)
		{
			EpChargeService.jmsgPauseOrderStat(userId, pkOrderId,orderNo, bdRemainAmt.toString());
		}
	}
	
	
	public static OrderStatInfo handleOrderDiscount(EpGunCache gunCache,NoCardConsumeRecord consumeRecord, 
			ChargeCache chargeCacheObj,UserCache chargeUser,int currentType,
			int totalChargeTime,int totalChargeMeterNum)
	{
		OrderStatInfo osInfo = null;
		if(chargeCacheObj.getThirdType() == ChargeStatus.CHARGEORDER_THIRDTYPE_VIN)
		{
			//使用vin统计
			osInfo=  useCarVinStat( gunCache, consumeRecord, chargeCacheObj, 
				chargeUser, currentType,totalChargeTime, totalChargeMeterNum);
			
		}
		else
		{
			//使用优惠券统计
			osInfo= useCouPonStat( gunCache, consumeRecord, 
					 chargeCacheObj, chargeUser, currentType,
					 totalChargeTime, totalChargeMeterNum);
		}
		osInfo.setTotalAmt(consumeRecord.getTotalAmt());
		
		return osInfo;
	}
	
	
	
	/**
	 * 处理电桩上送的正确消费记录
	 * 
	 * @param gunCache
	 * @param consumeRecord
	 * @param chargeCacheObj
	 * @param chargeUser
	 * @param currentType
	 * @param totalChargeTime
	 * @param totalChargeMeterNum
	 * @return
	 */
	public static int handleRightConsumeRecord(EpGunCache gunCache,NoCardConsumeRecord consumeRecord, 
			ChargeCache chargeCacheObj,UserCache chargeUser,int currentType,
			int totalChargeTime,int totalChargeMeterNum,boolean isPauseStat)
	{
		if(gunCache == null)
		{
			logger.error("endcharge handleRightConsumeRecord fail,gunCache is null,epCode:{},epGunNo:{},SerialNo:{}",
					new Object[]{consumeRecord.getEpCode(),consumeRecord.getEpGunNo(),consumeRecord.getSerialNo()});
			return 0;
		}
		if(chargeUser ==null)
		{
			logger.error("endcharge handleRightConsumeRecord fail,chargeUser is null,epCode:{},epGunNo:{},SerialNo:{}",
					new Object[]{consumeRecord.getEpCode(),consumeRecord.getEpGunNo(),consumeRecord.getSerialNo()});
			return 0;
		}
		
		int userId = chargeUser.getId();
		String epCode =  gunCache.getEpCode();
		int epGunNo = gunCache.getEpGunNo();
		String chargeSerialNo = chargeCacheObj.getChargeSerialNo();
		
		
		OrderStatInfo orderStatInfo  = handleOrderDiscount(gunCache, consumeRecord, chargeCacheObj, 
				chargeUser, currentType,totalChargeTime, totalChargeMeterNum);
		
		if(orderStatInfo==null)
		{
			logger.error("endcharge handleOrderDiscount accountId:{},epCode:{}",
					new Object[]{userId,epCode,epGunNo,chargeSerialNo});
			return 0;
			
		}
		int couPonAmt =orderStatInfo.getCouPonAmt();
		 //2.结算钱
		EpChargeService.statChargeAmt(chargeUser.getId(),chargeUser.getLevel(),chargeCacheObj.getFronzeAmt(),
				                 chargeCacheObj.getPayMode(),consumeRecord,couPonAmt,
				                 chargeCacheObj.getPkOrderId(),chargeCacheObj.getChOrCode(),isPauseStat);


		RateService.addPurchaseHistoryToDB(UtilProtocol.intToBigDecimal2(orderStatInfo.getTotalAmt()),1,userId,0,"充电消费",epCode,chargeSerialNo,"");

		if(couPonAmt>0)//如果有优惠券抵扣
		{
		    RateService.addPurchaseHistoryToDB(UtilProtocol.intToBigDecimal2(orderStatInfo.getRealCouPonAmt()),6,userId,0,"优惠券抵扣",epCode,chargeSerialNo,"");
		}
		
		int AcStatus = chargeUser.getNewcouponAcStatus();
		int DcStatus = chargeUser.getNewcouponDcStatus();
		//3.送邀请者优惠券
		if(AcStatus==0 && DcStatus==0)
		{
			sendInviteCoupon(chargeUser.getInvitePhone());
		}
		//4.更新用户新手状态
	    updateNewcouponStatus(chargeUser,currentType);
	    
	  //3.记录正常数据到订单
	    int totalAmt = consumeRecord.getTotalAmt();
	    if(chargeCacheObj.getPkOrderId()>0 )
	    {
	    	updateChargeToDb(gunCache,chargeCacheObj, 
					consumeRecord,false,new BigDecimal(orderStatInfo.getRealCouPonAmt()),orderStatInfo.getPkCouPon(),
					orderStatInfo.getThirdType(),chargeCacheObj.getRateInfo().getServiceRate());
	    }
	    else
	    {
	    	int chorType= EpChargeService.getOrType(chargeUser.getLevel());
			
	    	int pkUserCard = chargeCacheObj.getPkUserCard();
	    	insertFullChargeOrder(chargeUser.getId(),chorType,chargeUser.getAccount(),pkUserCard,consumeRecord.getUserOrgin(),gunCache.getPkEpGunId(),
	    			gunCache.getEpCode(),gunCache.getEpGunNo(),EpConstant.CHARGE_TYPE_CARD,"",chargeCacheObj.getChOrCode(),2,consumeRecord,chargeCacheObj.getRateInfo(),EpConstant.ORDER_STATUS_SUCCESS);
	    	
	    }
		
		logger.info("endcharge success,epCode:{},epGunNo:{},SerialNo:{},TotalDl:{},totalAmt:{}," +
				"getChargeUseTimes:{},getPkCouPon:{},ThirdType:{}",
					new Object[]{consumeRecord.getEpCode(),consumeRecord.getEpGunNo(),
					consumeRecord.getSerialNo(),totalChargeMeterNum,totalAmt,totalChargeTime,
					orderStatInfo.getPkCouPon(),orderStatInfo.getThirdType()});
		
		
		//4.记录充电统计
		EpChargeService.addChargeStat(gunCache.getPkEpGunId(),totalChargeMeterNum,
					totalChargeTime,totalAmt);
		
		if(orderStatInfo.getThirdType() == ChargeStatus.CHARGEORDER_THIRDTYPE_COUPON)
		{
			return orderStatInfo.getCouPonAmt();
		}
		    
		
		return 0;
	}
	
	 /**
     * 计算实际优惠
     * @param totalAmt
     * @param couPonAmt
     * @return
     */
	public static int calcRealCouPonAmt(int totalAmt,int couPonAmt)
	{
		int realCouPonAmt=0;
		if(couPonAmt<=0)
			return realCouPonAmt;
		
		if(totalAmt>=couPonAmt)
		    realCouPonAmt = couPonAmt;
		else
			realCouPonAmt = totalAmt;
		
		return realCouPonAmt;
	}
	/**
	 * 查找用户的可用优惠券
	 * @param usrId
	 * @param currentType
	 * @param acStatus
	 * @param dcStatus
	 * @param totalAmt
	 * @return
	 */
	public static TblCoupon findCoupon(int usrId,int currentType,int acStatus,int dcStatus,int totalAmt)
	{
		try
		{
			TblCoupon tblCoupon = null;
			int cpLimitation= getCouponEpType(currentType);
			int cpStatus = getNewCouponStatus(currentType,acStatus,dcStatus);
			if(cpStatus ==0)
			{   
				tblCoupon = queryCoupon(usrId,cpLimitation,2,totalAmt);
				if(tblCoupon==null)
				{
					tblCoupon = queryCoupon(usrId,cpLimitation,totalAmt);
				}
			}
			else
				tblCoupon = queryCoupon(usrId,cpLimitation,totalAmt);
			
			return tblCoupon;
		}
		catch(Exception e)
		{
			logger.error("findCoupon exception! usrId:{},currentType:{},acStatus:{},dcStatus:{},itotalAmt:{}",
					new Object[]{usrId,currentType,acStatus,dcStatus, totalAmt});
			return null;
		}
	}
	/**
	 * 给邀请车发送新手优惠券
	 * @param inviteUsrAccount
	 */
	public static void sendInviteCoupon(String inviteUsrAccount)
	{
		UserRealInfo userInfo = UserService.findUserRealInfo(inviteUsrAccount);
		if(userInfo ==null)
		{
			logger.error("endcharge stat not find Invite user,phone:{}",inviteUsrAccount);
			return ;
		}
		
	   insertInviteCoupon(userInfo.getId());
	}
	
	public static int getNewCouponStatus(int currentType,int acStatus,int dcStatus)
	{
		if(currentType == EpConstant.EP_AC_TYPE)
			return acStatus;
		return dcStatus;
	}
	public static int getCouponEpType(int currentType)
	{
		if(currentType == EpConstant.EP_AC_TYPE)
			return 1;
		return 2;
	}
	/**
	 * 使用优惠券统计
	 * @param gunCache
	 * @param consumeRecord
	 * @param chargeCacheObj
	 * @param chargeUser
	 * @param currentType
	 * @param totalChargeTime
	 * @param totalChargeMeterNum
	 * @return
	 */
	public static OrderStatInfo useCouPonStat(EpGunCache gunCache,NoCardConsumeRecord consumeRecord, 
			ChargeCache chargeCacheObj,UserCache chargeUser,int currentType,
			int totalChargeTime,int totalChargeMeterNum)
	{
		int couPonAmt = 0; //优惠券金额
		int pkCouPon=0;
		
		//1.获取优惠券
		TblCoupon tblCoupon = null;
		
		try
		{
			int AcStatus = chargeUser.getNewcouponAcStatus();
			int DcStatus = chargeUser.getNewcouponDcStatus();
			int totalAmt = consumeRecord.getTotalAmt();
			int iTotalAmt = totalAmt/100;//总金额 ，单位：元
			tblCoupon = findCoupon(chargeUser.getId(),currentType,AcStatus,DcStatus,iTotalAmt);
			OrderStatInfo statInfo =  new OrderStatInfo();
			if(tblCoupon ==null)
			{
				
			}
			else
			{
				 //3.设置优惠券已使用
				pkCouPon = tblCoupon.getPkCoupon();
				couPonAmt = tblCoupon.getCpCouponvalue()*100;
				updateCoupon(pkCouPon);
				
				statInfo.setCouPonAmt(couPonAmt);
				statInfo.setThirdType(ChargeStatus.CHARGEORDER_THIRDTYPE_COUPON);
				statInfo.setPkCouPon(pkCouPon);
				int realCouPonAmt = calcRealCouPonAmt(totalAmt,couPonAmt);
				statInfo.getRealCouPonAmt();
			}
			
			logger.info("endcharge useCouPonStat,accountId:{},pkCouPon:{},couPonAmt:{},epCode:{},epGunNo:{},AcStatus:{},DcStatus:{},currentType:{}",
					new Object[]{chargeUser.getId(),pkCouPon,couPonAmt,gunCache.getEpCode(),
					gunCache.getEpGunNo(),AcStatus,DcStatus,currentType});
			return statInfo;
			
		
		}
		catch(Exception e)
		{
			logger.error("endcharge get coupon exception,userId:{}",chargeUser.getId());
			return null;
		}
		
		
	}
	
	/**
	 *  使用vin统计
	 * @param gunCache
	 * @param consumeRecord
	 * @param chargeCacheObj
	 * @param chargeUser
	 * @param totalChargeTime
	 * @param totalChargeMeterNum
	 * @return
	 */
	public static OrderStatInfo useCarVinStat(EpGunCache gunCache,NoCardConsumeRecord consumeRecord, 
			ChargeCache chargeCacheObj,UserCache chargeUser,int currentType,
			int totalChargeTime,int totalChargeMeterNum)
	{
		
		BigDecimal servicePrice = chargeCacheObj.getRateInfo().getServiceRate();
			
	
		//1.重新计算服务费
	    int totalDl = consumeRecord.getTotalDl();
	    BigDecimal dl = new BigDecimal(totalDl).multiply(Global.Dec3);
	    int serviceAmt = dl.multiply(servicePrice).multiply(Global.DecTime2).intValue();    
	    int totalAmt = consumeRecord.getTotalChargeAmt()+serviceAmt;
	    
	    logger.info("endcharge useCarVinStat,accountId:{},vinCode:{},vinServicePrice:{},totalDl:{},FronzeAmt:{}," +
	    		"TotalChargeAmt:{},serviceAmt:{},calcServiceAmt:{},calcTotalAmt:{},epCode:{},epGunNo:{}",
    			new Object[]{chargeUser.getId(),chargeCacheObj.getThirdCode(),servicePrice,totalDl,
	    		chargeCacheObj.getFronzeAmt(),consumeRecord.getTotalChargeAmt(),
    			consumeRecord.getServiceAmt(),serviceAmt,totalAmt,
	    		gunCache.getEpCode(),gunCache.getEpGunNo()});
	    
	    if(totalAmt>chargeCacheObj.getFronzeAmt())//重新计算后如果总的费用大于预冻
	    {
	    	consumeRecord.setServiceAmt(chargeCacheObj.getFronzeAmt()-consumeRecord.getTotalChargeAmt());
	    	consumeRecord.setTotalAmt(chargeCacheObj.getFronzeAmt());
	    }
	    else //重新计算后总的费用<=预冻
	    {
	    	consumeRecord.setServiceAmt(serviceAmt);
	    	consumeRecord.setTotalAmt(totalAmt);
	    }
	    
	    OrderStatInfo statInfo = new OrderStatInfo();
	    
	    statInfo.setThirdType(ChargeStatus.CHARGEORDER_THIRDTYPE_VIN);
	    statInfo.setPkCouPon(chargeCacheObj.getThirdCode());
	    
	    logger.info("endcharge useCarVinStat,accountId:{},realServiceAmt:{},realTotalAmt:{}" +
	    		"epCode:{},epGunNo:{}",new Object[]{chargeUser.getId(),consumeRecord.getServiceAmt(),
	    		consumeRecord.getTotalAmt(),gunCache.getEpCode(),gunCache.getEpGunNo()});
	    
	    
		
		return statInfo;
	}
	/**
	 * 从数据库中获取对应的vin
	 * @param epCode
	 * @param epGunNo
	 * @param vinCode
	 * @return
	 */
	public static TblCarVin getCarVinByCode(String epCode,int epGunNo,String vinCode )
	{
		if(vinCode == null || vinCode.length()<=0)
		{
			logger.error("charge vinCode=null,epCode:{},epGunNo:{}",
					new Object[]{epCode,epGunNo});
			return null;
		}
		List<TblCarVin> carVinList= DB.carVinDao.selectByCode(vinCode);
		if(carVinList==null || carVinList.size()==0)
		{
			logger.error("charge not find carVin in db,epCode:{},epGunNo:{},carVin:{},carVin(hex):{}",
					new Object[]{epCode,epGunNo,vinCode,WmIce104Util.ConvertHex2(vinCode.getBytes())});
			return null;
		}
		
		TblCarVin carVin = carVinList.get(0);
		return carVin;
	}
    
	/**
	 * 从数据库中获取对应的vin
	 * @param epCode
	 * @param epGunNo
	 * @param vinCode
	 * @return
	 */
	public static TblCarVin getCarVinById(int vinId )
	{
		List<TblCarVin> carVinList= DB.carVinDao.selectById(vinId);
		if(carVinList==null || carVinList.size()==0)
		{
			logger.error("find carVin in db,vinId:{}",vinId);
			return null;
		}
		
		TblCarVin carVin = carVinList.get(0);
		return carVin;
	}
	/**
	 * 
	 */
	public static void updateChargeThirdtoDB(String epCode,int epGunNo,String vinCode,ChargeCache chargeCache)
	{
		TblCarVin  carVin = getCarVinByCode(epCode,epGunNo,vinCode );
		if(carVin !=null)
		{
			chargeCache.setThirdCode(carVin.getPkCarVin());
			chargeCache.setThirdType(ChargeStatus.CHARGEORDER_THIRDTYPE_VIN);
			chargeCache.getRateInfo().setServiceRate(carVin.getVinServicemoney());
		}
		// 根据交易流水号更新充电记录
		TblChargingrecord record = new TblChargingrecord();
		record.setChreTransactionnumber(chargeCache.getChargeSerialNo());
		record.setThirdCode(chargeCache.getThirdCode());
		record.setThirdType(chargeCache.getThirdType());
		record.setServicePrice(chargeCache.getRateInfo().getServiceRate());
		
		DB.chargingrecordDao.updateThird(record);
		
	}
	/**
	 * 
	 */
	public static void updateNewcouponStatus(UserCache chargeUser,int epType)
	{
		int AcStatus = chargeUser.getNewcouponAcStatus();
		int DcStatus = chargeUser.getNewcouponDcStatus();
		
		if(AcStatus ==0 && epType==EpConstant.EP_AC_TYPE)
		{
			chargeUser.setNewcouponAcStatus(1);
			UserService.updateNewcoupon(chargeUser);
		}
		else if(DcStatus ==0 && epType==EpConstant.EP_DC_TYPE)
		{
			chargeUser.setNewcouponDcStatus(1);
			UserService.updateNewcoupon(chargeUser);
		}
		
	}
	
	public static int pauseStatCharge(ChargeCache chargeCache)
	{
		logger.info("pauseStatCharge enter");
		TblChargingOrder tblChargeOrder =  new TblChargingOrder();
		tblChargeOrder.setChorTransactionnumber(chargeCache.getChargeSerialNo());
		tblChargeOrder.setChorChargingstatus(""+EpConstant.ORDER_PAUSE_STAT);
		DB.chargeOrderDao.updateStatus(tblChargeOrder);
		
		chargeCache.setStatus(ChargeStatus.CS_PAUSE_STAT);
		
		if(chargeCache.getAccount()!=null && chargeCache.getAccount().length()==11)
		{
			msgPauseStat(chargeCache.getAccount());
			jmsgPauseStat(chargeCache.getUserId(),chargeCache.getAccount(),chargeCache.getPkOrderId(), chargeCache.getChOrCode());
		}
		
		return 0;
	}
	
	
	public static void msgRMAmtWarningToManager(int usrId,String phone,String name,String amt)
	{
		logger.info("onRMAmtWarningToManager,usrId:{},phone:{},name:{}",
				new Object[]{usrId,phone,name});
		if(phone==null||phone.length()!=11)
			return;
		if(GameConfig.sms != 1)
		{
			HashMap<String,Object>  params=new HashMap<String,Object>();
			params.put("name", name);
			params.put("cost", amt);
			
			JSONObject jsonObject = JSONObject.fromObject(params);
			
			boolean flag = AliSMS.sendAliSMS(phone, "SMS_34475278", jsonObject.toString());
			if(!flag)
			{
				logger.error("onRMAmtWarningToManager fail,usrId,phone:{}",usrId,phone);
			}
		}
	}
	public static void msgRemainAmtWarning(int usrId,String phone,String amt)
	{
		logger.info("onRemainAmtWarning,usrId:{},phone:{}",usrId,phone);
		if(phone==null||phone.length()!=11)
			return;
		
		
		if(GameConfig.sms != 1)
		{
			HashMap<String,Object>  params=new HashMap<String,Object>();
			params.put("cost", amt);
			
			JSONObject jsonObject = JSONObject.fromObject(params);
			
			boolean flag = AliSMS.sendAliSMS(phone, "SMS_34445317", jsonObject.toString());
			if(!flag)
			{
				logger.error("onRemainAmtWarning fail,usrId,phone:{}",usrId,phone);
			}
		}
	}
	
	public static void jmsgPauseOrderStat(int userId,int pkOrderId,String chargeOrderNo,String remainAmt)
	{
		TblJpush ju=DB.jpushDao.getByuserInfo(userId);
		if(ju==null)
		{
			logger.error("msgOrderPauseNotic do not find userId:{},chargeOrderNo,{},remainAmt:{}", new Object[]{userId,chargeOrderNo,remainAmt});
			return ;
		}
		
		logger.info("[endCharge]msgOrderPauseNotic userId:{},chargeOrderNo:{}",userId,chargeOrderNo);
		
		String msg= String.format("您的订单{}已结算，结算余额{}元，已经返回到您的账户。", 
				chargeOrderNo,remainAmt);
		
		Map<String, String> extras = new HashMap<String, String>();
        extras.put( "msg", msg );
        extras.put( "orderid", ""+pkOrderId );
        extras.put("type", "12" );
        extras.put( "title", "充电订单正式结算" );
        extras.put( "tm", ""+DateUtil.getCurrentSeconds());
		
		JPushUtil.point2point("充电订单正式结算",msg,extras,ju.getJpushRegistrationid(),ju.getJpushDevicetype());
	}
	
	
	public static void jmsgPauseStat(int userId,String account,int pkOrderId,String chargeOrderNo)
	{
		TblJpush ju=DB.jpushDao.getByuserInfo(userId);
		if(ju==null)
		{
			logger.error("jmsgNoticePauseStat do not find userId:{},chargeOrderNo,{},remainAmt:{}", new Object[]{userId,account,pkOrderId,chargeOrderNo});
			return ;
		}
		
		logger.info("[endCharge]orderPauseNotic userId:{},chargeOrderNo:{}",userId,chargeOrderNo);
		
		String msg= String.format("{手机号}您好，您的订单{订单编号}临时结算。结算完成后，订单余额会返回到您的账户中，临时结算不影响充电。", 
				account,chargeOrderNo);
		
		Map<String, String> extras = new HashMap<String, String>();
        extras.put( "msg", msg );
        extras.put( "orderid", ""+pkOrderId );
        extras.put("type", "11" );
        extras.put( "title", "充电订单临时结算" );
        extras.put( "tm", ""+DateUtil.getCurrentSeconds());
		
		JPushUtil.point2point("充电订单临时结算",msg,extras,ju.getJpushRegistrationid(),ju.getJpushDevicetype());
	
		
	}
	public static void msgPauseStat(String curUserAccount)
	{
		logger.debug("onPauseStatNotic send msg,curUserAccount:{}",curUserAccount);
		
		if(GameConfig.sms == 1)
		{
			try
			{
				String content = MessageFormat.format("{0}您好，上一次充电未结算。结算完成后，订单余额会返回到您的账户中，不影响本次充电。",curUserAccount);
				MobiCommon.sendWanMatMessage(content,curUserAccount);
			}
			catch(Exception e)
			{
				logger.error("onPauseStatNotic fail,e.getMessage:{}",e.getMessage());
			}
			return ;
		}
		
		
		
		HashMap<String,Object>  params=new HashMap<String,Object>();
		params.put("mbcode", curUserAccount);
		
		JSONObject jsonObject = JSONObject.fromObject(params);
		
		boolean flag = AliSMS.sendAliSMS(curUserAccount, "SMS_25850225", jsonObject.toString());
		if(!flag)
		{
			logger.debug("onPuaseStatNotic fail,userAccount:{}",curUserAccount);
		}
		
	}
}
	
