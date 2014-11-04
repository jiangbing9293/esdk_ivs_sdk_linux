#include "UserMgr.h"
#include "AlarmMgr.h"
#include "nss_mt.h"
#include "bp_log.h"
#include "AlarmMgrXMLProcess.h"
#include "Cmd.h"
#include "NSSOperator.h"
#include "bp_environments.h"
#include "SDKDef.h"
#include "IVS_Trace.h"
#include "SDKSecurityClient.h"
CAlarmMgr::CAlarmMgr(void)
	:m_pUserMgr(NULL)
{
}
CAlarmMgr::~CAlarmMgr(void)
{
	m_pUserMgr = NULL;
}

void CAlarmMgr::SetUserMgr(CUserMgr *pUserMgr)
{
    m_pUserMgr = pUserMgr;
}


// �鿴�澯�����б�
IVS_INT32 CAlarmMgr::SubscribeQuery(const IVS_CHAR* pReqXml, IVS_CHAR** pRspXml) const
{
	CHECK_POINTER(pReqXml, IVS_PARA_INVALID);
	CHECK_POINTER(m_pUserMgr, IVS_OPERATE_MEMORY_ERROR);
	IVS_DEBUG_TRACE("");

	CXml xmlReq;
	std::string strDstDomainCode;
	IVS_INT32 iRet = CAlarmMgrXMLProcess::GetDstDomainCode(xmlReq, pReqXml, strDstDomainCode);
    if (strDstDomainCode.empty())
    {
        m_pUserMgr->GetDomainCode(strDstDomainCode);
    }
    // ���췢�ͽṹ��
    CSendNssMsgInfo sendNssMsgInfo;	
    sendNssMsgInfo.SetNeedXml(TYPE_NSS_XML);
    sendNssMsgInfo.SetNetElemType(NET_ELE_SMU_NSS);
    sendNssMsgInfo.SetReqID(NSS_SUBSCRIBE_QUERY_REQ);
    sendNssMsgInfo.SetReqData(pReqXml);	
    sendNssMsgInfo.SetDomainCode(strDstDomainCode);

    iRet = IVS_FAIL;
    std::string strpRsp;
    IVS_INT32 iNeedRedirect = IVS_FAIL;
    iRet = m_pUserMgr->SendCmd(sendNssMsgInfo,strpRsp,iNeedRedirect);

	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet, "Subscribe Query fail", "NA");
		return iRet;
	}
	// ������Ӧ��Ϣ���޸��б��е�ÿ��AlarmInCode
	CXml xmlRsq;
	if (!xmlRsq.Parse(strpRsp.c_str()))
	{
		BP_RUN_LOG_ERR(IVS_SMU_XML_INVALID, "pRspXml error", "NA");
		return IVS_SMU_XML_INVALID;
	}
	// SubscribeList��o�ֶΣ�����û�У����û�У������Ĳ���Ҳ�����ڣ�ֱ��return
	if (!xmlRsq.FindElemEx("Content/SubscribeList"))
	{
		// ����Ҳ���SubscribeList����Ϊ��Ӧ��ϢΪ�գ�û�鵽����
		BP_RUN_LOG_INF("no 'SubscribeList' Element ", "NA");
	}
	else if (xmlRsq.FindElem("SubscribeInfo"))
	{
		for (;;)
		{
			(void)xmlRsq.IntoElem();
            const char* pDevDoaminCode = NULL;
            if (xmlRsq.FindElem("DevDomainCode"))
            {
                pDevDoaminCode = xmlRsq.GetElemValue();
            }
            if (NULL == pDevDoaminCode)
            {
                pDevDoaminCode = strDstDomainCode.c_str();
            }
            if (!xmlRsq.FindElem("AlarmInCode"))
            {
                BP_RUN_LOG_ERR(IVS_SMU_XML_INVALID, "xmlRsq.FindElem(AlarmInCode) error", "NA");
                return IVS_SMU_XML_INVALID;
            }
			std::string strBigCode = xmlRsq.GetElemValue();
			if (!strBigCode.empty())
			{				
				strBigCode.append("#").append(pDevDoaminCode);
				xmlRsq.ModifyElemValue(strBigCode.c_str());
			}			
			xmlRsq.OutOfElem();
			if (!xmlRsq.NextElem())
			{
				break;
			}
		}
	}
	// AlarmInCode�޸�֮���ͷŵ�ԭ�ȴ����Ӧ��Ϣ���ڴ棬���¿����ڴ����޸�֮�����Ӧ��Ϣ���¿��ٵ��ڴ���ocx�ͷ�


    CHECK_POINTER(pRspXml, IVS_OPERATE_MEMORY_ERROR);

	IVS_UINT32 xmlLen = 0;
	const IVS_CHAR* pRsq = xmlRsq.GetXMLStream(xmlLen);
	IVS_DELETE(*pRspXml, MUILI);
	CHECK_POINTER(pRsq, IVS_OPERATE_MEMORY_ERROR);
	IVS_CHAR* pResult = IVS_NEW(pResult, strlen(pRsq) + 1);
    CHECK_POINTER(pResult, IVS_OPERATE_MEMORY_ERROR);
	
	memset(pResult, 0x0, strlen(pRsq) + 1);
	if (!CToolsHelp::Memcpy(pResult, strlen(pRsq) + 1, pRsq, strlen(pRsq) + 1))
	{
		BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "CToolsHelp::Memcpy pRsq to pResult failed", "NA");
		IVS_DELETE(pResult, MUILI);
		return IVS_ALLOC_MEMORY_ERROR;
	}
	*pRspXml = pResult;

	return iRet;
}

// �����澯����
IVS_INT32 CAlarmMgr::SubscribeAlarm(const IVS_CHAR* pReqXml) const
{
	CHECK_POINTER(pReqXml,IVS_PARA_INVALID);
	CHECK_POINTER(m_pUserMgr,IVS_OPERATE_MEMORY_ERROR);
	IVS_DEBUG_TRACE("");

	CXml xmlReq;
	std::string strDstDomainCode;
	IVS_INT32 iRet = CAlarmMgrXMLProcess::GetDstDomainCode(xmlReq, pReqXml, strDstDomainCode);
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet, "Get Dst DomainCode fail", "NA");
		return IVS_XML_INVALID;
	}
	if (strDstDomainCode.empty())
	{
		m_pUserMgr->GetDomainCode(strDstDomainCode);
	}
	// ����ɾ���ľ�������ӿڣ�ɾ���в������޸�AlarmInCode�����⣬������������ķ���ֵ�����ж�
	(void)CAlarmMgrXMLProcess::SetAlarmInCodeValue(xmlReq);
	IVS_UINT32 xmlLen = 0;
	const IVS_CHAR* pGetXml = xmlReq.GetXMLStream(xmlLen);
	CHECK_POINTER(pGetXml, IVS_OPERATE_MEMORY_ERROR);

    CSendNssMsgInfo sendNssMsgInfo;	
    sendNssMsgInfo.SetNeedXml(TYPE_NSS_NOXML);
    sendNssMsgInfo.SetNetElemType(NET_ELE_SMU_NSS);
    sendNssMsgInfo.SetReqID(NSS_SUBSCRIBE_ALARM_REQ);
    sendNssMsgInfo.SetReqData(pGetXml);	
    sendNssMsgInfo.SetDomainCode(strDstDomainCode);

    iRet = IVS_FAIL;
    std::string strpRsp;
    IVS_INT32 iNeedRedirect = IVS_FAIL;
    iRet = m_pUserMgr->SendCmd(sendNssMsgInfo,strpRsp,iNeedRedirect);
    if (IVS_SUCCEED != iRet)
    {
        BP_RUN_LOG_ERR(iRet, "Subscribe Alarm fail","NA");
    }
    return iRet;	
}

// ���ø澯�����б�
IVS_INT32 CAlarmMgr::SetAlarmTypeLevel(const IVS_ALARM_TYPE_LIST* pAlarmTypeList)
{
    CHECK_POINTER(pAlarmTypeList, IVS_OPERATE_MEMORY_ERROR);
    CHECK_POINTER(m_pUserMgr, IVS_OPERATE_MEMORY_ERROR);
	IVS_DEBUG_TRACE("");

    CXml xmlReq;
    IVS_UINT32 xmlLen = 0;

    //��װxml��׼�����͸�smu
    IVS_INT32 iRet = CAlarmMgrXMLProcess::SetAlarmTypeLevelGetXML(pAlarmTypeList, xmlReq);     
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet, "SetAlarmTypeLevelGetXML fail", "NA");
		return iRet;
	}

    const IVS_CHAR* pReq = xmlReq.GetXMLStream(xmlLen);
    CHECK_POINTER(pReq, IVS_OPERATE_MEMORY_ERROR);

    std::string strDomainCode;
    CSendNssMsgInfo sendNssMsgInfo;	
    sendNssMsgInfo.SetNeedXml(TYPE_NSS_XML);
	sendNssMsgInfo.SetNetElemType(NET_ELE_SMU_NSS);
	sendNssMsgInfo.SetReqID(NSS_SET_ALARM_LEVEL_REQ);
	sendNssMsgInfo.SetReqData(pReq);	
	sendNssMsgInfo.SetDomainCode(strDomainCode);

	iRet = IVS_FAIL;
	std::string strpRsp;
	IVS_INT32 iNeedRedirect = IVS_FAIL;
	iRet = m_pUserMgr->SendCmd(sendNssMsgInfo,strpRsp,iNeedRedirect);
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet, "Set Alarm Type Level fail","Send Cmd failed");
		return iRet;
	}
    return iRet;
}

// ��ѯ�Զ���澯�����б�
IVS_INT32 CAlarmMgr::GetAlarmTypeList(const IVS_QUERY_UNIFIED_FORMAT* pUnifiedQuery,
    IVS_ALARM_TYPE_LIST* pAlarmTypeList, IVS_UINT32 uiBufferSize)
{
    CHECK_POINTER(m_pUserMgr, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pUnifiedQuery, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pAlarmTypeList, IVS_OPERATE_MEMORY_ERROR);
	IVS_DEBUG_TRACE("");

    CXml xmlReq;
    IVS_UINT32 xmlLen = 0;
    //��װxml��׼�����͸�smu     
    IVS_INT32 iRet = CXmlProcess::GetUnifiedFormatQueryXML(const_cast<IVS_QUERY_UNIFIED_FORMAT*>(pUnifiedQuery), xmlReq);     
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet, "GetUnifiedFormatQueryXML fail", "NA");
		return iRet;
	}

    const IVS_CHAR* pReq = xmlReq.GetXMLStream(xmlLen);
    CHECK_POINTER(pReq, IVS_OPERATE_MEMORY_ERROR);
    //����nssЭ��ͷ
    // ��ȡ����SMU����
    std::string strSMULinkID;
    IVS_INT32 iGetLinkRet = m_pUserMgr->GetLocalDomainLinkID(NET_ELE_SMU_NSS, strSMULinkID);
    if (IVS_SUCCEED != iGetLinkRet)
    {
        BP_RUN_LOG_ERR(iGetLinkRet, "Get LocalDomainLinkID failed", "NA");
        return iGetLinkRet;
    }
    CCmd *pCmd = CNSSOperator::instance().BuildSMUCmd(NSS_GET_ALARM_TYPE_LIST_REQ, pReq,strSMULinkID);    
    CHECK_POINTER(pCmd, IVS_SDK_RET_INIT_RTSPCLIENT_ERROR);
    //����nssЭ�� 	
    CCmd *pCmdRsp = CNSSOperator::instance().SendSyncCmd(pCmd);
    CHECK_POINTER(pCmdRsp, IVS_NET_RECV_TIMEOUT);

	// ����ֵ
    IVS_CHAR *pRetResult = CNSSOperator::instance().ParseCmd2XML(pCmdRsp,iRet);
	HW_DELETE(pCmdRsp);
	if (IVS_SUCCEED != iRet)
	{
		IVS_DELETE(pRetResult,MUILI); 
		BP_RUN_LOG_ERR(iRet, "ParseCmd2XML fail", "NA");
		return iRet;
	}

    CHECK_POINTER(pRetResult, IVS_OPERATE_MEMORY_ERROR);

    CXml rspXML;
	if (!rspXML.Parse(pRetResult))
	{
		 BP_RUN_LOG_ERR(IVS_XML_PACK_ERROR, "parse xml failed","NA");
		 IVS_DELETE(pRetResult,MUILI); 
		 return IVS_XML_PACK_ERROR;
	}

    // ���õ���xmlת���ɽṹ�巵��
    iRet = CAlarmMgrXMLProcess::GetAlarmTypeListParseXML(pAlarmTypeList, rspXML, uiBufferSize);
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet, "Parse response xml fail", "NA");
		IVS_DELETE(pRetResult,MUILI); 
		return iRet;
	}
    IVS_DELETE(pRetResult,MUILI); 

    return iRet;
}

