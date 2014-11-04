/********************************************************************
	filename	: 	ToolsHelp.cpp
	author		:	s00191067
	created		:	2011/10/29	
	description	:	�ṩ����ģ�鹫�������Ĺ����ļ�
	copyright	:	Copyright (C) 2011-2015
	history		:	2011/10/29 ��ʼ�汾
*********************************************************************/
/*!< Ԥ����ͷ�ļ� */
#include "stdafx.h"
#include <atlenc.h>     // ������������ʧ�ܣ��Ҳ���PathRemoveFileSpec
#include <shlwapi.h>    // ��ȡ����·��ʹ��:PathRemoveFileSpec

/*!< ����ͷ�ļ� */
#include "ToolsHelp.h"

/*!< �궨�� */
#define  STR_LENGTH_SIZE	    20
#define	 INT_FORMAT_D		    10
#define  INT_FORMAT_X		    16
#define  STR_FORMAT_D		    "%d"
#define  STR_FORMAT_X		    "%x"
#define SDK_CONFIG_INI_NAME     ("SDKConfig.ini")
#define SDK_LOG_LEVEL           ("LogLevel")
#define ZERO_ASCII_VALUE        48          //0��ASCII��ֵ
#define TEN                     10          //������10

//��ȡ��������·��
string CToolsHelp::GetProgramDir()
{
    // ��ȡ����װĿ¼ strPath
    CHAR szPathName[MAX_PATH];
    (void)::GetModuleFileName(NULL , szPathName , MAX_PATH);
    (void)::PathRemoveFileSpec(szPathName);
    string strPath = szPathName;
    strPath += "\\";

    return strPath;
}

//������ת��Ϊ�ַ���
string CToolsHelp::Int2Str ( int iValue, bool bHex /*= false*/  )
{
    char szTmp[STR_LENGTH_SIZE];
    if ( bHex )
    {
        (void)_itoa_s( iValue, szTmp, INT_FORMAT_X );
    }
    else
    {
        (void)_itoa_s( iValue, szTmp, INT_FORMAT_D );
    }	

    return string( szTmp );
}

//������ת��Ϊ�ַ���
int CToolsHelp::Str2Int( const string& strSrc, bool bHex )
{
    int iRet = 0;	
    if ( bHex )
    {
        (void)sscanf_s ( strSrc.c_str(), STR_FORMAT_X, &iRet );
    }
    else
    {
        (void)sscanf_s ( strSrc.c_str(), STR_FORMAT_D, &iRet );
    }	

    return iRet;
}

//�ַ�����ת������ANSI��UNICODE
const WCHAR* CToolsHelp::ANSI2Unicode(const char*src)
{
    static WCHAR tszBuffer[10240];
    memset(tszBuffer,0,10240*sizeof(WCHAR));
    if (NULL == src)
    {
        (void)MultiByteToWideChar(CP_ACP,0, "",
            (int)(strlen("")+1),
            tszBuffer,sizeof(tszBuffer)/sizeof(tszBuffer[0]));
    }
    else
    {
        (void)MultiByteToWideChar(CP_ACP,0, src,
            (int)(strlen(src)+1),
            tszBuffer,sizeof(tszBuffer)/sizeof(tszBuffer[0]));
    }
    return tszBuffer;
}

//�ַ�����ת������UNICODE��ANSI
const char* CToolsHelp:: Unicode2ANSI(const WCHAR *src)
{
    static char szBuffer[20480];
	//modify by limingjie lWX175483 :2013-5-24   memset����д����
    memset(szBuffer,0,20480);
    if (NULL == src)
    {
        (void)WideCharToMultiByte(CP_ACP,0,L"",-1,szBuffer,20480,NULL,NULL);
    }
    else
    {
        (void)WideCharToMultiByte(CP_ACP,0,src,-1,szBuffer,20480,NULL,NULL);
    }
    return szBuffer;
}


//ȡ�����ļ��е���־����
IvsLogLevel CToolsHelp::GetLogLevel()
{
    char szBuf[MAX_PATH] = {'\0'}; 
    
	//�ļ�·��   
    string strProgrameDir = GetProgramDir();
    string strFileName = strProgrameDir + SDK_CONFIG_INI_NAME;
    string strDefault = Int2Str(LOG_ERROR);

    (void)::GetPrivateProfileString(SDK_LOG_LEVEL,SDK_LOG_LEVEL,strDefault.c_str(),szBuf,MAX_PATH,strFileName.c_str());
    
    string strLogLevel = szBuf;
    IvsLogLevel LogLevel = (IvsLogLevel)Str2Int(strLogLevel);

    return LogLevel;
}

/**************************************************************************
* name       : 
* description: 
* input      : 
* output     : 
* return     : NA
* remark     : NA
**************************************************************************/
bool CToolsHelp::ConvertIPToInt(char *pszIP,unsigned int &iIP)
{

    int iStrLen = strlen(pszIP);

    char *pszTempIP = pszIP;
    int iTempIP = 0;
    int iDotCount = 0;

    if ('.' == *(pszTempIP+iStrLen-1))
    {
        return false;
    }
    for (int i = 0;i <(iStrLen);i++)
    {
        if ('.' == *pszTempIP)
        {
            if (0xFF < iTempIP)
            {
                return false;
            }
            iIP += iTempIP;
            iIP <<= 8; 
            iTempIP = 0;
            iDotCount++;
        }
        else
        {
            iTempIP *= TEN;
            iTempIP += (int)(*pszTempIP - ZERO_ASCII_VALUE);
        }
        pszTempIP++;
    }
    iIP += iTempIP;
    if (3 != iDotCount)
    {
        return false;
    }
    return true;
}

unsigned int CToolsHelp::AddrChar2Uint(const char *name)
{
	unsigned int ret = 0;
	unsigned int segment = 0;
	unsigned int valid = 1;        //��ַ�Ƿ�Ϸ�

	for (unsigned int i = 0; i < 4; ++i)
	{
		ret <<= 8;
		if (*name != '\0')
		{
			segment = 0;

			while ((*name >= '0') && (*name <= '9'))
			{
				segment *= 10;
				segment += (*name - '0'); 
				name++;
			}

			if (*name != '.' && *name != '\0')
			{
				valid = 0;
				break;
			}

			ret |= segment;

			if (*name == '.')
			{
				++name;
			}
		}
	}
	return valid ? ret : 0;
}