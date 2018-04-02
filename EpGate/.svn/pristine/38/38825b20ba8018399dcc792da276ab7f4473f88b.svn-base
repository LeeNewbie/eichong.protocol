package com.epcentre.cache;

import java.math.BigDecimal;
import java.text.MessageFormat;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;

import net.sf.json.JSONObject;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.cooperate.OrgSendConfig;
import com.cooperate.Push;
import com.cooperate.RealDataRT;
import com.cooperate.measurePoint;
import com.cooperate.real3rdFactory;
import com.epcentre.config.GameConfig;
import com.epcentre.config.Global;
import com.epcentre.constant.ChargeStatus;
import com.epcentre.constant.EpConstant;
import com.epcentre.constant.EpConstantErrorCode;
import com.epcentre.constant.EpProtocolConstant;
import com.epcentre.constant.EventConstant;
import com.epcentre.constant.UserConstant;
import com.epcentre.dao.DB;
import com.epcentre.epconsumer.ChinaMobileService;
import com.epcentre.epconsumer.StopCarOrganService;
import com.epcentre.model.RateInfo;
import com.epcentre.model.TblBespoke;
import com.epcentre.model.TblChargingrecord;
import com.epcentre.model.TblElectricPileGun;
import com.epcentre.net.message.AliSMS;
import com.epcentre.net.message.MobiCommon;
import com.epcentre.protocol.ChargeCmdResp;
import com.epcentre.protocol.ChargeEvent;
import com.epcentre.protocol.EpBespResp;
import com.epcentre.protocol.EpCancelBespResp;
import com.epcentre.protocol.EpEncodeProtocol;
import com.epcentre.protocol.Iec104Constant;
import com.epcentre.protocol.NoCardConsumeRecord;
import com.epcentre.protocol.SingleInfo;
import com.epcentre.protocol.UtilProtocol;
import com.epcentre.protocol.WmIce104Util;
import com.epcentre.sender.EpMessageSender;
import com.epcentre.service.AnalyzeService;
import com.epcentre.service.AppApiService;
import com.epcentre.service.ChargingInfo;
import com.epcentre.service.EpBespokeService;
import com.epcentre.service.EpChargeService;
import com.epcentre.service.EpGunService;
import com.epcentre.service.EpService;
import com.epcentre.service.PhoneService;
import com.epcentre.service.RateService;
import com.epcentre.service.StatService;
import com.epcentre.service.UserService;
import com.epcentre.test.ImitateEpService;
import com.epcentre.utils.DateUtil;
import com.epcentre.utils.StringUtil;

public class EpGunCache {
	
	private static final Logger logger = LoggerFactory.getLogger(EpGunCache.class);
	
	private int concentratorId;

	private int pkEpId; 
	
	private String  epCode;
	
	private int pkEpGunId;
	
	private int epGunNo;
	
	int currentType;
	
	private int status;
	
	private INetObject epNetObject;//电桩网络连接
	
	private BespCache bespCache;
	
	private ChargeCache chargeCache;
	
	private AuthUserCache authCache;
	
	RealChargeInfo realChargeInfo;
	
	
	private String identyCode;// 识别码
	
	private long createIdentyCodeTime;//生成识别码的时间
	
	
	private long lastUDTime;//更新到数据库的信息
	
	private long lastUPTime;//手机更新时间
	
		
	private long lastSendToMonitorTime; //记录上一次发送给监控系统的时间
	
	RealDataRT sendInfo3rd=null;	
	
	//充电时 保存发送到监控中心的遥测和变遥测 
	Map<Integer, SingleInfo> changeYcMap= new ConcurrentHashMap<Integer, SingleInfo>();
	Map<Integer, SingleInfo> changeVarYcMap= new ConcurrentHashMap<Integer, SingleInfo>();
	
	private boolean statusChangeOfMonitor=false;
	
	public boolean isStatusChangeOfMonitor() {
		return statusChangeOfMonitor;
	}
	public void setStatusChangeOfMonitor(boolean statusChangeOfMonitor) {
		this.statusChangeOfMonitor = statusChangeOfMonitor;
	}
	
	public long getLastSendToMonitorTime() {
		return lastSendToMonitorTime;
	}
	public AuthUserCache getAuthCache() {
		return authCache;
	}
	public void setAuthCache(AuthUserCache authCache) {
		this.authCache = authCache;
	}
	
	

	public void setLastSendToMonitorTime(long lastSendToMonitorTime) {
		this.lastSendToMonitorTime = lastSendToMonitorTime;
	}

	public EpGunCache(){
		
		concentratorId=0;

		pkEpId=0; 
		
		epCode="";
		
		pkEpGunId=0;
		
		epGunNo=0;
		
		currentType=0;
		
		status=0;
		
		epNetObject=null;//电桩网络连接
		
		bespCache=null;
		
		chargeCache=null;
		
		authCache=null;
		
		realChargeInfo=null;

		identyCode="";// 识别码
		
		createIdentyCodeTime=0;//生成识别码的时间
		
		lastUDTime=0;//更新到数据库的信息
		
		lastUPTime=0;//手机更新时间
		
		lastSendToMonitorTime=0; //记录上一次发送给监控系统的时间
	}
	
	
	public long getCreateIdentyCodeTime() {
		return createIdentyCodeTime;
	}
	public void setCreateIdentyCodeTime(long createIdentyCodeTime) {
		this.createIdentyCodeTime = createIdentyCodeTime;
	}
	
	public String getIdentyCode() {
		return identyCode;
	}
	
	public void setIdentyCode(String identyCode) {
		this.identyCode = identyCode;
	}
	
	
	public int getPkEpGunId() {
		return pkEpGunId;
	}

	public void setPkEpGunId(int pkEpGunId) {
		this.pkEpGunId = pkEpGunId;
	}

	public int getConcentratorId() {
		return concentratorId;
	}

	public void setConcentratorId(int concentratorId) {
		this.concentratorId = concentratorId;
	}

	public int getPkEpId() {
		return pkEpId;
	}
	

	

	public INetObject getEpNetObject() {
		return epNetObject;
	}

	public void setEpNetObject(INetObject epNetObject) {
		onNetStatus(1);
		this.epNetObject = epNetObject;
	}

	public void setPkEpId(int pkEpId) {
		this.pkEpId = pkEpId;
	}

	public String getEpCode() {
		return epCode;
	}

	public void setEpCode(String epCode) {
		this.epCode = epCode;
	}

	public int getEpGunNo() {
		return epGunNo;
	}

	public void setEpGunNo(int epGunNo) {
		this.epGunNo = epGunNo;
	}

	public int getStatus() {
		return status;
	}

	public BespCache getBespCache() {
		
		 
		 BespCache retBespCache= null;
		 retBespCache = this.bespCache;
         //释放读锁   
		return retBespCache;
	}
	

	public void setBespCache(BespCache bespCache) {
        this.bespCache = bespCache; 
	}

	public ChargeCache getChargeCache() {
		
		return chargeCache;
	}

	public void setChargeCache(ChargeCache chargeCache) {
		 
		
		this.chargeCache = chargeCache;
	}

	public RealChargeInfo getRealChargeInfo() {
		return realChargeInfo;
	}

	public void setRealChargeInfo(RealChargeInfo realChargeInfo) {
		this.realChargeInfo = realChargeInfo;
	}


	public int checkSingleYx(int value)
	{
		int ret=0;
		if(value!=0 && value!=1)
		{
			ret = -1;
		}
		return ret;
	}
	
	public int get_gun2carLinkStatus()
	{
		return this.realChargeInfo.getLinkCarStatus();
	}
	
	
	/**
	 * 处理充电中的车VIN码
	 */
	public void checkChargeCarVin()
	{
		int currentType = realChargeInfo.getCurrentType();
		
		if(this.chargeCache==null|| currentType== EpConstant.EP_AC_TYPE)
			return ;
		ChargeCarInfo carInfo = ((RealDCChargeInfo)realChargeInfo).getChargeCarInfo();
		if(carInfo==null)
			return ;
			
		String carIdenty= carInfo.getCarIdenty();
		if(carIdenty==null || carIdenty.length()<=0)
			return ;
		if(carIdenty.equals(chargeCache.getCarVin()))
			return ;
		
		logger.info("charge realData epCode:{},epGunNo:{},carVin:{},carVin(hex):{}",new Object[]{epCode,epGunNo,carIdenty,WmIce104Util.ConvertHex2(carIdenty.getBytes())});

		chargeCache.setCarVin(carIdenty);
		
		EpChargeService.updateChargeThirdtoDB(epCode,epGunNo,carIdenty,chargeCache);
	}
	
	public void onRealDataChange(Map<Integer, SingleInfo> pointMap,int type)
	{
		if(type!=1 && type!=3 && type!=11 &&type!=132)
		{
			logger.error("onRealDataChange type error type:{},epCode:{},epGunNo:{}",
					new Object[]{type,epCode,epGunNo});
			return;
		}
		int currentType = realChargeInfo.getCurrentType();
		if(currentType!= EpConstant.EP_AC_TYPE && currentType !=  EpConstant.EP_DC_TYPE )
		{
			logger.error("onrealDataChange currentType error,epCode:{},epGunNo:{},currentType:{}",
					new Object[]{epCode,epGunNo,currentType});
			return ;
		}
		
		
		int oldEpWorkStatus= this.realChargeInfo.getWorkingStatus();
		int oldGun2CarStatus = this.realChargeInfo.getLinkCarStatus();
		

		Map<Integer, SingleInfo> changePointMap= new ConcurrentHashMap<Integer, SingleInfo>();
		//1.处理实时信息到内存
		handleRealData(pointMap,type,changePointMap);
		//2.处理枪状态
		int newGun2CarStatus = this.realChargeInfo.getLinkCarStatus();
		if(newGun2CarStatus!=oldGun2CarStatus)
		{
			logger.debug("newGun2CarStatus:{},oldGun2CarStatus:{}",newGun2CarStatus,oldGun2CarStatus);
			this.handleGun2CarLinkStatus(newGun2CarStatus);
		}
		//3.处理车架号
		checkChargeCarVin();
	
		int newStatus=-1;
		
		//4.处理监控，第三方,用户端充电实时信息
		//如果电桩重来没有更新过工作状态，那么不处理工作状态
		if(realChargeInfo!=null && this.realChargeInfo.getWorkStatusUpdateTime()>0)//
		{
			newStatus = EpGunService.convertEpWorkStatus(this.realChargeInfo.getWorkingStatus());
			SingleInfo workPoint =pointMap.get(EpProtocolConstant.YC_WORKSTATUS);
			if(newStatus!=-1 && workPoint != null)
			{
				this.onStatusChange(newStatus);
			}
			//4.2
			onRealDataChangeToMonitor(oldEpWorkStatus,changePointMap,type);
			//e租网
			if(checkOrgNo1010()==1 && oldEpWorkStatus!=this.realChargeInfo.getWorkingStatus())
			{
				this.handleSignleOrgNo(1010, true);
			}
		
			//4.3
			//handleSCO(changePointMap,type,oldEpWorkStatus,this.realChargeInfo.getWorkingStatus());

			//4.4
			handleUserRealInfo();
			
		}
		
	}
	public int checkOrgNo1010()
	{
		String strArea = this.epCode.substring(0,4);
		if(!strArea.equals("1110") )//不是北京地区的不上报
		{
			return 0;
		}
		ElectricPileCache epCache = EpService.getEpByCode(epCode);
		if(epCache==null)
		{
			return 0;
		}
		if(epCache.getState()!=15 || epCache.getDeleteFlag()==1)
		{
			return 0;
		}
		
		return 1;
	}
	
	/**
	 * 处理实时数据到内存对象
	 * @param pointMap
	 * @param type
	 * @param changePointMap
	 */
	private void handleRealData(Map<Integer, SingleInfo> pointMap,int type,Map<Integer, SingleInfo> changePointMap)
	{
		Iterator iter = pointMap.entrySet().iterator();
		while (iter.hasNext()) {
			Map.Entry entry = (Map.Entry) iter.next();
			
			int pointAddr = ((Integer)entry.getKey()).intValue();
			SingleInfo info=(SingleInfo) entry.getValue();
			int ret=0;
			
			if(currentType== EpConstant.EP_AC_TYPE)
				ret = ((RealACChargeInfo)realChargeInfo).setFieldValue(pointAddr,info);
			else
				ret = ((RealDCChargeInfo)realChargeInfo).setFieldValue(pointAddr,info);
			
			if((type==3) && info.getAddress()>EpProtocolConstant.YX_2_START_POS&& info.getAddress()<EpProtocolConstant.YC_START_POS)
			{
				info.setAddress(info.getAddress()-EpProtocolConstant.YX_2_START_POS);
			}
			else if((type==11 ) && info.getAddress()>EpProtocolConstant.YC_START_POS&& info.getAddress()<EpProtocolConstant.YC_VAR_START_POS)
			{
				info.setAddress(info.getAddress()-EpProtocolConstant.YC_START_POS);
			}
			else if((type==132) && info.getAddress()>EpProtocolConstant.YC_VAR_START_POS)
			{
				info.setAddress(info.getAddress()-  EpProtocolConstant.YC_VAR_START_POS);
			}
			
			if(ret==-1)
			{
				logger.info("onrealDataChange,epCode:{},epGunNo:{},address:{},value:{} setFieldValue value invalid",
						new Object[]{epCode,epGunNo,pointAddr,info.getIntValue()});			
			}
			else if(ret ==-3)
			{
					logger.info("onrealDataChange,epCode:{},epGunNo:{},address:{},setFieldValue address invalid",
						new Object[]{epCode,epGunNo,pointAddr});	
			}
			else if(ret == -2)
			{
					logger.debug("onrealDataChange,epCode:{},epGunNo:{},address:{},setFieldValue reserved address",
						new Object[]{epCode,epGunNo,pointAddr});
			}
			else if(ret==1)
			{
				if(type==1 || type==3 || type==11|| type==132 )
				{
					changePointMap.put(info.getAddress(),info);
					if( type==11)
					{
						changeYcMap.put(info.getAddress(),info);
					}
					else if(type==132)
					{
						changeVarYcMap.put(info.getAddress(),info);
					}
				}
			}
		}
	}
	
	private void handleUserRealInfo()
	{
		if(this.status == EpConstant.EP_GUN_STATUS_CHARGE)
		{	
			saveRealInfoToDbWithCharge();
			
			if(chargeCache ==null)
			{
				logger.error("handleUserRealInfo chargeCache ==null epCode:{},epGunNo:{}",epCode,epGunNo);
				return;
			}
			
			Map<String ,Object> respMap = new ConcurrentHashMap<String, Object>();
			respMap.put("epcode", epCode);
			respMap.put("epgunno", epGunNo);
			
			ChargingInfo  chargingInfo = getCharingInfo();
			
			if(chargingInfo == null)
			{
				return ;
			}
			
			ChargeCardCache cardCache=chargeCache.getCard();
			if(cardCache!=null && cardCache.getCompanyNumber() == 5)
			{
				handleEventExtra(EventConstant.EVENT_REAL_CHARGING,cardCache.getCompanyNumber(),0,0,respMap,(Object)chargingInfo);
			}
			else
			{
				handleEvent(EventConstant.EVENT_REAL_CHARGING,0,0,respMap,(Object)chargingInfo);
			}
			
		}
		else
		{
			saveRealInfoToDb();
		}
		
	}


	
	private void sendToMonitor(int eventType,UserOrigin userOrigin,int ret,int cause, Object changePointMap)
	{
		Map<String ,Object> paramsMap = new ConcurrentHashMap<String, Object>();
		paramsMap.put("epcode", epCode);
		paramsMap.put("epgunno", epGunNo);
		paramsMap.put("currenttype", this.realChargeInfo.getCurrentType());
		
		AnalyzeService.onEvent(eventType,userOrigin,0,0,(Object)paramsMap,(Object)changePointMap);
		
	}
	
	
	public void sendRealChangeYxtoMonitor(Map<Integer, SingleInfo> changePointMap,int type,int oldEpWorkStatus, int newEpWorkStatus)
	{
		if(type!=1 && type!=3)
		{
			logger.error("sendRealChangeYxtoMonitor,invalid type:{},epCode:{},epGunNo:{}",
					new Object[]{type,epCode,epGunNo});
			return ;
		}
		
		int changeSize = changePointMap.size();
		if(changeSize<1)
		{
			//logger.debug("sendRealChangeYxtoMonitor,realData,type:{},epCode:{},epGunNo:{},changeSize<1",
			//		new Object[]{type,epCode,epGunNo});
			return;
		}
		if(type==1)
		{
			sendToMonitor(EventConstant.EVENT_ONE_BIT_YX,null,0,0,(Object)changePointMap);
			
		}
		else if(type==3)
		{
			sendToMonitor(EventConstant.EVENT_TWO_BIT_YX,null,0,0,(Object)changePointMap);
			  
		}
	}
	
