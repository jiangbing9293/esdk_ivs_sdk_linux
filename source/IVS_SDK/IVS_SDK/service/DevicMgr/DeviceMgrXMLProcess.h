/********************************************************************
filename : DevicMgrXMLProcess.h
author : gwx151720
created : 2012/12/03
description : 
copyright : Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history : 2012/12/03 ��ʼ�汾
*********************************************************************/

#ifndef __DEVICEMGR_XML_PROCESS_H__
#define __DEVICEMGR_XML_PROCESS_H__

#include "SDKDef.h"

class CDeviceMgrXMLProcess
{
public:
    /******************************************************************
    function : AddDeviceInfoGetXML
    description: ��Ⱥ�������豸��װxml
    input : const IVS_CHAR * pLoginID:��¼ID
    input : const IVS_CHAR * pDomainCode:�����
    input : IVS_UINT32 uiDeviceNum:�豸����
    input : const IVS_DEVICE_OPER_INFO * pDevList:�豸��Ϣ
    input : CXml & xml
    output : NA
    return : �ɹ�����0��ʧ�ܷ��ظ���������
    *******************************************************************/    
    static IVS_INT32 AddDeviceClusterGetXML( 
        const IVS_CHAR* pDomainCode, 
        IVS_UINT32 uiDeviceNum, 
        const IVS_DEVICE_OPER_INFO* pDevList,
		std::vector<std::string> &devPWDList,
		std::vector<std::string> &devRegPWDList,
        CXml &xml);

    /******************************************************************
    function : AddDeviceInfoPraseXML
    description: �����豸����xml
    input : CXml & xml
    input : IVS_DEVICE_OPER_RESULT_LIST * pResultList
    input : const IVS_INT32 & iLen
    output : NA
    return : �ɹ�����0��ʧ�ܷ��ظ���������
    *******************************************************************/    
    static IVS_INT32 AddDeviceClusterPraseXML(CXml &xml,
        IVS_DEVICE_OPER_RESULT_LIST* pResultList, 
        const IVS_INT32 &iLen);
    
    /******************************************************************
    function : GetDeviceListNum
    description: ��ȡ������б���Ŀ
    input : CXml & xml
    output : NA
    return : �ɹ�����0��ʧ�ܷ��ظ���������
    *******************************************************************/    
    static IVS_INT32 GetDeviceListNum(CXml &xml ,IVS_UINT32 &uiTotal);

    /******************************************************************
    function : GetMainDeviceListParseXML
    description: ��ȡ���豸�б����XML
    input : IVS_DEVICE_BRIEF_INFO_LIST * pDevInfoList
    input : IVS_UINT32 uiBufferSize
    input : CXml & Rspxml
    output : NA
    return : �ɹ�����0��ʧ�ܷ��ظ���������
    *******************************************************************/    
    static IVS_INT32 GetMainDeviceListParseXML(IVS_DEVICE_BRIEF_INFO_LIST* pDevInfoList, 
        IVS_UINT32 uiBufferSize,
        CXml &Rspxml);

    /******************************************************************
    function : DeleteDeviceGetXML
    description: ɾ���豸ƴ��XML
    input : const IVS_CHAR * pLoginID
    input : const IVS_UINT32 & uiDeviceNum
    input : const IVS_DEVICE_CODE * pDevCodeList
    input : CXml & xmlReq
    output : NA
    return : �ɹ�����0��ʧ�ܷ��ظ���������
    *******************************************************************/    
    static IVS_INT32 DeleteDeviceGetXML(
        const IVS_UINT32& uiDeviceNum,
        const IVS_DEVICE_CODE* pDevCodeList,
        CXml& xmlReq,
        const IVS_CHAR*  pDomainCode);
    /******************************************************************
    function : DeleteDeviceParseXML
    description: ɾ���豸
    input : IVS_RESULT_LIST * pResultList
    input : IVS_UINT32 uiResultBufferSize
    input : CXml & xmlRes
    output : NA
    return : �ɹ�����0��ʧ�ܷ��ظ���������
    *******************************************************************/    
    static IVS_INT32 DeleteDeviceParseXML(IVS_RESULT_LIST* pResultList,
        IVS_UINT32 uiResultBufferSize,
        CXml& xmlRes);

