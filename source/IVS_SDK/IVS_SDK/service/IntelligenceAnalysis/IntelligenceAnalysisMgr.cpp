#include "UserMgr.h"
#include "bp_log.h"
#include "Cmd.h"
#include "NSSOperator.h"
#include "IntelligenceAnalysisMgr.h"
#include "IVS_Trace.h"

CIntelligenceAnalysisMgr::CIntelligenceAnalysisMgr(void)
    : m_pUserMgr(NULL)
{
    
}

CIntelligenceAnalysisMgr::~CIntelligenceAnalysisMgr(void)
{
    m_pUserMgr = NULL;
}

void CIntelligenceAnalysisMgr::SetUserMgr( CUserMgr *pUserMgr)
{
    m_pUserMgr = pUserMgr;
}
// ���ܷ���ͳһ͸���ӿ�
IVS_INT32 CIntelligenceAnalysisMgr::IntelligenceAnalysis(IVS_UINT32 uiInterfaceType, IVS_CHAR* pTransID, IVS_UINT32 uiTransIDLen, 
															const IVS_CHAR* pReqXml, IVS_CHAR** pRspXml)
{
	CHECK_POINTER(pRspXml, IVS_PARA_INVALID);
    CHECK_POINTER(m_pUserMgr, IVS_OPERATE_MEMORY_ERROR);
//	***********pReqXml����Ϊ��*********************
//  CHECK_POINTER(pReqXml, NULL);
//	***********pTransID����Ϊ��*********************
//  CHECK_POINTER(pTransID, NULL);
	IVS_DEBUG_TRACE("InterfaceType: %u", uiInterfaceType);

	CCmd* pCmd = NULL;
    // ��ȡ����SMU���ӣ�����״̬Ϊ�Ͽ�ʱ���������Ӵ���
    std::string strLinkID;
    IVS_INT32 iGetLinkRet = m_pUserMgr->GetLocalDomainLinkID(NET_ELE_SMU_NSS, strLinkID);
    if (IVS_SUCCEED != iGetLinkRet)
    {
        BP_RUN_LOG_ERR(iGetLinkRet, "Get LocalDomainLinkID failed", "NA");
        return iGetLinkRet;
    }
	if (NULL == pTransID) //ͨ�����ܷ�����Ϣ IVS_SDK_IA
	{
		pCmd = CNSSOperator::instance().BuildSMUCmd((NSS_MSG_TYPE_E)uiInterfaceType, pReqXml, strLinkID);
	} 
	else if ('\0' == pTransID[0]) // ��һ������¼��켣���ӣ���Ҫ�������ɵ�TransID
	{
		std::string strTransID = CMKTransID::Instance().GenerateTransID();
		if (!CToolsHelp::Strncpy(pTransID, uiTransIDLen, strTransID.c_str(), strlen(strTransID.c_str())))
		{
			BP_RUN_LOG_INF("Intelligence Analysis", "Get TransID %s", strTransID.c_str());
			BP_RUN_LOG_ERR(IVS_OPERATE_MEMORY_ERROR, "Intelligence Analysis", "Get TransID Failed");
			return IVS_OPERATE_MEMORY_ERROR;
		}
		BP_RUN_LOG_INF("Intelligence Analysis", "Get TransID %s", pTransID);
		//std::string strLinkID = CNSSOperator::instance().GetSMULinkID();
		pCmd = CNSSOperator::instance().BuildCmd(strLinkID, NET_ELE_SMU_NSS, (NSS_MSG_TYPE_E)uiInterfaceType, pReqXml, strTransID);
	} 
	else // ����TransIDʱ�Ը�TransID��������
	{
		std::string strTransID = pTransID;
		//std::string strLinkID = CNSSOperator::instance().GetSMULinkID();
		pCmd = CNSSOperator::instance().BuildCmd(strLinkID, NET_ELE_SMU_NSS, (NSS_MSG_TYPE_E)uiInterfaceType, pReqXml, strTransID);
	}
	CHECK_POINTER(pCmd, IVS_OPERATE_MEMORY_ERROR);

	// ͬ��������Ϣ
	CCmd *pCmdRsp = NULL;
	if (NSS_BA_ADD_VIDEO_SOURCE_REQ == uiInterfaceType)
	{
		BP_RUN_LOG_INF("NSS_BA_ADD_VIDEO_SOURCE_REQ","na");
		pCmdRsp = CNSSOperator::instance().SendSyncCmd(pCmd, 60, 1);
	}
	else
	{
		pCmdRsp = CNSSOperator::instance().SendSyncCmd(pCmd);
	}
	 
	CHECK_POINTER(pCmdRsp, IVS_NET_RECV_TIMEOUT);
	IVS_INT32 iRet = IVS_SUCCEED;
	*pRspXml = CNSSOperator::instance().ParseCmd2XML(pCmdRsp, iRet);
	HW_DELETE(pCmdRsp);

	return iRet;
}

