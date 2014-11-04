#include "UserMgr.h"
#include "AlarmLinkageMgr.h"
#include "nss_mt.h"
#include "bp_log.h"
#include "AlarmLinkageXMLProcess.h"
#include "Cmd.h"
#include "NSSOperator.h"
#include "IVS_Trace.h"
CAlarmLinkageMgr::CAlarmLinkageMgr(void)
	:m_pUserMgr(NULL)
{
}
CAlarmLinkageMgr::~CAlarmLinkageMgr(void)
{
	m_pUserMgr = NULL;
}

void CAlarmLinkageMgr::SetUserMgr(CUserMgr *pUserMgr)
{
	m_pUserMgr = pUserMgr;
}

// �����澯��������
IVS_INT32 CAlarmLinkageMgr::AddAlarmLinkage(const IVS_CHAR* pReqXml,IVS_CHAR** pRspXml)
{
	CHECK_POINTER(m_pUserMgr, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pReqXml, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pRspXml, IVS_OPERATE_MEMORY_ERROR);
	IVS_DEBUG_TRACE("");

	CXml xmlReq;
	std::string strDomainCode;
	if (IVS_SUCCEED != CAlarmLinkagrXMLProcess::SetAlarmInCodeValue(pReqXml, xmlReq, strDomainCode))
	{
		BP_RUN_LOG_ERR(IVS_XML_INVALID, "Set AlarmInCode Value error", "NA");
		return IVS_XML_INVALID;
	}

	IVS_UINT32 xmlLen = 0;
	const IVS_CHAR* pReq = xmlReq.GetXMLStream(xmlLen);
    if (NULL == pReq)
    {
        return IVS_XML_INVALID;
    }


	CSendNssMsgInfo sendNssMsgInfo;	
	sendNssMsgInfo.SetNeedXml(TYPE_NSS_XML);
	sendNssMsgInfo.SetNetElemType(NET_ELE_SMU_NSS);
	sendNssMsgInfo.SetReqID(NSS_ADD_ALARM_LINKAGE_REQ);
	sendNssMsgInfo.SetReqData(pReq);
	sendNssMsgInfo.SetDomainCode(strDomainCode);

	IVS_INT32 iRet = IVS_FAIL;
	std::string strpRsp;
	IVS_INT32 iNeedRedirect = IVS_FAIL;
	iRet = m_pUserMgr->SendCmd(sendNssMsgInfo,strpRsp, iNeedRedirect);
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet, "Add Alarm Linkage fail","Send Cmd failed");
		return iRet;
	}
	IVS_CHAR* pResult = IVS_NEW(pResult, strpRsp.size() + 1);
	CHECK_POINTER(pResult, IVS_ALLOC_MEMORY_ERROR);
	memset(pResult, 0x0, strpRsp.size() + 1);
	if (!CToolsHelp::Memcpy(pResult, strpRsp.size() + 1, strpRsp.c_str(), strpRsp.size() + 1))
	{
		IVS_DELETE(pResult, MUILI);
		BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "Memcpy(pResult, strpRsp.size(), strpRsp.c_str(), strpRsp.size()) fail", "NA");
		return IVS_ALLOC_MEMORY_ERROR;
	}
	*pRspXml = pResult;

	return iRet;
}

// �޸ĸ澯��������
IVS_INT32 CAlarmLinkageMgr::ModifyAlarmLinkage(const IVS_CHAR* pReqXml)
{
	CHECK_POINTER(m_pUserMgr, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pReqXml, IVS_OPERATE_MEMORY_ERROR);
	IVS_DEBUG_TRACE("");

	CXml xmlReq;
	std::string strDomainCode;
	if (IVS_SUCCEED != CAlarmLinkagrXMLProcess::SetAlarmInCodeValue(pReqXml, xmlReq, strDomainCode))
	{
		BP_RUN_LOG_ERR(IVS_XML_INVALID, "Set AlarmInCode Value error", "NA");
		return IVS_XML_INVALID;
	}
	IVS_UINT32 xmlLen = 0;
	const IVS_CHAR* pReq = xmlReq.GetXMLStream(xmlLen);
	CHECK_POINTER(pReq, IVS_OPERATE_MEMORY_ERROR);

	CSendNssMsgInfo sendNssMsgInfo;	
	sendNssMsgInfo.SetNeedXml(TYPE_NSS_NOXML);
	sendNssMsgInfo.SetNetElemType(NET_ELE_SMU_NSS);
	sendNssMsgInfo.SetReqID(NSS_MOD_ALARM_LINKAGE_REQ);
	sendNssMsgInfo.SetReqData(pReq);	
	sendNssMsgInfo.SetDomainCode(strDomainCode);

	IVS_INT32 iRet = IVS_FAIL;
	std::string strpRsp;
	IVS_INT32 iNeedRedirect = IVS_FAIL;
	iRet = m_pUserMgr->SendCmd(sendNssMsgInfo,strpRsp, iNeedRedirect);
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet, "Modify Alarm Linkage fail","Send Cmd failed");
		return iRet;
	}

	return iRet;
}

