/********************************************************************
filename    :    ChannelInfo.h
author      :    s00191067
created     :    2012/11/01
description :    �˿���Դ
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2012/11/01 ��ʼ�汾
*********************************************************************/
#ifndef CHANNEL_INFO_H
#define CHANNEL_INFO_H

#include "hwsdk.h"
#include "ivs_error.h"
#include "NetSourceCommon.h"

class CConnection;
class CChannelInfo
{
public:
    CChannelInfo();
    ~CChannelInfo(void);

    /******************************************************************
     function   : SetChannelNo
     description: ����ͨ����;
     input      : unsigned long ulChannel ͨ����;
     output     : NA
     return     : void
    *******************************************************************/
    void SetChannelNo(unsigned long ulChannel){ m_ulChannel = ulChannel; }

    /******************************************************************
     function   : GetChannelNo
     description: ��ȡͨ����;
     output     : NA
     return     : unsigned long ͨ����;
    *******************************************************************/
    unsigned long GetChannelNo(){ return m_ulChannel; }

    /******************************************************************
     function   : SetProtocolType
     description: ����Э������;
     input      : NET_PROTOCOL_TYPE enProtocolType Э������;
     output     : NA
     return     : void
    *******************************************************************/
    void SetProtocolType(IVS_PROTOCOL_TYPE enProtocolType){ m_enProtocolType = enProtocolType;}

    /******************************************************************
     function   : GetProtocolType
     description: ��ȡЭ������
     output     : NA
     return     : NET_PROTOCOL_TYPE Э������;
    *******************************************************************/
    IVS_PROTOCOL_TYPE GetProtocolType(){ return m_enProtocolType;}

    /******************************************************************
     function   : SetConnection
     description: ��������
     input      : SOCKET_TYPE enSocketType  socket����;
     input      : CConnection * pConnection ���Ӷ���;
     output     : NA
     return     : void
    *******************************************************************/
    void SetConnection(SOCKET_TYPE enSocketType, CConnection* pConnection);

    /******************************************************************
     function   : GetConnection
     description: ��ȡ���Ӷ���;
     input      : SOCKET_TYPE enSocketType  socket����;
     output     : NA
     return     : CConnection* ���Ӷ���
    *******************************************************************/
    CConnection* GetConnection(SOCKET_TYPE enSocketType);

    /******************************************************************
     function   : GetSocket
     description: ��ȡsocket����
     input      : SOCKET_TYPE enSocketType
     output     : NA
     return     : SOCKET
    *******************************************************************/
    SOCKET GetSocket(SOCKET_TYPE enSocketType);

    /******************************************************************
     function   : SetLocalPort
     description: ���ñ��ض˿�
     input      : PORT_TYPE enPortType  �˿�����;
     input      : unsigned int uiPort   �˿ں�;
     output     : NA
     return     : void
    *******************************************************************/
    void SetLocalPort(PORT_TYPE enPortType, unsigned int uiPort);

    /******************************************************************
     function   : GetLocalPort
     description: ��ȡ���ض˿�;
     input      : PORT_TYPE enPortType  �˿�����;
     output     : unsigned int & uiPort �˿ں�;
     return     : int �ɹ�:IVS_SUCCEED ʧ��:IVS_FAIL;
    *******************************************************************/
    int GetLocalPort(PORT_TYPE enPortType, unsigned int& uiPort);

    /******************************************************************
     function   : SetRemotePort
     description: ����Զ�˶˿�;
     input      : PORT_TYPE enPortType  �˿�����;
     input      : unsigned int uiPort   �˿ں�;
     output     : NA
     return     : void
    *******************************************************************/
    void SetRemotePort(PORT_TYPE enPortType, unsigned int uiPort);

    /******************************************************************
     function   : GetRemotePort
     description: ��ȡԶ�˶˿�;
     input      : PORT_TYPE enPortType  �˿�����;
     output     : unsigned int & uiPort �˿ں�;
     return     : int �ɹ�:IVS_SUCCEED ʧ��:IVS_FAIL;
    *******************************************************************/
    int GetRemotePort(PORT_TYPE enPortType, unsigned int& uiPort);
    
    /******************************************************************
     function   : SetStreamIP
     description: ������IP��ַ;
     input      : STREAM_IP_TYPE enStreamIPType �����ͽ��ն˻��Ͷ�;
     input      : const char * pszIP            ��Ӧ��IP
     output     : NA
     return     : int �ɹ�:IVS_SUCCEED ʧ��:IVS_FAIL;
    *******************************************************************/
    int SetStreamIP(STREAM_IP_TYPE enStreamIPType, const char* pszIP);

