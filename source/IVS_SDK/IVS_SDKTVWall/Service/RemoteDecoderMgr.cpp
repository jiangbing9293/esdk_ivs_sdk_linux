#include "StdAfx.h"
#include "LockGuard.h"
#include "RemoteDecoderMgr.h"
#include "ToolsHelp.h"

//Ϊ�˼�СȦ���Ӷ�
#define CHECK_STRNCPY_RET(bSucc, errCode, strLog)\
{\
	if(!bSucc)\
	{\
		IVS_RUN_LOG_ERR(strLog);\
		return errCode;\
	}\
}

CRDAPacket::CRDAPacket():m_ulRecvBuffLen(0)
	,m_pRecvBuff(NULL)
	,m_ulSendBuffLen(0)
	,m_pSendBuff (NULL)
	,m_usPacketNo(0)	
	,m_pReadEvent(NULL)

{

}

//BEGIN V100R001C05 MODIFY 2010-01-19 zhaoyuzhen z00137994 for ���������ظ���
CRDAPacket::~CRDAPacket()
{
    VOS_DELETE(m_pReadEvent);/*lint !e1551��ȷ�ϲ�������쳣��VOS_DELETE����try-catch�����쳣��*/
    VOS_DELETE(m_pRecvBuff, MULTI);/*lint !e1551��ȷ�ϲ�������쳣��VOS_DELETE����try-catch�����쳣��*/
    VOS_DELETE(m_pSendBuff, MULTI);/*lint !e1551��ȷ�ϲ�������쳣��VOS_DELETE����try-catch�����쳣��*/
    (void)VOS_DestroyEvent(m_pReadEvent);
    m_pRecvBuff  = NULL;
    m_pSendBuff  = NULL;
    m_pReadEvent = NULL;
}

//END V100R001C05 MODIFY 2010-01-19 zhaoyuzhen z00137994 for ���������ظ���

IVS_INT32 CRDAPacket::InitRDAPacket()
{
    IVS_DBG_LOG("InitRDAPacket Start.");

    (void)VOS_NEW(m_pRecvBuff, MAX_RDAMSG_LEN);
    if (NULL == m_pRecvBuff)
    {
        IVS_RUN_LOG_ERR( "InitRDAPacket End. New m_pRecvBuff fail.");

        return VOS_FAIL;
    }

    (void)VOS_NEW(m_pSendBuff, MAX_RDAMSG_LEN);
    if (NULL == m_pSendBuff)
    {
        IVS_RUN_LOG_ERR( "InitRDAPacket End. New m_pSendBuff fail.");

        VOS_DELETE(m_pRecvBuff, MULTI);
        return VOS_FAIL;
    }

    (void)VOS_NEW(m_pReadEvent);
    if (NULL == m_pReadEvent)
    {
        IVS_RUN_LOG_ERR( "InitRDAPacket End. New m_pReadEvent fail.");

        VOS_DELETE(m_pRecvBuff, MULTI);
        VOS_DELETE(m_pSendBuff, MULTI);
        return VOS_FAIL;
    }

    LONG lRet = VOS_CreateEvent(m_pReadEvent);
    if (VOS_OK != lRet)
    {
        IVS_RUN_LOG_ERR( "InitRDAPacket End. CreateEvent fail ret= %ld.", lRet);

        VOS_DELETE(m_pReadEvent);
        VOS_DELETE(m_pRecvBuff, MULTI);
        VOS_DELETE(m_pSendBuff, MULTI);
        return VOS_FAIL;
    }

    IVS_DBG_LOG("InitRDAPacket End.");
    return VOS_SUCCESS;
};

IVS_INT32 CRDAPacket::WaitReadPacketEv()
{
    IVS_DBG_LOG("WaitReadPacketEv Start."
            "ReadEvent(0x%x) RDAPacket(0x%x) PacketNo(%d).",
            m_pReadEvent, this, this->m_usPacketNo);

    LONG lResult = VOS_WaitEvent(m_pReadEvent, WAITEVENT_TIMEOUT);
    if (VOS_OK != lResult)
    {
        IVS_RUN_LOG_ERR( "WaitReadPacketEv End. "
                "WaitEvent fail, ret = %ld.", lResult);
        return VOS_FAIL;
    }

    IVS_DBG_LOG("WaitReadPacketEv End. "
            "ReadEvent(0x%x) RDAPacket(0x%x) PacketNo(%d).",
            m_pReadEvent, this, this->m_usPacketNo);
    return VOS_SUCCESS;
};

IVS_INT32 CRDAPacket::SetReadPacketEv()
{
    IVS_DBG_LOG("SetReadPacketEv Start.");

    LONG lResult = VOS_SetEvent(m_pReadEvent);
    if (VOS_OK != lResult)
    {
        IVS_RUN_LOG_ERR( "SetReadPacketEv End. ret = %ld, "
                "ReadEvent(0x%x) RDAPacket(0x%x) PacketNo(%d).", lResult,
                m_pReadEvent, this, this->m_usPacketNo);
        return VOS_FAIL;
    }

    IVS_DBG_LOG("SetReadPacketEv End. ReadEvent(0x%x), RDAPacket(0x%x), "
            "PacketNo(%d).", m_pReadEvent, this, this->m_usPacketNo);
    return VOS_SUCCESS;
};

IVS_INT32 CRDAPacketMgr::InitRDAPacketMgr()
{
    IVS_DBG_LOG("InitRDAPacketMgr Start.");

    m_pMutexList = VOS_CreateMutex();
    if (NULL == m_pMutexList)
    {
        IVS_RUN_LOG_ERR( "InitRDAPacketMgr End. CreateMutex fail.");
        return VOS_FAIL;
    }

    //����RDAPacket����ѿռ�
    (void)VOS_NEW(m_pArrayRDAPacket, MAX_RDAPACKET_NUM);
    if (NULL == m_pArrayRDAPacket)
    {
        IVS_RUN_LOG_ERR( "InitRDAPacketMgr End. New m_pArrayRDAPacket fail.");

        (void)VOS_DestroyMutex(m_pMutexList);
        return VOS_FAIL;
    }

    //��ʼ��RDAPacket����
    LONG lRet = VOS_SUCCESS;
    CRDAPacket* pRDAPacket = m_pArrayRDAPacket;
    for (LONG i = 0; i < MAX_RDAPACKET_NUM; i++)
    {
        lRet = pRDAPacket->InitRDAPacket();
        if (VOS_SUCCESS != lRet)
        {
            IVS_RUN_LOG_ERR( "InitRDAPacketMgr End. InitRDAPacket fail.");

            VOS_DELETE(m_pArrayRDAPacket, MULTI);
            (void)VOS_DestroyMutex(m_pMutexList);
            return VOS_FAIL;
        }

        m_listFreeRDAPacket.push_back(pRDAPacket);
        pRDAPacket++; //++��ƫ�� һ�� CRDAPacket
    }

    IVS_DBG_LOG("InitRDAPacketMgr End. Init CRDAPacketMgr success.");

    return VOS_SUCCESS;
};

CRDAPacket* CRDAPacketMgr::GetRDAPacket()
{
    IVS_DBG_LOG("Get RDA Packet Begin.");

    if ((NULL == m_pMutexList) || (NULL == m_pArrayRDAPacket))
    {
        IVS_RUN_LOG_ERR( "Get RDA Packet Failed. Not Init yet.");
        return NULL;
    }

    CRDAPacket* pRDAPacket = NULL;

    //�Ӷ����л�ȡָ��RDAPacket��ָ��
    (void)VOS_MutexLock(m_pMutexList);
    const unsigned long ulPacketNum = m_listFreeRDAPacket.size();
    if (0 == ulPacketNum)
    {
        (void)VOS_MutexUnlock(m_pMutexList);

        IVS_RUN_LOG_ERR("Get RDA Packet End. No Idle RDA Packet.");
        return NULL;
    }

    pRDAPacket = *(m_listFreeRDAPacket.begin()); //lint !e48

    //�ӿ��ж���ͷ��ɾ��
    m_listFreeRDAPacket.pop_front();

    //���빤������β
    m_listWorkRDAPacket.push_back(pRDAPacket);

    (void)VOS_MutexUnlock(m_pMutexList);

    IVS_DBG_LOG("Get RDA Packet Success. Remain %lu Idle Packets.", ulPacketNum - 1);

    return pRDAPacket;
};

void CRDAPacketMgr::ReleaseRDAPacket( CRDAPacket* pRDAPacket )
{
    IVS_DBG_LOG("ReleaseRDAPacket Start.");

    if ((NULL == m_pMutexList)
       || (NULL == m_pArrayRDAPacket) || (NULL == pRDAPacket))
    {
        IVS_RUN_LOG_ERR( "ReleaseRDAPacket End. Param is Null");
        return;
    }

    (void)VOS_MutexLock(m_pMutexList);

    //�ͷŹ������еİ�
    VOS_BOOLEAN bFind = VOS_FALSE;
    CRDAPacket* pTempRDAPacket = NULL;
    ListRDAPacketIte itListRDAPacket;
    for (itListRDAPacket = m_listWorkRDAPacket.begin();
         itListRDAPacket != m_listWorkRDAPacket.end(); ++itListRDAPacket) //lint !e53 !e81
    {
        pTempRDAPacket = *itListRDAPacket; //lint !e48
        if (pTempRDAPacket->m_usPacketNo == pRDAPacket->m_usPacketNo)
        {
            (void)m_listWorkRDAPacket.erase(itListRDAPacket); //lint !e1025
            bFind = VOS_TRUE;
            break;
        }
    }

    //����ڹ���������û�з���,��ֱ���˳�
    if (VOS_FALSE == bFind)
    {
        IVS_RUN_LOG_ERR( "ReleaseRDAPacket End. release "
                "RDAPacket(0x%x, PacketNo %d) fail. because not exist in work list",
                pRDAPacket, pRDAPacket->m_usPacketNo);
        (void)VOS_MutexUnlock(m_pMutexList);
        return;
    }

    //��ӵ����ж���
    m_listFreeRDAPacket.push_back(pRDAPacket);

    (void)VOS_MutexUnlock(m_pMutexList);

    IVS_DBG_LOG("ReleaseRDAPacket End. release RDAPacket(0x%x, PacketNo %d) success.",
            pRDAPacket, pRDAPacket->m_usPacketNo);

    return;
};

CRDAPacket* CRDAPacketMgr::FindRDAPacket( IVS_USHORT usPacketNo )
{
    IVS_DBG_LOG("FindRDAPacket Start.");

    if ((NULL == m_pMutexList) || (NULL == m_pArrayRDAPacket))
    {
        IVS_RUN_LOG_ERR( "FindRDAPacket End. param is NULL.");
        return NULL;
    }

    CRDAPacket* pRDAPacket = NULL;

    //���Ҷ���
    VOS_BOOLEAN bFind = VOS_FALSE;
    (void)VOS_MutexLock(m_pMutexList);
    ListRDAPacketIte itListRDAPacket;
    for (itListRDAPacket = m_listWorkRDAPacket.begin();
         itListRDAPacket != m_listWorkRDAPacket.end(); ++itListRDAPacket) //lint !e53 !e81
    {
        if (usPacketNo == (*itListRDAPacket)->m_usPacketNo) //lint !e40 !e48
        {
            pRDAPacket = (*itListRDAPacket); //lint !e48
            bFind = VOS_TRUE;
            break;
        }
    }

    (void)VOS_MutexUnlock(m_pMutexList);

    if (VOS_FALSE == bFind)
    {
        IVS_RUN_LOG_ERR( "FindRDAPacket End. "
                "find RDAPacket fail, return NULL, PacketNo is %d",
                usPacketNo);
        return NULL;
    }

    IVS_DBG_LOG("FindRDAPacket End. find RDAPacket success RDAPacket is 0x%x.", pRDAPacket);
    return pRDAPacket;
};

void CCheckConnTimeOTrigger::onTrigger(void* pArg, ULONGLONG ullScales,
                                       TriggerStyle style)
{
    style = style;
    ullScales = ullScales;

    CDecoderConnHandle* pDecoderConnHandle =
        (CDecoderConnHandle*)pArg;
    if (NULL == pDecoderConnHandle)
    {
        IVS_RUN_LOG_ERR("CheckConnTimeOTrigger pDecoderConnHandle is NULL");
        return;
    }

    //BEGIN R001C05LITS01 MOD 2010-06-11 ligengqiang 00124479 for ���15λIP��������
    //��IPת��Ϊ�ַ���
    char* pszIP = inet_ntoa((in_addr &)pDecoderConnHandle->m_peerAddr.m_lIpAddr);

    IVS_RUN_LOG_ERR("Remote Decoder(IP:%.15s) Connect Time Out.", pszIP);
    //END   R001C05LITS01 MOD 2010-06-11 ligengqiang 00124479 for ���15λIP��������

    //����ʱ
    pDecoderConnHandle->HandleTimeout();

    return;
};

CDecoderConnHandle::CDecoderConnHandle()
	:m_bNeedUse(VOS_FALSE)
	,m_pRemoteDecoder(NULL)
	,m_pEventQ(NULL)
	,m_ulReference(0)
	,m_pMutexDecoderConnHandle(NULL)
	,m_objTriggerConnTimeOut(CUMW::TRIGGER_TYPE_DECODER_CONN_TIME_OUT)
{
    //BEGIN R002C01SPC001 ADD 2011-08-16 ligengqiang 00124479 for ������֧�ֱ���˫����
    memset(m_szLocalIp, 0, sizeof(m_szLocalIp));
    //END   R002C01SPC001 ADD 2011-08-16 ligengqiang 00124479 for ������֧�ֱ���˫����
}

//BEGIN V100R001C05 MODIFY 2010-01-19 zhaoyuzhen z00137994 for ���������ظ���

CDecoderConnHandle::~CDecoderConnHandle()
{
    if (NULL != m_pMutexDecoderConnHandle)
    {
        (void)VOS_DestroyMutex(m_pMutexDecoderConnHandle);
    }

    m_pEventQ  = NULL;
    m_bNeedUse = VOS_FALSE;
    m_pMutexDecoderConnHandle = NULL;
    m_pRemoteDecoder = NULL;
}

//END V100R001C05 MODIFY 2010-01-19 zhaoyuzhen z00137994 for ���������ظ���

LONG CDecoderConnHandle::Init(CDecoderMgrEventQueue* pEventQ)
{
    if (NULL == pEventQ)
    {
        IVS_RUN_LOG_ERR( "DecoderConnHandle Init, param is NULL.");
        return VOS_FAIL;
    }

    m_pEventQ = pEventQ;

    //�������������������ü���
    m_pMutexDecoderConnHandle = VOS_CreateMutex();
    if (NULL == m_pMutexDecoderConnHandle)
    {
        IVS_RUN_LOG_ERR( "DecoderConnHandle Init, create mutex fail.");
        return VOS_FAIL;
    }

    IVS_DBG_LOG("DecoderConnHandle Init sucess.");

    return VOS_SUCCESS;
}

void CDecoderConnHandle::handle_recv(void)
{
    IVS_DBG_LOG("[ DecoderConnHandle = %#p ] Handel Receive New Message From(IP:%.15s, Port:%d).",
            this, inet_ntoa((in_addr &)m_peerAddr.m_lIpAddr), ntohs(m_peerAddr.m_usPort));

    if (NULL == m_pEventQ)
    {
        IVS_RUN_LOG_ERR( "[ DecoderConnHandle = %#p ] Handel Receive New Message Failed. Not Init yet.", this);
        return;
    }

    //�˴�new�������ڴ棬����Ϣ�����̣߳���������Ϣ���ͷ�
    CDecoderMgrEvent* pEventRecv = VOS_NEW(pEventRecv);
    if (NULL == pEventRecv)
    {
        IVS_RUN_LOG_ERR( "[ DecoderConnHandle = %#p ] Create Event Failed On Handle Receive New Message.", this);
        return;
    }

    pEventRecv->m_ulEventType = RDA_PACKETMGR_EVENT_RECV_RD_DATA;
    pEventRecv->m_pDecoderConnHandle = this;

    //���ܴ�ʱ��Ϣ�����߳�����ɾ����handle������ȵ���addRef������ɹ�����������
    //��Ϣ�����̲߳������ɾ����handle�������ʾҵ���߳��Ѿ�ɾ���˸�handle������
    //�Ѹ��¼������¼����С�
    long lResult = this->AddRef();
    if (VOS_SUCCESS != lResult)
    {
        IVS_RUN_LOG_ERR( "[ DecoderConnHandle = %#p ] Add Reference Failed On Handle Receive New Message.", this);
        VOS_DELETE(pEventRecv);
        return;
    }

    //�����¼�����
    lResult = m_pEventQ->pushBackEv(pEventRecv);
    if (VOS_OK != lResult)
    {
        IVS_RUN_LOG_ERR("[ DecoderConnHandle = %#p ] Add Event Failed On Handle Receive New Message.", this);
        VOS_DELETE(pEventRecv);
        return;
    }
};

void CDecoderConnHandle::SetTimeOut
(   IN unsigned long ulInterval)
{
    //ע����Ϣ����,ȡ����ʱ����
    CancelTimeOut();

    //��ö�ʱ��ʵ��
    CNVSTimer& objNVSTimer = CStatusSingleton::Instance().GetNVSTimer();

    //ע�ᶨʱ������
    (void)objNVSTimer.registerTimer(&m_objTriggerConnTimeOut, (void*)this, ulInterval * CUMW::TIMER_INTERVAL, enOneShot);

    //IVS_RUN_LOG_ERR("[ DecoderConnHandle = %#p ] Set Decoder Connection Handle Time Out Timer. Interval = %us", this,
    //        ulInterval);
};

void CDecoderConnHandle::CancelTimeOut(void)
{
    //��ö�ʱ��ʵ��
    CNVSTimer& objNVSTimer = CStatusSingleton::Instance().GetNVSTimer();

    //ע����ʱ��
    (void)objNVSTimer.cancelTimer(&m_objTriggerConnTimeOut);

    //IVS_RUN_LOG_ERR("[ DecoderConnHandle = %#p ] Cancel Decoder Connection Handle Time Out Timer.", this);
};

void CDecoderConnHandle::HandleTimeout(void)
{
    if (NULL == m_pEventQ)
    {
        IVS_RUN_LOG_ERR( "DecoderConnHandle HandleTimeout fail, param is NULL.");
        return;
    }

    CDecoderMgrEvent* pEventRecv = NULL;
    (void)VOS_NEW(pEventRecv);
    if (VOS_NULL == pEventRecv)
    {
        IVS_RUN_LOG_ERR( "DecoderConnHandle HandleTimeout new Event fail.");
        return;
    }

    //BEGIN R001C05LITS01 MOD 2010-06-11 ligengqiang 00124479 for ���15λIP��������
    //��IPת��Ϊ�ַ���
    char* pszIP = inet_ntoa((in_addr &)m_peerAddr.m_lIpAddr);

    IVS_RUN_LOG_ERR("DecoderConnHandle HandleTimeout, "
            "connect(IP:%.15s, Port:%d), DecoderConnHandle(%#p)",
            pszIP, ntohs(m_peerAddr.m_usPort), this);
    //END   R001C05LITS01 MOD 2010-06-11 ligengqiang 00124479 for ���15λIP��������

    pEventRecv->m_ulEventType = RDA_PACKETMGR_EVENT_RD_CONN_TIMEO;
    pEventRecv->m_pDecoderConnHandle = this;

    //���ܴ�ʱ��Ϣ�����߳�����ɾ����handle������ȵ���addRef������ɹ�����������
    //��Ϣ�����̲߳������ɾ����handle�������ʾҵ���߳��Ѿ�ɾ���˸�handle������
    //�Ѹ��¼������¼����С�
    LONG lRetValue = this->AddRef();
    if (VOS_SUCCESS != lRetValue)
    {
        IVS_RUN_LOG_ERR("DecoderConnHandle HandleTimeout addRef fail,"
                "DecoderConnHandle(0x%x)", this);
        VOS_DELETE(pEventRecv);
        return;
    }

    IVS_DBG_LOG("DecoderConnHandle HandleTimeout pushBackEv,"
            "Event(0x%x), EventType(%lu), DecoderConnHandle(0x%x)", pEventRecv,
            pEventRecv->m_ulEventType, pEventRecv->m_pDecoderConnHandle);

    //�����¼�����
    lRetValue = m_pEventQ->pushBackEv(pEventRecv);
    if (VOS_OK != lRetValue)
    {
        IVS_RUN_LOG_ERR( "CDecoderConnHandle::HandleTimeout, pushBackEv fail,"
                "Event(0x%x), EventType(%lu), DecoderConnHandle(0x%x)", pEventRecv,
                pEventRecv->m_ulEventType, pEventRecv->m_pDecoderConnHandle);
        VOS_DELETE(pEventRecv);
        return;
    }

    return;
};

LONG CDecoderConnHandle::AddRef(void)
{
    LONG lRet = VOS_FAIL;

    (void)VOS_MutexLock(m_pMutexDecoderConnHandle);

    //���ü�����1
    if (m_bNeedUse)
    {
        m_ulReference++;
        lRet = VOS_SUCCESS;
    }

    (void)VOS_MutexUnlock(m_pMutexDecoderConnHandle);

    //IVS_RUN_LOG_INF("[ DecoderConnHandle = %#p ] Add Reference. NeedUse = %d. Reference = %u.",
    //        this, m_bNeedUse, m_ulReference);

    return lRet;
};

