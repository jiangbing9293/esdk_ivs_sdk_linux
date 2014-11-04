#include "StdAfx.h"
#include "CumwStatusClasses.h"
#include "RemoteDecoderMgr.h"

CStatusConfig::CStatusConfig() : m_ulLogLM(CUMW::DEFAULT_VALUE_LOG_LM)
								, m_usRemoteDecoderListenPort(CUMW::DEFAULT_VALUE_REMOTE_DECODER_LISTEN_PORT)
{
}

long CStatusConfig::InitConfig(const string& strConfigFileFullName)
{
    //���������ļ�����
    m_strConfigFileFullName = strConfigFileFullName;

    //���¼���������
    return ReloadConfig();
}

long CStatusConfig::ReloadConfig()
{
    long lResult = IVS_SUCCEED;

    //��ȡ�������ֵ
    GetConfigValue();

    return lResult;
}

long CStatusConfig::SaveConifg()
{
    //��������������
    SetConfigValue();
	return IVS_SUCCEED;
}

void CStatusConfig::GetConfigValue()
{
    //��־����
    SetLogLM(CUMW::DEFAULT_VALUE_LOG_LM);

    //Զ�̽����������˿�
    SetRemoteDecoderListenPort(CUMW::DEFAULT_VALUE_REMOTE_DECODER_LISTEN_PORT);
}

void CStatusConfig::SetConfigValue()
{

}

unsigned long CStatusConfig::GetLogLM() const
{
    return m_ulLogLM;
}

void CStatusConfig::SetLogLM(const unsigned long ulLogLM)
{
    //ֵ�Ϸ��Լ��
    if (IVSLOG_DEBUG >= ulLogLM)
    {
        //����ֵ
        m_ulLogLM = ulLogLM;
    }
}

unsigned short CStatusConfig::GetRemoteDecoderListenPort() const
{
    return m_usRemoteDecoderListenPort;
}

void CStatusConfig::SetRemoteDecoderListenPort
(   IN const unsigned short usListenPort)
{
    if (CUMW::PORT_MIN < usListenPort)
    {
        m_usRemoteDecoderListenPort = usListenPort;
    }
	else
	{
		IVS_RUN_LOG_ERR( "Listen Port Value should be larger than %ld.", CUMW::PORT_MIN);
	}
}

CStatusSingleton::CStatusSingleton()
{
    memset(m_szLocalIp, 0, sizeof(m_szLocalIp));
};

const char* CStatusSingleton::GetLocalIp()
{
    //�Ѿ���ȡ����ֱ�ӷ���
    if (0 != m_szLocalIp[0])
    {
        return m_szLocalIp;
    }

    //��ȡ����IP
    char szHostName[CUMW_PATH_LEN] = {0};

    //��ȡ������
    int nResult = gethostname(szHostName, sizeof(szHostName));
    if (SOCKET_ERROR == nResult)
    {
        IVS_RUN_LOG_ERR( "gethostname Failed On Get Local Address. ErrorCode = %ld.", GetLastError());
        return m_szLocalIp;
    }

    //��ȡ������ַ
    sockaddr_in stAddr = {0};
    struct hostent* pstHostent = NULL;
    pstHostent = gethostbyname(szHostName);   //ϵͳ����
    if (pstHostent)
    {
        stAddr.sin_addr.s_addr = *(unsigned long*)(void*)pstHostent->h_addr;
    }
	else
	{
		IVS_RUN_LOG_ERR( "gethostbyname Failed On Get Local Address. ErrorCode = %ld.", GetLastError());
		return m_szLocalIp;
	}

	char* pstrAdd = inet_ntoa(stAddr.sin_addr);
	if (pstrAdd)
	{
		if(!CToolsHelp::Strncpy(m_szLocalIp, sizeof(m_szLocalIp), pstrAdd, strlen(pstrAdd)))
		{
			IVS_RUN_LOG_ERR("GetLocalIp Copy LocalIp failed");
		}
	}
	else
	{
		IVS_RUN_LOG_ERR( "inet_ntoa Failed On Get Local Address. ErrorCode = %ld.", GetLastError());
		return m_szLocalIp;
	}
	
    IVS_DBG_LOG("Local IP: %.15s.", m_szLocalIp);

    return m_szLocalIp;
}

void CStatusSingleton::SetLocalIp(const char* pszIp)
{
    if(!CToolsHelp::Strncpy(m_szLocalIp, sizeof(m_szLocalIp), pszIp, sizeof(m_szLocalIp) - 1))
	{
		IVS_RUN_LOG_ERR("SetLocalIp copy LocalIp failed");
	}
}

