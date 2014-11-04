/********************************************************************
filename    :    Connection.h
author      :    s00191067
created     :    2012/11/10
description :    socket���ӻ���
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2012/11/10 ��ʼ�汾
*********************************************************************/
#ifndef CONNECTION_H
#define CONNECTION_H

#include "hwsdk.h"
#include "ivs_error.h"
#include "NetSourceCommon.h"

//select ģʽ�£�����״̬��ѯֵ;
typedef enum SelectType 
{
    READ,
    WRITE,
    EXCEPT
}SELECT_TYPE;

//����ģʽ;
#define BLOCKSTATE      0
//������ģʽ;
#define NOBLOCKSTATE    1
#ifndef WIN32
void closesocket(SOCKET s);
#endif
class CConnection
{
    
public:
    CConnection(void);
    virtual ~CConnection(void);

    /******************************************************************
     function   : Bind
     description: ��ָ���˿�;������洢IP��Port��������ʵ�ְ�;
     input      : const char * pszLocalIP   ����IP;
     input      : unsigned int uiLocalPort  ���ض˿�;
     output     : NA
     return     : int   IVS_SUCCEED �ɹ� IVS_FAIL ʧ��;
    *******************************************************************/
    virtual int Bind(const char* pszLocalIP, unsigned int uiLocalPort);

    /******************************************************************
     function   : Connect
     description: ���ӷ�����;������洢IP��Port��������ʵ������;
     input      : const char * pszServerIP  ������IP;
     input      : unsigned int uiServerPort �������˿�;
     output     : NA
     return     : int IVS_SUCCEED �ɹ� IVS_FAIL ʧ��;
    *******************************************************************/
    virtual int Connect(const char* pszServerIP, unsigned int uiServerPort);

    /******************************************************************
     function   : RecvData
     description: ��������;
     input      : NA
     input      : char * pData  �洢���պ������
     output     : int uiDataLen ���յ����ݳ���;
     return     : int ��::recvfrom�ķ���ֵ����һ��
    *******************************************************************/
    virtual int RecvData(char* pData, unsigned int uiDataLen) = 0;
    
    /******************************************************************
     function   : SendData
     description: ��������;
     input      : char * pData �����͵�����;
     input      : int uiDataLen ���ݳ���;
     output     : NA
     return     : int ��::sendto�ķ���ֵ����һ��;
    *******************************************************************/
    virtual int SendData(const char* pData, unsigned int uiDataLen) = 0;

    /******************************************************************
     function   : SetSocketBuf
     description: ����Socket��������С;
     input      : unsigned int uiSize   ��������С;
     output     : NA
     return     : int 
    *******************************************************************/
    int SetSocketBuf(unsigned int uiSize);

    /******************************************************************
     function   : SetIOState
     description: ����IO״̬;
     input      : bool bIsBlock true����, false������;
     output     : NA
     return     : void
    *******************************************************************/
    void SetIOState(bool bIsBlock);

    /******************************************************************
     function   : ShutDown
     description: �ر�Socket;
     output     : NA
     return     : void
    *******************************************************************/
    void ShutDown();

    /******************************************************************
     function   : Select
     description: 
     input      : SELECT_TYPE enType    ģʽ����,read,write,except;
     input      : long lUsec            �ȴ�ʱ�䣬Ĭ��Ϊ5΢��;
     output     : NA
     return     : int �ɹ�: ���ھ���״̬�����Ѿ�������fd_set�ṹ�е�����������
                  ʧ��: 0 -- ��ʱ, �������󷵻�SOCKET_ERROR,����WSAGetLastError���Ҵ�����;  
    *******************************************************************/
    int Select(SELECT_TYPE enType, long lUsec = 5);

    /******************************************************************
     function   : GetLocalIpPort
     description: ��ȡ����IP�Ͷ˿�;
     input      : std::string & strIP   ����IP;
     input      : unsigned int & uiPort ���ض˿�;
     output     : NA
     return     : void
    *******************************************************************/
    void GetLocalIpPort(std::string& strIP, unsigned int& uiPort);

    /******************************************************************
     function   : GetServerIpPort
     description: ��ȡ������IP�Ͷ˿�;
     input      : std::string & strIP   ������IP;
     input      : unsigned int & uiPort �������˿�;
     output     : NA
     return     : void
    *******************************************************************/
    void GetServerIpPort(std::string& strIP, unsigned int& uiPort);

    /******************************************************************
     function   : InitMulticast
     description: ��ʼ���鲥;
     input      : NA
     output     : NA
     return     : int IVS_SUCCEED �ɹ� IVS_FAIL ʧ��;
    *******************************************************************/
    int InitMulticast(const std::string& strRemoteSendStreamIp,
                      unsigned int uRemoteSendStreamPort,
                      const std::string& strLocalRecvStreamIp);

    // ��������ʵ���󶨵�socket����;
    SOCKET GetSocket() const { return m_socket; };

protected:
    SOCKET          m_socket;           // soket����;
    std::string     m_strLocalIP;       // ����IP;
    std::string     m_strServerIP;      // ������IP;
    unsigned int    m_uiLocalPort;      // ���ض˿�;
    unsigned int    m_uiServerPort;     // �������˿�;
};
#endif //CONNECTION_H