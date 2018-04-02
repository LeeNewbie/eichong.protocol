package com.usrlayer.service;

import java.util.Iterator;
import java.util.List;
import java.util.Map;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.ormcore.dao.DB;
import com.ormcore.model.TblElectricPileGun;
import com.usrlayer.cache.EpGunCache;
import com.usrlayer.constant.EpConstant;

public class EpGunService {
	
	private static final Logger logger = LoggerFactory.getLogger(EpGunService.class);
	
	public static boolean checkWorkStatus(int status)
	{
		if(	status!= EpConstant.EP_GUN_W_STATUS_OFF_LINE && 
			status!= EpConstant.EP_GUN_W_STATUS_FAULT &&
			status!= EpConstant.EP_GUN_W_STATUS_IDLE &&
			status!= EpConstant.EP_GUN_W_STATUS_WORK &&
			status!= EpConstant.EP_GUN_W_STATUS_BESPOKE &&
			status!= EpConstant.EP_GUN_W_STATUS_UPGRADE &&
			status!= EpConstant.EP_GUN_W_STATUS_USER_OPER &&
			status!= EpConstant.EP_GUN_W_STATUS_SETTING &&
			status!= EpConstant.EP_GUN_W_STATUS_SELECT_CHARGE_MODE &&
			status!= EpConstant.EP_GUN_W_INIT && 
			status<(EpConstant.EP_GUN_W_STATUS_SELECT_CHARGE_MODE+1))//以后充电模式加了状态往后移
		{
			return false;
		}
		else
		{
			if(status> EpConstant.EP_GUN_W_STATUS_URGENT_STOP )
				return false;
		}
		return true;
	}
	
	public static int convertEpWorkStatus(int epWorStatus)
	{
		int ret=-1;
		switch(epWorStatus)
		{
		
		case EpConstant.EP_GUN_W_STATUS_OFF_LINE://离线
			ret = EpConstant.EP_GUN_STATUS_OFF_LINE;
			break;
		case EpConstant.EP_GUN_W_STATUS_FAULT://故障，停用
			ret = EpConstant.EP_GUN_STATUS_STOP_USE;
			break;
		case EpConstant.EP_GUN_W_STATUS_IDLE://空闲
			ret = EpConstant.EP_GUN_STATUS_IDLE;//空闲
			break;
		case EpConstant.EP_GUN_W_STATUS_WORK:// 工作(充电)
			ret = EpConstant.EP_GUN_STATUS_CHARGE;
			break;
		case EpConstant.EP_GUN_W_STATUS_BESPOKE://预约
			ret = EpConstant.EP_GUN_STATUS_BESPOKE_LOCKED;
			break;
		case EpConstant.EP_GUN_W_STATUS_UPGRADE:// 在线升级
			ret = EpConstant.EP_GUN_STATUS_EP_UPGRADE;
			break;
		case EpConstant.EP_GUN_W_STATUS_USER_OPER:// 操作中(待定,防止用户在操作中被预约)
			ret = EpConstant.EP_GUN_STATUS_EP_OPER;//操作
		    break;
		case EpConstant.EP_GUN_W_STATUS_SETTING://设置状态
			ret = EpConstant.EP_GUN_STATUS_SETTING;//设置
			break;
			
		case EpConstant.EP_GUN_W_STATUS_SELECT_CHARGE_MODE://充电模式选择
			ret = EpConstant.EP_GUN_STATUS_SELECT_CHARGE_MODE;
			break;
		default:
			if(epWorStatus>=EpConstant.EP_GUN_W_STATUS_LESS_VOL_FAULT && epWorStatus<=EpConstant.EP_GUN_W_STATUS_URGENT_STOP)
			{
				ret= EpConstant.EP_GUN_STATUS_STOP_USE;
			}
			
			break;
		}
		return ret;
	}
	
	public static boolean checkCarPlaceLockStatus(int status)
	{
		if(	status!= 0 && 
			status!= 1 &&
			status!= 2 &&
			status!= 3 &&
			status!= 4 )
		{
			return false;
		}
		return true;
	}
	
	public static boolean checkCardInfoAddr(int addr)
	{
		
			return false;
		
		
	}
	
	
	public static TblElectricPileGun getDbEpGun(int pkEpId,int epGunNo)
	{
		TblElectricPileGun tblEpGun= new TblElectricPileGun();
		tblEpGun.setPkEpId(pkEpId);
		tblEpGun.setEpGunNo(epGunNo);
		
		TblElectricPileGun dbEpGun=null;
		List<TblElectricPileGun> epGunList=DB.epGunDao.findEpGunInfo(tblEpGun);
		
		if(epGunList==null)
		{
			logger.error("getDbEpGun not find dbEpGun,epGunList==null!pkEpId:{},epGunNo:{}",pkEpId,epGunNo);
			return null;
		}
			
		if(epGunList.size()!=1)
		{
			logger.error("getDbEpGun not find dbEpGun,pkEpId:{},epGunList.size:{}",pkEpId,epGunList.size());
			return null;
		}
		
		dbEpGun = epGunList.get(0);
		return dbEpGun;
	}

	
	@SuppressWarnings("rawtypes")
	public static void checkTimeout()
	{
		
		Iterator iter = CacheService.getMapGun().entrySet().iterator();
		
		while (iter.hasNext()) {

			Map.Entry entry = (Map.Entry) iter.next();
			
			EpGunCache epGunCache=(EpGunCache) entry.getValue();
			if(null == epGunCache )
			{
				logger.info("checkTimeout: epGunCache=null:\n");
				continue;
			}
			
			
		}
	}
	
	public static String getGunStatusDesc(int status)
	{
		switch(status)
		{
		case EpConstant.EP_GUN_STATUS_IDLE:
			return "空闲";
		case EpConstant.EP_GUN_STATUS_EP_INIT:
			return "电桩初始化中";
		case EpConstant.EP_GUN_STATUS_BESPOKE_LOCKED:
			return "预约锁定中";
		case EpConstant.EP_GUN_STATUS_CHARGE:
			return "充电中";
		case EpConstant.EP_GUN_STATUS_STOP_USE:
			return "停用";
		case EpConstant.EP_GUN_STATUS_EP_OPER:
			return "电桩有人使用中";
		
		case EpConstant.EP_GUN_STATUS_USER_AUTH:
			return "用户占用";
		case EpConstant.EP_GUN_STATUS_SETTING:
			return "设置界面";
		case EpConstant.EP_GUN_STATUS_SELECT_CHARGE_MODE:
			return "充电模式选择";
		case EpConstant.EP_GUN_STATUS_EP_UPGRADE:
			return "升级中";
		case EpConstant.EP_GUN_STATUS_OFF_LINE:
			return "离线状态";
			
		default:
			return "未知状态("+status +")";
		}
	}

	public static void updateGunState(int pkEpGunId,int status)
	{
		TblElectricPileGun info= new TblElectricPileGun();
		info.setPkEpGunId(pkEpGunId);
		info.setEpState(status);
		DB.epGunDao.updateGunState(info);
	}
	  
}