    /******************************************************************
     function   : GetStreamIP
     description: ��ȡ��IP��ַ;
     input      : STREAM_IP_TYPE enStreamIPType
     output     : std::string & strStreamIP
     return     : int �ɹ�:IVS_SUCCEED ʧ��:IVS_FAIL;
    *******************************************************************/
    int GetStreamIP(STREAM_IP_TYPE enStreamIPType, std::string& strStreamIP);

    /******************************************************************
     function   : SetDataCallBack
     description: �������ݻص�ָ����û�����;
     input      : NET_DATA_CALLBACK cbDataCallBack
     input      : void * pUser
     output     : NA
     return     : void
    *******************************************************************/
    void SetDataCallBack(NET_DATA_CALLBACK cbDataCallBack, void* pUser);

    /******************************************************************
     function   : GetDataCallBack
     description: ��ȡ���ݻص�ָ����û�����;
     input      : NA
     output     : NET_DATA_CALLBACK & cbDataCallBack
     output     : void * & pUser
     return     : void
    *******************************************************************/
    void GetDataCallBack(NET_DATA_CALLBACK& cbDataCallBack, void*& pUser);

    /******************************************************************
     function   : SetExceptionCallBack
     description: �����쳣�ص�ָ��;
     input      : NET_EXCEPTION_CALLBACK cbExceptionCallBack
     output     : NA
     return     : void
    *******************************************************************/
    void SetExceptionCallBack(NET_EXCEPTION_CALLBACK cbExceptionCallBack);

    /******************************************************************
     function   : GetExceptionCallBack
     description: ��ȡ�쳣�ص�ָ��;
     input      : NA
     output     : NET_EXCEPTION_CALLBACK & cbExceptionCallBack
     return     : void
    *******************************************************************/
    void GetExceptionCallBack(NET_EXCEPTION_CALLBACK& cbExceptionCallBack);

    /******************************************************************
     function   : Clear
     description: �������
     output     : NA
     return     : void
    *******************************************************************/
    void Clear();

	void SetMuilt(bool bIsMuilt){ m_bIsMuilt = bIsMuilt; }
	bool IsMuilt(){ return m_bIsMuilt; }

	void SetOnlyVideo(bool bIsOnlyVideo){ m_bIsOnlyVideo = bIsOnlyVideo; }
	bool IsOnlyVideo(){ return m_bIsOnlyVideo; }

    void SetSessionInfo(const char* pSessionUrl, const char* pSessionId);
    std::string GetSessionUrl();
    std::string GetSessionId();
private:
    unsigned int        m_uiLocalVideoRtpPort;     // ��ƵRtp�˿�
    unsigned int        m_uiLocalVideoRtcpPort;    // ��ƵRtcp�˿�
    unsigned int        m_uiLocalAudioRtpPort;     // ��ƵRtp�˿�
    unsigned int        m_uiLocalAudioRtcpPort;    // ��ƵRtcp�˿�
    unsigned int        m_uiRemoteVideoRtpPort;    // ��ƵRtp�˿�
    unsigned int        m_uiRemoteVideoRtcpPort;   // ��ƵRtcp�˿�
    unsigned int        m_uiRemoteAudioRtpPort;    // ��ƵRtp�˿�
    unsigned int        m_uiRemoteAudioRtcpPort;   // ��ƵRtcp�˿�

    std::string         m_strRemoteSendStreamIp;   // Դ��IP
    std::string         m_strLocalRecvStreamIP;    // ����IP

    IVS_PROTOCOL_TYPE   m_enProtocolType;          //Э������;
    unsigned long       m_ulChannel;               //ͨ����;

    NET_DATA_CALLBACK   m_cbDataCallBack;          //�������ݻص�����;
    void*               m_pDataUserData;           //�û�����;
    NET_EXCEPTION_CALLBACK m_cbExceptionCallBack;  //�쳣�ص�����;

    CConnection*        m_pConnVideoRtp;           //VideoRTP�����Ӷ���
    CConnection*        m_pConnVideoRtcp;          //VideoRTCP�����Ӷ���
    CConnection*        m_pConnAudioRtp;           //AudioRTP�����Ӷ���
    CConnection*        m_pConnAudioRtcp;          //AudioRTCP�����Ӷ���
    CConnection*        m_pConnMulticast;          //�鲥�����Ӷ���;

	bool				m_bIsMuilt;				   // �鲥���;
	bool				m_bIsOnlyVideo;			   // ������Ƶ�˿ڣ�û����Ƶ�˿�;
    std::string         m_strSessionUrl;           // SessionUrl
    std::string         m_strSessionId;            // SessionID
};
#endif //CHANNEL_INFO_H
