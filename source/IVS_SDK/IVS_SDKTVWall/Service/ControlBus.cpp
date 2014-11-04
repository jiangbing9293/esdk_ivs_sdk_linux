#include "StdAfx.h"
#include "ControlBus.h"
#include "StreamMgr.h"
#include "bp_def.h"
#include "ToolsHelp.h"

//Default constructor
CControlBus::CControlBus()
{
    m_pStreamMgr = NULL; //StreamMgr����ָ��

    m_bisFirstLogin = FALSE;    //�Ƿ���ε�½
    m_enLoginStatus = CUMW::LOGIN_STATUS_INIT;  //��½״̬Ϊ��ʼ��

    m_ulBusinessId = 0;        //ҵ��ID
	for(int i=0;i<THREAD_MAX_COUNT;i++)
	{
		m_ThreadPara[i].i = 0;
		m_ThreadPara[i].pThread = NULL;
	}
    m_pThreadMutex = VOS_CreateMutex();
    m_pBusinessIdMutex = VOS_CreateMutex();
}

//Destructor
CControlBus::~CControlBus()
{
    m_pStreamMgr = NULL; //StreamMgr����ָ��

    (void)VOS_DestroyMutex(m_pThreadMutex);
	m_pThreadMutex = NULL;

    (void)VOS_DestroyMutex(m_pBusinessIdMutex);
    m_pBusinessIdMutex = NULL;
}

/**************************************************************************
* name       : InitControlBus
* description: ��ʼ��ControlBus
* input      : pParaInfo   ��ʼ������
* output     : NA
* return     : long ������
* remark     : NA
**************************************************************************/
IVS_INT32 CControlBus::InitControlBus(PCUMW_CALLBACK_NOTIFY pFunNotifyCallBack, void* pUserData)
{
    //���������飺
    //  1��pParaInfoָ��ǿ�
    //  2��ulStructLen�������IPI_INIT_CUMW�ṹ�峤��
    //  3��pParaInfo->pFunNotifyCallBack����Ϊ��
	CHECK_POINTER(pFunNotifyCallBack, IVS_PARA_INVALID);
	CHECK_POINTER(pUserData, IVS_PARA_INVALID);

	IPI_INIT_CUMW ParaInfo;
    IPI_INIT_CUMW* pParaInfo = &ParaInfo;
    pParaInfo->pFunNotifyCallBack = pFunNotifyCallBack;
    pParaInfo->pCallBackUserData = pUserData;

    //���Ͻӿ���
    CUMW::CLock objLock(m_objInterfaceLock);

    //�Ѿ���ʼ��
    if (m_enLoginStatus > CUMW::LOGIN_STATUS_INIT)
    {
        IVS_RUN_LOG_ERR( "CUMW Init Failed. Has Inited already.");
        return IVS_TVWALL_INIT_ALREADY;
    }

    //����֪ͨ�ص�����״ֵ̬
    m_objStatusNotifyCallBack.SetStatusValue(pParaInfo->pFunNotifyCallBack, pParaInfo->pCallBackUserData);

    //��ʼ��ControlBus��Դ
    long lResult = InitControlBusResource();
    if (IVS_SUCCEED != lResult)
    {
        IVS_RUN_LOG_ERR( "Init ControlBus Resource Failed On CUMW Init.");

        //�ͷ�ControlBus����Դ
        ReleaseControlBusResource();

        return IVS_FAIL;
    }

    //�����ʼ���ɹ�״̬
    m_enLoginStatus = CUMW::LOGIN_STATUS_INITED;

    IVS_DBG_LOG("CUMW Init Success.");
    return IVS_SUCCEED;
}

/**************************************************************************
* name       : InitControlBusResource
* description: ��ʼ��ControlBus����Դ
* input      : NA
* output     : NA
* return     : long ������
* remark     : NA
**************************************************************************/
IVS_INT32 CControlBus::InitControlBusResource()
{
    long lResult = IVS_SUCCEED;    //����������������ֵ

    IVS_DBG_LOG("Init ControlBus Resource Begin.");

    srand(GetTickCount()); //��������

    //��ʼ��ȫ�ֶ�ʱ��
    lResult = InitNvsTimer();
    if (IVS_SUCCEED != lResult)
    {
        IVS_RUN_LOG_ERR( "Init Global Timer Failed On Init ControlBus Resource.");
        return IVS_FAIL;
    }

    //��ʼ��ȫ��ͨ�Ź���ģ��
    lResult = InitConnMgr();
    if (IVS_SUCCEED != lResult)
    {
        IVS_RUN_LOG_ERR( "Init Global Connection Manager Failed On Init ControlBus Resource.");
        return IVS_FAIL;
    }

    //����StreamMgr
    if (NULL == VOS_NEW((CStreamMgr * &)m_pStreamMgr))
    {
        IVS_RUN_LOG_ERR( "Create Stream Manager Failed On Init ControlBus Resource.");
        return IVS_ALLOC_MEMORY_ERROR;
    }

    //��ʼ��StreamMgr
    lResult = m_pStreamMgr->InitStreamMgr(HandleNotifyCallBack, this);
    if (IVS_SUCCEED != lResult)
    {
        IVS_RUN_LOG_ERR( "Init Stream Manager Failed On Init ControlBus Resource.");
        return IVS_FAIL;
    }

    //�����¼������߳�
    for (int i = 0; i < THREAD_MAX_COUNT; i++)
    {
        m_ThreadPara[i].i = i;
        m_ThreadPara[i].pThread = this;
        lResult = m_objProcEventThread[i].StartThread(ProcEventThreadEntry, (void*)&m_ThreadPara[i], CUMW::EVENT_SYNC_QUEUE_NUM);
        if (IVS_SUCCEED != lResult)
        {
            IVS_RUN_LOG_ERR( "Start Process Event Thread Failed On Init ControlBus Resource. ThreadID = %#p. i = %d",
                    &m_objProcEventThread[i], i);
            return IVS_TVWALL_CREATE_THREAD_FAIL;
        }
    }

    //����Զ�̽���������
    lResult = m_pStreamMgr->StartRemoteDecoderMgr();
    if (IVS_SUCCEED != lResult)
    {
        IVS_RUN_LOG_ERR( "Start Remote Decoder Mgr Failed On Init ControlBus Resource.");
        return IVS_TVWALL_CREATE_THREAD_FAIL;
    }

    IVS_DBG_LOG("Init ControlBus Resource Success.");
    return IVS_SUCCEED;
}

/**************************************************************************
* name       : InitNvsTimer
* description: ��ʼ����ʱ��
* input      : NA
* output     : NA
* return     : long ������
* remark     : NA
**************************************************************************/
IVS_INT32 CControlBus::InitNvsTimer() const
{
    IVS_DBG_LOG("Init Global Timer Begin.");

    //����ֵ
    IVS_INT32 lResult = IVS_SUCCEED;

    //��ö�ʱ��ʵ��
    CNVSTimer& objNVSTimer = CStatusSingleton::Instance().GetNVSTimer();

    //��ʼ����ʱ��
    lResult = objNVSTimer.init(CUMW::TIMER_INTERVAL);
    if (IVS_SUCCEED != lResult)
    {
        IVS_RUN_LOG_ERR( "Init Global Timer Failed.");
        return IVS_FAIL;
    }

    //������ʱ��
    lResult = objNVSTimer.run();
    if (IVS_SUCCEED != lResult)
    {
        IVS_RUN_LOG_ERR( "Start Global Timer Failed.");
        return IVS_FAIL;
    }

    IVS_DBG_LOG("Init Global Timer Success.");
    return lResult;
}

/**************************************************************************
* name       : InitConnMgr
* description: ��ʼ��ͨ�Ź���ģ��
* input      : NA
* output     : NA
* return     : long ������
* remark     : NA
**************************************************************************/
IVS_INT32 CControlBus::InitConnMgr() const
{
    IVS_DBG_LOG("Init Connection Manager Begin.");

    //����ֵ
    IVS_INT32 lResult = IVS_SUCCEED;

    //���ͨ�Ź�����ʵ��
    CConnMgr& objConnMgr = CStatusSingleton::Instance().GetConnMgr();

    //��ʼ��ͨ��ƽ̨
    lResult = objConnMgr.init(CUMW::CONN_MGR_SELECT_PERIOD, VOS_FALSE, VOS_FALSE, VOS_TRUE); //(CUMW::CONN_MGR_SELECT_PERIOD, VOS_TRUE, VOS_TRUE, VOS_TRUE);
    if (IVS_SUCCEED != lResult)
    {
        IVS_RUN_LOG_ERR( "Init Connection Manager Failed.");
        return IVS_FAIL;
    }

    //����ͨ��ƽ̨
    lResult = objConnMgr.run();
    if (IVS_SUCCEED != lResult)
    {
        IVS_RUN_LOG_ERR( "Start Connection Manager Failed.");
        return IVS_FAIL;
    }

    IVS_DBG_LOG("Init Connection Manager Success.");
    return lResult;
}

/**************************************************************************
* name       : ReleaseControlBus
* description: �ͷ�ControlBus
* input      : NA
* output     : NA
* return     : long ������
* remark     : NA
**************************************************************************/
IVS_INT32 CControlBus::ReleaseControlBus()
{
    //���Ͻӿ���
    CUMW::CLock objLock(m_objInterfaceLock);

    IVS_DBG_LOG("CUMW Release Begin.");

    //��δ��ʼ��
    if (IVS_SUCCEED != CheckStatus(CUMW::LOGIN_STATUS_INITED))
    {
        IVS_RUN_LOG_ERR( "CUMW Release Failed. Has not Inited yet.");
        return IVS_TVWALL_INIT_NO;
    }

    //����δ��ʼ��״̬
    m_enLoginStatus = CUMW::LOGIN_STATUS_INIT;

    //�ͷ�ControlBus����Դ
    ReleaseControlBusResource();

    IVS_DBG_LOG("CUMW Release Success.");

    return IVS_SUCCEED;
}

/**************************************************************************
* name       : ReleaseControlBusResource
* description: �ͷ�ControlBus����Դ
* input      : NA
* output     : NA
* return     : long ������
* remark     : NA
**************************************************************************/
void CControlBus::ReleaseControlBusResource()
{
    IVS_DBG_LOG("Release ControlBus Resource Begin.");

    //�˳������¼��߳�
    for (int i = 0; i < THREAD_MAX_COUNT; i++)
    {
        m_objProcEventThread[i].StopThread();
    }

	//���ͨ�Ź�����ʵ��
	CConnMgr& objConnMgr = CStatusSingleton::Instance().GetConnMgr();

	//ֹͣͨ�Ź�����
	objConnMgr.exit();

    //�ͷ�StreamMgr
    if (NULL != m_pStreamMgr)
    {
        (void)m_pStreamMgr->StopRemoteDecoderMgr();

        (void)m_pStreamMgr->ReleaseStreamMgr();

        VOS_DELETE(m_pStreamMgr);
    }

    //��ö�ʱ��ʵ��
    CNVSTimer& objNVSTimer = CStatusSingleton::Instance().GetNVSTimer();

    //ֹͣ��ʱ��
    objNVSTimer.exit();

    //����֪ͨ�ص�����״ֵ̬
    m_objStatusNotifyCallBack.ResetStatus();

    IVS_DBG_LOG("Release ControlBus Resource Success.");
}