// ɾ���澯��������
IVS_INT32 CAlarmLinkageMgr::DeleteAlarmLinkage(const IVS_CHAR* pReqXml)
{
	CHECK_POINTER(m_pUserMgr, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pReqXml, IVS_OPERATE_MEMORY_ERROR);
	IVS_DEBUG_TRACE("");
	CXml xmlReq;
	std::string strDomainCode;
	IVS_INT32 iRet = CAlarmLinkagrXMLProcess::GetAndDelAlarmLinkageGetXML(pReqXml, xmlReq, strDomainCode);
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet, "DeleteLinkage GetXML fail", "NA");
		return iRet;
	}

	IVS_UINT32 xmlLen = 0;
	const IVS_CHAR* pReq = xmlReq.GetXMLStream(xmlLen);
	CHECK_POINTER(pReq,IVS_OPERATE_MEMORY_ERROR);
	
	CSendNssMsgInfo sendNssMsgInfo;	
	sendNssMsgInfo.SetNeedXml(TYPE_NSS_NOXML);
	sendNssMsgInfo.SetNetElemType(NET_ELE_SMU_NSS);
	sendNssMsgInfo.SetReqID(NSS_DEL_ALARM_LINKAGE_REQ);
	sendNssMsgInfo.SetReqData(pReq);	
	sendNssMsgInfo.SetDomainCode(strDomainCode);

	iRet = IVS_FAIL;
	std::string strpRsp;
	IVS_INT32 iNeedRedirect = IVS_FAIL;
	iRet = m_pUserMgr->SendCmd(sendNssMsgInfo,strpRsp, iNeedRedirect);
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet, "Delete Alarm Linkage fail","Send Cmd failed");
		return iRet;
	}

	return iRet;
}

