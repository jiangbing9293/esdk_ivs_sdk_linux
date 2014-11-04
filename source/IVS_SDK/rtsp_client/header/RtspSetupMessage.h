/******************************************************************************
   ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� �� : RtspSetupMessage.h
  ��    �� : y00182336
  �������� : RTSP����
  �޸���ʷ :
    1 ���� : 2011-10-12
      ���� : y00182336
      �޸� : ����

 ******************************************************************************/
#ifndef __RTSPSETUPMESSAGE_H_
#define __RTSPSETUPMESSAGE_H_

#include <string>
#include "ace_header.h"
#include "RtspMessage.h"
#include "RtspDef.h"

class CRtspSetupMessage : public CRtspMessage
{
public:
    CRtspSetupMessage();
    virtual ~CRtspSetupMessage();

    int encodeMessage(std::string &strMessage);

    // ��װRTSP Transport����
    void encodeTransport(std::string &strMessage);
    
    // ���ô���ģʽ
    void setTransType(unsigned int unTransType);

    // ���ý��ն˿�
    void setClientPort(const unsigned short usVedioPort,
                       const unsigned short usAedioPort);
    
    // ���ý��յ�ַ
    void setDestinationIp(unsigned int unIp);

    // ��ȡ���͵�ַ
    unsigned int getServerIp() const;
    
    // ��ȡ������Ƶ�˿�
    unsigned short getServerVideoPort()const;

    // ��ȡ������Ƶ�˿�
    unsigned short getServerAudioPort()const;
    
    // ������Ӧ��Ϣ
    int decodeMessage(const char* pszMessage, const unsigned int nMsgLen);

private:
    // ����Transport
    int parseUdpTransParam(const std::string &strRtspMessage);

    int parseTcpTransParam(const std::string &strRtspMessage);

public:
    bool                m_bTcpFlag;                     // ���ӷ�ʽ�� true TCP, false UDP
    //MEDIA_ADDR*         m_stPeerAddr;                   // �첽ģʽ�£���¼Peer��ַ

private:
    unsigned int        m_unTransType;                  // ����ģʽ: TCP/UDP 
    unsigned short      m_usClientVideoPort;            // Rtp�˿ڣ�RTCPĬ��Ϊ+1
    unsigned short      m_usClientAudioPort;
    unsigned int        m_unDestIp;
    unsigned short      m_usServerVideoPort;            // Rtp��Ƶ�˿ڣ�RTCPĬ��Ϊ+1
    unsigned short      m_usServerAudioProt;            // Rtp��Ƶ�Ͽ���RTCPĬ��Ϊ+1
    unsigned int        m_unSrcIp;
    bool                m_bIsNoAudio;
};

#endif //__RTSPSETUPMESSAGE_H_

