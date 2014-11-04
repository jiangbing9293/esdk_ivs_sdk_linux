#include "TcpConnection.h"
#include "ivs_log.h"

//���ӳ�ʱʱ��;
#define CONNECT_TIMEOUT 3
#define RECV_TIMES 28

CTcpConnection::CTcpConnection(void)
{
}


CTcpConnection::~CTcpConnection(void)
{
}

int CTcpConnection::Bind(const char* pszLocalIP, unsigned int uiLocalPort)
{
    int iRet = CConnection::Bind(pszLocalIP, uiLocalPort);
    if (IVS_SUCCEED != iRet)
    {
        IVS_LOG(IVS_LOG_ERR, "Bind", "Call base class failed.");
        return IVS_FAIL;
    }

    m_socket = ::socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    if(m_socket == INVALID_SOCKET)
    {
        IVS_LOG(IVS_LOG_ERR, "Bind", "Create tcp socket failed, error:%d",  CONN_ERRNO);
        return IVS_FAIL;
    }

    //��Ϊ��
    int iTimeout = 1000;
    iRet = setsockopt(m_socket, SOL_SOCKET, SO_RCVTIMEO, (char *)&iTimeout,sizeof(iTimeout));
    if (0 > iRet)
    {
        IVS_LOG(IVS_LOG_ERR, "Bind", "setsockopt return error. error:%d",  CONN_ERRNO);
        iRet = IVS_FAIL;
    }

    sockaddr_in _sin;
    ZeroMemory(&_sin,sizeof(sockaddr_in));
    _sin.sin_family = AF_INET;
    _sin.sin_port = htons((u_short)m_uiLocalPort);
    _sin.sin_addr.s_addr = inet_addr(m_strLocalIP.c_str());

    if(SOCKET_ERROR == bind(m_socket,(sockaddr*)&_sin,sizeof(_sin)))
    {
        IVS_LOG(IVS_LOG_ERR, "Bind", "Bind tcp socket failed, error:%d",  CONN_ERRNO);
        return IVS_FAIL;
    }

    return IVS_SUCCEED;
}

int CTcpConnection::Connect(const char* pszServerIP, unsigned int uiServerPort)
{
    int iRet = CConnection::Connect(pszServerIP, uiServerPort);
    if (IVS_SUCCEED != iRet)
    {
        IVS_LOG(IVS_LOG_ERR, "Connect", "Call base class failed.");
        return IVS_FAIL;
    }

    unsigned long ul = NOBLOCKSTATE; 
#ifdef WIN32
    iRet = ioctlsocket(m_socket, (long)FIONBIO, (u_long*)(&ul));
#else
	iRet = fcntl(m_socket,(long)FIONBIO,(u_long*)(&ul));
#endif
    if(SOCKET_ERROR == iRet)
    {
        IVS_LOG(IVS_LOG_ERR, "Connect", "Set tcp socket to no block state failed, error:%d",  CONN_ERRNO);
        return IVS_FAIL; 
    }

    sockaddr_in servAddr;
    ZeroMemory(&servAddr,sizeof(sockaddr_in));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = inet_addr(m_strServerIP.c_str());
    servAddr.sin_port = htons((u_short)m_uiServerPort);

    if(SOCKET_ERROR == ::connect(m_socket,(sockaddr*)&servAddr,sizeof(servAddr)))
    {
        //��ʼ�������Ϊ��������connect����ʧ��,�ᱨ10035���޷��������һ�����赲���׽��ֲ���;
        //�����������ܳɹ�����;��������������򷵻�;
        int iLastErr = CONN_ERRNO;
        if (WSAEWOULDBLOCK != iLastErr)
        {
            IVS_LOG(IVS_LOG_ERR, "Connect", "Connect socket error, code:%d.", iLastErr);
            return IVS_PLAYER_RET_CONNECT_SOCKET_ERROR;
        }

        IVS_LOG(IVS_LOG_DEBUG, "Connect", "Cannot connect tcp socket with no block state, but still connect");
    }

    //select ģ�ͣ������ó�ʱ;
    struct timeval timeout ;
    fd_set w;
    FD_ZERO(&w);
    FD_SET(m_socket, &w);
    timeout.tv_sec = CONNECT_TIMEOUT; // ���ӳ�ʱ3s;
    timeout.tv_usec = 0;
    (void)select(0, NULL, &w, NULL, &timeout);
   /* if(iRet <= 0 )
    {
        IVS_LOG(IVS_LOG_DEBUG, "Connect", "Select failed, error:%d",  CONN_ERRNO);
        return IVS_PLAYER_RET_CONNECT_SOCKET_ERROR; 
    } */

    //һ�������ģʽ�׽ӱȽ��ѿ��ƣ����Ը���ʵ��������� ���������ģʽ; 
    unsigned long ulState = BLOCKSTATE;
#ifdef WIN32
    iRet = ioctlsocket(m_socket, (long)FIONBIO, (u_long*)&ulState); 
#else
	iRet = fcntl(m_socket,(long)FIONBIO,(u_long*)(&ul));
#endif
    if(SOCKET_ERROR == iRet)
    {
        IVS_LOG(IVS_LOG_DEBUG, "Connect", "Set block state failed, error:%d",  CONN_ERRNO);
    }

    return IVS_SUCCEED;
}

