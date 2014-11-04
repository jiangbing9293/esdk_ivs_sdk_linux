//lint -e1795
/******************************************************************************
   ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� �� : RtspSession.h
  ��    �� : y00182336
  �������� : RTSP����
  �޸���ʷ :
    1 ���� : 2011-10-12
      ���� : y00182336
      �޸� : ����

 ******************************************************************************/

#ifndef __RTSPSESSION_H_
#define __RTSPSESSION_H_

#include "ace_header.h"
#include <string>
#include <sstream>
#include "RtspMessage.h"
#include "Static_PreAssign_Buf.h"
#include "rtsp_client_datatype.h"
#include "RtspDef.h"

#ifndef ETIME
#define ETIME               137
#endif

// ������ջ���Ĵ�С
#define RECV_BUFFER_SIZE        2048

// ͬ����ʽ�£����ν�����Ϣ�ĳ�ʱʱ��
#define RECV_SINGLE_TIMEOUT     1

// ����ANNOUNCE����
#define RTSP_ANNOUNCE_MSG       "ANNOUNCE "

// ����RTP��֯��־
#define RTP_INTERLEAVE_FLAG    '$'

// ���彻֯��Ϣ�ĳ���
#define RTP_INTERLEAVE_LENGTH   4

// RTP ���ݽ���
#define  RTP_DATA_INTERVAL_MS  5 // RTP�����ճ�ʱ��� ���5s
#define  RTP_TIME_OUT_MAX      6 // �ۻ���ʱ6�� 30s����Ϊ����   

class CRtspSessionManager;


class CRtspSession : public ACE_Event_Handler
{
    friend class CRtspSessionManager;

public:
    CRtspSession();
    virtual ~CRtspSession();

    // ����TCP����
    int connectPeer(const ACE_INET_Addr &remoteAddr);

    // �Ͽ�TCP����
    int disconnectPeer();

    // ����RTSP ��Ϣ��
    template <typename T>
    T* createRtspMessage(T* )
    {
        if (NULL != m_pRtspMsg)
        {
            delete m_pRtspMsg;
            m_pRtspMsg = NULL;
        }
        try
        {
            m_pRtspMsg = new T;
        }
        catch (...)
        {
            IVS_RUN_LOG_CRI("create rtsp message fail.");
            return NULL;
        }
        m_pRtspMsg->setCSeq(getCSeq());
        return dynamic_cast<T*>(m_pRtspMsg);
    }

    // ����RTSP SETUP��Ϣ
    int setRtspSetupMsg();


    // ɾ����Ϣ
    void destroyRtspMsg();

    // ������Ϣ��Ϣ
    int sendMessage(const char *pDataBuffer, unsigned int unDataSize);
    
    // ������Ӧ��Ϣ��ͬ����ʽ��
    int recvMessage(unsigned int timeout);

    // ȡ����ʱ��ʱ��
    void cancelRecvTimer();

    // �첽��ʽ�£���Ӧ��Ϣ��ʱ�ص�����
    int handle_timeout(const ACE_Time_Value &tv, const void *arg);

    // ��Ӧ��Ϣ���ջص�����
    int handle_input(ACE_HANDLE handle);

    // �ر����ӻص�����
    int handle_close(ACE_HANDLE handle, ACE_Reactor_Mask close_mask);

    // ���ó�ʱ��ʱ��
    int setRecvTimer(unsigned int timeout);

    // ��ȡ�����ע�ᷴӦ��ʱ��Ҫʹ��
    ACE_HANDLE get_handle() const;

    // �������ӵ�״̬
    void setStatus(unsigned int status);

    // ��ȡ����״̬
    RTSP_SESSION_STATUS getStatus() const;
    
    unsigned int getCSeq();

    // ������յ���Ϣ
    int handleRecvedData(const char* pData, unsigned int unDataSize);

    // ����ý����Ϣ
    int parseRtpRtcpPacket(const char* pData, unsigned int unDataSize);

    // ����RTSP��Ϣ
    unsigned int parseRtspPacket(const char* pData, unsigned int unDataSize);

