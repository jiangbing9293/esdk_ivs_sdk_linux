/********************************************************************
    filename	: 	LoginMgr.cpp
    author		:	x00192614
    created		:	2011/11/1
    description	:	�û���¼������
    copyright	:	Copyright (C) 2011-2015
    history		:	2011/5/20 ��ʼ�汾
 *********************************************************************/


#include "LoginMgr.h"
#include "UserMgr.h"
#include "bp_log.h"
//#include "LinkIdMgr.h"
#include "UserInfo.h"

#include "ToolsHelp.h"
#include "NSSOperator.h"
#include "MKTransID.h"
#include "nss_mt.h"
#include "vos.h"
#include "ivs_xml.h"
#include "LinkRouteMgr.h"
#include "LoginXMLProcess.h"
#include "SDKSecurityClient.h"
#include "IVS_Trace.h"

CLoginMgr::CLoginMgr(void)
    : m_bLoginOmuThreadRun(false)
     ,m_pLoginOMUThread(NULL)
    ,m_pUserMgr(NULL)
	,m_enLoginType(NORMAL_LOGIN)
{
}


CLoginMgr::~CLoginMgr(void)
{
    m_pUserMgr = NULL;
    
    if (m_bLoginOmuThreadRun  &&  (NULL != m_pLoginOMUThread)   &&  (NULL != m_pLoginOMUThread->pthead))
    {
        BP_DBG_LOG("Stop login Omu failed, wait next.");

        m_bLoginOmuThreadRun = false;
#ifdef WIN32
        unsigned long uiRet = WaitForSingleObject(m_pLoginOMUThread->pthead, 11000);		

        //�߳�δ������ǿ��ɱ���߳�
        if (WAIT_OBJECT_0 != uiRet)
        {
            BP_RUN_LOG_INF("CLoginMgr::~CLoginMgr:", "kill the hRleaseThread, VOS_SemWaitTimeout iRet:%u", uiRet);
            DWORD dwExitWord = 0;
            (void)GetExitCodeThread(m_pLoginOMUThread->pthead,&dwExitWord);
            (void)TerminateThread(m_pLoginOMUThread->pthead,dwExitWord);
        }
        (void)CloseHandle(m_pLoginOMUThread->pthead);
#else
		//�ȴ�11s�������߳�
		VOS_Sleep(11000);
		VOS_StopThread(m_pLoginOMUThread);
#endif
        m_pLoginOMUThread->pthead = NULL;
    }

	if (NULL != m_pLoginOMUThread)
	{
		VOS_free(m_pLoginOMUThread);
		m_pLoginOMUThread = NULL;
	}
}

// �����û�ָ��
void CLoginMgr::SetUserMgr( CUserMgr *pUserMgr)
{
    m_pUserMgr = pUserMgr;
}

// ��¼OMU�߳����
long __SDK_CALL CLoginMgr::LoginOmuThreadEntry(void* pArg)
{
    //IVS_DEBUG_TRACE("");
	BP_DBG_LOG("Enter  LoginOmuThreadEntry");
    CLoginMgr* pLoginMgr = (CLoginMgr*)pArg;
    if (NULL == pLoginMgr)
    {
        BP_RUN_LOG_ERR(IVS_OPERATE_MEMORY_ERROR, "Login Omu Thread Entry failed", "pLoginMgr is null");
        return IVS_OPERATE_MEMORY_ERROR;
    }

    pLoginMgr->m_bLoginOmuThreadRun = true;

    int iRet = IVS_FAIL;

    while (pLoginMgr->m_bLoginOmuThreadRun)
    {
        iRet = pLoginMgr->LoginOmu();
        if (IVS_SUCCEED == iRet)
		{
			BP_DBG_LOG("Login Omu Suc.");
            pLoginMgr->m_bLoginOmuThreadRun = false;
            //(void)CloseHandle(pLoginMgr->m_pLoginOMUThread->pthead);
			VOS_StopThread(pLoginMgr->m_pLoginOMUThread);
            pLoginMgr->m_pLoginOMUThread->pthead = NULL;
            break;
        }

        BP_DBG_LOG("Login Omu failed, wait next.");

        VOS_Sleep(10 * 1000); //û�е�½�ɹ����ȴ�10s�ٽ�����һ�ε�½
    }

    return iRet;
}

// �û���½
IVS_INT32 CLoginMgr::Login(const IVS_LOGIN_INFO* pLoginReqInfo)
{
    IVS_DEBUG_TRACE("");
    CHECK_POINTER(pLoginReqInfo, IVS_OPERATE_MEMORY_ERROR);
    CHECK_POINTER(m_pUserMgr, IVS_OPERATE_MEMORY_ERROR);
	//���ڱ����¼��Ӧ��Ϣ�ṹ 
	ST_SRU_LOGIN_RSP stLoginRspInfo;
	memset(&stLoginRspInfo, 0, sizeof(stLoginRspInfo));

	//��¼��SMU��������Ӧ��Ϣ
	IVS_INT32 iRet = LoginSmu(pLoginReqInfo, stLoginRspInfo);
	if (IVS_SUCCEED != iRet && IVS_SMU_USER_FIRST_LOGIN != iRet && IVS_SMU_USER_PWD_NEED_MOD != iRet)
	{
        BP_RUN_LOG_ERR(iRet, "Login failed", "iRet = %d", iRet);
        return iRet;
	}
	// ��ʼ���û���Ϣ
	(void)m_userInfo.Init(m_pUserMgr->GetSessionID(), pLoginReqInfo, stLoginRspInfo);
	// ��ȡ��·�ɱ�NVRList��������б�
	GetInitData();

    // CU�ӽ���,����ǽ��¼������Ҫ��¼��OMU
    if (CLIENT_PC_CHILD ==  pLoginReqInfo->uiClientType 
        || CLIENT_TV_WALL == pLoginReqInfo->uiClientType)
    {
        return iRet;
    }
	//��OMU����tcp������,����loginId��Ȩ
    long lResult = (long)VOS_CreateThread((VOS_THREAD_FUNC)LoginOmuThreadEntry, (void*)this, &m_pLoginOMUThread, VOS_DEFAULT_STACK_SIZE);
    if (0 != lResult)
    {
        BP_RUN_LOG_ERR(IVS_FAIL, "Login Omu", "VOS_CreateThread to LoginOmu failed");
    }
	return iRet;
}


