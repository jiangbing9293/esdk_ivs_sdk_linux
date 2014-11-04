#include "AlarmLinkageXMLProcess.h"
#include "ToolsHelp.h"
#include "bp_log.h"
#include "ivs_xml.h"
#include "UserMgr.h"
#include "IVS_Trace.h"
CAlarmLinkagrXMLProcess::CAlarmLinkagrXMLProcess()
{
}


CAlarmLinkagrXMLProcess::~CAlarmLinkagrXMLProcess()
{
}


// ��������ƴװxml����������������ID��(��ѯ�������������ɾ�����������õ�)
IVS_INT32 CAlarmLinkagrXMLProcess::GetAndDelAlarmLinkageGetXML(const IVS_CHAR* pReqXml, CXml& xml, std::string& strDomainCode)
{
	CHECK_POINTER(pReqXml, IVS_PARA_INVALID);
	IVS_DEBUG_TRACE("");
	CXml xmlReq;//�޸��ڴ�й¶���⣨�õ����ս��ǰ����ֱ�Ӳ������xml��
	CXml xmlModified;
	// �ҵ�AlarmInCode�ڵ㣬���ȡ������룬�޸�AlarmInCode�ڵ�ֵ
	if (!xmlReq.Parse(pReqXml))
	{
		BP_RUN_LOG_ERR(IVS_XML_INVALID, "xmlReq.Parse(pReqXml) fail", "NA");
		return IVS_XML_INVALID;
	}
	if (!xmlReq.FindElemEx("Content/Linkage/AlarmInCode"))
	{
		BP_RUN_LOG_ERR(IVS_XML_INVALID, "xmlReq.FindElemEx(\"Content/Linkage/AlarmInCode\") fail", "NA");
		return IVS_XML_INVALID;
	}
	const IVS_CHAR* pAlarmInCode = xmlReq.GetElemValue();
	IVS_CHAR szAlarmInCode[IVS_ALARM_CODE_LEN + 1] = {0};
	IVS_CHAR szDomainCode[IVS_DOMAIN_CODE_LEN + 1] = {0};
	IVS_INT32 iRet = CXmlProcess::ParseDevCode(pAlarmInCode, szAlarmInCode, szDomainCode);
	strDomainCode = szDomainCode;
	xmlReq.ModifyElemValue(szAlarmInCode);
	// Content�ڵ�������DomainCode�ڵ�
	IVS_UINT32 xmlLen = 0;
	const IVS_CHAR* pReq = xmlReq.GetXMLStream(xmlLen);
	CHECK_POINTER(pReq, IVS_FAIL);
	iRet = AddDomainCode4XML(szDomainCode, "Content", "DomainCode", pReq, xmlModified);
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet, "AddDomainCode4XML fail", "NA");
		return iRet;
	}
	// AlarmInType�ڵ������DevDomainCode�ڵ�
	pReq = xmlModified.GetXMLStream(xmlLen);
	CHECK_POINTER(pReq, IVS_FAIL);
	iRet = AddDomainCode4XML(szDomainCode, "Content/Linkage/AlarmInType", "DevDomainCode", pReq, xml);
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet, "AddDomainCode4XML fail", "NA");
		return iRet;
	}
	return iRet;
}