//�鿴�澯���������б�
IVS_INT32 CAlarmLinkageMgr::GetAlarmlinkageList(const IVS_CHAR* pReqXml, IVS_CHAR** pRspXml)
{
	CHECK_POINTER(pReqXml, IVS_PARA_INVALID);
	CHECK_POINTER(m_pUserMgr, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pRspXml, IVS_OPERATE_MEMORY_ERROR);
	IVS_DEBUG_TRACE("");
	// ƴװ����xml
	CXml xmlReq;
	std::string strDomainCode;
	IVS_INT32 iRet = CAlarmLinkagrXMLProcess::GetAlarmLinkageListGetXml(pReqXml, xmlReq, strDomainCode);
	IVS_UINT32 xmlLen = 0;
	const IVS_CHAR* pReq = xmlReq.GetXMLStream(xmlLen);
	CHECK_POINTER(pReq,IVS_OPERATE_MEMORY_ERROR);
	
	// ������Ϣ
	CSendNssMsgInfo sendNssMsgInfo;	
	sendNssMsgInfo.SetNeedXml(TYPE_NSS_XML);
	sendNssMsgInfo.SetNetElemType(NET_ELE_SMU_NSS);
	sendNssMsgInfo.SetReqID(NSS_GET_ALARM_LINKAGE_LIST_REQ);
	sendNssMsgInfo.SetReqData(pReq);	
	sendNssMsgInfo.SetDomainCode(strDomainCode);

	iRet = IVS_FAIL;
	std::string strpRsp;
	IVS_INT32 iNeedRedirect = IVS_FAIL;
	iRet = m_pUserMgr->SendCmd(sendNssMsgInfo,strpRsp, iNeedRedirect);
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet, "Get Alarm Linkage List fail","Send Cmd failed");
		return iRet;
	}

	// �޸���Ӧ��Ϣ�е�AlarmInCode���õ��µ���Ӧ��Ϣ
	CXml xmlRsp;
	if (!xmlRsp.Parse(strpRsp.c_str()))
	{
		BP_RUN_LOG_ERR(IVS_SMU_XML_INVALID, "xml.Parse(pRspXml)", "NA");
		return IVS_SMU_XML_INVALID;
	}

	if (xmlRsp.FindElemEx("Content/LinkageList"))
	{
		if (!xmlRsp.FindElem("Linkage"))
		{
			BP_RUN_LOG_ERR(IVS_SMU_XML_INVALID, "xml.FindElemEx(Linkage)", "NA");
			return IVS_SMU_XML_INVALID;
		}
		for(;;)
		{
			(void)xmlRsp.IntoElem();
			if (!xmlRsp.FindElem("DevDomainCode"))
			{
				BP_RUN_LOG_ERR(IVS_SMU_XML_INVALID, "xml.FindElemEx(DevDomainCode)", "NA");
				return IVS_SMU_XML_INVALID;
			}
 			std::string strDevDomainCode = xmlRsp.GetElemValue();
			if (!xmlRsp.FindElem("AlarmInCode"))
			{
				BP_RUN_LOG_ERR(IVS_SMU_XML_INVALID, "xml.FindElemEx(AlarmInCode)", "NA");
				return IVS_SMU_XML_INVALID;
			}
			std::string strAlarmInCode = xmlRsp.GetElemValue();
			if (!strAlarmInCode.empty())
			{
				strAlarmInCode.append("#");
				strAlarmInCode.append(strDevDomainCode);
				xmlRsp.ModifyElemValue(strAlarmInCode.c_str());
			}	
			xmlRsp.OutOfElem();
			if (!xmlRsp.NextElem())
			{
				break;
			}
		}	
	}	

	IVS_UINT32 uiXmlLen = 0;
	const IVS_CHAR* pRsp = xmlRsp.GetXMLStream(uiXmlLen);
	CHECK_POINTER(pRsp,IVS_SMU_XML_INVALID);//���ж�pRsp�Ƿ�ΪNULL���ٵ���strlen
	IVS_CHAR* pResultXml = IVS_NEW(pResultXml, strlen(pRsp) + 1);
	memset(pResultXml, 0x0, strlen(pRsp) + 1);
	if (!CToolsHelp::Memcpy(pResultXml, strlen(pRsp) + 1, pRsp, strlen(pRsp) + 1))
	{
		BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "Copy pRsp to pResultXml fail", "NA");
		IVS_DELETE(pResultXml, MUILI);
		return IVS_ALLOC_MEMORY_ERROR;
	}
	*pRspXml = pResultXml;
	return iRet;
}

