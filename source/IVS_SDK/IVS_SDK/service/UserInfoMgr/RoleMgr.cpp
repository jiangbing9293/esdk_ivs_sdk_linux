#include "RoleMgr.h"
#include "UserMgr.h"
#include "nss_mt.h"
#include "NSSOperator.h"
#include "UserInfoMgrXMLProcess.h"
#include "IVS_Trace.h"

CRoleMgr::CRoleMgr(void)
    :m_UserMgr(NULL)
{
}

CRoleMgr::~CRoleMgr(void)
{
    m_UserMgr = NULL;
}

void CRoleMgr::SetUserMgr( CUserMgr *pUserMgr)
{
    m_UserMgr = pUserMgr;
}

// ���ӽ�ɫ
IVS_INT32 CRoleMgr::AddRole(const IVS_ROLE_INFO* pRoleInfo, IVS_UINT32* puiRoleID)
{
    //��У��
    CHECK_POINTER(m_UserMgr, IVS_OPERATE_MEMORY_ERROR);
    CHECK_POINTER(pRoleInfo, IVS_PARA_INVALID);

    IVS_DEBUG_TRACE("");

    IVS_INT32 iRet = IVS_FAIL;
    CXml xmlReq;
    std::string strDomainCode;
    // ��ֹ�����ﵽ�ٽ�ֵ�޽�������\0�� modify by xiongfeng
    char cDomainCodeTmp[IVS_DOMAIN_CODE_LEN + 1] = {0};
    (void)CToolsHelp::Memcpy(cDomainCodeTmp, IVS_DOMAIN_CODE_LEN, pRoleInfo->cDomainCode, IVS_DOMAIN_CODE_LEN);
    strDomainCode = cDomainCodeTmp;
    if (strDomainCode.empty())
    {
        m_UserMgr->GetDomainCode(strDomainCode);
    }

    CUserInfoMgrXMLProcess::GetRoleXML(strDomainCode.c_str(), pRoleInfo, xmlReq);
    IVS_UINT32 xmlLen = 0;
    const IVS_CHAR* pReq = xmlReq.GetXMLStream(xmlLen);
    CHECK_POINTER(pReq, IVS_OPERATE_MEMORY_ERROR);

    // ��������������Ϣ��������
    CSendNssMsgInfo sendNssMsgInfo;
    sendNssMsgInfo.SetNeedXml(TYPE_NSS_XML);
    sendNssMsgInfo.SetNetElemType(NET_ELE_SMU_NSS);
    sendNssMsgInfo.SetReqID(NSS_ADD_ROLE_REQ);
    sendNssMsgInfo.SetReqData(pReq);
    sendNssMsgInfo.SetDomainCode(strDomainCode);
    //��������
    std::string strRsp;
    IVS_INT32 iNeedRedirect = IVS_FAIL;
    iRet = m_UserMgr->SendCmd(sendNssMsgInfo,strRsp,iNeedRedirect);
    if(IVS_SUCCEED != iRet)
    {
        BP_RUN_LOG_ERR(iRet, "Add Role","SendCmd to SMU Return Failed");
        return  iRet;
    }

    //CHECK_POINTER(strRsp.c_str(), IVS_OPERATE_MEMORY_ERROR);
    CXml xmlRsp;
    if(!xmlRsp.Parse(strRsp.c_str()))
    {
        BP_RUN_LOG_ERR(IVS_XML_INVALID, "Add Role", "xml parsing error");

        return IVS_XML_INVALID;
    }
    iRet = CUserInfoMgrXMLProcess::AddRoleParseXML(xmlRsp, puiRoleID);

    return iRet;
}

