/********************************************************************
 filename    :    ConnMgr.h
 author      :    g00209332
 created     :    2012/12/19
 description :    ���ӹ���
 copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
 history     :    2012/12/19 ��C30�̳�
*********************************************************************/

#ifndef CONN_MGR
#define CONN_MGR

#ifdef ENV_LINUX
 #include <sys/types.h>
 #include <sys/socket.h>
 #include <netinet/in.h>
 #include <arpa/inet.h>
 #include <sys/epoll.h>
#endif //VOS_OS_LINUX

// #ifdef WIN32
// #include <winsock2.h>
// #endif

/*!< ����ͷ�ļ� */
#include <list>

/*!< ������Ϣͷ�ļ� */
#include "vos_config.h"

/*!< �����������Ͷ���ͷ�ļ� */
#include "vos_basetype.h"

/*!< ģ�������õĻ���ͷ�ļ� */
#include "vos_common.h"

/*!< ģ�����ͷ�ļ� */
#include "vos.h"
#include "ToolsHelp.h"

#define InvalidFd -1
#define InvalidSocket -1
#define InvalidIp INADDR_ANY
#define Invalidport 0
#define DEFAULT_SENDRECV_SIZE (1024 * 1024)

//BEGIN R001C05LITSB010 ADD 2010-07-12 ligengqiang 00124479 for ¼�������������ƣ�����Ӧ��������С
#define MIN_RECV_SIZE (16 * 1024)   //��С�Ľ��ջ�������С
//END   R001C05LITSB010 ADD 2010-07-12 ligengqiang 00124479 for ¼�������������ƣ�����Ӧ��������С

#define SendRecvError -1 //���ͻ���մ���
#define SendRecvErrorTIMEO -2 //���ͻ���ճ�ʱ
#define SendRecvErrorEBADF -3 //socket�������
#define SendRecvErrorEOF -4 //tcp����

#define MAX_LISTEN_QUEUE_SIZE 2000
#define EPOLL_MAX_EVENT (MAX_LISTEN_QUEUE_SIZE + 1000)
#define MAX_EPOLL_FD_SIZE 3000
#define LINGER_WAIT_SECONDS 1 //LINGER�ȴ�ʱ��(seconds)

//�������ӹ������ô�����
#if VOS_APP_OS == VOS_OS_LINUX
 #define CONN_ERR_TIMEO
 #define CONN_ERR_EBADF EBADF
#elif VOS_APP_OS == VOS_OS_WIN32
 #define CONN_ERR_EBADF WSAEINTR
 #define CONN_ERR_TIMEO WSAETIMEDOUT
#endif

#if VOS_APP_OS == VOS_OS_WIN32
enum tagSockEvent
{
    EPOLLIN  = 0x1,
    EPOLLOUT = 0x2
};

 #ifndef INET_ADDRSTRLEN
  #define  INET_ADDRSTRLEN 16
 #endif

 #ifndef MSG_WAITALL
  #define MSG_WAITALL 0
 #endif

 #ifndef MSG_DONTWAIT
  #define MSG_DONTWAIT 0
 #endif

 #ifndef MSG_NOSIGNAL
  #define MSG_NOSIGNAL 0
 #endif

 #ifndef socklen_t
  #define socklen_t int
 #endif

#endif //#if win32

class CNetworkAddr
{
public:
    CNetworkAddr();
    ~CNetworkAddr();
public:
    long m_lIpAddr;   //��������
    USHORT m_usPort;  //��������
};

typedef enum tagConnStatus
{
    enIdle = 0,
    enConnecting,
    enConnected,
    enConnFailed,
    enClosed
} ConnStatus;

typedef enum tagEnumSyncAsync
{
    enSyncOp  = 1,
    enAsyncOp = 0
} EnumSyncAsync;

typedef enum tagEpollEventType
{
    enEpollRead  = 0,
    enEpollWrite = 1
} EpollEventType;

class CHandle;
class CHandleNode;
typedef std::list <CHandleNode *> ListOfHandle;
typedef ListOfHandle::iterator    ListOfHandleIte;

class CHandle
{
public:
    CHandle();
    virtual ~CHandle();

public:

    /**************************************************************************
    * name       : initHandle
    * description: ��ʼ������
    * input      : NA
    * output     : NA
    * return     : VOS_SUCCESS    init success
                   VOS_FAIL       init fail
    * remark     : NA
    **************************************************************************/
    virtual long initHandle(void);

