package com.cooperate.TCEC_echong;

import com.cooperate.Push;
import com.cooperate.config.ConfigManager;
import com.cooperate.constant.KeyConsts;
import com.cooperate.utils.Strings;
import com.ec.constants.Symbol;
import com.ec.logs.LogConstants;
import com.ec.utils.LogUtil;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.util.HashMap;
import java.util.Map;

public class TCECEChongPush extends Push {
    private static final Logger logger = LoggerFactory.getLogger(LogUtil.getLogName(TCECEChongPush.class.getName()));
    public static String OPERATOR_ID = "";
    public static String OPERATOR_SECRET = "";
    public static String DATA_SECRET = "";
    public static String SIG_SECRET = "";
    public static String DATA_SECRET_IV = "";

    @Override
    public boolean init(String filename) {
        ConfigManager conf = ConfigManager.getMessageManager();
        boolean b = conf.init(filename);

        OPERATOR_ID = conf.getEChongProperties(KeyConsts.OPERATOR_ID);
        OPERATOR_SECRET = conf.getEChongProperties(KeyConsts.OPERATOR_SECRET);
        DATA_SECRET = conf.getEChongProperties(KeyConsts.DATA_SECRET);
        SIG_SECRET = conf.getEChongProperties(KeyConsts.SIG_SECRET);
        DATA_SECRET_IV = conf.getEChongProperties(KeyConsts.T_DATA_SECRET_IV);

        statusChangeUrl = conf.getEChongProperties(KeyConsts.STATUS_CHANGE_URL);
        tokenUrl = conf.getEChongProperties(KeyConsts.TOKEN_URL);

        orgNo = Integer.valueOf(conf.getEChongProperties(KeyConsts.ORG_NO, "9012"));
        mode = Integer.valueOf(conf.getEChongProperties(KeyConsts.REAL_DATA_MODE, "1"));
        period = Integer.valueOf(conf.getEChongProperties(KeyConsts.REAL_DATA_PERIOD, "30"));

        return b;
    }

    @Override
    public void onChargeEpResp(String token, int orgNo, String userIdentity, String epCode, int epGunNo, String extra, int ret, int errorCode) {

    }

    @Override
    public void onStopChargeEpResp(String token, int orgNo, String userIdentity, String epCode, int epGunNo, String extra, int ret, int errorCode) {

    }

    @Override
    public void onChargeEvent(int orgNo, String userIdentity, String epCode, int epGunNo, String extra, int ret, int errorCode) {
    }

    @Override
    public void onEpStatusChange(String token, int orgNo, String userIdentity, String epCode, int epGunNo
            , int inter_type, Map<String, Object> realData, String extra) {
        try {
            logger.info(LogUtil.addBaseExtLog("realData|extra"), new Object[]{LogConstants.FUNC_ONEPSTATUSCHANGE,
                    epCode, epGunNo, orgNo, userIdentity, token, realData, extra});

            HashMap<String, Object> resMap = new HashMap<String, Object>();
            resMap.put("ConnectorID", String.format("%s%02d", epCode, epGunNo));//pile_code 是 string 充电桩编码
            int status = Strings.getIntValue(realData, "3_1");
            //操作中不推送
            resMap.put("Status", status);
            if (status == 1 || status > 30) {
                resMap.put("Status", 255);
            } else if (status == 2) {
                resMap.put("Status", 1);
            } else if (status == 9 || status == 10 || status == 11 || status == 12) {
                resMap.put("Status", 2);
            } else if (status == 8) {
                resMap.put("Status", 4);
            }

            resMap.put("ParkStatus", 0);//parking_state 否 int 车位状态:0:未知 1:空闲 2:占用 3:故障
            resMap.put("LockStatus", 0);//地锁

            TCECEChongService.addRealData(resMap.get("pile_code") + Symbol.SPLIT + resMap.get("time")
                    + Symbol.SPLIT + KeyConsts.STATUS_CHANGE_URL, resMap);
        } catch (Exception e) {
            logger.error(LogUtil.addExtLog("exception"), e.getStackTrace());
        }
    }

    @Override
    public void onEpNetStatusChange(int orgNo, String epCode, int netStatus) {
    }

    @Override
    public void onRealData(String token, int orgNo, String userIdentity, String epCode, int epGunNo, int inter_type, float servicePrice, Map<String, Object> realData, String extra) {

    }

    @Override
    public void onChargeOrder(String token, int orgNo, String userIdentity, String epCode, int epGunNo,
                              int inter_type, float money, float elect_money, float service_money, float elect, float start_elect, float end_elect
            , float cusp_elect, float cusp_elect_price, float cusp_service_price, float cusp_money, float cusp_elect_money, float cusp_service_money
            , float peak_elect, float peak_elect_price, float peak_service_price, float peak_money, float peak_elect_money, float peak_service_money
            , float flat_elect, float flat_elect_price, float flat_service_price, float flat_money, float flat_elect_money, float flat_service_money
            , float valley_elect, float valley_elect_price, float valley_service_price, float valley_money, float valley_elect_money, float valley_service_money
            , int start_time, int end_time, int stop_model, int stop_reason, int soc, int time, String extra) {


    }

}
