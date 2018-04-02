package com.reg.epconsumer;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.MalformedURLException;
import java.net.URL;
//import java.security.GeneralSecurityException;  
//import java.security.KeyStore;  
import java.security.cert.CertificateException;
import java.security.cert.X509Certificate;

import javax.net.ssl.HostnameVerifier;
import javax.net.ssl.HttpsURLConnection;
import javax.net.ssl.SSLContext;
import javax.net.ssl.SSLSession;
import javax.net.ssl.TrustManager;
import javax.net.ssl.X509TrustManager;

public class HttpsPost {
	
	private static class TrustAnyTrustManager implements X509TrustManager {  
	      
        public void checkClientTrusted(X509Certificate[] chain, String authType) throws CertificateException {  
        }  
      
        public void checkServerTrusted(X509Certificate[] chain, String authType) throws CertificateException {  
        }  
      
        public X509Certificate[] getAcceptedIssuers() {  
            return new X509Certificate[]{};  
        }  
    }  
      
    private static class TrustAnyHostnameVerifier implements HostnameVerifier {  
        public boolean verify(String hostname, SSLSession session) {  
            return true;  
        }  
    }  

    /** 
     * ·¢ËÍÇëÇó. 
     * @param httpsUrl 
     *            ÇëÇóµÄµØÖ· 
     * @param xmlStr 
     *            ÇëÇóµÄÊý¾Ý 
     */  
    public static String GetAuthToken(String httpsUrl, String formStr) {  
        HttpsURLConnection urlCon = null;  
        try {  
        	SSLContext sc = SSLContext.getInstance("SSL");  
        	sc.init(null, new TrustManager[]{new TrustAnyTrustManager()}, new java.security.SecureRandom());  
            
            urlCon = (HttpsURLConnection) (new URL(httpsUrl)).openConnection();
            
            urlCon.setSSLSocketFactory(sc.getSocketFactory());  
            urlCon.setHostnameVerifier(new TrustAnyHostnameVerifier());  
           
            urlCon.setDoInput(true);  
            urlCon.setDoOutput(true);  
            urlCon.setRequestMethod("POST");  
            urlCon.setRequestProperty("Authorization", "Basic c1MzMTNMSzU1WFA0OUlFZU9wYWJ5d2RsTTdnYTpJXzE5VUVOYlhZWDRUZU5pVmVBWkp3NzhrbkFh");  
            urlCon.setRequestProperty("Content-Type", "application/x-www-form-urlencoded");  
            urlCon.setRequestProperty("Content-Length",  
                    String.valueOf(formStr.getBytes().length));  
            urlCon.setUseCaches(false);
            
            urlCon.getOutputStream().write(formStr.getBytes("gbk"));  
            urlCon.getOutputStream().flush();  
            urlCon.getOutputStream().close();  
            BufferedReader in = new BufferedReader(new InputStreamReader(  
                    urlCon.getInputStream()));  
            String line;  
            String content="";
            while ((line = in.readLine()) != null) {
               // System.out.println(line);
                content+= line;
            } 
            return content;
        } catch (MalformedURLException e) {  
            e.printStackTrace();  
            return "";
        } catch (IOException e) {  
            e.printStackTrace();  
            return "";
        } catch (Exception e) {  
            e.printStackTrace();
            return "";
        }
        
    }  

    @SuppressWarnings("finally")
	public static String PostPileInfo(String httpsUrl, String formStr, String token) { 
    	String content="";
        HttpsURLConnection urlCon = null;  
        try {  
        	SSLContext sc = SSLContext.getInstance("SSL");  
        	sc.init(null, new TrustManager[]{new TrustAnyTrustManager()}, new java.security.SecureRandom());  
            
            urlCon = (HttpsURLConnection) (new URL(httpsUrl)).openConnection();
            
            urlCon.setSSLSocketFactory(sc.getSocketFactory());  
            urlCon.setHostnameVerifier(new TrustAnyHostnameVerifier());  

            urlCon.setDoInput(true);  
            urlCon.setDoOutput(true);  
            urlCon.setRequestMethod("POST");  
            String auth = "Bearer "+token;
            urlCon.setRequestProperty("Authorization", auth);  
            urlCon.setRequestProperty("Content-Type", "no-cache");  
            urlCon.setRequestProperty("Cache-Control", "application/json");  
            urlCon.setRequestProperty("Content-Length",  String.valueOf(formStr.getBytes().length));  
            urlCon.setUseCaches(false);
            
            urlCon.getOutputStream().write(formStr.getBytes("gbk"));  
            urlCon.getOutputStream().flush();  
            urlCon.getOutputStream().close();  
            BufferedReader in = new BufferedReader(new InputStreamReader(  
                    urlCon.getInputStream()));  
            String line; 
            content="";
            while ((line = in.readLine()) != null) {
                content += line;
            }  
            return content;
        } catch (MalformedURLException e) {  
            e.printStackTrace(); 
        } catch (IOException e) {  
            e.printStackTrace();
        } catch (Exception e) {  
            e.printStackTrace(); 
        } 
        finally{
        	return content;
        }
    }  

}
