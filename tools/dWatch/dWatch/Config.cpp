#include "StdAfx.h"
#include "Config.h"

#define MAX_LENGTH 256

//��INI�ļ���ȡ�ַ�����������  
// target: ���ݷ���
// filename: ini�ļ���
// title: ������
// key:��������
int GetIniKeyString(char* target, const char *filename, const char *title, const char *key)
{   
    FILE *fp;   
    char szLine[1024];  
    static char tmpstr[1024];
    int filechar;
    int i = 0;
    int flag = 0;   // 0-���࣬1-С��
    char *tmp;  
  
    if((fp = fopen(filename, "r")) == NULL)
    {   
        //("open config file error\n");
        return -1;   
    }

    while(!feof(fp))   
    {   
        filechar = fgetc(fp);
        if(filechar == EOF)
        {   
            break;
        }   
        else   
        {
			if(i==0 && (filechar==' '||filechar=='\t'))
			{
				continue;	// ������ͷ�Ŀո�
			}
			else
				szLine[i++] = filechar;

        }

        if(filechar == '\n' || filechar == '\r')	// ����
        {   
            szLine[i-1] = '\0';
            i = 0;

            tmp = strchr(szLine, '#');	// ��'#'ע��
			if (tmp && strcmp("pw", key) != 0)
				*tmp = '\0';	// ȥ��ע��
            tmp = strstr(szLine, "//");	// ��'//'ע��
			if (tmp)
				*tmp = '\0';	// ȥ��ע��

            tmp = strchr(szLine, '=');	// �ҵȺ�
  
            if(( tmp != NULL )&&(flag == 1))	// ��С��
            {
				*tmp = '\0';

				int i;
				for(i=strlen(szLine); i>0; i--)
				{
					if (szLine[i-1]==' ' || szLine[i-1]=='\t')
						continue;
					break;
				}
				szLine[i] = '\0';	// ȥ��β���ո�

                if(strcmp(szLine, key) == 0)
                {
                    //�ҵ�key��Ӧ����
					tmp ++;
					while(*tmp==' ' || *tmp=='\t')
						tmp++;	// ������ͷ�Ŀո�
					for(i=strlen(tmp); i>0; i--)
					{
						if (tmp[i-1]==' ' || tmp[i-1]=='\t')
							continue;
						break;
					}
					tmp[i] = '\0';	// ȥ��β���ո�

					strcpy(target, tmp);
                    
					fclose(fp);
                    return 0;
                }   
            }  
            else   
            {
				if (szLine[0] != '[')
					continue;

				tmp = strchr(szLine, ']');	// ��']'
				if (tmp)
					*tmp = '\0';
				else
					continue;

				tmp = szLine + 1;
				while(*tmp==' ' || *tmp=='\t')
					tmp++;	// ������ͷ�Ŀո�
				int i;
				for(i=strlen(tmp); i>0; i--)
				{
					if (tmp[i-1]==' ' || tmp[i-1]=='\t')
						continue;
					break;
				}
				tmp[i] = '\0';	// ȥ��β���ո�

                if( strcmp(tmp, title) == 0 )
                {  
                    //�ҵ�title  
                    flag = 1;
                }
            }
        }
    }

    fclose(fp);   
    return -1;   
}

void GetModuleFilePath(char* target)
{
	char path[256];

#ifdef WIN32
	GetModuleFileName(NULL, path, 256);
    char* p = strrchr(path, '\\');
	if (p)
		*p = 0;
	strcat(path, "\\");

#else	// linux
	//strcpy(path, "/var/log/");

/*
	char pidfile[100];
    int bytes;  
    int fd;

    sprintf(pidfile, "/proc/%d/cmdline", getpid());  

    fd = open(pidfile, O_RDONLY, 0);
    bytes = read(fd, path, 256);
    close(fd);
    pidfile[256] = '\0';

	strcat(path, "/");

	PRINTF("%s\n", path);
*/

	//strcpy(path, "/home/fengbin/");
	strcpy(path, "/etc/");

#endif	//
	strcpy(target, path);
}

