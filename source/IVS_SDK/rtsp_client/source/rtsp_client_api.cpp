/******************************************************************************
   ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� �� : RTSP_CLIENT_API.cpp
  ��    �� : y00182336
  �������� : RTSP�ͻ���ͨѶ��
  �޸���ʷ :
    1 ���� : 2011-10-12
      ���� : y00182336
      �޸� : ����

 ******************************************************************************/
#include "rtsp_client_api.h"
#include "RtspRetCode.h"
#include "Rtsp_Log_Manager.h"
#include "RtspReactorManager.h"
#include "RtspSessionManager.h"
#include "RtspSetupMessage.h"
#include "RtspPlayMessage.h"
#include "RtspPauseMessage.h"
#include "RtspTeardownMessage.h"
#include "RtspOptionsMessage.h"
//#include "windows.h"

// ����ͷ��������־λ
static bool g_bIsFirst = true;         //lint !e956

// Ϊjava�ṩ__stdcall��ʽ�µı���
#ifdef WIN32
#pragma comment(linker, "/EXPORT:RTSP_CLIENT_SetLog=_RTSP_CLIENT_SetLog@12")
#pragma comment(linker, "/EXPORT:RTSP_CLIENT_Init=_RTSP_CLIENT_Init@8")
#pragma comment(linker, "/EXPORT:RTSP_CLIENT_GetHandle=_RTSP_CLIENT_GetHandle@8")
#pragma comment(linker, "/EXPORT:RTSP_CLIENT_SetExceptionCallBack=_RTSP_CLIENT_SetExceptionCallBack@8")
#pragma comment(linker, "/EXPORT:RTSP_CLIENT_SetDataCallBack=_RTSP_CLIENT_SetDataCallBack@12")
#pragma comment(linker, "/EXPORT:RTSP_CLIENT_SendSetupMsg=_RTSP_CLIENT_SendSetupMsg@28")
#pragma comment(linker, "/EXPORT:RTSP_CLIENT_SendPlayMsg=_RTSP_CLIENT_SendPlayMsg@24")
#pragma comment(linker, "/EXPORT:RTSP_CLIENT_SendPauseMsg=_RTSP_CLIENT_SendPauseMsg@12")
#pragma comment(linker, "/EXPORT:RTSP_CLIENT_SendTeardownMsg=_RTSP_CLIENT_SendTeardownMsg@12")
#pragma comment(linker, "/EXPORT:RTSP_CLIENT_GetStatus=_RTSP_CLIENT_GetStatus@4")
#pragma comment(linker, "/EXPORT:RTSP_CLIENT_FreeHandle=_RTSP_CLIENT_FreeHandle@4")
#pragma comment(linker, "/EXPORT:RTSP_CLIENT_Cleanup=_RTSP_CLIENT_Cleanup@0")
#pragma comment(linker, "/EXPORT:RTSP_CLIENT_SendOptionMsg=_RTSP_CLIENT_SendOptionMsg@20")
#endif

#ifdef OS_REDHAT_i386
#define MAX_TRY_COUNT  5000
#define USLEEP_INTERVAL 2000
#endif

/***************************************************************************
 �� �� �� : RTSP_CLIENT_SetLog
 �������� : ͨѶ����־�̳߳�ʼ�����������ó�ʼ������־����ӡ
 ������� : enLevel        ��־�ȼ�
            bDebugflag     ��ӡDEBUG��Ϣ��־
			pszPath       ��־�ļ�·��
 ������� : ��
 �� �� ֵ : RET_OK-�ɹ�     RET_FAIL-ʧ��
***************************************************************************/
int RTSPAPI RTSP_CLIENT_SetLog(const unsigned int unLevel, const int iDebugflag, 
                               const char* pszPath)
{ //lint !e1784
    if (unLevel > RTSP_LOG_LEVEL_DEBUG)
    {
        IVS_RUN_LOG_CRI("set rtsp client log fail, Level[%u], debug switch[%d].", 
                        unLevel, iDebugflag); //lint !e730
        return RTSP_ERR_PARAMETER;
    }

    if (NULL != pszPath)
    {
        if (NSS_LOG_FILE_PATH_LEN < strlen(pszPath))
        {
            return RTSP_ERR_PARAMETER;
        }
        strncpy(CRtsp_Log_Manager::instance().m_strLogRootPath, pszPath, strlen(pszPath));
    }

    // ������־
    CRtsp_Log_Manager::instance().m_RefreshLevel = (RTSP_LOG_LEVEL_E) unLevel;
    CRtsp_Log_Manager::instance().m_bDebugFlag = iDebugflag ? true : false;
    IVS_DBG_LOG("init log manager success.");
    return RET_OK;
}

#ifndef WIN32
/***************************************************************************
 �� �� �� : GetModuleFilePath
 �������� : ��ȡָ�����ļ�����Ŀ¼���ϼ�Ŀ¼
 ������� : sModuleName  ���ļ�����
               unSize  sPath����
 ������� : sPath  ���ļ�Ŀ¼���ϼ�Ŀ¼
 �� �� ֵ : RET_OK-�ɹ�     RET_FAIL-ʧ��
***************************************************************************/
int GetModuleFilePath(const char* sModuleName, char* sPath, unsigned int unSize)
{
	int iRet = RET_FAIL;
	char sLine[1024] = {0};
	FILE *fp = NULL;
	char *pTmpFullDir = NULL;
	char *pTmpModuleDir = NULL;

	fp = fopen ("/proc/self/maps", "r");
	if (NULL == fp)
	{
		return iRet;
	}

	while (0 == feof(fp))
	{
		if (NULL == fgets(sLine, sizeof(sLine), fp))
		{
			continue;
		}

		pTmpFullDir = strchr(sLine, '/');
		if (NULL == strstr(sLine, "r-xp") ||  NULL == pTmpFullDir || NULL == strstr(sLine, sModuleName))//r-xp:˽��ֻ����ִ������
		{
			continue;
		}

		//������ҵ�ģ��ֱ�ӷ��ڸ�Ŀ¼�£�����ʧ��
		pTmpModuleDir = strrchr(pTmpFullDir, '/');            
		if (pTmpModuleDir == pTmpFullDir)
		{
			break;
		}

		*pTmpModuleDir = '\0';
		//���ҵ�ģ��������libĿ¼�£���lib/libXXX.so
		pTmpModuleDir = strrchr(pTmpFullDir, '/'); 
		if (pTmpModuleDir == pTmpFullDir)
		{
			iRet = RET_OK;
			strcpy(sPath, "/");
			break;
		}

		*pTmpModuleDir = '\0';            

		if (strlen(pTmpFullDir) >= unSize)
		{
			break;
		}

		iRet = RET_OK;
		strncpy(sPath, pTmpFullDir, strlen(pTmpFullDir) + 1);
		break;
	}

	return iRet;

}
#endif