	public void sendRealChangeYctoMonitor( int newEpWorkStatus)
	{
		if(newEpWorkStatus != 3 )
		{
			Map<Integer, SingleInfo> carLockMap= new ConcurrentHashMap<Integer, SingleInfo>();
			SingleInfo info = changeYcMap.get(2);
			if(info!=null)
			{
				carLockMap.put(2, info);
				sendToMonitor(EventConstant.EVENT_YC,null,0,0,(Object)carLockMap);
				changeYcMap.remove(2);
			}
			return ;
		}
		long now  = DateUtil.getCurrentSeconds();
		long diff = now - this.lastSendToMonitorTime;
		if(diff> GameConfig.montiorTimeInterval)
		{
			if(changeYcMap.size()>0)
				sendToMonitor(EventConstant.EVENT_YC,null,0,0,(Object)changeYcMap);
			if(changeVarYcMap.size()>0)
				sendToMonitor(EventConstant.EVENT_VAR_YC,null,0,0,(Object)changeVarYcMap);
			changeYcMap.clear();
			changeVarYcMap.clear();
			
			lastSendToMonitorTime = now;
		}
	}
	
	public ChargingInfo getCharingInfo()
	{
		if(chargeCache==null)
		{
			return null;
		}
		long now = DateUtil.getCurrentSeconds();
		long diff = now - this.lastUPTime;
		if(diff>GameConfig.chargeInfoUPTime)//三分钟
		{
			this.lastUPTime= now;
			
			ChargingInfo charingInfo= calcCharingInfo();
			
			return charingInfo;
		}
		return null;
	}
	
	public void saveRealInfoToDb()
	{
		long now=DateUtil.getCurrentSeconds();
		long diff = DateUtil.getCurrentSeconds() - this.lastUDTime;
		if(diff>GameConfig.saveRealInfoToDbInterval)//三分钟
		{
			lastUDTime = now;
			int currentType = realChargeInfo.getCurrentType();
			if(currentType == EpConstant.EP_AC_TYPE){
				((RealACChargeInfo)realChargeInfo).saveDb();
			}
			else
			{		
				((RealDCChargeInfo)realChargeInfo).saveDb();
			}
		}
	}
	public void saveRealInfoToDbWithCharge()
	{
		long now=DateUtil.getCurrentSeconds();
		long diff = now - this.lastUDTime;
		if(diff<GameConfig.saveRealInfoToDbInterval)//三分钟
			return ;
		
		lastUDTime = now;
		int currentType = realChargeInfo.getCurrentType();
		if(currentType == EpConstant.EP_AC_TYPE){
			((RealACChargeInfo)realChargeInfo).saveDb();
		}
		else
		{		
			((RealDCChargeInfo)realChargeInfo).saveDb();
		}
			
		if(realChargeInfo.getWorkingStatus() == EpConstant.EP_GUN_W_STATUS_WORK&&
				currentType == EpConstant.EP_DC_TYPE)
		{
			if(chargeCache ==null)
				return;
			String chargeSerialNo= chargeCache.getChargeSerialNo();
			
			((RealDCChargeInfo)realChargeInfo).saveChargeCarInfoToDB(chargeSerialNo);
			((RealDCChargeInfo)realChargeInfo).savePowerModuleToDB(chargeSerialNo);
		}
		
	}
	public void onRedoBespokeSuccess()
	{
		logger.info("[bespoke]onRedobespokeSuccess,epCode:{},epGunNo:{},status:{},Redo:{}",
				new Object[]{epCode,epGunNo,this.status,bespCache.getRedo()});
		if(bespCache.getRedo()!=1)
		{
			return ;
		}
		
		bespCache.onRedoBespokeSuccess();
		
		EpBespokeService.updateRedoBespokeToDb(bespCache.getBespId(), bespCache);
		
	
		
		String messagekey = String.format("%03d%s", Iec104Constant.C_BESPOKE, bespCache.getBespNo());
		EpGunService.removeRepeatMsg(messagekey);  
	
		 do_bespoke_consume_resp(1,0,bespCache.getAccountId(),1,bespCache.getBespNo());
  	
	}
	/**
	 * 1:根据预约应答成功
	 * 2：根据状态预约成功
	 * @param method
	 */
	public void onBespokeSuccess(int method)
	{
		logger.info("[bespoke]onbespokeSuccess,epCode:{},epGunNo:{},method:{}",new Object[]{this.epCode,this.epGunNo,method});
		
		if(bespCache!=null && bespCache.getStatus()!= EpConstant.BESPOKE_STATUS_LOCK)
		{
			StatService.addBespoke();
			
			String messagekey = String.format("%03d%s", Iec104Constant.C_BESPOKE, bespCache.getBespNo());
			
			EpGunService.removeRepeatMsg(messagekey);
			
			
			bespCache.onBespokeSuccess();
			
			long pkBespId = EpBespokeService.insertBespokeToDb(this.pkEpId, this.pkEpGunId, bespCache);	
			bespCache.setBespId(pkBespId);
			
			UserCache u= UserService.getUserCache(bespCache.getAccount());
			
			u.addBesp(bespCache);
			
			UserService.putUserCache(u);
            
            do_bespoke_consume_resp(1,0,bespCache.getAccountId(),0,bespCache.getBespNo());
    		
		   
		}
		
	}
	
	public void onBespokeFail(int errorCode)
	{
		logger.info("[bespoke]onBespokeFail,epCode:{},epGunNo:{},errorCode:{}",new Object[]{this.epCode,this.epGunNo,errorCode});
		
		if(bespCache!=null && bespCache.getStatus()== EpConstant.BESPOKE_STATUS_CONFIRM)
		{
			StatService.subBespoke();
			
			String messagekey = String.format("%03d%s", Iec104Constant.C_BESPOKE,bespCache.getBespNo());
			
			EpGunService.removeRepeatMsg(messagekey);
			
			bespCache.setStatus(EpConstant.BESPOKE_STATUS_FAIL);

			//通知前端
			Map<String, Object> chargeMap = new ConcurrentHashMap<String, Object>();
			
			chargeMap.put("bespNo", bespCache.getBespNo());
			chargeMap.put("redo", bespCache.getRedo());
			chargeMap.put("userId", bespCache.getAccountId());
			
			handleEvent(EventConstant.EVENT_BESPOKE,0,errorCode,null,chargeMap);
		}
		
		cleanBespokeInfo();
	}
	
	public void onEndBespoke()
	{
		logger.debug("onEndBespoke");
		if(this.bespCache!=null)
		{
			long now = DateUtil.getCurrentSeconds();
			long diff = now - this.bespCache.getStartTime();
			if(diff >GameConfig.bespokeCmdTime)
			{
				endBespoke( EpConstant.END_BESPOKE_CANCEL );
			}
		}
	}
	
	public void onEndCharge()
	{
		logger.debug("onEndCharge");
	}

	
	public void onStartChargeSuccess()
	{
		if(chargeCache!=null&& chargeCache.getStatus() == ChargeStatus.CS_ACCEPT_CONSUMEER_CMD )
		{
			chargeCache.setStatus(ChargeStatus.CS_WAIT_INSERT_GUN);
			
			String messagekey = String.format("%03d%s", Iec104Constant.C_START_ELECTRICIZE,chargeCache.getChargeSerialNo());
			EpGunService.removeRepeatMsg(messagekey);
			
			
			TblChargingrecord record = new TblChargingrecord();
			record.setChreTransactionnumber(chargeCache.getChargeSerialNo());
			record.setStatus(ChargeStatus.CS_WAIT_INSERT_GUN);
			record.setThirdCode(chargeCache.getThirdCode());
			record.setThirdType(chargeCache.getThirdType());
			
			DB.chargingrecordDao.updateBeginRecordStatus(record);
			
			Map<String, Object> chargeMap = new ConcurrentHashMap<String, Object>();
			
			chargeMap.put("epcode", epCode);
			chargeMap.put("epgunno", epGunNo);
			
			//logger.debug("onStartChargeSuccess EventConstant.EVENT_CHARGE,epCode:{},epGunNo:{}",epCode,epGunNo);
			handleEvent(EventConstant.EVENT_CHARGE,1,0,null,chargeMap);
			
			logger.info("charge reponse success accountId:{},serialNo:{},epCode:{},epGunNo:{}",
					new Object[]{chargeCache.getUserId(),chargeCache.getChargeSerialNo(),epCode,epGunNo});
			
		}
		
		
	}
	
	public void onInsertGunSuccess()
	{
		if(chargeCache!=null&& (chargeCache.getStatus() == ChargeStatus.CS_ACCEPT_CONSUMEER_CMD ||
				chargeCache.getStatus() == ChargeStatus.CS_WAIT_INSERT_GUN))
		{
			chargeCache.setStatus(ChargeStatus.CS_WAIT_CHARGE);
			
			String messagekey = String.format("%03d%s", Iec104Constant.C_START_ELECTRICIZE,chargeCache.getChargeSerialNo());
			EpGunService.removeRepeatMsg(messagekey);
			
			
			TblChargingrecord record = new TblChargingrecord();
			record.setChreTransactionnumber(chargeCache.getChargeSerialNo());
			record.setStatus(ChargeStatus.CS_WAIT_CHARGE);
			record.setThirdCode(chargeCache.getThirdCode());
			record.setThirdType(chargeCache.getThirdType());
			
			DB.chargingrecordDao.updateBeginRecordStatus(record);
			
			Map<String, Object> chargeMap = new ConcurrentHashMap<String, Object>();
			
			chargeMap.put("epcode", epCode);
			chargeMap.put("epgunno", epGunNo);
			
			//logger.debug("onStartChargeSuccess EventConstant.EVENT_CHARGE,epCode:{},epGunNo:{}",epCode,epGunNo);
			handleEvent(EventConstant.EVENT_CHARGE,1,0,null,chargeMap);
			
			logger.info("charge reponse success accountId:{},serialNo:{},epCode:{},epGunNo:{}",
					new Object[]{chargeCache.getUserId(),chargeCache.getChargeSerialNo(),epCode,epGunNo});
			
		}
		
		
	}
	
	public void onStartChargeFail(int method,int errorCode)
	{
		logger.debug("onStartChargeFail,epCharge,epCode:{},epGunNo:{},method:{},errorCode:{}",
				new Object[]{epCode,epGunNo,method,errorCode});
	
		//电桩接受充电失败.
		if(chargeCache!=null && chargeCache.getStatus()== ChargeStatus.CS_ACCEPT_CONSUMEER_CMD) // 没有在充电状态
		{
			int userId= chargeCache.getUserId();
			
			String messagekey = String.format("%03d%s", Iec104Constant.C_START_ELECTRICIZE,chargeCache.getChargeSerialNo());
			EpGunService.removeRepeatMsg(messagekey);
			
			//1.退钱和修改状态
			EpChargeService.onChargeFail(userId,chargeCache);
			
			//2.通知前端
			Map<String, Object> chargeMap = new ConcurrentHashMap<String, Object>();
			
			chargeMap.put("epcode", epCode);
			chargeMap.put("epgunno", epGunNo);
			
			logger.info("charge reponse fail accountId:{},serialNo:{},epCode:{},epGunNo:{},method:{},errorCode:{}",
					new Object[]{chargeCache.getUserId(),chargeCache.getChargeSerialNo(),epCode,epGunNo,method,errorCode});
			
			handleEvent(EventConstant.EVENT_CHARGE,0,errorCode,null,chargeMap);
			
			cleanChargeInfo();
		}
	}
	/**
	 * 1:表示通过电桩充电事件
	 * 2：表示通过状态
	 * @param method
	 */
	
	public int  onStartChargeEventFail(int method,int errorCode)
	{
		logger.debug("onStartChargeEventFail,epCharge,epCode:{},epGunNo:{},method:{},chargeCache:{}",
				new Object[]{this.epCode,this.epGunNo,method,chargeCache});
		
		if(chargeCache != null && (chargeCache.getStatus() == ChargeStatus.CS_WAIT_INSERT_GUN||
				chargeCache.getStatus() == ChargeStatus.CS_WAIT_CHARGE))
		{
			int userId= chargeCache.getUserId();
		
			//1.退钱和修改状态
			EpChargeService.onChargeFail(userId,chargeCache);
			
			//2.清空实时表信息
			BigDecimal bdZero = new BigDecimal(0.0);
			EpGunService.updateChargeInfoToDbByEpCode(this.currentType,this.epCode,this.epGunNo,
					bdZero,"",bdZero,0,0);
			
			//3.跟前段应答
			Map<String ,Object> respMap = new ConcurrentHashMap<String, Object>();
			respMap.put("epcode", epCode);
			respMap.put("epgunno", epGunNo);
			respMap.put("account",chargeCache.getAccount());
			respMap.put("userId",chargeCache.getUserId());
			
			handleEvent(EventConstant.EVENT_START_CHARGE_EVENT,0,errorCode,null,respMap);
			
			logger.info("charge event fail accountId:{},serialNo:{},epCode:{},epGunNo:{},method:{},errorCode:{}",
					new Object[]{chargeCache.getUserId(),chargeCache.getChargeSerialNo(),epCode,epGunNo,method,errorCode});
			
			cleanChargeInfo();
			
			
			
			return 1;
		}
		else
		{
			return 2;
		}

		
	}
	private void cleanIdentyCode()
	{
		EpGunService.clearIdentyCode(this.pkEpGunId);
		 
		 this.setIdentyCode("");
		 this.setCreateIdentyCodeTime(0);
	}
	/**
	 * method,1:通过事件
	 *        2：通过状态
	 * @param method
	 */
	public int onStartChargeEventSuccess(int method,long st)
	{
		logger.debug("onStartChargeEventSuccess,method:{},epCode:{},epGunNo:{}",new Object[]{method,epCode,epGunNo});
		
		if(chargeCache!=null && chargeCache.getStatus()!= ChargeStatus.CS_CHARGING)
		{
			chargeCache.setStatus(ChargeStatus.CS_CHARGING);
			
			String bespokeNo= StringUtil.repeat("0", 12);
			if(this.bespCache!=null)
			{
				bespokeNo =this.bespCache.getBespNo();
				//结束预约
				this.endBespoke(EpConstant.END_BESPOKE_CHARGE);
			}
			
			//给前段应答
			Map<String ,Object> respMap = new ConcurrentHashMap<String, Object>();
			respMap.put("epcode", epCode);
			respMap.put("epgunno", epGunNo);
			respMap.put("account",chargeCache.getAccount());
			respMap.put("userId",chargeCache.getUserId());
			
				
			handleEvent(EventConstant.EVENT_START_CHARGE_EVENT,1,0,null,respMap);
			
			UserCache u= UserService.getUserCache(chargeCache.getAccount());
			ChargeCache  historyCharge= u.getHistoryCharge(chargeCache.getEpCode()+ chargeCache.getEpGunNo());
			//logger.info("getHistoryCharge,{}",historyCharge);
			if(historyCharge!=null)
			{
				EpChargeService.pauseStatCharge(historyCharge);
				u.removeCharge(historyCharge.getChargeSerialNo());
			}	
		
			Date date = new Date();
			String chOrCode = EpChargeService.makeChargeOrderNo(this.pkEpGunId,chargeCache.getUserId());
			
			chargeCache.setChOrCode(chOrCode);
			
			chargeCache.setSt(st);
			
					
			int chorType= EpChargeService.getOrType(u.getLevel());
			
			int userOrgNo=1000;
			if(chargeCache.getUserOrigin()!=null)
				userOrgNo= chargeCache.getUserOrigin().getOrgNo();
			
			int payMode = chargeCache.getPayMode();
			int pkOrderId= EpChargeService.insertChargeOrderToDb(chargeCache.getUserId(), chorType,chargeCache.getPkUserCard(),userOrgNo, 
					pkEpId, epCode, epGunNo, chargeCache.getChargingMethod(),bespokeNo, chOrCode, 
					chargeCache.getFronzeAmt(),payMode,userOrgNo,st,
					chargeCache.getChargeSerialNo(), this.chargeCache.getRateInfo(),
					chargeCache.getThirdUsrIdentity());
			chargeCache.setPkOrderId(pkOrderId);
				
			EpChargeService.updateBeginRecordToDb(chargeCache.getUserId(), chorType,chargeCache.getAccount(),chargeCache.getPkUserCard(),userOrgNo, 
					pkEpId, epCode, epGunNo, chargeCache.getChargingMethod(),bespokeNo, chOrCode, 
					chargeCache.getFronzeAmt(),st,chargeCache.getChargeSerialNo(),0, this.chargeCache.getRateInfo(),0,this.chargeCache.getThirdCode(),this.chargeCache.getThirdType());
			
				
			pushFirstRealData();
				
			
			StatService.addCharge();
			
			cleanIdentyCode();
			 
			 
			 logger.info("charge event success accountId:{},serialNo:{},epCode:{},epGunNo:{},method:{},st:{}",
						new Object[]{chargeCache.getUserId(),chargeCache.getChargeSerialNo(),epCode,epGunNo,method,st});
				
			 
			return 1;
		}
		else
		{
			return 2;
		}
		
	}
	