// ɾ����ɫ
IVS_INT32 CRoleMgr::DeleteRole(const IVS_CHAR* pDomainCode, IVS_UINT32 uiRoleID)
{
    CHECK_POINTER(m_UserMgr, IVS_OPERATE_MEMORY_ERROR);
    IVS_DEBUG_TRACE("");

    std::string strDomainCode;
    if(NULL == pDomainCode || '\0' == pDomainCode[0])
    {
        m_UserMgr->GetDomainCode(strDomainCode);
    }
    else
    {
        strDomainCode = pDomainCode;
    }

    IVS_ROLE_INFO pRoleInfo;
    memset(&pRoleInfo, 0, sizeof(IVS_ROLE_INFO));
    pRoleInfo.uiRoleID = uiRoleID;

    //ƴ��������Ϣ
    IVS_INT32 iRet = IVS_FAIL;
    CXml xmlReq;
    CUserInfoMgrXMLProcess::GetRoleXML(strDomainCode.c_str(), &pRoleInfo, xmlReq);
    unsigned int xmlLen = 0;
    const IVS_CHAR* pReq = xmlReq.GetXMLStream(xmlLen);
    CHECK_POINTER(pReq, IVS_OPERATE_MEMORY_ERROR);

    // ��������������Ϣ��������
    CSendNssMsgInfo sendNssMsgInfo;
    sendNssMsgInfo.SetNeedXml(TYPE_NSS_NOXML);
    sendNssMsgInfo.SetNetElemType(NET_ELE_SMU_NSS);
    sendNssMsgInfo.SetReqID(NSS_DEL_ROLE_REQ);
    sendNssMsgInfo.SetReqData(pReq);
    sendNssMsgInfo.SetDomainCode(strDomainCode);
    std::string strpRsp;
    IVS_INT32 iNeedRedirect = IVS_FAIL;
    iRet = m_UserMgr->SendCmd(sendNssMsgInfo,strpRsp,iNeedRedirect);

    return iRet;
}

// �޸Ľ�ɫ
IVS_INT32 CRoleMgr::ModifyRole(const IVS_ROLE_INFO* pRoleInfo)
{
    CHECK_POINTER(m_UserMgr, IVS_OPERATE_MEMORY_ERROR);
    CHECK_POINTER(pRoleInfo, IVS_PARA_INVALID);
    IVS_DEBUG_TRACE("");

    // ��ֹ�����ﵽ�ٽ�ֵ�޽�������\0�� modify by xiongfeng
    std::string strDomainCode;
    char cDomainCodeTmp[IVS_DOMAIN_CODE_LEN + 1] = {0};
    (void)CToolsHelp::Memcpy(cDomainCodeTmp, IVS_DOMAIN_CODE_LEN, pRoleInfo->cDomainCode, IVS_DOMAIN_CODE_LEN);
    strDomainCode = cDomainCodeTmp;
    if (strDomainCode.empty())
    {
        m_UserMgr->GetDomainCode(strDomainCode);
    }

    //ƴ���޸Ľ�ɫxml����
    IVS_INT32 iRet = IVS_FAIL;
    CXml xmlReq;
    CUserInfoMgrXMLProcess::GetRoleXML(strDomainCode.c_str(), pRoleInfo, xmlReq);
    unsigned int xmlLen = 0;
    const IVS_CHAR* pReq = xmlReq.GetXMLStream(xmlLen);
    CHECK_POINTER(pReq, IVS_OPERATE_MEMORY_ERROR);

    // ��������������Ϣ��������
    CSendNssMsgInfo sendNssMsgInfo;
    sendNssMsgInfo.SetNeedXml(TYPE_NSS_NOXML);
    sendNssMsgInfo.SetNetElemType(NET_ELE_SMU_NSS);
    sendNssMsgInfo.SetReqID(NSS_MOD_ROLE_REQ);
    sendNssMsgInfo.SetReqData(pReq);
    sendNssMsgInfo.SetDomainCode(strDomainCode);
    std::string strpRsp;
    IVS_INT32 iNeedRedirect = IVS_FAIL;
    iRet = m_UserMgr->SendCmd(sendNssMsgInfo,strpRsp,iNeedRedirect);

    return iRet;
}