VOS_BOOLEAN CDecoderConnHandle::ReleaseRef(void)
{
    VOS_BOOLEAN bIfRemove = VOS_FALSE;

    //����Ѿ��ͷŹ�������Ҫ�ٱ��ͷ�
    if (InvalidSocket == getSockFD())
    {
        IVS_RUN_LOG_ERR( "DecoderConnHandle ReleaseRef, "
                "DecoderConnHandle(0x%x) has been closed.", this);
        return bIfRemove;
    }

    (void)VOS_MutexLock(m_pMutexDecoderConnHandle);

    //���ü�1
    if (0 != m_ulReference)
    {
        m_ulReference--;
    }

    //�ж��ܷ�ɾ��
    if ((0 == m_ulReference) && (VOS_FALSE == m_bNeedUse))
    {
        bIfRemove = VOS_TRUE;
    }

    (void)VOS_MutexUnlock(m_pMutexDecoderConnHandle);

    //IVS_RUN_LOG_INF("CDecoderConnHandle::ReleaseRef, DecoderConnHandle(0x%x), "
    //        "bIfRemove %d bNeedUse %d, ulReference %u.",
    //        this, bIfRemove, m_bNeedUse, m_ulReference);

    return bIfRemove;
};

//BEGIN R002C01SPC001 ADD 2011-08-16 ligengqiang 00124479 for ������֧�ֱ���˫����

void CDecoderConnHandle::GetLocalIp(string& strIp)
{
    //�Ѿ���ȡ����ֱ�ӷ��ص�ǰֵ
    if (0 != m_szLocalIp[0])
    {
        strIp = m_szLocalIp;

        IVS_DBG_LOG("Decoder Manager Local IP = %.15s.", m_szLocalIp);
        return;
    }

    //���»�ȡ������ַ
    sockaddr_in stAddr = {0};
    int nAddrLen = sizeof(stAddr);
    int nResult = getsockname((unsigned int)m_lSockFD, (struct sockaddr *)&stAddr, &nAddrLen);
    if (SOCKET_ERROR == nResult)
    {
        int nLastError = WSAGetLastError();
        IVS_RUN_LOG_ERR( "GetLocalIp Failed. socket = %ld. ErrorCode = %d.", m_lSockFD, nLastError);
        return;
    }

    char* pszLocalIp = inet_ntoa(stAddr.sin_addr);
    if (VOS_NULL == pszLocalIp)
    {
        IVS_RUN_LOG_ERR( "inet_ntoa Failed On GetLocalIp. socket = %ld.", m_lSockFD);
        return;
    }

    //���沢����
    if(!CToolsHelp::Strncpy(m_szLocalIp, sizeof(m_szLocalIp), pszLocalIp, strlen(pszLocalIp)))
	{
		IVS_RUN_LOG_ERR( "Copy LocalIp failed");
		return;
	}
    strIp = m_szLocalIp;

    IVS_DBG_LOG("Decoder Manager Local IP = %.15s.", m_szLocalIp);
}

//END   R002C01SPC001 ADD 2011-08-16 ligengqiang 00124479 for ������֧�ֱ���˫����

CDecoderConnHandleMgr::CDecoderConnHandleMgr() :  m_pMutexList(NULL),m_pArrayDecoderConnHandle(NULL)
{

}

CDecoderConnHandleMgr::~CDecoderConnHandleMgr()
{
    VOS_DELETE(m_pArrayDecoderConnHandle, MULTI);/*lint !e1551��ȷ�ϲ�������쳣��VOS_DELETE����try-catch�����쳣��*/
    if (NULL != m_pMutexList)
    {
        (void)VOS_DestroyMutex(m_pMutexList);
    }

    m_pMutexList = NULL;
    m_pArrayDecoderConnHandle = NULL;
}

LONG CDecoderConnHandleMgr::Init(CDecoderMgrEventQueue* pEventQ)
{
    IVS_DBG_LOG("Init Start.");

    if (NULL == pEventQ)
    {
        IVS_RUN_LOG_ERR( "Init End, param is NULL");
        return VOS_FAIL;
    }

    m_pMutexList = VOS_CreateMutex();
    if (NULL == m_pMutexList)
    {
        IVS_RUN_LOG_ERR( "Init End, CreateMutex Fail.");
        return VOS_FAIL;
    }

    //��ʼ���ͻ���������Դ��
    (void)VOS_NEW(m_pArrayDecoderConnHandle, MAX_RD_NUM);
    if (NULL == m_pArrayDecoderConnHandle)
    {
        IVS_RUN_LOG_ERR( "Init End, New pArrayDecoderConnHandle Fail.");
        (void)VOS_DestroyMutex(m_pMutexList);
        return VOS_FAIL;
    }

    CDecoderConnHandle* pDecoderConnHandle = m_pArrayDecoderConnHandle;
    for (int i = 0; i < MAX_RD_NUM; i++)
    {
        if (VOS_SUCCESS != pDecoderConnHandle->Init(pEventQ))
        {
            IVS_RUN_LOG_ERR( "Init End, init DecoderConnHandle(0x%x) fail.",
                    pDecoderConnHandle);

            VOS_DELETE(m_pArrayDecoderConnHandle, MULTI);
            (void)VOS_DestroyMutex(m_pMutexList);
            return VOS_FAIL;
        }

        m_ListDecoderConnHandle.push_back(pDecoderConnHandle);
        pDecoderConnHandle++;
    }

    IVS_DBG_LOG("Init End. init CDecoderConnHandleMgr success.");
    return VOS_SUCCESS;
};

CDecoderConnHandle* CDecoderConnHandleMgr::GetConnHandle(void)
{
    if ((NULL == m_pMutexList) || (NULL == m_pArrayDecoderConnHandle))
    {
        IVS_RUN_LOG_ERR( "Get Decoder Connection Handle Failed. Not Init yet.");
        return NULL;
    }

    CDecoderConnHandle* pDecoderConnHandle = NULL;

    (void)VOS_MutexLock(m_pMutexList);
    ULONG ulListSize = (ULONG)m_ListDecoderConnHandle.size();
    if (0 != ulListSize)
    {
        //�Ӷ���ͷ��popһ����Դ
        pDecoderConnHandle = *(m_ListDecoderConnHandle.begin()); //lint !e48
        m_ListDecoderConnHandle.pop_front();
        pDecoderConnHandle->m_bNeedUse = VOS_TRUE;
    }

    (void)VOS_MutexUnlock(m_pMutexList);

    IVS_DBG_LOG("Get Decoder Connection Handle End. DecoderConnHandle = %#p. Remain Size = %lu.",
            pDecoderConnHandle, ulListSize - 1);

    return pDecoderConnHandle;
};

void CDecoderConnHandleMgr::ReleaseConnHandle(CDecoderConnHandle* pDecoderConnHandle)
{
    //IVS_RUN_LOG_INF("ReleaseConnHandle Start.");

    if ((NULL == m_pMutexList) || (NULL == pDecoderConnHandle)
       || (NULL == m_pArrayDecoderConnHandle))
    {
        IVS_RUN_LOG_ERR( "ReleaseConnHandle End, param is NULL");
        return;
    }

    VOS_BOOLEAN bIfRemove = pDecoderConnHandle->ReleaseRef();
    if (VOS_FALSE == bIfRemove)
    {
        //����ɾ����˵��pDecoderConnHandle����ʹ����
        //IVS_RUN_LOG_INF("ReleaseConnHandle End, "
        //        "DecoderConnHandle(0x%x) is being used, "
        //        "can not release.", pDecoderConnHandle);
        return;
    }

    pDecoderConnHandle->m_pRemoteDecoder = NULL;

    //���ͨ�Ź�����ʵ��
    CConnMgr& objConnMgr = CStatusSingleton::Instance().GetConnMgr();

    //�ͷ�tcp������Դ
    objConnMgr.removeTcpClient(pDecoderConnHandle);

    (void)VOS_MutexLock(m_pMutexList);

    //pDecoderConnHandle->close();
    //����ɾ�����ŵ�����β��
    m_ListDecoderConnHandle.push_back(pDecoderConnHandle);

    (void)VOS_MutexUnlock(m_pMutexList);

    //IVS_RUN_LOG_INF("ReleaseConnHandle End, release DecoderConnHandle(0x%x) success.",
    //        pDecoderConnHandle);
    return;
}

LONG CDecoderMgrServerHandle::Init(CDecoderConnHandleMgr* pDecoderConnHandleMgr)
{
    IVS_DBG_LOG("Init Start.");

    if (NULL == pDecoderConnHandleMgr)
    {
        IVS_RUN_LOG_ERR( "Init End, param is NULL");
        return VOS_FAIL;
    }

    m_pDecoderConnHandleMgr = pDecoderConnHandleMgr;

    IVS_DBG_LOG("Init End, init CDecoderMgrServerHandle success.");
    return VOS_SUCCESS;
}

LONG CDecoderMgrServerHandle::handle_accept(const CNetworkAddr* pRemoteAddr,
                                            CTcpConnHandle* &   pTcpConnHandle)
{
    IVS_DBG_LOG("Decoder Manager Server Handle Connection Begin.");

    if (NULL == m_pDecoderConnHandleMgr)
    {
        IVS_RUN_LOG_ERR( "Decoder Manager Server Handle Connection Failed. Handle Manager is Emtpy.");
        return VOS_OK;
    }

    CDecoderConnHandle* pDecoderConnHandle = m_pDecoderConnHandleMgr->GetConnHandle();
    if (NULL == pDecoderConnHandle)
    {
        IVS_RUN_LOG_ERR( "Decoder Manager Server Handle Connection Failed. Get Connection Handle Failed.");
        return IVS_FAIL;
    }

    //�������ӳ�ʱʱ��
    pDecoderConnHandle->SetTimeOut(CUMW::INTERVAL_DECODER_CONN_TIME_OUT);

    pTcpConnHandle = (CTcpConnHandle*)pDecoderConnHandle;

	if(pRemoteAddr)
	{
		IVS_DBG_LOG(
				"[ DecoderConnHandle = %#p ] Decoder Manager Server Handle Connection(IP:%.15s, Port:%d) Success. Handle = %#p.",
				pDecoderConnHandle, inet_ntoa((in_addr &)pRemoteAddr->m_lIpAddr), ntohs(pRemoteAddr->m_usPort),
				pTcpConnHandle);
	}
    return VOS_SUCCESS;
}

LONG CRemoteDecoderArry::Init(CDecoderMgrEventQueue* pEventQ,
                              CDecoderConnHandleMgr *pDecoderConnHandleMgr)
{
    IVS_DBG_LOG("Init Start.");

    if ((NULL == pEventQ) || (NULL == pDecoderConnHandleMgr))
    {
        IVS_RUN_LOG_ERR( "Init End, param is NULL");
        return VOS_FAIL;
    }

    m_pDecoderConnHandleMgr = pDecoderConnHandleMgr;
    m_pEventQ = pEventQ;

    m_pMutexList = VOS_CreateMutex();
    if (NULL == m_pMutexList)
    {
        IVS_RUN_LOG_ERR( "Init End, create mutex fail.");
        return VOS_FAIL;
    }

    //����RemoteDecoder�б�ѿռ�
    (void)VOS_NEW(m_pArrayRemoteDecoder, MAX_LIST_NUM);
    if (NULL == m_pArrayRemoteDecoder)
    {
        (void)VOS_DestroyMutex(m_pMutexList);
        IVS_RUN_LOG_ERR( "Init End, new m_pArrayRemoteDecoder fail.");
        return VOS_FAIL;
    }

    //��ʼ��RemoteDecoder�б�
    CRemoteDecoder* pRemoteDecoder = m_pArrayRemoteDecoder;
    for (LONG i = 0; i < MAX_LIST_NUM; i++)
    {
        pRemoteDecoder->m_ulHeartBeatCount   = 0;
        pRemoteDecoder->m_pDecoderConnHandle = NULL;
        m_FreeListRemoteDecoder.push_back(pRemoteDecoder);
        pRemoteDecoder++; //++ƫ�� һ�� RemoteDecoder
    }

    IVS_DBG_LOG("Init End, init CRemoteDecoderArry success.");
    return VOS_SUCCESS;
}

void CRemoteDecoderArry::Release()
{
    if (NULL != m_pMutexList)
    {
        (void)VOS_DestroyMutex(m_pMutexList);
    }

    //�ͷŽ���������ȷ��ע�͵��޴���
    CRemoteDecoder* pTempRemoteDecoder = NULL;
    ListRemoteDecoderIte itListRemoteDecoder;

    //�ر���������
    for (itListRemoteDecoder = m_WorkListRemoteDecoder.begin();
         itListRemoteDecoder != m_WorkListRemoteDecoder.end();         //lint !e81
         ++itListRemoteDecoder)                                        //lint !e53
    {
        pTempRemoteDecoder = *itListRemoteDecoder; //lint !e48

        //�ر�tcp����
        CDecoderConnHandle * pDecoderConnHandle =
            pTempRemoteDecoder->m_pDecoderConnHandle;
        if (NULL == pDecoderConnHandle)
        {
            continue;
        }

        pDecoderConnHandle->m_bNeedUse = VOS_FALSE;

        //��������жϣ���Ϊ��m_pDecoderConnHandleMgr���ڳ�ʼ����ʱ�����
        if (NULL == m_pDecoderConnHandleMgr)
        {
            continue;
        }

        m_pDecoderConnHandleMgr->ReleaseConnHandle(pDecoderConnHandle);
    }

    VOS_DELETE(m_pArrayRemoteDecoder, MULTI);
}

CRemoteDecoderArry::~CRemoteDecoderArry()
{
    m_pMutexList = NULL;
    m_pEventQ = NULL;
    m_pDecoderConnHandleMgr = NULL;
}

CRemoteDecoder* CRemoteDecoderArry::GetRemoteDecoder(void)
{
    if ((NULL == m_pMutexList) || (NULL == m_pArrayRemoteDecoder))
    {
        IVS_RUN_LOG_ERR( "Get Remote Decoder Object Failed. Not Init yet.");
        return NULL;
    }

    CRemoteDecoder* pRemoteDecoder = NULL;

    //�Ӷ����л�ȡָ��RemoteDecoder��ָ��
    (void)VOS_MutexLock(m_pMutexList);

    ULONG ulListSize = (ULONG)m_FreeListRemoteDecoder.size();

    if (0 == ulListSize)
    {
        (void)VOS_MutexUnlock(m_pMutexList);

        IVS_RUN_LOG_ERR("Get Remote Decoder Object Failed. No More Object.");
        return NULL;
    }

    pRemoteDecoder = *(m_FreeListRemoteDecoder.begin()); //lint !e48

    //�ӿ��ж���ͷ��ɾ��
    m_FreeListRemoteDecoder.pop_front();

    //���빤������β
    m_WorkListRemoteDecoder.push_back(pRemoteDecoder);

    pRemoteDecoder->m_ulHeartBeatCount   = 0;
    pRemoteDecoder->m_pDecoderConnHandle = NULL;

    (void)VOS_MutexUnlock(m_pMutexList);

    IVS_DBG_LOG("Get Remote Decoder Object End. RemoteDecoder = %#p. RemainSize = %lu.",
            pRemoteDecoder, ulListSize - 1);

    return pRemoteDecoder;
}

void CRemoteDecoderArry::ReleaseRemoteDecoder(CRemoteDecoder* pRemoteDecoder)
{
    IVS_DBG_LOG("ReleaseRemoteDecoder Start.");

    if ((NULL == m_pMutexList)
       || (NULL == m_pArrayRemoteDecoder) || (NULL == pRemoteDecoder))
    {
        IVS_RUN_LOG_ERR( "ReleaseRemoteDecoder End, param is NULL");
        return;
    }

    //��ȡ������id
    char szDecoderID[CUMW_DECODER_ID_LEN + 1];
    if(!CToolsHelp::Strncpy(szDecoderID, sizeof(szDecoderID), pRemoteDecoder->m_stDecoderInfo.szDecoderID, sizeof(szDecoderID)-1))
	{
		IVS_RUN_LOG_ERR( "ReleaseRemoteDecoder Copy DecoderID failed.");
		return;
	}

    //������������ʱ���Զ��ͷ�
    CLockGuard guardLock(m_pMutexList);

    //BEGIN V100R001C05 MODIFY 2010-01-19 zhaoyuzhen z00137994 for ���������ظ���
    //�ӹ��������в��ҽ���������
    CRemoteDecoder* pRemoteDecoderRelease = NULL;
    long lResult = FindDecoderInfoByID(szDecoderID, pRemoteDecoderRelease);
    if ((VOS_SUCCESS != lResult) || (NULL == pRemoteDecoderRelease))
    {
        //δ���ҵ�����������ֱ�ӷ���
        IVS_RUN_LOG_ERR("ReleaseRemoteDecoder End, "
                "release RemoteDecoder(DecoderID %.20s) fail,"
                "because can not find this RemoteDecoder in worklist.", szDecoderID);
        return;
    }

    //����ҵ������ͷ�Զ�̽���������
    m_WorkListRemoteDecoder.remove(pRemoteDecoderRelease);
    pRemoteDecoderRelease = NULL;

    //��ӵ����ж���
    pRemoteDecoder->m_pDecoderConnHandle = NULL;
    pRemoteDecoder->m_ulHeartBeatCount = 0;
    m_FreeListRemoteDecoder.push_back(pRemoteDecoder);

    IVS_DBG_LOG("ReleaseRemoteDecoder End, "
            "Release RemoteDecoder success ,DecoderID %.20s.", szDecoderID);
    //END V100R001C05 MODIFY 2010-01-19 zhaoyuzhen z00137994 for ���������ظ���

    return;
}

CDecoderConnHandle* CRemoteDecoderArry::GetConnHandle(const char* pszDecoderID)
{
    IVS_DBG_LOG("GetConnHandle Start.");

    if ((NULL == m_pMutexList) || (NULL == m_pArrayRemoteDecoder)
        || (NULL == pszDecoderID))
    {
        IVS_RUN_LOG_ERR( "GetConnHandle End, param is NULL");
        return NULL;
    }

    CDecoderConnHandle* pDecoderConnHandle = NULL;

    //�����ں�������ʱ���Զ��ͷ�
    CLockGuard guardLock(m_pMutexList);

    //BEGIN V100R001C05 MODIFY 2010-01-19 zhaoyuzhen z00137994 for ���������ظ���
    //�ӹ��������в��ҽ���������
    CRemoteDecoder *pRemoteDecoder = NULL;
    long lResult = FindDecoderInfoByID(pszDecoderID, pRemoteDecoder);
    if ((VOS_SUCCESS != lResult) || (NULL == pRemoteDecoder))
    {
        //δ�ҵ�
        IVS_RUN_LOG_ERR("GetConnHandle End, can not find "
                "any DecoderInfo by DecoderID(%.20s)", pszDecoderID);
        return NULL;
    }

    //�ҵ��˽�������Ӧ������
    pDecoderConnHandle = pRemoteDecoder->m_pDecoderConnHandle;
    if (NULL == pDecoderConnHandle)
    {
        IVS_RUN_LOG_INF("GetConnHandle End, DecoderConnHandle is null.");
        return NULL;
    }

    //END V100R001C05 MODIFY 2010-01-19 zhaoyuzhen z00137994 for ���������ظ���

    //�ж������Ƿ����ʹ��
    if (VOS_FALSE == pDecoderConnHandle->m_bNeedUse)
    {
        IVS_RUN_LOG_INF("GetConnHandle End, "
                "DecoderConnHandle(0x%x) is not need use, pszDecoderID %.20s",
                pDecoderConnHandle, pszDecoderID);
        return NULL;
    }

    IVS_DBG_LOG("GetConnHandle End, "
            "DecoderConnHandle(0x%x), pszDecoderID %.20s",
            pDecoderConnHandle, pszDecoderID);
    return pDecoderConnHandle;
}

LONG CRemoteDecoderArry::GetDecoderInfo
(   char*               pszDecoderID,
    CUMW_DECODER_INFO&  stDecoderInfo)
{
    if ((NULL == m_pMutexList)
       || (NULL == m_pArrayRemoteDecoder) || (NULL == pszDecoderID))
    {
        IVS_RUN_LOG_ERR( "GetDecoderInfo End, param is NULL");
        return VOS_FAIL;
    }

    //��ʼ������ֵ
    long lResult = VOS_SUCCESS;

    //�����ں�������ʱ���Զ��ͷ�
    CLockGuard guardLock(m_pMutexList);

    //���Ҷ��У���ȡ����������
    CRemoteDecoder *pRemoteDecoder = NULL;
    lResult = FindDecoderInfoByID(pszDecoderID, pRemoteDecoder);
    if ((VOS_SUCCESS != lResult) || (NULL == pRemoteDecoder))
    {
        //δ�ҵ�
        IVS_RUN_LOG_INF("[ DecoderID = %.20s ] Receive a new Decoder.", pszDecoderID);
        return VOS_FAIL;
    }

    //�ҵ�
    if(!CToolsHelp::Memcpy(&stDecoderInfo, sizeof(stDecoderInfo), &pRemoteDecoder->m_stDecoderInfo, sizeof(stDecoderInfo)))
	{
		IVS_RUN_LOG_ERR("GetDecoderInfo Copy DecoderInfo failed");
		return IVS_ALLOC_MEMORY_ERROR;
	}
    //��ӡ��Ϣ
    PrintDecoderInfo(*pRemoteDecoder);

    IVS_DBG_LOG("[ DecoderID = %.20s ] Decoder Exist Already.", pszDecoderID);
    return VOS_SUCCESS;
}

