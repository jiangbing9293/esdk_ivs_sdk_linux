/********************************************************************
	filename	: 	AlarmMgrXMLProcess.h
	author		:	z00193167
	created		:	2012/12/03	
	description	:	����澯XML
	copyright	:	Copyright (C) 2011-2015
	history		:	2012/12/03 ��ʼ�汾
*********************************************************************/

#ifndef __ALARM_MGR_XML_PROCESS_H__
#define __ALARM_MGR_XML_PROCESS_H__

#include "IVSCommon.h"
#include "ivs_error.h"
#include "ToolsHelp.h"
#include "XmlProcess.h"
// #include <string.h>
#include "QueryFiled.h"
#include "hwsdk.h"
#include "SDKDef.h"

class CAlarmMgrXMLProcess
{
private:
	CAlarmMgrXMLProcess();
public:
	~CAlarmMgrXMLProcess();

public:
	/***********************************************************************************
    * name       : GetConfirmWaterAlarmXML
    * description: ˮӡ�澯ȷ��xml
    * input      : ullAlarmEventID:�澯ID; pConfirmInfo:�澯������Ա�������Ϣ; pConfirmTime:�澯����ʱ��; xml:����XML
    * output     : NA
    * return     : �ɹ�����0��ʧ�ܷ��ظ���������
    * remark     : NA
    ***********************************************************************************/
	static IVS_INT32 GetConfirmWaterAlarmXML(const IVS_UINT64 ullAlarmEventID,const IVS_CHAR* pConfirmInfo,
													const IVS_CHAR* pConfirmTime,const std::string& strUserDomainCode,const std::string& strDomainCode,CXml &xml);

	/***********************************************************************************
    * name       : GetCancelWaterAlarmXML
    * description: ˮӡ�澯����xml
    * input      : ullAlarmEventID:�澯ID; pCancelInfo:�澯ȡ��������Ϣ; pCancelTime:�澯ȡ��ʱ��; xml:����XML
    * output     : NA
    * return     : �ɹ�����0��ʧ�ܷ��ظ���������
    * remark     : NA
    ***********************************************************************************/
	static IVS_INT32 GetCancelWaterAlarmXML(const IVS_UINT64 ullAlarmEventID,const IVS_CHAR* pCancelInfo,
													const IVS_CHAR* pCancelTime,const std::string& strUserDomainCode,const std::string& strDomainCode,CXml &xml);

	/******************************************************************
	* name       : GetWaterAlarmListParseXML
	* description: ˮӡ�澯��ѯ
	* input      : pWaterAlarmInfoList
				   strDoamianCode
				   uiBufferSize
	* output     : Rspxml
	* return     : �ɹ�����0��ʧ�ܷ��ظ���������
	* remark     : NA
    *******************************************************************/   
	static IVS_INT32 GetWaterAlarmListParseXML(IVS_WATERMARK_ALARM_INFO_LIST* pWaterAlarmInfoList,const std::string& strDoamianCode, IVS_UINT32 uiBufferSize, CXml &Rspxml);

	/***********************************************************************************
    * name       : AlarmLevelGetXML
    * description: �澯����ƴ��xml
    * input      : cLoginID:��¼ID; pAlarmLevel:�澯����ṹ��; i:�ж������Ӹ澯�������޸�; xml:����XML
    * output     : NA
    * return     : �ɹ�����0��ʧ�ܷ��ظ���������
    * remark     : NA
    ***********************************************************************************/
	static IVS_INT32 AlarmLevelGetXML(const IVS_ALARM_LEVEL *pAlarmLevel, IVS_INT32 i, CXml &xml);

	/***********************************************************************************
    * name       : AddAlarmLevelPraseXML
    * description: ���Ӹ澯�������xml
    * input      : pAlarmLevelID:�澯����ID; xml:����XML
    * output     : NA
    * return     : �ɹ�����0��ʧ�ܷ��ظ���������
    * remark     : NA
    ***********************************************************************************/
	static IVS_INT32 AddAlarmLevelPraseXML(IVS_UINT32* pAlarmLevelID,CXml &xml);

	/***********************************************************************************
    * name       : DeleteAlarmLevelGetXML
    * description: ɾ���澯����ƴ��xml
    * input      : cLoginID:��¼ID; uiAlarmLevelId:�澯����ID; xml:����XML
    * output     : NA
    * return     : �ɹ�����0��ʧ�ܷ��ظ���������
    * remark     : NA
    ***********************************************************************************/
	static IVS_INT32 DeleteAlarmLevelGetXML(const IVS_UINT32 uiAlarmLevelId, CXml &xml); 
	
