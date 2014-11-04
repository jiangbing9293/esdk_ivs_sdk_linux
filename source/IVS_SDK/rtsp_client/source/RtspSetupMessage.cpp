/******************************************************************************
   ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� �� : RtspSetupMessage.cpp
  ��    �� : y00182336
  �������� : RTSP����
  �޸���ʷ :
    1 ���� : 2011-10-12
      ���� : y00182336
      �޸� : ����

 ******************************************************************************/
#include <sstream>
#include "ace_header.h"
#include "Rtsp_Log.h"
#include "RtspSetupMessage.h"
#include "RtspMessage.h"
#include "RtspRetCode.h"
#include "rtsp_client_datatype.h"

using namespace std;

/***************************************************************************
 �� �� �� : CRtspSetupMessage
 �������� : ���캯��
 ������� : ��
 ������� : ��
 �� �� ֵ : ��
***************************************************************************/
CRtspSetupMessage::CRtspSetupMessage()
{
    m_unRtspMethod      = RTSP_METHOD_SETUP;
    m_unTransType       = TRANS_TYPE_UDP;
    m_usClientVideoPort = 0;
    m_usClientAudioPort = 0;
    m_unDestIp          = 0;
    m_usServerVideoPort = 0;
    m_usServerAudioProt = 0;
    m_unSrcIp           = 0;

    m_bTcpFlag          = false;
    m_bIsNoAudio        = false;
    //m_stPeerAddr        = NULL;
}


/***************************************************************************
 �� �� �� : ~CRtspSetupMessage
 �������� : ��������
 ������� : ��
 ������� : ��
 �� �� ֵ : ��
***************************************************************************/
CRtspSetupMessage::~CRtspSetupMessage()
{
    //m_stPeerAddr        = NULL;
}


/***************************************************************************
 �� �� �� : encodeMessage
 �������� : ��װRTSP SETUP��Ϣ
 ������� : ��
 ������� : strMessage              ��װ����ַ���
 �� �� ֵ : RET_OK-�ɹ�          RET_FAIL-ʧ��
***************************************************************************/
int CRtspSetupMessage::encodeMessage(std::string &strMessage)
{
    strMessage.clear();
    if (RET_OK != encodeUrl(strMessage))
    {
        return RET_FAIL;
    }
    // ��װTransport
    encodeTransport(strMessage);
    encodeCSeq(strMessage);
    strMessage += RTSP_END_TAG; // ��Ϣ����
    return RET_OK;
}



/***************************************************************************
 �� �� �� : encodeTransport
 �������� : ��װRTSP SETUP��Ϣ��Transport����
 ������� : ��
 ������� : strMessage              ��װ����ַ���
 �� �� ֵ : ��
***************************************************************************/
void CRtspSetupMessage::encodeTransport(std::string &strMessage)
{
    // Transport
    strMessage += RTSP_TOKEN_STR_TRANSPORT;
    //   �������(RTP)
    strMessage += RTSP_TRANSPORT_RTP;
    strMessage += RTSP_TRANSPORT_SPEC_SPLITER;
    strMessage += RTSP_TRANSPORT_PROFILE_AVP;
    //  ��������(TCP/UDP)
    if (TRANS_TYPE_TCP == m_unTransType)
    {
        strMessage += RTSP_TRANSPORT_SPEC_SPLITER;
        strMessage += RTSP_TRANSPORT_TCP;
    }
    strMessage += SIGN_SEMICOLON;
    // �����TCP����Ҫ��֯
    if (TRANS_TYPE_TCP == m_unTransType)
    {
        // TCP���䣬��Ҫ��֯ͨ����interleaved=:
        strMessage += RTSP_TRANSPORT_INTERLEAVED;
        stringstream strChannelNo;
        strChannelNo << RTSP_INTER_LEAVE_NUM;
        strMessage += strChannelNo.str() + SIGN_H_LINE;

        strChannelNo.str("");
        strChannelNo << (RTSP_INTER_LEAVE_NUM + 1); //lint !e835
        strMessage += strChannelNo.str();
        if (0 != m_unDestIp)
        {
            strMessage += SIGN_SEMICOLON;
            strMessage += RTSP_TRANSPORT_UNICAST;
            strMessage += SIGN_SEMICOLON;
            ACE_INET_Addr addr;
            addr.set(m_usClientVideoPort, m_unDestIp);
            strMessage += RTSP_TRANSPORT_DESTINATIION;

			const char* pszHostAddr = addr.get_host_addr();
            strMessage += ((NULL == pszHostAddr) ? "" : pszHostAddr);
            strMessage += SIGN_SEMICOLON;
            stringstream strPort;
            strPort << (unsigned int)m_usClientVideoPort;
            strMessage += RTSP_TRANSPORT_CLIENT_PORT;
            strMessage += strPort.str();
        }
    }
    else
    {
        // �����UDP����Ҫ�ͻ��˺ͷ���˵ĵ�ַ
        strMessage += RTSP_TRANSPORT_UNICAST;
        strMessage += SIGN_SEMICOLON;

        ACE_INET_Addr addr;
        addr.set(m_usClientVideoPort, m_unDestIp);
        // destination����
        strMessage += RTSP_TRANSPORT_DESTINATIION;
		const char* pszHostAddr = addr.get_host_addr();
        strMessage += ((NULL == pszHostAddr) ? "" : pszHostAddr);
        strMessage += SIGN_SEMICOLON;
        // client_port����
        strMessage += RTSP_TRANSPORT_CLIENT_PORT;
        stringstream strPort;
        strPort << (unsigned int)m_usClientVideoPort;
        strMessage += strPort.str() + SIGN_H_LINE;
        strPort.str("");
        strPort << (unsigned int)(m_usClientVideoPort + 1);
        strMessage += strPort.str();
        strPort.str("");
        strPort << (unsigned int)m_usClientAudioPort;
        strMessage += SIGN_COMMA;
        strMessage += strPort.str() + SIGN_H_LINE;
        strPort.str("");
        strPort << (unsigned int)(m_usClientAudioPort + 1);
        strMessage += strPort.str();
        if (0 == m_usClientAudioPort)
        {
            m_bIsNoAudio = true;
        }

    }
    strMessage += RTSP_END_TAG;         // Transport����
}


