/********************************************************************
	filename	: 	SocketService.h
	author		:	s00194833
	created		:	2011/12/07	
	description	:	SocketͨѶ��
	copyright	:	Copyright (C) 2011-2015
	history		:	2011/12/07 ��ʼ�汾
*********************************************************************/
#include "SocketService.h"
#include "KBInternalDef.h"
#include "bp_log.h"

#define LISTEN_PORT_NUMBER 5

bool CSocketService::m_bInitLabel = false;
int CSocketService::m_iInUse = 0;

CSocketService::CSocketService()
{
    m_hSocket = INVALID_SOCKET;
    m_hConnectSocket = INVALID_SOCKET;

    m_ServerAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
    m_ServerAddr.sin_family = AF_INET;
    m_ServerAddr.sin_port = htons(0);
    memset(m_ServerAddr.sin_zero, 0, sizeof(m_ServerAddr.sin_zero));

    m_ClientAddr.sin_addr.S_un.S_addr = inet_addr("");
    m_ClientAddr.sin_family = AF_INET;
    m_ClientAddr.sin_port = htons(0);
    memset(m_ClientAddr.sin_zero, 0, sizeof(m_ClientAddr.sin_zero));
};

CSocketService::~CSocketService()
{
    m_hConnectSocket = INVALID_SOCKET;
    m_hSocket = INVALID_SOCKET;
};

/**************************************************************************
* name       : InitSock
* description: ��ʼ��SocketͨѶ��
* input      : NA
* output     : NA
* return     : IVS_SUCCEED �ɹ������� ʧ��
* remark     : NA
**************************************************************************/
int CSocketService::InitSock()
{
    //���ͨѶ��û�д�ִ�����²���
    //�Ѿ���ֱ�ӷ���
    if (!m_bInitLabel)
    {
        WORD wVersionRequested;
        WSADATA wsaData;
        int iRet = 0;

        wVersionRequested = MAKEWORD(2,2);

        //��socketͨѶ��
        iRet = WSAStartup(wVersionRequested,&wsaData);
        if (0 != iRet)
        {
            return iRet;
        }

        //���汾���Ƿ�Ϊ1.1
        if ((2 != LOBYTE(wsaData.wVersion))||(2 != HIBYTE(wsaData.wVersion)))
        {
            FreeSock();
            return IVS_FAIL;
        }
        m_bInitLabel = true;
        m_iInUse++;
    }
    return IVS_SUCCEED;
}

/**************************************************************************
* name       : FreeSock
* description: �ͷ�SocketͨѶ����Դ
* input      : NA
* output     : NA
* return     : NA
* remark     : NA
**************************************************************************/
void CSocketService::FreeSock()
{
    m_iInUse--;
    //����ʹ��ʱ���ر�ͨѶ��
    if (0 == m_iInUse)
    {
        (void)WSACleanup();
        m_bInitLabel = false;
    }
}

/**************************************************************************
* name       : CreateSocket
* description: �������ֽ�
* input      : iAF��ַ�� ֻ֧��AF_INET
               iType Socket���� ֧��SOCK_STREAM��SOCK_DGRAM
               iRrotocol Э�鷽�� IPPROTO_TCP��IPPROTO_UDP
* output     : NA
* return     : IVS_SUCCEED �ɹ������� ʧ��
* remark     : NA
**************************************************************************/
int CSocketService::CreateSocket(int iAF,int iType,int iProtocol)
{
    if(m_hSocket != INVALID_SOCKET)
    {
        BP_RUN_LOG_INF("NetKeyBoard Needn't to Create Socket", "NA");
        return IVS_SUCCEED;
    }

    m_hSocket = socket(iAF,iType,iProtocol);

    if (INVALID_SOCKET == m_hSocket)
    {
        int iRet;
        iRet = WSAGetLastError();
        BP_RUN_LOG_INF("NetKeyBoard Create Socket failed", "iRet:%d", iRet);
        return iRet;
    }

	//����رն˿ڼ�������������ʹ������˿ڵ�����
	long lReuseAddrFlag = 1;
	if (setsockopt((SOCKET)m_hSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&lReuseAddrFlag,
		sizeof(lReuseAddrFlag)) < 0)
	{	
		int iRet;
		iRet = WSAGetLastError();
		BP_RUN_LOG_INF("setsockopt client socket failed", "error(%d)", iRet);
		(void)CloseSocket();
		return iRet;
	}

    return IVS_SUCCEED;
}


/**************************************************************************
* name       : SetServerAddr
* description: ���÷�����IP�Ͷ˿ں�
* input      : iPort �������˿ں�
               pszIP ������IP
* output     : NA
* return     : NA
* remark     : NA
**************************************************************************/
void CSocketService::SetServerAddr(u_short iPort,const char *pszIP)
{
    if (NULL != pszIP)
    {
        m_ServerAddr.sin_addr.S_un.S_addr = inet_addr(pszIP);
    }
    m_ServerAddr.sin_port = htons(iPort);
}