   /******************************************************************
   function : GetDeviceGroupXML
   description: �����ѯ�豸���б�����xml
   input : const IVS_CHAR * cLoginID
   input : const IVS_CHAR * pDomainCode
   input : const IVS_UINT32 uiGroupID
   input : const IVS_INDEX_RANGE * pIndexRange
   input : CXml & xml
   output : NA
   return : �ɹ�����0��ʧ�ܷ��ظ���������
   *******************************************************************/   
   static IVS_INT32 GetDeviceGroupXML(IVS_INT32 iType,
        const IVS_CHAR* pDomainCode,
        const IVS_CHAR* pDevGroupCode,
        CXml& xml);

   /******************************************************************
   function : GetDeviceGroupParseXML
   description: ������ѯ�豸���б���Ӧxml
   input : IVS_DEV_GROUP_LIST * pDeviceGroupList
   input : const IVS_UINT32 uiBufferSize
   input : CXml & xml
   output : NA
   return : �ɹ�����0��ʧ�ܷ��ظ���������
   *******************************************************************/   
   static IVS_INT32 GetDeviceGroupParseXML(IVS_UINT32 &uiSize, IVS_DEVICE_GROUP_LIST* pDeviceGroupList, 
       const IVS_UINT32 uiBufferSize, 
       CXml& xml);

   /******************************************************************
   function : DeleteDeviceGroupGetXML
   description: ����ɾ���豸������xml
   input : const IVS_CHAR * cLoginID
   input : IVS_UINT32 uiGroupNum
   input : const IVS_DEVICE_GROUP * pDeviceGroupList
   input : CXml & xmlReq
   output : NA
   return : �ɹ�����0��ʧ�ܷ��ظ���������
   *******************************************************************/   
   static IVS_INT32  DeleteDeviceGroupGetXML(
                                             const IVS_CHAR* pDomainCode,
                                             IVS_UINT32 uiDevGroupID,
                                             CXml& xmlReq);

   /******************************************************************
   function : GetDeviceInfoParseXML
   description: ��ȡ�豸��ϸ��Ϣ����XML
   input : IVS_DEVICE_INFO * pDeviceInfo
   input : CXml & xml
   output : NA
   return : �ɹ�����0��ʧ�ܷ��ظ���������
   *******************************************************************/   
   static IVS_INT32 GetDeviceInfoParseXML(IVS_DEVICE_CFG* pDeviceInfo, CXml& xml);

    /******************************************************************
    function : GetDeviceInfoXML
    description: ��ѯ�豸��ϸ��Ϣƴ��XML
    input : const IVS_CHAR * cLoginID
    input : const IVS_CHAR * pDomainCode
    input : const IVS_CHAR * pDevCode
    input : CXml & xml
    output : NA
    return : �ɹ�����0��ʧ�ܷ��ظ���������
    *******************************************************************/    
    static IVS_INT32 GetDeviceInfoXML(
        const IVS_CHAR* pDomainCode,
        const IVS_CHAR* pDevCode,
        CXml& xml);

    /******************************************************************
    function : ModifyDeviceGroupGetXML
    description: �޸��豸��(����)ƴ��XML
    input : const IVS_CHAR * pLoginID
    input : const IVS_DEV_GROUP * pDevGroupInfo
    input : CXml & xml
    output : NA
    return : �ɹ�����0��ʧ�ܷ��ظ���������
    *******************************************************************/    
    static IVS_INT32 ModifyDeviceGroupGetXML(
                                             const IVS_CHAR* pDomainCode,
                                             IVS_UINT32 uiDevGroupID,
                                             const IVS_CHAR* pNewDevGroupName ,
                                             CXml &xml);

