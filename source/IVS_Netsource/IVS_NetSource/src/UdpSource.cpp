#include "UdpSource.h"
#include "ChannelInfo.h"
#include "UdpConnection.h"
#include "ivs_log.h"

CUdpSource::CUdpSource(void)
    : m_bMulticast(false)
{
	ZeroMemory(&m_SenderVideoRTCPSockAddr,sizeof(sockaddr_in));
	ZeroMemory(&m_SenderAudioRTCPSockAddr,sizeof(sockaddr_in));
}


CUdpSource::~CUdpSource(void)
{
}

int CUdpSource::Init(CChannelInfo* pChannelInfo)
{
    int iRet = CNetSource::Init(pChannelInfo);
    if (IVS_SUCCEED != iRet)
    {
        IVS_LOG(IVS_LOG_ERR, "Init", "Init function of base class failed");
        return iRet;
    }

    // ����socket;
    std::string strRemoteSendStreamIp;
    iRet = pChannelInfo->GetStreamIP(REMOTE_SEND_STREAM_IP, strRemoteSendStreamIp);
    if (strRemoteSendStreamIp.empty() || IVS_SUCCEED != iRet)
    {
        IVS_LOG(IVS_LOG_ERR, "Init", "UDP GetStreamIP failed. strRemoteSendStreamIp is empty");
        return IVS_PARA_INVALID;
    }

    iRet = ConnSocket(strRemoteSendStreamIp);
    if (IVS_SUCCEED != iRet)
    {
        ShutDownSocket();
        return iRet;
    }
    return iRet;
}

