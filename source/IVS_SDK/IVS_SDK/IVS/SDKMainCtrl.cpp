#include "SDKMainCtrl.h"
#include "SDKMainService.h"
#include "vos.h"
#include "EventCallBackJob.h"
#include "LinkRouteMgr.h"
#include "hwsdk.h"
#include "ivs_xml.h"
#include "DeviceMgrXMLProcess.h"
#include "IVSCommon.h"
#include "AlarmMgrXMLProcess.h"
#include "LoginXMLProcess.h"
#include "SDKInterface.h"
#include "GenerateID.h"
#include "DeviceMgr.h"
#include "nss_mt.h"
#include "RealXMLProcess.h"

CSDKMainctrl::CSDKMainctrl()
{
	/*m_ModuleType = CToolsHelp::Int2Str(SUBSYS_SDK);
	m_NodeCode = CToolsHelp::Int2Str(NET_ELE_SDK_NSS);*/
}//lint !e1401


CSDKMainctrl::~CSDKMainctrl()
{
}

extern CSDKInterface *g_pNvs; 
//extern VOS_Sem* s_pGetSdpSem;
void CSDKMainctrl::OnLinkOpened(const std::string& strDevName, int iNetElementType, const std::string& strLinkID, const std::string& strIP, int iPort)
{

    // note by wanglei 00165153:2014.1.26 ���߼�δ�����ã���Ҫ��BPȷ��ԭ�� 


    // ��¼��Ԫ���ӱ����Ϣ
    BP_RUN_LOG_INF("net element status change notify.", "linkage opened, ip:[%s], port:[%d], element type:[%d], link id:[%s].", 
        strIP.c_str(), iPort, iNetElementType, strLinkID.c_str());

    //����linkId����sessionID������CUserMgr����
    int iSessionID = CLinkRouteMgr::instance().FindSessionIDByLinkID(strLinkID);
    if (SDK_SESSIONID_INIT == iSessionID)
    {
        BP_RUN_LOG_INF("can't find sessionID by strLinkID, strLinkID is:","%s",strLinkID.c_str());
        return;
    }
    if (NULL == g_pNvs)
    {
        BP_RUN_LOG_INF("link opened","g_pNvs is null");
        return;
    }
    CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
    if (NULL == pUserMgr)
    {
        BP_RUN_LOG_INF("link opened","pUserMgr is null");
        return;
    }

    CLinkRouteMgr::instance().SetLinkIDStatus(strLinkID, LINKID_STATUS_OPENED);
}

void CSDKMainctrl::OnLinkClosed(const std::string& strDevName, int iNetElementType, const std::string& strLinkID, const std::string& strIP, int iPort)
{

    // ��¼��Ԫ���ӱ����Ϣ
    BP_RUN_LOG_INF("net element status change notify.", "linkage closed, ip:[%s], port:[%d], element type:[%d], link id:[%s].", 
        strIP.c_str(), iPort, iNetElementType, strLinkID.c_str());


	// TODO �����Ȳ���
    //����linkId����sessionID����������״̬Ϊ��   
    CLinkRouteMgr::instance().SetLinkIDStatus(strLinkID, LINKID_STATUS_CLOSED);
    
	int iSessionID = CLinkRouteMgr::instance().FindSessionIDByLinkID(strLinkID);
	if (SDK_SESSIONID_INIT == iSessionID)
	{
		BP_RUN_LOG_INF("can't find sessionID by strLinkID, strLinkID is:","%s",strLinkID.c_str());
		return;
	}
	if (NULL == g_pNvs)
	{
		BP_RUN_LOG_INF("link opened","g_pNvs is nul");
		return;
	}
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	if (NULL == pUserMgr)
	{
		BP_RUN_LOG_INF("link opened","pUserMgr is null");
		return;
	}

	pUserMgr->AddLinkIdToBusinessResumeList(strLinkID,iNetElementType,strIP,iPort,TYPE_ADD_RELINK/*TYPE_ADD_CLOSECOUNT*/);
}

// ���������ı�
CSDKMainctrl::REQUEST_NAME CSDKMainctrl::m_reqNames[] =
{
	{ NSS_ALARM_NOTIFY_REQ,		                 &CSDKMainctrl::OnAlarmNotify},            //
	{ NSS_ALARM_STATUS_NOTIFY_REQ,               &CSDKMainctrl::OnAlarmStatusNotify},
	{ NSS_USER_OFFLINE_REQ,                      &CSDKMainctrl::OnUserOffLineNotify},
	{ NSS_DO_LINKAGE_ACTION_REQ,                 &CSDKMainctrl::DoLinkageAction},
    { NSS_STOP_LINKAGE_ACTION_REQ,               &CSDKMainctrl::StopLinkageAction},
	{ NSS_BA_LEARNING_SCHEDULE_REPORT_REQ,       &CSDKMainctrl::HandleMAUCmd},
	{ NSS_FR_REPORT_SCHEDULE_REQ,                &CSDKMainctrl::HandleMAUCmd},
	{ NSS_PUSH_ALARM_LOCUS_REQ,                  &CSDKMainctrl::HandleMAUCmd},
    { NSS_ADDPLAN_SCHEDULE_REPORT_REQ,           &CSDKMainctrl::HandleMAUCmd},
	{ NSS_OMU_ALARM_CU_REPORT_REQ,               &CSDKMainctrl::OnDevAlarmNotify},
	{ NSS_DISCOVERED_DEVICE_NOTIFY_REQ,          &CSDKMainctrl::OnDisCoveredDevNotify},
#ifdef WIN32
	{ NSS_NOTIF_MANUAL_RECORD_STATE_REQ,         &CSDKMainctrl::OnManualRecordStateNotify},
#endif
	{ NSS_MODE_CRUISE_RECORD_OVER_REQ,           &CSDKMainctrl::OnModeCruiseRecordOver},
    {NSS_GET_USER_CHANNEL_LIST_RSP,                    &CSDKMainctrl::HandleGetUserChannelListRsp},
	{NSS_SHUTDOWN_STREAM_NOTIFIY_REQ,			&CSDKMainctrl::OnShutdownStreamNotify}
}; 