CStatusNotifyCallBack::CStatusNotifyCallBack()
{
    //����״̬
    ResetStatus();
}

void CStatusNotifyCallBack::ResetStatus()
{
    m_pFunCallBack = NULL; //�ص�����ָ��
    m_pUserData = NULL; //�û�����ָ��
}

void CStatusNotifyCallBack::SetStatusValue(PCUMW_CALLBACK_NOTIFY pFunCallBack, void* pUserData)
{
    if (NULL != pFunCallBack) //Ϊnull����ֵ
    {
        m_pFunCallBack = pFunCallBack; //�ص�����ָ��
    }

    if (NULL != pUserData)
    {
        m_pUserData = pUserData;    //�û�����ָ��
    }
}

long CStatusNotifyCallBack::NotifyCallBack(CUMW_NOTIFY_INFO& stInfo)
{
    //ָ��ǿռ��
    if (NULL == m_pFunCallBack)
    {
        IVS_RUN_LOG_ERR( "NotifyCallBack Point is NULL");
        return IVS_TVWALL_MEMORY_NULL;
    }

    //���ûص�����
    long lResult = m_pFunCallBack(&stInfo, m_pUserData);

    return lResult;
}

long CStatusNotifyCallBack::NotifyAsynRet
(   const CUMW_NOTIFY_ASYNC_RET& stNotifyInfo)
{
    CUMW_NOTIFY_INFO stInfo = {0};

    stInfo.ulNotifyType = CUMW_NOTIFY_TYPE_ASYNC_RET; //��������Ϊ�첽����
    stInfo.pNotifyInfo = (void*)&stNotifyInfo;
    stInfo.ulNotifyInfoLen = sizeof(stNotifyInfo);

    long lResult = NotifyCallBack(stInfo);

    return lResult;
}

long CStatusNotifyCallBack::NotifyAsynRetNoInfo
(   IN unsigned long ulAsynSeq,
    IN long          lAsynRetCode,
    IN unsigned long ulBusinessId)
{
    CUMW_NOTIFY_ASYNC_RET stAsynRet = {0};

    stAsynRet.ulAsyncSeq = ulAsynSeq;
    stAsynRet.ulAsyncRetCode = (unsigned long)lAsynRetCode;
    stAsynRet.ulAsyncType  = CUMW_ASYNC_TYPE_NO_RET_INFO;
    stAsynRet.ulBusinessID = ulBusinessId;

    long lResult = NotifyAsynRet(stAsynRet);

    return lResult;
}

long CStatusNotifyCallBack::NotifyBusinessAsynRet
(   IN unsigned long ulAsyncSeq,
    IN long          lAsyncRetCode,
    IN unsigned long ulBusinessID)
{
    CUMW_NOTIFY_ASYNC_RET stSyncRet = {0};

    stSyncRet.ulAsyncSeq = ulAsyncSeq;
    stSyncRet.ulAsyncRetCode = (unsigned long)lAsyncRetCode;
    stSyncRet.ulAsyncType  = CUMW_ASYNC_TYPE_START_MEDIA_BUSINESS;
    stSyncRet.ulBusinessID = ulBusinessID;

    long lResult = NotifyAsynRet(stSyncRet);

    return lResult;
}

long CStatusNotifyCallBack::NotifyEvent
(   IN unsigned long ulEventType,
    IN unsigned long ulEventPara)
{
    //�����¼�֪ͨ�ṹ��
    CUMW_NOTIFY_EVENT stEvent = {0};

    stEvent.ulEventType = ulEventType;
    stEvent.ulEventPara = ulEventPara;

    //�����¼�֪ͨ��Ϣ�ṹ��
    CUMW_NOTIFY_INFO stInfo = {0};
    stInfo.ulNotifyType = CUMW_NOTIFY_TYPE_EVENT;
    stInfo.pNotifyInfo = &stEvent;
    stInfo.ulNotifyInfoLen = sizeof(stEvent);

    long lResult = NotifyCallBack(stInfo);

    return lResult;
}

long CStatusNotifyCallBack::NotifyDecoderDisConnect
(   IN CUMW_NOTIFY_REMOTE_DECODER_OFF_LINE_INFO& stNotifyInfo)
{
    CUMW_NOTIFY_INFO stInfo = {0};

    stInfo.ulNotifyType = CUMW_NOTIFY_TYPE_REMOTE_DECODER_DISCONNECT;
    stInfo.pNotifyInfo = (void*)&stNotifyInfo;
    stInfo.ulNotifyInfoLen = sizeof(stNotifyInfo);

    long lResult = NotifyCallBack(stInfo);

    return lResult;
}

