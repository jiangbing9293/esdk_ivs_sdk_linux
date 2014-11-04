/********************************************************************
	filename	: 	XMLProcess.h
	author		:	z00193167
	created		:	2012/12/03	
	description	:	XML���ö���
	copyright	:	Copyright (C) 2011-2015
	history		:	2012/12/03 ��ʼ�汾
*********************************************************************/

#ifndef __XML_PROCESS_H__
#define __XML_PROCESS_H__

#include "IVSCommon.h"
#include "ToolsHelp.h"
#include "QueryFiled.h"
#include "SDKDef.h"
#include "bp_os.h"

#define QUERYFILED CQueryFiled::GetInstance()

#define MATCHED_SUCCEED 1
#define MATCHED_FAIL 0
#define ON_LINE 1
#define OFF_LINE 0
#define MAXLEN 64
#define MATCHED 0

//sdk�ڲ������
#define IVS_SDK_DEV_CODE_LEN 32

//ͳһʹ��
#define BUILDRET   1
#define BUILSTRING 2

//��������
enum enConfigType
{
	SDKCAMERA = 1, //CameraCode
	SDKDEVICE,  //DevCode
	SDKALARMINCODE, //AlarmInCode
	SDKALARMOUTCODE, //AlarmOutCode
	SDKSERIAL, //SerialCode
	SDKVOICE //VoiceCode
};

#define GET_ELEM_VALUE_POINTER(elem, pXMLValue, xml)  \
    pXMLValue = NULL;                             \
    if (xml.FindElem(elem))                        \
{                                                                  \
    pXMLValue = xml.GetElemValue();    \
}

/* 
*  ��ȡ�����Ϣ-�ַ�
*  elem        �����ҵ�Ԫ�أ���"DEV_INFO"
*  szXMLValue  ��Ž��Ԫ�ص�ָ��
*  NodeInfo    ������Ԫ�ص�ֵ
*  iMaxLen     �����������ֵ
*  xml         ������XML
*/
#define GET_ELEM_VALUE_CHAR(elem, szXMLValue, NodeInfo, iMaxLen, xml)                  \
    szXMLValue = NULL;                                                                  \
	if (xml.FindElem(elem))                                                                \
{																	                   \
	szXMLValue = xml.GetElemValue();                                                   \
	if (NULL != szXMLValue)                                                            \
	{                                                                                  \
	(void)CToolsHelp::Memcpy(NodeInfo, iMaxLen, szXMLValue, strlen(szXMLValue));  \
	}                                                                                  \
}

/* 
*  ��ȡ�����Ϣ-��ֵ
*  elem        �����ҵ�Ԫ�أ���"DEV_INFO"
*  szXMLValue  ��Ž��Ԫ�ص�ָ��
*  NodeInfo    ������Ԫ�ص�ֵ
*  xml         ������XML
*/
#define GET_ELEM_VALUE_NUM(elem, szXMLValue, NodeInfo, xml)                                 \
	NodeInfo = 0;                                                                                        \
	if (xml.FindElem(elem))                                                                \
{																	                   \
	szXMLValue = xml.GetElemValue();                                                   \
	if (NULL != szXMLValue)                                                            \
   {                                                                                   \
   NodeInfo = atoi(szXMLValue);                                                    \
   }                                                                                   \
}

#define GET_ELEM_VALUE_NUM_FOR_UINT(elem, szXMLValue, NodeInfo, xml)                                 \
	NodeInfo = 0;                                                                                        \
	if (xml.FindElem(elem))                                                                \
{																	                   \
	szXMLValue = xml.GetElemValue();                                                   \
	if (NULL != szXMLValue)                                                            \
   {                                                                                   \
   NodeInfo = static_cast<IVS_UINT32>(atoi(szXMLValue));                                                    \
   }                                                                                   \
}

#define GET_ELEM_VALUE_NUM_FOR_UINT64(elem, szXMLValue, NodeInfo, xml)                                 \
	NodeInfo = 0;                                                                                        \
	if (xml.FindElem(elem))                                                                \
{																	                   \
	szXMLValue = xml.GetElemValue();                                                   \
	if (NULL != szXMLValue)                                                            \
   {                                                                                   \
   NodeInfo = static_cast<IVS_UINT64>(BP_OS::strtoll(szXMLValue,NULL,10));                                             \
   }                                                                                   \
}

#define GET_ELEM_VALUE_NUM_FOR_FLOAT(elem, szXMLValue, NodeInfo, xml)                                 \
	NodeInfo = 0;                                                                                        \
	if (xml.FindElem(elem))                                                                \
{																	                   \
	szXMLValue = xml.GetElemValue();                                                   \
	if (NULL != szXMLValue)                                                            \
   {                                                                                   \
   NodeInfo = static_cast<IVS_FLOAT>(atof(szXMLValue));                                                    \
   }                                                                                   \
}

