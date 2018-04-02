package com.third.cache;

import java.math.BigDecimal;

import com.third.service.ThirdConfigService;
import com.third.utils.DateUtil;


public class SCOEp {
	private String epCode;
	private int currentType;
	
	private int gunNo=1;
	int workStatus;
	int gunConnStatus;
	int faultCode;
	BigDecimal vol_a=new BigDecimal(0);
	BigDecimal vol_b=new BigDecimal(0);
	BigDecimal vol_c=new BigDecimal(0);
	BigDecimal cur_a=new BigDecimal(0);
	BigDecimal cur_b=new BigDecimal(0);
	BigDecimal cur_c=new BigDecimal(0); 
	BigDecimal power=new BigDecimal(0);
	BigDecimal volt=new BigDecimal(0);
	BigDecimal cur=new BigDecimal(0);
	int soc=0;
	int restTime=0;
	
	BigDecimal initMeterNum =new BigDecimal(0);
	BigDecimal bdmeterNum =new BigDecimal(0);
	long lStartTime=0;
	long lEndTime=0;
	long sendScoLastTime=0;
	
	int ratePower=0;
	
	public SCOEp(String epCode,int currentType,long lastSendTime,long endChargeTime,int power,double initMeterNum)
	{
		this.epCode= epCode;
		this.currentType = currentType;
		this.initMeterNum = new BigDecimal(initMeterNum);
		ratePower = power;
		gotoIdle();
	}
	
	
	public void calcNextChargeTime()
	{
		int min = 5+(int)(Math.random()*480) ;
		int chargeMin = 10+(int)(Math.random()*40);
		long startSeconds = min * 60;
		this.lStartTime = DateUtil.getCurrentSeconds() + startSeconds ;
		
		this.lEndTime=lStartTime  +chargeMin * 60;
		
	}
	
	public void forceGotoCharge()
	{
		this.lStartTime = DateUtil.getCurrentSeconds() ;
		int chargeMin = (int)(Math.random()*50);
		this.lEndTime=lStartTime  +chargeMin * 60;
		
		gotoCharge();
	}
	
	public void gotoCharge()
	{
		workStatus=3;
		
		gunConnStatus=1;
		faultCode=0;
		vol_a= new BigDecimal(0.0);
		vol_b= new BigDecimal(0.0);
		vol_c= new BigDecimal(0.0);
		cur_a= new BigDecimal(0.0);
		cur_b= new BigDecimal(0.0);
		cur_c= new BigDecimal(0.0);
		
		volt= new BigDecimal(0.0);
		cur= new BigDecimal(0.0);
		power=   volt.multiply(cur);
		soc=0;
		restTime=0;
		
		bdmeterNum= new BigDecimal(0.0);
	}
	public BigDecimal calcStepMeterNum()
	{
		long now = DateUtil.getCurrentSeconds();
		if(now<this.lStartTime)
			return this.initMeterNum;
		long t_len = (int) (DateUtil.getCurrentSeconds() - this.lStartTime);
		
		double addMeterNum = 1.0* ratePower * t_len/3600;
		return initMeterNum.add(new BigDecimal(addMeterNum));
	}
	public BigDecimal calcDiff(BigDecimal b)
	{
		double d=1.0+(Math.random()-0.5)/100.0;
		return b.multiply(new BigDecimal(d));
	}
	public BigDecimal getRatedVol()
	{
		//public static int EP_AC_TYPE = 14;
		//public static int EP_DC_TYPE = 5;
		if(this.currentType==5)
			return new BigDecimal(370.0);
		else
			return new BigDecimal(220.0);
			
	}
	public BigDecimal getRatedCur()
	{
		if(this.currentType==5)
		{
			int moduleCount =  this.ratePower/15;
			return new BigDecimal(moduleCount * 27.0);
		}
		else
		{
			return new BigDecimal(31.0);
		}
		
	}
	public int calcSoc()
	{
		//充电，30~95
		int width= 65;
		long t_width = this.lEndTime-this.lStartTime;
		if(t_width<=0)
			return 0;
		long t_len = (int) (DateUtil.getCurrentSeconds() - this.lStartTime);
		double a=1.0 *t_len*width;
		a /= t_width;
		int soc =  (int) (30+ (t_len/t_width)*width);
		return soc;
	}
	public void gotoStepCharge()
	{
		workStatus=3;
		
		gunConnStatus=1;
		faultCode=0;
		
		BigDecimal rateVol = getRatedVol() ;
		BigDecimal rateCur = getRatedCur() ;
		vol_a=  calcDiff(rateVol) ;
		vol_b=  calcDiff(rateVol) ;
		vol_c=  calcDiff(rateVol) ;
		cur_a=  calcDiff(rateCur);
		cur_b=  calcDiff(rateCur);
		cur_c=  calcDiff(rateCur);
		
		volt = rateVol;
		cur=  rateCur;
		
		power=   volt.multiply(cur);
		soc= calcSoc();
		restTime=0;
		BigDecimal stepMeterNum = this.calcStepMeterNum();
		bdmeterNum = initMeterNum.add(stepMeterNum);
	}
	public void fixMeterNum()
	{
		BigDecimal stepMeterNum = this.calcStepMeterNum();
		initMeterNum = initMeterNum.add(stepMeterNum);
		initMeterNum = initMeterNum.setScale(3,BigDecimal.ROUND_HALF_UP);
		ThirdConfigService.writeMeter(epCode, this.currentType, initMeterNum.doubleValue());
	}
	
