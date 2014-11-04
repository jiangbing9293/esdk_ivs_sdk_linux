/********************************************************************
filename    :    NetChannelMgr.h
author      :    s00191067
created     :    2012/11/01
description :    �˿���Դ����
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2012/11/01 ��ʼ�汾
*********************************************************************/
#ifndef NET_CHANNEL_MGR_H
#define NET_CHANNEL_MGR_H

#include <list>
#include "NetSourceCommon.h"
#include "Lock.h"

class CNetChannel;
class CConnection;
class CNetChannelMgr
{
private:
    // ��ʵ�������캯����Ϊ˽��;
    CNetChannelMgr(void); 

public:
    /******************************************************************
     function   : instance
     description: ��ʵ��
     output     : NA
     return     : CNetChannelMgr& ��ʵ������;
    *******************************************************************/
    static CNetChannelMgr& instance()
    {
        static CNetChannelMgr NetChannelMgr;
        return NetChannelMgr;
    }

    /******************************************************************
     function   : ~CNetChannelMgr
     description: ��������;
     input      : void
     output     : NA
     return     : NA
    *******************************************************************/
    ~CNetChannelMgr(void);

    /******************************************************************
     function   : Init
     description: ��ʼ��
     input      : void
     output     : NA
     return     : void
    *******************************************************************/
    void Init(void);

    /******************************************************************
     function   : UnInit
     description: ���;
     input      : void
     output     : NA
     return     : void
    *******************************************************************/
    void UnInit(void);

    /******************************************************************
     function   : GetChannel
     description: ��ȡ���þ��
     input      : NA
     output     : unsigned long & ulChannel
     return     : int �ɹ�:IVS_SUCCEED ʧ��:IVS_FAIL;
    *******************************************************************/
    int GetChannel(unsigned long& ulChannel);

    /******************************************************************
     function   : FreeChannel
     description: �ͷ�ָ�����
     input      : unsigned long ulChannel
     output     : NA
     return     : int �ɹ�:IVS_SUCCEED ʧ��:IVS_FAIL;
    *******************************************************************/
    int FreeChannel(unsigned long ulChannel);

    /******************************************************************
     function   : SetProtocolType
     description: ����ý��������Э������
     input      : unsigned long ulChannel
     input      : unsigned int uiProtocolType
     output     : NA
     return     : int �ɹ�:IVS_SUCCEED ʧ��:IVS_FAIL;
    *******************************************************************/
    int SetProtocolType(unsigned long ulChannel, unsigned int uiProtocolType);

    /******************************************************************
     function   : GetProtocolType
     description: ���ý��������Э������
     input      : unsigned long ulChannel
     output     : unsigned int & uiProtocolType
     return     : int �ɹ�:IVS_SUCCEED ʧ��:IVS_FAIL;
    *******************************************************************/
    int GetProtocolType(unsigned long ulChannel, unsigned int& uiProtocolType);

    /******************************************************************
     function   : SetLocalRecvStreamIP
     description: ���ý���������Ip��ַ;
     input      : unsigned long ulChannel
     input      : const char * pszLocalRecvStreamIp
     output     : NA
     return     : int �ɹ�:IVS_SUCCEED ʧ��:IVS_FAIL;
    *******************************************************************/
    int SetLocalRecvStreamIP(unsigned long ulChannel, const char* pszLocalRecvStreamIp);

    /******************************************************************
     function   : GetLocalRecvStreamIP
     description: ��ý���������Ip��ַ
     input      : unsigned long ulChannel
     output     : char * pszLocalRecvStreamIp
     return     : int �ɹ�:IVS_SUCCEED ʧ��:IVS_FAIL;
    *******************************************************************/
    int GetLocalRecvStreamIP(unsigned long ulChannel, char* pszLocalRecvStreamIp);

    /******************************************************************
     function   : GetLocalRecvStreamPort
     description: ��ȡ���������ĵ�ַIP�Ͷ˿ں�;
     input      : unsigned long ulChannel
     output     : unsigned int & uiLocalRecvStreamVideoRtpPort
     output     : unsigned int & uiLocalRecvStreamAudioRtpPort
     return     : int �ɹ�:IVS_SUCCEED ʧ��:IVS_FAIL;
    *******************************************************************/
    int GetLocalRecvStreamPort(unsigned long ulChannel, unsigned int& uiLocalRecvStreamVideoRtpPort, 
                               unsigned int& uiLocalRecvStreamAudioRtpPort);

    /******************************************************************
     function   : OpenNetStream
     description: ��������
     input      : unsigned long ulChannel
     input      : const char * pRemoteSendStreamIp
     input      : unsigned int uiRemoteSendStreamVideoPort
     input      : unsigned int uiRemoteSendStreamAudioPort
     output     : NA
     return     : int �ɹ�:IVS_SUCCEED ʧ��:IVS_FAIL;
    *******************************************************************/
    int OpenNetStream(unsigned long ulChannel, const char* pRemoteSendStreamIp, 
                      unsigned int uiRemoteSendStreamVideoPort, 
                      unsigned int uiRemoteSendStreamAudioPort);