//�û������¼;
IVS_INT32 CLoginMgr::LoginByTicket(const IVS_LOGIN_INFO_EX* pLoginReqInfoEx)
{
	IVS_DEBUG_TRACE("");
	CHECK_POINTER(pLoginReqInfoEx, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(m_pUserMgr, IVS_OPERATE_MEMORY_ERROR);

	//���ڱ����¼��Ӧ��Ϣ�ṹ;
	ST_SRU_LOGIN_RSP stLoginRspInfoEx;
	memset(&stLoginRspInfoEx, 0, sizeof(stLoginRspInfoEx));

	//��¼��SMU��������Ӧ��Ϣ
	IVS_INT32 iRet = LoginSmuByTicket(pLoginReqInfoEx, stLoginRspInfoEx);
	if (IVS_SUCCEED != iRet && IVS_SMU_USER_FIRST_LOGIN != iRet && IVS_SMU_USER_PWD_NEED_MOD != iRet)
	{
		BP_RUN_LOG_ERR(iRet, "Login failed", "iRet = %d", iRet);
		return iRet;
	}

	//��ȡ�û���Ϣ;
	IVS_USER_INFO stUserInfo;
	memset(&stUserInfo, 0, sizeof(IVS_USER_INFO));
	//delete by zwx211831, Date:20140730, SMU���ĵ�¼�߼�����Ҫ�ڵȴ�2s�ˡ�
	//add by zwx211831, Date:20140630,�ȴ�2s���ٻ�ȡ�û���Ϣ����Ϊsmu��Ҫʱ���ʼ��һЩ��Դ
	//VOS_Sleep(2 * 1000); 
	iRet = m_pUserMgr->GetUserInfoMgr().GetUserInfo("", stLoginRspInfoEx.uiUserID, &stUserInfo);
	if (IVS_SUCCEED != iRet)
	{
		//add by zwx211831����¼ʧ��Ҫɾ���Ѵ洢��linkinfo
		CLinkRouteMgr::instance().DeleteLinkList(m_pUserMgr->GetSessionID());
		BP_RUN_LOG_ERR(iRet, "LoginByTicket", "Reason:GetUserInfo failed");
		return iRet;
	}

	// ��ʼ���û���Ϣ;
	(void)m_userInfo.Init(m_pUserMgr->GetSessionID(), stUserInfo.cLoginName, pLoginReqInfoEx, stLoginRspInfoEx);

	// ��ȡ��·�ɱ�NVRList��������б�;
	GetInitData();

	// CU�ӽ���,����ǽ��¼������Ҫ��¼��OMU
	if (CLIENT_PC_CHILD ==  pLoginReqInfoEx->uiClientType 
		|| CLIENT_TV_WALL == pLoginReqInfoEx->uiClientType)
	{
		return iRet;
	}

	//��OMU����tcp������,����loginId��Ȩ
	long lResult = (long)VOS_CreateThread((VOS_THREAD_FUNC)LoginOmuThreadEntry, (void*)this, &m_pLoginOMUThread, VOS_DEFAULT_STACK_SIZE);
	if (IVS_SUCCEED != lResult)
	{
		BP_RUN_LOG_ERR(IVS_FAIL, "Login Omu", "VOS_CreateThread to LoginOmu failed");
	}
	return iRet;
}


/**************************************************************************
* name       : GetLoginRspInfo
* description: ��ȡ��¼��Ӧ��Ϣ
* input      : NA
* output     : pLoginRspInfo  ��¼�ɹ����ص���Ϣ
* return     : �ɹ�-IVS_SUCCEED��ʧ��-IVS_FAIL
* remark     : NA
**************************************************************************/
// IVS_INT32 CLoginMgr::GetLoginRspInfo(LOGIN_RSP_INFO* pLoginRspInfo )
// {
// 	BP_RUN_LOG_INF("Enter CLoginMgr::GetLoginRspInfo","");
// 	CHECK_POINTER(pLoginRspInfo, IVS_SDK_RET_INTRINSIC_PTR_ERROR);
// 	ST_SRU_LOGIN_RSP stSRULoginRsp;
// 	memset(&stSRULoginRsp, 0, sizeof(ST_SRU_LOGIN_RSP));
// 	if (IVS_SUCCEED == m_userInfo.GetLoginRspInfo(stSRULoginRsp))
// 	{
// 		memset(pLoginRspInfo, 0, sizeof(LOGIN_RSP_INFO));
//         strncpy(pLoginRspInfo->cPlatformVersion, stSRULoginRsp.cPlatformVersion, PLATFORM_VERSION_LEN);
//         strncpy(pLoginRspInfo->cSessionID1, stSRULoginRsp.szSessionID, SESSION_ID_LEN);
//         strncpy(pLoginRspInfo->cTimeZone, stSRULoginRsp.szTimeZone, TIME_ZONE_LEN);
//         strncpy(pLoginRspInfo->cLoginID, stSRULoginRsp.szLoginID, LOGIN_ID_LEN);
//         strncpy(pLoginRspInfo->cOrgCode, stSRULoginRsp.szOrgCode, ORG_CODE_LEN);
//         pLoginRspInfo->iResultCode = stSRULoginRsp.iStatusCode;
// 		BP_RUN_LOG_INF("Leave CLoginMgr::GetLoginRspInfo", "");
// 		return IVS_SUCCEED;
// 	}
// 	BP_RUN_LOG_INF("Leave CLoginMgr::GetLoginRspInfo", "");
// 	return IVS_FAIL;
// }//lint !e1762

// �û�ע��
IVS_INT32 CLoginMgr::Logout(bool bSendLogoutCmd)
{
    IVS_DEBUG_TRACE("");
    CHECK_POINTER(m_pUserMgr, IVS_OPERATE_MEMORY_ERROR);
	int iRet = IVS_FAIL;
    // ��ȡ��½smuIP,����ע������,�û�ǩ�˲���Ҫ����Logout����
	if (bSendLogoutCmd)
	{
		string strSmuLinkID;
		m_pUserMgr->GetSMULinkID(strSmuLinkID);// lint !e526
		iRet = LogoutSmu(strSmuLinkID);
	}
	       
	if (NULL != m_pLoginOMUThread)
	{
	       m_bLoginOmuThreadRun = false;
              if (NULL != m_pLoginOMUThread->pthead)
              {
       	    VOS_ThreadJoin(m_pLoginOMUThread);//�ȴ���½OMU�߳�
              }
              
		VOS_free(m_pLoginOMUThread);
		m_pLoginOMUThread = NULL;
	}
	
	m_userInfo.Clear();
	return iRet;
}

// ��ȡ�û���Ϣ
CUserInfo* CLoginMgr::GetUserInfo()
{
	return &m_userInfo;//lint !e1536
}

// ��ȡ�û���¼���ص�LoginId
char* CLoginMgr::GetLoginId()
{
    return m_userInfo.GetLoginId();
}

// ��ȡ�û���¼���ص�UserID
IVS_UINT32 CLoginMgr::GetUserID() const
{
    return m_userInfo.GetUserID();
}

// ��ȡ�û��ĵ�¼���ͣ������¼����ͨ��¼
IVS_LOGIN_TYPE_E CLoginMgr::GetLoginType() const
{
	return m_enLoginType;
}

// �û���¼������SMU
IVS_INT32 CLoginMgr::LoginSmu(const IVS_LOGIN_INFO* pLoginReqInfo, ST_SRU_LOGIN_RSP &stSRULoginRsp)
{
    IVS_DEBUG_TRACE("");
    CHECK_POINTER(pLoginReqInfo,IVS_OPERATE_MEMORY_ERROR);
    CHECK_POINTER(m_pUserMgr,IVS_OPERATE_MEMORY_ERROR);
    int resultValue = IVS_FAIL;
    int iSessionID = m_pUserMgr->GetSessionID();

    //���ν��������
    string strLinkID;
	resultValue = CNSSOperator::instance().CreateSyncClientLink(NET_ELE_SMU_NSS, pLoginReqInfo->stIP.cIP, static_cast<int>(pLoginReqInfo->uiPort), strLinkID);
	if (IVS_SUCCEED != resultValue)
	{
		BP_RUN_LOG_ERR(IVS_NET_CONNECT_ERROR, "Create client smuLink failed", "failed");
		return IVS_NET_CONNECT_ERROR;
	}
	CNSSOperator::instance().SetSMULinkID(strLinkID); //lint !e1036
	BP_RUN_LOG_INF("Create client smuLink ", "ok %s", strLinkID.c_str());
	// �������Ӷ��� 
    CLinkInfo oLinkIDInfoTemp;
    oLinkIDInfoTemp.Init(NET_ELE_SMU_NSS, LINKID_STATUS_OPENED, strLinkID, LOCAL_DOMAIN_CODE_INIT, pLoginReqInfo->stIP.cIP,"");

	//modify by zwx211831, Date:20140618, ��Ϊ��¼�ɹ������LinkInfo��Map
    //// add by s00191067
    //// �����û���link·��ˢ�µ�·�ɱ���;
    //int iLinkRouteRet = CLinkRouteMgr::instance().UpdateLinkInfo(iSessionID, oLinkIDInfoTemp);
    //if (IVS_SUCCEED != iLinkRouteRet)
    //{
    //    CLinkRouteMgr::instance().DeleteLinkList(iSessionID);
    //    BP_RUN_LOG_ERR(IVS_FAIL, "Login Smu", "AddLinkMgr failed");
    //    return iLinkRouteRet;
    //}
    //// end 

    // ��ʼ����Ϣ��ȫ�ͻ��˶��� TODO ��SMU SCUһ���ύ
    CSDKSecurityClient oSecurityClient;
    int iInitSecurityClient = oSecurityClient.Init(NET_ELE_SMU_NSS, strLinkID);
    if (IVS_SUCCEED != iInitSecurityClient)
    {
        CLinkRouteMgr::instance().DeleteLinkList(iSessionID);
        BP_RUN_LOG_ERR(IVS_FAIL, "Login Smu", "oSecurityClient Init failed");
        return IVS_FAIL;
    }
    // �����û���������
    
    char cLoginName[IVS_NAME_LEN + 1] = {0};
    strncpy(cLoginName, pLoginReqInfo->cUserName, IVS_NAME_LEN);

	char cLoignPWD[IVS_PWD_LEN + 1] = {0};
    strncpy(cLoignPWD, pLoginReqInfo->pPWD, IVS_PWD_LEN);

    std::string strLoignCipherName;
    std::string strLoignCipherPWD;
    int iEncryptRet = oSecurityClient.EncryptString(cLoginName, strLoignCipherName);
    iEncryptRet += oSecurityClient.EncryptString(cLoignPWD, strLoignCipherPWD);
	memset(cLoignPWD, 0, IVS_PWD_LEN + 1);
    if (IVS_SUCCEED != iEncryptRet)
    {
        CLinkRouteMgr::instance().DeleteLinkList(iSessionID);
        BP_RUN_LOG_ERR(IVS_FAIL, "Login Smu", "oSecurityClient Init failed");
        return IVS_FAIL;
    }

    //ƴװ����xml
    CXml xml;
    resultValue = CLoginXMLProcess::GetLoginXML(pLoginReqInfo,strLoignCipherName,strLoignCipherPWD, m_pUserMgr, xml);
    if (IVS_SUCCEED != resultValue)
    {
        CLinkRouteMgr::instance().DeleteLinkList(iSessionID);
        BP_RUN_LOG_ERR(resultValue, "Login Smu", "iRet = %d", resultValue);
        return resultValue;
    }
    unsigned int iLen = 0;
    const char* pszData = xml.GetXMLStream(iLen);
    CHECK_POINTER(pszData, IVS_FAIL);
    std::string strSecurityTransID  = oSecurityClient.GetSecurityDataTransID();
    //std::string strSecurityTransID = CMKTransID::Instance().GenerateTransID();
    CCmd *pCmd = CNSSOperator::instance().BuildCmd(strLinkID, NET_ELE_SMU_NSS, NSS_LOGIN_REQ, pszData, strSecurityTransID);
    if (NULL == pCmd)
    {
        CLinkRouteMgr::instance().DeleteLinkList(iSessionID);
        BP_RUN_LOG_ERR(IVS_FAIL, "Login Smu", "Build SMUCmd failed");
        return IVS_FAIL;
    }

    //����nssЭ��
    CCmd *pCmdRsp = CNSSOperator::instance().SendSyncCmd(pCmd);
    if(NULL == pCmdRsp)
    {
        CLinkRouteMgr::instance().DeleteLinkList(iSessionID);
        BP_RUN_LOG_ERR(IVS_NET_RECV_TIMEOUT, "Login Smu", "SendSyncCmd but no response or time is out");
        return IVS_NET_RECV_TIMEOUT;
    }

    //��Ӧ��xml��Ϣ��ʱ��ParseCmd2XML����NULL�����������ڴ棬����delete pRspXml
    char* pRspXml = CNSSOperator::instance().ParseCmd2XML(pCmdRsp,resultValue);
    if (NULL == pRspXml)
    {
        HW_DELETE(pCmdRsp);
        CLinkRouteMgr::instance().DeleteLinkList(iSessionID);
        BP_RUN_LOG_ERR(resultValue, "Login Smu", "ParseCmd2XML failed");
        return resultValue;
    }
    //��������
    CXml xmlRsp;
    (void)xmlRsp.Parse(pRspXml);
    (void)CLoginXMLProcess::ParseLoginXML(xmlRsp, oSecurityClient, stSRULoginRsp);
    IVS_DELETE(pRspXml);
    HW_DELETE(pCmdRsp);

	//add by zwx211831,Date:20140618,��Link��Ϣ���µ�LinkInfo��Map����
	// �����û���link·��ˢ�µ�·�ɱ���;
	int iLinkRouteRet = CLinkRouteMgr::instance().UpdateLinkInfo(iSessionID, oLinkIDInfoTemp);
	if (IVS_SUCCEED != iLinkRouteRet)
	{
		CLinkRouteMgr::instance().DeleteLinkList(iSessionID);
		BP_RUN_LOG_ERR(IVS_FAIL, "Login Smu", "AddLinkMgr failed");
		return iLinkRouteRet;
	}
	// end 

	m_enLoginType = NORMAL_LOGIN;

    return resultValue;
}


//�û������¼������SMU;
IVS_INT32 CLoginMgr::LoginSmuByTicket(const IVS_LOGIN_INFO_EX* pLoginReqInfoEx, ST_SRU_LOGIN_RSP &stSRULoginRspEx)
{
	IVS_DEBUG_TRACE("");
	CHECK_POINTER(pLoginReqInfoEx,IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(m_pUserMgr,IVS_OPERATE_MEMORY_ERROR);
	int resultValue = IVS_FAIL;
	int iSessionID = m_pUserMgr->GetSessionID();

	//���ν��������;
	string strLinkID;
	resultValue = CNSSOperator::instance().CreateSyncClientLink(NET_ELE_SMU_NSS, pLoginReqInfoEx->stIP.cIP, static_cast<int>(pLoginReqInfoEx->uiPort), strLinkID);
	if (IVS_SUCCEED != resultValue)
	{
		BP_RUN_LOG_ERR(IVS_NET_CONNECT_ERROR, "Create client smuLink failed", "failed");
		return IVS_NET_CONNECT_ERROR;
	}
	CNSSOperator::instance().SetSMULinkID(strLinkID); //lint !e1036
	BP_RUN_LOG_INF("Create client smuLink ", "ok %s", strLinkID.c_str());

	// �������Ӷ��� ;
	CLinkInfo oLinkIDInfoTemp;
	oLinkIDInfoTemp.Init(NET_ELE_SMU_NSS, LINKID_STATUS_OPENED, strLinkID, LOCAL_DOMAIN_CODE_INIT, pLoginReqInfoEx->stIP.cIP,"");
	
	// �����û���link·��ˢ�µ�·�ɱ���;
	int iLinkRouteRet = CLinkRouteMgr::instance().UpdateLinkInfo(iSessionID, oLinkIDInfoTemp);
	if (IVS_SUCCEED != iLinkRouteRet)
	{
		CLinkRouteMgr::instance().DeleteLinkList(iSessionID);
		BP_RUN_LOG_ERR(IVS_FAIL, "Login Smu By TicketID", "AddLinkMgr failed");
		return iLinkRouteRet;
	}

	// ��ʼ����Ϣ��ȫ�ͻ��˶��� TODO ��SMU SCUһ���ύ;
	CSDKSecurityClient oSecurityClient;
	int iInitSecurityClient = oSecurityClient.Init(NET_ELE_SMU_NSS, strLinkID);
	if (IVS_SUCCEED != iInitSecurityClient)
	{
		CLinkRouteMgr::instance().DeleteLinkList(iSessionID);
		BP_RUN_LOG_ERR(IVS_FAIL, "Login Smu By TicketID", "oSecurityClient Init failed");
		return IVS_FAIL;
	}

	//ƴװ����xml;
	CXml xml;
	resultValue = CLoginXMLProcess::GetLoginByTicketXML(pLoginReqInfoEx, m_pUserMgr, xml);
	if (IVS_SUCCEED != resultValue)
	{
		CLinkRouteMgr::instance().DeleteLinkList(iSessionID);
		BP_RUN_LOG_ERR(resultValue, "Login Smu By TicketID", "iRet = %d", resultValue);
		return resultValue;
	}

	unsigned int iLen = 0;
	const char* pszData = xml.GetXMLStream(iLen);
	CHECK_POINTER(pszData, IVS_FAIL);
	std::string strSecurityTransID  = oSecurityClient.GetSecurityDataTransID();
	CCmd *pCmd = CNSSOperator::instance().BuildCmd(strLinkID, NET_ELE_SMU_NSS, NSS_SSO_LOGIN_REQ, pszData, strSecurityTransID);
	if (NULL == pCmd)
	{
		CLinkRouteMgr::instance().DeleteLinkList(iSessionID);
		BP_RUN_LOG_ERR(IVS_FAIL, "Login Smu By TicketID", "Build SMUCmd failed");
		return IVS_FAIL;
	}

	//����nssЭ��;
	CCmd *pCmdRsp = CNSSOperator::instance().SendSyncCmd(pCmd);
	if(NULL == pCmdRsp)
	{
		CLinkRouteMgr::instance().DeleteLinkList(iSessionID);
		BP_RUN_LOG_ERR(IVS_NET_RECV_TIMEOUT, "Login Smu By TicketID", "SendSyncCmd but no response or time is out");
		return IVS_NET_RECV_TIMEOUT;
	}

	//��Ӧ��xml��Ϣ��ʱ��ParseCmd2XML����NULL�����������ڴ棬����delete pRspXml;
	char* pRspXml = CNSSOperator::instance().ParseCmd2XML(pCmdRsp,resultValue);
	if (NULL == pRspXml)
	{
		HW_DELETE(pCmdRsp);
		CLinkRouteMgr::instance().DeleteLinkList(iSessionID);
		BP_RUN_LOG_ERR(resultValue, "Login Smu By TicketID", "ParseCmd2XML failed");
		return resultValue;
	}

	//��������;
	CXml xmlRsp;
	(void)xmlRsp.Parse(pRspXml);
	(void)CLoginXMLProcess::ParseLoginByTicketXML(xmlRsp, oSecurityClient, stSRULoginRspEx);
	IVS_DELETE(pRspXml);
	HW_DELETE(pCmdRsp);

	m_enLoginType = SSO_LOGIN;

	return resultValue;
}


// ��½��ָ����SCU
IVS_INT32 CLoginMgr::LoginScu(const std::string &strDomainCode,const std::string &strNVRCode, std::string &strLinkID)
{
    IVS_DEBUG_TRACE("strDomainCode:%s, strNVRCode:%s, strLinkID:%s", strDomainCode.c_str(), strNVRCode.c_str(), strLinkID.c_str());
    CHECK_POINTER(m_pUserMgr,IVS_OPERATE_MEMORY_ERROR);
    int resultValue = IVS_FAIL;
    int iSessionID = m_pUserMgr->GetSessionID();
	// �ɹ���¼����ֱ�ӷ���linkID
	CLinkInfo oLinkInfo;
	bool bGetLinkIDRet = CLinkRouteMgr::instance().GetScuLinkByDomainCodeAndNVRCode(iSessionID, strDomainCode, strNVRCode, oLinkInfo);
	if (bGetLinkIDRet)
	{
		strLinkID = oLinkInfo.GetStrLinkId();
		BP_RUN_LOG_INF("Login Scu", "linkID already exist,no need to login scu,strLinkID:%s", strLinkID.c_str());
		return IVS_SUCCEED;
	}
    CDeviceMgr& deviceMgr = m_pUserMgr->GetDeviceMgr();
    // ����������NVR�������NVR��Ϣ
    IVS_DEVICE_BASIC_INFO stDeviceBasicInfo;
    memset(&stDeviceBasicInfo, 0, sizeof(IVS_DEVICE_BASIC_INFO));
    resultValue = deviceMgr.GetNVRInfoByDomainCodeAndNVRCode(strDomainCode, strNVRCode, stDeviceBasicInfo);
    if (IVS_SUCCEED != resultValue)
    {
        BP_RUN_LOG_ERR(resultValue, "Login Scu", "GetNVRInfo By DomainCode And NVRCode failed");
        return resultValue;
    }
    // Ŀ��IPͨ��NVR��Ϣ��ȡ,Ŀ��Portʹ��Ĭ�϶˿�
    char cPeerIPTemp[IVS_IP_LEN + 1] = {0};
    strncpy(cPeerIPTemp, stDeviceBasicInfo.stIP.cIP, IVS_IP_LEN);
    
    int iPeerPort = DEFAULT_SCU_PORT;

    // ����NVR��Ϣ�е�IP���˿ڴ������ӣ�ͬ����
    resultValue = CNSSOperator::instance().CreateSyncClientLink(NET_ELE_SCU_NSS, cPeerIPTemp, iPeerPort, strLinkID);
    if (IVS_SUCCEED != resultValue)
    {
        BP_RUN_LOG_ERR(resultValue, "Login Scu", "Create SyncClientLink to scu failed");
        return IVS_NET_CONNECT_ERROR;
    }

    // ͬ���������ӳɹ�������linkID��ӵ�linkIDRouteMgr,ͬ���������ӣ������ɹ���Ϊ����״̬
    CLinkInfo oLinkIDInfoTemp;
    oLinkIDInfoTemp.Init(NET_ELE_SCU_NSS, LINKID_STATUS_OPENED, strLinkID, strDomainCode, cPeerIPTemp, strNVRCode);
    resultValue = CLinkRouteMgr::instance().UpdateLinkInfo(iSessionID, oLinkIDInfoTemp);
    if (IVS_SUCCEED != resultValue)
    {
        // ���ʧ�ܣ�ɾ��linkIDRoute��strLinkID���Ͽ�strLinkID��Ӧ����
        (void)CLinkRouteMgr::instance().RemoveLinkInfo(iSessionID, strLinkID);
        BP_RUN_LOG_ERR(resultValue, "Login Scu", "Add LinkInfo failed");
        return resultValue;
    }
    

    do
    {

        // ��ʼ����Ϣ��ȫ�ͻ��˶���
        CSDKSecurityClient oSecurityClient;
        resultValue = oSecurityClient.Init(NET_ELE_SCU_NSS, strLinkID);
        if (IVS_SUCCEED != resultValue)
        {
            BP_RUN_LOG_ERR(resultValue, "Login Scu", "oSecurityClient Init failed");
            break;
        }
        // ��ȡ��½LoginID
        char* pLoginID = GetLoginId();
        CHECK_POINTER(pLoginID, IVS_OPERATE_MEMORY_ERROR);
        char cLoginID[LOGIN_ID_LEN + 1] = {0};
        strncpy(cLoginID, pLoginID, LOGIN_ID_LEN);

        // ����LoginID TODO ��SMU SCUһ���ύ
        std::string strCipherLoignID;
        resultValue = oSecurityClient.EncryptString(cLoginID, strCipherLoignID);
        if (IVS_SUCCEED != resultValue)
        {
            BP_RUN_LOG_ERR(IVS_FAIL, "Login Scu", "oSecurityClient Init failed");
            break;
        }
        // ƴװxml,���õ�¼OMU��xml
        CXml xml;
        resultValue = CLoginXMLProcess::GetLoginOmuXML(strCipherLoignID, xml);
        if (IVS_SUCCEED != resultValue)
        {
            BP_RUN_LOG_ERR(resultValue, "Get LogScuXML fail", "iRet=%d", resultValue);
            break;
        }
        // ����ͨ��ҵ��������ϢXML����ΪContent����LoginID TODO ��SMU SCUһ���ύ
        std::string strTransID = oSecurityClient.GetSecurityDataTransID();
        //std::string strTransID = CMKTransID::Instance().GenerateTransID();
        resultValue = SendGeneralAuthCmd(NET_ELE_SCU_NSS, strLinkID, xml, strTransID);
        if (IVS_SUCCEED != resultValue)
        {
            BP_RUN_LOG_ERR(IVS_FAIL,"Login Scu failed", "send cmd or auth failed iRet = %d", resultValue);
            break;
        }
        return IVS_SUCCEED;

    } while(false); //lint !e717

    // ͨ��ҵ������ʧ�ܣ�ɾ��linkIDRoute��strLinkID���Ͽ�strLinkID��Ӧ����
    (void)CLinkRouteMgr::instance().RemoveLinkInfo(iSessionID, strLinkID);

    // ����ʧ�ܣ���Ҫɾ��BP�ײ������б�
    (void)CNetFactory::Instance().DeleteClientLink(strLinkID, NET_ELE_SCU_NSS);

    return resultValue;

}

// ��¼��ָ��SMU������SMU���⣩
IVS_INT32 CLoginMgr::GeneralLoginSmu(const std::string &strDomainCode, std::string &strLinkID)
{
    IVS_DEBUG_TRACE("strDomainCode:%s, strLinkID:%s", strDomainCode.c_str(), strLinkID.c_str());
    CHECK_POINTER(m_pUserMgr,IVS_OPERATE_MEMORY_ERROR);
	std::string strLocalDomainCode;
    m_pUserMgr->GetDomainCode(strLocalDomainCode); // lint !e526

    int resultValue = IVS_FAIL;

    CDomainRouteMgr& domainRouteMgr = m_pUserMgr->GetDomainRouteMgr();

    // ������CODE��ȡ����Ϣ
    IVS_DOMAIN_ROUTE stDomainRouteInfo;
    memset(&stDomainRouteInfo, 0, sizeof(IVS_DOMAIN_ROUTE));
    resultValue = domainRouteMgr.GetDomainRoutebyCode(strDomainCode, stDomainRouteInfo);
    if (IVS_SUCCEED != resultValue)
    {
        BP_RUN_LOG_ERR(IVS_SDK_ERR_DOMAINCODE_NOT_EXIST, "General Login Smu", " failed");
        return IVS_SDK_ERR_DOMAINCODE_NOT_EXIST;
    }

    // Ŀ��IPͨ������Ϣ��ȡ,Ŀ��Portʹ��Ĭ�϶˿�
    char cPeerIPTemp[IVS_IP_LEN + 1] = {0};
    strncpy(cPeerIPTemp, stDomainRouteInfo.stIP.cIP, IVS_IP_LEN);
    int iPeerPort = DEFAULT_SMU_PORT;
    
    CLinkInfo oLinkIDInfoTemp;
    // ��������Ϣ�е�IP���˿ڴ������ӣ�ͬ����

    
    resultValue = CNSSOperator::instance().CreateSyncClientLink(NET_ELE_SMU_NSS, cPeerIPTemp, iPeerPort, strLinkID);

    if (IVS_SUCCEED != resultValue)
    {
         
        BP_RUN_LOG_ERR(resultValue, "General Login Smu", "Create SyncClientLink to smu failed");
       //  (void)CLinkRouteMgr::instance().UpdateLinkInfo(iSessionID, NET_ELE_SMU_NSS, LINKID_STATUS_CLOSED, strLinkID, strDomainCode, "");
       //  m_pUserMgr->AddLinkIdToBusinessResumeList(strLinkID, NET_ELE_SMU_NSS, cPeerIPTemp, iPeerPort, TYPE_ADD_RELINK);
        return IVS_NET_CONNECT_ERROR;
    }
	
    // ͬ���������ӳɹ������linkID��linkIDRouteMgr�������ɹ���Ϊ����״̬
  
    //resultValue = CLinkRouteMgr::instance().UpdateLinkInfo(iSessionID, NET_ELE_SMU_NSS, LINKID_STATUS_OPENED, strLinkID, strDomainCode, "");
    //if (IVS_SUCCEED != resultValue)
    //{
    //    BP_RUN_LOG_ERR(resultValue, "General Login Smu", "Add LinkInfo failed");
    //    return resultValue;
    //}

    // ��ʼ����Ϣ��ȫ�ͻ��˶���
    CSDKSecurityClient oSecurityClient;
    int iInitSecurityClient = oSecurityClient.Init(NET_ELE_SMU_NSS, strLinkID);
    if (IVS_SUCCEED != iInitSecurityClient)
    {
      //  (void)CLinkRouteMgr::instance().RemoveLinkInfo(iSessionID, strLinkID);
        BP_RUN_LOG_ERR(IVS_FAIL, "General Login Smu", "oSecurityClient Init failed");
        return iInitSecurityClient;
    }
    // ��ȡ��½LoginID
    char* pLoginID = GetLoginId();
    CHECK_POINTER(pLoginID, IVS_OPERATE_MEMORY_ERROR);
    char cLoginID[LOGIN_ID_LEN + 1] = {0};
    bool bRet = CToolsHelp::Memcpy(cLoginID, LOGIN_ID_LEN, pLoginID, LOGIN_ID_LEN);
    if (!bRet)
    {
        //(void)CLinkRouteMgr::instance().RemoveLinkInfo(iSessionID, strLinkID);
        BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "General Login Smu", "Memcpy failed");
        return IVS_ALLOC_MEMORY_ERROR;
    }
    // ����LoginID
    std::string strCipherLoignID;
    int iEncryptRet = oSecurityClient.EncryptString(cLoginID, strCipherLoignID);
    if (IVS_SUCCEED != iEncryptRet)
    {
       // (void)CLinkRouteMgr::instance().RemoveLinkInfo(iSessionID, strLinkID);
        BP_RUN_LOG_ERR(IVS_FAIL, "General Login Smu", "oSecurityClient Init failed");
        return IVS_FAIL;
    }

    // ƴװxml
    CXml xml;
    int iRet = CLoginXMLProcess::GetGeneralLoginSmuXML(strCipherLoignID.c_str(), strLocalDomainCode, xml);
    if (IVS_SUCCEED != iRet)
    {
      //  (void)CLinkRouteMgr::instance().RemoveLinkInfo(iSessionID, strLinkID);
        BP_RUN_LOG_ERR(IVS_FAIL,"General Login Smu", "iRet=%d", iRet);
        return iRet;
    }
    // ����ͨ��ҵ��������ϢXML����ΪContent����LoginID,�������
    std::string strTransID = oSecurityClient.GetSecurityDataTransID();
    //std::string strTransID = CMKTransID::Instance().GenerateTransID();
    resultValue = SendGeneralAuthCmd(NET_ELE_SMU_NSS, strLinkID, xml, strTransID);
    if (IVS_SUCCEED != resultValue)
    {
        // ͨ��ҵ������ʧ�ܣ�ɾ��linkIDRoute��strLinkID���Ͽ�strLinkID��Ӧ����
       // (void)CLinkRouteMgr::instance().RemoveLinkInfo(iSessionID, strLinkID);
        BP_RUN_LOG_ERR(resultValue, "General Login Smu", "iAuthRet = %d", resultValue);
        return resultValue;
    }

    //// �����ڲ���ȡNVRList����,NVRList��Ϣ������SDK�ڲ��� uiServerType��0 ��ʾ��ѯ����NVR����
    //(void)deviceMgr.DeleteNVRListByDomainCode(strDomainCode);
    //resultValue = deviceMgr.GetAllNVRInfo(strDomainCode.c_str(), 0);
    return resultValue;
}

// �û�ע��
IVS_INT32 CLoginMgr::LogoutSmu(const string& sSmuLinkId)
{
    IVS_DEBUG_TRACE("sSmuLinkId:%s", sSmuLinkId.c_str());
	CXml xml;
	int iRet = IVS_FAIL;
    iRet = CLoginXMLProcess::GetLogoutXML(xml);
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet, "Logout Smu", "GetLogoutXML fail");
		return iRet;
	}
	unsigned int iLen = 0;
	const char* pszData = xml.GetXMLStream(iLen);
	CHECK_POINTER(pszData, IVS_OPERATE_MEMORY_ERROR);

    //���췢��Cmd;
	CCmd *pCmd = NULL;
	switch(m_enLoginType)
	{
	case NORMAL_LOGIN:
		pCmd = CNSSOperator::instance().BuildSMUCmd(NSS_LOGOUT_REQ, pszData, sSmuLinkId);
		break;
	case SSO_LOGIN:
		pCmd = CNSSOperator::instance().BuildSMUCmd(NSS_SSO_LOGOUT_REQ, pszData, sSmuLinkId);
		break;
	default:;
	}
	
	if (NULL == pCmd)
	{
		BP_RUN_LOG_ERR(IVS_FAIL, "Logout Smu", "Build SMUCmd failed");
		return IVS_FAIL;
	}

	//����nssЭ��
	CCmd *pCmdRsp = CNSSOperator::instance().SendSyncCmd(pCmd);
    CHECK_POINTER(pCmdRsp, IVS_NET_RECV_TIMEOUT);
	(void)CNSSOperator::instance().ParseCmd2XML(pCmdRsp,iRet);
    HW_DELETE(pCmdRsp);
	return iRet;
}//lint !e1762