    /**************************************************************************
    * name       : setHandleSend
    * description: �����Ƿ���д�¼�
    * input      : bHandleSend: VOS_TRUE��ʾ��⣬VOS_FALSE��ʾ�����
    * output     : NA
    * return     : NA
    * remark     : NA
    **************************************************************************/
    virtual void setHandleSend(VOS_BOOLEAN bHandleSend);

    /**************************************************************************
    * name       : setHandleRecv
    * description: �����Ƿ�����¼�
    * input      : bHandleSend: VOS_TRUE��ʾ��⣬VOS_FALSE��ʾ�����
    * output     : NA
    * return     : NA
    * remark     : NA
    **************************************************************************/
    virtual void setHandleRecv(VOS_BOOLEAN bHandleRecv);

    /**************************************************************************
    * name       : getEvents
    * description: ȡ�¼�ֵ
    * input      : NA
    * output     : NA
    * return     : �¼�ֵ
    * remark     : NA
    **************************************************************************/
    ULONG getEvents(void)
    {
        if (m_pMutexHandle != NULL)
        {
            (void)VOS_MutexLock(m_pMutexHandle);
        }

        ULONG ulEvents = m_ulEvents;

        if (m_pMutexHandle != NULL)
        {
            (void)VOS_MutexUnlock(m_pMutexHandle);
        }

        return ulEvents;
    };

    /**************************************************************************
    * name       : close
    * description: �ر���������
    * input      : NA
    * output     : NA
    * return     : NA
    * remark     : NA
    **************************************************************************/
    virtual void close(void);

public:
    long m_lSockFD;					//����SOCKET
    CHandleNode *m_pHandleNode;		//Node�ڵ�
    CNetworkAddr m_localAddr;		//�����ַ

#if VOS_APP_OS == VOS_OS_WIN32
    VOS_BOOLEAN m_bReadSelected;
    VOS_BOOLEAN m_bWriteSelected;
#endif  //#if

#if VOS_APP_OS == VOS_OS_LINUX
    long m_lEpfd;
#endif  //#if
    ULONG m_ulEvents;				//��Ӧ�¼�
    VOS_Mutex *m_pMutexHandle;		//������
};

class CHandleNode
{
public:
    CHandleNode() : m_pHandle(NULL), m_bRemoved(VOS_FALSE)
    {
    };

public:
    CHandle *m_pHandle;			//����Handle
    VOS_BOOLEAN m_bRemoved;		//�Ƿ��Ѿ�ɾ��
};

class CNetworkHandle : public CHandle
{
public:
    CNetworkHandle();
    virtual ~CNetworkHandle(){};

public:

    /**************************************************************************
    * name       : initHandle
    * description: ��ʼ����������
    * input      : NA
    * output     : NA
    * return     : NA
    * remark     : NA
    **************************************************************************/
    virtual long initHandle(void);

    /**************************************************************************
    * name       : getSockFD
    * description: ȡSockFD
    * input      : NA
    * output     : NA
    * return     : SockFD
    * remark     : NA
    **************************************************************************/
    long getSockFD(void) const /*lint -e1714*/ //�ӿں��������಻����
    {
        return m_lSockFD;
    };
#if VOS_APP_OS == VOS_OS_LINUX
    long         sendMsg(const struct msghdr *pMsg);
#endif

    /**************************************************************************
    * name       : recv
    * description: ���պ���
    * input      : ulDataSize  ���ݳ���
                   bSyncRecv   VOS_TRUE��ʾͬ����VOS_FALSE��ʾ�첽
    * output     : pArrayData  ����buffer
                   pPeerAddr   �Զ˵�ַ
    * return     : �����ֽ���(>0)
                   SendRecvError  ����ʧ��
    * remark     : NA
    **************************************************************************/
    virtual long recv(char *pArrayData, CNetworkAddr *pPeerAddr, const ULONG ulDataSize,
                      const EnumSyncAsync bSyncRecv);

public:

    /**************************************************************************
    * name       : handle_recv
    * description: �ѽ��յ����¼��ŵ��¼������к���
    * input      : NA
    * output     : NA
    * return     : NA
    * remark     : NA
    **************************************************************************/
    virtual void handle_recv(void);