    // ���ջص������д���RTSP��Ӧ��Ϣ
    int handleRtspRespMsg(const int nRet,const char* psRtspMsg, const unsigned int nLen);

    // ���������ʹ���ANNOUNCE����
    int handleRtspAnnounceMsg(const char* psRtspMsg, const unsigned int nLen);

    // ����RTSP Setup��Ӧ
    void handleRtspSetupMsg(const int nRet);
    
    // ����RTSP Play��Ӧ
    void handleRtspPlayMsg(const int nRet);

    // ����RTSP Options��Ӧ��Ϣ
    void handleRtspOptionsMsg(int nRet, const char* psRtspMsg, const unsigned int nLen);

    // ����RTSP Pause��Ӧ��Ϣ
    void handleRtspPauseMsg(const int nRet);

    // ����RTSP Teardown��Ӧ��Ϣ
    void handleRtspTeardownMsg(const int nRet);

    // ��װRTSP��Ӧ��Ϣ
    int encodeRtspRespMsg(const unsigned int unStatusCode, std::string &strMessage);

    void setDelFlag();

    bool getDelFalg();

	int  SetupRtpTimer();
	void KillRtpTimer();
	bool CheckRtpTimeout();

	void EnableRtpTimeoutCheck(bool enable) { m_bEnableTmoCheck = enable; }

	int InitData();

	bool  IsConnectFail() const  { return m_bConnectFail; }

private:

    // �ж������Ƿ�����
    bool checkIsDisconnect(int nErrNo) const;

    // �����Ƿ�ΪRTSP������Ϣ
    bool isRtspMessage(const char* psRtspMsg,const unsigned int nLen);

public:
    bool                    m_bBlocked;                 // ͬ���첽��־λ��true����ͬ����false�����첽��

    unsigned int            m_unSessionIndex;           // ���Ӻ�

    ACE_Thread_Mutex        m_SessionMutex;             // �����߳���
    ACE_Thread_Mutex        m_StreamMutex;              // ������̵߳���
    ACE_SOCK_Stream         m_tcpStream;                // tcp���ӵ�Stream
    ACE_INET_Addr           m_remoteAddr;               // Զ�˵�ַ
    ACE_INET_Addr           m_localAddr;

    fDataCallBack           m_fDataCallBack;            // ý�����ݻص�����
    void*                   m_pUser;                    // �û�����

    CRtspMessage*           m_pRtspMsg;                 // RTSP��Ϣ��

    //std::string             m_strDate;                  // playing״̬�£���Ӧ��Ϣ��ʱ�����
    unsigned int            m_unPlayTimePos;            // options��Ӧ��Ϣ�еľ���ʱ��

    ACE_Time_Value          m_NowTime;                  // ���Ӽ�¼�ĵ�ǰʱ�䣬����options��鳬ʱ

    ACE_Thread_Semaphore*   m_pRtspSemaphore;           // �ź���
    bool                    m_bRecvRetFlag;             // ��Ϣ���ս����־λ
	int						m_iRetCode;
    std::string             m_strTimeInfo;

    ACE_Thread_Mutex            m_Mutex;                        // �߳���
private:
    RTSP_SESSION_STATUS     m_enSessionStatus;          // ����״̬
    ACE_Thread_Mutex        m_StatusMutex;              // ״̬��  
    
    unsigned int            m_unCSeq;                   // RTSP��Ϣ�����к�

    bool                    m_bDelete;

    long                    m_lTimerId;                 // RTSP��ʱ��ʱ��
    Static_PreAssign_Buffer*    m_pRecvBuffer;          // ���ջ�����

    static const char*      m_strRtspMethod[];          // RTSP ��Ϣ����

	bool                    m_bDataRecved;              // �������ݱ�ǣ����ڳ�ʱ���
	unsigned int            m_uTmoCounter;              // ��ʱ������
	long                    m_lDataTimer;               // ���ݽ��ռ�ⶨʱ��
	bool                    m_bEnableTmoCheck;          // ʹ��RTP��ʱ���

	bool                    m_bConnectFail;
};


#endif //__RTSPSESSION_H_
