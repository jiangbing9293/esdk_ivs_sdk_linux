/********************************************************************
filename    :    ivs_security_rsa_public_key.h
author      :    yWX150774
created     :    2012/11/05
description :    RSA��Կ��������
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2012/11/05 ��ʼ�汾
*********************************************************************/

#ifndef CBB_SECURITY_RSA_PUBLIC_KEY_H
#define CBB_SECURITY_RSA_PUBLIC_KEY_H


#include "ivs_uncopy.hpp"

#include "openssl/ossl_typ.h"

NAMESPACE_CBB_BEGIN

/*********************************************************************
 * ������RSA��Կ��������, 
 * ���ܺ�����ݵĴ�С�ǹ̶���, 
 * �̳���CUnCopy, �Ǹ����ܿ�������
 ********************************************************************/
class EXPORT_DLL CRSA_Public_Key : private CUnCopy
{
public:
    explicit CRSA_Public_Key(RSA * pRSA);

    explicit CRSA_Public_Key(
        const char * pszPubKeyN, 
        const char * pszPubKeyE);

    ~CRSA_Public_Key();

public:
    /*****************************************************************
     * ȷ����Ҫ���ļ�����������С
     ****************************************************************/
    int EncryptedBufferLength() const;

    /*****************************************************************
     * ��pInBuff�е������ù�Կ���м���, ���ܺ�����ݱ�����pOutBuff��, Ҫ��
     * pOutBuff�Ļ����С�㹻��, �û����ȵ���EncryptedBufferLength()
     * ��ȷ����Ҫ������������С, iOutLen��Ϊ�������ʱ, ΪpOutBuff�Ļ����С
     * ���� >= EncryptedBufferLength(), ����, ʧ�ܷ���! ����, iOutLen��Ϊ
     * �������, ���ᱣ����ܺ�����ݵĴ�С, ��Ӧ==EncryptedBufferLength()
    *****************************************************************/
    bool Encrypt(
        const void * pInBuff, int iInLen, 
        void * pOutBuff, int & iOutLen) const;

private:
    RSA     * m_pRSA;
    int       m_iEncryptedBufferLen;
};

NAMESPACE_CBB_END


#endif // CBB_SECURITY_RSA_PUBLIC_KEY_H