    /**************************************************************************
    * name       : handle_send
    * description: ��������
    * input      : NA
    * output     : NA
    * return     : NA
    * remark     : NA
    **************************************************************************/
    virtual void handle_send(void);
};

class CTcpConnHandle : public CNetworkHandle
{
public:
    CTcpConnHandle();
    virtual ~CTcpConnHandle();

public:

    /**************************************************************************
    * name       : initHandle
    * description: ��ʼ������
    * input      : NA
    * output     : NA
    * return     : VOS_SUCCESS      init success
                   VOS_FAIL         init fail
    * remark     : NA
    **************************************************************************/
    virtual long initHandle(void);

    /**************************************************************************
    * name       : conn
    * description: �������Ӻ���
    * input      : pLocalAddr  ���ص�ַ
                   pPeerAddr   �Զ˵�ַ
                   bSyncConn   VOS_TRUE��ʾͬ�����ӣ�VOS_FALSE��ʾ�첽����
                   ulTimeOut   ��ʱʱ��
    * output     : NA
    * return     : VOS_SUCCESS      connect success
                   VOS_FAIL         connect fail
    * remark     : NA
    **************************************************************************/
    virtual long conn( const CNetworkAddr *pLocalAddr, const CNetworkAddr *pPeerAddr,
                       const EnumSyncAsync bSyncConn, ULONG ulTimeOut);

    long syn2Asy(const long lSockFd, const EnumSyncAsync bSyncConn, ULONG ulTimeOut);

    long connPeer(const long lSockFd, const CNetworkAddr *pPeerAddr, 
                  const EnumSyncAsync bSyncConn, ULONG ulTimeOut);

    long connMode(const long lSockFd, const EnumSyncAsync bSyncConn, ULONG ulTimeOut);
    
    /**************************************************************************
    * name       : send
    * description: ���ͺ���
    * input      : pArrayData  ����buffer
                   ulDataSize  ���ݳ��ȣ�
                   bSyncSend   VOS_TRUE��ʾͬ�����ͣ�VOS_FALSE��ʾ�첽����
                   bSyncSend   ͬ��/�첽
    * output     : NA
    * return     : �����ֽ���(>0)
                   SendRecvError  ����ʧ��
    * remark     : NA
    **************************************************************************/
    virtual long send(const char *pArrayData, const ULONG ulDataSize,
                      const EnumSyncAsync bSyncSend);

    /**************************************************************************
    * name       : send_n
    * description: ����ȫ������
    * input      : pArrayData  ����buffer
                   ulDataSize  ���ݳ���
    * output     : NA
    * return     : �����ֽ���(>0)
                   SendRecvError  ����ʧ��
    * remark     : NA
    **************************************************************************/
    virtual long send_n(const char *pArrayData, const ULONG ulDataSize);

    /**************************************************************************
    * name       : recv
    * description: ���պ���
    * input      : ulDataSize  ���ݳ���
                   bSyncRecv   VOS_TRUE��ʾͬ����VOS_FALSE��ʾ�첽
    * output     : pArrayData  ����buffer
                   pPeerAddr   �Զ˵�ַ
    * return     : �����ֽ���(>0)
                   SendRecvError  ����ʧ��
    * remark     : NA
    **************************************************************************/
    virtual long recv(char *pArrayData, CNetworkAddr *pPeerAddr, const ULONG ulDataSize,
                      const EnumSyncAsync bSyncRecv);

    /**************************************************************************
    * name       : recvWithTimeout
    * description: ���պ���
    * input      : ulDataSize   ���ݳ���
                   ulTimeOut    �ȴ�ʱ��
                   ulSleepTime  �����(ms)
    * output     : pArrayData  ����buffer
                   pPeerAddr   �Զ˵�ַ
    * return     : �����ֽ���(>0)
                   SendRecvError  ����ʧ��
    * remark     : NA
    **************************************************************************/
    virtual long recvWithTimeout(char *pArrayData, CNetworkAddr *pPeerAddr,
                                 const ULONG ulDataSize, const ULONG ulTimeOut, const ULONG ulSleepTime);

    /**************************************************************************
    * name       : getStatus
    * description: ���պ���
    * input      : NA
    * output     : NA
    * return     : ����״̬
    * remark     : NA
    **************************************************************************/
    virtual ConnStatus getStatus(void) const
    {
        return m_lConnStatus;
    };