void CUdpSource::ProcessRecevData()
{
    if (NULL == m_pChannelInfo)
    {
        IVS_LOG(IVS_LOG_ERR, "ProcessRecevData", "m_pChannelInfo = NULL.");
        return;
    }

    fd_set  fdread;

	/*
	 * DTS2013072800500
	 * fixed by w00210470, ����select��ʱ100����
	 */
	struct timeval tv = {0, 100000}; 
	//struct timeval tv = {RECV_MEDIA_DATA_TIMEOUT, 0}; //ָ����ʱ������û����������Ϊý�����ж�;
	FD_ZERO(&fdread);

    //�����о������fdread����;
	SOCKET sock;
	SOCKET sockmax = 0;
    for (int i = 0; i < NET_SOURCE_UDP_SOCKET_NUM; i++)
    {
		// ֧�ֽ�����Ƶ�˿ڣ�û�з�����Ƶ�˿ڵ����;
		if ((AUDIO_RTP_SOCKET == i || AUDIO_RTCP_SOCKET == i) && m_pChannelInfo->IsOnlyVideo())
		{
			//IVS_LOG(IVS_LOG_ERR, "Process Recev Data", "Udp source handle socket Index[%d] Only video port, do not set audio socket to fdset", i);
			continue;
		}
        sock = m_pChannelInfo->GetSocket((SOCKET_TYPE)i);
		if(sock>sockmax)
		{
			sockmax = sock;
		}
        if (INVALID_SOCKET == sock)
        {
            IVS_LOG(IVS_LOG_ERR, "Process Recev Data", "invalid socket, Socket type index:%d.", i);
            return;
        }
		
        FD_SET(sock, &fdread);
    } 
#ifdef WIN32
 	int iSelectState = select(0, &fdread, NULL, NULL, &tv);
#else	
	int maxfdp = sockmax+1;
    int iSelectState = select(maxfdp, &fdread, NULL, NULL, &tv);
#endif
    // �����쳣;
    if (SOCKET_ERROR == iSelectState)
    {
        IVS_LOG(IVS_LOG_ERR, "Process Recev Data", "Select error, error code:%d", CONN_ERRNO);
        return;
    }
    // ��鳬ʱ
    if (0 == iSelectState)
    {
        IVS_LOG(IVS_LOG_DEBUG, "Process Recev Data", "Select failed, select over time, error code:%d", CONN_ERRNO);
        CheckTimeOut();
        return;
    }

    // ��������
    sockaddr_in addr;
    ZeroMemory(&addr,sizeof(sockaddr_in));
#ifdef WIN32
    int nLen = sizeof(addr);
#else 
	socklen_t nLen = sizeof(addr);
#endif
    for (int i = 0; i < NET_SOURCE_UDP_SOCKET_NUM; i++)
    {
		// �����������Ƶ�˿ڣ�δ������Ƶ�˿ڵ�����;
		if ((AUDIO_RTCP_SOCKET == i || AUDIO_RTP_SOCKET == i) && m_pChannelInfo->IsOnlyVideo())
		{
			//IVS_LOG(IVS_LOG_ERR, "Process Recev Data", "Udp source handle socket Index[%d] Only video port,do not recevie audio stream", i);
			continue;
		}
        // �˴�socketһ����ΪINVALID_SOCKET
        SOCKET socket = m_pChannelInfo->GetSocket((SOCKET_TYPE)i);
        if (!FD_ISSET(socket, &fdread))
        {
            continue;   
        }
        int len = recvfrom(socket, m_pRecvBuf, RTP_MEDIA_BUF_LEN, 0, (sockaddr *)&addr, &nLen);
        if (0 >= len)
        {
            IVS_LOG(IVS_LOG_ERR, "Process Recev Data", "Udp source handle socket Index[%d] receive data failed. error[%d]", i, CONN_ERRNO);
			
			/*
			 * ������ recvfrom ����ֵ -1
			 * ��Զ˶˿ڷ���NET��Խ��Ϣ�� ���Է��Կ�δ�򿪣����򱾻�����ICMP�˿ڲ��ɴ
			 * ����UDP Socket�Ѿ�bind��recvfrom �᷵�� -1
			 */
#if 0
            // ��Ƶ�˿������쳣���������ϲ�;
            if (VIDEO_RTP_SOCKET == i)
            {
                unsigned long ulChannel = m_pChannelInfo->GetChannelNo();
				IVS_LOG(IVS_LOG_ERR, "Process Recev Data", "Udp source handle socket video do exception");
				DoExceptionCallBack(IVS_PLAYER_RET_RECV_DATA_ERROR, (void*)ulChannel);
            }
#endif 
            return;
        }
		// �����鲥���������Դ��IP
		if (m_pChannelInfo->IsMuilt())
		{
			if (VIDEO_RTCP_SOCKET == i)
			{
				m_SenderVideoRTCPSockAddr.sin_addr = addr.sin_addr;
				m_SenderVideoRTCPSockAddr.sin_port = addr.sin_port;
				m_SenderVideoRTCPSockAddr.sin_family = addr.sin_family;
			}
			else if (AUDIO_RTCP_SOCKET == i)
			{
				m_SenderAudioRTCPSockAddr.sin_addr = addr.sin_addr;
				m_SenderAudioRTCPSockAddr.sin_port = addr.sin_port;
				m_SenderAudioRTCPSockAddr.sin_family = addr.sin_family;
			}
		}
		else
        {
			// ����Ƿ���;�ж��Ƿ�Ϊ�û����õ�IP�Ͷ˿ںŷ����������������ֱ���ӵ�
			//��ƵRTPͨ��
			if (VIDEO_RTP_SOCKET == i)
			{
				unsigned int uiRemoteVideoRtpPort = 0;
				std::string strRemoteSendStreamIp;
				(void)m_pChannelInfo->GetStreamIP(REMOTE_SEND_STREAM_IP, strRemoteSendStreamIp);
				m_pChannelInfo->GetRemotePort(VIDEO_RTP_PORT, uiRemoteVideoRtpPort);

				if ((addr.sin_addr.s_addr != inet_addr(strRemoteSendStreamIp.c_str())) 
					|| (addr.sin_port != htons((unsigned short)uiRemoteVideoRtpPort)))
				{
					char szRemoteIp[IVS_IP_LEN] = {0};
					strncpy(szRemoteIp, inet_ntoa(addr.sin_addr), IVS_IP_LEN - 1);
					IVS_LOG(IVS_LOG_ERR, "Process Recev Data",
						"Udp source video ip or port is illegal, remote ip set[%s] socket[%s], video rtpport set[%u] socket[%u]", 
						strRemoteSendStreamIp.c_str(), szRemoteIp, uiRemoteVideoRtpPort, ntohs(addr.sin_port));
					return;
				}
			}

			//��ƵRTPͨ��
			if (AUDIO_RTP_SOCKET == i)
			{
				unsigned int uiRemoteAudioRtpPort = 0;
				std::string strRemoteSendStreamIp;
				(void)m_pChannelInfo->GetStreamIP(REMOTE_SEND_STREAM_IP, strRemoteSendStreamIp);
				m_pChannelInfo->GetRemotePort(AUDIO_RTP_PORT, uiRemoteAudioRtpPort);

				if ((addr.sin_addr.s_addr != inet_addr(strRemoteSendStreamIp.c_str())) 
					|| (addr.sin_port != htons((unsigned short)uiRemoteAudioRtpPort)))
				{
					char szRemoteIp[IVS_IP_LEN] = {0};
					strncpy(szRemoteIp, inet_ntoa(addr.sin_addr), IVS_IP_LEN - 1);

					IVS_LOG(IVS_LOG_ERR, "Process Recev Data",
						"Udp source audio ip or port is illegal, ip set[%s] socket[%s], audio rtpport set[%u] socket[%u]", 
						strRemoteSendStreamIp.c_str(), szRemoteIp, uiRemoteAudioRtpPort, ntohs(addr.sin_port));
					return;
				}
			}
        }

        ProcPacket(i, len);
    }
}