    /******************************************************************
     function   : CloseNetStream
     description: �ر�������
     input      : unsigned long ulChannel
     output     : NA
     return     : int �ɹ�:IVS_SUCCEED ʧ��:IVS_FAIL;
    *******************************************************************/
    int CloseNetStream(unsigned long ulChannel);

    /******************************************************************
     function   : StartRecvStream
     description: ��������
     input      : unsigned long ulChannel
     output     : NA
     return     : int �ɹ�:IVS_SUCCEED ʧ��:IVS_FAIL;
    *******************************************************************/
    int StartRecvStream(unsigned long ulChannel);

    /******************************************************************
     function   : SetExceptionCallBack
     description: �����쳣�ص�
     input      : NET_EXCEPTION_CALLBACK cbExceptionCallBack
     input      : void * pUser
     output     : NA
     return     : void
    *******************************************************************/
    void SetExceptionCallBack(NET_EXCEPTION_CALLBACK cbExceptionCallBack, void* pUser);

    /******************************************************************
     function   : SetDataCallBack
     description: �������ݻص�
     input      : unsigned long ulChannel
     input      : NET_DATA_CALLBACK cbDataCallBack
     input      : void * pUser
     output     : NA
     return     : void
    *******************************************************************/
    void SetDataCallBack(unsigned long ulChannel, NET_DATA_CALLBACK cbDataCallBack, void* pUser);

    /******************************************************************
     function   : DoExceptionCallBack
     description: �����쳣���ú������ڲ�ģ����ã����쳣ͳһ�������������У���ͳһ�ص��ϲ�;
     input      : int iMsgType      �쳣����;
     input      : void * pParam     ������Ϣ�������쳣���Ϳ���չ;
     output     : NA
     return     : void
    *******************************************************************/
    void DoExceptionCallBack(int iMsgType, void* pParam);

	/******************************************************************
     function   : FreePortfromList
     description: ���ն˿ڣ��Ƶ����ж˿��б���;
     input      : unsigned int uiSocketPort �����ն˿ں�
     output     : NA
     return     : void
    *******************************************************************/
    void FreePortfromList(unsigned int uiSocketPort);

    /******************************************************************
     function   : SetSessionInfo
     description: ����Session��Ϣ
     input      : unsigned long ulChannel
     input      : char* pSessionUrl
     input      : char* pSessionId
     output     : NA
     return     : void;
    *******************************************************************/
    void SetSessionInfo(unsigned long ulChannel, char* pSessionUrl, char* pSessionId);
private:
    /******************************************************************
     function   : GetUdpSocketPort
     description: ��ȡUDPģʽ��RTP�˿ڼ����Ӷ���;
     input      : const char * pszLocalRecvStreamIp ����IP
     output     : unsigned int & uiRtpPort          rtp�˿�
     output     : CConnection *  & pRtpConnection   rtp���Ӷ���
     output     : CConnection *  & pRtcpConnection  rtcp���Ӷ���
     return     : int �ɹ�:IVS_SUCCEED ʧ��:IVS_FAIL;
    *******************************************************************/
    int GetUdpSocketPort(const char* pszLocalRecvStreamIp, 
                         unsigned int& uiRtpPort, 
                         CConnection* &pRtpConnection, 
                         CConnection* &pRtcpConnection);

    /******************************************************************
     function   : GetTcpSocketPort
     description: ��ȡTCPģʽ�¶˿ڼ����Ӷ���;
     input      : const char * pszLocalRecvStreamIp ����IP;
     input      : unsigned int & uiPort             ���ض˿�;
     input      : CConnection *  & pConnection      ���Ӷ���;
     output     : NA
     return     : int
    *******************************************************************/
    int GetTcpSocketPort(const char* pszLocalRecvStreamIp, 
                        unsigned int& uiPort, 
                        CConnection* &pConnection);

    /******************************************************************
     function   : GetChannelByID
     description: ����ͨ���Ż�ȡ����ͨ������;
     input      : unsigned int ulChannel
     output     : NA
     return     : CNetChannel* ͨ������;
    *******************************************************************/
    CNetChannel* GetChannelByID(unsigned long ulChannel);

private:
    // �쳣���;
    NET_EXCEPTION_CALLBACK      m_cbExceptionCallBack;       // �ϲ��쳣�ص�ָ��;
    void*                       m_pUserData;                 // �ϲ��쳣�ص���ص��û�����;
    CIVSMutex                   m_mutexExceptionCallback;    

    // ͨ������ͨ����Դ��ӳ���;
    typedef std::map<unsigned long, CNetChannel*> NetChannelMap;
    typedef NetChannelMap::iterator NetChannelMapIter;
    NetChannelMap               m_mapNetChannel;	
    CIVSMutex                   m_mutexChannel;

    // �˿ں��б�;
    typedef std::list<unsigned int> PortList;
    typedef PortList::iterator PortListIter;
    PortList                    m_lstFreePort;             // ���ж˿��б�;
    PortList                    m_lstBusyPort;             // ���ö˿��б�;
    CIVSMutex                   m_mutexFreePortList;
    CIVSMutex                   m_mutexBusyPortListLock;
};
#endif //NET_CHANNEL_MGR_H
