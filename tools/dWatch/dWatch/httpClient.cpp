#include "stdafx.h"
#include   "httpClient.h"  

bool BoxFlag = false;


  CHttpClient::CHttpClient()
 {
 }
 CHttpClient:: ~CHttpClient()
 {
 }


/*************************类实现********************************/  
    // 添加字段  
    void   CHttpClient::addParam(CString   name,   CString   value)    
    {    
        names.AddTail((LPCTSTR)name);    
        values.AddTail((LPCTSTR)value);    
        CString   eq="=";    
        CString   an="&";    
        CONTENT=CONTENT+name+eq+value+an;    
        CL=CONTENT.GetLength();    
    }     
    // 以http Get方式请求URL  
    CString   CHttpClient::doGet(CString   href)    
    {    
        CString   httpsource="";    
        CInternetSession   session(NULL,0);   

        CHttpFile*   pHTTPFile=NULL;    
        try{    
            pHTTPFile=(CHttpFile*)session.OpenURL(href,1,INTERNET_FLAG_TRANSFER_ASCII|INTERNET_FLAG_RELOAD,NULL,0);    
            //session1.    
        }  
        catch(CInternetException  *pEx)  
        {    
            pHTTPFile=NULL;   
			
			TCHAR szErr[1024]; 
            CString strInfo;
            pEx->GetErrorMessage(szErr, 1024);
             strInfo += szErr;
			 if(!BoxFlag)
			 {
			   BoxFlag = true;
			   AfxMessageBox(strInfo);
			   BoxFlag = false;
			 }
            pEx->Delete();
        }    
        if(pHTTPFile)    
        {    
            CString   text;    
            for(int   i=0;pHTTPFile->ReadString(text);i++)    
            {    
			
                httpsource=httpsource+text+"\r\n";    
            }    
            pHTTPFile->Close();    
            delete   pHTTPFile;    
        }else    
        {    
              
        }    

        CHttpConnection* pHttpConnection = session.GetHttpConnection("10.9.0.72",0,7777,0,0);
		pHttpConnection->Close();
		session.Close();
		char content[4096];

		UTF82GBK((LPSTR)(LPCTSTR)httpsource,content,4096); 
		CString content1 = content;
        return   content1;    
    }    
    //// 以Http Post方式请求URL  
    //CString   CHttpClient::doPost(CString   href)    
    //{    
    //    CString   httpsource="";    
    //    CInternetSession   session1;    
    //    CHttpConnection*   conn1=NULL;    
    //    CHttpFile*   pFile   =   NULL;        
    //    CString   strServerName;    
    //    CString   strObject;    
    //    INTERNET_PORT   nPort;    
    //    DWORD   dwServiceType;    
    //    AfxParseURL((LPCTSTR)href, dwServiceType, strServerName, strObject, nPort);    
    //    DWORD   retcode;                
    //    char*   outBuff   =   CONTENT.GetBuffer(1000);    
    //    try        
    //    {                    
    //        conn1   =   session1.GetHttpConnection(strServerName, nPort);            
    //        pFile   =   conn1->OpenRequest(0,strObject,NULL,1,NULL,"HTTP/1.1",INTERNET_FLAG_EXISTING_CONNECT|INTERNET_FLAG_NO_AUTO_REDIRECT);            
    //        pFile   ->   AddRequestHeaders("Content-Type:   application/x-www-form-urlencoded");            
    //        pFile   ->   AddRequestHeaders("Accept:   */*");                            
    //        pFile   ->   SendRequest(NULL,0,outBuff,strlen(outBuff)+1);                    
    //        pFile   ->   QueryInfoStatusCode(retcode);                    
    //    }                              
    //    catch   (CInternetException   *   e){};                    
    //    if(pFile)    
    //    {    
    //        CString   text;    
    //        for(int   i=0;pFile->ReadString(text);i++)    
    //        {    
    //            httpsource=httpsource+text+"\r\n";    
    //        }    
    //        pFile->Close();    
    //    }else    
    //    {    
    //        // do anything.....  
    //    }    
    //    return   httpsource;    
    //    delete   pFile;            
    //    delete   conn1;            
    //    session1.Close();    
    //}    
      
    // 使用范例  
    //int main ()  
    //{  
    //  //get方式    
    //  CHttpClient   conn;    
    //  CString   value="http://xxx.com/test.asp";    
    //  value=conn.doGet(value);    
    //  
    //  //post方式    
    //  CHttpClient   conn;    
    //  value="http://xxx.com/test.asp";    
    //  conn.addParam("name1","value1");    
    //  conn.addParam("name2","value2");    
    //  conn.addParam("name3","value3");    
    //  value=conn.doPost(value);     
    //} 

//UTF-8 GBK  
int CHttpClient::UTF82GBK(char *szUtf8,char *szGbk,int Len)  
{  
    int n = MultiByteToWideChar(CP_UTF8, 0, szUtf8, -1, NULL, 0);  
    WCHAR * wszGBK = new WCHAR[sizeof(WCHAR) * n];  
    memset(wszGBK, 0, sizeof(WCHAR) * n);  
    MultiByteToWideChar(CP_UTF8, 0,szUtf8,-1, wszGBK, n);  
   
    n = WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, NULL, 0, NULL, NULL);  
    if (n > Len)  
    {  
        delete[]wszGBK;  
        return -1;  
    }  
   
    WideCharToMultiByte(CP_ACP,0, wszGBK, -1, szGbk, n, NULL, NULL);  
   
    delete[]wszGBK;  
    wszGBK = NULL;  
   
    return 0;  
}