void CRemoteDecoderArry::CheckHeartBeat( void )
{
    //IVS_RUN_LOG_INF("CheckHeartBeat Start.");

    if ((NULL == m_pMutexList) || (NULL == m_pArrayRemoteDecoder))
    {
        IVS_RUN_LOG_ERR( "CheckHeartBeat End, param is NULL");
        return;
    }

    ULONG ulWorkDecoderNum = 0;
    CRemoteDecoder* pTempRemoteDecoder = NULL;

    //��������list
    CLockGuard guardLock(m_pMutexList);

    //��ȡ�ܽ�������Ŀ
    ulWorkDecoderNum = m_WorkListRemoteDecoder.size();
    IVS_RUN_LOG_INF("In CheckHeartBeat, now %u RemoteDecoder in work list",
            ulWorkDecoderNum);

    //���Ҷ���
    ListRemoteDecoderIte itListRemoteDecoder;
    for (itListRemoteDecoder = m_WorkListRemoteDecoder.begin();
         itListRemoteDecoder != m_WorkListRemoteDecoder.end(); //lint !e81
         ++itListRemoteDecoder) //lint !e53
    {
        pTempRemoteDecoder = (*itListRemoteDecoder); //lint !e48

        //��ӡ��������Ϣ
        PrintDecoderInfo(*pTempRemoteDecoder, false);

        //���pDecoderConnHandleΪ�գ�����Ҫ��������
        CDecoderConnHandle* pDecoderConnHandle =
            pTempRemoteDecoder->m_pDecoderConnHandle;
        if (NULL == pDecoderConnHandle)
        {
            IVS_RUN_LOG_ERR("CheckHeartBeat End, "
                    "this RemoteDecoder is in work list, "
                    "but its DecoderConnHandle is NULL .");
            continue;
        }

        CDecoderMgrEvent* pEventSend = NULL;

        (void)VOS_NEW(pEventSend);
        if (VOS_NULL == pEventSend)
        {
            IVS_RUN_LOG_ERR( "CheckHeartBeat End, new event fail.");
            continue;
        }

        //��������һ
        pTempRemoteDecoder->m_ulHeartBeatCount++;

        //�����������, ��ӷ�����������
        pEventSend->m_ulEventType = RDA_PACKETMGR_EVENT_SEND_HEARTBEAT;
        pEventSend->m_pDecoderConnHandle = pDecoderConnHandle;

        //�������3��û����Ӧ, ������ӳ�ʱ�������񣬵��������
        if (pTempRemoteDecoder->m_ulHeartBeatCount > MAX_HEARTBEAT_RETRY_COUNT)
        {
            pEventSend->m_ulEventType = RDA_PACKETMGR_EVENT_RD_CONN_TIMEO;
        }

        LONG lRetValue = pDecoderConnHandle->AddRef();
        if (VOS_SUCCESS != lRetValue)
        {
            VOS_DELETE(pEventSend);
            IVS_RUN_LOG_ERR( "CheckHeartBeat End, "
                    "addRef fail DecoderConnHandle(0x%x).",
                    pDecoderConnHandle);
            return;
        }

        //IVS_RUN_LOG_INF("CheckHeartBeat End, "
        //        "pushBackEv, Event(0x%x), EventType(%d), "
        //        "DecoderConnHandle(0x%x), HeartBeatCount(%u)", pEventSend,
        //        pEventSend->m_ulEventType, pEventSend->m_pDecoderConnHandle,
        //        pTempRemoteDecoder->m_ulHeartBeatCount);

        //�����¼�����
		if(NULL == m_pEventQ)
		{
			IVS_RUN_LOG_ERR("CRemoteDecoderArry::CheckHeartBeat", "m_pEventQ=NULL");
			return;
		}
        lRetValue = m_pEventQ->pushBackEv(pEventSend);
        if (VOS_OK != lRetValue)
        {
            IVS_RUN_LOG_ERR( "CheckHeartBeat End, "
                    "pushBackEv fail, Event(0x%x), EventType(%lu), "
                    "DecoderConnHandle(0x%x), HeartBeatCount(%lu)", pEventSend,
                    pEventSend->m_ulEventType, pEventSend->m_pDecoderConnHandle,
                    pTempRemoteDecoder->m_ulHeartBeatCount);

            VOS_DELETE(pEventSend);
            return;
        }
    }

    //IVS_RUN_LOG_INF("CheckHeartBeat End.");
    return;
}

//BEGIN V100R001C05 MODIFY 2010-01-18 zhaoyuzhen z00137994 for ���������ظ���

void CRemoteDecoderArry::ClearHeartBeatCount( char* pszDecoderID )
{
    if ((NULL == m_pMutexList) || (NULL == m_pArrayRemoteDecoder))
    {
        IVS_RUN_LOG_ERR( "ClearHeartBeatCount End, param is NULL");
        return;
    }

    //��ʼ������ֵ
    long lResult = VOS_SUCCESS;

    //��������list
    CLockGuard guardLock(m_pMutexList);

    //���ݽ�����ID���ҽ������ṹ����Ϣ
    CRemoteDecoder *pRemoteDecoder = NULL;
    lResult = FindDecoderInfoByID(pszDecoderID, pRemoteDecoder);
    if ((VOS_SUCCESS != lResult) || (NULL == pRemoteDecoder))
    {
        //δ�ҵ�
        IVS_RUN_LOG_ERR("ClearHeartBeatCount End, can not find "
                "any DecoderInfo by DecoderID(%.20s)", pszDecoderID);
        return;
    }

    //�ҵ��󣬽�������������
    pRemoteDecoder->m_ulHeartBeatCount = 0;
    return;
}

//END V100R001C05 MODIFY 2010-01-18 zhaoyuzhen z00137994 for ���������ظ���

void CRemoteDecoderArry::PrintDecoderInfo
(   const CRemoteDecoder& objRemoteDecoder, bool bChannel) const
{
    const CUMW_DECODER_INFO& stDecoderInfo = objRemoteDecoder.m_stDecoderInfo;

    IVS_RUN_LOG_INF("[ DecoderID = %.20s ] Decoder Info: Name = %.32s. "
            "ConnHandle = %#p. IP = %.15s. ChannelNum = %lu.",
            stDecoderInfo.szDecoderID, stDecoderInfo.szDecoderName,
            objRemoteDecoder.m_pDecoderConnHandle, stDecoderInfo.szDecoderIP,
            stDecoderInfo.ulChannelNum);
	if(bChannel)
	{
		unsigned long ulChannelNum = stDecoderInfo.ulChannelNum;
		if (CUMW_DECODER_CHANNEL_MAX_NUM < ulChannelNum)
		{
			ulChannelNum = CUMW_DECODER_CHANNEL_MAX_NUM;
		}

		for (unsigned long ulIndex = 0; ulIndex < ulChannelNum; ++ulIndex)
		{
			IVS_RUN_LOG_INF("[ DecoderID = %.20s ] Channel Info: ID = %u. Name = %.32s.",
				stDecoderInfo.szDecoderID,
				(stDecoderInfo.stChannelInfoArray[ulIndex]).ulChannelID,
				(stDecoderInfo.stChannelInfoArray[ulIndex]).szChannelName);
		}
	}
}

//BEGIN V100R001C05 ADD 2010-01-18 zhaoyuzhen z00137994 for ���������ظ���

long CRemoteDecoderArry::FindDecoderInfoByID(const char*      pszDecoderID,
                                             CRemoteDecoder*& pDecoderInfo)
{
    //�������
    if (NULL == pszDecoderID)
    {
        IVS_RUN_LOG_ERR( "In FindDecoderInfoByID, parameter is null.");
        return VOS_FAIL;
    }

    //���Ҷ���
    ListRemoteDecoderIte itListRemoteDecoder;
    for (itListRemoteDecoder = m_WorkListRemoteDecoder.begin();
         itListRemoteDecoder != m_WorkListRemoteDecoder.end(); //lint !e81
         ++itListRemoteDecoder) //lint !e53
    {
        pDecoderInfo = (*itListRemoteDecoder); //lint !e48
        long lResult = strncmp(pDecoderInfo->m_stDecoderInfo.szDecoderID, pszDecoderID, LEN_RDA_ID);

        //�ҵ��˽����������سɹ�
        if (0 == lResult)
        {
            return VOS_SUCCESS;
        }
    }

    //δ�ҵ�������ʧ��
    return VOS_FAIL;
}

//END V100R001C05 ADD 2010-01-18 zhaoyuzhen z00137994 for ���������ظ���
CRemoteDecoderReqProc::CRemoteDecoderReqProc() : m_pRecvBuffer(NULL)
	,m_ulRecvBuffLen(0)
	,m_pSendBuffer(NULL)	
	,m_ulSendBuffLen(0)	
	,m_pRDAPacketMgr(NULL)
	,m_pDecoderConnHandleMgr(NULL)
	,m_pRemoteDecoderArry(NULL)
	,m_pRDAProtStack(NULL)
{

}

//BEGIN V100R001C05 MODIFY 2010-01-18 zhaoyuzhen z00137994 for ���������ظ���

CRemoteDecoderReqProc::~CRemoteDecoderReqProc()
{
    VOS_DELETE(m_pRecvBuffer, MULTI);/*lint !e1551��ȷ�ϲ�������쳣��VOS_DELETE����try-catch�����쳣��*/
    m_pRDAPacketMgr = NULL;
    m_pDecoderConnHandleMgr = NULL;
    m_pRemoteDecoderArry = NULL;
    m_pRDAProtStack = NULL;
    m_pRecvBuffer = NULL;    //�������ݻ�����
    m_pSendBuffer = NULL;
}

//END V100R001C05 MODIFY 2010-01-18 zhaoyuzhen z00137994 for ���������ظ���

BOOL CRemoteDecoderReqProc::CheckIsInited()
{
    IVS_DBG_LOG("CheckIsInited Start.");

    //���������ݻ�����ָ��
    if (NULL == m_pRecvBuffer)
    {
        IVS_RUN_LOG_ERR( "CheckIsInited End . m_pRecvBuffer is NULL.");
        return FALSE;
    }

    //��鷢�����ݻ�����ָ��
    if (NULL == m_pSendBuffer)
    {
        IVS_RUN_LOG_ERR( "CheckIsInited End . m_pSendBuffer is NULL.");
        return FALSE;
    }

    //���ͻ������ӹ���ָ��
    if (NULL == m_pDecoderConnHandleMgr)
    {
        IVS_RUN_LOG_ERR( "CheckIsInited End . m_pDecoderConnHandleMgr is NULL.");
        return FALSE;
    }

    //���Զ�̽���������ָ��
    if (NULL == m_pRemoteDecoderArry)
    {
        IVS_RUN_LOG_ERR( "CheckIsInited End . m_pRemoteDecoderArry is NULL.");
        return FALSE;
    }

    //����������ָ��
    if (NULL == m_pRDAPacketMgr)
    {
        IVS_RUN_LOG_ERR( "CheckIsInited End . m_pRDAPacketMgr is NULL.");
        return FALSE;
    }

    //���Զ�̽�����Э��ջָ��
    if (NULL == m_pRDAProtStack)
    {
        IVS_RUN_LOG_ERR( "CheckIsInited End . m_pRDAProtStack is NULL.");
        return FALSE;
    }

    IVS_DBG_LOG("CheckIsInited End. ");
    return TRUE;
} /*lint !e1762*/ //�˴����趨���const����������ÿ�����ô˺����ĵط������������Ӧ��PCLint����

LONG CRemoteDecoderReqProc::Init
(   IN PCUMW_CALLBACK_NOTIFY   pFunNotifyCallBack,
    IN void*                   pCallBackUserData,
    CRDAPacketMgr*             pRDAPacketMgr,
    CDecoderConnHandleMgr*     pDecoderConnHandleMgr,
    CRemoteDecoderArry*        pRemoteDecoderArry,
    CRDAProtStack*             pRDAProtStack)
{
    IVS_DBG_LOG("Init Start.");

    //�������
    if ((NULL == pRDAPacketMgr) || (NULL == pDecoderConnHandleMgr)
        || (NULL == pRemoteDecoderArry)
        || (NULL == pRDAProtStack))
    {
        IVS_RUN_LOG_ERR( "Init End, param is NULL");
        return VOS_FAIL;
    }

    //����֪ͨ�ص�����״ֵ̬
    m_objStatusNotifyCallBack.SetStatusValue(pFunNotifyCallBack, pCallBackUserData);

    m_pRDAPacketMgr = pRDAPacketMgr;
    m_pDecoderConnHandleMgr = pDecoderConnHandleMgr;
    m_pRemoteDecoderArry = pRemoteDecoderArry;
    m_pRDAProtStack = pRDAProtStack;

    (void)VOS_NEW(m_pRecvBuffer, MAX_RDAMSG_LEN);
    if (NULL == m_pRecvBuffer)
    {
        IVS_RUN_LOG_ERR( "Init End, new m_pRecvBuffer fail");
        return VOS_FAIL;
    }

    (void)VOS_NEW(m_pSendBuffer, MAX_RDAMSG_LEN);
    if (NULL == m_pSendBuffer)
    {
		VOS_DELETE(m_pRecvBuffer, MAX_RDAMSG_LEN);
        IVS_RUN_LOG_ERR( "Init End, new m_pSendBuffer fail");
        return VOS_FAIL;
    }

    IVS_DBG_LOG("Init End, init CRemoteDecoderReqProc success.");
    return VOS_SUCCESS;
};

void CRemoteDecoderReqProc::Exit(void)
{
    m_pRDAPacketMgr = NULL;
    m_pDecoderConnHandleMgr = NULL;
    m_pRemoteDecoderArry = NULL;
    VOS_DELETE(m_pRecvBuffer, MAX_RDAMSG_LEN);
    VOS_DELETE(m_pSendBuffer, MAX_RDAMSG_LEN);

    IVS_DBG_LOG("Exit CRemoteDecoderReqProc Success.");

    return;
}

LONG CRemoteDecoderReqProc::ProcMsg( CDecoderMgrEvent* pEventRecv)
{
    //����Ƿ��Ѿ���ʼ��
    if (!CheckIsInited())
    {
        IVS_RUN_LOG_ERR( "ProcMsg End, Not Inited.");
        return VOS_FAIL;
    }

    if (NULL == pEventRecv)
    {
        IVS_RUN_LOG_ERR( "ProcMsg End, param is NULL");
        return VOS_FAIL;
    }

    //���¼��ڵ��л�ȡ����handle��ָ��
    CDecoderConnHandle *pDecoderConnHandle = pEventRecv->m_pDecoderConnHandle;
    if (NULL == pDecoderConnHandle)
    {
        IVS_RUN_LOG_ERR("ProcMsg End, "
                "pEventRecv->m_pDecoderConnHandle is NULL, "
                "Event(0x%x), EventType(%lu)",
                pEventRecv, pEventRecv->m_ulEventType);
        return VOS_FAIL;
    }

    IVS_DBG_LOG("[ DecoderConnHandle = %#p ] Process Decoder Message Begin. EventType = 0x%04X.",
            pDecoderConnHandle, pEventRecv->m_ulEventType);

    LONG lRet = VOS_FAIL;
    CNetworkAddr rdAddr; 

    //�Ƚ�����Ϣͷ���ٽ�����Ϣ��
    //������Ϣͷ����
    lRet = pDecoderConnHandle->recvWithTimeout(m_pRecvBuffer, &rdAddr,
                                               LEN_RDA_MSG_HEAD, MAX_RECV_WAITTIME, GARBAGE_PARAM);
    if (lRet <= 0)
    {
        IVS_RUN_LOG_ERR( "ProcMsg End, socket closed "
                "when receive message head. release DecoderConnHandle(0x%x).",
                pDecoderConnHandle);
        return VOS_FAIL;
    }

    RDAMsgHead stRDAMsgHead;
    (void)m_pRDAProtStack->GetMsgHeadFromBuff(m_pRecvBuffer,
                                              LEN_RDA_MSG_HEAD, stRDAMsgHead);
    m_ulRecvBuffLen = stRDAMsgHead.ulPackLength;

    /*//�ж��յ������ݵĳ���
    if((RDA_MSGHEAD_LEN > m_ulRecvBuffLen) ||
                    (MAX_RDAMSG_LEN < m_ulRecvBuffLen))
    {
        IVS_LOG(LOG_CRITICAL, "ProcMsg End, packet length(%u) is invalid. "
            "The length should range from %u to %u, "
            "release DecoderConnHandle(0x%x)",  m_ulRecvBuffLen,
            RDA_MSGHEAD_LEN,  MAX_RDAMSG_LEN,  pDecoderConnHandle);

        return VOS_FAIL;
    }*/

    /*//������Ϣ������
    lRet = pDecoderConnHandle->recvWithTimeout(m_pRecvBuffer + LEN_RDA_MSG_HEAD,
                              &rdAddr, m_ulRecvBuffLen - LEN_RDA_MSG_HEAD,
                               MAX_RECV_WAITTIME, GARBAGE_PARAM);
    if(lRet <= 0)
    {
        IVS_RUN_LOG_ERR( "ProcMsg End, socket closed."
            "when receive message body. release DecoderConnHandle(0x%x).",
            pDecoderConnHandle);
        return VOS_FAIL;
    }*/

    //���������¼�
    pDecoderConnHandle->setHandleRecv(VOS_TRUE);

	const ULONG ulTotalRecvBytes = lRet;
	ULONG ulHadDealBytes = 0; //�Ѿ������˵��ֽڳ���

	char* pszBuf = NULL;
	(void)VOS_NEW(pszBuf, ulTotalRecvBytes + 1);
	if (NULL == pszBuf)
	{
		IVS_RUN_LOG_ERR( "ProcMsg End, Create Buffer Failed.");
		return IVS_ALLOC_MEMORY_ERROR;
	}

	do 
	{
		IVS_DBG_LOG("ProcMsg: MsgType Is %d, PackLength Is %d, Total Recv Bytes Is %d", 
			stRDAMsgHead.usMsgType, stRDAMsgHead.ulPackLength, ulTotalRecvBytes);

		//����Ϣ���ʹ����¼�
		USHORT usReqProMsgType = stRDAMsgHead.usMsgType;
		switch (usReqProMsgType)
		{
		case RDA_REGISTER_REQ:
			{
				//����ע���¼�
				lRet = ProcRegisterMsg(*pEventRecv);
				break;
			}
		case RDA_UNREGISTER_REQ:
			{
				//����ע��������
				lRet = ProcUnRegisterMsg( pEventRecv );
				break;
			}
		case RDA_INIT_BUSINESS_RSP:
		case RDA_START_BUSINESS_RSP:
		case RDA_STOP_BUSINESS_RSP:
		case RDA_TUNEUP_VOLUME_RSP:
		case RDA_AUDIO_CONTROL_RSP:
			{
				//����ͬ����Ӧ
				lRet = ProcSyncRspMsg( pEventRecv );
				break;
			}
		case RDA_HEARTBEAT_RSP:
			{
				//����������Ӧ��Ϣ
				lRet = ProcHeartBeatRsp();
				break;
			}
		case RDA_HEARTBEAT_REQ:
			{
				//��������������Ϣ
				lRet = ProcHeartBeatReq(*pEventRecv);
				break;
			}
		case RDA_ERROR_NOTIFY_REQ:
			{
				//��������ϱ�������Ϣ
				lRet = ProcErrorNotifyReq(*pEventRecv);
				break;
			}
		default:
			{
				IVS_RUN_LOG_ERR("Remote Decoder unknown msg type %d", usReqProMsgType);
				lRet = VOS_SUCCESS;
				break;
			}
		}
		
		IVS_DBG_LOG("[ DecoderConnHandle = %#p ] Process Decoder One Message End. Result = 0x%04X.",
			pDecoderConnHandle, lRet);

		ulHadDealBytes += stRDAMsgHead.ulPackLength;
		IVS_DBG_LOG("ulTotalRecvBytes = %d, ulHadDealBytes = %d.", ulTotalRecvBytes, ulHadDealBytes);

		if (ulTotalRecvBytes <= ulHadDealBytes)
		{
			break;
		}

		memset(pszBuf, 0, ulTotalRecvBytes + 1);
		memcpy(pszBuf, m_pRecvBuffer, ulTotalRecvBytes);

		memset(m_pRecvBuffer, 0, ulTotalRecvBytes + 1);
		memcpy(m_pRecvBuffer, &pszBuf[stRDAMsgHead.ulPackLength], ulTotalRecvBytes - ulHadDealBytes);
	
		memset(&stRDAMsgHead, 0x00, sizeof(RDAMsgHead));
		long lRes = m_pRDAProtStack->GetMsgHeadFromBuff(m_pRecvBuffer, LEN_RDA_MSG_HEAD, stRDAMsgHead);
		if (VOS_SUCCESS != lRes)
		{
			IVS_RUN_LOG_ERR("Remote Decoder Msg Invalid!");
			break;
		}
		m_ulRecvBuffLen = stRDAMsgHead.ulPackLength;

	} while (1);

	VOS_DELETE(pszBuf, MULTI);

    IVS_DBG_LOG("[ DecoderConnHandle = %#p ] Process Decoder Message End. Result = 0x%04X.",
            pDecoderConnHandle, lRet);
    return lRet;
}

long CRemoteDecoderReqProc::ProcHeartBeatRsp()
{
    //IVS_RUN_LOG_INF("Process Heart Beat Response Message Begin.");

    //����Ƿ��Ѿ���ʼ��
    if (!CheckIsInited())
    {
        IVS_RUN_LOG_ERR( "Process Heart Beat Response Message Failed. Not Init yet.");
        return IVS_FAIL;
    }

    //���յ�������Ϣ��������Ĭ�϶�Ϊ��������
    CRDAHeartBeatRsp objRDARsp;
    long lResult = m_pRDAProtStack->DecodeMsg(m_pRecvBuffer, MAX_RDAMSG_LEN, objRDARsp);
    if (VOS_SUCCESS != lResult)
    {
        IVS_RUN_LOG_ERR( "Decode Message Failed On Process Heart Beat Response Message.");
        return IVS_FAIL;
    }

    //��ӡ������Ϣ
    //m_pRDAProtStack->Print(pRDAHeartBeatReq);

    //��������������
    m_pRemoteDecoderArry->ClearHeartBeatCount(objRDARsp.m_szDecoderID);

    IVS_DBG_LOG("[ DecoderID = %.20s ] Process Heart Beat Response Message Success.", objRDARsp.m_szDecoderID);
    return IVS_SUCCEED;
}