	public void onNetStatus(int epStatus)
	{
		if(this.chargeCache!=null && this.status == EpConstant.EP_GUN_STATUS_CHARGE)
		{
			handleEvent( EventConstant.EVENT_EP_NET_STATUS,epStatus,0,null,null);
			
		}
	}
	private void checkWaitInsertGunCharge()
	{
		if(chargeCache!=null &&( chargeCache.getStatus()== ChargeStatus.CS_WAIT_INSERT_GUN||
				chargeCache.getStatus() ==  ChargeStatus.CS_WAIT_CHARGE))
		{
			long now = DateUtil.getCurrentSeconds();
			long diff = now - this.chargeCache.getSt();//超时判断为充电后10分钟
			if(diff > GameConfig.epWaitGun)
			{
				this.onStartChargeEventFail(2,6001);
				
			}
		}
	}
	public void checkChargeCmdTimeOut(int method)
	{
		if(chargeCache!=null && chargeCache.getStatus()== ChargeStatus.CS_ACCEPT_CONSUMEER_CMD)
		{
			long now = DateUtil.getCurrentSeconds();
			long diff = now - this.chargeCache.getLastCmdTime();//超时判断为充电后10分钟
			if(diff > GameConfig.chargeCmdTime)
			{
				this.onStartChargeFail(method,6001);
				
			}
		}
	}
	public void checkBespokeCmdTimeOut()
	{
		if(bespCache!=null && bespCache.getStatus()== EpConstant.BESPOKE_STATUS_CONFIRM)
		{
			long now = DateUtil.getCurrentSeconds();
			long diff = now - this.bespCache.getStartTime();
			if(diff>GameConfig.bespokeCmdTime)
			{
				onBespokeFail(6001);
			}
		}
	}
	private boolean gunFault(int status)
	{
		if(status==1 ||
				(status>=EpConstant.EP_GUN_W_STATUS_LESS_VOL_FAULT && status<=EpConstant.EP_GUN_W_STATUS_URGENT_STOP))
		{
			return true;
		}
		return false;
	}
	
	@SuppressWarnings("unused")
	private void handleEpTimeout(int oldWorkStatus,int newWorkStatus )
	{
		ElectricPileCache epCache= EpService.getEpByCode(epCode);
		if(epCache.getConcentratorId()<=0)
			return;
		
		//集中器
		if(newWorkStatus == EpConstant.EP_GUN_W_STATUS_OFF_LINE &&
				this.status != EpConstant.EP_GUN_STATUS_IDLE)
		{
			this.modifyStatus(EpConstant.EP_GUN_STATUS_IDLE, true);
			EpService.updateEpCommStatusToDb(epCache.getPkEpId(), 0, 0);
			
		}
		else if(oldWorkStatus == EpConstant.EP_GUN_W_STATUS_OFF_LINE &&
				oldWorkStatus !=newWorkStatus) 
		{
			EpService.updateEpCommStatusToDb(epCache.getPkEpId(), 1, 0);
		}
	}
	/**
	 * 进入电桩初始化状态,不处理电桩任何事物
	 */
	
	private void gotoEpInitStatus()
	{
		this.modifyStatus(EpConstant.EP_GUN_STATUS_EP_INIT, false);
		
	}
	/**
	 * 进入电桩空闲状态。
	 * 1.如果电桩有未完成的预约命令，那么终止预约
	 * 2.如果电桩有未结算的预约，终止并结算预约
	 * 3.如果有等待插枪的充电，那么终止充电
	 */
	private void gotoIdleStatus()//进入空闲状态
	{	
		if(chargeCache!=null)
		{
			if(chargeCache.getStatus() ==  ChargeStatus.CS_ACCEPT_CONSUMEER_CMD)
			{
				checkChargeCmdTimeOut(2);
			}
			else if(chargeCache.getStatus() ==  ChargeStatus.CS_WAIT_INSERT_GUN ||
					chargeCache.getStatus() ==  ChargeStatus.CS_WAIT_CHARGE)
			{
				//如果电桩已经变为空闲，还有等待擦枪，不需要等待10
				checkWaitInsertGunCharge();
				
			}
			else
			{
				
			}
			
		}
		if(bespCache!=null)
		{
			if( bespCache.getStatus() == EpConstant.BESPOKE_STATUS_LOCK)
			{
				EpCommClient epCommClient =  (EpCommClient)this.getEpNetObject();
				
				if(epCommClient!=null && epCommClient.isComm())
				{
					long now = DateUtil.getCurrentSeconds();
					
					long diff = now - this.bespCache.getStartTime();
					if(diff>180)//直流桩上报状态数据太慢，3分钟之内不根据判断超时
					{
						logger.info("[bespoke] end by gotoIdleStatus,accountId:{},bespNo:{},epCode:{},epGunNo:{}",
								new Object[]{bespCache.getAccountId(),bespCache.getBespNo(),epCode,epGunNo});
						
						endBespoke( EpConstant.END_BESPOKE_CANCEL );
					}
				}
			}
			else if(bespCache.getStatus() == EpConstant.BESPOKE_STATUS_CONFIRM)
			{
				this.checkBespokeCmdTimeOut();
				
			}
			else
			{
				
			}
		}
		
		
		if(this.status !=  EpConstant.EP_GUN_STATUS_IDLE)
		{
			this.modifyStatus(EpConstant.EP_GUN_STATUS_IDLE, true);
			cleanEpRealChargeInfo();
		}
		
		this.setAuthCache(null);
	
	}
	private void gotoUserOperStatus()//插枪或者收枪状态
	{
		if(status != EpConstant.EP_GUN_STATUS_EP_OPER)
		{
			this.modifyStatus(EpConstant.EP_GUN_STATUS_EP_OPER, false);
		}
		
	}
	private void gotoWaitCharge()//等待充电
	{
		this.onInsertGunSuccess();
		if(status != EpConstant.EP_GUN_STATUS_WAIT_CHARGE)
		{
			this.modifyStatus(EpConstant.EP_GUN_STATUS_WAIT_CHARGE, true);
		}
		
	}
	private void gotoUserAuthStatus()//进入用户鉴权状态
	{
		if(status != EpConstant.EP_GUN_STATUS_USER_AUTH)
		{
			this.modifyStatus(EpConstant.EP_GUN_STATUS_USER_AUTH, false);
		}
	}
	
	
	private void gotoBespokeStatus()//进入预约状态
	{
		this.onBespokeSuccess(2);
		if(this.status !=  EpConstant.EP_GUN_STATUS_BESPOKE_LOCKED)
		{
			this.modifyStatus(EpConstant.EP_GUN_STATUS_BESPOKE_LOCKED, true);
			
		}
	}
	private void gotoChargeStatus()//进入充电状态
	{
		this.onStartChargeEventSuccess(2,DateUtil.getCurrentSeconds());
		if(this.status !=  EpConstant.EP_GUN_STATUS_CHARGE)
		{
			this.modifyStatus(EpConstant.EP_GUN_STATUS_CHARGE, true);
		}
	}
	private void gotoFaultStatus()//进入故障状态
	{
		checkWaitInsertGunCharge();
	
		
		if(this.status !=  EpConstant.EP_GUN_STATUS_STOP_USE)
		{
			this.modifyStatus(EpConstant.EP_GUN_STATUS_STOP_USE, true);
		}
		
	}
	private void gotoSettingStatus()//进入设置状态
	{
		this.checkChargeCmdTimeOut(1);
		
		if(this.status!= EpConstant.EP_GUN_STATUS_SETTING)
		{
			this.modifyStatus(EpConstant.EP_GUN_STATUS_SETTING, false);
			EpGunService.updateGunState(getPkEpGunId(), 9);
		}
		
	}
	private void gotoChargeModeStatus()//进入充电模式选择状态
	{
		if(status != EpConstant.EP_GUN_STATUS_SELECT_CHARGE_MODE)
		{
			this.modifyStatus(EpConstant.EP_GUN_STATUS_SELECT_CHARGE_MODE, false);
		}
	}
	//该函数主要用来处理集中器内的电桩
	private void gotoOffLineStatus()
	{
		logger.debug("gotoOffLineStatus,realData,epcode:{},gunno:{},this.status:{}",new Object[]{epCode,epGunNo,this.status});
		
		EpCommClient epCommClient = (EpCommClient)epNetObject;
		
		
		if(epCommClient==null || !epCommClient.isComm() || epCommClient.getMode()!=2)
			return;
	
		if(this.status!= EpConstant.EP_GUN_STATUS_OFF_LINE)//
		{
			EpService.updateEpCommStatusToDb(getPkEpId(), 0, 0);
		}
	}
	private void gotoUpgradeStatus()
	{
		if(this.status!= EpConstant.EP_GUN_STATUS_EP_UPGRADE)
		{
			EpGunService.updateGunState(this.getPkEpGunId(), EpConstant.EP_GUN_STATUS_STOP_USE);
			
			this.modifyStatus(EpConstant.EP_GUN_STATUS_EP_UPGRADE, false);
		}
	}
	
	private void onRealDataChangeToMonitor(int oldEpWorkStatus,Map<Integer, SingleInfo> changePointMap,int changeType)
	{
		
		if(oldEpWorkStatus!=this.realChargeInfo.getWorkingStatus())
		{
			if( oldEpWorkStatus==3)
			{
				this.cleanEpRealChargeInfo();
				
			}
			dispatchWholeRealToMonitor(oldEpWorkStatus);
		}
		else
		{
			if(changeType==1 || changeType==3)
			{
				sendRealChangeYxtoMonitor(changePointMap,changeType,oldEpWorkStatus, this.realChargeInfo.getWorkingStatus());
			}
			else
			{
				sendRealChangeYctoMonitor(this.realChargeInfo.getWorkingStatus());
			}
		}
		
	}
	
	private void onStatusChange(int newStatus)
	{
	
		switch(newStatus)
		{
		case EpConstant.EP_GUN_STATUS_IDLE:
			this.gotoIdleStatus();
			break;
		case EpConstant.EP_GUN_STATUS_BESPOKE_LOCKED:
			this.gotoBespokeStatus();
			break;
		case EpConstant.EP_GUN_STATUS_CHARGE:
			this.gotoChargeStatus();
			break;
		case EpConstant.EP_GUN_STATUS_STOP_USE:
			this.gotoFaultStatus();
			break;
		case EpConstant.EP_GUN_STATUS_EP_OPER:
			this.gotoUserOperStatus();
			break;
		case EpConstant.EP_GUN_STATUS_USER_AUTH:
			this.gotoUserAuthStatus();
			break;
		case EpConstant.EP_GUN_STATUS_SETTING:
			gotoSettingStatus();
			break;
		case EpConstant.EP_GUN_STATUS_FROZEN_AMT:
			break;
		case EpConstant.EP_GUN_STATUS_SELECT_CHARGE_MODE:
			gotoChargeModeStatus();
			break;
		case EpConstant.EP_GUN_STATUS_EP_UPGRADE:
			break;
		case EpConstant.EP_GUN_STATUS_OFF_LINE:
			gotoOffLineStatus();
			break;
		case EpConstant.EP_GUN_STATUS_EP_INIT:
			this.gotoEpInitStatus();
			break;
		case EpConstant.EP_GUN_STATUS_WAIT_CHARGE:
			this.gotoWaitCharge();
			break;
		default:
			break;
		}
	}

	
	
	
	
	/**
	 * 
	 * @param chargeCmdResp
	 * @return
	 */
	public int onEpStartCharge(ChargeCmdResp chargeCmdResp)
	{
		logger.debug("onEpStartCharge,chargeCmdResp:{}",chargeCmdResp);
		
		if(chargeCmdResp.getRet() ==  1) //电桩接受充电成功,变为等待插枪
		{
			onStartChargeSuccess();
		}
		else
		{
			//电桩接受充电失败
			onStartChargeFail(1,chargeCmdResp.getErrorCause());
		}
		
		return 0;
	}
	private void modifyStatus(int status,boolean modifyDb)
	{
		logger.debug("modifyStatus,epcode:{},gunno:{},this.status:{},newStatus:{}",
				new Object[]{epCode,epGunNo,this.status,status});
		this.status = status;
		
		if(modifyDb)
		{
			EpGunService.updateGunState(this.getPkEpGunId(), status);
		}
	}
	private void cleanEpRealChargeInfo()
	{
		if(realChargeInfo != null)
		{
			if(realChargeInfo.getCurrentType() == EpConstant.EP_AC_TYPE)
				((RealACChargeInfo)realChargeInfo).endCharge();
			else if(realChargeInfo.getCurrentType() ==EpConstant.EP_DC_TYPE)
				((RealDCChargeInfo)realChargeInfo).endCharge(); 
		}
		
	}
	public int onEpStopChargeEvent(int epRet,String userAccount, String aerialNo)
	{
		if(epRet ==  0)//充电桩,充电成功
		{
			if(this.status == EpConstant.EP_GUN_STATUS_CHARGE)
			{
				return 3;
			}
			this.status = EpConstant.EP_GUN_STATUS_IDLE;
			cleanEpRealChargeInfo();
		}
		else//没插枪超时，那么转为空闲
		{
			//失败.变为空闲
			this.status = EpConstant.EP_GUN_STATUS_IDLE;
		}
		
		return 0;
	}
	/**
	 * 
	 * @param epChargeEvent
	 * @return
	 */
	public int handleStartChargeEvent(ChargeEvent epChargeEvent)
	{
		logger.debug("handleStartchargeEvent enter,epcode:{},gunno:{},epChargeEvent:{},chargeCache:{}",
				new Object[]{epCode,epGunNo,epChargeEvent,chargeCache});	
		if(chargeCache==null )//特殊卡
		{
			logger.error("handleStartchargeEvent fail,chargeCache==null,epcode:{},gunno:{}",epCode,epGunNo);

			return 2; //
		}
		if(epChargeEvent.getSerialNo().compareTo(chargeCache.getChargeSerialNo())!=0)
		{
			logger.error("onEpStartchargeEvent fail,invalid serialNo,epcode:{},gunno:{},epChargeEvent.getSerialNo():{},chargeCache.getChargeSerialNo():{}",
					  new Object[]{epCode,epGunNo,epChargeEvent.getSerialNo(),chargeCache.getChargeSerialNo()});
			return 2;//数据不存在
		}
		int retCode=0;
		if(epChargeEvent.getSuccessFlag() ==  1)//充电桩,充电成功
		{	
			retCode = this.onStartChargeEventSuccess(1,epChargeEvent.getStartChargeTime());
		}
		else//没插枪超时，那么转为空闲
		{
			retCode= onStartChargeEventFail(1,0);
		}
		
		return retCode;

	}
	

	
	public void cleanBespokeInfo()
	{
		if(bespCache!=null)
		{
			logger.info("cleanUsrBespoke,userId:{},usrAccount:{},bespNo()",
							new Object[]{bespCache.getAccountId(),
					bespCache.getAccount(),bespCache.getBespNo()});
			
			UserService.cleanUsrBespoke(bespCache.getAccountId(),
					bespCache.getAccount(),bespCache.getBespNo());
			
			
		}
 		 setBespCache(null);
	}
	
	public void cleanChargeInfo()
	{
		if(chargeCache!=null)
		{
			UserService.cleanUsrCharge(chargeCache.getUserId(),
					chargeCache.getAccount(),chargeCache.getChargeSerialNo());
			
			setChargeCache(null);
		}
		//清除车端信息和电源模块数据
		if(this.currentType == EpConstant.EP_DC_TYPE)
		{
			((RealDCChargeInfo)realChargeInfo).cleanChargeInfo();
		}
		
	}
	public ChargeCache makeChargeInfo(UserCache chargeUser,String thirdUsrIdentity,RateInfo rateInfo,short chargeStyle,int nFrozenAmt,
			int payMode,int orgNo, int cmdFromSource,String cmdChIdentity)
	{
		ChargeCache chargingCacheObj = new ChargeCache();
		
		String serialNo = epCode + EpChargeService.makeSerialNo();
		chargingCacheObj.setChargeSerialNo(serialNo);
		
		chargingCacheObj.setSt(DateUtil.getCurrentSeconds());
		
		chargingCacheObj.setUserId(chargeUser.getId());
		chargingCacheObj.setAccount(chargeUser.getAccount());
		chargingCacheObj.setThirdUsrIdentity(thirdUsrIdentity);
		
		chargingCacheObj.setFronzeAmt(nFrozenAmt);
		
		RateInfo curRateInfo = new RateInfo();
		curRateInfo.setJ_Rate(rateInfo.getJ_Rate());
		curRateInfo.setF_Rate(rateInfo.getF_Rate());
		curRateInfo.setP_Rate(rateInfo.getP_Rate());
		curRateInfo.setG_Rate(rateInfo.getG_Rate());
		curRateInfo.setServiceRate(rateInfo.getServiceRate());
		curRateInfo.setQuantumDate(rateInfo.getQuantumDate());
		
		chargingCacheObj.setRateInfo(curRateInfo);
		chargingCacheObj.setStartChargeStyle(chargeStyle);
		chargingCacheObj.setPayMode(payMode);
		UserOrigin userOrigin = new UserOrigin(orgNo,cmdFromSource,cmdChIdentity);
		
		chargingCacheObj.setUserOrigin(userOrigin);
		
		chargingCacheObj.setStatus(ChargeStatus.CS_ACCEPT_CONSUMEER_CMD);
		
		chargingCacheObj.setLastCmdTime(DateUtil.getCurrentSeconds());
		
		return chargingCacheObj;
	}
		
