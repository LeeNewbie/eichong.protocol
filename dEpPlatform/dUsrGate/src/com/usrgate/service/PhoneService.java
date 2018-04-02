package com.usrgate.service;

import io.netty.channel.Channel;

import java.util.Iterator;
import java.util.Map;
import java.util.concurrent.TimeUnit;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.netCore.core.pool.TaskPoolFactory;
import com.usrgate.cache.ElectricPileCache;
import com.usrgate.cache.EpGunCache;
import com.usrgate.cache.ProtocolVersion;
import com.usrgate.cache.UserRealInfo;
import com.usrgate.config.GameConfig;
import com.usrgate.constant.BaseConstant;
import com.usrgate.constant.CommStatusConstant;
import com.usrgate.constant.EpConstant;
import com.usrgate.constant.EpConstantErrorCode;
import com.usrgate.net.client.EpGateNetConnect;
import com.usrgate.net.client.PhoneClient;
import com.usrgate.net.sender.PhoneMessageSender;
import com.usrgate.protocol.PhoneConstant;
import com.usrgate.protocol.PhoneProtocol;
import com.usrgate.protocol.WmIce104Util;
import com.usrgate.task.CheckUsrNetTimeOutTask;
import com.usrgate.utils.DateUtil;

public class PhoneService {

	private static final Logger logger = LoggerFactory.getLogger(PhoneService.class + BaseConstant.SPLIT + GameConfig.serverName);

	public static String getDebugInfo() {
		final StringBuilder sb = new StringBuilder();
		sb.append("PhoneService:\n");

		sb.append("mapCh2PhoneClient count:")
				.append(CacheService.getCh2PhoneClientCount()).append("\n");
		sb.append("mapPhoneClients count:")
				.append(CacheService.getPhoneClientCount()).append("\n");

		return sb.toString();

	}
	
	private static void sendMessage(Channel channel, short cmdtype, int ret, int errorCode) {
		if (errorCode > 0) {
			logger.info("send cmdtype:{} fail errorCode:{}", cmdtype, errorCode);
		}

		byte[] data = PhoneProtocol.do_confirm(cmdtype, (byte) ret,
				(short) errorCode);
		PhoneMessageSender.sendMessage(channel, data);
	}
	
	private static void sendEPMessage(Channel channel, short cmdtype, int ret, int errorCode,int status, int currentType) {
		if (errorCode > 0) {
			logger.info("send epGate cmdtype:{} fail errorCode:{}", cmdtype, errorCode);
		}

		byte[] data = PhoneProtocol.do_connect_ep_resp(cmdtype, (byte) ret,
				(short) errorCode, (short) 0, (short) 0);
		PhoneMessageSender.sendMessage(channel, data);
	}

	/**
	 * 连接充电桩（phone->usrGate）
	 */
	public static void handleConnectEp(Channel ch, int accountId,
			String epCode, int epGunNo, String checkCode, int version, int cmd) {
		logger.info("handle connectEp epCode:{},epGunNo:{},accountId:{},checkCode:{},cmd:{}",new Object[]{epCode,epGunNo,accountId,checkCode,cmd});

		// 判断通道是否正常
		PhoneClient phoneClient = CacheService.getPhoneClientByChannel(ch);
		if (phoneClient == null) {
			logger.error("handle connectEp phoneClient == null");
			return;
		}
		phoneClient.setVersion(version);
		phoneClient.setCmd(cmd);
		
		int errorCode = initPhoneConnect(phoneClient, accountId,
				epCode, epGunNo, checkCode);
		if (errorCode > 0) {
			logger.info("handle connectEp epCode:{},epGunNo:{},accountId:{},errorCode:{}",new Object[]{epCode,epGunNo,accountId,errorCode});
			sendEPMessage(ch, (short)cmd, 0, errorCode, 0, 0);
		}
	}

	/**
	 * 充电（phone->usrGate）
	 */
	public static void handleStartCharge(PhoneClient phoneClient,
			int fronzeAmt, short chargeType) {
		logger.info("handle charge phoneClient:{},fronzeAmt:{},chargeType:{}",new Object[]{phoneClient,fronzeAmt,chargeType});

		int errorCode = EpChargeService.apiStartElectric(
				phoneClient.getEpCode(), phoneClient.getEpGunNo(),
				phoneClient.getAccount(), "", (short) EpConstant.CHARGE_TYPE_QRCODE, 0,
				fronzeAmt, 1, 1000, 2, phoneClient.getAccount());
		if (errorCode > 0) {
			logger.info("handle charge epCode:{},epGunNo:{},accountId:{},errorCode:{}",new Object[]{phoneClient.getEpCode(),phoneClient.getEpGunNo(),phoneClient.getAccount(),errorCode});
			sendMessage(phoneClient.getChannel(), PhoneConstant.D_START_CHARGE, 1, errorCode);
		}
	}

