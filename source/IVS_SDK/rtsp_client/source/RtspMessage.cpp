/******************************************************************************
   ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� �� : RtspMessage.cpp
  ��    �� : y00182336
  �������� : RTSP����
  �޸���ʷ :
    1 ���� : 2011-10-12
      ���� : y00182336
      �޸� : ����

 ******************************************************************************/
#include <sstream>
#include "Rtsp_Log.h"
#include "RtspRetCode.h"
#include "RtspMessage.h"
using namespace std;

const char* CRtspMessage::m_strRtspMethod[] = RTSP_METHOD_STRING;
const char* CRtspMessage::m_RtspCode[]      = RTSP_CODE_STRING;

/***************************************************************************
 �� �� �� : CRtspMessage
 �������� : ���캯��
 ������� : ��
 ������� : ��
 �� �� ֵ : ��
***************************************************************************/
CRtspMessage::CRtspMessage()
{
    m_unRtspMethod = RTSP_METHOD_NUM;
    m_unCSeq       = 0;
    m_strUrl       = string("");
}

/***************************************************************************
 �� �� �� : ~CRtspMessage
 �������� : ��������
 ������� : ��
 ������� : ��
 �� �� ֵ : ��
***************************************************************************/
CRtspMessage::~CRtspMessage()
{

}



/***************************************************************************
 �� �� �� : encodeUrl
 �������� : ��װRTSP URL
 ������� : ��
 ������� : strMessage              ��װ����ַ���
 �� �� ֵ : RET_OK-�ɹ�          RET_FAIL-ʧ��
***************************************************************************/
int CRtspMessage::encodeUrl(std::string &strMessage)
{
    if (RTSP_METHOD_NUM <= getMethodType())
    {
        return RET_FAIL;
    }
    strMessage += m_strRtspMethod[getMethodType()];
    strMessage += " " + m_strUrl + " " + RTSP_PROTOCOL_VERSION;
    strMessage += RTSP_END_TAG;
    return RET_OK;
} //lint !e1762


/***************************************************************************
 �� �� �� : encodeCSeq
 �������� : ��װRTSPϵ�к�
 ������� : ��
 ������� : strMessage              ��װ����ַ���
 �� �� ֵ : RET_OK-�ɹ�          RET_FAIL-ʧ��
***************************************************************************/
void CRtspMessage::encodeCSeq(std::string &strMessage)
{
    std::stringstream strCSeq;
    strCSeq<<m_unCSeq;
    strMessage += RTSP_TOKEN_STR_CSEQ;
    strMessage += strCSeq.str();
    strMessage += RTSP_END_TAG;
	strCSeq.str("");//�����ã�����stringstream��й©�ڴ�;
}

/***************************************************************************
 �� �� �� : getMethodType
 �������� : ��ȡRTSP��Ϣ����
 ������� : ��
 ������� : ��
 �� �� ֵ : ��Ϣ����
***************************************************************************/
unsigned int CRtspMessage::getMethodType() const
{
    return m_unRtspMethod;
}

/***************************************************************************
 �� �� �� : getMethodType
 �������� : ����RTSP���к�
 ������� : unCSeq                  ���к�
 ������� : ��
 �� �� ֵ : ��
***************************************************************************/
void CRtspMessage::setCSeq(unsigned int unCSeq)
{
//    ACE_Guard <ACE_Thread_Mutex> locker (m_CSeqMutex);
    m_unCSeq = unCSeq;
}


/***************************************************************************
 �� �� �� : getMethodType
 �������� : ��ȡ��Ϣ���к�
 ������� : unCSeq                  ���к�
 ������� : ��
 �� �� ֵ : ��
***************************************************************************/
unsigned int CRtspMessage::getCSeq() const
{
    return m_unCSeq;
}



/***************************************************************************
 �� �� �� : setRtspUrl
 �������� : ����URL
 ������� : unCSeq                  RTSP URL
 ������� : ��
 �� �� ֵ : ��
***************************************************************************/
void CRtspMessage::setRtspUrl(const std::string &strUrl)
{
    m_strUrl = strUrl;
    return;
}



/***************************************************************************
 �� �� �� : isRtspRespMsg
 �������� : �ж��Ƿ���RTSP��Ϣ�������CSeqֵ
 ������� : psRtspMsg               RTSP��Ϣ�ַ���
            nLen                    ��Ϣ����
 ������� : ��
 �� �� ֵ : true-��             false-��
***************************************************************************/
bool CRtspMessage::isRtspRespMsg(const char* psRtspMsg, const unsigned int unLen)
{
    // ����Ƿ�����RTSP/1.0��ͷ����Ӧ��Ϣ
    if((NULL == psRtspMsg) 
        || (0 != strncmp(psRtspMsg, RTSP_PROTOCOL_VERSION, strlen(RTSP_PROTOCOL_VERSION))))
    {
        return false;
    }
    
    // ��λ"CSeq: "�ֶ�
    const char* pCSeq = strstr(psRtspMsg, RTSP_TOKEN_STR_CSEQ);
    if (NULL == pCSeq)
    {
        return false;
    }
    unsigned int unCSeq = 0;
    // ȡ��"CSeq: "������֣�����뷢��CSeq�Ƿ�һ��
    sscanf(pCSeq, RTSP_TOKEN_STR_CSEQ "%u", &unCSeq);
    if (unCSeq != getCSeq())
    {
        IVS_DBG_LOG("CSeq wrong, message CSeq[%u].", unCSeq);
        return false;
    }
    return true;
} //lint !e1762



/***************************************************************************
 �� �� �� : decodeMessage
 �������� : ������Ӧ��Ϣ,�˴�ֻ����200 ok
 ������� : psRtspMsg               RTSP��Ϣ�ַ���
            nLen                    ��Ϣ����
 ������� : ��
 �� �� ֵ : RET_OK-�ɹ�             RET_FAIL-ʧ��
***************************************************************************/
int CRtspMessage::decodeMessage(const char* pszMessage, const unsigned int unMsgLen)
{
    if((NULL == pszMessage) || (0 == unMsgLen))
    {
        return RET_FAIL;
    }
    std::string strRtspMsg = "";
    strRtspMsg.append(pszMessage, unMsgLen);
    // ȡ����Ӧ���
    std::string strValue = "";
    strValue = strRtspMsg.substr(strlen(RTSP_PROTOCOL_VERSION));
    string::size_type nPos = strValue.find(RTSP_END_TAG);
    if (string::npos == nPos)
    {
        return RET_FAIL;
    }
    std::string strRet = "";
    strRet = strValue.substr(0, nPos);
    trimString(strRet);
    unsigned int i = 0;
    for(; i < RTSP_STATUS_CODES_BUTT; i++)
    {
        if(0 == strncmp(m_RtspCode[i], strRet.c_str(), strRet.length()))
        {
            break;
        }
    }
    // ���������200 OK����ʧ��
    if (RTSP_SUCCESS_OK != i)
    {
        return i;
    }
    return RET_OK;

}

/***************************************************************************
 �� �� �� : trimString
 �������� : ɾ���ַ���ǰ��Ŀ��ַ�
 ������� : srcString               �ַ���
 ������� : srcString
 �� �� ֵ : ��
***************************************************************************/
void CRtspMessage::trimString(std::string& srcString) const
{
    string::size_type pos = srcString.find_last_not_of(' ');
    if (pos != string::npos)
    {
        (void) srcString.erase(pos + 1);
        pos = srcString.find_first_not_of(' ');
        if (pos != string::npos)
            (void) srcString.erase(0, pos);
    }
    else
        (void) srcString.erase(srcString.begin(), srcString.end());
    return;
}