// ����ҵ����֤����
IVS_INT32 CLoginMgr::SendGeneralAuthCmd(int iNetElementType, const string& strLinkId, CXml& xml, const std::string& strTransID) const
{
    IVS_DEBUG_TRACE("iNetElementType:%d, strLinkId:%s, strTransID:%s", iNetElementType, strLinkId.c_str(), strTransID.c_str());
	CHECK_POINTER(m_pUserMgr, IVS_OPERATE_MEMORY_ERROR);
    // ������������
    if (strLinkId.empty())
    {
        BP_RUN_LOG_ERR(IVS_PARA_INVALID, "Send General AuthCmd failed", "strLinkId is empty");
        return IVS_PARA_INVALID;
    }
    int iRet = IVS_FAIL;
    unsigned int iLen = 0;
    const char* pszData = xml.GetXMLStream(iLen);
    CHECK_POINTER(pszData, IVS_OPERATE_MEMORY_ERROR);
    CCmd *pCmd = NULL;
    // ����SMUģ���������ϢΪNSS_CONNECT_SERVER_REQ������SCU��OMUģ���������ϢNSS_LOGIN_REQ
    unsigned short iReqID = NSS_CONNECT_SERVER_REQ;
    switch (iNetElementType)
    {
        case NET_ELE_SMU_NSS :
            iReqID = NSS_CONNECT_SERVER_REQ;
            break;
        case NET_ELE_SCU_NSS :
            iReqID = NSS_LOGIN_REQ;
            break;
        case NET_ELE_OMU_NSS :
            iReqID = NSS_LOGIN_REQ;
            break;
        default:
            break;
    }
    // ���췢��CMD����
    pCmd = CNSSOperator::instance().BuildCmd(strLinkId, iNetElementType, iReqID, pszData, strTransID);
    if (NULL == pCmd)
    {
        BP_RUN_LOG_ERR(IVS_PARA_INVALID, "Send General AuthCmd failed", "Build Cmd error iNetElementType = %d", iNetElementType);
        return IVS_PARA_INVALID;
    }
    // ����nss��Ϣ
    CCmd *pCmdRsp = CNSSOperator::instance().SendSyncCmd(pCmd);
    if (NULL == pCmdRsp)
    {
        BP_RUN_LOG_ERR(IVS_NET_RECV_TIMEOUT, "Send General AuthCmd failed", "msg no response");
        return IVS_NET_RECV_TIMEOUT;
    }
    // ������ӦCMD����ȡ������
    iRet = CNSSOperator::instance().ParseCmd2NSS(pCmdRsp);
    HW_DELETE(pCmdRsp);
    return iRet;
}

