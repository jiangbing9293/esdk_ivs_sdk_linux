/******************************************************************************
   ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� �� : RtspSession.cpp
  ��    �� : y00182336
  �������� : RTSP����
  �޸���ʷ :
    1 ���� : 2011-10-12
      ���� : y00182336
      �޸� : ����

 ******************************************************************************/
#include "Rtsp_Log.h"
#include "RtspRetCode.h"
#include "RtspSession.h"
#include "RtspReactorManager.h"
#include "RtspMessage.h"
#include "RtspSetupMessage.h"
#include "RtspPlayMessage.h"
#include "RtspSessionManager.h"
#include "RtspAnnounceMessage.h"
#include "RtspOptionsMessage.h"

const char* CRtspSession::m_strRtspMethod[] = {
    "RTSP/1.0",
    "SETUP",
    "PLAY",
    "PAUSE",
    "TEARDOWN",
    "ANNOUNCE",
    "DESCRIBE"
};

/***************************************************************************
 �� �� �� : CRtspSession
 �������� : ���캯��
 ������� : ��
 ������� : ��
 �� �� ֵ : ��
***************************************************************************/
CRtspSession::CRtspSession()
{
    m_bBlocked        = true;
    m_tcpStream.set_handle(ACE_INVALID_HANDLE);

    m_fDataCallBack   = NULL;
    m_pUser           = NULL;
    m_pRtspMsg        = NULL;
    //m_strDate         = "";
    m_unPlayTimePos   = 0;

    m_pRtspSemaphore  = NULL;
    m_bRecvRetFlag    = false;

    m_enSessionStatus = RTSP_SESSION_STATUS_INIT;
    m_unCSeq          = 0;
    m_lTimerId        = -1;    
    
    m_pRecvBuffer     = NULL;

    m_unSessionIndex  = 0;
    m_bDelete         = false;

	m_bDataRecved = false;      // �������ݱ�ǣ����ڳ�ʱ���
	m_uTmoCounter = 0;          // ��ʱ������
	m_lDataTimer  = -1;         // ���ݽ��ռ�ⶨʱ��
	m_bEnableTmoCheck = false;  // Ĭ�ϲ�ʹ�ܳ�ʱ���

	m_iRetCode = 0;

	m_bConnectFail = false;
}

/***************************************************************************
 �� �� �� : ~CRtspSession
 �������� : ��������
 ������� : ��
 ������� : ��
 �� �� ֵ : ��
***************************************************************************/
CRtspSession::~CRtspSession()
{
	{
		IVS_RUN_LOG_CRI("RtspSession Destroy %08x %08x", this, m_pRecvBuffer);
		
		ACE_Guard <ACE_Thread_Mutex> lockerSession (m_Mutex);
	    try
	    {
	        if (NULL != m_pRecvBuffer)
	        {
	            delete m_pRecvBuffer;
	            m_pRecvBuffer = NULL;
	        }
	    }
	    catch (...)
	    {
	    }

	    try
	    {
	        if (NULL != m_pRtspSemaphore)
	        {
	            delete m_pRtspSemaphore;
	            m_pRtspSemaphore = NULL;
	        }
	    }
	    catch (...)
	    {
	    }

		try
		{
			if (NULL != m_pRtspMsg)
			{
				delete m_pRtspMsg;
				m_pRtspMsg = NULL;
			}
		}
		catch (...)
		{
		}
	}
    m_fDataCallBack   = NULL;
    m_pUser           = NULL;
    m_pRtspMsg        = NULL;

	// ȥע��󣬸ö���ע��Ķ�ʱ����Ч �� FIX ME!!!
}


/***************************************************************************
 �� �� �� : handle_timeout
 �������� : �첽��ʽ�£���Ӧ��Ϣ��ʱ�ص�����
 ������� : ��
 ������� : ��
 �� �� ֵ : 0-�ɹ�          -1-ʧ��
***************************************************************************/
int CRtspSession::handle_timeout(const ACE_Time_Value &tv, const void *arg)
{
#if 0
	ACE_Guard <ACE_Thread_Mutex> locker (m_Mutex);
    if (NULL == m_pRtspMsg)
    {
        return RET_FAIL;
    }
    // ����Teardown��Ϣ���������Ͽ�����
    unsigned int unMethodType = m_pRtspMsg->getMethodType();
    if (RTSP_METHOD_TEARDOWN == unMethodType)
    {
        return disconnectPeer();
    }

    //ȡ����ʱ��ʱ��
    cancelRecvTimer();
    
    // ɾ����Ϣ
    destroyRtspMsg();

    IVS_RUN_LOG_CRI("rtsp receive message timeout. method type[%u].", unMethodType);

    // �����쳣�ص�����
    CRtspSessionManager::instance()->m_fExceptionCallBack((long)this, 
                                                          (unsigned int)unMethodType, 
                                                          (unsigned int)RTSP_ERR_TIMEOUT,
                                                          NULL,
                                                          CRtspSessionManager::instance()->m_pUser);
#endif 
	
	if (!m_bEnableTmoCheck)
	{
		return 0;
	}

	if (CheckRtpTimeout())
	{
		KillRtpTimer();
		CRtspSessionManager::instance()->m_fExceptionCallBack((long)this, 
			(unsigned int)RTSP_ERR_TIMEOUT, 
			(unsigned int)RTSP_ERR_TIMEOUT,
			NULL,
			CRtspSessionManager::instance()->m_pUser);
	}
	else 
	{
		//KillRtpTimer();
		//m_lDataTimer = -1;
		//SetupRtpTimer();
	}
    return 0;
}