void CUdpSource::ProcPacket(int iIndex, int iLen)
{
    //����RTSP��Ӧ��
#ifdef WIN32
    if (0 == _strnicmp(m_pRecvBuf, RTSP::RTSP_VERSION, strlen(RTSP::RTSP_VERSION)))
#else
	if (0 == strncasecmp(m_pRecvBuf, RTSP::RTSP_VERSION, strlen(RTSP::RTSP_VERSION)))
#endif
    {
        //���Ͻ�����;
        m_pRecvBuf[iLen] = 0;
        return;
    }

	//������������ʱ��;
	m_ulLastCheckTimeoutTick = GetTickCount();

    switch (iIndex)
    {
        //ý������;
    case VIDEO_RTP_SOCKET:
    case AUDIO_RTP_SOCKET:
        m_ulRecvBufLen = (unsigned long)iLen;
        ProcMediaPacket();//��������;
        break;
        //RTCP
    case VIDEO_RTCP_SOCKET:
    case AUDIO_RTCP_SOCKET:
        IVS_LOG(IVS_LOG_DEBUG, "ProcPacket", "HandleRecv index:%d recv RTCP",iIndex);
        break;
    default:
        IVS_LOG(IVS_LOG_ERR, "ProcPacket", "Udp source process packet error, unknown index:%d", iIndex);
        break;
    }
}

// ����Ƿ�����NAT��Խ����,ÿ��30s�����������nat��Ϣ;
int CUdpSource::CheckNatSend()
{
	// ����Ƿ����㷢��Nat������;
    int iRet = CNetSource::CheckNatSend();
	if (IVS_SUCCEED != iRet)
	{
		return IVS_FAIL;
	}

	if (NULL == m_pChannelInfo)
	{
		IVS_LOG(IVS_LOG_ERR, "CheckNatSend", "Channel info object is null.");
		return IVS_FAIL;
	}

    string strRtspHeartBeat;
    RTSP::NAT_REQ_PARA stPara = {0};
    stPara.ulProtocalType = RTSP::PROTOCAL_TYPE_RTP;

    for (int i = 0; i < NET_SOURCE_UDP_SOCKET_NUM; i++)
    {
		// �����������Ƶ�˿ڣ�δ������Ƶ�˿ڵ�����;
		if ((AUDIO_RTCP_SOCKET == i || AUDIO_RTP_SOCKET == i) 
			&& (NULL != m_pChannelInfo) 
			&& (m_pChannelInfo->IsOnlyVideo()))
		{
			//IVS_LOG(IVS_LOG_ERR, "Check Nat Send", "Udp source handle socket Index[%d] Only video port,do not send nat for audio port", i);
			continue;
		}

        //���ݲ�ͬ�˿ڻ�ȡ��ͬЭ��;
        if ((VIDEO_RTP_SOCKET == i) || (AUDIO_RTP_SOCKET == i))
        {
            stPara.ulProtocalType = RTSP::PROTOCAL_TYPE_RTP;
        }
        else
        {
            stPara.ulProtocalType = RTSP::PROTOCAL_TYPE_RTCP;
        }

        unsigned int uiPort = 0;
        m_pChannelInfo->GetLocalPort((PORT_TYPE)i, uiPort);
        stPara.usLocalPort = (unsigned short)uiPort;

        (void)m_RtspStack.GetNatPacket(stPara, strRtspHeartBeat);

        //�������ʹ����ӡ��־;
		CUdpConnection* pConn = dynamic_cast<CUdpConnection*>(m_pChannelInfo->GetConnection((SOCKET_TYPE)i));
		if (NULL == pConn)
		{
			continue;
		}

		if (m_pChannelInfo->IsMuilt())
		{
			if (VIDEO_RTCP_SOCKET == i)
			{
				iRet = pConn->SendMulitCastData(strRtspHeartBeat.c_str(), strRtspHeartBeat.length(), m_SenderVideoRTCPSockAddr);
			}
			else if (AUDIO_RTCP_SOCKET == i)
			{
				iRet = pConn->SendMulitCastData(strRtspHeartBeat.c_str(), strRtspHeartBeat.length(), m_SenderAudioRTCPSockAddr);
			}
		}
		else
		{
			iRet = pConn->SendData(strRtspHeartBeat.c_str(), strRtspHeartBeat.length());
		}

		if (SOCKET_ERROR != iRet)
		{
			IVS_LOG(IVS_SUCCEED, "HandleNatSend", "Udp source handle nat send Succeed,socket index:%d", i);
		}
		else
		{
			IVS_LOG(IVS_LOG_ERR, "HandleNatSend", "Udp source handle nat send error, WSAGetLastError:%d,socket index:%d",CONN_ERRNO, i);
		}
    }
	return IVS_SUCCEED;
}

