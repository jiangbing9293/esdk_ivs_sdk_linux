/********************************************************************
filename    :    ToolsMsgMethod.h
author      :    yKF70958
created     :    2012/04/12
description :    ��ϢID��������
copyright   :    Copyright (C) 2011-2015
history     :    2012/04/12 ��ʼ�汾
*********************************************************************/
#ifndef TOOL_HELP_H
#define TOOL_HELP_H

#include <string.h>
#include <map>
#include <list>
#include <vector>
#include <bp_os.h>
#include "ace_header.h"
#include "bp_def.h"

NAMESPACE_BP_BEGIN  //namespace begin

#define  STR_LENGTH_SIZE	20
#define	 INT_FORMAT_D		10
#define  INT_FORMAT_X		16
#define  STR_FORMAT_D		"%d"
#define  STR_FORMAT_X		"%x"
static const int MAX_INT    = 2147483647;
static const signed int MIN_INT    = -2147483647-1;

typedef std::map<std::string,std::string> IvspNodeMap;
typedef IvspNodeMap::iterator IvspNodeMapIter;

class EXPORT_DLL CToolHelp 
{
public:
    /***********************************************************************************
    * name       : IntToStr
    * description: ������ת��Ϊ�ַ���
    * input      : iValue:Ҫת����������ֵ; bHex:�Ƿ���ʮ������������
    * output     : NA
    * return     : ת������ַ���
    * remark     : NA
    ***********************************************************************************/
    static std::string IntToStr ( int iValue, bool bHex = false );

	/***********************************************************************************
    * name       : StrToInt
    * description: ������ת��Ϊ�ַ���
    * input      : strSrc:Ҫת�����ַ���; bHex:�Ƿ���ʮ������������
    * output     : NA
    * return     : ת�����������
    * remark     : NA
    ***********************************************************************************/
    static int StrToInt ( const std::string& strSrc, bool bHex = false );
    
    //����һ���ṹ�壬�����ݣ��������ڣ����ڣ������ǷǷ��ģ�������mktime��sttm&��������Է��غϷ�����ֵ��
    static struct tm ymdhmsToTime(const std::string& strDt);
   
    //����һ��time_t������ֵ���Ǳ���ʱ�䣨��������Ӧ�ĸ�������ʱ����ԣ�1970-1-1 00:00:00GMT����������
    //���Ҫ���ɱ��ص�ʱ�䣬��Ҫ����localtime����
    static time_t ymdhmsToTime_t(const std::string& strDt);
    //unit=0(��)1ʱ2�֣�4�룬��3�����Ǳ���
    static void timeadd(struct tm& stTime,int unit,int num);

    /***********************************************************************************
    * name       : IsLegalParam
    * description: �ж�xml�����Ƿ񳬹���󳤶�
    * input      : pcParam:xml�������ַ���ֵ; uiMaxLen:���ݿ��в�������
    * output     : NA
    * return     : IVS_SUCCEED - �Ϸ���IVS_FAIL - �Ƿ�
    * remark     : NA
    ***********************************************************************************/
    static int IsLegalParam(const char* pcParam, unsigned int uiMaxLen);

    /***********************************************************************************
    * name       : IsLegalInt
    * description: �ж�xml�����Ƿ�Ϊint����
    * input      : pcParam:xml�������ַ���ֵ; iMax:�������ֵ
    * output     : NA
    * return     : IVS_SUCCEED - �Ϸ���IVS_FAIL - �Ƿ�
    * remark     : NA
    ***********************************************************************************/
    static int IsLegalInt(const char* pcParam, int iMax = MAX_INT);

    static bool IsLegalUIntR(unsigned int &Result,const char* pcParam, unsigned int iMax = -1,unsigned int iMin = 0);
    static bool IsLegalSIntR(int &Result,const char* pcParam,int iMax =MAX_INT , int iMin = MIN_INT);

    /***********************************************************************************
    * name       : IsLegalEnum
    * description: �ж�xml�����Ƿ�Ϊ�Ϸ���ö������ֵ
    * input      : pcParam:xml�������ַ���ֵ; iLow:��ֵ; iHigh:��ֵ; 
    * output     : NA
    * return     : IVS_SUCCEED - �Ϸ���IVS_FAIL - �Ƿ�
    * remark     : NA
    ***********************************************************************************/
    static int IsLegalEnum(const char* pcParam, int iLow, int iHigh);
    