	public int startChargeAction(UserCache chargeUser,String thirdUsrIdentity,ChargeCardCache card,RateInfo rateInfo,
			String bespNo,short chargeStyle,int frozenAmt,int payMode,int orgNo, 
			int fromSource, String actionIdentity,byte[]cmdTimes)
	{
		
		int chargingUserId = chargeUser.getId();
		String chargingAccout = chargeUser.getAccount();
		//1.有别人预约,不能充电
		BespCache bespCache = getBespCache();
		if(bespCache!=null && bespCache.getAccountId() != chargingUserId)
		{
			logger.error("startcharge fail,bespCache.getAccountId() != chargingUserId,chargingUserId:{},bespUserID:{},epcode:{},gunno:{}",new Object[]{chargingUserId,bespCache.getAccountId(),this.epCode,this.epGunNo});
			return EpConstantErrorCode.EPE_OTHER_BESP;
		}
		
		if(epNetObject == null )
		{
			logger.error("startcharge fail,epCode:{},epGunNo:{},chargingAccout:{} epNetObject == null",new Object[]{this.epCode,this.epGunNo,chargingAccout});
			return EpConstantErrorCode.EP_UNCONNECTED;
		}
		if( !epNetObject.isComm())
		{
			logger.error("startcharge fail,epCode:{},epGunNo:{},chargingAccout:{},status:{}",new Object[]{this.epCode,this.epGunNo,chargingAccout,epNetObject.getStatus()});
			return EpConstantErrorCode.EP_UNCONNECTED;
		}
		if(this.chargeCache!=null  )
		{
			if(chargeCache.getUserId() != chargeUser.getId())
			{
				logger.error("startcharge fail,other is chargeing chargeCache:{}",this.chargeCache);
				return EpConstantErrorCode.EPE_OTHER_CHARGING;
			}
			else
			{
				logger.error("startcharge fail,repeat charge chargeCache:{}",this.chargeCache);
				return EpConstantErrorCode.EPE_REPEAT_CHARGE;
			}
		}
		
		EpCommClient epCommClient = (EpCommClient)epNetObject;
		
		if(this.status == EpConstant.EP_GUN_STATUS_STOP_USE)
		{
			return EpConstantErrorCode.EPE_GUN_FAULT;
		}
		if(this.status == EpConstant.EP_GUN_STATUS_CHARGE)
		{
			return EpConstantErrorCode.EPE_REPEAT_CHARGE;
		}
		 if(this.status == EpConstant.EP_GUN_STATUS_SETTING)
		 {
			return EpConstantErrorCode.EPE_IN_EP_OPER;
		 }

		if(payMode == EpConstant.P_M_FIRST)
		{
			int userId = chargeUser.getId();
			BigDecimal bdRemainAmt = UserService.getRemainBalance(userId);
			
			
			logger.info("before startcharge remainAmt,accountId:{},amt:{},payMode:{}",
					new Object[]{userId,bdRemainAmt.doubleValue(),payMode});
			//100倍后转为整数
			bdRemainAmt = bdRemainAmt.multiply(Global.DecTime2);
			int nRemainAmt= UtilProtocol.BigDecimal2ToInt(bdRemainAmt);
			BigDecimal bdFrozenAmt = UtilProtocol.intToBigDecimal2(frozenAmt);
			
			//冻结金额
			if(nRemainAmt<0 || frozenAmt<=0 || nRemainAmt<frozenAmt)
			{
				logger.error("startcharge fail,no money,userId:{},bdRemainAmt:{},bdFrozenAmt:{}",new Object[]{userId,bdRemainAmt.doubleValue(),bdFrozenAmt.doubleValue()});
				return EpConstantErrorCode.EPE_NO_ENOUGH_MONEY;
			}
		//	logger.debug("remainAmtWarn,chargeUser.isRemainAmtWarn():{},nRemainAmt:{},frozenAmt:{},chargeUser.getRemainAmtWarnValue():{},chargeUser.getAccount():{}",
		//			new Object[]{chargeUser.isRemainAmtWarn(),nRemainAmt,frozenAmt,chargeUser.getRemainAmtWarnValue(),chargeUser.getAccount()});
			if(!chargeUser.isRemainAmtWarn() &&
					(nRemainAmt-frozenAmt)< (chargeUser.getRemainAmtWarnValue()*100) && 
					chargeUser.getAccount().length()==12)
			{
				chargeUser.setRemainAmtWarn(true);
				int n=nRemainAmt-frozenAmt;
				String warnAmt = UtilProtocol.intToBigDecimal2(n).toString();
				
				logger.info("big account remainAmtWarning,userId:{},userAccount:{},managPhone:{},customerPhone:{}",
						new Object[]{userId,chargeUser.getAccount(),chargeUser.getRemainAmtWarnPhone(),chargeUser.getRemainAmtWarnCPhone()});
				
				EpChargeService.msgRMAmtWarningToManager(userId,chargeUser.getRemainAmtWarnPhone(),
						chargeUser.getName(),warnAmt);
				
				EpChargeService.msgRemainAmtWarning(userId, chargeUser.getRemainAmtWarnCPhone(), warnAmt);
			}
			
			UserService.subAmt(chargeUser.getId(), bdFrozenAmt);
			
		}
		
		
		ChargeCache chargingCacheObj = makeChargeInfo(chargeUser, thirdUsrIdentity,rateInfo, chargeStyle, frozenAmt, payMode,
				orgNo,fromSource,actionIdentity);
		chargingCacheObj.setEpCode(this.getEpCode());
		chargingCacheObj.setEpGunNo(this.getEpGunNo());
		
		
		String transactionNumber = chargingCacheObj.getChargeSerialNo();
		this.chargeCache = chargingCacheObj;
		
		if(status != EpConstant.EP_GUN_STATUS_BESPOKE_LOCKED)
		{
			modifyStatus(EpConstant.EP_GUN_STATUS_USER_AUTH, false);
				
			
		}
		
		if(bespNo.length()==0)
		{
			bespNo= StringUtil.repeat("0", 12);//如果预约编号空
		}
		
		chargingCacheObj.setBespNo(bespNo);
		byte[] bcdBespNo = WmIce104Util.str2Bcd(bespNo);
		assert(bcdBespNo.length==6);
		
		
		if(ImitateEpService.IsStart())
			ImitateEpService.startCharge(epCode, epGunNo, chargeUser.getAccount());
		
		else
		{
			chargingCacheObj.setCard(card);
			String curUserAccount = chargingCacheObj.getAccount();
			Date date = new Date();
			String chOrCode = (int)(date.getTime()/1000) + curUserAccount;
			
			ChargeEvent chargeEvent = 
					new ChargeEvent(epCode,epGunNo,transactionNumber,0,(int)(date.getTime()/1000),0,1,0);
			
			BigDecimal bdFronzeAmt = UtilProtocol.intToBigDecimal2(chargeCache.getFronzeAmt());
			
			EpGunService.updateChargeInfoToDbByEpCode(this.currentType,this.epCode,this.epGunNo,
					new BigDecimal(this.realChargeInfo.getChargedMeterNum()).multiply(Global.Dec3),chargeEvent.getSerialNo(),bdFronzeAmt,0,chargeCache.getUserId());
	
			
			chargeCache.setChOrCode(chOrCode);
			
			
			UserCache memUserInfo= UserService.getUserCache(curUserAccount);
			UserRealInfo realUserInfo= UserService.findUserRealInfo(curUserAccount);
			
			if(realUserInfo!=null && memUserInfo != null && card !=null)
			{
				logger.debug("startcharge,memUserInfo.getCard().getUserId():{}",card.getId());
			   //chargeCache.setPkUserCard(memUserInfo.getCard().getId());
			}
			int chorType= EpChargeService.getOrType(memUserInfo.getLevel());
			
			memUserInfo.addCharge(chargeCache);
			String bespokeNo= StringUtil.repeat("0", 12);
			if(this.bespCache!=null)
			{
				bespokeNo =this.bespCache.getBespNo();
			}
		
			int curUserId =  chargeCache.getUserId();
			EpChargeService.insertChargeRecordToDb(curUserId, chorType,curUserAccount,chargeCache.getPkUserCard(),orgNo, 
			pkEpId, epCode, epGunNo, chargeCache.getChargingMethod(),bespokeNo, chOrCode, 
			chargeCache.getFronzeAmt(),payMode,orgNo,chargeEvent, 
			this.chargeCache.getRateInfo(),4,
			chargeCache.getThirdUsrIdentity());
	

			logger.info("charge start accountId:{},account:{},chargeSerialNo:{},epCode:{},epGunNo:{} to ep",
					new Object[]{chargeCache.getUserId(),chargingAccout,chargeCache.getChargeSerialNo(),epCode,epGunNo});
			
			byte[] data = EpEncodeProtocol.do_start_electricize(epCode, (byte)epGunNo, 
					chargingAccout, 0, (byte)chargeStyle,frozenAmt,1,realUserInfo.getPassword(),chargeCache.getChargeSerialNo());
			
			if(data==null)
			{
				logger.error("startcharge fail,do_start_electricize error,epCode:{},epGunNo:{},account:{} ",
						new Object[]{epCode,epGunNo,chargingAccout});
			}
			
			//命令加时标
			String messagekey = String.format("%03d%s", Iec104Constant.C_START_ELECTRICIZE,chargeCache.getChargeSerialNo());
			EpMessageSender.sendRepeatMessage(epCommClient,messagekey,0,0,Iec104Constant.C_START_ELECTRICIZE, data,cmdTimes,epCommClient.getVersion());	
		}
		
		//清空鉴权用户
		setAuthCache(null);
		
		return 0;
	}
	
	public int stopChargeAction(int orgNo,int userId,String thirdUsrIdentity,int source,String actionIdentity)
	{
		EpCommClient commClient= (EpCommClient)getEpNetObject();
		if(commClient ==null || !commClient.isComm())
		{
			return EpConstantErrorCode.EP_UNCONNECTED;//
		}
		
		ChargeCache chargeCacheObj = getChargeCache();
		//没有在充电，不能结束充电
		if(chargeCacheObj==null )
		{
			logger.error("stopcharge fail,chargeCacheObj==null,epCode:{},epGunNo:{}",epCode,epGunNo);
			return  EpConstantErrorCode.EPE_NOT_ENABLE_STOP_WITHOUT_CHARGING;//
		}
		//不是充电的用户不能结束充电  
		if(orgNo == UserConstant.ORG_I_CHARGE) 
		{
			if( chargeCacheObj.getUserId() != userId)
			{
				logger.error("stopcharge fail,user without charge,epCode:{},epGunNo:{},orgNo:{},userId:{},chargeCacheObj.getUserId():{}",
						new Object[]{epCode,epGunNo,orgNo,userId,chargeCacheObj.getUserId()});return  EpConstantErrorCode.EPE_NOT_ENABLE_STOP_WITHOUT_CHARGING;
			}
		}
		else
		{
			if( thirdUsrIdentity==null||
				!thirdUsrIdentity.equals(chargeCacheObj.getThirdUsrIdentity())||
				chargeCacheObj.getUserId() != userId  )
			{
				logger.error("stopcharge fail,user without charge,epCode:{},epGunNo:{},orgNo:{},userId:{},chargeCacheObj.getUserId():{},thirdUsrIdentity:{},chargeCacheObj.getThirdUsrIdentity():{}",
						new Object[]{epCode,epGunNo,orgNo,userId,chargeCacheObj.getUserId(),thirdUsrIdentity,chargeCacheObj.getThirdUsrIdentity()});
				return  EpConstantErrorCode.EPE_NOT_ENABLE_STOP_WITHOUT_CHARGING;
			}
			
		}
		//TODO,记得测试，在没插枪之前取消
		if(this.status != EpConstant.EP_GUN_STATUS_CHARGE && this.status != EpConstant.EP_GUN_STATUS_EP_OPER)
		{
			logger.error("stopcharge fail,gun not charging,epCode:{},epGunNo:{},status:{}",new Object[]{epCode,epGunNo,status});
			return  EpConstantErrorCode.EPE_NOT_ENABLE_STOP_WITHOUT_CHARGING;//
		}
        
		byte bcdqno = (byte)epGunNo;
		
		/*if(ImitateEpService.IsStart())
		{
			ImitateEpService.ImitateStopCharge(this.epCode,this.epGunNo , this.curUserAccount, chargeCacheObj);
		}
		else*/
		{
			byte[] data= EpEncodeProtocol.do_stop_electricize(epCode, bcdqno);
			if(data == null)
			{
				logger.error("stopcharge fail,epCode:{},epGunNo:{}, do_stop_electricize exception",epCode,epGunNo);
				return EpConstantErrorCode.EP_PACK_ERROR;
				
			}
			byte[] cmdTimes = WmIce104Util.timeToByte();
			
			EpMessageSender.sendMessage(commClient,0,0,Iec104Constant.C_STOP_ELECTRICIZE, data,cmdTimes,commClient.getVersion());
			logger.info("stopcharge accountId:{},account:{},chargeSerialNo:{},epCode:{},epGunNo:{},cmdTimes:{}{}{} to ep",
					new Object[]{chargeCache.getUserId(),chargeCache.getAccount(),chargeCache.getChargeSerialNo(),epCode,epGunNo,(int)cmdTimes[0],(int)cmdTimes[1],(int)cmdTimes[2]});
			
			
		}
		
		return 0;
	}
	
	public int getCurUserId()
	{
		if( authCache!=null && authCache.getUsrId()>0)
			return authCache.getUsrId();
		
		if(bespCache!=null && bespCache.getAccountId()>0)
			return bespCache.getAccountId();
		
		if( chargeCache!=null && chargeCache.getUserId()>0)
			return chargeCache.getUserId();
		
		return 0;
	}
	
