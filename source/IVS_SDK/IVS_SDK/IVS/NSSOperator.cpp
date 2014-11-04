#include "NSSOperator.h"
#include "nss_xml_msg.h"
#include "nss_msg_header.h"
#include "UserMgr.h"
#include "ace_header.h"
#include "SDKConfig.h"
#include <errno.h>
//#include "LinkRouteMgr.h"
#include "EventCallBackJob.h"
#include "ToolsHelp.h"
#include "IVS_Trace.h"

// �ֶ������ͻ�������
int CNSSOperator::CreateClientLink(std::string& strNetElementID, int iNetElementType, const char* pszPeerIP, int iPeerPort, const char* pszLocalIP)
{
	return CNetFactory::Instance().CreateClientLink(strNetElementID, iNetElementType, pszPeerIP, iPeerPort, pszLocalIP,false);
}//lint !e1762 ����Ҫ

// �ֶ�ͬ�������ͻ�������
int CNSSOperator::CreateSyncClientLink(int iNetElementType, const char* pszPeerIP, int iPeerPort, std::string& strNetElementID)const
{
	// ���ӳ�ʱʱ��Ĭ��Ϊ5��
    IVS_DEBUG_TRACE("target ip:[%s:%u].", pszPeerIP, iPeerPort);

	CREATE_CLIENT_INFO stCreateClientInfoSycn;
	memset(&stCreateClientInfoSycn, 0, sizeof(CREATE_CLIENT_INFO));
	if (!CToolsHelp::Memcpy(stCreateClientInfoSycn.PeerIP, IP_LENGTH, pszPeerIP, strlen(pszPeerIP)))// ����λ�ñ�֤ip�� '\0'��β
	{
		BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "memcpy fail", "NA");
		return IVS_ALLOC_MEMORY_ERROR;
	}
	stCreateClientInfoSycn.PeerPort = iPeerPort;
	stCreateClientInfoSycn.NetElementType = iNetElementType;
	stCreateClientInfoSycn.NeedSwitchId = false;
	stCreateClientInfoSycn.uiTimeout = 1;

	int iCreateLinkRet = CNetFactory::Instance().CreateClientLinkSyn(stCreateClientInfoSycn);


    char cNetElementIDTemp[NET_ELEMENT_ID_LEGTH + 1] = {0}; 
	if (!CToolsHelp::Memcpy(cNetElementIDTemp, NET_ELEMENT_ID_LEGTH + 1, stCreateClientInfoSycn.NetElementID, NET_ELEMENT_ID_LEGTH))// ����λ�ñ�֤ip�� '\0'��β
	{
		BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "memcpy fail", "NA");
		return IVS_ALLOC_MEMORY_ERROR;
	}
    strNetElementID = cNetElementIDTemp;
    BP_DBG_LOG("create sync client link, ret code:%d, target ip:[%s:%u]", iCreateLinkRet, pszPeerIP, iPeerPort);

    if (IVS_SUCCEED != iCreateLinkRet)
    {
        BP_DBG_LOG("fail to create syn link, target ip:[%s:%u], linkid:[%s]", pszPeerIP, iPeerPort, strNetElementID.c_str());
        // (void)CNetFactory::Instance().DeleteClientLink(strNetElementID, iNetElementType);
        (void)CNetFactory::Instance().DeleteClientLinkByAddr(pszPeerIP, iPeerPort, iNetElementType);
    }

    
    return iCreateLinkRet;
}
// ��ȡ����NSS��ʵ������
CNSSOperator& CNSSOperator::instance()
{ 
	static CNSSOperator _instance; 
	return _instance; 
} 

// ͬ��������Ϣ����Ҫɾ�����ص�cmd
CCmd* CNSSOperator::SendSyncCmd(CCmd* pCmd, uint32_t uiTimeout/* = 5*/, IVS_BOOL bTimeout) const
{
	uint32_t uiTimeoutTemp = 0;
	if (0 == bTimeout)
	{
		uiTimeoutTemp = CSDKConfig::instance().GetTimeOutValue();
	}
    else
	{
		uiTimeoutTemp = uiTimeout;
	}
	return CDispatcher::instance().dispatcherSyncMsgOut(pCmd, uiTimeoutTemp);
}