/***************************************************************************
 �� �� �� : connectPeer
 �������� : ��������
 ������� : remoteAddr      �Զ˵�ַ
 ������� : ��
 �� �� ֵ : RET_OK-�ɹ�          RET_FAIL-ʧ��
***************************************************************************/
int CRtspSession::connectPeer(const ACE_INET_Addr &remoteAddr)
{
    if ((m_bBlocked) && (NULL == m_pRtspSemaphore))
    {
        // ͬ����ʽ�£������ź���
        try
        {
            m_pRtspSemaphore = new ACE_Thread_Semaphore(0);
        }
        catch(...)
        {
            IVS_RUN_LOG_CRI("create thread semaphore fail.");
            return RET_FAIL;
        }
    }

    ACE_SOCK_Connector peerConnector;
    ACE_Time_Value     tv((time_t)1);
    m_remoteAddr = remoteAddr;
    // �����������ӣ������ظ�����
    if (ACE_INVALID_HANDLE != m_tcpStream.get_handle())
    {
        IVS_DBG_LOG("session already connected, handle[%p].", this);
        return RET_OK;
    }

	// ����������ռ��ʱ������tcp����ʧ�ܵ�����
	bool bConFlag = false;
	int iConCount = 3;
	while(iConCount > 0)
	{
		if (-1 == peerConnector.connect(m_tcpStream, m_remoteAddr, &tv, ACE_Addr::sap_any, 1))
		{
			int iErrorNo = ACE_OS::last_error();
			IVS_DBG_LOG("connect [%s:%d] fail, error[%d].", 
						remoteAddr.get_host_addr(),
						remoteAddr.get_port_number(),
						iErrorNo);
			iConCount--;
			//Sleep(50);
			ACE_OS::sleep(1);
			continue;
		}
		else
		{
			bConFlag = true;
			break;
		}
	}

	if (!bConFlag)
	{
		if (m_tcpStream.get_handle() != ACE_INVALID_HANDLE)
		{
			m_tcpStream.close();
			m_tcpStream.set_handle(ACE_INVALID_HANDLE);
		}

		m_bConnectFail = true;
		IVS_DBG_LOG("connect [%s:%d] fail for three times.",
					remoteAddr.get_host_addr(),
					remoteAddr.get_port_number());
		return RET_FAIL;
	}

    unsigned int nSize = DEFAULT_BUFFER_SIZE;
    if (0 != ACE_OS::setsockopt(get_handle(), SOL_SOCKET, SO_RCVBUF, (const char*)&nSize, sizeof(nSize)))
    {
        IVS_RUN_LOG_WAR("Rtsp push session set recv buffer fail, errno=%d",
            errno);
    }


    // ���÷�����
#ifndef WIN32
    if (ACE_OS::fcntl(get_handle(), F_SETFL, ACE_OS::fcntl(get_handle(), F_GETFL) | O_NONBLOCK))
#else
    ULONG ulNoBlock = true;
    if (ACE_OS::ioctl(get_handle(), (long)(long)FIONBIO, &ulNoBlock))
#endif
    {
        IVS_RUN_LOG_WAR("connect[%s:%d] set options fail, errno[%d].",
            m_localAddr.get_host_addr(),
            m_localAddr.get_port_number(),
            errno);
        return RET_FAIL;
    }

    // ��������ʱ����m_localAddr��Ա��ֵ
    m_tcpStream.get_local_addr(m_localAddr);
    IVS_RUN_LOG_INF("Connect to [%s:%d] success, local port[%d].",
                    m_remoteAddr.get_host_addr(),
                    m_remoteAddr.get_port_number(),
                    m_localAddr.get_port_number());

    // ����ע�ᵽ��Ӧ��
    int nRet = CRtspReactorManager::instance()->RegistHandle(this, 
                                                             ACE_Event_Handler::READ_MASK);
    if (RET_OK != nRet)
    {
        //m_tcpStream.close();
        return RET_FAIL;
    } 
    return RET_OK;
}


/***************************************************************************
 �� �� �� : disconnectPeer
 �������� : �Ͽ�����
 ������� : ��
 ������� : ��
 �� �� ֵ : RET_OK-�ɹ�          RET_FAIL-ʧ��
***************************************************************************/
int CRtspSession::disconnectPeer()
{
    // ȡ����ʱ��
    cancelRecvTimer();
    //destroyRtspMsg();
    // ����RTP���ն�ʱ��
	KillRtpTimer();

    // ��������Ƿ���
    ACE_HANDLE nHandle = m_tcpStream.get_handle();
    if (ACE_INVALID_HANDLE == nHandle)
    {
        return RET_FAIL;
    }

	// �����Ӧ��
    int nRet = CRtspReactorManager::instance()->unRegistHandle((ACE_Event_Handler*)this,
                                                                ACE_Event_Handler::READ_MASK | ACE_Event_Handler::DONT_CALL);

    if (RET_OK != nRet)
    {
        IVS_RUN_LOG_CRI("Unregist reactor fail,remote address[%s:%d].",
                        m_remoteAddr.get_host_addr(), m_remoteAddr.get_port_number());
    }

    // ע�⣡�˴��в����ͷŻ���m_pRecvBuffer�����������������
    // ԭ��disconnect�����������߳���ֹʱ�ͼ����߳����ߵ��ã������̶߳Ի�����ý�������
    // ������delete����NULL�����̲߳�δ�ı䣬�������delete�������ڴ����
    // ����취����Session�������������ͷŻ��棬��ֻ֤�ͷ�һ�μ���

    // �ر�tcp����
    (void)ACE_OS::shutdown(nHandle, -1);
    (void)m_tcpStream.close();
	m_tcpStream.set_handle(ACE_INVALID_HANDLE);


    //setStatus(RTSP_SESSION_STATUS_ABNORMAL);

    IVS_RUN_LOG_INF("RTSP session disconnect [%s:%d] success.",
                    m_remoteAddr.get_host_addr(), m_remoteAddr.get_port_number());
    return RET_OK;
}


/***************************************************************************
 �� �� �� : sendMessage
 �������� : ������Ϣ
 ������� : pDataBuffer         ��Ϣ�ַ���
            unDataSize          ��Ϣ����
 ������� : ��
 �� �� ֵ : RET_OK-�ɹ�          RET_FAIL-ʧ��
***************************************************************************/
int CRtspSession::sendMessage(const char *pDataBuffer, unsigned int unDataSize)
{
    ACE_Guard <ACE_Thread_Mutex> locker (m_StreamMutex);
    int nRet = 0;
    unsigned int unBuffSize = unDataSize;
    IVS_RUN_LOG_INF("try to send message, len[%u]:\n%s", unDataSize, pDataBuffer);
    // һ��û�з�������Ҫ�ظ�����
    do
    {
        nRet = m_tcpStream.send(pDataBuffer, unBuffSize);
        if (0 >= nRet)
        {
            int iErrorCode = ACE_OS::last_error();
            if (checkIsDisconnect(iErrorCode))
            {
                // �����Ѿ��Ͽ�������ʧ�ܣ�ǿ�ƹر����ӣ�����ʧ��
                (void)disconnectPeer();
                return RTSP_ERR_DISCONNECT;
            }

            // ���ӿ��ã�����ǰ��Դæ����Ҫ�������ٴε���
            destroyRtspMsg();
            return RTSP_ERR_SEND_FAIL;
        }
    
        IVS_RUN_LOG_INF("send msg to [%s:%d] success, len[%d].", 
                        m_remoteAddr.get_host_addr(),
                        m_remoteAddr.get_port_number(),
                        nRet);
        
        pDataBuffer += nRet;
        unBuffSize -= (unsigned int)nRet;
    }while(unBuffSize > 0);

	IVS_RUN_LOG_INF("sendMessage [%s:%d] success.", 
					m_remoteAddr.get_host_addr(),
					m_remoteAddr.get_port_number());
    return RET_OK;
}