	public int startBespokeAction(UserCache userInfo,RateInfo rateInfo,int redo,int secBuyOutTime,String bespNo,
			int payMode,int orgNo,int cmdFromSource,String cmdIdentily)
	{	
		//1.充电桩未连接不能充电
		EpCommClient commClient = (EpCommClient)getEpNetObject();
		if(commClient==null || commClient.isComm()==false) {
			
			
			return EpConstantErrorCode.EP_UNCONNECTED;//
		}
		
		if( redo == 1 )
		{
			// 11.这个枪没有预约不能续约
			if(status != EpConstant.EP_GUN_STATUS_BESPOKE_LOCKED)
			{
				
				return EpConstantErrorCode.BESP_NO_NOT_EXIST;
			}
			if (this.bespCache.getAccountId() != userInfo.getId()) {
				return EpConstantErrorCode.NOT_SELF_REDO_BESP;//
			}
		}
		if(redo == 0)
		{
			if(status ==EpConstant.EP_GUN_STATUS_EP_OPER)//用户使用状态，允许使用用户预约
			{
				int curUserId = getCurUserId(); 
				if(curUserId!=0 && curUserId != userInfo.getId())
				{
					return EpConstantErrorCode.CAN_NOT_BESP_IN_BESP_COOLING;
				}
				return EpConstantErrorCode.USED_GUN;//
			}
			if(status == EpConstant.EP_GUN_STATUS_BESPOKE_LOCKED)
				return EpConstantErrorCode.EPE_OTHER_BESP;//
			
			if(this.status == EpConstant.EP_GUN_STATUS_SETTING)
			 {
				return EpConstantErrorCode.EPE_IN_EP_OPER;
			 }
			if(this.status == EpConstant.EP_GUN_STATUS_SELECT_CHARGE_MODE)
			 {
				return EpConstantErrorCode.EPE_IN_EP_OPER;
			 }
			
		}
		if(status == EpConstant.EP_GUN_STATUS_IDLE )
		{
			status = EpConstant.EP_GUN_STATUS_EP_OPER;
		}
				
		if (redo == 1) {
			long bespTotalTime = this.bespCache.getEndTime() + secBuyOutTime
					- bespCache.getStartTime();
			if (bespTotalTime > (6 * 3600)) {
				return EpConstantErrorCode.BESP_TO_MAX_TIME;//
			}
		}
		

		BigDecimal fronzingAmt = RateService.calcBespAmt(rateInfo.getBespokeRate(),secBuyOutTime / 60);

		int userId= userInfo.getId();
		
		BigDecimal userRemainAmt= UserService.getRemainBalance(userId);
		logger.info("bespoke remainAmt,accountId:{},amt:{},payMode:{} before fronze amt",
				new Object[]{userId,userRemainAmt.doubleValue(),payMode});
		
	
		// 12.钱不够不能充电

		if(payMode == EpConstant.P_M_FIRST && userRemainAmt.compareTo(fronzingAmt)<0)
		{
			logger.error("bespoke fail,not enough money,accountId{},amt:{},userRemainAmt:{},curUserAccount{},bespNo{},epCode{},epGunNo{},redo:{}"
					,new Object[]{userId,fronzingAmt.doubleValue(),userRemainAmt,userInfo.getAccount(),bespNo,this.epCode,this.epGunNo,redo});

			
			return EpConstantErrorCode.EPE_NO_ENOUGH_MONEY;//
		}

		byte bcdqno = (byte) epGunNo;
		byte bredo = (byte) redo;
		byte[] start_time = WmIce104Util.getP56Time2a();

		// todo:20150803
		String CardNo = new String("1234567891234567");// 充电卡号
		String CarCardNo = new String("1234567891234567");// 车牌号
		
		//String bespNoMd5 = Md5Encoder.encodeByMD5(bespNo);

		java.util.Date dt = new Date();
		long bespSt = dt.getTime() / 1000;

		if (redo == 0) {
			BespCache bespCacheObj = new BespCache();
			
			
			bespCacheObj.setAccount(userInfo.getAccount());
			bespCacheObj.setBespNo(bespNo);
			
			long et = bespSt + (long) (secBuyOutTime);
			
			bespCacheObj.setStartTime(bespSt);
			bespCacheObj.setEndTime(et);
			bespCacheObj.setRealEndTime(et);

			bespCacheObj.setAccountId(userInfo.getId());
			bespCacheObj.setRate(rateInfo.getBespokeRate());
			bespCacheObj.setStatus(EpConstant.BESPOKE_STATUS_CONFIRM);
			
			bespCacheObj.setEpCode(this.epCode);
			bespCacheObj.setEpGunNo(this.epGunNo);
			
			bespCacheObj.setPayMode(payMode);
			UserOrigin userOrigin = new UserOrigin(orgNo,cmdFromSource,cmdIdentily);
			
			bespCacheObj.setUserOrigin(userOrigin);
		
			this.bespCache = bespCacheObj;
			
			userInfo.addBesp(bespCache);
			
			
			
		}
		bespCache.setBuyTimes(secBuyOutTime / 60);
		
		bespCache.setRedo((short)redo);
	
		if(ImitateEpService.IsStart())
			ImitateEpService.ImitateBespoke(this.epCode, this.epGunNo, redo, bespNo);
		
		else
		{
			byte[] bcdAccountNo = WmIce104Util.str2Bcd(userInfo.getAccount());
		
			byte[] orderdata = EpEncodeProtocol.do_bespoke(
					WmIce104Util.str2Bcd(epCode), bespNo, bcdqno, bredo,
					bcdAccountNo, WmIce104Util.str2Bcd(CardNo), start_time,
					(short)(secBuyOutTime/60), StringUtil.repeat("0", 16).getBytes());
	
			byte[] cmdTimes = WmIce104Util.timeToByte();
			
			String messagekey = String.format("%03d%s", Iec104Constant.C_BESPOKE,bespCache.getBespNo());
			
			EpMessageSender.sendRepeatMessage(commClient, messagekey, 0, 0,Iec104Constant.C_BESPOKE, orderdata,cmdTimes,commClient.getVersion());
			logger.info("bespoke start accountId:{},amt:{},bredo:{},curUserAccount{},bespNo{},epCode{},epGunNo{},cmdTimes:{}{}{}"
					,new Object[]{userId,fronzingAmt.doubleValue(),bredo,bespCache.getAccount(),bespNo,epCode,epGunNo,(int)cmdTimes[0],(int)cmdTimes[1],(int)cmdTimes[2]});
		   
		}					
		return 0;
		
	}
	public int stopBespokeAction(int source,String srcIdentity ,String bespno,int userId)
	{
		
		if(ImitateEpService.IsStart())
			ImitateEpService.ImitateCancelBespoke(srcIdentity, source, bespno);
		
		else
		{
			byte[] sendMsg = EpEncodeProtocol.do_cancel_bespoke(epCode, this.epGunNo,bespno);
			
			byte[] cmdTimes = WmIce104Util.timeToByte();
			EpCommClient commClient = (EpCommClient)this.epNetObject;
			EpMessageSender.sendMessage(commClient, 0, 0, Iec104Constant.C_CANCEL_BESPOKE, sendMsg,cmdTimes,commClient.getVersion());
			logger.info("cancelbespoke accountId,{},bespNo:{},epCode:{},epGunNo:{},cmdTimes:{}{}{}"
					,new Object[]{userId,bespno,epCode,epGunNo,(int)cmdTimes[0],(int)cmdTimes[1],(int)cmdTimes[2]});
		  
		}
	
		return 0;
	}
	
	
	public void cleanChargeInfoInRealData()
	{
		try
		{
		BigDecimal bdZero = new BigDecimal(0.0);
		EpGunService.updateChargeInfoToDbByEpCode(this.currentType,this.epCode,this.epGunNo,
				bdZero,"",bdZero,0,0);
		}
		catch(Exception e)
		{
			logger.error("cleanChargeInfoInRealData exception,getStackTrace:{}",e.getStackTrace());
			
		}
	
	}
	public UserCache getChargeUser(int chargeStyle,String account)
	{
		UserCache u=null;
		if(chargeStyle ==1)
			u= UserService.getUserCache(account);
		if(chargeStyle==3)
		{
			ChargeCardCache card = UserService.getCard(account);
			if(card !=null)
				u= UserService.getUserCache(card.getUserId());
		}
		
		if(u==null)
		{
			logger.error("getChargeUser invalid params,chargeStyle:{},account:{}",chargeStyle, account);
			
		}
		return u;
	}
	
	public NoCardConsumeRecord statChargeEvent(int userId,NoCardConsumeRecord consumeRecord)
	{
		consumeRecord.setStatRet(0);
		
		int chargeTime = (int)((consumeRecord.getEndTime() - consumeRecord.getStartTime())/60);
		if(chargeTime<0 || chargeTime> GameConfig.maxChargeTime)
		{
			consumeRecord.setStatRet(-1);
		}
		consumeRecord.setChargeUseTimes(chargeTime);
		if(consumeRecord.getTotalDl()>GameConfig.maxChargeMeterNum  || consumeRecord.getTotalDl()<0 )
		{
			consumeRecord.setStatRet(-2);
		}
		return consumeRecord;
		
	}
	public NoCardConsumeRecord checkChargeAmt(int userId,int fronzeAmt,int payMode,NoCardConsumeRecord consumeRecord)
	{
		consumeRecord.setStatAmtRet(0);
		int chargeAmt = consumeRecord.getTotalChargeAmt();
		int serviceAmt = consumeRecord.getServiceAmt();
		String epCode = consumeRecord.getEpCode();
		int gunNo = consumeRecord.getEpGunNo();
		String serialNo=consumeRecord.getSerialNo();
		
		if(chargeAmt<0 || chargeAmt>GameConfig.maxChargeAmt)
		{
			logger.error("endcharge stat invalid,chargeAmt:{},serialNo:{},epCode:{},gunNo:{},accountId:{}",
					new Object[]{chargeAmt,serialNo,epCode,gunNo,userId});
			consumeRecord.setStatAmtRet(-1);
			return consumeRecord;
		}
		if(serviceAmt<0 || serviceAmt>GameConfig.maxChargeServiceAmt)
		{
			logger.error("endcharge stat invalid,serviceAmt:{},serialNo:{},epCode:{},gunNo:{},accountId:{}",
					new Object[]{serviceAmt,serialNo,epCode,gunNo,userId});
			consumeRecord.setStatAmtRet(-2);
			return consumeRecord;
		}
		logger.info("endcharge stat accountId:{},chargeAmt:{},serviceAmt:{},serialNo:{},epCode:{},gunNo:{}",
				new Object[]{userId,chargeAmt,serviceAmt,serialNo,epCode,gunNo});
	    
		int consumeAmt = chargeAmt+serviceAmt;
		if(consumeAmt<0 || consumeAmt>GameConfig.maxChargeCost)
		{
			logger.error("endcharge stat invalid,consumeAmt:{},serialNo:{},epCode:{},gunNo:{},accountId:{}",
					new Object[]{consumeAmt,serialNo,epCode,gunNo,userId});
			consumeRecord.setStatAmtRet(-3);
			return consumeRecord;
		}
		int chargeCost=0;
		if(payMode== EpConstant.P_M_FIRST)
		{
			if(fronzeAmt< consumeAmt)
			{
				int diff = consumeAmt-fronzeAmt;
				logger.error("endcharge stat error,fronzeAmt:{} < totalConsumeAmt:{},diff:{},serialNo:{},epCode:{},gunNo:{},accountId:{}",
						new Object[]{fronzeAmt,consumeAmt,(consumeAmt-fronzeAmt),serialNo,epCode,gunNo,userId});	
				chargeCost= fronzeAmt;
				
				serviceAmt = serviceAmt-diff;//多出的钱从服务金额中扣除
				if(serviceAmt<0)
				{
					serviceAmt=0;
					chargeAmt=fronzeAmt;
				}
				logger.error("endcharge restat,chargeAmt:{},serviceAmt:{},serialNo:{},epCode:{},gunNo:{},accountId:{}",
						new Object[]{chargeAmt,serviceAmt,serialNo,epCode,gunNo,userId});	
				consumeRecord.setTotalChargeAmt(chargeAmt);	
				consumeRecord.setServiceAmt(serviceAmt);
			}
			else
			{
				chargeCost = consumeAmt;
			}
		}
		else
		{
			chargeCost= consumeAmt;
		}
		consumeRecord.setTotalAmt(chargeCost);	//计算总的消费金额
		
		return consumeRecord;
	}
	
	public  int checkConsumeRecord(String chargeSerialNo,String consumeAccount)
	{
		ChargeCache chargeCacheObj = getChargeCache();
		
		int retCode=1;//默认情况下认为正常
		
		if(chargeCacheObj==null)
		{
			//1.如果内存中没有,那么从数据库中插找
			//先查数据库订单状态
			retCode = EpChargeService.checkDBOrderStatus(chargeSerialNo);
			//订单已经处理过
			if(retCode !=EpConstant.ORDER_STATUS_DONE)//
			{
				chargeCacheObj = EpChargeService.GetChargeCacheFromDb(chargeSerialNo);
				if(chargeCacheObj ==null)//充电记录没有
				{
					logger.info("[endCharge]checkConsumeRecord!epCode:{},epGunNo:{},chargeSerialNo:{} did not find charge record",
							new Object[]{this.epCode,this.epGunNo,chargeSerialNo});
					
					retCode= 2;
					return retCode;
				}
				else if( chargeCacheObj.getStatus() == ChargeStatus.CS_CHARGE_FAIL||
						chargeCacheObj.getStatus() == ChargeStatus.CS_STAT
						)
				{
					//已经失败或者已经结算的充电不结算
					logger.info("checkConsumeRecord,had no order,have charge record status=3,chargeSerialNo:{}",chargeSerialNo);
					retCode= 3;
					return retCode;
				}
				else
				{
					this.chargeCache = chargeCacheObj;
					retCode=1;
				}
			}
		
			else //订单已完成支付
			{
				retCode=3;
				return retCode;
			}
		}	
			
		String mChargeSerialNo = chargeCacheObj.getChargeSerialNo();
		//3.和缓存中的充电流水号不一致，认为不存在
		if(chargeSerialNo.compareTo(mChargeSerialNo)!=0)
		{
			logger.info("[endCharge]checkConsumeRecord!epCode:{},epGunNo:{},chargeSerialNo:{} is not equals mChargeSerialNo:{},accountId:{},account:{}",
							new Object[]{this.epCode,this.epGunNo,chargeSerialNo,mChargeSerialNo,chargeCacheObj.getUserId(),chargeCacheObj.getAccount()});
			retCode= 2;
			return retCode;
		}
		//4.如果已经结算,那么立即还回
		if(chargeCacheObj.getStatus() >= ChargeStatus.CS_STAT)
		{
			logger.info("[endCharge]checkConsumeRecord had been end,accountId:{},epCode:{},epGunNo:{}:chargeSerialNo:{}",
					new Object[]{chargeCacheObj.getUserId(),epCode,epGunNo,chargeCacheObj.getChargeSerialNo()});
			retCode= 3;
		}
		else
		{
			UserCache chargeUser =  getChargeUser(chargeCacheObj.getStartChargeStyle(),consumeAccount);
			if(chargeUser==null)
			{
				logger.info("endcharge did not find user,accountId:{},epCode:{},epGunNo:{}:chargeSerialNo:{}",
						new Object[]{chargeCacheObj.getUserId(),epCode,epGunNo,chargeCacheObj.getChargeSerialNo()});
					
				retCode= 2;
			}
		}
			
		
		
		return retCode;
		
	}
	
	
	
	
	
