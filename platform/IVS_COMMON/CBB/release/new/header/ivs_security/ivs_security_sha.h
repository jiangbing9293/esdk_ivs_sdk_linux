/********************************************************************
filename    :    ivs_security_sha.h
author      :    yWX150774
created     :    2012/11/06
description :    SHA(256)ժҪ�㷨
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2012/11/06 ��ʼ�汾
*********************************************************************/

#ifndef CBB_SECURITY_SHA_H
#define CBB_SECURITY_SHA_H


#include "ivs_namespace.h"

#include "openssl/sha.h"

NAMESPACE_CBB_BEGIN

/* ���ݾ���ժҪ����, �پ���ʮ������ת��, �õ�����������ռ�õĻ����С */
const int SHA256_HEX_LENGTH = 2 * SHA256_DIGEST_LENGTH + 1;

/****************************************************************
 * SHA(256)��һ��ժҪ�㷨, OpenSSL���Ѿ�����Ӧʵ��, ���������м��׷�װ
 * pszData  : ��������
 * iDataLen : �������ݳ���
 * pszDigest: ����õ���ժҪ����
 * ����ֵ    : ժҪ���ݵĳ���, 
 *            �������������Ч, ���ع̶�ֵ: SHA256_DIGEST_LENGTH
 *            ����Ϊ0
 ****************************************************************/
extern "C" int EXPORT_DLL IVS_Security_SHA256(
    const unsigned char * pszData, int iDataLen, 
    unsigned char * pszDigest);

/****************************************************************
 * �Ƚ���SHA256ժҪ����, �ٽ���ʮ������ת������ɿɴ�ӡ������
 * pszData     : ��������
 * iDataLen    : �������ݳ���
 * pszDigestHex: ���澭��ʮ������ת���õ���ժҪ����
 * ����ֵ       : ժҪ���ݾ���ʮ������ת����ĳ���, 
 *               �������������Ч, ���ع̶�ֵ: SHA256_HEX_LENGTH
 *               ����Ϊ0
 ****************************************************************/
extern "C" int EXPORT_DLL IVS_Security_SHA256_Hex(
    const unsigned char * pszData, int iDataLen, 
    char * pszDigestHex);

NAMESPACE_CBB_END


#endif // CBB_SECURITY_SHA_H
