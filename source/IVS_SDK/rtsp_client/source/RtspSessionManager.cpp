/******************************************************************************
   ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� �� : RtspSessionManager.cpp
  ��    �� : y00182336
  �������� : RTSP�ͻ���ͨѶ��
  �޸���ʷ :
    1 ���� : 2011-10-12
      ���� : y00182336
      �޸� : ����

 ******************************************************************************/
#include "Rtsp_Log.h"
#include "RtspRetCode.h"
#include "RtspSessionManager.h"
#include "RtspReactorManager.h"
#include "rtsp_client_datatype.h"
using namespace std;

/***************************************************************************
 �� �� �� : CRtspSessionManager
 �������� : ���캯��
 ������� : ��
 ������� : ��
 �� �� ֵ : ��
***************************************************************************/
CRtspSessionManager::CRtspSessionManager()
{
    m_fExceptionCallBack = NULL;
    m_pUser              = NULL;
    m_unSessionIndex     = 0;
    m_lTimerId           = -1;
}


/***************************************************************************
 �� �� �� : ~CRtspSessionManager
 �������� : ��������
 ������� : ��
 ������� : ��
 �� �� ֵ : ��
***************************************************************************/
CRtspSessionManager::~CRtspSessionManager()
{
    m_fExceptionCallBack = NULL;
    m_pUser              = NULL;
}


// ��ʱ���ص����������ڶ��ڷ��ͼ������״̬
int CRtspSessionManager::handle_timeout(const ACE_Time_Value &tv, const void *arg)
{
    ACE_Guard<ACE_Thread_Mutex> locker(m_RtspSessionListMutex);
    if (m_RtspSessionList.empty())
    {
		//IVS_RUN_LOG_CRI("handle_timeout failed. list is empty.");
        return 0;
    }

    // �������״̬��Ϣ
    RtspSessionIter iter = m_RtspSessionList.begin();
    CRtspSession *pSession = NULL;
    while (iter != m_RtspSessionList.end())
    {
        pSession = *iter;
        RtspSessionIter pIter = iter++;
        if (NULL == pSession)
        {
            // ��List���Ƴ�session���ͷ�
            m_RtspSessionList.erase(pIter);
            continue;
        }

        if (RTSP_SESSION_STATUS_ABNORMAL== pSession->getStatus())
        {
			// ����쳣����
            (void)pSession->disconnectPeer();
            pSession->setStatus(RTSP_SESSION_STATUS_DELETE);
            continue;
        }

        if (RTSP_SESSION_STATUS_DELETE == pSession->getStatus())
        {
			IVS_RUN_LOG_INF("start release rtsp session [%p]", pSession);
            // ��List���Ƴ�session���ͷ�
            m_RtspSessionList.erase(pIter);
            pSession->setDelFlag();
        }
    }
    return 0;
}

// ��ʼ�����ӹ�����
int CRtspSessionManager::init()
{
    ACE_Time_Value tv(1, 0); //lint !e747
    if (-1 != m_lTimerId)
    {
        CRtspReactorManager::instance()->unRegistTimer(m_lTimerId, 0);
        m_lTimerId = -1;
    }
    m_lTimerId = CRtspReactorManager::instance()->RegistTimer((ACE_Event_Handler *)this, 0, tv, tv);
    if (-1 == m_lTimerId)
    {
        IVS_RUN_LOG_CRI("start check timer fail.");
        return RET_FAIL;
    }
    return RET_OK;
}

/***************************************************************************
 �� �� �� : closeManager
 �������� : �ر����ӹ�����
 ������� : ��
 ������� : ��
 �� �� ֵ : ��
***************************************************************************/
void CRtspSessionManager::closeManager()
{
    ACE_Guard<ACE_Thread_Mutex> locker(m_RtspSessionListMutex);
    // �ر��������ӣ��������ӳ�����
    if (m_RtspSessionList.empty())
    {
        IVS_RUN_LOG_INF("Session list already empty.");
        return;
    }
    RtspSessionIter iter = m_RtspSessionList.begin();
    CRtspSession *pSession = NULL;
    // modified by y00182336 �ر�ͨѶ��ʱ���ж�
    while(iter != m_RtspSessionList.end())
    {
        pSession = *iter;
        RtspSessionIter pIter = iter++;
        if (NULL == pSession)
        {
            // ��List���Ƴ�session���ͷ�
            m_RtspSessionList.erase(pIter);
            continue;
        }
        else
        {
            // ����쳣����
            (void)pSession->disconnectPeer();
            IVS_RUN_LOG_INF("start release rtsp session [%p]", pSession);
            // ��List���Ƴ�session���ͷ�
            m_RtspSessionList.erase(pIter);
            //ACE_Guard <ACE_Thread_Mutex> lockerSession (pSession->m_Mutex);
            pSession->setDelFlag();
        }
    }
    // end modified by y00182336

    if (-1 != m_lTimerId)
    {
        CRtspReactorManager::instance()->unRegistTimer(m_lTimerId, 0);
        m_lTimerId = -1;
    }

    return;
}



