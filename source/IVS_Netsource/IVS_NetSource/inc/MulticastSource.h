/********************************************************************
filename    :    MulticastSource.h
author      :    z90003203
created     :    2013/1/27
description :    �鲥��Դ�ඨ��
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2012/11/10 ��ʼ�汾
*********************************************************************/

#pragma once

#ifndef _C_MULTICAST_SOURCE_H_
#define _C_MULTICAST_SOURCE_H_

#include "UdpSource.h"

class CChannelInfo;
class CMulticastSource : public CUdpSource
{
public:
    CMulticastSource(void);
    ~CMulticastSource(void);

    /******************************************************************
     function   : Init
     description: ��ʼ��
     input      : CChannelInfo * pChannelInfo
     output     : NA
     return     : int �ɹ�:IVS_SUCCEED ʧ��:IVS_FAIL;
    *******************************************************************/
    virtual int Init(CChannelInfo* pChannelInfo);

private:
    /******************************************************************
     function   : InitMulticast
     description: ��ʼ���鲥����
     input      : NA
     output     : NA
     return     : int �ɹ�:IVS_SUCCEED ʧ��:IVS_FAIL;
    *******************************************************************/
    int InitMulticast();
};

#endif
