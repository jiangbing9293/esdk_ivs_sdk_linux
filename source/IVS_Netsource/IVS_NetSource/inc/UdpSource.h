/********************************************************************
filename    :    UdpSource.h
author      :    s00191067
created     :    2012/11/05
description :    UDP������
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2012/11/05 ��ʼ�汾
*********************************************************************/
#ifndef UDP_SOURCE_H
#define UDP_SOURCE_H

#include "NetSource.h"
#include "RtspPacket.h"
#include "Lock.h"

#define SOCKET_COUNT  4     // socket�ĸ���;

class CChannelInfo;
class CUdpSource : public CNetSource
{
public:
    CUdpSource(void);
    virtual ~CUdpSource(void);

    /******************************************************************
     function   : Init
     description: ��ʼ��
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
    void ProcessRecevData();

    /******************************************************************
     function   : ProcPacket
     description: �ַ�����
     input      : int iIndex    soket����������;
     input      : int iLen       ���ݳ���;
     output     : NA
     return     : void
    *******************************************************************/
    void ProcPacket(int iIndex, int iLen);

    /******************************************************************
     function   : ConnSocket
     description: ��������socket����������Ƶ��rtp\rtcp����;
     input      : strRemoteSendStreamIp   Զ��IP;
     output     : NA
     return     : int �ɹ�:IVS_SUCCEED ʧ��:IVS_FAIL;
    *******************************************************************/
    int ConnSocket(const std::string& strRemoteSendStreamIp);

    /******************************************************************
     function   : ShutDownSocket
     description: �ر�����socket����������Ƶ��rtp\rtcp����;
     output     : NA
     return     : NA
    *******************************************************************/
    void ShutDownSocket();

	/******************************************************************
     function   : CheckNatSend
     description: ����Ƿ�����NAT��Խ����,ÿ��30s�����������nat��Ϣ;
     output     : NA
     return     : int IVS_FAIL��ʾ������������ IVS_SUCCEED��ʾ��������;
    *******************************************************************/
    int CheckNatSend();

protected:
    bool m_bMulticast;                          // �鲥��ʶ;

private:
    CRtspPacket      m_RtspStack;               // RTSPЭ��ջ;
	sockaddr_in		 m_SenderVideoRTCPSockAddr;	// �鲥����¼�¼Դ��IPC��������Ƶrtcp socket���󣬴�Խ�����ʹ��;
	sockaddr_in		 m_SenderAudioRTCPSockAddr; // �鲥����¼�¼Դ��IPC��������Ƶrtcp socket���󣬴�Խ�����ʹ��;
};
#endif //UDP_SOURCE_H