int GetParaString(char* target, const char *title, const char *key)
{
	char filename[256];
	char path[256];

#ifdef WIN32
	GetModuleFileName(NULL, path, 256);
    char* p = strrchr(path, '\\');
	if (p)
		*p = 0;
	strcat(path, "\\");

#else	// linux
	//strcpy(path, "/var/log/");

/*
	char pidfile[100];
    int bytes;  
    int fd;

    sprintf(pidfile, "/proc/%d/cmdline", getpid());  

    fd = open(pidfile, O_RDONLY, 0);
    bytes = read(fd, path, 256);
    close(fd);
    pidfile[256] = '\0';

	strcat(path, "/");

	PRINTF("%s\n", path);
*/

	//strcpy(path, "/home/fengbin/");
	strcpy(path, "/etc/");

#endif	//

	sprintf(filename, "%sdwatch.ini", path);

	

	return GetIniKeyString(target, filename, title, key);

}

/////////////////////////////////////////////////////////////////////////////
// ��ȡkey
int GetKey(char* key,char * user)
{

	char buff[100];

	int retval = GetParaString(buff, "USER", "user");
	if (retval == 0)
	{
		strcpy(user, buff);
	}
	else
	{
		return -1;
	}
	 retval = GetParaString(buff, "USER", "key");
	if (retval == 0)
	{
		strcpy(key, buff);
	}
	else
	{
		return -1;
	}

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// ��ȡǹ�ڱ��
int GetEpGun(char* gunno)	
{
	char buff[100];

	int retval = GetParaString(buff, "OTHER", "GunNo");
	if (retval == 0)
	{
		strcpy(gunno, buff);
	}
	else
	{
		return -1;
	}

	return 0;
}


/////////////////////////////////////////////////////////////////////////////

int GetIniResult (char* result,const char * title,const char * name)	
{
	
	char buff[1024];

	int retval = GetParaString(buff, title, name);
	if (retval == 0)
	{
		strcpy(result, buff);
	}
	else
	{
		return -1;
	}

	return 0;
}


/////////////////////////////////////////////////////////////////////////////
// ��ȡ��׮���
int GetEpCode (char* code)	
{
	
	char buff[1024];

	int retval = GetParaString(buff, "OTHER", "EpCodes");
	if (retval == 0)
	{
		strcpy(code, buff);
	}
	else
	{
		return -1;
	}

	return 0;
}

//��ȡ�û��˺�
int GetAccount(char* account)
{

	char buff[100];

	int retval = GetParaString(buff, "OTHER", "Account");
	if (retval == 0)
	{
		strcpy(account, buff);
	}
	else
	{
		return -1;
	}

	return 0;
}

//��ȡserverNum
int GetServerNumPara(int& num)
{
	unsigned int val;

	char buff[100];
	
	

	int retval = GetParaString(buff, "SERVER", "serverNum");
	if (retval == 0)
	{
		num =atoi(buff);
	}
	else
	{
		return -1;
	}


	return 0;
}


//��ȡEpgateIP�Ͷ˿�
int GetServerPara(char* ip, char* port,char * name,const char * title)
{
	unsigned int val;

	char buff[100];
	
	

	int retval = GetParaString(buff, title, "ip");
	if (retval == 0)
	{
		strcpy(ip, buff);
	}
	else
	{
		return -1;
	}

	retval = GetParaString(buff, title, "port");
	if (retval == 0)
	{
		strcpy(port, buff);
		
	}
	else
	{
		return -1;
	}
	retval = GetParaString(buff, title, "name");
	if (retval == 0)
	{
		strcpy(name, buff);
		
	}
	else
	{
		return -1;
	}

	return 0;
}


int GetParaStringA(CString target, CString title, CString key)
{
	char filename[256];
	char path[256];

#ifdef WIN32
	GetModuleFileName(NULL, path, 256);
    char* p = strrchr(path, '\\');
	if (p)
		*p = 0;
	strcat(path, "\\");

#else	// linux
	//strcpy(path, "/var/log/");

/*
	char pidfile[100];
    int bytes;  
    int fd;

    sprintf(pidfile, "/proc/%d/cmdline", getpid());  

    fd = open(pidfile, O_RDONLY, 0);
    bytes = read(fd, path, 256);
    close(fd);
    pidfile[256] = '\0';

	strcat(path, "/");

	PRINTF("%s\n", path);
*/

	//strcpy(path, "/home/fengbin/");
	strcpy(path, "/etc/");

#endif	//

	sprintf(filename, "%sepgatemaint.ini", path);

	CString file = filename;
	
	return GetPrivateProfileString(title, key, "",target.GetBuffer(MAX_LENGTH),MAX_LENGTH,file);

}

int SetParaStringA(CString target, CString title, CString key)
{
	char filename[256];
	char path[256];

#ifdef WIN32
	GetModuleFileName(NULL, path, 256);
    char* p = strrchr(path, '\\');
	if (p)
		*p = 0;
	strcat(path, "\\");

#else	// linux
	//strcpy(path, "/var/log/");

/*
	char pidfile[100];
    int bytes;  
    int fd;

    sprintf(pidfile, "/proc/%d/cmdline", getpid());  

    fd = open(pidfile, O_RDONLY, 0);
    bytes = read(fd, path, 256);
    close(fd);
    pidfile[256] = '\0';

	strcat(path, "/");

	PRINTF("%s\n", path);
*/

	//strcpy(path, "/home/fengbin/");
	strcpy(path, "/etc/");

#endif	//

	sprintf(filename, "%sdwatch.ini", path);
    
	CString file = filename;
	
	return WritePrivateProfileString(title, key, target, file);
}

int GetVersionString(char* target, const char *filename, const char *key)
{   
    FILE *fp;   
    char szLine[1024];  
    static char tmpstr[1024];
    int filechar;
    int i = 0;
    char *tmp;  
  
    if((fp = fopen(filename, "r")) == NULL)
    {   
        //("open config file error\n");
        return -1;   
    }

    while(!feof(fp))   
    {   
        filechar = fgetc(fp);
        if(filechar == EOF)
        {   
            break;
        }   
        else   
        {
			if(i==0 && (filechar==' '||filechar=='\t'))
			{
				continue;	// ������ͷ�Ŀո�
			}
			else
				szLine[i++] = filechar;

        }

        if(filechar == '\n' || filechar == '\r')	// ����
        {   
            szLine[i-1] = '\0';
            i = 0;

            tmp = strchr(szLine, '#');	// ��'#'ע��
			if (tmp)
				*tmp = '\0';	// ȥ��ע��
            tmp = strstr(szLine, "//");	// ��'//'ע��
			if (tmp)
				*tmp = '\0';	// ȥ��ע��

			tmp = strchr(szLine,':');
  
            if( tmp != NULL )
            {
				*tmp = '\0';

				int i;
				for(i=strlen(szLine); i>0; i--)
				{
					if (szLine[i-1]==' ' || szLine[i-1]=='\t')
						continue;
					break;
				}
				szLine[i] = '\0';	// ȥ��β���ո�

                if(strcmp(szLine, key) == 0)
                {
                    //�ҵ�key��Ӧ����
					tmp ++;
					while(*tmp==' ' || *tmp=='\t')
						tmp++;	// ������ͷ�Ŀո�
					for(i=strlen(tmp); i>0; i--)
					{
						if (tmp[i-1]==' ' || tmp[i-1]=='\t')
							continue;
						break;
					}
					tmp[i] = '\0';	// ȥ��β���ո�

					strcpy(target, tmp);
                    
					fclose(fp);
                    return 0;
                } 
            }
        }
    }

    fclose(fp);   
    return -1;   
}

