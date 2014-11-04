/********************************************************************
filename	: 	IVSSDK.cpp
author		:	z00201790
created		:	2012/10/23	
description	:	���� ��������;
copyright	:	Copyright (C) 2011-2015
history		:	2012/10/23 ��ʼ�汾;
				2013/09/06 ɾ��Linux�����ŵĽӿڣ��������ŵĽӿ� f00152768
				2013/10/15 ����Ԥ��λ��ص��ĸ��ӿ� z00233055
*********************************************************************/

#ifndef IVS_SDK_H
#define IVS_SDK_H

#include "hwsdk.h"
#include "ivs_error.h"
//#include "SDKDef.h"

#ifdef WIN32
#ifdef  IVS_SDK_EXPORTS
#define IVS_SDK_API __declspec(dllexport)
#else
#define IVS_SDK_API __declspec(dllimport)
#endif
#else
//#define IVS_SDK_API __attribute__((visibility))
#define IVS_SDK_API __attribute__((visibility("default")))
#endif

#ifdef WIN32
#define __SDK_CALL __stdcall
#else
#define __SDK_CALL
#endif

#ifdef __cplusplus
extern "C"
{
#endif

	/**************************************************************************
	* name       : IVS_SDK_Init
	* description: ��ʼ��SDK
	* input      : NA
	* output     : NA
	* return     : �ɹ�����0��IVS_SUCCEED����ʧ�ܷ��ش�����
	* remark     : NA
	**************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_Init();

	/**************************************************************************
	* name       : IVS_SDK_Cleanup()
	* description: �ͷ�SDK
	* input      : NA
	* output     : NA
	* return     : �ɹ�����0��IVS_SUCCEED����ʧ�ܷ��ش�����
	* remark     : NA
	**************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_Cleanup();

	/**************************************************************************
	* name       : IVS_SDK_GetVersion()
	* description: ��ȡSDK�汾�ţ���ǰ�汾��Ϊ2.2.0.1
	* input      : NA
	* output     : NA
	* return     : �ɹ�����SDK�汾�ţ���16λ��25λ~32λ��ʾ���汾�ţ�17~24λ��ʾ�Ӱ汾�š���16λ��9λ~16λ��ʾ�����汾�ţ�1~8λ��ʾԤ���汾��;
	* remark     : ����SDK�汾����16���Ʊ�ʾ������2.2.0.1����ֵΪ0x02020001
	**************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetVersion();

	/**************************************************************************
	* name       : IVS_SDK_SetEventCallBack()
	* description: �����¼��ص�����
	* input      : iSessionID        IVS_SDK_Login���صĻỰID	
	*              fEventCallBack    �û����õ��¼��ص�������Ϊ�ձ�ʾȡ�����õĻص�����;
	*              pUserData		 �û����ݣ��Զ��壬��Ӧ�ص�����������ʱ���ظò���;
	* output     : NA
	* return     : �ɹ�����0��IVS_SUCCEED����ʧ�ܷ��ش�����
	* remark     : NA
	**************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_SetEventCallBack(IVS_INT32 iSessionID, 
		EventCallBack fEventCallBack = NULL, 
		void *pUserData = NULL);

	/**************************************************************************
	* name       : IVS_SDK_Login
	* description: �û���¼
	* input      : IVS_LOGIN_INFO ��½��νṹ��
                       �� pUserName �û���
	                    pPassword ����
	                    pIP       ������IP��ַ
	                    iPort     �������˿�
	                    loginType ��½����	��		0 �C���������֤ 	1 �CWindows�������֤����ǰ�û� 	2 �CWindows�������֤�������û�
	                    pDomainName   ��¼������Ϣ
	                    pMachineName �ͻ��˵�¼�Ļ�������
    * output     : �ỰID��SessionID��SDK�ڲ�������0-127����������Ϊ�����ӿڵ���Σ�������ʶ�û���һ�ε�¼�����֧�ֵĵ�¼��Ϊ128
	* return     : �ɹ�����0��IVS_SUCCEED����ʧ�ܷ��ش�����
	* remark     : NA
	**************************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_Login(IVS_LOGIN_INFO* pLoginReqInfo, IVS_INT32* pSessionId);
	
	
	/**************************************************************************
	* name       : IVS_SDK_LoginByTicket
	* description: �û������¼;
	* input      : IVS_LOGIN_INFO_EX ��½��νṹ��;
					(cTicketID		�û���Ʊ�ݣ����û�Ψһ��Ӧ;
					stIP			������IP��ַ;
					uiPort			�������˿�;
					uiLoginType		��½����: 0 �C���������֤;
					cDomainName		��¼������Ϣ;
					cMachineName	�ͻ��˵�¼�Ļ�����;
					uiClientType	�ͻ������ͣ��ο� IVS_CLIENT_TYPE 0-PC�ͻ���, 1-web�����, 
						2-�ƶ��ͻ���, 3-PC�ͻ����ӽ��̣�4-����ǽ����;);
    * output     : pSessionId  ����sessionId SessionID��SDK�ڲ�����������0��������
                        ��Ϊ�����ӿڵ���Σ��������ַ��ʵķ�����������¼128��������;
	* return     : �ɹ�����0��ʧ�ܷ��ش�����;
	* remark     : NA
	**************************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_LoginByTicket(IVS_LOGIN_INFO_EX* pLoginReqInfoEx, IVS_INT32* pSessionId);


	/**************************************************************************
	* name       : IVS_SDK_Logout
	* description: �û�ע��
	* input      : iSessionID ��¼�ɹ���ĻỰID
	* output     : NA
	* return     : �ɹ�����0��IVS_SUCCEED����ʧ�ܷ��ش�����
	* remark     : NA
	**************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_Logout(IVS_INT32 iSessionID);

	/******************************************************************
	function : IVS_SDK_GetDomainRoute
	description: ��ѯ��·����Ϣ
	input : iSessionID       ��¼�ɹ���ĻỰID
			uiBufferSize     �����С
	output : pDomainRouteList ·����Ϣ�б�
	return : �ɹ�����0��IVS_SUCCEED����ʧ�ܷ��ش�����
	*******************************************************************/  
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetDomainRoute( IVS_INT32 iSessionID, 
		IVS_DOMAIN_ROUTE_LIST* pDomainRouteList,
		IVS_UINT32 uiBufferSize);

	/**************************************************************************
    * name			: IVS_SDK_GetNVRList
    * description	: ��ѯ��Ԫ�б�
    * input			: iSessionID        ��¼�ɹ���ĻỰID
						  pDomainCode       �����
						  uiServerType      ��������
						  pIndexRange   ��ҳ��Ϣ
						  uiBufferSize      �ṹָ���С
    * output		: pNvrList        ��Ԫ�����б�ṹָ��
    * return		: �ɹ�����0��IVS_SUCCEED����ʧ�ܷ��ش�����
    * remark		: NA
    **************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetNVRList(IVS_INT32 iSessionID, 
		                                                const IVS_CHAR* pDomainCode,
		                                                IVS_UINT32 uiServerType,
		                                                const IVS_INDEX_RANGE* pIndexRange,
		                                                IVS_DEVICE_BRIEF_INFO_LIST* pNvrList,
		                                                IVS_UINT32 uiBufferSize);

	/*************************************************************************
	* name			: IVS_SDK_AddDeviceGroup
	* description	: �����豸��
	* input			: iSessionID ��¼�ɹ���ĻỰID
						  pDevGroup �豸����Ϣ
	* output			: puiDevGroupID �豸��ID
	* return			: �ɹ�����0��IVS_SUCCEED����ʧ�ܷ��ش�����
	* remark			: NA
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_AddDeviceGroup (IVS_INT32 iSessionID, const IVS_DEV_GROUP* pDevGroup,IVS_UINT32* puiDevGroupID);

	/*************************************************************************
    * name			: IVS_SDK_DeleteDeviceGroup
    * description	: ɾ���豸��
    * input			: iSessionID ��¼�ɹ���ĻỰID
						  pDomainCode ɾ���豸�������������
						  uiDevGroupID ɾ�����豸��ID
    * output			: NA
    * return			: �ɹ�����0��IVS_SUCCEED����ʧ�ܷ��ش�����
    * remark			: NA
    *************************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_DeleteDeviceGroup( IVS_INT32 iSessionID, const IVS_CHAR* pDomainCode, IVS_UINT32 uiDevGroupID);

	/**************************************************************************
    * name			: IVS_SDK_ModifyDeviceGroupName
    * description	: �޸��豸������
    * input			: iSessionID  ��¼�ɹ���ĻỰID
						  pDomainCode  �豸�����������
						  uiDevGroupID  �豸��ID
						  pNewDevGroupName  �豸��������
    * return			: �ɹ�����0��IVS_SUCCEED����ʧ�ܷ��ش�����
    * remark			: NA
    **************************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_ModifyDeviceGroupName(IVS_INT32 iSessionID,  const IVS_CHAR* pDomainCode, IVS_UINT32 uiDevGroupID, const IVS_CHAR* pNewDevGroupName);

	/*************************************************************************
	* name			: IVS_SDK_MoveDeviceGroup
	* description	: �ƶ��豸��
	* input			: iSessionID ��¼�ɹ���ĻỰID
						  pDomainCode  �豸�����������
						  uiNewParentDevGroupID  �豸����¸���ID
						  uiDevGroupID  �豸��ID
	* output			: NA
	* return			: �ɹ�����0��IVS_SUCCEED����ʧ�ܷ��ش�����
	* remark			: NA
	*************************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_MoveDeviceGroup(IVS_INT32 iSessionID,  const IVS_CHAR* pDomainCode, IVS_UINT32 uiNewParentDevGroupID, IVS_UINT32 uiDevGroupID);

	/*************************************************************************
	* name       : IVS_SDK_GetDeviceGroup
	* description: ��ѯ�豸���б�
	* input      : iSessionID    ��¼�ɹ���ĻỰID
						  pDomainCode   �豸�����������
						  pDevGroupCode �豸��ID
						  uiBufferSize  �����С
	* output     : pDeviceGroupList �豸���б�
	* return     : �ɹ�����0��IVS_SUCCEED����ʧ�ܷ��ش�����
	* remark     : NA
	*************************************************************************/		
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetDeviceGroup(IVS_INT32 iSessionID,
		const IVS_CHAR* pDomainCode,
		const IVS_CHAR* pDevGroupCode,
		IVS_DEVICE_GROUP_LIST* pDeviceGroupList,
		IVS_UINT32 uiBufferSize);

	/******************************************************************
    function		: IVS_SDK_AddDeviceToGroup
    description	: �����豸�����豸
    input			: iSessionID  ��¼�ɹ���ĻỰID
					  pTargetDomainCode  Ŀ���豸�������
					  uiTargetDevGroupID  Ŀ���豸��ID
					  uiDeviceNum  �����豸����
					  pGroupCameraList  �����豸��Ϣ����ָ��
    output		: NA
    return		: �ɹ�����0��IVS_SUCCEED����ʧ�ܷ��ش�����
    *******************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_AddDeviceToGroup( IVS_INT32 iSessionID, 
                                        const IVS_CHAR* pTargetDomainCode,
                                        IVS_UINT32 uiTargetDevGroupID,
                                        IVS_UINT32 uiDeviceNum,
                                        const IVS_DEV_GROUP_CAMERA* pGroupCameraList);

	/******************************************************************
    function		: IVS_SDK_DeleteDeviceFromGroup
    description	: ɾ���豸�����豸
    input			: iSessionID  ��¼�ɹ���ĻỰID
					  pTargetDomainCode  Ŀ���豸�������
					  uiDeviceNum  ɾ���豸����
					  pGroupCameraList  ɾ���豸��Ϣ����ָ��
    output		: NA
    return		: �ɹ�����0��IVS_SUCCEED����ʧ�ܷ��ش�����
    *******************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_DeleteDeviceFromGroup( IVS_INT32 iSessionID, 
                                                                    const IVS_CHAR* pTargetDomainCode,
                                                                    IVS_UINT32 uiDeviceNum,
                                                                    const IVS_DEV_GROUP_CAMERA* pGroupCameraList);
	  
	/******************************************************************
	* name			: IVS_SDK_GetDeviceList
	* description	: ��ȡ�豸�б�
	* input			: iSessionID   ��¼�ɹ���ĻỰID
						  uiDeviceType �豸���ͣ�ȡֵ�ο�IVS_DEVICE_TYPE��Ŀǰ֧�ֻ�ȡ����3���豸�б�
                                2-������豸
                                3-�����豸
                                4-�澯�豸
						  pIndexRange  ��ҳ��Ϣ
						  uiBufferSize �����С
	* output		: pDeviceList  �豸�б�ָ�룬����˵�����£�
                                ������豸��IVS_CAMERA_BRIEF_INFO_LIST
                                �����豸��IVS_AUDIO_BRIEF_INFO_LIST
                                �澯�豸��IVS_ALARM_BRIEF_INFO_LIST
	* return		: �ɹ�����0��IVS_SUCCEED����ʧ�ܷ��ش�����
	* remark		: NA
	*******************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetDeviceList(
		IVS_INT32 iSessionID,
		IVS_UINT32 uiDeviceType,
		const IVS_INDEX_RANGE* pIndexRange,
		IVS_VOID* pDeviceList,
		IVS_UINT32 uiBufferSize);


	/**************************************************************************
    * name			: IVS_SDK_StartRealPlayCBRaw
    * description	: ��ʼʵʱ������Իص���ʽ���ƴ֡���������
    * input			: iSessionID           ��¼�ɹ���ĻỰID
						  pRealplayPara        ʵ�����������ʵ�������ṹ��
						  pCamerCode           ����ͷ����
						  fRealPlayCallBackRaw ʵʱ����ص�����
						  pUserData            �û����ݣ��Զ��壬��Ӧ�ص�����������ʱ���ظò���
    * output		: pHandle              ���ž��
    * return		: �ɹ�����0��IVS_SUCCEED����ʧ�ܷ��ش�����
	* remark		: NA
    **************************************************************************/

	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StartRealPlayCBRaw(IVS_INT32 iSessionID, 
		IVS_REALPLAY_PARAM* pRealplayPara, 
		const IVS_CHAR* pCameraCode, 
		RealPlayCallBackRaw fRealPlayCallBackRaw, 
		void* pUserData, 
		IVS_ULONG* pHandle );


	/**************************************************************************
    * name			: IVS_SDK_StopRealPlay
    * description	: ֹͣʵʱ���
    * input			: iSessionID ��¼�ɹ���ĻỰID
						  ulHandle   ���ž��
    * output		: NA
    * return		: �ɹ�����0��IVS_SUCCEED����ʧ�ܷ��ش�����
	* remark		: NA
    **************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StopRealPlay( IVS_INT32 iSessionID, IVS_ULONG ulHandle);


	/**************************************************************************
    * name			: IVS_SDK_StartRealPlayByIP
    * description	: ��ʼʵʱ���
    * input			: iSessionID ��¼�ɹ���ĻỰID
						  pCameraCode  ���������
						  pMediaAddrDst  ý����Ŀ���ַ
	* output		: pMediaAddrSrc ý����Դ��ַ
						  pHandle ���ž����Ψһָ���˲���ͨ����
    * return		: �ɹ�����0��IVS_SUCCEED����ʧ�ܷ��ش�����
	* remark		: NA
    **************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StartRealPlayByIP(IVS_INT32 iSessionID, 
															const IVS_CHAR* pCameraCode,
															const IVS_MEDIA_ADDR* pMediaAddrDst,
															IVS_MEDIA_ADDR* pMediaAddrSrc,
															IVS_ULONG* pHandle);


	/**************************************************************************
    * name			: IVS_SDK_StopRealPlayByIP
    * description	: ֹͣʵʱ���
    * input			: iSessionID ��¼�ɹ���ĻỰID
						  ulHandle ���ž����Ψһָ���˲���ͨ����
	* output		: NA
    * return		: �ɹ�����0��IVS_SUCCEED����ʧ�ܷ��ش�����
	* remark		: NA
    **************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StopRealPlayByIP(IVS_INT32 iSessionID, 
															IVS_ULONG ulHandle);


	/**************************************************************************
    * name			: IVS_SDK_SetPlayQualityMode
    * description	: ����ʵ������ģʽ������������
    * input			: iSessionID   ��¼�ɹ���ĻỰID
						  ulHandle   ҵ����������ʵ�����ط�
						  iPlayQualityMode  ����ģʽ �ο���IVS_PLAY_QUALITY_MODE
    * output		: NA
    * return		: �ɹ�����0��IVS_SUCCEED����ʧ�ܷ��ش�����
    * remark		: NA
    **************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_SetPlayQualityMode(IVS_INT32 iSessionID, IVS_ULONG ulHandle,IVS_INT32 iPlayQualityMode);

	
	/******************************************************************
	function   : IVS_SDK_GetPTZAbsPosition
	description: ��ȡ��̨��ǰλ��
	input      : iSessionID ��¼�ɹ��󷵻صĻỰID
			   pDomainCode �����
               pCameraCode ���������
	output     : pPTZPosInfo ��̨λ����Ϣ
	return     : �ɹ�����0��ʧ�ܷ��ش�����
	*******************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetPTZAbsPosition(IVS_INT32 iSessionID,
		const IVS_CHAR* pDomainCode, const IVS_CHAR* pCameraCode, IVS_PTZ_ABSPOSITION *pPtzAbsPosition);

	/******************************************************************
	function   : IVS_SDK_PtzControl
	description: �ƾ�����
	input      : iSessionID ��¼�ɹ��󷵻صĻỰID
			   pCameraCode ���������
			   iControlCode ��̨�����룬ֵ�ο�IVS_PTZ_CODE
			   pControlPara1 ����1
			   pControlPara2 ����2
	output     : pLockStatus   ��̨����״̬��0-������1-����
	return     : �ɹ�����0��IVS_SUCCEED����ʧ�ܷ��ش�����
	*******************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_PtzControl(IVS_INT32 iSessionID,
		const IVS_CHAR* pCameraCode, 
		IVS_INT32 iControlCode,
		const IVS_CHAR* pControlPara1,
		const IVS_CHAR* pControlPara2, 
		IVS_UINT32* pLockStatus);

	/**************************************************************************
    * name			: IVS_SDK_AddPTZPreset
    * description	: ����Ԥ��λ
    * input			: iSessionID    ��¼�ɹ���ĻỰID
						  pCameraCode   ���������
						  pPTZPreset    Ԥ��λ����
	* output		: uiPresetIndex Ԥ��λ������
    * return		: �ɹ�����0��IVS_SUCCEED����ʧ�ܷ��ش�����
	* remark		: NA
    **************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_AddPTZPreset (IVS_INT32 iSessionID, const IVS_CHAR* pCameraCode, const IVS_CHAR* pPresetName, IVS_UINT32* uiPresetIndex);

	/**************************************************************************
    * name			: IVS_SDK_DelPTZPreset
    * description	: ɾ��Ԥ��λ
    * input			: iSessionID    ��¼�ɹ���ĻỰID
						  pCameraCode   ���������
						  uiPresetIndex Ԥ��λ����
	* output		: NA
    * return		: �ɹ�����0��IVS_SUCCEED����ʧ�ܷ��ش�����
	* remark		: NA
    **************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_DelPTZPreset (IVS_INT32 iSessionID, const IVS_CHAR* pCameraCode, IVS_UINT32 uiPresetIndex);

	/**************************************************************************
    * name			: IVS_SDK_ModPTZPreset
    * description	: �޸�Ԥ��λ
    * input			: iSessionID  ��¼�ɹ���ĻỰID
						  pCameraCode ���������
						  pPTZPreset  Ԥ��λ��Ϣ
	* output		: NA
    * return		: �ɹ�����0��IVS_SUCCEED����ʧ�ܷ��ش�����
	* remark		: NA
    **************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_ModPTZPreset (IVS_INT32 iSessionID, const IVS_CHAR* pCameraCode, const IVS_PTZ_PRESET* pPTZPreset);

	/**************************************************************************
    * name			: IVS_SDK_GetPTZPresetList
    * description	: ��ѯԤ��λ�б�
    * input			: iSessionID     ��¼�ɹ���ĻỰID
						  pCameraCode    ���������
						  pPTZPresetList Ԥ��λ��Ϣ�б����128������Ԥ�ȷ���128�����ڴ�ռ䣩
						  uiBufferSize   �����С
	* output		: uiPTZPresetNum ʵ��Ԥ��λ����
    * return		: �ɹ�����0��IVS_SUCCEED����ʧ�ܷ��ش�����
	* remark		: NA
    **************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetPTZPresetList (IVS_INT32 iSessionID, const IVS_CHAR* pCameraCode, IVS_PTZ_PRESET* pPTZPresetList, IVS_UINT32 uiBufferSize, IVS_UINT32* uiPTZPresetNum);

	/*************************************************************************
	* name			: IVS_SDK_GetRecordList
	* description	: ��ѯ¼���б�
	* input			: iSessionID ��¼�ɹ���ĻỰID
						  pCameraCode �����ID
						  iRecordMethod ¼��ʽ��0-ƽ̨¼�� 1-ǰ��¼�� 2-���ݷ�����
						  pTimeSpan ��ѯʱ���
						  pIndexRange ��ҳ��Ϣ
						  iBufSize �ڴ����ռ��С
	* output		: pRecordList ����¼���б�
	* return		: �ɹ�����0��IVS_SUCCEED����ʧ�ܷ��ش�����
	* remark		: NA
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetRecordList(IVS_INT32 iSessionID,
		const IVS_CHAR* pCameraCode,
		IVS_INT32 iRecordMethod,
		const IVS_TIME_SPAN* pTimeSpan,
		const IVS_INDEX_RANGE* pIndexRange,
		IVS_RECORD_INFO_LIST* pRecordList,
		IVS_UINT32 uiBufSize);

	/*************************************************************************
	* name			: IVS_SDK_QueryRecordList
	* description	: ��ѯ¼���б�
	* input			: iSessionID ��¼�ɹ���ĻỰID
						  pUnifiedQuery ͨ�ò�ѯ�ṹ
						  uiBufferSize �����ڴ��С
	* output		: pRecordInfoList ����¼���ѯ�ṹ�б�
	* return		: �ɹ�����0��IVS_SUCCEED����ʧ�ܷ��ش�����
	* remark		: NA
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_QueryRecordList(IVS_INT32 iSessionID, const IVS_QUERY_UNIFIED_FORMAT* pUnifiedQuery, 
		IVS_RECORD_INFO_LIST* pRecordInfoList,IVS_UINT32 uiBufferSize);

    /**************************************************************************
    * name			: IVS_SDK_StopPlatformPlayBack
    * description	: ֹͣƽ̨¼��ط�
    * input			: iSessionID      �ỰID
						  ulPlayHandle    ���ž��
    * output		: NA
    * return		: �ɹ�����0��IVS_SUCCEED����ʧ�ܷ��ش�����
    * remark		: NA
    **************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StopPlatformPlayBack(IVS_INT32 iSessionID,IVS_ULONG ulPlayHandle);

    /**************************************************************************
    * name			: IVS_SDK_StartPlatformPlayBackCBRaw
    * description	: ��ʼƽ̨¼��ط�(�������ص�)
    * input			: iSessionID           ��¼�ɹ���ĻỰID
						  pCameraCode          ���������
						  pPlaybackParam       �طŲ���
						  fPlayBackCallBackRaw ¼��ط��������ص�����
						  pUserData            �û����ݣ��Զ��壬��Ӧ�ص�����������ʱ���ظò���
    * output		: pHandle              ���ž��
    * return		: �ɹ�����0��IVS_SUCCEED����ʧ�ܷ��ش�����
    * remark		: NA
    **************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StartPlatformPlayBackCBRaw(IVS_INT32 iSessionID,
																		const IVS_CHAR* pCameraCode,
																		const IVS_PLAYBACK_PARAM* pPlaybackParam,
																		PlayBackCallBackRaw fPlayBackCallBackRaw, 
																		void* pUserData,
																		IVS_ULONG* pHandle);

	/*************************************************************************
	* name			: IVS_SDK_QueryDeviceList
	* description	: ��ѯ�豸�б�
	* input			: iSessionID ��¼�ɹ���ĻỰID
						  pDomainCode �����
						  uiDeviceType �豸���ͣ�ȡֵ�ο�IVS_DEVICE_TYPE��Ŀǰ֧�ֻ�ȡ����3���豸�б�
							 2-������豸
							 3-�����豸
							 4-�澯�豸
						  pQueryFormat ͨ�ò�ѯ�ṹ��
						  uiBufferSize �����С
	* output		: pBuffer  �豸�б�ָ�룬����˵�����£�
                                ������豸��IVS_CAMERA_BRIEF_INFO_LIST
                                �����豸��IVS_AUDIO_BRIEF_INFO_LIST
                                �澯�豸��IVS_ALARM_BRIEF_INFO_LIST
	* return		: �ɹ�����0��IVS_SUCCEED����ʧ�ܷ��ش�����
	* remark		: NA
	*************************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_QueryDeviceList(IVS_INT32 iSessionID,
		const IVS_CHAR* pDomainCode,
		IVS_UINT32 uiDeviceType, 
		const IVS_QUERY_UNIFIED_FORMAT *pQueryFormat, 
		IVS_VOID* pBuffer, 
		IVS_UINT32 uiBufferSize);

	/******************************************************************
    function		: IVS_SDK_GetChannelList
    description: �������豸��ѯ���豸�б�
    input			: iSessionID  ��¼�ɹ��󷵻صĻỰID
					  pDevCode   ���豸����
					  uiBufferSize �����С
    output		: pChannelList  ���豸�����豸�б�
    return		: �ɹ�����0��IVS_SUCCEED����ʧ�ܷ��ش�����
    *******************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetChannelList(IVS_INT32 iSessionID, const IVS_CHAR* pDevCode, IVS_DEV_CHANNEL_BRIEF_INFO_LIST* pChannelList, IVS_UINT32 uiBufferSize);
		
	/**************************************************************************
    * name			: IVS_SDK_GetDeviceConfig
    * description	: ��ȡ�豸����
    * input			: iSessionID        ��¼�ɹ���ĻỰID
						  pDevCode          �豸����
						  uiConfigType      �������ͣ�ֵ�ο�IVS_CONFIG_TYPE
						  uiBufferSize      �����С
    * output		: pBuffer           ���ݻ��棬ʹ�õ����ݽṹ��ο�IVS_CONFIG_TYPE��˵��       
    * return		: �ɹ�����0��IVS_SUCCEED����ʧ�ܷ��ش�����
    * remark		: NA
    **************************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetDeviceConfig(IVS_INT32 iSessionID,
                                                             const IVS_CHAR* pDevCode,
                                                             IVS_UINT32 uiConfigType,
                                                             IVS_VOID* pBuffer,
                                                             IVS_UINT32 uiBufSize);

	/**************************************************************************
    * name			: IVS_SDK_SetDeviceConfig
    * description	: �������豸�����豸����
    * input			: iSessionID        ��¼�ɹ���ĻỰID
						  pDevCode          �豸����
						  uiConfigType      �������ͣ�ֵ�ο�IVS_CONFIG_TYPE
						  pBuffer           �����ṹ��ָ��
						  uiBufferSize      �����ṹ���С
    * output		: NA
    * return		: �ɹ�����0��IVS_SUCCEED����ʧ�ܷ��ش�����
    * remark		: NA
    **************************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_SetDeviceConfig(IVS_INT32 iSessionID,
                                                             const IVS_CHAR* pDevCode, 
                                                             IVS_UINT32 uiConfigType,
                                                             IVS_VOID* pBuffer,
                                                             IVS_UINT32 uiBufSize);

	/*************************************************************************
	* name			: IVS_SDK_GetAlarmEventInfo
	* description	: ��ѯ�澯��Ϣ
	* input			: iSessionID ��¼�ɹ���ĻỰID
	* output		: NA
	* return		: �ɹ�����0��IVS_SUCCEED����ʧ�ܷ��ش�����
	* remark		: NA
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetAlarmEventInfo(IVS_INT32 iSessionID, IVS_UINT64 ullAlarmEventID, const IVS_CHAR* pAlarmInCode, IVS_ALARM_EVENT* pAlarmEvent);

	/*************************************************************************
	* name			: IVS_SDK_GetAlarmEventList
	* description	: ��ѯ��ʷ�澯��Ϣ�б�
	* input			: iSessionID ��¼�ɹ���ĻỰID
	* output		: NA
	* return		: �ɹ�����0��IVS_SUCCEED����ʧ�ܷ��ش�����
	* remark		: NA
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetAlarmEventList(IVS_INT32 iSessionID, const IVS_QUERY_UNIFIED_FORMAT* pUnifiedQuery, IVS_ALARM_EVENT_LIST* pAlarmEventList, IVS_UINT32 uiBufferSize);

	/*************************************************************************
	* name			: IVS_SDK_AddRecordPlan
	* description	: ����¼��ƻ�
	* input			: iSessionID ��¼�ɹ���ĻỰID
						  uiDeviceNum �豸����
						  pDeviceList �豸�����б�
						  pRecordPlan ¼��ƻ�
						  uiResultBufferSize ����б����С
	* output		: pResultList ��������б�
	* return		: �ɹ�����0��IVS_SUCCEED����ʧ�ܷ��ش�����
	* remark		: NA
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_AddRecordPlan(IVS_INT32 iSessionID,IVS_UINT32 uiDeviceNum,
		const IVS_DEVICE_CODE* pDeviceList,const IVS_RECORD_PLAN* pRecordPlan,IVS_RESULT_LIST* pResultList,IVS_UINT32 uiResultBufferSize);

	/*************************************************************************
	* name			: IVS_SDK_ModifyRecordPlan
	* description	: �޸�¼��ƻ�
	* input			: iSessionID ��¼�ɹ���ĻỰID
						  uiDeviceNum �豸����
						  pDeviceList �豸�����б�
						  pRecordPlan ¼��ƻ�
						  uiResultBufferSize ����б����С
	* output		: pResultList ��������б�
	* return		: �ɹ�����0��IVS_SUCCEED����ʧ�ܷ��ش�����
	* remark		: NA
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_ModifyRecordPlan(IVS_INT32 iSessionID,IVS_UINT32 uiDeviceNum,
		const IVS_DEVICE_CODE* pDeviceList,	const IVS_RECORD_PLAN* pRecordPlan,IVS_RESULT_LIST* pResultList,IVS_UINT32 uiResultBufferSize);

	/*************************************************************************
	* name			: IVS_SDK_DeleteRecordPlan
	* description	: ɾ��¼��ƻ�
	* input			: iSessionID ��¼�ɹ���ĻỰID
						  uiDeviceNum �豸����
						  pDeviceList �豸�����б�
						  uiRecordMethod ¼��ʽ
						  uiResultBufferSize ����б����С
	* output		: pResultList ��������б�
	* return		: �ɹ�����0��IVS_SUCCEED����ʧ�ܷ��ش�����
	* remark		: NA
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_DeleteRecordPlan(IVS_INT32 iSessionID,IVS_UINT32 uiDeviceNum,
		const IVS_DEVICE_CODE* pDeviceList,	IVS_UINT32 uiRecordMethod,IVS_RESULT_LIST* pResultList,IVS_UINT32 uiResultBufferSize);

	/*************************************************************************
	* name			: IVS_SDK_GetRecordPlan
	* description	: ��ѯ¼��ƻ�
	* input			: iSessionID ��¼�ɹ���ĻỰID
						  pCameraCode �豸����
						  uiRecordMethod ¼��ʽ
	* output		: pRecordPlan ¼��ƻ�
	* return		: �ɹ�����0��IVS_SUCCEED����ʧ�ܷ��ش�����
	* remark		: NA
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetRecordPlan(IVS_INT32 iSessionID,const char* pCameraCode,IVS_UINT32 uiRecordMethod,IVS_RECORD_PLAN *pRecordPlan);

	/******************************************************************
    function		: IVS_SDK_AddUser
    description: ����û�
    input			: iSessionID ��¼�ɹ���ĻỰID
					  pUserInfo     �û���Ϣ�ṹ��
    output		: puiUserID ��ӳɹ��󷵻��û�ID
    return		: �ɹ�����0��IVS_SUCCEED����ʧ�ܷ��ش�����
    *******************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_AddUser(IVS_INT32 iSessionID, const IVS_USER_INFO* pUserInfo, IVS_UINT32* puiUserID);

    /******************************************************************
    function		: IVS_SDK_ModifyUser
    description: �޸��û�
    input			: iSessionID ��¼�ɹ���ĻỰID
					  pUserInfo �û���Ϣ�ṹ��
	output		: NA
    return		: �ɹ�����0��IVS_SUCCEED����ʧ�ܷ��ش�����
    *******************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_ModifyUser(IVS_INT32 iSessionID, const IVS_USER_INFO* pUserInfo);

    /******************************************************************
    function		: IVS_SDK_DeleteUser
    description: ɾ���û�
    input			: iSessionID ��¼�ɹ���ĻỰID
					  pDomainCode �����
					  uiUserID �û�ID
    output		: NA
    return		: �ɹ�����0��IVS_SUCCEED����ʧ�ܷ��ش�����
    *******************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_DeleteUser(IVS_INT32 iSessionID, const IVS_CHAR* pDomainCode, IVS_UINT32 uiUserID);

	/******************************************************************
    function		: IVS_SDK_ChangePWD
    description: �޸�����
    input			: iSessionID ��¼�ɹ���ĻỰID
					  pOldPWD   ������
					  pNewPWD   ������
    return		: �ɹ�����0��IVS_SUCCEED����ʧ�ܷ��ش�����
    *******************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_ChangePWD(IVS_INT32 iSessionID, const IVS_CHAR* pOldPWD, const IVS_CHAR* pNewPWD);

	/**************************************************************************
	* name			: IVS_SDK_GetUserID
	* description	: ��ȡ��½�û����û�ID
	* input			: iSessionID     ��¼���ص�SessionID
	* output		: puiUserID  �û�ID
	* return		: �ɹ�����0��IVS_SUCCEED����ʧ�ܷ��ش�����
	* remark		: NA
	**************************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetUserID(IVS_INT32 iSessionID, IVS_UINT32* puiUserID);

	/******************************************************************
    function		: IVS_SDK_GetUserInfo
    description: ��ѯ�û���Ϣ
    input			: iSessionID ��¼�ɹ���ĻỰID
					  pDomainCode �����
					  uiUserID �û�ID
    output		: pUserInfo �û���Ϣ
    return		: �ɹ�����0��IVS_SUCCEED����ʧ�ܷ��ش�����
    *******************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetUserInfo(IVS_INT32 iSessionID, const IVS_CHAR* pDomainCode, IVS_UINT32 uiUserID, IVS_USER_INFO* pUserInfo);

	/******************************************************************
    function		: IVS_SDK_GetUserList
    description: ��ѯ�û��б�
    input			: iSessionID ��¼�ɹ���ĻỰID
					  pDomainCode �����
					  pUnifiedQuery  ͨ�ò�ѯ����
					  uiBufferSize �û���Ϣ�б�ṹ�建���С
    output		: pUserList  �û���Ϣ�б�ṹ��
    return		: �ɹ�����0��IVS_SUCCEED����ʧ�ܷ��ش�����
    *******************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetUserList(IVS_INT32 iSessionID, const IVS_CHAR* pDomainCode, 
		const IVS_QUERY_UNIFIED_FORMAT* pUnifiedQuery, IVS_USER_INFO_LIST* pUserList, IVS_UINT32 uiBufferSize);

	/******************************************************************
	function		: IVS_SDK_ReleaseBuffer
	description: �ͷ�SDK�ڲ�������ڴ�
	input			: pBuffer �ڴ��ַ
	output		: NA
	return		: �ɹ�����0��IVS_SUCCEED����ʧ�ܷ��ش�����
	*******************************************************************/  
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_ReleaseBuffer(IVS_CHAR *pBuffer);

	/*************************************************************************
	* name			: IVS_SDK_SubscribeAlarmQuery
	* description	: �鿴�澯�����б�
	* input			: iSessionID ��¼�ɹ���ĻỰID
						  pReqXml ����XML��Ϣ
	* output		: pRspXml ��ӦXML��Ϣ
	* return		: �ɹ�����0��IVS_SUCCEED����ʧ�ܷ��ش�����
	* remark		: NA
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_SubscribeAlarmQuery(IVS_INT32 iSessionID, const IVS_CHAR* pReqXml, IVS_CHAR** pRspXml);

	/*************************************************************************
	* name			: IVS_SDK_SubscribeAlarm
	* description	: �����澯����
	* input			: iSessionID ��¼�ɹ���ĻỰID
						  pReqXml ����XML��Ϣ
	* output		: NA
	* return		: �ɹ�����0��IVS_SUCCEED����ʧ�ܷ��ش�����
	* remark		: NA
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_SubscribeAlarm(IVS_INT32 iSessionID, const IVS_CHAR* pReqXml);

	/**************************************************************************
    * name			: IVS_SDK_RebootDevice
    * description	: ����ǰ���豸
    * input			: iSessionID ��¼�ɹ���ĻỰID
						  pDeviceCode  ���豸����
						  uiRebootType �������ͣ�0-���� 1-�ָ�Ĭ�����ò�����
	* output		: NA
    * return		: �ɹ�����0��IVS_SUCCEED����ʧ�ܷ��ش�����
	* remark		: NA
    **************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_RebootDevice (IVS_INT32 iSessionID, const IVS_CHAR* pDeviceCode, IVS_UINT32 uiRebootType);

	/*************************************************************************
	* name			: IVS_SDK_SetMainDeviceName
	* description	: �޸����豸����
	* input			: iSessionID ��¼�ɹ���ĻỰID, 
						  pDevCode ���豸����
						  pNewDevName ���豸�µ�����
	* output		: NA
	* return		: �ɹ�����0��IVS_SUCCEED����ʧ�ܷ��ش�����
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_SetMainDeviceName(	IVS_INT32 iSessionID,
																const IVS_CHAR* pDevCode,
																const IVS_CHAR* pNewDevName);

	/*************************************************************************
	* name			: IVS_SDK_SetCameraName
	* description	: �޸����������
	* input			: iSessionID ��¼�ɹ���ĻỰID, 
						  pCameraCode ���������
						  pNewCameraName �����������
	* output		: NA
	* return		: �ɹ�����0��IVS_SUCCEED����ʧ�ܷ��ش�����
	* remark		: NA
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_SetCameraName(	IVS_INT32 iSessionID, const IVS_CHAR* pCameraCode, const IVS_CHAR* pNewCameraName);

	/*****************************************************************
	function		: IVS_SDK_StartSearchDevice
	description	: ��������ǰ���豸
	input			: iSessionID   ��¼�ɹ���ĻỰID
					  pDomainCode  Ŀ�������
					  pSearchInfo �豸������ϢXML
	output		: NA
	return		: �ɹ�����0��IVS_SUCCEED����ʧ�ܷ��ش�����
	*****************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StartSearchDevice(IVS_INT32 iSessionID, const IVS_CHAR* pDomainCode, const IVS_CHAR* pSearchInfo);

	/*****************************************************************
	function		: IVS_SDK_StopSearchDevice
	description	: ֹͣ����ǰ���豸
	input			: iSessionID   ��¼�ɹ���ĻỰID
					  pDomainCode  Ŀ�������
					  pSearchInfo �豸������ϢXML
	output		: NA
	return		: �ɹ�����0��IVS_SUCCEED����ʧ�ܷ��ش�����
	*****************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StopSearchDevice(IVS_INT32 iSessionID, const IVS_CHAR* pDomainCode, const IVS_CHAR* pSearchInfo);

	/*****************************************************************
	function		: IVS_SDK_VerifyDevice
	description	: ��֤ǰ���豸
	input			: iSessionID   ��¼�ɹ���ĻỰID
					  pDomainCode  Ŀ�������
					  pVerifyInfo �豸��֤��ϢXML
	output		: pRspXml   ��֤���XML 
	return		: �ɹ�����0��IVS_SUCCEED����ʧ�ܷ��ش�����
	*****************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_VerifyDevice(IVS_INT32 iSessionID, const IVS_CHAR* pDomainCode, const IVS_CHAR* pVerifyInfo, IVS_CHAR** pRspXml);

	/*************************************************************************
	* name			: IVS_SDK_AddDevice
	* description	: �����豸
	* input			: iSessionID ��¼�ɹ���ĻỰID
						  pDomainCode �����
						  uiDeviceNum ��ӵ��豸��Ŀ
						  pDevList �豸�����б�
						  uiResultBufferSize ����б����С
	* output		: pResultList �豸������������б�
	* return		: �ɹ�����0��IVS_SUCCEED����ʧ�ܷ��ش�����
	*************************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_AddDevice(IVS_INT32 iSessionID, 
                                                       const IVS_CHAR* pDomainCode, 
                                                       IVS_UINT32 uiDeviceNum, 
                                                       const IVS_DEVICE_OPER_INFO* pDevList, 
                                                       IVS_DEVICE_OPER_RESULT_LIST* pResultList, 
                                                       IVS_UINT32 uiResultBufferSize);

	/*************************************************************************
	* name			: IVS_SDK_DeleteDevice
	* description	: ɾ���豸
	* input			: iSessionID ��¼�ɹ���ĻỰID
						  pDomainCode�����
						  uiDeviceNum ɾ�����豸��Ŀ
						  pDevCodeList �豸�����б�
						  uiResultBufferSize ����б����С
	* output		: pResultList �豸������������б�
	* return		: �ɹ�����0��IVS_SUCCEED����ʧ�ܷ��ش�����
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_DeleteDevice(IVS_INT32 iSessionID,const IVS_CHAR* pDomainCode,IVS_UINT32 uiDeviceNum,
		const IVS_DEVICE_CODE* pDevCodeList, IVS_RESULT_LIST* pResultList,IVS_UINT32 uiResultBufferSize);

	/*************************************************************************
	* name			: IVS_SDK_SetRecordPolicyByTime
	* description	: �޸�¼�����
	* input			: iSessionID  ��¼�ɹ���ĻỰID
						  pCamerCode ����ͷ����
						  pRecordPolicy ¼��ʱ�����
	* output		: NA
	* return		: �ɹ�����0��IVS_SUCCEED����ʧ�ܷ��ش�����
	* remark		: NA
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_SetRecordPolicyByTime(IVS_INT32 iSessionID,const IVS_CHAR* pCameraCode,const IVS_RECORD_POLICY_TIME* pRecordPolicy);

	/*************************************************************************
	* name			: IVS_SDK_GetRecordPolicyByTime
	* description	: ��ѯƽ̨¼�����
	* input			: iSessionID  ��¼�ɹ���ĻỰID
	                      pCamerCode ����ͷ����
	* output		: pRecordPolicy ¼��ʱ�����
	* return		: �ɹ�����0��IVS_SUCCEED����ʧ�ܷ��ش�����
	* remark		: NA
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetRecordPolicyByTime(IVS_INT32 iSessionID,const IVS_CHAR* pCameraCode,IVS_RECORD_POLICY_TIME* pRecordPolicy);

	/*************************************************************************
	* name			: IVS_SDK_ConfirmAlarm
	* description	: �澯ȷ��
	* input			: iSessionID ��¼�ɹ���ĻỰID
						  ullAlarmEventID �澯�¼�ID
						  pAlarmInCode �澯Դ����
						  pAlarmOperateInfo �澯������Ϣ
	* output		: NA
	* return		: �ɹ�����0��IVS_SUCCEED����ʧ�ܷ��ش�����
	* remark		: NA
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_AlarmConfirm(IVS_INT32 iSessionID, IVS_UINT64 ullAlarmEventID, const IVS_CHAR* pAlarmInCode, const IVS_ALARM_OPERATE_INFO* pAlarmOperateInfo);

	/*************************************************************************
	* name			: IVS_SDK_CancelAlarm
	* description	: �澯����
	* input			: iSessionID ��¼�ɹ���ĻỰID
						  ullAlarmEventID �澯�¼�ID
						  pAlarmInCode �澯Դ����
						  pAlarmOperateInfo �澯������Ϣ
	* output		: NA
	* return		: �ɹ�����0��IVS_SUCCEED����ʧ�ܷ��ش�����
	* remark		: NA
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_AlarmCancel(IVS_INT32 iSessionID, IVS_UINT64 ullAlarmEventID, const IVS_CHAR* pAlarmInCode, const IVS_ALARM_OPERATE_INFO* pAlarmOperateInfo);

	/*************************************************************************
	* name			: IVS_SDK_AlarmCommission
	* description	: �澯��Ȩ
	* input			: iSessionID ��¼�ɹ���ĻỰID
						  pAlarmInCode �澯Դ����
						  pAlarmCommission �澯��Ȩ�ṹ��
	* output		: NA
	* return		: �ɹ�����0��IVS_SUCCEED����ʧ�ܷ��ش�����
	* remark		: NA
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_AlarmCommission (IVS_INT32 iSessionID, const IVS_CHAR* pAlarmInCode, const IVS_ALARM_COMMISSION* pAlarmCommission);

	/*************************************************************************
	* name			: IVS_SDK_UserAlarmAuthorization
	* description	: ��Ȩ�澯�û���֤
	* input			: iSessionID ��¼�ɹ���ĻỰID
						  pPWD ����
	* output		: NA
	* return		: �ɹ�����0��IVS_SUCCEED����ʧ�ܷ��ش�����
	* remark		: NA
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_UserAuthentication(IVS_INT32 iSessionID, const IVS_CHAR *pPWD);

	/*************************************************************************
	* name			: IVS_SDK_AlarmReport
	* description	: �澯��Ϣ�ϱ����澯���ͣ�
	* input			: iSessionID ��¼�ɹ���ĻỰID
						  pAlarmReport �澯�ϱ��ṹ��
	* output		: NA
	* return		: �ɹ�����0��IVS_SUCCEED����ʧ�ܷ��ش�����
	* remark		: NA
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_AlarmReport(IVS_INT32 iSessionID, const IVS_ALARM_REPORT* pAlarmReport);

	/*************************************************************************
	* name			: IVS_SDK_AddAlarmLinkage
	* description	: �����澯��������
	* input			: iSessionID ��¼�ɹ���ĻỰID
						  pReqXml ������Ϣ
	* output		: pRspXml ��Ӧ��Ϣ����������������ID
	* return		: �ɹ�����0��IVS_SUCCEED����ʧ�ܷ��ش�����
	* remark		: NA
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_AddAlarmLinkage(IVS_INT32 iSessionID,	const IVS_CHAR* pReqXml, IVS_CHAR** pRspXml);

	/*************************************************************************
	* name			: IVS_SDK_ModifyAlarmLinkage
	* description	: �޸ĸ澯��������
	* input			: iSessionID ��¼�ɹ���ĻỰID
						  pReqXml ������Ϣ
	* output		: NA
	* return		: �ɹ�����0��IVS_SUCCEED����ʧ�ܷ��ش�����
	* remark		: NA
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_ModifyAlarmLinkage(IVS_INT32 iSessionID, const IVS_CHAR* pReqXml);

	/*************************************************************************
	* name			: IVS_SDK_DeleteAlarmLinkage
	* description	: ɾ���澯��������
	* input			: iSessionID ��¼�ɹ���ĻỰID
						  pReqXml ������Ϣ
	* output		: NA
	* return		: �ɹ�����0��IVS_SUCCEED����ʧ�ܷ��ش�����
	* remark		: NA
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_DeleteAlarmLinkage(IVS_INT32 iSessionID, const IVS_CHAR* pReqXml);

	/*************************************************************************
	* name			: IVS_SDK_GetAlarmlinkageList
	* description	: �鿴�澯���������б�
	* input			: iSessionID ��¼�ɹ���ĻỰID
						  pReqXml ������Ϣ
	* output		: pRspXml ��Ӧ��Ϣ����������������ID
	* return		: �ɹ�����0��IVS_SUCCEED����ʧ�ܷ��ش�����
	* remark		: NA
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetAlarmlinkageList(IVS_INT32 iSessionID, const IVS_CHAR* pReqXml, IVS_CHAR** pRspXml);

	/*************************************************************************
	* name			: IVS_SDK_GetAlarmLinkage
	* description	: ��ѯ�澯��������
	* input			: iSessionID ��¼�ɹ���ĻỰID
						  pReqXml ������Ϣ
	* output		: pRspXml ��Ӧ��Ϣ����������������ID
	* return		: �ɹ�����0��IVS_SUCCEED����ʧ�ܷ��ش�����
	* remark		: NA
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetAlarmLinkage (IVS_INT32 iSessionID, const IVS_CHAR* pReqXml, IVS_CHAR** pRspXml);

	/*****************************************************************
	* name			: IVS_SDK_GetAlarmLinkageAction
	* description	: ��ѯ�澯������������
	* input			: iSessionID ��¼�ɹ���ĻỰID
						  pReqXml ������Ϣ
	* output		: pRspXml ��Ӧ��Ϣ����������������ID
	* return		: �ɹ�����0��IVS_SUCCEED����ʧ�ܷ��ش�����
	* remark		: NA
	*****************************************************************/	
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetAlarmLinkageAction(IVS_INT32 iSessionID, const IVS_CHAR* pReqXml, IVS_CHAR** pRsqXml);

	/*************************************************************************
	* name			: IVS_SDK_StartAlarmOut
	* description	: ��������������澯
	* input			: iSessionID ��¼�ɹ���ĻỰID
						  pAlarmOutCode ����������豸����
	* output		: NA
	* return		: �ɹ�����0��IVS_SUCCEED����ʧ�ܷ��ش�����
	* remark		: NA
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StartAlarmOut(IVS_INT32 iSessionID, const IVS_CHAR* pAlarmOutCode);

	/*************************************************************************
	* name			: IVS_SDK_StopAlarmOut
	* description	: ֹͣ����������澯
	* input			: iSessionID ��¼�ɹ���ĻỰID
						  pAlarmOutCode ����������豸����
	* output		: NA
	* return		: �ɹ�����0��IVS_SUCCEED����ʧ�ܷ��ش�����
	* remark		: NA
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StopAlarmOut(IVS_INT32 iSessionID, const IVS_CHAR* pAlarmOutCode);

	/*************************************************************************
	* name			: IVS_SDK_GetAlarmTypeList
	* description	: ��ѯ�澯�����б�
	* input			: iSessionID ��¼�ɹ���ĻỰID
						  pUnifiedQuery ͨ�ò�ѯ�ṹ��
						  uiBufferSize �����С
	* output		: pAlarmTypeList �澯�����б�
	* return		: �ɹ�����0��IVS_SUCCEED����ʧ�ܷ��ش�����
	* remark		: NA
	*************************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetAlarmTypeList (IVS_INT32 iSessionID, 
        const IVS_QUERY_UNIFIED_FORMAT* pUnifiedQuery, IVS_ALARM_TYPE_LIST* pAlarmTypeList, IVS_UINT32 uiBufferSize);

	/*************************************************************************
	* name			: IVS_SDK_SetAlarmTypeLevel
	* description	: ��ѯ�澯���ͼ���
	* input			: iSessionID ��¼�ɹ���ĻỰID
						  pAlarmTypeList �澯�����б�
	* output		: NA
	* return		: �ɹ�����0��IVS_SUCCEED����ʧ�ܷ��ش�����
	* remark		: NA
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_SetAlarmTypeLevel(IVS_INT32 iSessionID, const IVS_ALARM_TYPE_LIST* pAlarmTypeList);

	/*************************************************************************
	* name			: IVS_SDK_GetAlarmLevelList
	* description	: ��ѯ�澯�����б�
	* input			: iSessionID ��¼�ɹ���ĻỰID
						  uiMaxAlarmLevelNum ϵͳ���澯�������IVS_MAX_ALARM_LEVEL_NUM
						  uiBufferSize  �澯���������ڴ�ռ��С
	* output			: puiAlarmLevelNum ��ѯ���ص�ϵͳ�澯�������
						  pAlarmLevel �澯�����б�
	* return			: �ɹ�����0��IVS_SUCCEED����ʧ�ܷ��ش�����
	* remark			: NA
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetAlarmLevelList(IVS_INT32 iSessionID,
		IVS_UINT32 uiMaxAlarmLevelNum,IVS_UINT32* puiAlarmLevelNum,IVS_ALARM_LEVEL* pAlarmLevel,IVS_UINT32 uiBufferSize);

	/**************************************************************************
    * name			: IVS_SDK_GetAlarmLevel
    * description	: ��ѯ�澯��������
    * input			: iSessionID     ��¼�ɹ���ĻỰID
						  uiAlarmLevelID    �澯����ID
    * output			: pAlarmLevel    �澯�������� 
    * return			: �ɹ�����0��IVS_SUCCEED����ʧ�ܷ��ش�����
    * remark			: NA
    **************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetAlarmLevel(IVS_INT32 iSessionID,IVS_UINT32 uiAlarmLevelID,IVS_ALARM_LEVEL* pAlarmLevel);

	/*************************************************************************
	* name			: IVS_SDK_AddAlarmArea
	* description	: ��������
	* input			: iSessionID ��¼�ɹ���ĻỰID
						  pReq ����XML
	* output			: pRsp ��ӦXML
	* return			: �ɹ�����0��IVS_SUCCEED����ʧ�ܷ��ش�����
	* remark			: NA
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_AddAlarmArea(IVS_INT32 iSessionID, const IVS_CHAR* pReq, IVS_CHAR** pRsp);

	/*************************************************************************
	* name			: IVS_SDK_ModifyAlarmArea
	* description	: �޸ķ���
	* input			: iSessionID ��¼�ɹ���ĻỰID
						  pReqXml ����XML
	* output			: NA
	* return			: �ɹ�����0��IVS_SUCCEED����ʧ�ܷ��ش�����
	* remark			: NA
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_ModifyAlarmArea(IVS_INT32 iSessionID, const IVS_CHAR* pReqXml);

	/*************************************************************************
	* name			: IVS_SDK_DeleteAlarmArea
	* description	: ɾ������
	* input			: iSessionID ��¼�ɹ���ĻỰID
						  pDomainCode Ŀ�������
						  uiAlarmAreaId ����ID
	* output			: NA
	* return			: �ɹ�����0��IVS_SUCCEED����ʧ�ܷ��ش�����
	* remark			: NA
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_DeleteAlarmArea(IVS_INT32 iSessionID, const IVS_CHAR* pDomainCode, IVS_UINT32 uiAlarmAreaId);

	/*************************************************************************
	* name			: IVS_SDK_GetAlarmAreaList
	* description	: �鿴�����б�
	* input			: iSessionID ��¼�ɹ���ĻỰID
						  pReqXml ����XML
	* output			: pRspXml ��ӦXML
	* return			: �ɹ�����0��IVS_SUCCEED����ʧ�ܷ��ش�����
	* remark			: NA
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetAlarmAreaList(IVS_INT32 iSessionID, const IVS_CHAR* pReqXml, IVS_CHAR** pRspXml);

	/*************************************************************************
	* name			: IVS_SDK_GetAlarmAreaInfo
	* description	: �鿴��������
	* input			: iSessionID ��¼�ɹ���ĻỰID
						  pDomainCode �����
						  uiAlarmAreaId ����ID
	* output			: pAlarmArea ��������
	* return			: �ɹ�����0��IVS_SUCCEED����ʧ�ܷ��ش�����
	* remark			: NA
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetAlarmAreaInfo(IVS_INT32 iSessionID,const IVS_CHAR* pDomainCode,IVS_UINT32 uiAlarmAreaId,IVS_CHAR** pAlarmArea);

	/*************************************************************************
	* name			: IVS_SDK_StartAlarmAreaGuard
	* description	: �ֶ�����
	* input			: iSessionID ��¼�ɹ���ĻỰID
						  pDomainCode �����
						  uiAlarmAreaId  ����ID
	* output			: NA
	* return			: �ɹ�����0��IVS_SUCCEED����ʧ�ܷ��ش�����
	* remark			: NA
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StartAlarmAreaGuard(IVS_INT32 iSessionID, const IVS_CHAR* pDomainCode,IVS_UINT32 uiAlarmAreaId);

	/*************************************************************************
	* name			: IVS_SDK_SubscribeAlarmQuery
	* description	: �ֶ�����
	* input			: iSessionID ��¼�ɹ���ĻỰID
						  pDomainCode �����
						  uiAlarmAreaId  ����ID
	* output			: NA
	* return			: �ɹ�����0��IVS_SUCCEED����ʧ�ܷ��ش�����
	* remark			: NA
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StopAlarmAreaGuard(IVS_INT32 iSessionID, const IVS_CHAR* pDomainCode,IVS_UINT32 uiAlarmAreaId);

	/*************************************************************************
	* name			: IVS_SDK_AllocAreaGuardPlan
	* description	: ���������ƻ�
	* input			: iSessionID ��¼�ɹ���ĻỰID
						  pReqXml ����XML
	* output			: NA
	* return			: �ɹ�����0��IVS_SUCCEED����ʧ�ܷ��ش�����
	* remark			: NA
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_AllocAreaGuardPlan(IVS_INT32 iSessionID,const IVS_CHAR* pReqXml);

	/*************************************************************************
	* name			: IVS_SDK_GetAreaGuardPlan
	* description	: �鿴�����ƻ�
	* input			: iSessionID ��¼�ɹ���ĻỰID
						  pDomainCode �����
						  uiAlarmAreaId ����ID
	* output			: pRspXml ��ӦXML
	* return			: �ɹ�����0��IVS_SUCCEED����ʧ�ܷ��ش�����
	* remark			: NA
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetAreaGuardPlan(IVS_INT32 iSessionID, const IVS_CHAR* pDomainCode,IVS_UINT32 uiAlarmAreaId, IVS_CHAR** pRspXml);

	/*********************************************************************************************
	function		: IVS_SDK_SetDeviceGuard
	description	: �豸������
	input			: iSessionID ��¼�ɹ���ĻỰID
					  pReqXml  ����XML��Ϣ
	output		: NA
	return		: �ɹ�����0��IVS_SUCCEED����ʧ�ܷ��ش�����
	**********************************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_SetDeviceGuard(IVS_INT32 iSessionID, const IVS_CHAR* pReqXml);

	/******************************************************************
    function		: IVS_SDK_LockUser
    description: ����/�����û�
    input			: iSessionID   ��¼�ɹ���ĻỰID
					  pDomainCode   �����
					  uiUserID   �û�ID
					  bLock   �Ƿ������û���0-������1-����
    return		: �ɹ�����0��IVS_SUCCEED����ʧ�ܷ��ش�����
    *******************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_LockUser(IVS_INT32 iSessionID, const IVS_CHAR* pDomainCode, IVS_UINT32 uiUserID, IVS_BOOL bLock);

	/******************************************************************
    function		: IVS_SDK_SignOutUser
    description: �����û�IDǩ���û������лỰ
    input			: iSessionID  ��¼�ɹ���ĻỰID
					  pDomainCode  �����
					  uiUserID  �û�ID
    return		: �ɹ�����0��IVS_SUCCEED����ʧ�ܷ��ش�����
    *******************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_SignOutUser(IVS_INT32 iSessionID, const IVS_CHAR* pDomainCode, IVS_UINT32 uiUserID);

	/******************************************************************
    function		: IVS_SDK_SignOutUserByRole
    description: ���ݽ�ɫIDǩ���û������лỰ
    input			: iSessionID  ��¼�ɹ���ĻỰID
					  pDomainCode  �����
					  uiRoleID  ��ɫID
    return		: �ɹ�����0��IVS_SUCCEED����ʧ�ܷ��ش�����
    *******************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_SignOutUserByRole(IVS_INT32 iSessionID, const IVS_CHAR* pDomainCode, IVS_UINT32 uiRoleID);

    /******************************************************************
    function		: IVS_SDK_SignOutOnlineUser
    description: ǩ�������û�
    input			: iSessionID ��¼�ɹ���ĻỰID
					  pDomainCode  �����
					  uiUserOnlineID  �����û��ĻỰID
    return		: �ɹ�����0��IVS_SUCCEED����ʧ�ܷ��ش�����
    *******************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_SignOutOnlineUser(IVS_INT32 iSessionID, const IVS_CHAR* pDomainCode, IVS_UINT32 uiUserOnlineID);

	/******************************************************************
    function		: IVS_SDK_GetOnlineUser
    description: ��ѯ�����û��б�
    input			: iSessionID ��¼�ɹ���ĻỰID
					  pDomainCode  �����
					  pUnifiedQuery  ͨ�ò�ѯ����
					  uiBufferSize  �����С
    output		: pOnlineUserList   �����û���Ϣ�б�ṹ��
    return		: �ɹ�����0��IVS_SUCCEED����ʧ�ܷ��ش�����
    *******************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetOnlineUser(IVS_INT32 iSessionID, const IVS_CHAR* pDomainCode, 
		const IVS_QUERY_UNIFIED_FORMAT* pUnifiedQuery, IVS_ONLINE_USER_LIST* pOnlineUserList, IVS_UINT32 uiBufferSize);

	/******************************************************************
    function		: IVS_SDK_ResetPWD
    description: ��������
    input			: iSessionID  ��¼�ɹ���ĻỰID
					  pDomainCode  �����
					  iUserID  �û�ID
					  pNewPWD �û�������
    return		: �ɹ�����0��IVS_SUCCEED����ʧ�ܷ��ش�����
    *******************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_ResetPWD(IVS_INT32 iSessionID, const IVS_CHAR* pDomainCode, const IVS_UINT32 uiUserID, const IVS_CHAR* pNewPWD);

    /******************************************************************
    function		: IVS_SDK_CheckPWDByRole
    description: У�������Ƿ������˺Ź���
    input			: iSessionID  ��¼�ɹ���ĻỰID
					  pDomainCode  �����
					  uiRoleID  ��ɫID
					  pLoginName �û���¼��
					  pPWD   �û����õ�����
    return		: �ɹ�����0��IVS_SUCCEED����ʧ�ܷ��ش�����
    *******************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_CheckPWDByRole(IVS_INT32 iSessionID, 
		const IVS_CHAR* pDomainCode, const IVS_UINT32 uiRoleID, const IVS_CHAR* pLoginName, const IVS_CHAR* pPWD);

    /******************************************************************
    function		: IVS_SDK_GetAutoCreatePWD
    description: ���ݽ�ɫ��ȡ�����˺Ź��������
    input			: iSessionID  ��¼�ɹ���ĻỰID
					  pDomainCode  �����
					  uiRoleID  ��ɫID
					  pLoginName  �û���¼��
					  uiPWDLen  ���볤��
    output		: pNewPWD  �����ַ���
    return		: �ɹ�����0��IVS_SUCCEED����ʧ�ܷ��ش�����
    *******************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetAutoCreatePWD(IVS_INT32 iSessionID, 
		const IVS_CHAR* pDomainCode, IVS_UINT32 uiRoleID, const IVS_CHAR* pLoginName, IVS_CHAR* pNewPWD, IVS_UINT32 uiPWDLen);

    /******************************************************************
    function		: IVS_SDK_GetPWDStrength
    description: ��ȡ����ǿ��
    input			: pPWD   ����
    output		: uiPWDStrength  ����ǿ��
    return		: ��������ǿ�ȣ�ֵ�ο� IVS_KEY_RELIABILITY��
    *******************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetPWDStrength(const IVS_CHAR* pPWD, IVS_UINT32* uiPWDStrength);

	/******************************************************************
    function		: IVS_SDK_GetPWDExpireInfo
    description	: ��ȡ���������Ϣ
	input			: iSessionID    ��¼�ɹ���ĻỰID
    output		: pPWDExpireInfo    ���������Ϣ
    return		: �ɹ�����0��IVS_SUCCEED����ʧ�ܷ��ش�����
    *******************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetPWDExpireInfo(IVS_INT32 iSessionID, IVS_PWD_EXPIRE_INFO* pPWDExpireInfo);

	/******************************************************************
    function		: IVS_SDK_SetUserDeviceList
    description: �����û��豸�б�
    input			: iSessionID  ��¼�ɹ���ĻỰID
					  pDomainCode  �����
					  uiUserID  �û�ID
					  uiBufferSize   �����С
    output		: pUserDevList  �û��б�ṹ��
    return		: �ɹ�����0��IVS_SUCCEED����ʧ�ܷ��ش�����
    *******************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_SetUserDeviceList(IVS_INT32 iSessionID, 
		const IVS_CHAR* pDomainCode, IVS_UINT32 uiUserID, const IVS_USER_DEVICE_INFO_LIST* pUserDevList, IVS_UINT32 uiBufferSize);

    /******************************************************************
    function		: IVS_SDK_GetUserDeviceList
    description: ��ȡ�û��豸�б�
    input			: iSessionID  ��¼�ɹ���ĻỰID
					  pDomainCode  �����
					  uiUserID  �û�ID
					  pIndexRange    ��ҳ��Ϣ
					  uiBufferSize    �����С
    output		: pUserDevList   �û��б�ṹ��
    return		: �ɹ�����0��IVS_SUCCEED����ʧ�ܷ��ش�����
    *******************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetUserDeviceList(IVS_INT32 iSessionID, 
		const IVS_CHAR* pDomainCode, IVS_UINT32 uiUserID, const IVS_INDEX_RANGE* pIndexRange, 
		IVS_USER_DEVICE_INFO_LIST* pUserDevList, IVS_UINT32 uiBufferSize);

	/**************************************************************************
    * name			: IVS_SDK_GetDeviceUserList
    * description	: ��ȡӵ��ָ���豸Ȩ�޵��û��б�
    * input			: iSessionID ��¼�ɹ���ĻỰID
						  pDomainCode  �����
						  pDevCode   �豸����
						  uiIsUserOnline  �û��Ƿ����ߣ�0-���� 1-���� 2-����
						  pIndexRange ��ҳ��Ϣ
						  uiBufferSize �����С
	* output		: pUserDevList �û��豸�б�ṹ
    * return		: �ɹ�����0��IVS_SUCCEED����ʧ�ܷ��ش�����
	* remark		: NA
    **************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL  IVS_SDK_GetDeviceUserList(IVS_INT32 iSessionID, 
		const IVS_CHAR* pDomainCode, const IVS_CHAR* pDevCode, IVS_UINT32 uiIsUserOnline, 
		const IVS_INDEX_RANGE* pIndexRange, IVS_DEVICE_USER_INFO_LIST* pDevUserList, IVS_UINT32 uiBufferSize);

	/******************************************************************
    function		: IVS_SDK_AddRole
    description: ���ӽ�ɫ
    input			: iSessionID  ��¼�ɹ���ĻỰID
					  pRoleInfo   ��ɫ��Ϣ�ṹ��
    output		: puiRoleID   ��ɫID
    return		: �ɹ�����0��IVS_SUCCEED����ʧ�ܷ��ش�����
    *******************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_AddRole(IVS_INT32 iSessionID, const IVS_ROLE_INFO* pRoleInfo, IVS_UINT32* puiRoleID);

	/******************************************************************
    function		: IVS_SDK_ModifyRole
    description: �޸Ľ�ɫ
    input			: iSessionID  ��¼�ɹ���ĻỰID
					  pRoleInfo   ��ɫ��Ϣ�ṹ��
    return		: �ɹ�����0��IVS_SUCCEED����ʧ�ܷ��ش�����
    *******************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_ModifyRole(IVS_INT32 iSessionID, const IVS_ROLE_INFO* pRoleInfo);

	/******************************************************************
    function		: IVS_SDK_DeleteRole
    description: ɾ����ɫ
    input			: iSessionID   ��¼�ɹ���ĻỰID
					  pDomainCode    �����
					  uiRoleID   ��ɫID
    return		: �ɹ�����0��IVS_SUCCEED����ʧ�ܷ��ش�����
    *******************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_DeleteRole(IVS_INT32 iSessionID, const IVS_CHAR* pDomainCode, IVS_UINT32 uiRoleID);

	/******************************************************************
    function		: IVS_SDK_GetRoleList
    description: ��ѯ��ɫ�б�
    input			: iSessionID  ��¼�ɹ���ĻỰID
					  pDomainCode   �����
					  pIndexRange    ��ҳ�����ṹ��
					  uiBufferSize    �����С
    output		: pRoleInfoList     ��ɫ�б�ṹ��
    return		: �ɹ�����0��IVS_SUCCEED����ʧ�ܷ��ش�����
    *******************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetRoleList(IVS_INT32 iSessionID, const IVS_CHAR* pDomainCode, const IVS_INDEX_RANGE* pIndexRange, IVS_ROLE_INFO_LIST* pRoleInfoList, IVS_UINT32 uiBufferSize);

	/******************************************************************
    function		: IVS_SDK_GetRoleInfoByRoleID
    description: ���ݽ�ɫID��ȡ��ɫ��ϸ��Ϣ
    input			: iSessionID   ��¼�ɹ���ĻỰID
					  pDomainCode    �����
    input			: uiRoleID     ��ɫID
    output		: pRoleInfo      ��ɫ��Ϣ�ṹ��
    return		: �ɹ�����0��IVS_SUCCEED����ʧ�ܷ��ش�����
    *******************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetRoleInfoByRoleID(IVS_INT32 iSessionID, const IVS_CHAR* pDomainCode, IVS_UINT32 uiRoleID, IVS_ROLE_INFO* pRoleInfo);

    /******************************************************************
    function		: IVS_SDK_GetRoleInfoByUserID
    description: �����û�ID��ȡ��ɫ��ϸ��Ϣ
    input			: iSessionID   ��¼�ɹ���ĻỰID
					  pDomainCode    �����
					  uiUserID     ��ɫID
    output		: pRoleInfo      ��ɫ��Ϣ�ṹ��
    return		: �ɹ�����0��IVS_SUCCEED����ʧ�ܷ��ش�����
    *******************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetRoleInfoByUserID(IVS_INT32 iSessionID, const IVS_CHAR* pDomainCode, IVS_UINT32 uiUserID, IVS_ROLE_INFO* pRoleInfo);

	/******************************************************************
    function		: IVS_SDK_GetDefaultRoleAccountRule
    description: ��ȡĬ���˻�����
    input			: iSessionID    ��¼�ɹ���ĻỰID
					  pDomainCode    �����
    output		: pRoleAccountRule   �˻�����
    return		: �ɹ�����0��IVS_SUCCEED����ʧ�ܷ��ش�����
    *******************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetDefaultRoleAccountRule(IVS_INT32 iSessionID, const IVS_CHAR* pDomainCode, IVS_ROLE_ACCOUNT_RULE* pRoleAccountRule);

    /******************************************************************
    function		: IVS_SDK_GetOperationRightGroup
    description: ��ȡ�û�Ȩ����Ϣ
    input			: iSessionID    ��¼�ɹ���ĻỰID
					  pDomainCode    �����
					  uiBufferSize     �����С
    output		: pRightGroupList   Ȩ�����б�
    return		: �ɹ�����0��IVS_SUCCEED����ʧ�ܷ��ش�����
    *******************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetOperationRightGroup(IVS_INT32 iSessionID, const IVS_CHAR* pDomainCode, IVS_OPERATION_RIGHT_GROUP_LIST* pRightGroupList, IVS_UINT32 uiBufferSize);

	/******************************************************************
    function		: IVS_SDK_SetLogPath
    description: ������־��Ŀ¼
    input			: pLogPath ��־��Ŀ¼
    return		: �ɹ�����0��IVS_SUCCEED����ʧ�ܷ��ش�����
    *******************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_SetLogPath(const IVS_CHAR* pLogPath);

	/******************************************************************
	function		: IVS_SDK_GetRecordStatus
	description: ��ѯ¼��״̬
	input			: iSessionID ��¼�ɹ��󷵻صĻỰID
					  pCameraCode ���������
					  uiRecordMethod ¼��ʽ
	output		: pRecordState ¼��״̬
	return		: �ɹ�����0��IVS_SUCCEED����ʧ�ܷ��ش�����
	*******************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetRecordStatus(IVS_INT32 iSessionID,const IVS_CHAR* pCameraCode,IVS_UINT32 uiRecordMethod,IVS_UINT32* pRecordState);

	/*****************************************************************
	function		: IVS_SDK_GetRecordTask
	description: ��ѯ��ǰ�û����ֶ�¼������
	input			: iSessionID    ��¼�ɹ���ĻỰID
	output		: pRspXml   ����¼��������ϢXML
	return		: �ɹ�����0��IVS_SUCCEED����ʧ�ܷ��ش�����
	*****************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetRecordTask(IVS_INT32 iSessionID, IVS_CHAR** pRspXml);

	/******************************************************************
    function		: IVS_SDK_AddUserGroup
    description	: �����û���
    input			: iSessionID    ��¼�ɹ���ĻỰID
					  pUserGroup   �û�����Ϣ
    output		: puiUserGroupID    �û���ID
    return		: �ɹ�����0��IVS_SUCCEED����ʧ�ܷ��ش�����
    *******************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_AddUserGroup(IVS_INT32 iSessionID, const IVS_USER_GROUP* pUserGroup, IVS_UINT32* puiUserGroupID);

    /******************************************************************
    function		: IVS_SDK_ModifyUserGroup
    description	: �޸��û���
    input			: iSessionID   ��¼�ɹ���ĻỰID
					  pUserGroup   �û�����Ϣ
    return		: �ɹ�����0��IVS_SUCCEED����ʧ�ܷ��ش�����
    *******************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_ModifyUserGroup(IVS_INT32 iSessionID, const IVS_USER_GROUP* pUserGroup);

    /******************************************************************
    function		: IVS_SDK_DeleteUserGroup
    description	: ɾ���û���
    input			: iSessionID    ��¼�ɹ���ĻỰID
					  pDomainCode    �����
					  uiUserGroupID    �û���ID
    return		: �ɹ�����0��IVS_SUCCEED����ʧ�ܷ��ش�����
    *******************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_DeleteUserGroup(IVS_INT32 iSessionID, const IVS_CHAR* pDomainCode, IVS_UINT32 uiUserGroupID);

    /******************************************************************
    function		: IVS_SDK_MoveUserGroup
    description	: �ƶ��û���
    input			: iSessionID   ��¼�ɹ���ĻỰID
					  pDomainCode   �����
					  uiTargetGroupID   Ŀ���û���ID
					  pUserGroupList  �û����б�
    return		: �ɹ�����0��IVS_SUCCEED����ʧ�ܷ��ش�����
    *******************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_MoveUserGroup(IVS_INT32 iSessionID, const IVS_CHAR* pDomainCode, IVS_UINT32 uiTargetGroupID, const IVS_USER_GROUP_LIST* pUserGroupList);

	/******************************************************************
    function		: IVS_SDK_GetUserGroupList
    description: ��ѯ�û����б�
    input			: iSessionID    ��¼�ɹ���ĻỰID
					  pDomainCode   �����
					  pIndexRange     ��ҳ�����ṹ��
					  uiBufferSize    �����С
    output		: pUserGroupList   �û����б�ṹ��
    return		: �ɹ�����0��IVS_SUCCEED����ʧ�ܷ��ش�����
    *******************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetUserGroupList(IVS_INT32 iSessionID, const IVS_CHAR* pDomainCode, 
		const IVS_INDEX_RANGE* pIndexRange, IVS_USER_GROUP_LIST* pUserGroupList, IVS_UINT32 uiBufferSize);

	/**************************************************************************
    * name			: IVS_SDK_OMUQueryRaidGroup
    * description	: ��������Ϣ��ѯ
    * input			: iSessionID ��¼�ɹ���ĻỰID
	* output		: pRspXml  ��ӦXML
    * return		: �ɹ�����0��IVS_SUCCEED����ʧ�ܷ��ش�����
    **************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_OMUQueryRaidGroup(IVS_INT32 iSessionID, IVS_CHAR** pRspXml);

	/**************************************************************************
    * name			: IVS_SDK_OMUQueryRaidGroupDisk
    * description	: ������Ϣ��ѯ
    * input			: iSessionID ��¼�ɹ���ĻỰID
						  pReqXml ����XML
	* output		: pRspXml  ��ӦXML
    * return		: �ɹ�����0��IVS_SUCCEED����ʧ�ܷ��ش�����
    **************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_OMUQueryRaidGroupDisk(IVS_INT32 iSessionID, const IVS_CHAR* pReqXml, IVS_CHAR** pRspXml);

	/******************************************************************
	function		: IVS_SDK_SetDevPWD
	description: ����ǰ���豸����
	input			: iSessionID  ��¼�ɹ��󷵻صĻỰID��
					  pDevCode	���豸���룻
					  pDevUserName   �豸��¼�û���
					  pDevPWD  �豸��¼����(��Ӧ�豸��¼�û���������)
					  pDevRegPWD  �豸��ƽ̨ע������(ƽ̨�����豸ע��ʱ��У������)
	output		: NA
	return		: �ɹ�����0��IVS_SUCCEED����ʧ�ܷ��ش�����
	*******************************************************************/
   IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_SetDevPWD(IVS_INT32 iSessionID, 
	   const IVS_CHAR* pDevCode, const IVS_CHAR* pDevUserName, const IVS_CHAR* pDevPWD, const IVS_CHAR* pDevRegPWD);

	/**************************************************************************
    * name			: IVS_SDK_StartPlatformRecord
    * description	: ��ʼƽ̨¼��
    * input			: iSessionID      ��¼�ɹ���ĻỰID
						  pCameraCode     ��ʼƽ̨¼������������
						  iRecordTime     ¼��ʱ��
    * return		: �ɹ�����0��IVS_SUCCEED����ʧ�ܷ��ش�����
    * remark		: NA
	**************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StartPlatformRecord(IVS_INT32 iSessionID,const IVS_CHAR* pCameraCode,IVS_INT32 iRecordTime);

	/**************************************************************************
    * name			: IVS_SDK_StopPlatformRecord
    * description	: ֹͣƽ̨¼��
    * input			: iSessionID      ��¼�ɹ���ĻỰID
						  pCameraCode     ֹͣƽ̨¼������������
    * return		: �ɹ�����0��IVS_SUCCEED����ʧ�ܷ��ش�����
    * remark		: NA
	**************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StopPlatformRecord(IVS_INT32 iSessionID,const IVS_CHAR* pCameraCode);

	/**************************************************************************
    * name			: IVS_SDK_StartRealPlayByIPEx
    * description	: ��ʼʵʱ���
    * input			: iSessionID ��¼�ɹ���ĻỰID
						  pRealplayParam  ���Ų���
						  pCameraCode  ���������
						  pMediaAddrDst  ý����Ŀ���ַ
	* output		: pMediaAddrSrc ý����Դ��ַ
						  pHandle ���ž����Ψһָ���˲���ͨ����
    * return		: �ɹ�����0��IVS_SUCCEED����ʧ�ܷ��ش�����
    **************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StartRealPlayByIPEx(IVS_INT32 iSessionID, 
															const IVS_REALPLAY_PARAM  *pRealplayParam,
															const IVS_CHAR* pCameraCode,
															const IVS_MEDIA_ADDR* pMediaAddrDst,
															IVS_MEDIA_ADDR* pMediaAddrSrc,
															IVS_ULONG* pHandle);

	/**************************************************************************
    * name			: IVS_SDK_StopRealPlayByIPEx
    * description	: ֹͣʵʱ���
    * input			: iSessionID ��¼�ɹ���ĻỰID
						  ulHandle ���ž����Ψһָ���˲���ͨ����
	* output		: NA
    * return		: �ɹ�����0��IVS_SUCCEED����ʧ�ܷ��ش�����
    **************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StopRealPlayByIPEx(IVS_INT32 iSessionID, IVS_ULONG ulHandle);

	/**************************************************************************
    * name			: IVS_SDK_StartPlatformPlayBackByIP
    * description	: ��ʼ�ͻ���ƽ̨¼��ط�
    * input			: iSessionID      �ỰID
						  pCameraCode     ���������
						  pPlaybackParam  �طŲ���
						  pMediaAddrDst  ý����Ŀ���ַ
	* output		: pMediaAddrSrc ý����Դ��ַ
						  pHandle ���ž��
    * return		: �ɹ�����0��IVS_SUCCEED����ʧ�ܷ��ش�����
    * remark		: NA
    **************************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StartPlatformPlayBackByIP(IVS_INT32 iSessionID,
																	const IVS_CHAR* pCameraCode,
																	const IVS_PLAYBACK_PARAM* pPlaybackParam,
																	const IVS_MEDIA_ADDR* pMediaAddrDst,
																	IVS_MEDIA_ADDR* pMediaAddrSrc,
																	IVS_ULONG* pHandle);

   /**************************************************************************
    * name			: IVS_SDK_StopPlatformPlayBackByIP
    * description	: ֹͣ�ͻ���ƽ̨¼��ط�
    * input			: iSessionID      �ỰID
						  ulPlayHandle    ���ž��
    * output		: NA
    * return		: �ɹ�����0��IVS_SUCCEED����ʧ�ܷ��ش�����
    * remark		: NA
    **************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StopPlatformPlayBackByIP(IVS_INT32 iSessionID,IVS_ULONG ulPlayHandle);

/**************************************************************************
    * name			: IVS_SDK_PlatformPlayBackPauseByIP
    * description: ��ͣƽ̨¼��ط�
    * input			: iSessionID	�ỰID
							: ulPlayHandle	���ž��
    * output			: NA
    * return			: �ɹ�����0��IVS_SUCCEED����ʧ�ܷ��ش�����
    * remark		: NA
    **************************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_PlatformPlayBackPauseByIP(IVS_INT32 iSessionID,
																	IVS_ULONG ulPlayHandle);

	/**************************************************************************
    * name			: IVS_SDK_PlatformPlayBackResumeByIP
    * description: �ָ�ƽ̨¼��ط�
    * input			: iSessionID	�ỰID
							: ulPlayHandle	���ž��
    * output			: NA
    * return			: �ɹ�����0��IVS_SUCCEED����ʧ�ܷ��ش�����
    * remark		: NA
    **************************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_PlatformPlayBackResumeByIP(IVS_INT32 iSessionID,
																	IVS_ULONG ulPlayHandle);

	 /**************************************************************************
    * name			: IVS_SDK_SetPlayBackSpeedByIP
    * description: ����ƽ̨¼�������ط��ٶ�
    * input			: iSessionID	�ỰID
							: ulPlayHandle	���ž��
							: fSpeed	�����ٶ�
    * output			: NA
    * return			: �ɹ�����0��IVS_SUCCEED����ʧ�ܷ��ش�����
    * remark		: NA
    **************************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_SetPlayBackSpeedByIP(IVS_INT32 iSessionID, 
		IVS_ULONG ulPlayHandle, 
		IVS_FLOAT fSpeed);

	 /**************************************************************************
    * name			: IVS_SDK_SetPlayBackTimeByIP
    * description: ����ƽ̨¼�������ط�ʱ��
    * input			: iSessionID	�ỰID
							: ulPlayHandle	���ž��
							: uiTime	�϶��߶ȣ���λ�룬������������ʱ��
    * output			: NA
    * return			: �ɹ�����0��IVS_SUCCEED����ʧ�ܷ��ش�����
    * remark		: NA
    **************************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_SetPlayBackTimeByIP(IVS_INT32 iSessionID, 
		IVS_ULONG ulPlayHandle, 
		IVS_UINT32 uiTime);
		
	/******************************************************************
	function		: IVS_SDK_PtzControlWithLockerInfo
	description: �ƾ�����
	input			: iSessionID ��¼�ɹ��󷵻صĻỰID
					  pCameraCode ���������
					  iControlCode ��̨������
					  pControlPara1 ����1
					  pControlPara2 ����2
	output		: pPtzControlInfo ��̨������Ϣ
	return		: �ɹ�����0��IVS_SUCCEED����ʧ�ܷ��ش�����
	*******************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_PtzControlWithLockerInfo(IVS_INT32 iSessionID,
															  const IVS_CHAR* pCameraCode, 
															  IVS_INT32 iControlCode,
															  const IVS_CHAR* pControlPara1,
															  const IVS_CHAR* pControlPara2, 
															  IVS_PTZ_CONTROL_INFO* pPtzControlInfo);

	/**************************************************************************
    * name			: IVS_SDK_GetOperationLog
    * description	: ��ѯ������־
    * input			: iSessionID ��¼�ɹ���ĻỰID
						  pUnifiedQuery ͨ�ò�ѯ�ṹ��
						  uiBufSize �����С
    * output			: pResOperLogList ������־
    * return			: �ɹ�����0��IVS_SUCCEED����ʧ�ܷ��ش�����
	* remark			: NA
    **************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetOperationLog(IVS_INT32 iSessionID,
		const IVS_QUERY_UNIFIED_FORMAT* pUnifiedQuery,IVS_OPERATION_LOG_LIST* pResOperLogList,IVS_UINT32 uiBufSize);


/******************************************************************
    function			: IVS_SDK_OMUQueryDiskProperty
    description	: ��ѯ������Ϣ�ӿ�
    input				: iSessionID ��¼�ɹ���ĻỰID
    output			: pRspXml ���XML
	return				: �ɹ�����0��ʧ�ܷ��ش�����
    remark			: pRspXml��SDK�ڲ������ڴ棬ʹ����Ӧ�ͷš�
    return				: �ɹ�����0��IVS_SUCCEED����ʧ�ܷ��ش�����
    *******************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_OMUQueryDiskProperty(IVS_INT32 iSessionID, 
		IVS_CHAR** pRspXml);
		
	/**************************************************************************
    * name			: IVS_SDK_GetVideoDiagnose
    * description: ��ѯ��̨VCN����Ƶ������ϼƻ�
    * input			: iSessionID ��¼�ɹ���ĻỰID
	* input			: pDomainCode �����
	* input			: pCameraCode ���������
	* output			: pRspXml ��̨VCN����Ƶ������ϼƻ�xml
    * return			: �ɹ�����0��IVS_SUCCEED����ʧ�ܷ��ش�����
	* remark		: pRspXml��SDK�ڲ������ڴ棬ʹ����Ӧ����IVS_SDK_ReleaseBuffer�ͷš�
	* history			: 2014-6-9
    **************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetVideoDiagnose(IVS_INT32 iSessionID,
		const IVS_CHAR* pDomainCode,
		const IVS_CHAR* pCameraCode,
		IVS_CHAR** pRspXml);

	/**************************************************************************
    * name			: IVS_SDK_SetVideoDiagnose
    * description: ���õ�̨VCN����Ƶ������ϼƻ�
    * input			: iSessionID ��¼�ɹ���ĻỰID
	* input			: pReqXml ��̨VCN����Ƶ������ϼƻ�xml
	* output			: NA
    * return			: �ɹ�����0��IVS_SUCCEED����ʧ�ܷ��ش�����
	* remark		: NA
	* history			: 2014-6-9
    **************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_SetVideoDiagnose(IVS_INT32 iSessionID, 
		const IVS_CHAR* pReqXml);

	/**************************************************************************
    * name			: IVS_SDK_GetStreamListByCam
    * description: ���������Code����ѯ����������ڸ���Щ�û��ṩ��Ƶҵ��
    * input			: iSessionID ��¼�ɹ���ĻỰID
	* input			: pDomainCode �����
	* input			: pCameraCode ���������
	* input			: uiBufSize �����ڴ��С
	* output			: pStreamListByCam �����������ý��ҵ���б�
    * return			: �ɹ�����0��IVS_SUCCEED����ʧ�ܷ��ش�����
	* remark		: C/C++��uiBufSize�̶�Ϊsizeof(IVS_STREAM_LIST_BY_CAM) + 
								(IVS_MAX_STREAM_USER_INFO_NUM -1) * sizeof(IVS_STREAM_USER_INFO)
	* history			: 2014-6-10
    **************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetStreamListByCam(IVS_INT32 iSessionID, 
		const IVS_CHAR* pDomainCode,
		const IVS_CHAR* pCameraCode,
		IVS_UINT32 uiBufSize,
		IVS_STREAM_LIST_BY_CAM* pStreamListByCam);

	/**************************************************************************
    * name			: IVS_SDK_GetStreamListByUser
    * description: �����û����룬��ѯ���û����ڸ�������Щ���������Ƶҵ��
    * input			: iSessionID ��¼�ɹ���ĻỰID
	* input			: pDomainCode ����ѯ���û����������
	* input			: uiUserID ����ѯ���û�ID
	* input			: uiBufSize �����ڴ��С
	* output			: pStreamListByUser �û����ڵ��õ�ý��ҵ���б�
    * return			: �ɹ�����0��IVS_SUCCEED����ʧ�ܷ��ش�����
	* remark		: C/C++��uiBufSize�̶�Ϊsizeof(IVS_STREAM_LIST_BY_USER) + 
								(IVS_MAX_STREAM_CAM_INFO_NUM -1) * sizeof(IVS_STREAM_CAM_INFO)
	* history			: 2014-6-24
    **************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetStreamListByUser(IVS_INT32 iSessionID, 
		const IVS_CHAR* pDomainCode,
		IVS_UINT32 uiUserID,
		IVS_UINT32 uiBufSize,
		IVS_STREAM_LIST_BY_USER* pStreamListByUser);

	/**************************************************************************
    * name			: IVS_SDK_ShutdownStreamByUser
    * description: ���������Code���û�ID���رո��û�����ʹ�õ���Ƶҵ��
    * input			: iSessionID ��¼�ɹ���ĻỰID
	* input			: pDomainCode �����
	* input			: uiUserID �û�ID
	* input			: pCameraCode ���������
	* input			: iStreamType ��Ƶҵ������ 1-ʵ����2-�ط�
	* input			: iDuration �ر�ʱ�����ڴ�ʱ��֮�ڸ��û������ٴε㲥����λΪ�룬0Ϊ�����ƹر�ʱ��
	* output			: NA
    * return			: �ɹ�����0��IVS_SUCCEED����ʧ�ܷ��ش�����
	* remark		: NA
	* history			: 2014-6-10
    **************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_ShutdownStreamByUser(IVS_INT32 iSessionID,
		const IVS_CHAR* pDomainCode,
		IVS_UINT32 uiUserID,
		const IVS_CHAR* pCameraCode,
		IVS_INT32 iStreamType,
		IVS_INT32 iDuration);
		
	/**************************************************************************
    * name			: IVS_SDK_PasswdValidation
    * description: �û�����������֤
    * input			: iSessionID ��¼�ɹ���ĻỰID
	* input			: pLoginName ��¼�û���
	* input			: pPasswd ����
	* output			: NA
    * return			: �ɹ�����0��IVS_SUCCEED����ʧ�ܷ��ش�����
	* remark		: NA
	* history			: 2014-6-27
    **************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_PasswdValidation(IVS_INT32 iSessionID,
		const IVS_CHAR* pLoginName,
		const IVS_CHAR *pPasswd);
		
	/**************************************************************************
    * name			: IVS_SDK_GetDomainDeviceList
    * description: ��ȡָ�������������豸�б�
	* input			: iSessionID ��¼�ɹ���ĻỰID
    * input			: pDomainCode ����룬Ϊ��Ҫ��ѯ�豸����ı���
	* input			: uiDeviceType �豸���ͣ�ֵ�ο�IVS_DEVICE_TYPE��֧�ֳ����豸������澯�豸��������豸
	* input			: pIndexRange ��ҳ��Ϣ
	* output			: pDeviceList �豸�б�ָ��
	* input			: uiBufferSize �����С
    * return			: �ɹ�����0��IVS_SUCCEED����ʧ�ܷ��ش�����
	* remark		: NA
	* history			: 2014-9-9
    **************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetDomainDeviceList(IVS_INT32 iSessionID,
		const IVS_CHAR* pDomainCode,
		IVS_UINT32 uiDeviceType,
		const IVS_INDEX_RANGE* pIndexRange,
		IVS_VOID* pDeviceList,
		IVS_UINT32 uiBufferSize);
		
#ifdef __cplusplus
}
#endif //__cplusplus

#endif //_IVS_SDK_H_