/***************************************************************************
 �� �� �� : RTSP_CLIENT_Init
 �������� : ͨѶ���ʼ��
 ������� : ��
 ������� : ��
 �� �� ֵ : RET_OK-�ɹ�     RET_FAIL-ʧ��
***************************************************************************/
int RTSPAPI RTSP_CLIENT_Init(unsigned int unLevel, const char* pszPath)
{ //lint !e1784
    // �����ظ�����
    if (!g_bIsFirst)
    {
        return RET_OK;
    }
    g_bIsFirst = false;
//#ifdef WIN32
    ACE::init();
//#endif
    // �����־·��Ϊ�գ�ȡĬ��·��
#ifdef WIN32 
	std::string strLogPath = RTSP_CLIENT_LOG_PATH;
#else
	char pModulePath[1024] = {0};
	int iResult = GetModuleFilePath("libivs_rtsp_client.so", pModulePath, 1024);
	if (RET_OK != iResult)
	{
		IVS_RUN_LOG_CRI("GetModuleFileName Failed.");
		return iResult;
	}
	std::string strLogPath = pModulePath;
	strLogPath += "/log/rtsp_log";
#endif
    if (NULL != pszPath && 0 != strlen(pszPath))
    {
        strLogPath = pszPath;
        for(string::size_type pos(0); pos!=string::npos; pos += strlen("/")) 
        {
            if((pos = strLogPath.find("\\",pos)) != string::npos)    
                strLogPath.replace(pos,strlen("\\"),"/"); 
            else 
                break;    
        }    
    }
    // ����־
    int nRet = CRtsp_Log_Manager::instance().initLog(RTSP_CLIENT_SERVICE_NAME, 
                                                     strLogPath.c_str(), unLevel);
    if (RET_OK != nRet)
    {
        return RET_FAIL;
    }

    // �򿪷�Ӧ������
    nRet = CRtspReactorManager::instance()->openManager();
    if (RET_OK != nRet)
    {
        (void)CRtsp_Log_Manager::instance().endLog();
        return RET_FAIL;
    }

    // �������ӹ���
    nRet= CRtspSessionManager::instance()->init();
    if (RET_OK != nRet)
    {
        (void)CRtspReactorManager::instance()->closeManager();
        (void)CRtsp_Log_Manager::instance().endLog();
        return RET_FAIL;
    }

    IVS_DBG_LOG("init rtsp client success.");
    return RET_OK;
}



/***************************************************************************
 �� �� �� : RTSP_CLIENT_GetHandle
 �������� : ��ȡ���Ӿ��
 ������� : bBlocked        ͬ��/�첽��ʽ
 ������� : nHandle         �������Ӿ��
 �� �� ֵ : RET_OK-�ɹ�     RET_FAIL-ʧ��
***************************************************************************/
int RTSPAPI RTSP_CLIENT_GetHandle(const bool bBlocked, 
                                  long *nHandle)
{ //lint !e1784
    if (NULL == nHandle)
    {
        IVS_RUN_LOG_CRI("handle is NULL.");
        return RET_FAIL;
    }
    CRtspSession* pSession = CRtspSessionManager::instance()->createRtspSession(bBlocked);
    if (NULL == pSession)
    {
        IVS_RUN_LOG_CRI("get session handle fail.");
        return RET_FAIL;
    }
    *nHandle = (long)pSession;
    
    IVS_DBG_LOG("get session handle success, mode[%d], handle[%p].", bBlocked, pSession); //lint !e730
    return RET_OK;
}


/***************************************************************************
 �� �� �� : RTSP_CLIENT_SetExceptionCallBack
 �������� : ���ûص�����
 ������� : cbExceptionCallBack          �ص�����
            pUser                        �û��������ص������з���
 ������� : ��
 �� �� ֵ : RET_OK-�ɹ�     RET_FAIL-ʧ��
***************************************************************************/
int RTSPAPI RTSP_CLIENT_SetExceptionCallBack(fExceptionCallBack cbExceptionCallBack, void* pUser)
{ //lint !e1784
    // �������
    if (NULL == cbExceptionCallBack)
    {
        IVS_RUN_LOG_CRI("set Exception Call Back function fail, parameter abnormal.");
        return RTSP_ERR_PARAMETER;
    }

    CRtspSessionManager::instance()->m_fExceptionCallBack = cbExceptionCallBack;
    CRtspSessionManager::instance()->m_pUser = pUser;
    IVS_DBG_LOG("set Exception Call Back function success.");
    return RET_OK;
}