//����REQ_ID��ȡ������ָ��
CSDKMainctrl::FnProcessCmd CSDKMainctrl::GetFnProcessCmd(uint32_t enReqID) const
{
	FnProcessCmd fn = NULL;
 	int iSize = sizeof(m_reqNames) / sizeof(REQUEST_NAME);
 
	for ( int i = 0; i < iSize; i++ )
	{
		if ( m_reqNames[i].reqID == enReqID )
		{
			fn = m_reqNames[i].fnProcessCmd;
			break;
		}
	}
	return fn;
}

int CSDKMainctrl::HandleCmd(CCmd* pCmd)
{
	int iRet = IVS_FAIL;
    if (NULL == pCmd)
    {
		BP_RUN_LOG_ERR(IVS_FAIL, "handle cmd","iRet = %d", iRet);
        return iRet;
    }

	if (LINK_STATUS_REPORT_CMD == pCmd->GetReqID())
	{
		this->LinkStatusReport(pCmd);
		return IVS_SUCCEED;
	}
	// ����ȫ���Ӷȴ˴����˷ַ��������Ҫ����������������m_reqNames�����������Ϣ���ͣ��ʹ���������
	FnProcessCmd fnProcessCmd = GetFnProcessCmd(pCmd->GetReqID());
	if (NULL == fnProcessCmd)
	{
		BP_RUN_LOG_ERR(IVS_FAIL, "no process fn","iRet = %d", iRet);
		return iRet;
	}
	return (this->*fnProcessCmd)(pCmd);
}

int CSDKMainctrl::HandleMAUCmd(CCmd* pCmd) const
{
	CHECK_POINTER(pCmd, IVS_OPERATE_MEMORY_ERROR);
	//ParseCmd2XML�Ѿ���utf8ת��ansi
	IVS_CHAR* pData = CSDKMainService::ProcessMAUCmd(pCmd);
	CHECK_POINTER(pData, IVS_OPERATE_MEMORY_ERROR);

	int iSessionID = CLinkRouteMgr::instance().FindSessionIDByLinkID(pCmd->GetNetLinkID());  //lint !e1013 !e1055 !e746 !e64
	if (SDK_SESSIONID_INIT == iSessionID)
	{
		BP_RUN_LOG_ERR(IVS_FAIL, "session id invalid","NA");
		IVS_DELETE(pData, MUILI);
		return IVS_FAIL;
	}
	// ���ݲ�ͬ��REQID��ȡ�¼�ID
	IVS_INT32 iEventType = 0;
	switch(pCmd->GetReqID())  //lint !e1055 !e1013 !e746
	{
	case NSS_BA_LEARNING_SCHEDULE_REPORT_REQ:
		iEventType = IVS_EVENT_IA_BA_LEARNING_SCHEDULE_REPORT;
		break;
	case NSS_FR_REPORT_SCHEDULE_REQ:
		iEventType = IVS_EVENT_IA_FR_SCHEDULE_REPORT;
		break;
	case NSS_PUSH_ALARM_LOCUS_REQ:
		iEventType = IVS_EVENT_IA_PUSH_ALARM_LOCUS;
		break;
    case NSS_ADDPLAN_SCHEDULE_REPORT_REQ:
        iEventType = IVS_EVENT_IA_ADDPLAN_SCHEDULE_REPORT;
        break;
	default:
		break;
	}
	CEventCallBackJob::instance().PostEvent(iSessionID, iEventType, pData, static_cast<IVS_INT32>(strlen(pData)) + 1);
    IVS_DELETE(pData, MUILI);
	return IVS_SUCCEED;
}

