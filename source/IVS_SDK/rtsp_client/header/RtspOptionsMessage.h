#ifndef __RTSPOPTIONSMESSAGE_H_
#define __RTSPOPTIONSMESSAGE_H_

#include "RtspMessage.h"

class CRtspOptionsMessage : public CRtspMessage
{
public:
    CRtspOptionsMessage();
    virtual ~CRtspOptionsMessage();
    
    // ��װ��Ϣ
    int encodeMessage(std::string &strMessage);

    // ������Ϣ
    int decodeMessage(const char* pszMessage,const int nMsgLen);

    // ����Data
    int parseData(const std::string &strRtspMessage);

public:
    std::string         m_strDate;          // ʱ����Ϣ
    std::string         m_strTimeinfo;
};

#endif //__RTSPOPTIONSMESSAGE_H_