	/***********************************************************************************
    * name       : GetAlarmTypeLevelPraseXML
    * description: ��ѯ�澯�����б����xml
    * input      : uiAlarmLevelNum:ϵͳ���澯�������; pAlarmLevelNum:��ѯ���ص�ϵͳ�澯�������; pAlarmLevel:�澯�����б�; xml:����XML
    * output     : NA
    * return     : �ɹ�����0��ʧ�ܷ��ظ���������
    * remark     : NA
    ***********************************************************************************/
	static IVS_INT32 GetAlarmTypeLevelPraseXML(IVS_UINT32 uiAlarmLevelNum,IVS_UINT32* pAlarmLevelNum,IVS_ALARM_LEVEL* pAlarmLevel,CXml &xml);

	/***********************************************************************************
    * name       : GetAlarmLevelGetXML
    * description: ��ѯ�澯������ϸ��Ϣƴ��xml
    * input      : pLoginID:��¼ID; uiAlarmLevelId:�澯����ID; xml:����XML
    * output     : NA
    * return     : �ɹ�����0��ʧ�ܷ��ظ���������
    * remark     : NA
    ***********************************************************************************/
	static IVS_INT32 GetAlarmLevelGetXML(IVS_UINT32 uiAlarmLevelID, CXml& xml);

	/***********************************************************************************
    * name       : GetAlarmLevelPraseXML
    * description: ��ѯ�澯������ϸ��Ϣ����xml
    * input      : pAlarmLevel:�澯����ṹ��; xml:����XML
    * output     : NA
    * return     : �ɹ�����0��ʧ�ܷ��ظ���������
    * remark     : NA
    ***********************************************************************************/
	static IVS_INT32 GetAlarmLevelPraseXML(IVS_ALARM_LEVEL* pAlarmLevel, CXml& xml);

	/***********************************************************************************
    * name       : HelpInfoGetXML
    * description: �澯����ƴ��xml
    * input      : cLoginID:��¼ID; helpInfo:������Ϣ�ṹ��; xml:����XML
    * output     : NA
    * return     : �ɹ�����0��ʧ�ܷ��ظ���������
    * remark     : NA
    ***********************************************************************************/
	static IVS_INT32 HelpInfoGetXML(const IVS_HELP_INFO *helpInfo, CXml &xml);

	/***********************************************************************************
    * name       : DeleteHelpInfoGetXML
    * description: ɾ���澯����ƴ��xml
    * input      : cLoginID:��¼ID; uiSourceId:Ŀ������1������2���澯����; uiSourceType:������ϢĿ��IDĿǰ֧�ַ���/�澯����; xml:����XML
    * output     : NA
    * return     : �ɹ�����0��ʧ�ܷ��ظ���������
    * remark     : NA
    ***********************************************************************************/
	static IVS_INT32 DeleteHelpInfoGetXML(const IVS_UINT32 uiSourceId,IVS_UINT32 uiSourceType, CXml &xml);

	/***********************************************************************************
    * name       : QueryHelpInfoGetXML
    * description: ��ѯ�澯����ƴ��xml
    * input      : uiAlarmHelpSourceType:������ϢĿ��IDĿǰ֧�ַ���/�澯����; uiAlarmHelpSourceId:Ŀ������1������2���澯����; helpInfo:������Ϣ�ṹ��; xml:����XML
    * output     : NA
    * return     : �ɹ�����0��ʧ�ܷ��ظ���������
    * remark     : NA
    ***********************************************************************************/
	static IVS_INT32 QueryHelpInfoGetXML(IVS_UINT32 uiAlarmHelpSourceType,IVS_UINT32 uiAlarmHelpSourceId,CXml &xml);

    /******************************************************************
     function   : QueryHelpInfoTextGetXML
     description: ��ѯ�澯������Ϣ����
     input      : const IVS_CHAR * pAlarmInCode �澯Դ����
     input      : const IVS_CHAR * pAlarmType �澯����
     output      : CXml & xml
     return     : IVS_INT32
    *******************************************************************/
    static IVS_INT32 QueryHelpInfoTextGetXML(const IVS_CHAR* pAlarmInCode, const IVS_CHAR* pAlarmType, CXml &xml);