// �澯ȷ��
IVS_INT32 CAlarmMgr::ConfirmAlarm(IVS_UINT64 ullAlarmEventID, const IVS_CHAR* pAlarmInCode, const IVS_ALARM_OPERATE_INFO* pAlarmOperateInfo)
{
	CHECK_POINTER(pAlarmInCode, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pAlarmOperateInfo, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(m_pUserMgr, IVS_OPERATE_MEMORY_ERROR);
	IVS_DEBUG_TRACE("");
	// ���AlarmInCode����ȡ�����
	IVS_CHAR szAlarmInCode[IVS_ALARM_CODE_LEN + 1] = {0};
	IVS_CHAR szDomainCode[IVS_DOMAIN_CODE_LEN + 1] = {0};
	IVS_INT32 iRet = CXmlProcess::ParseDevCode(pAlarmInCode, szAlarmInCode, szDomainCode);
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet, "ParseDevCode fail", "NA");
		return iRet;
	}

	// ׼����������
	IVS_UINT32 iBufferSize = sizeof(IVS_UINT64) + IVS_ALARM_DESCRIPTION_LEN + IVS_TIME_LEN;
	IVS_CHAR* pData = IVS_NEW((IVS_CHAR* &)pData, iBufferSize);
	if (NULL == pData)
	{
		BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "new pData fail", "NA");
		return IVS_ALLOC_MEMORY_ERROR;
	}
	memset(pData, 0, iBufferSize);

	IVS_UINT64 uiID = 0;
	BP_htonll(ullAlarmEventID, uiID);
	if (!CToolsHelp::Memcpy(pData, iBufferSize, &uiID, sizeof(IVS_UINT64)))
	{
		BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "CToolsHelp::Memcpy uiID to pData failed", "NA");
		IVS_DELETE(pData, MUILI);
		return IVS_ALLOC_MEMORY_ERROR;
	}
    // ��pAlarmOperateInfo->cOperateInfoת����UTF-8��ʽ��������pData��
	IVS_CHAR szOperationInfo[IVS_ALARM_DESCRIPTION_LEN + 1] = {0};
	if (!CToolsHelp::Memcpy(szOperationInfo, IVS_ALARM_DESCRIPTION_LEN, pAlarmOperateInfo->cOperateInfo, IVS_ALARM_DESCRIPTION_LEN))
	{
		BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "CToolsHelp::Memcpy pAlarmOperateInfo->cOperateInfo to szOperationInfo failed", "NA");
		IVS_DELETE(pData, MUILI);
		return IVS_ALLOC_MEMORY_ERROR;
	}
	IVS_CHAR* pOperationInfo = CToolsHelp::ANSIToUTF8(szOperationInfo);
	if (NULL == pOperationInfo)
	{
		BP_RUN_LOG_INF("CAlarmMgr::ConfirmAlarm", "NULL == pOperationInfo");
		IVS_DELETE(pData, MUILI);
		return IVS_FAIL;
	}
	if (!CToolsHelp::Memcpy(pData + sizeof(IVS_UINT64), iBufferSize - sizeof(IVS_UINT64), pOperationInfo, strlen(pOperationInfo) + 1))
	{
		BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "CToolsHelp::Memcpy pOperationInfo to pData failed", "NA");
		IVS_DELETE(pData, MUILI);
		free(pOperationInfo);
		pOperationInfo = NULL;
		return IVS_ALLOC_MEMORY_ERROR;
	}
    // ��pAlarmOperateInfo->cOperateTimeת����UTF-8��ʽ��������pData��
	IVS_CHAR szOperationTime[IVS_TIME_LEN + 1] = {0};
	if (!CToolsHelp::Memcpy(szOperationTime, IVS_TIME_LEN, pAlarmOperateInfo->cOperateTime, IVS_TIME_LEN))
	{
		BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "CToolsHelp::Memcpy pAlarmOperateInfo->cOperateTime to szOperationTime failed", "NA");
		IVS_DELETE(pData, MUILI);
		free(pOperationInfo);
		pOperationInfo = NULL;		
		return IVS_ALLOC_MEMORY_ERROR;
	}
	IVS_CHAR* pOperationTime = CToolsHelp::ANSIToUTF8(szOperationTime);
	if (NULL == pOperationTime)
	{
		BP_RUN_LOG_INF("CAlarmMgr::ConfirmAlarm", "NULL == pOperationTime");
		free(pOperationInfo);
		pOperationInfo = NULL;
		IVS_DELETE(pData, MUILI);
		return IVS_FAIL;
	}
	if (!CToolsHelp::Memcpy(pData + sizeof(IVS_UINT64) + IVS_ALARM_DESCRIPTION_LEN, (iBufferSize - sizeof(IVS_UINT64)) - IVS_ALARM_DESCRIPTION_LEN, pOperationTime, strlen(pOperationTime) + 1))
	{
		BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "CToolsHelp::Memcpy pOperationTime to pData failed", "NA");
		free(pOperationInfo);
		pOperationInfo = NULL;
		free(pOperationTime);
		pOperationTime = NULL;
		IVS_DELETE(pData, MUILI);
		return IVS_ALLOC_MEMORY_ERROR;
	}
	
	CSendNssMsgInfo sendNssMsgInfo;	
	sendNssMsgInfo.SetNeedXml(TYPE_NSS_NOXML);
	sendNssMsgInfo.SetNetElemType(NET_ELE_SMU_NSS);
	sendNssMsgInfo.SetReqID(NSS_CONFIRM_ALARM_REQ);
	sendNssMsgInfo.SetReqDataPointer(pData);
	sendNssMsgInfo.SetReqLength(iBufferSize);
	sendNssMsgInfo.SetDomainCode(szDomainCode);

	iRet = IVS_FAIL;
	std::string strpRsp;
	IVS_INT32 iNeedRedirect = IVS_FAIL;
	iRet = m_pUserMgr->SendCmd(sendNssMsgInfo,strpRsp, iNeedRedirect);
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet, "Alarm Confirm fail","Send Cmd failed");
		free(pOperationInfo);
		pOperationInfo = NULL;
		free(pOperationTime);
		pOperationTime = NULL;
		IVS_DELETE(pData, MUILI);
		return iRet;
	}
	IVS_DELETE(pData, MUILI);
	free(pOperationInfo);
	pOperationInfo = NULL;
	free(pOperationTime);
	pOperationTime = NULL;
	return iRet;
}

//ˮӡ�澯ȷ��
IVS_INT32 CAlarmMgr::ConfirmWaterAlarm(const IVS_WATERMARK_ALARM_CONFIRM* pConfirmInfo)
{
	CHECK_POINTER(pConfirmInfo, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(m_pUserMgr,IVS_OPERATE_MEMORY_ERROR);
	IVS_INT32 iRet = IVS_FAIL;
	IVS_UINT64 uiID = 0;
	uiID = pConfirmInfo->ullAlarmEventID;
	std::string strCameraDomainCode = pConfirmInfo->cCameraCode;
	std::string strCameraCode;
	std::string strDomainCode;
	std::string strUserDomainCode;
	iRet = CToolsHelp::SplitCameraDomainCode(strCameraDomainCode.c_str(), strCameraCode, strDomainCode);
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet,"Confirm Water Alarm", "SplitCameraCode failed, RetCode=%d.", iRet);
		return iRet;
	}
	m_pUserMgr->GetDomainCode(strUserDomainCode);
	//��װxml��׼�����͸�smu
	CXml xmlReq;
	IVS_UINT32 xmlLen = 0;
	iRet = CAlarmMgrXMLProcess::GetConfirmWaterAlarmXML(uiID,pConfirmInfo->cConfirmInfo,pConfirmInfo->cConfirmTime,strUserDomainCode,strDomainCode, xmlReq);
	const IVS_CHAR* pReq = xmlReq.GetXMLStream(xmlLen);
	CHECK_POINTER(pReq, IVS_OPERATE_MEMORY_ERROR);

	CSendNssMsgInfo sendNssMsgInfo;
	sendNssMsgInfo.SetNeedXml(TYPE_NSS_XML);
	sendNssMsgInfo.SetNetElemType(NET_ELE_SMU_NSS);
	sendNssMsgInfo.SetReqID(NSS_WATERMARK_CONFIRM_ALARM_REQ);
	sendNssMsgInfo.SetReqData(pReq);	
	sendNssMsgInfo.SetDomainCode(strDomainCode);

	std::string strpRsp;
	IVS_INT32 iNeedRedirect = IVS_FAIL;
	iRet = m_pUserMgr->SendCmd(sendNssMsgInfo,strpRsp,iNeedRedirect);
	//if(IVS_SUCCEED != iRet)
	//{
	//	BP_RUN_LOG_ERR(iRet, "Get Water Alarm List fail","SendCmd to SMU Return Failed");
	//	return  iRet;
	//}


	////����Ҫ���͵�CMD��ƴװ��NSS��Ϣͷ
	//CCmd *pCmd = CNSSOperator::instance().BuildSMUCmd(NSS_WATERMARK_CONFIRM_ALARM_REQ, pReq); //lint !e713
	//CHECK_POINTER(pCmd, IVS_OPERATE_MEMORY_ERROR);

	////ͬ��������Ϣ����
	//CCmd *pCmdRsp = CNSSOperator::instance().SendSyncCmd(pCmd);
	//CHECK_POINTER(pCmdRsp, IVS_NET_RECV_TIMEOUT);
	//iRet = CNSSOperator::instance().ParseCmd2NSS(pCmdRsp);
	//HW_DELETE(pCmdRsp);
	return iRet;
}