    /******************************************************************
    function : MoveDeviceGroupGetXML
    description: �ƶ��豸��ƴ��XML
    input : const IVS_CHAR * pLoginID
    input : const IVS_CHAR * pTargetDomainCode
    input : IVS_UINT32 uiTargetDevGroupID
    input : const IVS_DEV_GROUP_LIST * pDeviceGroupList
    input : CXml & xml
    output : NA
    return : �ɹ�����0��ʧ�ܷ��ظ���������
    *******************************************************************/    
    static IVS_INT32 MoveDeviceGroupGetXML(
                                            const IVS_CHAR* pDomainCode,
                                            IVS_UINT32 uiNewParentDevGroupID,
                                            IVS_UINT32 uiDevGroupID,
                                            CXml& xml);

    /******************************************************************
    function : AddDeviceGroupGetXML
    description: �����豸��ƴ��XML
    input : const IVS_CHAR * cLoginID
    input : const IVS_DEV_GROUP * pGroupInfo
    input : CXml & xml
    output : NA
    return : �ɹ�����0��ʧ�ܷ��ظ���������
    *******************************************************************/    
    static IVS_INT32 AddDeviceGroupGetXML(
        const IVS_DEV_GROUP* pGroupInfo,
        CXml &xml);
	/******************************************************************
    function : AddDeviceGroupParseXML
    description: �����豸�����XML
    input : CXml & xml
    input : IVS_UINT32 * uiDevGroupID
    output : NA
    return : �ɹ�����0��ʧ�ܷ��ظ���������
    *******************************************************************/    
    static IVS_INT32 AddDeviceGroupParseXML(CXml &xml, 
        IVS_UINT32* uiDevGroupID);

    /******************************************************************
    function : DeleteDeviceFromGroupReqGetXML
    description: ɾ���豸��������ƴ��XML
    input : const IVS_CHAR * pLoginID
    input : const IVS_DEV_GROUP_CAMERA_LIST * pDeviceGroupList
    input : CXml & xml
    output : NA
    return : �ɹ�����0��ʧ�ܷ��ظ���������
    *******************************************************************/    
    static IVS_INT32 DeleteDeviceFromGroupGetXML(
                                                 const IVS_CHAR* pTargetDomainCode,
                                                 IVS_UINT32 uiDeviceNum,
                                                 const IVS_DEV_GROUP_CAMERA* pDeviceGroupList,
                                                 CXml& xml);

    /*****************************************************************
         function   : AddDeviceToGroupGetXML
         description: �����豸�������ƴ��XML
         output     : const IVS_CHAR * pLoginID
         output     : const IVS_CHAR * pTargetDomainCode
         output     : IVS_UINT32 uiTargetDevGroupID
         output     : IVS_UINT32 uiDeviceNum
         output     : const IVS_DEV_GROUP_CAMERA * pDeviceGroupList
         output     : CXml & xml
         return     : 
    *****************************************************************/
    static IVS_INT32 AddDeviceToGroupGetXML(
                                            const IVS_CHAR* pTargetDomainCode,
                                            IVS_UINT32 uiTargetDevGroupID,
                                            IVS_UINT32 uiDeviceNum,
                                            const IVS_DEV_GROUP_CAMERA* pDeviceGroupList,
                                            CXml& xml);

    /******************************************************************
    function : GetNVRListReqXML
    description: ��ȡ��Ԫ�б����ƴ��XML
    input : const IVS_CHAR * pLoginID
    input : const IVS_CHAR * pDomainCode
    input : const IVS_INDEX_RANGE * pIndex
    input : IVS_UINT32 uiServerType
    input : CXml & xml
    output : NA
    return : �ɹ�����0��ʧ�ܷ��ظ���������
    *******************************************************************/
    static IVS_INT32 GetNVRListReqXML(
        const IVS_CHAR* pDomainCode,
        const IVS_INDEX_RANGE* pIndex,
        IVS_UINT32 uiServerType,
        CXml &xml);