	/**
	 * 停止充电（phone->usrGate）
	 */
	public static void handleStopCharge(PhoneClient phoneClient) {
		logger.info("handle stopCharge phoneClient:{}",phoneClient);

		int errorCode = EpChargeService.apiStopElectric(
				phoneClient.getEpCode(), phoneClient.getEpGunNo(),
				phoneClient.getAccountId(), 99, phoneClient.getIdentity());
		if (errorCode > 0) {
			logger.info("handle stopCharge epCode:{},epGunNo:{},accountId:{},errorCode:{}",new Object[]{phoneClient.getEpCode(),phoneClient.getEpGunNo(),phoneClient.getAccount(),errorCode});
			sendMessage(phoneClient.getChannel(), PhoneConstant.D_STOP_CHARGE, 1, errorCode);
		}
	}

	/**
	 * 消费记录确认（phone->usrGate）
	 */
	public static void handleConsumeRecordConfirm(PhoneClient phoneClient,String chargeOrder,short result) {
		logger.info("handle stopCharge phoneClient:{}",phoneClient);

		logger.info("receive phone ConsumeRecordConfirm,chargeOrder:{},result:{}",chargeOrder,result);
		String messagekey = chargeOrder;
		CacheService.removePhoneRepeatMsg(messagekey);
	}

	/**
	 * 心跳（phone->usrGate）
	 */
	public static void handleHeart(PhoneClient phoneClient) {
		// java.util.concurrent.
		byte[] data = PhoneProtocol.Package((byte) 1, PhoneConstant.D_HEART);
		PhoneMessageSender.sendMessage(phoneClient.getChannel(), data);
	}

	private static int initPhoneConnect(PhoneClient phoneClient, int accountId,
			String epCode, int epGunNo, String checkCode) {

		// 查电桩
		ElectricPileCache epCache = EpService.getEpCache(epCode);

		int error = EpService.checkEpCache(epCache);
		if (error > 0) return error;

		EpGateNetConnect commClient = CacheService.getEpGate(epCache.getGateid());
		error = EpService.checkEpGate(commClient);
		if (error > 0) return error;

		EpGunCache epGunCache = CacheService.getEpGunCache(epCode, epGunNo);
		if (epGunCache == null) {
			return EpConstantErrorCode.EP_UNCONNECTED;
		}

		UserRealInfo userRealInfo = UserService.findUserRealInfo(accountId);
		if (null == userRealInfo) {
			return EpConstantErrorCode.INVALID_ACCOUNT;
		}
		if (userRealInfo.getStatus() != 1) {
			return EpConstantErrorCode.INVALID_ACCOUNT_STATUS;
		}
		String chargeGun = epCode + epGunNo;
		error = UserService.checkUserGun(accountId, chargeGun);
		if (error > 0) {
			return error;
		}

		PhoneClient oldPhoneClient = CacheService.getPhoneClientByAccountId(accountId);
		if (oldPhoneClient != null) {
			if (oldPhoneClient.getChannel() != phoneClient.getChannel()
					&& oldPhoneClient.getChannel() != null) {
				oldPhoneClient.getChannel().close();
				oldPhoneClient.setStatus(0);
			}
		}

		// 验证码
		String src = userRealInfo.getDeviceid() + userRealInfo.getPassword()
				+ accountId;
		String calcCheckCode = WmIce104Util.MD5Encode(src.getBytes());

		if (calcCheckCode.compareTo(checkCode) != 0) {
			return EpConstantErrorCode.ERROR_PHONE_CRC_CODE;
		}

		if (epGunCache.getStatus() == EpConstant.EP_GUN_STATUS_CHARGE) {
			if (epGunCache.getChargeCache() != null
					&& epGunCache.getChargeCache().getUserId() != 0
					&& epGunCache.getChargeCache().getUserId() != userRealInfo.getId()) {
				return EpConstantErrorCode.EPE_OTHER_CHARGING;
			}
		}
		if (epGunCache.getStatus() == EpConstant.EP_GUN_STATUS_BESPOKE_LOCKED) {
			if (epGunCache.getBespCache() == null
					|| epGunCache.getBespCache().getAccountId() != userRealInfo.getId()) {
				return EpConstantErrorCode.EPE_OTHER_BESP;
			}
		}
		if (epGunCache.getStatus() == EpConstant.EP_GUN_STATUS_USER_AUTH) {
			if (epGunCache.getChargeCache() != null
					&& epGunCache.getChargeCache().getUserId() != userRealInfo.getId()) {
				return EpConstantErrorCode.USED_GUN;
			}
		}

		phoneClient.setAccountId(accountId);
		phoneClient.setAccount(userRealInfo.getAccount());
		phoneClient.setEpCode(epCode);
		phoneClient.setEpGunNo(epGunNo);
		phoneClient.setIdentity(userRealInfo.getAccount());
		phoneClient.setStatus(CommStatusConstant.INIT_SUCCESS);

		CacheService.addPhoneClientByAccount(phoneClient);

		commClient.setLastSendTime(DateUtil.getCurrentSeconds());
		EpGateService.sendPhoneConnect(commClient.getChannel(),epCode,epGunNo,accountId);

		return 0;
	}

