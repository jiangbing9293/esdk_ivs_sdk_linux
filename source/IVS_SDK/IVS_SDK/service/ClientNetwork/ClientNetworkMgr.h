/********************************************************************
filename : ClientNetworkMgr.h
author : pkf57481
created : 2013/01/22
description : ?????
copyright : Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history : 2013/01/22 ????
*********************************************************************/


#ifndef __CLIENTNETWORK_MGR_H_
#define __CLIENTNETWORK_MGR_H_

#include "hwsdk.h"
#include "IVSCommon.h"
#include "nss_mt.h"
class CUserMgr;
class CClientNetworkMgr
{
public:
    CClientNetworkMgr(void);
    ~CClientNetworkMgr(void);

    void SetUserMgr(CUserMgr *pUserMgr);

    // ��ѯ��Ԫ��
	IVS_INT32 GetDomainList(IVS_CHAR** pDomainList)const;

	// ��ԪCPU/�ڴ����ܲ�ѯ
	IVS_INT32 GetCPUMemory(const IVS_CHAR* pNodeCode, IVS_CHAR** pPerformance)const;

	// ���ܷ���CPU/�ڴ�����
	IVS_INT32 GetIACPUMemory(const IVS_CHAR* pNodeCode, IVS_CHAR** pPerformance)const;

	//	��Ԫ���ܲ�ѯ
	IVS_INT32 QueryPerformance(const IVS_CHAR* pNodeCode,	IVS_CHAR** pPerformance)const;

	// �����澯֪ͨ����ӿ�
	IVS_INT32 AddAlarmLinkageRule(const IVS_CHAR* pReqXml, IVS_CHAR** pRspXml)const;

	// �޸ĸ澯֪ͨ����ӿ�
	IVS_INT32 ModAlarmLinkageRule(const IVS_CHAR* pReqXml)const;

	// ɾ���澯֪ͨ����ӿ�
	IVS_INT32 DelAlarmLinkageRule(const IVS_CHAR* pReqXml)const;

	// ��ȡ�澯֪ͨ�����б�ӿ�
	IVS_INT32 GetAlarmLinkageRuleList(const IVS_CHAR* pReqXml, IVS_CHAR** pRspXml)const;

	// ��ȡ�澯֪ͨ��������ӿ�
	IVS_INT32 GetAlarmLinkageRuleDetail(const IVS_CHAR* pReqXml, IVS_CHAR** pRspXml)const;

	// ������Ԫ��ȡ�豸���ͽӿ�
	IVS_INT32 GetDevTypeByNodeCode(const IVS_CHAR* pReqXml, IVS_CHAR** pRspXml)const;

	// �ͻ����������
	IVS_INT32 CheckNewVersion(const IVS_CHAR* pReqXml, IVS_CHAR** pRspXml)const;

	//��������Ϣ��ѯ
	IVS_INT32 QueryRaidGroup(const IVS_CHAR* pReqXml, IVS_CHAR** pRspXml)const;

	//������Ϣ��ѯ
	IVS_INT32 QueryRaidGroupDisk(const IVS_CHAR* pReqXml, IVS_CHAR** pRspXml)const;

private:

	IVS_INT32 ComSendToOMU(NSS_MSG_TYPE_E msgType, const IVS_CHAR* pReqXml, IVS_CHAR** pRspXml)const;

	IVS_INT32 ComSendToOMU(NSS_MSG_TYPE_E msgType, const IVS_CHAR* pReqXml)const;
private:
    CUserMgr *m_UserMgr;
};

#endif