/***************************************************************************
 �� �� �� : RTSP_CLIENT_SetDataCallBack
 �������� : ����ý�����ص�����
 ������� :  nHandle                    ���Ӿ��
            cbDataCallBack              ý�����ص�����
            pUser                       �û��������ص������з���
 ������� : ��
 �� �� ֵ : RET_OK-�ɹ�     RET_FAIL-ʧ��
***************************************************************************/
int RTSPAPI RTSP_CLIENT_SetDataCallBack(long nHandle, fDataCallBack cbDataCallBack, void* pUser)
{ //lint !e1784
    // �����Ч�Լ��
    if(0 == nHandle)
    {
        IVS_RUN_LOG_CRI("parameter invalid. 0 == nHandle");
        return RTSP_ERR_PARAMETER;
    }

	if(NULL == cbDataCallBack)
	{
		IVS_RUN_LOG_CRI("set data Call Back for NULL.");
	}

    // ��������Ƿ����
    CRtspSession* pSession = (CRtspSession*) (nHandle);
    if (!CRtspSessionManager::instance()->findSession(pSession))
    {
        IVS_RUN_LOG_CRI("set data Call Back function fail, session not exist, handle[%p].", pSession);
        return RTSP_ERR_NOT_EXIST;
    }

    // ����
    ACE_Guard <ACE_Thread_Mutex> locker (pSession->m_SessionMutex);
    pSession->m_fDataCallBack = cbDataCallBack;
    pSession->m_pUser = pUser;
    IVS_DBG_LOG("set data Call Back function success, handle[%p].", pSession);
    return RET_OK;
}


static int sendSetupMessage(const bool bTcpFlag, CRtspSession* pSession, 
                           const char *sURL,
                           const INET_ADDR* pstServerAddr,
                           const MEDIA_ADDR* pstLocalMediaAddr)
{
    // ý����ͨ��TCP��ʽ���䣬�ûص��������ݳ�ȥ
    if ((bTcpFlag) && (NULL == pSession->m_fDataCallBack))
    {
        IVS_RUN_LOG_CRI("data call back funtion not exist.");
        return RTSP_ERR_PARAMETER;
    }

    // �������״̬
    if (RTSP_SESSION_STATUS_INIT != pSession->getStatus())
    {
        IVS_RUN_LOG_CRI("send rtsp setup message fail, invalid status[%d], handle[%p].", 
                        pSession->getStatus(), pSession);
        return RTSP_ERR_STAUTS_ABNORMAL;
    }

    // ����SETUP��Ϣ�࣬���յ���Ӧ���߳�ʱ�ͷ�
    CRtspSetupMessage* pRtspMsg = NULL;
    pRtspMsg = pSession->createRtspMessage((CRtspSetupMessage*)NULL); //lint !e838
    if (NULL == pRtspMsg)
    {
        IVS_RUN_LOG_CRI("create rtsp setup requst message fail.");
        return RET_FAIL;
    }

    // �������ӷ�ʽ
    pRtspMsg->m_bTcpFlag = bTcpFlag;
    // ����URL
    std::string strRtspUrl = "";
    strRtspUrl.append(sURL);
    pRtspMsg->setRtspUrl(strRtspUrl);
    // ���ô�������
    if (bTcpFlag)
    {
        // Setup��Ϣ����¼TCP���ʹ���ý������Ӳ����Ҫ
        pRtspMsg->setTransType(TRANS_TYPE_TCP);
        if (NULL != pstLocalMediaAddr)
        {
            pRtspMsg->setDestinationIp(pstLocalMediaAddr->unMediaIp);       //lint !e613
            pRtspMsg->setClientPort(pstLocalMediaAddr->unMediaVideoPort, 
                                    pstLocalMediaAddr->unMediaAudioPort);   //lint !e613
        }
    }
    else
    {
        pRtspMsg->setTransType(TRANS_TYPE_UDP);
        // ����ý�������ؽ��յ�ַ�Ͷ˿�
        pRtspMsg->setDestinationIp(pstLocalMediaAddr->unMediaIp);       //lint !e613
        pRtspMsg->setClientPort(pstLocalMediaAddr->unMediaVideoPort, 
                                pstLocalMediaAddr->unMediaAudioPort);   //lint !e613

    }


    // ��װRTSP SETUP��Ϣ
    std::string strRtspMsg = "";
    int nRet = pRtspMsg->encodeMessage(strRtspMsg);
    if (RET_OK != nRet)
    {
        pSession->destroyRtspMsg();
        return RET_FAIL;
    }

    // ��������
    ACE_INET_Addr remoteAddr(pstServerAddr->unAddrPort, pstServerAddr->unAddrIp);

    nRet = pSession->connectPeer(remoteAddr);
    if (RET_OK != nRet)
    {
        IVS_RUN_LOG_CRI("connect peer fail, remote address[%s:%d], handle[%p].", 
                        remoteAddr.get_host_addr(),
                        remoteAddr.get_port_number(),
                        pSession);
        (void)pSession->disconnectPeer();
        return RTSP_ERR_DISCONNECT;
    }
    IVS_RUN_LOG_INF("connect peer success, remote address [%s:%d], handle[%p].", 
                    remoteAddr.get_host_addr(),
                    remoteAddr.get_port_number(),
                    pSession);

    // ����RTSP SETUP��Ϣ
    nRet = pSession->sendMessage(strRtspMsg.c_str(), strRtspMsg.length());
    if (RET_OK != nRet)
    {
		IVS_RUN_LOG_INF("sendSetupMessage failed, remote address [%s:%d], handle[%p] nRet[%d].", 
						remoteAddr.get_host_addr(),
						remoteAddr.get_port_number(),
						pSession, nRet);
        return nRet;
    }

	IVS_RUN_LOG_INF("sendSetupMessage succeed, remote address [%s:%d], handle[%p].", 
					remoteAddr.get_host_addr(),
					remoteAddr.get_port_number(),
					pSession);
    return RET_OK;
}