    /***********************************************************************************
    * name       : IsLegalDate
    * description: �ж�pcParaml�����Ƿ�Ϊ�Ϸ���8λ��������ֵ
    * input      : pcParam �����ַ���ֵ; 
    * output     : NA
    * return     : IVS_SUCCEED - �Ϸ���IVS_FAIL - �Ƿ�
    * remark     : NA
    ***********************************************************************************/
    static int IsLegalDate(const char* pcParam);

    /***********************************************************************************
    * name       : IsLegalTime
    * description: �ж�pcParaml�����Ƿ�Ϊ�Ϸ���6λʱ������ֵ
    * input      : pcParam �����ַ���ֵ; 
    * output     : NA
    * return     : IVS_SUCCEED - �Ϸ���IVS_FAIL - �Ƿ�
    * remark     : NA
    ***********************************************************************************/
    static int IsLegalTime(const char* pcParam);

    /***********************************************************************************
    * name       : IsLegalDateTime
    * description: �ж�pcParaml�����Ƿ�Ϊ�Ϸ���14λ����ʱ������ֵ
    * input      : pcParam �����ַ���ֵ; 
    * output     : NA
    * return     : IVS_SUCCEED - �Ϸ���IVS_FAIL - �Ƿ�
    * remark     : NA
    ***********************************************************************************/
    static int IsLegalDateTime(const char* pcParam);
    static int IsLegalDateTimeMs(const char* pcParam);

    /***********************************************************************************
    * name       : SplitString
    * description: src��dlim�ָ��ΪslRet�б�
    * input      : src,dlim; 
    * output     : slRet
    * return     : NA
    * remark     : NA ��ascii�ַ�,�������src�������������slRet��Ԫ�أ���ΪslRet�ں���ִ�й����лᱻ����д���Ӷ��ƻ����������ֵ��
    ***********************************************************************************/
    static void SplitString(const char* src,const char* dlim,std::vector<std::string>& slRet );
    static void SplitString(const std::string &src,const std::string &dlim,std::vector<std::string>& slRet );

    /***********************************************************************************
    * name       : ToUpper
    * description: srcת���ɴ�д��
    * input      : src; 
    * output     : 
    * return     : ���ش�д��string
    * remark     : NA
    ***********************************************************************************/
    static std::string ToUpper(const std::string& strSrc);

    /***********************************************************************************
    * name       : LTrim
    * description: strSrcȥ����ո�
    * input      : src; 
    * output     : 
    * return     : ���ش�д��string
    * remark     : NA
    ***********************************************************************************/
    static void LTrim(std::string& strSrc);

    /***********************************************************************************
    * name       : LTrim
    * description: strSrcȥ���ҿո�
    * input      : src; 
    * output     : 
    * return     : ���ش�д��string
    * remark     : NA
    ***********************************************************************************/
    static void RTrim(std::string& strSrc);

    /***********************************************************************************
    * name       : LTrim
    * description: strSrcȥ�����ҿո�
    * input      : src; 
    * output     : 
    * return     : ���ش�д��string
    * remark     : NA
    ***********************************************************************************/
    static std::string Trim(std::string& strSrc);

    /************************************************************************/
    /*                                                                      */
    /************************************************************************/
    static void ReadIvspHeader(const std::string& strXmlData,IvspNodeMap& outHeaderMap);

    static void ReadIvspHeader(const char* pszXmlData,IvspNodeMap& outHeaderMap);

    static void GetValueFromMap(IvspNodeMap& inHeaderMap,const std::string& strKey, std::string& strOutValue);
    /**************************************************************************
    * name       : ReadNextXmlNode
    * description: ˳���ȡxml���¸��ڵ�
    * input      :  pszXmlData xml����
                    pszStartTag ��ʼ��ǩ
                    pszEndTag ������ǩ
                    pszTagName �ڵ����ƣ�map�е�key��
    * output     : outHeaderMap ����Ľڵ��ֵ��
    * return     : xml�����ڵ���������ָ��
    * remark     : NA
    **************************************************************************/    
    static const char* ReadNextXmlNode(const char* pszXmlData,const char* pszStartTag,const char* pszEndTag,const char* pszTagName,IvspNodeMap& outHeaderMap);
};

NAMESPACE_BP_END   // namespace end

#endif //CAPABILITY_MGR_H