// ��ѯ�澯�����б��޸���Ӧ��Ϣ�е�AlarmInCode
IVS_INT32 CAlarmLinkagrXMLProcess::GetAlarmInCode(const IVS_CHAR* pReqXML, IVS_CHAR*& pRspXml)
{
	CHECK_POINTER(pReqXML, IVS_OPERATE_MEMORY_ERROR);
	IVS_DEBUG_TRACE("");
	// ������xml���ҵ�AlarmInCode�������������
	CXml xml;
	if (!xml.Parse(pReqXML))
	{
		BP_RUN_LOG_ERR(IVS_XML_INVALID, "xml.Parse(pReqXML)", "NA");
		return IVS_XML_INVALID;
	}
	if (!xml.FindElemEx("Content/AlarmInCode"))
	{
		BP_RUN_LOG_ERR(IVS_XML_INVALID, "xml.FindElemEx(Content/AlarmInCode)", "NA");
		return IVS_XML_INVALID;
	}

	const IVS_CHAR* pAlarmInCode = xml.GetElemValue();
	IVS_CHAR szAlarmInCode[IVS_ALARM_CODE_LEN + 1] = {0};
	IVS_CHAR szDomainCode[IVS_DOMAIN_CODE_LEN + 1] = {0};	
	IVS_INT32 iRet = CXmlProcess::ParseDevCode(pAlarmInCode, szAlarmInCode, szDomainCode);

	// ������Ӧ��Ϣ��AlarmInCode������#�������
	CXml xmlRsp;
	if (!xmlRsp.Parse(pRspXml))
	{
		BP_RUN_LOG_ERR(IVS_SMU_XML_INVALID, "xml.Parse(pRspXml)", "NA");
		return IVS_SMU_XML_INVALID;
	}

	if (!xmlRsp.FindElem("Content"))
	{
		BP_RUN_LOG_ERR(IVS_SMU_XML_INVALID, "xml.FindElemEx(Content/LinkageList)", "NA");
		return IVS_SMU_XML_INVALID;
	}
	(void)xmlRsp.IntoElem();
	if (!xmlRsp.FindElem("LinkageList"))
	{
		BP_RUN_LOG_INF("Get NA Info", "NA");
		return iRet;
	}
	(void)xmlRsp.IntoElem();
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
		std::string strDomainCode;
        const IVS_CHAR* pDomainCode = xmlRsp.GetElemValue();
        if (NULL != pDomainCode)
        {
            strDomainCode = pDomainCode;
        }

		if (!xmlRsp.FindElem("AlarmInCode"))
		{
			BP_RUN_LOG_ERR(IVS_SMU_XML_INVALID, "xml.FindElemEx(AlarmInCode)", "NA");
			return IVS_SMU_XML_INVALID;
		}
		std::string strAlarmInCode;
        pAlarmInCode = xmlRsp.GetElemValue();
        if (NULL != pAlarmInCode)
        {
            strAlarmInCode = pAlarmInCode;
        }
		if (!strAlarmInCode.empty())
		{
			strAlarmInCode.append("#");
			strAlarmInCode.append(strDomainCode);
			xmlRsp.ModifyElemValue(strAlarmInCode.c_str());
		}	
		xmlRsp.OutOfElem();
		if (!xmlRsp.NextElem())
		{
			break;
		}
	}	

	IVS_UINT32 uiXmlLen = 0;
	const IVS_CHAR* pRsp = xmlRsp.GetXMLStream(uiXmlLen);
	IVS_DELETE(pRspXml, MUILI);
    CHECK_POINTER(pRsp,IVS_SMU_XML_INVALID);//���ж�pRsp�Ƿ�ΪNULL���ٵ���strlen
	IVS_CHAR* pResultXml = IVS_NEW(pResultXml, strlen(pRsp) + 1);
	memset(pResultXml, 0x0, strlen(pRsp) + 1);
	if (!CToolsHelp::Memcpy(pResultXml, strlen(pRsp) + 1, pRsp, strlen(pRsp) + 1))
	{
		IVS_DELETE(pResultXml, MUILI);
		BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "Copy pRsp to pResultXml fail", "NA");
		return IVS_ALLOC_MEMORY_ERROR;
	}
	pRspXml = pResultXml;
	return iRet;
}

