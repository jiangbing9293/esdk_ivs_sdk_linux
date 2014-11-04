#include "ConnMgr.h"
#include "CumwCommon.h"

#define CONN_SECOND_IN_MS 1000
#define CONN_MS_IN_US 1000

//lint -e526
CNetworkAddr::CNetworkAddr() : m_lIpAddr(InvalidIp), m_usPort(Invalidport)
{

}

CNetworkAddr::~CNetworkAddr()
{}

CHandle::CHandle() : m_lSockFD(InvalidSocket)
					, m_pHandleNode(NULL)
#if VOS_APP_OS == VOS_OS_LINUX
					, m_lEpfd(InvalidFd)
#endif //#if
#if VOS_APP_OS == VOS_OS_WIN32
					, m_bReadSelected(VOS_FALSE)
					, m_bWriteSelected(VOS_FALSE)
#endif  //#if    
					, m_ulEvents(EPOLLIN)
{    
    m_pMutexHandle = VOS_CreateMutex();
}

CHandle::~CHandle()
{
    try
    {
        if (NULL != m_pHandleNode)
        {
            VOS_DELETE(m_pHandleNode);
            m_pHandleNode = NULL;
        }

        if (NULL != m_pMutexHandle)
        {
            (void)VOS_DestroyMutex(m_pMutexHandle);
            m_pMutexHandle = NULL;
        }
    }
    catch (...)
    {}
}

long CHandle::initHandle(void)
{
    //��ֹ����handleʱδ�ر�����
    if (NULL != m_pMutexHandle)
    {
        this->close();
    }

    m_lSockFD = InvalidSocket;
    m_pHandleNode = NULL;
#if VOS_APP_OS == VOS_OS_LINUX
    m_lEpfd = InvalidFd;
#endif
    m_ulEvents = EPOLLIN;
    if (NULL == m_pMutexHandle)
    {
        m_pMutexHandle = VOS_CreateMutex();
    }

    if (NULL == m_pMutexHandle)
    {
		IVS_RUN_LOG_ERR("Handle Init Failed, MutexHandle is NULL");
        return VOS_FAIL;
    }

    return VOS_SUCCESS;
}

void CHandle::setHandleSend(VOS_BOOLEAN bHandleSend)
{
    if (m_pMutexHandle != NULL)
    {
        if (VOS_OK != VOS_MutexLock(m_pMutexHandle))
        {
			IVS_RUN_LOG_ERR("Handle Set Send Lock Failed");
            return;
        }
    }

    //����Ҫ������¼�����
    if (VOS_FALSE == bHandleSend)
    {
        m_ulEvents = m_ulEvents & (~EPOLLOUT);
    }
    else
    {
        m_ulEvents = m_ulEvents | EPOLLOUT;
    }

    if ((m_pHandleNode != NULL) && (m_lSockFD != InvalidSocket))
    {
#if VOS_APP_OS == VOS_OS_LINUX
        //��handle��ӵ�epoll�ļ�����
        struct epoll_event epEvent;
        memset(&epEvent, 0, sizeof(epEvent));

        //������Ҫ������¼���ص��ļ�������
        epEvent.data.ptr = (void *)m_pHandleNode;

        //����Ҫ������¼�����
        epEvent.events = m_ulEvents;

        //�޸�ע���epoll�¼�
        if (0 != epoll_ctl(m_lEpfd, EPOLL_CTL_MOD, m_lSockFD, &epEvent))
        {
            IVS_RUN_LOG_ERR("modify event fail, "
                           "m_lSockFD = %ld", m_lSockFD);
        }
#endif

    }

    if (m_pMutexHandle != NULL)
    {
        (void)VOS_MutexUnlock(m_pMutexHandle);
    }
}

void CHandle::setHandleRecv(VOS_BOOLEAN bHandleRecv)
{
    if (m_pMutexHandle != NULL)
    {
        if (VOS_OK != VOS_MutexLock(m_pMutexHandle))
        {
			IVS_RUN_LOG_ERR("Handle Set Recv Lock Failed");
            return;
        }
    }

    //����Ҫ������¼�����
    if (VOS_FALSE == bHandleRecv)
    {
        m_ulEvents = m_ulEvents & (~EPOLLIN);
    }
    else
    {
        m_ulEvents = m_ulEvents | EPOLLIN;
    }

    if ((m_pHandleNode != NULL) && (m_lSockFD != InvalidSocket))
    {
#if VOS_APP_OS == VOS_OS_LINUX
        //��handle��ӵ�epoll�ļ�����
        struct epoll_event epEvent;
        memset(&epEvent, 0, sizeof(epEvent));

        //������Ҫ������¼���ص��ļ�������
        epEvent.data.ptr = (void *)m_pHandleNode;

        //����Ҫ������¼�����
        epEvent.events = m_ulEvents;

        //�޸�ע���epoll�¼�
        if (0 != epoll_ctl(m_lEpfd, EPOLL_CTL_MOD, m_lSockFD, &epEvent))
        {
            IVS_RUN_LOG_ERR("modify event fail, "
                           "m_lSockFD = %ld", m_lSockFD);
        }
#endif

    }

    if (m_pMutexHandle != NULL)
    {
        (void)VOS_MutexUnlock(m_pMutexHandle);
    }
}

void CHandle::close(void)
{
    if (InvalidSocket != m_lSockFD)
    {
        (void)CLOSESOCK((SOCKET)m_lSockFD);
        m_lSockFD = InvalidSocket;
    }

    return;
}

CNetworkHandle::CNetworkHandle()
{

}

long CNetworkHandle::initHandle(void)
{
    if (VOS_SUCCESS != CHandle::initHandle())
    {
		IVS_RUN_LOG_ERR("Handle Init Failed");
        return VOS_FAIL;
    }

    m_lSockFD = InvalidSocket;

    return VOS_SUCCESS;
}

long CNetworkHandle::recv(char *pArrayData, CNetworkAddr *pPeerAddr, const ULONG ulDataSize,
                          const EnumSyncAsync bSyncRecv)
{
	IVS_RUN_LOG_ERR("Net Recv Failed");
    return VOS_FAIL;
}

void CNetworkHandle::handle_recv(void)
{
}

void CNetworkHandle::handle_send(void)
{
}

#if VOS_APP_OS == VOS_OS_LINUX

long CNetworkHandle::sendMsg(const struct msghdr *pMsg)
{
    if (InvalidSocket == m_lSockFD)
    {
		IVS_RUN_LOG_ERR("Net send Msg Failed");
        return SendRecvError;
    }

    return ::sendmsg(m_lSockFD, pMsg, 0);
}

#endif

CTcpConnHandle::CTcpConnHandle() : m_lConnStatus(enIdle), m_ulRecvBufferSize(DEFAULT_SENDRECV_SIZE)
{
}

CTcpConnHandle::~CTcpConnHandle()
{
    try
    {
        if (InvalidSocket != m_lSockFD)
        {
            IVS_RUN_LOG_ERR("handle not released, "
                           "m_lSockFD = %ld, peer_ip(0x%x), peer_port(%d)", m_lSockFD,
                           ntohl((ULONG)(m_peerAddr.m_lIpAddr)), ntohs(m_peerAddr.m_usPort));
            (void)CLOSESOCK((SOCKET)m_lSockFD);
            m_lSockFD = InvalidSocket;
        }
    }
    catch (...)
	{}//lint !e1775
}

long CTcpConnHandle::initHandle(void)
{
    if (VOS_SUCCESS != CNetworkHandle::initHandle())
    {
		IVS_RUN_LOG_ERR("Net Handle init Failed");
        return VOS_FAIL;
    }

    m_lConnStatus = enIdle;
    return VOS_SUCCESS;
}

//BEGIN R001C05LITSB010 ADD 2010-07-12 ligengqiang 00124479 for ¼�������������ƣ�����Ӧ��������С
//���ý��� Buffer��С
void CTcpConnHandle::SetRecvBufferSize(unsigned long ulSize)
{
    if (ulSize >= MIN_RECV_SIZE)
    {
        m_ulRecvBufferSize = ulSize;
    }
}

//END   R001C05LITSB010 ADD 2010-07-12 ligengqiang 00124479 for ¼�������������ƣ�����Ӧ��������С

