/********************************************************************
	filename	: 	RecordXMLProcess.h
	author		:	z00193167
	created		:	2012/12/03	
	description	:	¼��XML
	copyright	:	Copyright (C) 2011-2015
	history		:	2012/12/03 ��ʼ�汾
*********************************************************************/

#ifndef __RECORD_XML_PROCESS_H__
#define __RECORD_XML_PROCESS_H__

#include "XmlProcess.h"

typedef std::vector<IVS_CAMERA_PLAN_SET_INFO> PLAN_INFO_VEC;

class CRecordXMLProcess
{
//private:
//	CRecordXMLProcess();
//public:
//	~CRecordXMLProcess();

public:
	
	/***********************************************************************************
    * name       : GetRecordSegmentXML
    * description: ��ʱ����ڼ�¼����ͳ����Ϣ������¼���ҳ����
    * input      : pTimeSpan ����ʱ������
	* input      : iHoursPerSeg ÿ��ʱ��ֶε�Сʱ����Ŀǰ�ݶ�Ϊ6
    * output     : xml ��װXML
    * return     : �ɹ�����0��ʧ�ܷ��ظ���������
    * remark     : NA
    ***********************************************************************************/
	static IVS_INT32 GetRecordSegmentXML(const IVS_TIME_SPAN* pTimeSpan,const std::string& strCamerCode,IVS_INT32 iHoursPerSeg,CXml &xml);

	/***********************************************************************************
    * name       : GetCamerTraceXML
    * description: ��ѯ������켣XML
    * input      : pTimeSpan ����ʱ������
	* input      : strCamerCode ���������
    * output     : xml ��װXML
    * return     : �ɹ�����0��ʧ�ܷ��ظ���������
    * remark     : NA
    ***********************************************************************************/
	static IVS_INT32 GetCamerTraceXML(const IVS_TIME_SPAN* pTimeSpan,const std::string& strCamerCode,CXml &xml);
	/***********************************************************************************
    * name       : ParseCamerTraceInfo
    * description: ����¼��켣�������ȡ��������ڵ�NVR��Ϣ
    * input      : xml:����XML
    * output     : nvrList NVR��Ϣ
    * return     : �ɹ�����0��ʧ�ܷ��ظ���������
    * remark     : NA
    ***********************************************************************************/
	static IVS_INT32 ParseCamerTraceInfo(CXml& xml,std::list<std::string>& nvrList);

	/***********************************************************************************
    * name       : AddRecordSpaceGetXML
    * description: ����¼��ռ�
    * input      : pRecordSpace:¼��ռ�ṹ��; pCameraCode:���������; xml:����XML
    * output     : NA
    * return     : �ɹ�����0��ʧ�ܷ��ظ���������
    * remark     : NA
    ***********************************************************************************/
	static IVS_INT32 AddRecordSpaceGetXML(const IVS_RECORD_POLICY_TIME* pRecordSpace,const IVS_CHAR* pCameraCode,CXml &xml);
	/***********************************************************************************
    * name       : GetRecordSpaceGetXML
    * description: ��ѯ�澯�����б�ƴ��xml
    * input      : pRecordSpaceInfo:¼��ռ�ṹ��; xml:����XML
    * output     : NA
    * return     : �ɹ�����0��ʧ�ܷ��ظ���������
    * remark     : NA
    ***********************************************************************************/
	static IVS_INT32 GetRecordSpaceGetXML(const IVS_RECORD_POLICY_TIME* pRecordSpaceInfo,CXml &xml);

	/***********************************************************************************
    * name       : GetRecordPolicyGetXML
    * description: ��ѯ¼�����ƴ��xml
    * input      : IVS_RECORD_POLICY_TIME:¼��ռ�ṹ��; xml:����XML
    * output     : NA
    * return     : �ɹ�����0��ʧ�ܷ��ظ���������
    * remark     : NA
    ***********************************************************************************/
	static IVS_INT32 GetRecordPolicyGetXML(CXml &xml,const IVS_CHAR* pCameraCode,const IVS_CHAR* pDomainCode);

    /********************************************************************
	name : GetRecordPolicyParseRSP
	author : w00206574
	created : 2012/12/13
	description : ��ȡ¼����Ե���ӦXML
	copyright : Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
	history : 2012/12/13 ��ʼ�汾
	*********************************************************************/
	static IVS_INT32 GetRecordPolicyParseRSP(IVS_RECORD_POLICY_TIME* pResultList, CXml& xml);