int CSDKMainctrl::OnAlarmNotify(CCmd* pCmd)const
{
	CHECK_POINTER(pCmd, IVS_OPERATE_MEMORY_ERROR);
	IVS_ALARM_NOTIFY* pstAlarmNotify = CSDKMainService::ProcessAlarmAlarmNotifyCmd(pCmd);
    CHECK_POINTER(pstAlarmNotify, IVS_FAIL);
	int iSessionID = CLinkRouteMgr::instance().FindSessionIDByLinkID(pCmd->GetNetLinkID());  //lint !e1013 !e1055 !e746 !e64
	if (SDK_SESSIONID_INIT == iSessionID)
	{
		BP_RUN_LOG_ERR(IVS_SDK_RET_INVALID_SESSION_ID, "session id invalid","NA");
		IVS_DELETE(pstAlarmNotify);
		return IVS_FAIL;
	}
	// ����cDevDomainCode
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
    if (NULL == pUserMgr)
    {
        BP_RUN_LOG_ERR(IVS_SDK_RET_INVALID_SESSION_ID, "pUserMgr is null","NA");
        IVS_DELETE(pstAlarmNotify);
        return IVS_FAIL;
    }
    char cDevDomainCodeTemp[IVS_DOMAIN_CODE_LEN + 1] = {0};
    if (!CToolsHelp::Memcpy(cDevDomainCodeTemp, IVS_DOMAIN_CODE_LEN, pstAlarmNotify->cDevDomainCode,IVS_DOMAIN_CODE_LEN))
    {
        BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "Memcpy fail", "NA");
        IVS_DELETE(pstAlarmNotify);
        return IVS_ALLOC_MEMORY_ERROR;
    }
    // cDevDomainCodeΪ����䱾�����
	std::string strDomainCode = cDevDomainCodeTemp;
	if (strDomainCode.empty())
	{
		pUserMgr->GetDomainCode(strDomainCode);
		if (!CToolsHelp::Memcpy(pstAlarmNotify->cDevDomainCode, IVS_DOMAIN_CODE_LEN, strDomainCode.c_str(), strDomainCode.size()))
		{
			BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "Memcpy fail", "NA");
            IVS_DELETE(pstAlarmNotify);
			return IVS_ALLOC_MEMORY_ERROR;
		}
	}
    // ����cAlarmInCode��ƴ���������
    char cAlarmInCodeTemp[IVS_ALARM_CODE_LEN + 1] = {0};
    if (!CToolsHelp::Memcpy(cAlarmInCodeTemp, IVS_ALARM_CODE_LEN, pstAlarmNotify->cAlarmInCode,IVS_ALARM_CODE_LEN))
    {
        BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "Memcpy fail", "NA");
        IVS_DELETE(pstAlarmNotify);
        return IVS_ALLOC_MEMORY_ERROR;
    }
	std::string strDevCode = cAlarmInCodeTemp;
    std::string strCode = cAlarmInCodeTemp;
	strCode.append("#").append(strDomainCode);
	memset(pstAlarmNotify->cAlarmInCode, 0, IVS_ALARM_CODE_LEN);
	if (!CToolsHelp::Memcpy(pstAlarmNotify->cAlarmInCode, IVS_ALARM_CODE_LEN,strCode.c_str(), strlen(strCode.c_str())))
	{
		BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "CToolsHelp::Memcpy fail", "NA");
        IVS_DELETE(pstAlarmNotify);
		return IVS_ALLOC_MEMORY_ERROR;
	}
	// �жϸ澯���ͣ��Ƿ��漰���澯������
    char cAlarmTypeTemp[IVS_ALARM_CODE_LEN + 1] = {0};
    if (!CToolsHelp::Memcpy(cAlarmTypeTemp, IVS_ALARM_CODE_LEN, pstAlarmNotify->cAlarmType,IVS_ALARM_CODE_LEN))
    {
        BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "Memcpy fail", "NA");
        IVS_DELETE(pstAlarmNotify);
        return IVS_ALLOC_MEMORY_ERROR;
    }
	std::string strAlarmType = cAlarmTypeTemp; 
	uint32_t pos = strAlarmType.find_last_of("_");
	std::string suffix = strAlarmType.substr(pos+1);
	if ((suffix.compare("ONLINE") == 0) || (suffix.compare("OFFLINE") == 0))
	{
		// ����״̬
		strCode = strCode.substr(0, strCode.rfind("#"));//pstAlarmNotify->cAlarmInCode;*/
		IVS_UINT32 uiAlarmStatus = pstAlarmNotify->uiAlarmStatus;
			
		CDeviceMgr& deviceMgr =  pUserMgr->GetDeviceMgr();
		std::string strNVRCode = pstAlarmNotify->cNvrCode;
		deviceMgr.ModifyDevStatusByDomainCode(strDomainCode, strDevCode, uiAlarmStatus, strNVRCode);
	}

	// ת��
	char cAlarmDesc[IVS_ALARM_DESCRIPTION_LEN + 1] = {0};
	(void)CToolsHelp::Memcpy(cAlarmDesc, IVS_ALARM_DESCRIPTION_LEN, pstAlarmNotify->cAlarmDesc,IVS_ALARM_DESCRIPTION_LEN);
	char* pAlarmDescAnsi = CToolsHelp::UTF8ToANSI(cAlarmDesc);
	if (NULL != pAlarmDescAnsi)
	{
		// �������
		memset(pstAlarmNotify->cAlarmDesc, 0, IVS_ALARM_DESCRIPTION_LEN);
		(void)CToolsHelp::Memcpy(pstAlarmNotify->cAlarmDesc, IVS_ALARM_DESCRIPTION_LEN, pAlarmDescAnsi,strlen(pAlarmDescAnsi));
		free(pAlarmDescAnsi);
		pAlarmDescAnsi = NULL;
	}
	
	char cAlarmInName[IVS_ALARM_NAME_LEN + 1] = {0};
	(void)CToolsHelp::Memcpy(cAlarmInName, IVS_ALARM_NAME_LEN, pstAlarmNotify->cAlarmInName,IVS_ALARM_NAME_LEN);
	char* pAlarmInName = CToolsHelp::UTF8ToANSI(cAlarmInName);
	if (NULL != pAlarmInName)
	{
		// �������
		memset(pstAlarmNotify->cAlarmInName, 0, IVS_ALARM_NAME_LEN);
		(void)CToolsHelp::Memcpy(pstAlarmNotify->cAlarmInName, IVS_ALARM_NAME_LEN, pAlarmInName,strlen(pAlarmInName));
		free(pAlarmInName);
		pAlarmInName = NULL;
	}

	char cAlarmLevelName[IVS_NAME_LEN + 1] = {0};
	(void)CToolsHelp::Memcpy(cAlarmLevelName, IVS_NAME_LEN, pstAlarmNotify->cAlarmLevelName,IVS_NAME_LEN);
	char* pAlarmLevelName = CToolsHelp::UTF8ToANSI(cAlarmLevelName);
	if (NULL != pAlarmLevelName)
	{
		// �������
		memset(pstAlarmNotify->cAlarmLevelName, 0, IVS_NAME_LEN);
		(void)CToolsHelp::Memcpy(pstAlarmNotify->cAlarmLevelName, IVS_NAME_LEN, pAlarmLevelName,strlen(pAlarmLevelName));
		free(pAlarmLevelName);
		pAlarmLevelName = NULL;
	}
	
	char cAlarmTypeName[IVS_ALARM_NAME_LEN + 1] = {0};
	(void)CToolsHelp::Memcpy(cAlarmTypeName, IVS_ALARM_NAME_LEN, pstAlarmNotify->cAlarmTypeName,IVS_ALARM_NAME_LEN);
	char* pAlarmTypeName = CToolsHelp::UTF8ToANSI(cAlarmTypeName);
	if (NULL != pAlarmTypeName)
	{
		// �������
		memset(pstAlarmNotify->cAlarmTypeName, 0, IVS_ALARM_NAME_LEN);
		(void)CToolsHelp::Memcpy(pstAlarmNotify->cAlarmTypeName, IVS_ALARM_NAME_LEN, pAlarmTypeName,strlen(pAlarmTypeName));
		free(pAlarmTypeName);
		pAlarmTypeName = NULL;
	}

	char cExtParam[IVS_ALARM_EX_PARAM_LEN + 1] = {0};
	(void)CToolsHelp::Memcpy(cExtParam, IVS_ALARM_EX_PARAM_LEN, pstAlarmNotify->cExtParam,IVS_ALARM_EX_PARAM_LEN);
	char* pExtParam = CToolsHelp::UTF8ToANSI(cExtParam);
	if (NULL != pExtParam)
	{
		// �������
		memset(pstAlarmNotify->cExtParam, 0, IVS_ALARM_EX_PARAM_LEN);
		(void)CToolsHelp::Memcpy(pstAlarmNotify->cExtParam, IVS_ALARM_EX_PARAM_LEN, pExtParam,strlen(pExtParam));
		free(pExtParam);
		pExtParam = NULL;
	}
	
	CEventCallBackJob::instance().PostEvent(iSessionID, IVS_EVENT_REPORT_ALARM, pstAlarmNotify, sizeof(IVS_ALARM_NOTIFY));
	IVS_DELETE(pstAlarmNotify);
	return IVS_SUCCEED;
}