static int handleSynchSetup(CRtspSession* pSession, const unsigned int unTimeout,
                            const bool bTcpFlag, MEDIA_ADDR *pstPeerMediaAddr)
{
//BEGIN: Add by qiuweilin 64597
//��redhat 32λƽ̨�£�ʹ��acquire��ȡ�ź���һֱ��ʱ
//����취��ʹ��tryacquire���������Ի�ȡ5000�Σ�5000��֮����δ��ȡ���ź������򰴳�ʱ����
#ifdef OS_REDHAT_i386
	int iTryCnt = 0;
	int nRet = 0;
	while(iTryCnt <= MAX_TRY_COUNT)
	{
		nRet = pSession->m_pRtspSemaphore->tryacquire();
		if (0 == nRet)
		{
			break;
		}

		usleep(USLEEP_INTERVAL);
		iTryCnt++;
	}
#else
//END: Add by qiuweilin 64597
    // �ȴ��źŴ��������������ڴ�
    ACE_Time_Value wait = ACE_OS::gettimeofday();
    wait.sec (wait.sec () + unTimeout);

    int nRet = pSession->m_pRtspSemaphore->acquire(&wait);
#endif
    // ���ճ�ʱ��ֱ�ӷ���
    if(0 != nRet)
    {
        ACE_Guard <ACE_Thread_Mutex> locker (pSession->m_Mutex);
        IVS_RUN_LOG_ERR("receive rtsp setup response out of time, handle[%p].", pSession);
        pSession->destroyRtspMsg();
        return RTSP_ERR_TIMEOUT;
    }

	//IVS_RUN_LOG_ERR("----- handleSynchSetup 1, handle[%p].", pSession);

    // ��������ֱ�ӷ���
    if (!pSession->m_bRecvRetFlag)
    {
		ACE_Guard <ACE_Thread_Mutex> locker (pSession->m_Mutex);
		int iRet = pSession->m_iRetCode;
        pSession->destroyRtspMsg();
		IVS_RUN_LOG_ERR("receive rtsp setup response decode error, handle[%p], RetCode[%d].", pSession, iRet);
        return iRet;
    }

	//IVS_RUN_LOG_ERR("----- handleSynchSetup 2, handle[%p].", pSession);

    // UDP���䷽ʽ�£���ȡý�������͵�ַ
    //if (false == bTcpFlag)
    CRtspSetupMessage* pRtspMsg = dynamic_cast<CRtspSetupMessage*>(pSession->m_pRtspMsg);
    if (NULL == pRtspMsg)
    {
		IVS_RUN_LOG_ERR("receive rtsp setup response msg is null, handle[%p].", pSession);
        return RET_FAIL;
    }
    if (0 != pRtspMsg->getServerIp())
    {
        pstPeerMediaAddr->unMediaIp        = pRtspMsg->getServerIp();           //lint !e613
        pstPeerMediaAddr->unMediaVideoPort = pRtspMsg->getServerVideoPort();    //lint !e613
        pstPeerMediaAddr->unMediaAudioPort = pRtspMsg->getServerAudioPort();    //lint !e613
    }
 
	//IVS_RUN_LOG_ERR("----- handleSynchSetup 3, handle[%p].", pSession);

    // �ͷ���Ϣ
    pSession->destroyRtspMsg();
    // �����ɹ�������״̬Ϊready
    pSession->setStatus(RTSP_SESSION_STATUS_READY);
    IVS_RUN_LOG_INF("send setup message success, handle[%p].", pSession);
    return RET_OK;
}

/***************************************************************************
 �� �� �� : RTSP_CLIENT_SendSetupMsg
 �������� : ����RTSP SETUP����
 ������� :  nHandle                     ���Ӿ��
            pstServerAddr               RTSP��������ַ
            sURL                        SETUP��Ϣ��װ�е�URL
            unTimeout                   SETUP��Ϣ��ʱʱ�䣨��λ��S��
            bTcpFlag                    �����Ƿ�ΪTCP��ʽ
            pstLocalMediaAddr           ͬ��ģʽ�£�ý�������ؽ��յ�ַ���첽ģʽ��Ϊ��
 ������� : pstPeerMediaAddr            ͬ��ģʽ�£�ý�������Ͷ˵�ַ���첽ģʽ��Ϊ��
 �� �� ֵ : RET_OK-�ɹ�     ����-ʧ��
***************************************************************************/
int RTSPAPI RTSP_CLIENT_SendSetupMsg(long nHandle , const INET_ADDR* pstServerAddr,
                                    const char *sURL, unsigned int unTimeout, bool bTcpFlag,
                                    const MEDIA_ADDR* pstLocalMediaAddr, MEDIA_ADDR *pstPeerMediaAddr )
{ //lint !e1784
    // �������
    if ((0 == nHandle) || (NULL == pstServerAddr)
        || (0 == unTimeout) || (NULL == sURL) 
        || (NULL == CRtspSessionManager::instance()->m_fExceptionCallBack))
    {
        IVS_RUN_LOG_ERR("send rtsp setup message fail, parameter invalid.");
        return RTSP_ERR_PARAMETER;
    }

    // UDP��ʽ�����ý���ַ
    if ((!bTcpFlag) && ((NULL == pstLocalMediaAddr) || (NULL == pstPeerMediaAddr)))
    {
        IVS_RUN_LOG_ERR("udp channel must have media addr.");
        return RTSP_ERR_PARAMETER;
    }

    // ��������Ƿ����
    CRtspSession* pSession = (CRtspSession*) (nHandle);    
    if (!CRtspSessionManager::instance()->findSession(pSession))
    {
        IVS_RUN_LOG_ERR("send rtsp setup message fail, session not exist, handle[%p].", pSession);
        return RTSP_ERR_NOT_EXIST;
    }

	// add by w00210470
	pSession->EnableRtpTimeoutCheck(NULL == pstLocalMediaAddr);

    // ����
    ACE_Guard <ACE_Thread_Mutex> locker (pSession->m_SessionMutex);
    int nRet = sendSetupMessage(bTcpFlag, pSession, sURL, pstServerAddr, pstLocalMediaAddr);
    if (RET_OK != nRet)
    {
		IVS_RUN_LOG_ERR("sendSetupMessage fail 2, handle[%p].", pSession);
        return nRet;
    }

    // ����ͬ����ʽ���ȴ��ź���
    if (pSession->m_bBlocked)
    {
        nRet = handleSynchSetup(pSession, unTimeout, bTcpFlag, pstPeerMediaAddr);
        if (RET_OK != nRet)
        {
            IVS_RUN_LOG_INF("send setup message fail, synch mode handle[%p], RetCode[%d].", pSession, nRet);
        }
        return nRet;
    }
    else
    {
        // �첽��ʽ�£�ע�ᳬʱ��ʱ�����ڽ��յ���Ӧ��Ϣ��ʱ��ȡ����ʱ��
        nRet = pSession->setRecvTimer(unTimeout);
        if (RET_OK != nRet)
        {
            pSession->destroyRtspMsg();
            return RET_FAIL;
        }
    }

    IVS_RUN_LOG_INF("send setup message success, handle[%p].", pSession);
    return RET_OK;
}


