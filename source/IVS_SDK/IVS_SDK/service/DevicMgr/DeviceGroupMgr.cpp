/********************************************************************
	filename	: 	DeviceGroupMgr.cpp
	author		:	gwx151720
	created		:	2012/11/01	
	description	:	�豸������࣬�ṩ��cuʹ��
	copyright	:	Copyright (C) 2012
	history		:	2012/11/01
*********************************************************************/
#include "DeviceGroupMgr.h"
#include "nss_mt.h"
#include "bp_log.h"
#include "UserMgr.h"
#include "Cmd.h"
#include "NSSOperator.h"
#include "DeviceMgrXMLProcess.h"
#include "IVS_Trace.h"

USING_NAMESPACE_BP

CDeviceGroupMgr::CDeviceGroupMgr(void):m_pUserMgr(NULL)
{
	
}


CDeviceGroupMgr::~CDeviceGroupMgr(void)
{
	m_pUserMgr = NULL;
}

void CDeviceGroupMgr::SetUserMgr(CUserMgr *pUserMgr)
{
	m_pUserMgr = pUserMgr;
}


// �����豸��
 IVS_INT32 CDeviceGroupMgr::AddDeviceGroup( const IVS_DEV_GROUP* pDevGroupInfo,IVS_UINT32* uiDevGroupID)const
 {
     CHECK_POINTER(uiDevGroupID, IVS_OPERATE_MEMORY_ERROR);
	 IVS_DEBUG_TRACE("");

     CXml xmlTemp;
     IVS_INT32 iRet = CDeviceMgrXMLProcess::AddDeviceGroupGetXML(pDevGroupInfo, xmlTemp);
     if (IVS_SUCCEED != iRet)
     {
         BP_RUN_LOG_ERR(iRet, "Add Device GroupGetXML fail", "NA");
         return iRet;
     }

	 IVS_CHAR cDomainCode[IVS_DOMAIN_CODE_LEN + 1] = {0};
	 if (!CToolsHelp::Memcpy(cDomainCode, IVS_DOMAIN_CODE_LEN, pDevGroupInfo->cDomainCode, IVS_DOMAIN_CODE_LEN))
	 {
		 BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "Memcpy(cDomainCode, IVS_DOMAIN_CODE_LEN, pDevGroupInfo->cDomainCode, IVS_DOMAIN_CODE_LEN) fail", "NA");
		 return IVS_ALLOC_MEMORY_ERROR;
	 }

	 iRet = CXmlProcess::GetCommSendMgr(xmlTemp, m_pUserMgr, NSS_ADD_DEV_GROUP_REQ, BUILSTRING, cDomainCode);
     if (IVS_SUCCEED != iRet)
     {           
         BP_RUN_LOG_ERR(iRet, "Get Comm BuildRet fail", "NA");
		 return iRet;
     }
	 iRet = CDeviceMgrXMLProcess::AddDeviceGroupParseXML(xmlTemp,uiDevGroupID);

	 return iRet;
 }


// ��ѯ�豸���б�
IVS_INT32 CDeviceGroupMgr::GetDeviceGroup(const IVS_CHAR* pDomainCode,
                                          const IVS_CHAR* pDevGroupCode,
                                          IVS_DEVICE_GROUP_LIST* pDeviceGroupList,
                                          IVS_UINT32 uiBufferSize)
{
    CHECK_POINTER(m_pUserMgr, IVS_OPERATE_MEMORY_ERROR);
    CHECK_POINTER(pDeviceGroupList, IVS_OPERATE_MEMORY_ERROR);
	IVS_DEBUG_TRACE("");

    // У���ڴ�
	IVS_INT32 iRet = IVS_FAIL;
	IVS_UINT32 uiSize = 0;
	for (IVS_INT32 i = 0; i != 2; i++)//0�Ǳ���1���ⲿ��
	{
		CXml xmlReq;
		(IVS_VOID)CDeviceMgrXMLProcess::GetDeviceGroupXML(i, pDomainCode, pDevGroupCode, xmlReq);

            IVS_UINT32 xmlLen = 0;
	    const IVS_CHAR* pReq = xmlReq.GetXMLStream(xmlLen);
	    CHECK_POINTER(pReq, IVS_OPERATE_MEMORY_ERROR);
           BP_RUN_LOG_INF("Get CommSendMgr", "send XML = %s", pReq);

	     // ��������������Ϣ��������
	    CSendNssMsgInfo sendNssMsgInfo;
	    sendNssMsgInfo.SetNeedXml(TYPE_NSS_XML);
	    sendNssMsgInfo.SetNetElemType(NET_ELE_SMU_NSS);
	    sendNssMsgInfo.SetReqID(NSS_GET_DEV_GROUP_LIST_REQ);
	    sendNssMsgInfo.SetReqData(pReq);	
	    sendNssMsgInfo.SetDomainCode(pDomainCode);
	    std::string strpRsp;
	    IVS_INT32 iNeedRedirect = IVS_FAIL;
	    iRet = m_pUserMgr->SendCmd(sendNssMsgInfo,strpRsp,iNeedRedirect);
           if (IVS_SUCCEED != iRet && 0 == i)
           {
                BP_RUN_LOG_ERR(iRet, "Get DeviceGroup fail", "SendCmd fail");
                return iRet;
           }           

           CXml xmlRsp;
           if (iRet == IVS_SUCCEED)
           {
                iRet = IVS_XML_INVALID;
                if (xmlRsp.Parse(strpRsp.c_str()))
                {
                    iRet = IVS_SUCCEED;
                }
           }

           if (IVS_SUCCEED != iRet && 0 == i)
           {
                return iRet;
           }
           
           IVS_INT32 iReTurn = iRet;
	    iRet = IVS_SUCCEED;
	    if (IVS_SUCCEED == iReTurn)
           {                   
                iRet = CDeviceMgrXMLProcess::GetDeviceGroupParseXML(uiSize, pDeviceGroupList, uiBufferSize, xmlRsp); 
           }
	}
	pDeviceGroupList->uiTotal = uiSize;

    return iRet;
}