// �澯����
IVS_INT32 CAlarmMgr::CancelAlarm(IVS_UINT64 ullAlarmEventID, const IVS_CHAR* pAlarmInCode, const IVS_ALARM_OPERATE_INFO* pAlarmOperateInfo)
{
	CHECK_POINTER(pAlarmOperateInfo, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(m_pUserMgr, IVS_OPERATE_MEMORY_ERROR);
	IVS_DEBUG_TRACE("");

	// ���AlarmInCode����ȡ�����
	IVS_CHAR szAlarmInCode[IVS_ALARM_CODE_LEN + 1] = {0};
	IVS_CHAR szDomainCode[IVS_DOMAIN_CODE_LEN + 1] = {0};
	IVS_INT32 iRet = CXmlProcess::ParseDevCode(pAlarmInCode, szAlarmInCode, szDomainCode);
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet, "ParseDevCode fail", "NA");
		return iRet;
	}
	// ׼����������
	IVS_UINT32 iBufferSize = sizeof(IVS_UINT64) + IVS_ALARM_DESCRIPTION_LEN + IVS_TIME_LEN;
	IVS_CHAR* pData = IVS_NEW((IVS_CHAR* &)pData, iBufferSize);
	if (NULL == pData)
	{
		BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "new pData fail", "NA");
		return IVS_ALLOC_MEMORY_ERROR;
	}
	memset(pData, 0, iBufferSize);
	
	IVS_UINT64 uiID = 0;
	BP_htonll(ullAlarmEventID, uiID);
	if (!CToolsHelp::Memcpy(pData, iBufferSize, &uiID, sizeof(IVS_UINT64)))
	{
		BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "CToolsHelp::Memcpy uiID to pData failed", "NA");
		IVS_DELETE(pData, MUILI);
		return IVS_ALLOC_MEMORY_ERROR;
	}
	// ��pAlarmOperateInfo->cOperateInfoת����UTF-8��ʽ��������pData��
	IVS_CHAR szOperationInfo[IVS_ALARM_DESCRIPTION_LEN + 1] = {0};
	if (!CToolsHelp::Memcpy(szOperationInfo, IVS_ALARM_DESCRIPTION_LEN, pAlarmOperateInfo->cOperateInfo, IVS_ALARM_DESCRIPTION_LEN))
	{
		BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "CToolsHelp::Memcpy pAlarmOperateInfo->cOperateInfo to szOperationInfo failed", "NA");
		IVS_DELETE(pData, MUILI);
		return IVS_ALLOC_MEMORY_ERROR;
	}
	IVS_CHAR* pOperationInfo = CToolsHelp::ANSIToUTF8(szOperationInfo);
	if (NULL == pOperationInfo)
	{
		IVS_DELETE(pData, MUILI);
		BP_RUN_LOG_ERR(IVS_FAIL, "OperationInfo ANSIToUTF8 fail", "NA");
		return IVS_FAIL;
	}
	if (!CToolsHelp::Memcpy(pData + sizeof(IVS_UINT64), iBufferSize - sizeof(IVS_UINT64), pOperationInfo, strlen(pOperationInfo) + 1))
	{
		BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "CToolsHelp::Memcpy pOperationInfo to pData failed", "NA");
		IVS_DELETE(pData, MUILI);
		free(pOperationInfo);
		pOperationInfo = NULL;
		return IVS_ALLOC_MEMORY_ERROR;
	}
	// ��pAlarmOperateInfo->cOperateTimeת����UTF-8��ʽ��������pData��
	IVS_CHAR szOperationTime[IVS_TIME_LEN + 1] = {0};
	if (!CToolsHelp::Memcpy(szOperationTime, IVS_TIME_LEN, pAlarmOperateInfo->cOperateTime, IVS_TIME_LEN))
	{
		BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "CToolsHelp::Memcpy pAlarmOperateInfo->cOperateTime to szOpetationInfo failed", "NA");
		IVS_DELETE(pData, MUILI);
		free(pOperationInfo);
		pOperationInfo = NULL;
		return IVS_ALLOC_MEMORY_ERROR;
	}
	IVS_CHAR* pOperationTime = CToolsHelp::ANSIToUTF8(szOperationTime);
	if (NULL == pOperationTime)
	{
		BP_RUN_LOG_ERR(IVS_FAIL, "OperationTime ANSIToUTF8 fail", "NA");
		free(pOperationInfo);
		pOperationInfo = NULL;
		IVS_DELETE(pData, MUILI);
		return IVS_FAIL;
	}
	if (!CToolsHelp::Memcpy(pData + sizeof(IVS_UINT64) + IVS_ALARM_DESCRIPTION_LEN, (iBufferSize - sizeof(IVS_UINT64)) - IVS_ALARM_DESCRIPTION_LEN, pOperationTime, strlen(pOperationTime) + 1))
	{
		BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "CToolsHelp::Memcpy pOperationTime to pData failed", "NA");
		free(pOperationInfo);
		pOperationInfo = NULL;
		free(pOperationTime);
		pOperationTime = NULL;
		IVS_DELETE(pData, MUILI);
		return IVS_ALLOC_MEMORY_ERROR;
	}
    CSendNssMsgInfo sendNssMsgInfo;	
    sendNssMsgInfo.SetNeedXml(TYPE_NSS_NOXML);
    sendNssMsgInfo.SetNetElemType(NET_ELE_SMU_NSS);
    sendNssMsgInfo.SetReqID(NSS_CANCEL_ALARM_REQ);
    sendNssMsgInfo.SetReqDataPointer(pData);
    sendNssMsgInfo.SetReqLength(iBufferSize);
	sendNssMsgInfo.SetDomainCode(szDomainCode);

    iRet = IVS_FAIL;
    std::string strpRsp;
	IVS_INT32 iNeedRedirect = IVS_FAIL;
    iRet = m_pUserMgr->SendCmd(sendNssMsgInfo,strpRsp, iNeedRedirect);
    if (IVS_SUCCEED != iRet)
    {
        BP_RUN_LOG_ERR(iRet, "Cancel Alarm","Send Cmd failed");
		IVS_DELETE(pData, MUILI);
		free(pOperationInfo);
		pOperationInfo = NULL;
		free(pOperationTime);
		pOperationTime = NULL;
        return iRet;
    }

	IVS_DELETE(pData, MUILI);
	free(pOperationInfo);
	pOperationInfo = NULL;
	free(pOperationTime);
	pOperationTime = NULL;
	return iRet;
}

//ˮӡ�澯����
IVS_INT32 CAlarmMgr::CancelWaterAlarm(const IVS_WATERMARK_ALARM_CANCEL* pstConfirmInfo)
{
	CHECK_POINTER(pstConfirmInfo, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(m_pUserMgr, IVS_OPERATE_MEMORY_ERROR);
	IVS_DEBUG_TRACE("");

	IVS_INT32 iRet = IVS_FAIL;
	IVS_UINT64 uiID = 0;
	uiID = pstConfirmInfo->ullAlarmEventID;

	std::string strCameraDomainCode = pstConfirmInfo->cCameraCode;
	std::string strCameraCode;
	std::string strDomainCode;
	std::string strUserDoaminCode;
	iRet = CToolsHelp::SplitCameraDomainCode(strCameraDomainCode.c_str(), strCameraCode, strDomainCode);
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet,"Cancel Water Alarm", "SplitCameraCode failed, RetCode=%d.", iRet);
		return iRet;
	}
	m_pUserMgr->GetDomainCode(strUserDoaminCode);
	//��װxml��׼�����͸�smu
	CXml xmlReq;
	IVS_UINT32 xmlLen = 0;
	iRet = CAlarmMgrXMLProcess::GetCancelWaterAlarmXML(uiID,pstConfirmInfo->cCancelInfo,pstConfirmInfo->cCancelTime,strUserDoaminCode,strDomainCode, xmlReq);
	const IVS_CHAR* pReq = xmlReq.GetXMLStream(xmlLen);
	CHECK_POINTER(pReq, IVS_OPERATE_MEMORY_ERROR);
	//����Ҫ���͵�CMD��ƴװ��NSS��Ϣͷ

	CSendNssMsgInfo sendNssMsgInfo;
	sendNssMsgInfo.SetNeedXml(TYPE_NSS_XML);
	sendNssMsgInfo.SetNetElemType(NET_ELE_SMU_NSS);
	sendNssMsgInfo.SetReqID(NSS_WATERMARK_CANCEL_ALARM_REQ);
	sendNssMsgInfo.SetReqData(pReq);	
	sendNssMsgInfo.SetDomainCode(strDomainCode);

	std::string strpRsp;
	IVS_INT32 iNeedRedirect = IVS_FAIL;
	iRet = m_pUserMgr->SendCmd(sendNssMsgInfo,strpRsp,iNeedRedirect);


	////����nssЭ��ͷ
	//CCmd *pCmd = CNSSOperator::instance().BuildSMUCmd(NSS_WATERMARK_CANCEL_ALARM_REQ, pReq);    
	//CHECK_POINTER(pCmd, IVS_SDK_RET_INIT_RTSPCLIENT_ERROR);
	////����nssЭ�� 	
	//CCmd *pCmdRsp = CNSSOperator::instance().SendSyncCmd(pCmd);
	//CHECK_POINTER(pCmdRsp, IVS_NET_RECV_TIMEOUT);

	//iRet = CNSSOperator::instance().ParseCmd2NSS(pCmdRsp);
	//HW_DELETE(pCmdRsp);
	return iRet;
}

// �澯��Ȩ
IVS_INT32 CAlarmMgr::AlarmCommission(const IVS_CHAR* pAlarmInCode, const IVS_ALARM_COMMISSION* pAlarmCommission)
{
	CHECK_POINTER(pAlarmCommission, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(m_pUserMgr, IVS_OPERATE_MEMORY_ERROR);
	IVS_DEBUG_TRACE("");


    // ���AlarmInCode����ȡ�����
	IVS_CHAR szAlarmInCode[IVS_ALARM_CODE_LEN + 1] = {0};
	IVS_CHAR szDomainCode[IVS_DOMAIN_CODE_LEN + 1] = {0};
	IVS_INT32 iRet = CXmlProcess::ParseDevCode(pAlarmInCode, szAlarmInCode, szDomainCode);
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet, "ParseDevCode fail", "NA");
		return iRet;
	}
	// ����������ת��Ϊ�����ֽ���
	IVS_UINT32 uiLen = 0;
	IVS_UINT32 uiUserNum = pAlarmCommission->uiUserNum;
	IVS_UINT32 uiBufferSize = (sizeof(IVS_USER_ID_INFO) + (sizeof(IVS_SHORT) + sizeof(IVS_INT32))) * uiUserNum //TLV����
		                       + sizeof(IVS_UINT64) // AlarmEventID ����
							   + sizeof(IVS_UINT32);// UserNum ����
							 
	IVS_CHAR* pData = IVS_NEW((IVS_CHAR*&)pData,uiBufferSize);
	CHECK_POINTER(pData, IVS_ALLOC_MEMORY_ERROR);
	memset(pData, 0x0, uiBufferSize);
	
	IVS_UINT64 ulAlarmEventID = 0;
	BP_htonll(pAlarmCommission->ullAlarmEventID, ulAlarmEventID);
	if (!CToolsHelp::Memcpy(pData + uiLen, uiBufferSize - uiLen, &ulAlarmEventID, sizeof(ulAlarmEventID)))
	{
		BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "CToolsHelp::Memcpy ulAlarmEventID to pData failed", "NA");
		IVS_DELETE(pData, MUILI);
		return IVS_ALLOC_MEMORY_ERROR;
	}
	uiLen += sizeof(ulAlarmEventID);

	uiUserNum = (IVS_UINT32)htonl(pAlarmCommission->uiUserNum);	
	if (!CToolsHelp::Memcpy(pData + uiLen, uiBufferSize - uiLen, &uiUserNum, sizeof(uiUserNum)))
	{
		BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "CToolsHelp::Memcpy uiUserNum to pData failed", "NA");
		IVS_DELETE(pData, MUILI);
		return IVS_ALLOC_MEMORY_ERROR;
	}
	uiLen += sizeof(uiUserNum);	

	// ����TLV������UserID��UserDomainCode(UserID��Ҫת���ֽ���)
	IVS_UINT32 ui = 0;
	IVS_UINT32 uiUserID = 0;
	IVS_USHORT usiTag = htons(static_cast<IVS_USHORT>(USERINFO_TAG));	
	IVS_INT32 uiLength = (IVS_INT32)htonl(sizeof(IVS_USER_ID_INFO));
	for (ui = 0; ui < pAlarmCommission->uiUserNum; ui++ )
	{	
		if (!CToolsHelp::Memcpy(pData + uiLen, uiBufferSize - uiLen, &usiTag, sizeof(IVS_USHORT)))
		{
			BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "CToolsHelp::Memcpy usiTag to pData failed", "NA");
			IVS_DELETE(pData, MUILI);
			return IVS_ALLOC_MEMORY_ERROR;
		}
		uiLen += sizeof(IVS_SHORT);
		
		if (!CToolsHelp::Memcpy(pData + uiLen, uiBufferSize - uiLen, &uiLength, sizeof(IVS_UINT32)))
		{
			BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "CToolsHelp::Memcpy uiLength to pData failed", "NA");
			IVS_DELETE(pData, MUILI);
			return IVS_ALLOC_MEMORY_ERROR;
		}
		uiLen += sizeof(IVS_INT32);
		
		IVS_USER_ID_INFO stUserIDInfo;
		(void)CToolsHelp::Memcpy(&stUserIDInfo, sizeof(IVS_USER_ID_INFO), &pAlarmCommission->stUserInfo[ui], sizeof(IVS_USER_ID_INFO));
		uiUserID = htonl(static_cast<IVS_ULONG>(stUserIDInfo.uiUserID));
		if (!CToolsHelp::Memcpy(pData + uiLen, uiBufferSize - uiLen, &uiUserID, sizeof(IVS_UINT32)))
		{
			BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "CToolsHelp::Memcpy uiUserID to pData failed", "NA");
			IVS_DELETE(pData, MUILI);
			return IVS_ALLOC_MEMORY_ERROR;
		}
		uiLen += sizeof(IVS_UINT32);
		if (!CToolsHelp::Memcpy(pData + uiLen, uiBufferSize - uiLen, stUserIDInfo.cDomainCode, IVS_DOMAIN_CODE_LEN))
		{
			BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "CToolsHelp::Memcpy stUserIDInfo.cDomainCode to pData failed", "NA");
			IVS_DELETE(pData, MUILI);
			return IVS_ALLOC_MEMORY_ERROR;
		}
		uiLen += IVS_DOMAIN_CODE_LEN;
		uiUserID = 0;

	}	

	CSendNssMsgInfo sendNssMsgInfo;	
	sendNssMsgInfo.SetNeedXml(TYPE_NSS_NOXML);
	sendNssMsgInfo.SetNetElemType(NET_ELE_SMU_NSS);
	sendNssMsgInfo.SetReqID(NSS_COMMISSION_ALARM_REQ);
	sendNssMsgInfo.SetReqDataPointer(pData);
	sendNssMsgInfo.SetReqLength(uiBufferSize);
	sendNssMsgInfo.SetDomainCode(szDomainCode);

	iRet = IVS_FAIL;
	std::string strpRsp;
	IVS_INT32 iNeedRedirect = IVS_FAIL;
	iRet = m_pUserMgr->SendCmd(sendNssMsgInfo,strpRsp, iNeedRedirect);
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet, "ParseCmd2Data is fail","NA");
		IVS_DELETE(pData, MUILI);
		return iRet;
	}
	IVS_DELETE(pData, MUILI);
	return iRet;
}