// �޸�������Ϣ�е�AlarmInCode��DevCode(ȥ��#�����)����������DomainCode��DevDomainCode�ڵ�(�������޸���������)
IVS_INT32 CAlarmLinkagrXMLProcess::SetAlarmInCodeValue(const IVS_CHAR* pReqXml, CXml& xml, std::string& strDomainCode)
{
	CHECK_POINTER(pReqXml, IVS_OPERATE_MEMORY_ERROR);	
	IVS_DEBUG_TRACE("");
	IVS_UINT32 xmlLen = 0;
	CXml xmlReq;//�޸��ڴ�й¶���⣨�õ����ս��ǰ����ֱ�Ӳ������xml��
	CXml xmlModified;
	if (!xmlReq.Parse(pReqXml))
	{
		BP_RUN_LOG_ERR(IVS_XML_INVALID, "xmlReq.Parse(pReqXml)", "NA");
		return IVS_XML_INVALID;
	}

	IVS_CHAR szAlarmInCode[IVS_ALARM_CODE_LEN + 1] = {0};
	IVS_CHAR szDomainCode[IVS_DOMAIN_CODE_LEN + 1] = {0};
	// �޸�AlarmInCode�� ��ȡ��ϢĿ�������
	if (!xmlReq.FindElemEx("Content/Linkage/AlarmInCode"))
	{
		BP_RUN_LOG_ERR(IVS_XML_INVALID, "xmlReq.FindElemEx(Content/Linkage/AlarmInCode)", "NA");
		return IVS_XML_INVALID;
	}

	const IVS_CHAR* pElemValue = xmlReq.GetElemValue();	
	IVS_INT32 iRet = CXmlProcess::ParseDevCode(pElemValue, szAlarmInCode, szDomainCode);
	if (NULL != pElemValue)
	{
		xmlReq.ModifyElemValue(szAlarmInCode);
	}
	// �ڵ�AlarmInCode�����DevDomainCode�ڵ�
	const IVS_CHAR* pReq = xmlReq.GetXMLStream(xmlLen);
	CHECK_POINTER(pReq, IVS_FAIL);
	iRet = AddDomainCode4XML(szDomainCode, "Content/Linkage/AlarmInType", "DevDomainCode", pReq, xmlModified);
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet, "AddDomainCode4XML fail", "NA");
		return iRet;
	}
	strDomainCode = szDomainCode;

	// �޸������豸��DevCode ���DevCode�����DevDomainCode�ڵ�
	if (!xmlModified.FindElemEx("Content/Linkage/ActionList"))
	{
		BP_RUN_LOG_ERR(IVS_XML_INVALID, "xmlModified.FindElemEx(Content/Linkage/ActionList)", "NA");
		return IVS_XML_INVALID;
	}
	if (!xmlModified.FindElem("Action"))
	{
		BP_RUN_LOG_ERR(IVS_XML_INVALID, "xmlModified.FindElemEx(DevInfo)", "NA");
		return IVS_XML_INVALID;
	}
	const IVS_CHAR* szElemValue = NULL;
	IVS_CHAR szDevCode[IVS_DEV_CODE_LEN + 1] = {0};
	do 
	{
		(void)xmlModified.IntoElem();
		if (!xmlModified.FindElem("DevList"))
		{
			xmlModified.OutOfElem();
			continue;
		}
		(void)xmlModified.IntoElem();
		if (!xmlModified.FindElem("DevInfo"))
		{
			BP_RUN_LOG_ERR(IVS_XML_INVALID, "xmlModified.FindElemEx(DevInfo)", "NA");
			return IVS_XML_INVALID;
		}
		do 
		{
			(void)xmlModified.IntoElem();
			GET_ELEM_VALUE_CHAR("DevCode", szElemValue, szDevCode, IVS_DEV_CODE_LEN, xmlModified);
			iRet = CXmlProcess::ParseDevCode(szDevCode, szAlarmInCode, szDomainCode);
			xmlModified.ModifyElemValue(szAlarmInCode);
			if (!xmlModified.FindElem("Param"))
			{
				BP_RUN_LOG_ERR(IVS_XML_INVALID, "xmlModified.FindElem(\"Param\") fail", "NA");
				return IVS_XML_INVALID;
			}
			std::string strParam = xmlModified.GetElemValue();
			// ɾ��Param�ڵ㣬������DevDomainCode��Param�ڵ�(ʹDevdomainCode��Param֮ǰ)
			(void)xmlModified.RemoveElem();
			// �ж������豸������Ƿ�Ϊ���������(�����豸��������ϢĿ����Ƚ�)
			(void)xmlModified.AddElem("DevDomainCode");
			if (0 == strcmp(strDomainCode.c_str(), szDomainCode))
			{				
				(void)xmlModified.SetElemValue("");
			}
			else
			{
				(void)xmlModified.SetElemValue(szDomainCode);
			}
			(void)xmlModified.AddElem("Param");
			(void)xmlModified.SetElemValue(strParam.c_str());

			xmlModified.OutOfElem();
		} while (xmlModified.NextElem());

		xmlModified.OutOfElem();
		xmlModified.OutOfElem();
	} while (xmlModified.NextElem());

	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet, "SetDevDomainCode4Xml fail", "NA");
		return iRet;
	}
	// Content�ڵ������DomainCode�ڵ�
	pReq = xmlModified.GetXMLStream(xmlLen);
	CHECK_POINTER(pReq, IVS_FAIL);
	iRet = AddDomainCode4XML(strDomainCode.c_str(), "Content", "DomainCode", pReq, xml);
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet, "AddDomainCode4XML fail", "NA");
		return iRet;
	}
	return iRet;
}

