/******************************************************************************
   ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� �� : RtspMessage.h
  ��    �� : y00182336
  �������� : RTSP��Ϣ
  �޸���ʷ :
    1 ���� : 2011-10-12
      ���� : y00182336
      �޸� : ����

 ******************************************************************************/
#ifndef __RTSPMESSAGE_H_
#define __RTSPMESSAGE_H_

#include <string>
#include "ace_header.h"
#include "rtsp_client_datatype.h"
#include "RtspDef.h"

class CRtspMessage
{
public:
    CRtspMessage();
    virtual ~CRtspMessage();
    
    // ��װRTSP��Ӧ��Ϣ
    int encodeRtspRespMsg(unsigned int unStatusCode, std::string &strMessage);

    // ��װURL
    int encodeUrl(std::string &strMessage);
    
    // ��װRTSP���к�
    void encodeCSeq(std::string &strMessage);

    unsigned int getMethodType() const;

    // ������Ϣ���к�
    void setCSeq(unsigned int unCSeq);

    // ��ȡ��Ϣ���к�
    unsigned int getCSeq() const;

    // ����URL
    void setRtspUrl(const std::string &strUrl);
    
    // �����Ƿ�ΪRTSP��Ӧ��Ϣ
    bool isRtspRespMsg(const char* psRtspMsg,const unsigned int unLen);
    
    // �麯������װRTSP��Ϣ
    virtual int encodeMessage(std::string &strMessage) = 0;

    // ������Ӧ��Ϣ
    virtual int decodeMessage(const char* pszMessage,const unsigned int unMsgLen);

private:
    // ȥ���ո�
    void trimString(std::string& srcString) const;

public:
    unsigned int        m_unRtspMethod;         // Rtsp��Ϣ����
    std::string         m_strUrl;               // RTSP��Ϣ��URL

    static const char*  m_RtspCode[];           // RTSP ��Ӧ������
private:
    unsigned int        m_unCSeq;               // RTSP��Ϣ���к�
    static const char*  m_strRtspMethod[];      // RTSP ��Ϣ����

};


#endif //__RTSPMESSAGE_H_