// ��Ȩ�澯�û���֤
IVS_INT32 CAlarmMgr::UserAlarmAuthorization(const IVS_CHAR *pPWD) const
{
	CHECK_POINTER(pPWD, IVS_PARA_INVALID);
	CHECK_POINTER(m_pUserMgr, IVS_OPERATE_MEMORY_ERROR);
	IVS_DEBUG_TRACE("");

	 /************************************************************************
    ** ��Կ�������̣���ʼ��->��������ID->����Կ->��Կ����(���ɵ�����ID)->
    **              ��������->ʹ����Կ�������ɵ�����ID����Cmd->������Ϣ                                                                   
    ************************************************************************/
    // ��ʼ����Կ���������,�����Կ����
	std::string strDomainCode;
	m_pUserMgr->GetDomainCode(strDomainCode);
    std::string strLinkID;
    CSDKSecurityClient oSecurityClient;
    int iMultiExchangeKey = oSecurityClient.MultiDomainExchangeKey(NET_ELE_SMU_NSS, m_pUserMgr, strDomainCode, strLinkID);
    if (IVS_SUCCEED != iMultiExchangeKey)
    {
        BP_RUN_LOG_ERR(IVS_FAIL, "User Alarm Authorization", "oSecurityClient Init failed");
        return IVS_FAIL;
    }
    // �������ݺ�ƴװ
    std::string strPWDCipher;
    if (IVS_SUCCEED != oSecurityClient.EncryptString(pPWD, strPWDCipher))
    {
        BP_RUN_LOG_ERR(IVS_FAIL, "User AlarmAuthorization", "Encrypt String failed");
        return IVS_FAIL;
    }
	//��װxml��׼�����͸�smu
	CXml xmlReq;
	IVS_UINT32 xmlLen = 0;
	IVS_INT32 iRet = CAlarmMgrXMLProcess::UserAlarmAuthorGetXML(strPWDCipher.c_str(), xmlReq);
    const IVS_CHAR* pReq = xmlReq.GetXMLStream(xmlLen);
    CHECK_POINTER(pReq, IVS_OPERATE_MEMORY_ERROR);
    std::string strSecurityTransID  = oSecurityClient.GetSecurityDataTransID();
    
    //����Ҫ���͵�CMD��ƴװ��NSS��Ϣͷ
    CCmd *pCmd = CNSSOperator::instance().BuildCmd(strLinkID, NET_ELE_SMU_NSS, NSS_USER_AUTHENTICATION_REQ, pReq, strSecurityTransID);
    CHECK_POINTER(pCmd, IVS_OPERATE_MEMORY_ERROR);

	//ͬ��������Ϣ����
	CCmd *pCmdRsp = CNSSOperator::instance().SendSyncCmd(pCmd);
	CHECK_POINTER(pCmdRsp, IVS_NET_RECV_TIMEOUT);
	iRet = CNSSOperator::instance().ParseCmd2NSS(pCmdRsp);
	HW_DELETE(pCmdRsp);
	return iRet;	
}

// �澯�ϱ���CU->SMU��
IVS_INT32 CAlarmMgr::AlarmReport(const IVS_ALARM_REPORT* pAlarmReport)
{
	CHECK_POINTER(pAlarmReport, IVS_OPERATE_MEMORY_ERROR);	
	IVS_DEBUG_TRACE("");

	// ���ַ������б���ת��
	IVS_CHAR szAlarmDesc[IVS_ALARM_DESCRIPTION_LEN + 1] = {0};
	(void)CToolsHelp::Memcpy(szAlarmDesc, IVS_ALARM_DESCRIPTION_LEN, pAlarmReport->cAlarmDesc, IVS_ALARM_DESCRIPTION_LEN);
	IVS_CHAR* pAlarmDesc = CToolsHelp::ANSIToUTF8(szAlarmDesc);
	IVS_UINT32 strAlarmDescLen = strlen(szAlarmDesc);
	if (NULL == pAlarmDesc)
	{	
		BP_RUN_LOG_ERR(IVS_FAIL,"ansi to utf8 failed"," pAlarmDesc");
		return IVS_FAIL;
	}

	IVS_CHAR szExtParam[IVS_ALARM_EX_PARAM_LEN + 1] = {0};
	(void)CToolsHelp::Memcpy(szExtParam, IVS_ALARM_EX_PARAM_LEN, pAlarmReport->cExtParam, IVS_ALARM_EX_PARAM_LEN);
	
	IVS_CHAR* pExtParam = CToolsHelp::ANSIToUTF8(szExtParam);
	IVS_UINT32 strExtParamLen = strlen(szExtParam);
	if (NULL == pExtParam)
	{
		free(pAlarmDesc);
		pAlarmDesc = NULL;
		BP_RUN_LOG_ERR(IVS_FAIL,"ansi to utf8 failed"," pExtParam");
		return IVS_FAIL;
	}

	// ����������ת��Ϊ�����ֽ���
	IVS_UINT32 uiSTLength = strExtParamLen + strAlarmDescLen;
	IVS_UINT32 uiBufferSize = ((((sizeof(IVS_ALARM_REPORT)  + 2 * (sizeof(IVS_SHORT) + sizeof(IVS_INT32)) - IVS_ALARM_DESCRIPTION_LEN)) - IVS_ALARM_EX_PARAM_LEN) - 32) + uiSTLength;
	IVS_CHAR* pData = IVS_NEW((IVS_CHAR*&)pData, uiBufferSize);
	if (NULL == pData)
	{
		free(pAlarmDesc);
		pAlarmDesc = NULL;
		free(pExtParam);
		pExtParam = NULL;
		BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "pData is null", "NA");
		return IVS_ALLOC_MEMORY_ERROR;
	}
	memset(pData, 0x0, uiBufferSize);

	IVS_ALARM_REPORT stAlarmReport;
	memset(&stAlarmReport, 0x0, sizeof(IVS_ALARM_REPORT));
	(void)CToolsHelp::Memcpy(&stAlarmReport, sizeof(IVS_ALARM_REPORT), pAlarmReport, sizeof(IVS_ALARM_REPORT));
	stAlarmReport.bAction = static_cast<IVS_BOOL>(htonl(static_cast<IVS_ULONG>(stAlarmReport.bAction)));
	stAlarmReport.uiOccurNumber = htonl(pAlarmReport->uiOccurNumber);

	IVS_CHAR szAlarmInCode[IVS_ALARM_CODE_LEN + 1] = {0};
	IVS_CHAR szDomainCode[IVS_DOMAIN_CODE_LEN + 1] = {0};	
	(void)CXmlProcess::ParseDevCode(stAlarmReport.cAlarmInCode, szAlarmInCode, szDomainCode);
	(void)CToolsHelp::Memcpy(stAlarmReport.cAlarmInCode, IVS_ALARM_CODE_LEN, szAlarmInCode, strlen(szAlarmInCode));

	IVS_UINT32 uiSTSize = ((sizeof(IVS_ALARM_REPORT) - IVS_ALARM_DESCRIPTION_LEN) - IVS_ALARM_EX_PARAM_LEN) - 32;
	(void)CToolsHelp::Memcpy(pData, uiBufferSize, &stAlarmReport, uiSTSize);

	// ����TLV�ṹ
	IVS_USHORT usiTag = htons(static_cast<IVS_USHORT>(ALARMDESC_TAG));  
	IVS_UINT32 uiLength = htonl(strlen(stAlarmReport.cAlarmDesc));
	(void)CToolsHelp::Memcpy(pData + uiSTSize, uiBufferSize - uiSTSize, &usiTag, sizeof(IVS_USHORT));

	uiSTSize += sizeof(IVS_USHORT);
	(void)CToolsHelp::Memcpy(pData + uiSTSize, uiBufferSize - uiSTSize, &uiLength, sizeof(IVS_UINT32));
	uiSTSize += sizeof(IVS_UINT32);
	(void)CToolsHelp::Memcpy(pData + uiSTSize, uiBufferSize - uiSTSize, pAlarmDesc, strAlarmDescLen);

	uiSTSize += strlen(stAlarmReport.cAlarmDesc);

	usiTag = htons(static_cast<IVS_USHORT>(EXTPARAM_TAG));
	uiLength = htonl(strlen(stAlarmReport.cExtParam));	
	(void)CToolsHelp::Memcpy(pData + uiSTSize, uiBufferSize - uiSTSize, &usiTag, sizeof(IVS_USHORT));

	uiSTSize += sizeof(IVS_USHORT);
	(void)CToolsHelp::Memcpy(pData + uiSTSize, uiBufferSize - uiSTSize, &uiLength, sizeof(IVS_UINT32));
	uiSTSize += sizeof(IVS_UINT32);
	(void)CToolsHelp::Memcpy(pData + uiSTSize, uiBufferSize - uiSTSize, pExtParam, strExtParamLen);
    // ��ȡ����SMU����
    std::string strSMULinkID;
    IVS_INT32 iGetLinkRet = m_pUserMgr->GetLocalDomainLinkID(NET_ELE_SMU_NSS, strSMULinkID);
    if (IVS_SUCCEED != iGetLinkRet)
    {
		free(pAlarmDesc);
		pAlarmDesc = NULL;
		free(pExtParam);
		pExtParam = NULL;
		IVS_DELETE(pData);
        BP_RUN_LOG_ERR(iGetLinkRet, "Get LocalDomainLinkID failed", "NA");
        return iGetLinkRet;
    }
	CCmd* pCmd = CNSSOperator::instance().BuildCmd(strSMULinkID, NET_ELE_SMU_NSS, NSS_ALARM_REPORT_REQ, pData, static_cast<IVS_INT32>(uiBufferSize));
	if (NULL == pCmd)
	{
		free(pAlarmDesc);
		pAlarmDesc = NULL;
		free(pExtParam);
		pExtParam = NULL;
		IVS_DELETE(pData);
		BP_RUN_LOG_ERR(IVS_OPERATE_MEMORY_ERROR, "BuildCmd failed", "NA");
		return IVS_OPERATE_MEMORY_ERROR;
	}

	//�������ͬ����������
	CCmd * pCmdRsp = CNSSOperator::instance().SendSyncCmd(pCmd);
	CHECK_POINTER(pCmdRsp, IVS_NET_RECV_TIMEOUT);

	//������������Ӧ�ķ�����
	IVS_INT32 iRet = CNSSOperator::instance().ParseCmd2NSS(pCmdRsp);
	HW_DELETE(pCmdRsp);

	free(pAlarmDesc);
	pAlarmDesc = NULL;
	free(pExtParam);
	pExtParam = NULL;
	IVS_DELETE(pData);
	return iRet;
}