/**************************************************************************
* name       : HandleNotifyCallBack
* description: ����֪ͨ�ص�
* input      : pstNotifyInfo   ֪ͨ��Ϣָ��
               pUserData       �û�����ָ��
* output     : NA
* return     : long ������
* remark     : NA
**************************************************************************/
IVS_INT32 __stdcall CControlBus::HandleNotifyCallBack(CUMW_NOTIFY_INFO* pstNotifyInfo, void* pUserData)
{
    if (NULL == pstNotifyInfo)
    {
        IVS_RUN_LOG_ERR("ControlBus Handle Notify Call Back Failed. Notify Info is Emtpy.");
        return IVS_PARA_INVALID;
    }

    //����ָ�밲ȫת��
    CControlBus* pThis = VOS_CAST((CControlBus*)pUserData);
    if (NULL == pThis)
    {
        IVS_RUN_LOG_ERR( "ControlBus Handle Notify Call Back Failed. "
                "User Data(%#p) Can't Convert to ControlBus Object.", pUserData);
        return IVS_PARA_INVALID;
    }

    IVS_DBG_LOG("ControlBus Handle Notify Call Back Begin. NotifyType = 0x%04X.", pstNotifyInfo->ulNotifyType);
    long lResult = IVS_SUCCEED;

    switch (pstNotifyInfo->ulNotifyType)
    {
    case CUMW_NOTIFY_TYPE_ASYNC_RET:    //�첽����֪ͨ
    {
        lResult = pThis->HandleNotifyAsyncRet(*pstNotifyInfo);
        IVS_DBG_LOG("ControlBus Handle Notify Call Back End. Asynchronous Return Notify. Result = 0x%04X.",
                lResult);
        return lResult;    //������ֱ�ӷ���
        //break;
    }

    case CUMW_NOTIFY_TYPE_EVENT:
    {
        lResult = pThis->HandleNotifyEvent(*pstNotifyInfo);
        IVS_DBG_LOG("ControlBus Handle Notify Call Back End. Event Notify. Result = 0x%04X.", lResult);
        return lResult;

        //break;
    }

    case CUMW_NOTIFY_TYPE_REMOTE_DECODER_REGISTER:
    {
        CUMW_NOTIFY_REMOTE_DECODER_STATUS* pstInfo
            = (CUMW_NOTIFY_REMOTE_DECODER_STATUS*)pstNotifyInfo->pNotifyInfo; 

        long lIndex = 0;
        if (IVS_FAIL == pThis->GetIdleThread(lIndex))
        {
            IVS_RUN_LOG_ERR("ControlBus Handle Notify Call Back can't find idle thread. szDecoderID = %s.", pstInfo->szDecoderID);
            break;
        }

        memset(pThis->m_objProcEventThread[lIndex].m_szDecoderID, 0, CUMW_DECODER_ID_LEN + 1);
        if(!CToolsHelp::Memcpy(pThis->m_objProcEventThread[lIndex].m_szDecoderID, CUMW_DECODER_ID_LEN, pstInfo->szDecoderID, CUMW_DECODER_ID_LEN))
		{
			IVS_RUN_LOG_ERR("ControlBus Handle Notify Call Back Copy DecoderID failed");
			break;
		}

        IVS_DBG_LOG("ControlBus Handle Notify Call Back szDecoderID = %s. lIndex = %ld.", pstInfo->szDecoderID, lIndex);
        break;
    }

    case CUMW_NOTIFY_TYPE_REMOTE_DECODER_UNRIGISTER:
    case CUMW_NOTIFY_TYPE_REMOTE_DECODER_DISCONNECT:
        {
            CUMW_NOTIFY_REMOTE_DECODER_OFF_LINE_INFO* pstInfo
                = (CUMW_NOTIFY_REMOTE_DECODER_OFF_LINE_INFO*)pstNotifyInfo->pNotifyInfo; 

            long lIndex = 0;
            IVS_INT32 lRet = pThis->FindThread(pstInfo->szDecoderID, lIndex);
            if (IVS_FAIL == lRet)
            {
                IVS_RUN_LOG_ERR("ControlBus Handle Notify Call Back can't find idle thread. szDecoderID = %s.", pstInfo->szDecoderID);
                break;
            }

            pThis->m_objProcEventThread[lIndex].m_bIdle =  true;
            memset(pThis->m_objProcEventThread[lIndex].m_szDecoderID, 0, CUMW_DECODER_ID_LEN + 1);

            IVS_DBG_LOG("ControlBus Handle Notify Call Back delete szDecoderID = %s. lIndex = %ld.", pstInfo->szDecoderID, lIndex);
            break;
        }
	case CUMW_NOTIFY_TYPE_START_DECODER:
		{//����Ҫ����
			break;
		}
	case CUMW_NOTIFY_TYPE_DECODER_ERROR:             //������֪ͨ����
		{
			break;
		}
	case CUMW_NOTIFY_TYPE_AUDIO_CONTROL:             //����/ֹͣ������Ƶ��Ϣ֪ͨ
		{
			break;
		}
	case CUMW_NOTIFY_TYPE_SET_VOLUME:                //����������Ϣ֪ͨ
		{
			break;
		}
    default:
    {
		IVS_RUN_LOG_ERR("ControlBus Handle Notify Call Back unknown type = %lu.", pstNotifyInfo->ulNotifyType);
        break;
    }
    }

    //ֱ��͸��֪ͨ�ص�����
    lResult = pThis->m_objStatusNotifyCallBack.NotifyCallBack(*pstNotifyInfo);
    if (IVS_SUCCEED != lResult)
    {
        IVS_RUN_LOG_ERR( "ControlBus Handle Notify Call Back Failed. NotifyType = 0x%04X. UI Return Error(0x%04X).",
                pstNotifyInfo->ulNotifyType, lResult);
        return IVS_FAIL;
    }

    IVS_DBG_LOG("ControlBus Handle Notify Call Back Success. NotifyType = 0x%04X.", pstNotifyInfo->ulNotifyType);
    return IVS_SUCCEED;
}

IVS_INT32 CControlBus::GetIdleThread(long& lIndex)
{
    CLockGuard locker(m_pThreadMutex);

    IVS_DBG_LOG("Enter ControlBus GetIdleThread.");

    for (long i = 0; i < THREAD_MAX_COUNT; i++)
    {
        if (m_objProcEventThread[i].m_bIdle)
        {
            m_objProcEventThread[i].m_bIdle = false;
            lIndex = i;

            IVS_DBG_LOG("Out ControlBus GetIdleThread. lIndex = %ld", lIndex);

            return IVS_SUCCEED;
        }
    }

    IVS_RUN_LOG_ERR( "ControlBus GetIdleThread. can't find idle thread.");
    return IVS_FAIL;
}

IVS_INT32 CControlBus::FindThread(const char* pszDecoderID, long& lIndex)
{
    CLockGuard locker(m_pThreadMutex);

    IVS_DBG_LOG("Enter ControlBus FindThread.");

    for (long i = 0; i < THREAD_MAX_COUNT; i++)
    {
        if (0 == memcmp(m_objProcEventThread[i].m_szDecoderID, pszDecoderID, CUMW_DECODER_ID_LEN))
        {
            lIndex = i;

            IVS_DBG_LOG("Out ControlBus FindThread. szDecoderID = %s, lIndex = %ld", pszDecoderID, lIndex);

            return IVS_SUCCEED;
        }
    }

    IVS_RUN_LOG_ERR( "ControlBus FindThread can't find idle thread. szDecoderID = %s", pszDecoderID);
    return IVS_FAIL;
}

/**************************************************************************
* name       : HandleNotifyAsyncRet
* description: �����첽����֪ͨ
* input      : stNotifyInfo   ֪ͨ��Ϣ
* output     : NA
* return     : long ������
* remark     : NA
**************************************************************************/
IVS_INT32 CControlBus::HandleNotifyAsyncRet(CUMW_NOTIFY_INFO& stNotifyInfo)
{
    if (NULL == stNotifyInfo.pNotifyInfo)
    {
        IVS_RUN_LOG_ERR( "Handle Asynchronous Return Notify Failed. Notify Info is Emtpy.");
        return IVS_PARA_INVALID;
    }

    if (sizeof(CUMW_NOTIFY_ASYNC_RET) != stNotifyInfo.ulNotifyInfoLen)
    {
        IVS_RUN_LOG_ERR( "Handle Asynchronous Return Notify Failed. Notify Info Size(%lu) Must be %d.",
                stNotifyInfo.ulNotifyInfoLen, sizeof(CUMW_NOTIFY_ASYNC_RET));
        return IVS_PARA_INVALID;
    }

    //����ֵ
    long lResult = IVS_SUCCEED;

    CUMW_NOTIFY_ASYNC_RET& stAsyncRet = *(CUMW_NOTIFY_ASYNC_RET*)stNotifyInfo.pNotifyInfo;

    IVS_DBG_LOG(
            "[ AsyncSeq = %lu ] Handle Asynchronous Return Notify Begin. AsyncType = 0x%04X. RetCode = 0x%04X. BusinessID = %lu.",
            stAsyncRet.ulAsyncSeq, stAsyncRet.ulAsyncType, stAsyncRet.ulAsyncRetCode, stAsyncRet.ulBusinessID);

    if ((CUMW::ASYNC_TYPE_MEDIA_CHANNEL_OPEN != stAsyncRet.ulAsyncType)
        && (CUMW::ASYNC_TYPE_MEDIA_CHANNEL_CLOSE != stAsyncRet.ulAsyncType))
    {
        //ֱ��͸��֪ͨ�ص�����
        lResult = m_objStatusNotifyCallBack.NotifyCallBack(stNotifyInfo);
    }

    if (IVS_SUCCEED != lResult)
    {
        IVS_RUN_LOG_ERR(
                "[ AsyncSeq = %lu ] Handle Asynchronous Return Notify Failed. AsyncType = 0x%04X. Result = 0x%04X.",
                stAsyncRet.ulAsyncSeq, stAsyncRet.ulAsyncType, lResult);
        return lResult;
    }

    IVS_DBG_LOG("[ AsyncSeq = %lu ] Handle Asynchronous Return Notify Success. AsyncType = 0x%04X.",
            stAsyncRet.ulAsyncSeq, stAsyncRet.ulAsyncType);

    return lResult;
}

/**************************************************************************
* name       : ProcEventThreadEntry
* description: �����¼��߳����
* input      : pPara   �߳���ڲ���
* output     : NA
* return     : long ������
* remark     : NA
**************************************************************************/
unsigned long __stdcall CControlBus::ProcEventThreadEntry(void* pPara)
{
    //����ָ�밲ȫת��
    EVENT_THREAD_PARA* pThreadPara = VOS_CAST((EVENT_THREAD_PARA*)pPara);
    CControlBus* pThis = VOS_CAST((CControlBus*)pThreadPara->pThread);

    if (NULL == pThis)
    {
        IVS_RUN_LOG_ERR( "Control Bus Process Event Thread Parameter Invalidate. Parameter Address is %#p.", pPara);
        return IVS_PARA_INVALID;
    }

    //ִ�д����¼��߳�
    pThis->ProcEventThread(pThreadPara->i);

    return IVS_SUCCEED;
}