long CTcpConnHandle::conn(const CNetworkAddr *pLocalAddr,
                          const CNetworkAddr *pPeerAddr, const EnumSyncAsync bSyncConn, ULONG ulTimeOut)
{
    m_lConnStatus = enConnFailed;

    long lSockFd = (long)socket(AF_INET, SOCK_STREAM, 0);
    if (lSockFd < 0)
    {
        IVS_RUN_LOG_ERR("opening client socket error(%d)", CONN_ERRNO);
        return VOS_FAIL;
    }

    //setSendBufSize
    long lSendBufSize = DEFAULT_SENDRECV_SIZE;
    socklen_t lSendBufLength = sizeof(lSendBufSize);
    if (setsockopt((SOCKET)lSockFd, SOL_SOCKET, SO_SNDBUF, (char*)&lSendBufSize,
                   lSendBufLength) < 0)
    {
        (void)CLOSESOCK((SOCKET)lSockFd);
        IVS_RUN_LOG_ERR("setSendBufSize client socket error(%d)", CONN_ERRNO);
        return VOS_FAIL;
    }

    //setRecBufSize
    //BEGIN R001C05LITSB010 MOD 2010-07-12 ligengqiang 00124479 for ¼�������������ƣ�����Ӧ��������С
    long lRecvBufSize = (long)m_ulRecvBufferSize;
    //END   R001C05LITSB010 MOD 2010-07-12 ligengqiang 00124479 for ¼�������������ƣ�����Ӧ��������С
    socklen_t lRecvBufLength = sizeof(lRecvBufSize);
    if (setsockopt((SOCKET)lSockFd, SOL_SOCKET, SO_RCVBUF, (char*)&lRecvBufSize,
                   lRecvBufLength) < 0)
    {
        (void)CLOSESOCK((SOCKET)lSockFd);
        IVS_RUN_LOG_ERR("setRecvBufSize client socket error(%d)", CONN_ERRNO);
        return VOS_FAIL;
    }

    long flag = 1;
    if (setsockopt((SOCKET)lSockFd, IPPROTO_TCP, TCP_NODELAY, (char*)&flag,
                   sizeof(flag)) < 0)
    {
        (void)CLOSESOCK((SOCKET)lSockFd);
        IVS_RUN_LOG_ERR("set TCP_NODELAY client socket error(%d)", CONN_ERRNO);
        return VOS_FAIL;
    }

    //setReuseAddr();
    long lReuseAddrFlag = 1;
    if (setsockopt((SOCKET)lSockFd, SOL_SOCKET, SO_REUSEADDR, (char*)&lReuseAddrFlag,
                   sizeof(lReuseAddrFlag)) < 0)
    {
        (void)CLOSESOCK((SOCKET)lSockFd);
        IVS_RUN_LOG_ERR("setsockopt client socket error(%d)", CONN_ERRNO);
        return VOS_FAIL;
    }

    //�󶨱��ص�ַ
    if (((ULONG)(pLocalAddr->m_lIpAddr) != InvalidIp)
        && (pLocalAddr->m_usPort != Invalidport))
    {
        struct sockaddr_in serverAddr;
        memset((char *)&serverAddr, 0, (long)sizeof(serverAddr));
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_addr.s_addr = (unsigned long)pLocalAddr->m_lIpAddr;
        serverAddr.sin_port = pLocalAddr->m_usPort;
        errno = 0;
        if (0 > bind ((SOCKET)lSockFd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)))
        {
#if VOS_APP_OS == VOS_OS_LINUX
            char szServerAddr[INET_ADDRSTRLEN];
            if (NULL != inet_ntop(AF_INET, &serverAddr.sin_addr, szServerAddr,
                                  sizeof(szServerAddr)))
#elif VOS_APP_OS == VOS_OS_WIN32
            char *szServerAddr = inet_ntoa(serverAddr.sin_addr);
            if (NULL != szServerAddr)
#endif
            {
                IVS_RUN_LOG_ERR("Can not Bind Data_Sock %s:%d", 
                               szServerAddr, ntohs(serverAddr.sin_port));
            }
            else
            {
                IVS_RUN_LOG_ERR("Can not Bind Data_Sock %lu:%d", 
								serverAddr.sin_addr.s_addr, ntohs(serverAddr.sin_port));
            }

            (void)CLOSESOCK((SOCKET)lSockFd);
            return VOS_FAIL;
        }
    }

    //������첽���ӣ�����Ϊ������ģʽ
    if (VOS_SUCCESS != syn2Asy(lSockFd, bSyncConn, ulTimeOut))
    {
        IVS_DBG_LOG("set asynchronism failed. socket id = %ld", lSockFd);
         (void)CLOSESOCK((SOCKET)lSockFd);
        return VOS_FAIL;
    }

    //���ӶԶ�
    if (VOS_SUCCESS != connPeer(lSockFd, pPeerAddr, bSyncConn, ulTimeOut))
    {
       IVS_DBG_LOG("connect server failed. socket id = %ld", lSockFd);
        (void)CLOSESOCK((SOCKET)lSockFd);
       return VOS_FAIL;
    }

    //�������Ϊ������ģʽ���ָ�Ϊ����ģʽ
    if (VOS_SUCCESS != connMode(lSockFd, bSyncConn, ulTimeOut))
    {
        IVS_DBG_LOG("check mode failed. socket id = %ld", lSockFd);
         (void)CLOSESOCK((SOCKET)lSockFd);
        return VOS_FAIL;
    }

    m_lSockFD = lSockFd;

    m_peerAddr.m_lIpAddr = pPeerAddr->m_lIpAddr;
    m_peerAddr.m_usPort = pPeerAddr->m_usPort;

    IVS_DBG_LOG("CTcpConnHandle::conn, connect success, m_lSockFD = %ld, peer_ip(0x%x), peer_port(%d)", m_lSockFD,
                   ntohl((ULONG)(m_peerAddr.m_lIpAddr)), ntohs(m_peerAddr.m_usPort));

    return VOS_SUCCESS;
}


//������첽���ӣ�����Ϊ������ģʽ
long CTcpConnHandle::syn2Asy(const long lSockFd, const EnumSyncAsync bSyncConn, ULONG ulTimeOut)
{
    //������첽���ӣ�����Ϊ������ģʽ
    if ((enAsyncOp == bSyncConn) || (ulTimeOut > 0))
    {
#if VOS_APP_OS == VOS_OS_LINUX
        //����Ϊ������
        if (fcntl(lSockFd, F_SETFL, fcntl(lSockFd, F_GETFL) | O_NONBLOCK) < 0)
#elif VOS_APP_OS == VOS_OS_WIN32
        ULONG ulNoBlock = VOS_TRUE;
        if (SOCKET_ERROR == ioctlsocket((SOCKET)lSockFd, (long)(long)FIONBIO, &ulNoBlock))
#endif
        {
            IVS_RUN_LOG_ERR("fcntl client socket error(%d)", CONN_ERRNO);
            (void)CLOSESOCK((SOCKET)lSockFd);
            return VOS_FAIL;
        }

        setHandleSend(VOS_TRUE);
    }
    return VOS_SUCCESS;
}

//���ӶԶ�
long CTcpConnHandle::connPeer(const long lSockFd, const CNetworkAddr *pPeerAddr, 
                              const EnumSyncAsync bSyncConn, ULONG ulTimeOut)
{
    //���ӶԶ�
    struct sockaddr_in peerAddr;
    memset((char *)&peerAddr, 0, (long)sizeof(peerAddr));
    peerAddr.sin_family = AF_INET;
    peerAddr.sin_addr.s_addr = (UINT)pPeerAddr->m_lIpAddr;
    peerAddr.sin_port = pPeerAddr->m_usPort;
    long lRetVal = ::connect((SOCKET)lSockFd, (struct sockaddr*)&peerAddr,
                             sizeof(peerAddr));
    if (lRetVal < 0)
    {
        if ((enSyncOp == bSyncConn) && (0 == ulTimeOut))
        {
            (void)CLOSESOCK((SOCKET)lSockFd);
            IVS_RUN_LOG_ERR("SyncConn server fail. error(%d):%s",
                           CONN_ERRNO, strerror(CONN_ERRNO));
            return VOS_FAIL;
        }

        if ((EINPROGRESS != CONN_ERRNO) && (EWOULDBLOCK != CONN_ERRNO))
        {
            (void)CLOSESOCK((SOCKET)lSockFd);
            IVS_RUN_LOG_ERR("AsyncConn server fail. error(%d):%s",
                           CONN_ERRNO, strerror(CONN_ERRNO));
            return VOS_FAIL;
        }

        if (enSyncOp == bSyncConn)
        {
            fd_set fdWriteReady;
            struct timeval waitTime;
            waitTime.tv_sec  = (long)ulTimeOut;
            waitTime.tv_usec = 0;
            FD_ZERO(&fdWriteReady);
            FD_SET((SOCKET)lSockFd, &fdWriteReady);
            long lSelectResult = select(FD_SETSIZE, (fd_set*)0, &fdWriteReady,
                                        (fd_set*)0, &waitTime);
            if (lSelectResult <= 0)
            {
                IVS_RUN_LOG_ERR("wait client socket(%ld) time out", lSockFd);
                (void)CLOSESOCK((SOCKET)lSockFd);
                return VOS_FAIL;
            }

            long lErrorNo = 0;
            socklen_t len = sizeof(lErrorNo);
            if (getsockopt((SOCKET)lSockFd, SOL_SOCKET, SO_ERROR,
                           (SOCK_OPT_TYPE *)&lErrorNo, &len) < 0)
            {
                IVS_RUN_LOG_ERR("getsockopt of sockfd(%ld) has wrong when wait client", lSockFd);
                (void)CLOSESOCK((SOCKET)lSockFd);
                return VOS_FAIL;
            }
            else if (lErrorNo != 0)
            {
                IVS_RUN_LOG_ERR("wait client: socket(%ld) connect fail", lSockFd);
                (void)CLOSESOCK((SOCKET)lSockFd);
                return VOS_FAIL;
            }

            IVS_DBG_LOG("connect server OK. socket id = %ld", lSockFd);
            m_lConnStatus = enConnected;
        }
    }
    else
    {
        IVS_DBG_LOG("connect server OK. socket id = %ld", lSockFd);
    }

    return VOS_SUCCESS;
}


//�����������ģʽ
long CTcpConnHandle::connMode(const long lSockFd, const EnumSyncAsync bSyncConn, ULONG ulTimeOut)
{
    //�������Ϊ������ģʽ���ָ�Ϊ����ģʽ
    if ((enAsyncOp == bSyncConn) || (ulTimeOut > 0))
    {
#if VOS_APP_OS == VOS_OS_LINUX
        //�ָ�Ϊ����ģʽ
        if (fcntl(lSockFd, F_SETFL, fcntl(lSockFd, F_GETFL & (~O_NONBLOCK))) < 0)
#elif VOS_APP_OS == VOS_OS_WIN32
        ULONG ulBlock = 0;
        if (SOCKET_ERROR == ioctlsocket((SOCKET)lSockFd, (long)FIONBIO, &ulBlock))
#endif
        {
            IVS_RUN_LOG_ERR("fcntl client socket error(%d)", CONN_ERRNO);
            (void)CLOSESOCK((SOCKET)lSockFd);
            return VOS_FAIL;
        }

        if (enAsyncOp == bSyncConn)
        {
            m_lConnStatus = enConnecting;
        }
    }
    else
    {
        m_lConnStatus = enConnected;
    }


    return VOS_SUCCESS;
}