int CSDKMainctrl::OnAlarmStatusNotify(CCmd* pCmd)const
{
	CHECK_POINTER(pCmd, IVS_OPERATE_MEMORY_ERROR);
	IVS_ALARM_STATUS_NOTIFY* pstAlarmStatusNotify = CSDKMainService::ProcessAlarmStatusNotify(pCmd);
    CHECK_POINTER(pstAlarmStatusNotify, IVS_OPERATE_MEMORY_ERROR);
	int iSessionID = CLinkRouteMgr::instance().FindSessionIDByLinkID(pCmd->GetNetLinkID()); //lint !e1013 !e64
	if (SDK_SESSIONID_INIT == iSessionID)
	{
		BP_RUN_LOG_ERR(IVS_SDK_RET_INVALID_SESSION_ID, "session id invalid","NA");
		IVS_DELETE(pstAlarmStatusNotify);
		return IVS_FAIL;
	}
	// ����DevDomainCode(��������Ϊ�գ���Ϊ��������룩
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
    if (NULL == pUserMgr)
    {
        BP_RUN_LOG_ERR(IVS_SDK_RET_INVALID_SESSION_ID, "session id invalid","NA");
        IVS_DELETE(pstAlarmStatusNotify);
        return IVS_FAIL;
    }
	char cDevDomainCodeTemp[IVS_DOMAIN_CODE_LEN + 1] = {0};
	if (!CToolsHelp::Memcpy(cDevDomainCodeTemp, IVS_DOMAIN_CODE_LEN, pstAlarmStatusNotify->cDevDomainCode,IVS_DOMAIN_CODE_LEN))
	{
		BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "Memcpy fail", "NA");
        IVS_DELETE(pstAlarmStatusNotify);
		return IVS_ALLOC_MEMORY_ERROR;
	}
	std::string strDomainCode = cDevDomainCodeTemp;
	if (strDomainCode.empty())
	{
		pUserMgr->GetDomainCode(strDomainCode);
        if (!CToolsHelp::Memcpy(pstAlarmStatusNotify->cDevDomainCode, IVS_DOMAIN_CODE_LEN, strDomainCode.c_str(), strDomainCode.size()))
        {
            BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "Memcpy fail", "NA");
            IVS_DELETE(pstAlarmStatusNotify);
            return IVS_ALLOC_MEMORY_ERROR;
        }
    }
	// �������ƴ����AlarmInCode����
	char cAlarmInCodeTemp[IVS_ALARM_CODE_LEN + 1] = {0};
	if (!CToolsHelp::Memcpy(cAlarmInCodeTemp, IVS_ALARM_CODE_LEN, pstAlarmStatusNotify->cAlarmInCode,IVS_ALARM_CODE_LEN))
	{
		BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "Memcpy fail", "NA");
        IVS_DELETE(pstAlarmStatusNotify);
		return IVS_ALLOC_MEMORY_ERROR;
	}
	std::string strCode = cAlarmInCodeTemp;
	strCode.append("#").append(strDomainCode);
	memset(pstAlarmStatusNotify->cAlarmInCode, 0, IVS_ALARM_CODE_LEN);
	if (!CToolsHelp::Memcpy(pstAlarmStatusNotify->cAlarmInCode, IVS_ALARM_CODE_LEN,strCode.c_str(), strlen(strCode.c_str())))
	{
		BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "Memcpy fail", "NA");
        IVS_DELETE(pstAlarmStatusNotify);
		return IVS_ALLOC_MEMORY_ERROR;
	}

	// ת��utf8->ansi
	char cOperatorName[IVS_NAME_LEN + 1] = {0};
	(void)CToolsHelp::Memcpy(cOperatorName, IVS_NAME_LEN, pstAlarmStatusNotify->stOperateInfo.cOperatorName,IVS_NAME_LEN);
	char* pOperatorName = CToolsHelp::UTF8ToANSI(cOperatorName);
	if (NULL != pOperatorName)
	{
		// �������
		memset(pstAlarmStatusNotify->stOperateInfo.cOperatorName, 0, IVS_NAME_LEN);
		(void)CToolsHelp::Memcpy(pstAlarmStatusNotify->stOperateInfo.cOperatorName, IVS_NAME_LEN, pOperatorName,strlen(pOperatorName));
		free(pOperatorName);
		pOperatorName = NULL;
	}

	char cOperateInfo[IVS_ALARM_DESCRIPTION_LEN + 1] = {0};
	(void)CToolsHelp::Memcpy(cOperateInfo, IVS_ALARM_DESCRIPTION_LEN, pstAlarmStatusNotify->stOperateInfo.cOperateInfo,IVS_ALARM_DESCRIPTION_LEN);
	char* pOperateInfo = CToolsHelp::UTF8ToANSI(cOperateInfo);
	if (NULL != pOperateInfo)
	{
		// �������
		memset(pstAlarmStatusNotify->stOperateInfo.cOperateInfo, 0, IVS_ALARM_DESCRIPTION_LEN);
		(void)CToolsHelp::Memcpy(pstAlarmStatusNotify->stOperateInfo.cOperateInfo, IVS_ALARM_DESCRIPTION_LEN, pOperateInfo,strlen(pOperateInfo));
		free(pOperateInfo);
		pOperateInfo = NULL;
	}

	CEventCallBackJob::instance().PostEvent(iSessionID, IVS_EVENT_REPORT_ALARM_STATUS, pstAlarmStatusNotify, sizeof(IVS_ALARM_STATUS_NOTIFY));
	IVS_DELETE(pstAlarmStatusNotify);
	return IVS_SUCCEED;
}