	/**
	 * 手机连接应答（usrGate->phone）
	 */
	public static void sendPhoneConnect(String epCode,int epGunNo,int accountId,int ret,int errorCode,int status) {
		logger.info("send phoneConnect epCode:{},epGunNo:{},accountId:{},ret:{},errorCode:{},status:{}",new Object[]{epCode,epGunNo,accountId,ret,errorCode,status});

		PhoneClient phoneClient = CacheService.getPhoneClientByAccountId(accountId);
		if (phoneClient == null) {
			logger.error("send phoneConnect phoneClient == null");
			return;
		}

		int currentType = 5;//TODO电桩表获取

		sendEPMessage(phoneClient.getChannel(), (short)phoneClient.getCmd(), ret, errorCode, status, currentType);
		if (ret == 1) {
			phoneClient.setConnectFlg(true);
			VersionService.addVersion(phoneClient.getVersion(), accountId);
		}
	}

	/**
	 * 充电应答（usrGate->phone）
	 */
	public static void sendCharge(String epCode,int epGunNo,int accountId,int ret,int errorCode) {
		logger.info("send phone charge epCode:{},epGunNo:{},accountId:{},ret:{},errorCode:{}",new Object[]{epCode,epGunNo,accountId,ret,errorCode});

		PhoneClient phoneClient = CacheService.getPhoneClientByAccountId(accountId);
		if (phoneClient == null) {
			logger.error("send phone charge phoneClient == null");
			return;
		}

		sendMessage(phoneClient.getChannel(), PhoneConstant.D_START_CHARGE, ret, errorCode);
	}

	/**
	 * 充电事件（usrGate->phone）
	 */
	public static void sendChargeEvent(String epCode,int epGunNo,int accountId,int status) {
		logger.info("send phone charge epCode:{},epGunNo:{},accountId:{},status:{}",new Object[]{epCode,epGunNo,accountId,status});

		PhoneClient phoneClient = CacheService.getPhoneClientByAccountId(accountId);
		if (phoneClient == null) {
			logger.error("send phone charge phoneClient == null");
			return;
		}

		byte[] data = PhoneProtocol.do_start_charge_event(status);

		PhoneMessageSender.sendMessage(phoneClient.getChannel(), data);
	}

	/**
	 * 停止充电应答（usrGate->phone）
	 */
	public static void sendStopCharge(String epCode,int epGunNo,int accountId,int ret,int errorCode) {
		logger.info("send phone stopCharge epCode:{},epGunNo:{},accountId:{},ret:{},errorCode:{}",new Object[]{epCode,epGunNo,accountId,ret,errorCode});

		PhoneClient phoneClient = CacheService.getPhoneClientByAccountId(accountId);
		if (phoneClient == null) {
			logger.error("send phone stopCharge phoneClient == null");
			return;
		}

		sendMessage(phoneClient.getChannel(), PhoneConstant.D_STOP_CHARGE, ret, errorCode);
	}

	/**
	 * 充电实时数据（usrGate->phone）
	 */
	public static void sendChargeReal(String epCode,int epGunNo,int accountId,Object extraData) {
		logger.info("send phone charge realData epCode:{},epGunNo:{},accountId:{},extraData:{}",new Object[]{epCode,epGunNo,accountId,extraData});

		PhoneClient phoneClient = CacheService.getPhoneClientByAccountId(accountId);
		if (phoneClient == null) {
			logger.error("send phone charge phoneClient == null");
			return;
		}

		ChargingInfo chargingInfo = (ChargingInfo) extraData;
		byte[] data = PhoneProtocol.do_real_charge_info(chargingInfo);
		PhoneMessageSender.sendMessage(phoneClient.getChannel(), data);
	}