    /******************************************************************
    function : PraseNVRListRsp
    description: ��ȡ��Ԫ�б����XML
    input : CXml & xml
    input : IVS_DEVICE_BRIEF_INFO_LIST * pNvrList
    input : IVS_UINT32 uiBufferSize
    output : NA
    return : �ɹ�����0��ʧ�ܷ��ظ���������
    *******************************************************************/
    static IVS_INT32 ParseNVRListRsp(CXml& xml,
        IVS_DEVICE_BRIEF_INFO_LIST* pNvrList,
        IVS_UINT32 uiBufferSize);


    /******************************************************************
    function : GetCameraStreamParaGetXML
    description: ��ȡ�������������ƴ��XML
    input : const IVS_CHAR * pLoginID
    input : const IVS_CHAR * pDomainCode
    input : const IVS_CHAR * pCode
    input : CXml & xml
    output : NA
    return : �ɹ�����0��ʧ�ܷ��ظ���������
    *******************************************************************/
    static IVS_INT32 GetCameraStreamParaGetXML( 
        const IVS_CHAR* pDomainCode, 
        const IVS_CHAR* pCode, 
        CXml &xml);

    /******************************************************************
    function : GetDeviceState
    description: ��ȡ�豸״̬
    input : CXml & xml
    input : IVS_DEVICE_STATE & stDeviceState
    output : NA
    return : �ɹ�����0��ʧ�ܷ��ظ���������
    *******************************************************************/
    static IVS_INT32 GetDeviceState(CXml& xml, 
        IVS_DEVICE_STATE& stDeviceState);     

    /*****************************************************************
         function   : GetChannelListGetXML
         description: �������豸��ѯ���豸ƴ��XML
         output     : const IVS_CHAR * pLoginID
         output     : const IVS_CHAR * pDomainCode
         output     : const IVS_CHAR * pCode
         output     : CXml & xml
         return     : 
    *****************************************************************/
    static IVS_INT32 GetChannelListGetXML(
                                          const IVS_CHAR* pDomainCode, 
                                          const IVS_CHAR* pCode, 
                                          CXml &xml );

    /*****************************************************************
         function   : GetChannelListParseXML
         description: �������豸��ѯ���豸����XML
         output     : CXml & xml
         output     : IVS_DEV_CHANNEL_BRIEF_INFO_LIST * pChannelList
         return     : 
    *****************************************************************/
    static IVS_INT32 GetChannelListParseXML(CXml &xml ,
                                            IVS_DEV_CHANNEL_BRIEF_INFO_LIST* pChannelList);

	 /******************************************************************
    function : SetMainDeviceNameGetXML
    description: �޸����豸����ƴ��XML
    input : const IVS_CHAR * pLoginID
    input : const IVS_CHAR * pDevCode
    input : const IVS_CHAR * pNewDevName
    input : CXml & xml
    output : NA
    return : �ɹ�����0��ʧ�ܷ��ظ���������
    *******************************************************************/
	static IVS_INT32 SetMainDeviceNameGetXML(
											 const IVS_CHAR* pDomainCode,
											 const IVS_CHAR* pDevCode,
											 const IVS_CHAR* pNewDevName,
											 CXml &xml);

	/******************************************************************
	function : GetDomainList
	description: "ȡ���û�ӵ���豸�����б�"����xml
	author: fengjiaji f00152768
	input : const IVS_UINT32 &uiUserID
	output : CXml &xml
	return : �ɹ�����0��ʧ�ܷ��ظ���������
	*******************************************************************/
	static IVS_INT32 GetDomainListGetXml(
		                           const IVS_UINT32 &uiUserID,
								   CXml &xml);

	/******************************************************************
	function : GetDeviceListGetXml
	description:��ȡ�豸�б�ʱ��װ����xml
	author: fengjiaji f00152768
	input : const IVS_CHAR * pLoginID
	input : const IVS_CHAR * pDomainCode
	input : const IVS_UINT32 & uiDevType
	input : const IVS_INDEX_RANGE & stRange
	output : CXml & xml
	return : �ɹ�����0��ʧ�ܷ��ظ���������
	*******************************************************************/
	static IVS_INT32 GetDeviceListGetXml(IVS_INT32 iType,
									const IVS_CHAR* pDomainCode,
									const IVS_UINT32 &uiDevType,
									const IVS_INDEX_RANGE &stRange,
									CXml &xml);

