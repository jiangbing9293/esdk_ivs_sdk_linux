/********************************************************************
filename    :    ivs_security_md5.h
author      :    w00207027, yWX150774
created     :    2013/03/18
description :    MD5ժҪ�㷨 + HTTP��MD5ժҪ����
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2013/03/18 ��ʼ�汾
*********************************************************************/

#ifndef CBB_SECURITY_MD5_H
#define CBB_SECURITY_MD5_H


#include "ivs_namespace.h"

#include "openssl/md5.h"

NAMESPACE_CBB_BEGIN

/* ���ݾ���ժҪ����, �پ���ʮ������ת��, �õ�����������ռ�õĻ����С */
const int MD5_HEX_LENGTH = 2 * MD5_DIGEST_LENGTH + 1;

/****************************************************************
 * MD5��һ��ժҪ�㷨, OpenSSL���Ѿ�����Ӧʵ��, ���������м��׷�װ
 * pszData  : ��������
 * iDataLen : �������ݳ���
 * pszDigest: ����õ���ժҪ����
 * ����ֵ    : ժҪ���ݵĳ���, 
 *            �������������Ч, ���ع̶�ֵ: MD5_DIGEST_LENGTH
 *            ����Ϊ0
 ****************************************************************/
extern "C" int EXPORT_DLL IVS_Security_MD5(
    const unsigned char * pszData, int iDataLen, 
    unsigned char * pszDigest);

/****************************************************************
 * �Ƚ���MD5ժҪ����, �ٽ���ʮ������ת������ɿɴ�ӡ������
 * pszData     : ��������
 * iDataLen    : �������ݳ���
 * pszDigestHex: ���澭��ʮ������ת���õ���ժҪ����
 * ����ֵ       : ժҪ���ݾ���ʮ������ת����ĳ���, 
 *               �������������Ч, ���ع̶�ֵ: MD5_HEX_LENGTH
 *               ����Ϊ0
 ****************************************************************/
extern "C" int EXPORT_DLL IVS_Security_MD5_Hex(
    const unsigned char * pszData, int iDataLen, 
    char * pszDigestHex);

/****************************************************************
 * calculate H(A1) as per HTTP Digest spec
 * �������pszDigestHex�Ĵ�С����ΪMD5_HEX_LENGTH
 ****************************************************************/
extern "C" int EXPORT_DLL IVS_Security_Http_MD5_DigestCalcHA1(
    const char * pszAlg, 
    const char * pszUserName, 
    const char * pszRealm, 
    const char * pszPassword, 
    const char * pszNonce, 
    const char * pszCNonce, 
    char * pszDigestHex);

/******************************************************************
 * calculate request-digest/response-digest as per HTTP Digest spec
 * HA1, pszHEntity��ʮ�����Ƶ�ժҪ����, ���ȱ���Ϊ2 * MD5_DIGEST_LENGTH,
 * �������pszDigestHex�Ĵ�С����ΪMD5_HEX_LENGTH
 ******************************************************************/
extern "C" int EXPORT_DLL IVS_Security_Http_MD5_DigestCalcResponse(
    const char * pszHA1,        /* H(A1) */
    const char * pszNonce,      /* nonce from server */
    const char * pszNonceCount, /* 8 hex digits */
    const char * pszCNonce,     /* client nonce */
    const char * pszQop,        /* qop-value: "", "auth", "auth-int" */
    const char * pszMethod,     /* method from the request */
    const char * pszDigestUri,  /* requested URL */
    const char * pszHEntity,    /* H(entity body) if qop="auth-int" */
    char * pszDigestHex);       /* request-digest or response-digest */

NAMESPACE_CBB_END


#endif // CBB_SECURITY_MD5_H