#define GET_ELEM_VALUE_NUM_FOR_DOUBLE(elem, szXMLValue, NodeInfo, xml)                                 \
	NodeInfo = 0;                                                                                        \
	if (xml.FindElem(elem))                                                                \
{																	                   \
	szXMLValue = xml.GetElemValue();                                                   \
	if (NULL != szXMLValue)                                                            \
	{                                                                                   \
	NodeInfo = static_cast<IVS_DOUBLE>(atof(szXMLValue));                                                    \
	}                                                                                   \
}


// �ж�ָ���Ƿ�Ϊ�� 
#define CHECK_IVSBOOL( retValue )    \
	if (IVS_SUCCEED != CParaCheck::CheckIsIvsBool(retValue))                    \
{                                            \
	return IVS_PARA_INVALID;                         \
}                                            \

class CUserMgr;//lint !e763
class CXmlProcess
{
public:
	/**************************************************************************
    * name       : GetUnifiedFormatQueryXML
    * description: ͨ�ò�ѯת����XML
    * input      : pUnifiedQuery   ͨ�÷�ҳ��ѯ�ṹ��
                   xml  �����Cxml��
    * output     : NA
    * return     : ͨ�÷���
    * remark     : NA
    **************************************************************************/
	static IVS_INT32 GetUnifiedFormatQueryXML(const IVS_QUERY_UNIFIED_FORMAT* pUnifiedQuery, CXml &xml, 
													const IVS_CHAR* pDomainCode = NULL,const IVS_CHAR* pCamerCode = NULL);
	static IVS_INT32 PrintfXML2File(const IVS_CHAR* FileName,const IVS_CHAR* XMLContent);

    static IVS_INT32 GetCommBuildRet(CXml &xml, NSS_MSG_TYPE_E msgType, IVS_INT32 iType, const std::string& strLinkID);
	static IVS_INT32 GetCommBuildRet(CXml &xmlReq, CXml &xmlRsp, NSS_MSG_TYPE_E msgType, IVS_INT32 iType, const std::string& strLinkID);

    static IVS_INT32 ParseDevCode(const IVS_CHAR *pDevBigCode, IVS_CHAR *pDevCode,IVS_CHAR* pDomainCode );

	//ֻ֧��ͬһ�����µ��豸�����ȡ
	static IVS_INT32 ParseDevCodeList(std::list<std::string>& lDevCodeList,std::string& strCamerCode,IVS_CHAR* pDomainCode );

	//ת��int���͵�BOOL
	static bool TransIntToBool(unsigned int iRet);

	//ת��0,1���ִ�ON,OFF
	static std::string TransIntToOnOff(unsigned int iRet);

	//ת��ON,OFF��0,1��
	static unsigned int TransOnOffToInt(const std::string &str);

	//�ַ���ת����long
	static long TransStrToLong(const std::string& str);

	//�ַ���0,1������0,1��ת��
	static  int TransStrToInt(const std::string& str);

	//�����ַ����鵽�ִ���ת������Ҫ�������ݽṹ���ַ����ֵ�ת��//
	static void SetElemValueStr(const IVS_CHAR* pXmlValue,IVS_UINT32 ulLength,CXml &xml);

	//�������ִ�ת����ʮ��������
	static unsigned int TransBinStrToDec(const std::string& str);

	// �޸�AlarmInCode
	static IVS_INT32  SetAlarmInCodeValue(const IVS_CHAR* pPath, CXml& xml);

	//ͨ�õĲ�ѯ����
	static IVS_INT32 GetCommConfigGetXML(const IVS_CHAR* pDomainCode,const IVS_CHAR* pDevCode, CXml& xml, enConfigType enType);

	// ʵ����������Ϣ��������
	static IVS_INT32 GetCommSendMgr(CXml &xml, CUserMgr* pUserMgr, NSS_MSG_TYPE_E msgType, IVS_INT32 iType, const IVS_CHAR* pDomainCode);

	// ʵ����������Ϣ�������� -- ����ڴ�й¶
	static IVS_INT32 GetCommSendMgr(CXml &xml, CXml &OutXml,CUserMgr* pUserMgr, NSS_MSG_TYPE_E msgType, IVS_INT32 iType, const IVS_CHAR* pDomainCode);

	// ʵ����������Ϣ��������(֧���ض���)
	static IVS_INT32 GetCommSendMgrForRedirect(CXml &xml, CUserMgr* pUserMgr, NSS_MSG_TYPE_E msgType, IVS_INT32 iType,  const IVS_CHAR* pCameraCode,const IVS_CHAR* pDomainCode);

	// ʵ����������Ϣ��������(֧���ض���)---- ����ڴ�й¶
	static IVS_INT32 GetCommSendMgrForRedirect(CXml &reqXml, CXml &rspXml,CUserMgr* pUserMgr, NSS_MSG_TYPE_E msgType, IVS_INT32 iType, const IVS_CHAR* pCameraCode,const IVS_CHAR* pDomainCode);

	// ͨ������XML����QueryField�ֶ�
	static IVS_INT32 AddQueryFieldForUnifiedFormatQueryXML(const IVS_CHAR* pField,const IVS_CHAR* pValue, CXml& xmlReq);

};

#endif