long CTcpConnHandle::send(const char *pArrayData, const ULONG ulDataSize,
                          const EnumSyncAsync bSyncSend)
{
    if (InvalidSocket == m_lSockFD)
    {
        IVS_RUN_LOG_ERR("socket is invalid, send fail");
        return SendRecvError;
    }

    errno = 0;
    long lBytesSent = 0;
    if (enSyncOp == bSyncSend)
    {
        //ͬ������
#if VOS_APP_OS == VOS_OS_WIN32
        ULONG ulBlock = VOS_FALSE;
        if (SOCKET_ERROR == ioctlsocket((SOCKET)m_lSockFD, (long)FIONBIO, &ulBlock))
        {
            IVS_RUN_LOG_ERR("Set Socket Block fail.");
            return SendRecvError;
        }
#endif

        lBytesSent = ::send((SOCKET)m_lSockFD, pArrayData, (int)ulDataSize, MSG_NOSIGNAL);
    }
    else
    {
        //�첽����
#if VOS_APP_OS == VOS_OS_WIN32
        ULONG ulBlock = VOS_TRUE;
        if (SOCKET_ERROR == ioctlsocket((SOCKET)m_lSockFD, (long)FIONBIO, &ulBlock))
        {
            IVS_RUN_LOG_ERR("Set Socket NoBlock fail.");
            return SendRecvError;
        }
#endif

        lBytesSent = ::send((SOCKET)m_lSockFD, pArrayData, (int)ulDataSize,
                            MSG_DONTWAIT | MSG_NOSIGNAL);

        //��ʼ����Ƿ���Է�������
        setHandleSend(VOS_TRUE);
    }

    //�������ʧ�ܣ������ж��Ƿ�����Ϊ�������ǵĻ����ط���0�ֽڣ�����ر�����
    if (lBytesSent < 0)
    {
        if ((EWOULDBLOCK == CONN_ERRNO) || (EAGAIN == CONN_ERRNO))
        {
            return 0;
        }

        (void)CLOSESOCK((SOCKET)m_lSockFD);
        m_lSockFD = InvalidSocket;

        IVS_RUN_LOG_ERR("CTcpConnHandle::send to peer(IP:0x%x, Port:%d) "
                       "Error(%d): %s", ntohl((ULONG)(m_peerAddr.m_lIpAddr)),
                       ntohs(m_peerAddr.m_usPort), CONN_ERRNO, strerror(CONN_ERRNO));

        return SendRecvError;
    }

    return lBytesSent;
}

long CTcpConnHandle::send_n(const char *pArrayData, const ULONG ulDataSize)
{
    long lTotalSend = 0;
    while ((ulDataSize - static_cast<ULONG>(lTotalSend)) > 0) //ѭ������
    {
        long lSend = this->send(pArrayData + lTotalSend, ulDataSize - static_cast<ULONG>(lTotalSend), enSyncOp);
        if (lSend < 0)
        {
			IVS_RUN_LOG_ERR("Net send Failed");
            return SendRecvError;
        }

        lTotalSend += lSend;
    }

    return lTotalSend;
}

long CTcpConnHandle::recv(char *pArrayData, CNetworkAddr *pPeerAddr,
                          const ULONG ulDataSize, const EnumSyncAsync bSyncRecv)
{
    if (InvalidSocket == m_lSockFD)
    {
        IVS_RUN_LOG_ERR("CTcpConnHandle::recv: socket is invalid, recv fail");
        return SendRecvError;
    }

    errno = 0;
    long lBytesRecv = 0;
    if (enSyncOp == bSyncRecv)
    {
        //ͬ������
#if VOS_APP_OS == VOS_OS_WIN32
        ULONG ulBlock = VOS_FALSE;
        if (SOCKET_ERROR == ioctlsocket((SOCKET)m_lSockFD, (long)FIONBIO, &ulBlock))
        {
            IVS_RUN_LOG_ERR("Set Socket Block fail.");
            return SendRecvError;
        }
#endif

        lBytesRecv = ::recv((SOCKET)m_lSockFD, pArrayData, 4096 /*(int)ulDataSize*/, 0 /*////MSG_WAITALL*/);
    }
    else
    {
        //�첽����
#if VOS_APP_OS == VOS_OS_WIN32
        ULONG ulBlock = VOS_TRUE;
        if (SOCKET_ERROR == ioctlsocket((SOCKET)m_lSockFD, (long)FIONBIO, &ulBlock))
        {
            IVS_RUN_LOG_ERR("Set Socket NoBlock fail.");
            return SendRecvError;
        }
#endif

        lBytesRecv = ::recv((SOCKET)m_lSockFD, pArrayData, (int)ulDataSize, MSG_DONTWAIT);
    }

    //�������0����ʾ�Ѿ�����
    if (0 == lBytesRecv)
    {
        IVS_DBG_LOG("recv EOF!");
        return SendRecvError;
    }

    //���С��0�������ж��Ƿ�����Ϊ�������ǵĻ����ؽ���0�ֽڣ������������
    if (lBytesRecv < 0)
    {
        if ((EWOULDBLOCK == CONN_ERRNO) || (EAGAIN == CONN_ERRNO))
        {
            return 0;
        }

        IVS_DBG_LOG("recv error. Error(%d): %s", CONN_ERRNO, strerror(CONN_ERRNO));
        return SendRecvError;
    }

    if (NULL != pPeerAddr)
    {
        pPeerAddr->m_lIpAddr = m_peerAddr.m_lIpAddr;
        pPeerAddr->m_usPort = m_peerAddr.m_usPort;
    }

    return lBytesRecv;
}

long CTcpConnHandle::recvWithTimeout(char *pArrayData, CNetworkAddr *pPeerAddr,
                                     const ULONG ulDataSize, const ULONG ulTimeOut, const ULONG ulSleepTime)
{
    (void)ulSleepTime; //��PC-LINT
    long lRecvBytes = 0;
    ULONG ulTotalRecvBytes = 0;
    ULONG ulWaitTime = ulTimeOut;
    errno = 0;

    //����socket��ʱʱ��
#if VOS_APP_OS == VOS_OS_WIN32
    if (setsockopt((SOCKET)m_lSockFD, SOL_SOCKET, SO_RCVTIMEO,
                   (char *) &ulWaitTime, sizeof(int)) < 0)
    {
        (void)CLOSESOCK((SOCKET)m_lSockFD);
        IVS_RUN_LOG_ERR("setsockopt socket SO_RCVTIMEO  error(%d)\n", CONN_ERRNO);
        return SendRecvError;
    }

#elif VOS_APP_OS == VOS_OS_LINUX
    struct timeval recvWaitTime;
    recvWaitTime.tv_sec  = ulWaitTime / CONN_SECOND_IN_MS;
    recvWaitTime.tv_usec = (ulWaitTime % CONN_SECOND_IN_MS) * CONN_MS_IN_US;
    if (setsockopt((SOCKET)m_lSockFD, SOL_SOCKET, SO_RCVTIMEO,
                   (char *) &recvWaitTime, sizeof(recvWaitTime)) < 0)
    {
        (void)CLOSESOCK((SOCKET)m_lSockFD);
        IVS_RUN_LOG_ERR("setsockopt socket SO_RCVTIMEO  error(%d)\n", CONN_ERRNO);
        return SendRecvError;
    }
#endif


    //windows����ط��޷�ȫ�������꣬��Ϊѭ������
    //lRecvBytes = this->recv(pArrayData, pPeerAddr, ulDataSize, enSyncOp);

    //windows ѭ������ linux�˴�ֻ����һ��ѭ��
    if (NULL == pArrayData)
    {
		IVS_RUN_LOG_ERR("Tcp recv Failed, buffer is null");
        return SendRecvError;
    }

    while (ulTotalRecvBytes < ulDataSize)
    {
        lRecvBytes = this->recv(pArrayData + ulTotalRecvBytes, pPeerAddr, ulDataSize - ulTotalRecvBytes, enSyncOp);

        /***BEGIN  V100R001C01 MODIFY For �Է������� Modify 2009-06-22 c60705    **********/

        //        if(lRecvBytes <= 0) //recv���������ܷ���0�����ں����ڲ��Ѿ���0����ֵ���˴���
        if (lRecvBytes < 0)
        /***END    V100R001C01 MODIFY For �Է������� Modify 2009-06-22 c60705    **********/
        {
            break;
        }

        ulTotalRecvBytes += (unsigned long)lRecvBytes;
    }

    if (lRecvBytes < 0)
    {
        IVS_DBG_LOG("socket closed when receive. "
                       "m_lSockFD = %ld, peer_ip(0x%x), peer_port(%d) "
                       "errno = %d, error: %s", m_lSockFD,
                       ntohl((ULONG)(m_peerAddr.m_lIpAddr)), ntohs(m_peerAddr.m_usPort),
                       CONN_ERRNO, strerror(CONN_ERRNO));
        if (CONN_ERR_TIMEO == CONN_ERRNO)
        {
            return SendRecvErrorTIMEO;
        }

        if (CONN_ERR_EBADF == CONN_ERRNO)
        {
            return SendRecvErrorEBADF;
        }

        return SendRecvError;
    }

    /***BEGIN  V100R001C01 DELETE For �Է������� Modify 2009-06-22 c60705    **********/

    //    if(0 == lRecvBytes)
    //    {
    //        IVS_DBG_LOG((char *)"FILE(%s)LINE(%d): "
    //            "CTcpConnHandle::recvWithTimeout: recv time out. "
    //            "m_lSockFD = %d, peer_ip(0x%x), peer_port(%d) recv_msg_len(%lu)"
    //            "ulDataSize(%lu) errno = %d, error: %s", _FL_, m_lSockFD,
    //            ntohl((ULONG)(m_peerAddr.m_lIpAddr)), ntohs(m_peerAddr.m_usPort),
    //            ulTotalRecvBytes, ulDataSize,CONN_ERRNO, strerror(CONN_ERRNO) );
    //        return SendRecvErrorEOF;
    //    }
    /***END    V100R001C01 DELETE For �Է������� Modify 2009-06-22 c60705    **********/

    if (ulTotalRecvBytes < ulDataSize) //˵�����ܳ�ʱ
    {
        IVS_DBG_LOG("CTcpConnHandle::recvWithTimeout: recv time out. "
                       "m_lSockFD = %ld, peer_ip(0x%x), peer_port(%d) recv_msg_len(%lu)"
                       "ulDataSize(%lu) errno = %d, error: %s", m_lSockFD,
                       ntohl((ULONG)(m_peerAddr.m_lIpAddr)), ntohs(m_peerAddr.m_usPort),
                       ulTotalRecvBytes, ulDataSize, CONN_ERRNO, strerror(CONN_ERRNO));
        return SendRecvError;
    }

    //����socket��ʱʱ��Ϊ0��ʾ���õȴ�
#if VOS_APP_OS == VOS_OS_WIN32
    ulWaitTime = 0;
    if (setsockopt((SOCKET)m_lSockFD, SOL_SOCKET, SO_RCVTIMEO,
                   (char *) &ulWaitTime, sizeof(int)) < 0)
    {
        (void)CLOSESOCK((SOCKET)m_lSockFD);
        IVS_RUN_LOG_ERR("setsockopt socket SO_RCVTIMEO  error(%d)\n", CONN_ERRNO);
        return SendRecvError;
    }

#elif VOS_APP_OS == VOS_OS_LINUX
    //����socket��ʱʱ��Ϊ0��ʾ���õȴ�
    recvWaitTime.tv_sec  = 0;
    recvWaitTime.tv_usec = 0;
    if (setsockopt((SOCKET)m_lSockFD, SOL_SOCKET, SO_RCVTIMEO,
                   (char *) &recvWaitTime, sizeof(recvWaitTime)) < 0)
    {
        (void)CLOSESOCK((SOCKET)m_lSockFD);
        IVS_RUN_LOG_ERR("setsockopt socket SO_RCVTIMEO  error(%d)\n", CONN_ERRNO);
        return SendRecvError;
    }
#endif


    return (long)ulTotalRecvBytes;
}