// ��OMU����TCP������,��¼��OMU
IVS_INT32 CLoginMgr::LoginOmu()
{
    IVS_DEBUG_TRACE("");
    CHECK_POINTER(m_pUserMgr,IVS_OPERATE_MEMORY_ERROR);
    int iSessionID = m_pUserMgr->GetSessionID();
     
    //��½�ɹ��������Ӧ��Ϣ����OMUport
    ST_SRU_LOGIN_RSP stSRULoginRsp;
    memset(&stSRULoginRsp, 0, sizeof(stSRULoginRsp));
    (void)m_userInfo.GetLoginRspInfo(stSRULoginRsp);
    int iRet = IVS_FAIL;
    std::string strLocalDomainCode;
    m_pUserMgr->GetDomainCode(strLocalDomainCode);
    // ���ڽ���TCP����
    string strLinkID;
	int iLinkRet = CNSSOperator::instance().CreateSyncClientLink(NET_ELE_OMU_NSS, m_userInfo.GetServerIP().c_str(), stSRULoginRsp.iOmuPort, strLinkID);
    CLinkInfo oLinkIDInfoTemp;
    oLinkIDInfoTemp.Init(NET_ELE_OMU_NSS, LINKID_STATUS_OPENED, strLinkID, strLocalDomainCode, m_userInfo.GetServerIP().c_str(),"");
    if (IVS_SUCCEED != iLinkRet)
    {
        BP_RUN_LOG_ERR(IVS_NET_CONNECT_ERROR, "create client omuLink ", "connect to OMU fail ");
        return IVS_NET_CONNECT_ERROR;
    }
    BP_RUN_LOG_INF("create client omuLink ", "ok", "linkid [%s] ", strLinkID.c_str());
    CNSSOperator::instance().SetOMULinkID(strLinkID);//lint !e1036
	(void)CLinkRouteMgr::instance().UpdateLinkInfo(iSessionID, oLinkIDInfoTemp);

    /************************************************************************
    ** ��Կ�������̣���ʼ��->��������ID->����Կ->��Կ����(���ɵ�����ID)->
    **              ��������->ʹ����Կ�������ɵ�����ID����Cmd->������Ϣ                                                                   
    ************************************************************************/
    // ��ʼ����Ϣ��ȫ�ͻ��˶���

    do
    {
        CSDKSecurityClient oSecurityClient;
        iRet = oSecurityClient.Init(NET_ELE_OMU_NSS, strLinkID);
        if (IVS_SUCCEED != iRet)
        {
            BP_RUN_LOG_ERR(IVS_FAIL, "Login Omu", "oSecurityClient Init failed");
            break;
        }
        // ��ȡ��½LoginID
        char* pLoginID = GetLoginId();
        CHECK_POINTER(pLoginID, IVS_OPERATE_MEMORY_ERROR);
        char cLoginID[LOGIN_ID_LEN + 1] = {0};
        strncpy(cLoginID, pLoginID, LOGIN_ID_LEN);
        // ����LoginID
        std::string strCipherLoignID;
        iRet = oSecurityClient.EncryptString(cLoginID, strCipherLoignID);
        if (IVS_SUCCEED != iRet)
        {
            BP_RUN_LOG_ERR(IVS_FAIL, "Login Omu", "oSecurityClient Init failed");
            break;
        }
        // ƴװxml
        CXml xml;
        iRet = CLoginXMLProcess::GetLoginOmuXML(strCipherLoignID, xml);
        if (IVS_SUCCEED != iRet)
        {
            BP_RUN_LOG_ERR(iRet, "Login Omu", "iRet=%d", iRet);
            break;
        }
        // ����ͨ��ҵ��������ϢXML����ΪContent����LoginID
        std::string strTransID = oSecurityClient.GetSecurityDataTransID();
        iRet = SendGeneralAuthCmd(NET_ELE_OMU_NSS, strLinkID, xml, strTransID);
        if (IVS_SUCCEED != iRet)
        {

            BP_RUN_LOG_ERR(iRet, "Login Omu", "LoginOmu fail, iRet = %d", iRet);
            break;
        }
        return IVS_SUCCEED;
    } while(false); //lint !e717
    CLinkRouteMgr::instance().RemoveLinkInfo(iSessionID, strLinkID);
    return iRet;
}//lint !e818