	/**
	 *
	 * @param consumeRecord
	 * @return  
	 * 			3:异常
				2:充电记录不存在
				1:处理成功
	 */
	public int endChargeWithConsumeRecord(NoCardConsumeRecord consumeRecord)
	{	
		logger.debug("enter endchargeWithConsumeRecord,consumeRecord:{}",consumeRecord);
		try
		{ 
			ChargeCache chargeCacheObj= this.getChargeCache();
			if(chargeCacheObj==null)
				return 2;
		 
			int chargeUserId= chargeCacheObj.getUserId();
			String chargeUserAccount = chargeCacheObj.getAccount();
			chargeCacheObj.setEt(consumeRecord.getEndTime());
			
			//1.检查充电金额
			consumeRecord = checkChargeAmt(chargeUserId,chargeCacheObj.getFronzeAmt(),
					chargeCacheObj.getPayMode(),consumeRecord);
			logger.info("endchargeWithConsumeRecord wwwwwwwwwwwww,chargeUserId:{},chargeCacheObj:{},chargeUserId:{}",
					chargeUserId,chargeCacheObj);
			boolean isPauseStat = (chargeCacheObj.getStatus() == ChargeStatus.CS_PAUSE_STAT)?true:false;
			chargeCacheObj.setStatus(ChargeStatus.CS_STAT);
			//2.检查度数和时间
			consumeRecord = statChargeEvent(chargeUserId,consumeRecord);
			
			int totalAmt = 0;
			int totalChargeMeterNum=0;
			int totalChargeTime=0;
			
			//钱和电表读数正确，时间正确.记录正常订单.统计钱,电表读数,时间
			//钱和电表读数正确，时间错误.记录正常订单.统计钱,电表读数,统计时间为0
			//钱和电表读数错误，不管时间如何,记录异常订单.不做统计
			if(consumeRecord.getStatRet()>=0)//时间正确，取得时间统计值
			{
				totalChargeTime = consumeRecord.getChargeUseTimes();
				totalChargeMeterNum = consumeRecord.getTotalDl();
			}
			
			
			//钱和电表读数正确，那么统计
			int currentType = 0;//获取电桩类型
			ElectricPileCache epCache = EpService.getEpByCode(epCode);
			if(epCache !=null)
			{
				currentType = epCache.getCurrentType();
			}
			UserCache chargeUser = UserService.getUserCache(chargeUserId);
			
			int userFirst = EpChargeService.isNewCouponStatus(currentType,chargeUser.getNewcouponAcStatus(),chargeUser.getNewcouponDcStatus());
		
			//1.处理正常的消费记录数据
			int couPonAmt = 0; //优惠券金额
			if(consumeRecord.getStatAmtRet()>=0)
			{
				couPonAmt = EpChargeService.handleRightConsumeRecord(this,consumeRecord,
						 chargeCacheObj,chargeUser,currentType,totalChargeTime,
						 totalChargeMeterNum,isPauseStat);
				
				totalAmt = consumeRecord.getTotalAmt();
				
				//sendSCOConsumeRecord(consumeRecord.getTotalDl(),consumeRecord.getStartTime(),consumeRecord.getEndTime());
			}
			else
			{
				logger.error("endcharge consumeRecord StatAmtRet error,accountId:{},epCode:{},epGunNo:{},meterNum:{},totalAmt:{},chargeTime:{}", 
						new Object[]{chargeUserId,consumeRecord.getEpCode(),consumeRecord.getEpGunNo(),
						consumeRecord.getTotalDl(),totalAmt,consumeRecord.getChargeUseTimes()});
				
				//记录异常订单数据
				EpChargeService.updateChargeToDb(this,chargeCacheObj, 
						consumeRecord,true,new BigDecimal(0.0),0,0,chargeCacheObj.getRateInfo().getServiceRate());
				
			}
			logger.info("endcharge accountId:{},epCode:{},epGunNo:{}:chargeSerialNo:{},statAmtRet:{},statTimeRet:{},totalAmt:{},totalChargeMeterNum:{},totalChargeTime:{},couPonAmt:{}",
					new Object[]{chargeCacheObj.getUserId(),epCode,epGunNo,chargeCacheObj.getChargeSerialNo(),
					consumeRecord.getStatAmtRet(),consumeRecord.getStatRet(),totalAmt,totalChargeMeterNum,totalChargeTime,couPonAmt});
			
			//4.故障记录到故障表
			EpChargeService.insertFaultRecord((int)consumeRecord.getStopCause(),epCode,this.pkEpId,epGunNo,consumeRecord.getSerialNo(),new Date(consumeRecord.getEndTime()*1000));
			//5.非主动停止充电,给用户发短信
			if(consumeRecord.getStopCause() >2 && chargeUserAccount!=null)
			{
				this.onChargeNotice(consumeRecord.getStopCause(),chargeUserAccount);
			}
			//6.清除实时数据信息
			cleanChargeInfoInRealData();
			//7.给前段发消息
			if(PhoneService.isComm(chargeCacheObj.getUserOrigin()))
			{
				String chOrCode = chargeCacheObj.getChOrCode();
				Map<String, Object> consumeRecordMap = new ConcurrentHashMap<String, Object>();
				consumeRecordMap.put("userId",chargeUserId);
				consumeRecordMap.put("orderid",chOrCode);
				consumeRecordMap.put("st",chargeCacheObj.getSt());
				consumeRecordMap.put("et",chargeCacheObj.getEt());
				consumeRecordMap.put("totalMeterNum",consumeRecord.getTotalDl());
				consumeRecordMap.put("totalAmt",consumeRecord.getTotalChargeAmt());
				consumeRecordMap.put("serviceAmt",consumeRecord.getServiceAmt());
				consumeRecordMap.put("pkEpId",getPkEpId());
				consumeRecordMap.put("epCode",getEpCode());
				consumeRecordMap.put("chargStartEnergy",consumeRecord.getStartMeterNum());
				consumeRecordMap.put("chargEndEnergy",consumeRecord.getEndMeterNum());
				consumeRecordMap.put("account",consumeRecord.getEpUserAccount());
				consumeRecordMap.put("serialNo",consumeRecord.getSerialNo());
				//用户新手状态
				consumeRecordMap.put("userFirst",userFirst);
				//优惠券面值金额
				consumeRecordMap.put("couPonAmt",couPonAmt);
				//实际优惠金额
				int realCouPonAmt = EpChargeService.calcRealCouPonAmt(totalAmt,couPonAmt);//实际优惠金额
			    consumeRecordMap.put("realCouPonAmt",realCouPonAmt);
				
				
				handleEvent(EventConstant.EVENT_CONSUME_RECORD,0,0,null,(Object)consumeRecordMap);
				
				logger.info("endcharge send consumeRecord to phone,accountId:{},account:{},epCode:{},epGunNo:{},chargeSerialNo:{},userFirst:{},couPonAmt:{},realCouPonAmt:{}",
						new Object[]{chargeCacheObj.getUserId(),consumeRecord.getEpUserAccount(),epCode,epGunNo,
						chargeCacheObj.getChargeSerialNo(),userFirst,couPonAmt,realCouPonAmt});
			}
			else
			{
				logger.info("endcharge send consumeRecord to api,accountId:{},account:{},epCode:{},epGunNo:{},chargeSerialNo:{}",
						new Object[]{chargeCacheObj.getUserId(),consumeRecord.getEpUserAccount(),epCode,epGunNo,chargeCacheObj.getChargeSerialNo()});
				
					
				AppApiService.sendStopChargeByPhoneDisconnect(epCode,epGunNo, chargeUserId,1,0,
						consumeRecord.getChargeUseTimes());
			}
			
			//8.清空掉充电缓存对象
			cleanChargeInfo();
		
			return 1;
		}
		catch (Exception e) {
			logger.error("endcharge exception, message:{}",e.getMessage());
			return 3;
		}
		
	}
	
	
	
	
	
	
	/***
	 * 1:取消预约,2：预约到期，3：充电开始
	 */
	public void endBespoke(int style)
	{
		logger.debug("endBespoke,epCode:{},epGunNo:{},style:{}",new Object[]{epCode,epGunNo,style});
		BespCache bespCacheObj = getBespCache();
		if(bespCacheObj == null)
		{
			logger.info("endBespoke fail!bespCacheObj ==null");
			return ;
		}
		logger.info("endBespoke,accountId:{},bespNo:{},epCode:{},epGunNo:{},style:{}",
				new Object[]{bespCacheObj.getAccountId(),bespCacheObj.getBespNo(),epCode,epGunNo,style});
		
        
		if( bespCacheObj.getStatus() != EpConstant.BESPOKE_STATUS_LOCK)
		{
			logger.info("endBespoke fail!bespCacheObj:{}",bespCacheObj);
			return ;	
		}
		Date now = new Date();
		bespCacheObj.setEndTime(now.getTime()/1000);
		bespCacheObj.setRealEndTime(now.getTime()/1000);
		
		BigDecimal realBespAmt = EpBespokeService.statBespoke(bespCacheObj);
		
		EpBespokeService.endBespoke(epCode, realBespAmt, bespCacheObj, now);
		
		StatService.subBespoke();
	
		//5.给前端应答
		Map<String, Object> bespokeMap = new ConcurrentHashMap<String, Object>();

		bespokeMap.put("userId",bespCacheObj.getAccountId());
		bespokeMap.put("bespNo",bespCache.getBespNo());
		
		bespokeMap.put("amt",realBespAmt.doubleValue());
		
		BigDecimal curUserBalance = UserService.getRemainBalance(bespCacheObj.getAccountId());
		bespokeMap.put("remainAmt",curUserBalance.doubleValue());
		
		bespokeMap.put("account",bespCacheObj.getAccount());
		
		bespokeMap.put("epCode",epCode);
		bespokeMap.put("epGunNo",epGunNo);
		
		bespokeMap.put("st",bespCacheObj.getStartTime());
		bespokeMap.put("et",bespCacheObj.getEndTime());
		bespokeMap.put("style",style);
		
		handleEvent(EventConstant.EVENT_CANNEL_BESPOKE, 1, 0, null, bespokeMap);

		
		logger.info("endBespoke success,BespNo:{},accountId:{},account:{},realBespAmt:{},remainAmt:{},epcode:{},gunno:{}",new Object[]{
				bespCache.getBespNo(),bespCache.getAccountId(),bespCache.getAccount(),realBespAmt.doubleValue(),curUserBalance.doubleValue(),this.epCode,this.epGunNo});
		
		cleanBespokeInfo();
	}
	public boolean initBootLoader(ElectricPileCache epCache,int epGunNo)
	{
		String epCode = epCache.getCode();
		int currentType = epCache.getCurrentType();
	
		if(currentType!=EpConstant.EP_DC_TYPE && currentType!= EpConstant.EP_AC_TYPE)
		{
			logger.error("initBootLoader,initConnect,epCode:{},initGun fail,currentType:{} error",epCode,currentType);
			return false;
		}
		
		this.currentType = currentType;
		TblElectricPileGun tblEpGun= EpGunService.getDbEpGun(pkEpId,epGunNo);
		if(tblEpGun==null)
		{
			logger.error("initBootLoader,initConnect,epCode:{},initGun fail,did not find gun,pkEpId:{},epGunNo:{}",
					new Object[]{epCode,pkEpId,epGunNo});
			return false;
		}
		
		this.chargeCache = null;
		this.bespCache = null;
	
		this.setPkEpGunId(tblEpGun.getPkEpGunId());
		
		this.concentratorId = epCache.getConcentratorId();
		this.identyCode = tblEpGun.getQr_codes();
		this.createIdentyCodeTime = tblEpGun.getQrdate()-GameConfig.identycodeTimeout2;
		if(this.realChargeInfo == null)
		{
			//1.初始化实时数据
		    RealChargeInfo tmpRealChargeInfo =null;
		    if(currentType == EpConstant.EP_DC_TYPE)
		    {
			  RealDCChargeInfo chargeInfo = new RealDCChargeInfo();
			  tmpRealChargeInfo = chargeInfo;
		    }
		    else
		    {
			  RealACChargeInfo chargeInfo = new RealACChargeInfo();
			  tmpRealChargeInfo = chargeInfo;
		    }
		    tmpRealChargeInfo.init();
		    tmpRealChargeInfo.setCurrentType(currentType);
		    tmpRealChargeInfo.setEpCode(epCode);
		    tmpRealChargeInfo.setEpGunNo(epGunNo);
		    //1.装载实时数据
		    boolean loadSuccess = tmpRealChargeInfo.loadFromDb(epCode, epGunNo);
		    if(!loadSuccess)
		    {
			   if(currentType==EpConstant.EP_DC_TYPE)
				   logger.error("initBootLoader,initConnect,epCode:{},initGun fail,did not load in tbl_chargeinfo_dc,epGunNo:{}",
							new Object[]{epCode,epGunNo});
			   else
				   logger.error("initBootLoader,initConnect,epCode:{},initGun fail,did not load in tbl_chargeinfo_ac,epGunNo:{}",
							new Object[]{epCode,epGunNo});
			   
			   return false;
		    }
		    this.realChargeInfo = tmpRealChargeInfo;
		}
		
		this.modifyStatus(EpConstant.EP_GUN_STATUS_EP_UPGRADE, true);
		
		logger.info("initBootLoader,initConnect,epCode:{},initGun success,epGunNo:{},status:{}",
				new Object[]{epCode,epGunNo,status});
		
		
		return true;
	}

	public boolean init(ElectricPileCache epCache,int epGunNo)
	{
		String epCode = epCache.getCode();
		int currentType = epCache.getCurrentType();
	
		if(currentType!=EpConstant.EP_DC_TYPE && currentType!= EpConstant.EP_AC_TYPE)
		{
			logger.error("init,initConnect,epCode:{},initGun fail,currentType:{} error",epCode,currentType);
			return false;
		}
		
		this.currentType = currentType;
		TblElectricPileGun tblEpGun= EpGunService.getDbEpGun(pkEpId,epGunNo);
		if(tblEpGun==null)
		{
			logger.error("init,initConnect,epCode:{},initGun fail,did not find gun,pkEpId:{},epGunNo:{}",
					new Object[]{epCode,pkEpId,epGunNo});
			return false;
		}
		
		this.chargeCache = null;
		this.bespCache = null;
	
		this.setPkEpGunId(tblEpGun.getPkEpGunId());
		
		this.concentratorId = epCache.getConcentratorId();
		this.identyCode = tblEpGun.getQr_codes();
		this.createIdentyCodeTime = tblEpGun.getQrdate()-GameConfig.identycodeTimeout2;
		
		
		//1.初始化实时数据
		RealChargeInfo tmpRealChargeInfo =null;
		if(currentType == EpConstant.EP_DC_TYPE)
		{
			RealDCChargeInfo chargeInfo = new RealDCChargeInfo();
			tmpRealChargeInfo = chargeInfo;
		}
		else
		{
			RealACChargeInfo chargeInfo = new RealACChargeInfo();
			tmpRealChargeInfo = chargeInfo;
		}
		tmpRealChargeInfo.init();
		tmpRealChargeInfo.setCurrentType(currentType);
		tmpRealChargeInfo.setEpCode(epCode);
		tmpRealChargeInfo.setEpGunNo(epGunNo);
		//1.装载实时数据
		boolean loadSuccess = tmpRealChargeInfo.loadFromDb(epCode, epGunNo);
		
		if(!loadSuccess)
		{
			if(currentType==EpConstant.EP_DC_TYPE)
			{
				logger.error("init,initConnect,epCode:{},initGun fail,did not load in tbl_chargeinfo_dc,epGunNo:{}",
						new Object[]{epCode,epGunNo});
			}
			else
			{
				logger.error("init,initConnect,epCode:{},initGun fail,did not load in tbl_chargeinfo_ac,epGunNo:{}",
						new Object[]{epCode,epGunNo});
				
			}
			return false;
		}
		this.realChargeInfo = tmpRealChargeInfo;
		
		
		int epGunStatusInDb= tblEpGun.getEpState();
		//以数据库最后枪头状态为准
		this.modifyStatus(epGunStatusInDb, false);

		//2.取最新的预约中的预约记录
		BespCache tmpBespCache=null;
		TblBespoke besp = EpBespokeService.getUnStopBespokeFromDb(this.pkEpId, this.pkEpGunId);
		if (besp != null) {
			tmpBespCache = EpBespokeService.makeBespokeCache(besp);
			tmpBespCache.setEpCode(this.epCode);
			tmpBespCache.setEpGunNo(this.epGunNo);
			
			// 检查是否过期,如果过期.那么结算
			long diff  = EpBespokeService.expireTime(tmpBespCache);
		
			if (diff > 0) {
				//结算
				Date now = new Date();
				tmpBespCache.setRealEndTime(now.getTime()/1000);
				
				if(tmpBespCache.getRealEndTime() > tmpBespCache.getEndTime())
				{
					tmpBespCache.setRealEndTime(tmpBespCache.getEndTime());
				}
				logger.debug("init,initConnect,epCode:{},besp != null, diff > 0,epGunNo:{}",epCode,epGunNo);
				BigDecimal realBespAmt = EpBespokeService.statBespoke(tmpBespCache);
				EpBespokeService.endBespoke(epCode, realBespAmt, tmpBespCache, now);
			
				tmpBespCache=null;
			} else {
				tmpBespCache.setStatus(EpConstant.BESPOKE_STATUS_LOCK);
			}
		}
		if(tmpBespCache!=null)
		{
			logger.debug("init,initConnect,epCode:{},has besp in DB,epGunNo:{}",epCode,epGunNo);
			String chargeAccount = tmpBespCache.getAccount();
			//装载未完成充电用户
			UserCache userCache = UserService.getUserCache(chargeAccount);
			
			if(userCache!=null)
			{
				logger.debug("init,epCode:{},epGunNo:{},set bespinfo to userCache:{}",new Object[]{epCode,epGunNo,userCache});
				
				userCache.addBesp(tmpBespCache);
			}
			this.bespCache = tmpBespCache;
			
		}
		//3.取最新的未完成的充电记录
		ChargeCache tmpChargeCache=EpChargeService.getUnFinishChargeFromDb(epCode, epGunNo);
		if(tmpChargeCache!=null)
		{
			logger.debug("init,initConnect,epCode:{},has charge in DB,epGunNo:{},status():{}",
					new Object[]{epCode,epGunNo,tmpChargeCache.getStatus()});
			
			String chargeAccount = tmpChargeCache.getAccount();
			//装载未完成充电用户
			UserCache userCache = UserService.getUserCache(chargeAccount);
			
			tmpChargeCache.setEpCode(epCode);
			tmpChargeCache.setEpGunNo(epGunNo);
			String chorTransactionnumber = tmpChargeCache.getChargeSerialNo();
			int pkOrderId= EpChargeService.getOrderId(chorTransactionnumber);
			
			tmpChargeCache.setPkOrderId(pkOrderId);
			
			
			if(userCache!=null)
			{
				logger.debug("init,epCode:{},epGunNo:{},set chargeInfo to userCache:{}",new Object[]{epCode,epGunNo,userCache});
				
				
				userCache.addCharge(tmpChargeCache);
				
				tmpChargeCache.getUserOrigin().setCmdChIdentity(userCache.getAccount());
			}
			
			
			this.chargeCache = tmpChargeCache;
		}
		
		
		
		
		logger.info("init,initConnect,epCode:{},initGun success,epGunNo:{},status:{}",
				new Object[]{epCode,epGunNo,status});
		
		return true;
	}


	@Override
	public String toString() {
		final StringBuilder sb = new StringBuilder();
		
        sb.append("EpGunCache\n");
        sb.append("集中器pkId = ").append(this.getConcentratorId()).append("\n");
        sb.append("电桩pkId = ").append(this.getPkEpId()).append("\n");
        sb.append("电桩编号 = ").append(this.getEpCode()).append("\n");
        
        sb.append("枪口pkId = ").append(this.pkEpGunId).append("\n");
        sb.append("枪口编号 = ").append(this.epGunNo).append("\n");
        //sb.append("当前用户ID = ").append(curUserId).append("\n");
        //sb.append("当前用户账号 = ").append(curUserAccount).append("\n");
        
        sb.append("识别码 = ").append(identyCode).append("\n");
        
        String sTime= "";
        sTime = DateUtil.longDateToStrring(createIdentyCodeTime*1000);
        sb.append("识别码产生时间 = ").append(sTime).append("\n");
        
        sTime= DateUtil.longDateToStrring(lastSendToMonitorTime*1000);
        sb.append("监控实时数据更新时间= ").append(sTime).append("\n");
        
        sTime= DateUtil.longDateToStrring(this.lastUDTime*1000);
        sb.append("数据库实时数据更新时间 = ").append(sTime).append("\n");
        
        sTime= DateUtil.longDateToStrring(lastUPTime*1000);
        sb.append("手机充电信息更新时间  = ").append(sTime).append("\n");
        
        EpCommClient commClient =(EpCommClient)this.epNetObject;
        if(commClient == null)
        {
        	sb.append("not find comm client\n");
        }
        else
        {
        	sb.append("\nEpCommClient:").append(commClient.toString()).append("\n");
        }
       
        sb.append("枪口状态 = "+status+"(" ).append( EpGunService.getGunStatusDesc(status)).append( ")\n");
        if(this.realChargeInfo ==null)
        {
        	sb.append("无实时数据\n");
        }
        else
        {
        	sb.append("实时数据工作状态 = ").append(this.realChargeInfo.getWorkingStatus() ).append(this.realChargeInfo.getWorkingStatusDesc()).append("\n\n");
        }
        
        if(this.authCache ==null)
        {
        	sb.append("无鉴权用户\n");
        }
        else
        {
        	sb.append(this.authCache.toString() ).append("\n");
        }
        
        if(this.bespCache ==null)
        {
        	sb.append("无预约\n\r\n");
        }
        else
        {
        	sb.append(this.bespCache.toString() ).append("\n");
        }
        
        if(this.chargeCache ==null)
        {
        	sb.append("无充电\n\r\n");
        }
        else
        {
        	sb.append(this.chargeCache.toString() ).append("\n");
        }
        
      
        return sb.toString();
	}
	