void CTcpConnHandle::close(void)
{
    if (m_pMutexHandle != NULL)
    {
        if (VOS_OK != VOS_MutexLock(m_pMutexHandle))
        {
			IVS_RUN_LOG_ERR("Tcp close lock Failed");
            return;
        }
    }

    if (InvalidSocket != m_lSockFD)
    {
        IVS_DBG_LOG("CTcpConnHandle::close: close connection, "
                       "m_lSockFD = %ld, peer_ip(0x%x), "
                       "peer_port(%d) this(0x%x) m_pHandleNode(0x%x)",
                       m_lSockFD,
                       ntohl((ULONG)(m_peerAddr.m_lIpAddr)), ntohs(m_peerAddr.m_usPort), this,
                       this->m_pHandleNode);

        //The close of an fd will cause it to be removed from
        //all epoll sets automatically.
#if VOS_APP_OS == VOS_OS_LINUX
        struct epoll_event epEvent;
        memset(&epEvent, 0, sizeof(epEvent));
        epEvent.data.ptr = (void *)NULL;

        //����Ҫ������¼�����
        epEvent.events = (EPOLLIN | EPOLLOUT);

        //�޸�ע���epoll�¼�
        if (0 != epoll_ctl(m_lEpfd, EPOLL_CTL_MOD, m_lSockFD, &epEvent))
        {
            IVS_RUN_LOG_ERR("modify event fail, SockFD = %ld", m_lSockFD);
        }

        //������Ҫ������¼���ص��ļ�������
        //epEvent.data.ptr = (void *)m_pHandleNode;
        //����Ҫ������¼�����
        //epEvent.events = EPOLLIN;
        if (0 != epoll_ctl(m_lEpfd, EPOLL_CTL_DEL, m_lSockFD, &epEvent))
        {
            IVS_RUN_LOG_ERR(close: epoll_ctl EPOLL_CTL_DEL fail, "
                           "SockFD = %ld", m_lSockFD);
        }
#endif

        (void)CLOSESOCK((SOCKET)m_lSockFD);
        m_lSockFD = InvalidSocket;
    }

    m_lConnStatus = enClosed;

    CHandle::close();

    if (m_pMutexHandle != NULL)
    {
        (void)VOS_MutexUnlock(m_pMutexHandle);
    }

    return;
}

long CTcpServerHandle::listen(const CNetworkAddr *pLocalAddr)
{
    long lSockFd = (long)socket(AF_INET, SOCK_STREAM, 0);

    if (lSockFd < 0)
    {
        IVS_RUN_LOG_ERR("opening client socket error(%d)", CONN_ERRNO);
        return VOS_FAIL;
    }

    //setSendBufSize
    long lSendBufSize = DEFAULT_SENDRECV_SIZE;
    socklen_t lSendBufLength = sizeof(lSendBufSize);
    if (setsockopt((SOCKET)lSockFd, SOL_SOCKET, SO_SNDBUF, (char*)&lSendBufSize,
                   lSendBufLength) < 0)
    {
        (void)CLOSESOCK((SOCKET)lSockFd);
        IVS_RUN_LOG_ERR("setSendBufSize client socket error(%d)", CONN_ERRNO);
        return VOS_FAIL;
    }

    //setRecBufSize
    long lRecvBufSize = DEFAULT_SENDRECV_SIZE;
    socklen_t lRecvBufLength = sizeof(lRecvBufSize);
    if (setsockopt((SOCKET)lSockFd, SOL_SOCKET, SO_RCVBUF, (char*)&lRecvBufSize,
                   lRecvBufLength) < 0)
    {
        (void)CLOSESOCK((SOCKET)lSockFd);
        IVS_RUN_LOG_ERR("setRecvBufSize client socket error(%d)", CONN_ERRNO);
        return VOS_FAIL;
    }

#if VOS_APP_OS == VOS_OS_LINUX
    //setReuseAddr();
    long lReuseAddrFlag = 1;
    if (setsockopt((SOCKET)lSockFd, SOL_SOCKET, SO_REUSEADDR, (char*)&lReuseAddrFlag,
                   sizeof(lReuseAddrFlag)) < 0)
    {
        (void)CLOSESOCK((SOCKET)lSockFd);
        IVS_RUN_LOG_ERR("setsockopt client socket error(%d)", CONN_ERRNO);
        return VOS_FAIL;
    }
#endif


    //�󶨱��ص�ַ
    struct sockaddr_in serverAddr;
    memset((char *)&serverAddr, 0, (long)sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = (UINT)pLocalAddr->m_lIpAddr;
    serverAddr.sin_port = pLocalAddr->m_usPort;
    errno = 0;
    if (0 > bind ((SOCKET)lSockFd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)))
    {
#if VOS_APP_OS == VOS_OS_LINUX
        char szServerAddr[INET_ADDRSTRLEN];
        if (NULL != inet_ntop(AF_INET, &serverAddr.sin_addr, szServerAddr,
                              sizeof(szServerAddr)))
#elif VOS_APP_OS == VOS_OS_WIN32
        char *szServerAddr = inet_ntoa(serverAddr.sin_addr);
        if (NULL != szServerAddr)
#endif
        {
            IVS_RUN_LOG_ERR("Can not Bind Data_Sock %s:%d",
                           szServerAddr, ntohs(serverAddr.sin_port));
        }
        else
        {
            IVS_RUN_LOG_ERR("Can not Bind Data_Sock %d:%d",
                           serverAddr.sin_addr.s_addr, ntohs(serverAddr.sin_port));
        }

        (void)CLOSESOCK((SOCKET)lSockFd);
        return VOS_FAIL;
    }

    //��������
    errno = 0;
    if (::listen((SOCKET)lSockFd, MAX_LISTEN_QUEUE_SIZE) < 0)
    {
        IVS_RUN_LOG_ERR("listen Error(%d):%s.", CONN_ERRNO, strerror(CONN_ERRNO));
        (void)CLOSESOCK((SOCKET)lSockFd);
        return VOS_FAIL;
    }

    m_lSockFD = lSockFd;

    return VOS_SUCCESS;
}

long CTcpServerHandle::handle_accept(const CNetworkAddr *pRemoteAddr,
                                     CTcpConnHandle *&   pTcpConnHandle)
{
    return VOS_FAIL;
}

void CTcpServerHandle::close(void)
{
    if (m_pMutexHandle != NULL)
    {
        if (VOS_OK != VOS_MutexLock(m_pMutexHandle))
        {
			IVS_RUN_LOG_ERR("Tcp close lock Failed");
            return;
        }
    }

    if (InvalidSocket != m_lSockFD)
    {
        //The close of an fd will cause it to be removed from
        //all epoll sets automatically.
        (void)CLOSESOCK((SOCKET)m_lSockFD);
        m_lSockFD = InvalidSocket;
    }

    CHandle::close();

    if (m_pMutexHandle != NULL)
    {
        (void)VOS_MutexUnlock(m_pMutexHandle);
    }

    return;
}

CHandleManager::CHandleManager() : m_pMutexListOfHandle(NULL)
								 , m_ulSelectPeriod(DEFAULT_SELECT_PERIOD)
								 , m_pVosThread(NULL)
								 , m_bExit(VOS_FALSE)
{
#if VOS_APP_OS == VOS_OS_LINUX
    m_lEpfd = InvalidFd;
    memset(m_epEvents, 0, sizeof(m_epEvents));
#elif VOS_APP_OS == VOS_OS_WIN32
    //��ʼ��select��
    FD_ZERO(&m_readSet);
    FD_ZERO(&m_writeSet);
    m_stSelectPeriod.tv_sec  = 0;
    m_stSelectPeriod.tv_usec = 0;
#endif
	m_curListIte = m_listHandle.end();
    memset(m_szMgrType, 0, sizeof(m_szMgrType));
}

