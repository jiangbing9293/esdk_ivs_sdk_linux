/********************************************************************
filename	: 	AlarmAreaMgr.h
author		:	pkf57481
created		:	2012/11/7
description	:	�澯��������;
copyright	:	Copyright (C) 2011-2015
history		:	2012/11/7 ��ʼ�汾;
*********************************************************************/

#ifndef __ALARMAREA_MGR_H__
#define __ALARMAREA_MGR_H__

#include "hwsdk.h"
#include "ivs_error.h"
#include "SDKDef.h"

class CUserMgr;
class CAlarmAreaMgr
{
public:
	CAlarmAreaMgr(void);
	~CAlarmAreaMgr(void);

	void SetUserMgr(CUserMgr *pUserMgr);

public:
	// ��������
	IVS_INT32 AddAlarmArea(const IVS_CHAR* pReqXml,IVS_CHAR** pRspXml)  const;

	//�޸ķ���
	IVS_INT32 ModifyAlarmArea(const IVS_CHAR* pReqXml) const;

	//ɾ������
	IVS_INT32 DeleteAlarmArea(const IVS_CHAR* pDomainCode, IVS_UINT32 uiAlarmAreaId)const;

	//�鿴�����б�
	IVS_INT32 GetAlarmAreaList(const IVS_CHAR* pReqXml, IVS_CHAR** pRspXml)const;

	// �鿴��������
	IVS_INT32 GetAlarmAreaInfo(const IVS_CHAR* pDomainCode,IVS_UINT32 uiAlarmAreaId,IVS_CHAR** pAlarmArea)const;

	// �ֶ�����
	IVS_INT32 StartAlarmAreaGuard(const IVS_CHAR* pDomainCode,IVS_UINT32 uiAlarmAreaId)const;

	// �ֶ�����
	IVS_INT32 StopAlarmAreaGuard(const IVS_CHAR* pDomainCode, IVS_UINT32 uiAlarmAreaId)const;

	// ���������ƻ�
	IVS_INT32 AllocAreaGuardPlan(const IVS_CHAR* pReqXml)const;

	// �޸Ĳ����ƻ�
	IVS_INT32 ModifyAreaGuardPlan(const IVS_CHAR* pReqXml)const;

	// ɾ�������ƻ�
	IVS_INT32 DeleteAreaGuardPlan(const IVS_CHAR* pDomainCode,IVS_UINT32 uiAlarmAreaId)const;

	// �鿴�����ƻ�
	IVS_INT32 GetAreaGuardPlan(const IVS_CHAR* pDomainCode, IVS_UINT32 uiAlarmAreaId, IVS_CHAR** pRspXml)const;

private:
	CUserMgr *m_pUserMgr;
};
#endif // __ALARMAREA_MGR_H__
