/********************************************************************
filename	: 	AlarmMgr.h
author		:	pkf57481
created		:	2012/11/1
description	:	�澯����;
copyright	:	Copyright (C) 2011-2015
history		:	2012/11/1 ��ʼ�汾;
*********************************************************************/

#ifndef __ALARM_MGR_H__
#define __ALARM_MGR_H__

#include "hwsdk.h"

class CUserMgr;
class CAlarmMgr
{
public:
	CAlarmMgr(void);
	~CAlarmMgr(void);

public:
    void SetUserMgr(CUserMgr *pUserMgr);

	// �鿴�澯�����б�
	IVS_INT32 SubscribeQuery(const IVS_CHAR* pReqXml, IVS_CHAR** pRspXml) const;

	// �������澯����
	IVS_INT32 SubscribeAlarm(const IVS_CHAR* pReqXml) const;

	// ��ѯ�Զ���澯�����б�
    IVS_INT32 GetAlarmTypeList(const IVS_QUERY_UNIFIED_FORMAT* pUnifiedQuery,
        IVS_ALARM_TYPE_LIST* pAlarmTypeList, IVS_UINT32 uiBufferSize);

	// ���ø澯�����б�
    IVS_INT32 SetAlarmTypeLevel(const IVS_ALARM_TYPE_LIST* pAlarmTypeList);
	
	// �澯ȷ��
	IVS_INT32 ConfirmAlarm(IVS_UINT64 ullAlarmEventID, const IVS_CHAR* pAlarmInCode, const IVS_ALARM_OPERATE_INFO* pAlarmOperateInfo);

	//ˮӡ�澯ȷ��
	IVS_INT32 ConfirmWaterAlarm(const IVS_WATERMARK_ALARM_CONFIRM* pConfirmInfo);

	// �澯����
	IVS_INT32 CancelAlarm(IVS_UINT64 ullAlarmEventID, const IVS_CHAR* pAlarmInCode, const IVS_ALARM_OPERATE_INFO* pAlarmOperateInfo);

	//ˮӡ�澯����
	IVS_INT32 CancelWaterAlarm(const IVS_WATERMARK_ALARM_CANCEL* pstConfirmInfo);

	// �澯��Ȩ
	IVS_INT32 AlarmCommission(const IVS_CHAR* pAlarmInCode, const IVS_ALARM_COMMISSION* pAlarmCommission);

	// ��Ȩ�澯�û���֤
	IVS_INT32 UserAlarmAuthorization(const IVS_CHAR *pPWD) const;

	// �澯�ϱ���CU->SMU��
	IVS_INT32 AlarmReport(const IVS_ALARM_REPORT* pAlarmReport);

	//�������澯�ϱ�
	IVS_INT32 ManualAlarm(const IVS_CHAR* /*pReq*/, IVS_CHAR* &/*pRsp*/);

	//��������������澯
	IVS_INT32 StartAlarmOut(const IVS_CHAR* pAlarmOutCode);

	// ֹͣ����������澯
	IVS_INT32 StopAlarmOut(const IVS_CHAR* pAlarmOutCode);

	// ��ѯ�澯��Ϣ
	IVS_INT32 GetAlarmInfo(IVS_UINT64 ullAlarmEventID, const IVS_CHAR* pAlarmInCode, IVS_ALARM_EVENT* pAlarmEvent);

	// ��ѯ��ʷ�澯��Ϣ�б�
	IVS_INT32 GetAlarmList(const IVS_QUERY_UNIFIED_FORMAT* pUnifiedQuery, IVS_ALARM_EVENT_LIST* pAlarmEventList, IVS_UINT32 uiBufferSize);

	// ��ѯˮӡ�澯��Ϣ�б�
	IVS_INT32 QueryWatermarkAlarmList(const IVS_QUERY_UNIFIED_FORMAT *pQueryFormat, IVS_VOID* pBuffer,IVS_UINT32 uiBufferSize);

	// ��ȡӵ��ָ���豸Ȩ�޵��û��б�
	IVS_INT32 GetDeviceUserList(const IVS_CHAR* pDomainCode, const IVS_CHAR* pDevCode, IVS_UINT32 uiIsUserOnline, const IVS_INDEX_RANGE* pIndexRange, IVS_DEVICE_USER_INFO_LIST* pDevUserList, IVS_UINT32 uiBufferSize);

	// ��ȡƽ̨������Ϣ
	IVS_INT32 GetConfigInfo(const IVS_CHAR* pReqXml, IVS_CHAR** pRspXml);

	// ��ѯƽ̨��������Ϣ
	IVS_INT32 GetAbilityInfo (IVS_UINT32* uiAbility,IVS_CHAR** pRspXml);

	// ��ѯOMU�澯�¼��б�
	IVS_INT32 GetDeviceAlarmList(const IVS_QUERY_UNIFIED_FORMAT* pUnifiedQuery, IVS_DEVICE_ALARM_EVENT_LIST* pAlarmEventList, IVS_UINT32 uiBufferSize);

	// �豸������
	IVS_INT32 SetDeviceGuard(const IVS_CHAR* pReqXml);

	// ��ȡ�澯ץ��ͼƬ�б�
	IVS_INT32 GetAlarmSnapshotList(const IVS_CHAR* pReqXml, IVS_CHAR** pRsqXml);

	// ������λ
	IVS_INT32 ResetAlarm(const IVS_CHAR* pReqXml);

	// �ⲿ���豸�¼�����
	IVS_INT32 ExEventSubscribe(const IVS_CHAR* pReqXml);

	// ��ѯ�ⲿ���豸�¼�����
	IVS_INT32 GetExEventSubscribe(const IVS_CHAR* pReqXml, IVS_CHAR** pRsqXml);

	// �ⲿ���豸Ŀ¼����
	IVS_INT32 ExCatalogSubscribe(const IVS_CHAR* pReqXml);

	// ��ȡ�ⲿ���豸Ŀ¼����
	IVS_INT32 GetExCatalogSubscribeList(const IVS_CHAR* pReqXml, IVS_CHAR** pRsqXml);
private:
	IVS_INT32 GetDevCodeListInfo(const IVS_CHAR *pDevBigCode,std::list<std::string>& lDevCodeList);
 private:
 	CUserMgr *m_pUserMgr;
};
#endif // __ALARM_MGR_H__