int CSDKMainctrl::OnUserOffLineNotify(CCmd* pCmd)const
{
    // ��cmd�л�ȡxml
    CHECK_POINTER(pCmd, IVS_OPERATE_MEMORY_ERROR);

    IVS_INT32 iRet = IVS_FAIL;
    IVS_CHAR *pszInfo = CNSSOperator::instance().ParseCmd2XML(pCmd, iRet);
    CHECK_POINTER(pszInfo, IVS_OPERATE_MEMORY_ERROR);

    IVS_USER_OFFLINE_INFO pstUserOffline;
    memset(&pstUserOffline, 0, sizeof(IVS_USER_OFFLINE_INFO));

    IVS_CHAR cLoginID[LOGIN_ID_LEN + 1];
    memset(cLoginID, 0, LOGIN_ID_LEN+1);

    if (IVS_SUCCEED == iRet)
    {
        CXml xmlRsp;
        if (!xmlRsp.Parse(pszInfo))
        {
            BP_RUN_LOG_ERR(IVS_XML_INVALID, "UserOffLine Notify", "xml parsing error");
			HW_DELETE_A(pszInfo);
            return IVS_XML_INVALID;
        }
        iRet = CLoginXMLProcess::ParseUserOffLineXML(xmlRsp, cLoginID, &pstUserOffline);
    }
    if (IVS_SUCCEED != iRet)
    {
        BP_RUN_LOG_ERR(IVS_XML_INVALID, "UserOffLine Notify", "xml parsing error");
		HW_DELETE_A(pszInfo);
        return IVS_XML_INVALID;
    }
    //����loginID��ȡsessionID
    //IVS_INT32 iSessionID = g_pNvs->GetSessionIDByLoginID(cLoginID);
    IVS_INT32 iSessionID = CLinkRouteMgr::instance().FindSessionIDByLinkID(pCmd->GetNetLinkID()); //lint !e1013 !e64
    if (SDK_SESSIONID_INIT == iSessionID)
    {
        BP_RUN_LOG_ERR(IVS_SDK_RET_INVALID_SESSION_ID, "UserOffLine Notify", "iSessionID is -1");
		HW_DELETE_A(pszInfo);
        return IVS_SDK_RET_INVALID_SESSION_ID;
    }
    pstUserOffline.iSessionID = iSessionID;
    (void)CEventCallBackJob::instance().PostEvent(iSessionID, IVS_EVENT_USER_OFFLINE, &pstUserOffline, sizeof(pstUserOffline));
    HW_DELETE_A(pszInfo);
    
    //������Դ
    (void)g_pNvs->Logout(iSessionID, false);

    return IVS_SUCCEED;
}

uint32_t CSDKMainctrl::Routing(void) //lint !e1039 !e129
{//lint !e1039 !e745 !e18
	return 0;
}//lint !e1039

int CSDKMainctrl::Init()
{
	// ʹ��PIDȥ��ʼ���������ڶ�������ֻ��4λ����UUID���ɱ��ض� ��ͨ����ͬʱ�����и���һ����mod by z0019317
    CMKTransID::Instance().SetTransParam("SDK#", CToolsHelp::Int2Str((int)ACE_OS::getpid()));  
	if (IVS_SUCCEED != CMainCtrl::Init())
	{
		BP_RUN_LOG_ERR(IVS_FAIL, "Init failed","NA");
		return IVS_FAIL;
	}
	BP_RUN_LOG_INF("Init success","NA");
	if (IVS_SUCCEED != CMainCtrl::LoadNetService())
	{
		BP_RUN_LOG_ERR(IVS_FAIL, "LoadNetService failed","NA");
		return IVS_FAIL;
	}
	BP_RUN_LOG_INF("CMainCtrl::LoadNetService success","NA");
    if (IVS_SUCCEED != CMainCtrl::LoadBusinessService(false))
    {
		BP_RUN_LOG_ERR(IVS_FAIL, "LoadBusinessService failed","NA");
        return IVS_FAIL;
    }
	BP_RUN_LOG_INF("LoadBusinessService success","NA");

	return IVS_SUCCEED;
}

// void CSDKMainctrl::UnInit()
// {
// 
// }

int CSDKMainctrl::DoLinkageAction(CCmd* pCmd)const
{	
	CHECK_POINTER(pCmd, IVS_OPERATE_MEMORY_ERROR);
	// ת��utf8->ansi �Ѿ���parsexml������
	IVS_CHAR* pData = CSDKMainService::ProcessMAUCmd(pCmd);
	CHECK_POINTER(pData, IVS_OPERATE_MEMORY_ERROR);

	int iSessionID = CLinkRouteMgr::instance().FindSessionIDByLinkID(pCmd->GetNetLinkID());  //lint !e1013 !e1055 !e746 !e64
	if (SDK_SESSIONID_INIT == iSessionID)
	{
		BP_RUN_LOG_ERR(IVS_SDK_RET_INVALID_SESSION_ID, "linkage action", "iSessionID is -1");
		IVS_DELETE(pData, MUILI);
		return IVS_FAIL;
	}
	CXml xml;
	const IVS_CHAR* pDataXml = NULL;
	IVS_INT32 iRet = ModifyDevCode(xml, pData);
	
	if (IVS_SUCCEED != iRet)
	{
		IVS_DELETE(pData, MUILI);
		return iRet;
	}
	IVS_UINT32 uiXMLLen = 0;
	pDataXml = xml.GetXMLStream(uiXMLLen);
	IVS_DELETE(pData, MUILI);
	CHECK_POINTER(pDataXml, IVS_OPERATE_MEMORY_ERROR);
	CEventCallBackJob::instance().PostEvent(iSessionID, IVS_EVENT_DO_LINKAGE_ACTION, pDataXml, static_cast<IVS_INT32>(strlen(pDataXml)) + 1);
	return IVS_SUCCEED;
	
}