	/**
	 * 消费记录（usrGate->phone）
	 */
	public static void sendConsumeRecord(String epCode,int epGunNo,int accountId,Object extraData) {
		logger.info("send phone consumeRecord epCode:{},epGunNo:{},accountId:{},extraData:{}",new Object[]{epCode,epGunNo,accountId,extraData});

		PhoneClient phoneClient = CacheService.getPhoneClientByAccountId(accountId);
		if (phoneClient == null) {
			logger.error("send phone consumeRecord phoneClient == null");
			return;
		}

		Map<String, Object> consumeRecordMap = (Map<String, Object>) extraData;
		String chargeOrder = (String) consumeRecordMap.get("orderid");
		long lst = (long) consumeRecordMap.get("st");
		long let = (long) consumeRecordMap.get("et");

		int st = (int) lst;
		int et = (int) let;

		int totalMeterNum = (int) consumeRecordMap.get("totalMeterNum");
		int totalAmt = (int) consumeRecordMap.get("totalAmt");
		int serviceAmt = (int) consumeRecordMap.get("serviceAmt");
		int pkEpId = (int) consumeRecordMap.get("pkEpId");

		int version = phoneClient.getVersion();
		int couPonAmt = 0;
		int userFirst = 0; 
		int realCouPonAmt = 0;
		if(version>=2)
		{
			/*userFirst = (int)consumeRecordMap.get("userFirst");
			couPonAmt = (int)consumeRecordMap.get("couPonAmt");
			realCouPonAmt = (int)consumeRecordMap.get("realCouPonAmt");*/
		}
		
		byte[] data = PhoneProtocol.do_consume_record((short)version,chargeOrder,st,et,totalMeterNum,totalAmt,serviceAmt,
				pkEpId,userFirst,couPonAmt,realCouPonAmt);

		PhoneMessageSender.sendRepeatMessage(phoneClient.getChannel(), data, chargeOrder, phoneClient.getVersion());
	}

	@SuppressWarnings("rawtypes")
	public static void offLine(Channel channel) {
		PhoneClient phoneClient = CacheService.getPhoneClientByChannel(channel);

		if (phoneClient != null) {
			logger.debug("\n\n\noffLine,phoneClient:{}", phoneClient);
			if (phoneClient.isConnectFlg()) {
				phoneClient.setConnectFlg(false);
				VersionService.offProtocol(phoneClient.getVersion());
			}
		}

		Iterator iter = CacheService.getMapPhoneClient().entrySet().iterator();
		while (iter.hasNext()) {
			Map.Entry entry = (Map.Entry) iter.next();
			PhoneClient pClient = (PhoneClient) entry.getValue();
			if (null == pClient) continue;
			if (pClient.getChannel().equals(channel)) {
				CacheService.removePhoneClientByAccountId((int)entry.getKey());
			}
		}
		CacheService.removePhoneClientByChannel(channel);
	}

	public static void startCommClientTimeout(long initDelay) {

		CheckUsrNetTimeOutTask checkTask = new CheckUsrNetTimeOutTask();

		TaskPoolFactory.scheduleAtFixedRate("CHECK_PHONECLIENT_TIMEOUT_TASK",
				checkTask, initDelay, 10, TimeUnit.SECONDS);
	}

	@SuppressWarnings("rawtypes")
	public synchronized static void checkTimeOut() {
		Iterator iter = CacheService.getMapCh2PhoneClient().entrySet().iterator();
		while (iter.hasNext()) {
			Map.Entry entry = (Map.Entry) iter.next();
			PhoneClient phoneClient = (PhoneClient) entry.getValue();
			if (null == phoneClient) {
				continue;
			}

			long now = DateUtil.getCurrentSeconds();
			long diff = now - phoneClient.getLastUseTime();

			if (diff > GameConfig.phoneConnectTimeout) {
				if (phoneClient.getStatus() > 0
						&& phoneClient.getChannel() != null) {
					EpGateService.sendPhoneOnline(phoneClient.getChannel(),phoneClient.getAccountId(),0);
					phoneClient.setStatus(0);
					logger.debug("checkTimeOut phone:{}",
							phoneClient.getIdentity());

					phoneClient.getChannel().close();
				}
			/*} else {
				if (phoneClient.getStatus() == CommStatusConstant.INIT_SUCCESS
						&& phoneClient.getChannel() != null) {
					EpGateService.sendPhoneOnline(phoneClient.getChannel(),phoneClient.getAccountId(),1);
				}*/
			}

		}

		iter = VersionService.getMapVersion().entrySet().iterator();
		while (iter.hasNext()) {
			Map.Entry entry = (Map.Entry) iter.next();
			ProtocolVersion protocolVersion = (ProtocolVersion) entry.getValue();
			if (null != protocolVersion) {
				logger.info("protocal version count:{}", protocolVersion.toString());
			}
		}
	}
}