// ɾ���豸��
IVS_INT32 CDeviceGroupMgr::DeleteDeviceGroup(const IVS_CHAR* pDomainCode,
                                             IVS_UINT32 uiDevGroupID)const
{
    CHECK_POINTER(pDomainCode, IVS_OPERATE_MEMORY_ERROR)
	IVS_DEBUG_TRACE("");

    CXml xmlReq;
    if (IVS_SUCCEED != CDeviceMgrXMLProcess::DeleteDeviceGroupGetXML(pDomainCode, uiDevGroupID, xmlReq))
    {
        BP_RUN_LOG_ERR(IVS_OPERATE_MEMORY_ERROR, "Delete Device Group fail","pointer is Null");
        return IVS_OPERATE_MEMORY_ERROR;
    }
	IVS_CHAR cDomainCode[IVS_DOMAIN_CODE_LEN + 1] = {0};
	if (!CToolsHelp::Memcpy(cDomainCode, IVS_DOMAIN_CODE_LEN, pDomainCode, IVS_DOMAIN_CODE_LEN))
	{
		BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "Memcpy(cDomainCode, IVS_DOMAIN_CODE_LEN, pDomainCode, IVS_DOMAIN_CODE_LEN) fail", "NA");
		return IVS_ALLOC_MEMORY_ERROR;
	}

	IVS_INT32 iRet = CXmlProcess::GetCommSendMgr(xmlReq, m_pUserMgr, NSS_DEL_DEV_GROUP_REQ, BUILDRET, cDomainCode);

    return iRet;
}


// �޸��豸������
IVS_INT32 CDeviceGroupMgr::ModifyDeviceGroupName(const IVS_CHAR* pDomainCode,
                                             IVS_UINT32 uiDevGroupID,
                                             const IVS_CHAR* pNewDevGroupName)const
{
    CHECK_POINTER(pDomainCode, IVS_OPERATE_MEMORY_ERROR);
    CHECK_POINTER(pNewDevGroupName, IVS_OPERATE_MEMORY_ERROR);
	IVS_DEBUG_TRACE("");

    CXml xmlReq;
    // ��װxml��׼�����͸�smu
    IVS_INT32 iRet = CDeviceMgrXMLProcess::ModifyDeviceGroupGetXML(pDomainCode, uiDevGroupID, pNewDevGroupName, xmlReq);     
	IVS_CHAR cDomainCode[IVS_DOMAIN_CODE_LEN + 1] = {0};
	if (!CToolsHelp::Memcpy(cDomainCode, IVS_DOMAIN_CODE_LEN, pDomainCode, IVS_DOMAIN_CODE_LEN))
	{
		BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "Memcpy(cDomainCode, IVS_DOMAIN_CODE_LEN, pDomainCode, IVS_DOMAIN_CODE_LEN) fail", "NA");
		return IVS_ALLOC_MEMORY_ERROR;
	}

	iRet = CXmlProcess::GetCommSendMgr(xmlReq, m_pUserMgr, NSS_MOD_DEV_GROUP_REQ, BUILDRET, cDomainCode);

    return iRet;
}


// �ƶ��豸��
IVS_INT32 CDeviceGroupMgr::MoveDeviceGroup(const IVS_CHAR* pDomainCode,
                                           IVS_UINT32 uiNewParentDevGroupID,
                                           IVS_UINT32 uiDevGroupID)const
{
    CHECK_POINTER(pDomainCode, IVS_OPERATE_MEMORY_ERROR);
	IVS_DEBUG_TRACE("");

    CXml xmlReq;
    // ��װxml��׼�����͸�smu
    IVS_INT32 iRet = CDeviceMgrXMLProcess::MoveDeviceGroupGetXML(pDomainCode, uiNewParentDevGroupID, uiDevGroupID, xmlReq);     
	IVS_CHAR cDomainCode[IVS_DOMAIN_CODE_LEN + 1] = {0};
	if (!CToolsHelp::Memcpy(cDomainCode, IVS_DOMAIN_CODE_LEN, pDomainCode, IVS_DOMAIN_CODE_LEN))
	{
		BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "Memcpy(cDomainCode, IVS_DOMAIN_CODE_LEN, pDomainCode, IVS_DOMAIN_CODE_LEN) fail", "NA");
		return IVS_ALLOC_MEMORY_ERROR;
	}

	iRet = CXmlProcess::GetCommSendMgr(xmlReq, m_pUserMgr, NSS_MOVE_DEV_GROUP_REQ, BUILDRET, cDomainCode);

    return iRet;
}