	/********************************************************************
	name : SetRecordPolicyGetXML
	author : w00206574
	created : 2012/12/13
	description : ����¼�����ƴ��xml
	copyright : Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
	history : 2012/12/13 ��ʼ�汾
	*********************************************************************/
	static IVS_INT32 SetRecordPolicyGetXML(const IVS_RECORD_POLICY_TIME* pRecordPolicy ,const IVS_CHAR* pCameraCode,const IVS_CHAR* pDomainCode,CXml&xml);

	/***********************************************************************************
    * name       : AddRecordSpacePraseXML
    * description: ����¼��ռ�
    * input      : xml:����XML;pRecordSpaceInfo:¼��ռ�ṹ��
    * output     : NA
    * return     : �ɹ�����0��ʧ�ܷ��ظ���������
    * remark     : NA
    ***********************************************************************************/
	static IVS_INT32 AddRecordSpacePraseXML(CXml xml,const IVS_RECORD_POLICY_TIME* pRecordSpaceInfo);

	/********************************************************************
	* name       : ParseGetRecordList
	* description: ¼���������Ӧ����XML
	* input      : xml:��Ӧ���xml;pRecordInfoList:¼��ƻ��б�ṹ�����; uiBufferSize:��Ӧ�������ռ��С
	* output     : NA
	* return     : �ɹ�����0��ʧ�ܷ��ظ���������
	* remark     : NA
	*********************************************************************/

	static IVS_INT32 ParseGetRecordList(CXml& xml,IVS_RECORD_INFO_LIST* pRecordInfoList,IVS_RECORD_DATA_PLACE* pRecordData,IVS_UINT32 uiBufferSize,
		    IVS_UINT32& uiRecordNum,IVS_UINT32 uiFromIndex,IVS_UINT32 uiToIndex);


	/********************************************************************
	* name       : AddOrModRecordPlanGetXML
	* description: ���/�޸�¼��ƻ�������XML
	* input      : uiDeviceNum:�����������pDeviceList���������ţ� pRecordPlan:¼��ƻ��ṹ��; xml:����XML
	* output     : NA
	* return     : �ɹ�����0��ʧ�ܷ��ظ���������
	* remark     : NA
	*********************************************************************/
    static IVS_INT32 AddOrModRecordPlanGetXML(const std::string& strSendDomainCode,DeviceCodeList& devList,
	                                          const IVS_RECORD_PLAN* pRecordPlan, CXml &xml);

    /********************************************************************
	* name       : AddOrModExDomainRecordPlanGetXML
	* description: ���/�޸�¼��ƻ�������XML
	* input      : uiDeviceNum:�����������pDeviceList���������ţ� pRecordPlan:¼��ƻ��ṹ��; xml:����XML
	* output     : NA
	* return     : �ɹ�����0��ʧ�ܷ��ظ���������
	* remark     : NA
	*********************************************************************/
    static IVS_INT32 AddOrModExDomainRecordPlanGetXML(IVS_UINT32 uiDeviceNum,const IVS_DEVICE_CODE* pDeviceList,
        const IVS_RECORD_PLAN* pRecordPlan, CXml &xml);

	/********************************************************************
	* name       : DelRecordPlanGetXML
	* description: ɾ��¼��ƻ�������XML
	* input      : uiDeviceNum:�����������pDeviceList���������ţ� uiRecordMethod:¼��ƻ���ʽ; xml:����XML
	* output     : NA
	* return     : �ɹ�����0��ʧ�ܷ��ظ���������
	* remark     : NA
	*********************************************************************/
	static IVS_INT32 DelRecordPlanGetXML(const std::string& strSendDomainCode,DeviceCodeList& devList,
		IVS_UINT32 uiRecordMethod, CXml &xml);

    /********************************************************************
	* name       : DelExDomainRecordPlanXML
	* description: ɾ��¼��ƻ�������XML
	* input      : uiDeviceNum:�����������pDeviceList���������ţ� uiRecordMethod:¼��ƻ���ʽ; xml:����XML
	* output     : NA
	* return     : �ɹ�����0��ʧ�ܷ��ظ���������
	* remark     : NA
	*********************************************************************/
    static IVS_INT32 DelExDomainRecordPlanXML(IVS_UINT32 uiDeviceNum,const IVS_DEVICE_CODE* pDeviceList,
        IVS_UINT32 uiRecordMethod, CXml &xml);
	/********************************************************************
	* name       : GetRecordPlanGetXML
	* description: ��ѯ¼��ƻ�������XML
	* input      : pCameraCode���������ţ� uiRecordMethod:¼��ƻ���ʽ; xml:����XML
	* output     : NA
	* return     : �ɹ�����0��ʧ�ܷ��ظ���������
	* remark     : NA
	*********************************************************************/
	static IVS_INT32 GetRecordPlanGetXML(const char* pCameraCode,IVS_UINT32 uiRecordMethod, CXml &xml);