IVS_INT32 CNSSOperator::SendSyncCmd(CmdMap& cmdMap, uint32_t uiTimeout/* = 5*/) const
{
	uint32_t uiTimeoutTemp = CSDKConfig::instance().GetTimeOutValue();
	return CDispatcher::instance().dispatcherSyncMsgOutBatch(cmdMap, uiTimeoutTemp);
}

// �������ڴ��XML����Ҫ�ͷŷ��ص��ڴ�
char* CNSSOperator::ParseCmd2XML(CCmd* pCmd, int& iRspCode) const
{
	CHECK_POINTER(pCmd, NULL);

	// �����ͷ����xml����m_nssXML������
	CNssXMLMsg nssXML;
	uint32_t iRet = nssXML.decode(pCmd->GetMsgBlock()); 
	if(NssRet_Succeed != iRet)
	{
        BP_RUN_LOG_ERR(IVS_FAIL,"decode cmd failed", "iDecodeRet = %d", iRet);
		iRspCode = IVS_FAIL;
		return NULL;
	}
	// ���ش�����
	iRspCode = nssXML.GetHeader().uiRspCode; //lint !e10 !e713 �޴����� 
    
	// ����XML ���ظ��ⲿ����
    const char* xml = nssXML.GetXML(); //lint !e64 ƥ��
    if (NULL  == xml)
    {
		BP_RUN_LOG_ERR(iRspCode, "xml is null", "NA");
        return NULL;
    }
	// ��utf8->ansi
	char* xml2ANSI = CToolsHelp::UTF8ToANSI(xml);
    if (NULL == xml2ANSI)
    {
        BP_RUN_LOG_ERR(iRspCode, "xml2ANSI is null", "NA");
        return NULL;
    }
	unsigned int length = strlen(xml2ANSI);

	char* xmlBuf = IVS_NEW(xmlBuf,length+1);
	if (NULL == xmlBuf)
	{
		BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "memcpy fail", "NA");
		free(xml2ANSI);
		return NULL;
	}
	if (!CToolsHelp::Memcpy(xmlBuf, length+1, xml2ANSI, length+1))
	{
		free(xml2ANSI);
		xml2ANSI = NULL;
		free(xmlBuf);
		xmlBuf = NULL;
		BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "memcpy fail", "NA");
		return NULL;
	}
	
	free(xml2ANSI);
	return xmlBuf;
}

// ֱ�ӽ���NSSͷ������һ�������
int CNSSOperator::ParseCmd2NSS(CCmd* pCmd)
{
    CHECK_POINTER(pCmd, IVS_FAIL);     //add by xiongfeng00192614    
	CNssMsgBase nssMsg;
	uint32_t iRet = nssMsg.decode(pCmd->GetMsgBlock());
	if(NssRet_Succeed != iRet)
	{
        BP_RUN_LOG_ERR(IVS_FAIL,"decode cmd failed", "iRet = %d", iRet);
		return IVS_FAIL;       //modify by xiongfeng00192614    ���󷵻�NULL ��0���ᱻ����Ϊ�ɹ�
	}
	return nssMsg.GetHeader().uiRspCode; //lint !e10 !e713 �޴�����
}//lint !e1762

// ��ȡCmd�е�����ָ��,�������ҲҪ���ⲿ�ͷ�
char* CNSSOperator::ParseCmd2Data(CCmd* pCmd, int& length)
{
	CHECK_POINTER(pCmd, NULL);
	CNssMsgBase nssMsg;
 	char* pData = nssMsg.GetContentBuff(pCmd->GetMsgBlock());
	if (NULL == pData)
	{
		BP_RUN_LOG_ERR(IVS_FAIL,"get buffer is null", "NA");
		length = 0;
		return NULL;
	}
	length = nssMsg.GetContentLength(pCmd->GetMsgBlock());
	char* xmlBuf = IVS_NEW(xmlBuf, (unsigned long)length); //lint !e737  // ������ڴ���Ҫ���ⲿ�ͷŵ�
	if (!CToolsHelp::Memcpy(xmlBuf, (unsigned int)length, pData, (unsigned int)length))
	{
		BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "memcpy fail", "NA");
		return NULL;
	}

	return xmlBuf;
}//lint !e1762