	/******************************************************************
	function : GetDomainListParseXml
	description:���� "ȡ���û�ӵ���豸�����б�"����Ӧ
	author: fengjiaji f00152768
	input : CXml & xml
	output : std::list<std::string> & dmList
	return : �ɹ�����0��ʧ�ܷ��ظ���������
	*******************************************************************/
	static IVS_INT32 GetDomainListParseXml(CXml &xml,std::vector<std::string> &dmList);

	/******************************************************************
	function : GetCamerasParseXml
	description: ��ȡ������б�����Ӧxml����
	author: fengjiaji f00152768
	input : const IVS_CHAR * pDomainCode
	input : CXml & xml
	output : std::vector<IVS_CAMERA_BRIEF_INFO> * pCameraVec
	output : IVS_UINT32 & uiSize
	output : IVS_INDEX_RANGE & outRange
	return : �ɹ�����0��ʧ�ܷ��ظ���������
	*******************************************************************/
	static IVS_INT32 GetCamerasParseXml(IVS_INT32 iType, const IVS_CHAR* pDomainCode,
										CXml &xml,
										std::vector<IVS_CAMERA_BRIEF_INFO>* pCameraVec,
										IVS_UINT32 &uiSize,
										IVS_INDEX_RANGE& outRange);

    static IVS_INT32 ParseVideoChannelList(const IVS_CHAR* pDomainCode, 
                                       const IVS_CHAR* pData, 
                                       const IVS_INT32 iExDomainFlag,
                                       IVS_PAGE_INFO& pageInfo, 
                                       std::vector<IVS_CAMERA_BRIEF_INFO>& videoChannelVector);

    static IVS_INT32 ParseAlarmChannelList(const IVS_CHAR* pDomainCode, 
                                    const IVS_CHAR* pData, 
                                    const IVS_INT32 iExDomainFlag,
                                    IVS_PAGE_INFO& pageInfo, 
                                    std::vector<IVS_ALARM_BRIEF_INFO>& alarmChannelVector);

	/******************************************************************
	function : GetAlarmsParseXml
	description: ��ȡ�澯�б�����Ӧxml����
	author: fengjiaji f00152768
	input : const IVS_CHAR * pDomainCode
	input : CXml & xml
	output : std::vector<IVS_ALARM_BRIEF_INFO> * pAlarmVec
	output : IVS_UINT32 & uiSize
	output : IVS_INDEX_RANGE & outRange
	return : �ɹ�����0��ʧ�ܷ��ش�����
	*******************************************************************/
	static IVS_INT32 GetAlarmsParseXml(const IVS_CHAR* pDomainCode,
										CXml &xml,
										std::vector<IVS_ALARM_BRIEF_INFO>* pAlarmVec,
										IVS_UINT32 &uiSize,
										IVS_INDEX_RANGE& outRange);

	/******************************************************************
	function : GetAudiosParseXml
	description: ��ȡ�����豸�б�����Ӧxml����
	author: fengjiaji f00152768
	input : const IVS_CHAR * pDomainCode
	input : CXml & xml
	input : std::vector<IVS_AUDIO_BRIEF_INFO> * pAudioVec
	input : IVS_UINT32 & uiSize
	input : IVS_INDEX_RANGE & outRange
	output : NA
	return : �ɹ�����0��ʧ�ܷ��ظ���������
	*******************************************************************/
	static IVS_INT32 GetAudiosParseXml(const IVS_CHAR* pDomainCode,
										CXml &xml,
										std::vector<IVS_AUDIO_BRIEF_INFO>* pAudioVec,
										IVS_UINT32 &uiSize,
										IVS_INDEX_RANGE& outRange);

