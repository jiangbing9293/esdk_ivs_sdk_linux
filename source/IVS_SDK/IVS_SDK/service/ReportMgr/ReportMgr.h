/********************************************************************
filename : ReportMgr.h
author : pkf57481
created : 2013/03/06
description : ���������
copyright : Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history : 2013/03/06 ��ʼ�汾
*********************************************************************/


#ifndef __REPORT_MGR_H_
#define __REPORT_MGR_H_

#include "hwsdk.h"
#include "IVSCommon.h"

class CUserMgr;
class CReportMgr
{
public:
    CReportMgr(void);
    ~CReportMgr(void);

    void SetUserMgr(CUserMgr *pUserMgr);

public:
    // ʵʱ�����ѯ
    IVS_INT32 OMUQueryDevReport(const IVS_CHAR* pReqXml, IVS_CHAR** pRspXml);

    // ʵʱ���������ѯ
    IVS_INT32 OMUQueryDevReportDetail(const IVS_CHAR* pReqXml, IVS_CHAR** pRspXml);

    // ʵʱ�����ѯ
    IVS_INT32 OMUQueryRealtimeReport(const IVS_CHAR* pReqXml, IVS_CHAR** pRspXml);

    // ʵʱ���������ѯ
    IVS_INT32 OMUQueryRealtimeReportDetail(const IVS_CHAR* pReqXml, IVS_CHAR** pRspXml);

    // ��Ƶ������ϲ�ѯ
    IVS_INT32 OMUQueryQDReport(const IVS_CHAR* pReqXml, IVS_CHAR** pRspXml);

    // ��Ƶ������������ѯ
    IVS_INT32 OMUQueryQDReportDetail(const IVS_CHAR* pReqXml, IVS_CHAR** pRspXml);

    // ¼��ִ�мƻ������ѯ
    IVS_INT32 OMUQueryRecordPlanReport(const IVS_CHAR* pReqXml, IVS_CHAR** pRspXml);

    // ¼��ִ�мƻ����������ѯ
    IVS_INT32 OMUQueryRecordPlanReportDetail(const IVS_CHAR* pReqXml, IVS_CHAR** pRspXml);

    // ¼�������Ա����ѯ
    IVS_INT32 OMUQueryRecordIntegralityReport(const IVS_CHAR* pReqXml, IVS_CHAR** pRspXml);

    // ¼�������Ա��������ѯ
    IVS_INT32 OMUQueryRecordIntegralityReportDetail(const IVS_CHAR* pReqXml, IVS_CHAR** pRspXml);

    // �����ʱ����ѯ
    IVS_INT32 OMUQueryPacketLossRateReport(const IVS_CHAR* pReqXml, IVS_CHAR** pRspXml);

    // �����ʱ��������ѯ
    IVS_INT32 OMUQueryPacketLossRateReportDetail(const IVS_CHAR* pReqXml, IVS_CHAR** pRspXml);

    // ���̴洢ռ���ʱ����ѯ
    IVS_INT32 OMUQueryDiskUsageRateReport(const IVS_CHAR* pReqXml, IVS_CHAR** pRspXml);

    IVS_INT32 SendQueryCmd(const IVS_CHAR* pReqXml, IVS_CHAR** pRspXml, IVS_INT32 iMsgType, IVS_INT32 iTimeOut = -1);

	// ��ѯ������Ϣ�ӿ�
	IVS_INT32 OMUQueryDiskProperty(IVS_CHAR** pRspXml);

private:
    CUserMgr *m_UserMgr;
};

#endif
