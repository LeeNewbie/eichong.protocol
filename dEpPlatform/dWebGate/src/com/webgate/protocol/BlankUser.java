package com.webgate.protocol;

import java.io.ByteArrayOutputStream;
import java.io.IOException;



public class BlankUser {
	private byte state;
	private String card_no;
	
	public BlankUser(String vCardNo,byte vState)
	{
		card_no = vCardNo;
		
		state=vState;
	}
	
	public synchronized byte toByteArray()[] {
		
		try {
			ByteArrayOutputStream bout = new ByteArrayOutputStream(256);
			
			bout.write(WmIce104Util.str2Bcd(card_no));
			
			bout.write(state);
			
			return bout.toByteArray();
		} catch (IOException e) {
			e.printStackTrace();
			return null;
		}
	}
	
	

}