// ɾ���豸��������
IVS_INT32 CDeviceGroupMgr::DeleteDeviceFromGroup(const IVS_CHAR* pTargetDomainCode,
    IVS_UINT32 uiDeviceNum,
    const IVS_DEV_GROUP_CAMERA* pGroupCameraList)const
{
    CHECK_POINTER(pGroupCameraList, IVS_OPERATE_MEMORY_ERROR);
    CHECK_POINTER(pTargetDomainCode, IVS_OPERATE_MEMORY_ERROR);
	IVS_DEBUG_TRACE("");

    CXml xmlReq;
    if(IVS_SUCCEED != CDeviceMgrXMLProcess::DeleteDeviceFromGroupGetXML(pTargetDomainCode, uiDeviceNum, pGroupCameraList, xmlReq) )
    {
        BP_RUN_LOG_ERR(IVS_OPERATE_MEMORY_ERROR, "Delete Device FromGroup is failed", "NA");
        return IVS_OPERATE_MEMORY_ERROR;
    }	

	IVS_INT32 iRet = CXmlProcess::GetCommSendMgr(xmlReq, m_pUserMgr, NSS_DEL_DEV_GRP_DEV_REQ, BUILDRET, pTargetDomainCode);

    return iRet;
}


// �����豸�����豸
IVS_INT32 CDeviceGroupMgr::AddDeviceToGroup( const IVS_CHAR* pTargetDomainCode, IVS_UINT32 uiTargetDevGroupID, IVS_UINT32 uiDeviceNum, const IVS_DEV_GROUP_CAMERA* pGroupCameraList )const
{
    CHECK_POINTER(pGroupCameraList, IVS_OPERATE_MEMORY_ERROR);
    CHECK_POINTER(pTargetDomainCode, IVS_OPERATE_MEMORY_ERROR);
	IVS_DEBUG_TRACE("");

    CXml xmlReq;
    if(IVS_SUCCEED != CDeviceMgrXMLProcess::AddDeviceToGroupGetXML(pTargetDomainCode, uiTargetDevGroupID, uiDeviceNum, pGroupCameraList, xmlReq) )
    {
        BP_RUN_LOG_ERR(IVS_OPERATE_MEMORY_ERROR, "Add Device To Group failed", "NA");
        return IVS_OPERATE_MEMORY_ERROR;
    }	

	IVS_INT32 iRet = CXmlProcess::GetCommSendMgr(xmlReq, m_pUserMgr, NSS_ADD_DEV_GRP_DEV_REQ, BUILDRET, pTargetDomainCode);

    return iRet;
}

// �����豸�鱸ע��Ϣ
IVS_INT32 CDeviceGroupMgr::SetDevGroupRemark(const IVS_CHAR* pDomainCode, IVS_UINT32 uiDevGroupID, const IVS_CHAR* pDevGroupRemark)const
{
	CHECK_POINTER(pDomainCode, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pDevGroupRemark, IVS_OPERATE_MEMORY_ERROR);
	IVS_DEBUG_TRACE("");

	CXml xmlReq;
	IVS_INT32 iRet = CDeviceMgrXMLProcess::SetDevGroupRemarkGetXML(pDomainCode, uiDevGroupID, pDevGroupRemark, xmlReq);
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet, "Set DevGroupRemark GetXML fail", "NA");
		return iRet;
	}

	iRet = CXmlProcess::GetCommSendMgr(xmlReq, m_pUserMgr, NSS_MOD_DEV_GROUP_REMARK_REQ, BUILDRET, pDomainCode);

	return iRet;
}


// ��ѯ�豸�鱸ע��Ϣ
IVS_INT32 CDeviceGroupMgr::GetDevGroupRemark(const IVS_CHAR* pDomainCode, IVS_UINT32 uiDevGroupID, IVS_CHAR* pDevGroupRemark, IVS_UINT32 uiRemarkLen)const
{
	CHECK_POINTER(pDomainCode, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pDevGroupRemark, IVS_OPERATE_MEMORY_ERROR);
	IVS_DEBUG_TRACE("");

	CXml xmlReq;
	IVS_INT32 iRet = CDeviceMgrXMLProcess::GetDevGroupRemarkGetXML(pDomainCode, uiDevGroupID, xmlReq);
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet, "Get DevGroupRemark GetXML fail", "NA");
		return iRet;
	}

	iRet = CXmlProcess::GetCommSendMgr(xmlReq, m_pUserMgr, NSS_GET_DEV_GROUP_REMARK_REQ, BUILSTRING, pDomainCode);
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet, "Get DevGroup CommSendMgr fail", "NA");
		return iRet;
	}
	
	iRet = CDeviceMgrXMLProcess::GetDevGroupRemarkParseXML(pDevGroupRemark, xmlReq, uiRemarkLen);

	return iRet;
}