	/********************************************************************
	* name       : RecordPlanParseRSP
	* description:  ���/�޸�/ɾ��¼��ƻ�����ӦXML
	* input      : pResultList���б��ѯ����ṹ�壻 uiResultBufferSize:���ؽ��Buffer��С; xml:����XML
	* output     : NA
	* return     : �ɹ�����0��ʧ�ܷ��ظ���������
	* remark     : NA
	*********************************************************************/
	static IVS_INT32 RecordPlanParseRSP(IVS_UINT32 uiDeviceNum,const IVS_DEVICE_CODE* pDeviceList,IVS_RESULT_LIST* pResultList,IVS_UINT32 uiResultBufferSize, CXml& xml,IVS_UINT32& uiRecordNum);
	/********************************************************************
	* name       : GetRecordPlanParseRSP
	* description: ��ѯ¼��ƻ�������XML
	* input      : pRecordPlan:¼��ƻ��ṹ��; xml:����XML
	* output     : NA
	* return     : �ɹ�����0��ʧ�ܷ��ظ���������
	* remark     : NA
	*********************************************************************/
    static IVS_INT32 GetRecordPlanParseRSP(IVS_RECORD_PLAN* pRecordPlan, CXml& xml);
	/********************************************************************
	* name       : GetRecordListReqXML
	* description: ¼�����������XML
	* input      : pCameraCode:�������ţ�pDomainCode�����ţ� iRecordMethod:¼��ƻ���ʽ;
	               pTimeSpan��ʱ��Ƭ�νṹ��pIndexRange����ҳ��Ϣ�� xml:����XML
	* output     : NA
	* return     : �ɹ�����0��ʧ�ܷ��ظ���������
	* remark     : NA
	*********************************************************************/
	static IVS_INT32 GetRecordListReqXML(const IVS_CHAR* pCameraCode,const IVS_CHAR* pDomainCode,IVS_INT32 iRecordMethod,const IVS_TIME_SPAN* pTimeSpan,
		const IVS_INDEX_RANGE* pIndexRange,CXml& xml);
	/********************************************************************
	* name       : GetRecordStatusReqXML
	* description: ��ѯ¼��״̬������XML
	* input      : pCameraCode���������ţ� iRecordMethod:¼��ƻ���ʽ; xml:����XML
	* output     : NA
	* return     : �ɹ�����0��ʧ�ܷ��ظ���������
	* remark     : NA
	*********************************************************************/
	static IVS_INT32 GetRecordStatusReqXML(const IVS_CHAR* pCameraCode,IVS_UINT32 iRecordMethod,CXml& xml);
	/********************************************************************
	* name       : ParseRecordStatus
	* description: ��ѯ¼��״̬����Ӧ����XML
	* input      : ulStatus:¼��״̬; xml:����XML
	* output     : NA
	* return     : �ɹ�����0��ʧ�ܷ��ظ���������
	* remark     : NA
	*********************************************************************/
	static IVS_INT32 ParseRecordStatus(CXml& xml,IVS_UINT32* ulStatus);
	/********************************************************************
	* name       : StartRemoteRecordReqXML
	* description: ��ʼԶ��¼��طŵ�����XML
	* input      : pCameraCode���������ţ� iRecordType:¼��ʽ;iRecordTime:¼��ʱ��; xml:����XML
	* output     : NA
	* return     : �ɹ�����0��ʧ�ܷ��ظ���������
	* remark     : NA
	*********************************************************************/
	static IVS_INT32 StartRemoteRecordReqXML(const IVS_UINT32 uiUserId,const IVS_CHAR* pUserDomainCode,const IVS_CHAR* pCameraCode,IVS_UINT32 iRecordType,IVS_UINT32 iRecordTime,CXml& xml);
	/********************************************************************
	* name       : StopRemoteRecordReqXML
	* description: ֹͣԶ��¼��طŵ�����XML
	* input      : pCameraCode���������ţ� iRecordType:¼��ʽ; xml:����XML
	* output     : NA
	* return     : �ɹ�����0��ʧ�ܷ��ظ���������
	* remark     : NA
	*********************************************************************/
	static IVS_INT32 StopRemoteRecordReqXML(const IVS_UINT32 uiUserId,const IVS_CHAR* pUserDomainCode,const IVS_CHAR* pCameraCode,IVS_UINT32 iRecordType,CXml& xml);

