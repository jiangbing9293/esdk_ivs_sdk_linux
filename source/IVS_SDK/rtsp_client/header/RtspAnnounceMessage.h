/******************************************************************************
   ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� �� : RtspAnnounceMessage.h
  ��    �� : y00182336
  �������� : RTSP Announce��Ϣ
  �޸���ʷ :
    1 ���� : 2011-11-8
      ���� : y00182336
      �޸� : ����

 ******************************************************************************/
#ifndef __RTSPANNOUNCEMESSAGE_H_
#define __RTSPANNOUNCEMESSAGE_H_
#include "RtspMessage.h"

class CRtspAnnounceMessage : public CRtspMessage
{
public:
    CRtspAnnounceMessage();
    virtual ~CRtspAnnounceMessage();

    // ��װ��Ϣ
    int encodeMessage(std::string &strMessage);
    
    // ������Ϣ
    int decodeMessage(const char* pszMessage,const unsigned int unMsgLen);
    
    // ����SDP
    std::string getRtspSdp()const;
    
    void setStatusCode(unsigned int unStatusCode);
private:
    std::string     m_strRtspSdp;               // ��Ϣ���е�SDP�ֶΣ���Ҫ�ص���CU
    unsigned int    m_unStatusCode;             // ��Ӧ��Ϣ�еķ�����
};


#endif