// ��ѯ�澯���������б�ƴװ����xml����ȡ�����
IVS_INT32 CAlarmLinkagrXMLProcess::GetAlarmLinkageListGetXml(const IVS_CHAR* pReqXml, CXml& xml, std::string& strDomainCode)
{
	CHECK_POINTER(pReqXml, IVS_PARA_INVALID);
	IVS_DEBUG_TRACE("");
	CXml xmlReq;//�޸��ڴ�й¶���⣨�õ����ս��ǰ����ֱ�Ӳ������xml��
	if (!xmlReq.Parse(pReqXml))
	{
		BP_RUN_LOG_ERR(IVS_XML_INVALID, "xmlReq.Parse(pReqXml)", "NA");
		return IVS_XML_INVALID;
	}
	if (!xmlReq.FindElemEx("Content/AlarmInCode"))
	{
		BP_RUN_LOG_ERR(IVS_XML_INVALID, "GetAlarmLinkageListGetXml", "xmlReq.FindElemEx(pPath)");
		return IVS_XML_INVALID;
	}
	// �ҵ�AlarmInCode����ֲ��޸�AlarmInCode�ڵ��ֵ
	const IVS_CHAR* pElemValue = xmlReq.GetElemValue();
	IVS_CHAR szAlarmInCode[IVS_ALARM_CODE_LEN + 1] = {0};
	IVS_CHAR szDomainCode[IVS_DOMAIN_CODE_LEN + 1] = {0};
	IVS_INT32 iRet = CXmlProcess::ParseDevCode(pElemValue, szAlarmInCode, szDomainCode);
	strDomainCode = szDomainCode;
	xmlReq.ModifyElemValue(szAlarmInCode);
	// ����DevDomainCode�ڵ�
	(void)xmlReq.AddElem("DevDomainCode");
	(void)xmlReq.SetElemValue(szDomainCode);
	// Content�����DomainCode�ڵ�
	IVS_UINT32 xmlLen = 0;
	const IVS_CHAR* pReq = xmlReq.GetXMLStream(xmlLen);
	CHECK_POINTER(pReq, IVS_FAIL);
	iRet = AddDomainCode4XML(szDomainCode, "Content", "DomainCode", pReq, xml);
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet, "AddDomainCode4XML Add DomainCode under Content fail", "NA");
		return iRet;
	}
	return iRet;
}
// ��xml�в���ڵ�
IVS_INT32 CAlarmLinkagrXMLProcess::AddDomainCode4XML(const char* pDomainCode, const IVS_CHAR* pDomainCodePath, const IVS_CHAR* pValueName, const IVS_CHAR* pReqXml, CXml& xml)
{
	CHECK_POINTER(pReqXml, IVS_OPERATE_MEMORY_ERROR);
	
	IVS_INT32 iRet = IVS_SUCCEED;
	// ����LoginInfo�Ľڵ�
	TiXmlElement* pDomainCodeElem = NULL;
	try
	{
		pDomainCodeElem = new TiXmlElement(pValueName);
	}
	catch(...)
	{
		BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "new TiXmlElement(LoginInfo) is error", "NA");
		pDomainCodeElem = NULL;
		return IVS_ALLOC_MEMORY_ERROR;
	}

	TiXmlText* pXmlText = NULL;
	try
	{
		pXmlText = new TiXmlText(pDomainCode);
	}
	catch(...)
	{
		BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "new TiXmlText(pLoginID) is error", "NA");
		IVS_DELETE(pDomainCodeElem, 0);
		pXmlText = NULL;
		return IVS_ALLOC_MEMORY_ERROR;
	}
	pDomainCodeElem->LinkEndChild(pXmlText);
	// ����XML
	TiXmlDocument* pXmlDoc = IVS_NEW(pXmlDoc);
	if (NULL == pXmlDoc)
	{
		BP_RUN_LOG_INF("NULL == pXmlDoc", "NA");
		pXmlDoc = NULL;
		IVS_DELETE(pDomainCodeElem, 0);
		return IVS_FAIL; //lint !e429 pXmlText���ͷ�pDomainCodeElemʱһ���ͷ�
	}
	//CHECK_POINTER(pXmlDoc, IVS_FAIL);
	(void)(pXmlDoc->Parse(pReqXml, NULL, TIXML_ENCODING_LEGACY));
	if (0 != pXmlDoc->Error())
	{
		BP_RUN_LOG_ERR(IVS_FAIL, "parse xml failed", "NA");
		IVS_DELETE(pXmlDoc);
		IVS_DELETE(pDomainCodeElem, 0);
		return IVS_FAIL; //lint !e429
	}

	do 
	{		
		TiXmlElement* pRootElem = pXmlDoc->RootElement();
		if (strcmp(pDomainCodePath, "Content") == 0)
		{
			// ����content�ڵ㣬������ڵ����content�Ͳ�Ҫ���ˣ������Ǹ�������ȱ��
			//TiXmlElement* pRootElem = pXmlDoc->RootElement();
			if (NULL == pRootElem)
			{
				BP_RUN_LOG_INF("NULL == pRootElem", "NA");
				iRet = IVS_FAIL;
				break;
			}
			TiXmlElement* pContent = NULL;
			if (strcmp(pRootElem->Value(), "Content") == 0)
			{
				pContent = pXmlDoc->RootElement();
			}
			else
			{
				pContent = CToolsHelp::FindElementByPath(pXmlDoc->RootElement()->Parent(),"Content");
			}
			if (NULL == pContent)
			{
				BP_RUN_LOG_INF("NULL == pContent", "NA");
				iRet = IVS_FAIL;
				break;
			}

			// ���û���ӽڵ� ֱ�ӷ��ش�
			TiXmlElement* pContentFirstChild = pContent->FirstChildElement();
			if (NULL == pContentFirstChild)
			{
				BP_RUN_LOG_INF("NULL == pContentFirstChild", "NA");
				iRet = IVS_FAIL;
				break;
			}
			// ���ҵ��ĵ�һ���ڵ�֮ǰ����logininfo
			if (NULL == pContent->InsertBeforeChild(pContentFirstChild, *pDomainCodeElem))
			{
				BP_RUN_LOG_ERR(IVS_FAIL, "InsertBeforeChild error", "NA");
				iRet = IVS_FAIL;
				break;
			}
		}
		else
		{
			TiXmlElement* pElem = NULL;
			pElem = CToolsHelp::FindElementByPath(pXmlDoc->RootElement()->Parent(),pDomainCodePath);
			if ((NULL == pElem) || (NULL == pElem->Parent()->ToElement()->InsertAfterChild(pElem, *pDomainCodeElem)))
			{
				BP_RUN_LOG_ERR(IVS_FAIL, "InsertAfterChild  or FindElementByPath error", "NA");
				iRet = IVS_FAIL;
				break;
			}
		}
	} while (false); //lint !e717
	

	// ��ƴװ�õ�xml��Cxml�������棬�����ⲿһ��
	TiXmlPrinter xmlPrinter;
	xmlPrinter.SetStreamPrinting();		

	if (!pXmlDoc->Accept(&xmlPrinter))
	{
		BP_RUN_LOG_ERR(IVS_FAIL, "print error", "NA");
		IVS_DELETE(pXmlDoc);
		IVS_DELETE(pDomainCodeElem, 0);
		return IVS_FAIL;//lint !e429
	}

	(void)xml.Parse(xmlPrinter.CStr());

	IVS_DELETE(pXmlDoc);
	IVS_DELETE(pDomainCodeElem, 0);

	return iRet;//lint !e429
}//lint !e429

