/********************************************************************
	filename	: 	AlarmLinkageXMLProcess.h
	author		:	wulixia
	created		:	2012/12/07	
	description	:	����澯����XML
	copyright	:	Copyright (C) 2011-2015
	history		:	2012/12/07 ��ʼ�汾
*********************************************************************/

#ifndef __ALARM_LINKAGE_MGR_XML_PROCESS_H__
#define __ALARM_LINKAGE_MGR_XML_PROCESS_H__

#include "SDKDef.h"

class CAlarmLinkagrXMLProcess
{
private:
	CAlarmLinkagrXMLProcess();
public:
	~CAlarmLinkagrXMLProcess();

public:

	/***********************************************************************************
    * name       : GetAlarmLinkageXML
    * description: �澯�������ԣ� ƴװ����xml(ɾ���Ͳ�ѯ�����õ�)
    * input      : uiAlarmLinkageID:��������ID; 
    * output     : xml��Ŀ��xml
    * return     : �ɹ�����0��ʧ�ܷ��ظ���������
    * remark     : NA
    ***********************************************************************************/
	static IVS_INT32 GetAndDelAlarmLinkageGetXML(const IVS_CHAR* pReqXml, CXml& xml, std::string& strDomainCode);

	/***********************************************************************************
    * name       : GetAlarmInCode
    * description: ��ѯ�澯�����б��޸���Ӧ��Ϣ�е�AlarmInCode
    * input      : xml:�����xml; pRspXml�� ��Ӧ��Ϣ
    * output     : NA
    * return     : �ɹ�����0��ʧ�ܷ��ظ���������
    * remark     : NA
    ***********************************************************************************/
	static IVS_INT32 GetAlarmInCode(const IVS_CHAR* pReqXML, IVS_CHAR*& pRspXml);

	/***********************************************************************************
    * name       : SetAlarmInCodeValue
    * description: �޸�������Ϣ�е�AlarmInCode��DevCode(ȥ��#�����)(�������޸���������)
    * input      : pReqXMl��������  
	               strLocalDomainCode  ��������룬���ں������豸�������бȽϣ��������ͬ����DevDomainCode��Ϊ��
    * output     : xml�������Ժ��õ�xml��
	               strDomainCode����õ������
    * return     : �ɹ�����0��ʧ�ܷ��ظ���������
    * remark     : NA
    ***********************************************************************************/
	static IVS_INT32  SetAlarmInCodeValue(const IVS_CHAR* pReqXMl, CXml& xml, std::string& strDomainCode);

	/***********************************************************************************
    * name       : GetAlarmLinkageListGetXml
    * description: ��ѯ�澯���������б�ƴװ����xml����ȡ�����
    * input      : pReqXMl��������
    * output     : xml�������Ժ��õ�xml��strDomainCode����õ������
    * return     : �ɹ�����0��ʧ�ܷ��ظ���������
    * remark     : NA
    ***********************************************************************************/
	static IVS_INT32  GetAlarmLinkageListGetXml(const IVS_CHAR* pReqXMl, CXml& xml, std::string& strDomainCode);

	/***********************************************************************************
    * name       : AddDomainCode4XML
    * description: ��xml�м���ڵ�
    * input      : pDomainCode Ҫ����ڵ��ֵ
	               pDomainCodePath  ����·��
				   pValueName   �ڵ�����
				   pReqXml  ����xml
    * output     : xml�������Ժ��õ�xml��
    * return     : �ɹ�����0��ʧ�ܷ��ظ���������
    * remark     : NA
    ***********************************************************************************/
	static IVS_INT32 AddDomainCode4XML(const char* pDomainCode, const IVS_CHAR* pDomainCodePath, const IVS_CHAR* pValueName, const IVS_CHAR* pReqXml, CXml& xml);

	/***********************************************************************************
    * name       : AppendDevCode
    * description: ����Ӧ��Ϣ��ȡ�������ƴ�ڸ澯Դ�������
    * input      : strNSSRsq ��Ӧ��Ϣ
    * output     : xml�������Ժ��õ�xml��
    * return     : �ɹ�����0��ʧ�ܷ��ظ���������
    * remark     : NA
    ***********************************************************************************/
	static IVS_INT32 AppendDevCode(std::string& strNSSRsq, const IVS_CHAR* pPath, CXml& xml, const std::string& strDomainCode);










};

#endif // __ALARM_LINKAGE_MGR_XML_PROCESS_H__