/***************************************************************************
 �� �� �� : destroyRtspMsg
 �������� : ɾ��RTSP��Ϣ��
 ������� : ��
 ������� : ��
 �� �� ֵ : ��
***************************************************************************/
void CRtspSession::destroyRtspMsg()
{
    if (NULL != m_pRtspMsg)
    {
        delete m_pRtspMsg;
        m_pRtspMsg = NULL;
        IVS_RUN_LOG_INF("delete Rtsp message success.");
    }
    return;
}


/***************************************************************************
 �� �� �� : getCSeq
 �������� : ��ȡRTSP��Ϣ��ϵ�к�
 ������� : ��
 ������� : ��
 �� �� ֵ : ���к�
***************************************************************************/
unsigned int CRtspSession::getCSeq()
{
    return ++m_unCSeq;
}


/***************************************************************************
 �� �� �� : get_handle
 �������� : ��ȡhandle��������ڷ�Ӧ��ע��
 ������� : ��
 ������� : ��
 �� �� ֵ : >0-�ɹ�          -1-ʧ��
***************************************************************************/
ACE_HANDLE CRtspSession::get_handle() const
{
    return m_tcpStream.get_handle();
}


/***************************************************************************
 �� �� �� : setRecvTimer
 �������� : �첽��ʽ�£����ó�ʱ��ʱ��
 ������� : timeout             ��ʱʱ��
 ������� : ��
 �� �� ֵ : RET_OK-�ɹ�          RET_FAIL-ʧ��
***************************************************************************/
int CRtspSession::setRecvTimer(unsigned int timeout)
{
    // ������飬���ⶨʱ���ظ�ʹ��
    if ((0 == timeout) || (-1 != m_lTimerId))
    {
        return RET_FAIL;
    }
    time_t unTime = (time_t)timeout;
    ACE_Time_Value tv(unTime, 0);
    m_lTimerId = CRtspReactorManager::instance()->RegistTimer((ACE_Event_Handler*)this, m_unSessionIndex, tv, tv);
    if(-1 == m_lTimerId)
    {
        return RET_FAIL;
    }
    IVS_DBG_LOG("register receive timer success.");
    return RET_OK;
}



/***************************************************************************
 �� �� �� : cancelRecvTimer
 �������� : ȡ����ʱ��ʱ��
 ������� : ��
 ������� : ��
 �� �� ֵ : ��
***************************************************************************/
void CRtspSession::cancelRecvTimer()
{
    if (-1 != m_lTimerId)
    {
        // ȡ����ʱ��
        (void)CRtspReactorManager::instance()->unRegistTimer(m_lTimerId, m_unSessionIndex);
        IVS_DBG_LOG("rtsp session[%p] cancel receive timer success.", this);
        m_lTimerId = -1;
    }
    return;
}


/***************************************************************************
 �� �� �� : handle_input
 �������� : ��Ӧ��Ϣ���ջص�����
 ������� : handle              ���
 ������� : ��
 �� �� ֵ : 0-�ɹ�              -1-ʧ��
***************************************************************************/
int CRtspSession::handle_input(ACE_HANDLE handle)
{
    ACE_Guard <ACE_Thread_Mutex> locker (m_Mutex);
    // ��������Ƿ�����
    if ((ACE_INVALID_HANDLE == m_tcpStream.get_handle()) || (NULL == m_pRecvBuffer))
    {
		IVS_DBG_LOG("handle input fail, m_tcpStream handle is error[%d] or m_pRecvBuffer is null.", m_tcpStream.get_handle());
        return -1;
    }

    // ���㻺����ʣ�೤��
    int iRecvLen = (int) m_pRecvBuffer->size() - (int) m_pRecvBuffer->length();
    if (0 >= iRecvLen)
    {
        // ���������ˣ�������ֱ�ӷ���
        IVS_DBG_LOG("handle input fail, recv buff is full, iRecvLen=%d RecvBufferSize=%d Length=%d", iRecvLen, m_pRecvBuffer->size(), m_pRecvBuffer->length());
        //return 0;
    }
    else
    {
        //�������ݵ�������
        ACE_OS::last_error(0);
        iRecvLen = m_tcpStream.recv(m_pRecvBuffer->wr_ptr(), (size_t)iRecvLen);
        if (0 >= iRecvLen )
        {
            int iErrorCode = ACE_OS::last_error();
            if (checkIsDisconnect(iErrorCode))
            {
                (void)disconnectPeer();
                //���������߳�,��ֹͬ�����õ�CU�����ȴ���
                if (m_bBlocked)
                {
                    m_bRecvRetFlag = false;
                    m_pRtspSemaphore->release();
                }
                // �����Ѿ��Ͽ������ûص�����֪ͨ�û�
                CRtspSessionManager::instance()->m_fExceptionCallBack((long)this,
                                                                      RTSP_METHOD_NUM,
                                                                      RTSP_ERR_DISCONNECT,
                                                                      NULL, 
                                                                      CRtspSessionManager::instance()->m_pUser);
                return -1;
            }

            // ������������Դæ����������
            return 0;
        }

        //�ƶ���������дָ��
        m_pRecvBuffer->wr_ptr((size_t)(m_pRecvBuffer->length() + (size_t) iRecvLen));
        m_pRecvBuffer->rd_ptr((size_t) 0);
    }

    size_t processedSize = 0;
    size_t totalSize     = m_pRecvBuffer->length();
    int nSize            = 0;
    do
    {
        nSize = handleRecvedData(m_pRecvBuffer->rd_ptr() + processedSize,
                                 m_pRecvBuffer->length() - processedSize);
        if (nSize < 0)
        {
            // ������ʧ��
            IVS_RUN_LOG_WAR("rtsp connect handle recv data fail.");
            return -1;
        }

        if (0 == nSize)
        {
			IVS_RUN_LOG_WAR("handleRecvedData nSize = 0.");
            // ��������ʣ�����ݳ��Ȳ�����һ����Ϣ��
            break;
        }

		// ����RTP���ı��Ϊ��
		m_bDataRecved = true;

        processedSize += (size_t)nSize;
    }while(processedSize < totalSize);

	//IVS_DBG_LOG("handleRecvedData out.");
	
	// ���ݴ���ɹ������½��ջ�����ָ��
    size_t dataSize = m_pRecvBuffer->length() - processedSize;
    (void) m_pRecvBuffer->copy(m_pRecvBuffer->rd_ptr() + processedSize, dataSize);
    m_pRecvBuffer->rd_ptr((size_t) 0);
    m_pRecvBuffer->wr_ptr(dataSize);
    return 0;
}