//�������澯�ϱ�
IVS_INT32 CAlarmMgr::ManualAlarm(const IVS_CHAR* /*pReq*/, IVS_CHAR* &/*pRsp*/)//lint !e1762 !e1764
{
// 	����Ҫ���͵�CMD��ƴװ��NSS��Ϣͷ
// 		CCmd* pCmd = CNSSOperator::instance().BuildSMUCmd(NSS_MANUAL_ALARM_REQ, pReq);
// 	
// 		if (NULL == pCmd)
// 		{
// 			BP_RUN_LOG_ERR("CAlarmMgr::ManualAlarm", "failed", "BuildSMUCmd failed");
// 			return -1;
// 		}
// 	
// 		//ͬ��������Ϣ����
// 		CCmd* pRespCmd = CDispatcher::instance().dispatcherSyncMsgOut(pCmd);
// 		if (NULL != pRespCmd)
// 		{
// 			BP_RUN_LOG_INF("CAlarmMgr::ManualAlarm", "ok", "ReqID:", pRespCmd->GetReqID());
// 			HW_DELETE(pRespCmd);
// 			return 0;
// 		}
// 		else
// 		{
// 			BP_RUN_LOG_ERR("CAlarmMgr::ManualAlarm", "failed", "no response");
// 			return -1;
// 		}
	return 0;
}//lint !e1762 !e1764

//��������������澯
IVS_INT32 CAlarmMgr::StartAlarmOut(const IVS_CHAR* pAlarmOutCode)
{
	CHECK_POINTER(pAlarmOutCode, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(m_pUserMgr, IVS_OPERATE_MEMORY_ERROR);
	IVS_DEBUG_TRACE("");

	IVS_CHAR szAlarmOutCode[IVS_ALARM_CODE_LEN + 1] = {0};
	IVS_CHAR szDevDomainCode[IVS_DOMAIN_CODE_LEN + 1] = {0};
	IVS_INT32 iRet = CXmlProcess::ParseDevCode(pAlarmOutCode, szAlarmOutCode, szDevDomainCode);

	CXml xmlReq;
	IVS_INT32 iStateType = 0;
	iRet = CAlarmMgrXMLProcess::AlarmOutGetXML(xmlReq, szAlarmOutCode, szDevDomainCode, iStateType);

	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet, "AlarmOutGetXML fail", "NA");
		return iRet;
	}

	IVS_UINT32 xmlLen = 0;
	const IVS_CHAR* pReq = xmlReq.GetXMLStream(xmlLen);
	CHECK_POINTER(pReq, IVS_OPERATE_MEMORY_ERROR);

	CSendNssMsgInfo sendNssMsgInfo;	
	sendNssMsgInfo.SetNeedXml(TYPE_NSS_XML);
	sendNssMsgInfo.SetNetElemType(NET_ELE_SMU_NSS);
	sendNssMsgInfo.SetReqID(NSS_SET_ALARM_OUT_STATE_REQ);
	sendNssMsgInfo.SetReqData(pReq);	
	sendNssMsgInfo.SetDomainCode(szDevDomainCode);

	iRet = IVS_FAIL;
	std::string strpRsp;
	IVS_INT32 iNeedRedirect = IVS_FAIL;
	iRet = m_pUserMgr->SendCmd(sendNssMsgInfo,strpRsp,iNeedRedirect);
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet, "Start Alarm Out fail","Send Cmd failed");
		return iRet;
	}	

	return iRet;
}

// ֹͣ����������澯
IVS_INT32 CAlarmMgr::StopAlarmOut(const IVS_CHAR* pAlarmOutCode)
{
	CHECK_POINTER(pAlarmOutCode, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(m_pUserMgr, IVS_OPERATE_MEMORY_ERROR);
	IVS_DEBUG_TRACE("");

	IVS_CHAR szAlarmOutCode[IVS_ALARM_CODE_LEN + 1] = {0};
	IVS_CHAR szDevDomainCode[IVS_DOMAIN_CODE_LEN + 1] = {0};
	IVS_INT32 iRet = CXmlProcess::ParseDevCode(pAlarmOutCode, szAlarmOutCode, szDevDomainCode);

	CXml xmlReq;
	IVS_INT32 iStateType = 1;
	iRet = CAlarmMgrXMLProcess::AlarmOutGetXML(xmlReq, szAlarmOutCode, szDevDomainCode, iStateType);
	
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet, "AlarmOutGetXML fail", "NA");
		return iRet;
	}

	IVS_UINT32 xmlLen = 0;
	const IVS_CHAR* pReq = xmlReq.GetXMLStream(xmlLen);
	CHECK_POINTER(pReq, IVS_OPERATE_MEMORY_ERROR);

	CSendNssMsgInfo sendNssMsgInfo;	
	sendNssMsgInfo.SetNeedXml(TYPE_NSS_XML);
	sendNssMsgInfo.SetNetElemType(NET_ELE_SMU_NSS);
	sendNssMsgInfo.SetReqID(NSS_SET_ALARM_OUT_STATE_REQ);
	sendNssMsgInfo.SetReqData(pReq);	
	sendNssMsgInfo.SetDomainCode(szDevDomainCode);

	iRet = IVS_FAIL;
	std::string strpRsp;
	IVS_INT32 iNeedRedirect = IVS_FAIL;
	iRet = m_pUserMgr->SendCmd(sendNssMsgInfo,strpRsp,iNeedRedirect);
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet, "Stop Alarm Out fail","Send Cmd failed");
		return iRet;
	}	
	return iRet;
}

// ��ѯ�澯��Ϣ
IVS_INT32 CAlarmMgr::GetAlarmInfo(IVS_UINT64 ullAlarmEventID, const IVS_CHAR* pAlarmInCode, IVS_ALARM_EVENT* pAlarmEvent)
{
    CHECK_POINTER(pAlarmInCode, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pAlarmEvent, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(m_pUserMgr, IVS_OPERATE_MEMORY_ERROR);
    IVS_DEBUG_TRACE("pAlarmInCode = %s", pAlarmInCode);
	CXml xmlReq;
	IVS_INT32 iRet = CAlarmMgrXMLProcess::GetAlarmInfoGetXML(ullAlarmEventID, xmlReq);
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet, "Get AlarmInfo GetXML fail", "NA");
		return iRet;
	}
    IVS_CHAR szAlarmInCode[IVS_ALARM_CODE_LEN + 1] = {0};
    IVS_CHAR szDomainCode[IVS_DOMAIN_CODE_LEN + 1] = {0};
    iRet = CXmlProcess::ParseDevCode(pAlarmInCode, szAlarmInCode, szDomainCode);

    // �����������
    unsigned int xmlLen = 0;
    const IVS_CHAR * pReq = xmlReq.GetXMLStream(xmlLen);
    CHECK_POINTER(pReq, IVS_OPERATE_MEMORY_ERROR);
    CSendNssMsgInfo sendNssMsgInfo;	
    sendNssMsgInfo.SetNeedXml(TYPE_NSS_XML);
    sendNssMsgInfo.SetNetElemType(NET_ELE_SMU_NSS);
    sendNssMsgInfo.SetReqID(NSS_GET_ALARM_INFO_REQ);
    sendNssMsgInfo.SetReqData(pReq);
    sendNssMsgInfo.SetDomainCode(szDomainCode);

    iRet = IVS_FAIL;
    std::string strpRsp;
    IVS_INT32 iNeedRedirect = IVS_FAIL;
    iRet = m_pUserMgr->SendCmd(sendNssMsgInfo,strpRsp,iNeedRedirect);
    if (IVS_SUCCEED != iRet)
    {
        BP_RUN_LOG_ERR(iRet,  "Get AlarmInfo fail", "iRet = %d", iRet);
        return iRet;
    }

    CXml xmlRsp;
    if (!xmlRsp.Parse(strpRsp.c_str()))
    {
        BP_RUN_LOG_ERR(IVS_XML_PACK_ERROR, "prase xml fail", "NA");
        return IVS_XML_PACK_ERROR;
    }
	iRet = CAlarmMgrXMLProcess::GetAlarmInfoParseXML(pAlarmEvent, xmlRsp, szDomainCode);
	return iRet;
}

// ��ѯ��ʷ�澯��Ϣ�б�
IVS_INT32 CAlarmMgr::GetAlarmList(const IVS_QUERY_UNIFIED_FORMAT* pUnifiedQuery, IVS_ALARM_EVENT_LIST* pAlarmEventList, IVS_UINT32 uiBufferSize)//lint !e1762 !e1764 !e818
{
	CHECK_POINTER(pUnifiedQuery, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pAlarmEventList, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(m_pUserMgr, IVS_OPERATE_MEMORY_ERROR);
	IVS_DEBUG_TRACE("");

	CXml xmlReq;
	IVS_INT32 iRet = CXmlProcess::GetUnifiedFormatQueryXML(pUnifiedQuery, xmlReq);
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet,  "Get UnifiedFormatQuery XML is fail", "NA");
		return iRet;
	}
    // ����AlarmInCode	��ȡ��Ϣ����Ŀ����
    std::string strDstDomainCode;
	if (xmlReq.FindElemEx("Content/PageInfo/QueryCond"))
	{
		(void)CAlarmMgrXMLProcess::ModifyAlarmInCodeOfGetAlarmList(xmlReq, strDstDomainCode);
	}
    if (strDstDomainCode.empty())
    {
        m_pUserMgr->GetDomainCode(strDstDomainCode);
    }
    // �����������
    unsigned int xmlLen = 0;
    const IVS_CHAR * pReq = xmlReq.GetXMLStream(xmlLen);
    CHECK_POINTER(pReq, IVS_OPERATE_MEMORY_ERROR);
	CSendNssMsgInfo sendNssMsgInfo;	
    sendNssMsgInfo.SetNeedXml(TYPE_NSS_XML);
    sendNssMsgInfo.SetNetElemType(NET_ELE_SMU_NSS);
    sendNssMsgInfo.SetReqID(NSS_GET_ALARM_LIST_REQ);
    sendNssMsgInfo.SetReqData(pReq);
    sendNssMsgInfo.SetDomainCode(strDstDomainCode);

    iRet = IVS_FAIL;
    std::string strpRsp;
    IVS_INT32 iNeedRedirect = IVS_FAIL;
    iRet = m_pUserMgr->SendCmd(sendNssMsgInfo, strpRsp, iNeedRedirect, 45, 1);
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet,  "Get AlarmList fail", "iRet = %d", iRet);
		return iRet;
	}

    CXml xmlRsp;
	if (!xmlRsp.Parse(strpRsp.c_str()))
	{
		BP_RUN_LOG_ERR(IVS_XML_PACK_ERROR, "prase xml fail", "NA");
		return IVS_XML_PACK_ERROR;
	}
	iRet = CAlarmMgrXMLProcess::GetAlarmListParseXML(pAlarmEventList, uiBufferSize, xmlRsp, strDstDomainCode);
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet, "Parse response xml  fail", "NA");
		return iRet;
	}
	return iRet;
}