//��ѯ�澯��������
IVS_INT32 CAlarmLinkageMgr::GetAlarmLinkage(const IVS_CHAR* pReqXml,IVS_CHAR** pRspXml)
{
	CHECK_POINTER(m_pUserMgr, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pReqXml, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pRspXml, IVS_OPERATE_MEMORY_ERROR);
	IVS_DEBUG_TRACE("");
	
	// ƴװ����xml
	CXml xmlReq;
	std::string strDomainCode;
	IVS_INT32 iRet = CAlarmLinkagrXMLProcess::GetAndDelAlarmLinkageGetXML(pReqXml, xmlReq, strDomainCode);
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet, "AlarmLinkage GetReqXML is fail", "NA");
		return iRet;
	}
	const IVS_CHAR* pReq = NULL;
	IVS_UINT32 xmlLen = 0;
	pReq = xmlReq.GetXMLStream(xmlLen);
	CHECK_POINTER(pReq,IVS_OPERATE_MEMORY_ERROR);

	CSendNssMsgInfo sendNssMsgInfo;	
	sendNssMsgInfo.SetNeedXml(TYPE_NSS_XML);
	sendNssMsgInfo.SetNetElemType(NET_ELE_SMU_NSS);
	sendNssMsgInfo.SetReqID(NSS_GET_ALARM_LINKAGE_REQ);
	sendNssMsgInfo.SetReqData(pReq);	
	sendNssMsgInfo.SetDomainCode(strDomainCode);

	iRet = IVS_FAIL;
	std::string strpRsp;
	IVS_INT32 iNeedRedirect = IVS_FAIL;
	iRet = m_pUserMgr->SendCmd(sendNssMsgInfo,strpRsp, iNeedRedirect);
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet, "Get Alarm Linkage fail","Send Cmd failed");
		return iRet;
	}

	CXml xmlRsp;//�޸��ڴ�й¶����

	iRet = CAlarmLinkagrXMLProcess::AppendDevCode(strpRsp, "Content/Linkage/ActionList", xmlRsp, strDomainCode);
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet, "Append DomainCode after DevCode fail", "NA");
		return iRet;
	}

	// ���´�����Ӧ��Ϣ���ͷŵ�ԭ�ȵ��ڴ棬���¿��ٿռ�
	IVS_UINT32 uiXmlLen = 0;
	const IVS_CHAR* pRsp = xmlRsp.GetXMLStream(uiXmlLen);
	CHECK_POINTER(pRsp,IVS_SMU_XML_INVALID);//���ж�pRsp�Ƿ�ΪNULL���ٵ���strlen
	IVS_CHAR* pResultXml = IVS_NEW(pResultXml, strlen(pRsp) + 1);
	memset(pResultXml, 0x0, strlen(pRsp) + 1);
	if (!CToolsHelp::Memcpy(pResultXml, strlen(pRsp) + 1, pRsp, strlen(pRsp) + 1))
	{
		IVS_DELETE(pResultXml, MUILI);
		BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "Copy pRsp to pResultXml fail", "NA");
		return IVS_ALLOC_MEMORY_ERROR;
	}
	*pRspXml = pResultXml;

	return iRet;
}