/***************************************************************************
 �� �� �� : handle_close
 �������� : �ر����ӻص�����
 ������� : handle              ���
            close_mask          ȡ��ע���¼�
 ������� : ��
 �� �� ֵ : 0-�ɹ�              -1-ʧ��
***************************************************************************/
int CRtspSession::handle_close(ACE_HANDLE handle, ACE_Reactor_Mask close_mask)
{
    if (RTSP_SESSION_STATUS_INIT == getStatus())
    {
        // �����Ѿ����رգ�ֱ�ӷ���
        return -1;
    }

    // �ر����ӣ���������״̬
    (void)disconnectPeer();
    IVS_RUN_LOG_INF("close rtsp session success,local addr[%s:%d].",
                    m_localAddr.get_host_addr(), m_localAddr.get_port_number());
    return 0;
}

/***************************************************************************
 �� �� �� : handleRecvedData
 �������� : ������յ���Ϣ
 ������� : pData               ��Ϣ�ַ���
            unDataSize          ��Ϣ����
 ������� : ��
 �� �� ֵ : �ɹ���>=0�������������Ϣ���򷵻�ʵ�ʴ������Ϣ���ȡ������Ϣδ�����꣬�򷵻�0
            ʧ�ܣ�<0
***************************************************************************/
int CRtspSession::handleRecvedData(const char* pData, unsigned int unDataSize)
{
    /*IVS_DBG_LOG("handle received data message from [%s:%d], len[%u].", 
                m_remoteAddr.get_host_addr(), m_remoteAddr.get_port_number(),
                unDataSize);*/

    int nRet = 0;
    if (RTP_INTERLEAVE_FLAG == pData[0]) 
    {
        // ����ý����Ϣ
        nRet = parseRtpRtcpPacket(pData, unDataSize);
    }
    else if (isRtspMessage(pData, unDataSize))
    {
        // ����RTSP��Ϣ�������������Ӧ
        nRet = (int)parseRtspPacket(pData, unDataSize);
    }    
    else
    {   
        if ((RTSP_SESSION_STATUS_PLAYING == getStatus()) && (NULL != m_fDataCallBack))
        {
            // ������Ϣ��ֱ�ӻص�
            m_fDataCallBack((long)this, const_cast<char*>(pData), unDataSize, m_pUser);
        }
        nRet = (int)unDataSize;
    }
	//IVS_DBG_LOG("handleRecvedData out.");
    return nRet;
}

// ����ý����Ϣ
int CRtspSession::parseRtpRtcpPacket(const char* pData, unsigned int unDataSize)
{
    if (unDataSize < RTP_INTERLEAVE_LENGTH)
    {
		IVS_RUN_LOG_WAR("parseRtpRtcpPacket failed, unDataSize[%u].", unDataSize);
        // ʣ�����ݲ�������������0
        return 0;
    }

    // ���ݽ�֯���Ƚ�����RTP��RTCP���ĳ���
    unsigned int unSize = (unsigned int) ACE_NTOHS(*(unsigned short*)const_cast<char*>(&pData[2])); //lint !e826
    if (unDataSize - RTP_INTERLEAVE_LENGTH < unSize)
    {
		IVS_RUN_LOG_WAR("parseRtpRtcpPacket jiaozhi, unDataSize[%u].", unDataSize);
        // ����û�н����꣬��ʱ����
        return 0;
    }
    unSize += RTP_INTERLEAVE_LENGTH;

    // ý����Ϣ�����״̬�����ûص�����
    if (RTSP_SESSION_STATUS_PLAYING == getStatus())
    {
        if(NULL == m_fDataCallBack)
        {
			IVS_RUN_LOG_WAR("parseRtpRtcpPacket failed, m_fDataCallBack = null.");
            return RET_FAIL;
        }
        m_fDataCallBack((long)this, const_cast <char*> (pData), (unsigned int)unSize, m_pUser);
    }
    // ����ʵ����Ϣ����
	//IVS_DBG_LOG("parseRtpRtcpPacket succeed.");
    return (int)unSize;
}

// ����RTSP��Ϣ
unsigned int CRtspSession::parseRtspPacket(const char* pData, unsigned int unDataSize)
{
	IVS_RUN_LOG_WAR("parseRtspPacket in.");
    // ��ȷ���߽�
    string strRecvedData = "";
    strRecvedData.append(pData, unDataSize);
    string::size_type nPos = strRecvedData.find(RTSP_MSG_END_TAG);
    if (string::npos == nPos)
    {
		IVS_RUN_LOG_WAR("parseRtspPacket out npos.");
        // ��Ϣû�н�����ȫ������0
        return 0;
    }
    unsigned int unSize = (unsigned int)(nPos + strlen(RTSP_MSG_END_TAG));
    // �������Content-Length:�ֶΣ����ϸ��ֶγ���
    string::size_type nLenPos = strRecvedData.find(RTSP_TOKEN_STR_CONTENT_LENGTH);
    if ((string::npos != nLenPos) && (nPos > nLenPos))
    {
        string strValue = "";
        strValue = strRecvedData.substr(nLenPos + strlen(RTSP_TOKEN_STR_CONTENT_LENGTH));
        string strCententLen = "";
        strCententLen = strValue.substr(0, strValue.find(RTSP_END_TAG));
        unsigned int unContentLen = (unsigned int)ACE_OS::atoi(strCententLen.c_str());
        unSize += unContentLen;
    }
    if (unSize > unDataSize)
    {
		IVS_RUN_LOG_WAR("parseRtspPacket out 1 unSize[%u] unDataSize[%u].", unSize, unDataSize);
        return 0;
    }

    if (NULL != m_pRtspMsg)
    {
        if (m_pRtspMsg->isRtspRespMsg(pData, unSize))
        {
            // ������Ӧ��Ϣ
            int nRet = m_pRtspMsg->decodeMessage(pData, unSize);
            // ������ҲҪ�׵��ϲ㣬�˴�������������߼�.
			/*if (RET_OK != nRet)
            {
                IVS_RUN_LOG_CRI("decode message fail.");
                return 0;
            }*/

            string strBuffer = "";
            strBuffer.append(pData, unSize);
            IVS_DBG_LOG("start decode rtsp message, len[%u]:\n%s", unSize, strBuffer.c_str());
            // ����RTSP��Ӧ��Ϣ�����Ͻ������
            (void)handleRtspRespMsg(nRet,pData, unSize);
			IVS_RUN_LOG_WAR("parseRtspPacket out 2.");
            return unSize;
        }
    }
    // ʣ�µ�ֻ������RTSP������Ϣ����ʱֻ����ANNOUNCE����
    if(0 == strncmp(pData, RTSP_ANNOUNCE_MSG, strlen(RTSP_ANNOUNCE_MSG)))
    {
        int nRet = handleRtspAnnounceMsg(pData, unSize);
        if (RET_OK != nRet)
        {
            // ����ʧ�ܣ�����
            IVS_RUN_LOG_CRI("hande rtsp announce message fail.");
            return unSize;
        }
    }
	IVS_RUN_LOG_WAR("parseRtspPacket out.");
    return unSize;
}