/***************************************************************************
 �� �� �� : createRtspSession
 �������� : ����RTSP����
 ������� : bBlocked                ͬ��/�첽��־λ��true��ͬ����false���첽��
 ������� : ��
 �� �� ֵ : CRtspSession*           NULL-ʧ��
***************************************************************************/
CRtspSession* CRtspSessionManager::createRtspSession(const bool bBlocked)
{
    CRtspSession* RtspSession = NULL;
    try
    {
        RtspSession = new CRtspSession;
    }
    catch(...)
    {
		IVS_RUN_LOG_CRI("new session throw exception.");
        return NULL;
    }

	int nRet = RtspSession->InitData();
	if (RET_OK != nRet)
	{
		delete RtspSession;
		RtspSession = NULL;
		IVS_RUN_LOG_CRI("init rtsp session data fail.");
		return NULL;
	}

	IVS_RUN_LOG_CRI("createRtspSession this = %08x m_pRecvBuffer = %08x", RtspSession, RtspSession->m_pRecvBuffer);

    // ����ͬ����־λ
    RtspSession->m_bBlocked = bBlocked;
    RtspSession->m_unSessionIndex = getSessionIndex();

    // ���������
    nRet = pushRtspSession(RtspSession);
    if (RET_OK != nRet)
    {
        delete RtspSession;
        RtspSession = NULL;
        IVS_RUN_LOG_CRI("push rtsp session fail.");
    }
    return RtspSession;
}


/***************************************************************************
 �� �� �� : findSession
 �������� : ��������
 ������� : pSession                �����ҵ�����
 ������� : ��
 �� �� ֵ : true-�ɹ�               false-ʧ��
***************************************************************************/
bool CRtspSessionManager::findSession(const CRtspSession *pSession)
{
    if (NULL == pSession)
    {
        return false;
    }
    
    // �����д��������Ӷ����в���
    ACE_Guard<ACE_Thread_Mutex> locker(m_RtspSessionListMutex);
    return isSessionInList(pSession, m_RtspSessionList);
}

bool CRtspSessionManager::deleteSession(const CRtspSession *pSession)
{
	if (NULL == pSession)
	{
		return false;
	}

	bool bRet = false;
	// �����д��������Ӷ����в���
	ACE_Guard<ACE_Thread_Mutex> locker(m_RtspSessionListMutex);
	RtspSessionIter iter;
	for (iter = m_RtspSessionList.begin(); iter != m_RtspSessionList.end(); ++iter)
	{
		if (pSession == *iter)
		{
			m_RtspSessionList.erase(iter);
			bRet = true;
			break;
		}
	}
	return bRet;
}

//*************************�ڲ�ʵ��**************************//

/***************************************************************************
 �� �� �� : pushRtspSession
 �������� : RtspSession���������
 ������� : pSession                ������������
 ������� : ��
 �� �� ֵ : RET_OK-�ɹ�             NULL-ʧ��
***************************************************************************/
int CRtspSessionManager::pushRtspSession(CRtspSession* pSession)
{
    try
    {
        ACE_Guard<ACE_Thread_Mutex>  locker(m_RtspSessionListMutex);
        m_RtspSessionList.push_back(pSession);
    }
    catch (...)
    {
        return RET_FAIL;
    }
    return RET_OK;
}



/***************************************************************************
 �� �� �� : isSessionInList
 �������� : ��������Ƿ����
 ������� : pSession                ������������
            SessionList             �������Ķ���
 ������� : ��
 �� �� ֵ : true-����               false-������
***************************************************************************/
bool CRtspSessionManager::isSessionInList(const CRtspSession *pSession, 
                                          RtspSessionList &SessionList) const
{
    RtspSessionIter iter;
    for (iter = SessionList.begin(); iter != SessionList.end(); ++iter)
    {
        if (pSession == *iter)
        {
            return true;
        }
    }
    return false;
}


unsigned int CRtspSessionManager::getSessionIndex()
{
    ACE_Guard<ACE_Thread_Mutex> locker (m_unSessionIndexMutex);
    return m_unSessionIndex++;
}

