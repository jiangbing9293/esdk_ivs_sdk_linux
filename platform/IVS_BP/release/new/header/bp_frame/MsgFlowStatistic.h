#ifndef MSG_FLOW_STATISTIC_H
#define MSG_FLOW_STATISTIC_H
/********************************************************************
filename    :    MsgFlowStatistic.h
author      :    j00213308
created     :    2012/07/09
description :    ͳ��ģ��Ľ�����Ϣ����
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2012/07/09 ��ʼ�汾
*********************************************************************/
#include "ace_header.h"
#include "bp_def.h"

NAMESPACE_BP_BEGIN

static unsigned int DEFAULT_MAX_MSG_FLOW   = 1000;			// Ĭ�ϵĽ�����Ϣ�������

// ��Ϣ����ͳ�Ƶ�����
class EXPORT_DLL CMsgFlowStatistic
{
public:
	static CMsgFlowStatistic& GetInstance(void);
	~CMsgFlowStatistic(void){}

	bool IsOverload(bool bInput);

	bool IsLocusOverLoad(uint32_t tMsgId, bool bInput = true);

	inline void Init(void)
	{
		m_lastTime = ACE_OS::time();
	}

	inline void SetMaxMsgCntPerSec(unsigned int uiMsgCntPerSec) { m_uiMaxMsgFlow = uiMsgCntPerSec; }

	inline unsigned int GetMaxMsgCntPerSec(void) { return m_uiMaxMsgFlow; }

private:
	CMsgFlowStatistic(void)
		: m_lastTime(0)
		, m_uiCurOutput(0)
		, m_uiCurInput(0)
		, m_uiMaxMsgFlow(DEFAULT_MAX_MSG_FLOW)
	{
	}

private:
	time_t                    m_lastTime;					// �ϴ�ͳ��������ʱ��
	unsigned int              m_uiCurOutput;				// ��Ϣ������
	unsigned int              m_uiCurInput;					// ��Ϣ��������
	unsigned int              m_uiMaxMsgFlow;				// ÿ�������Ϣ��������
	ACE_Thread_Mutex		  m_mutex;
	
};

NAMESPACE_BP_END 

#endif