int CTcpConnection::RecvData(char * pData, unsigned int uiDataLen)
{
    if(NULL == pData || NULL == m_socket)
    {
        IVS_LOG(IVS_LOG_ERR, "RecvData", "Input Param error");
        return IVS_PARA_INVALID;
    }

    int iRecvTimes = 0;
    int iRet = -1;
    unsigned int uiDataLenCur = 0;
	char* pTemp = pData;
    while(uiDataLenCur < uiDataLen)
    {       
        // ���յ�����;
        iRet = ::recv(m_socket, pTemp + uiDataLenCur, (int)(uiDataLen - uiDataLenCur), 0);
        if(0 == iRet)//������ӶϿ�;
        {
            IVS_LOG(IVS_LOG_ERR, "RecvData", "Receive data error, (pData:0x%x, nDataLen:%d)", (long)pData, uiDataLen);
            closesocket(m_socket);
            return iRet;
        }
        else if(SOCKET_ERROR  >= iRet)
        {
            
            int   error = errno;
            int iLastErr = CONN_ERRNO;
            if (EAGAIN == error || WSAETIMEDOUT == iLastErr)
            {
                iRecvTimes++;
                if (iRecvTimes < RECV_TIMES)
                {
                    IVS_LOG(IVS_LOG_ERR, "RecvData", "Receive data Timeout");
                    continue;
                }
                else
                {
                    IVS_LOG(IVS_LOG_ERR, "RecvData", "Receive data Timeout times over [%d]", iRecvTimes);
                    closesocket(m_socket);
                    iRecvTimes = 0;
                    return IVS_PLAYER_RET_RECV_DATA_TIMEOUT;
                }
                
            }
            else if (ENOTSOCK == iLastErr)
            {
                //����յ�10038����ֱ�ӷ���
                IVS_LOG(IVS_LOG_ERR, "RecvData", "Receive data error, error code:%d.", CONN_ERRNO);
                return IVS_PLAYER_RET_RECV_DATA_TIMEOUT;
            }

            IVS_LOG(IVS_LOG_ERR, "RecvData", "Receive data error, (pData:0x%x, nDataLen:%d, error code:%d)", (long)pData, uiDataLen, CONN_ERRNO);
            closesocket(m_socket);
            return iRet;
        }

        uiDataLenCur += (unsigned int)iRet;
    }
    return (int)uiDataLenCur;
}

int CTcpConnection::SendData(const char * pData, unsigned int uiDataLen) 
{
    if(NULL == pData || NULL == m_socket)
    {
        IVS_LOG(IVS_LOG_ERR, "SendData", "Input Param error");
        return IVS_PARA_INVALID;
    }
    int iRet = -1;
    unsigned int uiDataLenCur = 0;
    while(uiDataLenCur < uiDataLen)
    {
        int iSelectRet = Select(WRITE);
        if (iSelectRet < 0) // �쳣;����SOCKET_ERROR
        {
            IVS_LOG(IVS_LOG_ERR, "SendData", "Select write mode error, error code:%d", CONN_ERRNO);
            return iRet;
        }
        else if(0 == iSelectRet) // ��ʱ;
        {
            break;
        }

        iRet = ::send(m_socket, pData + uiDataLenCur, (int)(uiDataLen - uiDataLenCur), 0);
        if(SOCKET_ERROR >= iRet)
        {
            IVS_LOG(IVS_LOG_ERR, "SendData", "Send data error, (pData:0x%x, nDataLen:%d, error code:%d)", long(pData), uiDataLen, CONN_ERRNO);
            return iRet;
        }
        uiDataLenCur += (unsigned int)iRet;
    }

    return IVS_SUCCEED;
}
