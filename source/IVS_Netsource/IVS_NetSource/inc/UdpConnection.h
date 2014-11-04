/********************************************************************
filename    :    UdpConnection.h
author      :    s00191067
created     :    2012/11/10
description :    socket���ӻ���
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2012/11/10 ��ʼ�汾
*********************************************************************/
#ifndef UDP_CONNECTION_H
#define UDP_CONNECTION_H

#include "hwsdk.h"
#include "NetSourceCommon.h"
#include "Connection.h"

class CUdpConnection : public CConnection
{
public:
    CUdpConnection(void);
    virtual ~CUdpConnection(void);

    /******************************************************************
     function   : Bind
     description: �󶨶˿�;
     input      : const char * pszLocalIP   ����IP
     input      : unsigned int uiLocalPort  ���ض˿�
     output     : NA
     return     : int �ɹ�:IVS_SUCCEED ʧ��:IVS_FAIL;
    *******************************************************************/
    virtual int Bind(const char* pszLocalIP, unsigned int uiLocalPort);

    /******************************************************************
     function   : Connect
     description: ����socket
     input      : const char * pszServerIP ������IP
     input      : unsigned int uiServerPort�������˿�
     output     : NA
     return     : int �ɹ�:IVS_SUCCEED ʧ��:IVS_FAIL;
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
    virtual int RecvData(char* pData, unsigned int uiDataLen);

    /******************************************************************
     function   : SendData
     description: ��������;
     input      : char * pData �����͵�����;
     input      : int uiDataLen ���ݳ���;
     output     : NA
     return     : int ��::sendto�ķ���ֵ����һ��;
    *******************************************************************/
    virtual int SendData(const char* pData, unsigned int uiDataLen);

	/******************************************************************
     function   : SendMulitCastData
     description: ��������;
     input      : char * pData �����͵�����;
     input      : int uiDataLen ���ݳ���;
	 input      : sockaddr_in& sockAddr ʹ�õ�socket����;
     output     : NA
     return     : int ��::sendto�ķ���ֵ����һ��;
    *******************************************************************/
	int SendMulitCastData(const char* pData, unsigned int uiDataLen, const sockaddr_in& sockAddr);
};
#endif //UDP_CONNECTION_H