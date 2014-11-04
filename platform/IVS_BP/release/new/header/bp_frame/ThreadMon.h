/********************************************************************
filename    :    ThreadMon.h
author      :    s00191067
created     :    2012/03/28
description :    �̼߳����
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2012/03/28 ��ʼ�汾
*********************************************************************/
#ifndef THREAD_MON_H
#define THREAD_MON_H

#include "ThreadMgr.h"
#include "ace_header.h"
#include "bp_namespace.h"

NAMESPACE_BP_BEGIN // namespace begin
enum CHECK_INTERVAL
{
    MIN_INTERVAL         = 180,              // ��С���
    MAX_INTERVAL         = 3600,           // �����
    DEFAULT_INTERVAL    = 1800,            // Ĭ�ϼ��

    DEFAULT_REPORT_INTERVAL        = 10,    // �����߳�״̬�ϱ����������,��λ��
    DEFAULT_THREAD_CHECK_INTERVAL  = 10,     // �����̼߳������������λ��
    DEFAULT_THREAD_CHECK_TIME_PROOFREADING = 10 // ʱ�䳬ʱ����У��ֵ
};

class EXPORT_DLL CThreadEventHandle
{
public:
	CThreadEventHandle(){}
	virtual ~CThreadEventHandle(){}

public:
	virtual void OnThreadTimeOut(CThread*& pThread);
};

class CThreadMon
{
public:
    ~CThreadMon(void);

    static CThreadMon& instance();

    /**************************************************************************
    * name       : CheckMonitor
    * description: ����߳�
    * input      : NA
    * output     : NA
    * return     : IVS_SUCCEED - �ɹ���IVS_FAIL - ʧ��
    * remark     : NA
    **************************************************************************/
    int CheckMonitor();

    // �����̹߳��������;
    void SetThreadMgr(CThreadMgr* pThreadMgr) { m_pThreadMgr = pThreadMgr; }
	void SetEventHandle(CThreadEventHandle* pEventHandle);

private:
    CThreadMon(void);
    CThreadMon(const CThreadMon &src)
    {
    }

    CThreadMon& operator=(const CThreadMon&)
    {
        return *this;
    }

private:
	static CThreadMon m_Instance;

    time_t            m_tLastCheckTime;                 // ���һ�μ��ʱ��;
    unsigned long     m_ulMaxThreadCheckInterval;        // �̼߳����;

	CThreadMgr*       m_pThreadMgr;
	CThreadEventHandle* m_pEventHandle;
};

NAMESPACE_BP_END   // namespace end
#endif //THREAD_MON_H