int CSocketService::Connect(int iTimeout)
{
    if( INVALID_SOCKET == m_hSocket)
    {
        return SDK_SOCKET_INVALID;
    }

    //���÷�������ʽ����
    u_long ulNonBlock = 1;
    int iRet = 0;
    iRet = ioctlsocket(m_hSocket,FIONBIO,(u_long*)&ulNonBlock);//lint !e569
    if(SOCKET_ERROR == iRet)
    {
        iRet = WSAGetLastError();
        return iRet;
    }

    iRet = connect(m_hSocket,(sockaddr *)&m_ServerAddr,sizeof(sockaddr));
    //���ӳɹ�
    if(0 == iRet)
    {
        return IVS_SUCCEED;
    }
    else if(SOCKET_ERROR == iRet)
    {
        iRet = WSAGetLastError();
        if(WSAEWOULDBLOCK == iRet)//��ʱ����
        {
            timeval timeout;
            timeout.tv_sec = iTimeout;
            timeout.tv_usec = 0;

            fd_set wfd;
            FD_ZERO(&wfd);
            FD_SET(m_hSocket,&wfd);

            int nfds = select(1,(fd_set*)0,&wfd,(fd_set*)0,&timeout);
            //select ����
            if(nfds < 0)
            {
                iRet = WSAGetLastError();
                return iRet;
            }
            //��ʱ
            else if(nfds ==0)
            {
                return SDK_SOCKET_TIMEOUT;
            }
            //�Ѿ�����
            else
            {
                int iErr = -1;
                int len = sizeof(int);

                //��ȡsocket��Ϣ����
                iRet = getsockopt(m_hSocket, SOL_SOCKET, SO_ERROR, (char*)&iErr, &len);
                if ( iRet< 0) 
                { 
                    iRet = WSAGetLastError();
                    return iRet;
                }
                //���ӷ�������
                if(0 != iErr)
                {
                    return IVS_FAIL;
                }
            }
        }
        else//��������
        {
            return iRet;
        }
    }

    return IVS_SUCCEED;
}
/**************************************************************************
* name       : Listen
* description: �����˿�
* input      : NA
* output     : NA
* return     : IVS_SUCCEED �ɹ������� ʧ��
* remark     : NA
**************************************************************************/
int CSocketService::Listen()
{
    if (INVALID_SOCKET == m_hSocket)
    {
        return SDK_SOCKET_INVALID;
    }

    int iRet;
    iRet = bind(m_hSocket,(sockaddr*)&m_ServerAddr,sizeof(sockaddr));
    if (SOCKET_ERROR == iRet)
    {
        iRet = WSAGetLastError();
        return iRet;
    }

    iRet = listen(m_hSocket,LISTEN_PORT_NUMBER);
    if (SOCKET_ERROR == iRet)
    {
        iRet = WSAGetLastError();
        return iRet;
    }
    return IVS_SUCCEED;
}

// UDP�����˿�
int CSocketService::UDPListen()
{
    if (INVALID_SOCKET == m_hSocket)
    {
        return SDK_SOCKET_INVALID;
    }

    int iRet;
    iRet = bind(m_hSocket,(sockaddr*)&m_ServerAddr,sizeof(sockaddr));
    if (SOCKET_ERROR == iRet)
    {
        iRet = WSAGetLastError();
        return iRet;
    }
    return IVS_SUCCEED;
}

/**************************************************************************
* name       : AcceptClientRequst
* description: �ȴ��ͻ�����
* input      : iTimeout ���ӳ�ʱʱ��
* output     : NA
* return     : IVS_SUCCEED �ɹ������� ʧ��
* remark     : NA
**************************************************************************/
int CSocketService::AcceptClientRequst(int iTimeout)
{
    if (INVALID_SOCKET == m_hSocket)
    {
        return SDK_SOCKET_INVALID;
    }

    timeval timeout;
    timeout.tv_sec = iTimeout;
    timeout.tv_usec = 0;

    fd_set rfd;
    FD_ZERO(&rfd);
    FD_SET(m_hSocket,&rfd);

    int nfds = select(1,&rfd,(fd_set*)0,(fd_set*)0,&timeout);

    int iRet = 0;
    //ֻ��������Ӧ��
    if(nfds > 0)
    {
        FD_CLR(m_hSocket,&rfd);
        int iAddrLen = sizeof(sockaddr);
        m_hConnectSocket = accept(m_hSocket,(sockaddr*)&m_ClientAddr,&iAddrLen);
        if(INVALID_SOCKET == m_hConnectSocket)
        {
            iRet = WSAGetLastError();
            return iRet;
        }
    }
    //��ʱ��
    else if(0 == nfds)
    {
        return SDK_SOCKET_TIMEOUT;
    }
    //������
    else
    {
        iRet = WSAGetLastError();
        return iRet;
    }

    return IVS_SUCCEED;
}

