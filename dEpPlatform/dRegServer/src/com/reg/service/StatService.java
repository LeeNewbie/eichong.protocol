package com.reg.service;

public class StatService {
	
	
	private static int commConcentratorNum=0;
	private static int commDiscreteEpNum=0;
	
	private static int bespokeCount=0;
	private static int chargeCount=0;
	
	public static void addCommConcentrator()
	{
		commConcentratorNum+=1;
	}
	public static void subCommConcentrator()
	{
		commConcentratorNum-=1;
	}
	public static int getCommConcentratorNum() {
		return commConcentratorNum;
	}
	public static void addCommDiscreteEp()
	{
		commDiscreteEpNum+=1;
	}
	public static void subCommDiscreteEp()
	{
		commDiscreteEpNum-=1;
	}
	public static int getCommDiscreteEpNum() {
		return commDiscreteEpNum;
	}
	public static void addBespoke()
	{
		bespokeCount+=1;
	}
	public static void subBespoke()
	{
		bespokeCount-=1;
	}
	public static int getBespokeCount() {
		return bespokeCount;
	}

	public static void addCharge()
	{
		chargeCount+=1;
	}
	public static void subCharge()
	{
		chargeCount-=1;
	}
	public static int getChargeCount() {
		return chargeCount;
	}
	
	public static String stat()
	{
		
		final StringBuilder sb = new StringBuilder();
		
		int count=StatService.getCommConcentratorNum();
    	sb.append("连接的集中器总数：").append(count).append(" \n");
    	
    	int epCount = StatService.getCommDiscreteEpNum();
    	sb.append("连接的充电桩总数：").append(epCount).append(" \n");
    	
    	int epBespCount= StatService.getBespokeCount();
    	sb.append("已经预约的充电桩（枪口）总数：").append(epBespCount).append(" \n");
    	
    	int epChargeCount = StatService.getChargeCount();
    	sb.append("正在充电的充电桩（枪口）总数：").append(epChargeCount).append(" \n");
    	
		return sb.toString();
	}
}