/**************************************************************************
* name       : ProcEventThread
* description: �����¼��߳�
* input      : NA
* output     : NA
* return     : long ������
* remark     : NA
**************************************************************************/
void CControlBus::ProcEventThread(int i)
{
    IVS_DBG_LOG("Enter Control Bus Process Event Thread.");

    ////m_mapIdToBusiness.clear();

    //����ֵ
    long lResult = IVS_SUCCEED;

    //�¼���Ϣָ��
    CUMW::EVENT_INFO* pstEventInfo = VOS_NULL;

    while (m_objProcEventThread[i].m_bThreadRun)
    {
        //����Ϊ��ʱ�����ȣ�ֱ����ȡһ���¼����������˳����̣߳��������в���EVENT_TYPE_EXIT_THREAD�¼�
        lResult = m_objProcEventThread[i].m_objEventSyncQueue.popFrontEv(pstEventInfo);

        IVS_DBG_LOG("ControlBus ProcEventThread. ThreadID is %d", i);

        //ȡ�¼�ʧ�ܣ�����Ϊ���¼��������
        if ((VOS_OK != lResult) || (VOS_NULL == pstEventInfo))
        {
            continue;
        }

        //�ֱ���ͬ���¼�
        switch ((long)pstEventInfo->enEventType)
        {
        case CUMW::EVENT_TYPE_CTRLBUS_BUSINESS: //ý��ҵ���¼�
        {
            lResult = HandleEventBusiness(pstEventInfo->pEventInfo, pstEventInfo->ulEventInfoLen);

            //�ͷ��¼���Ϣ
            VOS_DELETE((BUSINESS_ASYNC_INFO * &)pstEventInfo->pEventInfo);
            break;
        }

        case CUMW::EVENT_TYPE_EXIT_THREAD:    //�߳��˳�
        {
            IVS_DBG_LOG("Receive Thread Exit Event. Control Bus Process Event Thread Will Exit.");
            m_objProcEventThread[i].m_bThreadRun = FALSE;

            //VOS_DELETE((char*&)pstEventInfo->pEventInfo, MULTI); //û����Ϣ��
            break;
        }

        default:
        {
            IVS_RUN_LOG_ERR("Receive Unsupport Event Type(0x%04X) On Control Bus Process Event Thread.");

            //��ֹ���ַǷ�ֵ
            pstEventInfo->enEventType = CUMW::EVENT_TYPE_INVALID;
            break;
        }
        }

        if (IVS_SUCCEED != lResult)
        {
            IVS_RUN_LOG_ERR( "Error(0x%04X) In Control Bus Process Event Thread. %s.",
                    lResult, CUMW::STR_ARR_EVENT_TYPE[pstEventInfo->enEventType]);
        }

        //�ͷ��¼�
        VOS_DELETE(pstEventInfo);
    }

    //ֹͣ����ҵ��
    ////StopAllBusiness();

    //����¼�ͬ������
    while (VOS_OK == m_objProcEventThread[i].m_objEventSyncQueue.popFrontEv(pstEventInfo, 0, QUEUE_MODE_NOWAIT))
    {    
        //begin: modify by liangyun at 2012.2.14.15:00
        if (CUMW::EVENT_TYPE_CTRLBUS_BUSINESS == (long)pstEventInfo->enEventType)
        {
            BUSINESS_ASYNC_INFO* pstAsyncInfo = (BUSINESS_ASYNC_INFO*)pstEventInfo->pEventInfo;
            if ((NULL != pstAsyncInfo) && (NULL != pstAsyncInfo->pParameter) )
            {
                CBusiness* pBusiness = (CBusiness*)pstAsyncInfo->pParameter;
                if (NULL != pBusiness)
                {
                    VOS_DELETE(pBusiness);
                }
            }
            
            if (NULL != pstAsyncInfo)
            {
                VOS_DELETE(pstAsyncInfo);
            }
        }
        //end: modify by liangyun at 2012.2.14.15:00

        //�ͷ��¼�
        VOS_DELETE(pstEventInfo);
    }

    IVS_DBG_LOG("Exit  Control Bus Process Event Thread.");
}

/**************************************************************************
* name       : AddBuesinessEvent
* description: ���ҵ���¼�
* input      : ulAsynSeq      �첽���
               enAsynType  �첽����
               ulBusinessId    ҵ��ID
               pParameter  ����ָ�룬ȡ���� �첽����
* output     : NA
* return     : long ������
* remark     : NA
**************************************************************************/
IVS_INT32 CControlBus::AddBuesinessEvent
(   IN unsigned long       ulAsynSeq,
    IN CUMW::ASYNC_TYPE    enAsynType,
    IN unsigned long       ulBusinessId,
    IN void*               pParameter)
{
	IVS_DBG_LOG("Add Event To Control Bus Synchronous Queue, BusinessID = %lu.", ulBusinessId);

    //���캯���г�ʼ��
    BUSINESS_ASYNC_INFO* pstAsyncInfo = VOS_NEW(pstAsyncInfo);

    if (NULL == pstAsyncInfo)
    {
        IVS_RUN_LOG_ERR( "[ AsyncSeq = %lu ] Create Business Asynchronous Info Failed "
                "On Add Business Event To Control Bus Synchronous Queue.", ulAsynSeq);
        return IVS_ALLOC_MEMORY_ERROR;
    }

    //����ҵ���첽��Ϣ
    pstAsyncInfo->ulAsyncSeq   = ulAsynSeq;
    pstAsyncInfo->enAsyncType  = enAsynType;
    pstAsyncInfo->ulBusinessId = ulBusinessId;
    pstAsyncInfo->pParameter = pParameter;

    CBusiness* pBusiness = (CBusiness*)pParameter;

    long lIndex = 0;
    IVS_INT32 lResult = FindThread(pBusiness->m_szDecoderId, lIndex);
    if (IVS_FAIL == lResult)
    {
        //�ͷ���ʱ�ڴ�
        VOS_DELETE(pstAsyncInfo);

        IVS_RUN_LOG_ERR( "[ AysncSeq = %lu ] Add Event To Control Bus Synchronous Queue Failed", ulAsynSeq);
        return lResult;
    }

    //���¼�ͬ�����в����¼�
    lResult = m_objProcEventThread[lIndex].AddEvent(CUMW::EVENT_TYPE_CTRLBUS_BUSINESS, pstAsyncInfo, sizeof(*pstAsyncInfo));
    if (IVS_SUCCEED != lResult)
    {
        //�ͷ���ʱ�ڴ�
        VOS_DELETE(pstAsyncInfo);

        IVS_RUN_LOG_ERR( "[ AysncSeq = %lu ] Add Event To Control Bus Synchronous Queue Failed", ulAsynSeq);
        return lResult;
    }

	IVS_DBG_LOG("Add Event To Control Bus Synchronous Queue Suc, BusinessID = %lu.", ulBusinessId);

    return lResult;
}

/**************************************************************************
* name       : AddGeneralAsynEvent
* description: ���ͨ���첽�¼�
* input      : ulAsynSeq      �첽���
               enAsynType  �첽����
* output     : NA
* return     : long ������
* remark     : NA
**************************************************************************/
IVS_INT32 CControlBus::AddGeneralAsynEvent
(   IN unsigned long    ulAsynSeq,
    IN CUMW::ASYNC_TYPE enAsynType)
{
    //���캯���г�ʼ��
    /*CUMW::GENERAL_ASYN_INFO* pstAsynInfo = VOS_NEW(pstAsynInfo);

    if (NULL == pstAsynInfo)
    {
        IVS_RUN_LOG_ERR( "[ AsyncSeq = %lu ] Create General Asynchronous Info Failed "
                "On Add General Asynchronous Event To Control Bus Synchronous Queue.", ulAsynSeq);
        return IVS_ALLOC_MEMORY_ERROR;
    }

    //�����첽��Ϣ
    pstAsynInfo->ulAsynSeq  = ulAsynSeq;
    pstAsynInfo->enAsynType = enAsynType;

    //���¼�ͬ�����в����¼�
    long lResult = m_objProcEventThread[0].AddEvent(CUMW::EVENT_TYPE_GENERAL_ASYN, pstAsynInfo, sizeof(*pstAsynInfo));
    if (IVS_SUCCEED != lResult)
    {
        //�ͷ���ʱ�ڴ�
        VOS_DELETE(pstAsynInfo);

        IVS_RUN_LOG_ERR( "[ AysncSeq = %lu ] Add Event To Control Bus Synchronous Queue Failed", ulAsynSeq);
        return lResult;
    }*/

    return IVS_SUCCEED;
}//lint !e1762

/**************************************************************************
* name       : HandleNotifyEvent
* description: �����¼�֪ͨ
* input      : stNotifyInfo   ֪ͨ��Ϣ
* output     : NA
* return     : long ������
* remark     : NA
**************************************************************************/
IVS_INT32 CControlBus::HandleNotifyEvent
(   IN CUMW_NOTIFY_INFO& stNotifyInfo)
{
    if (NULL == stNotifyInfo.pNotifyInfo)
    {
        IVS_RUN_LOG_ERR( "Handle Event Notify Failed. Notify Info is Emtpy.");
        return IVS_FAIL;
    }

    if (sizeof(CUMW_NOTIFY_EVENT) != stNotifyInfo.ulNotifyInfoLen)
    {
        IVS_RUN_LOG_ERR( "Handle Event Notify Failed. Notify Info Size(%lu) Must be %d.",
                stNotifyInfo.ulNotifyInfoLen, sizeof(CUMW_NOTIFY_EVENT));
        return IVS_FAIL;
    }

    long lResult = IVS_SUCCEED;

    CUMW_NOTIFY_EVENT& stNotifyEvent = *(CUMW_NOTIFY_EVENT*)stNotifyInfo.pNotifyInfo;

    IVS_DBG_LOG("Handle Event Notify Begin. EventType = 0x%04X.", stNotifyEvent.ulEventType);

    //Ĭ��ֱ��͸��֪ͨ�ص�����
    lResult = m_objStatusNotifyCallBack.NotifyCallBack(stNotifyInfo);

    IVS_DBG_LOG("Handle Event Notify End. EventType = 0x%04X. Result = 0x%04X.",
            stNotifyEvent.ulEventType, lResult);

    return lResult;
}

/**************************************************************************
* name       : CheckStatus
* description: ����Ƿ�ﵽ�ﵽԤ��״̬
* input      : enExpectedStatus Ԥ��״̬
* output     : NA
* return     : long ������
* remark     : NA
**************************************************************************/
IVS_INT32 CControlBus::CheckStatus(CUMW::LOGIN_STATUS enExpectedStatus)
{
    //��δ�ﵽ��ǰ״̬
    if (m_enLoginStatus < enExpectedStatus)
    {
        IVS_RUN_LOG_ERR( "CUMW Check Status Failed. Current Status: %s. Expectd Status = %d.",
                CUMW::STR_LOGIN_STATUS[m_enLoginStatus], enExpectedStatus);
        return IVS_FAIL;
    }

    //StreamMgrָ�벻��Ϊ��
    if (NULL == m_pStreamMgr)
    {
        IVS_RUN_LOG_ERR( "CUMW Init Status is Abnormal. Stream Manager Address is NULL.");
        return IVS_TVWALL_MEMORY_NULL;
    }

    return IVS_SUCCEED;
} //lint !e1762 ����ÿ�����ô˺����ĵط������ظ�����ָ��

/**************************************************************************
* name       : AddBusinessCtrlEvent
* description: ���ҵ������¼�
* input      : ulAsyncSeq                �첽���
               ulBusinessID              ҵ��ID
               pParaInfo                 ���Ʋ���
               ulParaLength              ���Ʋ�������
               enCtrlCode                ������
* output     : NA
* return     : long ������
* remark     : NA
**************************************************************************/
IVS_INT32 CControlBus::AddBusinessCtrlEvent
(   IN const unsigned long            ulAsyncSeq,
    IN const unsigned long            ulBusinessID,
    IN const void*                    pParaInfo,
    IN const unsigned long            ulParaLength,
    IN const CUMW::BUSINESS_CTRL_CODE enCtrlCode)
{
    IVS_DBG_LOG("[ AysncSeq = %lu ][ BusinessID = %lu ] Add Business Ctrl Event Begin, Ctrl Code: %d.", ulAsyncSeq,
            ulBusinessID, enCtrlCode);

    unsigned long ulCtrlParaLen = sizeof(CUMW::BUSINESS_CTRL_PARA) + ulParaLength - 1;
    CUMW::BUSINESS_CTRL_PARA* pCtrlPara = (CUMW::BUSINESS_CTRL_PARA*)VOS_NEW((char* &)pCtrlPara, ulCtrlParaLen); //lint !e826 �ɱ�����
    if (NULL == pCtrlPara)
    {
        IVS_RUN_LOG_ERR(
                "[ AysncSeq = %lu ][ BusinessID = %lu ] Create Business Control Param Failed On Add Business Ctrl.",
                ulAsyncSeq,
                ulBusinessID);
        return IVS_ALLOC_MEMORY_ERROR;
    }

    memset((char*) pCtrlPara, 0, ulCtrlParaLen);

    //���ÿ��Ʋ���
    pCtrlPara->enCtrlCode = enCtrlCode;
    pCtrlPara->ulCtrlParaLen = ulParaLength;
    if(CToolsHelp::Memcpy(pCtrlPara->vCtrlPara, ulParaLength, pParaInfo, ulParaLength))    //lint !e419 �ɱ�����
	{
		IVS_RUN_LOG_ERR("Add Business Ctrl Event Copy CtrlPara failed");
		return IVS_ALLOC_MEMORY_ERROR;
	}
    //���ҵ������¼�
    long lResult = AddBuesinessEvent(ulAsyncSeq, CUMW::ASYNC_TYPE_BUSINESS_CTRL, ulBusinessID, pCtrlPara);
    if (IVS_SUCCEED != lResult)
    {
        IVS_RUN_LOG_ERR( "[ AysncSeq = %lu ][ BusinessID = %lu ] Add Business Ctrl Event Failed On Add Business Ctrl.",
                ulAsyncSeq, ulBusinessID);
        VOS_DELETE(pCtrlPara, MULTI);
        return lResult;
    }

    IVS_DBG_LOG("[ AysncSeq = %lu ][ BusinessID = %lu ] Add Business Ctrl Event Success.", ulAsyncSeq, ulBusinessID);
    return lResult;
}

/**************************************************************************
* name       : StartBusiness
* description: Զ�̽�������ʼ����
* input      : pParaInfo   ����
* output     : NA
* return     : long ������
* remark     : NA
**************************************************************************/
IVS_INT32 CControlBus::StartBusiness
(   IN CBusiness& objBusiness)
{
    if (NULL == m_pStreamMgr)
    {
        IVS_RUN_LOG_ERR( "Start Business In Control Bus Failed. Stream Manager is Empty.");
        return IVS_TVWALL_MEMORY_NULL;
    }

    long lResult = IVS_SUCCEED;

    const unsigned long ulBusinessId = objBusiness.GetBusinessId();

    IVS_DBG_LOG("[ AsyncSeq = %lu ][ BusinessID = %lu ] Start Business In Control Bus Begin.",
            objBusiness.m_ulAsyncSeq, ulBusinessId);

    //��ʼ��ҵ��
    lResult = m_pStreamMgr->InitBusiness(objBusiness);
    if (IVS_SUCCEED != lResult)
    {
        IVS_RUN_LOG_ERR(
                "[ AsyncSeq = %lu ][ BusinessID = %lu ] Init Business Failed On Start Business In Control Bus.",
                objBusiness.m_ulAsyncSeq, ulBusinessId);
        return lResult;
    }

    //��������ӵ�ӳ���StartBusiness�еĻص����õ�
    m_mapIdToBusiness[ulBusinessId] = &objBusiness;

    CBusiness* pSharedBusiness = NULL;
    lResult = GetBusinessById(objBusiness.m_ulSharedBusinessId, pSharedBusiness);
    if (IVS_SUCCEED != lResult)
    {
        IVS_RUN_LOG_ERR(
                "[ AsyncSeq = %lu ][ BusinessID = %lu ] Get Shared Business Object Failed On Start Business In Control Bus.",
                objBusiness.m_ulAsyncSeq, ulBusinessId);
        return lResult;
    }

    //����ҵ��
    lResult = RunBusiness(objBusiness);
    if (IVS_SUCCEED != lResult)
    {
        IVS_RUN_LOG_ERR(
                "[ AsyncSeq = %lu ][ BusinessID = %lu ] Start Business Failed On Start Business In Control Bus.",
                objBusiness.m_ulAsyncSeq, ulBusinessId);
        return lResult;
    }

    IVS_DBG_LOG("[ AsyncSeq = %lu ][ BusinessID = %lu ] Start Business In Control Bus Success.",
            objBusiness.m_ulAsyncSeq, ulBusinessId);
    return IVS_SUCCEED;
}

/**************************************************************************
* name       : RunBusiness
* description: ����ҵ��
* input      : objBusiness     ҵ���������
* output     : NA
* return     : long ������
* remark     : NA
**************************************************************************/
IVS_INT32 CControlBus::RunBusiness
(   IN CBusiness& objBusiness)
{
    if (NULL == m_pStreamMgr)
    {
        IVS_RUN_LOG_ERR( "Run Business In Control Bus Failed. Stream Manager is Empty.");
        return IVS_TVWALL_MEMORY_NULL;
    }

    long lResult = IVS_SUCCEED;

    const unsigned long ulBusinessId = objBusiness.GetBusinessId();

    IVS_DBG_LOG("[ BusinessID = %lu ] Run Business In Control Bus Begin.", ulBusinessId);

    //��ʼҵ��
    lResult = m_pStreamMgr->StartBusiness(objBusiness);

    if (IVS_SUCCEED != lResult)
    {
        IVS_RUN_LOG_ERR( "[ BusinessID = %lu ] Run Business Failed On Handle Start Business Event.", ulBusinessId);
        return lResult;
    }

    IVS_DBG_LOG("[ AsyncSeq = %lu ][ BusinessID = %lu ] Run Business In Control Bus Success.",
            objBusiness.m_ulAsyncSeq, ulBusinessId);
    return IVS_SUCCEED;
}

/**************************************************************************
* name       : StopBusinessWhole
* description: ֹͣ����ҵ�񣬰�������ֹͣ���ƽ̨
* input      : objBusiness     ��ֹͣ��ҵ���������
* output     : NA
* return     : long ������
* remark     : NA
**************************************************************************/
IVS_INT32 CControlBus::StopBusinessWhole
(   IN CBusiness& objBusiness)
{
    if (NULL == m_pStreamMgr)
    {
        IVS_RUN_LOG_ERR( "Stop Business Whole In Control Bus Failed. Stream Manager is Empty.");
        return IVS_FAIL;
    }

    const unsigned long ulBusinessId = objBusiness.GetBusinessId();

    long lResult = IVS_SUCCEED;

    IVS_DBG_LOG("[ BusinessID = %lu ] Stop Business Whole In Control Bus Begin.", ulBusinessId);

    //ɾ��ҵ�������
    CBusiness* pBusiness = &objBusiness;
    VOS_DELETE(pBusiness);

    IVS_DBG_LOG("[ BusinessID = %lu ] Stop Business Whole In Control Bus End. Result = 0x%04X.", ulBusinessId,
            lResult);
    return lResult;
}//lint !e1762

/**************************************************************************
* name       : HandleEventBusiness
* description: ����ý��ҵ���¼�
* input      : pEventInfo      �¼���Ϣָ��
               ulEventInfoLen  �¼���Ϣ���ݳ���
* output     : NA
* return     : long ������
* remark     : NA
**************************************************************************/
IVS_INT32 CControlBus::HandleEventBusiness
(   IN void*           pEventInfo,
    IN unsigned long   ulEventInfoLen)
{
    if (NULL == pEventInfo)
    {
        IVS_RUN_LOG_ERR( "Handle Business Event Failed. Event Info is Empty.");
        return IVS_PARA_INVALID;
    }

    if (sizeof(BUSINESS_ASYNC_INFO) != ulEventInfoLen)
    {
        IVS_RUN_LOG_ERR( "Handle Business Event Failed. Event Info Length(%ld) Must be %d.",
                ulEventInfoLen, sizeof(BUSINESS_ASYNC_INFO));
        return IVS_PARA_INVALID;
    }

    long lResult = IVS_SUCCEED;

    BUSINESS_ASYNC_INFO& stAsyncInfo = *(BUSINESS_ASYNC_INFO*)pEventInfo;

    IVS_DBG_LOG("[ AsyncSeq = %lu ][ BusinessID = %lu ] Handle Business Event Begin.",
            stAsyncInfo.ulAsyncSeq, stAsyncInfo.ulBusinessId);

    switch ((long)stAsyncInfo.enAsyncType)
    {
    case CUMW::ASYNC_TYPE_BUSINESS_START:
    {
        lResult = HandleEventBusinessStart(stAsyncInfo.pParameter, stAsyncInfo.ulAsyncSeq);
        break;
    }

    case CUMW::ASYNC_TYPE_BUSINESS_RUN:
    {
        lResult = HandleEventBusinessRun(stAsyncInfo.ulBusinessId);
        break;
    }

    case CUMW::ASYNC_TYPE_DECORDER_INIT:
    {
        lResult = HandleEventIniBusiness(stAsyncInfo);
        break;
    }

    case CUMW::ASYNC_TYPE_DECORDER_STOP:
    {
        lResult = HandleEventStopBusiness(stAsyncInfo);
        break;
    }
    case CUMW::ASYNC_TYPE_DECORDER_START:
    {
        lResult = HandleEventStartBusiness(stAsyncInfo);
        break;
    }
    case CUMW::ASYNC_TYPE_AUDIO_CONTROL:
    {
        lResult = HandleEventPlayAudio(stAsyncInfo);
        break;
    }
    case CUMW::ASYNC_TYPE_TUNEUP_VOLUME:
    {
        lResult = HandleEventSetVolume(stAsyncInfo);
        break;
    }
    default:
    {
        IVS_RUN_LOG_ERR(
                "[ AsyncSeq = %lu ][ BusinessID = %lu ] Handle Business Event Failed. AsyncType(0x%04X) Unsurpport.",
                stAsyncInfo.ulAsyncSeq, stAsyncInfo.ulBusinessId, stAsyncInfo.enAsyncType);
        return IVS_FAIL;

        //break;
    }
    }

    if (IVS_SUCCEED != lResult)
    {
        IVS_RUN_LOG_ERR( "[ AsyncSeq = %lu ][ BusinessID = %lu ] Handle Business Event Failed. ErrorCode = 0x%04X.",
                stAsyncInfo.ulAsyncSeq, stAsyncInfo.ulBusinessId, lResult);
        return lResult;
    }

    IVS_DBG_LOG("[ AsyncSeq = %lu ][ BusinessID = %lu ] Handle Business Event Success.",
            stAsyncInfo.ulAsyncSeq, stAsyncInfo.ulBusinessId);

    return IVS_SUCCEED;
}