// ֱ�Ӵ���ƴװ��ɵ����ݲ��֣����CMD�������ļ�ʹ��
CCmd* CNSSOperator::BuildCmd(const std::string& strLinkID, int iNetElementType, unsigned short iReqID, const char* pData, int length) const
{
	CCmd* pCmd = NULL;
	HW_NEW(pCmd, CCmd); //lint !e774
	CHECK_POINTER(pCmd, NULL);
	pCmd->SetNetElemType(iNetElementType); 
	pCmd->SetNetLinkID(strLinkID);
	pCmd->SetServiceID(SERVICE_COMMU);
	pCmd->SetReqID(iReqID);
	pCmd->SetCmdType(CMD_SEND);

	// ��������ID
	std::string strTransID = CMKTransID::Instance().GenerateTransID();
	pCmd->SetTransID(strTransID);
	// NSSͷ
	TNssMsgHeader m_header;
	memset(&m_header, 0, sizeof(TNssMsgHeader));
	m_header.usiProtocolVersion = 0x0100;
	m_header.usiMsgType = iReqID;
	m_header.uiSeqID = 0; 
	m_header.uiTraceID = 0;
	strncpy(m_header.szTransactionNo, strTransID.c_str(), MaxTransactionNoLength - 1);
	m_header.uiRspCode = 0;  
	m_header.uiPacketLength = sizeof(TNssMsgHeader) + static_cast<IVS_UINT32>(length);

	// ת�������ֽ���
	m_header.usiProtocolVersion = ntohs(m_header.usiProtocolVersion);
	m_header.usiMsgType = ntohs(m_header.usiMsgType);
	m_header.uiSeqID = ntohl(m_header.uiSeqID);
	m_header.uiTraceID = ntohl(m_header.uiTraceID);
	m_header.uiRspCode = ntohl(m_header.uiRspCode);
	m_header.uiPacketLength = ntohl(m_header.uiPacketLength);
	
	// ����ACE����Ϣ����,�����cmd����ȥ�ͷţ����ﲻ�ͷ�
	ACE_Message_Block* pMsgBlock = new ACE_Message_Block(sizeof(TNssMsgHeader) + static_cast<IVS_UINT32>(length)); //lint !e429
	
	pMsgBlock->copy((char*)&m_header, sizeof(TNssMsgHeader));
	pMsgBlock->copy(pData, static_cast<IVS_UINT32>(length));

	// �������ݵ�CMD������Ӧ����CMDͳһ�ͷŵ�
	pCmd->SetMsgBlock(pMsgBlock);//lint !e613
	return pCmd;//lint !e429
}

// �޷���ֵ�ļ��
// Ҫ���͵����ݼ���NSSͷ����װ��CMD
CCmd* CNSSOperator::BuildCmd(const std::string& strLinkID, int iNetElementType, unsigned short iReqID,const char* pData) const
{
	CCmd* pCmd = NULL;
	HW_NEW(pCmd, CCmd); //lint !e774 
	CHECK_POINTER(pCmd, NULL);
	pCmd->SetNetElemType(iNetElementType); 
	pCmd->SetNetLinkID(strLinkID); 
	pCmd->SetServiceID(SERVICE_COMMU); 
	pCmd->SetReqID(iReqID);
	pCmd->SetCmdType(CMD_SEND);

	// ��������ID
	std::string strTransID = CMKTransID::Instance().GenerateTransID();
	if (IVS_SUCCEED != ConfCmd(pCmd,iReqID,pData,strTransID))
	{
		return NULL;
	}
	return pCmd;//lint !e429
}//lint !e1764