void CLoginMgr::GetInitData()
{
    IVS_DEBUG_TRACE("");
	CHECK_POINTER_VOID(m_pUserMgr);
	// ��ȡ��·�ɣ���¼��־
	CDomainRouteMgr& domainRouteMgr = m_pUserMgr->GetDomainRouteMgr();

    if (IVS_SUCCEED != domainRouteMgr.GetDomainRoute())
    {
        BP_RUN_LOG_ERR(IVS_FAIL, "Get Domain Route", "Get Domain Route failed");
        return;
    }
    // ����UserMgr��Ա �������
    string strLocalDomainCode;
    if (IVS_SUCCEED == domainRouteMgr.GetLocalDomainCode(strLocalDomainCode))
    {
		int iRet = IVS_FAIL;
        m_pUserMgr->SetDomainCode(strLocalDomainCode); // lint !e526 !e628
        // ����linkRouteMgr�еı���Code
        CLinkRouteMgr::instance().UpdateLocalDomainCode(m_pUserMgr->GetSessionID(), strLocalDomainCode);
        CDeviceMgr& deviceMgr = m_pUserMgr->GetDeviceMgr();

		iRet = deviceMgr.GetAllNVRInfo(strLocalDomainCode.c_str(), 0); //0��NVRType����ʾ��ѯ���С�
        if (IVS_SUCCEED != iRet)
        {
            BP_RUN_LOG_ERR(iRet, "Get AllNVR Info", "iRet = %d", iRet);
        }

		// ��ȡ������б�,��������ݺͿռ����Ϊ�˳�����ѯ�߳�
// 		IVS_INDEX_RANGE indexRange;
// 		indexRange.uiFromIndex = 1;
// 		indexRange.uiToIndex = 2;
// 
// 		IVS_UINT32 uiBufferSize = sizeof(IVS_CAMERA_BRIEF_INFO_LIST) + sizeof(IVS_CAMERA_BRIEF_INFO);
// 		IVS_CAMERA_BRIEF_INFO_LIST* pCameraList = (IVS_CAMERA_BRIEF_INFO_LIST*)IVS_NEW((IVS_CHAR* &)pCameraList, uiBufferSize);//lint !e826
// 		iRet = deviceMgr.GetCameras(&indexRange, pCameraList, uiBufferSize);
// 		if (IVS_SUCCEED != iRet)
// 		{
// 			BP_RUN_LOG_ERR(iRet, "GetCameras", "iRet = %d", iRet);
// 		}
// 		// ֱ��ɾ��
// 		IVS_DELETE(pCameraList, MUILI);
		return;
    }
    BP_RUN_LOG_INF("Get LocalDomainCode failed��", "DomainList Data error");
}