long CRemoteDecoderReqProc::ProcHeartBeatReq
(   IN CDecoderMgrEvent& objEvent)
{
    IVS_DBG_LOG("Process Heart Beat Request Message Begin.");

    //����Ƿ��Ѿ���ʼ��
    if (!CheckIsInited())
    {
        IVS_RUN_LOG_ERR( "Process Heart Beat Request Message Failed. Not Init yet.");
        return IVS_FAIL;
    }

    //���յ�������Ϣ��������Ĭ�϶�Ϊ��������
    CRDAHeartBeatReq objRDAReq;
    long lResult = m_pRDAProtStack->DecodeMsg(m_pRecvBuffer, MAX_RDAMSG_LEN, objRDAReq);
    if (VOS_SUCCESS != lResult)
    {
        IVS_RUN_LOG_ERR( "Decode Message Failed On Process Heart Beat Request Message.");
        return IVS_FAIL;
    }

    //��ӡ������Ϣ
    //m_pRDAProtStack->Print(pRDAHeartBeatReq);

    //��������������
    m_pRemoteDecoderArry->ClearHeartBeatCount(objRDAReq.m_szDecoderID);

    //��䷵�ظ�����������Ӧ��Ϣ��stRDAHeartBeat
    CRDAHeartBeatRsp objRDAResp;
    objRDAResp.m_stMsgHead.usPackID = objRDAReq.m_stMsgHead.usPackID;
    if(!CToolsHelp::Strncpy(objRDAResp.m_szDecoderID, sizeof(objRDAResp.m_szDecoderID), objRDAReq.m_szDecoderID, sizeof(objRDAResp.m_szDecoderID) - 1))
	{
		IVS_RUN_LOG_ERR("ProcHeartBeatReq Copy DecoderId error.");
		return IVS_ALLOC_MEMORY_ERROR;
	}

    IVS_ULONG ulValidLength = 0;
    lResult = m_pRDAProtStack->EncodeMsg(m_pSendBuffer, MAX_RDAMSG_LEN, &objRDAResp, ulValidLength);
    if (VOS_SUCCESS != lResult)
    {
        IVS_RUN_LOG_ERR( "[ DecoderID = %.20s ] Encode Message Failed On Process Heart Beat Request Message.",
                objRDAReq.m_szDecoderID);
        return IVS_FAIL;
    }

    //��ӡ������Ӧ��Ϣ
    //m_pRDAProtStack->Print(&cRDAHeartBeatRsp);

    //������Ӧ��Ϣ
    CDecoderConnHandle* pDecoderConnHandle = objEvent.m_pDecoderConnHandle;
    if (NULL == pDecoderConnHandle)
    {
        IVS_RUN_LOG_ERR(
                "[ DecoderID = %.20s ] Process Heart Beat Request Message Failed. Decoder Connection Handle is Emtpy.",
                objRDAReq.m_szDecoderID);
        return IVS_FAIL;
    }

    lResult = pDecoderConnHandle->send(m_pSendBuffer, ulValidLength, enSyncOp);
    if (lResult < 0)
    {
        IVS_RUN_LOG_ERR( "[ DecoderID = %.20s ] Send Response Message Failed On Process Heart Beat Request Message.",
                objRDAReq.m_szDecoderID);
        return IVS_FAIL;
    }

    IVS_DBG_LOG("[ DecoderID = %.20s ] Process Heart Beat Request Message Success.", objRDAReq.m_szDecoderID);
    return IVS_SUCCEED;
}

long CRemoteDecoderReqProc::ProcErrorNotifyReq
(   IN CDecoderMgrEvent& objEvent)
{
    IVS_DBG_LOG("Process Error Notify Request Message Begin.");

    //����Ƿ��Ѿ���ʼ��
    if (!CheckIsInited())
    {
        IVS_RUN_LOG_ERR( "Process Error Notify Request Message Failed. Not Init yet.");
        return IVS_FAIL;
    }

    //����������Ϣ
    CRDAErrorNotifyReq objRDAReq;
    long lResult = m_pRDAProtStack->DecodeMsg(m_pRecvBuffer, MAX_RDAMSG_LEN, objRDAReq);
    if (VOS_SUCCESS != lResult)
    {
        IVS_RUN_LOG_ERR( "Decode Message Failed On Process Error Notify Request Message.");
        return IVS_FAIL;
    }

    //��ӡ��Ϣ
    m_pRDAProtStack->Print(&objRDAReq);

    //��䷵�ظ�����������Ӧ��Ϣ��stRDAHeartBeat
    CRDAErrorNotifyRsp objRDAResp;
    objRDAResp.m_stMsgHead.usPackID = objRDAReq.m_stMsgHead.usPackID;
    if(!CToolsHelp::Strncpy(objRDAResp.m_szDecoderID, sizeof(objRDAResp.m_szDecoderID), objRDAReq.m_szDecoderID, sizeof(objRDAResp.m_szDecoderID) - 1))
	{
		IVS_RUN_LOG_ERR("ProcErrorNotifyReq Copy DecoderId error.");
		return IVS_ALLOC_MEMORY_ERROR;
	}
    objRDAResp.m_ulErrorCode = 0;
    const char* pErrorDescription = "���ճɹ�";
    if(!CToolsHelp::Strncpy(objRDAResp.m_szErrorDescription, sizeof(objRDAResp.m_szErrorDescription), pErrorDescription, sizeof(objRDAResp.m_szErrorDescription)-1))
	{
		IVS_RUN_LOG_ERR("ProcErrorNotifyReq Copy ErrorDescription error.");
		return IVS_ALLOC_MEMORY_ERROR;
	}
    IVS_ULONG ulValidLength = 0;
    lResult = m_pRDAProtStack->EncodeMsg(m_pSendBuffer, MAX_RDAMSG_LEN, &objRDAResp, ulValidLength);
    if (VOS_SUCCESS != lResult)
    {
        IVS_RUN_LOG_ERR( "[ DecoderID = %.20s ] Encode Message Failed On Process Error Notify Request Message.",
                objRDAReq.m_szDecoderID);
        return IVS_FAIL;
    }

    //IVS_RUN_LOG_INF("[ DecoderID = %.20s ] Process Error Notify Request Message: DecoderID = %.20s, ChannelNo = %u, ErrorCode = %u, ErrorDescription = %s", 
    //        objRDAReq.m_szDecoderID, objRDAReq.m_usChannelNo, objRDAReq.m_ulErrorCode, objRDAReq.m_szErrorDescription);

    //������Ӧ��Ϣ
    CDecoderConnHandle* pDecoderConnHandle = objEvent.m_pDecoderConnHandle;
    if (NULL == pDecoderConnHandle)
    {
        IVS_RUN_LOG_ERR(
                "[ DecoderID = %.20s ] Process Error Notify Request Message Failed. Decoder Connection Handle is Emtpy.",
                objRDAReq.m_szDecoderID);
        return IVS_FAIL;
    }

    lResult = pDecoderConnHandle->send(m_pSendBuffer, ulValidLength, enSyncOp);
    if (lResult < 0)
    {
        IVS_RUN_LOG_ERR(
                "[ DecoderID = %.20s ] Send Response Message Failed On Process Error Notify Request Message.",
                objRDAReq.m_szDecoderID);

        //return IVS_FAIL;
    }

    //֪ͨ�ϲ�
    CUMW_NOTIFY_TYPE_DECODER_ERROR_INFO* pstErrorInfo = VOS_NEW(pstErrorInfo);
    CUMW_NOTIFY_INFO* pstNotifyInfo = VOS_NEW(pstNotifyInfo);

    //pstErrorInfo->ulBusinessID = objRDAReq.m_ulBusinessID;
    pstErrorInfo->stErrorCode.ulErrorCode = objRDAReq.m_ulErrorCode;
    pstErrorInfo->usChannelNo = objRDAReq.m_usChannelNo;
    if(!CToolsHelp::Strncpy(pstErrorInfo->stErrorCode.szErrorDescription, sizeof(pstErrorInfo->stErrorCode.szErrorDescription), objRDAReq.m_szErrorDescription, sizeof(pstErrorInfo->stErrorCode.szErrorDescription)- 1))
	{
		IVS_RUN_LOG_ERR("ProcErrorNotifyReq Copy ErrorDescription error.");
        VOS_DELETE(pstErrorInfo);
        VOS_DELETE(pstNotifyInfo);
		return IVS_ALLOC_MEMORY_ERROR;
	}
    if(!CToolsHelp::Strncpy(pstErrorInfo->szDecoderID, sizeof(pstErrorInfo->szDecoderID), objRDAReq.m_szDecoderID, sizeof(pstErrorInfo->szDecoderID) - 1))
	{
		IVS_RUN_LOG_ERR("ProcErrorNotifyReq Copy DecoderId error.");
        VOS_DELETE(pstErrorInfo);
        VOS_DELETE(pstNotifyInfo);
		return IVS_ALLOC_MEMORY_ERROR;
	}

    pstNotifyInfo->ulNotifyType = CUMW_NOTIFY_TYPE_DECODER_ERROR;
    pstNotifyInfo->pNotifyInfo = (void*)pstErrorInfo;
    pstNotifyInfo->ulNotifyInfoLen = sizeof(CUMW_NOTIFY_INFO);

    //���ûص�����
    (void)m_objStatusNotifyCallBack.NotifyCallBack(*pstNotifyInfo);

    VOS_DELETE(pstErrorInfo);
    VOS_DELETE(pstNotifyInfo);

    IVS_DBG_LOG("[ DecoderID = %.20s ] Process Error Notify Request Message Success.", objRDAReq.m_szDecoderID);
    return IVS_SUCCEED;
}

LONG CRemoteDecoderReqProc::ProcRegisterMsg(CDecoderMgrEvent& objEvent)
{
    IVS_DBG_LOG("Process Decoder Register Message Begin.");
	bool bSucc = false;
    //����Ƿ��Ѿ���ʼ��
    if (!CheckIsInited())
    {    
        IVS_RUN_LOG_ERR( "Process Decoder Register Message Failed. Not Init yet.");
        return IVS_FAIL;
    }

    //���ս���ע��������Ϣ
    CRDARegisteReq objRDAReq;
    long lResult = m_pRDAProtStack->DecodeMsg(m_pRecvBuffer, MAX_RDAMSG_LEN, objRDAReq);     
    if (VOS_SUCCESS != lResult)
    {
        IVS_RUN_LOG_ERR( "Decode Message Failed On Process Decoder Register Message Failed.");
        return IVS_FAIL;
    }

    CUMW_DECODER_INFO stDecoderInfo = {0};

    //��ȡע����Ϣ�еĽ�������Ϣ
    lResult = GetDecoderInfoFromRegisterReq(objRDAReq, stDecoderInfo);    
    if(VOS_SUCCESS != lResult)
    {
        IVS_RUN_LOG_ERR( "Get Decoder Info From Register Request Message Failed On Process Decoder Register Message.");
        return IVS_FAIL;
    }    

    //��������н�����,��ת������������
    if (0 == memcmp(stDecoderInfo.szDecoderEncoding, "UTF-8", strlen("UTF-8")))
    {
        char szDecoderName[CUMW_DECODER_NAME_LEN + 1] = {0};
        wchar_t szUnicodeBuffer[128] = {0};

        //��utf8ת��Ϊunicode
        int len = ::MultiByteToWideChar(CP_UTF8, 0, stDecoderInfo.szDecoderName, -1, NULL, 0);
        if (len == 0) 
        {
            return IVS_FAIL;
        }
        ::MultiByteToWideChar(CP_UTF8, 0, stDecoderInfo.szDecoderName, -1, szUnicodeBuffer, len);

        //��unicodeת��Ϊansi
        (void)WideCharToMultiByte(CP_ACP,0,szUnicodeBuffer,-1,szDecoderName,CUMW_DECODER_NAME_LEN,NULL,NULL);

        memset(stDecoderInfo.szDecoderName, 0, CUMW_DECODER_NAME_LEN + 1);
        bSucc = CToolsHelp::Memcpy(stDecoderInfo.szDecoderName, CUMW_DECODER_NAME_LEN, szDecoderName, CUMW_DECODER_NAME_LEN);
		CHECK_STRNCPY_RET(bSucc, IVS_ALLOC_MEMORY_ERROR, "Process Decoder Register Message Copy DecoderName failed");

        memset(stDecoderInfo.szDecoderEncoding, 0, CUMW_DECODER_ENCODING_LEN + 1);
        bSucc = CToolsHelp::Memcpy(stDecoderInfo.szDecoderEncoding, CUMW_DECODER_ENCODING_LEN, "ANSI", strlen("ANSI"));
		CHECK_STRNCPY_RET(bSucc, IVS_ALLOC_MEMORY_ERROR,"Process Decoder Register Message Copy DecoderEncoding failed");
    }

    //��ӡ��ʾ��Ϣ����
    m_pRDAProtStack->Print(&objRDAReq);

    //�жϽ�����ID�Ƿ��ظ�������ظ���Ͽ�����
    CUMW_DECODER_INFO stConflictDecoderInfo = {0};

    //����ܹ��Ҵ��Ѿ����ڵĽ�����ID����Ͽ�����
    lResult = m_pRemoteDecoderArry->GetDecoderInfo(stDecoderInfo.szDecoderID, stConflictDecoderInfo);
    if (VOS_SUCCESS == lResult)
    { 
        IVS_RUN_LOG_ERR( "Process Decoder Register Message Failed. DecoderID(%.20s) Already Exist.", stDecoderInfo.szDecoderID);
        return IVS_FAIL;       
    }

    //��䷵�ظ�����������Ӧ��Ϣ��CRDARegisteRsp
    CRDARegisteRsp objRDAResp;    
    objRDAResp.m_stMsgHead.usPackID = objRDAReq.m_stMsgHead.usPackID;
    bSucc = CToolsHelp::Strncpy(objRDAResp.m_szDecoderID, sizeof(objRDAResp.m_szDecoderID), stDecoderInfo.szDecoderID, sizeof(objRDAResp.m_szDecoderID) - 1);
	CHECK_STRNCPY_RET(bSucc, IVS_ALLOC_MEMORY_ERROR, "ProcRegisterMsg Copy DecoderId error.");
    objRDAResp.m_ulErrorCode = RDA_ERROR_CODE_OK;
    bSucc = CToolsHelp::Strncpy(objRDAResp.m_szErrorDescription, sizeof(objRDAResp.m_szErrorDescription), "register success.", sizeof(objRDAResp.m_szErrorDescription) - 1);
	CHECK_STRNCPY_RET(bSucc, IVS_ALLOC_MEMORY_ERROR, "ProcRegisterMsg Copy ErrorDescription error.");

    IVS_ULONG ulValidLength = 0;
    lResult = m_pRDAProtStack->EncodeMsg(m_pSendBuffer, MAX_RDAMSG_LEN, &objRDAResp, ulValidLength);
    if (VOS_SUCCESS != lResult)
    {
        IVS_RUN_LOG_ERR( "[ DecoderID = %.20s ] Encode Message Failed On Process Decoder Register Message.", stDecoderInfo.szDecoderID);
        return IVS_FAIL;
    }

    m_pRDAProtStack->Print(&objRDAResp);

    //���¼���ȡ���Ӿ��
    CDecoderConnHandle* pDecoderConnHandle = objEvent.m_pDecoderConnHandle;
    if (NULL == pDecoderConnHandle)
    {
        IVS_RUN_LOG_ERR( "[ DecoderID = %.20s ] Process Response Decoder Register Message Failed. Decoder Connection Handle is Empty.", stDecoderInfo.szDecoderID);
        return IVS_FAIL;
    }

    CNetworkAddr& objPeerAddr = pDecoderConnHandle->m_peerAddr;
    //��IPת��Ϊ�ַ���
    char* pszIP = inet_ntoa((in_addr&)objPeerAddr.m_lIpAddr);

    //����������Ϣ�е�ip
    bSucc = CToolsHelp::Strncpy(stDecoderInfo.szDecoderIP, sizeof(stDecoderInfo.szDecoderIP), pszIP, sizeof(stDecoderInfo.szDecoderIP) - 1);
	CHECK_STRNCPY_RET(bSucc, IVS_ALLOC_MEMORY_ERROR, "Decode Message Copy DecoderIP error.");

    IVS_DBG_LOG("[ DecoderConnHandle = %#p ][ DecoderID = %.20s ] Process Decoder Register Message In. Will Send Response To %.15s:%d.", 
        pDecoderConnHandle, stDecoderInfo.szDecoderID, pszIP, ntohs(objPeerAddr.m_usPort));

    //������Ӧ��Ϣ
    lResult = pDecoderConnHandle->send(m_pSendBuffer, ulValidLength, enSyncOp);
    if (lResult < 0)
    {
        IVS_RUN_LOG_ERR( "[ DecoderID = %.20s ] Send Response Message Failed On Process Decoder Register Message.", stDecoderInfo.szDecoderID);
        return IVS_FAIL;
    }

    IVS_RUN_LOG_INF("[ DecoderID = %.20s ] Process Decoder Register Message In. Send Response Success.", stDecoderInfo.szDecoderID);

    //ȡ�����ӳ�ʱ���
    pDecoderConnHandle->CancelTimeOut();

    if (!pDecoderConnHandle->m_bNeedUse)
    {
        IVS_RUN_LOG_ERR( "[ DecoderID = %.20s ] Process Decoder Register Message Failed. DecoderConnHandle(%#p) Has been Released.", stDecoderInfo.szDecoderID, pDecoderConnHandle);
        return IVS_FAIL;
    }

    //����remotedecoder����
    CRemoteDecoder* pRemoteDecoder = m_pRemoteDecoderArry->GetRemoteDecoder();
    if (NULL == pRemoteDecoder)
    {
        IVS_RUN_LOG_ERR( "[ DecoderID = %.20s ] Process Decoder Register Message Failed. No Idle Decoder Object.", stDecoderInfo.szDecoderID);
        return IVS_FAIL;
    }

    bSucc = CToolsHelp::Memcpy(&(pRemoteDecoder->m_stDecoderInfo), sizeof(CUMW_DECODER_INFO), &stDecoderInfo, sizeof(CUMW_DECODER_INFO));
	CHECK_STRNCPY_RET(bSucc, IVS_ALLOC_MEMORY_ERROR, "Process Decoder Register Message Copy DecoderInfo failed")

    pRemoteDecoder->m_pDecoderConnHandle = pDecoderConnHandle;
    pDecoderConnHandle->m_pRemoteDecoder = pRemoteDecoder;

    IVS_RUN_LOG_INF("[ DecoderConnHandle = %#p ][ DecoderID = %.20s ] Process Decoder Register Message In. Will Notify Decoder Status.", 
        pDecoderConnHandle, stDecoderInfo.szDecoderID);    
    //���ûص�����
    (void)m_objStatusNotifyCallBack.NotifyDecoderRegister(stDecoderInfo);

    m_pRemoteDecoderArry->PrintDecoderInfo(*pRemoteDecoder);

    IVS_DBG_LOG("[ DecoderConnHandle = %#p ][ DecoderID = %.20s ] Process Decoder Register Message Success.", 
        pDecoderConnHandle, stDecoderInfo.szDecoderID);    
    return IVS_SUCCEED;
}

LONG CRemoteDecoderReqProc::ProcUnRegisterMsg(CDecoderMgrEvent* pEventRecv)
{
    IVS_DBG_LOG("ProcUnRegisterMsg Start. ");

    //�������
    if (NULL == pEventRecv)
    {
        IVS_RUN_LOG_ERR( "ProcUnRegisterMsg End, param is NULL");
        return VOS_FAIL;
    }

    //����Ƿ��Ѿ���ʼ��
    if (!CheckIsInited())
    {
        IVS_RUN_LOG_ERR( "ProcUnRegisterMsg End, Not Inited.");
        return VOS_FAIL;
    }

    CRDAUnregisterReq objRDAMsgReq;
    LONG lRet = m_pRDAProtStack->DecodeMsg(m_pRecvBuffer, MAX_RDAMSG_LEN, objRDAMsgReq);
    if (VOS_SUCCESS != lRet)
    {
        IVS_RUN_LOG_ERR( "ProcUnRegisterMsg End, "
                "Fail to decode CRDAUnregisterReq.EventRecv(0x%x)", pEventRecv);
        return VOS_FAIL;
    }

    IVS_DBG_LOG("ProcUnRegisterMsg End, "
            "recv and decode CRDAUnregisterReq successfully. Event(0x%x)", pEventRecv);

    m_pRDAProtStack->Print(&objRDAMsgReq);

    CDecoderConnHandle* pDecoderConnHandle = pEventRecv->m_pDecoderConnHandle;

    //�Ͽ����Ӻ���������еĽ���������
    RemoteDecoderDisConnect(pDecoderConnHandle, TRUE);

    IVS_DBG_LOG("ProcUnRegisterMsg End, "
            "release DecoderConnHandle(0x%x), pszDecoderID(%.20s), EventRecv(0x%x).",
            pDecoderConnHandle, objRDAMsgReq.m_szDecoderID, pEventRecv);

    return RDA_ERROR_DISCONNECT;
}