	public UserOrigin getBespokeUserOrigin()
	{
		if(bespCache==null)
			return null;
		return bespCache.getUserOrigin();
	}
	public UserOrigin getChargeUserOrigin()
	{
		if(chargeCache==null)
			return null;
		return chargeCache.getUserOrigin();
	}
	public UserOrigin getActionUserOrigin(int action)
	{
		UserOrigin userOrigin=null;
	
		switch(action)
		{
		case EventConstant.EVENT_BESPOKE:
			userOrigin = getBespokeUserOrigin();
			break;
		case EventConstant.EVENT_CANNEL_BESPOKE:
			userOrigin = getBespokeUserOrigin();
			break;
		case EventConstant.EVENT_CHARGE:
			userOrigin = getChargeUserOrigin();
			break;
		case EventConstant.EVENT_STOP_CHARGE:
			userOrigin = getChargeUserOrigin();
			break;
		case EventConstant.EVENT_START_CHARGE_EVENT:
			userOrigin = getChargeUserOrigin();
			break;
		case EventConstant.EVENT_REAL_CHARGING:
			userOrigin = getChargeUserOrigin();
			break;
		
		case EventConstant.EVENT_CARD_AUTH:
			userOrigin = getChargeUserOrigin();
			break;
		case EventConstant.EVENT_EP_STATUS:
			userOrigin = getChargeUserOrigin();
			break;
		case EventConstant.EVENT_CONSUME_RECORD:
			userOrigin = getChargeUserOrigin();
			break;
		
		case EventConstant.EVENT_EP_STAT:
			userOrigin = getChargeUserOrigin();
			break;
		case EventConstant.EVENT_EP_NET_STATUS:
			userOrigin = getChargeUserOrigin();
			break;
		
		default:
			break;
		}
		return userOrigin;
	}
	public int getDefaultUserOrigin(int action)
	{
		int userOrigin=-1;
	
		switch(action)
		{
		case EventConstant.EVENT_BESPOKE:
			
			break;
		case EventConstant.EVENT_CANNEL_BESPOKE:
			userOrigin = 0;//取消预约默认
			break;
		case EventConstant.EVENT_CHARGE:
			
			break;
		case EventConstant.EVENT_STOP_CHARGE:
			
			break;
		case EventConstant.EVENT_START_CHARGE_EVENT:
			
			break;
		case EventConstant.EVENT_REAL_CHARGING:
			
			break;
		
		case EventConstant.EVENT_CARD_AUTH:
			
			break;
		case EventConstant.EVENT_EP_STATUS:
			break;
		case EventConstant.EVENT_CONSUME_RECORD:
			userOrigin=0;
			break;
		
		case EventConstant.EVENT_EP_STAT:
			
			break;
		case EventConstant.EVENT_EP_NET_STATUS:
			
			break;
		case EventConstant.EVENT_ONE_BIT_YX:
		case EventConstant.EVENT_TWO_BIT_YX:
		case EventConstant.EVENT_YC:
		case EventConstant.EVENT_VAR_YC:
		
			userOrigin= UserConstant.CMD_FROM_MONTIOR;
			break;
		default:
			break;
		}
		return userOrigin;
	}
	public void onEvent(int action,int source,UserOrigin userOrigin,int ret,int cause,Object w,Object extraData)
	{
		switch(source)
		{
		case UserConstant.CMD_FROM_API://app api
			AppApiService.onEvent(action,userOrigin,ret,cause,w,extraData);
			break;
		case UserConstant.CMD_FROM_PHONE://phone client
			
			PhoneService.onEvent(action,userOrigin,ret,cause,w,extraData);
			break;
		case UserConstant.CMD_FROM_MONTIOR://phone client
			AnalyzeService.onEvent(action,userOrigin,ret,cause,w,extraData);
		case UserConstant.ORG_PARTNER_MOBILE:
			ChinaMobileService.onEvent(action, userOrigin,ret,cause,w,extraData);
			break;
		
		default:
			logger.error("onEvent,epCode:{},epGunNo:{},error source:{}",
					new Object[]{epCode,epGunNo,source});
			break;
				
		}
		
	}
	public void handleEvent(int action,int ret,int cause,Object w,Object extraData)
	{
		UserOrigin userOrigin = getActionUserOrigin(action);
		
		logger.debug("handleEvent,epCode:{},epGunNo:{},action:{}.userOrigin:{}",
				new Object[]{epCode,epGunNo,action,userOrigin});
		if(userOrigin!=null)
		{
			onEvent(action,userOrigin.getCmdFromSource(),userOrigin,ret,cause,w,extraData);
		}
		else
		{
			logger.debug("handleEvent,epCode:{},epGunNo:{},action:{},userOrigin:{},curUserId:{}",
					new Object[]{epCode,epGunNo,action,userOrigin,this.getCurUserId()});
		}
		
	}
	
	public void handleEventExtra(int action,int source,int ret,int cause,Object w,Object extraData)
	{
		
		onEvent(action,source,null,ret,cause,w,extraData);
		
	}
	

	public int onEpCancelBespRet(EpCommClient epCommClient, EpCancelBespResp cancelBespResp)
	{
		logger.debug("onEpCancelBespRet,epCommClient:{},cancelBespResp:{}",epCommClient,cancelBespResp);
		
		if(bespCache!=null)
		{
			if(bespCache.getBespNo().compareTo(cancelBespResp.getBespNo()) !=0)
			{
				logger.debug("onEpCancelBespRet bespCache.getBespNo:{},cancelBespResp.getBespNo:{}", bespCache.getBespNo(),cancelBespResp.getBespNo());
				return 0;
			}
			Map<String, Object> bespokeMap = new ConcurrentHashMap<String, Object>();
			int curUserId =  bespCache.getAccountId();
			bespokeMap.put("userId",curUserId);
			//bespokeMap.put("redo",nRedo);
			bespokeMap.put("bespNo",bespCache.getBespNo());
			
			
			if(cancelBespResp.getSuccessFlag() ==0)//取消预约失败
			{	//5.给前端应答
				handleEvent(EventConstant.EVENT_CANNEL_BESPOKE, 0, 0, null, bespokeMap);
			}
			else////取消预约成功
			{
				this.endBespoke( EpConstant.END_BESPOKE_CANCEL);
			}
		}
		else
		{
			logger.debug("onEpCancelBespRet bespCache=null,epCode:{},epGunNo:{}", this.epCode,this.epGunNo);
		}
	
		return 0;
	}
	public int onEpBespokeResp(EpBespResp bespResp)
	{
		if(bespCache==null)
			return 2; //数据不存在

		String epBespokeNo= bespResp.getBespNo();
		String bespokeNo= this.bespCache.getBespNo();
		
		if(bespokeNo.compareTo(epBespokeNo)!=0)
		{
			return 2; //数据不存在
		}
		if(getStatus() == EpConstant.EP_GUN_STATUS_BESPOKE_LOCKED && bespCache.getRedo()!=1)
		{
			return 3;//已经处理
		}
		int nEpRedo = bespResp.getnRedo();
		if(bespCache.getRedo()!=nEpRedo) //下发的预约标识和回复的预约标识不一致
		{
			return 2;//数据不存在
		}
		
		int errorCode=2;
		if(bespResp.getSuccessFlag() == 1)
		{
			int curUserId =  getCurUserId();
			errorCode = onEpBespSuccess( bespResp.getBespNo(), bespResp.getnRedo(),curUserId);
		}
		else
		{
			errorCode = onEpBespFail(bespResp.getBespNo(),bespResp.getnRedo());
		}
		return errorCode;
	}
	public int onEpBespSuccess(String bespNo,int nRedo,int usrId)
	{
		logger.info("bespoke response success,accountId:{},bespNo:{},redoFlag:{},epCode:{},epGunNo:{}",
				new Object[]{usrId,bespNo,nRedo,epCode,epGunNo});
		//算钱和时间,并且保存到数据库
		if (nRedo == 0) {
			
			this.onBespokeSuccess(1);
		} else {
			onRedoBespokeSuccess();
		}
		    
		return 1;
	}
	public void do_bespoke_consume_resp(int ret,int cause,int userId,int redo,String bespokeNo)
	{
		//5.给前端应答
		Map<String, Object> bespokeMap = new ConcurrentHashMap<String, Object>();

		bespokeMap.put("userId",userId);
		bespokeMap.put("redo",redo);
		bespokeMap.put("bespNo",bespokeNo);
		
		logger.debug("do_bespoke_consume_resp,EventConstant.EVENT_BESPOKE.ret:{}, cause:{}",ret, cause);
		
		handleEvent(EventConstant.EVENT_BESPOKE, ret, cause, null, bespokeMap);
	}
	public int onEpBespFail(String bespNo,int nRedo)
	{
		int userId = this.getCurUserId();
		logger.info("bespoke response fail,accountId:{},bespNo:{},redoFlag:{},epCode:{},epGunNo:{}",
				new Object[]{userId,bespNo,nRedo,epCode,epGunNo});
		//电桩可能会发多条预约上来
		if(this.bespCache!=null && this.bespCache.getStatus() == EpConstant.BESPOKE_STATUS_CONFIRM)
		{
			do_bespoke_consume_resp(0,0,userId,nRedo,bespNo);
		
			String messagekey = String.format("%03d%s", Iec104Constant.C_BESPOKE,bespCache.getBespNo());
		
			EpGunService.removeRepeatMsg(messagekey);
		
			if (nRedo == 0)// 预约失败
			{
				bespCache.setStatus(EpConstant.BESPOKE_STATUS_FAIL);
				
				this.cleanBespokeInfo();
			}
		}
		
		return 1;
	}
	
	public int dropCarPlaceLockAction()
	{
		EpCommClient commClient= (EpCommClient)getEpNetObject();
		if(commClient ==null || !commClient.isComm())
		{
			logger.debug("dropCarPlaceLockAction commClient is null,epCode:{},epGunNo:{}",this.epCode,this.epGunNo);
			return EpConstantErrorCode.EP_UNCONNECTED;//
		}
		
		if(!commClient.isComm())
		{
			
			return EpConstantErrorCode.EP_UNCONNECTED;
		}
		byte[] data = EpEncodeProtocol.do_drop_carplace_lock(epCode, epGunNo);
		
		byte[] cmdTimes = WmIce104Util.timeToByte();
		EpMessageSender.sendMessage(commClient,0,0, Iec104Constant.C_DROP_CARPLACE_LOCK,data,cmdTimes,commClient.getVersion());
		
		return 0;
	}

	
	// 过期,强制结束
	public void forceEndBespoke() 
	{
		BespCache bespokeCache = this.getBespCache();
		if(bespokeCache==null || bespokeCache.getStatus() != EpConstant.BESPOKE_STATUS_LOCK)
			return ;
		
		EpCommClient commClient = (EpCommClient)getEpNetObject();
		if(commClient!=null && commClient.isComm())
		{
			//向桩发取消预约
			byte[] cancelMsg = EpEncodeProtocol.do_cancel_bespoke( epCode, (byte)epGunNo, bespokeCache.getBespNo());
		    byte[] cmdTimes = WmIce104Util.timeToByte();
			EpMessageSender.sendMessage(commClient, (short)0, 0, (byte)Iec104Constant.C_CANCEL_BESPOKE, cancelMsg,cmdTimes,commClient.getVersion());
		}
		else
		{
			this.endBespoke( EpConstant.END_BESPOKE_EXPIRE_TIME);
			
		}
	}
	private ChargingInfo calcCharingInfo()
	{
		
		long now = DateUtil.getCurrentSeconds();
		
		this.lastUPTime= now;
		ChargingInfo charingInfo = new ChargingInfo();
		
		charingInfo.setTotalTime(this.realChargeInfo.getChargedTime());
		charingInfo.setChargeMeterNum(this.realChargeInfo.getChargedMeterNum());
		if(this.chargeCache!=null)
		{
			charingInfo.setFronzeAmt(this.chargeCache.getFronzeAmt());
		}
		else
		{
			charingInfo.setFronzeAmt(0);
		}
		
		
		charingInfo.setOutCurrent(this.realChargeInfo.getOutVoltage());
		charingInfo.setOutVol(this.realChargeInfo.getOutCurrent());
		charingInfo.setChargeAmt(this.realChargeInfo.getChargedCost());
		charingInfo.setRateInfo(realChargeInfo.getChargePrice()/10);
		if(currentType == EpConstant.EP_DC_TYPE)
		{
			charingInfo.setSoc(((RealDCChargeInfo)realChargeInfo).getSoc());
		}
		else
		{
			charingInfo.setSoc(0);
		}
		charingInfo.setDeviceStatus(0);
		charingInfo.setWarns(0);
		charingInfo.setWorkStatus(this.status);
		
		return charingInfo;
		
	}
	public void pushFirstRealData()
	{
		ChargingInfo  chargingInfo = calcCharingInfo();
		if(chargingInfo!=null)
		{
			Map<String ,Object> respMap = new ConcurrentHashMap<String, Object>();
			respMap.put("epcode", epCode);
			respMap.put("epgunno", epGunNo);
			
			handleEvent(EventConstant.EVENT_REAL_CHARGING,0,0,respMap,(Object)chargingInfo);
		}
	}
	

	
	
	public  void  onChargeNotice(int stopCause,String curUserAccount)
	{
		logger.debug("onChargeNotice send msg,stopCause:{},curUserAccount:{}",stopCause,curUserAccount);
		String stopChargeDesc= EpChargeService.getStopChargeDesc(stopCause);
		SimpleDateFormat dateFormat = new SimpleDateFormat("yyyy/MM/dd HH:mm:ss");
		
		if(GameConfig.sms ==1)
		{
			try
			{
				String content = MessageFormat.format("结束充电提醒：尊敬的用户，由于{0}，您的本次充电于{1}结束，请收枪后查看结算信息。", stopChargeDesc,dateFormat.format(new Date()));
				MobiCommon.sendWanMatMessage(content,curUserAccount);
			}
			catch(Exception e)
			{
				logger.error("onChargeNotice fail,e.getMessage:{}",e.getMessage());
				
			}
		}
		else
		{
		
			HashMap<String,Object>  params=new HashMap<String,Object>();
			params.put("error", stopChargeDesc);
			params.put("time", dateFormat.format(new Date()));
			
			JSONObject jsonObject = JSONObject.fromObject(params);
			
			boolean flag=AliSMS.sendAliSMS(curUserAccount, "SMS_16775083", jsonObject.toString());
			if(!flag)
			{
				logger.debug("SMS:onChargeNotice fail,userAccount:{}",curUserAccount);
			}
		}
	}
	
	public void dispatchWholeRealToMonitor(int preWorkStatus)
	{
		logger.debug("realData dispatchWholeRealToMonitor,epCode:{},epGunNo:{}",epCode,epGunNo);
		Map<Integer, SingleInfo> oneYxRealInfo=null;
		if(currentType== EpConstant.EP_AC_TYPE)
			oneYxRealInfo = ((RealACChargeInfo)realChargeInfo).getWholeOneBitYx();
		else
			oneYxRealInfo = ((RealDCChargeInfo)realChargeInfo).getWholeOneBitYx();
		
		
		 sendToMonitor(EventConstant.EVENT_ONE_BIT_YX,null,0,0,(Object)oneYxRealInfo);
			
		 Map<Integer, SingleInfo> twoYxRealInfo=null;
		 if(currentType== EpConstant.EP_AC_TYPE)
			 twoYxRealInfo = ((RealACChargeInfo)realChargeInfo).getWholeTwoBitYx();
		else
			twoYxRealInfo = ((RealDCChargeInfo)realChargeInfo).getWholeTwoBitYx();
			
		 
		 sendToMonitor(EventConstant.EVENT_TWO_BIT_YX,null,0,0,(Object)twoYxRealInfo);
			
		 Map<Integer, SingleInfo> ycRealInfo=null;
		 if(currentType== EpConstant.EP_AC_TYPE)
			 ycRealInfo = ((RealACChargeInfo)realChargeInfo).getWholeYc();
		else
			ycRealInfo = ((RealDCChargeInfo)realChargeInfo).getWholeYc(preWorkStatus);
			
		
		//遥测
		 sendToMonitor(EventConstant.EVENT_YC,null,0,0,(Object)ycRealInfo);
		 
		 
		 Map<Integer, SingleInfo> varYcRealInfo=null;
		 if(currentType== EpConstant.EP_AC_TYPE)
			 varYcRealInfo = ((RealACChargeInfo)realChargeInfo).getWholeVarYc();
		else
			varYcRealInfo = ((RealDCChargeInfo)realChargeInfo).getWholeVarYc();
		
		 
		sendToMonitor(EventConstant.EVENT_VAR_YC,null,0,0,(Object)varYcRealInfo);
	
		changeYcMap.remove(1);
		long now = DateUtil.getCurrentSeconds();
		setLastSendToMonitorTime(now);
		
	}
	
	
	