IVS_INT32 CAlarmMgr::GetDevCodeListInfo(const IVS_CHAR *pDevBigCode,std::list<std::string>& lDevCodeList)
{
	CHECK_POINTER(pDevBigCode, IVS_OPERATE_MEMORY_ERROR);
	IVS_DEBUG_TRACE("");
	std::string stdDevCode;
	std::string stdDevBigCode = pDevBigCode;
	unsigned int uiBeginNum = 0;
	//���Ҷ��ų��ֵ�λ��
	std::string::size_type pStartPos = stdDevBigCode.find(",");	
	//ֻ����һ���豸�����
	if (std::string::npos == pStartPos)
	{
		lDevCodeList.push_back(stdDevBigCode);
		return IVS_SUCCEED;
	}
	unsigned int uiStrLen = stdDevBigCode.size();
	while (std::string::npos != pStartPos && uiStrLen > 0)
	{
		stdDevCode = stdDevBigCode.substr(0,pStartPos);
		lDevCodeList.push_back(stdDevCode);
		uiBeginNum = pStartPos + 1;
		stdDevBigCode = stdDevBigCode.substr(uiBeginNum,stdDevBigCode.size());
		pStartPos = stdDevBigCode.find(",");
		uiStrLen --;
	}
	//�������һ���豸����
	if (!stdDevBigCode.empty())
	{
		lDevCodeList.push_back(stdDevBigCode);
	}
	return IVS_SUCCEED;
}

//ˮӡ�澯��ѯ
IVS_INT32 CAlarmMgr::QueryWatermarkAlarmList(const IVS_QUERY_UNIFIED_FORMAT *pQueryFormat, IVS_VOID* pBuffer,IVS_UINT32 uiBufferSize)
{
	CHECK_POINTER(pQueryFormat, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pBuffer,      IVS_OPERATE_MEMORY_ERROR);
	IVS_DEBUG_TRACE("");

	//�豸����List��������룩
	std::list<std::string> lDevList;
	//���������List
	std::string strCamerCode;
	//��������ȥ�����Ŵ���
	//IVS_CHAR chDevCode[IVS_DEV_CODE_LEN + 1] = {0};
	IVS_CHAR chDomaCode[IVS_DOMAIN_CODE_LEN + 1] = {0};
	for (int i = 0; i < pQueryFormat->iFieldNum; i++)
	{
		if (QUERY_CAMERA_CODE == pQueryFormat->stQueryField[i].eFieID)
		{
			//(IVS_VOID) CXmlProcess::ParseDevCode(pQueryFormat->stQueryField[i].cValue, chDevCode, chDomaCode);
			(void)GetDevCodeListInfo(pQueryFormat->stQueryField[i].cValue,lDevList);
			CXmlProcess::ParseDevCodeList(lDevList,strCamerCode,chDomaCode);
			break;
		}
	}

	std::string strDoaminCode = chDomaCode;
	//��fromindex��toindex�����ж�
	if (pQueryFormat->stIndex.uiToIndex < pQueryFormat->stIndex.uiFromIndex)
	{
		BP_RUN_LOG_ERR(IVS_PARA_INVALID, "Query Watermark Alarm List fail", "uiToIndex < uiFromIndex,uiToIndex=%u,uiFromIndex=%u",
			pQueryFormat->stIndex.uiToIndex, pQueryFormat->stIndex.uiFromIndex);
		return IVS_PARA_INVALID;
	}
	//�鿴�������ڴ��Ƿ�Ϸ�
	IVS_UINT32 uiReqNum = (pQueryFormat->stIndex.uiToIndex - pQueryFormat->stIndex.uiFromIndex) + 1;
	IVS_UINT32 uiCheckBufferSize = 0;
	 uiCheckBufferSize = sizeof(IVS_WATERMARK_ALARM_INFO_LIST) + sizeof(IVS_WATERMARK_ALARM_INFO_LIST)*(uiReqNum-1);       
	// �ж��Ƿ���ȣ��������ȣ��������������Ч
	if (uiCheckBufferSize != uiBufferSize)
	{
		return IVS_PARA_INVALID;
	}
	IVS_WATERMARK_ALARM_INFO_LIST* pAlarmList = (IVS_WATERMARK_ALARM_INFO_LIST*) pBuffer;
	//׼�����Ͳ�ѯָ��
	CXml xmlReq;
	IVS_UINT32 xmlLen = 0;
	IVS_QUERY_UNIFIED_FORMAT *pNoConst = const_cast<IVS_QUERY_UNIFIED_FORMAT*>(pQueryFormat);
	// ��װxml��׼�����͸�smu

	IVS_INT32 iRet = IVS_SUCCEED;
	iRet = CXmlProcess::GetUnifiedFormatQueryXML(pNoConst, xmlReq,chDomaCode,strCamerCode.c_str());  
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet, "Get Main DeviceList fail","IVS_SUCCEED != iRet");
		return iRet;
	}
	const IVS_CHAR* pReq = xmlReq.GetXMLStream(xmlLen);
	CHECK_POINTER(pReq, IVS_OPERATE_MEMORY_ERROR);

	// ��������������Ϣ��������
	CSendNssMsgInfo sendNssMsgInfo;
	sendNssMsgInfo.SetNeedXml(TYPE_NSS_XML);
	sendNssMsgInfo.SetNetElemType(NET_ELE_SMU_NSS);
	sendNssMsgInfo.SetReqID(NSS_GET_WATERMARK_ALARM_LIST_REQ);
	sendNssMsgInfo.SetReqData(pReq);	
	sendNssMsgInfo.SetDomainCode(strDoaminCode);
	std::string strpRsp;
	IVS_INT32 iNeedRedirect = IVS_FAIL;
	iRet = m_pUserMgr->SendCmd(sendNssMsgInfo,strpRsp,iNeedRedirect);
	if(IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet, "Get Water Alarm List fail","SendCmd to SMU Return Failed");
		return  iRet;
	}
	BP_DBG_LOG("Get Main DeviceList", "send XML = %s", pReq);
	BP_DBG_LOG("Get Main DeviceList", "revice XML = %s", strpRsp.c_str());
	iRet = IVS_XML_INVALID;
	CXml xmlRsp;
	// ������xmlʧ��
	if (xmlRsp.Parse(strpRsp.c_str()))
	{
		
		iRet = CAlarmMgrXMLProcess::GetWaterAlarmListParseXML(pAlarmList,strDoaminCode, uiBufferSize, xmlRsp);
	} 
	return iRet;
}

// ��ȡӵ��ָ���豸Ȩ�޵��û��б�
IVS_INT32 CAlarmMgr::GetDeviceUserList(const IVS_CHAR* pDomainCode, const IVS_CHAR* pDevCode, IVS_UINT32 uiIsUserOnline, const IVS_INDEX_RANGE* pIndexRange, IVS_DEVICE_USER_INFO_LIST* pDevUserList, IVS_UINT32 uiBufferSize)
{	
	CHECK_POINTER(pDevCode, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pDomainCode, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pIndexRange, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pDevUserList, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(m_pUserMgr, IVS_OPERATE_MEMORY_ERROR);
	IVS_DEBUG_TRACE("");

	// �����豸�����ȡ�豸��ID
	IVS_CHAR szDevCode[IVS_DEV_CODE_LEN + 1] = {0};
	IVS_CHAR szDomainCode[IVS_DOMAIN_CODE_LEN + 1] = {0};
	IVS_INT32 iRet = CXmlProcess::ParseDevCode(pDevCode, szDevCode, szDomainCode);
	// ƴװ��Ӧ��Ϣ
	CXml xmlReqGroupID;
	iRet = CAlarmMgrXMLProcess::GetGroupListGetXml(szDevCode, szDomainCode, xmlReqGroupID);
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet, "Get Group List GetXml fail", "NA");
		return iRet;
	}
	unsigned int xmlLen = 0;
	const IVS_CHAR* pReqGroupID = xmlReqGroupID.GetXMLStream(xmlLen);
	CHECK_POINTER(pReqGroupID, IVS_OPERATE_MEMORY_ERROR);

	// ��������������Ϣ��������
	CSendNssMsgInfo sendNssMsgInfo4GroupID;
	sendNssMsgInfo4GroupID.SetNeedXml(TYPE_NSS_XML);
	sendNssMsgInfo4GroupID.SetNetElemType(NET_ELE_SMU_NSS);
	sendNssMsgInfo4GroupID.SetReqID(NSS_GET_GROUP_LIST_BY_DEV_REQ);
	sendNssMsgInfo4GroupID.SetReqData(pReqGroupID);
	sendNssMsgInfo4GroupID.SetDomainCode(szDomainCode);
	std::string strpRsp;
	IVS_INT32 iNeedRedirect = IVS_FAIL;
	iRet = m_pUserMgr->SendCmd(sendNssMsgInfo4GroupID,strpRsp,iNeedRedirect);
	if(IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet, "Get Group List By DevCode  fail","SendCmd to SMU Return Failed");
		return  iRet;
	}
	// ����Ӧ��Ϣ�е�GroupIDȡ������Ϊ��ȡ�û��б�����������GroupID���м���","������
	CXml xmlGrpIDRsp;
	std::string strGroupIDs;
	if (!xmlGrpIDRsp.Parse(strpRsp.c_str()))
	{
		BP_RUN_LOG_ERR(IVS_SMU_XML_INVALID, "xmlReqGroupID.Parse(strpRsp)", "NA");
		return IVS_SMU_XML_INVALID;
	}
	iRet = CAlarmMgrXMLProcess::GetGroupID(xmlGrpIDRsp, strGroupIDs);

	// ƴװ��ȡӵ��ָ��Ȩ���豸���û��б� ����xml
	CXml xmlReq;
	iRet = CAlarmMgrXMLProcess::GetDeviceUserListGetXML(pDomainCode, pDevCode, uiIsUserOnline, pIndexRange,strGroupIDs,  xmlReq);
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet, "Get DeviceUserList GetXML fail", "NA");
		return iRet;
	}
	xmlLen = 0;
	const IVS_CHAR* pReq = xmlReq.GetXMLStream(xmlLen);
	CHECK_POINTER(pReq,IVS_XML_PACK_ERROR);

	// ��������������Ϣ��������
	CSendNssMsgInfo sendNssMsgInfo;
	sendNssMsgInfo.SetNeedXml(TYPE_NSS_XML);
	sendNssMsgInfo.SetNetElemType(NET_ELE_SMU_NSS);
	sendNssMsgInfo.SetReqID(NSS_GET_PERMISSION_USER_LIST_REQ);
	sendNssMsgInfo.SetReqData(pReq);
	sendNssMsgInfo.SetDomainCode(pDomainCode);
	strpRsp.clear();
	iNeedRedirect = IVS_FAIL;
	iRet = m_pUserMgr->SendCmd(sendNssMsgInfo,strpRsp,iNeedRedirect);
	if(IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet, "Get Device User List fail","SendCmd to SMU Return Failed");
		return  iRet;
	}
	CXml xmlRsp;
	if (!xmlRsp.Parse(strpRsp.c_str()))
	{
		BP_RUN_LOG_ERR(IVS_XML_PACK_ERROR, "prase xml fail", "NA");
		return IVS_XML_PACK_ERROR;
	}

	iRet = CAlarmMgrXMLProcess::GetDeviceUserListParseXML(pDevUserList, uiBufferSize, xmlRsp);
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet, "Parse response xml fail", "NA");
		return iRet;
	}
	return iRet;
}