	/********************************************************************
	* name       : AddBookmarkGetXML
	* description: ���¼����ǩ������XML
	* input      : iRecordMethod��¼��ʽ��
				   uiUserId���û�ID;
				   pUserDomainCode:�û�������
				   cUserName���û�����;
				   pDomainCode:������������ţ�
				   pNVRCode��NVR��ţ�
				   pCameraCode���������ţ�
				   pBookmarkName��¼����ǩ���ƣ�
				   pBookmarkTime��¼����ǩʱ�䣻
	* output     : NA
	* return     : �ɹ�����0��ʧ�ܷ��ظ���������
	* remark     : NA
	*********************************************************************/
    static IVS_INT32 AddBookmarkGetXML(const IVS_INT32 iRecordMethod,const IVS_UINT32 uiUserId,const IVS_CHAR* pUserDomainCode,const IVS_CHAR* cUserName,const IVS_CHAR* pDomainCode,const IVS_CHAR* pNVRCode,
		const IVS_CHAR* pCameraCode,const IVS_CHAR* pBookmarkName,const IVS_CHAR* pBookmarkTime, CXml &xml);
	/********************************************************************
	* name       : ModifyBookmarkGetXML
	* description: �޸�¼����ǩ������XML
	* input      : uiUserId���û�ID;cUserName:�û�����;pCameraCode:�������ţ�pBookmarkName����ǩ���ƣ� uiBookmarkID:¼����ǩID; xml:����XML
	* output     : NA
	* return     : �ɹ�����0��ʧ�ܷ��ظ���������
	* remark     : NA
	*********************************************************************/
	static IVS_INT32 ModifyBookmarkGetXML(const IVS_INT32 iRecordMethod,const IVS_UINT32 uiUserId,const IVS_CHAR* cUserName,const IVS_CHAR* pUserDomainCode,const IVS_CHAR* pDomainCode,const IVS_CHAR* pNVRCode,
		const IVS_CHAR* pCameraCode,IVS_UINT32 uiBookmarkID,const IVS_CHAR* pNewBookmarkName, CXml &xml);
	/********************************************************************
	* name       : DeleteBookmarkGetXML
	* description: ɾ��¼����ǩ������XML
	* input      : pCameraCode:�������ţ� uiBookmarkID:¼����ǩID; xml:����XML
	* output     : NA
	* return     : �ɹ�����0��ʧ�ܷ��ظ���������
	* remark     : NA
	*********************************************************************/
	static IVS_INT32 DeleteBookmarkGetXML(const IVS_INT32 iRecordMethod,const IVS_CHAR* pDomainCode,const IVS_CHAR* pNVRCode,const IVS_CHAR* pCameraCode,IVS_UINT32 uiBookmarkID,CXml &xml);
	/********************************************************************
	* name       : ParseGetBookmarkList
	* description: ������ѯƽ̨��ǩ����ӦXML
	* input      : pRecordBookmarkInfoList��¼����ǩ��Ϣ�б� strDomainCode:�����; xml:����XML��uiBufferSize:��Ӧ�������ռ��С
	* output     : NA
	* return     : �ɹ�����0��ʧ�ܷ��ظ���������
	* remark     : NA
	*********************************************************************/
	static IVS_INT32 ParseGetBookmarkList(CXml& xml,IVS_RECORD_BOOKMARK_INFO_LIST* pRecordBookmarkInfoList,const std::string& /*strDomainCode*/,IVS_UINT32 uiBufferSize);

	/********************************************************************
	* name       : ParseGetBookmarkListForBackup
	* description: ������ѯ���ݷ�������ǩ����ӦXML
	* input      : pRecordBookmarkInfoList��¼����ǩ��Ϣ�б� strDomainCode:�����; xml:����XML��uiBufferSize:��Ӧ�������ռ��С
	* output     : NA
	* return     : �ɹ�����0��ʧ�ܷ��ظ���������
	* remark     : NA
	*********************************************************************/
	static IVS_INT32 ParseGetBookmarkListForBackup(CXml& xml,IVS_RECORD_BOOKMARK_INFO_LIST* pRecordBookmarkInfoList,const std::string& /*strDomainCode*/,IVS_UINT32 uiBufferSize,
		 IVS_UINT32& uiRecordNum,IVS_UINT32 uiFromIndex,IVS_UINT32 uiToIndex);