/***************************************************************************
 �� �� �� : RTSP_CLIENT_SendPlayMsg
 �������� : ����RTSP PLAY����
 ������� :  nHandle                    ���Ӿ��
            sURL                        PLAY��Ϣ��װ�е�URL
            dScale                      �ط�/���ز�������
            stRange                     �ط�/����ʱ���
            unTimeout                   PLAY��Ϣ��ʱʱ�䣨��λ��S��
 ������� : ��
 �� �� ֵ : RET_OK-�ɹ�     RET_FAIL-ʧ��
***************************************************************************/
int RTSPAPI RTSP_CLIENT_SendPlayMsg(long nHandle, const char *sURL, double dScale, 
                                    const MEDIA_RANGE_S* stRange, unsigned int unTimeout)
{ //lint !e1784
    // �����Ч�Լ��
    if ((0 == nHandle) || (0 == unTimeout) || (NULL == sURL))
    {
        IVS_RUN_LOG_CRI("send rtsp play message fail, parameter is invalid.");
        return RTSP_ERR_PARAMETER;
    }

    // ��������Ƿ����
    CRtspSession* pSession = (CRtspSession*) (nHandle);    
    if (!CRtspSessionManager::instance()->findSession(pSession))
    {
        IVS_RUN_LOG_CRI("send rtsp play message fail, session not exist, handle[%p].", pSession);
        return RTSP_ERR_NOT_EXIST;
    }

    // ����
    ACE_Guard <ACE_Thread_Mutex> locker (pSession->m_SessionMutex);
    // �������״̬
    if (RTSP_SESSION_STATUS_INIT == pSession->getStatus())
    {
        IVS_RUN_LOG_CRI("send rtsp play message fail, invalid status[%d], handle[%p].",
                        pSession->getStatus(),
                        pSession);
        return RTSP_ERR_STAUTS_ABNORMAL;
    }

    // ����PLAY��Ϣ�࣬ʹ�����ɾ��
    CRtspPlayMessage* pRtspMsg = NULL;
    pRtspMsg = pSession->createRtspMessage((CRtspPlayMessage*)NULL); //lint !e838
    if (NULL == pRtspMsg)
    {
        return RET_FAIL;
    }

    // ����RTSP URL
    std::string strRtspUrl = "";
    strRtspUrl.append(sURL);
    pRtspMsg->setRtspUrl(strRtspUrl);
    pRtspMsg->setRang(stRange);
    pRtspMsg->setScale(dScale);

    // RTSP PLAY��Ϣ��װ
    string strRtspMsg = "";
    int nRet = pRtspMsg->encodeMessage(strRtspMsg);
    if (RET_OK != nRet)
    {
        pSession->destroyRtspMsg();
        return RET_FAIL;
    }

    // ����RTSP PLAY��Ϣ
    nRet = pSession->sendMessage(strRtspMsg.c_str(), strRtspMsg.length());
    if (RET_OK != nRet)
    {
        return nRet;
    }
    // ����ͬ����ʽ
    if (pSession->m_bBlocked)
    {
//BEGIN: Add by qiuweilin 64597
//��redhat 32λƽ̨�£�ʹ��acquire��ȡ�ź���һֱ��ʱ
//����취��ʹ��tryacquire���������Ի�ȡ5000�Σ�5000��֮����δ��ȡ���ź������򰴳�ʱ����
#ifdef OS_REDHAT_i386
		int iTryCnt = 0;
		while(iTryCnt <= MAX_TRY_COUNT)
		{
			nRet = pSession->m_pRtspSemaphore->tryacquire();
			if (0 == nRet)
			{
				break;
			}

			usleep(USLEEP_INTERVAL);
			iTryCnt++;
		}
#else
//END: Add by qiuweilin 64597
        // �ȴ��źŴ��������������ڴ�
        ACE_Time_Value wait = ACE_OS::gettimeofday();
        wait.sec (wait.sec () + unTimeout);
        nRet = pSession->m_pRtspSemaphore->acquire(&wait);
#endif
        if (0 != nRet)
        {
            ACE_Guard <ACE_Thread_Mutex> locker (pSession->m_Mutex); //lint !e578
            pSession->destroyRtspMsg();
            return RTSP_ERR_TIMEOUT;
        }
        // ����ʧ�ܣ�ֱ�ӷ���
        if (!pSession->m_bRecvRetFlag)
        {
            return RET_FAIL;
        }
		// ���سɹ�������RTP���ݼ��
		pSession->SetupRtpTimer(); 
    }
    else
    {
        // �첽��ʽ�£�ע�ᳬʱ��ʱ��
        nRet = pSession->setRecvTimer(unTimeout);
        if (RET_OK != nRet)
        {
            pSession->destroyRtspMsg();
            return RET_FAIL;
        }
    }


    IVS_DBG_LOG("send play message success, handle[%p].", pSession);
    return RET_OK;
}

