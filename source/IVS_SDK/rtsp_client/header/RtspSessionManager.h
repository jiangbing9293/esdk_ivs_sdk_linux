/******************************************************************************
   ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� �� : RtspSessionManager.h
  ��    �� : y00182336
  �������� : RTSP�ͻ���ͨѶ��
  �޸���ʷ :
    1 ���� : 2011-10-12
      ���� : y00182336
      �޸� : ����

 ******************************************************************************/
#ifndef __RTSPSESSIONMANAGER_H_
#define __RTSPSESSIONMANAGER_H_

#include <list>
#include "ace_header.h"
#include "RtspSession.h"
#include "rtsp_client_datatype.h"
using namespace std;

// ����RTSP���Ӷ���
typedef std::list<CRtspSession*>        RtspSessionList;
typedef std::list<CRtspSession*>::iterator       RtspSessionIter;

class CRtspSessionManager : public ACE_Event_Handler
{
public:
    virtual ~CRtspSessionManager();
    
    static CRtspSessionManager* instance()
    {
        static CRtspSessionManager SessionManager;
        return &SessionManager;
    };
    
    //��ʼ����ע�ᷴӦ��
    int init();

    // �ر����ӹ�����
    void closeManager();

    // ��������
    CRtspSession* createRtspSession(const bool bBlocked);

    // �ͷ�����
    //int releaseRtspSession(CRtspSession* pSession);

    // ��ʱ���ص����������ڶ��ڷ���Options��Ϣ
    int handle_timeout(const ACE_Time_Value &tv, const void *arg);
    
    // ��������Ƿ����
    bool findSession(const CRtspSession *pSession);

	bool deleteSession(const CRtspSession *pSession);
private:
    CRtspSessionManager();
    
    unsigned int getSessionIndex();

    // ���������Ƿ��������
    bool isSessionInList(const CRtspSession *pSession, 
                            RtspSessionList &SessionList) const;

    // ��������Ƿ�ʱ
    void checkSession();

    // ���������
    int pushRtspSession(CRtspSession* pSession);


public:

    fExceptionCallBack          m_fExceptionCallBack;           // �쳣�ص�����
    void*                       m_pUser;                        // �û�����

private:
    ACE_Thread_Mutex            m_unSessionIndexMutex;          // ���Ӻ��߳���
    unsigned int                m_unSessionIndex;               // ���Ӻ�

    ACE_Thread_Mutex            m_RtspSessionListMutex;         // �߳���
    RtspSessionList             m_RtspSessionList;              // ����RTSP����

    long                        m_lTimerId;                     // ��ʱ��ID

};

#endif //__RTSPSESSIONMANAGER_H_