    /**************************************************************************
    * name       : close
    * description: �ر�����
    * input      : NA
    * output     : NA
    * return     : NA
    * remark     : NA
    **************************************************************************/
    virtual void close(void);

    //BEGIN R001C05LITSB010 ADD 2010-07-12 ligengqiang 00124479 for ¼�������������ƣ�����Ӧ��������С
    //���ý��� Buffer��С
    void         SetRecvBufferSize(unsigned long ulSize);
    //END   R001C05LITSB010 ADD 2010-07-12 ligengqiang 00124479 for ¼�������������ƣ�����Ӧ��������С

public:
    ConnStatus m_lConnStatus;	//����״̬
    CNetworkAddr m_peerAddr;	//�Զ˵�ַ

    //BEGIN R001C05LITSB010 ADD 2010-07-12 ligengqiang 00124479 for ¼�������������ƣ�����Ӧ��������С
    unsigned long m_ulRecvBufferSize;		//����Buffer��С
    //END   R001C05LITSB010 ADD 2010-07-12 ligengqiang 00124479 for ¼�������������ƣ�����Ӧ��������С
};

class CTcpServerHandle : public CHandle
{
public:

    /**************************************************************************
    * name       : listen
    * description: �����ȴ��Զ�����
    * input      : pLocalAddr  ���ص�ַ
    * output     : NA
    * return     : VOS_SUCCESS       listen success
                   VOS_FAIL          listen fail
    * remark     : NA
    **************************************************************************/
    long         listen(const CNetworkAddr *pLocalAddr);

public:

    /**************************************************************************
    * name       : handle_accept
    * description: ʵ�ֻ����handle_accept�����ڽ��յ�tcp���Ӻ󣬲���TcpConnHandle
    * input      : pRemoteAddr  Զ�̽�������ַ
                   pTcpConnHandle ���Ӿ��
    * output     : NA
    * return     : VOS_SUCCESS     �ɹ�
                   VOS_FAIL        ʧ��
    * remark     : NA
    **************************************************************************/
    virtual long handle_accept(const CNetworkAddr *pRemoteAddr,
                               CTcpConnHandle *&   pTcpConnHandle);

    /**************************************************************************
    * name       : close
    * description: �ر�����
    * input      : NA
    * output     : NA
    * return     : NA
    * remark     : NA
    **************************************************************************/
    virtual void close(void);
};

#define MAX_HANDLE_MGR_TYPE_LEN 20
class CHandleManager
{
public:
    CHandleManager();
    virtual ~CHandleManager();

public:

    /**************************************************************************
    * name       : init
    * description: ��ʼ������
    * input      : ulSelectPeriod    �¼����������λΪms
    * output     : NA
    * return     : VOS_SUCCESS       init success
                   VOS_FAIL          init fail
    * remark     : NA
    **************************************************************************/
    long          init(const ULONG ulMilSeconds);

    /**************************************************************************
    * name       : run
    * description: �����̣߳������¼������ѭ��
    * input      : ulSelectPeriod    �¼����������λΪms
    * output     : NA
    * return     : VOS_SUCCESS       success
                   VOS_FAIL          fail
    * remark     : NA
    **************************************************************************/
    long          run();

    /**************************************************************************
    * name       : exit
    * description: �����˳�֪ͨ���¼������ѭ�����˳��߳�
    * input      : NA
    * output     : NA
    * return     : NA
    * remark     : NA
    **************************************************************************/
    void          exit();

public:

    /**************************************************************************
    * name       : addHandle
    * description: ע����Ҫ����¼���handle
    * input      : pHandle  ��Ҫ����¼���handle
    * output     : NA
    * return     : NA
    * remark     : NA
    **************************************************************************/
    long          addHandle(CHandle *pHandle,
                            VOS_BOOLEAN       bIsListOfHandleLocked = VOS_FALSE);

    long addHandle2List(CHandle *   pHandle,
                        CHandleNode *pHandleNode);
    /**************************************************************************
    * name       : removeHandle
    * description: ȡ��ע����Ҫ����¼���handle
    * input      : pHandle  ��Ҫ����¼���handle
    * output     : NA
    * return     : NA
    * remark     : NA
    **************************************************************************/
    void          removeHandle(CHandle *pHandle);

