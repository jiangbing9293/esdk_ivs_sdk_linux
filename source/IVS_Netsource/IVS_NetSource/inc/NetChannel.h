/********************************************************************
filename    :    NetChannel.h
author      :    s00191067
created     :    2012/11/01
description :    �˿���Դ
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2012/11/01 ��ʼ�汾
*********************************************************************/
#ifndef NET_CHANNEL_H
#define NET_CHANNEL_H

#include "hwsdk.h"
#include "ivs_error.h"
#include "NetSourceCommon.h"
#include "ChannelInfo.h"

class CNetSource;
class CConnection;
class CNetChannel
{
public:
    CNetChannel();
    ~CNetChannel(void);

    /******************************************************************
     function   : SetChannelNo
     description: ����ͨ����
     input      : unsigned int ulChannel
     output     : NA
     return     : void
    *******************************************************************/
    void SetChannelNo(unsigned long ulChannel);

    /******************************************************************
     function   : GetChannelNo
     description: ��ȡͨ����
     output     : NA
     return     : unsigned int ͨ����;
    *******************************************************************/
    unsigned long GetChannelNo();

    /******************************************************************
     function   : SetProtocolType
     description: ����Э������
     input      : NET_PROTOCOL_TYPE enProtocolType
     output     : NA
     return     : void
    *******************************************************************/
    void SetProtocolType(IVS_PROTOCOL_TYPE enProtocolType);

    /******************************************************************
     function   : GetProtocolType
     description: ��ȡЭ������
     output     : NA
     return     : NET_PROTOCOL_TYPE Э������;
    *******************************************************************/
    IVS_PROTOCOL_TYPE GetProtocolType();

    /******************************************************************
     function   : SetConnection
     description: �������Ӷ���
     input      : SOCKET_TYPE enSocketType  ����;
     input      : CConnection * pConnection ���Ӷ���;
     output     : NA
     return     : void
    *******************************************************************/
    void SetConnection(SOCKET_TYPE enSocketType, CConnection* pConnection);

    /******************************************************************
     function   : GetConnection
     description: ��ȡ���Ӷ���
     input      : SOCKET_TYPE enSocketType
     output     : NA
     return     : CConnection* ���Ӷ���;
    *******************************************************************/
    CConnection* GetConnection(SOCKET_TYPE enSocketType);

    /******************************************************************
     function   : SetLocalPort
     description: ���ñ��ض˿�
     input      : PORT_TYPE enPortType
     input      : unsigned int uiPort
     output     : NA
     return     : void
    *******************************************************************/
    void SetLocalPort(PORT_TYPE enPortType, unsigned int uiPort);

    /******************************************************************
     function   : GetLocalPort
     description: ��ȡ���ض˿�
     input      : PORT_TYPE enPortType �˿�����;
     output     : unsigned int & uiPort 
     return     : int �ɹ�:IVS_SUCCEED ʧ��:IVS_FAIL;
    *******************************************************************/
    int GetLocalPort(PORT_TYPE enPortType, unsigned int& uiPort);

    /******************************************************************
     function   : SetRemotePort
     description: ����Զ�˶˿�
     input      : PORT_TYPE enPortType
     input      : unsigned int uiPort
     output     : NA
     return     : void
    *******************************************************************/
    void SetRemotePort(PORT_TYPE enPortType, unsigned int uiPort);

    /******************************************************************
     function   : GetRemotePort
     description: ��ȡԶ�˶˿�
     input      : PORT_TYPE enPortType  �˿�����;
     output     : unsigned int & uiPort �˿ں�;
     return     : int �ɹ�:IVS_SUCCEED ʧ��:IVS_FAIL;
    *******************************************************************/
    int GetRemotePort(PORT_TYPE enPortType, unsigned int& uiPort);

    /******************************************************************
     function   : SetStreamIP
     description: ����IP��ַ�����غ�Զ��;
     input      : STREAM_IP_TYPE enStreamIPType ����
     input      : const char * pszIP    IP��ַ
     output     : NA
     return     : int �ɹ�:IVS_SUCCEED ʧ��:IVS_FAIL;
    *******************************************************************/
    int SetStreamIP(STREAM_IP_TYPE enStreamIPType, const char* pszIP);

    /******************************************************************
     function   : GetStreamIP
     description: ��ȡIP��ַ;
     input      : STREAM_IP_TYPE enStreamIPType
     input      : std::string & strStreamIP
     output     : NA
     return     : int �ɹ�:IVS_SUCCEED ʧ��:IVS_FAIL;
    *******************************************************************/
    int GetStreamIP(STREAM_IP_TYPE enStreamIPType, std::string& strStreamIP);

    /******************************************************************
     function   : OpenNetStream
     description: ��������
     input      : const char * pszRemoteSendStreamIp
     input      : unsigned int uiRemoteSendStreamVideoPort
     input      : unsigned int uiRemoteSendStreamAudioPort
     input      : NET_EXCEPTION_CALLBACK cbExceptionCallBack
     output     : NA
     return     : int �ɹ�:IVS_SUCCEED ʧ��:IVS_FAIL;
    *******************************************************************/
    int OpenNetStream(const char* pszRemoteSendStreamIp, 
                      unsigned int uiRemoteSendStreamVideoPort, 
                      unsigned int uiRemoteSendStreamAudioPort,
                      NET_EXCEPTION_CALLBACK cbExceptionCallBack);

    /******************************************************************
     function   : CloseNetStream
     description: �ر�������
     output     : NA
     return     : int �ɹ�:IVS_SUCCEED ʧ��:IVS_FAIL;
    *******************************************************************/
    int CloseNetStream();

    /******************************************************************
     function   : StartRecvStream
     description: ��������
     output     : NA
     return     : int �ɹ�:IVS_SUCCEED ʧ��:IVS_FAIL;
    *******************************************************************/
    int StartRecvStream();

    /******************************************************************
     function   : SetDataCallBack
     description: �������ݻص�
     input      : NET_DATA_CALLBACK cbDataCallBack
     input      : void * pUser
     output     : NA
     return     : void
    *******************************************************************/
    void SetDataCallBack(NET_DATA_CALLBACK cbDataCallBack, void* pUser);

    /******************************************************************
     function   : SetSessionInfo
     description: ����Session��Ϣ
     input      : char* pSessionUrl
     input      : char* pSessionId
     output     : NA
     return     : void
    *******************************************************************/
    void SetSessionInfo(const char* pSessionUrl, const char* pSessionId);

private:
	int BindMulticastPort(unsigned int uiRemoteSendStreamVideoPort, unsigned int uiRemoteSendStreamAudioPort);

private:
    CNetSource*     m_pSource;          //��������;
    CChannelInfo    m_channelInfo;      //ͨ����Ϣ,�������;
};
#endif //NET_CHANNEL_H