	/***********************************************************************************
    * name       : QueryHelpInfoPraseXML
    * description: ��ѯ�澯��������xml
    * input      : helpInfo:������Ϣ�ṹ��; xml:����XML
    * output     : NA
    * return     : �ɹ�����0��ʧ�ܷ��ظ���������
    * remark     : NA
    ***********************************************************************************/
	static IVS_INT32 QueryHelpInfoPraseXML(IVS_HELP_INFO *helpInfo, CXml &xml);

	/***********************************************************************************
    * name       : SetAlarmTypeLevelGetXML
    * description: ���ø澯����ƴ��xml
    * input      : pLoginID:��¼ID; pAlarmTypeList:�澯�����б�ṹ��; xml:����XML
    * output     : NA
    * return     : �ɹ�����0��ʧ�ܷ��ظ���������
    * remark     : NA
    ***********************************************************************************/
	static IVS_INT32 SetAlarmTypeLevelGetXML(const IVS_ALARM_TYPE_LIST* pAlarmTypeList, CXml& xml);

	/***********************************************************************************
    * name       : GetAlarmTypeListParseXML
    * description: ��ѯ�Զ���澯�����б����xml
    * input      : pAlarmTypeList:�澯�����б�ṹ��; xml:����XML; uiBufferSize:������
    * output     : NA
    * return     : �ɹ�����0��ʧ�ܷ��ظ���������
    * remark     : NA
    ***********************************************************************************/
	static IVS_INT32 GetAlarmTypeListParseXML(IVS_ALARM_TYPE_LIST* pAlarmTypeList, CXml &xml, IVS_UINT32 uiBufferSize);

	/***********************************************************************************
    * name       : UserAlarmAuthorGetXML
    * description: �澯�û���֤ƴ��xml
    * input      : pLoginID:��¼ID; pPasswordInfo:�û������ֶ�; xml:����XML
    * output     : NA
    * return     : �ɹ�����0��ʧ�ܷ��ظ���������
    * remark     : NA
    ***********************************************************************************/
	static IVS_INT32 UserAlarmAuthorGetXML(const IVS_CHAR *pPasswordInfo, CXml &xml);

	/***********************************************************************************
    * name       : GetAlarmNotifyGetXML
    * description: �澯�ϱ�ƴ��xml
    * input      : palarmNotify:�澯�ϱ��ṹ��; xml:����XML
    * output     : NA
    * return     : �ɹ�����0��ʧ�ܷ��ظ���������
    * remark     : NA
    ***********************************************************************************/
	static IVS_INT32 GetAlarmNotifyGetXML(const IVS_ALARM_NOTIFY* palarmNotify,CXml& xml);

	/***********************************************************************************
    * name       : GetAlarmTypeListGetXML
    * description: ��ѯ�澯�����б�ƴ��xml
    * input      : pLoginID:��¼ID; palarmNotify:�澯�ϱ��ṹ��; xml:����XML
    * output     : NA
    * return     : �ɹ�����0��ʧ�ܷ��ظ���������
    * remark     : NA
    ***********************************************************************************/
	static IVS_INT32 GetAlarmTypeListGetXML(const IVS_CHAR* pLoginID, const IVS_QUERY_UNIFIED_FORMAT* pUnifiedQuery, CXml& xml);

	/***********************************************************************************
    * name       : GetAlarmAreaPlanXML
    * description: ���������ƻ� ƴװ����xml
    * input      : pLoginID:��¼ID; pAlarmAreaPlan:���������ƻ��ṹ��; 
    * output     : xml��Ŀ��xml
    * return     : �ɹ�����0��ʧ�ܷ��ظ���������
    * remark     : NA
    ***********************************************************************************/
	static IVS_INT32 GetAlarmAreaPlanXML(const IVS_CHAR* pDomainCode,IVS_UINT32& uiAlarmAreaID,CXml& xml);

	/***********************************************************************************
    * name       : AlarmAreaGetXML
    * description: ɾ���澯����, ��ѯ�������飬�ֶ�������ƴ��XML
    * input      : pDomainCode:�����; uiAlarmAreaId:����ID; xml:ƴ�ӵ�xml;
    * output     : NA
    * return     : �ɹ�������ȷ�룬ʧ�ܷ��ش�����
    * remark     : NA
    ***********************************************************************************/
	static IVS_INT32 AlarmAreaGetXML(const IVS_CHAR* pDomainCode, const IVS_UINT32 uiAlarmAreaId, CXml &xml);