int CUdpSource::ConnSocket(const std::string& strRemoteSendStreamIp)
{
    int iRet = IVS_FAIL;
    if (NULL == m_pChannelInfo)
    {
        IVS_LOG(IVS_LOG_ERR, "Init", "m_pChannelInfo = NULL.");
        return iRet;
    }

    for (int i = 0; i < SOCKET_COUNT; i++)
    {
        CConnection* pConn = m_pChannelInfo->GetConnection((SOCKET_TYPE)i);
        if (NULL == pConn)
        {
            iRet = IVS_FAIL;
            IVS_LOG(IVS_LOG_ERR, "Init", "Get connection from ChannelInfo failed, pConn = NULL ,socket index:%d", i);
            break;
        }

        unsigned int uiPort = 0;
        iRet = m_pChannelInfo->GetRemotePort((PORT_TYPE)i, uiPort);

        // add for ����ֻ����Ƶͨ���ĳ���
        if (0 == uiPort
            && VIDEO_RTP_PORT == i)
        {
            i = VIDEO_RTCP_SOCKET;
            continue;
        }
        // end add

		// add for ����ǰ�˽��ܷ�����Ƶ�˿ڣ����ܷ�����Ƶ�˿ڵ�����,��ΪBT��ǰ�˷������һ����0��1���˴�ʹ��С��5����󲿷����;
		if (5 > uiPort && (AUDIO_RTP_PORT == i || AUDIO_RTCP_PORT == i))
		{
			IVS_LOG(IVS_LOG_ERR, "Init", "Connection of audio port[%u] is error,socket index[%d]", uiPort, i);
			m_pChannelInfo->SetOnlyVideo(true);
			continue;
		}
		// end add

        iRet += pConn->Connect(strRemoteSendStreamIp.c_str(), uiPort);
        if (IVS_SUCCEED != iRet)
        {
            IVS_LOG(IVS_LOG_ERR, "Init", "socket connect failed ,socket index:%d", i);
            break;
        }
    }
    
    if (IVS_SUCCEED != iRet)
    {
        ShutDownSocket();
        iRet = IVS_FAIL;
    }
    return iRet;
}

void CUdpSource::ShutDownSocket()
{
    if (NULL == m_pChannelInfo)
    {
        IVS_LOG(IVS_LOG_ERR, "Init", "m_pChannelInfo = NULL.");
        return;
    }

    for (int i = 0; i < SOCKET_COUNT; i++)
    {
        CConnection* pConn = m_pChannelInfo->GetConnection((SOCKET_TYPE)i);
        if (NULL == pConn)
        {
            continue;
        }

        pConn->ShutDown();
    }
}