// ��ѯ��ɫ�б�
IVS_INT32 CRoleMgr::GetRoleList(const IVS_CHAR* pDomainCode, const IVS_INDEX_RANGE* pIndexRange, IVS_ROLE_INFO_LIST* pRoleInfoList, IVS_UINT32 /*uiBufferSize*/)
{
    CHECK_POINTER(m_UserMgr, IVS_OPERATE_MEMORY_ERROR);
    CHECK_POINTER(pIndexRange, IVS_PARA_INVALID);
    CHECK_POINTER(pRoleInfoList, IVS_PARA_INVALID);
    IVS_DEBUG_TRACE("");

    std::string strDomainCode;
    if ( NULL == pDomainCode || '\0' == pDomainCode[0])
    {
        m_UserMgr->GetDomainCode(strDomainCode);
    }
    else
    {
        strDomainCode = pDomainCode;
    }

    //��������xml
    IVS_INT32 iRet = IVS_FAIL;
    CXml xmlReq;
    CUserInfoMgrXMLProcess::CommDomainCodeXml(strDomainCode.c_str(), xmlReq);
    unsigned int xmlLen = 0;
    const IVS_CHAR* pReq = xmlReq.GetXMLStream(xmlLen);
    CHECK_POINTER(pReq, IVS_OPERATE_MEMORY_ERROR);

    // ��������������Ϣ��������
    CSendNssMsgInfo sendNssMsgInfo;
    sendNssMsgInfo.SetNeedXml(TYPE_NSS_XML);
    sendNssMsgInfo.SetNetElemType(NET_ELE_SMU_NSS);
    sendNssMsgInfo.SetReqID(NSS_GET_ROLE_LIST_REQ);
    sendNssMsgInfo.SetReqData(pReq);
    sendNssMsgInfo.SetDomainCode(strDomainCode);
    std::string strpRsp;
    IVS_INT32 iNeedRedirect = IVS_FAIL;
    iRet = m_UserMgr->SendCmd(sendNssMsgInfo,strpRsp,iNeedRedirect);
    if(IVS_SUCCEED != iRet)
    {
        BP_RUN_LOG_ERR(iRet, "Get Main DeviceList fail","SendCmd to SMU Return Failed");
        return  iRet;
    }

    //CHECK_POINTER(strpRsp.c_str(), IVS_XML_INVALID);
    CXml xmlRsp;
    if(!xmlRsp.Parse(strpRsp.c_str()))
    {
        BP_RUN_LOG_ERR(IVS_XML_INVALID, "Get Role List", "xml parsing error");
        return IVS_XML_INVALID;
    }
    iRet = CUserInfoMgrXMLProcess::GetRoleListParseXML(xmlRsp, pRoleInfoList);

    return iRet;
}

// ���ݽ�ɫID��ȡ��ɫ��ϸ��Ϣ
IVS_INT32 CRoleMgr::GetRoleInfoByRoleID(const IVS_CHAR* pDomainCode, IVS_UINT32 uiRoleID, IVS_ROLE_INFO* pRoleInfo)
{
    CHECK_POINTER(m_UserMgr, IVS_OPERATE_MEMORY_ERROR);
    CHECK_POINTER(pRoleInfo, IVS_PARA_INVALID);
    IVS_DEBUG_TRACE("");

    std::string domainCode;
    if(NULL == pDomainCode)
    {
        m_UserMgr->GetDomainCode(domainCode);
    }
    else
    {
        domainCode = pDomainCode;
    }

    IVS_INT32 iRet = IVS_FAIL;
    CXml xmlReq;
    CUserInfoMgrXMLProcess::GetRoleInfoByRoleIDGetXML(domainCode.c_str(), uiRoleID, xmlReq);
    unsigned int xmlLen = 0;
    const IVS_CHAR* pReq = xmlReq.GetXMLStream(xmlLen);
    CHECK_POINTER(pReq, IVS_OPERATE_MEMORY_ERROR);

    // ��������������Ϣ��������
    CSendNssMsgInfo sendNssMsgInfo;
    sendNssMsgInfo.SetNeedXml(TYPE_NSS_XML);
    sendNssMsgInfo.SetNetElemType(NET_ELE_SMU_NSS);
    sendNssMsgInfo.SetReqID(NSS_GET_ROLE_BY_ID_REQ);
    sendNssMsgInfo.SetReqData(pReq);
    sendNssMsgInfo.SetDomainCode(domainCode);
    std::string strpRsp;
    IVS_INT32 iNeedRedirect = IVS_FAIL;
    iRet = m_UserMgr->SendCmd(sendNssMsgInfo,strpRsp,iNeedRedirect);
    if(IVS_SUCCEED != iRet)
    {
        BP_RUN_LOG_ERR(iRet, "Get Role Info By RoleID","SendCmd to SMU Return Failed");
        return  iRet;
    }

    //CHECK_POINTER(strpRsp.c_str(), IVS_OPERATE_MEMORY_ERROR);

    CXml xmlRsp;
    if(!xmlRsp.Parse(strpRsp.c_str()))
    {
        BP_RUN_LOG_ERR(IVS_XML_INVALID, "Get RoleInfo By RoleID", "xml parsing error");
        return IVS_XML_INVALID;
    }
    iRet = CUserInfoMgrXMLProcess::GetRoleInfoParseXML(xmlRsp, pRoleInfo); 

    return iRet;
}

