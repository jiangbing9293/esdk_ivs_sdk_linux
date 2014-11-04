/********************************************************************
filename    :    ivs_security_rsa_private_key.h
author      :    yWX150774
created     :    2012/11/05
description :    RSA˽Կ��������
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2012/11/05 ��ʼ�汾
*********************************************************************/

#ifndef CBB_SECURITY_RSA_PRIVATE_KEY_H
#define CBB_SECURITY_RSA_PRIVATE_KEY_H


#include "ivs_uncopy.hpp"

#include "openssl/ossl_typ.h"

NAMESPACE_CBB_BEGIN

/****************************************************************
 * ������RSA˽Կ��������, 
 * ���ܺ�����ݵĴ�С�ǲ�ȷ����, 
 * �̳���CUnCopy, �Ǹ����ܿ�������
 ***************************************************************/
class EXPORT_DLL CRSA_Private_Key : private CUnCopy
{
public:
    explicit CRSA_Private_Key(RSA * pRSA);
    ~CRSA_Private_Key();

public:
    /************************************************************
     * ��pInBuff�е�������˽Կ���н���, ���ܺ�����ݱ�����pOutBuff��, 
     * Ҫ��pOutBuff�Ļ����СiOutLen�����㹻��, ����, ���ܻ����! 
     * ������ж��, ��û������صĵײ����, iOutLen������ܺ�����ݴ�С
    ************************************************************/
    bool Decrypt(
        const void * pInBuff, int iInLen, 
        void * pOutBuff, int & iOutLen) const;

private:
    RSA * m_pRSA;
};

NAMESPACE_CBB_END


#endif // CBB_SECURITY_RSA_PRIVATE_KEY_H