/**************************************************************************
* name       : HandleEventBusinessStart
* description: ����ý��ҵ��ʼ�¼�
* input      : pParameter  ����ָ��
               ulAsyncSeq  �첽���
* output     : NA
* return     : long ������
* remark     : NA
**************************************************************************/
IVS_INT32 CControlBus::HandleEventBusinessStart
(   IN void*           pParameter,
    IN unsigned long   ulAsyncSeq)
{
    CBusiness* pBusiness = VOS_CAST((CBusiness*)pParameter);

    if (NULL == pBusiness)
    {
        IVS_RUN_LOG_ERR( "[ AsyncSeq = %lu ] Handle Start Business Event Failed. Business Object is Empty.",
                ulAsyncSeq);
        return IVS_PARA_INVALID;
    }

    long lResult = IVS_SUCCEED;

    IVS_DBG_LOG("[ AsyncSeq = %lu ] Handle Start Business Event Begin.", ulAsyncSeq);

    //�����첽���
    pBusiness->m_ulAsyncSeq = ulAsyncSeq;

    lResult = StartBusiness(*pBusiness);

    //����ʱ���ϱ�֪ͨ������ʱ��StartBusiness�л����������ϱ�
    if (IVS_SUCCEED != lResult)
    {
        IVS_RUN_LOG_ERR( "[ AsyncSeq = %lu ] Start Business Failed On Handle Start Business Event. "
                "Will Notify Asynchorous Return, and Stop Business. ErrorCode = 0x%04X.", ulAsyncSeq, lResult);

        //�ϱ��첽����֪ͨ
        (void)m_objStatusNotifyCallBack.NotifyBusinessAsynRet(ulAsyncSeq, lResult, pBusiness->GetBusinessId());

        //ʧ��ʱֹͣҵ�񣬻��ͷ�pBusiness���󣬺��治����ʹ��
        (void)StopBusinessWhole(*pBusiness);

        return lResult;
    }

    IVS_DBG_LOG("[ AsyncSeq = %lu ] Handle Start Business Event Success.", ulAsyncSeq);
    return IVS_SUCCEED;
}

/**************************************************************************
* name       : HandleEventBusinessRun
* description: ����ý��ҵ�������¼�
* input      : ulBusinessId    ҵ��ID
* output     : NA
* return     : long ������
* remark     : NA
**************************************************************************/
IVS_INT32 CControlBus::HandleEventBusinessRun
(   IN unsigned long ulBusinessId)
{
    IVS_DBG_LOG("[ BusinessID = %lu ] Handle Run Business Event Begin.", ulBusinessId);

    long lResult = IVS_SUCCEED;

    CBusiness* pBusiness = NULL;
    lResult = GetBusinessById(ulBusinessId, pBusiness);
    if (IVS_SUCCEED != lResult)
    {
        IVS_RUN_LOG_ERR( "[ BusinessID = %lu ] Handle Run Business Event Failed. Business Object Not Exist.",
                ulBusinessId);
        return lResult;
    }

    //����ҵ��
    lResult = RunBusiness(*pBusiness);

    //����ʱ���ϱ�֪ͨ������ʱ��RunBusiness�л����������ϱ�
    if (IVS_SUCCEED != lResult)
    {
        IVS_RUN_LOG_ERR( "[ BusinessID = %lu ] Run Business Failed On Handle Run Business Event. "
                "Will Notify Asynchorous Return, and Stop Business.", ulBusinessId);

        //�ϱ��첽����֪ͨ
        (void)m_objStatusNotifyCallBack.NotifyBusinessAsynRet(pBusiness->m_ulAsyncSeq, lResult, ulBusinessId);

        //ʧ��ʱֹͣҵ��
        (void)StopBusinessWhole(*pBusiness);

        //ʧ�ܺ����
        //return lResult;
    }

    //BEGIN R002C01SPC100 ADD 2011-07-26 ligengqiang 00124479 for ֧����ƵPayloadType��̬�仯
    //�����Ƿ��й���ҵ��
    MAP_SHARED_BUSINESS_ID::iterator iter = m_mapSharedBusinessId.find(ulBusinessId);
    if (m_mapSharedBusinessId.end() == iter)
    {
        IVS_DBG_LOG("[ BusinessID = %lu ] Handle Run Business Event Success. No Shared Business.", ulBusinessId);
        return IVS_SUCCEED;
    }

    VECTOR_BUSINESS_ID& vectorSharedId = iter->second;

    //�����������й���ҵ��
    VECTOR_BUSINESS_ID::iterator iterShared = vectorSharedId.begin();
    for (; iterShared != vectorSharedId.end(); ++iterShared)
    {
        unsigned long& ulSharedBusinessId = *iterShared;
        CBusiness* pSharedBusiness = NULL;
        lResult = GetBusinessById(ulSharedBusinessId, pSharedBusiness);
        if (IVS_SUCCEED != lResult)
        {
            IVS_RUN_LOG_ERR( "[ BusinessID = %lu ] Handle Run Business Event Failed. Business Object Not Exist.",
                    ulSharedBusinessId);

            //ʧ�ܺ����
            continue;
        }

        //���Ʒ������˵�ý����Ϣ
        pSharedBusiness->CopyServerMediaInfo(*pBusiness);

        //����ҵ��
        lResult = RunBusiness(*pSharedBusiness);

        //����ʱ���ϱ�֪ͨ������ʱ��RunBusiness�л����������ϱ�
        if (IVS_SUCCEED != lResult)
        {
            IVS_RUN_LOG_ERR( "[ BusinessID = %lu ] Run Business Failed On Handle Run Business Event. "
                    "Will Notify Asynchorous Return, and Stop Business.", ulSharedBusinessId);

            //�ϱ��첽����֪ͨ
            (void)m_objStatusNotifyCallBack.NotifyBusinessAsynRet(pSharedBusiness->m_ulAsyncSeq, lResult,
                                                                  ulSharedBusinessId);

            //ʧ��ʱֹͣҵ��
            (void)StopBusinessWhole(*pSharedBusiness);

            //ʧ�ܺ����
            //return lResult;
        }
        else
        {
            IVS_DBG_LOG(
                    "[ AsyncSeq = %lu ][ BusinessID = %lu ] Start Business In Control Bus In. Will Notify Stream Status : Arrive.",
                    pSharedBusiness->m_ulAsyncSeq, ulSharedBusinessId);
        }
    }

    //END   R002C01SPC100 ADD 2011-07-26 ligengqiang 00124479 for ֧����ƵPayloadType��̬�仯

    IVS_DBG_LOG("[ BusinessID = %lu ] Handle Run Business Event Success.", ulBusinessId);
    return IVS_SUCCEED;
}

/**************************************************************************
* name       : GetBusinessByID4Invite
* description: ��ȡý��ҵ��re-invite���ҵ��
* input      : ulBusinessID   ҵ��ID
* output     : pBusiness      �첽��Ϣ
* return     : long ������
* remark     : NA
**************************************************************************/
IVS_INT32 CControlBus::GetBusinessById4Invite
(   IN unsigned long ulBusinessID,
    IO CBusiness*&   pBusiness)
{
    IVS_DBG_LOG(
            "[ BusinessID = %lu ] Get Business Object By BusinessID Begin On Handle Get Business By ID for Invite.",
            ulBusinessID);

    long lResult = GetBusinessById(ulBusinessID, pBusiness);
    if (IVS_SUCCEED != lResult)
    {
        MAP_ID_TO_BUSINESS::iterator iter = m_mapIdToBusiness.begin();
        CBusiness* pTmpBusiness = NULL;
        for (; iter != m_mapIdToBusiness.end(); ++iter)
        {
            pTmpBusiness = iter->second;//lint !e64 
            if ((NULL != pTmpBusiness) && (pTmpBusiness->m_ulSharedBusinessId == ulBusinessID))
            {
                pBusiness = pTmpBusiness;
                IVS_DBG_LOG(
                        "[ BusinessID = %lu ] Get Business Object By Shared BusinessID Success On Handle Get Business By ID for Invite, BusinessID = %lu.",
                        ulBusinessID, pTmpBusiness->GetBusinessId());
                break;
            }
        }
    }

    if (NULL == pBusiness)
    {
        IVS_RUN_LOG_ERR( "[ BusinessID = %lu ] Get Business Object Failed On Handle Get Business By ID for Invite.",
                ulBusinessID);
        return IVS_FAIL;
    }

    IVS_DBG_LOG("[ BusinessID = %lu ] Get Business Object Success On Handle Get Business By ID for Invite.",
            ulBusinessID);
    return IVS_SUCCEED;
}

/**************************************************************************
* name       : GetNewBusinessId
* description: ��ȡ�µ�ҵ��ID
* input      : objBusiness     ��ֹͣ��ҵ���������
* output     : NA
* return     : ҵ��ID
* remark     : NA
**************************************************************************/
unsigned long CControlBus::GetNewBusinessId()
{
    (void)VOS_MutexLock(m_pBusinessIdMutex);

    if (0xFFFFFFFF == m_ulBusinessId)
    {
        m_ulBusinessId = 0;
    }

    unsigned long ulBusinessId = ++m_ulBusinessId;

    if (CUMW::INVALID_BUSINESS_ID == ulBusinessId)
    {
        ulBusinessId = ++m_ulBusinessId;
    }

    (void)VOS_MutexUnlock(m_pBusinessIdMutex);

    return ulBusinessId;
}

/**************************************************************************
* name       : HandleEventBusinessStop
* description: ����ҵ��ID��ȡҵ�����
* input      : ulBusinessId    ҵ��ID
* output     : pBusiness       ���ص�ҵ�����ָ��
* return     : long ������
* remark     : NA
**************************************************************************/
IVS_INT32 CControlBus::GetBusinessById
(   IN unsigned long    ulBusinessId,
    OUT CBusiness*&     pBusiness)
{
    MAP_ID_TO_BUSINESS::iterator iter = m_mapIdToBusiness.find(ulBusinessId);

    if (m_mapIdToBusiness.end() == iter)
    {
        IVS_RUN_LOG_ERR("Business ID(%lu) isn't Exist On Get Business.", ulBusinessId);
        return IVS_TVWALL_BUSINESS_ID_NO_EXIST;
    }

    //���ҵ�����ָ��
    pBusiness = iter->second;//lint !e64 

    return IVS_SUCCEED;
}

/**************************************************************************
* name       : StopAllBusiness
* description: ֹͣ����ҵ��ֻ���ڴ����¼��߳��д���
* input      : NA
* output     : NA
* return     : long ������
* remark     : NA
**************************************************************************/
void CControlBus::StopAllBusiness()
{
    IVS_DBG_LOG("Stop All Business Begin.");

    //����ֹͣ����ҵ��
    MAP_ID_TO_BUSINESS::iterator iter = m_mapIdToBusiness.begin();
    while (m_mapIdToBusiness.end() != iter)
    {
        CBusiness* pBusiness = iter->second;//lint !e64 

        //�����ȵ�����������ֹͣҵ��ʱ��ɾ����ǰ����
        ++iter;

        if (NULL == pBusiness)
        {
            continue;
        }

        //ֹͣҵ��
        (void)StopBusinessWhole(*pBusiness);
    }

    //���map��
    m_mapIdToBusiness.clear();

    IVS_DBG_LOG("Stop All Business End.");
}