// �����û�ID��ȡ��ɫ��ϸ��Ϣ
IVS_INT32 CRoleMgr::GetRoleInfoByUserID(const IVS_CHAR* pDomainCode, IVS_UINT32 uiUserID, IVS_ROLE_INFO* pRoleInfo)
{
    CHECK_POINTER(m_UserMgr, IVS_OPERATE_MEMORY_ERROR);
    CHECK_POINTER(pRoleInfo, IVS_PARA_INVALID);
    IVS_DEBUG_TRACE("");

    std::string sDomainCode;
    if(NULL == pDomainCode)
    {
        m_UserMgr->GetDomainCode(sDomainCode);
    }
    else
    {
        sDomainCode = pDomainCode;
    }

    IVS_INT32 iRet = IVS_FAIL;
    CXml xmlReq;
    CUserInfoMgrXMLProcess::GetUserInfoGetXML(sDomainCode.c_str(), uiUserID, xmlReq);
    unsigned int xmlLen = 0;
    const IVS_CHAR* pReq = xmlReq.GetXMLStream(xmlLen);
    CHECK_POINTER(pReq, IVS_OPERATE_MEMORY_ERROR);

    // ��������������Ϣ��������
    CSendNssMsgInfo sendNssMsgInfo;
    sendNssMsgInfo.SetNeedXml(TYPE_NSS_XML);
    sendNssMsgInfo.SetNetElemType(NET_ELE_SMU_NSS);
    sendNssMsgInfo.SetReqID(NSS_GET_USER_ROLE_REQ);
    sendNssMsgInfo.SetReqData(pReq);
    sendNssMsgInfo.SetDomainCode(sDomainCode);
    std::string strpRsp;
    IVS_INT32 iNeedRedirect = IVS_FAIL;
    iRet = m_UserMgr->SendCmd(sendNssMsgInfo,strpRsp,iNeedRedirect);
    if(IVS_SUCCEED != iRet)
    {
        BP_RUN_LOG_ERR(iRet, "Get Role Info By UserID","SendCmd to SMU Return Failed");
        return  iRet;
    }

    //CHECK_POINTER(strpRsp.c_str(), IVS_OPERATE_MEMORY_ERROR);

    CXml xmlRsp;
    if(!xmlRsp.Parse(strpRsp.c_str()))
    {
        BP_RUN_LOG_ERR(IVS_XML_INVALID, "Get RoleInfo By UserID", "xml parsing error");
        return IVS_XML_INVALID;
    }
    iRet = CUserInfoMgrXMLProcess::GetRoleInfoParseXML(xmlRsp, pRoleInfo); 

    return iRet;
}

