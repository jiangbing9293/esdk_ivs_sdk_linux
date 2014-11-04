/********************************************************************
 filename    :    IVSEventQueue.h
 author      :    s00191067
 created     :    2012/11/20
 description :    �¼����У�������Ϣ���쳣�¼���;
 copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
 history     :    2012/11/20 ��ʼ�汾
*********************************************************************/
#ifndef IVS_EVENT_QUEUE_H
#define IVS_EVENT_QUEUE_H
// #include <queue>
#include "EventMgr.h"
#include "BaseJob.h"
#include "vos.h"
#include "SDKDef.h"

class CEventCallBackJob : public BaseJob
{
public:
    virtual ~CEventCallBackJob(void);

    static CEventCallBackJob& instance()
    {
        static CEventCallBackJob s_eventCallBackJob;
        return s_eventCallBackJob;
    }

    virtual void MainLoop();

    virtual void kill();

    int PostEvent(int iSessionID, int iEventType, const void* pEventData, int iBufSize);

    void SetEventMgr(CEventMgr* pEventMgr)
    {
        m_pEventMgr = pEventMgr;
		m_bRun = true;
    }

private:
    CEventCallBackJob(void);
    EVENT_CALL_BACK_DATA* PopEvent();

private:
    std::queue<EVENT_CALL_BACK_DATA*>   m_eventQueue;
    VOS_Mutex*                          m_pMutexEventQueue;       // ������;
    bool                                m_bRun;                   // �߳����б�־; 
    CEventMgr*                          m_pEventMgr;              // �¼����������;
};
#endif //IVS_EVENT_QUEUE_H
