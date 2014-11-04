/********************************************************************
filename    :    ivs_security_rsa_public_key_info.h
author      :    yWX150774
created     :    2012/11/05
description :    �����RSA����ȡ�ĵĹ�Կ��Ϣ��ת������ʮ��������N��E
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2012/11/05 ��ʼ�汾
*********************************************************************/

#ifndef CBB_SECURITY_RSA_PUBLIC_KEY_INFO_H
#define CBB_SECURITY_RSA_PUBLIC_KEY_INFO_H


#include "ivs_uncopy.hpp"

#include "openssl/ossl_typ.h"

NAMESPACE_CBB_BEGIN

/***********************************************************
 * �����RSA����ȡ�ĵĹ�Կ��Ϣ��ת������ʮ��������N��E
 * �̳���UnCopy, �Ǹ����ܿ�������
 **********************************************************/
class EXPORT_DLL CRSA_Public_Key_Info : private CUnCopy
{
public:
    explicit CRSA_Public_Key_Info(const RSA * pRSA);
    ~CRSA_Public_Key_Info();

public:
    /*****************************************************************
     * ��֪�Ƿ��Ѿ���ʼ��
     ****************************************************************/
    bool HasInit() const;

    /*****************************************************************
     * ��ȡRSA��Կ��ģ��N
     ****************************************************************/
    const char * GetPublicKeyN() const;

    /*****************************************************************
     * ��ȡRSA��Կ�Ĺ�Կָ��E
     ****************************************************************/
    const char * GetPublicKeyE() const;

private:
    bool Init(const BIGNUM * pRSAPubN, const BIGNUM * pRSAPubE);
    void Clean();

private:
    char             * m_pszPubKeyN;
    char             * m_pszPubKeyE;
    bool               m_bInit;
};

NAMESPACE_CBB_END


#endif // CBB_SECURITY_RSA_PUBLIC_KEY_INFO_H
