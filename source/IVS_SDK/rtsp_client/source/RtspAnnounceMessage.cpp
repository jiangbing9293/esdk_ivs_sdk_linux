/******************************************************************************
   ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� �� : RtspOptionsMessage.cpp
  ��    �� : y00182336
  �������� : RTSP Options��Ϣ
  �޸���ʷ :
    1 ���� : 2011-10-12
      ���� : y00182336
      �޸� : ����

 ******************************************************************************/
#include "RtspAnnounceMessage.h"
#include "Rtsp_Log.h"
#include "RtspRetCode.h"
#include "rtsp_client_datatype.h"
#include <sstream>
#include <string>

using namespace std;

CRtspAnnounceMessage::CRtspAnnounceMessage()
{
    m_strRtspSdp    = "";
    m_unStatusCode  = 0;
}

CRtspAnnounceMessage::~CRtspAnnounceMessage()
{

}


int CRtspAnnounceMessage::decodeMessage(const char* pszMessage,const unsigned int unMsgLen)
{
    std::string strRtspMsg = "";
    strRtspMsg.append(pszMessage, unMsgLen);
    IVS_DBG_LOG("start decode rtsp message:\n%s", strRtspMsg.c_str());
    // ����CSeq������ʧ��ֱ�ӷ���
    string::size_type nPos = strRtspMsg.find(RTSP_TOKEN_STR_CSEQ);
    if(string::npos == nPos)
    {
        return RET_FAIL;
    }
    string strValue = "";
    strValue = strRtspMsg.substr(nPos + strlen(RTSP_TOKEN_STR_CSEQ));
    nPos = strValue.find(RTSP_END_TAG);
    if (string::npos == nPos)
    {
        return RET_FAIL;
    }
    std::string strCSeq = "";
    strCSeq = strValue.substr(0, nPos);
    unsigned int unCSeq = strtoul(strCSeq.c_str(), NULL, 0);
    setCSeq(unCSeq);

    // ����Content-Length:������ʧ�ܣ�����Ӧ
    nPos = strRtspMsg.find(RTSP_TOKEN_STR_CONTENT_LENGTH);
    if(string::npos == nPos)
    {
        // ���ش���
        return RET_FAIL;
    }
    strValue = strRtspMsg.substr(nPos + strlen(RTSP_TOKEN_STR_CONTENT_LENGTH));
    nPos = strValue.find(RTSP_END_TAG);
    if (string::npos == nPos)
    {
        // ���ش���
        return RET_FAIL;
    }
    std::string strContentLen = "";
    strContentLen = strValue.substr(0, nPos);
    unsigned int unContentLen = strtoul(strContentLen.c_str(), NULL, 0);
    if (0 == unContentLen)
    {
        IVS_DBG_LOG("decode announce message success, Content Length = 0");
        return RET_OK;
    }
    // ��ȡContent-Length:�ֶ�
    nPos = strRtspMsg.find(RTSP_MSG_END_TAG);
    m_strRtspSdp = strRtspMsg.substr(nPos + strlen(RTSP_MSG_END_TAG), unContentLen);
    IVS_DBG_LOG("get content result:%s.", m_strRtspSdp.c_str());
    return RET_OK;
}

// ��װRTSP��Ӧ��Ϣ
int CRtspAnnounceMessage::encodeMessage(std::string &strMessage)
{
    strMessage.clear();
    // ��Ӧ��Ϣ������״̬��
    if (m_unStatusCode >= RTSP_STATUS_CODES_BUTT)
    {
        IVS_RUN_LOG_WAR("encode rtsp common response fail, status code[%u] invalid.",
            m_unStatusCode);
        return RET_FAIL;
    }
    strMessage = RTSP_PROTOCOL_VERSION;
    strMessage += " ";
    strMessage += CRtspMessage::m_RtspCode[m_unStatusCode];
    strMessage += RTSP_END_TAG;

    // Cseq
    std::stringstream strCSeq;
    strCSeq << getCSeq();
    strMessage += RTSP_TOKEN_STR_CSEQ;
    strMessage += strCSeq.str();
    strMessage += RTSP_END_TAG;
    
    // ����Content-Lengthͷ��
    strMessage += RTSP_TOKEN_STR_CONTENT_LENGTH;
    strMessage += "0";
    strMessage += RTSP_END_TAG;

    // ����Sessionͷ��
    strMessage += RTSP_TOKEN_STR_SESSION;
    strMessage += "1";
    strMessage += RTSP_END_TAG;
    strMessage += RTSP_END_TAG;

    IVS_DBG_LOG("success to encode common response.\n%s", strMessage.c_str());
    return RET_OK;
}


// ����RTSP SDP
string CRtspAnnounceMessage::getRtspSdp()const
{
    return m_strRtspSdp;
}


// ���÷�����
void CRtspAnnounceMessage::setStatusCode(unsigned int unStatusCode)
{
    m_unStatusCode = unStatusCode;
}