long CStatusNotifyCallBack::NotifyDecoderRegister
(   IN CUMW_NOTIFY_REMOTE_DECODER_STATUS& stNotifyInfo)
{
    CUMW_NOTIFY_INFO stInfo = {0};

    stInfo.ulNotifyType = CUMW_NOTIFY_TYPE_REMOTE_DECODER_REGISTER;
    stInfo.pNotifyInfo = (void*)&stNotifyInfo;
    stInfo.ulNotifyInfoLen = sizeof(stNotifyInfo);

    long lResult = NotifyCallBack(stInfo);

    return lResult;
}

long CStatusNotifyCallBack::NotifyDecoderUnRegister
(   IN CUMW_NOTIFY_REMOTE_DECODER_OFF_LINE_INFO& stNotifyInfo)
{
    CUMW_NOTIFY_INFO stInfo = {0};

    stInfo.ulNotifyType = CUMW_NOTIFY_TYPE_REMOTE_DECODER_UNRIGISTER;
    stInfo.pNotifyInfo = (void*)&stNotifyInfo;
    stInfo.ulNotifyInfoLen = sizeof(stNotifyInfo);

    long lResult = NotifyCallBack(stInfo);

    return lResult;
}

CStatusTrigger::CStatusTrigger
(
    IN CUMW::TRIGGER_TYPE enTriggerType
)
{
    m_enTriggerType = enTriggerType;
}

void CStatusTrigger::onTrigger
(   IN void*           pArg,
    IN ULONGLONG       ullScales,
    IN TriggerStyle    enStyle)
{
    switch ((unsigned long)m_enTriggerType)
    {
    case CUMW::TRIGGER_TYPE_DECODER_CONN_TIME_OUT:
    {
        HandleTriggerDecoderConnTimeOut(pArg);
        break;
    }

    default:
    {
		IVS_DBG_LOG("Decoder Conn not TimeOut");
        break;
    }
    }

    //�����������账��
    ullScales = ullScales;
    enStyle = enStyle;
}

void CStatusTrigger::HandleTriggerDecoderConnTimeOut
(   IN void* pArg) const
{
    CDecoderConnHandle* pConnHandle = VOS_CAST((CDecoderConnHandle*)pArg);

    if (NULL == pConnHandle)
    {
        IVS_RUN_LOG_ERR(
                "Parameter(%#p) Can't Convert To Decoder Connection Handle Object On Handlet Decoder Connection Time Out Tringger.",
                pArg);
        return;
    }

    IVS_RUN_LOG_ERR("Handlet Decoder Connection Time Out Tringger. Remote Decoder(IP:%.15s) Connect Time Out.",
            inet_ntoa((in_addr &)pConnHandle->m_peerAddr.m_lIpAddr));

    //����ʱ
    pConnHandle->HandleTimeout();
}

CStatusThread::CStatusThread()
{
    m_hThread = NULL;     //�����¼��߳̾��
    m_bThreadRun  = FALSE;      //�����¼��߳��Ƿ�����
    m_ulQueueSize = 0;        //���д�С
    m_bIdle = true;
    memset(m_szDecoderID, 0, CUMW_DECODER_ID_LEN);
}

CStatusThread::~CStatusThread()
{
    m_hThread = NULL;   //�����¼��߳̾��
}

long CStatusThread::StartThread
(   IN CUMW::PTHREAD_FUN   pThreadFun,
    IN void*               pThreadEntryPara,
    IN unsigned long       ulQueueSize)
{
    IVS_DBG_LOG("[ ThreadID = %#p ] Start Thread Begin. QueueSize = %u.", this, ulQueueSize);

    m_ulQueueSize = ulQueueSize;

    if (0 < m_ulQueueSize)
    {
        //��ʼ���¼�ͬ������
        long lResult = m_objEventSyncQueue.init((long)ulQueueSize);
        if (IVS_SUCCEED != lResult)
        {
            IVS_RUN_LOG_ERR( "[ ThreadID = %#p ] Init Event Synchronous Queue Failed On Start Thread.", this);
            return IVS_FAIL;
        }
    }

    //�����߳�ǰ������Ϊ�߳�����״̬
    m_bThreadRun = TRUE;

    //���������¼��߳�
    m_hThread = CreateThread(NULL, 0, pThreadFun, pThreadEntryPara, 0, NULL);
    if (NULL == m_hThread)
    {
        m_bThreadRun = FALSE;
        IVS_RUN_LOG_ERR( "[ ThreadID = %#p ] Create Thread Failed On Start Thread. ErrorCode = %ld.", this,
                GetLastError());
        return IVS_TVWALL_CREATE_THREAD_FAIL;
    }

    IVS_DBG_LOG("[ ThreadID = %#p ] Start Thread Success. QueueSize = %lu.", this, ulQueueSize);
    return IVS_SUCCEED;
}