CHandleManager::~CHandleManager()
{
    try
    {
		m_bExit = VOS_TRUE;
#if VOS_APP_OS == VOS_OS_LINUX
		pthread_t pt = VOS_pthread_self();
        IVS_DBG_LOG("manager type: %s. thread = %d, m_lEpfd = %d",
                       m_szMgrType, pt, m_lEpfd);
#elif VOS_APP_OS == VOS_OS_WIN32
		HANDLE Handle = VOS_pthread_self();
        IVS_DBG_LOG("manager type: %s. thread = %x ", m_szMgrType, Handle);
#endif

        ListOfHandleIte itListOfHandle = m_listHandle.begin();
        while (itListOfHandle != m_listHandle.end()) //lint !e81
        {
            if ((*itListOfHandle)->m_pHandle != NULL) //lint !e40 !e48
            {
                (*itListOfHandle)->m_pHandle->close(); //lint !e40 !e48 !e1055
            }

#if VOS_APP_OS == VOS_OS_LINUX
#endif

            //����Ӧ��HandleNodeɾ��
            VOS_DELETE(*itListOfHandle); //lint !e48 !e1025 !e1058
            ++itListOfHandle; //lint !e53
        }

#if VOS_APP_OS == VOS_OS_LINUX
        if (m_lEpfd != InvalidFd)
        {
            (void)CLOSESOCK(m_lEpfd);
            m_lEpfd = InvalidFd;
        }
#endif


        if (m_pVosThread != NULL)
        {
            VOS_free(m_pVosThread);
        }

        m_pVosThread = NULL;

        if (m_pMutexListOfHandle != NULL)
        {
            if (VOS_OK == VOS_DestroyMutex(m_pMutexListOfHandle))
            {
                m_pMutexListOfHandle = NULL;
            }
        }

        m_pMutexListOfHandle = NULL;
    }
    catch (...)
    {}
}

long CHandleManager::init(const ULONG ulSelectPeriod)
{
    if (0 == ulSelectPeriod)
    {
        m_ulSelectPeriod = DEFAULT_SELECT_PERIOD;
    }
    else
    {
        m_ulSelectPeriod = ulSelectPeriod;
    }

#if VOS_APP_OS == VOS_OS_LINUX
    m_lEpfd = epoll_create(MAX_EPOLL_FD_SIZE);

    if (m_lEpfd < 0)
    {
        m_lEpfd = InvalidFd;
        IVS_RUN_LOG_ERR("manager type: %s", m_szMgrType);
        return VOS_FAIL;
    }

#elif VOS_APP_OS == VOS_OS_WIN32
    //��ulSelectPeriodת����timeval�ṹ
    m_stSelectPeriod.tv_sec  = (long)(ulSelectPeriod / CONN_SECOND_IN_MS);
    m_stSelectPeriod.tv_usec = (ulSelectPeriod % CONN_SECOND_IN_MS) * CONN_MS_IN_US;

    //��ʼ��select��
    FD_ZERO(&m_readSet);
    FD_ZERO(&m_writeSet);
#endif

    m_pMutexListOfHandle = VOS_CreateMutex();
    if (NULL == m_pMutexListOfHandle)
    {
#if VOS_APP_OS == VOS_OS_LINUX
        close(m_lEpfd);
        m_lEpfd = InvalidFd;
#endif
        IVS_RUN_LOG_ERR("manager type: %s", m_szMgrType);
        return VOS_FAIL;
    }

    return VOS_SUCCESS;
}

long CHandleManager::run()
{
    errno = 0;
    if (VOS_OK != VOS_CreateThread((VOS_THREAD_FUNC)invoke, (void *)this,
                                   &m_pVosThread, VOS_DEFAULT_STACK_SIZE))
    {
        IVS_RUN_LOG_ERR("Create play thread failed. manager type: %s. error(%d):%s",
                       m_szMgrType, CONN_ERRNO, strerror(CONN_ERRNO));
        return VOS_FAIL;
    }
    return VOS_SUCCESS;
}

void *CHandleManager::invoke(void *argc)
{
    CHandleManager *pHandleManager = (CHandleManager *)argc;

    IVS_DBG_LOG("invoke mainLoop",pHandleManager->m_szMgrType);
    pHandleManager->mainLoop();
    VOS_pthread_exit(NULL);

    return NULL;
}

#if VOS_APP_OS == VOS_OS_WIN32

void CHandleManager::mainLoop()
{
    while (VOS_FALSE == m_bExit)
    {
        errno = 0;
        long lWaitFds = 0;

        //����
        if (VOS_OK != VOS_MutexLock(m_pMutexListOfHandle))
        {
            break;
        }

        FD_ZERO(&m_readSet);
        FD_ZERO(&m_writeSet);

        //��ulSelectPeriodת����timeval�ṹ
        m_stSelectPeriod.tv_sec  = (long)(m_ulSelectPeriod / CONN_SECOND_IN_MS);
        m_stSelectPeriod.tv_usec =
            (m_ulSelectPeriod % CONN_SECOND_IN_MS) * CONN_MS_IN_US;

        setSelectEvent();

        //����
        (void)VOS_MutexUnlock(m_pMutexListOfHandle);

        //��û��Ҫ����socket
        if ((0 == m_readSet.fd_count) && (0 == m_writeSet.fd_count))
        {
            Sleep(m_ulSelectPeriod);
            continue;
        }
        else
        {
            if (0 == m_readSet.fd_count)
            {
                lWaitFds = select(0, NULL, &m_writeSet, NULL, &m_stSelectPeriod);
            }
            else
            {
                if (0 == m_writeSet.fd_count)
                {
                    lWaitFds = select(0, &m_readSet, NULL, NULL, &m_stSelectPeriod);
                }
                else
                {
                    lWaitFds = select(0, &m_readSet, &m_writeSet, NULL, &m_stSelectPeriod);
                }
            }
        }

        if (0 == lWaitFds)
        {
            continue;
        }

        if (SOCKET_ERROR == lWaitFds)
        {
            IVS_RUN_LOG_ERR("select failed: manager type: %s. errno = %d",
                           m_szMgrType, CONN_ERRNO);

            //�����select����֮ǰFD_SET�����е�socket��close��
            //select�����ᱨWSAENOTSOCK��10038������, break�ᵼ���߳��˳�
            //break;
            continue;
        }

        //����
        if (VOS_OK != VOS_MutexLock(m_pMutexListOfHandle))
        {
            break;
        }

        netEventDeal();

        //����
        (void)VOS_MutexUnlock(m_pMutexListOfHandle);
    }

    return;
}

void CHandleManager::setSelectEvent()
{
	/****************************
	2012-12-26 ������
	����ĵ���Ҫ��֧��256����������������Ϊʹ��selectģ�ͣ�fd_set�ṹ�����֧��64��
	Ϊ���ܹ���������64�����ӵ��¼���ʹ��ÿ�β�64������α�����m_listHandle�����ʹ��
	��m_curListIte,����¼�ϴα�����λ�á�
	*****************************/
    if(m_curListIte == m_listHandle.end())
	{
		m_curListIte = m_listHandle.begin();
	}
	int count = 0;
    while (count<64 && m_curListIte != m_listHandle.end()) //lint !e81
    {
		count++;
        //BEGIN V100R002C01 MOD 2011-01-17 ligengqiang l00124479 for �ڴ�й¶
        CHandle *pHandle = NULL;
        long lSockFd = InvalidSocket;

        CHandleNode *pHandleNode = *m_curListIte; //lint !e48
		if(NULL==pHandleNode)
		{
			 m_curListIte = m_listHandle.erase(m_curListIte); //lint !e1025
			 continue;
		}
		else if (VOS_TRUE == pHandleNode->m_bRemoved)
        {
            m_curListIte = m_listHandle.erase(m_curListIte); //lint !e1025
            VOS_DELETE(pHandleNode);
            continue;
        }
        else
        {
            pHandle = pHandleNode->m_pHandle;
			lSockFd = pHandle->m_lSockFD;
			pHandle->m_bReadSelected  = VOS_FALSE;
			pHandle->m_bWriteSelected = VOS_FALSE;
			if (InvalidSocket != lSockFd)
			{
				ULONG ulEvent = pHandle->getEvents();
				if ((EPOLLIN==(ulEvent & EPOLLIN)) && !FD_ISSET(lSockFd, &m_readSet))
				{
					//if (!FD_ISSET(lSockFd, &m_readSet))
					//{
						FD_SET((SOCKET)lSockFd, &m_readSet);
						pHandle->m_bReadSelected = VOS_TRUE;
					//}
				}

				if ((EPOLLOUT==(ulEvent & EPOLLOUT)) && !FD_ISSET(lSockFd, &m_writeSet))
				{
					//if (!FD_ISSET(lSockFd, &m_writeSet))
					//{
						FD_SET((SOCKET)lSockFd, &m_writeSet);
						pHandle->m_bWriteSelected = VOS_TRUE;
					//}
				}
			}
        }

        //END    V100R002C01 MOD 2011-01-17 ligengqiang l00124479 for �ڴ�й¶

        ++m_curListIte; //lint !e53
    }

}

void CHandleManager::netEventDeal()
{
    CHandleNode *pHandleNode = NULL;
    for (ListOfHandleIte it = m_listHandle.begin();
         it != m_listHandle.end(); ++it) //lint !e53 !e81
    {
        pHandleNode = *it; //lint !e48
        if (VOS_TRUE == pHandleNode->m_bRemoved)
        {
            continue;
        }

        //����Ƿ�Ϊ������
        CHandle *pHandle = pHandleNode->m_pHandle;
        if (NULL!=pHandle && pHandle->m_lSockFD != InvalidSocket)
        {
            if (FD_ISSET(pHandle->m_lSockFD, &m_readSet)
                && (VOS_TRUE == pHandle->m_bReadSelected))

            {
                this->checkSelectResult(enEpollRead, pHandleNode->m_pHandle);
            }

            //����Ƿ�Ϊд����
            if (FD_ISSET(pHandle->m_lSockFD, &m_writeSet)
                && (VOS_TRUE == pHandle->m_bWriteSelected))
            {
                this->checkSelectResult(enEpollWrite, pHandleNode->m_pHandle);
            }
         }
     }
}



#endif

#if VOS_APP_OS == VOS_OS_LINUX