	/***********************************************************************************
    * name       : GetAlarmListParseXML
    * description: ��ѯ��ʷ�澯��Ϣ�б�ƴ��XML
    * input      : pAlarmEventList:�澯�ϱ���Ϣ��ҳ��ѯ�б�; uiBufferSize:�ƻ�������ڴ��С; xml:ƴ�ӵ�xml,strDomainCode,����룬ƴ����AlarmInCode��
    * output     : NA
    * return     : �ɹ�������ȷ�룬ʧ�ܷ��ش�����
    * remark     : NA
    ***********************************************************************************/
	static IVS_INT32 GetAlarmListParseXML(IVS_ALARM_EVENT_LIST* pAlarmEventList, IVS_UINT32 uiBufferSize, CXml &xml, const std::string& strDomainCode);

	/***********************************************************************************
    * name       : GetAlarmStatusGetXML
    * description: �澯״̬�ϱ�ƴ��XML
    * input      : pAlarmEventList:�澯�ϱ���Ϣ��ҳ��ѯ�б�; uiBufferSize:�ƻ�������ڴ��С; xml:ƴ�ӵ�xml;
    * output     : NA
    * return     : �ɹ�������ȷ�룬ʧ�ܷ��ش�����
    * remark     : NA
    ***********************************************************************************/
	static IVS_INT32 GetAlarmStatusGetXML(const IVS_ALARM_EVENT* pAlarmEvent, CXml& xml);

	/***********************************************************************************
    * name       : GetAlarmInfoGetXML
    * description: ��ѯ�澯��Ϣ�¼�ƴ��XML
    * input      : pLoginID:��¼ID; ullAlarmEventID:�澯�¼�ID; xml:ƴ�ӵ�xml;
    * output     : NA
    * return     : �ɹ�������ȷ�룬ʧ�ܷ��ش�����
    * remark     : NA
    ***********************************************************************************/
	static IVS_INT32 GetAlarmInfoGetXML(IVS_UINT64 ullAlarmEventID, CXml &xml);


	/***********************************************************************************
    * name       : GetAlarmInfoParseXML
    * description: ��ѯ�澯��Ϣ�¼�����XML
    * input      : pAlarmEvent:�澯��Ϣ�ṹ��; xml:������xml;
    * output     : NA
    * return     : �ɹ�������ȷ�룬ʧ�ܷ��ش�����
    * remark     : NA
    ***********************************************************************************/
	static IVS_INT32 GetAlarmInfoParseXML(IVS_ALARM_EVENT* pAlarmEvent, CXml &xml, const IVS_CHAR* pDomainCode);

	/***********************************************************************************
    * name       : GetDeviceUserListGetXML
    * description: ��ȡӵ��ָ���豸Ȩ�޵��û��б�ƴ��XML
    * input      : pDevCode:�豸����; pIndexRange:��ҳ��Ϣ xml:ƴ�ӵ�xml;
    * output     : NA
    * return     : �ɹ�������ȷ�룬ʧ�ܷ��ش�����
    * remark     : NA
    ***********************************************************************************/
	static IVS_INT32 GetDeviceUserListGetXML(const IVS_CHAR* pDomainCode, const IVS_CHAR* pDevCode, IVS_UINT32 uiIsUserOnline, const IVS_INDEX_RANGE* pIndexRange, const std::string& strGroupID, CXml &xml);

	/***********************************************************************************
    * name       : GetDeviceUserListParseXML
    * description: ��ȡӵ��ָ���豸Ȩ�޵��û��б����XML
    * input      : pDevUserList:�豸�û��б�ṹ��; uiBufferSize:�����С xml:������xml;
    * output     : NA
    * return     : �ɹ�������ȷ�룬ʧ�ܷ��ش�����
    * remark     : NA
    ***********************************************************************************/
	static IVS_INT32 GetDeviceUserListParseXML(IVS_DEVICE_USER_INFO_LIST* pDevUserList, IVS_UINT32 uiBufferSize, CXml &xml);

		/***********************************************************************************
    * name       : GetAbilityInfoParseXML
    * description: ��ȡӵ��ָ���豸Ȩ�޵��û��б����XML
    * input      : uiAbility:ƽ̨֧�ֵ�����ֵ xml:������xml;
    * output     : NA
    * return     : �ɹ�������ȷ�룬ʧ�ܷ��ش�����
    * remark     : NA
    ***********************************************************************************/
	static IVS_INT32 GetAbilityInfoParseXML(IVS_UINT32* uiAbility,   const char*);

