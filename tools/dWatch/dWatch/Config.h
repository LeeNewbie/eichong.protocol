#pragma once

#include <list>
using namespace std;

typedef struct treeItemType
{
	int serverType;
	int ipType;
	int userType;
	int serverIndex;
	
}TREEITEMTYPE;

typedef struct userInfo
{
	CString user;
	CString key;
	
}USER;

typedef struct epGate
{
	CString name;
	CString out_ip;
	CString out_port;
	CString in_ip;
	CString in_port;
	USER    ctrlUser;
    USER    queryUser;
	CString gunNo;
	CString epCodes;
	CString accounts;

	CString curEpCode;
	CString curAccount;
	
}EPGATE;

typedef struct userGate
{
	CString name;
	CString out_ip;
	CString out_port;
	CString in_ip;
	CString in_port;
	USER    ctrlUser;
    USER    queryUser;
	CString gunNo;
	CString epCodes;
	CString accounts;
	
}USERGATE;

typedef struct regeditGate
{
	CString name;
	CString out_ip;
	CString out_port;
	CString in_ip;
	CString in_port;
	USER    ctrlUser;
    USER    queryUser;
	CString gunNo;
	CString epCodes;
	CString accounts;
	
}REGEDITGATE;

typedef struct	thirdGate
{
	CString name;
	CString out_ip;
	CString out_port;
	CString in_ip;
	CString in_port;
	USER    ctrlUser;
    USER    queryUser;
	CString gunNo;
	CString epCodes;
	CString accounts;
	
}THIRDGATE;


typedef list<CString> STRLIST;

void GetModuleFilePath(char* target);
int GetParaString(char* target, const char *title, const char *key);
int GetIniKeyString(char* target, const char *filename, const char *title, const char *key);
int GetKey(char* key,char * user);
int GetEpCode(char* code);
int GetEpGun(char* gunno);
int GetAccount(char* account);
int GetIniResult (char* result,const char * title,const char * name);

int GetServerNumPara(int& num);
int GetServerPara(char* ip, char* port,char * name,const char * title);

int GetParaStringA(CString target, CString title, CString key);
int SetParaStringA(CString target, CString title, CString key);
int GetVersionString(char* target, const char *filename, const char *key);