	public void gotoIdle()
	{
		workStatus=2;
		gunConnStatus=0;
		
		if(this.currentType==5)
		{
			BigDecimal rateVol = getRatedVol() ;
			vol_a=  calcDiff(rateVol) ;
			vol_b=  calcDiff(rateVol) ;
			vol_c=  calcDiff(rateVol) ;
			cur_a=  new BigDecimal(0.0);
			cur_b=  new BigDecimal(0.0);
			cur_c=  new BigDecimal(0.0); 
		}
		else
		{
			vol_a=  new BigDecimal(0.0);
			vol_b=  new BigDecimal(0.0);
			vol_c=  new BigDecimal(0.0);
			cur_a=  new BigDecimal(0.0);
			cur_b=  new BigDecimal(0.0);
			cur_c=  new BigDecimal(0.0); 
		}
		
		volt =  this.getRatedVol();
		cur=    new BigDecimal(0.0); 
		power=   volt.multiply(cur);
		
		
		calcNextChargeTime();
	}

	public String getEpCode() {
		return epCode;
	}

	public void setEpCode(String epCode) {
		this.epCode = epCode;
	}

	public int getCurrentType() {
		return currentType;
	}

	public void setCurrentType(int currentType) {
		this.currentType = currentType;
	}

	public int getGunNo() {
		return gunNo;
	}

	public void setGunNo(int gunNo) {
		this.gunNo = gunNo;
	}

	public int getWorkStatus() {
		return workStatus;
	}

	public void setWorkStatus(int workStatus) {
		this.workStatus = workStatus;
	}

	public int getGunConnStatus() {
		return gunConnStatus;
	}

	public void setGunConnStatus(int gunConnStatus) {
		this.gunConnStatus = gunConnStatus;
	}

	public int getFaultCode() {
		return faultCode;
	}

	public void setFaultCode(int faultCode) {
		this.faultCode = faultCode;
	}

	

	public BigDecimal getVol_a() {
		return vol_a;
	}

	public void setVol_a(BigDecimal vol_a) {
		this.vol_a = vol_a;
	}

	public BigDecimal getVol_b() {
		return vol_b;
	}

	public void setVol_b(BigDecimal vol_b) {
		this.vol_b = vol_b;
	}

	public BigDecimal getVol_c() {
		return vol_c;
	}

	public void setVol_c(BigDecimal vol_c) {
		this.vol_c = vol_c;
	}

	public BigDecimal getCur_a() {
		return cur_a;
	}

	public void setCur_a(BigDecimal cur_a) {
		this.cur_a = cur_a;
	}

	public BigDecimal getCur_b() {
		return cur_b;
	}

	public void setCur_b(BigDecimal cur_b) {
		this.cur_b = cur_b;
	}

	public BigDecimal getCur_c() {
		return cur_c;
	}

	public void setCur_c(BigDecimal cur_c) {
		this.cur_c = cur_c;
	}

	public BigDecimal getPower() {
		return power;
	}

	public void setPower(BigDecimal power) {
		this.power = power;
	}

	public BigDecimal getVolt() {
		return volt;
	}

	public void setVolt(BigDecimal volt) {
		this.volt = volt;
	}

	public BigDecimal getCur() {
		return cur;
	}

	public void setCur(BigDecimal cur) {
		this.cur = cur;
	}

	public int getSoc() {
		return soc;
	}

	public void setSoc(int soc) {
		this.soc = soc;
	}

	public int getRestTime() {
		return restTime;
	}

	public void setRestTime(int restTime) {
		this.restTime = restTime;
	}

	public BigDecimal getBdmeterNum() {
		return bdmeterNum;
	}

	public void setBdmeterNum(BigDecimal bdmeterNum) {
		this.bdmeterNum = bdmeterNum;
	}

	public long getlStartTime() {
		return lStartTime;
	}

	public void setlStartTime(long lStartTime) {
		this.lStartTime = lStartTime;
	}

	public long getlEndTime() {
		return lEndTime;
	}

	public void setlEndTime(long lEndTime) {
		this.lEndTime = lEndTime;
	}

	public long getSendScoLastTime() {
		return sendScoLastTime;
	}

	public void setSendScoLastTime(long sendScoLastTime) {
		this.sendScoLastTime = sendScoLastTime;
	}
}
