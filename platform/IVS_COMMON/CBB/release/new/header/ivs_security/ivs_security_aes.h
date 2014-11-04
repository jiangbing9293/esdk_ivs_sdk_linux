/********************************************************************
filename    :    ivs_security_aes.h
author      :    yWX150774
created     :    2012/11/06
description :    ACE�ӽ���
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2012/11/06 ��ʼ�汾
*********************************************************************/

#ifndef CBB_SECURITY_AES_H
#define CBB_SECURITY_AES_H


#include "ivs_uncopy.hpp"

#include "openssl/aes.h"

NAMESPACE_CBB_BEGIN

/*************************************************************
 * ����ѧ�еĸ߼����ܱ�׼(Advanced Encryption Standard, AES),  
 * �ֳ�Rijndael���ܷ�, �����������������õ�һ��������ܱ�׼, 
 * ����һ�ֶԳƼ����㷨, OpenSSL���Ѿ�����Ӧʵ��, ���������м��׷�װ
 *
 * OpenSSL������һЩ˵��:
 *
 * 1. ���캯���е�bytesָ������key�ĳ���, ֻ֧��16, 24, 32
 *    bytesԽ��, ��������Խ���Ա��ƽ�
 * 2. ����key��16, 24, 32bytes��, �ӽ��ܺ�����ֻ��ȡ16�ֽ�
 *    ������ݲ���, �ͻ��������Խ��, 
 *    �������ÿ�ζ�ֻ�ܳ���16�ֽ� (������ֻ��ǰ16�ֽڵ���Ҫ���ܵ�����)
 ************************************************************/
class EXPORT_DLL CAES : private CUnCopy
{
public:
    explicit CAES();
    ~CAES();

public:
    /*****************************************************
     * ���üӽ��ܵ�Keyֵ
     * pKey:       �����Ƶ�����, �����ӽ���Keyֵ
     * iBytes:     pszKey�ĳ���, ����Ϊ16, 24, 32��֮һ
     * ����ֵ:      iBytes��Ϊ16, 24, 32ʱ�ض�ʧ��
     *****************************************************/
    bool SetKey(const unsigned char * pszKey, int iBytes);

public:
    /*****************************************************
     * AES����
     * pszInBuff:  ����
     * pszOutBuff: ����
     * iLen:       ���ݳ���
     *             ����, ����ΪAES_BLOCK_SIZE��������
     *             ����, ��Ϊ�ǶԳƼ����㷨, �������������һ��
     * ����ֵ:      ���ݳ��Ȳ���AES_BLOCK_SIZE��������ʱ, ��ʧ��
     *****************************************************/
    bool EncryptBinary(
        const unsigned char * pszInBuff, int iLen, 
        unsigned char * pszOutBuff) const;

    /*****************************************************
     * AES����
     * pszInBuff:  ����
     * pszOutBuff: ����
     * iLen:       ���ݳ���
     *             ����, ����ΪAES_BLOCK_SIZE��������
     *             ����, ��Ϊ�ǶԳƼ����㷨, �������������һ��
     * ����ֵ:      ���ݳ��Ȳ���AES_BLOCK_SIZE��������ʱ, ��ʧ��
     *****************************************************/
    bool DecryptBinary(
        const unsigned char * pszInBuff, int iLen, 
        unsigned char * pszOutBuff) const;

    /*****************************************************
     * ����: ����AES����, ����Base64������ַ�����ʽ����
     * pszInBuff:   �ַ������͵����� (�մ����ܻ��ǿմ�)
     * pszOutBuff:  �������
     * iOutBuffLen: �������Ĵ�С, �ɹ��Ļ�, �ᱣ��'\0'
     * ����ֵ:       �Ƿ�ɹ�
     *****************************************************/
    bool EncryptString(const char * pszInBuff, 
        char * pszOutBuff, int iOutBuffLen) const;

    /*****************************************************
     * ����: ����Base64����, ����AES���ܳ��ַ�������
     * pszInBuff:   �ַ������͵�Base64��������� (�մ����ܻ��ǿմ�)
     * pszOutBuff:  �������
     * iOutBuffLen: �������Ĵ�С, �ɹ��Ļ�, �ᱣ��'\0'
     * ����ֵ:       �Ƿ�ɹ�
     *****************************************************/
    bool DecryptString(const char * pszInBuff, 
        char * pszOutBuff, int iOutBuffLen) const;

    /*****************************************************
     * ����: 
     *        ��֪����ΪiStringLen(������'\0')���ַ�������
     *        EncryptString()�������������İ�ȫ��С(���ֵ)
     *****************************************************/
    static int GetStringEncryptedBufferLen(int iStringLen);

    /*****************************************************
     * ����: 
     *        ��֪����ΪiStringLen(������'\0')���ַ�������
     *        DecryptString()�������������İ�ȫ��С(���ֵ)
     *****************************************************/
    static int GetStringDecryptedBufferLen(int iStringLen);

    /*****************************************************
     * ����: 
     *        ��֪�ַ���pszString����
     *        EncryptString()�������������İ�ȫ��С(���ֵ)
     *****************************************************/
    static int GetStringEncryptedBufferLen(const char * pszString);

    /*****************************************************
     * ����: 
     *        ��֪�ַ���pszString����
     *        DecryptString()�������������İ�ȫ��С(���ֵ)
     *****************************************************/
    static int GetStringDecryptedBufferLen(const char * pszString);

private:
    /*****************************************************
     * ����: 
     *        �������Ƿ��ʼ��, �Լ���������ĺϷ���(�������)
     *        �����ӽ��ܺ������Ӻ���, ����Ȧ���Ӷ�
     *****************************************************/
    bool CheckStateAndArgument(
        const void * pszInBuff, const void * pszOutBuff, int iLen) const;

private:
    bool        m_bSetKey;
    AES_KEY     m_aAesEncryptKey;
    AES_KEY     m_aAesDecryptKey;
};

NAMESPACE_CBB_END


#endif // CBB_SECURITY_AES_H