// ��ȡĬ���˺Ź���
IVS_INT32 CRoleMgr::GetDefaultRoleAccountRule(const IVS_CHAR* pDomainCode, IVS_ROLE_ACCOUNT_RULE* pRoleAccountRule)
{
    CHECK_POINTER(m_UserMgr, IVS_OPERATE_MEMORY_ERROR);
    IVS_DEBUG_TRACE("");

    std::string strDomainCode;
    if(NULL == pDomainCode || '\0' == pDomainCode[0])
    {
        m_UserMgr->GetDomainCode(strDomainCode);
    }
    else
    {
        strDomainCode = pDomainCode;
    }

    IVS_INT32 iRet = IVS_FAIL;
    CXml xmlReq;
    CUserInfoMgrXMLProcess::CommDomainCodeXml(strDomainCode.c_str(), xmlReq);
    unsigned int xmlLen = 0;
    const IVS_CHAR* pReq = xmlReq.GetXMLStream(xmlLen);
    CHECK_POINTER(pReq, IVS_OPERATE_MEMORY_ERROR);

    // ��������������Ϣ��������
    CSendNssMsgInfo sendNssMsgInfo;
    sendNssMsgInfo.SetNeedXml(TYPE_NSS_XML);
    sendNssMsgInfo.SetNetElemType(NET_ELE_SMU_NSS);
    sendNssMsgInfo.SetReqID(NSS_GET_DEFAULT_RULE_REQ);
    sendNssMsgInfo.SetReqData(pReq);
    sendNssMsgInfo.SetDomainCode(strDomainCode);
    std::string strResponse;
    IVS_INT32 iNeedRedirect = IVS_FAIL;
    iRet = m_UserMgr->SendCmd(sendNssMsgInfo,strResponse,iNeedRedirect);
    if(IVS_SUCCEED != iRet)
    {
        BP_RUN_LOG_ERR(iRet, "Get Default Role Account Rule","SendCmd to SMU Return Failed");
        return  iRet;
    }

    //CHECK_POINTER(strResponse.c_str(), IVS_OPERATE_MEMORY_ERROR);

    CXml xmlRsp;
    if(!xmlRsp.Parse(strResponse.c_str()))
    {
        BP_RUN_LOG_ERR(IVS_XML_INVALID, "Get Default Role Account Rule", "xml parsing error");
        return IVS_XML_INVALID;
    }
    iRet = CUserInfoMgrXMLProcess::GetDefaultRoleAccountRuleParseXML(xmlRsp, pRoleAccountRule); 

    return iRet;
}

// ��ȡ�û�Ȩ����Ϣ
IVS_INT32 CRoleMgr::GetOperationRightGroup(const IVS_CHAR* pDomainCode, IVS_OPERATION_RIGHT_GROUP_LIST* pRightGroupList)
{
    CHECK_POINTER(m_UserMgr, IVS_OPERATE_MEMORY_ERROR);
    IVS_DEBUG_TRACE("");

    std::string strDomainCode;
    if(NULL == pDomainCode || '\0' == pDomainCode[0])
    {
        m_UserMgr->GetDomainCode(strDomainCode);
    }
    else
    {
        strDomainCode = pDomainCode;
    }

    IVS_INT32 iRet = IVS_FAIL;
    CXml xmlReq;
    CUserInfoMgrXMLProcess::CommDomainCodeXml(strDomainCode.c_str(), xmlReq);
    unsigned int xmlLen = 0;
    const IVS_CHAR* pReq = xmlReq.GetXMLStream(xmlLen);
    CHECK_POINTER(pReq, IVS_OPERATE_MEMORY_ERROR);

    // ��������������Ϣ��������
    CSendNssMsgInfo sendNssMsgInfo;
    sendNssMsgInfo.SetNeedXml(TYPE_NSS_XML);
    sendNssMsgInfo.SetNetElemType(NET_ELE_SMU_NSS);
    sendNssMsgInfo.SetReqID(NSS_GET_ALL_OP_REQ);
    sendNssMsgInfo.SetReqData(pReq);
    sendNssMsgInfo.SetDomainCode(strDomainCode);
    std::string sRsp;
    IVS_INT32 iNeedRedirect = IVS_FAIL;
    iRet = m_UserMgr->SendCmd(sendNssMsgInfo,sRsp,iNeedRedirect);
    if(IVS_SUCCEED != iRet)
    {
        BP_RUN_LOG_ERR(iRet, "Get Operation Right Group","SendCmd to SMU Return Failed");
        return  iRet;
    }

    //CHECK_POINTER(sRsp.c_str(), IVS_OPERATE_MEMORY_ERROR);

    CXml xmlRsp;
    if(!xmlRsp.Parse(sRsp.c_str()))
    {
        BP_RUN_LOG_ERR(IVS_XML_INVALID, "Get Operation Right Group", "xml parsing error");
        return IVS_XML_INVALID;
    }
    iRet = CUserInfoMgrXMLProcess::GetOperationRightGroupParseXML(xmlRsp, pRightGroupList); 

    return iRet;
}