/***************************************************************************
 �� �� �� : handleRtspAnnounceMsg
 �������� : �����ʹ���ANNOUNCE����,��Ҫ����û��SDP�������ֱ�ӻ���Ӧ�����ص�
 ������� : psRtspMsg           ��Ϣ�ַ���
            nLen                ��Ϣ����
 ������� : ��
 �� �� ֵ : RET_OK-�ɹ�         RET_FAIL-ʧ��
***************************************************************************/
int CRtspSession::handleRtspAnnounceMsg(const char* psRtspMsg, const unsigned int nLen)
{

    CRtspAnnounceMessage* pAnnounceMsg = NULL;
    try
    {
        pAnnounceMsg = new CRtspAnnounceMessage;
    }
    catch (...)
    {
        pAnnounceMsg = NULL;
    }

    if (NULL == pAnnounceMsg)
    {
        IVS_RUN_LOG_CRI("session ANNOUNCE msg response is invalid");
        return RET_FAIL;
    }

    string AnnounceResp = "";
    int nRet = pAnnounceMsg->decodeMessage(psRtspMsg, nLen);
    if (RET_OK != nRet)
    {
        pAnnounceMsg->setStatusCode(RTSP_CLIENT_BAD_REQUEST);
        (void) pAnnounceMsg->encodeMessage(AnnounceResp);
    }
    else
    {
        pAnnounceMsg->setStatusCode(RTSP_SUCCESS_OK);
        (void) pAnnounceMsg->encodeMessage(AnnounceResp);
    }

    // ������Ӧ�ɹ���SDP�ֶβ�Ϊ��ʱ�����ûص�����
    if (RET_OK == sendMessage(AnnounceResp.c_str(), AnnounceResp.length())
        && (RET_OK == nRet) && ("" != pAnnounceMsg->getRtspSdp()))
    {
        string strConntent = "";
        strConntent = pAnnounceMsg->getRtspSdp();
        string::size_type nPos = strConntent.find(RTSP_ANNOUNCE_CONTENT);
        if (string::npos == nPos)
        {
            // ���ûص���������SDP�ֶδ���ȥ
            CRtspSessionManager::instance()->m_fExceptionCallBack((long)this,
                RTSP_METHOD_ANNOUNCE,  
                RET_OK, 
                (void*)const_cast<char*>(pAnnounceMsg->getRtspSdp().c_str()),
                CRtspSessionManager::instance()->m_pUser);
        }
        else
        {
            string strValue = "";
            strValue = strConntent.substr(nPos + strlen(RTSP_ANNOUNCE_CONTENT));
            int nValue = ACE_OS::atoi(strValue.c_str());
            unsigned int unRet = RTSP_ERR_RET_SYS;
            // ��������
            if (RTSP_NORMAL_STOP == nValue)
            {
                unRet = RTSP_ERR_NORMAL_STOP;
            }
            // �쳣����
            else if(RTSP_ABNORMAL_STOP == nValue)
            {
                unRet = RTSP_ERR_ABNORMAL_STOP;
            }
            // ���ûص���������SDP�ֶδ���ȥ
            CRtspSessionManager::instance()->m_fExceptionCallBack((long)this,
                RTSP_METHOD_ANNOUNCE,  
                unRet, 
                (void*)const_cast<char*>(pAnnounceMsg->getRtspSdp().c_str()),
                CRtspSessionManager::instance()->m_pUser);
        }
    }
    delete pAnnounceMsg;
    return RET_OK;
}



/***************************************************************************
 �� �� �� : setStatus
 �������� : ��������״̬
 ������� : status              ���ĺ������״̬
 ������� : ��
 �� �� ֵ : ��
***************************************************************************/
void CRtspSession::setStatus(unsigned int status)
{
    ACE_Guard<ACE_Thread_Mutex>  locker(m_StatusMutex);
    if (RTSP_SESSION_STATUS_DELETE < status)//lint !e685
    {
        IVS_RUN_LOG_CRI("set status from [%d] to [%u] fail.", getStatus(), status);
        return;
    }
    IVS_DBG_LOG("set status from [%d] to [%u].", getStatus(), status);
    m_enSessionStatus = (RTSP_SESSION_STATUS)status;
}


/***************************************************************************
 �� �� �� : getStatus
 �������� : ��ȡ��ǰ����״̬
 ������� : ��
 ������� : ��
 �� �� ֵ : status              ���ĺ������״̬
***************************************************************************/
RTSP_SESSION_STATUS CRtspSession::getStatus() const
{
    //ACE_Guard<ACE_Thread_Mutex>  locker(m_StatusMutex);
    return m_enSessionStatus;
}