/***************************************************************************
 �� �� �� : decodeMessage
 �������� : ����RTSP SETUP��Ӧ��Ϣ
 ������� : pszMessage              ��Ϣ�ַ���
            nMsgLen                 ��Ϣ����
 ������� : ��
 �� �� ֵ : RET_OK-�ɹ�          RET_FAIL-ʧ��
***************************************************************************/
int CRtspSetupMessage::decodeMessage(const char* pszMessage, const unsigned int nMsgLen)
{
	int iRet = CRtspMessage::decodeMessage(pszMessage, nMsgLen);
    if (iRet)
    {
        IVS_RUN_LOG_CRI("decode Rtsp Setup messsage fail.");
        return iRet;
    }
    string strRtspMsg = "";
    strRtspMsg.assign(pszMessage, nMsgLen);
    // �����UDP��ʽ����Ҫ����Transport
    if (TRANS_TYPE_UDP == m_unTransType)
    {
        if (RET_OK != parseUdpTransParam(strRtspMsg))
        {
            return RET_FAIL;
        }
    }
    else
    {
        if (RET_OK != parseTcpTransParam(strRtspMsg))
        {
            return RET_FAIL;
        }
    }
    return RET_OK;
}



/***************************************************************************
 �� �� �� : parseUdpTransParam
 �������� : ����RTSP SETUP��ӦTransport
 ������� : strRtspMessage          ��Ϣ�ַ���
 ������� : ��
 �� �� ֵ : RET_OK-�ɹ�          RET_FAIL-ʧ��
***************************************************************************/
int CRtspSetupMessage::parseUdpTransParam(const std::string &strRtspMessage)
{
    // ����source.
    string::size_type nPos = strRtspMessage.find(RTSP_TRANSPORT_SOURCE);
    if(string::npos == nPos)
    {
        return RET_FAIL;
    }
    // ip����";",��ȡ�������IP;
    string strValue = "";
    strValue = strRtspMessage.substr(nPos + strlen(RTSP_TRANSPORT_SOURCE));
    nPos = strValue.find(SIGN_SEMICOLON);
    if (string::npos == nPos)
    {
        return RET_FAIL;
    }
    string strIP = strValue.substr(0, nPos);
    m_unSrcIp = ACE_OS::inet_addr(strIP.c_str());
    //m_unSrcIp = ACE_NTOHL(m_unSrcIp);

    // ����server_port
    nPos = strRtspMessage.find(RTSP_TRANSPORT_SERVER_PORT);
    if(string::npos == nPos)
    {
        return RET_FAIL;
    }
    // port����"-",��ȡ�������port;
    strValue.clear();
    strValue = strRtspMessage.substr(nPos + strlen(RTSP_TRANSPORT_SERVER_PORT));
    nPos = strValue.find(SIGN_H_LINE);
    if (string::npos == nPos)
    {
        return RET_FAIL;
    }
    string strPort = strValue.substr(0, nPos);
    m_usServerVideoPort = (unsigned short) atoi(strPort.c_str());

    if (!m_bIsNoAudio)
    {
        nPos = strValue.find(SIGN_COMMA);
        if(string::npos == nPos)
        {
            return RET_FAIL;
        }
        // port����"-",��ȡ�������port;
        string strPorts = "";
        strPorts = strValue.substr(nPos + strlen(SIGN_COMMA));
        nPos = strPorts.find(SIGN_H_LINE);
        if (string::npos == nPos)
        {
            return RET_FAIL;
        }
        strPort = strPorts.substr(0, nPos);
        m_usServerAudioProt = (unsigned short) atoi(strPort.c_str());
    }

    IVS_DBG_LOG("decode transport success, serverIP[%s], Video Port[%d], Audio Port[%d].", 
                                strIP.c_str(), 
                                m_usServerVideoPort,
                                m_usServerAudioProt);
    return RET_OK;
}