// ����
IVS_INT32 CLoginMgr::ReLoginSMU(const std::string& strIP, int iPort) 
{
    IVS_DEBUG_TRACE("relogin smu, target ip:[%s:%u].", strIP.c_str(), iPort);
    CHECK_POINTER(m_pUserMgr,IVS_OPERATE_MEMORY_ERROR);
    // int iSessionID = m_pUserMgr->GetSessionID();

    IVS_DOMAIN_ROUTE stDomainRoute;
    memset(&stDomainRoute, 0, sizeof(stDomainRoute));
    IVS_INT32 iRet = m_pUserMgr->GetDomainRouteMgr().GetDomainRouteByIP(strIP, stDomainRoute);
    if (IVS_SUCCEED != iRet)
    {
        return IVS_FAIL;
    }
    char cDomainCode[IVS_DOMAIN_CODE_LEN + 1] = {0};
    strncpy(cDomainCode, stDomainRoute.cDomainCode, IVS_DOMAIN_CODE_LEN);

    std::string strLoginServerIP = m_pUserMgr->GetLoginMgr().GetUserInfo()->GetServerIP();

    std::string strLinkID;
    
    if (0 == strLoginServerIP.compare(strIP))
    {
        // ����ǵ�ǰ��ͨ����ǰ���¼�ӿڽ��е�¼
        m_pUserMgr->UpdateResumeStartTime(::time(NULL));
        iRet = ReLogin(strLinkID);
    }
    else
    {
        // �ǵ�ǰ����Ҫ�����¼�����ͨ�õ�¼�ӿڵ�¼
        iRet = GeneralLoginSmu(cDomainCode, strLinkID); 
    }

    if (IVS_SUCCEED == iRet || IVS_SMU_USER_FIRST_LOGIN == iRet)
    {
        // �ɹ���½������RoutLink״̬ΪOpen
        CLinkInfo oLinkIdInfo;
        bool bRet = CLinkRouteMgr::instance().GetSmuLinkIDInfoByDomainCode(m_pUserMgr->GetSessionID(), cDomainCode, oLinkIdInfo);// �˴����ò���LinkRouteMgr�ĺ�����

        if (bRet)
        {
            oLinkIdInfo.SetStrLinkId(strLinkID);
            oLinkIdInfo.SetLinkStatus(LINKID_STATUS_OPENED);
            CLinkRouteMgr::instance().UpdateLinkInfo(m_pUserMgr->GetSessionID(), oLinkIdInfo);
            
        }
    }

    return iRet;
}//lint !e715