	/******************************************************************
    function : SetMainDeviceNameGetXML
    description: �޸����������ƴ��XML
    input : const IVS_CHAR * pLoginID
    input : const IVS_CHAR * pCameraCode
    input : const IVS_CHAR * pNewDevName
    input : CXml & xml
    output : NA
    return : �ɹ�����0��ʧ�ܷ��ظ���������
    *******************************************************************/
	static IVS_INT32 SetCameraNameGetXML(
										 const IVS_CHAR* pDomainCode,
										 const IVS_CHAR* pCameraCode,
										 const IVS_CHAR* pNewCameraName,
										 CXml &xml);


	/******************************************************************
    function : RestartDeviceGetXML
    description: ����ǰ���豸ƴ��XML
    input : const IVS_CHAR * pLoginID
    input : const IVS_CHAR * pDomainCode
    input : const IVS_CHAR * pDeviceCode
    input : CXml & xml
    output : NA
    return : �ɹ�����0��ʧ�ܷ��ظ���������
    *******************************************************************/
	static IVS_INT32 RestartDeviceGetXML(
										 const IVS_CHAR* pDomainCode,
										 const IVS_CHAR* pDeviceCode,
										 IVS_UINT32 uiRebootType,
										 CXml &xml);

	/***********************************************************************************
    * function: GetRecordTaskGetXML
	* description: ƴװ��ѯ��ǰ�û����ֶ�¼�����������xml
	* input      : pCameraCode ���������
	* output     : xml  ƽ̨�ֶ�ץ�ĵ�xml
	* return     : �ɹ�����0��ʧ�ܷ��ش�����;
    ***********************************************************************************/
	static IVS_INT32 GetRecordTaskGetXML(const IVS_UINT32 uiUserId,const IVS_CHAR* pUserDomainCode,const IVS_CHAR* pCameraCode,CXml &xml);

	/***********************************************************************************
    * function:    SearchDeviceParseXML
	* description: ��������ǰ���豸����XML
	* input      : xml  ��������ǰ���豸XML
	* output     : NA
	* return     : �ɹ�����0��ʧ�ܷ��ش�����;
    ***********************************************************************************/
	static IVS_INT32 SearchDeviceParseXML(CXml &xml, const IVS_CHAR* pAddress);

	/***********************************************************************************
    * function:    StartSearchDeviceParseXML
	* description: ��������ǰ���豸����XML
	* input      : xml  ��������ǰ���豸XML
	* output     : NA
	* return     : �ɹ�����0��ʧ�ܷ��ش�����;
    ***********************************************************************************/
	static IVS_INT32 ModifyServerCode(CXml &xml, const std::string& strServerCode, const IVS_CHAR* pAddress);

	/***********************************************************************************
    * function:    FindIdlestNvrParseXML
	* description: ��������ǰ���豸����XML
	* input      : xml  ��������ǰ���豸XML
	* output     : NA
	* return     : �ɹ�����0��ʧ�ܷ��ش�����;
    ***********************************************************************************/
	static IVS_INT32 FindIdlestNvrParseXML(std::string& strServerCode, CXml &xml);

	/***********************************************************************************
    * function:    SetDevGroupRemarkGetXML
	* description: �����豸�鱸ע��Ϣƴ��XML
	* input      : pDomainCode�������
				   uiDevGroupID���豸��ID
				   pDevGroupRemark����ע��Ϣ
				   xml��ƴ��XML
	* output     : NA
	* return     : �ɹ�����0��ʧ�ܷ��ش�����;
    ***********************************************************************************/
	static IVS_INT32 SetDevGroupRemarkGetXML(const IVS_CHAR* pDomainCode, IVS_UINT32 uiDevGroupID, const IVS_CHAR* pDevGroupRemark, CXml &xml);

	/***********************************************************************************
    * function:    GetDevGroupRemarkGetXML
	* description: ��ѯ�豸�鱸ע��Ϣƴ��XML
	* input      : pDomainCode�������
				   uiDevGroupID���豸��ID
				   xml��ƴ��XML
	* output     : NA
	* return     : �ɹ�����0��ʧ�ܷ��ش�����;
    ***********************************************************************************/
	static IVS_INT32 GetDevGroupRemarkGetXML(const IVS_CHAR* pDomainCode, IVS_UINT32 uiDevGroupID, CXml &xml);