/**************************************************************************
* name       : IniBusiness
* description: ��ʼ��������
* input      : pParaInfo   ����
* output     : NA
* return     : long ������
* remark     : NA
**************************************************************************/
IVS_INT32 CControlBus::IniBusiness(IPI_INIT_BUSINESS_REQ* pParaInfo )
{
	IVS_DBG_LOG("Ini Business Begin.");

    //���Ͻӿ���
    //CUMW::CLock objLock(m_objInterfaceLock);

    //����������
    //1��pParaInfoָ��ǿ�
    if (NULL == pParaInfo)
    {
        IVS_RUN_LOG_ERR( "Parameter Invalidate. Parameter is Empty.");
        return IVS_PARA_INVALID;
    }

    //2��m_szDecoderID����Ϊ���ַ���
    if (0 == pParaInfo->szDecoderID[0])
    {
        IVS_RUN_LOG_ERR( "[ AysncSeq = %lu ] Parameter Invalidate. Decoder ID is Empty.", pParaInfo->ulAsyncSeq);
        return IVS_PARA_INVALID;
    }

    //3��Э���ֶα���Ϊ��Чֵ
    if ((RDA_PROTOCOL_NSS_OVER_UDP > pParaInfo->ucTransportProtocol)
        || (RDA_PROTOCOL_RTP_OVER_TCP < pParaInfo->ucTransportProtocol))
    {
        IVS_RUN_LOG_ERR( "[ AysncSeq = %lu ] Parameter Invalidate. TransportProtocol is Invalidate.", pParaInfo->ulAsyncSeq);
        return IVS_PARA_INVALID;
    }

    //����ҵ����󣬹��캯�������ʼ��
    CBusiness* pBusiness = VOS_NEW(pBusiness);
    if (NULL == pBusiness)
    {
        IVS_RUN_LOG_ERR( "[ AysncSeq = %lu ] Create Business Failed.", pParaInfo->ulAsyncSeq);
        return IVS_ALLOC_MEMORY_ERROR;
    }

    pBusiness->m_ulAsyncSeq = pParaInfo->ulAsyncSeq;
    pBusiness->m_ulChannelId = pParaInfo->usChannelNo;
    (void)pBusiness->SetProtocolType(pParaInfo->ucTransportProtocol);
    (void)pBusiness->SetPlayType(CUMW_PLAY_TYPE_REMOTE);
    if(!CToolsHelp::Strncpy(pBusiness->m_szDecoderId, sizeof(pBusiness->m_szDecoderId), pParaInfo->szDecoderID, sizeof(pBusiness->m_szDecoderId) - 1))
	{
		IVS_RUN_LOG_ERR("Copy DecoderId error.");
        VOS_DELETE(pBusiness);
		return IVS_ALLOC_MEMORY_ERROR;
	}

    //����BusinessId
    const unsigned long ulBusinessId = GetNewBusinessId();
    pBusiness->SetBusinessId(ulBusinessId);

	IVS_DBG_LOG("Ini Business, BusinessID = %lu.", ulBusinessId);

    //��ӿ�ʼҵ���¼�
    long lResult = AddBuesinessEvent(pParaInfo->ulAsyncSeq, CUMW::ASYNC_TYPE_DECORDER_INIT, ulBusinessId, pBusiness);
    if (IVS_SUCCEED != lResult)
    {
        IVS_RUN_LOG_ERR( "[ AysncSeq = %lu ][ BusinessID = %lu ] Add Business Event Failed.",
                pParaInfo->ulAsyncSeq, ulBusinessId);

        //�ͷ��ڴ�
        VOS_DELETE(pBusiness); //modify by liangyun at 2012.2.14.19:28

        return lResult;
    }

    IVS_DBG_LOG("[ AysncSeq = %lu ][ BusinessID = %lu ] Ini Decoder Success.", pParaInfo->ulAsyncSeq, ulBusinessId);
    return IVS_SUCCEED;
}//lint !e818

/**************************************************************************
* name       : HandleEventIniBusiness
* description: �����ʼ����������Ϣ
* input      : objBusiness     ҵ���������
* output     : NA
* return     : long ������
* remark     : NA
**************************************************************************/
IVS_INT32 CControlBus::HandleEventIniBusiness(BUSINESS_ASYNC_INFO& stAsyncInfo)
{
    long lResult = IVS_SUCCEED;

    if (NULL == m_pStreamMgr)
    {
        IVS_RUN_LOG_ERR( "Init Business In Control Bus Failed. Stream Manager is Empty.");

        //�ϱ��첽����֪ͨ
        lResult = m_objStatusNotifyCallBack.NotifyAsynRetNoInfo(stAsyncInfo.ulAsyncSeq, IVS_FAIL);

        //�ͷ��ڴ�
        VOS_DELETE((CBusiness*&)stAsyncInfo.pParameter); //modify by liangyun at 2012.2.14.19:28

        return IVS_TVWALL_MEMORY_NULL;
    }

    CBusiness* pBusiness = (CBusiness*)stAsyncInfo.pParameter;
    const unsigned long ulBusinessId = pBusiness->GetBusinessId();

    IVS_DBG_LOG("[ BusinessID = %lu ] Init Business In Control Bus Begin.", ulBusinessId);

    //��ʼ��ҵ��
    lResult = m_pStreamMgr->InitBusiness(*pBusiness);
    if (IVS_SUCCEED != lResult)
    {
        IVS_RUN_LOG_ERR(
                "[ AsyncSeq = %lu ][ BusinessID = %lu ] Init Business Failed In Control Bus.",
                pBusiness->m_ulAsyncSeq, ulBusinessId);

        //�ͷ��ڴ�
        VOS_DELETE(pBusiness); //modify by liangyun at 2012.2.14.19:28

        return lResult;
    }

    //�ͷ��ڴ�
    VOS_DELETE(pBusiness); //modify by liangyun at 2012.2.14.19:28

    return lResult;
}

/**************************************************************************
* name       : StartBusiness
* description: Զ�̽�������ʼ����
* input      : pParaInfo   ����
* output     : NA
* return     : long ������
* remark     : NA
**************************************************************************/
IVS_INT32 CControlBus::StartBusiness(IPI_START_BUSINESS_REQ* pParaInfo)
{
    //���Ͻӿ���
    //CUMW::CLock objLock(m_objInterfaceLock);

    //����������
    //1��pParaInfoָ��ǿ�
    if (NULL == pParaInfo)
    {
        IVS_RUN_LOG_ERR( "Start Decoder Failed. Parameter Invalidate. Parameter is Empty.");
        return IVS_PARA_INVALID;
    }

    //2��m_szDecoderID����Ϊ���ַ���
    if (0 == pParaInfo->szDecoderID[0])
    {
        IVS_RUN_LOG_ERR( "[ AysncSeq = %lu ] Start Business Failed. Decoder ID Invalidate.", pParaInfo->ulAsyncSeq);
        return IVS_PARA_INVALID;
    }

    //3��Э��
    if ((RDA_PROTOCOL_NSS_OVER_UDP > pParaInfo->stMediaNetInfo.ucTransportProtocol)
        || (RDA_PROTOCOL_RTP_OVER_TCP < pParaInfo->stMediaNetInfo.ucTransportProtocol))
    {
        IVS_RUN_LOG_ERR( "[ AysncSeq = %lu ] Start Business Failed. Parameter Invalidate. Protocol is error.", pParaInfo->ulAsyncSeq);
        return IVS_PARA_INVALID;
    }

    //����ҵ����󣬹��캯�������ʼ��
    CBusiness* pBusiness = VOS_NEW(pBusiness);
    if (NULL == pBusiness)
    {
        IVS_RUN_LOG_ERR( "[ AysncSeq = %lu ] Create Business Failed On Start Business.", pParaInfo->ulAsyncSeq);
        return IVS_ALLOC_MEMORY_ERROR;
    }

    pBusiness->m_ulAsyncSeq = pParaInfo->ulAsyncSeq;
    pBusiness->m_ulChannelId = pParaInfo->usChannelNo;
    (void)pBusiness->SetPlayType(CUMW_PLAY_TYPE_REMOTE_DIRECT_CONNECT);
    if(!CToolsHelp::Strncpy(pBusiness->m_szDecoderId, sizeof(pBusiness->m_szDecoderId), pParaInfo->szDecoderID, sizeof(pParaInfo->szDecoderID) - 1))
	{
		IVS_RUN_LOG_ERR("Copy DecoderId error.");
        VOS_DELETE(pBusiness);
		return IVS_ALLOC_MEMORY_ERROR;
	}

    if (RDA_PROTOCOL_RTP_OVER_TCP == pParaInfo->stMediaNetInfo.ucTransportProtocol)
    {
        (void)pBusiness->SetProtocolType(RDA_PROTOCOL_RTP_OVER_TCP);
        pBusiness->m_stServerPortInfo.usInterleavedPort = pParaInfo->stMediaNetInfo.usTcpInterleavedPort;
    }
    else
    {
        (void)pBusiness->SetProtocolType(RDA_PROTOCOL_RTP_OVER_UDP);
        pBusiness->m_stServerPortInfo.usVideoRtpPort  = pParaInfo->stMediaNetInfo.usUdpVideoRtpPort;
        pBusiness->m_stServerPortInfo.usVideoRtcpPort = pParaInfo->stMediaNetInfo.usUdpVideoRtcpPort;
        pBusiness->m_stServerPortInfo.usAudioRtpPort  = pParaInfo->stMediaNetInfo.usUdpAudioRtpPort;
        pBusiness->m_stServerPortInfo.usAudioRtcpPort = pParaInfo->stMediaNetInfo.usUdpAudioRtcpPort;
    }

    pBusiness->m_enVideoPt = (CUMW::PAY_LOAD_TYPE)pParaInfo->ucVideoPayloadType;
    pBusiness->m_enAudioPt = (CUMW::PAY_LOAD_TYPE)pParaInfo->ucAudioPayloadType;
    if(!CToolsHelp::Strncpy(pBusiness->m_szServerIp, sizeof(pBusiness->m_szServerIp), pParaInfo->stMediaNetInfo.szMediaIP, sizeof(pBusiness->m_szServerIp) - 1))
	{
		IVS_RUN_LOG_ERR("Copy ServerIp error.");
        VOS_DELETE(pBusiness);
		return IVS_ALLOC_MEMORY_ERROR;
	}
    if(!CToolsHelp::Strncpy(pBusiness->m_szSessionId, sizeof(pBusiness->m_szSessionId), pParaInfo->szSessionId, sizeof(pBusiness->m_szSessionId) - 1))
	{
		IVS_RUN_LOG_ERR("Copy SessionId error.");
        VOS_DELETE(pBusiness);
		return IVS_ALLOC_MEMORY_ERROR;
	}
    if(!CToolsHelp::Strncpy(pBusiness->m_szSessionUrl, sizeof(pBusiness->m_szSessionUrl), pParaInfo->szSessionUrl, sizeof(pBusiness->m_szSessionUrl) - 1))
	{
		IVS_RUN_LOG_ERR("Copy SessionUrl error.");
        VOS_DELETE(pBusiness);
		return IVS_ALLOC_MEMORY_ERROR;
	}

    //����BusinessId
    const unsigned long ulBusinessId = pParaInfo->ulBusinessID;
    pBusiness->SetBusinessId(ulBusinessId);

    //��ӿ�ʼҵ���¼�
    long lResult = AddBuesinessEvent(pParaInfo->ulAsyncSeq, CUMW::ASYNC_TYPE_DECORDER_START, 0, pBusiness);
    if (IVS_SUCCEED != lResult)
    {
        IVS_RUN_LOG_ERR( "[ AysncSeq = %lu ] Add Business Event Failed On Start Business.",
                pParaInfo->ulAsyncSeq);

        //�ͷ��ڴ�
        VOS_DELETE(pBusiness); //modify by liangyun at 2012.2.14.19:28

        return lResult;
    }

    IVS_DBG_LOG("[ AysncSeq = %lu ] Start Business Success.", pParaInfo->ulAsyncSeq);
    
    return IVS_SUCCEED;
}//lint !e818