// ��������ֹ֪ͣͨ
int CSDKMainctrl::StopLinkageAction(CCmd* pCmd)const
{	
    // ��ȡ֪ͨ����
	CHECK_POINTER(pCmd, IVS_OPERATE_MEMORY_ERROR);
	IVS_CHAR* pData = CSDKMainService::ProcessMAUCmd(pCmd);
	CHECK_POINTER(pData, IVS_OPERATE_MEMORY_ERROR);

	int iSessionID = CLinkRouteMgr::instance().FindSessionIDByLinkID(pCmd->GetNetLinkID());  //lint !e1013 !e1055 !e746 !e64
	if (SDK_SESSIONID_INIT == iSessionID)
	{
		BP_RUN_LOG_ERR(IVS_SDK_RET_INVALID_SESSION_ID, "Stop linkage action", "iSessionID = %d", iSessionID);
		IVS_DELETE(pData, MUILI);
		return IVS_FAIL;
	}

	CXml xml;
	const IVS_CHAR* pDataXml = NULL;
	IVS_INT32 iRet = ModifyDevCode(xml, pData);

	if (IVS_SUCCEED != iRet)
	{
		IVS_DELETE(pData, MUILI);
		return iRet;
	}
	IVS_UINT32 uiXMLLen = 0;
	pDataXml = xml.GetXMLStream(uiXMLLen);

	IVS_DELETE(pData, MUILI);
	CHECK_POINTER(pDataXml, IVS_OPERATE_MEMORY_ERROR);

    CEventCallBackJob::instance().PostEvent(iSessionID, IVS_EVENT_STOP_LINKAGE_ACTION, pDataXml, static_cast<IVS_INT32>(strlen(pDataXml)) + 1);
    return IVS_SUCCEED;
}

//��XML�е��豸����ƴװ�����
int CSDKMainctrl::ModifyDevCode(CXml& xml, IVS_CHAR* pData)const
{
	CHECK_POINTER(pData, IVS_OPERATE_MEMORY_ERROR);

	if (!xml.Parse(pData))
	{
		BP_RUN_LOG_ERR(IVS_SMU_XML_INVALID, "xml.Parse(pData)", "NA");
		//IVS_DELETE(pData, MUILI);
		return IVS_SMU_XML_INVALID;
	}
	if (!xml.FindElemEx("Content/Action"))
	{
		BP_RUN_LOG_ERR(IVS_SMU_XML_INVALID, "xml.FindElem(Content/Action)", "NA");
		//IVS_DELETE(pData, MUILI);
		return IVS_SMU_XML_INVALID;		
	}
	// ��xml����DevList����Ҫ����DevCode��ƴ���豸���������DevDomainCode��
	if (xml.FindElem("DevList"))
	{
		xml.IntoElem();
		if (!xml.FindElem("DevInfo"))
		{
			BP_RUN_LOG_ERR(IVS_SMU_XML_INVALID, "xml.FindElem(DevInfo)", "NA");
			//IVS_DELETE(pData, MUILI);
			return IVS_SMU_XML_INVALID;		
		}
		do 
		{
			(void)xml.IntoElem();
			// �ҵ��豸���������ƴ��DevCode����
			if (!xml.FindElem("DevDomainCode"))
			{
				BP_RUN_LOG_ERR(IVS_SMU_XML_INVALID, "xml.FindElem(DevCode)", "NA");
				//IVS_DELETE(pData, MUILI);
				return IVS_SMU_XML_INVALID;		
			}
			std::string strDevDomainCode;
            const IVS_CHAR* pDevDomainCode = xml.GetElemValue();
            if (NULL != pDevDomainCode)
            {
                strDevDomainCode = pDevDomainCode;
            }
			// ƴ���豸���������
			if (!xml.FindElem("DevCode"))
			{
				BP_RUN_LOG_ERR(IVS_SMU_XML_INVALID, "xml.FindElem(DevCode)", "NA");
				//IVS_DELETE(pData, MUILI);
				return IVS_SMU_XML_INVALID;		
			}
			std::string strDevCode;
            
            const IVS_CHAR* pDevCode = xml.GetElemValue();
            if (NULL != pDevCode)
            {
                strDevCode = pDevCode;
            }

			if (!strDevCode.empty())
			{
				strDevCode.append("#").append(strDevDomainCode);
				xml.ModifyElemValue(strDevCode.c_str());
			}
			xml.OutOfElem();
		} while (xml.NextElem());
		xml.OutOfElem();
	}
// 
// 	IVS_UINT32 uiXmlLen = 0;
// 	pDataXml = xml.GetXMLStream(uiXmlLen);
// 	CHECK_POINTER(pDataXml, IVS_OPERATE_MEMORY_ERROR);
	return IVS_SUCCEED;
}//lint !e818

// ���ͷ��ֵ���ǰ���豸
int CSDKMainctrl::OnDisCoveredDevNotify(CCmd* pCmd)const
{
	BP_RUN_LOG_INF("OnDisCoveredDevNotify", "Begin");
	CHECK_POINTER(pCmd, IVS_OPERATE_MEMORY_ERROR);
	
	IVS_CHAR* pData = CSDKMainService::ProcessDevNotify(pCmd);
	CHECK_POINTER(pData, IVS_OPERATE_MEMORY_ERROR);

	int iSessionID = CLinkRouteMgr::instance().FindSessionIDByLinkID(pCmd->GetNetLinkID());  //lint !e1013 !e1055 !e746 !e64
	if (SDK_SESSIONID_INIT == iSessionID)
	{
		BP_RUN_LOG_ERR(IVS_SDK_RET_INVALID_SESSION_ID, "linkage action", "iSessionID is -1");
		IVS_DELETE(pData, MUILI);
		return IVS_FAIL;
	}
	CEventCallBackJob::instance().PostEvent(iSessionID, IVS_EVENT_FIND_DEVICE, pData, static_cast<IVS_INT32>(strlen(pData)) + 1);
    IVS_DELETE(pData, MUILI);
	BP_RUN_LOG_INF("OnDisCoveredDevNotify", "End");
	return IVS_SUCCEED;

}