	/***********************************************************************************
    * function:    GetDevGroupRemarkParseXML
	* description: ��ѯ�豸�鱸ע��Ϣƴ��XML
	* input      : pDevGroupRemark����ע��Ϣ
				   xml��ƴ��XML
				   uiRemarkLen: ��ע�ַ����ȣ����324�ֽڣ�
	* output     : NA
	* return     : �ɹ�����0��ʧ�ܷ��ش�����;
    ***********************************************************************************/
	static IVS_INT32 GetDevGroupRemarkParseXML(IVS_CHAR* pDevGroupRemark, CXml &xml, IVS_UINT32 uiRemarkLen);

	/***********************************************************************************
    * function:    GetDeviceCapabilityGetXML
	* description: ��ѯ�������������ƴ��XML
	* input      : pCameraCode�����������
				   pDomainCode�������
				   xml        : ƴ��XML
	* output     : NA
	* return     : �ɹ�����0��ʧ�ܷ��ش�����;
    ***********************************************************************************/
	static IVS_INT32 GetDeviceCapabilityGetXML(const IVS_CHAR* pCameraCode, const IVS_CHAR* pDomainCode, CXml &xml);

	/***********************************************************************************
    * function:    ImportDevCfgFileGetXML
	* description: ����ǰ�������ļ�
	* input      : pDevCode��   �豸����
				   pDomainCode�������
				   pCFGFile   �������ļ�ȫ·��
				   xml        : ƴ��XML
	* output     : NA
	* return     : �ɹ�����0��ʧ�ܷ��ش�����;
    ***********************************************************************************/
	static IVS_INT32 ImportDevCfgFileGetXML(const IVS_CHAR* pDomainCode, const IVS_CHAR* pDevCode, const IVS_CHAR* pCFGFile, CXml &xml);

	/***********************************************************************************
    * function:    ExportDevCfgFileGetXML
	* description: ����ǰ���豸����
	* input      : pDevCode��   �豸����
				   pDomainCode�������
				   pDevUserName:�豸��¼�û���
				   strDevPWD  : �豸��¼����
				   xml        : ƴ��XML
	* output     : NA
	* return     : �ɹ�����0��ʧ�ܷ��ش�����;
    ***********************************************************************************/
	static IVS_INT32 SetDevPWDGetXML(const IVS_CHAR* pDomainCode, const IVS_CHAR* pDevCode, const IVS_CHAR* pDevUserName, const std::string &strDevPWD, const std::string &strDevRegPWD, CXml &xml);

	// �ѵ������豸ƴ��XML
	static IVS_INT32 AddDevicePileGetXML(const IVS_CHAR* pDomainCode, const IVS_DEVICE_OPER_INFO* pDevList, const std::string &strDevPWD, const std::string &strDevRegPWD, IVS_UINT32 uiNum, CXml &xml);

	static IVS_INT32 AddDevicePileParseXML(IVS_DEVICE_OPER_RESULT_LIST* pResultList, IVS_UINT32 uiNum, CXml &xml);

    // ˢ���豸�б�
    static IVS_INT32 RefreshCameraListXML(IVS_INT32 iSessionID, CXml &xml);

    //�������ص�Onvif�豸�������XML
    static IVS_INT32 ParseCameraEncodeCapabilities(CXml& xml,
	    IVS_STREAM_INFO_LIST* pStreameList);

	/******************************************************************
   function		: GetVideoDiagnoseGetXML
   description: �����ѯ��̨VCN����Ƶ������ϼƻ�����xml
   input			: pDomainCode
   input			: pCameraCode
   output			: xml
   return			: �ɹ�����0��ʧ�ܷ��ظ���������
   *******************************************************************/   
	static IVS_INT32 GetVideoDiagnoseGetXML(const IVS_CHAR* pDomainCode,
		const IVS_CHAR* pCameraCode,
        CXml& xml);

    ~CDeviceMgrXMLProcess();
protected:
private:
    CDeviceMgrXMLProcess();
    

};


#endif