LONG CRemoteDecoderReqProc::ProcSyncRspMsg( CDecoderMgrEvent* pEventRecv)
{
    IVS_DBG_LOG("ProcSyncRspMsg Start. ");

    //����Ƿ��Ѿ���ʼ��
    if (!CheckIsInited())
    {
        IVS_RUN_LOG_ERR( "ProcSyncRspMsg End, Not Inited.");
        return VOS_FAIL;
    }

    ;

    //�������
    if (NULL == pEventRecv)
    {
        IVS_RUN_LOG_ERR( "ProcSyncRspMsg End, param is NULL.");
        return VOS_FAIL;
    }

    //���¼��ڵ������Ӿ��
    CDecoderConnHandle* pDecoderConnHandle = pEventRecv->m_pDecoderConnHandle;
    if (NULL == pDecoderConnHandle)
    {
        IVS_RUN_LOG_ERR( "ProcSyncRspMsg End,"
                "pEventRecv->m_pDecoderConnHandle is NULL");
        return VOS_FAIL;
    }

    if (NULL != pDecoderConnHandle->m_pRemoteDecoder)
    {
        CRemoteDecoder* pRemoteDecoder = pDecoderConnHandle->m_pRemoteDecoder;
        char pszDecoderID[CUMW_DECODER_ID_LEN + 1] = {0};
        if(!CToolsHelp::Strncpy(pszDecoderID, sizeof(pszDecoderID), pRemoteDecoder->m_stDecoderInfo.szDecoderID, sizeof(pszDecoderID)-1))
		{
			IVS_RUN_LOG_ERR("ProcSyncRspMsg Copy DecoderID error.");
			return IVS_ALLOC_MEMORY_ERROR;
		}
        //��������������
        m_pRemoteDecoderArry->ClearHeartBeatCount(pszDecoderID);
    }

    RDAMsgHead stRDAMsgHead;
    (void)m_pRDAProtStack->GetMsgHeadFromBuff(m_pRecvBuffer,
                                              LEN_RDA_MSG_HEAD, stRDAMsgHead);

    m_ulRecvBuffLen = stRDAMsgHead.ulPackLength;

    USHORT usTransactionNo = stRDAMsgHead.usPackID;

    //�ݰ����(��������)���Ұ�
    CRDAPacket* pRDAPacket = m_pRDAPacketMgr->FindRDAPacket(usTransactionNo);
    if (NULL == pRDAPacket)
    {
        IVS_RUN_LOG_ERR( "ProcSyncRspMsg End, "
                "find RDAPacket fail, usTransactionNo is %d.", usTransactionNo );
        return VOS_FAIL;
    }

    //��������
    if(!CToolsHelp::Memcpy(pRDAPacket->m_pRecvBuff, MAX_RDAMSG_LEN, m_pRecvBuffer, m_ulRecvBuffLen))
	{
		IVS_RUN_LOG_ERR("ProcSyncRspMsg Copy RecvBuffer failed");
		return IVS_ALLOC_MEMORY_ERROR;
	}
    pRDAPacket->m_ulRecvBuffLen = m_ulRecvBuffLen;

    //֪ͨ�ȴ��̶߳�ȡ��������
    (void)pRDAPacket->SetReadPacketEv();

    IVS_DBG_LOG("ProcSyncRspMsg End, "
            "proc sync rsp msg success, EventRecv(0x%x).", pEventRecv);
    return VOS_SUCCESS;
}

void CRemoteDecoderReqProc::RemoteDecoderDisConnect(CDecoderConnHandle *pDecoderConnHandle, BOOL bUnRigister)
{
    IVS_DBG_LOG("RemoteDecoderDisConnect Start, ");

    //����Ƿ��Ѿ���ʼ��
    if (!CheckIsInited())
    {
        IVS_RUN_LOG_ERR( "RemoteDecoderDisConnect End, Not Inited.");
        return;
    }

    if (NULL == pDecoderConnHandle)
    {
        IVS_RUN_LOG_ERR( "RemoteDecoderDisConnect End, "
                "pEventRecv->m_pDecoderConnHandle is NULL");
        return;
    }

    if (NULL != pDecoderConnHandle->m_pRemoteDecoder)
    {
        //��ȡ��������Ϣ
        CRemoteDecoder *pRemoteDecoder = pDecoderConnHandle->m_pRemoteDecoder;
        CUMW_DECODER_INFO *pstDecoderInfo = &(pRemoteDecoder->m_stDecoderInfo);

        if (TRUE == bUnRigister)
        {
            //�ص���֪ͨ�ϲ㣬������ע��
            CUMW_NOTIFY_REMOTE_DECODER_OFF_LINE_INFO stInfo = {0};
            if(!CToolsHelp::Strncpy(stInfo.szDecoderID, sizeof(stInfo.szDecoderID), pstDecoderInfo->szDecoderID, sizeof(stInfo.szDecoderID) - 1))
			{
				IVS_RUN_LOG_ERR("RemoteDecoderDisConnect Copy DecoderID error.");
				return ;
			}
            IVS_RUN_LOG_INF("[ DecoderConnHandle = %#p ] Will Notify Decoder Unrigister.", pDecoderConnHandle);
            (void)m_objStatusNotifyCallBack.NotifyDecoderUnRegister(stInfo);
        }
        else
        {
            //�ص���֪ͨ�ϲ㣬����������
            CUMW_NOTIFY_REMOTE_DECODER_OFF_LINE_INFO stInfo = {0};
            if(!CToolsHelp::Strncpy(stInfo.szDecoderID, sizeof(stInfo.szDecoderID), pstDecoderInfo->szDecoderID, sizeof(stInfo.szDecoderID) - 1))
			{
				IVS_RUN_LOG_ERR("RemoteDecoderDisConnect Copy DecoderID error.");
				return ;
			}
            IVS_RUN_LOG_INF("[ DecoderConnHandle = %#p ] Will Notify Decoder Offline.", pDecoderConnHandle);

            //���ûص�����
            (void)m_objStatusNotifyCallBack.NotifyDecoderDisConnect(stInfo);
        }

        IVS_DBG_LOG("In RemoteDecoderDisConnect, "
                "pDecoderConnHandle(0x%x) pszDecoderID(%.20s).",
                pDecoderConnHandle, pstDecoderInfo->szDecoderID);

        //�ͷŽ�������Դ
        m_pRemoteDecoderArry->ReleaseRemoteDecoder(pRemoteDecoder);
        pDecoderConnHandle->m_pRemoteDecoder = NULL;
    }

    //�ͷ�������Դ
    //ɾ��tcp���Ӿ��,�˴�ֻ����ͼȥɾ�����Ӿ����
    //ֻ�е�ͬʱ������������ʱ��������ɾ��,
    //(1) m_bNeedUse == VOS_FALSE (2) ���ü���Ϊ0
    pDecoderConnHandle->m_bNeedUse = VOS_FALSE;
    m_pDecoderConnHandleMgr->ReleaseConnHandle(pDecoderConnHandle);

    IVS_DBG_LOG("RemoteDecoderDisConnect End. ");
    return;
}

long CRemoteDecoderReqProc::GetDecoderInfoFromRegisterReq
(   IN CRDARegisteReq&      ojRegisterReq,
    OUT CUMW_DECODER_INFO&  stDecoderInfo) const
{
    //ǿ�н������Ϊ��������������ID����Ϊ20λ
    ojRegisterReq.m_szDecoderID[sizeof(ojRegisterReq.m_szDecoderID) - 1] = 0;
    if (CUMW_DECODER_ID_LEN != strlen(ojRegisterReq.m_szDecoderID))
    {
        IVS_RUN_LOG_ERR( "Decoder Register Request Message Invalid. DecoderID(%.20s) Invalid.",
                ojRegisterReq.m_szDecoderID);
        return IVS_FAIL;
    }

    //��ȡ������id
    if(!CToolsHelp::Strncpy(stDecoderInfo.szDecoderID, sizeof(stDecoderInfo.szDecoderID), ojRegisterReq.m_szDecoderID, sizeof(stDecoderInfo.szDecoderID) - 1))
	{
		IVS_RUN_LOG_ERR("Decoder Register Request Message Copy DecoderId error.");
		return IVS_ALLOC_MEMORY_ERROR;
	}

    //��ȡ����������
    if(!CToolsHelp::Strncpy(stDecoderInfo.szDecoderName, sizeof(stDecoderInfo.szDecoderName), ojRegisterReq.m_szDecoderName, sizeof(stDecoderInfo.szDecoderName) - 1))
	{
		IVS_RUN_LOG_ERR("Decoder Register Request Message Copy DecoderName error.");
		return IVS_ALLOC_MEMORY_ERROR;
	}

	//��ȡ�����������ʽ
	if(!CToolsHelp::Strncpy(stDecoderInfo.szDecoderEncoding, sizeof(stDecoderInfo.szDecoderEncoding), ojRegisterReq.m_szDecoderEncoding, sizeof(stDecoderInfo.szDecoderEncoding) - 1))
	{
		IVS_RUN_LOG_ERR("Decoder Register Request Message Copy DecoderEncoding error.");
		return IVS_ALLOC_MEMORY_ERROR;
	}

    //���ý�����״̬
    stDecoderInfo.ulDecoderStatus = CUMW_NOTIFY_TYPE_REMOTE_DECODER_REGISTER;

    //��ȡ������ͨ����Ϣ
    const RDA_CHANNELS_INFO& stChannelsInfo = ojRegisterReq.m_stChannelsInfo;
    stDecoderInfo.ulChannelNum = stChannelsInfo.usChannelNum;
    if (MAX_CHANNEL_NUM < stDecoderInfo.ulChannelNum)
    {
        IVS_RUN_LOG_ERR(
                "Decoder Register Request Message Invalid. ChannelNum(%u) is Too Large. Only Use First %u.",
                stChannelsInfo.usChannelNum, MAX_CHANNEL_NUM);
        stDecoderInfo.ulChannelNum = MAX_CHANNEL_NUM;
    }

    unsigned long ulIndex = 0;

    for (ulIndex = 0; ulIndex < stDecoderInfo.ulChannelNum; ++ulIndex)
    {
        CUMW_DECODER_CHANNEL_INFO& stCurChannelInfoOut = stDecoderInfo.stChannelInfoArray[ulIndex];
        const RDA_CHANNEL_INFO&    stCurChannelInfoIn = stChannelsInfo.stChannelInfo[ulIndex];

        stCurChannelInfoOut.ulChannelID = stCurChannelInfoIn.usChannelNo;
        if(!CToolsHelp::Strncpy(stCurChannelInfoOut.szChannelName, sizeof(stCurChannelInfoOut.szChannelName), 
								stCurChannelInfoIn.szChannelName, sizeof(stCurChannelInfoOut.szChannelName) - 1))
		{
			IVS_RUN_LOG_ERR("Decoder Register Request Message Copy ChannelName error.");
			return IVS_ALLOC_MEMORY_ERROR;
		}
		stCurChannelInfoOut.ucPortMode = 1;
		stCurChannelInfoOut.ucChannelType = 0;
    }

    stDecoderInfo.ulVideoPayloadTypeNum = ojRegisterReq.m_stVideoPayloadTypes.ucPayloadTypeNum;
    if (CUMW_PAY_LOAD_TYPE_NUM < stDecoderInfo.ulVideoPayloadTypeNum)
    {
        stDecoderInfo.ulVideoPayloadTypeNum = CUMW_PAY_LOAD_TYPE_NUM;
    }

    for (ulIndex = 0; ulIndex < stDecoderInfo.ulVideoPayloadTypeNum; ++ulIndex)
    {
        stDecoderInfo.ucVideoPayloadTypeArray[ulIndex] = ojRegisterReq.m_stVideoPayloadTypes.stPayloadTypeArry[ulIndex];
    }

    stDecoderInfo.ulAudioPayloadTypeNum = ojRegisterReq.m_stAudioPayloadTypes.ucPayloadTypeNum;
    if (CUMW_PAY_LOAD_TYPE_NUM < stDecoderInfo.ulAudioPayloadTypeNum)
    {
        stDecoderInfo.ulAudioPayloadTypeNum = CUMW_PAY_LOAD_TYPE_NUM;
    }

    for (ulIndex = 0; ulIndex < stDecoderInfo.ulAudioPayloadTypeNum; ++ulIndex)
    {
        stDecoderInfo.ucAudioPayloadTypeArray[ulIndex] = ojRegisterReq.m_stAudioPayloadTypes.stPayloadTypeArry[ulIndex];
    }

    return IVS_SUCCEED;
}

LONG CCheckHeartBeatTrigger::Init(CDecoderMgrEventQueue* pEventQ)
{
    m_pEventQ = pEventQ;

    IVS_DBG_LOG(
            "CCheckHeartBeatTrigger::Init,init CCheckHeartBeatTrigger success");

    return VOS_SUCCESS;
}

void CCheckHeartBeatTrigger::onTrigger(void* pArg, ULONGLONG ullScales,
                                       TriggerStyle style)
{
    style = style;
    ullScales = ullScales;
    pArg = pArg;

    CDecoderMgrEvent* pEvent = NULL;
    (void)VOS_NEW(pEvent);
    if (VOS_NULL == pEvent)
    {
        IVS_RUN_LOG_ERR( "CCheckHeartBeatTrigger::onTrigger, "
                "new pEvent fail.");
        return;
    }

    pEvent->m_ulEventType = RDA_PACKETMGR_EVENT_CHECK_RD_INVALID;

    IVS_DBG_LOG("put check heartbeat event(0x%x, type %lu) "
            "to event quene sucess ", pEvent, pEvent->m_ulEventType);

    //�����¼�����
    LONG lRetValue = m_pEventQ->pushBackEv(pEvent);
    if (VOS_OK != lRetValue)
    {
        IVS_RUN_LOG_ERR("CCheckHeartBeatTrigger::onTrigger, "
                "pushBackEv fail, Event(0x%x), EventType(%lu)",
                pEvent, pEvent->m_ulEventType);
        VOS_DELETE(pEvent);
        return;
    }

    return;
}

// Default constructor
CRemoteDecoderMgr::CRemoteDecoderMgr() : m_pVosThread(NULL)
	,m_bExit(VOS_FALSE)
	,m_pDecoderMgrEventQ(NULL)
	,m_pDecoderMgrServerHandle(NULL)
	,m_pDecoderConnHandleMgr(NULL)
	,m_pRemoteDecoderReqProc(NULL)
	,m_pRDAPacketMgr(NULL)
	,m_pRemoteDecoderArry(NULL)
	,m_pCheckHeartBeatTrigger(NULL)
	,m_usTransNo(0)
	,m_pRDAProtStack(NULL)
	,m_pSendBuffer  (NULL)
	,m_ulSendBuffLen(0)
{

}

//Destructor
CRemoteDecoderMgr::~CRemoteDecoderMgr()
{
    m_pVosThread = NULL;
    m_pDecoderMgrEventQ = NULL;
    m_pDecoderMgrServerHandle = NULL;
    m_pDecoderConnHandleMgr = NULL;
    m_pRemoteDecoderReqProc = NULL;
    m_pRDAPacketMgr = NULL;
    m_pRemoteDecoderArry = NULL;
    m_pCheckHeartBeatTrigger = NULL;
    m_pRDAProtStack = NULL;
    m_pSendBuffer = NULL;
}

BOOL CRemoteDecoderMgr::CheckIsInited()
{
    IVS_DBG_LOG("CheckIsInited Start.");

    //����¼�����ָ��
    if (NULL == m_pDecoderMgrEventQ)
    {
        IVS_RUN_LOG_ERR( "CheckIsInited End . m_pDecoderMgrEventQ is NULL.");
        return FALSE;
    }

    //�����������ָ��
    if (NULL == m_pDecoderMgrServerHandle)
    {
        IVS_RUN_LOG_ERR( "CheckIsInited End . m_pDecoderMgrServerHandle is NULL.");
        return FALSE;
    }

    //���ͻ���������Դ����ָ��
    if (NULL == m_pDecoderConnHandleMgr)
    {
        IVS_RUN_LOG_ERR( "CheckIsInited End . m_pDecoderConnHandleMgr is NULL.");
        return FALSE;
    }

    //���Զ�̽�������Ϣ������ָ��
    if (NULL == m_pRemoteDecoderReqProc)
    {
        IVS_RUN_LOG_ERR( "CheckIsInited End . m_pRemoteDecoderReqProc is NULL.");
        return FALSE;
    }

    //����������ָ��
    if (NULL == m_pRDAPacketMgr)
    {
        IVS_RUN_LOG_ERR( "CheckIsInited End . m_pRDAPacketMgr is NULL.");
        return FALSE;
    }

    //���Զ�̽�������Դ����ָ��
    if (NULL == m_pRemoteDecoderArry)
    {
        IVS_RUN_LOG_ERR( "CheckIsInited End . m_pRemoteDecoderArry is NULL.");
        return FALSE;
    }

    //���������ݻ�����ָ��
    if (NULL == m_pSendBuffer)
    {
        IVS_RUN_LOG_ERR( "CheckIsInited End . m_pSendBuffer is NULL.");
        return FALSE;
    }

    //���Զ�̽�����Э��ջָ��
    if (NULL == m_pRDAProtStack)
    {
        IVS_RUN_LOG_ERR( "CheckIsInited End . m_pRDAProtStack is NULL.");
        return FALSE;
    }

    IVS_DBG_LOG("CheckIsInited End. ");
    return TRUE;
} /*lint !e1762*/ //�˴����趨���const����������ÿ�����ô˺����ĵط������������Ӧ��PCLint����

long CRemoteDecoderMgr::InitDecoderMgrEventQ()
{
    IVS_DBG_LOG("Enter CRemoteDecoderMgr::InitDecoderMgrEventQ().");
    long lRet = IVS_SUCCEED;

    //��ʼ����Ϣ����
    (void)VOS_NEW(m_pDecoderMgrEventQ);
    if (NULL == m_pDecoderMgrEventQ)
    {
        IVS_RUN_LOG_ERR( "CRemoteDecoderMgr::InitDecoderMgrEventQ(),"
                " new m_pDecoderMgrEventQ fail.");
        return IVS_FAIL;
    }

    lRet = m_pDecoderMgrEventQ->init(MAX_RDMGR_EVENT_NUM);
    if (VOS_OK != lRet)
    {
        IVS_RUN_LOG_ERR( "CRemoteDecoderMgr::InitDecoderMgrEventQ(),"
                "init m_pDecoderMgrEventQ fail.");
        return VOS_FAIL;
    }

    IVS_DBG_LOG(
            "Exit CRemoteDecoderMgr::InitDecoderMgrEventQ()lRet:%ld.", lRet);
    return lRet;
}

long CRemoteDecoderMgr::InitDecoderMgrServerHandle()
{
    IVS_DBG_LOG("InitDecoderMgrServerHandle Start.");
    long lRet = IVS_SUCCEED;

    //BEGIN R001C05LYYS01 MOD 2010-03-08 ligengqiang 00124479 for ��������״̬��
    //�������
    if (NULL == m_pDecoderConnHandleMgr)
    {
        IVS_RUN_LOG_ERR( "InitDecoderMgrServerHandle End, m_pDecoderConnHandleMgr is NULL");
        return IVS_FAIL;
    }

    //���ñ���IP ��ַ��˿ں�
    CNetworkAddr localAddres;
    localAddres.m_lIpAddr = 0; //���԰�������е�����

    //�������״̬��ʵ��
    CStatusConfig& objStatusConfig = CStatusSingleton::Instance().GetConfig();

    localAddres.m_usPort = objStatusConfig.GetRemoteDecoderListenPort();
    //END   R001C05LYYS01 MOD 2010-03-08 ligengqiang 00124479 for ��������״̬��
    localAddres.m_usPort = htons(localAddres.m_usPort);

    //BEGIN R001C05LITS01 MOD 2010-06-11 ligengqiang 00124479 for ���15λIP��������
    //��IPת��Ϊ�ַ���
    char* pszIP = inet_ntoa((in_addr &)localAddres.m_lIpAddr);

    //��ӡ���˿ںųɹ���־
    IVS_RUN_LOG_INF("InitDecoderMgrServerHandle End, IP is %.15s, ListenPort: %d.",
            pszIP, ntohs(localAddres.m_usPort));
    //END   R001C05LITS01 MOD 2010-06-11 ligengqiang 00124479 for ���15λIP��������

    //��ʼ��TCP������
    (void)VOS_NEW(m_pDecoderMgrServerHandle);
    if (NULL == m_pDecoderMgrServerHandle)
    {
        IVS_RUN_LOG_ERR( "InitDecoderMgrServerHandle End, "
                "new m_pDecoderMgrServerHandle fail.");
        return IVS_FAIL;
    }

    lRet = m_pDecoderMgrServerHandle->Init(m_pDecoderConnHandleMgr);
    if (VOS_SUCCESS != lRet)
    {
        IVS_RUN_LOG_ERR( "InitDecoderMgrServerHandle End, "
                "init m_pDecoderMgrServerHandle fail.");
        return IVS_FAIL;
    }

    //���ͨ�Ź�����ʵ��
    CConnMgr& objConnMgr = CStatusSingleton::Instance().GetConnMgr();

    //����tcp������
    lRet = objConnMgr.regTcpServer(&localAddres, m_pDecoderMgrServerHandle);
    if (VOS_SUCCESS != lRet)
    {
        IVS_RUN_LOG_ERR( "InitDecoderMgrServerHandle End, reg tcp server fail. ");
        return VOS_FAIL;
    }

    IVS_DBG_LOG("InitDecoderMgrServerHandle End. lRet:%ld.", lRet);
    return lRet;
}

long CRemoteDecoderMgr::InitDecoderConnHandleMgr()
{
    IVS_DBG_LOG("InitDecoderConnHandleMgr Start.");

    long lRet = IVS_SUCCEED;

    //��ʼ��������Դ����
    (void)VOS_NEW(m_pDecoderConnHandleMgr);
    if (NULL == m_pDecoderConnHandleMgr)
    {
        IVS_RUN_LOG_ERR( "InitDecoderConnHandleMgr End, "
                "new m_pDecoderConnHandleMgr fail.");
        return IVS_FAIL;
    }

    lRet = m_pDecoderConnHandleMgr->Init(m_pDecoderMgrEventQ);
    if (VOS_SUCCESS != lRet)
    {
        IVS_RUN_LOG_ERR( "InitDecoderConnHandleMgr End, "
                "init m_pDecoderConnHandleMgr fail.");
        return IVS_FAIL;
    }

    IVS_DBG_LOG("InitDecoderConnHandleMgr End, lRet:%ld.", lRet);
    return lRet;
}

long CRemoteDecoderMgr::InitRemoteDecoderReqProc()
{
    IVS_DBG_LOG("InitRemoteDecoderReqProc Start.");

    long lRet = IVS_SUCCEED;

    (void)VOS_NEW(m_pRemoteDecoderReqProc);
    if (NULL == m_pRemoteDecoderReqProc)
    {
        IVS_RUN_LOG_ERR( "InitRemoteDecoderReqProc End, "
                "new m_pRemoteDecoderReqProc fail.");
        return IVS_FAIL;
    }

    lRet = m_pRemoteDecoderReqProc->Init(HandleNotifyCallBack, this, m_pRDAPacketMgr,
                                         m_pDecoderConnHandleMgr, m_pRemoteDecoderArry, m_pRDAProtStack);
    if (VOS_SUCCESS != lRet)
    {
        IVS_RUN_LOG_ERR( "InitRemoteDecoderReqProc End, "
                "init m_pRemoteDecoderReqProc fail.");
        return IVS_FAIL;
    }

    IVS_DBG_LOG("InitRemoteDecoderReqProc End, lRet:%ld.", lRet);
    return lRet;
}