// ��ȡƽ̨������Ϣ
IVS_INT32 CAlarmMgr::GetConfigInfo(const IVS_CHAR* pReqXml, IVS_CHAR** pRspXml)
{
	CHECK_POINTER(pReqXml, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pRspXml, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(m_pUserMgr, IVS_OPERATE_MEMORY_ERROR);
	IVS_DEBUG_TRACE("");
    // ��ȡ����SMU����
    std::string strSMULinkID;
    IVS_INT32 iGetLinkRet = m_pUserMgr->GetLocalDomainLinkID(NET_ELE_SMU_NSS, strSMULinkID);
    if (IVS_SUCCEED != iGetLinkRet)
    {
        BP_RUN_LOG_ERR(iGetLinkRet, "Get LocalDomainLinkID failed", "NA");
        return iGetLinkRet;
    }
	//����nssЭ��ͷ
	CCmd *pCmd = CNSSOperator::instance().BuildSMUCmd(NSS_GET_CONFIG_INFO_REQ, pReqXml,strSMULinkID);    
	CHECK_POINTER(pCmd, IVS_SDK_RET_INIT_RTSPCLIENT_ERROR);
	//����nssЭ�� 	
	CCmd *pCmdRsp = CNSSOperator::instance().SendSyncCmd(pCmd);
	CHECK_POINTER(pCmdRsp, IVS_NET_RECV_TIMEOUT);

	// ����ֵ
	IVS_INT32 iRet = IVS_SUCCEED;
	*pRspXml = CNSSOperator::instance().ParseCmd2XML(pCmdRsp, iRet);
	HW_DELETE(pCmdRsp);
	return iRet;
}

// ��ѯƽ̨��������Ϣ
IVS_INT32 CAlarmMgr::GetAbilityInfo(IVS_UINT32* uiAbility,IVS_CHAR** pRspXml)
{
	CHECK_POINTER(m_pUserMgr, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pRspXml, IVS_OPERATE_MEMORY_ERROR);
	IVS_DEBUG_TRACE("");
    // ��ȡ����SMU����
    std::string strSMULinkID;
    IVS_INT32 iGetLinkRet = m_pUserMgr->GetLocalDomainLinkID(NET_ELE_SMU_NSS, strSMULinkID);
    if (IVS_SUCCEED != iGetLinkRet)
    {
        BP_RUN_LOG_ERR(iGetLinkRet, "Get LocalDomainLinkID failed", "NA");
        return iGetLinkRet;
    }
	//����nssЭ��ͷ
	CCmd *pCmd = CNSSOperator::instance().BuildSMUCmd(NSS_GET_ABILITY_INFO_REQ, "",strSMULinkID);    
	CHECK_POINTER(pCmd, IVS_SDK_RET_INIT_RTSPCLIENT_ERROR);
	//����nssЭ�� 	
	CCmd *pCmdRsp = CNSSOperator::instance().SendSyncCmd(pCmd);
	CHECK_POINTER(pCmdRsp, IVS_NET_RECV_TIMEOUT);

	// ����ֵ
	IVS_INT32 iRet = IVS_SUCCEED;
	*pRspXml = CNSSOperator::instance().ParseCmd2XML(pCmdRsp, iRet);
	
	CXml xmlReq;
	if (!xmlReq.Parse(*pRspXml))
	{
		HW_DELETE(pCmdRsp);
		BP_RUN_LOG_ERR(IVS_XML_PACK_ERROR, "prase xml fail", "NA");
		return IVS_XML_PACK_ERROR;
	}
	iRet = CAlarmMgrXMLProcess::GetAbilityInfoParseXML(uiAbility, *pRspXml);
	HW_DELETE(pCmdRsp);
	return iRet;
}

// ��ѯOMU�澯�¼��б�
IVS_INT32 CAlarmMgr::GetDeviceAlarmList(const IVS_QUERY_UNIFIED_FORMAT* pUnifiedQuery, IVS_DEVICE_ALARM_EVENT_LIST* pAlarmEventList, IVS_UINT32 uiBufferSize)
{
	CHECK_POINTER(pAlarmEventList, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pUnifiedQuery, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(m_pUserMgr, IVS_OPERATE_MEMORY_ERROR);
	IVS_DEBUG_TRACE("");

	CXml xmlReq;
	IVS_INT32 iRet = CXmlProcess::GetUnifiedFormatQueryXML(pUnifiedQuery, xmlReq);
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet,  "Get UnifiedFormatQuery XML is fail", "NA");
		return iRet;
	}

	IVS_UINT32 uiXmlLen = 0;
	const IVS_CHAR* pReqXMl = xmlReq.GetXMLStream(uiXmlLen);
    // ��ȡ����OMU����
    std::string strOMULinkID;
    IVS_INT32 iGetLinkRet = m_pUserMgr->GetLocalDomainLinkID(NET_ELE_OMU_NSS, strOMULinkID);
    if (IVS_SUCCEED != iGetLinkRet)
    {
        BP_RUN_LOG_ERR(iGetLinkRet, "Get LocalDomainLinkID failed", "NA");
        return iGetLinkRet;
    }
	CCmd* pCmd = CNSSOperator::instance().BuildOMUCmd(NSS_OMU_GET_ALARM_REQ, pReqXMl, strOMULinkID);
	CHECK_POINTER(pCmd, IVS_OPERATE_MEMORY_ERROR);

	CCmd* pRspCmd = CNSSOperator::instance().SendSyncCmd(pCmd);
	CHECK_POINTER(pRspCmd, IVS_NET_RECV_TIMEOUT);

	iRet = IVS_SUCCEED;
	const IVS_CHAR* pRspXml = CNSSOperator::instance().ParseCmd2XML(pRspCmd, iRet);
	HW_DELETE(pRspCmd);
	CHECK_POINTER(pRspXml, iRet);
	if (IVS_SUCCEED == iRet)
	{
		CXml xmlRsp;
		if (!xmlRsp.Parse(pRspXml))
		{
			BP_RUN_LOG_ERR(IVS_SMU_XML_INVALID, "xmlRsp.Parse(pRspXml) fail", "NA");
			IVS_DELETE(pRspXml, MUILI);
			return IVS_SMU_XML_INVALID;
		}
		IVS_DELETE(pRspXml, MUILI);
		iRet = CAlarmMgrXMLProcess::GetDevAlarmListParseXML(xmlRsp, pAlarmEventList, uiBufferSize);
	}
	else
	{
		IVS_DELETE(pRspXml, MUILI);
		BP_RUN_LOG_ERR(iRet, "Parse Get DevAlarmList XML fail", "NA");
		return iRet;
	}
	return iRet;
}

// �豸������
IVS_INT32 CAlarmMgr::SetDeviceGuard(const IVS_CHAR* pReqXml)
{
	CHECK_POINTER(pReqXml, IVS_OPERATE_MEMORY_ERROR);
	IVS_DEBUG_TRACE("");

	CXml xml;
	IVS_CHAR szDevCode[IVS_DEV_CODE_LEN + 1] = {0};
	IVS_CHAR szDomainCode[IVS_DOMAIN_CODE_LEN + 1] = {0};
	IVS_INT32 iRet = CAlarmMgrXMLProcess::SetDevCode(xml, pReqXml, "Content/DevInfo/DevCode", szDomainCode, szDevCode);
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet, "Set Device Guard Set DevCode fail!! szDomainCode: %s, szDevCode: %s", szDomainCode, szDevCode);
		return iRet;
	}
	iRet = CXmlProcess::GetCommSendMgr(xml, m_pUserMgr, NSS_SET_GUARD_REQ, BUILDRET, szDomainCode);
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet, "Set Device Guard Send CMD fail", "NA");
		return iRet;
	}
	return iRet;
}

// ������λ
IVS_INT32 CAlarmMgr::ResetAlarm(const IVS_CHAR* pReqXml)
{
	CHECK_POINTER(pReqXml, IVS_PARA_INVALID);
	IVS_DEBUG_TRACE("");

	CXml xmlReq;
	IVS_CHAR szDevCode[IVS_DEV_CODE_LEN + 1] = {0};
	IVS_CHAR szDstDomainCode[IVS_DOMAIN_CODE_LEN + 1] = {0};
	IVS_INT32 iRet = CAlarmMgrXMLProcess::SetDevCode(xmlReq, pReqXml, "Content/AlarmOutChanInfo/AlarmOutCode", szDstDomainCode, szDevCode);
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet, "Reset Alarm Set DevCode fail!! szDomainCode: %s, szDevCode: %s", szDstDomainCode, szDevCode);
		return iRet;
	}
	iRet = CXmlProcess::GetCommSendMgr(xmlReq, m_pUserMgr, NSS_SET_ALARM_OUT_STATE_REQ, BUILDRET, szDstDomainCode);
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet, "Reset Alarm Send CMD fail", "NA");
		return iRet;
	}
	return iRet;
}

// �ⲿ��澯�¼�����
IVS_INT32 CAlarmMgr::ExEventSubscribe(const IVS_CHAR* pReqXml)
{
	CHECK_POINTER(pReqXml, IVS_PARA_INVALID);
	IVS_DEBUG_TRACE("");

	CXml xml;
	IVS_INT32 iRet = CAlarmMgrXMLProcess::ParseLocalDevCode(xml, pReqXml, "Content/EventSubscribeList", "EventSubscribeInfo");
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet, "Ex Event Subscribe Parse Local DevCode fail", "NA");
		return iRet;
	}
	// ���ĵ���ϢĿ�������û���¼������
	std::string strDstDomainCode;
	m_pUserMgr->GetDomainCode(strDstDomainCode);
	iRet = CXmlProcess::GetCommSendMgr(xml, m_pUserMgr, NSS_EVENT_SUBSCRIBE_REQ, BUILDRET, strDstDomainCode.c_str());
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet, "ExEventSubscribe Send CMD fail", "NA");
		return iRet;
	}
	return iRet;
}

// �ⲿ���豸Ŀ¼����
IVS_INT32 CAlarmMgr::ExCatalogSubscribe(const IVS_CHAR* pReqXml)
{
	CHECK_POINTER(pReqXml, IVS_PARA_INVALID);
	IVS_DEBUG_TRACE("");

	CXml xmlReq;
	IVS_INT32 iRet = CAlarmMgrXMLProcess::ParseLocalDevCode(xmlReq, pReqXml, "Content/CatalogSubscribeList", "CatalogSubscribeInfo");
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet, "Ex Catalog Subscribe Parse Local DevCode fail", "NA");
		return iRet;
	}
	std::string strDomainCode;
	m_pUserMgr->GetDomainCode(strDomainCode);
	iRet = CXmlProcess::GetCommSendMgr(xmlReq, m_pUserMgr, NSS_CATALOG_SUBSCRIBE_REQ, BUILDRET, strDomainCode.c_str());
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet, "Ex Catalog Subscribe Send Cmd fail", "NA");
		return iRet;
	}
	return iRet;
}

