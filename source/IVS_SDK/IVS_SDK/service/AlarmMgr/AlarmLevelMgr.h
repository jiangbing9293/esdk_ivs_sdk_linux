/********************************************************************
filename	: 	AlarmLevelMgr.h
author		:	pkf57481
created		:	2012/11/7
description	:	�澯�������;
copyright	:	Copyright (C) 2011-2015
history		:	2012/11/7 ��ʼ�汾;
*********************************************************************/

#ifndef __ALARMLEVEL_MGR_H
#define __ALARMLEVEL_MGR_H

#include "hwsdk.h"
class CUserMgr;
class CAlarmLevelMgr
{
public:
	CAlarmLevelMgr(void);
	~CAlarmLevelMgr(void);
	void SetUserMgr(CUserMgr *pUserMgr);
public:
	//�����澯����
	IVS_INT32 AddAlarmLevel(const IVS_ALARM_LEVEL *pAlarmLevel,IVS_UINT32* pAlarmLevelID) const;

	//�޸ĸ澯����
	IVS_INT32 ModifyAlarmLevel(const IVS_ALARM_LEVEL *pAlarmLevel) const;

	//ɾ���澯����
	IVS_INT32 DeleteAlarmLevel(const IVS_UINT32 uiAlarmLevelId) const;

	//��ѯ�澯�����б�
	IVS_INT32 GetAlarmLevelList(IVS_UINT32 uiMaxAlarmLevelNum, IVS_UINT32* puiAlarmLevelNum, IVS_ALARM_LEVEL* pAlarmLevel, IVS_UINT32 /*uiBufferSize*/) const;

	//��ѯ�澯��������
	IVS_INT32 GetAlarmLevel(IVS_UINT32 uiAlarmLevelID,IVS_ALARM_LEVEL* pAlarmLevel) const;


private:
	CAlarmLevelMgr* m_AlarmLevelMgr;
	CUserMgr* m_UserMgr;
};
#endif
