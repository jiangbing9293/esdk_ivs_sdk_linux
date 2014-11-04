/********************************************************************
filename	: 	AlarmMgr.h
author		:	pkf57481
created		:	2012/12/7
description	:	�澯����;
copyright	:	Copyright (C) 2011-2015
history		:	2012/12/7 ��ʼ�汾;
*********************************************************************/

#ifndef __ALARMLINKAGE_MGR_H__
#define __ALARMLINKAGE_MGR_H__

#include "hwsdk.h"

class CUserMgr;
class CAlarmLinkageMgr
{
public:
	CAlarmLinkageMgr();
	~CAlarmLinkageMgr();

	void SetUserMgr(CUserMgr *pUserMgr);
public:
	// �����澯��������
	IVS_INT32 AddAlarmLinkage(const IVS_CHAR* pReqXml,IVS_CHAR** pRspXml);

	// �޸ĸ澯��������
	IVS_INT32 ModifyAlarmLinkage(const IVS_CHAR* pReqXml);

	// ɾ���澯��������
	IVS_INT32 DeleteAlarmLinkage(const IVS_CHAR* pReqXml);

	// �鿴�澯���������б�
	IVS_INT32 GetAlarmlinkageList(const IVS_CHAR* pReqXml, IVS_CHAR** pRspXml);

	// ��ѯ�澯��������
	IVS_INT32 GetAlarmLinkage(const IVS_CHAR* pReqXml,IVS_CHAR** pRspXml);

	// ��ѯ�澯������������
	IVS_INT32 GetAlarmLinkageAction(const IVS_CHAR* pReqXml, IVS_CHAR** pRspXml);
private:
	CUserMgr *m_pUserMgr;
};

#endif //__ALARMLINKAGE_MGR_H__