/***************************************************************************
 �� �� �� : parseTcpTransParam
 �������� : ����RTSP SETUP��ӦTransport
 ������� : strRtspMessage          ��Ϣ�ַ���
 ������� : ��
 �� �� ֵ : RET_OK-�ɹ�          RET_FAIL-ʧ��
***************************************************************************/
int CRtspSetupMessage::parseTcpTransParam(const std::string &strRtspMessage)
{
    // ����source.
    string::size_type nPos = strRtspMessage.find(RTSP_TRANSPORT_SOURCE);
    if(string::npos == nPos)
    {
        // ����Ӳ��
        return RET_OK;
    }
    string strValue = "";
    strValue = strRtspMessage.substr(nPos + strlen(RTSP_TRANSPORT_SOURCE));
    nPos = strValue.find(SIGN_SEMICOLON);
    if (string::npos == nPos)
    {
        return RET_FAIL;
    }
    string strIP = strValue.substr(0, nPos);
    m_unSrcIp = ACE_OS::inet_addr(strIP.c_str());
    // ����server_port
    nPos = strRtspMessage.find(RTSP_TRANSPORT_SERVER_PORT);
    if(string::npos == nPos)
    {
        return RET_FAIL;
    }
    // port����"-",��ȡ�������port;
    strValue.clear();
    strValue = strRtspMessage.substr(nPos + strlen(RTSP_TRANSPORT_SERVER_PORT));
    nPos = strValue.find(RTSP_END_TAG);
    if (string::npos == nPos)
    {
        return RET_FAIL;
    }
    string strPort = strValue.substr(0, nPos);
    m_usServerVideoPort = (unsigned short) atoi(strPort.c_str());

    return RET_OK;
}


/***************************************************************************
 �� �� �� : setTransType
 �������� : ���ô���ģʽ
 ������� : unTransType             ����ģʽ
 ������� : ��
 �� �� ֵ : ��
***************************************************************************/
void CRtspSetupMessage::setTransType(unsigned int unTransType)
{
    m_unTransType = unTransType;
    return;
}



/***************************************************************************
 �� �� �� : setTransType
 �������� : ���ý��ն˿�
 ������� : unTransType                 ���ն˿ں�
 ������� : ��
 �� �� ֵ : ��
***************************************************************************/
void CRtspSetupMessage::setClientPort(const unsigned short usVedioPort,
                                      const unsigned short usAudioPort)
{
    m_usClientVideoPort = usVedioPort;
    m_usClientAudioPort = usAudioPort;
    return;
}



/***************************************************************************
 �� �� �� : setDestinationIp
 �������� : ���ý��յ�ַ
 ������� : unIp                    ���յ�ַ
 ������� : ��
 �� �� ֵ : ��
***************************************************************************/
void CRtspSetupMessage::setDestinationIp(unsigned int unIp)
{
    m_unDestIp = unIp;
    return;
}


/***************************************************************************
 �� �� �� : getServerIp
 �������� : ��ȡ���͵�ַ
 ������� : ��
 ������� : ��
 �� �� ֵ : ���͵�ַ
***************************************************************************/
unsigned int CRtspSetupMessage::getServerIp() const
{
    return m_unSrcIp;
}



/***************************************************************************
 �� �� �� : getServerVideoPort
 �������� : ��ȡ��Ƶ���Ͷ˿�
 ������� : ��
 ������� : ��
 �� �� ֵ : ���Ͷ˿�
***************************************************************************/
unsigned short CRtspSetupMessage::getServerVideoPort()const
{
    return m_usServerVideoPort;
}


/***************************************************************************
 �� �� �� : getServerAudioPort
 �������� : ��ȡ��Ƶ���Ͷ˿�
 ������� : ��
 ������� : ��
 �� �� ֵ : ���Ͷ˿�
***************************************************************************/
unsigned short CRtspSetupMessage::getServerAudioPort()const
{
    return m_usServerAudioProt;
}



