/********************************************************************
 filename    :    StreamEventMgr.h
 author      :    s00191067
 created     :    2013/03/12
 description :    ý�����������ƹ����࣬���ݲ��ſ��н���ģ�黺����������������������շ��ٶ�;
 copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
 history     :    2013/03/12 ��ʼ�汾
*********************************************************************/
#ifndef STREAM_EVENT_MGR
#define STREAM_EVENT_MGR

#include "vos.h"
#include "BaseJob.h"
#include "SDKDef.h"
#include "IVSPlayerDataType.h"

struct STREAM_EVENT_DATA
{
    int iSessionID;             
    unsigned long ulPlayHandle;
    int iBufferState;
};

class _BaseThread;
class CStreamEventMgr : public BaseJob
{
public:
    virtual ~CStreamEventMgr(void);

    static CStreamEventMgr& instance()
    {
        static CStreamEventMgr s_eventStreamMgr;
        return s_eventStreamMgr;
    }

    int Init();

    void UnInit();

    virtual void MainLoop();

    virtual void kill();

    int PostEvent(int iSessionID, unsigned long ulPlayHandle, int iBufferState);

    PLAYER_EVENT_CALLBACK GetPlayerEventCallBack() const;
private:
    CStreamEventMgr(void);
    STREAM_EVENT_DATA* PopEvent();
    void DoEventMsg(STREAM_EVENT_DATA*& pstEvent);

private:
    _BaseThread     *m_pEventThread;    //�¼��쳣�ص�;
    std::queue<STREAM_EVENT_DATA*>      m_eventQueue;
    VOS_Mutex*                          m_pMutexEventQueue;       // ������;
    bool                                m_bRun;                   // �߳����б�־; 
};
#endif //STREAM_EVENT_MGR
