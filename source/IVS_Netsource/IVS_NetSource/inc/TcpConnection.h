/********************************************************************
filename    :    Connection.h
author      :    s00191067
created     :    2012/11/10
description :    socket���ӻ���
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2012/11/10 ��ʼ�汾
*********************************************************************/
#ifndef TCP_CONNECTION_H
#define TCP_CONNECTION_H

#include "hwsdk.h"
#include "NetSourceCommon.h"
#include "Connection.h"

class CTcpConnection : public CConnection
{
public:
    CTcpConnection(void);
    virtual ~CTcpConnection(void);

    /******************************************************************
     function   : Bind
     description: �󶨶˿�;
     input      : const char * pszLocalIP
     input      : unsigned int uiLocalPort
     output     : NA
     return     : int �ɹ�:IVS_SUCCEED ʧ��:IVS_FAIL;
    *******************************************************************/
    virtual int Bind(const char* pszLocalIP, unsigned int uiLocalPort);

    /******************************************************************
     function   : Connect
     description: socket����
     input      : const char * pszServerIP
     input      : unsigned int uiServerPort
     output     : NA
     return     : int �ɹ�:IVS_SUCCEED ʧ��:IVS_FAIL;
    *******************************************************************/
    virtual int Connect(const char* pszServerIP, unsigned int uiServerPort);

    /******************************************************************
     function   : RecvData
     description: ��������;
     input      : char * pData
     input      : int uiDataLen
     output     : NA
     return     : int �ɹ�:IVS_SUCCEED ʧ��:IVS_FAIL;
    *******************************************************************/
    virtual int RecvData(char * pData, unsigned int uiDataLen);

    /******************************************************************
     function   : SendData
     description: ��������;
     input      : char * pData
     input      : int uiDataLen
     output     : NA
     return     : int �ɹ�:IVS_SUCCEED ʧ��:IVS_FAIL;
    *******************************************************************/
    virtual int SendData(const char * pData, unsigned int uiDataLen);
};
#endif //TCP_CONNECTION_H