// ����SMU
IVS_INT32 CLoginMgr::ReLogin(std::string& strLinkID)
{
    IVS_DEBUG_TRACE("");
    CHECK_POINTER(m_pUserMgr,IVS_OPERATE_MEMORY_ERROR);
    int resultValue = IVS_FAIL;
    int iSessionID = m_pUserMgr->GetSessionID();
    
    // ��������
    // string strLinkID;
    resultValue = CNSSOperator::instance().CreateSyncClientLink(NET_ELE_SMU_NSS, m_userInfo.GetServerIP().c_str(), m_userInfo.GetServerPort(), strLinkID);
    if (IVS_SUCCEED != resultValue)
    {
        BP_RUN_LOG_ERR(IVS_NET_CONNECT_ERROR, "Create client smuLink failed", "failed");
        return IVS_NET_CONNECT_ERROR;
    }
    // �������Ӷ��� 
    std::string strLocalDomainCode;
    m_pUserMgr->GetDomainCode(strLocalDomainCode);
    std::string strLoginID;
    // ������������,�ɹ�ֱ�Ӹ���loginID
    resultValue = SendReConnectCmd(strLinkID, strLoginID);
    if (IVS_SUCCEED == resultValue)
    {
        RefreshLoginInfo(strLoginID, iSessionID, strLocalDomainCode, strLinkID);//lint !e1036

         //GetInitData();

        // �ӽ��̵�¼����Ҫ��¼OMU
        if (CLIENT_PC_CHILD == m_userInfo.GetClientType() 
            || CLIENT_TV_WALL == m_userInfo.GetClientType())
        {
            return resultValue;
        }
        //��OMU����tcp������,����loginId��Ȩ
//         long lResult = (long)VOS_CreateThread((VOS_THREAD_FUNC)LoginOmuThreadEntry, (void*)this, &m_pLoginOMUThread, VOS_DEFAULT_STACK_SIZE);
//         if (IVS_SUCCEED != lResult)
//         {
//             BP_RUN_LOG_ERR(IVS_FAIL, "Login Omu", "VOS_CreateThread to ReLoginOmu failed");
//         }
        return resultValue;
    }

    // ʧ�����·��͵�¼����,�ɹ�ֱ�Ӹ���LoginID
    if (IVS_SMU_HAS_RESTARED == resultValue) //lint !e774
    {
		//add by zwx211831, Date:20140707,����ǵ����¼����ʧ�ܾͲ���Ҫʹ���û����������¼
		if (SSO_LOGIN == m_enLoginType)
		{
			(void)CNetFactory::Instance().DeleteClientLink(strLinkID, NET_ELE_SMU_NSS);
			return resultValue;
		}
		//end
        resultValue = SendLoginCmd(strLinkID, strLoginID);
    }
    
    if (IVS_SUCCEED == resultValue || IVS_SMU_USER_PWD_NEED_MOD == resultValue)
    {
        RefreshLoginInfo(strLoginID, iSessionID, strLocalDomainCode, strLinkID);//lint !e1036
        // �ӽ��̵�¼����Ҫ��¼OMU
        if (CLIENT_PC_CHILD == m_userInfo.GetClientType() 
            || CLIENT_TV_WALL == m_userInfo.GetClientType())
        {
            return resultValue;
        }
        //��OMU����tcp������,����loginId��Ȩ
//         long lResult = (long)VOS_CreateThread((VOS_THREAD_FUNC)LoginOmuThreadEntry, (void*)this, &m_pLoginOMUThread, VOS_DEFAULT_STACK_SIZE);
//         if (IVS_SUCCEED != lResult)
//         {
//             BP_RUN_LOG_ERR(IVS_FAIL, "Login Omu", "VOS_CreateThread to ReLoginOmu failed");
//         }
    }
    // �ص�¼ʧ�ܣ�ɾ���շ����ص�¼���������
    else
    {
        (void)CNetFactory::Instance().DeleteClientLink(strLinkID, NET_ELE_SMU_NSS);
    }
    return resultValue;
}

// �����������󣬺�LoginID��xml����
IVS_INT32 CLoginMgr::SendReConnectCmd(const std::string &strLinkID, std::string &strLoginID)
{
    CSDKSecurityClient oSecurityClient;
    int iRet = IVS_FAIL;
    iRet = oSecurityClient.Init(NET_ELE_SMU_NSS, strLinkID);
    if (IVS_SUCCEED != iRet)
    {
        BP_RUN_LOG_ERR(IVS_FAIL, "ReLogin Smu Send ReConnectCmd", "oSecurityClient Init failed");
        return IVS_FAIL;
    }
    //ƴװ����xml
    CXml xml;
    char* pLoginID = m_userInfo.GetLoginId();
    if (NULL == pLoginID)
    {
        BP_RUN_LOG_ERR(IVS_OPERATE_MEMORY_ERROR, "ReLogin Smu Send ReConnectCmd", "LoginId is null");
        return IVS_OPERATE_MEMORY_ERROR;
    }
    char cLoginIDTemp[LOGIN_ID_LEN + 1] = {0};
    if (!CToolsHelp::Memcpy(cLoginIDTemp, LOGIN_ID_LEN, pLoginID, LOGIN_ID_LEN))
    {
        BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "Memcpy fail", "NA");
        return IVS_ALLOC_MEMORY_ERROR;
    }
    std::string strCipherLoginID;
    int iEncryptLoginIDRet = oSecurityClient.EncryptString(cLoginIDTemp, strCipherLoginID);
    if (IVS_SUCCEED != iEncryptLoginIDRet)
    {
        BP_RUN_LOG_ERR(IVS_FAIL, "ReLogin Smu Send ReConnectCmd", "oSecurityClient Init failed");
        return IVS_FAIL;
    }
    int resultValue = CLoginXMLProcess::GetLoginOmuXML(strCipherLoginID, xml);
    if (IVS_SUCCEED != resultValue)
    {
        BP_RUN_LOG_ERR(resultValue, "ReLogin Smu Send ReConnectCmd", "iRet = %d", resultValue);
        return resultValue;
    }
    unsigned int iLen = 0;
    const char* pszData = xml.GetXMLStream(iLen);
    CHECK_POINTER(pszData, IVS_FAIL);
    std::string strSecurityTransID  = oSecurityClient.GetSecurityDataTransID();
    CCmd *pCmd = CNSSOperator::instance().BuildCmd(strLinkID, NET_ELE_SMU_NSS, NSS_RECONNECT_SERVER_REQ, pszData, strSecurityTransID);
    if (NULL == pCmd)
    {
        BP_RUN_LOG_ERR(IVS_FAIL, "ReLogin Smu Send ReConnectCmd", "Build SMUCmd failed");
        return IVS_FAIL;
    }

    //����nssЭ��
    CCmd *pCmdRsp = CNSSOperator::instance().SendSyncCmd(pCmd);
    if(NULL == pCmdRsp)
    {
        BP_RUN_LOG_ERR(IVS_NET_RECV_TIMEOUT, "ReLogin Smu Send ReConnectCmd", "SendSyncCmd but no response or time is out");
        return IVS_NET_RECV_TIMEOUT;
    }
    //��Ӧ��xml��Ϣ��ʱ��ParseCmd2XML����NULL�����������ڴ棬����delete pRspXml
    char* pRspXml = CNSSOperator::instance().ParseCmd2XML(pCmdRsp,resultValue);
    if (NULL == pRspXml)
    {
        HW_DELETE(pCmdRsp);
        BP_RUN_LOG_ERR(resultValue, "ReLogin Smu Send ReConnectCmd", "ParseCmd2XML failed");
        return resultValue;
    }
    //��������
    CXml xmlRsp;
    (void)xmlRsp.Parse(pRspXml);
    resultValue = CLoginXMLProcess::ParseLoginID(xmlRsp, oSecurityClient, strLoginID);
    IVS_DELETE(pRspXml, MUILI);
    HW_DELETE(pCmdRsp);
    return resultValue;
}

