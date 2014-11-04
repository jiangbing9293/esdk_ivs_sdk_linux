#include "StreamEventMgr.h"
#include "_BaseThread.h"
#include "IVSCommon.h"
#include "LockGuard.h"
#include "SDKInterface.h"
#ifdef WIN32
#include "IVS_Player.h"
#endif
#include "SDKConfig.h"

//lint -e526
extern CSDKInterface *g_pNvs ;

static void __SDK_CALL PlayerEventCallback(IVS_ULONG  ulChannel,
                                            IVS_INT32  iMsgType,
                                            IVS_VOID*  pParam,
                                            IVS_VOID*  pUser)
{
    BP_DBG_LOG("PlayerEventCallback", " Channel:%d, MsgType:%d", ulChannel, iMsgType);
    int iSessionID = reinterpret_cast <long>(pUser);
    CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
    if (NULL == pUserMgr)
    {
        BP_RUN_LOG_ERR(IVS_PARA_INVALID, "PlayerEventCallback", "Can not get usermgr by sessionid:%d", iSessionID);
        return;
    }
#ifdef WIN32
    unsigned long playHandle = pUserMgr->GetPlayHandlebyPlayChannel(ulChannel);
#else
	unsigned long playHandle = 0;
#endif
    if (0 == playHandle)
    {
        BP_RUN_LOG_ERR(IVS_PARA_INVALID, "PlayerEventCallback", "Get playhandle by rtsphandle failed, rtsp handle:%d", playHandle);
        return;
    }

    switch (iMsgType)
    {
    case IVS_EVENT_PLAYER_BUFFER:
        {
            int iFullFlag = *((int*)pParam);
            (void)CStreamEventMgr::instance().PostEvent(iSessionID, playHandle, iFullFlag);
            BP_DBG_LOG("PlayerEventCallback, IVS_EVENT_PLAYER_BUFFER by sessionid:%d, rtsp handle:%d, iFullFlag:%d", iSessionID, playHandle, iFullFlag);
        }
        break;
    default:
        break;
    }
}//lint !e818

CStreamEventMgr::CStreamEventMgr(void)
    : m_pEventThread(NULL)
    , m_pMutexEventQueue(NULL)
    , m_bRun(false)
{
    m_pMutexEventQueue = VOS_CreateMutex();
}

CStreamEventMgr::~CStreamEventMgr(void)
{
    try
    {
        UnInit();

        if (NULL != m_pMutexEventQueue)
        {
            VOS_DestroyMutex(m_pMutexEventQueue);
            m_pMutexEventQueue = NULL;
            m_pEventThread = NULL;
        }
    }
    catch(...)
    {}//lint !e1775    
}//lint !e732 !e1740

int CStreamEventMgr::Init()
{
    IVS_INT32 iRet = IVS_FAIL;
    
    try
    {
        // ����ý�建�����������ްٷֱ�
        IVS_UINT32 uiLowerLimit = 0;
        IVS_UINT32 uiUpperLimit = 0;
        CSDKConfig::instance().GetMediaBufferLimit(uiLowerLimit, uiUpperLimit);

		BP_RUN_LOG_INF("CStreamEventMgr::Init", "Set MediaBfferLimit Low[%u] Up[%u]", uiLowerLimit, uiUpperLimit);
#ifdef WIN32
        IVS_PLAY_SetMediaBufferLimit(uiLowerLimit, uiUpperLimit);]
#endif

        //�����¼��ص��߳�;
        m_pEventThread = new _BaseThread((BaseJob* )this, 0);
        m_bRun = true;
        iRet = IVS_SUCCEED;
    }
    catch (...)
    {
        BP_RUN_LOG_ERR(IVS_PLAYER_RET_ALLOC_MEM_ERROR, "new thread", "NA");
        m_pEventThread = NULL;
    }
    return iRet;
}

void CStreamEventMgr::UnInit()
{
    m_bRun = false;
    IVS_DELETE(m_pEventThread);
}

void CStreamEventMgr::MainLoop()
{
    while (m_bRun)
    {
        STREAM_EVENT_DATA* pstEvent = PopEvent();
        // ������������sleep100����;
        if (NULL == pstEvent)
        {
#ifdef WIN32
            Sleep(100);
#else
			VOS_Sleep(100);
#endif
            continue;
        }
        
        DoEventMsg(pstEvent);
    }
}

void CStreamEventMgr::kill()
{
    //ֹͣ�߳�;
    m_bRun = false;
}

int CStreamEventMgr::PostEvent(int iSessionID, unsigned long ulPlayHandle, int iBufferState)
{
    CLockGuard lock(m_pMutexEventQueue);
    //�����¼�����
    STREAM_EVENT_DATA* pstEventData = IVS_NEW(pstEventData);
    if (NULL == pstEventData)
    {
        BP_RUN_LOG_ERR(IVS_FAIL, "event data null", "NA");
        return IVS_FAIL;
    }

    memset(pstEventData, 0, sizeof(STREAM_EVENT_DATA));
    pstEventData->iSessionID = iSessionID;
    pstEventData->ulPlayHandle = ulPlayHandle;
    pstEventData->iBufferState = iBufferState;

    //�������ǰ�ȼ���µ�ǰ�ĳ���,����Ѵ����ֵ����ɾ����һ��;
    if (m_eventQueue.size() >= MAX_EVENT_QUEUE_LEN)
    {
        STREAM_EVENT_DATA* pstEventDataFront = m_eventQueue.front();
        IVS_DELETE(pstEventDataFront);
        m_eventQueue.pop();
        BP_DBG_LOG("event queue over max size[%u]", m_eventQueue.size());
    }

    //�������
    m_eventQueue.push(pstEventData);
    return IVS_SUCCEED;
}

STREAM_EVENT_DATA* CStreamEventMgr::PopEvent()
{
    CLockGuard lock(m_pMutexEventQueue);
    if (0 == m_eventQueue.size())
    {
        return NULL;
    }

    STREAM_EVENT_DATA* pstEventData = m_eventQueue.front();
    m_eventQueue.pop();
    return pstEventData;
}

void CStreamEventMgr::DoEventMsg(STREAM_EVENT_DATA*& pstEvent)
{
    if (NULL == pstEvent)
    {
        return;
    }

    CHECK_POINTER_VOID(g_pNvs);
    int iSessionID = pstEvent->iSessionID;
    CUserMgr* pUserMgr = g_pNvs->GetUserMgr(iSessionID);
    if (NULL == pUserMgr)
    {
        BP_RUN_LOG_ERR(IVS_PARA_INVALID, "DoEventMsg", "Can not get usermgr by sessionid:%d", iSessionID);
        return;
    }
#ifdef WIN32
    (void)pUserMgr->NotifyStreamBufferState(pstEvent->ulPlayHandle, pstEvent->iBufferState);
#endif
    IVS_DELETE(pstEvent);
}//lint !e1762 pstEvent�ں������ͷţ���������Ϊconst

PLAYER_EVENT_CALLBACK CStreamEventMgr::GetPlayerEventCallBack() const
{
    return PlayerEventCallback;
}
