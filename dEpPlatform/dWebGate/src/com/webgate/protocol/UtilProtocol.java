package com.webgate.protocol;

import java.math.BigDecimal;

import com.netCore.netty.buffer.DynamicByteBuffer;
import com.webgate.config.Global;
/**
 * 通用的的协议
 * @author haojian
 * Apr 26, 2013 11:59:03 AM
 */
public class UtilProtocol {
	
	/**
	 * 将字符串转换成字节数组
	 * @author haojian
	 * Apr 26, 2013 11:59:32 AM
	 * @param str
	 * @return
	 */
	public static byte[] stringToBytes(String str){
		DynamicByteBuffer byteBuffer = DynamicByteBuffer.allocate();
		byteBuffer.putString(str);
		return byteBuffer.getBytes();
		
	}
	
	/**
	 * 将byte转换成字节数组
	 * @author haojian
	 * Apr 26, 2013 11:59:37 AM
	 * @param b
	 * @return
	 */
	public static byte[] byteToBytes(byte b){
		DynamicByteBuffer byteBuffer = DynamicByteBuffer.allocate();
		byteBuffer.put(b);
		return byteBuffer.getBytes();
		
	}
	
	/**
	 * 将short转换成字节数组
	 * @author haojian
	 * Apr 26, 2013 11:59:54 AM
	 * @param s
	 * @return
	 */
	public static byte[] shortToBytes(short s){
		DynamicByteBuffer byteBuffer = DynamicByteBuffer.allocate();
		byteBuffer.putShort(s);
		return byteBuffer.getBytes();
		
	}
	
	/**
	 * 将int转换成字节数组
	 * @author haojian
	 * Apr 26, 2013 12:00:06 PM
	 * @param i
	 * @return
	 */
	public static byte[] intToBytes(int i){
		DynamicByteBuffer byteBuffer = DynamicByteBuffer.allocate();
		byteBuffer.putInt(i);
		return byteBuffer.getBytes();
		
	}
	/**
	 * 整数转换成0.1精度小数
	 * @param i
	 * @return
	 */
	
	public static BigDecimal intToBigDecimal1(int i)
	{
		BigDecimal value = new BigDecimal(i).multiply(Global.Dec1);
		value = value.setScale(1,BigDecimal.ROUND_HALF_UP);
		return value;
	}
	public static int BigDecimal1ToInt(BigDecimal value)
	{
		value = value.setScale(1,BigDecimal.ROUND_HALF_UP);
		return value.intValue();
	}
	/**
	 * 整数转换成0.01精度小数
	 * @param i
	 * @return
	 */
	
	public static BigDecimal intToBigDecimal2(int i)
	{
		BigDecimal value = new BigDecimal(i).multiply(Global.Dec2);
		value = value.setScale(2,BigDecimal.ROUND_HALF_UP);
		return value;
	}
	public static int BigDecimal2ToInt(BigDecimal value)
	{
		value = value.setScale(2,BigDecimal.ROUND_HALF_UP);
		return value.intValue();
	}
	/**
	 * 整数转换成0.001精度小数
	 * @param i
	 * @return
	 */
	
	public static BigDecimal intToBigDecimal3(int i)
	{
		BigDecimal value = new BigDecimal(i).multiply(Global.Dec3);
		value = value.setScale(3,BigDecimal.ROUND_HALF_UP);
		return value;
	}
	
	public static int BigDecimal3ToInt(BigDecimal value)
	{
		value = value.setScale(3,BigDecimal.ROUND_HALF_UP);
		return value.intValue();
	}

}