int CSDKMainctrl::OnDevAlarmNotify(CCmd* pCmd)const
{
	BP_RUN_LOG_INF("OnDevAlarmNotify", "Begin");
	CHECK_POINTER(pCmd, IVS_OPERATE_MEMORY_ERROR);
	IVS_CHAR* pData = IVS_NEW(pData, sizeof(IVS_DEVICE_ALARM_NOTIFY));
    CHECK_POINTER(pData, IVS_OPERATE_MEMORY_ERROR);
	memset(pData, 0x0, sizeof(IVS_DEVICE_ALARM_NOTIFY));
	IVS_DEVICE_ALARM_NOTIFY* pDevAlarmNotify = reinterpret_cast<IVS_DEVICE_ALARM_NOTIFY*>(pData); //lint !e826
//     if (NULL == pDevAlarmNotify)
//     {
//         BP_RUN_LOG_ERR(IVS_SDK_RET_INTRINSIC_PTR_ERROR, "pDevAlarmNotify is null", "NA");
//         IVS_DELETE(pData, MUILI);
//         return IVS_SDK_RET_INTRINSIC_PTR_ERROR;
//     }
	IVS_INT32 iRet = CSDKMainService::ProcessDevAlarmNotify(pCmd, pDevAlarmNotify);
	
	// NSSͷ
	TNssMsgHeader m_header;
	memset(&m_header, 0, sizeof(TNssMsgHeader));
	m_header.usiProtocolVersion = 0x0100;
	m_header.usiMsgType = NSS_OMU_ALARM_CU_REPORT_RSP;
	m_header.uiSeqID = 0; 
	m_header.uiTraceID = 0;
	if (!CToolsHelp::Strncpy(m_header.szTransactionNo, TRANSACTIONNO_LEN, pCmd->GetTransID().c_str(), strlen(pCmd->GetTransID().c_str())))
	{
		BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "CToolsHelp::Strncpy(m_header.szTransactionNo, TRANSACTIONNO_LEN, pCmd->GetTransID().c_str(), 64)", "NA");
		IVS_DELETE(pData, MUILI);
		return IVS_ALLOC_MEMORY_ERROR;
	}
	m_header.uiRspCode = static_cast<IVS_UINT32>(iRet);  
	m_header.uiPacketLength = sizeof(TNssMsgHeader);

	// ת�������ֽ���
	m_header.usiProtocolVersion = ntohs(m_header.usiProtocolVersion);
	m_header.usiMsgType = ntohs(m_header.usiMsgType);
	m_header.uiSeqID = ntohl(m_header.uiSeqID);
	m_header.uiTraceID = ntohl(m_header.uiTraceID);
	m_header.uiRspCode = ntohl(m_header.uiRspCode);
	m_header.uiPacketLength = ntohl(m_header.uiPacketLength);

	// ����ACE����Ϣ����,�����cmd����ȥ�ͷţ����ﲻ�ͷ�
	ACE_Message_Block* pMsgBlock = new ACE_Message_Block(sizeof(TNssMsgHeader)); 

	pMsgBlock->copy((char*)&m_header, sizeof(TNssMsgHeader));
	CDispatcher::instance().dispatcherOutWithoutTransaction(pCmd->GetNetElemType(), pCmd->GetNetLinkID().c_str(), pCmd->GetTransID().c_str(), NSS_OMU_ALARM_CU_REPORT_RSP, pMsgBlock);
	if (IVS_SUCCEED == iRet)
	{
		int iSessionID = CLinkRouteMgr::instance().FindSessionIDByLinkID(pCmd->GetNetLinkID());  //lint !e1013 !e1055 !e746 !e64
		if (SDK_SESSIONID_INIT == iSessionID)
		{
			BP_RUN_LOG_ERR(IVS_FAIL, "session id invalid","NA");
			IVS_DELETE(pData, MUILI);
			return IVS_FAIL;
		}	

		// utf8->ansi
		char cAlarmDest[IVS_ALARM_DESCRIPTION_LEN + 1] = {0};
		(void)CToolsHelp::Memcpy(cAlarmDest, IVS_ALARM_DESCRIPTION_LEN, pDevAlarmNotify->cAlarmDest,IVS_ALARM_DESCRIPTION_LEN);
		char* pAlarmDest = CToolsHelp::UTF8ToANSI(cAlarmDest);
		if (NULL != pAlarmDest)
		{
			// �������
			memset(pDevAlarmNotify->cAlarmDest, 0, IVS_ALARM_DESCRIPTION_LEN);
			(void)CToolsHelp::Memcpy(pDevAlarmNotify->cAlarmDest, IVS_ALARM_DESCRIPTION_LEN, pAlarmDest,strlen(pAlarmDest));
			free(pAlarmDest);
			pAlarmDest = NULL;
		}

		char cAlarmInName[IVS_ALARM_NAME_LEN + 1] = {0};
		(void)CToolsHelp::Memcpy(cAlarmInName, IVS_ALARM_NAME_LEN, pDevAlarmNotify->cAlarmInName,IVS_ALARM_NAME_LEN);
		char* pAlarmInName = CToolsHelp::UTF8ToANSI(cAlarmInName);
		if (NULL != pAlarmInName)
		{
			// �������
			memset(pDevAlarmNotify->cAlarmInName, 0, IVS_ALARM_NAME_LEN);
			(void)CToolsHelp::Memcpy(pDevAlarmNotify->cAlarmInName, IVS_ALARM_NAME_LEN, pAlarmInName,strlen(pAlarmInName));
			free(pAlarmInName);
			pAlarmInName = NULL;
		}

		CEventCallBackJob::instance().PostEvent(iSessionID, IVS_EVENT_OMU_REPORT_ALARM, pDevAlarmNotify, sizeof(IVS_DEVICE_ALARM_NOTIFY));
	}
    IVS_DELETE(pData, MUILI);
	BP_RUN_LOG_INF("OnDevAlarmNotify", "End");
	return iRet;
}
#ifdef WIN32
int CSDKMainctrl::OnManualRecordStateNotify(CCmd* pCmd)const
{
	CHECK_POINTER(pCmd, IVS_OPERATE_MEMORY_ERROR);

	IVS_MANUAL_RECORD_STATUS_NOTIFY stManualRecordStatusNotify;
	memset(&stManualRecordStatusNotify, 0, sizeof(IVS_MANUAL_RECORD_STATUS_NOTIFY));
	IVS_INT32 iRet = CSDKMainService::ProcessManualRecordStateNotify(pCmd,stManualRecordStatusNotify);

	int iSessionID = CLinkRouteMgr::instance().FindSessionIDByLinkID(pCmd->GetNetLinkID()); //lint !e1013 !e64
	if (SDK_SESSIONID_INIT == iSessionID)
	{
		BP_RUN_LOG_ERR(IVS_FAIL, "session id invalid","NA");
		return IVS_SDK_RET_INVALID_SESSION_ID;
	}

	CEventCallBackJob::instance().PostEvent(iSessionID, IVS_EVENT_MANUAL_RECORD_STATE, &stManualRecordStatusNotify, sizeof(IVS_MANUAL_RECORD_STATUS_NOTIFY));
	iRet = IVS_SUCCEED;
	return iRet;
}
#endif
// ���豸���豸������Ϣ�첽��Ӧ����
int CSDKMainctrl::HandleGetUserChannelListRsp(CCmd* pCmd)const
{
    CHECK_POINTER(pCmd, IVS_OPERATE_MEMORY_ERROR);
    CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
    std::string strTransID = pCmd->GetTransID();
    IVS_CHANNLELIST_RETRIVE_STATUS_INFO channelListRetriveStatus;
    memset(&channelListRetriveStatus, 0, sizeof(channelListRetriveStatus));
    IVS_INT32 iRet = g_pNvs->GetChannelListRetriveStatusByTransID(strTransID, channelListRetriveStatus);
    if (IVS_SUCCEED != iRet)
    {
        BP_RUN_LOG_ERR(iRet, "handle get user channel list rsp", "fail to get channel list retrieve status by key word transID:%s.", strTransID.c_str());
        return iRet;
    }
  
    IVS_CHAR* pDataXML = CNSSOperator::instance().ParseCmd2XML(pCmd, iRet);   // ����pDataXMLΪ��
    channelListRetriveStatus.iLastRetCode = iRet;   // ��¼������
    //modify by zwx211831, Date:20140616,���ӶԻ�ȡ��UserMgrָ���пղ�������ֹ���û������ߡ�
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(channelListRetriveStatus.iSessionID);
	if (NULL == pUserMgr)
	{
		BP_RUN_LOG_ERR(IVS_SDK_RET_INVALID_SESSION_ID, "handle get user channel list rsp", "Invalid SessionID, SessionID is:%d.", channelListRetriveStatus.iSessionID);
		IVS_DELETE(pDataXML, MUILI);
		return IVS_SDK_RET_INVALID_SESSION_ID;
	}
	(void)pUserMgr->GetDeviceMgr().ProcessGetUserChannelListRsp(channelListRetriveStatus, pDataXML);
	//(void)g_pNvs->GetUserMgr(channelListRetriveStatus.iSessionID)->GetDeviceMgr().ProcessGetUserChannelListRsp(channelListRetriveStatus, pDataXML);
	//end
    g_pNvs->RemoveChannelListRetriveStatusByTransID(strTransID);
    IVS_DELETE(pDataXML, MUILI);
    return IVS_SUCCEED;
}