// ���͵�¼���󣬺��û�������
IVS_INT32 CLoginMgr::SendLoginCmd(const std::string &strLinkID, std::string &strLoginID)
{
    CSDKSecurityClient oSecurityClient;
    int iRet = IVS_FAIL;
    iRet = oSecurityClient.Init(NET_ELE_SMU_NSS, strLinkID);
    if (IVS_SUCCEED != iRet)
    {
        BP_RUN_LOG_ERR(IVS_FAIL, "ReLogin Smu", "oSecurityClient Init failed");
        return IVS_FAIL;
    }
    //ƴװ����xml
    CXml xml;
    // �����û���������
    std::string strLoignCipherName;
    std::string strLoignCipherPWD;
    int iEncryptRet = oSecurityClient.EncryptString(m_userInfo.GetLoginName().c_str(), strLoignCipherName);
    iEncryptRet += oSecurityClient.EncryptString(m_userInfo.GetPWD().c_str(), strLoignCipherPWD);
    if (IVS_SUCCEED != iEncryptRet)
    {
        BP_RUN_LOG_ERR(IVS_FAIL, "Login Smu", "oSecurityClient Encrypt failed");
        return IVS_FAIL;
    }
    int resultValue = CLoginXMLProcess::GetReLoginXML(strLoignCipherName, strLoignCipherPWD, &m_userInfo, xml);
    if (IVS_SUCCEED != resultValue)
    {
        BP_RUN_LOG_ERR(resultValue, "ReLogin Smu Send LoginCmd", "Get ReLoginXML failed");
        return resultValue;
    }
    unsigned int iLen = 0;
    const char* pszData = xml.GetXMLStream(iLen);
    CHECK_POINTER(pszData, IVS_FAIL);
    std::string strSecurityTransID  = oSecurityClient.GetSecurityDataTransID();
    CCmd *pCmd = CNSSOperator::instance().BuildCmd(strLinkID, NET_ELE_SMU_NSS, NSS_LOGIN_REQ, pszData, strSecurityTransID);
    if (NULL == pCmd)
    {
        BP_RUN_LOG_ERR(IVS_FAIL, "ReLogin Smu Send LoginCmd", "Build SMUCmd failed");
        return IVS_FAIL;
    }
    //����nssЭ��
    CCmd *pCmdRsp = CNSSOperator::instance().SendSyncCmd(pCmd);
    if(NULL == pCmdRsp)
    {
        BP_RUN_LOG_ERR(IVS_NET_RECV_TIMEOUT, "ReLogin Smu Send LoginCmd", "SendSyncCmd but no response or time is out");
        return IVS_NET_RECV_TIMEOUT;
    }
    //��Ӧ��xml��Ϣ��ʱ��ParseCmd2XML����NULL�����������ڴ棬����delete pRspXml
    char* pRspXml = CNSSOperator::instance().ParseCmd2XML(pCmdRsp,resultValue);
    if (NULL == pRspXml)
    {
        HW_DELETE(pCmdRsp);
        BP_RUN_LOG_ERR(resultValue, "ReLogin Smu Send LoginCmd", "ParseCmd2XML failed");
        return resultValue;
    }
    //��������
    CXml xmlRsp;
    (void)xmlRsp.Parse(pRspXml);
    resultValue = CLoginXMLProcess::ParseLoginID(xmlRsp, oSecurityClient, strLoginID);
    IVS_DELETE(pRspXml, MUILI);
    HW_DELETE(pCmdRsp);
    return resultValue;
}

// ��¼�ɹ���ˢ��LinkID��loginID
void CLoginMgr::RefreshLoginInfo(const std::string strLoginID, int iSessionID, const std::string strLocalDomainCode, const std::string strLinkID)
{
    m_userInfo.SetLoginID(strLoginID);
    CLinkRouteMgr::instance().SetLinkIDByDomainCode(iSessionID, NET_ELE_SMU_NSS, strLocalDomainCode, strLinkID, LINKID_STATUS_OPENED);
    CNSSOperator::instance().SetSMULinkID(strLinkID); //lint !e1036
    // CLinkRouteMgr::instance().DeleteLinkExceptLocalDomain(iSessionID, NET_ELE_SMU_NSS, strLocalDomainCode);
}//lint !e1746

// ����OMU
IVS_INT32 CLoginMgr::ReLoginOmu(const std::string& strIP, int iPort)
{
    CHECK_POINTER(m_pUserMgr,IVS_FAIL);
    IVS_DEBUG_TRACE("");
    std::string strLocalDomainCode;
    m_pUserMgr->GetDomainCode(strLocalDomainCode);
    if (strLocalDomainCode.empty())
    {
        BP_RUN_LOG_ERR(IVS_FAIL, "Create client omuLink failed", "strLocalDomainCode is empty");
        return IVS_FAIL;
    }
    // ���ڽ���TCP����
    string strLinkID;
    int iCreatLinkRet = CNSSOperator::instance().CreateSyncClientLink(NET_ELE_OMU_NSS, strIP.c_str(), iPort, strLinkID);
    if (IVS_SUCCEED != iCreatLinkRet)
    {
        BP_RUN_LOG_ERR(IVS_NET_CONNECT_ERROR, "Create client omuLink failed", "failed");
        return IVS_NET_CONNECT_ERROR;
    }
    // ִ������OMU����
    int iReloginRet = IVS_FAIL;
    iReloginRet = SendReLoginOMUCmd(strLinkID);
    if (IVS_SUCCEED != iReloginRet)
    {
        BP_RUN_LOG_ERR(iReloginRet, "ReLogin Omu", "failed");
        (void)CNetFactory::Instance().DeleteClientLink(strLinkID, NET_ELE_OMU_NSS);
    }
    else
    {
        CLinkRouteMgr::instance().SetLinkIDByDomainCode(m_pUserMgr->GetSessionID(), NET_ELE_OMU_NSS, strLocalDomainCode, strLinkID, LINKID_STATUS_OPENED);
        CNSSOperator::instance().SetOMULinkID(strLinkID); //lint !e1036
    }
    return iReloginRet;
}

IVS_INT32 CLoginMgr::SendReLoginOMUCmd(const std::string& strLinkID)
{
    CSDKSecurityClient oSecurityClient;
    int iRet = IVS_FAIL;
    iRet = oSecurityClient.Init(NET_ELE_OMU_NSS, strLinkID);
    if (IVS_SUCCEED != iRet)
    {
        BP_RUN_LOG_ERR(IVS_FAIL, "ReLogin Omu Send ReConnectCmd", "oSecurityClient Init failed");
        return IVS_FAIL;
    }
    //ƴװ����xml
    CXml xml;
    char* pLoginID = m_userInfo.GetLoginId();
    if (NULL == pLoginID)
    {
        BP_RUN_LOG_ERR(IVS_OPERATE_MEMORY_ERROR, "ReLogin Omu Send ReConnectCmd", "LoginId is null");
        return IVS_OPERATE_MEMORY_ERROR;
    }
    char cLoginIDTemp[LOGIN_ID_LEN + 1] = {0};
    if (!CToolsHelp::Memcpy(cLoginIDTemp, LOGIN_ID_LEN, pLoginID, LOGIN_ID_LEN))
    {
        BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "Memcpy fail", "NA");
        return IVS_ALLOC_MEMORY_ERROR;
    }
    std::string strCipherLoginID;
    iRet = oSecurityClient.EncryptString(cLoginIDTemp, strCipherLoginID);
    if (IVS_SUCCEED != iRet)
    {
        BP_RUN_LOG_ERR(IVS_FAIL, "ReLogin Smu Send ReConnectCmd", "oSecurityClient Init failed");
        return IVS_FAIL;
    }
    int resultValue = CLoginXMLProcess::GetLoginOmuXML(strCipherLoginID, xml);
    if (IVS_SUCCEED != resultValue)
    {
        BP_RUN_LOG_ERR(resultValue, "ReLogin Omu Send ReConnectCmd", "iRet = %d", resultValue);
        return resultValue;
    }
    unsigned int iLen = 0;
    const char* pszData = xml.GetXMLStream(iLen);
    CHECK_POINTER(pszData, IVS_FAIL);
    std::string strSecurityTransID  = oSecurityClient.GetSecurityDataTransID();
    CCmd *pCmd = CNSSOperator::instance().BuildCmd(strLinkID, NET_ELE_OMU_NSS, NSS_LOGIN_REQ, pszData, strSecurityTransID);
    if (NULL == pCmd)
    {
        BP_RUN_LOG_ERR(IVS_FAIL, "ReLogin Smu Send ReConnectCmd", "Build SMUCmd failed");
        return IVS_FAIL;
    }

    //����nssЭ��
    CCmd *pCmdRsp = CNSSOperator::instance().SendSyncCmd(pCmd);
    if(NULL == pCmdRsp)
    {
        BP_RUN_LOG_ERR(IVS_NET_RECV_TIMEOUT, "ReLogin Smu Send ReConnectCmd", "SendSyncCmd but no response or time is out");
        return IVS_NET_RECV_TIMEOUT;
    }
    //��Ӧ��xml��Ϣ��ʱ��ParseCmd2XML����NULL�����������ڴ棬����delete pRspXml
    int pRspRet = CNSSOperator::instance().ParseCmd2NSS(pCmdRsp);
    if (IVS_SUCCEED != pRspRet)
    {
        BP_RUN_LOG_ERR(pRspRet, "ReLogin Omu Send ReConnectCmd", "pRspRet = %d", pRspRet);
    }
    HW_DELETE(pCmdRsp);
    return pRspRet;
}
//lint +e438
//lint +e550
//lint +e838

