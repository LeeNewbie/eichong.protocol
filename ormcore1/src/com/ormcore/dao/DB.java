package com.ormcore.dao;

import com.ormcore.cache.GameContext;
import com.ormcore.dao.BespokeDao;
import com.ormcore.dao.BomListDao;
import com.ormcore.dao.ChargeCardDao;
import com.ormcore.dao.ChargingOrderDao;
import com.ormcore.dao.ChargingfaultrecordDao;
import com.ormcore.dao.ChargingrecordDao;
import com.ormcore.dao.EpGunDao;
import com.ormcore.dao.EquipmentRepairDao;
import com.ormcore.dao.PurchaseHistoryDao;
import com.ormcore.dao.RateInfoDao;
import com.ormcore.dao.TblChargeACInfoDao;
import com.ormcore.dao.TblChargeDCInfoDao;
import com.ormcore.dao.TblConcentratorDao;
import com.ormcore.dao.TblElectricPileDao;
import com.ormcore.dao.TblEquipmentVersionDao;
import com.ormcore.dao.TblPartnerDao;
import com.ormcore.dao.TblPartnerTimeDao;
import com.ormcore.dao.TblPowerModuleDao;
import com.ormcore.dao.TblUserInfoDao;
import com.ormcore.dao.TblVehicleBatteryDao;
import com.ormcore.dao.TypeSpanDao;

public class DB {
	public static RateInfoDao rateInfoDao = (RateInfoDao) GameContext.getBean("rateInfoDao");
		
	public static TblUserInfoDao userInfoDao = (TblUserInfoDao) GameContext.getBean("tblUserInfoDao");
	
	public static TblElectricPileDao epClientDao = (TblElectricPileDao) GameContext.getBean("tblElectricPileDao");

	public static BespokeDao bespDao = (BespokeDao) GameContext.getBean("bespokeDao");
	
	public static ChargingOrderDao chargeOrderDao = (ChargingOrderDao) GameContext.getBean("chargingOrderDao");
	
	public static PurchaseHistoryDao phDao = (PurchaseHistoryDao) GameContext.getBean("purchaseHistoryDao");
	
	public static EpGunDao epGunDao = (EpGunDao) GameContext.getBean("epGunDao");
	
	public static ChargingrecordDao chargingrecordDao = (ChargingrecordDao) GameContext.getBean("chargingrecordDao");
	
	public static ChargingfaultrecordDao chargingfaultrecordDao = (ChargingfaultrecordDao) GameContext.getBean("chargingfaultrecordDao");
	
	
    public static TblConcentratorDao concentratorDao = (TblConcentratorDao) GameContext.getBean("tblConcentratorDao");
    
    //public static TblPowerStationDao powerStationDao = (TblPowerStationDao) GameContext.getBean("tblPowerStationDao");

    public static TblPartnerDao partnerDao = (TblPartnerDao) GameContext.getBean("tblPartnerDao");
    
    public static TblPartnerTimeDao partnerTimeDao = (TblPartnerTimeDao) GameContext.getBean("tblPartnerTimeDao");

    public static TblEquipmentVersionDao equipmentVersionDao = (TblEquipmentVersionDao) GameContext.getBean("tblEquipmentVersionDao");

    public static ChargeCardDao chargeCardDao = (ChargeCardDao) GameContext.getBean("chargeCardDao");
   
    public static TblChargeACInfoDao chargeACInfoDao = (TblChargeACInfoDao) GameContext.getBean("tblChargeACInfoDao");
	
	public static TblChargeDCInfoDao chargeDCInfoDao = (TblChargeDCInfoDao) GameContext.getBean("tblChargeDCInfoDao");
	
    public static BomListDao bomListDao = (BomListDao) GameContext.getBean("bomListDao");
    
    public static TypeSpanDao typeSpanDao = (TypeSpanDao) GameContext.getBean("typeSpanDao");
    
    public static EquipmentRepairDao equipmentRepairDao = (EquipmentRepairDao) GameContext.getBean("equipmentRepairDao");
    
    public static TblVehicleBatteryDao vehicleBatteryDao = (TblVehicleBatteryDao) GameContext.getBean("tblVehicleBatteryDao");

    public static TblPowerModuleDao powerModuleDao = (TblPowerModuleDao) GameContext.getBean("tblPowerModuleDao");

    
}
