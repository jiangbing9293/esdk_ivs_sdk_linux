/********************************************************************
	filename	: 	ToolsHelp.h
	author		:	s00191067
	created		:	2011/10/28	
	description	:	�ṩ����ģ�鹫�������Ĺ����ļ�
	copyright	:	Copyright (C) 2011-2015
	history		:	2011/10/28
*********************************************************************/

#ifndef __TOOLSHELP_H__
#define __TOOLSHELP_H__

/*!< Ԥ����ͷ�ļ� */
#include <map>
#include <string>

/*!< ����PCLint */
#include "IvsLog.h"

using namespace std;

//typedef std::map <string, string> HttpHeadInfoMap;

class CToolsHelp
{
public:
    /**************************************************************************
    * name       : GetProgramDir
    * description: ��ȡ��������·��
    * input      : NA
    * output     : NA
    * return     : ��������·��
    * remark     : NA
    **************************************************************************/
    static string GetProgramDir();

    /***********************************************************************************
    * name       : Int2Str
    * description: ������ת��Ϊ�ַ���
    * input      : iValue:Ҫת����������ֵ; bHex:�Ƿ���ʮ������������
    * output     : NA
    * return     : ת������ַ���
    * remark     : NA
    ***********************************************************************************/
    static string Int2Str ( int iValue, bool bHex = false );

    /***********************************************************************************
    * name       : Str2Int
    * description: ������ת��Ϊ�ַ���
    * input      : strSrc:Ҫת�����ַ���; bHex:�Ƿ���ʮ������������
    * output     : NA
    * return     : ת�����������
    * remark     : NA
    ***********************************************************************************/
    static int Str2Int( const string& strSrc, bool bHex = false );

    /**************************************************************************
    * name       : ANSI2Unicode
    * description: �ַ�����ת������ANSI��UNICODE
    * input      : src ��ת�����ַ�
    * output     : NA
    * return     : TCHAR* ���ַ�
    * remark     : Լ������ַ�������Ϊ1024,������Static�����洢��������Է�ֹƵ��ʹ��ʱ
    �����ڴ���Ƭ
    **************************************************************************/
    static const WCHAR* ANSI2Unicode(const char* src);

    /**************************************************************************
    * name       : Unicode2ANSI
    * description: �ַ�����ת������UNICODE��ANSI
    * input      : src ��ת���Ŀ��ַ�
    * output     : NA
    * return     : char* Ansi�ַ�
    * remark     : Լ������ַ�������Ϊ1024,������Static�����洢��������Է�ֹƵ��ʹ��ʱ
    �����ڴ���Ƭ
    **************************************************************************/
    static const char* Unicode2ANSI(const WCHAR* src);

    /**************************************************************************
    * name       : 
    * description: 
    * input      : 
    * output     : 
    * return     : NA
    * remark     : NA
    **************************************************************************/
    static  bool ConvertIPToInt(char *pszIP,unsigned int &iIP);

    /**************************************************************************
    * name       : GetLogLevel
    * description: ȡ�����ļ��е���־����
    * input      : 
    * output     : 
    * return     : unsigned int
    * remark     : NA
    **************************************************************************/
    static IvsLogLevel GetLogLevel();

	static unsigned int AddrChar2Uint(const char *name);
};

#endif //__TOOLSHELP_H__               