void CStatusThread::StopThread()
{
    IVS_DBG_LOG("[ ThreadID = %#p ] Stop Thread Begin.", this);

    //�˳������¼��߳�
    //m_bThreadRun = FALSE;

    if (NULL != m_hThread)
    {
        //if (0 < m_ulQueueSize)
        {
            //���¼�ͬ�����в����˳��߳��¼������账�����
            (void)AddEvent(CUMW::EVENT_TYPE_EXIT_THREAD);

            //IVS_RUN_LOG_INF("[ ThreadID = %#p ] Add Stop Thread Info.", this);
        }

        //���ȴ�30�룬����δ�˳���������߼�������
        if (WAIT_OBJECT_0 != WaitForSingleObject(m_hThread, CUMW::WAIT_FOR_THREAD_EXIT_TIME))
        {
            IVS_RUN_LOG_ERR( "[ ThreadID = %#p ] Thread Hasn't Exit. There May be some Problem.", this);
        }

        //�ͷ��߳̾��
        (void)CloseHandle(m_hThread);
        m_hThread = NULL;
    }

    IVS_DBG_LOG("[ ThreadID = %#p ] Stop Thread End.", this);
}

long CStatusThread::AddEvent
(   IN CUMW::EVENT_TYPE    enEventType,
    IN void*               pEventInfo,
    IN unsigned long       ulEventInfoLen)
{
    if (0 == m_ulQueueSize)
    {
        IVS_RUN_LOG_ERR( "[ ThreadID = %#p ] Add Event Failed. QueueSize = 0.", this);
        return IVS_FAIL;
    }

    IVS_DBG_LOG("[ ThreadID = %#p ] Add Event Begin. %s.", this, CUMW::STR_ARR_EVENT_TYPE[enEventType]);

    long lResult = IVS_SUCCEED;

    //�����¼�
    CUMW::EVENT_INFO* pstEventInfo = VOS_NEW(pstEventInfo);

    //����ʧ��
    if (NULL == pstEventInfo)
    {
        //�˳��߳�ʱ���������¼�ͬ�����в���һ���¼�����ʱʹ�ÿ��¼�
        if (CUMW::EVENT_TYPE_EXIT_THREAD == enEventType)
        {
            //���¼�ͬ�����в���һ�����¼�
            lResult = m_objEventSyncQueue.pushBackEv(NULL);
            if (IVS_SUCCEED != lResult)
            {
                IVS_RUN_LOG_ERR( "[ ThreadID = %#p ] Push Event Failed On Add Event. %s.", this,
                        CUMW::STR_ARR_EVENT_TYPE[enEventType]);
                return IVS_FAIL;
            }

            return IVS_SUCCEED;
        }

        //�������������ʧ��
        IVS_RUN_LOG_ERR( "[ ThreadID = %#p ] Create Event Failed On Add Event. %s.", this,
                CUMW::STR_ARR_EVENT_TYPE[enEventType]);
        return IVS_ALLOC_MEMORY_ERROR;
    }

    //�����¼���Ϣ
    pstEventInfo->enEventType = enEventType;
    pstEventInfo->pEventInfo = pEventInfo;
    pstEventInfo->ulEventInfoLen = ulEventInfoLen;

    //���¼�ͬ�����в����¼�
    lResult = m_objEventSyncQueue.pushBackEv(pstEventInfo);
    if (IVS_SUCCEED != lResult)
    {
        //�ͷ���ʱ�ڴ�
        VOS_DELETE(pstEventInfo);

        IVS_RUN_LOG_ERR( "[ ThreadID = %#p ] Push Event Failed On Add Event. %s.", this,
                CUMW::STR_ARR_EVENT_TYPE[enEventType]);
        return IVS_FAIL;
    }

    IVS_DBG_LOG("[ ThreadID = %#p ] Add Event Success. %s.", this, CUMW::STR_ARR_EVENT_TYPE[enEventType]);
    return IVS_SUCCEED;
}