	/********************************************************************
	* name       : ParseManualRecordStateXML
	* description: �ֶ�¼��״̬�ı��Ľ���XML
	* input      : pManualRecordStatusNotify:�ֶ�¼��״̬��Ϣ�ṹ��; xml:����XML
	* output     : NA
	* return     : �ɹ�����0��ʧ�ܷ��ظ���������
	* remark     : NA
	*********************************************************************/
	static IVS_INT32 ParseManualRecordStateXML(CXml& xml,IVS_MANUAL_RECORD_STATUS_NOTIFY* pManualRecordStatusNotify);

	/********************************************************************
	* name       : LockRecordReqXML
	* description: ����¼�������XML
	* input      : xml:��Ӧ���xml;
	               iRecordMethod:¼��ʽ��
				   pCameraCode���������ţ�
				   pRecordLockInfo��¼�������ṹ����Ϣ
	* output     : NA
	* return     : �ɹ�����0��ʧ�ܷ��ظ���������
	* remark     : NA
	*********************************************************************/
	static IVS_INT32 LockRecordReqXML(IVS_INT32 iRecordMethod,const IVS_CHAR* pDomainCode,const IVS_CHAR* pNVRCode,const IVS_CHAR* pCameraCode,const IVS_RECORD_LOCK_INFO* pRecordLockInfo, CXml &xml);

	/********************************************************************
	* name       : UnLockRecordReqXML
	* description: ����¼�������XML
	* input      : xml:��Ӧ���xml;
	               iRecordMethod:¼��ʽ��
				   pCameraCode���������ţ�
				   pRecordLockInfo��¼�������ṹ����Ϣ
	* output     : NA
	* return     : �ɹ�����0��ʧ�ܷ��ظ���������
	* remark     : NA
	*********************************************************************/
	static IVS_INT32 UnLockRecordReqXML(IVS_INT32 iRecordMethod,const IVS_CHAR* pDomainCode,const IVS_CHAR* pNVRCode,const IVS_CHAR* pCameraCode,const IVS_RECORD_LOCK_INFO* pRecordLockInfo, CXml &xml);

	/********************************************************************
	* name       : ModLockRecordReqXML
	* description: �޸�¼��������Ϣ������XML
	* input      : xml:��Ӧ���xml;
	               iRecordMethod:¼��ʽ��
				   pCameraCode���������ţ�
				   pRecordLockInfo��¼�������ṹ����Ϣ
	* output     : NA
	* return     : �ɹ�����0��ʧ�ܷ��ظ���������
	* remark     : NA
	*********************************************************************/
	static IVS_INT32 ModLockRecordReqXML(IVS_INT32 iRecordMethod,const IVS_CHAR* pDomainCode,const IVS_CHAR* pNVRCode,const IVS_CHAR* pCameraCode,const IVS_RECORD_LOCK_INFO* pRecordLockInfo, CXml &xml);

    /******************************************************************
    function : GetCameraPlanInfoGetXML
    description: ��ѯ������ƻ�������Ϣ����xml
    input : IVS_UINT32 uiPlanType
    input : const IVS_CHAR * pDomainCode
    input : CXml & xml
    output : NA
    return : IVS_VOID
    *******************************************************************/
    static IVS_VOID GetCameraPlanInfoGetXML(IVS_UINT32 uiPlanType, const IVS_CHAR* pDomainCode, CXml &xml);

    /******************************************************************
    function : GetCameraPlanInfoParseXML
    description: ��ѯ������ƻ�������Ϣ������Ӧxml
    input : CXml & xml
    input : PLAN_INFO_VEC * cameraPlanInfoList
    output : NA
    return : IVS_INT32
    *******************************************************************/
    static IVS_INT32 GetCameraPlanInfoParseXML(CXml &xml, PLAN_INFO_VEC &cameraPlanInfoList);

    static IVS_VOID GetCameraPlanInfoGetXml(const std::map<std::string, PLAN_INFO_VEC> &planMap, CXml &xml);

	private:
	/********************************************************************
	* name       : GetCameraDomainCodeByCameraCode
	* description: ������������е�����
	* input      : uiDeviceNum�������������pDeviceList:�����ŵ��������ţ�CameraCode[64]:��������;
	* output     : NA
	* return     : �ɹ�����0��ʧ�ܷ��ظ���������
	* remark     : NA
	*********************************************************************/
	static IVS_CHAR* GetCameraDomainCodeByCameraCode(IVS_UINT32 uiDeviceNum,const IVS_DEVICE_CODE* pDeviceList,const IVS_CHAR CameraCode[64]);

};
#endif // __RECORD_XML_PROCESS_H__