void CHandleManager::mainLoop()
{
    while (VOS_FALSE == m_bExit)
    {
        errno = 0;
        long lWaitFds = epoll_wait(m_lEpfd, m_epEvents, EPOLL_MAX_EVENT,
                                   (long)m_ulSelectPeriod);
        if (0 == lWaitFds)
        {
            continue;
        }

        if (0 > lWaitFds)
        {
            if (EINTR == CONN_ERRNO)
            {
                continue;
            }

            IVS_RUN_LOG_ERR("epoll_wait failed: manager type: %s. errno(%d):%s",
                           m_szMgrType, CONN_ERRNO, strerror(CONN_ERRNO));
            break;
        }

        //����
        if (VOS_OK != VOS_MutexLock(m_pMutexListOfHandle))
        {
            break;
        }

        CHandleNode *pHandleNode = NULL;
        for (long i = 0; i < lWaitFds; ++i)
        {
            pHandleNode = (CHandleNode *)(m_epEvents[i].data.ptr);
            if (NULL == pHandleNode)
            {
                IVS_RUN_LOG_ERR("pHandleNode is NULL, sequence = %ld", i);
                continue;
            }

            //���������m_bRemoved��ֵֻ��2�֣�Ϊ�˷�ֹ�ڴ汻��д������ֵ��
            //��������:(VOS_FALSE != pHandleNode->m_bRemoved)
            if ((VOS_TRUE == pHandleNode->m_bRemoved)
               || (VOS_FALSE != pHandleNode->m_bRemoved))
            {
                continue;
            }

            CHandle *pHandle = pHandleNode->m_pHandle;
            if (NULL == pHandle)
            {
                IVS_RUN_LOG_ERR("pHandle is NULL, sequence = %ld", i);
                continue;
            }

            //ͨ���¼����ͼ���Ƿ�Ϊ������
            if (m_epEvents[i].events & EPOLLIN)
            {
                this->checkSelectResult(enEpollRead, pHandle);
            }

            //ͨ���¼����ͼ���Ƿ�Ϊд����
            if (m_epEvents[i].events & EPOLLOUT)
            {
                this->checkSelectResult(enEpollWrite, pHandle);
            }
        }

        ListOfHandleIte itListOfHandle = m_listHandle.begin();
        while (itListOfHandle != m_listHandle.end())
        {
            if (VOS_TRUE == (*itListOfHandle)->m_bRemoved)
            {
                IVS_DBG_LOG("removed, pHandleNode = 0x%x", (long)(*itListOfHandle));

                //����Ӧ��HandleNodeɾ��
                pHandleNode = *itListOfHandle;
                itListOfHandle = m_listHandle.erase(itListOfHandle);
                VOS_DELETE(pHandleNode);
                continue;
            }

            ++itListOfHandle;
        }

        //����
        (void)VOS_MutexUnlock(m_pMutexListOfHandle);
    }
    return;
}

#endif

void CHandleManager::exit()
{
    if(VOS_TRUE==this->m_bExit)
        return;
    if (NULL == m_pVosThread)
    {
        IVS_RUN_LOG_ERR("HandleManager exit VosThread is null");
        return;
    }

    this->m_bExit = VOS_TRUE;
    errno = 0;
    long ret_val = VOS_ThreadJoin(m_pVosThread);
    if (ret_val != VOS_OK)
    {
        IVS_RUN_LOG_ERR("Wait play thread exit failed. ret_val(%ld). error(%d):%s",
                       ret_val, CONN_ERRNO, strerror(CONN_ERRNO));
    }

    IVS_RUN_LOG_ERR("manager type: %s. exit complete."
                   "Thread = %d", m_szMgrType, m_pVosThread->pthead);

    return;
}

long CHandleManager::addHandle(CHandle *   pHandle,
                               VOS_BOOLEAN bIsListOfHandleLocked)
{
    if (NULL == pHandle)
    {
        IVS_RUN_LOG_ERR("add Handl Handle is NULL");
        return VOS_FAIL;
    }

    if (InvalidSocket == pHandle->m_lSockFD)
    {
        IVS_RUN_LOG_ERR("add Handle Handle's socket is invalid");
        return VOS_FAIL;
    }

    CHandleNode *pHandleNode = NULL;
    (void)VOS_NEW(pHandleNode);
    if (NULL == pHandleNode)
    {
        IVS_RUN_LOG_ERR("add Handle new HandleNode fail");
        return VOS_FAIL;
    }

    //����(�����mainloop����ͬһ�̲߳���Ҫ����)
    VOS_BOOLEAN bNeedLock = VOS_FALSE;
    VOS_BOOLEAN bLocked = VOS_FALSE;
    if (VOS_FALSE == bIsListOfHandleLocked) //û�мӹ�����Ҫ����
    {
        if (NULL == m_pVosThread)
        {
            bNeedLock = VOS_TRUE;
        }
        else
        {
            if (VOS_pthread_self() != m_pVosThread->pthead)
            {
                bNeedLock = VOS_TRUE;
            }
        }

        if (VOS_TRUE == bNeedLock)
        {
            if (VOS_OK != VOS_MutexLock(m_pMutexListOfHandle))
            {
                IVS_RUN_LOG_ERR("remove Handle get lock failed");
            }
            else
            {
                bLocked = VOS_TRUE;
            }
        }
    }

    if (VOS_SUCCESS != addHandle2List(pHandle, pHandleNode)) //��Ȼ�᷵�سɹ����������û���ͷ���
    {
		IVS_RUN_LOG_ERR("add handle to list Failed");
        return VOS_FAIL;
    }

    //����
    if (VOS_TRUE == bLocked)
    {
        if (VOS_OK != VOS_MutexUnlock(m_pMutexListOfHandle))
        {
            IVS_RUN_LOG_ERR("add Handle release lock failed");
        }
    }

    return VOS_SUCCESS;
}

long CHandleManager::addHandle2List(CHandle *   pHandle,
                                    CHandleNode *pHandleNode)
{
#if VOS_APP_OS == VOS_OS_LINUX
    //��handle��ӵ�epoll�ļ�����
    struct epoll_event epEvent;
    memset(&epEvent, 0, sizeof(epEvent));

    //������Ҫ������¼���ص��ļ�������
    epEvent.data.ptr = (void *)pHandleNode;

    //����Ҫ������¼�����
    epEvent.events = pHandle->getEvents();

    //ע��epoll�¼�
    errno = 0;
    if (0 != epoll_ctl(m_lEpfd, EPOLL_CTL_ADD, pHandle->m_lSockFD, &epEvent))
    {
        IVS_RUN_LOG_ERR("add Handle: add event fail errno = %d, error: %s", CONN_ERRNO, strerror(CONN_ERRNO));
        VOS_DELETE(pHandleNode);

        //����
        if (VOS_TRUE == bLocked)
        {
            if (VOS_OK != VOS_MutexUnlock(m_pMutexListOfHandle))
            {
                IVS_RUN_LOG_ERR("add Handle release lock failed");
            }
        }

        return VOS_FAIL;
    }
#endif

    pHandle->m_pHandleNode = pHandleNode;

#if VOS_APP_OS == VOS_OS_LINUX
    pHandle->m_lEpfd = m_lEpfd;
#endif
    pHandleNode->m_pHandle = pHandle;
#if VOS_APP_OS == VOS_OS_LINUX
    IVS_DBG_LOG("CHandleManager::addHandle: "
                   "new pHandleNode(0x%x) m_pHandle(0x%x) fd(%ld) Epfd(%ld)"
                   "peer_ip(0x%x) peer_port(%d)",
                   pHandleNode, pHandleNode->m_pHandle,
                   pHandleNode->m_pHandle->m_lSockFD, pHandleNode->m_pHandle->m_lEpfd,
                   pHandleNode->m_pHandle->m_localAddr.m_lIpAddr,
                   pHandleNode->m_pHandle->m_localAddr.m_usPort);
#elif VOS_APP_OS == VOS_OS_WIN32
    IVS_DBG_LOG("CHandleManager::addHandle: "
                   "new pHandleNode(0x%x) m_pHandle(0x%x) fd(%ld) "
                   "peer_ip(0x%x) peer_port(%d)",
                   pHandleNode, pHandleNode->m_pHandle,
                   pHandleNode->m_pHandle->m_lSockFD,
                   pHandleNode->m_pHandle->m_localAddr.m_lIpAddr,
                   pHandleNode->m_pHandle->m_localAddr.m_usPort);
#endif
    m_listHandle.push_back(pHandleNode);

    return VOS_SUCCESS;
}

void CHandleManager::removeHandle(CHandle *pHandle)
{
    if (NULL == pHandle)
    {
        IVS_RUN_LOG_ERR("remove Handle Handle is NULL");
        return;
    }

    //����(�����mainloop����ͬһ�̲߳���Ҫ����)
    VOS_BOOLEAN bNeedLock = VOS_FALSE;
    VOS_BOOLEAN bLocked = VOS_FALSE;
    if (NULL == m_pVosThread)
    {
        bNeedLock = VOS_TRUE;
    }
    else
    {
        if (VOS_pthread_self() != m_pVosThread->pthead)
        {
            bNeedLock = VOS_TRUE;
        }
    }

    if (VOS_TRUE == bNeedLock)
    {
        if (VOS_OK != VOS_MutexLock(m_pMutexListOfHandle))
        {
            IVS_RUN_LOG_ERR("remove Handle get lock failed");
        }
        else
        {
            bLocked = VOS_TRUE;
        }
    }

    if (NULL == pHandle->m_pHandleNode)
    {
        IVS_RUN_LOG_ERR("HandleNode is NULL");
    }
    else
    {
        if (pHandle->m_pHandleNode->m_bRemoved != VOS_TRUE)
        {
            pHandle->close();
            pHandle->m_pHandleNode->m_bRemoved = VOS_TRUE;
            pHandle->m_pHandleNode->m_pHandle = NULL;
        }
        else
        {
            IVS_RUN_LOG_ERR("Handle removed more than once");
        }

        pHandle->m_pHandleNode = NULL;
    }

    //����(�������ͬһ�߳�)
    if (VOS_TRUE == bLocked)
    {
        if (VOS_OK != VOS_MutexUnlock(m_pMutexListOfHandle))
        {
            IVS_RUN_LOG_ERR("remove Handle release lock failed");
        }
    }

    return;
}

void  CHandleManager::checkSelectResult(const EpollEventType enEpEvent,
                                CHandle *             pHandle)
{
}

void CTcpConnMgr::lockListOfHandle()
{
    if (VOS_OK != VOS_MutexLock(m_pMutexListOfHandle))
    {
        IVS_RUN_LOG_ERR("lockListOfHandle get lock failed");
    }
}