/**************************************************************************
* name       : HandleEventStartDecoder
* description: ����������������Ϣ
* input      : objBusiness     ҵ���������
* output     : NA
* return     : long ������
* remark     : NA
**************************************************************************/
IVS_INT32 CControlBus::HandleEventStartBusiness(BUSINESS_ASYNC_INFO& stAsyncInfo)
{
    CBusiness* pCurBusiness = (CBusiness*)stAsyncInfo.pParameter;

    if (NULL == m_pStreamMgr)
    {
        IVS_RUN_LOG_ERR( "Start Business In Control Bus Failed. Stream Manager is Empty.");

        //�ϱ��첽����֪ͨ
        (void)m_objStatusNotifyCallBack.NotifyAsynRetNoInfo(stAsyncInfo.ulAsyncSeq, IVS_FAIL);

        //�ͷ��ڴ�
        VOS_DELETE(pCurBusiness); //modify by liangyun at 2012.2.14.19:28

        return IVS_TVWALL_MEMORY_NULL;
    }

    //��ʼҵ��
    long lResult = m_pStreamMgr->StartBusiness(*pCurBusiness);
    if (IVS_SUCCEED != lResult)
    {
        IVS_RUN_LOG_ERR( "[ BusinessID = %lu ] Start Business Failed On Handle Start Business Event.",
                pCurBusiness->GetBusinessId());

        //�ͷ��ڴ�
        VOS_DELETE(pCurBusiness); //modify by liangyun at 2012.2.14.19:28

        return lResult;
    }

    IVS_DBG_LOG("[ AsyncSeq = %lu ][ BusinessID = %lu ] Start Business In Control Bus Success.",
            pCurBusiness->m_ulAsyncSeq, pCurBusiness->GetBusinessId());

    //�ͷ��ڴ�
    VOS_DELETE(pCurBusiness); //modify by liangyun at 2012.2.14.19:28

    return IVS_SUCCEED;
}

/**************************************************************************
* name       : StopBusiness
* description: ֹͣ����
* input      : objBusiness     ҵ���������
* output     : NA
* return     : long ������
* remark     : NA
**************************************************************************/
IVS_INT32 CControlBus::StopBusiness(IPI_STOP_BUSINESS_REQ* pParaInfo)
{
    //���Ͻӿ���
    //CUMW::CLock objLock(m_objInterfaceLock);

    //����������
    //1��pParaInfoָ��ǿ�
    if (NULL == pParaInfo)
    {
        IVS_RUN_LOG_ERR( "Parameter Invalidate. Parameter is Empty.");
        return IVS_PARA_INVALID;
    }

    //2��m_szDecoderID����Ϊ���ַ���
    if (0 == pParaInfo->szDecoderID[0])
    {
        IVS_RUN_LOG_ERR( "[ AysncSeq = %lu ] Parameter Invalidate. Decoder ID is Empty.", pParaInfo->ulAsyncSeq);
        return IVS_PARA_INVALID;
    }

    //����ҵ����󣬹��캯�������ʼ��
    CBusiness* pBusiness = VOS_NEW(pBusiness);
    if (NULL == pBusiness)
    {
        IVS_RUN_LOG_ERR( "[ AysncSeq = %lu ] Create Business Failed.", pParaInfo->ulAsyncSeq);
        return IVS_ALLOC_MEMORY_ERROR;
    }

    pBusiness->m_ulAsyncSeq = pParaInfo->ulAsyncSeq;
    pBusiness->m_ulChannelId = pParaInfo->usChannelNo;
    (void)pBusiness->SetPlayType(CUMW_PLAY_TYPE_REMOTE_DIRECT_CONNECT);
    if(!CToolsHelp::Strncpy(pBusiness->m_szDecoderId, sizeof(pBusiness->m_szDecoderId), pParaInfo->szDecoderID, sizeof(pParaInfo->szDecoderID) - 1))
	{
		IVS_RUN_LOG_ERR("Copy DecoderId error.");
        VOS_DELETE(pBusiness);
		return IVS_ALLOC_MEMORY_ERROR;
	}
    //����BusinessId
    const unsigned long ulBusinessId = pParaInfo->ulBusinessID;
    pBusiness->SetBusinessId(ulBusinessId);

    //���ֹͣҵ���¼�
    long lResult = AddBuesinessEvent(pParaInfo->ulAsyncSeq, CUMW::ASYNC_TYPE_DECORDER_STOP, ulBusinessId, pBusiness);
    if (IVS_SUCCEED != lResult)
    {
        IVS_RUN_LOG_ERR( "[ AysncSeq = %lu ][ BusinessID = %lu ] Add Business Event Failed.",
                pParaInfo->ulAsyncSeq, ulBusinessId);

        //�ͷ��ڴ� 
        VOS_DELETE(pBusiness); //modify by liangyun at 2012.2.14.19:28

        return lResult;
    }

    IVS_DBG_LOG("[ AysncSeq = %lu ][ BusinessID = %lu ] Stop Business Success.", pParaInfo->ulAsyncSeq, ulBusinessId);
    return IVS_SUCCEED;
}//lint !e818

/**************************************************************************
* name       : HandleEventStopBusiness
* description: ����ֹͣ�������Ϣ
* input      : objBusiness     ҵ���������
* output     : NA
* return     : long ������
* remark     : NA
**************************************************************************/
IVS_INT32 CControlBus::HandleEventStopBusiness(BUSINESS_ASYNC_INFO& stAsyncInfo)
{
    long lResult = IVS_SUCCEED;

    if (NULL == m_pStreamMgr)
    {
        IVS_RUN_LOG_ERR( "Stop Business In Control Bus Failed. Stream Manager is Empty.");

        //�ͷ��ڴ�
        VOS_DELETE((CBusiness*&)stAsyncInfo.pParameter); //modify by liangyun at 2012.2.14.19:28

        return IVS_TVWALL_MEMORY_NULL;
    }

    CBusiness* pBusiness = (CBusiness*)stAsyncInfo.pParameter;
    const unsigned long ulBusinessId = pBusiness->GetBusinessId();

    IVS_DBG_LOG("[ BusinessID = %lu ] Stop Business In Control Bus Begin.", ulBusinessId);

    //��ʼ��ҵ��
    lResult = m_pStreamMgr->StopBusiness(*pBusiness);
    if (IVS_SUCCEED != lResult)
    {
        IVS_RUN_LOG_ERR(
                "[ AsyncSeq = %lu ][ BusinessID = %lu ] Stop Business Failed In Control Bus.",
                pBusiness->m_ulAsyncSeq, ulBusinessId);

        //�ͷ��ڴ�
        VOS_DELETE(pBusiness); //modify by liangyun at 2012.2.14.19:28

        return lResult;
    }

    //�ͷ��ڴ�
    VOS_DELETE(pBusiness); //modify by liangyun at 2012.2.14.19:28

    return lResult;
}

/**************************************************************************
* name       : PlayAudio
* description: ����/ֹͣ��������
* input      : pParaInfo ��ʼ������
* output     : void
* return     : RET_OK-�ɹ�     RET_FAIL-ʧ��
* remark     : NA
**************************************************************************/
IVS_INT32 CControlBus::PlayAudio(IPI_AUDIO_CONTROL_REQ *pParaInfo)
{
    /*
    //���Ͻӿ���
    //CUMW::CLock objLock(m_objInterfaceLock);

    //����������
    //1��pParaInfoָ��ǿ�
    if (NULL == pParaInfo)
    {
        IVS_RUN_LOG_ERR( "Parameter Invalidate. Parameter is Empty.");
        return IVS_PARA_INVALID;
    }

    //2��m_szDecoderID����Ϊ���ַ���
    if (0 == pParaInfo->szDecoderID[0])
    {
        IVS_RUN_LOG_ERR( "[ AysncSeq = %lu ] Parameter Invalidate. Camera ID is Empty.", pParaInfo->ulAsyncSeq);
        return IVS_PARA_INVALID;
    }

    //����ҵ����󣬹��캯�������ʼ��
    CBusiness* pBusiness = VOS_NEW(pBusiness);
    if (NULL == pBusiness)
    {
        IVS_RUN_LOG_ERR( "[ AysncSeq = %lu ] Create Business Failed.", pParaInfo->ulAsyncSeq);
        return IVS_ALLOC_MEMORY_ERROR;
    }

    pBusiness->m_ulAsyncSeq = pParaInfo->ulAsyncSeq;
    pBusiness->m_ulChannelId = pParaInfo->usChannelNo;
    (void)pBusiness->SetPlayType(CUMW_PLAY_TYPE_REMOTE_DIRECT_CONNECT);
    if(!CToolsHelp::Strncpy(pBusiness->m_szDecoderId, sizeof(pBusiness->m_szDecoderId), (const char *)pParaInfo->szDecoderID, sizeof(pParaInfo->szDecoderID) - 1))
	{
		IVS_RUN_LOG_ERR("Copy DecoderId error.");
        VOS_DELETE(pBusiness);
		return IVS_ALLOC_MEMORY_ERROR;
	}

    //����BusinessId
    const unsigned long ulBusinessId = pParaInfo->ulBusinessID;
    pBusiness->SetBusinessId(ulBusinessId);

    //���ÿ�����Ϣ
    pBusiness->m_cAudioCmd = pParaInfo->cAudioCmd;

    //���ֹͣҵ���¼�
    long lResult = AddBuesinessEvent(pParaInfo->ulAsyncSeq, CUMW::ASYNC_TYPE_AUDIO_CONTROL, ulBusinessId, pBusiness);
    if (IVS_SUCCEED != lResult)
    {
        IVS_RUN_LOG_ERR( "[ AysncSeq = %lu ][ BusinessID = %lu ] Add Business Event Failed.",
                pParaInfo->ulAsyncSeq, ulBusinessId);

        //�ͷ��ڴ�
        VOS_DELETE(pBusiness); //modify by liangyun at 2012.2.14.19:28

        return lResult;
    }

    IVS_DBG_LOG("[ AysncSeq = %lu ][ BusinessID = %lu ] Play Audio Success.", pParaInfo->ulAsyncSeq, ulBusinessId);
    return IVS_SUCCEED;
    */
    return IVS_SYS_NO_SUPPORT;
}//lint !e818 !e1762