/***************************************************************************
 �� �� �� : handleRtspRespMsg
 �������� : ����RTSP��Ӧ��Ϣ
 ������� : nRet                ��Ϣ�������
 ������� : ��
 �� �� ֵ : RET_OK-�ɹ�         RET_FAIL-ʧ��
***************************************************************************/
int CRtspSession::handleRtspRespMsg(const int nRet,const char* psRtspMsg, const unsigned int nLen)
{
    if (NULL == m_pRtspMsg)
    {
        IVS_DBG_LOG("message handle is invalid.");
        return RET_FAIL;
    }

    IVS_RUN_LOG_INF("handle rtsp response message, CSeq[%u], ret[%d].", 
                    m_pRtspMsg->getCSeq(), nRet);

    switch(m_pRtspMsg->getMethodType())
    {
    case RTSP_METHOD_SETUP:     // setup��Ϣ��Ӧ
        handleRtspSetupMsg(nRet);
        break;
    case RTSP_METHOD_PLAY:      // play��Ϣ��Ӧ
        handleRtspPlayMsg(nRet);
        break;
    case RTSP_METHOD_PAUSE:     // pause��Ϣ��Ӧ
        handleRtspPauseMsg(nRet);
        break;
    case RTSP_METHOD_TEARDOWN:  // teardown��Ϣ��Ӧ
        handleRtspTeardownMsg(nRet);
        break;
    case RTSP_METHOD_OPTIONS:
        handleRtspOptionsMsg(nRet,psRtspMsg,nLen);
        break;
    default:                    // �Ƿ�״̬
        return RET_FAIL;
    }
    return RET_OK;
}



/***************************************************************************
 �� �� �� : handleRtspSetupMsg
 �������� : ����RTSP Setup��Ӧ���첽��ʽ�ڴ˻ص���ͬ����ʽͨ��������
 ������� : nRet                ��Ϣ�������
 ������� : ��
 �� �� ֵ : ��
***************************************************************************/
void CRtspSession::handleRtspSetupMsg(const int nRet)
{
    // ȡ����ʱ��
    cancelRecvTimer();

    // ���״̬
    if ((RTSP_SESSION_STATUS_INIT == getStatus()) && (RET_OK == nRet))
    {
        setStatus(RTSP_SESSION_STATUS_READY);
        // ͬ����ʽ�£������֪ͨ������
        if (m_bBlocked)
        {
            m_bRecvRetFlag = true;
            m_pRtspSemaphore->release();
        }
        else
        {
            // �첽��ʽ�£�TCPý�������Ӳ���ҪPeerд���ַ
            CRtspSetupMessage* pSetupMsg = dynamic_cast<CRtspSetupMessage*>(m_pRtspMsg);
            if (NULL == pSetupMsg)
            {
                IVS_RUN_LOG_CRI("handle rtsp setup message response fail.");
                // �ͷ���Դ
                destroyRtspMsg();
                CRtspSessionManager::instance()->m_fExceptionCallBack((long)this, 
                                                                      (unsigned int)RTSP_METHOD_SETUP,
                                                                      (unsigned int)RTSP_ERR_RET_SYS,
                                                                      NULL,
                                                                      CRtspSessionManager::instance()->m_pUser);
                return;
            }
            if (pSetupMsg->m_bTcpFlag)
            {
                // �ͷ���Դ
                destroyRtspMsg();
                CRtspSessionManager::instance()->m_fExceptionCallBack((long)this, 
                                                                      (unsigned int)RTSP_METHOD_SETUP,
                                                                      (unsigned int)RET_OK,
                                                                      NULL,
                                                                      CRtspSessionManager::instance()->m_pUser);
            }
            // UDP���ӷ�ʽ�£�����Peerд���ַ
            else
            {
                ACE_INET_Addr addr;
                addr.set((unsigned short)0, ntohl(pSetupMsg->getServerIp()));
                string strMediaAddr = "";
				const char* pszHostAddr = addr.get_host_addr();
				string strHostAddr = ((NULL == pszHostAddr) ? "" : pszHostAddr);
                strMediaAddr.append(strHostAddr.c_str());
                strMediaAddr += ":";
                stringstream buf;
                buf << pSetupMsg->getServerVideoPort();
                strMediaAddr += buf.str();
                strMediaAddr += ",";
                buf.str("");
                buf << pSetupMsg->getServerAudioPort();
                strMediaAddr += buf.str();
                strMediaAddr += ";";
                // �ͷ���Դ
                destroyRtspMsg();
                CRtspSessionManager::instance()->m_fExceptionCallBack((long)this, 
                                                                      (unsigned int)RTSP_METHOD_SETUP,
                                                                      (unsigned int)RET_OK,
                                                                      (void*)const_cast<char*>(strMediaAddr.c_str()),
                                                                      CRtspSessionManager::instance()->m_pUser);
            }
            
        }
        return;
    }
    else
    {
        // ״̬�쳣
        IVS_RUN_LOG_CRI("Setup messsage response is invalid.Session status[%u], response[%d].",
                        (unsigned int)getStatus(), nRet);
        // ͬ����ʽ�£������ͨ��������
        if (m_bBlocked)
        {   
            m_bRecvRetFlag = false;
			m_iRetCode = nRet;
            m_pRtspSemaphore->release();
        }
        else
        {
            // �ͷ���Դ
            destroyRtspMsg();
            // �첽��ʽ�£����ûص�����
            CRtspSessionManager::instance()->m_fExceptionCallBack((long)this, 
                                                                  (unsigned int)RTSP_METHOD_SETUP,
                                                                  (unsigned int)RTSP_ERR_STAUTS_ABNORMAL,
                                                                  NULL,
                                                                  CRtspSessionManager::instance()->m_pUser);
        }
        return;
    }

}


/***************************************************************************
 �� �� �� : handleRtspPlayMsg
 �������� : ����RTSP Play��Ӧ��Ϣ���첽��ʽ�ڴ˻ص���ͬ����ʽͨ��������
 ������� : nRet                ��Ϣ�������
 ������� : ��
 �� �� ֵ : ��
***************************************************************************/
void CRtspSession::handleRtspPlayMsg(const int nRet)
{
    // ȡ����ʱ��
    cancelRecvTimer();

    // �ͷ���Դ
    destroyRtspMsg();

    // ���״̬
    if ((RTSP_SESSION_STATUS_INIT != getStatus()) && (RET_OK == nRet))
    {
        setStatus(RTSP_SESSION_STATUS_PLAYING);
        // ͬ����ʽ�£�ͨ��������
        if (m_bBlocked)
        {
            m_bRecvRetFlag = true;
            m_pRtspSemaphore->release();
        }
        else
        {
            // �����쳣�ص�����
            CRtspSessionManager::instance()->m_fExceptionCallBack((long)this, 
                                                                  (unsigned int)RTSP_METHOD_PLAY,
                                                                  (unsigned int)RET_OK,
                                                                  NULL,
                                                                  CRtspSessionManager::instance()->m_pUser);
        }
    }
    else
    {
        IVS_RUN_LOG_CRI("session [%p] Play msg response is invalid. Session status[%u], response[%d].",
                        this, (unsigned int)getStatus(), nRet);
        // ͬ����ʽ�£�ͨ��������
        if (m_bBlocked)
        {
            m_bRecvRetFlag = false;
            m_pRtspSemaphore->release();
        }
        else
        {
            // ״̬�쳣�����ûص���������������
            CRtspSessionManager::instance()->m_fExceptionCallBack((long)this, 
                                                                  (unsigned int)RTSP_METHOD_PLAY,
                                                                  (unsigned int)RTSP_ERR_STAUTS_ABNORMAL,
                                                                  NULL,
                                                                  CRtspSessionManager::instance()->m_pUser);
            
        }        
    }

    return;
}