CCmd* CNSSOperator::BuildCmd(const std::string& strLinkID, int iNetElementType,unsigned short iReqID,const char* pData, const std::string& strTransID) const
{
    CCmd* pCmd = NULL;
    HW_NEW(pCmd, CCmd); //lint !e774 
	CHECK_POINTER(pCmd, NULL);
    pCmd->SetNetElemType(iNetElementType);
    pCmd->SetNetLinkID(strLinkID);
    pCmd->SetServiceID(SERVICE_COMMU);
    pCmd->SetReqID(iReqID);
    pCmd->SetCmdType(CMD_SEND);

	if (IVS_SUCCEED != ConfCmd(pCmd,iReqID,pData,strTransID))
	{
		return NULL;
	}

    return pCmd;//lint !e429
}

int CNSSOperator::ConfCmd(CCmd* pCmd,unsigned short iReqID,const char* pData, const std::string& strTransID)const
{
	CHECK_POINTER(pCmd, IVS_FAIL);

	pCmd->SetTransID(strTransID);//lint !e613

	// ���캯���й�����NSS��Ϣͷ,seq id��ʱ�޸�Ϊ0
	CNssXMLMsg nssXML(iReqID, 0, strTransID.c_str(), 0); //lint !e119 �޴�����
	// ��ansiת��utf-8
	char* pDataUTF8 = CToolsHelp::ANSIToUTF8(pData);
	if (NULL == pDataUTF8)
	{
		BP_RUN_LOG_ERR(IVS_FAIL,"ansi to utf8 failed", "NA");
		HW_DELETE(pCmd);
		return IVS_FAIL;
	}
	nssXML.SetXML(pDataUTF8, strlen(pDataUTF8)); // �������ڴ�
	// ɾ������
	free(pDataUTF8);
	pDataUTF8 = NULL;

	// ����ACE����Ϣ����,�����cmd����ȥ�ͷţ����ﲻ�ͷ�
	ACE_Message_Block* pMsgBlock = new ACE_Message_Block; //lint !e429

	// �ڲ��������ڴ�
	uint32_t iRet = nssXML.encode(pMsgBlock);
	if (NssRet_Succeed != iRet)
	{
		BP_RUN_LOG_ERR(IVS_FAIL,"encode block failed", "NA");
		HW_DELETE(pCmd);
		return IVS_FAIL;//lint !e429
	}

	// �������ݵ�CMD������Ӧ����CMDͳһ�ͷŵ�
	pCmd->SetMsgBlock(pMsgBlock);//lint !e613
	//return pCmd;//lint !e429
	return IVS_SUCCEED;
}

// ƴװ���͵�SMU��CMD����
CCmd* CNSSOperator::BuildSMUCmd(unsigned short iReqID,const char* pData, int length)
{
	if (length != 0)
	{
		return BuildCmd(m_strSMULinkID, NET_ELE_SMU_NSS, iReqID, pData, length);
	}
	return BuildCmd(m_strSMULinkID, NET_ELE_SMU_NSS, iReqID, pData);
}//lint !e1762 ����Ҫ

CCmd* CNSSOperator::BuildSCUCmd(unsigned short iReqID,const char* pData)
{
	return BuildCmd(m_strSCULinkID, NET_ELE_SCU_NSS, iReqID, pData);
}//lint !e1762 ����Ҫ


// ƴװ���͵�SMU��CMD����
CCmd* CNSSOperator::BuildSMUCmd(unsigned short iReqID,const char* pData, const string& sLinkId)
{
    if (sLinkId.empty())
    {
        BP_RUN_LOG_ERR(IVS_PARA_INVALID, "build smu cmd failed", "sLinkId is empty");
        return NULL;
    }
    return BuildCmd(sLinkId, NET_ELE_SMU_NSS, iReqID, pData);
}//lint !e1762 ����Ҫ