/**************************************************************************
* name       : PlayAudio
* description: ������/ֹͣ����������Ϣ
* input      : pParaInfo ��ʼ������
* output     : void
* return     : RET_OK-�ɹ�     RET_FAIL-ʧ��
* remark     : NA
**************************************************************************/
IVS_INT32 CControlBus::HandleEventPlayAudio(BUSINESS_ASYNC_INFO& stAsyncInfo)//lint !e830
{
    /*
    long lResult = IVS_SUCCEED;

    if (NULL == m_pStreamMgr)
    {
        IVS_RUN_LOG_ERR( "Play Audio In Control Bus Failed. Stream Manager is Empty.");

        //�ͷ��ڴ�
        VOS_DELETE((CBusiness*&)stAsyncInfo.pParameter); //modify by liangyun at 2012.2.14.19:28

        return IVS_TVWALL_MEMORY_NULL;
    }

    CBusiness* pBusiness = (CBusiness*)stAsyncInfo.pParameter;

    unsigned long ulCtrlParaLen = sizeof(CUMW::BUSINESS_CTRL_PARA) + sizeof(IPI_CTRL_LOCAL_AUDIO) - 1;
    CUMW::BUSINESS_CTRL_PARA* pCtrlPara = (CUMW::BUSINESS_CTRL_PARA*)VOS_NEW((char* &)pCtrlPara, ulCtrlParaLen); //lint !e826 �ɱ�����
    if (NULL == pCtrlPara)
    {
        IVS_RUN_LOG_ERR(
                "[ AysncSeq = %lu ][ BusinessID = %lu ] Create Business Control Param Failed On Add Business Ctrl.",
                stAsyncInfo.ulAsyncSeq, stAsyncInfo.ulBusinessId);

        //�ͷ��ڴ�
        VOS_DELETE(pBusiness); //modify by liangyun at 2012.2.14.19:28

        return IVS_ALLOC_MEMORY_ERROR;
    }

    memset((char*) pCtrlPara, 0, ulCtrlParaLen);

    //���ÿ��Ʋ���
    pCtrlPara->enCtrlCode = BUSINESS_CTRL_CODE_E_LOCAL_AUDIO;
    pCtrlPara->ulCtrlParaLen = sizeof(IPI_CTRL_LOCAL_AUDIO);

    //���ÿ��Ʋ���
    IPI_CTRL_LOCAL_AUDIO *pParaInfo = VOS_NEW(pParaInfo);
    if (NULL == pParaInfo)
    {
        IVS_RUN_LOG_ERR( "Set Control Param Failed ");

        //�ͷ��ڴ�
        VOS_DELETE(pBusiness); //modify by liangyun at 2012.2.14.19:28

        return IVS_ALLOC_MEMORY_ERROR;
    }

    pParaInfo->ulAsyncSeq   = stAsyncInfo.ulAsyncSeq;
    pParaInfo->ulAudioType  = CUMW_AUDIO_TYPE_OUT;
    pParaInfo->ulCtrlCode = pBusiness->m_cAudioCmd;
    pParaInfo->ulVolume = 0;

    if(!CToolsHelp::Memcpy(pCtrlPara->vCtrlPara, sizeof(IPI_CTRL_LOCAL_AUDIO), pParaInfo, sizeof(IPI_CTRL_LOCAL_AUDIO)))    //lint !e419 �ɱ���
	{
		IVS_RUN_LOG_ERR("Play Audio In Control Bus Copy CtrlPara failed");
        VOS_DELETE(pBusiness);
    VOS_DELETE((IPI_CTRL_LOCAL_AUDIO*&)pParaInfo);

		return IVS_ALLOC_MEMORY_ERROR;
	}
    const unsigned long ulBusinessId = pBusiness->GetBusinessId();

    IVS_DBG_LOG("[ BusinessID = %lu ] Play Audio In Control Bus Begin.", ulBusinessId);

    //��ʼ��ҵ��
    lResult = m_pStreamMgr->ControlBusiness(*pBusiness, *pCtrlPara);
    if (IVS_SUCCEED != lResult)
    {
        IVS_RUN_LOG_ERR( "[ AsyncSeq = %lu ][ BusinessID = %lu ] Play Audio Failed On Start Business In Control Bus.",
                pBusiness->m_ulAsyncSeq, ulBusinessId);
    }

    //�ͷ��ڴ�
    VOS_DELETE((char*&)pCtrlPara);
    VOS_DELETE((IPI_CTRL_LOCAL_AUDIO*&)pParaInfo);
    VOS_DELETE((CBusiness*&)stAsyncInfo.pParameter);

    return lResult;
    */
    return IVS_SYS_NO_SUPPORT;
}//lint !e1762 !e1764

/**************************************************************************
* name       : SetVolume
* description: ��������
* input      : pParaInfo ��ʼ������
* output     : void
* return     : RET_OK-�ɹ�     RET_FAIL-ʧ��
* remark     : NA
**************************************************************************/
IVS_INT32 CControlBus::SetVolume(IPI_TUNEUP_VOLUME_REQ *pParaInfo)
{
    /*
    //���Ͻӿ���
    //CUMW::CLock objLock(m_objInterfaceLock);

    //����������
    //1��pParaInfoָ��ǿ�
    if (NULL == pParaInfo)
    {
        IVS_RUN_LOG_ERR( "Parameter Invalidate. Parameter is Empty.");
        return IVS_PARA_INVALID;
    }

    //2��m_szDecoderID����Ϊ���ַ���
    if (0 == pParaInfo->szDecoderID[0])
    {
        IVS_RUN_LOG_ERR( "[ AysncSeq = %lu ] Parameter Invalidate. Camera ID is Empty.", pParaInfo->ulAsyncSeq);
        return IVS_PARA_INVALID;
    }

    //����ҵ����󣬹��캯�������ʼ��
    CBusiness* pBusiness = VOS_NEW(pBusiness);
    if (NULL == pBusiness)
    {
        IVS_RUN_LOG_ERR( "[ AysncSeq = %lu ] Create Business Failed.", pParaInfo->ulAsyncSeq);
        return IVS_ALLOC_MEMORY_ERROR;
    }

    pBusiness->m_ulAsyncSeq = pParaInfo->ulAsyncSeq;
    pBusiness->m_ulChannelId = pParaInfo->usChannelNo;
    (void)pBusiness->SetPlayType(CUMW_PLAY_TYPE_REMOTE_DIRECT_CONNECT);
    if(!CToolsHelp::Strncpy(pBusiness->m_szDecoderId, sizeof(pBusiness->m_szDecoderId), (const char *)pParaInfo->szDecoderID, sizeof(pParaInfo->szDecoderID) - 1))
	{
		IVS_RUN_LOG_ERR("Copy DecoderId error.");
        VOS_DELETE(pBusiness);
		return IVS_ALLOC_MEMORY_ERROR;
	}

    //����BusinessId
    const unsigned long ulBusinessId = pParaInfo->ulBusinessID;
    pBusiness->SetBusinessId(ulBusinessId);

    //���ÿ�����Ϣ����������
    pBusiness->m_cAudioCmd = CUMW_AUDIO_CTRL_CODE_SET_VOLUME;
    pBusiness->m_cVolume = pParaInfo->cVolume;

    //���ֹͣҵ���¼�
    long lResult = AddBuesinessEvent(pParaInfo->ulAsyncSeq, CUMW::ASYNC_TYPE_TUNEUP_VOLUME, ulBusinessId, pBusiness);
    if (IVS_SUCCEED != lResult)
    {
        IVS_RUN_LOG_ERR( "[ AysncSeq = %lu ][ BusinessID = %lu ] Add Business Event Failed.",
                pParaInfo->ulAsyncSeq, ulBusinessId);

        //�ͷ��ڴ�
        VOS_DELETE(pBusiness); //modify by liangyun at 2012.2.14.19:28

        return lResult;
    }

    IVS_DBG_LOG("[ AysncSeq = %lu ][ BusinessID = %lu ] Ini Decoder Success.", pParaInfo->ulAsyncSeq, ulBusinessId);
    return IVS_SUCCEED;
    */
    return IVS_SYS_NO_SUPPORT;
}//lint !e818 !e1762

/**************************************************************************
* name       : SetVolume
* description: ��������������Ϣ
* input      : pParaInfo ��ʼ������
* output     : void
* return     : RET_OK-�ɹ�     RET_FAIL-ʧ��
* remark     : NA
**************************************************************************/
IVS_INT32 CControlBus::HandleEventSetVolume(BUSINESS_ASYNC_INFO& stAsyncInfo)//lint !e830
{
    /*
    long lResult = IVS_SUCCEED;

    if (NULL == m_pStreamMgr)
    {
        IVS_RUN_LOG_ERR( "Set Volume In Control Bus Failed. Stream Manager is Empty.");

        //�ͷ��ڴ�
        VOS_DELETE((CBusiness*&)stAsyncInfo.pParameter); //modify by liangyun at 2012.2.14.19:28

        return IVS_TVWALL_MEMORY_NULL;
    }

    CBusiness* pBusiness = (CBusiness*)stAsyncInfo.pParameter;

    unsigned long ulCtrlParaLen = sizeof(CUMW::BUSINESS_CTRL_PARA) + sizeof(IPI_CTRL_LOCAL_AUDIO) - 1;
    CUMW::BUSINESS_CTRL_PARA* pCtrlPara = (CUMW::BUSINESS_CTRL_PARA*)VOS_NEW((char* &)pCtrlPara, ulCtrlParaLen); //lint !e826 �ɱ�����
    if (NULL == pCtrlPara)
    {
        IVS_RUN_LOG_ERR(
                "[ AysncSeq = %lu ][ BusinessID = %lu ] Create Business Control Param Failed On Add Business Ctrl.",
                stAsyncInfo.ulAsyncSeq, stAsyncInfo.ulBusinessId);

        //�ͷ��ڴ�
        VOS_DELETE(pBusiness); //modify by liangyun at 2012.2.14.19:28

        return IVS_ALLOC_MEMORY_ERROR;
    }

    memset((char*) pCtrlPara, 0, ulCtrlParaLen);

    //���ÿ��Ʋ���
    pCtrlPara->enCtrlCode = BUSINESS_CTRL_CODE_E_LOCAL_AUDIO;
    pCtrlPara->ulCtrlParaLen = sizeof(IPI_CTRL_LOCAL_AUDIO);

    //���ÿ��Ʋ���
    IPI_CTRL_LOCAL_AUDIO *pParaInfo = VOS_NEW(pParaInfo);
    if (NULL == pParaInfo)
    {
        IVS_RUN_LOG_ERR( "Set Control Param Failed ");

        //�ͷ��ڴ�
        VOS_DELETE(pBusiness); //modify by liangyun at 2012.2.14.19:28

        return IVS_ALLOC_MEMORY_ERROR;
    }

    pParaInfo->ulAsyncSeq   = stAsyncInfo.ulAsyncSeq;
    pParaInfo->ulAudioType  = CUMW_AUDIO_TYPE_OUT;
    pParaInfo->ulCtrlCode = pBusiness->m_cAudioCmd;
    pParaInfo->ulVolume = pBusiness->m_cVolume;

    if(!CToolsHelp::Memcpy(pCtrlPara->vCtrlPara, sizeof(IPI_CTRL_LOCAL_AUDIO), pParaInfo, sizeof(IPI_CTRL_LOCAL_AUDIO)))    //lint !e419 �ɱ�����
	{
		IVS_RUN_LOG_ERR("Set Volume In Control Bus Copy CtrlPara failed");
        VOS_DELETE(pBusiness);
    VOS_DELETE((IPI_CTRL_LOCAL_AUDIO*&)pParaInfo);

		return IVS_ALLOC_MEMORY_ERROR;
	}
    const unsigned long ulBusinessId = pBusiness->GetBusinessId();

    IVS_DBG_LOG("[ BusinessID = %lu ] Play Audio In Control Bus Begin.", ulBusinessId);

    //��ʼ��ҵ��
    lResult = m_pStreamMgr->ControlBusiness(*pBusiness, *pCtrlPara);
    if (IVS_SUCCEED != lResult)
    {
        IVS_RUN_LOG_ERR( "[ AsyncSeq = %lu ][ BusinessID = %lu ] Set Volume Failed On Start Business In Control Bus.",
                pBusiness->m_ulAsyncSeq, ulBusinessId);
    }

    //�ͷ��ڴ�
    VOS_DELETE((char*&)pCtrlPara);
    VOS_DELETE((IPI_CTRL_LOCAL_AUDIO*&)pParaInfo);
    VOS_DELETE((CBusiness*&)stAsyncInfo.pParameter);

    return lResult;
    */
    return IVS_SYS_NO_SUPPORT;
}//lint !e1762 !e1764
