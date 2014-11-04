/********************************************************************
filename    :    TcpSource.h
author      :    s00191067
created     :    2012/11/05
description :    TCP������
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2012/11/05 ��ʼ�汾
*********************************************************************/
#ifndef TCP_SOURCE_H
#define TCP_SOURCE_H

#include "NetSource.h"
#include "RtspPacket.h"

class CChannelInfo;
class CTcpSource : public CNetSource
{
public:
    CTcpSource(void);
    virtual ~CTcpSource(void);

    /******************************************************************
     function   : Init
     description: ��ʼ��;
     input      : CChannelInfo * pChannelInfo
     output     : NA
     return     : int �ɹ�:IVS_SUCCEED ʧ��:IVS_FAIL;
    *******************************************************************/
    virtual int Init(CChannelInfo* pChannelInfo); 

protected:
	/******************************************************************
     function   : ProcessRecevData
     description: ����
     output     : NA
     return     : void
    *******************************************************************/
    virtual void ProcessRecevData();

	/******************************************************************
     function   : CheckNatSend
     description: ����Ƿ�����NAT��Խ����,ÿ��30s�����������nat��Ϣ;
     output     : NA
     return     : int IVS_FAIL��ʾ������������ IVS_SUCCEED��ʾ��������;
    *******************************************************************/
    int CheckNatSend();

private:
	long RecvRTSP();

	void ProcRtsp();

	void PacketRtspInfo(const string strLocalIp, unsigned long ulLocalPort, const string strSourceIp, unsigned long ulSourcePort);

    void HandleTimeOutEvent();
private:
    CRtspPacket      m_RtspStack;               // RTSPЭ��ջ;
	SOCKET           m_socket;                  // soket����;
    bool             m_bIsFirstConnect;         // ��һ������
    std::string      m_strRemoteIp;             // Զ��IP
    unsigned int     m_uiRemotePort;            // Զ�˶˿�
}; 
#endif // TCP_SOURCE_H