void CTcpConnMgr::unlockListOfHandle()
{
    if (VOS_OK != VOS_MutexUnlock(m_pMutexListOfHandle))
    {
        IVS_RUN_LOG_ERR("unlockListOfHandle release lock failed");
    }
}

void CTcpConnMgr::checkSelectResult(const EpollEventType enEpEvent,
                                    CHandle *            pHandle)
{
    if (NULL == pHandle)
    {
        IVS_RUN_LOG_ERR("checkSelectResult Handle is NULL");
        return;
    }

    CTcpConnHandle *pTcpConnHandle = dynamic_cast <CTcpConnHandle *>(pHandle);
    if (NULL == pTcpConnHandle)
    {
		IVS_RUN_LOG_ERR("TCP Handle is null");
        return;
    }

    //������¼�
    if (enEpollRead == enEpEvent)
    {
        //������¼����
        pTcpConnHandle->setHandleRecv(VOS_FALSE);

        //����handle��������¼�
        pTcpConnHandle->handle_recv();
    }

    //����д�¼�
    if (enEpollWrite == enEpEvent)
    {
        //���д�¼����
        pTcpConnHandle->setHandleSend(VOS_FALSE);

        //����Ƿ����ӳɹ�
        if (pTcpConnHandle->getStatus() == enConnecting)
        {
            long lErrorNo = 0;
            socklen_t len = sizeof(lErrorNo);
            if (getsockopt((SOCKET)(pTcpConnHandle->m_lSockFD), SOL_SOCKET, SO_ERROR,
                           (SOCK_OPT_TYPE *)&lErrorNo, &len) < 0)
            {
                IVS_RUN_LOG_ERR("getsockopt of sockfd has wrong");
                pTcpConnHandle->close();
                pTcpConnHandle->m_lConnStatus = enConnFailed;
            }
            else if (lErrorNo != 0)
            {
                IVS_RUN_LOG_ERR("getsockopt says sockfd is wrong");
                pTcpConnHandle->close();
                pTcpConnHandle->m_lConnStatus = enConnFailed;
            }

            pTcpConnHandle->m_lConnStatus = enConnected;
        }

        //����handle����д�¼�
        pTcpConnHandle->handle_send();
    }
}

void CTcpServerMgr::checkSelectResult(const EpollEventType enEpEvent,
                                      CHandle *            pHandle)
{
    if (NULL == pHandle)
    {
        IVS_RUN_LOG_ERR("checkSelectResult Handle is NULL");
        return;
    }

    if (NULL == m_pTcpConnMgr)
    {
        IVS_RUN_LOG_ERR("checkSelectResult TcpConnMgr is NULL.");
        return;
    }

    CTcpServerHandle *pTcpServerHandle = dynamic_cast <CTcpServerHandle *>(pHandle);
    if (NULL == pTcpServerHandle)
    {
		IVS_RUN_LOG_ERR("TCP Handle is null");
        return;
    }
   
    //�������ӵ����¼�
    if (VOS_SUCCESS != readEvent(enEpEvent, pTcpServerHandle))
    {
		IVS_RUN_LOG_ERR("read event failed");
        return;
    }

    //��Ӧ�ü�⵽д�¼�
    if (enEpollWrite == enEpEvent)
    {
        IVS_RUN_LOG_ERR("should not process write event");
    }
}

//�������ӵ����¼�
long CTcpServerMgr::readEvent(const EpollEventType enEpEvent, CTcpServerHandle *pTcpServerHandle)
{
    //�������ӵ����¼�
    if (enEpollRead == enEpEvent)
    {
		if(NULL==m_pTcpConnMgr)
		{
			IVS_RUN_LOG_ERR("read event tcp connmgr is null");
			return VOS_FAIL;
		}
        struct sockaddr_in peerAddr;
        memset(&peerAddr, 0, sizeof(struct sockaddr_in));

        //��������
        socklen_t len = sizeof(struct sockaddr_in);
        long lClientSockfd = InvalidFd;
        errno = 0;
        lClientSockfd = (long)::accept((SOCKET)(pTcpServerHandle->m_lSockFD),
                                       (struct sockaddr *)&peerAddr, &len);
        if (0 > lClientSockfd)
        {
            if ((EWOULDBLOCK != CONN_ERRNO) && (CONN_ERRNO != EAGAIN))
            {
                IVS_RUN_LOG_ERR("accept Error(%d): %s. ", CONN_ERRNO, strerror(CONN_ERRNO));
            }

            return VOS_FAIL;
        }

        //setSendBufSize
        long lSendBufSize = DEFAULT_SENDRECV_SIZE;
        socklen_t lSendBufLength = sizeof(lSendBufSize);
        if (setsockopt((SOCKET)lClientSockfd, SOL_SOCKET, SO_SNDBUF, (char*)&lSendBufSize,
                       lSendBufLength) < 0)
        {
            (void)CLOSESOCK((SOCKET)lClientSockfd);
            IVS_RUN_LOG_ERR("setSendBufSize client socket error(%d)", CONN_ERRNO);
            return VOS_FAIL;
        }

        //setRecBufSize
        long lRecvBufSize = DEFAULT_SENDRECV_SIZE;
        socklen_t lRecvBufLength = sizeof(lRecvBufSize);
        if (setsockopt((SOCKET)lClientSockfd, SOL_SOCKET, SO_RCVBUF, (char*)&lRecvBufSize,
                       lRecvBufLength) < 0)
        {
            (void)CLOSESOCK((SOCKET)lClientSockfd);
            IVS_RUN_LOG_ERR("setRecvBufSize client socket error(%d)", CONN_ERRNO);
            return VOS_FAIL;
        }

        long flag = 1;
        if (setsockopt((SOCKET)lClientSockfd, IPPROTO_TCP, TCP_NODELAY, (char*)&flag,
                       sizeof(flag)) < 0)
        {
            (void)CLOSESOCK((SOCKET)lClientSockfd);
            IVS_RUN_LOG_ERR("set TCP_NODELAY client socket error(%d)", CONN_ERRNO);
            return VOS_FAIL;
        }

        //setReuseAddr();
        long lReuseAddrFlag = 1;
        if (setsockopt((SOCKET)lClientSockfd, SOL_SOCKET, SO_REUSEADDR, (char*)&lReuseAddrFlag,
                       sizeof(lReuseAddrFlag)) < 0)
        {
            (void)CLOSESOCK((SOCKET)lClientSockfd);
            IVS_RUN_LOG_ERR("setsockopt client socket error(%d)", CONN_ERRNO);
            return VOS_FAIL;
        }

        //����server handle�������ӵ���
        CNetworkAddr clientAddr;
        clientAddr.m_lIpAddr = (LONG)peerAddr.sin_addr.s_addr;
        clientAddr.m_usPort = peerAddr.sin_port;
        CTcpConnHandle *pTcpConnHandle = NULL;

        /*�˴�����,ʹ�������ɵ�pTcpConnHandle,��removeTcpClient����*/
        m_pTcpConnMgr->lockListOfHandle();
        if (VOS_SUCCESS != pTcpServerHandle->handle_accept(&clientAddr, pTcpConnHandle))
        {
            (void)CLOSESOCK((SOCKET)lClientSockfd);
            IVS_RUN_LOG_ERR("accept fail.");
            m_pTcpConnMgr->unlockListOfHandle();
            return VOS_FAIL;
        }

        if (NULL == pTcpConnHandle)
        {
            (void)CLOSESOCK((SOCKET)lClientSockfd);
            IVS_RUN_LOG_ERR("TcpConnHandle is NULL");
            m_pTcpConnMgr->unlockListOfHandle();
            return VOS_FAIL;
        }

        if (VOS_SUCCESS != pTcpConnHandle->initHandle())
        {
            IVS_RUN_LOG_ERR("TcpConnHandle init fail");
            m_pTcpConnMgr->unlockListOfHandle();
            return VOS_FAIL;
        }

        pTcpConnHandle->m_lSockFD = lClientSockfd;
        pTcpConnHandle->m_localAddr.m_lIpAddr = pTcpServerHandle->m_localAddr.m_lIpAddr;
        pTcpConnHandle->m_localAddr.m_usPort = pTcpServerHandle->m_localAddr.m_usPort;
        pTcpConnHandle->m_lConnStatus = enConnected;
        pTcpConnHandle->m_peerAddr.m_lIpAddr = clientAddr.m_lIpAddr;
        pTcpConnHandle->m_peerAddr.m_usPort = clientAddr.m_usPort;

        VOS_BOOLEAN bIsListOfHandleLocked = VOS_TRUE;
        if (VOS_SUCCESS != m_pTcpConnMgr->addHandle(pTcpConnHandle,
                                                    bIsListOfHandleLocked))
        {
            IVS_RUN_LOG_ERR("checkSelectResult addHandle fail.");
            pTcpConnHandle->close();
        }

        m_pTcpConnMgr->unlockListOfHandle();
        IVS_DBG_LOG("CTcpServerMgr::checkSelectResult: accept connect, "
                       "m_lSockFD = %ld, peer_ip(0x%x), peer_port(%d)",
                       pTcpConnHandle->m_lSockFD,
                       ntohl((ULONG)(pTcpConnHandle->m_peerAddr.m_lIpAddr)),
                       ntohs(pTcpConnHandle->m_peerAddr.m_usPort));
    }

    return VOS_SUCCESS;
}

CConnMgr::CConnMgr() : m_lLocalIpAddr(InvalidIp)
					 , m_pTcpConnMgr(NULL)
					 , m_pTcpServerMgr(NULL)
{

}

CConnMgr::~CConnMgr()
{
    try
    {
        exit();
        VOS_DELETE(m_pTcpConnMgr);
        VOS_DELETE(m_pTcpServerMgr);
    }
    catch (...)
    {}
}