//
IVS_INT32 CAlarmLinkagrXMLProcess::AppendDevCode(std::string& strNSSRsq, const IVS_CHAR* pPath, CXml& xml, const std::string& strDomainCode)
{
	IVS_DEBUG_TRACE("");
	CHECK_POINTER(pPath, IVS_PARA_INVALID);

	if (!xml.Parse(strNSSRsq.c_str()))
	{
		BP_RUN_LOG_ERR(IVS_SMU_XML_INVALID, "xml.Parse(strNSSRsq.c_str())", "NA");
		return IVS_SMU_XML_INVALID;
	}
	if (!xml.FindElemEx(pPath))
	{
		BP_RUN_LOG_ERR(IVS_SMU_XML_INVALID, "xml.FindElemEx(pPath)", "NA");
		return IVS_SMU_XML_INVALID;
	}
	if (!xml.FindElem("Action"))
	{
		BP_RUN_LOG_ERR(IVS_SMU_XML_INVALID, "xml.FindElem(Action)", "NA");
		return IVS_SMU_XML_INVALID;
	}

	const IVS_CHAR* szElemValue = NULL;
	IVS_CHAR szDevCode[IVS_DEV_CODE_LEN + 1] = {0};
	IVS_CHAR szDomainCode[IVS_DOMAIN_CODE_LEN + 1] = {0};
	std::string strBigCode;
	do 
	{
		(void)xml.IntoElem();
		// DevList��O�ֶΣ�����û�У�����Action��M�ֶΣ��Ҳ���DevList��ѭ������Ҫ����
		if (!xml.FindElem("DevList"))
		{
			xml.OutOfElem();
			continue;
		}
		(void)xml.IntoElem();
		if (!xml.FindElem("DevInfo"))
		{
			BP_RUN_LOG_ERR(IVS_SMU_XML_INVALID, "xml.FindElemEx(DevInfo) fail", "NA");
			return IVS_SMU_XML_INVALID;
		}
		do 
		{
			(void)xml.IntoElem();
			memset(szDomainCode,0,IVS_DOMAIN_CODE_LEN+1);
			GET_ELEM_VALUE_CHAR("DevDomainCode", szElemValue, szDomainCode, IVS_DOMAIN_CODE_LEN, xml);
			if (0 == strcmp(szDomainCode, ""))
			{
				(void)CToolsHelp::Memcpy(szDomainCode, IVS_DOMAIN_CODE_LEN, strDomainCode.c_str(), strDomainCode.size());
			}
			GET_ELEM_VALUE_CHAR("DevCode", szElemValue, szDevCode, IVS_DEV_CODE_LEN, xml);
			strBigCode.assign(szDevCode);
			strBigCode.append("#").append(szDomainCode);
			xml.ModifyElemValue(strBigCode.c_str());
			xml.OutOfElem();
		} while (xml.NextElem());

		xml.OutOfElem();
		xml.OutOfElem();
	} while (xml.NextElem());
	return IVS_SUCCEED;
}//lint !e1764