/***************************************************************************
 �� �� �� : RTSP_CLIENT_SendOptionMsg
 �������� : ����RTSP OPTIONS����
 ������� :  nHandle                    ���Ӿ��
            sURL                        OPTIONS��Ϣ��װ�е�URL
            unTimeout                   OPTIONS��Ϣ��ʱʱ�䣨��λ��S��
 ������� : ��
 �� �� ֵ : RET_OK-�ɹ�     RET_FAIL-ʧ��
***************************************************************************/
int RTSPAPI RTSP_CLIENT_SendOptionMsg(long nHandle,const char *sURL,unsigned int unTimeout, char *strMsg, int &iLength)
{
    if ((0 == nHandle) || (NULL == sURL) || (0 == unTimeout))
    {
        IVS_RUN_LOG_CRI("send teardown message fail, parameter is invalid.");
        return RTSP_ERR_PARAMETER;
    }

    // ��������Ƿ����
    CRtspSession* pSession = (CRtspSession*)nHandle;
    if (!CRtspSessionManager::instance()->findSession(pSession))
    {
        IVS_RUN_LOG_CRI("handle is not a valid Session.");
        return RTSP_ERR_NOT_EXIST;
    }

    // ����
    ACE_Guard <ACE_Thread_Mutex> locker (pSession->m_SessionMutex);

    // ��װoption��Ϣ
    CRtspOptionsMessage* pRtspMsg = NULL;
    pRtspMsg = pSession->createRtspMessage((CRtspOptionsMessage*)NULL); //lint !e838
    if (NULL == pRtspMsg)
    {
        IVS_RUN_LOG_CRI("create option message failed.");
        return RET_FAIL;
    }

    std::string strRtspMsg = "";
    pRtspMsg->setRtspUrl(sURL);
    int nRet = pRtspMsg->encodeMessage(strRtspMsg);
    if (RET_OK != nRet)
    {
        pSession->destroyRtspMsg();
        IVS_RUN_LOG_CRI("encode pause message fail.");
        return RET_FAIL;
    }

    // ����RTSP OPTION��Ϣ
    nRet = pSession->sendMessage(strRtspMsg.c_str(), strRtspMsg.length());
    if (RET_OK != nRet)
    {
        IVS_RUN_LOG_CRI("send pause message fail.");
        return RET_FAIL;
    }

    // �����ͬ��ģʽ���ȴ����Ž��
    if (pSession->m_bBlocked)
    {
//BEGIN: Add by qiuweilin 64597
//��redhat 32λƽ̨�£�ʹ��acquire��ȡ�ź���һֱ��ʱ
//����취��ʹ��tryacquire���������Ի�ȡ5000�Σ�5000��֮����δ��ȡ���ź������򰴳�ʱ����
#ifdef OS_REDHAT_i386
		int iTryCnt = 0;
		while(iTryCnt <= MAX_TRY_COUNT)
		{
			nRet = pSession->m_pRtspSemaphore->tryacquire();
			if (0 == nRet)
			{
				break;
			}

			usleep(USLEEP_INTERVAL);
			iTryCnt++;
		}
#else
//END: Add by qiuweilin 64597
        // ����Pause��Ϣ�ĳ�ʱʱ��
        ACE_Time_Value wait = ACE_OS::gettimeofday();
        wait.sec(wait.sec() + unTimeout);
        nRet = pSession->m_pRtspSemaphore->acquire(&wait);
#endif
        if (-1 == nRet)
        {
            ACE_Guard <ACE_Thread_Mutex> locker (pSession->m_Mutex); //lint !e578
            pSession->destroyRtspMsg();
            IVS_RUN_LOG_CRI("Sync mode:recv pause response timeout.");
            return RTSP_ERR_TIMEOUT;
        }
        else if (0 != nRet)
        {
            IVS_RUN_LOG_CRI("Sync mode:recv pause response fail.");
            return RET_FAIL;
        }
        if (!pSession->m_bRecvRetFlag)
        {
            return RET_FAIL;
        }
        else
        {
            if (NULL == strMsg)
            {
                return RET_FAIL;
            }
            iLength = (int)pSession->m_strTimeInfo.length();
            memcpy(strMsg,pSession->m_strTimeInfo.c_str(),(unsigned int)iLength);
        }
    }
    else
    {   //�첽ģʽ��ע�ᳬʱ��ʱ��
        nRet = pSession->setRecvTimer(unTimeout);
        if (RET_OK != nRet)
        {
            pSession->destroyRtspMsg();
            IVS_RUN_LOG_CRI("Async mode:set pause recv timer failed.");
            return RET_FAIL;
        }
    }
    return RET_OK;
}
/***************************************************************************
 �� �� �� : RTSP_CLIENT_SendPauseMsg
 �������� : ����RTSP PAUSE����
 ������� :  nHandle                     ���Ӿ��
             sURL                        PAUSE��Ϣ��װ�е�URL
             unTimeout                   PAUSE��Ϣ��ʱʱ�䣨��λ��S��
 ������� : ��
 �� �� ֵ : RET_OK-�ɹ�     RET_FAIL-ʧ��
***************************************************************************/
int RTSPAPI RTSP_CLIENT_SendPauseMsg(long nHandle, const char *sURL, unsigned int unTimeout)
{ //lint !e1784
    // �����Ч�Լ��
    if ((0 == nHandle) || (NULL == sURL) || (0 == unTimeout))
    {
        IVS_RUN_LOG_CRI("send pause message fail, parameter invalid.");
        return RTSP_ERR_PARAMETER;
    }

    // ��������Ƿ����
    CRtspSession* pSession = (CRtspSession*)nHandle;
    if (!CRtspSessionManager::instance()->findSession(pSession))
    {
        IVS_RUN_LOG_CRI("handle is not a valid Session.");
        return RTSP_ERR_NOT_EXIST;
    }

    // ����
    ACE_Guard <ACE_Thread_Mutex> locker (pSession->m_SessionMutex);
    // ���Ự״̬������Ѿ���READY״̬��ֱ�ӷ���
    if (RTSP_SESSION_STATUS_READY == pSession->getStatus())
    {
        return RET_OK;
    } 
    else if (RTSP_SESSION_STATUS_PLAYING != pSession->getStatus())
    {
        IVS_RUN_LOG_CRI("send rtsp pause message fail, invalid status[%d], handle[%p].", 
                        pSession->getStatus(),
                        pSession);
        // �������PLAYING״̬�����ش���
        return RTSP_ERR_STAUTS_ABNORMAL;
    }

    // ��װpause��Ϣ
    CRtspPauseMessage* pRtspMsg = NULL;
    pRtspMsg = pSession->createRtspMessage((CRtspPauseMessage*)NULL); //lint !e838
    if (NULL == pRtspMsg)
    {
        IVS_RUN_LOG_CRI("create pause message failed.");
        return RET_FAIL;
    }

    pRtspMsg->setRtspUrl(sURL);
    std::string strRtspMsg = "";
    int nRet = pRtspMsg->encodeMessage(strRtspMsg);
    if (RET_OK != nRet)
    {
        pSession->destroyRtspMsg();
        IVS_RUN_LOG_CRI("encode pause message fail.");
        return RET_FAIL;
    }

    // ����RTSP Pause��Ϣ
    nRet = pSession->sendMessage(strRtspMsg.c_str(), strRtspMsg.length());
    if (RET_OK != nRet)
    {
        IVS_RUN_LOG_CRI("send pause message fail.");
        return RET_FAIL;
    }

    // �����ͬ��ģʽ���ȴ����Ž��
    if (pSession->m_bBlocked)
    {
//BEGIN: Add by qiuweilin 64597
//��redhat 32λƽ̨�£�ʹ��acquire��ȡ�ź���һֱ��ʱ
//����취��ʹ��tryacquire���������Ի�ȡ5000�Σ�5000��֮����δ��ȡ���ź������򰴳�ʱ����
#ifdef OS_REDHAT_i386
		int iTryCnt = 0;
		while(iTryCnt <= MAX_TRY_COUNT)
		{
			nRet = pSession->m_pRtspSemaphore->tryacquire();
			if (0 == nRet)
			{
				break;
			}

			usleep(USLEEP_INTERVAL);
			iTryCnt++;
		}
#else
//END: Add by qiuweilin 64597
        // ����Pause��Ϣ�ĳ�ʱʱ��
        ACE_Time_Value wait = ACE_OS::gettimeofday();
        wait.sec(wait.sec() + unTimeout);
        nRet = pSession->m_pRtspSemaphore->acquire(&wait);
#endif
        if (-1 == nRet)
        {
            ACE_Guard <ACE_Thread_Mutex> locker (pSession->m_Mutex); //lint !e578
            pSession->destroyRtspMsg();
            IVS_RUN_LOG_CRI("Sync mode:recv pause response timeout.");
            return RTSP_ERR_TIMEOUT;
        }
        else if (0 != nRet)
        {
            IVS_RUN_LOG_CRI("Sync mode:recv pause response fail.");
            return RET_FAIL;
        }
        if (!pSession->m_bRecvRetFlag)
        {
            return RET_FAIL;
        }
    }
    else
    {   //�첽ģʽ��ע�ᳬʱ��ʱ��
        nRet = pSession->setRecvTimer(unTimeout);
        if (RET_OK != nRet)
        {
            pSession->destroyRtspMsg();
            IVS_RUN_LOG_CRI("Async mode:set pause recv timer failed.");
            return RET_FAIL;
        }
    }
    return RET_OK;
}