/***************************************************************************
 �� �� �� : handleRtspOptionsMsg
 �������� : ����RTSP Options��Ӧ��Ϣ����¼��ǰʱ��
 ������� : nRet                ��Ϣ�������
 ������� : ��
 �� �� ֵ : ��
***************************************************************************/
void CRtspSession::handleRtspOptionsMsg(int nRet,const char* psRtspMsg, const unsigned int nLen)
{
    // ȡ����ʱ��
    cancelRecvTimer();

    // �ͷ���Դ
    destroyRtspMsg();

    CRtspOptionsMessage* pOptionsMsg = NULL;
    try
    {
        pOptionsMsg = new CRtspOptionsMessage;
    }
    catch(...)
    {
        pOptionsMsg = NULL;
    }

    if (NULL == pOptionsMsg)
    {
        IVS_RUN_LOG_CRI("session OPTIONS msg response is invalid");
        return ;
    }
    string OptionResp = "";
    nRet += pOptionsMsg->decodeMessage(psRtspMsg, (int)nLen);

    // ���״̬
    if ((RTSP_SESSION_STATUS_INIT != getStatus()) && (RET_OK == nRet))
    {
        // ͬ����ʽ�£�ͨ��������
        if (m_bBlocked)
        {
            m_strTimeInfo = pOptionsMsg->m_strTimeinfo;
            m_bRecvRetFlag = true;
            m_pRtspSemaphore->release();
        }
        else
        {
            // �����쳣�ص�����
            CRtspSessionManager::instance()->m_fExceptionCallBack((long)this, 
                (unsigned int)RTSP_METHOD_OPTIONS,
                (unsigned int)RET_OK,
                (void*)pOptionsMsg->m_strTimeinfo.c_str(),          //lint !e1773
                CRtspSessionManager::instance()->m_pUser);
        }
    }
    else
    {
        IVS_RUN_LOG_CRI("session [%p] OPTIONS msg response is invalid. Session status[%u], response[%d].",
            this, (unsigned int)getStatus(), nRet);
        // ͬ����ʽ�£�ͨ��������
        if (m_bBlocked)
        {
            m_bRecvRetFlag = false;
            m_pRtspSemaphore->release();
        }
        else
        {
            // ״̬�쳣�����ûص���������������
            CRtspSessionManager::instance()->m_fExceptionCallBack((long)this, 
                (unsigned int)RTSP_METHOD_OPTIONS,
                (unsigned int)RTSP_ERR_STAUTS_ABNORMAL,
                NULL,
                CRtspSessionManager::instance()->m_pUser);
        }
    }

    if (NULL != pOptionsMsg) //lint !e774
    {
        delete pOptionsMsg;
        pOptionsMsg = NULL;
    }
    return ;    //lint !e438
}


/***************************************************************************
 �� �� �� : handleRtspOptionsMsg
 �������� : ����RTSP Pause��Ӧ��Ϣ
 ������� : nRet                ��Ϣ�������
 ������� : ��
 �� �� ֵ : ��
***************************************************************************/
void CRtspSession::handleRtspPauseMsg(const int nRet)
{
    // ȡ����ʱ��
    cancelRecvTimer();

    // �ͷ���Դ
    destroyRtspMsg();

    // ���READY״̬��ֱ�ӷ���
    if (RTSP_SESSION_STATUS_READY == getStatus())
    {
        return;
    }

    // ����״̬
    if ((RTSP_SESSION_STATUS_PLAYING == getStatus()) && (RET_OK == nRet))
    {
        setStatus(RTSP_SESSION_STATUS_READY);
        // ͬ����ʽ�£������֪ͨ������
        if (m_bBlocked)
        {
            m_bRecvRetFlag = true;
            m_pRtspSemaphore->release();
        }
        else
        {
            // �첽��ʽ�£������쳣�ص�����
            CRtspSessionManager::instance()->m_fExceptionCallBack((long)this, 
                                                                  (unsigned int)RTSP_METHOD_PAUSE,
                                                                  (unsigned int)RET_OK,
                                                                  NULL,
                                                                  CRtspSessionManager::instance()->m_pUser);
        }
    }
    else
    {
        IVS_RUN_LOG_CRI("Session [%p] Pause messsage response is invalid.Session status[%u], response[%d].",
                        this, (unsigned int)getStatus(), nRet);
        // ͬ����ʽ�£������֪ͨ������
        if (m_bBlocked)
        {
            m_bRecvRetFlag = false;
            m_pRtspSemaphore->release();
        }
        else
        {
            // ״̬�쳣�����ûص���������������
            CRtspSessionManager::instance()->m_fExceptionCallBack((long)this, 
                                                                  (unsigned int)RTSP_METHOD_PAUSE,
                                                                  (unsigned int)RTSP_ERR_STAUTS_ABNORMAL,
                                                                  NULL,
                                                                  CRtspSessionManager::instance()->m_pUser);
        }
    }
    return;
}


