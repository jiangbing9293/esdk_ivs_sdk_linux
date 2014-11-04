/******************************************************************************
   ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� �� : RtspPlayMessage.h
  ��    �� : y00182336
  �������� : RTSP Play��Ϣ
  �޸���ʷ :
    1 ���� : 2011-10-12
      ���� : y00182336
      �޸� : ����

 ******************************************************************************/
#ifndef __RTSPPLAYMESSAGE_H_
#define __RTSPPLAYMESSAGE_H_

#include "RtspMessage.h"

class CRtspPlayMessage : public CRtspMessage
{
public:
    CRtspPlayMessage();
    virtual ~CRtspPlayMessage();
    
    // ��װ��Ϣ
    int encodeMessage(std::string &strMessage);
    
    // ��װ����ʱ���
    void encodeRangeField(std::string &strMessage);

    // ���ò���ʱ���
    void setRang(const MEDIA_RANGE_S* stRange);

    // ���ò�������
    void setScale(const double dScale);

public:
    MEDIA_RANGE_S         m_stRange;             // ����ʱ���
    double              m_dScale;               // ��������
};


#endif //__RTSPPLAYMESSAGE_H_