CCmd* CNSSOperator::BuildSCUCmd(unsigned short iReqID,const char* pData, const string& sLinkId)
{
    if (sLinkId.empty())
    {
        BP_RUN_LOG_ERR(IVS_PARA_INVALID,"build scu cmd failed", "sLinkId is empty");
        return NULL;
    }
    return BuildCmd(sLinkId, NET_ELE_SCU_NSS, iReqID, pData);
}//lint !e1762 ����Ҫ

CCmd* CNSSOperator::BuildOMUCmd(unsigned short iReqID,const char* pData, const string& sLinkId)
{
    if (sLinkId.empty())
    {
        BP_RUN_LOG_ERR(IVS_PARA_INVALID,"build omu cmd failed", "sLinkId is empty");
        return NULL;
    }
    return BuildCmd(sLinkId, NET_ELE_OMU_NSS, iReqID, pData);
}//lint !e1762 ����Ҫ

void CNSSOperator::ParseData2TLV(const void* pData, TNssTLVMsg& tlvMsg)const
{
	CHECK_POINTER_VOID(pData);
	tlvMsg.usiTag = ntohs(*(uint16_t*)pData);
	tlvMsg.uiLength = (uint32_t)ntohl(*(uint32_t*)((char*)pData+sizeof(uint16_t)));//lint !e826
	if (0 != tlvMsg.uiLength)
	{
		tlvMsg.pszValue = (char*)pData + sizeof(uint16_t) + sizeof(uint32_t);
	}
// 	tlvMsg = *(TNssTLVMsg*)pData;
// 	tlvMsg.usiTag = ntohs(tlvMsg.usiTag);
// 	tlvMsg.uiLength = (uint32_t)ntohl(tlvMsg.uiLength);
// 	tlvMsg.pszValue = (char*)pData + sizeof(uint16_t) + sizeof(uint32_t);
}

// ��xml���һ��loginID,�����͵�SMU
IVS_INT32 CNSSOperator::AddLoginIdAndSendCmd(unsigned short iReqID, const IVS_CHAR* pReqXml, IVS_CHAR*& pRsqXml) const
{

	CHECK_POINTER(pReqXml, IVS_OPERATE_MEMORY_ERROR);
	BP_RUN_LOG_INF("Add login id and send cmd", "send xml = %s", pReqXml);
	CCmd* pCmd = CNSSOperator::instance().BuildSMUCmd(iReqID, pReqXml);
	CHECK_POINTER(pCmd, IVS_OPERATE_MEMORY_ERROR);

	// ͬ��������Ϣ
	CCmd *pCmdRsp = CNSSOperator::instance().SendSyncCmd(pCmd);
	CHECK_POINTER(pCmdRsp, IVS_NET_RECV_TIMEOUT);
	IVS_INT32 iRet = IVS_SUCCEED;
	pRsqXml = CNSSOperator::instance().ParseCmd2XML(pCmdRsp, iRet);
	BP_RUN_LOG_INF("send cmd", "rev xml = %s, iRet= %d", pRsqXml, iRet);
	HW_DELETE(pCmdRsp);

	return iRet;
}

// ��xml���һ��loginID,�����͵�SMU
IVS_INT32 CNSSOperator::AddLoginIdAndSendCmd(unsigned short iReqID, const IVS_CHAR* pReqXml) const
{
	CHECK_POINTER(pReqXml, IVS_OPERATE_MEMORY_ERROR);
	BP_RUN_LOG_INF("Add login id and send cmd", "send xml = %s", pReqXml);
	CCmd* pCmd = CNSSOperator::instance().BuildSMUCmd(iReqID, pReqXml);
	CHECK_POINTER(pCmd, IVS_OPERATE_MEMORY_ERROR);

	// ͬ��������Ϣ
	CCmd *pCmdRsp = CNSSOperator::instance().SendSyncCmd(pCmd);
	CHECK_POINTER(pCmdRsp, IVS_NET_RECV_TIMEOUT);
	IVS_INT32 iRet = CNSSOperator::instance().ParseCmd2NSS(pCmdRsp);
	BP_RUN_LOG_INF("send cmd", "iRet= %d", iRet);
	HW_DELETE(pCmdRsp);

	return iRet;
}