/***************************************************************************
 �� �� �� : handleRtspTeardownMsg
 �������� : ����RTSP Teardown��Ӧ��Ϣ
 ������� : nRet                ��Ϣ�������
 ������� : ��
 �� �� ֵ : ��
***************************************************************************/
void CRtspSession::handleRtspTeardownMsg(const int nRet)
{
    // INIT״̬����Ӧ��Ϣ����������ֱ�ӷ���
    if (RTSP_SESSION_STATUS_INIT == getStatus())
    {
        return;
    }

    (void)disconnectPeer();

    // ����״̬
    if ((RET_OK == nRet))
    {
        // ͬ����ʽ�£������֪ͨ������
        if (m_bBlocked)
        {
            m_bRecvRetFlag = true;
            m_pRtspSemaphore->release();
        }
        else
        {
            // �첽��ʽ�£������쳣�ص�����
            CRtspSessionManager::instance()->m_fExceptionCallBack((long)this, 
                                                                  (unsigned int)RTSP_METHOD_TEARDOWN,
                                                                  (unsigned int)RET_OK,
                                                                  NULL,
                                                                  CRtspSessionManager::instance()->m_pUser);
        }
    }
    else
    {
        // ���ؽ���쳣���������Ӻ���Դ���ȴ��´η���Teradown��Ϣ
        IVS_RUN_LOG_CRI("Teardown messsage response is invalid.Session status[%u], response[%d].",
                        (unsigned int)getStatus(), nRet);
        // ͬ����ʽ�£������֪ͨ������
        if (m_bBlocked)
        {
            m_bRecvRetFlag = false;
            m_pRtspSemaphore->release();
        }
        else
        {
            // ״̬�쳣�����ûص���������������
            CRtspSessionManager::instance()->m_fExceptionCallBack((long)this, 
                                                                  (unsigned int)RTSP_METHOD_TEARDOWN,
                                                                  (unsigned int)RTSP_ERR_STAUTS_ABNORMAL,
                                                                  NULL,
                                                                  CRtspSessionManager::instance()->m_pUser);
        }
    }
    return;
}


//********************************�ڲ�ʵ��**********************//
/***************************************************************************
 �� �� �� : checkIsDisconnect
 �������� : ��������Ƿ�Ͽ�
 ������� : nErrNo              ���Ӵ�����
 ������� : ��
 �� �� ֵ : true-�Ͽ�           false-δ�Ͽ�
***************************************************************************/
bool CRtspSession::checkIsDisconnect(int nErrNo) const
{
#ifndef WIN32
    if ((EAGAIN == nErrNo) 
        || (ETIME == nErrNo) 
        || (EWOULDBLOCK == nErrNo) 
        || (ETIMEDOUT == nErrNo) 
        || (EINTR == nErrNo))
#else
    if ((WSATRY_AGAIN == nErrNo) 
        || (WSAEWOULDBLOCK == nErrNo) 
        || (WSAETIMEDOUT == nErrNo) 
        || (WSAEINTR == nErrNo))

#endif
    {
        return false;
    }

    IVS_DBG_LOG("[%s:%d] is disconnected. error code [%d].",
				m_remoteAddr.get_host_addr(), m_remoteAddr.get_port_number(),
				nErrNo);
    return true;

}


/***************************************************************************
 �� �� �� : isRtspMessage
 �������� : ����Ƿ���RTSP��Ϣ
 ������� : psRtspMsg           ��Ϣ�ַ���
            nLen                ��Ϣ����
 ������� : ��
 �� �� ֵ : true-��             false-��
***************************************************************************/
bool CRtspSession::isRtspMessage(const char* psRtspMsg,const unsigned int unLen)
{
    // �����Ч�Լ��
    if (NULL == psRtspMsg)
    {
		IVS_DBG_LOG("isRtspMessage failed, psRtspMsg = NULL .");
        return false;
    }

	if (0 == unLen)
	{
		IVS_DBG_LOG("isRtspMessage failed, unLen = 0.");
		return false;
	}

    unsigned int i = 0;
    for (; i < RTSP_METHOD_NUM; i++)
    {
        if (0 == strncmp(psRtspMsg, m_strRtspMethod[i], strlen(m_strRtspMethod[i])))
        {
            break;
        }
    }
    if (i >= RTSP_METHOD_NUM)
    {
        // ��RTSP��Ϣ���طǷ�ͷ
		IVS_DBG_LOG("isRtspMessage failed, i[%u] >= RTSP_METHOD_NUM.", i);
        return false;
    }
    // TODO ȷ��RTSP��Ϣ�߽�
    return true;
} //lint !e1762

void CRtspSession::setDelFlag()
{
    m_bDelete = true;
}

bool CRtspSession::getDelFalg()
{
    return m_bDelete;
}

/*
 * SetupRtpTimer
 * ����RTP ��������ⶨʱ��
 */
int CRtspSession::SetupRtpTimer()
{
	// ������飬���ⶨʱ���ظ�ʹ��
	//ACE_Guard <ACE_Thread_Mutex> locker (m_Mutex);
	if (0 <= m_lDataTimer)
	{
		return RET_FAIL;
	}

	ACE_Time_Value tv(RTP_DATA_INTERVAL_MS, 0);
	m_lDataTimer = CRtspReactorManager::instance()->RegistTimer((ACE_Event_Handler*)this, m_unSessionIndex, tv, tv);
	if(-1 == m_lDataTimer)
	{
		return RET_FAIL;
	}
	IVS_DBG_LOG("register RTP receive timer success.");
	return RET_OK;

}

/*
 * KillRtpTimer
 * ɾ��RTP���ݼ�ⶨʱ��
 */
void CRtspSession::KillRtpTimer()
{
	//ACE_Guard <ACE_Thread_Mutex> locker (m_Mutex);
	if (-1 != m_lDataTimer)
	{
		// ȡ����ʱ��
		(void)CRtspReactorManager::instance()->unRegistTimer(m_lDataTimer, m_unSessionIndex);
		IVS_DBG_LOG("rtsp session[%p] cancel receive timer success.", this);
		m_lDataTimer = -1;
	}
	return;
}

bool CRtspSession::CheckRtpTimeout()
{
	if (m_bDataRecved)
	{
		m_bDataRecved = false;
		m_uTmoCounter = 0;
		return false;
	}

	// ���ձ�ʶΪ false
    m_uTmoCounter++;
	if (RTP_TIME_OUT_MAX <= m_uTmoCounter)
	{
		return true;
	}
	return false;
}

int CRtspSession::InitData()
{
	// ����������
	if (NULL == m_pRecvBuffer)
	{
		try
		{
			m_pRecvBuffer = new Static_PreAssign_Buffer;
		}
		catch(...)
		{
			IVS_RUN_LOG_CRI("create m_pRecvBuffer fail.");
			return RET_FAIL;
		}
		m_pRecvBuffer->wr_ptr(0);
		m_pRecvBuffer->rd_ptr(0);
	}

	return RET_OK;
}