long CRemoteDecoderMgr::InitRDAPacketMgr()
{
    IVS_DBG_LOG("InitRDAPacketMgr Start.");

    long lRet = IVS_SUCCEED;

    (void)VOS_NEW(m_pRDAPacketMgr);
    if (NULL == m_pRDAPacketMgr)
    {
        IVS_RUN_LOG_ERR( "InitRDAPacketMgr End, new m_pRDAPacketMgr fail.");
        return IVS_FAIL;
    }

    lRet = m_pRDAPacketMgr->InitRDAPacketMgr();
    if (VOS_SUCCESS != lRet)
    {
        IVS_RUN_LOG_ERR( "InitRDAPacketMgr End, init m_pRDAPacketMgr fail.");
        return IVS_FAIL;
    }

    IVS_DBG_LOG("InitRDAPacketMgr End, lRet:%ld.", lRet);
    return lRet;
}

long CRemoteDecoderMgr::InitRemoteDecoderArry()
{
    IVS_DBG_LOG("InitRemoteDecoderArry Start.");

    long lRet = IVS_SUCCEED;

    (void)VOS_NEW(m_pRemoteDecoderArry);
    if (NULL == m_pRemoteDecoderArry)
    {
        IVS_RUN_LOG_ERR( "InitRemoteDecoderArry End, "
                "new m_pRemoteDecoderArry fail.");
        return IVS_FAIL;
    }

    lRet = m_pRemoteDecoderArry->Init(m_pDecoderMgrEventQ,
                                      m_pDecoderConnHandleMgr);
    if (VOS_SUCCESS != lRet)
    {
        IVS_RUN_LOG_ERR( "InitRemoteDecoderArry End, "
                "init m_pRemoteDecoderArry fail.");
        return IVS_FAIL;
    }

    IVS_DBG_LOG("InitRemoteDecoderArry End, lRet:%ld.", lRet);
    return lRet;
}

long CRemoteDecoderMgr::InitCheckHeartBeatTrigger()
{
    IVS_DBG_LOG("InitCheckHeartBeatTrigger Start.");

    long lRet = IVS_SUCCEED;

    //��ʼ����ʱ������
    (void)VOS_NEW(m_pCheckHeartBeatTrigger);
    if (NULL == m_pCheckHeartBeatTrigger)
    {
        IVS_RUN_LOG_ERR( "InitCheckHeartBeatTrigger End, "
                "new m_pCheckHeartBeatTrigger fail.");
        return IVS_FAIL;
    }

    lRet = m_pCheckHeartBeatTrigger->Init(m_pDecoderMgrEventQ);
    if (VOS_SUCCESS != lRet)
    {
        IVS_RUN_LOG_ERR( "InitCheckHeartBeatTrigger End, "
                "init m_pCheckHeartBeatTrigger fail.");
        return IVS_FAIL;
    }

    IVS_DBG_LOG("InitCheckHeartBeatTrigger End, lRet:%ld.", lRet);
    return lRet;
}

long CRemoteDecoderMgr::InitRDAProtStack()
{
    IVS_DBG_LOG("InitRDAProtStack Start.");

    long lRet = IVS_SUCCEED;

    (void)VOS_NEW(m_pRDAProtStack);
    if (NULL == m_pRDAProtStack)
    {
        IVS_RUN_LOG_ERR( "InitRDAProtStack End, fail to new m_pRDAProtStack.");
        return IVS_FAIL;
    }

    //��ʼ��RDAЭ��ջ ,����MD5��Կ����Կ����
    lRet = m_pRDAProtStack->Init(DEFAULT_MD5_KEY, strlen(DEFAULT_MD5_KEY));
    if (IVS_SUCCEED != lRet)
    {
        IVS_RUN_LOG_ERR( "InitRDAProtStack End, fail to init m_pRDAProtStack.");
        return lRet;
    }

    IVS_DBG_LOG("InitRDAProtStack End, lRet:%ld.", lRet);
    return lRet;
}

long CRemoteDecoderMgr::InitRemoteDecoderMgr
(   IN PCUMW_CALLBACK_NOTIFY   pFunNotifyCallBack,
    IN void*                   pCallBackUserData)
{
    IVS_DBG_LOG("Init Remote Decoder Manager Begin.");
    long lResult = IVS_SUCCEED;

    //����֪ͨ�ص�����״ֵ̬
    m_objStatusNotifyCallBack.SetStatusValue(pFunNotifyCallBack, pCallBackUserData);

    //��ʼ����Ϣ����
    lResult = InitDecoderMgrEventQ();
    if (VOS_OK != lResult)
    {
        IVS_RUN_LOG_ERR( "Init Event Queue Failed On Init Remote Decoder Manager.");
        return IVS_FAIL;
    }

    //��ʼ��������Դ����
    lResult = InitDecoderConnHandleMgr();
    if (VOS_SUCCESS != lResult)
    {
        IVS_RUN_LOG_ERR( "Init Connection Manager Failed On Init Remote Decoder Manager.");
        return IVS_FAIL;
    }

    //��ʼ��RDA����Դ����
    lResult = InitRDAPacketMgr();
    if (VOS_SUCCESS != lResult)
    {
        IVS_RUN_LOG_ERR( "Init Packet Manager Failed On Init Remote Decoder Manager.");
        return IVS_FAIL;
    }

    //��ʼ��������������Դ����
    lResult = InitRemoteDecoderArry();
    if (VOS_SUCCESS != lResult)
    {
        IVS_RUN_LOG_ERR( "Init Remote Decoder Array Failed On Init Remote Decoder Manage.");
        return IVS_FAIL;
    }

    //��ʼ��RDAЭ��ջ
    lResult = InitRDAProtStack();
    if (IVS_SUCCEED != lResult)
    {
        IVS_RUN_LOG_ERR( "Init RDA Prototocl Stack On Init Remote Decoder Manage.");
        return lResult;
    }

    //��ʼ�����ͻ�����
    (void)VOS_NEW(m_pSendBuffer, MAX_RDAMSG_LEN);
    if (NULL == m_pSendBuffer)
    {
        IVS_RUN_LOG_ERR( "Create Send Buffer Failed On Init Remote Decoder Manage.");
        return IVS_ALLOC_MEMORY_ERROR;
    }

    //��ʼ��RDA������Ϣ����
    lResult = InitRemoteDecoderReqProc();
    if (VOS_SUCCESS != lResult)
    {
        IVS_RUN_LOG_ERR( "Init Request Process Failed On Init Remote Decoder Manage.");
        return IVS_FAIL;
    }

    //����tcp������
    lResult = InitDecoderMgrServerHandle();
    if (VOS_SUCCESS != lResult)
    {
        IVS_RUN_LOG_ERR( "Init Tcp Server Failed On Init Remote Decoder Manage.");
        return IVS_FAIL;
    }

    //��ʼ����ʱ������
    lResult = InitCheckHeartBeatTrigger();
    if (VOS_SUCCESS != lResult)
    {
        IVS_RUN_LOG_ERR( "Init Check Heart Beat Trigger Failed On Init Remote Decoder Manage.");
        return IVS_FAIL;
    }

    //��ö�ʱ��ʵ��
    CNVSTimer& objNVSTimer = CStatusSingleton::Instance().GetNVSTimer();

    //ע�ᶨʱ������
    (void)objNVSTimer.registerTimer(m_pCheckHeartBeatTrigger, NULL,
                                    MAX_HEARTBEAT_INTERVAL / CUMW::TIMER_INTERVAL, enRepeated);

    IVS_DBG_LOG("Init Remote Decoder Manager Success.");
    return IVS_SUCCEED;
};

long CRemoteDecoderMgr::StartRemoteDecoderMgr()
{
    IVS_DBG_LOG("Start Remote Decoder Manager Begin.");

    //����Ƿ��Ѿ���ʼ��
    if (!CheckIsInited())
    {
        IVS_RUN_LOG_ERR( "Start Remote Decoder Manager Failed. Not Init yet.");
        return IVS_FAIL;
    }

    //�������մ����߳�
    ULONG ulRet = VOS_CreateThread((VOS_THREAD_FUNC)invoke, (void*)this,
                                   &m_pVosThread, VOS_DEFAULT_STACK_SIZE);
    if (VOS_OK != ulRet)
    {
        IVS_RUN_LOG_ERR( "Create Thread Failed On Start Remote Decoder Manager. ErrorCode = 0x%04X.", ulRet);
        return IVS_TVWALL_CREATE_THREAD_FAIL;
    }

    ;

    IVS_DBG_LOG("Start Remote Decoder Manager Success.");
    return IVS_SUCCEED;
}

long CRemoteDecoderMgr::StopRemoteDecoderMgr()
{
    IVS_DBG_LOG("Stop Remote Decoder Manager Begin.");

    //����Ƿ��Ѿ���ʼ��
    if (!CheckIsInited())
    {
        IVS_RUN_LOG_ERR( "Stop Remote Decoder Manager Failed. Not Init yet.");
        return IVS_FAIL;
    }

    CDecoderMgrEvent* pEvent = NULL;
    (void)VOS_NEW(pEvent);
    if (VOS_NULL == pEvent)
    {
        IVS_RUN_LOG_ERR( "CRemoteDecoderMgr::Exit, new pEvent fail.");
        return IVS_FAIL;
    }

    pEvent->m_ulEventType = RDA_PACKETMGR_EVENT_EXIT_THREAD;

    IVS_DBG_LOG("push exit event to event list,"
            "Event(0x%x), EventType(%lu)", pEvent, pEvent->m_ulEventType);

    //�����¼�����
    LONG lRetValue = m_pDecoderMgrEventQ->pushBackEv( pEvent );
    if (VOS_OK != lRetValue)
    {
        IVS_RUN_LOG_ERR("push exit event to event fail, "
                "Event(0x%x), EventType(%lu)", pEvent, pEvent->m_ulEventType);
        VOS_DELETE(pEvent);
        return IVS_FAIL;
    }

    //�ȴ��߳������˳�
    (void)VOS_ThreadJoin(m_pVosThread);

    //���ͨ�Ź�����ʵ��
    CConnMgr& objConnMgr = CStatusSingleton::Instance().GetConnMgr();

    //ȡ��tcp������
    objConnMgr.removeTcpServer(m_pDecoderMgrServerHandle);

    VOS_DELETE(m_pDecoderMgrEventQ);

    //m_pRemoteDecoderArry������m_pDecoderConnHandleMgr֮ǰ�ͷ�
    //��Ҫԭ�����ͷ�m_pRemoteDecoderArryʱ��ر��������ӡ�
    m_pRemoteDecoderArry->Release();
    VOS_DELETE(m_pRemoteDecoderArry);

    VOS_DELETE(m_pDecoderConnHandleMgr);
    VOS_DELETE(m_pDecoderMgrServerHandle);

    //BEGIN V100R001C05 ADD 2010-01-29 ligengqiang l00124479 for Purify�������ڴ�й¶
    m_pRemoteDecoderReqProc->Exit();
    //END   V100R001C05 ADD 2010-01-29 ligengqiang l00124479 for Purify�������ڴ�й¶
    VOS_DELETE(m_pRemoteDecoderReqProc);
    VOS_DELETE(m_pRDAPacketMgr);

    //��ö�ʱ��ʵ��
    CNVSTimer& objNVSTimer = CStatusSingleton::Instance().GetNVSTimer();

    //ȡ����ʱ��
    (void)objNVSTimer.cancelTimer(m_pCheckHeartBeatTrigger);
    VOS_DELETE(m_pCheckHeartBeatTrigger);

    VOS_DELETE(m_pRDAProtStack);
    VOS_DELETE(m_pSendBuffer, MAX_RDAMSG_LEN);

    if (NULL != m_pVosThread)
    {
        VOS_free(m_pVosThread);
    }

    IVS_DBG_LOG("Stop Remote Decoder Manager End.");
    return IVS_SUCCEED;
}

void* CRemoteDecoderMgr::invoke(void *argc)
{
    IVS_DBG_LOG("Invoke Start.");

    if (NULL == argc)
    {
        IVS_RUN_LOG_ERR( "Invoke End, param argc is null");
        return NULL;
    }

    CRemoteDecoderMgr* pRemoteDecoderMgr = (CRemoteDecoderMgr*)argc;
    pRemoteDecoderMgr->ProcEventThread();

    IVS_DBG_LOG("Invoke End, ProcEventThread exit.");
    VOS_pthread_exit(NULL);
    return NULL;
}

long CRemoteDecoderMgr::InitBusiness
(   IN CBusiness& objBusiness)
{
    const unsigned long ulBusinessId = objBusiness.GetBusinessId();

    IVS_RUN_LOG_INF(
            "[ BusinessID = %lu ] Init Business In Remote Decoder Manager Begin. DecoderID = %.20s. ChannelID = %lu.",
            ulBusinessId, objBusiness.m_szDecoderId, objBusiness.m_ulChannelId);

    //�����ʼ��ҵ��������Ϣ
    CRDAInitBusinessReq objRDAReq;
    objRDAReq.m_stMsgHead.usPackID = GetTransNo();
    objRDAReq.m_ulBusinessID = ulBusinessId;
    objRDAReq.m_usChannelNo = (RDA_UINT2)(objBusiness.m_ulChannelId);
    if(!CToolsHelp::Strncpy(objRDAReq.m_szDecoderID, sizeof(objRDAReq.m_szDecoderID), objBusiness.m_szDecoderId, sizeof(objRDAReq.m_szDecoderID) - 1))
	{
		IVS_RUN_LOG_ERR("Init Business Copy DecoderId error.");
		return IVS_ALLOC_MEMORY_ERROR;
	}
    //Э������ֵ
    objRDAReq.m_ucTransportProtocol = objBusiness.GetDecodeProtocolType();
	
    //�ص���������
    CUMW_NOTIFY_INFO stNotifyInfo;
    CUMW_NOTIFY_INIT_DECODER_INFO stInitInfo;

    memset((void*)&stInitInfo, 0, sizeof(CUMW_NOTIFY_INIT_DECODER_INFO));
    stInitInfo.ulBusinessID   = ulBusinessId;
    stNotifyInfo.ulNotifyType = CUMW_NOTIFY_TYPE_INIT_DECODER;
    stNotifyInfo.pNotifyInfo  = (void*)&stInitInfo;
    stNotifyInfo.ulNotifyInfoLen = sizeof(CUMW_NOTIFY_INFO);	//2012-12-18 ������ Ϊʲô��sizeof(CUMW_NOTIFY_INFO)
    stInitInfo.ulAsyncSeq = objBusiness.m_ulAsyncSeq;
    if(!CToolsHelp::Strncpy(stInitInfo.szDecoderID, sizeof(stInitInfo.szDecoderID), objBusiness.m_szDecoderId, sizeof(stInitInfo.szDecoderID) - 1))
	{
		IVS_RUN_LOG_ERR("Init Business Copy DecoderId error.");
		return IVS_ALLOC_MEMORY_ERROR;
	}
    //����������Ϣ��ͬ���ȴ���Ӧ��Ϣ������
    CRDAInitBusinessRsp objRDAResp;

    //������������Ӧ��Ϣ��Э�����ͣ������޷�����
    objRDAResp.m_ucTransportProtocol = objRDAReq.m_ucTransportProtocol;
    long lResult = SendReqMsg(objRDAReq, objBusiness.m_szDecoderId, objRDAResp);
    if (VOS_SUCCESS != lResult)
    {
        stInitInfo.stErrorCode.ulErrorCode = IVS_TVWALL_SEND_FAIL;
        lResult = m_objStatusNotifyCallBack.NotifyCallBack(stNotifyInfo);

        IVS_RUN_LOG_ERR( "[ BusinessID = %lu ] Init Business In Remote Decoder Manager Failed. "
                "Request or Response Message Problem.", ulBusinessId);
        return lResult;
    }

    IVS_RUN_LOG_INF("[ BusinessID = %lu ] Init Business Return: DecoderID = %s, ChannelId = %lu, ErrorCode = %lu, ErrorDescription = %s."
            "Request or Response Message Problem.", ulBusinessId, objRDAResp.m_szDecoderID, objBusiness.m_ulChannelId, objRDAResp.m_ulErrorCode, objRDAResp.m_szErrorDescription);

    stInitInfo.stErrorCode.ulErrorCode = objRDAResp.m_ulErrorCode;

    stInitInfo.stMediaNetInfo.ucTransportProtocol = objRDAResp.m_ucTransportProtocol;
    if(!CToolsHelp::Strncpy(stInitInfo.stMediaNetInfo.szMediaIP, sizeof(stInitInfo.stMediaNetInfo.szMediaIP), objRDAResp.m_szMediaIP, sizeof(stInitInfo.stMediaNetInfo.szMediaIP)-1))
	{
		IVS_RUN_LOG_ERR("Init Business Copy MediaIP error.");
		return IVS_ALLOC_MEMORY_ERROR;
	}
    if(!CToolsHelp::Strncpy(stInitInfo.stErrorCode.szErrorDescription, sizeof(stInitInfo.stErrorCode.szErrorDescription), objRDAResp.m_szErrorDescription, sizeof(stInitInfo.stErrorCode.szErrorDescription) - 1))
	{
		IVS_RUN_LOG_ERR("Init Business Copy ErrorDescription error.");
		return IVS_ALLOC_MEMORY_ERROR;
	}
    if ((RDA_PROTOCOL_RTP_OVER_TCP == objBusiness.GetDecodeProtocolType()) ||
        (RDA_PROTOCOL_NSS_OVER_TCP == objBusiness.GetDecodeProtocolType()))
    {
        stInitInfo.stMediaNetInfo.usTcpInterleavedPort = objRDAResp.m_usTcpInterleavedPort;

        IVS_RUN_LOG_INF(
                "[ BusinessID = %lu ] Init Business In Remote Decoder Manager In. DecoderIp = %.15s. TcpInterleavedPort = %u.",
                ulBusinessId, objRDAResp.m_szMediaIP, objRDAResp.m_usTcpInterleavedPort);
    }
    else
    {
        stInitInfo.stMediaNetInfo.usUdpVideoRtpPort  = objRDAResp.m_usUdpVideoRtpPort;
        stInitInfo.stMediaNetInfo.usUdpVideoRtcpPort = objRDAResp.m_usUdpVideoRtcpPort;
        stInitInfo.stMediaNetInfo.usUdpAudioRtpPort  = objRDAResp.m_usUdpAudioRtpPort;
        stInitInfo.stMediaNetInfo.usUdpAudioRtcpPort = objRDAResp.m_usUdpAudioRtcpPort;

        IVS_RUN_LOG_INF("[ BusinessID = %lu ] Init Business In Remote Decoder Manager In. DecoderIp = %.15s. "
                "Udp: VideoPort(Rtp = %u, Rtcp = %u). AudioPort(Rtp = %u, Rtcp = %u)",
                ulBusinessId, objRDAResp.m_szMediaIP,
                objRDAResp.m_usUdpVideoRtpPort,
                objRDAResp.m_usUdpVideoRtcpPort,
                objRDAResp.m_usUdpAudioRtpPort,
                objRDAResp.m_usUdpAudioRtcpPort);
    }

    //�ص�֪ͨ
    lResult = m_objStatusNotifyCallBack.NotifyCallBack(stNotifyInfo);

    IVS_DBG_LOG("[ BusinessID = %lu ] Init Business In Remote Decoder Manager End. Result = 0x%04X.", ulBusinessId,
            lResult);
    return lResult;
}