/***************************************************************************
 �� �� �� : RTSP_CLIENT_SendTeardownMsg
 �������� : ����RTSP TEARDOWN����
 ������� :  nHandle                    ���Ӿ��
            sURL                        TEARDOWN��Ϣ��װ�е�URL
            unTimeout                   PAUSE��Ϣ��ʱʱ�䣨��λ��S��
 ������� : ��
 �� �� ֵ : RET_OK-�ɹ�     RET_FAIL-ʧ��
***************************************************************************/
int RTSPAPI RTSP_CLIENT_SendTeardownMsg(long nHandle, const char *sURL, unsigned int unTimeout)
{ //lint !e1784
    // ��μ��
    if ((0 == nHandle) || (NULL == sURL) || (0 == unTimeout))
    {
        IVS_RUN_LOG_CRI("send teardown message fail, parameter is invalid.");
        return RTSP_ERR_PARAMETER;
    }

    // ���Ӳ���
    CRtspSession* pSession = (CRtspSession*)nHandle;
    // ��������Ƿ����
    if (!CRtspSessionManager::instance()->findSession(pSession))
    {
        IVS_RUN_LOG_CRI("handle is not a valid Session.");
        return RTSP_ERR_NOT_EXIST;
    }

    // ����
    ACE_Guard <ACE_Thread_Mutex> locker (pSession->m_SessionMutex);
    // �����INIT״̬����������ֱ�ӷ��سɹ�
    if (RTSP_SESSION_STATUS_INIT == pSession->getStatus())
    {
        IVS_RUN_LOG_INF("session[%p] status has been init already, don't need to teardown.", nHandle);
        return RET_OK;
    }

    // ��װTeardown��Ϣ
    CRtspTeardownMessage* pRtspMsg = NULL;
    pRtspMsg = pSession->createRtspMessage((CRtspTeardownMessage*)NULL); //lint !e838
    if (NULL == pRtspMsg)
    {
        IVS_RUN_LOG_CRI("create teardown message fail.");
        return RET_FAIL;
    }

    pRtspMsg->setRtspUrl(sURL);
    std::string strRtspMsg = "";
    int nRet = pRtspMsg->encodeMessage(strRtspMsg);
    if (RET_OK != nRet)
    {
        pSession->destroyRtspMsg();
        IVS_RUN_LOG_CRI("encode teardown message fail.");
        return RET_FAIL;
    }
    // ������Ϣ
    nRet = pSession->sendMessage(strRtspMsg.c_str(), strRtspMsg.length());
    if (RET_OK != nRet)
    {
        IVS_RUN_LOG_CRI("send teardown message fail.");
        return RET_FAIL;
    }

    // ͬ��ģʽ������Ϣ
    if (pSession->m_bBlocked)
    {
//BEGIN: Add by qiuweilin 64597
//��redhat 32λƽ̨�£�ʹ��acquire��ȡ�ź���һֱ��ʱ
//����취��ʹ��tryacquire���������Ի�ȡ5000�Σ�5000��֮����δ��ȡ���ź������򰴳�ʱ����
#ifdef OS_REDHAT_i386
		int iTryCnt = 0;
		while(iTryCnt <= MAX_TRY_COUNT)
		{
			nRet = pSession->m_pRtspSemaphore->tryacquire();
			if (0 == nRet)
			{
				break;
			}

			usleep(USLEEP_INTERVAL);
			iTryCnt++;
		}
#else
//END: Add by qiuweilin 64597
        // ����Teardown��Ϣ�ĳ�ʱʱ��
        ACE_Time_Value wait = ACE_OS::gettimeofday();
        wait.sec(wait.sec() + unTimeout);
        nRet = pSession->m_pRtspSemaphore->acquire(&wait);
#endif
        if (-1 == nRet)
        {
            // �ǲ���Ӧ�ü���
            ACE_Guard <ACE_Thread_Mutex> locker (pSession->m_Mutex); //lint !e578
            pSession->destroyRtspMsg();
            IVS_RUN_LOG_CRI("sync mode recv teardown response timeout, handle[%p].", pSession);

            return RTSP_ERR_TIMEOUT;
        }
        if (!pSession->m_bRecvRetFlag)
        {
            return RET_FAIL;
        }
    }
    else
    {
        // �첽ģʽ�����ö�ʱ��
        nRet = pSession->setRecvTimer(unTimeout);
        if (RET_OK != nRet)
        {
            pSession->destroyRtspMsg();
            IVS_RUN_LOG_CRI("async mode set teardown recv timer fail.");
            return RET_FAIL;
        }
    }

    return RET_OK;
}