    /**************************************************************************
    * name       : checkSelectResult
    * description: ���ݵõ����¼�������Ӧ��handle�����¼�
    * input      : enEpEvent   ��⵽���¼�
                   pHandle     ��Ҫ����¼���handle
    * output     : NA
    * return     : NA
    * remark     : NA
    **************************************************************************/
    virtual void  checkSelectResult(const EpollEventType enEpEvent,
                                    CHandle *             pHandle);

protected:

    /**************************************************************************
    * name       : invoke
    * description: �����̣߳������¼������ѭ��
    * input      : rgc    �������ʵ��ָ��
    * output     : NA
    * return     : VOS_SUCCESS       success
                   VOS_FAIL          fail
    * remark     : NA
    **************************************************************************/
    static void * invoke(void *argc);

    /**************************************************************************
    * name       : mainLoop
    * description: �¼������ѭ��
    * input      : NA
    * output     : NA
    * return     : NA
    * remark     : NA
    **************************************************************************/
    void          mainLoop();
    void setSelectEvent();

    void netEventDeal();

protected:
    ListOfHandle m_listHandle;				//Handle�б�
	ListOfHandleIte m_curListIte;			//��ǰ������λ�� 2012-12-26 ������ fd_setֻ��64��������64�����ӾͲ�������
    VOS_Mutex *m_pMutexListOfHandle;		//Handle������

#if VOS_APP_OS == VOS_OS_LINUX
    long m_lEpfd; //����epoll�ľ��
    struct epoll_event m_epEvents[EPOLL_MAX_EVENT];
#elif VOS_APP_OS == VOS_OS_WIN32
    fd_set m_readSet;
    fd_set m_writeSet;
    timeval m_stSelectPeriod;            //select����
#endif

    ULONG m_ulSelectPeriod;
    VOS_Thread *m_pVosThread;			//�����̶߳���
    VOS_BOOLEAN m_bExit;				//�Ƿ��˳�
    char m_szMgrType[MAX_HANDLE_MGR_TYPE_LEN];//��������
};

class CTcpConnMgr : public CHandleManager
{
public:
    CTcpConnMgr()
    {
        if(!CToolsHelp::Strncpy(m_szMgrType, MAX_HANDLE_MGR_TYPE_LEN, "CTcpConnMgr", strlen("CTcpConnMgr")))
		{
			
		}
    };

    /**************************************************************************
    * name       : lockListOfHandle
    * description: ��List Handle ����
    * input      : NA
    * output     : NA
    * return     : NA
    * remark     : NA
    **************************************************************************/
    void         lockListOfHandle();

    /**************************************************************************
    * name       : unlockListOfHandle
    * description: ��List Handle ����
    * input      : NA
    * output     : NA
    * return     : NA
    * remark     : NA
    **************************************************************************/
    void         unlockListOfHandle();

protected:

    /**************************************************************************
    * name       : checkSelectResult
    * description: ���ݵõ����¼�������Ӧ��handle�����¼�
    * input      : enEpEvent   ��⵽���¼�
                   pHandle     ��Ҫ����¼���handle
    * output     : NA
    * return     : NA
    * remark     : NA
    **************************************************************************/
    virtual void checkSelectResult(const EpollEventType enEpEvent,
                                   CHandle *            pHandle); /*lint !e1768*/ //��Ҫ�������θýӿ�
};

class CTcpServerMgr : public CHandleManager
{
public:
    CTcpServerMgr() : m_pTcpConnMgr(NULL)
    {
        if(!CToolsHelp::Strncpy(m_szMgrType, MAX_HANDLE_MGR_TYPE_LEN, "CTcpServerMgr", strlen("CTcpServerMgr")))
		{
			
		}
    };

public:

    /**************************************************************************
    * name       : setTcpClientMgr
    * description: ����TCP���ӹ������
    * input      : pTcpConnMgr  TCP���ӹ������
    * output     : NA
    * return     : NA
    * remark     : NA
    **************************************************************************/
    void setTcpClientMgr(CTcpConnMgr *pTcpConnMgr)
    {
        m_pTcpConnMgr = pTcpConnMgr;
    };

protected:

    /**************************************************************************
    * name       : checkSelectResult
    * description: ���ݵõ����¼�������Ӧ��handle�����¼�
    * input      : enEpEvent   ��⵽���¼�
                   pHandle     ��Ҫ����¼���handle
    * output     : NA
    * return     : NA
    * remark     : NA
    **************************************************************************/
    virtual void checkSelectResult(const EpollEventType enEpEvent,
                                   CHandle *            pHandle); /*lint !e1768*/ //��Ҫ�������θýӿ�

