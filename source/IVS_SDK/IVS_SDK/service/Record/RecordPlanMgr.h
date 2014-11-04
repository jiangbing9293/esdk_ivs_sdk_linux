/********************************************************************
	filename	: 	RecordPlanMgr.h
	author		:	z00201790
	created		:	2012/11/05	
	description	:	����¼��ƻ�������;
	copyright	:	Copyright (C) 2011-2015
	history		:	2012/11/05 ��ʼ�汾;
*********************************************************************/

#ifndef __RECORD_PLAN_MGR_H__
#define __RECORD_PLAN_MGR_H__

#include "RecordXMLProcess.h"


class CUserMgr;
class CRecordPlanMgr
{
public:
    CRecordPlanMgr(void);
    ~CRecordPlanMgr(void);

    void SetUserMgr(CUserMgr *pUserMgr);

public:
	/******************************************************************
	function : AddRecordPlan
	description: ���ƽ̨¼��ƻ�;
	input : uiDeviceNum,�����������pDeviceList���������ţ�pRecordPlan��¼��ƻ����ݣ�
	pResultList��¼��ƻ��������������uiResultBufferSize������ڴ��СУ��ֵ
	output : NA
	return : �ɹ�-0��ʧ��-1
	*******************************************************************/
	IVS_INT32 AddRecordPlan(IVS_UINT32 uiDeviceNum,const IVS_DEVICE_CODE* pDeviceList,
		const IVS_RECORD_PLAN* pRecordPlan,IVS_RESULT_LIST* pResultList,IVS_UINT32 uiResultBufferSize)const;

	/******************************************************************
	function : ModifyRecordPlan
	description: �޸�ƽ̨¼��ƻ�;
	input : uiDeviceNum,�����������pDeviceList���������ţ�pRecordPlan��¼��ƻ����ݣ�
	pResultList��¼��ƻ������޸Ľ����uiResultBufferSize������ڴ��СУ��ֵ
	output : NA
	return : �ɹ�-0��ʧ��-1
	*******************************************************************/
	IVS_INT32 ModifyRecordPlan(IVS_UINT32 uiDeviceNum,const IVS_DEVICE_CODE* pDeviceList,
		const IVS_RECORD_PLAN* pRecordPlan,IVS_RESULT_LIST* pResultList,IVS_UINT32 uiResultBufferSize)const;

	/******************************************************************
	function : DeleteRecordPlan
	description: ɾ��ƽ̨¼��ƻ�;
	input : uiDeviceNum,�����������pDeviceList���������ţ�uiRecordMethod��¼��ƻ���ʽ��
	pResultList��¼��ƻ�����ɾ�������uiResultBufferSize������ڴ��СУ��ֵ
	output : NA
	return : �ɹ�-0��ʧ��-1
	*******************************************************************/
	IVS_INT32 DeleteRecordPlan(IVS_UINT32 uiDeviceNum,const IVS_DEVICE_CODE* pDeviceList,
		IVS_UINT32 uiRecordMethod,IVS_RESULT_LIST* pResultList,IVS_UINT32 uiResultBufferSize)const;

	/******************************************************************
	function : GetRecordPlan
	description: ��ѯƽ̨¼��ƻ�;
	input : pCameraCode,�������ţ�uiRecordMethod��¼��ƻ���ʽ��pRecordPlan��¼��ƻ���ѯ�����
	output : NA
	return : �ɹ�-0��ʧ��-1
	*******************************************************************/
	IVS_INT32 GetRecordPlan(const char* pCameraCode,IVS_UINT32 uiRecordMethod,
		IVS_RECORD_PLAN* pRecordPlan);

    /******************************************************************
    function : GetCameraPlanInfo
    description: ��ѯ������ƻ�������Ϣ
    input : IVS_UINT32 uiPlanType       ¼������
    input : IVS_UINT32 uiDeviceNum      ���������
    output : IVS_CHAR** pRspXm          ��Ӧ��Ϣ
    return : �ɹ�-0��ʧ��-1
    *******************************************************************/
    IVS_INT32 GetCameraPlanInfo(IVS_UINT32 uiPlanType, IVS_CHAR** pRspXml);

	/******************************************************************
	function : SetWaterMarkState
	description: ���ÿ������ر�ˮӡУ��;
	input : iHandle,�����bIsStart���Ƿ���
	output : NA
	return : �ɹ�-0��ʧ��-1
	*******************************************************************/
	IVS_INT32 SetWaterMarkState(IVS_INT32 /*iHandle*/,IVS_BOOL /*bIsStart*/)const;

private:
    CUserMgr *m_UserMgr;
};

#endif