	/***********************************************************************************
    * name       : SetAlarmInCodeValue
	* description: �澯���Ĵ��������AlarmInCode(����ɾ����ͨ��)
	* input      : xml�������xml;
    * output     : NA
    * return     : �ɹ�������ȷ�룬ʧ�ܷ��ش�����
    * remark     : NA
    ***********************************************************************************/
	static IVS_INT32 SetAlarmInCodeValue(CXml& xml);

	/***********************************************************************************
    * name       : ModifyAlarmInCodeOfGetAlarmList
    * description: ��ѯ�澯��ʷ�б��޸�AlarmInCode�ֶ�
    * input      : xml�������xml;
    * output     : std::string strDstDomainCode ��ϢĿ����
    * return     : �ɹ�������ȷ�룬ʧ�ܷ��ش�����
    * remark     : NA
    ***********************************************************************************/
	static IVS_INT32 ModifyAlarmInCodeOfGetAlarmList(CXml& xml, std::string& strDstDomainCode);

	/***********************************************************************************
    * name       : GetDevAlarmListParseXML
    * description: ��ѯOMU�澯�¼��б�
    * input      : xml����Ӧ��Ϣxml;
	               pDevAlarmList���澯�¼��б�ṹ��
				   uiBufSize, �����С
    * output     : NA
    * return     : �ɹ�������ȷ�룬ʧ�ܷ��ش�����
    * remark     : NA
    ***********************************************************************************/
	static IVS_INT32 GetDevAlarmListParseXML(CXml& xml, IVS_DEVICE_ALARM_EVENT_LIST* pDevAlarmList, const IVS_UINT32& uiBufSize);

	/***********************************************************************************
    * name       : AlarmOutGetXML
    * description: ��ͣ�澯������ƴװxml
    * input      : xml����Ӧ��Ϣxml;
	               pAlarmInCode���澯����豸����
				   pDomainCode, �����
    * output     : NA
    * return     : �ɹ�������ȷ�룬ʧ�ܷ��ش�����
    * remark     : NA
    ***********************************************************************************/
	static IVS_INT32 AlarmOutGetXML(CXml& xml, const IVS_CHAR* pAlarmInCode, const IVS_CHAR* pDomainCode, IVS_INT32 iStateType);

	/***********************************************************************************
    * name       : GetAlarmAreaListGetDomainCode
    * description: ��ȡ������Ϣ�б������xml�л�ȡ�����,��ֲ�ѯ�����е�AlarmInCode
    * input      : pReqXml������xml;
    * output     : strDomainCode�� �����  xml���޸�֮���xml
    * return     : �ɹ�������ȷ�룬ʧ�ܷ��ش�����
    * remark     : NA
    ***********************************************************************************/
	static IVS_INT32 GetDomainCodeANDParseAlarmInCode(const IVS_CHAR* pReqXml, std::string& strDomainCode, CXml& xml);

   /***********************************************************************************
    * name       : GetGroupListGetXml
    * description: ���������豸ȡ���豸���б�ƴװxml
    * input      : pDevCode���豸����
	               pDomainCode, �����
    * output     : xml��ƴװ֮���xml
    * return     : �ɹ�������ȷ�룬ʧ�ܷ��ش�����
    * remark     : NA
    ***********************************************************************************/
	static IVS_INT32 GetGroupListGetXml(const IVS_CHAR* pDevCode, const IVS_CHAR* pDomainCode, CXml& xml);

    /***********************************************************************************
    * name       : GetGroupID
    * description: ��ȡGroupID�������GroupID����һ���м���","����
    * input      : xml��Դxml
    * output     : strGroupID�����GroupID���֮����ַ���
    * return     : �ɹ�������ȷ�룬ʧ�ܷ��ش�����
    * remark     : NA
    ***********************************************************************************/
	static IVS_INT32 GetGroupID(CXml& xml, std::string& strGroupIDs);
	/***********************************************************************************
    * name       : GetRsqXML
    * description: ͸���ӿ��У�����Ӧ��Ϣ��stringת��pRsqXml��ָ�ڴ���
    * input      : pRsp, ��Ӧ��Ϣ
    * output     : pRsqXml�� ת֮���xml
    * return     : �ɹ�������ȷ�룬ʧ�ܷ��ش�����
    * remark     : NA
    ***********************************************************************************/
	static IVS_INT32 GetRsqXML(const IVS_CHAR* pRsp, IVS_CHAR*& pRspXml);

