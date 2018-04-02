package com.epcentre.net.http;

import java.io.UnsupportedEncodingException;



public class HttpAccessErrorResult {
	
	/** 处理成功标识 */
	private int RESULT_ERROR = 0;
	/** 处理成功信息 */
	private static final String RESULT_ERROR_MSG = "处理失败";

	private StringBuffer sb;

	/**
	 * 构造函数
	 * 
	 * @param errorMsgKey
	 *            返回信息对象
	 */
	public HttpAccessErrorResult(int errCode,String errorMsgKey) {
		String errorMsg = "";
		errorMsg =errorMsgKey;
		handleResult(errCode,errorMsg);
	}

	/**
	 * 构造函数
	 * 
	 * @param obj
	 *            返回信息对象
	 */
	public HttpAccessErrorResult() {
		handleResult(1001,RESULT_ERROR_MSG);
	}

	/**
	 * 生成JSON数据（App端调用时使用）
	 * 
	 * @param obj
	 */
	private void handleResult(int errCode,String errorMsg) {
		sb = new StringBuffer();

		sb.append("{");
		sb.append("\"ret\": " + errCode + ",");
		sb.append("\"msg\": \"" + errorMsg + "\",");
		
		sb.append("}");

	}

	/**
	 * 返回JSON文字列
	 */
	public String toString() {
		return sb.toString();
	}

	/**
	 * 返回JSON文字列
	 */
	public byte[]  toBytes() {

		byte[] reBytes = new byte[0];
		try {
			reBytes =  sb.toString().getBytes("UTF-8");
		} catch (UnsupportedEncodingException e) {
			e.printStackTrace();
		}
		return reBytes;
	}


}
