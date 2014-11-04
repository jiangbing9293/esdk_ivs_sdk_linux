#include "LogMgr.h"
#include "NSSOperator.h"
#include "nss_mt.h"
#include "bp_log.h"
#include "UserMgr.h"
#include "ivs_xml.h"
#include "XmlProcess.h"
#include "IVS_Trace.h"

#include "LogMgrXMLProcess.h"
CLogMgr::CLogMgr(void):m_pUserMgr(NULL)
{
	
}


CLogMgr::~CLogMgr(void)
{
	m_pUserMgr = NULL;
}

// �����û�������
void CLogMgr::SetUserMgr(void *pUserMgr)
{
	m_pUserMgr = (CUserMgr *)pUserMgr;
}

// ��ѯ������־
IVS_INT32 CLogMgr::GetOperationLog(const IVS_QUERY_UNIFIED_FORMAT* pUnifiedQuery, IVS_OPERATION_LOG_LIST* pResOperLogList,const IVS_UINT32& uiBufSize) const
{	
	CHECK_POINTER(m_pUserMgr,IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pUnifiedQuery,IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pResOperLogList,IVS_OPERATE_MEMORY_ERROR);

	IVS_DEBUG_TRACE("");
// �ѽṹ��pUnifiedQuery��iLoginID��װ��xmlReq��
	CXml xmlReq;
	IVS_INT32 iRet = CXmlProcess::GetUnifiedFormatQueryXML(pUnifiedQuery,xmlReq);
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet, "Get Operation Log", "Get Unified Format Query XML is fail");
		//modify by zwx211831 ���Ĵ��󷵻���
		return iRet;
	}

	unsigned int xmlLen = 0;
	const IVS_CHAR* pReq = xmlReq.GetXMLStream(xmlLen);	
	CHECK_POINTER(pReq,IVS_OPERATE_MEMORY_ERROR);
	// ����������̳�����֧�ֶ���
	IVS_CHAR szCameraCode[IVS_DEV_CODE_LEN+1] = {0};
	IVS_CHAR szDomainCode[IVS_DOMAIN_CODE_LEN+1] = {0};
	for (int i = 0; i < pUnifiedQuery->iFieldNum; i++)
	{
		if (QUERY_CAMERA_CODE == pUnifiedQuery->stQueryField[i].eFieID || QUERY_OPERATION_OBJECT_CODE == pUnifiedQuery->stQueryField[i].eFieID)
		{
			(IVS_VOID)CXmlProcess::ParseDevCode(pUnifiedQuery->stQueryField[i].cValue, szCameraCode, szDomainCode);
		}
	}

	// ��װ����
	CSendNssMsgInfo sendNssMsgInfo;	
	sendNssMsgInfo.SetNeedXml(TYPE_NSS_XML);
	sendNssMsgInfo.SetNetElemType(NET_ELE_SMU_NSS);
	sendNssMsgInfo.SetReqID(NSS_GET_OPERATION_LOG_REQ);
	sendNssMsgInfo.SetReqData(pReq);
	sendNssMsgInfo.SetCameraCode(szCameraCode);
	sendNssMsgInfo.SetDomainCode(szDomainCode);

	std::string strpRsp;
	IVS_INT32 iNeedRedirect = IVS_FAIL;
	// ���Ͳ���ʧ�ܣ�ֱ�ӷ���
	iRet = m_pUserMgr->SendCmd(sendNssMsgInfo,strpRsp,iNeedRedirect);
	// NSS��Ϣ��������󣬲��������ݣ�ֱ�ӷ��ش�����
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet, "Get Motion Range Data Platform", "SendCmd operation succeed,rspCode = %d", iRet);
		return iRet;
	}

    CXml xmlRsp;
    if (!xmlRsp.Parse(strpRsp.c_str()))
    {
        BP_RUN_LOG_ERR(IVS_SMU_XML_INVALID, "Get Operation Log", "xmlRsp.Parse(pRsp) is fail");
        return IVS_SMU_XML_INVALID;
    }
    // ����XML��Ӧ��ϢΪ�ṹ��
    iRet = CLogMgrXMLProcess::PraseResOperationLogXML(xmlRsp,pResOperLogList,uiBufSize);	
	
	if (iRet != IVS_SUCCEED)
	{
		BP_RUN_LOG_ERR(iRet, "Get Operation Log", "Prase Response Operation Log XML is fail");
		return iRet;
	}
	return iRet;
}