   /***********************************************************************************
    * name       : SetAlarmInCode4AlarmAreaReq
    * description: ���AlarmInCode�����DevDomainCode�ڵ�(�������ӿշ�����������û�и澯Դ�豸��Ϣ��Ϣ)�����������޸ķ�����ʹ��
    * input      : iMsgType, ��Ϣ����
	               pReqXml�� ����xml
				   pDomainCode�� ��ϢĿ����
				   iNeedRsp, �Ƿ���Ҫ��Ӧ��Ϣ
    * output     : strpRsp�� ��Ӧ��Ϣ
    * return     : �ɹ�������ȷ�룬ʧ�ܷ��ش�����
    * remark     : NA
    ***********************************************************************************/
	static IVS_INT32 SetAlarmInCode4AlarmAreaReq(CXml& xml);

	/***********************************************************************************
    * name       : GetDstDomainCode
    * description: ��ȡ��ϢĿ����
    * input      : pReqXml�� ����xml
    * output     : strDomainCode�� ��ϢĿ����
    * return     : �ɹ�������ȷ�룬ʧ�ܷ��ش�����
    * remark     : NA
    ***********************************************************************************/
	static IVS_INT32 GetDstDomainCode(CXml& xml, const IVS_CHAR* pReqXml, std::string& strDomainCode);

	/***********************************************************************************
    * name       : GetAlarmSnapListGetXml
    * description: ��ȡץ��ͼƬ�б�ƴװ����xml
    * input      : ulAlarmEventID�� �¼�ID
	               pAlarmInCode���澯Դ����
				   strDomainCode�� ��ϢĿ����
    * output     : xml�� ����xml
    * return     : �ɹ�������ȷ�룬ʧ�ܷ��ش�����
    * remark     : NA
    ***********************************************************************************/
	static IVS_INT32 GetAlarmSnapListGetXml(IVS_ULONG ulAlarmEventID, const IVS_CHAR* pAlarmInCode, const std::string& strDomainCode, CXml& xml);

    /***********************************************************************************
    * name       : SetDevCode
    * description: ����豸���룬ȡ���������Ϊ��ϢĿ����, �豸��������������λ�õ�
    * input      : xml�� ����xml
    * output     : pDevDomainCode�� ��ϢĿ����
				   xml, �޸�֮���xml
    * return     : �ɹ�������ȷ�룬ʧ�ܷ��ش�����
    * remark     : NA
    ***********************************************************************************/
	static IVS_INT32 SetDevCode(CXml& xml, const IVS_CHAR* pReqXml, const std::string& strPath, IVS_CHAR* pDevDomainCode, IVS_CHAR* pDevCode);

    /***********************************************************************************
    * name       : ParseLocalDevCode
    * description: ����豸���룬
    * input      : xml�� ����xml
	               pReqXml, ����xml
				   pGrandPath, һ��·��
				   pParentPath�� ����·��
				   xml, �޸�֮���xml
    * return     : �ɹ�������ȷ�룬ʧ�ܷ��ش�����
    * remark     : NA
    ***********************************************************************************/
	static IVS_INT32 ParseLocalDevCode(CXml& xml, const IVS_CHAR* pReqXml, const IVS_CHAR* pGrandPath, const IVS_CHAR* pParentPath);

	/***********************************************************************************
    * name       : ModifyLocalDevCode
    * description: ƴװ�豸���룬
    * input      : xml�� ����xml
				   pGrandPath, һ��·��
				   pParentPath�� ����·��
				   xml, �޸�֮���xml
				   strDomainCode����LocalDevCode����ƴװ��xml
    * return     : �ɹ�������ȷ�룬ʧ�ܷ��ش�����
    * remark     : NA
    ***********************************************************************************/
	static IVS_INT32 ModifyLocalDevCode(CXml& xml, const std::string& strDomainCode, const IVS_CHAR* pGrandPath, const IVS_CHAR* pParentPath);

	/***********************************************************************************
    * name       : GetAlarmSnapshotListGetXml
    * description: ��������Ӧ��xml��
    * input      : xml�� ����xml
				   strInXml, ÿ�ν�������xml
				   iFlag, ����Ƿ��ǵ�һ�ν���������xml����һ�εĲ���������
    * output     :strOutXml, �����Ϻõ�xml
    * return     : �ɹ�������ȷ�룬ʧ�ܷ��ش�����
    * remark     : NA
    ***********************************************************************************/
	static IVS_INT32 GetAlarmSnapshotListGetXml(const std::string& strInXml, std::string& strOutXml, IVS_INT32& iFlag);
};
#endif // __ALARM_MGR_XML_PROCESS_H__