	public int endBQConsumeRecord(NoCardConsumeRecord consumeRecord )
	{
		String cardInNo= consumeRecord.getEpUserAccount();
		
		int orgNo = consumeRecord.getUserOrgin();
		int user_id = UserService.getUserIdByOrgNo(orgNo);
		if(user_id<=0)
		{
			logger.info("endcharge endBQConsumeRecord not find user info,cardInNo:{},orgNo:{}",cardInNo,orgNo);
			return 4;
		}
		//检查有没有卡，如果没有，插入一条
		ChargeCardCache cardCache= UserService.getCard(cardInNo);
		int pkCardId = 0;
		if(cardCache==null)
		{
			pkCardId = UserService.insertBQCard(cardInNo, orgNo, user_id);
		}
		else
		{
			pkCardId=cardCache.getId();
		}
		
		UserCache cardUser= UserService.getUserCache(user_id);
		
		
		int orderStatus = EpChargeService.getChargeOrderStatus(consumeRecord.getSerialNo());
		logger.debug("endcharge endBQConsumeRecord serialNo:{},orderStatus:{}",consumeRecord.getSerialNo(),orderStatus);
		if(orderStatus==2|| orderStatus==3)//
			return 3;
		if(cardUser==null)
		{
			logger.info("endcharge endBQConsumeRecord not find user info,cardInNo:{}",cardInNo);
			return 4;
		}
		logger.debug("endcharge endBQConsumeRecord cardUser:{}",cardUser);
		
		RateInfo rateInfo = RateService.getRateInfo(getEpCode());
		String chOrCode = EpChargeService.makeChargeOrderNo(this.pkEpGunId,cardUser.getId());
		
		int chorType= EpChargeService.getOrType(cardUser.getLevel());
		
		int chargeTime = (int)((consumeRecord.getEndTime()-consumeRecord.getStartTime())/60);			
		EpChargeService.addChargeStat(getPkEpGunId(),consumeRecord.getTotalDl(),chargeTime,consumeRecord.getTotalAmt());

		
		EpChargeService.insertChargeWithConsumeRecord(cardUser.getId(),chorType,cardUser.getAccount(),pkCardId,consumeRecord.getUserOrgin(),getPkEpGunId(),
				getEpCode(),getEpGunNo(),EpConstant.CHARGE_TYPE_CARD,"",chOrCode,2,consumeRecord,rateInfo);
		
		cardUser.removeCharge(consumeRecord.getSerialNo());
		return 1;
	}
	/**
	 *处理大账户信用卡
	 * @param consumeRecord
	 * @return  4：无效的交易流水号
	 * 			3:已经处理
				2:数据不存在
				1:处理成功
	 */
	public int endCreditConsumeRecord(NoCardConsumeRecord consumeRecord )
	{
		String cardInNo= consumeRecord.getEpUserAccount();
		ChargeCardCache cardCache = UserService.getCard(cardInNo);
		if(cardCache==null)
		{
			logger.info("endcharge endCreditConsumeRecord not find user info,cardInNo:{}",cardInNo);
			return 4;
		}
		UserCache cardUser= UserService.getUserCache(cardCache.getUserId());
		
		int orderStatus = EpChargeService.getChargeOrderStatus(consumeRecord.getSerialNo());
		logger.debug("endcharge endCreditConsumeRecord serialNo:{},orderStatus:{}",consumeRecord.getSerialNo(),orderStatus);
		if(orderStatus==2|| orderStatus==3)//
			return 3;
		if(cardUser==null)
		{
			logger.info("endcharge endCreditConsumeRecord not find user info,cardInNo:{}",cardInNo);
			return 4;
		}
		logger.debug("endcharge endCreditConsumeRecord cardUser:{}",cardUser);
		
		RateInfo rateInfo = RateService.getRateInfo(getEpCode());
		String chOrCode = EpChargeService.makeChargeOrderNo(this.pkEpGunId,cardUser.getId());
		
		int chorType= EpChargeService.getOrType(cardUser.getLevel());
		
		int chargeTime = (int)((consumeRecord.getEndTime()-consumeRecord.getStartTime())/60);			
		EpChargeService.addChargeStat(getPkEpGunId(),consumeRecord.getTotalDl(),chargeTime,consumeRecord.getTotalAmt());

		
		EpChargeService.insertChargeWithConsumeRecord(cardUser.getId(),chorType,cardUser.getAccount(),cardCache.getId(),consumeRecord.getUserOrgin(),getPkEpGunId(),
				getEpCode(),getEpGunNo(),EpConstant.CHARGE_TYPE_CARD,"",chOrCode,2,consumeRecord,rateInfo);
		
		return 1;
		
	}
	public void handleGun2CarLinkStatus(int status)
	{
		int usrId = this.getCurUserId();
		if(usrId == 0)
			return ;
		UserCache  u= UserService.getUserCache(usrId);
		if(u==null)
			return ;
		PhoneService.handleGun2CarLinkStatus(status,u.getAccount());	
	}
	/**
	 *处理萍乡特殊的消费记录
	 * @param consumeRecord
	 * @return  4：无效的交易流水号
	 * 			3:已经处理
				2:数据不存在
				1:处理成功
	 */
	public int endPXConsumeRecord(NoCardConsumeRecord consumeRecord )
	{
		
		String account= GameConfig.bigAccount1002;
		logger.debug("endcharge endPXConsumeRecord,account:{}",account);
		
		
		UserCache cardUser= UserService.getUserCache(account);
		if(cardUser==null)
		{
			logger.info("endcharge endPXConsumeRecord not find user info,account:{}",account);
			return 4;
		}
		logger.debug("endcharge endPXConsumeRecord,cardUser:{}",cardUser);
		
			
		
		RateInfo rateInfo = RateService.getRateInfo(getEpCode());
		
		String chOrCode = EpChargeService.makeChargeOrderNo(this.pkEpGunId,cardUser.getId());
		
		
		int chorType= EpChargeService.getOrType(3);
		
			
		int chargeTime = (int)((consumeRecord.getEndTime()-consumeRecord.getStartTime())/60);
		logger.debug("endcharge endPXConsumeRecord 4-1002-5:chargeTime{}",chargeTime);
		EpChargeService.addChargeStat(getPkEpGunId(),consumeRecord.getTotalDl(),chargeTime,consumeRecord.getTotalAmt());

		
		EpChargeService.insertChargeWithConsumeRecord(cardUser.getId(),chorType,cardUser.getAccount(),0,consumeRecord.getUserOrgin(),getPkEpGunId(),
				getEpCode(),getEpGunNo(),EpConstant.CHARGE_TYPE_ACCOUNT,"",chOrCode,2,
				consumeRecord,rateInfo);
		
		return 1;
		
	}
	public int canRedoBespoke(int bespUsrId,String redoBespNo)
	{
		if(bespCache ==null)
			return EpConstantErrorCode.BESP_NO_NOT_EXIST;// 
		
		String bespNo = bespCache.getBespNo();
		if(bespNo.compareTo(redoBespNo)!=0)//没有同编号的预约
		{
			logger.info("canRedoBespoke,bespNo:{},redoBespNo",bespNo,redoBespNo);
			return EpConstantErrorCode.BESP_NO_NOT_EXIST;//
		}
		return 0;
		
	}
	public int canBespoke(int bespUsrId)
	{
		//先判断业务
		if( status == EpConstant.EP_GUN_STATUS_CHARGE||
		    status == EpConstant.EP_GUN_STATUS_BESPOKE_LOCKED||
		    status== EpConstant.EP_GUN_STATUS_USER_AUTH)
		{
			int curUsrId = getCurUserId();
					
			 if(status == EpConstant.EP_GUN_STATUS_CHARGE )
			 {
				 if(curUsrId!=bespUsrId)
				 {
					 return EpConstantErrorCode.EPE_OTHER_CHARGING;
				 }
				 else
				 {
					 return EpConstantErrorCode.CAN_NOT_BESP_IN_ELE;
				 }
			 }
			 else if(status == EpConstant.EP_GUN_STATUS_BESPOKE_LOCKED)
			 {
				 if(curUsrId!=bespUsrId)
				 {
					 return EpConstantErrorCode.EPE_OTHER_BESP;
				 }
				 else
				 {
					 return EpConstantErrorCode.SELF_HAVED_BESP;
				 }
			 }
			 else
			 {
				return EpConstantErrorCode.USED_GUN;
			 }
		}
		else if( status ==  EpConstant.EP_GUN_STATUS_SETTING||
				 status == EpConstant.EP_GUN_STATUS_EP_OPER||
			     status == EpConstant.EP_GUN_STATUS_SELECT_CHARGE_MODE||
				    status ==  EpConstant.EP_GUN_STATUS_WAIT_CHARGE)
		{
			return EpConstantErrorCode.EPE_IN_EP_OPER;//
		}
		else if( status>30 || 
				 status == EpConstant.EP_GUN_STATUS_EP_INIT||
				 status== EpConstant.EP_GUN_STATUS_OFF_LINE||
			     status == EpConstant.EP_GUN_STATUS_STOP_USE)
		{
			return EpConstantErrorCode.EPE_GUN_FAULT;
		}	
		else if( status == EpConstant.EP_GUN_STATUS_EP_UPGRADE)
		{
			return EpConstantErrorCode.EP_UPDATE;//
		}
		
		return 0;
	}
	/**
	 * >0:可以直接返回的错误
	 * 0：当前用户在充电其他用户正在充电
	 * -1：没有充电 
	 * @param chargingUsrId
	 * @return
	 */
	public int canWatchCharge(int chargingUsrId)
	{
		if(chargingUsrId<=0)
			return -1;
		if( status == EpConstant.EP_GUN_STATUS_CHARGE)
		{
			int curUsrId = getCurUserId();
			
			 if(curUsrId<=0 || curUsrId != chargingUsrId)
			 {
				 logger.error("canCharge innormal,status:{},curUsrId:{}",
						 new Object[]{status,curUsrId,chargingUsrId});
				 return EpConstantErrorCode.EPE_OTHER_CHARGING;
			 }
			 else
			 {
				 return 0;
			 }
		}
		else
		{
			return -1;
		}
				
		
	}
	
	public int canCharge(int startChargeStyle,int chargingUsrId,boolean init)
	{
		//先判断业务
		if( status == EpConstant.EP_GUN_STATUS_CHARGE||
		    status == EpConstant.EP_GUN_STATUS_BESPOKE_LOCKED||
		    status== EpConstant.EP_GUN_STATUS_USER_AUTH)
		{
			int curUsrId = getCurUserId();
			
			 if(curUsrId<=0)
			 {
				 logger.error("canCharge innormal,status:{},curUsrId:{}",status,curUsrId);
				 return 0;
			 }
			
			 
			 if(status == EpConstant.EP_GUN_STATUS_CHARGE)
			 {
				 if(curUsrId != chargingUsrId)
				 {
					 
					 return EpConstantErrorCode.EPE_OTHER_CHARGING;
				 }
				 else
				 {
					 if(!init)
						 return EpConstantErrorCode.EPE_REPEAT_CHARGE;
				 }
			 }
			 else if(status == EpConstant.EP_GUN_STATUS_BESPOKE_LOCKED)
			 {
				 if(curUsrId != chargingUsrId)
				 {
					 return EpConstantErrorCode.EPE_OTHER_BESP;
				 }
				 return 0;
			 }
			 else
				return EpConstantErrorCode.USED_GUN;
		}
		else if(status ==  EpConstant.EP_GUN_STATUS_SETTING)
		 {
			return EpConstantErrorCode.EPE_IN_EP_OPER;
		 }
		
		else if(status ==  EpConstant.EP_GUN_STATUS_EP_OPER && startChargeStyle == EpConstant.CHARGE_TYPE_QRCODE)
		{
			return EpConstantErrorCode.EPE_IN_EP_OPER;//
		}
		else if( status ==  EpConstant.EP_GUN_STATUS_WAIT_CHARGE)
		{
			return EpConstantErrorCode.EPE_IN_EP_OPER;
		   
		}
		else if(status ==  EpConstant.EP_GUN_STATUS_EP_UPGRADE)
		{
			return EpConstantErrorCode.EP_UPDATE;//
		}
		else if(status >30||
				 status == EpConstant.EP_GUN_STATUS_EP_INIT||
				 status== EpConstant.EP_GUN_STATUS_OFF_LINE||
			     status == EpConstant.EP_GUN_STATUS_STOP_USE)
		{
			return EpConstantErrorCode.EPE_GUN_FAULT;//
		}
		return 0;
		
	}
	public SingleInfo getSingleInfo(int addr)
    {
		SingleInfo singInfo = null;
		if (this.currentType == EpConstant.EP_AC_TYPE) {
			RealACChargeInfo acReals = (RealACChargeInfo)realChargeInfo;
			singInfo = acReals.getFieldValue(addr);

		} else {
			RealDCChargeInfo dcReals = (RealDCChargeInfo) realChargeInfo;
			singInfo = dcReals.getFieldValue(addr);
		}
		return singInfo;
	}
	
	public  Map<String ,Object> getRealData(ElectricPileCache epCache)
	{
		Map<String ,Object> realInfo=null;
		
		Map<String, measurePoint> mapTrd = real3rdFactory.getmeasurePoints();
		if(mapTrd==null || mapTrd.size()<=0)
			return null;

		realInfo = new HashMap<>();
		
		Iterator iterTrd = mapTrd.entrySet().iterator();
		while (iterTrd.hasNext()) {
			Map.Entry entry = (Map.Entry) iterTrd.next();
			String key = (String)entry.getKey();
			measurePoint thirdRealData = (measurePoint) entry.getValue();
			if (thirdRealData == null) {
				continue;
			}
			
			int addr=0;
			
			if(thirdRealData.getType()==1){
				addr=thirdRealData.getAddr();
			}
			else if(thirdRealData.getType()==2){
				addr=thirdRealData.getAddr()+ EpProtocolConstant.YX_2_START_POS;
			}
			else if(thirdRealData.getType()==3){
				addr=thirdRealData.getAddr()+ EpProtocolConstant.YC_START_POS;
			}
			else if(thirdRealData.getType()==4){
				addr=thirdRealData.getAddr()+ EpProtocolConstant.YC_VAR_START_POS;
			}
			
			SingleInfo singInfo = getSingleInfo(addr);
			if (singInfo == null) {
				continue;
			}
			realInfo.put(key, singInfo.getIntValue());
		}
	
		return realInfo;
		
	}
	public void handleSignleOrgNo(int orgNo,boolean workStatus)
	{
		logger.info("handleSignleOrgNo enter,orgNo:{},workStatus:{}",orgNo,workStatus);
		ElectricPileCache epCache = EpService.getEpByCode(epCode);
		if(epCache==null)
		{
			logger.info("handleSignleOrgNo did not find ElectricPileCache:{}",epCode);
			return ;
		}
		
		//int orgNo = epCache.getCompany_number();
		if(orgNo<=0)
		{
			logger.info("handleSignleOrgNo orgNo:{} is not valid",orgNo);
			return ;
		}
		
		
		Push rd =  real3rdFactory.getRealData(orgNo);
		if(rd==null)
		{
			logger.info("handleSignleOrgNo did not find RealData:{}",orgNo);
			return ;
		}
		
		if(rd.getMode()!=1 && rd.getMode()!=2)
		{
			logger.info("handleSignleOrgNo did not find OrgSendConfig:{}",rd.getMode());
			
			return ;
		}
		
		if(this.sendInfo3rd==null)
		{
			sendInfo3rd = new RealDataRT(epCache.getCompany_number(),0);
		}
		
		long now=  DateUtil.getCurrentSeconds();
		long diff = now - sendInfo3rd.getLastTime();
		if(workStatus==false &&diff<rd.getPeriod())
		{
			logger.info("handleSignleOrgNo now:{},sendInfo3rd.getLastTime():{},diff:{},osc.getPeriod():{}",
					new Object[]{now,sendInfo3rd.getLastTime(),diff,rd.getPeriod()});
			return;
		}
		
		
		
		String token="";
		String userIdentity="";
		boolean needSend=false;
		if(rd.getMode()==1)
		{
			if(this.chargeCache!=null)
			{
			//token = chargeCache.getToken();
				userIdentity = chargeCache.getThirdUsrIdentity();
				needSend=true;
			}
		}
		else
		{
			needSend=true;
		}
		if(needSend)
		{
			Map<String ,Object> realData= getRealData(epCache);
			if(realData==null)
			{
				logger.info("handleSignleOrgNo realData==null");
				
				return ;
			}
			
			rd.onRealData(token, orgNo,userIdentity,epCode, epGunNo,this.currentType,  realData,"");
			sendInfo3rd.setLastTime(now);
		}
	}
	
	public static void dispach3rdrealdata()
	{
			
	}
}