// ��ѯ�澯������������
IVS_INT32 CAlarmLinkageMgr::GetAlarmLinkageAction(const IVS_CHAR* pReqXml, IVS_CHAR** pRspXml)
{
	CHECK_POINTER(m_pUserMgr, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pReqXml, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pRspXml, IVS_OPERATE_MEMORY_ERROR);
	IVS_DEBUG_TRACE("");

	CXml xmlReq;
	CXml xmlModified;//�����޸��ڴ�й¶����
	if (!xmlModified.Parse(pReqXml))
	{
		BP_RUN_LOG_ERR(IVS_XML_INVALID, "xmlModified.Parse(pReqXml)", "NA");
		return IVS_XML_INVALID;
	}
	if (!xmlModified.FindElemEx("Content/Linkage/AlarmInCode"))
	{
		BP_RUN_LOG_ERR(IVS_XML_INVALID, "xmlModified.FindElemEx(\"Content/Linkage/AlarmInCode\")", "NA");
		return IVS_XML_INVALID;
	}
	std::string strAlarmInCode = xmlModified.GetElemValue();
	IVS_CHAR szAlarmInCode[IVS_DEV_CODE_LEN + 1] = {0};
	IVS_CHAR szDomainCode[IVS_DOMAIN_CODE_LEN + 1] = {0};
	IVS_INT32 iRet = CXmlProcess::ParseDevCode(strAlarmInCode.c_str(), szAlarmInCode, szDomainCode);
	xmlModified.ModifyElemValue(szAlarmInCode);

	IVS_UINT32 uiXmlLen = 0;
	iRet = CAlarmLinkagrXMLProcess::AddDomainCode4XML(szDomainCode, "Content", "DomainCode", xmlModified.GetXMLStream(uiXmlLen), xmlReq);
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet, "AddDomainCode4XML fail", "NA");
		return iRet;
	}

	const IVS_CHAR* pReq = xmlReq.GetXMLStream(uiXmlLen);
	CHECK_POINTER(pReq,IVS_OPERATE_MEMORY_ERROR);

	CSendNssMsgInfo sendNssMsgInfo;	
	sendNssMsgInfo.SetNeedXml(TYPE_NSS_XML);
	sendNssMsgInfo.SetNetElemType(NET_ELE_SMU_NSS);
	sendNssMsgInfo.SetReqID(NSS_GET_ALARM_LINKAGE_ACTION_REQ);
	sendNssMsgInfo.SetReqData(pReq);	
	sendNssMsgInfo.SetDomainCode(szDomainCode);

	iRet = IVS_FAIL;
	std::string strpRsp;
	IVS_INT32 iNeedRedirect = IVS_FAIL;
	iRet = m_pUserMgr->SendCmd(sendNssMsgInfo,strpRsp, iNeedRedirect);
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet, "Get Alarm Linkage Action fail","Send Cmd failed");
		return iRet;
	}
	// ����ѯ���Ϊ�գ��򷵻�����xmlֻ��Content�ڵ�
	CXml xmlRsp;
	if (!xmlRsp.Parse(strpRsp.c_str()))
	{
		BP_RUN_LOG_ERR(IVS_SMU_XML_INVALID, "xml.Parse(strNSSRsq.c_str())", "NA");
		return IVS_SMU_XML_INVALID;
	}
	if (xmlRsp.FindElemEx("Content/ActionList"))
	{
		if (!xmlRsp.FindElem("Action"))
		{
			BP_RUN_LOG_ERR(IVS_SMU_XML_INVALID, "xml.FindElem(Action)", "NA");
			return IVS_SMU_XML_INVALID;
		}

		const IVS_CHAR* szElemValue = NULL;
		IVS_CHAR szDevCode[IVS_DEV_CODE_LEN + 1] = {0};
		std::string strBigCode;
		do 
		{
			(void)xmlRsp.IntoElem();
			// DevList��O�ֶΣ�����û�У�����Action��M�ֶΣ��Ҳ���DevList��ѭ������Ҫ����
			if (!xmlRsp.FindElem("DevList"))
			{
				xmlRsp.OutOfElem();
				continue;
			}
			(void)xmlRsp.IntoElem();
			if (!xmlRsp.FindElem("DevInfo"))
			{
				BP_RUN_LOG_ERR(IVS_SMU_XML_INVALID, "xml.FindElemEx(DevInfo) fail", "NA");
				return IVS_SMU_XML_INVALID;
			}
			do 
			{
				(void)xmlRsp.IntoElem();

				if (!xmlRsp.FindElem("DevDomainCode"))
				{
					BP_RUN_LOG_ERR(IVS_SMU_XML_INVALID, "xmlRsp.FindElem(\"DevDomainCode\")", "NA");
					return IVS_SMU_XML_INVALID;
				}
				std::string strDevDomainCode = xmlRsp.GetElemValue();
				xmlRsp.RemoveElem();
				GET_ELEM_VALUE_CHAR("DevCode", szElemValue, szDevCode, IVS_DEV_CODE_LEN, xmlRsp);
				strBigCode.assign(szDevCode);
				if (strDevDomainCode.empty())
				{
					strDevDomainCode = szDomainCode;
				}
				strBigCode.append("#").append(strDevDomainCode);
				xmlRsp.ModifyElemValue(strBigCode.c_str());
				xmlRsp.OutOfElem();
			} while (xmlRsp.NextElem());

			xmlRsp.OutOfElem();
			xmlRsp.OutOfElem();
		} while (xmlRsp.NextElem());
	}
	

	// ���´�����Ӧ��Ϣ���ͷŵ�ԭ�ȵ��ڴ棬���¿��ٿռ�,
	const IVS_CHAR* pRsp = xmlRsp.GetXMLStream(uiXmlLen);
	CHECK_POINTER(pRsp,IVS_SMU_XML_INVALID);
	IVS_CHAR* pResultXml = IVS_NEW(pResultXml, strlen(pRsp) + 1);
	memset(pResultXml, 0x0, strlen(pRsp) + 1);
	if (!CToolsHelp::Memcpy(pResultXml, strlen(pRsp) + 1, pRsp, strlen(pRsp) + 1))
	{
		IVS_DELETE(pResultXml, MUILI);
		BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "Copy pRsp to pResultXml fail", "NA");
		return IVS_ALLOC_MEMORY_ERROR;
	}
	*pRspXml = pResultXml;

	return iRet;
}
