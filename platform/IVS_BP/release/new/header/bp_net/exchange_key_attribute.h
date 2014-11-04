/********************************************************************
filename    :    exchange_key_attribute.h
author      :    yWX150774
created     :    2012/12/20
description :    ������Կ������
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2012/12/20 ��ʼ�汾
*********************************************************************/

#ifndef BP_EXCHANGE_KEY_ATTRIBUTE_H
#define BP_EXCHANGE_KEY_ATTRIBUTE_H


#include "ivs_error.h"
#include "bp_stdint.h"
#include "bp_nss_def.h"

NAMESPACE_BP_BEGIN

const int EXCHANGE_KEY_LENGTH  = 32; // ������Կ�ĳ���
const int EXCHANGE_KEY_TIMEOUT = 60; // ������Կ����Чʱ��
const unsigned int RSA_DECRYPT_FAIL = IVS_SUCCEED + IVS_FAIL + 1; // RSA����ʧ�� (����������һ��RSA��˽Կ��)
const unsigned int SECURITY_MESSAGE_MIN_LENGTH = sizeof(TNssMsgHeader) +  sizeof(uint16_t) + sizeof(uint32_t);

enum DOMAIN_TYPE
{
    DOMAIN_NONE    = 0, // ��Чֵ
    DOMAIN_PILE    = 1, // �ѵ�
    DOMAIN_CLUSTER = 2, // ��Ⱥ
    DOMAIN_SINGLE  = 3, // ����
    DOMAIN_MAX
};

NAMESPACE_BP_END


#endif // BP_EXCHANGE_KEY_ATTRIBUTE_H