/**************************************************************************
* name       : Send
* description: �������� 
* input      : pszSendData �������ݻ�����
               iLength �������ݳ���
               iTimeout ��ʱʱ��
* output     : NA
* return     : totalSendBytes  �ѷ��͵����ݳ���
* remark     : NA
**************************************************************************/
int CSocketService::Send(const char *pszSendData,int iLength,int iTimeout)
{
    if (NULL == pszSendData)
    {
        return IVS_FAIL;
    }

    //��Ϊ��
    iTimeout *= 1000;

    int iRet = 0;;
    iRet = setsockopt(m_hConnectSocket,SOL_SOCKET,SO_RCVTIMEO,(char *)&iTimeout,sizeof(iTimeout));
    if (SOCKET_ERROR == iRet)
    {
        iRet = WSAGetLastError();
        return iRet;
    }

    int totalBytes = iLength;
    int totalSendBytes = 0;
    int sendBytes = 0;

    //�������ݱ�֤���ݿ���ȫ������
    while(totalSendBytes<totalBytes)
    {
        sendBytes = send(m_hConnectSocket,pszSendData+totalSendBytes,totalBytes-totalSendBytes,0);
        if(sendBytes<=0)
        {
            break;
        }
        totalSendBytes+=sendBytes;
    }

    return totalSendBytes;
}

/**************************************************************************
* name       : Receive
* description: ��������
* input      : iTimeout ��ʱʱ��
* output     : pszRecieveData �������ݻ�����
               iLength �������ݻ���������
* return     : iReceiveLength ʵ�ʽ������ݳ���
* remark     : NA
**************************************************************************/
int CSocketService::Receive(char *pszReceiveData,u_int iLength,int iTimeout)
{
    if (NULL == pszReceiveData)
    {
        return IVS_FAIL;
    }
 
    //��Ϊ��
    iTimeout *= 1000;//��Ϊ��

    int iRet = 0;
    iRet = setsockopt(m_hConnectSocket,SOL_SOCKET,SO_RCVTIMEO,(char *)&iTimeout,sizeof(iTimeout));
    if (SOCKET_ERROR == iRet)
    {
        iRet = WSAGetLastError();
        return iRet;
    }

    int iReceiveLength =0 ;
    iReceiveLength = recv(m_hConnectSocket,pszReceiveData,(int)iLength,0);
    return iReceiveLength;
}

// UDP��������
int CSocketService::UDPReceive(char *pszReceiveData,u_int iLength, int iTimeout)
{
    if (NULL == pszReceiveData)
    {
		BP_RUN_LOG_ERR(IVS_FAIL, "NetKeyBoard Para Is NULL!","NA");
        return IVS_FAIL;
    }
    int iRet = 0;
    iRet = setsockopt(m_hSocket,SOL_SOCKET,SO_RCVTIMEO,(char *)&iTimeout,sizeof(iTimeout));
    if (SOCKET_ERROR == iRet)
    {
        iRet = WSAGetLastError();
		BP_RUN_LOG_ERR(iRet, "NetKeyBoard Setsockopt failed!","NA");
        return iRet;
    }
    int iReceiveLength =0 ;
    int	len = sizeof(SOCKADDR);
    iReceiveLength = recvfrom(m_hSocket,pszReceiveData,(int)iLength,0,(SOCKADDR*)&m_ClientAddr,&len);
    return iReceiveLength;
}
/**************************************************************************
* name       : CloseConnectSocket
* description: �ر��������ֽ�
* input      : NA
* output     : NA
* return     : NA
* remark     : NA
**************************************************************************/
void CSocketService::CloseConnectSocket()
{
    if (INVALID_SOCKET != m_hConnectSocket)
    {
		for (int i = 0; i < 15; i++) //�رղ��ɹ��Ļ����ظ�ȥ��
		{
			int iRet = closesocket(m_hConnectSocket);
			if (SOCKET_ERROR != iRet) //�رճɹ������ɹ��Ļ���Ȼ����SOCKET_ERROR
			{
				m_hConnectSocket = INVALID_SOCKET;
				break;
			}

			iRet = WSAGetLastError();
			BP_RUN_LOG_ERR(iRet, "NetKeyBoard CloseConnectSocket failed!","NA");
			Sleep(100);
		}
    }
}

/**************************************************************************
* name       : CloseSocket
* description: �ر����ֽ�
* input      : NA
* output     : NA
* return     : NA
* remark     : NA
**************************************************************************/
void CSocketService::CloseSocket()
{
    if (INVALID_SOCKET != m_hSocket)
    {
		for (int i = 0; i < 15; i++)
		{
			int iRet = closesocket(m_hSocket);
			if (SOCKET_ERROR != iRet)
			{
				m_hSocket = INVALID_SOCKET;
				break;
			}

			iRet = WSAGetLastError();
			BP_RUN_LOG_ERR(iRet, "NetKeyBoard CloseSocket failed!","NA");
			Sleep(100);
		}
    }
}