    long readEvent(const EpollEventType enEpEvent, CTcpServerHandle *pTcpServerHandle);


protected:
    CTcpConnMgr *m_pTcpConnMgr;			// ���ӹ���
};

#define DEFAULT_SELECT_PERIOD 20

// 4����־����
#define    CONN_OPERATOR_LOG 16
#define    CONN_RUN_LOG 17
#define    CONN_SECURITY_LOG 20
#define    CONN_USER_LOG 19

class CConnMgr
{
public:
    CConnMgr();
    ~CConnMgr();

public:

    /**************************************************************************
    * name       : init
    * description: ��ʼ������
    * input      : NA
    * output     : NA
    * return     : VOS_SUCCESS    init success
                   VOS_FAIL       init fail
    * remark     : NA
    **************************************************************************/
    virtual long init(const ULONG ulSelectPeriod, const VOS_BOOLEAN bHasUdpSock,
                      const VOS_BOOLEAN bHasTcpClient, const VOS_BOOLEAN bHasTcpServer);

    /**************************************************************************
    * name       : run
    * description: ��������manager
    * input      : NA
    * output     : NA
    * return     : VOS_SUCCESS   start success
                   VOS_FAIL      start fail
    * remark     : NA
    **************************************************************************/
    virtual long run(void);

    /**************************************************************************
    * name       : exit
    * description: �˳�����manager
    * input      : NA
    * output     : NA
    * return     : NA
    * remark     : NA
    **************************************************************************/
    void exit(void);

public:

    /**************************************************************************
    * name       : setDefaultLocalAddr
    * description: ���ñ���ȱʡ��ַ
    * input      : szLocalIpAddr  ���ص�ַ
    * output     : NA
    * return     : NA
    * remark     : NA
    **************************************************************************/
    virtual void setDefaultLocalAddr(const char *szLocalIpAddr);

    /**************************************************************************
    * name       : regTcpClient
    * description: ����TCP�ͻ���
    * input      : pLocalAddr  ���ص�ַ��
                   pPeerAddr   �Զ˵�ַ��
                   pTcpConnHandle   ���Ӷ�Ӧ��handle
                   bSyncConn        VOS_TRUE��ʾͬ�����ӣ�VOS_FALSE��ʾ�첽����
    * output     : NA
    * return     : VOS_SUCCESS  connect success
                   VOS_FAIL     connect fail
    * remark     : NA
    **************************************************************************/
    virtual long regTcpClient( const CNetworkAddr *pLocalAddr,
                               const CNetworkAddr *pPeerAddr, CTcpConnHandle *pTcpConnHandle,
                               const EnumSyncAsync bSyncConn, ULONG ulTimeOut);

    /**************************************************************************
    * name       : removeTcpClient
    * description: ע�����Ӻ���
    * input      : pTcpConnHandle  ���Ӷ�Ӧ��handle
    * output     : NA
    * return     : NA
    * remark     : NA
    **************************************************************************/
    virtual void removeTcpClient(CTcpConnHandle *pTcpConnHandle);

    /**************************************************************************
    * name       : regTcpServer
    * description: ����TCP������
    * input      : pLocalAddr  ���ص�ַ
                   pTcpServerHandle  TCP��������Ӧ��handle
    * output     : NA
    * return     : VOS_SUCCESS    listen success
                   VOS_FAIL       listen fail
    * remark     : NA
    **************************************************************************/
    virtual long regTcpServer(const CNetworkAddr *pLocalAddr,
                              CTcpServerHandle *  pTcpServerHandle);

    /**************************************************************************
    * name       : regTcpClient
    * description: ע��TCP������
    * input      : pTcpServerHandle  TCP��������Ӧ��handle
    * output     : NA
    * return     : NA
    * remark     : NA
    **************************************************************************/
    virtual void removeTcpServer(CTcpServerHandle *pTcpServerHandle);

protected:
    long m_lLocalIpAddr;			// ����IP��ַ
    CTcpConnMgr *m_pTcpConnMgr;		// TCP�ͻ������ӹ���
    CTcpServerMgr *m_pTcpServerMgr; // TCP����˹���
};

#endif //CONN_MGR
