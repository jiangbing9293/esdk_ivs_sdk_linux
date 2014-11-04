/********************************************************************
filename    :    FluxControl.h
author      :    l00193369
created     :    2013/01/05
description :    ����������
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2013/01/05 ��ʼ�汾
*********************************************************************/
#ifndef BP_FLUX_CONTORL_H
#define BP_FLUX_CONTORL_H

#include "bp_def.h"
#include "bp_config_alarmer.h"

#define MAX_MSG_NUMBER 0xFFFF + 1 //��ǰ��ϢΪ4λ16����
#define LEVEL_NUMBER   4

NAMESPACE_BP_BEGIN  // namespace end
class EXPORT_DLL CFluxControl : private CConfigAlarmer
{
private:
    CFluxControl();

public:
	static CFluxControl& Instance();
	bool CheckIfAllowPush(int nMsgType, int nMsgQueueLength, int nCurMsgNumber, unsigned short & usLevel);
	bool CheckIfBPSystemMsg(int nMsgType);
	bool CheckIfResume(int nMsgQueueLength, int nCurMsgNumber, unsigned short usLevel);
	void Init();
	void Uninit();

	int GetLimitOfLevel(int nLevel, unsigned short & usLimitOfLevel);
	int SetLimitOfLevel(int nLevel, unsigned short usLimitOfLevel);

	int GetLevelOfMsg(int nMsgType, unsigned short & usLevelOfMsg);
	int SetLevelOfMsg(int nMsgType, unsigned short usLevelOfMsg);

private:
	void Reset();

	unsigned short m_szLevelOfMsg[MAX_MSG_NUMBER];            //��Ϣ���ڵ����ؼ���,Ĭ��Ϊ����0����ͼ���
	unsigned short m_szLimitOfLevel[LEVEL_NUMBER];            //ÿ�������Ӧ�Ŀ��ưٷֱ�
};

NAMESPACE_BP_END   // namespace end
#endif
