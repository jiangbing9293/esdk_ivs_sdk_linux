/********************************************************************
filename    :    ivs_security_rsa_key.h
author      :    yWX150774
created     :    2012/11/05
description :    �������RSA��˽Կ, ��˽Կ�ɱ��ֱ𱸷�
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2012/11/05 ��ʼ�汾
*********************************************************************/

#ifndef CBB_SECURITY_RSA_KEY_H
#define CBB_SECURITY_RSA_KEY_H


#include "ivs_uncopy.hpp"

#include "openssl/ossl_typ.h"

NAMESPACE_CBB_BEGIN

class CRSA_Private_Key;
class CRSA_Public_Key;
class CRSA_Public_Key_Info ;

/*************************************************************
 * �����������RSA��˽Կ
 * �̳���CUnCopy, �Ǹ����ܿ�������
 * ����, ʵ��Ϊ����ģʽ
 ************************************************************/
class EXPORT_DLL CRSA_Key : private CUnCopy
{
private:
    explicit CRSA_Key(int iBits = 2048);
    ~CRSA_Key();

public:
    /*********************************************************
     * ��ȡʵ��
     ********************************************************/
    static CRSA_Key & GetInstance();

public:
    /*********************************************************
     * ������˽Կ
     ********************************************************/
    CRSA_Private_Key * DupPrivateKey() const;

    /*********************************************************
     * ��������Կ
     ********************************************************/
    CRSA_Public_Key * DupPublicKey() const;

    /*********************************************************
     * ��������Կ��Ϣ(N��E)
     ********************************************************/
    CRSA_Public_Key_Info * DupPublicKeyInfo() const;

private:
    /*********************************************************
     * ����������Ե�RSA��˽Կ, ��ʱ���ṩ�˽ӿ�, �ʶ���Ϊprivate����
     ********************************************************/
    bool ReInit(int iBits = 2048);

private:
    static CRSA_Key s_aRSA;

private:
    RSA * m_pRSA;
};

NAMESPACE_CBB_END


#endif // CBB_SECURITY_RSA_KEY_H
