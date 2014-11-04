#include "Connection.h"
#include "ivs_log.h"
#ifdef WIN32
#include "ws2ipdef.h"
#endif
#define IVS_PLAYER_RTP_MAX_SOCKET_BUFFER (1024 * 1024 * 4) //������4MB����Ƶsocket�Ļ�����

#ifndef WIN32
void closesocket(SOCKET s)
{
	close(s);
}
#endif

CConnection::CConnection(void)
    : m_socket(INVALID_SOCKET)
    , m_uiLocalPort(0)
    , m_uiServerPort(0)
{
}

CConnection::~CConnection(void)
{
    ShutDown();
}

int CConnection::Bind(const char* pszLocalIP, unsigned int uiLocalPort)
{
    if (NULL == pszLocalIP || 0 == uiLocalPort)
    {
        IVS_LOG(IVS_LOG_ERR, "Bind", "Input param local ip or local port error");
        return IVS_PARA_INVALID;
    }

    m_strLocalIP = pszLocalIP;
    m_uiLocalPort = uiLocalPort;
    return IVS_SUCCEED;
}

int CConnection::Select(SELECT_TYPE enType, long lUsec)
{
    fd_set fdSocket;
    FD_ZERO(&fdSocket);
    FD_SET(m_socket,&fdSocket);
    timeval _time;
    _time.tv_sec = 0;
    _time.tv_usec = lUsec;//΢��;

    int iRet = 0;
    switch(enType)
    {
    case READ:
        {
            iRet = ::select(0,&fdSocket,NULL,NULL,&_time);
            break;
        }
    case WRITE:
        {
            iRet = ::select(0,NULL,&fdSocket,NULL,&_time);
            break;
        }
    case EXCEPT:
        {
            iRet = ::select(0,NULL,NULL,&fdSocket,&_time);
            break;
        }
    default:
        IVS_LOG(IVS_LOG_ERR, "Select", "Select socket type error, type = %d", enType);
        break;
    }

//#ifdef _DEBUG
//    sockaddr_in stAddr = {0};
//    int  nAddrLen =  sizeof(stAddr);
//    int nResult = getsockname(m_socket, (struct sockaddr *)&stAddr, &nAddrLen);   
//    char* pszLocalIp = inet_ntoa(stAddr.sin_addr);
//    u_short port = htons((u_short)stAddr.sin_port);
//
//    getpeername(m_socket, (struct sockaddr *)&stAddr, &nAddrLen);   
//    char* pszPeerIp = inet_ntoa(stAddr.sin_addr);
//    u_short portPeer = htons((u_short)stAddr.sin_port);
//#endif 
    return iRet;
}

int CConnection::SetSocketBuf(unsigned int uiSize)
{
    // ���ý��ջ�������С;
    int iRet = setsockopt(m_socket, SOL_SOCKET, SO_RCVBUF, (const char*)&uiSize, sizeof(uiSize));
    if (0 != iRet)
    {
        IVS_LOG(IVS_LOG_ERR, "SetSocketBuf", "setsockopt return error. error:%d",  CONN_ERRNO);
        iRet = IVS_FAIL;
    }
    return iRet;
}

void CConnection::SetIOState(bool bIsBlock)
{
    if(NULL == m_socket)
    {
        IVS_LOG(IVS_LOG_ERR, "SetIOState", "Order error,socket is null");
        return;
    }

    unsigned long ulState = bIsBlock ? BLOCKSTATE : NOBLOCKSTATE;
#ifdef WIN32
    (void)ioctlsocket(m_socket, (long)FIONBIO, (u_long*)(&ulState));
#else
	fcntl(m_socket,(long)FIONBIO,(u_long*)(&ulState));
#endif
}

int CConnection::Connect(const char* pszServerIP, unsigned int uiServerPort)
{
    if (NULL == pszServerIP || 0 == uiServerPort)
    {
        IVS_LOG(IVS_LOG_ERR, "Connect", "pszServerIP = NULL, uiServerPort = 0.");
        return IVS_PARA_INVALID;
    }
    m_strServerIP = pszServerIP;
    m_uiServerPort = uiServerPort;
    return IVS_SUCCEED;
}

