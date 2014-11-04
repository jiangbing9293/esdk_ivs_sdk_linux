/********************************************************************
filename	: 	CIntelligenceAnalysisMgr.h
author		:	z00227377
created		:	2012/12/5
description	:	���ܷ����������;
copyright	:	Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history		:	2012/12/5 ��ʼ�汾;
*********************************************************************/

#ifndef __INTELLIGENCE_ANALYSIS_MGR_H
#define __INTELLIGENCE_ANALYSIS_MGR_H

#include "hwsdk.h"
#include "nss_mt.h"

class CIntelligenceAnalysisMgr
{
public:
    CIntelligenceAnalysisMgr(void);
    ~CIntelligenceAnalysisMgr(void);
    
    // �����û�����ָ��
    void SetUserMgr(CUserMgr *pUserMgr);

	// ���ܷ���ͳһ͸���ӿ�
	IVS_INT32 IntelligenceAnalysis(IVS_UINT32 uiInterfaceType, IVS_CHAR* pTransID, IVS_UINT32 uiTransIDLen, const IVS_CHAR* pReqXml, IVS_CHAR** pRspXml);

private:
    CUserMgr   *m_pUserMgr;       // �û�����ָ��
};

#endif


