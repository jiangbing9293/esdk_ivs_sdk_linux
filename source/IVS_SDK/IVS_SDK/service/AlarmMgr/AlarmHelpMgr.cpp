#include "AlarmHelpMgr.h"
#include "AlarmMgrXMLProcess.h"
#include "nss_mt.h"
#include "NSSOperator.h"
#include "UserMgr.h"
#include "bp_log.h"
#include "IVS_Trace.h"

CAlarmHelpMgr::CAlarmHelpMgr(void)
	:m_AlarmHelpMgr(NULL)
	,m_UserMgr(NULL)
{
}
CAlarmHelpMgr::~CAlarmHelpMgr(void)
{
	m_AlarmHelpMgr = NULL;
	m_UserMgr = NULL;
}

void CAlarmHelpMgr::SetUserMgr( CUserMgr *pUserMgr)
{
	m_UserMgr = pUserMgr;
}

// �����澯����
IVS_INT32 CAlarmHelpMgr::AddHelpInfo(const IVS_CHAR* pDomainCode, const IVS_HELP_INFO *helpInfo) const
{	
	CHECK_POINTER(helpInfo,IVS_PARA_INVALID);
	CHECK_POINTER(m_UserMgr,IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pDomainCode, IVS_PARA_INVALID);
	IVS_DEBUG_TRACE("");

	// ���ݽṹƴװXML��ת��Ϊconst IVS_CHAR *	
	CXml xmlReq;
	IVS_INT32 iRet = CAlarmMgrXMLProcess::HelpInfoGetXML(helpInfo, xmlReq);
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet, "Get HelpInfo XML fail", "NA");
		return iRet;
	}

	iRet = CXmlProcess::GetCommSendMgr(xmlReq, m_UserMgr, NSS_ADD_HELP_INFO_REQ, BUILDRET, pDomainCode);
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet, "Add HelpInfo Send CMD fail", "NA");
		return iRet;
	}
	return iRet;
}

// �޸ĸ澯����
IVS_INT32 CAlarmHelpMgr::ModifyHelpInfo(const IVS_CHAR* pDomainCode, const IVS_HELP_INFO *helpInfo) const
{
	// ���ݽṹƴװXML��ת��Ϊconst IVS_CHAR *	
	CHECK_POINTER(helpInfo, IVS_PARA_INVALID);
	CHECK_POINTER(m_UserMgr, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pDomainCode, IVS_OPERATE_MEMORY_ERROR);
	IVS_DEBUG_TRACE("");
	CXml xmlReq;
	IVS_INT32 iRet = CAlarmMgrXMLProcess::HelpInfoGetXML(helpInfo, xmlReq);
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet, "Get HelpInfo XML fail", "NA");
		return iRet;
	}
	iRet = CXmlProcess::GetCommSendMgr(xmlReq, m_UserMgr, NSS_MOD_HELP_INFO_REQ, BUILDRET, pDomainCode);
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet, "Modify HelpInfo Send CMD fail", "NA");
		return iRet;
	}
	return iRet;
}

// ɾ���澯����
IVS_INT32 CAlarmHelpMgr::DeleteHelpInfo(IVS_UINT32 uiSourceId,IVS_UINT32 uiSourceType, const IVS_CHAR* pDomainCode) const
{
	CHECK_POINTER(m_UserMgr, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pDomainCode, IVS_PARA_INVALID);
	IVS_DEBUG_TRACE("");
	// ���ݽṹƴװXML��ת��Ϊconst IVS_CHAR *
	CXml xmlReq;
	IVS_INT32 iRet = CAlarmMgrXMLProcess::DeleteHelpInfoGetXML(uiSourceId, uiSourceType, xmlReq);
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet,  "Get DeleteHelpInfo XML fail", "NA");
		return iRet;
	}
	iRet = CXmlProcess::GetCommSendMgr(xmlReq, m_UserMgr, NSS_DEL_HELP_INFO_REQ, BUILDRET, pDomainCode);
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet, "Delete HelpInfo Send CMD fail", "NA");
		return iRet;
	}

	return iRet;
}

// ��ѯ�澯��������
IVS_INT32 CAlarmHelpMgr::QueryHelpInfo(IVS_UINT32 uiAlarmHelpSourceType,IVS_UINT32 uiAlarmHelpSourceId,const IVS_CHAR* pDomainCode, IVS_HELP_INFO *helpInfo) const
{
	// ���ݽṹƴװXML��ת��Ϊconst IVS_CHAR *	
	CHECK_POINTER(helpInfo,IVS_PARA_INVALID);
	CHECK_POINTER(pDomainCode, IVS_PARA_INVALID);
	IVS_DEBUG_TRACE("");
	CXml xmlReq;
	IVS_INT32 iRet = CAlarmMgrXMLProcess::QueryHelpInfoGetXML(uiAlarmHelpSourceType, uiAlarmHelpSourceId, xmlReq);
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet, "Get QueryHelpInfo XML fail", "NA");
		return iRet;
	}
	iRet = CXmlProcess::GetCommSendMgr(xmlReq, m_UserMgr, NSS_QUERY_HELP_INFO_REQ, BUILSTRING, pDomainCode);
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet, "Query HelpInfo Send CMD fail", "NA");
		return iRet;
	}
	iRet = CAlarmMgrXMLProcess::QueryHelpInfoPraseXML(helpInfo, xmlReq);

	return iRet;
}