long CRemoteDecoderMgr::StartBusiness
(   IN const CBusiness& objBusiness)
{
    const unsigned long ulBusinessId = objBusiness.GetBusinessId();

    IVS_RUN_LOG_INF(
            "[ BusinessID = %lu ] Start Business In Remote Decoder Manager Begin. DecoderID = %.20s. ChannelID = %lu.",
            ulBusinessId, objBusiness.m_szDecoderId, objBusiness.m_ulChannelId);

    //���쿪ʼҵ��������Ϣ
    CRDAStartBusinessReq objRDAReq;
    objRDAReq.m_stMsgHead.usPackID = GetTransNo();
    objRDAReq.m_ulBusinessID = ulBusinessId;
    objRDAReq.m_usChannelNo = (USHORT)objBusiness.m_ulChannelId;
    if(!CToolsHelp::Strncpy(objRDAReq.m_szDecoderID, sizeof(objRDAReq.m_szDecoderID), objBusiness.m_szDecoderId, sizeof(objRDAReq.m_szDecoderID) - 1))
	{
		IVS_RUN_LOG_ERR("Start Business Copy DecoderID error.");
		return IVS_ALLOC_MEMORY_ERROR;
	}
    if (RDA_PROTOCOL_RTP_OVER_TCP == objBusiness.GetDecodeProtocolType())
    {
        objRDAReq.m_ucTransportProtocol  = RDA_PROTOCOL_RTP_OVER_TCP;
        objRDAReq.m_usTcpInterleavedPort = objBusiness.m_stMediaDistributePortInfo.usInterleavedPort;
    }
    else
    {
        objRDAReq.m_ucTransportProtocol = RDA_PROTOCOL_RTP_OVER_UDP;
        objRDAReq.m_usUdpVideoRtpPort  = objBusiness.m_stMediaDistributePortInfo.usVideoRtpPort;
        objRDAReq.m_usUdpVideoRtcpPort = objBusiness.m_stMediaDistributePortInfo.usVideoRtcpPort;
        objRDAReq.m_usUdpAudioRtpPort  = objBusiness.m_stMediaDistributePortInfo.usAudioRtpPort;
        objRDAReq.m_usUdpAudioRtcpPort = objBusiness.m_stMediaDistributePortInfo.usAudioRtcpPort;
    }

    if(!CToolsHelp::Strncpy(objRDAReq.m_szMediaIP, sizeof(objRDAReq.m_szMediaIP), objBusiness.m_szServerIp, sizeof(objRDAReq.m_szMediaIP)-1))
	{
		IVS_RUN_LOG_ERR("Start Business Copy MediaIP error.");
		return IVS_ALLOC_MEMORY_ERROR;
	}
    objRDAReq.m_ucVideoPayloadType = objBusiness.m_enVideoPt;
    objRDAReq.m_ucAudioPayloadType = objBusiness.m_enAudioPt;
    if(!CToolsHelp::Strncpy(objRDAReq.m_szSessionId, sizeof(objRDAReq.m_szSessionId), objBusiness.m_szSessionId, sizeof(objRDAReq.m_szSessionId) - 1))
	{
		IVS_RUN_LOG_ERR("Start Business Copy SessionId error.");
		return IVS_ALLOC_MEMORY_ERROR;
	}
    if(!CToolsHelp::Strncpy(objRDAReq.m_szSessionUrl, sizeof(objRDAReq.m_szSessionUrl), objBusiness.m_szSessionUrl, sizeof(objRDAReq.m_szSessionUrl) - 1))
	{
		IVS_RUN_LOG_ERR("Start Business Copy SessionUrl error.");
		return IVS_ALLOC_MEMORY_ERROR;
	}
    //�ص���������
    CUMW_NOTIFY_INFO stNotifyInfo;
    CUMW_NOTIFY_START_STOP_DECODER_INFO stStartInfo;

    memset((void*)&stStartInfo, 0, sizeof(CUMW_NOTIFY_START_STOP_DECODER_INFO));
    stStartInfo.ulBusinessID   = ulBusinessId;
    stNotifyInfo.ulNotifyType = CUMW_NOTIFY_TYPE_START_DECODER;
    stNotifyInfo.pNotifyInfo  = (void*)&stStartInfo;
    stNotifyInfo.ulNotifyInfoLen = sizeof(CUMW_NOTIFY_INFO);
    stStartInfo.ulAsyncSeq = objBusiness.m_ulAsyncSeq;
    if(!CToolsHelp::Strncpy(stStartInfo.szDecoderID, sizeof(stStartInfo.szDecoderID), objBusiness.m_szDecoderId, sizeof(stStartInfo.szDecoderID) - 1))
	{
		IVS_RUN_LOG_ERR("Start Business Copy DecoderID error.");
		return IVS_ALLOC_MEMORY_ERROR;
	}
    //����������Ϣ��ͬ���ȴ���Ӧ��Ϣ������
    CRDAStartBusinessRsp objRDAMsgResp;
    long lResult = SendReqMsg(objRDAReq, objBusiness.m_szDecoderId, objRDAMsgResp);
    if (VOS_SUCCESS != lResult)
    {
        stStartInfo.stErrorCode.ulErrorCode = IVS_TVWALL_SEND_FAIL;
        lResult = m_objStatusNotifyCallBack.NotifyCallBack(stNotifyInfo);

        IVS_RUN_LOG_ERR( "[ BusinessID = %lu ] Start Business In Remote Decoder Manager Failed. "
                "Request or Response Message Problem.", ulBusinessId);
        return lResult;
    }

    IVS_DBG_LOG("[ BusinessID = %lu ] Start Business Return: DecoderID = %s, ChannelId = %lu, ErrorCode = %lu, ErrorDescription = %s.", ulBusinessId, objRDAMsgResp.m_szDecoderID, objBusiness.m_ulChannelId, objRDAMsgResp.m_ulErrorCode, objRDAMsgResp.m_szErrorDescription);

    stStartInfo.stErrorCode.ulErrorCode = objRDAMsgResp.m_ulErrorCode;
    if(!CToolsHelp::Strncpy(stStartInfo.stErrorCode.szErrorDescription, sizeof(stStartInfo.stErrorCode.szErrorDescription), objRDAMsgResp.m_szErrorDescription, sizeof(stStartInfo.stErrorCode.szErrorDescription) - 1))
	{
		IVS_RUN_LOG_ERR("Start Business Copy ErrorDescription error.");
		return IVS_ALLOC_MEMORY_ERROR;
	}
    //�ص�֪ͨ
    lResult = m_objStatusNotifyCallBack.NotifyCallBack(stNotifyInfo);

    IVS_DBG_LOG("[ BusinessID = %lu ] Start Business In Remote Decoder Manager End. Result = 0x%04X.",
            ulBusinessId, lResult);
    return lResult;
}

long CRemoteDecoderMgr::StopBusiness
(   IN const CBusiness& objBusiness)
{ 
    unsigned long ulBusinessId = objBusiness.GetBusinessId();

    IVS_RUN_LOG_INF(
            "[ BusinessID = %lu ] Stop Business In Remote Decoder Manager Begin. DecoderID = %.20s. ChannelID = %lu.",
            ulBusinessId, objBusiness.m_szDecoderId, objBusiness.m_ulChannelId);

    //����ֹͣҵ��������Ϣ
    CRDAStopBusinessReq objRDAReq;
    objRDAReq.m_stMsgHead.usPackID = GetTransNo();
    objRDAReq.m_ulBusinessID = ulBusinessId;
    objRDAReq.m_usChannelNo = (RDA_UINT2)(objBusiness.m_ulChannelId);
    if(!CToolsHelp::Strncpy(objRDAReq.m_szDecoderID, sizeof(objRDAReq.m_szDecoderID), objBusiness.m_szDecoderId, sizeof(objRDAReq.m_szDecoderID) - 1))
	{
		IVS_RUN_LOG_ERR("Stop Business Copy DecoderId error.");
		return IVS_ALLOC_MEMORY_ERROR;
	}
    //�ص���������
    CUMW_NOTIFY_INFO stNotifyInfo;
    CUMW_NOTIFY_START_STOP_DECODER_INFO stInfo;

    memset((void*)&stInfo, 0, sizeof(CUMW_NOTIFY_START_STOP_DECODER_INFO));
    stInfo.ulBusinessID   = ulBusinessId;
    stNotifyInfo.ulNotifyType = CUMW_NOTIFY_TYPE_STOP_DECODER;
    stNotifyInfo.pNotifyInfo  = (void*)&stInfo;
    stNotifyInfo.ulNotifyInfoLen = sizeof(CUMW_NOTIFY_INFO);
    stInfo.ulAsyncSeq = objBusiness.m_ulAsyncSeq;
    if(!CToolsHelp::Strncpy(stInfo.szDecoderID, sizeof(stInfo.szDecoderID), objBusiness.m_szDecoderId, sizeof(stInfo.szDecoderID) - 1))
	{
		IVS_RUN_LOG_ERR("Stop Business Copy DecoderId error.");
		return IVS_ALLOC_MEMORY_ERROR;
	}
    //����������Ϣ��ͬ���ȴ���Ӧ��Ϣ������
    CRDAStopBusinessRsp objRDAMsgResp;
    long lResult = SendReqMsg(objRDAReq, objBusiness.m_szDecoderId, objRDAMsgResp);
    if (VOS_SUCCESS != lResult)
    {
        stInfo.stErrorCode.ulErrorCode = IVS_TVWALL_SEND_FAIL;
        lResult = m_objStatusNotifyCallBack.NotifyCallBack(stNotifyInfo);

        IVS_RUN_LOG_ERR( "[ BusinessID = %lu ] Stop Business In Remote Decoder Manager Failed. "
                "Request or Response Message Problem.", ulBusinessId);
        return lResult;
    }

    IVS_RUN_LOG_INF("[ BusinessID = %lu ] Stop Business Return: DecoderID = %s, ChannelId = %lu, ErrorCode = %lu, ErrorDescription = %s."
        "Request or Response Message Problem.", ulBusinessId, objRDAMsgResp.m_szDecoderID, objBusiness.m_ulChannelId, objRDAMsgResp.m_ulErrorCode, objRDAMsgResp.m_szErrorDescription);

    stInfo.stErrorCode.ulErrorCode = objRDAMsgResp.m_ulErrorCode;
    if(!CToolsHelp::Strncpy(stInfo.stErrorCode.szErrorDescription, sizeof(stInfo.stErrorCode.szErrorDescription), objRDAMsgResp.m_szErrorDescription, sizeof(stInfo.stErrorCode.szErrorDescription) - 1))
	{
		IVS_RUN_LOG_ERR("Stop Business Copy ErrorDescription error.");
		return IVS_ALLOC_MEMORY_ERROR;
	}
    //�ص�֪ͨ
    lResult = m_objStatusNotifyCallBack.NotifyCallBack(stNotifyInfo);

    IVS_DBG_LOG("[ BusinessID = %lu ] Stop Business In Remote Decoder Manager End. Result = 0x%04X.", ulBusinessId,
            lResult);
    return lResult;
}

long CRemoteDecoderMgr::PlayAudio
(   IN const CBusiness&    objBusiness,
    IN BOOL                bStart)
{
    const unsigned long ulBusinessId = objBusiness.GetBusinessId();

    IVS_RUN_LOG_INF(
            "[ BusinessID = %lu ] Play Audio In Remote Decoder Manager Begin. DecoderID = %.20s. ChannelID = %lu. bStart = %u.",
            ulBusinessId, objBusiness.m_szDecoderId, objBusiness.m_ulChannelId, bStart);

    //���쿪ʼ��ֹͣ������Ƶ��Ϣ
    CRDAAudioCtrolReq objRDAReq;
    objRDAReq.m_stMsgHead.usPackID = GetTransNo();
    objRDAReq.m_ulBusinessID = ulBusinessId;
    objRDAReq.m_usChannelNo = (RDA_UINT2)(objBusiness.m_ulChannelId);
    if(!CToolsHelp::Strncpy(objRDAReq.m_szDecoderID, sizeof(objRDAReq.m_szDecoderID), objBusiness.m_szDecoderId, sizeof(objRDAReq.m_szDecoderID) - 1))
	{
		IVS_RUN_LOG_ERR("Play Audio Copy DecoderID error.");
		return IVS_ALLOC_MEMORY_ERROR;
	}
    objRDAReq.m_ucAudioCmd = (RDA_UCHAR)bStart;

    //����������Ϣ��ͬ���ȴ���Ӧ��Ϣ������
    CRDAAudioCtrolRsp objRDAMsgResp;
    long lResult = SendReqMsg(objRDAReq, objBusiness.m_szDecoderId, objRDAMsgResp);
    if (VOS_SUCCESS != lResult)
    {
        IVS_RUN_LOG_ERR( "[ BusinessID = %lu ] Play Audio In Remote Decoder Manager Failed. "
                "Request or Response Message Problem.", ulBusinessId);
        return lResult;
    }

    //����ֵ�����뷵��
    lResult = (long)objRDAMsgResp.m_ulErrorCode;

    //�ص���������
    CUMW_NOTIFY_INFO stNotifyInfo;
    CUMW_NOTIFY_COM_RES_INFO stResInfo;

    memset((void*)&stResInfo, 0, sizeof(CUMW_NOTIFY_COM_RES_INFO));
    stResInfo.ulBusinessID   = ulBusinessId;
    stNotifyInfo.ulNotifyType = CUMW_NOTIFY_TYPE_AUDIO_CONTROL;
    stNotifyInfo.pNotifyInfo  = (void*)&stResInfo;
    stNotifyInfo.ulNotifyInfoLen = sizeof(CUMW_NOTIFY_INFO);

    //pstStartInfo->ulAsyncSeq = objBusiness.ulAsyncSeq;
    stResInfo.stErrorCode.ulErrorCode = objRDAMsgResp.m_ulErrorCode;
    if(!CToolsHelp::Strncpy(stResInfo.szDecoderID, sizeof(stResInfo.szDecoderID), objRDAMsgResp.m_szDecoderID, sizeof(stResInfo.szDecoderID)-1))
	{
		IVS_RUN_LOG_ERR("Play Audio Copy DecoderID failed");
		return IVS_ALLOC_MEMORY_ERROR;
	}
    if(!CToolsHelp::Strncpy(stResInfo.stErrorCode.szErrorDescription, sizeof(stResInfo.stErrorCode.szErrorDescription), objRDAMsgResp.m_szErrorDescription, sizeof(stResInfo.stErrorCode.szErrorDescription) - 1))
	{
		IVS_RUN_LOG_ERR("Play Audio Copy ErrorDescription error.");
		return IVS_ALLOC_MEMORY_ERROR;
	}
    //�ص�֪ͨ
    lResult = m_objStatusNotifyCallBack.NotifyCallBack(stNotifyInfo);

    IVS_DBG_LOG("[ BusinessID = %lu ] Play Audio In Remote Decoder Manager End. Result = 0x%04X.", ulBusinessId,
            lResult);
    return lResult;
}

long CRemoteDecoderMgr::SetVolume
(   IN const CBusiness&    objBusiness,
    IN unsigned long       ulVolume)
{
    /*
    const unsigned long ulBusinessId = objBusiness.GetBusinessId();

    IVS_RUN_LOG_INF(
            "[ BusinessID = %lu ] Set Volume In Remote Decoder Manager Begin. DecoderID = %.20s. ChannelID = %lu. Volume = %lu.",
            ulBusinessId, objBusiness.m_szDecoderId, objBusiness.m_ulChannelId, ulVolume);

    //������Ƶ������Ϣ
    CRDATuneupVolumeReq objRDAReq;
    objRDAReq.m_stMsgHead.usPackID = GetTransNo();
    objRDAReq.m_ulBusinessID = ulBusinessId;
    objRDAReq.m_usChannelNo = (RDA_UINT2)(objBusiness.m_ulChannelId);
    if(!CToolsHelp::Strncpy(objRDAReq.m_szDecoderID, sizeof(objRDAReq.m_szDecoderID), objBusiness.m_szDecoderId, sizeof(objRDAReq.m_szDecoderID) - 1))
	{
		IVS_RUN_LOG_ERR("Play Audio Copy DecoderID error.");
		return IVS_ALLOC_MEMORY_ERROR;
	}
    objRDAReq.m_ucVolume = (RDA_UCHAR)ulVolume;

    //����������Ϣ��ͬ���ȴ���Ӧ��Ϣ������
    CRDATuneupVolumeRsp objRDAMsgResp;
    long lResult = SendReqMsg(objRDAReq, objBusiness.m_szDecoderId, objRDAMsgResp);
    if (VOS_SUCCESS != lResult)
    {
        IVS_RUN_LOG_ERR( "[ BusinessID = %lu ] Set Volume In Remote Decoder Manager Failed. "
                "Request or Response Message Problem.", ulBusinessId);
        return lResult;
    }

    //����ֵ�����뷵��
    lResult = (long)objRDAMsgResp.m_ulErrorCode;

    //�ص���������
    CUMW_NOTIFY_INFO stNotifyInfo;
    CUMW_NOTIFY_COM_RES_INFO stResInfo;

    memset((void*)&stResInfo, 0, sizeof(CUMW_NOTIFY_COM_RES_INFO));
    stResInfo.ulBusinessID   = ulBusinessId;
    stNotifyInfo.ulNotifyType = CUMW_NOTIFY_TYPE_SET_VOLUME;
    stNotifyInfo.pNotifyInfo  = (void*)&stResInfo;
    stNotifyInfo.ulNotifyInfoLen = sizeof(CUMW_NOTIFY_INFO);

    //pstStartInfo->ulAsyncSeq = objBusiness.ulAsyncSeq;
    stResInfo.stErrorCode.ulErrorCode = objRDAMsgResp.m_ulErrorCode;
    if(!CToolsHelp::Strncpy(stResInfo.szDecoderID, sizeof(stResInfo.szDecoderID), objRDAMsgResp.m_szDecoderID, sizeof(stResInfo.szDecoderID)-1))
	{
		IVS_RUN_LOG_ERR("Play Audio Copy DecoderID failed");
		return IVS_ALLOC_MEMORY_ERROR;
	}
	if(!CToolsHelp::Strncpy(stResInfo.stErrorCode.szErrorDescription, sizeof(stResInfo.stErrorCode.szErrorDescription), objRDAMsgResp.m_szErrorDescription, sizeof(stResInfo.stErrorCode.szErrorDescription) - 1))
	{
		IVS_RUN_LOG_ERR("Play Audio Copy DecoderID error.");
		return IVS_ALLOC_MEMORY_ERROR;
	}

    //�ص�֪ͨ
    lResult = m_objStatusNotifyCallBack.NotifyCallBack(stNotifyInfo);

    IVS_DBG_LOG("[ BusinessID = %lu ] Set Volume In Remote Decoder Manager End. Result = 0x%04X.", ulBusinessId,
            lResult);
    return lResult;
    */
    return IVS_SYS_NO_SUPPORT;
}//lint !e1762

//BEGIN R002C01SPC001 ADD 2011-08-16 ligengqiang 00124479 for ������֧�ֱ���˫����

void CRemoteDecoderMgr::GetLocalIp
(   IN const char*  pszDecoderId,
    OUT string&     strLocalIp)
{
    if (NULL == m_pRemoteDecoderArry)
    {
        IVS_RUN_LOG_ERR( "GetLocalIp Failed. m_pRemoteDecoderArry = NULL.");
        return;
    }

    //�ӽ�������������Դ�����л�ȡpDecoderConnHandle
    CDecoderConnHandle* pDecoderConnHandle = NULL;
    pDecoderConnHandle = m_pRemoteDecoderArry->GetConnHandle(pszDecoderId);

    if (NULL == pDecoderConnHandle)
    {
        IVS_RUN_LOG_ERR( "GetLocalIp Failed. Decoder(ID = %.20s) Has no Connection Handle.", pszDecoderId);
        return;
    }

    pDecoderConnHandle->GetLocalIp(strLocalIp);
}

//END   R002C01SPC001 ADD 2011-08-16 ligengqiang 00124479 for ������֧�ֱ���˫����

void CRemoteDecoderMgr::ProcEventThread()
{
    IVS_DBG_LOG("Enter Remote Decoder Manager Process Event Thread.");

    //����Ƿ��Ѿ���ʼ��
    if (!CheckIsInited())
    {
        IVS_RUN_LOG_ERR( "Remote Decoder Manager Process Event Thread End. Not Init yet.");
        return;
    }

    CCUMWEvent* pCUMWEvent = NULL;
    LONG lRet = VOS_FAIL;

    //����߳�δ�˳�
    while (!m_bExit)
    {
        //���¼�������ȡ��һ����Ϣ����
        BP_DBG_LOG("CRemoteDecoderMgr::ProcEventThread: size = %d", m_pDecoderMgrEventQ->size());
        lRet = m_pDecoderMgrEventQ->popFrontEv(pCUMWEvent);
        if (VOS_OK != lRet)
        {
            //ȡ����ʧ��
            IVS_RUN_LOG_ERR( "ProcEventThread End, "
                    "popFrontEv fail,thread exit. err:0x%x", lRet);
            return;
        }

        CDecoderMgrEvent* pDecoderMgrEvent = (CDecoderMgrEvent*)pCUMWEvent;/*lint !e1774 ��pCUMWEvent�ɸ���ת��Ϊ���࣬�����������*/
        if (NULL == pDecoderMgrEvent)
        {
            IVS_RUN_LOG_ERR( "ProcEventThread End, pDecoderMgrEvent is NULL.");
            return;
        }

        lRet = VOS_SUCCESS;
        VOS_BOOLEAN bNeedReleaseRef = VOS_FALSE; //�����ж��Ƿ�Ҫ����handle�����ü���
        //����Ϣ�����ж���δ���
        switch (pCUMWEvent->m_ulEventType)
        {
            //����Զ�̽��������͹�������Ϣ
        case RDA_PACKETMGR_EVENT_RECV_RD_DATA:
        {
            lRet = m_pRemoteDecoderReqProc->ProcMsg(pDecoderMgrEvent);
            bNeedReleaseRef = VOS_TRUE;
            break;
        }

            //��Զ�̽�������������
        case RDA_PACKETMGR_EVENT_SEND_RD_DATA:
        {
            lRet = ProcSendMsg(pDecoderMgrEvent);
            bNeedReleaseRef = VOS_TRUE;
            break;
        }

            //������ֻ�����Ӳ�����Ϣ
        case RDA_PACKETMGR_EVENT_RD_CONN_TIMEO:
        {
            lRet = ProcConnTimeOut(pDecoderMgrEvent);
            bNeedReleaseRef = VOS_TRUE;
            break;
        }

            //������������,���������
        case RDA_PACKETMGR_EVENT_CHECK_RD_INVALID:
        {
            m_pRemoteDecoderArry->CheckHeartBeat();
            break;
        }

            //��Զ�̽�������������
        case RDA_PACKETMGR_EVENT_SEND_HEARTBEAT:
        {
            lRet = SendHeartBeatMsg(pDecoderMgrEvent->m_pDecoderConnHandle);
            bNeedReleaseRef = VOS_TRUE;
            break;
        }

            //�߳��˳���Ϣ
        case RDA_PACKETMGR_EVENT_EXIT_THREAD:
        {
            m_bExit = VOS_TRUE;
            break;
        }
        default:
        {
			IVS_RUN_LOG_ERR("unknown Remote Decoder event %lu", pCUMWEvent->m_ulEventType);
            break;
        }
        }

        //�������Ҫ�����Ӿ�������ü���������,����������һ����Ϣ
        if (VOS_TRUE != bNeedReleaseRef)
        {
            VOS_DELETE(pDecoderMgrEvent);
            continue;
        }

        CDecoderConnHandle* pDecoderConnHandle =
            pDecoderMgrEvent->m_pDecoderConnHandle;

        //����ʧ��,�Ͱ����ӶϿ�
        if (VOS_SUCCESS != lRet)
        {
            //����Զ�̽��������Ͽ�����
            m_pRemoteDecoderReqProc->RemoteDecoderDisConnect(pDecoderConnHandle);

            IVS_RUN_LOG_ERR("proc event fail or connhandle disconnect, "
                    "Event(0x%x), EventType(%lu)",
                    pCUMWEvent, pCUMWEvent->m_ulEventType);

            VOS_DELETE(pDecoderMgrEvent);
            continue;
        }

        //ɾ��tcp���Ӿ��,�˴�ֻ����ͼȥɾ�����Ӿ����
        //ֻ�е�ͬʱ������������ʱ��������ɾ��,
        //(1) m_bNeedUse == VOS_FALSE (2) ���ü���Ϊ0
        m_pDecoderConnHandleMgr->ReleaseConnHandle(pDecoderConnHandle );
        VOS_DELETE(pDecoderMgrEvent);
    }

    IVS_DBG_LOG("Exit  Remote Decoder Manager Process Event Thread.");
    return;
};

