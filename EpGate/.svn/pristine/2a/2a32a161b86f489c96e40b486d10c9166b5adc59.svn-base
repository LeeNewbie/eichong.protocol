package com.epcentre.utils;

import java.util.Random;

public class StringUtil {
	
	public static String repeat(String src, int num) {
        StringBuffer s = new StringBuffer();
        for (int i = 0; i < num; i++)
            s.append(src);
        return s.toString();
    }
	
	public static String divF(byte[] bytes) {
        StringBuffer s = new StringBuffer();
        for (int i = 0; i < bytes.length; i++)
        {
        	if( (bytes[i]&0x0FF)==255)
			{
				break;
			}
        	
            s.append(bytes[i]);
        }
        return s.toString();
    }
	
	
	 public static String getRandomString(int length)
	 {
		 String str="ABCDEFGHIGKLMNOPQRSTUVWXYZ0123456789";
		 Random random=new Random();
		 StringBuffer sf=new StringBuffer();
		 for(int i=0;i<length;i++)
		 {
			 int number=random.nextInt(36);//0~61
			 sf.append(str.charAt(number));
		 }
		 return sf.toString();
	 }
	 public static String getByteString(byte[] src)
	 {
		 String s= new String(src);
		 return s;
		 
	 }
	 
	 public static String getCString(byte[] src)
	 {
		 int pos=-1;
		 int len = src.length;
		 for(int i=0;i<len;i++)
		 {
			 if(src[i]==0)
			 {
				 pos =i+1;
				 break;
			 }
		 }
		 if(pos == -1)
			 return "";
		 
		 byte[] dest= new byte[pos-1];
		 System.arraycopy(src, 0, dest, 0, pos-1);
		 String s= new String(dest);
		 return s;
	 }
	 
	 public static String getAscii(byte[] src)
	 {
		 int pos=-1;
		 int len = src.length;
		 for(int i=0;i<len;i++)
		 {
			 if(src[i]==0)
			 {
				 pos =i+1;
				 break;
			 }
			 if(src[i]<0x20 || src[i]>0x7e)
			 {
				 return " ";
			 }
		 }
		 if(pos == -1)
			 return "";
		 
		 byte[] dest= new byte[pos-1];
		 System.arraycopy(src, 0, dest, 0, pos-1);
		 String s= new String(dest);
		 s.trim();
		 return s;
	 }



}