// ��ѯ������Ϣ����
IVS_INT32 CAlarmHelpMgr::QueryHelpInfoText(const IVS_CHAR* pAlarmInCode, const IVS_CHAR* pAlarmType, IVS_CHAR* pHelpInfo, IVS_UINT32 uiBufferSize)
{
    CHECK_POINTER(pAlarmInCode,IVS_PARA_INVALID);
    CHECK_POINTER(pAlarmType,IVS_PARA_INVALID);
    CHECK_POINTER(pHelpInfo,IVS_PARA_INVALID);
    CHECK_POINTER(m_UserMgr,IVS_PARA_INVALID);
    IVS_DEBUG_TRACE("pAlarmInCode:%s, pAlarmType:%s", pAlarmInCode, pAlarmType);
    // ƴ������XML
    CXml xmlReq;
    IVS_CHAR cAlarmInCodeTemp[IVS_ALARM_CODE_LEN + 1] = {0};
    IVS_CHAR cDomainCodeTemp[IVS_DOMAIN_CODE_LEN + 1] = {0};
    (void)CXmlProcess::ParseDevCode(pAlarmInCode, cAlarmInCodeTemp, cDomainCodeTemp);
    IVS_INT32 iRet = CAlarmMgrXMLProcess::QueryHelpInfoTextGetXML(cAlarmInCodeTemp, pAlarmType, xmlReq);
    if (IVS_SUCCEED != iRet)
    {
        BP_RUN_LOG_ERR(iRet, "Get Query HelpInfoText XML fail", "NA");
        return iRet;
    }
    unsigned int xmlLen = 0;
    const IVS_CHAR * pReqData = xmlReq.GetXMLStream(xmlLen);
    CHECK_POINTER(pReqData, IVS_XML_PACK_ERROR);

    // ������Ϣ������벻���ã�Ĭ�Ϸ��͵���¼��
    CSendNssMsgInfo oSendNssMsgInfo;	
    oSendNssMsgInfo.SetNeedXml(TYPE_NSS_XML);
    oSendNssMsgInfo.SetNetElemType(NET_ELE_SMU_NSS);
    oSendNssMsgInfo.SetReqID(NSS_GET_HELP_INFO_TEXT_REQ);
    oSendNssMsgInfo.SetReqData(pReqData);
	oSendNssMsgInfo.SetDomainCode(cDomainCodeTemp);
    // ���淵����Ϣ
    std::string strpRsp;
	IVS_INT32 iNeedRedirect = IVS_FAIL;
    iRet = m_UserMgr->SendCmd(oSendNssMsgInfo,strpRsp,iNeedRedirect);
    if (IVS_SUCCEED != iRet)
    {
        BP_RUN_LOG_ERR(iRet, "Query HelpInfo Text fail","Send Cmd failed");
        return iRet;
    }
    CXml xmlRsp;
    if (!xmlRsp.Parse(strpRsp.c_str()))
    {
        BP_RUN_LOG_ERR(IVS_XML_PACK_ERROR, "prase xml fail", "NA");
        return IVS_XML_PACK_ERROR;
    }
    // ֱ�ӽ���xml,��ȡ������Ϣ����
    if (!xmlRsp.FindElemEx("Content/HelpInfo/HelpInfo"))
    {
        BP_RUN_LOG_ERR(IVS_XML_INVALID, "Content/HelpInfo/HelpInfo not exist", "NA");
        return IVS_XML_INVALID;
    }
    const char* pHelpText = xmlRsp.GetElemValue();
    if (NULL == pHelpText)
    {
        BP_RUN_LOG_ERR(IVS_XML_INVALID, "the value of Content/HelpInfo/HelpInfo is null", "NA");
        return IVS_XML_INVALID;
    }
    // У�����ռ��Ƿ��㹻
    if (uiBufferSize < strlen(pHelpText))
    {
        BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "memory not enough", "NA");
        return IVS_ALLOC_MEMORY_ERROR;
    }
    (void)CToolsHelp::Memcpy(pHelpInfo, uiBufferSize, pHelpText, strlen(pHelpText));
    return iRet;
}