// ��ѯ�ⲿ���豸�¼����� 
IVS_INT32  CAlarmMgr::GetExEventSubscribe(const IVS_CHAR* pReqXml, IVS_CHAR** pRsqXml)
{
	CHECK_POINTER(pReqXml, IVS_PARA_INVALID);
	IVS_DEBUG_TRACE("");

	CXml xml;
	if (!xml.Parse(pReqXml))
	{
		BP_RUN_LOG_ERR(IVS_XML_INVALID, "xml.Parse(pReqXml) fail", "NA");
		return IVS_XML_INVALID;
	}
	std::string strDomainCode;
	m_pUserMgr->GetDomainCode(strDomainCode);
	IVS_INT32 iRet = CXmlProcess::GetCommSendMgr(xml, m_pUserMgr, NSS_GET_EVENT_SUBSCRIBE_LIST_REQ, BUILSTRING, strDomainCode.c_str());
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet, "Get ExEvent Subscribe List Send CMD fail", "NA");
		return iRet;
	}
	// ������Ӧ��Ϣ��ƴװ�����������LocalDevCode����
	iRet = CAlarmMgrXMLProcess::ModifyLocalDevCode(xml, strDomainCode, "Content/EventSubscribeList", "EventSubscribeInfo");
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet, "Modify Local DevCode when Get Event Subscribe List fail", "NA");
		return iRet;
	}
	IVS_UINT32 uiXmlLen = 0;
	const IVS_CHAR* pRsp = xml.GetXMLStream(uiXmlLen);
	CHECK_POINTER(pRsp, IVS_XML_PACK_ERROR);

    CHECK_POINTER(pRsqXml, IVS_OPERATE_MEMORY_ERROR);

	iRet = CAlarmMgrXMLProcess::GetRsqXML(pRsp, *pRsqXml);
	return iRet;
}

// ��ȡ�ⲿ���豸Ŀ¼����
IVS_INT32 CAlarmMgr::GetExCatalogSubscribeList(const IVS_CHAR* pReqXml, IVS_CHAR** pRsqXml)
{
	CHECK_POINTER(pReqXml, IVS_PARA_INVALID);
	CHECK_POINTER(pRsqXml, IVS_PARA_INVALID);
	IVS_DEBUG_TRACE("");

	CXml xmlReq;
	if (!xmlReq.Parse(pReqXml))
	{
		BP_RUN_LOG_ERR(IVS_XML_INVALID, "xmlReq.Parse(pReqXml) fail", "NA");
		return IVS_XML_INVALID;
	}
	std::string strLocalDomainCode;
	m_pUserMgr->GetDomainCode(strLocalDomainCode);
	IVS_INT32 iRet = CXmlProcess::GetCommSendMgr(xmlReq, m_pUserMgr, NSS_GET_CATALOG_SUBSCRIBE_LIST_REQ, BUILSTRING, strLocalDomainCode.c_str());
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet, "Get ExCatalog Subscribe List Send CMD fail", "NA");
		return iRet;
	}
	// ������Ӧ��Ϣ��ƴװ�����������LocalDevCode����
	iRet = CAlarmMgrXMLProcess::ModifyLocalDevCode(xmlReq, strLocalDomainCode, "Content/CatalogSubscribeList", "CatalogSubscribeInfo");
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet, "Modify Local DevCode when Get Catalog Subscribe List fail", "NA");
		return iRet;
	}

	IVS_UINT32 uiXmlLen = 0;
	const IVS_CHAR* pRsp = xmlReq.GetXMLStream(uiXmlLen);
	CHECK_POINTER(pRsp, IVS_XML_PACK_ERROR);

	iRet = CAlarmMgrXMLProcess::GetRsqXML(pRsp, *pRsqXml);
	return iRet;
}

// ��ȡ�澯ץ��ͼƬ�б�
IVS_INT32 CAlarmMgr::GetAlarmSnapshotList(const IVS_CHAR* pReqXml, IVS_CHAR** pRsqXml)
{
	CHECK_POINTER(pReqXml, IVS_PARA_INVALID);
	CHECK_POINTER(pRsqXml, IVS_PARA_INVALID);
	IVS_DEBUG_TRACE("");
	
	CXml xml;
	if (!xml.Parse(pReqXml))
	{
		BP_RUN_LOG_ERR(IVS_XML_INVALID, "xml.Parse(pRsqXml) fail", "NA");
		return IVS_XML_INVALID;
	}
	// �õ�������Ϣ�е�AlarmEventID��AlarmDomainCode�������ع�������Ϣ
	if (!xml.FindElemEx("Content/AlarmInfo"))
	{
		BP_RUN_LOG_ERR(IVS_XML_INVALID, "xml.FindElemEx(\"Content/AlarmInfo\") fail", "NA");
		return IVS_XML_INVALID;
	}
	const IVS_CHAR* szElemValue = NULL;
	IVS_ULONG ulAlarmEventID = 0;
	IVS_CHAR szAlarmInCode[IVS_ALARM_CODE_LEN + 1] = {0};
	GET_ELEM_VALUE_NUM_FOR_UINT("AlarmEventID", szElemValue, ulAlarmEventID, xml);
	GET_ELEM_VALUE_CHAR("AlarmInCode", szElemValue, szAlarmInCode, IVS_ALARM_CODE_LEN, xml);

	// ��DevCode��ȡ��ÿ������룬����(��ͬ�ģ�ĳ������������Ĵ����������ͬʱ��Ϊ��ϢĿ����)��Ȼ����������vector�����С�
	std::vector<std::string> ivecDomainCode;
    ivecDomainCode.clear();
	if (!xml.FindElemEx("Content/DeviceCodeList"))
	{
		BP_RUN_LOG_ERR(IVS_XML_INVALID, "xml.FindElemEx(\"Content/DeviceCodeList\") fail", "NA");
		return IVS_XML_INVALID;
	}
	if (!xml.FindElem("DeviceCode"))
	{
		BP_RUN_LOG_ERR(IVS_XML_INVALID, "xml.FindElemEx(\"DeviceCode\") fail", "NA");
		return IVS_XML_INVALID;
	}	

	IVS_INT32 iFlag = 0;
	std::string strDomainCode;
	IVS_INT32 iNum = 0;
	IVS_INT32 iRet = IVS_SUCCEED;
	do 
	{
		// ��ֳ�DevCode��DomainCode
		std::string strDevCode;
        const IVS_CHAR* pDevCode = xml.GetElemValue();
        if (NULL != pDevCode)
        {
            strDevCode = pDevCode;
        }

		IVS_CHAR szDevCode[IVS_ALARM_CODE_LEN + 1] = {0};
		IVS_CHAR szDomainCode[IVS_DOMAIN_CODE_LEN + 1] = {0};
		iRet = CXmlProcess::ParseDevCode(strDevCode.c_str(), szDevCode, szDomainCode);

		// ��������룬ĳ������������Ĵ����������ͬʱ��Ϊ��ϢĿ����ʱ��ֻ�跢һ����Ϣ����
		strDomainCode = szDomainCode;
		std::string strSendDomainCode;
		CDomainRouteMgr oDomainRouteMgr = m_pUserMgr->GetDomainRouteMgr();		
		bool bRet = oDomainRouteMgr.FindProxyDomainCode(strDomainCode, strSendDomainCode);
		// ���δ�ҵ�������ʹ��Ŀ�������;
		if (!bRet)
		{
			strSendDomainCode = strDomainCode;
		}
		// ������ͬ�������
		if (0 == iFlag)
		{
			ivecDomainCode.push_back(strSendDomainCode);
		}
		else
		{
			for (IVS_INT32 i = 0; i < iFlag; i++)
			{
				// һ���ҵ���ͬ�ģ�����ѭ��
				if (0 == ivecDomainCode[static_cast<IVS_ULONG>(i)].compare(strSendDomainCode))
				{
					iNum++;
					break;
				}
			}
			// ��û�ҵ���ͬ�ģ���������
			if (0 == iNum)
			{
				ivecDomainCode.push_back(strSendDomainCode);
			}			
		}
		iFlag++;
	} while (xml.NextElem());	
	
	// ��CMD����Ӧ������ID����map��
	CmdMap cmdMap;
	std::vector<std::string>::iterator iter = ivecDomainCode.begin();
	while(iter != ivecDomainCode.end())
	{
		CXml xmlReq;
		iRet = CAlarmMgrXMLProcess::GetAlarmSnapListGetXml(ulAlarmEventID, szAlarmInCode, *iter, xmlReq);
		IVS_UINT32 uiXmlLen = 0;
		const IVS_CHAR* pReq = xmlReq.GetXMLStream(uiXmlLen);
		CHECK_POINTER(pReq, IVS_OPERATE_MEMORY_ERROR);
		// �������������cmd
		CSendNssMsgInfo sendNssMsgInfo;	
		sendNssMsgInfo.SetNeedXml(TYPE_NSS_XML);
		sendNssMsgInfo.SetNetElemType(NET_ELE_SMU_NSS);
		sendNssMsgInfo.SetReqID(NSS_GET_ALARM_SNAPSHOT_LIST_REQ);
		sendNssMsgInfo.SetReqData(pReq);
		sendNssMsgInfo.SetDomainCode(*iter);
		CCmd* pCmd = m_pUserMgr->BuildCmd(sendNssMsgInfo);
		if (NULL == pCmd)
		{
			BP_RUN_LOG_INF("DomainCode: %s, NULL == pCmd", (*iter).c_str());
			iter++;
			continue;
		}
		(void)cmdMap.insert(std::make_pair(pCmd->GetTransID(), pCmd));
		iter++;
	}
	// ������Ϣ
	iRet = CNSSOperator::instance().SendSyncCmd(cmdMap, 35);
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet, "Get Alarm Snap shot List Send Cmd fail", "NA");
		return iRet;
	}
	// ������Ӧ��Ϣ
	CmdMapIterator iterCmdMap = cmdMap.begin();
	iRet = IVS_SUCCEED;
	// ��Ÿ����򷵻�������Ϣ����
	std::string strNSSRsq;
	iFlag = 0;
	while(iterCmdMap != cmdMap.end())
	{
		if(NULL == iterCmdMap->second)
		{
			// ������ӦCmdΪ�գ�����������һ��
			BP_RUN_LOG_INF("CmdRsp is NULL", "NA");
			iterCmdMap++;
			continue;
		}
		IVS_CHAR* pRsq = CNSSOperator::instance().ParseCmd2XML(iterCmdMap->second, iRet);
		if (IVS_SUCCEED != iRet || NULL == pRsq)
		{
			// ����Ӧ��Ϣ���Ϸ�������ѭ��������һ����Ӧ��Ϣ
			BP_RUN_LOG_INF("Parse RsqCmd fail or NULL == pRsq", "NA");
			iterCmdMap++;
			continue;
		}
		std::string strRsq = pRsq;
		// ������յ���Ӧxml
		iRet = CAlarmMgrXMLProcess::GetAlarmSnapshotListGetXml(strRsq, strNSSRsq, iFlag);
		if (IVS_SUCCEED != iRet)
		{
			BP_RUN_LOG_ERR(iRet, "Get final response xml fail", "NA");
			//return iRet;
			break;
		}
		HW_DELETE(iterCmdMap->second);
		cmdMap.erase(iterCmdMap++);
		IVS_DELETE(pRsq, MUILI);
		iFlag++;
	}
	
	if (IVS_SUCCEED != iRet)
	{
		CmdMapIterator iterCmdMapBegin = cmdMap.begin();
		while(iterCmdMapBegin != cmdMap.end())
		{
			HW_DELETE(iterCmdMapBegin->second);
		}
		cmdMap.clear();
		return iRet;
	}

	if (strNSSRsq.empty())
	{
		BP_RUN_LOG_INF("Response xml is empty, ", "NA");
		return iRet;
	}
	// ���¿����ڴ棬����Ӧ��string���������ڴ��У����ظ�CU
	IVS_CHAR* pResult = IVS_NEW(pResult, strNSSRsq.size() + 1);
	CHECK_POINTER(pResult, IVS_ALLOC_MEMORY_ERROR);
	memset(pResult, 0x0, strNSSRsq.size() + 1);
	if (!CToolsHelp::Memcpy(pResult, strNSSRsq.size() + 1, strNSSRsq.c_str(), strNSSRsq.size() + 1))
	{
		IVS_DELETE(pResult, MUILI);
		BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "Memcpy strNSSRsq to pResult fail, ", "NA");
		return IVS_ALLOC_MEMORY_ERROR;
	}
	*pRsqXml = pResult;

	return iRet;
}