int CSDKMainctrl::OnModeCruiseRecordOver(CCmd* pCmd)const
{
   
	CHECK_POINTER(pCmd, IVS_OPERATE_MEMORY_ERROR);

	IVS_CHAR* pData = CSDKMainService::ProcessDevNotify(pCmd);
	CHECK_POINTER(pData, IVS_OPERATE_MEMORY_ERROR);

	int iSessionID = CLinkRouteMgr::instance().FindSessionIDByLinkID(pCmd->GetNetLinkID());  //lint !e1013 !e1055 !e746 !e64
	if (SDK_SESSIONID_INIT == iSessionID)
	{
		BP_RUN_LOG_ERR(IVS_SDK_RET_INVALID_SESSION_ID, "mode cruise record over", "iSessionID is -1");
		IVS_DELETE(pData, MUILI);
		return IVS_SDK_RET_INVALID_SESSION_ID;
	}
	CEventCallBackJob::instance().PostEvent(iSessionID, IVS_EVENT_MODE_CRUISE_RECORD_OVER, pData, static_cast<IVS_INT32>(strlen(pData)) + 1);
	return IVS_SUCCEED;
}


// add by zwx211831, Date:20140605 ����ͻ��˹ر���Ƶҵ��֪ͨ
int CSDKMainctrl::OnShutdownStreamNotify(CCmd* pCmd) const
{
	CHECK_POINTER(pCmd, IVS_OPERATE_MEMORY_ERROR);

	IVS_INT32 iRet = IVS_FAIL;
	IVS_CHAR *pInfo = CNSSOperator::instance().ParseCmd2XML(pCmd, iRet);
	CHECK_POINTER(pInfo, IVS_OPERATE_MEMORY_ERROR);

	IVS_SHUTDOWN_NOTIFY_INFO stShutdownNotifyInfo = {0};
	if (IVS_SUCCEED == iRet)
	{
		CXml xmlRsp;
		if (!xmlRsp.Parse(pInfo))
		{
			BP_RUN_LOG_ERR(IVS_XML_INVALID, "OnShutdownStreamNotify", "XML parse failed");
			HW_DELETE_A(pInfo);
			return IVS_XML_INVALID;
		}
		iRet = CRealXMLProcess::ParseShutdownNotifyXML(xmlRsp, &stShutdownNotifyInfo);
	}
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(IVS_XML_INVALID, "OnShutdownStreamNotify", "XML parse failed");
		HW_DELETE_A(pInfo);
		return IVS_XML_INVALID;
	}

	IVS_INT32 iSessionID = CLinkRouteMgr::instance().FindSessionIDByLinkID(pCmd->GetNetLinkID());
	if (SDK_SESSIONID_INIT == iSessionID)
	{
		BP_RUN_LOG_ERR(IVS_SDK_RET_INVALID_SESSION_ID, "OnShutdownStreamNotify", "iSessionID is -1");
		HW_DELETE_A(pInfo);
		return IVS_SDK_RET_INVALID_SESSION_ID;
	}
	(void)CEventCallBackJob::instance().PostEvent(iSessionID, IVS_EVENT_SHUTDOWN_STREAM, &stShutdownNotifyInfo, sizeof(stShutdownNotifyInfo));
	HW_DELETE_A(pInfo);

	return IVS_SUCCEED;
}