/***************************************************************************
 �� �� �� : RTSP_CLIENT_GetStatus
 �������� : ��ȡ���ӵ�ǰ״̬
 ������� : nHandle                     ���Ӿ��
 ������� : ��
 �� �� ֵ : RTSP_SESSION_STATUS -����״̬
***************************************************************************/
RTSP_SESSION_STATUS RTSPAPI RTSP_CLIENT_GetStatus(long nHandle)
{ //lint !e1784
    // ��μ��
    if (0 == nHandle)
    {
        return RTSP_SESSION_STATUS_ABNORMAL;
    }

    // ��������Ƿ����
    CRtspSession* pSession = (CRtspSession*) (nHandle);
    if (!CRtspSessionManager::instance()->findSession(pSession))
    {
        IVS_RUN_LOG_CRI("rtsp session not exist.");
        return RTSP_SESSION_STATUS_ABNORMAL;
    }

    return pSession->getStatus();
}


/***************************************************************************
 �� �� �� : RTSP_CLIENT_FreeHandle
 �������� : �ͷ����Ӿ��
 ������� : nHandle                     ���Ӿ��
 ������� : ��
 �� �� ֵ : RET_OK-�ɹ�     RET_FAIL-ʧ��
***************************************************************************/
int RTSPAPI RTSP_CLIENT_FreeHandle(long nHandle)
{ //lint !e1784
    // �����Ч�Լ��
    if (0 == nHandle)
    {
        return RTSP_ERR_PARAMETER;
    }

    // ��������Ƿ����
    CRtspSession* pSession = (CRtspSession*) (nHandle);
    if (!CRtspSessionManager::instance()->findSession(pSession))
    {
        IVS_RUN_LOG_CRI("rtsp session not exist.");
        return RTSP_ERR_NOT_EXIST;
    }
 
	/* 
	 *  connect ʧ�ܵ� rtsp session ���ᱻע�ᵽreactor��������
	 *  �������ɵ�����ɾ��
	 */
	if (pSession->IsConnectFail())
	{
		CRtspSessionManager::instance()->deleteSession(pSession);
		delete pSession;
		return RET_OK;
	}

    // �ͷ�����
	IVS_RUN_LOG_CRI("begin Set Status abnormal state.");
    ACE_Guard <ACE_Thread_Mutex> locker (pSession->m_SessionMutex);
    pSession->setStatus(RTSP_SESSION_STATUS_ABNORMAL);
	IVS_RUN_LOG_CRI("end Set Status abnormal state.");
    return RET_OK;
}


/***************************************************************************
 �� �� �� : RTSP_CLIENT_Cleanup
 �������� : �ͷ�ͨѶ��
 ������� : ��
 ������� : ��
 �� �� ֵ : RET_OK-�ɹ�     RET_FAIL-ʧ��
***************************************************************************/
int RTSPAPI RTSP_CLIENT_Cleanup()
{ //lint !e1784
    if (g_bIsFirst)
    {
        return RET_OK;
    }
    g_bIsFirst = true;

    // �ȹر�RTSP���ӹ�����
    CRtspSessionManager::instance()->closeManager();

    // �رշ�Ӧ��������
    if (RET_OK != CRtspReactorManager::instance()->closeManager())
    {
        return RET_FAIL;
    }

    // �ر���־
    (void)CRtsp_Log_Manager::instance().endLog();
//
//#ifdef WIN32
//    IVS_DBG_LOG("ACE fini");
    ACE::fini();
//#endif

    return RET_OK;
}