LONG CRemoteDecoderMgr::ProcSendMsg(CDecoderMgrEvent* pEventRecv)
{
    IVS_DBG_LOG("ProcSendMsg Start.");

    //����Ƿ��Ѿ���ʼ��
    if (!CheckIsInited())
    {
        IVS_RUN_LOG_ERR( "ProcSendMsg End, not inited.");
        return VOS_FAIL;
    }

    //�������
    if (NULL == pEventRecv)
    {
        IVS_RUN_LOG_ERR( "ProcSendMsg End, param is NULL");
        return VOS_FAIL;
    }

    //���¼��ڵ��л�ȡ����handle��ָ��
    CDecoderConnHandle* pDecoderConnHandle = pEventRecv->m_pDecoderConnHandle;
    if (NULL == pDecoderConnHandle)
    {
        IVS_RUN_LOG_ERR( "ProcSendMsg End, send data fail, "
                "pEventRecv->m_pDecoderConnHandle is NULL");
        return VOS_FAIL;
    }

    //�ж��Ƿ��Ѿ�ע��
    if (NULL == pDecoderConnHandle->m_pRemoteDecoder)
    {
        IVS_RUN_LOG_ERR("ProcSendMsg End, send data fail, "
                "pDecoderConnHandle->m_pRemoteDecoder is NULL, "
                "maybe decoder not registed");
        return VOS_FAIL;
    }

    //��������
    CRDAPacket* pRDAPacket = pEventRecv->m_pRDAPacket;

    //BEGIN R001C05LITS01 MOD 2010-06-11 ligengqiang 00124479 for ���15λIP��������
    //��IPת��Ϊ�ַ���
    char* pszIP = inet_ntoa((in_addr &)pDecoderConnHandle->m_peerAddr.m_lIpAddr);

    IVS_DBG_LOG("send req msg, DecoderConnHandle %#p, IP is %.15s, Port is %d",
            pDecoderConnHandle, pszIP, ntohs((pDecoderConnHandle->m_peerAddr).m_usPort));
    //END   R001C05LITS01 MOD 2010-06-11 ligengqiang 00124479 for ���15λIP��������

    LONG lRet = pDecoderConnHandle->send(pRDAPacket->m_pSendBuff,
                                         pRDAPacket->m_ulSendBuffLen, enSyncOp);
    if (lRet < 0)
    {
        IVS_RUN_LOG_ERR( "ProcSendMsg End, send req msg fail, "
                "DecoderConnHandle(0x%x).", pDecoderConnHandle);
        return VOS_FAIL;
    }

    IVS_DBG_LOG("ProcSendMsg End, Event(0x%x), "
            "send req msg(len %ld) success.", pEventRecv, lRet);

    return VOS_SUCCESS;
}

long CRemoteDecoderMgr::SendHeartBeatMsg
(   IN CDecoderConnHandle* pDecoderConnHandle)
{
    //IVS_RUN_LOG_INF("Send Heart Beat Message Begin.");

    //����Ƿ��Ѿ���ʼ��
    if (!CheckIsInited())
    {
        IVS_RUN_LOG_ERR( "Send Heart Beat Message Failed. Not Init yet.");
        return IVS_FAIL;
    }

    if (NULL == pDecoderConnHandle)
    {
        IVS_RUN_LOG_ERR( "Send Heart Beat Message Failed. Decoder Connection Handle is Empty.");
        return IVS_FAIL;
    }

    //�ж��Ƿ��Ѿ�ע��
    if (NULL == pDecoderConnHandle->m_pRemoteDecoder)
    {
        IVS_RUN_LOG_ERR( "Send Heart Beat Message Failed. Decoder Object is Empty.");
        return IVS_FAIL;
    }

    CRemoteDecoder& objDecoder = *pDecoderConnHandle->m_pRemoteDecoder;

    //������������������Ϣ��
    CRDAHeartBeatReq objRDAReq;
    objRDAReq.m_stMsgHead.usPackID = GetTransNo();
    if(!CToolsHelp::Strncpy(objRDAReq.m_szDecoderID, sizeof(objRDAReq.m_szDecoderID), objDecoder.m_stDecoderInfo.szDecoderID, sizeof(objRDAReq.m_szDecoderID) - 1))
	{
		IVS_RUN_LOG_ERR("Send HeartBeat Msg Copy DecoderID error.");
		return IVS_ALLOC_MEMORY_ERROR;
	}

    IVS_ULONG ulValidLength = 0;

    long lResult = m_pRDAProtStack->EncodeMsg(m_pSendBuffer, MAX_RDAMSG_LEN, &objRDAReq, ulValidLength);
    if (VOS_SUCCESS != lResult)
    {
        IVS_RUN_LOG_ERR( "Encode Message Failed On Send Heart Beat Message.");
        return IVS_FAIL;
    }

    //��ӡ������Ϣ,������Ϣ̫Ƶ���ˣ����Բ���ӡ
    //m_pRDAProtStack->Print(&objRDAReq);

    lResult = pDecoderConnHandle->send(m_pSendBuffer, ulValidLength, enSyncOp);
    if (lResult < 0)
    {
        IVS_RUN_LOG_ERR( "[ DecoderConnHandle = %#p ] Send Heart Beat Message To %.15s:%u Failed.",
                pDecoderConnHandle, inet_ntoa((in_addr &)pDecoderConnHandle->m_peerAddr.m_lIpAddr),
                ntohs((pDecoderConnHandle->m_peerAddr).m_usPort));
        return IVS_FAIL;
    }

    //IVS_RUN_LOG_INF("[ DecoderConnHandle = %#p ] Send Heart Beat Message To %.15s:%u Success.",
    //        pDecoderConnHandle, inet_ntoa((in_addr &)pDecoderConnHandle->m_peerAddr.m_lIpAddr),
    //        ntohs((pDecoderConnHandle->m_peerAddr).m_usPort));
    return IVS_SUCCEED;
}

LONG CRemoteDecoderMgr::ProcConnTimeOut(const CDecoderMgrEvent* pEventRecv)
{
    IVS_DBG_LOG("ProcConnTimeOut Start.");

    //����Ƿ��Ѿ���ʼ��
    if (!CheckIsInited())
    {
        IVS_RUN_LOG_ERR( "ProcConnTimeOut End, not inited.");
        return VOS_FAIL;
    }

    //�������
    if (NULL == pEventRecv)
    {
        IVS_RUN_LOG_ERR( "ProcConnTimeOut End, param is NULL");
        return VOS_FAIL;
    }

    //���¼��ڵ��л�ȡ����handle��ָ��
    CDecoderConnHandle* pDecoderConnHandle = pEventRecv->m_pDecoderConnHandle;
    if (NULL == pDecoderConnHandle)
    {
        IVS_RUN_LOG_ERR( "ProcConnTimeOut End, "
                "pEventRecv->m_pDecoderConnHandle is NULL");
        return VOS_FAIL;
    }

    //��IPת��Ϊ�ַ���
    char* pszIP = inet_ntoa((in_addr &)pDecoderConnHandle->m_peerAddr.m_lIpAddr);

    IVS_RUN_LOG_INF("ProcConnTimeOut End, "
            "connect timeout(15s) or heartbeat timeout, "
            "release DecoderConnHandle 0x%x, IP %s, Port %d",
            pDecoderConnHandle, pszIP,
            ntohs((pDecoderConnHandle->m_peerAddr).m_usPort));

    return RDA_ERROR_DISCONNECT;
}

LONG CRemoteDecoderMgr::SendRDAPacket(CRDAPacket* pRDAPacket,
                                      const char* pszDecoderID)
{
    IVS_DBG_LOG("SendRDAPacket Start.");

    //����Ƿ��Ѿ���ʼ��
    if (!CheckIsInited())
    {
        IVS_RUN_LOG_ERR( "SendRDAPacket End, not inited.");
        return VOS_FAIL;
    }

    if ((NULL == pRDAPacket) || (NULL == pszDecoderID))
    {
        IVS_RUN_LOG_ERR( "SendRDAPacket End,"
                "param pRDAPacket or pszDecoderID is null.");
        return VOS_FAIL;
    }

    CDecoderMgrEvent* pEventSend = NULL;
    (void)VOS_NEW(pEventSend);
    if (VOS_NULL == pEventSend)
    {
        IVS_RUN_LOG_ERR( "SendRDAPacket End, new pEventSend fail.");
        return VOS_FAIL;
    }

    //�ӽ�������������Դ�����л�ȡpDecoderConnHandle
    CDecoderConnHandle* pDecoderConnHandle = NULL;
    pDecoderConnHandle = m_pRemoteDecoderArry->GetConnHandle(pszDecoderID);
    if (NULL == pDecoderConnHandle)
    {
        IVS_RUN_LOG_ERR( "SendRDAPacket End, "
                "get DecoderConnHandle by DecoderID(%.20s) fail.", pszDecoderID);

        VOS_DELETE(pEventSend);
        return VOS_FAIL;
    }

    pEventSend->m_ulEventType = RDA_PACKETMGR_EVENT_SEND_RD_DATA;
    pEventSend->m_pRDAPacket = pRDAPacket;
    pEventSend->m_pDecoderConnHandle = pDecoderConnHandle;

    //���ܴ�ʱ��Ϣ�����߳�����ɾ����handle������ȵ���addRef������ɹ�����������
    //��Ϣ�����̲߳������ɾ����handle�������ʾҵ���߳��Ѿ�ɾ���˸�handle������
    //�Ѹ��¼������¼����С�
    LONG lRetValue = pDecoderConnHandle->AddRef();
    if (VOS_SUCCESS != lRetValue)
    {
        IVS_RUN_LOG_ERR( "SendRDAPacket End, "
                "pDecoderConnHandle->AddRef fail, maybe this "
                "DecoderConnHandle(0x%x) has been deleted.",
                pDecoderConnHandle);

        VOS_DELETE(pEventSend);
        return VOS_FAIL;
    }

    IVS_DBG_LOG("In SendRDAPacket, pushBackEv,"
            "Event(0x%x), EventType(%lu), DecoderConnHandle(0x%x)",
            pEventSend, pEventSend->m_ulEventType,
            pEventSend->m_pDecoderConnHandle);

    //�����¼�����
    lRetValue = m_pDecoderMgrEventQ->pushBackEv(pEventSend);
    if (VOS_OK != lRetValue)
    {
        IVS_RUN_LOG_ERR( "SendRDAPacket End, "
                "pushBackEv fail(err 0x%x),"
                " Event(0x%x), EventType(%lu), DecoderConnHandle(0x%x)",
                lRetValue, pEventSend, pEventSend->m_ulEventType,
                pEventSend->m_pDecoderConnHandle);

        VOS_DELETE(pEventSend);
        return VOS_FAIL;
    }

    IVS_DBG_LOG("SendRDAPacket End.");
    return VOS_SUCCESS;
};

long CRemoteDecoderMgr::SendReqMsg
(   IN CRDAMsg&    objRDAMsgReq,
    IN const char* pszDecoderID,
    IN CRDAMsg&    objRDAMsgResp)
{
    IVS_DBG_LOG("Send Request Message(%s) Begin.", objRDAMsgReq.m_szRDAMsgTypeName);

    //�������
    if (NULL == pszDecoderID)
    {
        IVS_RUN_LOG_ERR( "Send Request Message(%s) Failed. DecoderID is Empty.", objRDAMsgReq.m_szRDAMsgTypeName);
        return IVS_FAIL;
    }

    //����Ƿ��Ѿ���ʼ��
    if (!CheckIsInited())
    {
        IVS_RUN_LOG_ERR( "Send Request Message(%s) Failed. Not Init yet.", objRDAMsgReq.m_szRDAMsgTypeName);
        return IVS_FAIL;
    }

    //��ȡһ�����е�pRDAPacket��ÿ�����ڶ����ͷ�
    CRDAPacket* pRDAPacket = m_pRDAPacketMgr->GetRDAPacket();
    if (NULL == pRDAPacket)
    {
        IVS_RUN_LOG_ERR( "Send Request Message(%s) Failed. No Idle Packet.", objRDAMsgReq.m_szRDAMsgTypeName);
        return IVS_FAIL;
    }

    //��ʼ������ֵ
    long lResult = IVS_SUCCEED;

    //��װ������Ϣ
    lResult = m_pRDAProtStack->EncodeMsg(pRDAPacket->m_pSendBuff,
                                         MAX_RDAMSG_LEN,
                                         &objRDAMsgReq,
                                         pRDAPacket->m_ulSendBuffLen);
    if (VOS_SUCCESS != lResult)
    {
        //�黹pRDAPacket
        m_pRDAPacketMgr->ReleaseRDAPacket(pRDAPacket);

        IVS_RUN_LOG_ERR( "Encode Message Failed On Send Request Message(%s).", objRDAMsgReq.m_szRDAMsgTypeName);
        return IVS_FAIL;
    }

    //���ð����к�
    pRDAPacket->m_usPacketNo = objRDAMsgReq.m_stMsgHead.usPackID;

    //��ӡ������Ϣ
    m_pRDAProtStack->Print(&objRDAMsgReq);

    //����
    lResult = SendRDAPacket(pRDAPacket, pszDecoderID);
    if (VOS_SUCCESS != lResult)
    {
        //�黹pRDAPacket
        m_pRDAPacketMgr->ReleaseRDAPacket(pRDAPacket);

        IVS_RUN_LOG_ERR( "Send RDA Packet Failed On Send Request Message(%s).", objRDAMsgReq.m_szRDAMsgTypeName);
        return IVS_FAIL;
    }

    //�ȴ���Ӧ
    lResult = pRDAPacket->WaitReadPacketEv();
    if (VOS_SUCCESS != lResult)
    {
        //�黹pRDAPacket
        m_pRDAPacketMgr->ReleaseRDAPacket(pRDAPacket);

        IVS_RUN_LOG_ERR( "Wait Response Message Failed On Send Request Message(%s).", objRDAMsgReq.m_szRDAMsgTypeName);
        return IVS_FAIL;
    }

    //ͬ��������Ӧ��Ϣ������
    lResult = m_pRDAProtStack->DecodeMsg(pRDAPacket->m_pRecvBuff, MAX_RDAMSG_LEN, objRDAMsgResp);

    //�黹pRDAPacket
    m_pRDAPacketMgr->ReleaseRDAPacket(pRDAPacket);

    if (VOS_SUCCESS != lResult)
    {
        IVS_RUN_LOG_ERR( "Decode Response Message Failed On Send Request Message(%s).",
                objRDAMsgReq.m_szRDAMsgTypeName);
        return IVS_FAIL;
    }

    //��ӡ��Ӧ��Ϣ
    m_pRDAProtStack->Print(&objRDAMsgResp);

    IVS_DBG_LOG("Send Request Message(%s) And Receive Response Message Success.", objRDAMsgReq.m_szRDAMsgTypeName);
    return IVS_SUCCEED;
}

//END V100R001C05 ADD 2010-01-19 zhaoyuzhen z00137994 for ���������ظ���
IVS_INT32 __stdcall CRemoteDecoderMgr::HandleNotifyCallBack
(   IN CUMW_NOTIFY_INFO* pstNotifyInfo,
    IN void*             pUserData)
{
    if (NULL == pstNotifyInfo)
    {
        IVS_RUN_LOG_ERR("Remote Decoder Manager Handle Notify Call Back Failed. Notify Info is Emtpy.");
        return IVS_FAIL;
    }

    IVS_DBG_LOG("Remote Decoder Manager Handle Notify Call Back Begin. NotifyType = 0x%04X.",
            pstNotifyInfo->ulNotifyType);

    //����ָ�밲ȫת��
    CRemoteDecoderMgr* pThis = VOS_CAST((CRemoteDecoderMgr*)pUserData);
    if (NULL == pThis)
    {
        IVS_RUN_LOG_ERR( "Remote Decoder Manager Handle Notify Call Back Failed. "
                "User Data(%#p) Can't Convert to Remote Decoder Manager Object.", pUserData);
        return IVS_FAIL;
    }

    //ֱ��͸��֪ͨ�ص�����
    long lResult = pThis->m_objStatusNotifyCallBack.NotifyCallBack(*pstNotifyInfo);
    if (IVS_SUCCEED != lResult)
    {
        IVS_RUN_LOG_ERR( "Remote Decoder Manager Handle Notify Call Back Failed. Return Error(0x%04X).", lResult);
        return IVS_FAIL;
    }

    IVS_DBG_LOG("Remote Decoder Manager Handle Notify Call Back Success. NotifyType = 0x%04X.",
            pstNotifyInfo->ulNotifyType);
    return IVS_SUCCEED;
}

unsigned short CRemoteDecoderMgr::GetTransNo()
{
    return ++m_usTransNo;
}

long CRemoteDecoderMgr::ControlBusiness
(   IN const CBusiness&            objBusiness,
    IN CUMW::BUSINESS_CTRL_PARA&   stCtrlPara)
{
    long lResult = IVS_SUCCEED;

    switch ((long)stCtrlPara.enCtrlCode)
    {
    case CUMW::BUSINESS_CTRL_CODE_E_LOCAL_AUDIO:
    {
        lResult = ControlAudio(objBusiness, stCtrlPara);
        break;
    }

    default:
    {
        IVS_RUN_LOG_ERR( "Control Business In Remote Decoder Manager Failed. CtrlCode(0x%08X) Unsurpport.",
                stCtrlPara.enCtrlCode);
        return IVS_FAIL;

        //break;
    }
    }

    if (IVS_SUCCEED != lResult)
    {
        IVS_RUN_LOG_ERR( "Control Business In Remote Decoder Manager Failed.");
        return lResult;
    }

    return IVS_SUCCEED;
}

long CRemoteDecoderMgr::ControlAudio
(   IN const CBusiness&            objBusiness,
    IN CUMW::BUSINESS_CTRL_PARA&   stCtrlPara)
{
    IVS_DBG_LOG("Control Audio In Remote Decoder Manager Begin.");

    if (sizeof(IPI_CTRL_LOCAL_AUDIO) != stCtrlPara.ulCtrlParaLen)
    {
        IVS_RUN_LOG_ERR( "Control Audio In Remote Decoder Manager Failed. Control Parameter length(%lu) Must be %d.",
                stCtrlPara.ulCtrlParaLen, sizeof(IPI_CTRL_LOCAL_AUDIO));
        return IVS_FAIL;
    }

    long lResult = IVS_SUCCEED;

    IPI_CTRL_LOCAL_AUDIO& stCtrlLocalAudio = *(IPI_CTRL_LOCAL_AUDIO*)stCtrlPara.vCtrlPara; //lint !e826 �ɱ�����

    switch (stCtrlLocalAudio.ulCtrlCode)
    {
    case CUMW_AUDIO_CTRL_CODE_OPEN:
    {
        lResult = PlayAudio(objBusiness, TRUE);
        break;
    }

    case CUMW_AUDIO_CTRL_CODE_CLOSE:
    {
        lResult = PlayAudio(objBusiness, FALSE);
        break;
    }

    case CUMW_AUDIO_CTRL_CODE_SET_VOLUME:
    {
        //lResult = SetVolume(objBusiness, stCtrlLocalAudio.ulVolume);
        break;
    }

    default:
    {
        IVS_RUN_LOG_ERR( "Control Audio In Remote Decoder Manager Failed. Audio Type(%lu) Unsurpport.",
                stCtrlLocalAudio.ulAudioType);
        return IVS_FAIL;

        //break;
    }
    }

    if (IVS_SUCCEED != lResult)
    {
        IVS_RUN_LOG_ERR( "Control Audio In Remote Decoder Manager Failed. Error From Gress Object.");
        return lResult;
    }

    IVS_DBG_LOG("Control Audio In Remote Decoder Manager Success.");
    return IVS_SUCCEED;
}

BOOL CRemoteDecoderMgr::HasDecoderBusiness
(   DECODER_BUSINESS&   stDecoderBusiness,
    REFER_CTRL_CODE     eReferCtrlCode)
{
    LIST_DECODER_BUSINESS::iterator iterFind;

    iterFind = find_if(m_listDecoderBusiness.begin(),
                       m_listDecoderBusiness.end(),
                       CFindDecoderBusiness(stDecoderBusiness));

    if (m_listDecoderBusiness.end() == iterFind) //lint !e81
    {
        return FALSE;
    }

    DECODER_BUSINESS& stExistBusiness = *iterFind; //lint !e48 !e1058
    switch ((long)eReferCtrlCode)
    {
    case REFER_CTRL_CODE_INC:
    {
        stExistBusiness.m_lReferNum++;
        break;
    }

    case REFER_CTRL_CODE_DEC:
    {
        stExistBusiness.m_lReferNum--;

        if (0 >= stExistBusiness.m_lReferNum)
        {
            //���ԭʼ��ҵ��ID
            stDecoderBusiness.m_ulBusinessId = stExistBusiness.m_ulBusinessId;

            (void)m_listDecoderBusiness.erase(iterFind);     //lint !e1025
            return FALSE;
        }

        break;
    }

    default:
    {
		IVS_DBG_LOG("Has Business no change");
        break;
    }
    }

    IVS_DBG_LOG("DecoderID = %.20s. ChannelID = %lu. Has Business. ReferNum = %ld.",
            stExistBusiness.m_szDecoderId, stExistBusiness.m_ulChannelId, stExistBusiness.m_lReferNum);

    return TRUE;
}