long CConnMgr::init(const ULONG ulSelectPeriod, const VOS_BOOLEAN bHasUdpSock,
                    const VOS_BOOLEAN bHasTcpClient, const VOS_BOOLEAN bHasTcpServer)
{
    if ((VOS_TRUE == bHasTcpClient) || (VOS_TRUE == bHasTcpServer))
    {
        (void)VOS_NEW(m_pTcpConnMgr);
        if (NULL == m_pTcpConnMgr)
        {
            IVS_RUN_LOG_ERR("create TcpConnMgr fail");
            return VOS_FAIL;
        }

        if (VOS_SUCCESS != m_pTcpConnMgr->init(ulSelectPeriod))
        {
            IVS_RUN_LOG_ERR("init TcpConnMgr fail");
            return VOS_FAIL;
        }
    }

    if (VOS_TRUE == bHasTcpServer)
    {
        (void)VOS_NEW(m_pTcpServerMgr);
        if (NULL == m_pTcpServerMgr)
        {
            IVS_RUN_LOG_ERR("create TcpServerMgr fail");
            return VOS_FAIL;
        }

        if (VOS_SUCCESS != m_pTcpServerMgr->init(ulSelectPeriod))
        {
            IVS_RUN_LOG_ERR("init TcpServerMgr fail");
            return VOS_FAIL;
        }

        m_pTcpServerMgr->setTcpClientMgr(m_pTcpConnMgr);
    }

    if (bHasUdpSock)
    {
        return VOS_SUCCESS;
    }

    return VOS_SUCCESS;
}

long CConnMgr::run(void)
{
    if (NULL != m_pTcpConnMgr)
    {
        if (VOS_SUCCESS != m_pTcpConnMgr->run())
        {
            IVS_RUN_LOG_ERR("run m_pTcpConnMgr fail");
            return VOS_FAIL;
        }
    }

    if (NULL != m_pTcpServerMgr)
    {
        if (VOS_SUCCESS != m_pTcpServerMgr->run())
        {
            IVS_RUN_LOG_ERR("run TcpServerMgr fail");
            return VOS_FAIL;
        }
    }

    return VOS_SUCCESS;
}

void CConnMgr::exit(void)
{
    if (NULL != m_pTcpServerMgr)
    {
        m_pTcpServerMgr->exit();
        //VOS_DELETE(m_pTcpServerMgr);
    }

    if (NULL != m_pTcpConnMgr)
    {
        m_pTcpConnMgr->exit();
        //VOS_DELETE(m_pTcpConnMgr);
    }

#if VOS_APP_OS == VOS_OS_WIN32
    //(void)WSACleanup();
#endif
    return;
}

void CConnMgr::setDefaultLocalAddr(const char *szLocalIpAddr)
{
    if (szLocalIpAddr != NULL)
    {
        long lLocalIp = (long)inet_addr(szLocalIpAddr);
        if ((ULONG)lLocalIp != InvalidIp)
        {
            m_lLocalIpAddr = (long)inet_addr(szLocalIpAddr);
        }
    }

    return;
}

long CConnMgr::regTcpClient( const CNetworkAddr *pLocalAddr,
                             const CNetworkAddr *pPeerAddr, CTcpConnHandle *pTcpConnHandle,
                             const EnumSyncAsync bSyncConn, ULONG ulTimeOut)
{
    if (NULL == pLocalAddr)
    {
        IVS_RUN_LOG_ERR("regTcpClient pLocalAddr is NULL");
        return VOS_FAIL;
    }

    if (NULL == pPeerAddr)
    {
        IVS_RUN_LOG_ERR("regTcpClient PeerAddr is NULL");
        return VOS_FAIL;
    }

    if (NULL == pTcpConnHandle)
    {
        IVS_RUN_LOG_ERR("regTcpClient TcpConnHandle is NULL");
        return VOS_FAIL;
    }

    if (VOS_SUCCESS != pTcpConnHandle->initHandle())
    {
        IVS_RUN_LOG_ERR("regTcpClient TcpConnHandle init fail");
        return VOS_FAIL;
    }

    if (NULL == m_pTcpConnMgr)
    {
        IVS_RUN_LOG_ERR("regTcpClient TcpConnMgr is NULL");
        return VOS_FAIL;
    }

    CNetworkAddr localAddr;
    if (InvalidIp == (ULONG)(pLocalAddr->m_lIpAddr))
    {
        localAddr.m_lIpAddr = this->m_lLocalIpAddr;
    }
    else
    {
        localAddr.m_lIpAddr = pLocalAddr->m_lIpAddr;
    }

    localAddr.m_usPort = pLocalAddr->m_usPort;

    pTcpConnHandle->m_localAddr.m_lIpAddr = pLocalAddr->m_lIpAddr;
    pTcpConnHandle->m_localAddr.m_usPort = pLocalAddr->m_usPort;

    long lRetVal = pTcpConnHandle->conn(&localAddr, pPeerAddr, bSyncConn, ulTimeOut);

    if (lRetVal != VOS_SUCCESS)
    {
        IVS_RUN_LOG_ERR("connect peer fail(0x%x:%d)",
                       ntohl((ULONG)(pPeerAddr->m_lIpAddr)), ntohs(pPeerAddr->m_usPort));
        return lRetVal;
    }

    lRetVal = m_pTcpConnMgr->addHandle(pTcpConnHandle);
    if (lRetVal != VOS_SUCCESS)
    {
        pTcpConnHandle->close();
        IVS_RUN_LOG_ERR("register connection fail");
        return lRetVal;
    }

    return VOS_SUCCESS;
}

void CConnMgr::removeTcpClient(CTcpConnHandle *pTcpConnHandle)
{
    if (NULL == pTcpConnHandle)
    {
        IVS_RUN_LOG_ERR("removeTcpClient TcpConnHandle is NULL");
        return;
    }

    IVS_DBG_LOG("CConnMgr::removeTcpClient: "
                   "remove pTcpConnHandle(0x%x) pHandleNode(0x%x) fd(%dl)"
                   "m_lIpAddr(0x%x) m_usPort(%d)",
                   pTcpConnHandle, pTcpConnHandle->m_pHandleNode,
                   pTcpConnHandle->m_lSockFD, pTcpConnHandle->m_localAddr.m_lIpAddr,
                   pTcpConnHandle->m_localAddr.m_usPort);

    //�˴����ܹر�socket��ԭ������:
    //����ͨ��ƽ̨�е�CConnMgr::removeTcpClient����ʱ��
    //�����ȹر���socket������ͨ��ƽ̨��socketɨ���̼߳�ص�socket�ж��¼���
    //���Ǵ�ʱsocket�Ѿ����ر���,socket �ϱ��Ķ��¼��ǷǷ��ġ�
    //���Թر�socket����Ҫ�ͼ��socket�¼���������ʱ����Ҫ����.
    //pTcpConnHandle->close();

    if (NULL == m_pTcpConnMgr)
    {
        IVS_RUN_LOG_ERR("removeTcpClient TcpConnMgr is NULL");
        return;
    }

    m_pTcpConnMgr->removeHandle(pTcpConnHandle);

    return;
}

long CConnMgr::regTcpServer(const CNetworkAddr *pLocalAddr,
                            CTcpServerHandle *  pTcpServerHandle)
{
    if (NULL == pLocalAddr)
    {
        IVS_RUN_LOG_ERR("regTcpServer  pLocalAddr is NULL");
        return VOS_FAIL;
    }

    if (NULL == pTcpServerHandle)
    {
        IVS_RUN_LOG_ERR("regTcpServer TcpServerHandle is NULL");
        return VOS_FAIL;
    }

    if (VOS_SUCCESS != pTcpServerHandle->initHandle())
    {
        IVS_RUN_LOG_ERR("regTcpServer TcpServerHandle init fail");
        return VOS_FAIL;
    }

    if (NULL == m_pTcpConnMgr)
    {
        IVS_RUN_LOG_ERR("regTcpServer TcpConnMgr is NULL");
        return VOS_FAIL;
    }

    if (NULL == m_pTcpServerMgr)
    {
        IVS_RUN_LOG_ERR("regTcpServer TcpServerMgr is NULL");
        return VOS_FAIL;
    }

    CNetworkAddr localAddr;
    if (InvalidIp == (ULONG)(pLocalAddr->m_lIpAddr))
    {
        localAddr.m_lIpAddr = this->m_lLocalIpAddr;
    }
    else
    {
        localAddr.m_lIpAddr = pLocalAddr->m_lIpAddr;
    }

    localAddr.m_usPort = pLocalAddr->m_usPort;

    pTcpServerHandle->m_localAddr.m_lIpAddr = pLocalAddr->m_lIpAddr;
    pTcpServerHandle->m_localAddr.m_usPort = pLocalAddr->m_usPort;

    long lRetVal = pTcpServerHandle->listen(&localAddr);

    if (lRetVal != VOS_SUCCESS)
    {
        IVS_RUN_LOG_ERR("regTcpServer: listen fail");
        return lRetVal;
    }

    lRetVal = m_pTcpServerMgr->addHandle(pTcpServerHandle);
    if (lRetVal != VOS_SUCCESS)
    {
        pTcpServerHandle->close();
        IVS_RUN_LOG_ERR("register tcp server fail");
        return lRetVal;
    }

    return VOS_SUCCESS;
}

void CConnMgr::removeTcpServer(CTcpServerHandle *pTcpServerHandle)
{
    if (NULL == pTcpServerHandle)
    {
        IVS_RUN_LOG_ERR(" intput TcpServerHandle is invalid");
        return;
    }

    //�˴����ܹر�socket��ԭ������:
    //����ͨ��ƽ̨�е�CConnMgr::removeTcpClient����ʱ��
    //�����ȹر���socket������ͨ��ƽ̨��socketɨ���̼߳�ص�socket�ж��¼���
    //���Ǵ�ʱsocket�Ѿ����ر���,socket �ϱ��Ķ��¼��ǷǷ��ġ�
    //���Թر�socket����Ҫ�ͼ��socket�¼���������ʱ����Ҫ����.
    //pTcpServerHandle->close();

    if (NULL == m_pTcpServerMgr)
    {
        IVS_RUN_LOG_ERR("TcpServerMgr is NULL");
        return;
    }

    m_pTcpServerMgr->removeHandle(pTcpServerHandle);

    return;
}