void CConnection::ShutDown()
{
	try
	{
		m_strLocalIP = "";
		m_strServerIP = "";
		m_uiLocalPort = 0;
		m_uiServerPort = 0;

		if(INVALID_SOCKET != m_socket)
		{
#ifdef WIN32
			::shutdown(m_socket, FD_CLOSE);
#else
			shutdown(m_socket,2);
#endif
			closesocket(m_socket);

		}
		m_socket = INVALID_SOCKET;
	}
	catch(...)
	{
		IVS_LOG(IVS_LOG_ERR, "ShutDown",
				"ShutDown or close socket throw exception, error code:%d", CONN_ERRNO);
	}
}

void CConnection::GetLocalIpPort(std::string& strIP, unsigned int& uiPort)
{
    strIP = m_strLocalIP;
    uiPort = m_uiLocalPort;
}

void CConnection::GetServerIpPort(std::string& strIP, unsigned int& uiPort)
{
    strIP = m_strServerIP;
    uiPort = m_uiServerPort;
}

int CConnection::InitMulticast(const std::string& strRemoteSendStreamIp,
                               unsigned int uRemoteSendStreamPort,
                               const std::string& strLocalRecvStreamIp)
{

    //�����鲥�˿ڸ���,����ѡ������ڰ�֮ǰ����
    //TODOΪ����Ȧ���Ӷȣ��˴�δ��¼���ö˿ڸ����Ƿ�ɹ�
    int isReusable = TRUE;
    if (::setsockopt(m_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&isReusable, sizeof(isReusable)) == SOCKET_ERROR)
    {
        IVS_LOG(IVS_LOG_ERR, "Init Multicast",
            "Set Multicast Port Reusable error, error code:%d", CONN_ERRNO);
        ShutDown();
        return IVS_FAIL;
    }

    // ��ʼ���ಥ��ַ;
    struct ip_mreq mreq;
    memset(&mreq, 0, sizeof(mreq));

    //�ಥ��ַ;
    mreq.imr_multiaddr.s_addr = inet_addr(strRemoteSendStreamIp.c_str());
    mreq.imr_interface.s_addr = inet_addr(strLocalRecvStreamIp.c_str());

    if (::setsockopt(m_socket, IPPROTO_IP, IP_ADD_MEMBERSHIP, (const char*)&mreq, sizeof(mreq)) == SOCKET_ERROR)
    {
        IVS_LOG(IVS_LOG_ERR, "Init Multicast",
            "Get Multicast Port error, add multicast group failed, error code:%d", CONN_ERRNO);
        ShutDown();
        return IVS_FAIL;
    }

    //����ƵSocket��������Ϊ1MByte ���������Ƶ
    int iRecvBuf = IVS_PLAYER_RTP_MAX_SOCKET_BUFFER;
#ifdef WIN32
    int iOptLen = sizeof(iRecvBuf);
#else
	socklen_t iOptLen = sizeof(iRecvBuf);
#endif

    if (::setsockopt(m_socket, SOL_SOCKET, SO_RCVBUF, (char*)&iRecvBuf, iOptLen) == SOCKET_ERROR)
    {
        IVS_LOG(IVS_LOG_ERR, "Init Multicast",
            "Get Multicast Port error, set receive buffer size=%d failed, error code:%d", iRecvBuf, errno);
    }
    else
    {
        iRecvBuf = 0;
        (void)getsockopt(m_socket, SOL_SOCKET, SO_RCVBUF, (char*)&iRecvBuf, &iOptLen);
        IVS_LOG(IVS_LOG_DEBUG, "Init Multicast",
            "Get Multicast Port success, set receive buffer size=%d", iRecvBuf);
    }

    return IVS_SUCCEED;
}
