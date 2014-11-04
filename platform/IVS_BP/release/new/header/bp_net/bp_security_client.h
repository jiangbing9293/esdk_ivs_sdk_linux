/********************************************************************
filename    :    bp_security_client.h
author      :    yWX150774
created     :    2012/11/08
description :    ��Ϣ��ȫ�ͻ��� (��Կ��������)
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2012/11/08 ��ʼ�汾
*********************************************************************/

#ifndef BP_SECURITY_CLIENT_H
#define BP_SECURITY_CLIENT_H


#include "Cmd.h"

#include "exchange_key_attribute.h"

#include "ivs_xml.h"
#include "ivs_uncopy.hpp"
#include "ivs_security_aes.h"
#include "ivs_security_rsa_public_key.h"

#include <string>

NAMESPACE_BP_BEGIN

/*********************************************
 * 
 * ��Ϣ��ȫ�ͻ��� (��Կ��������)
 * 
 ********************************************/
class EXPORT_DLL CSecurityClient : private CBB::CUnCopy
{
public:
    CSecurityClient();
    ~CSecurityClient();

public:
    /*****************************************************
     * ������Կ����
     * ����:      ���CCmd��Ҫ����Ϣ
     * ����ֵ:    �ɹ����� IVS_SUCCEED, ʧ�ܷ��� IVS_FAIL
     *****************************************************/
    int RequestExchangeKey(
        uint32_t uiServiceID, int iNetElemType, 
        const char * pszNetLinkID, 
        const char * pszCameraCode = "", 
        const char * pszDomainCode = "", 
        const char * pszDestModule = "", 
        const char * pszDeviceCode = "", 
        const char * pszNVRCode = "", 
        enum DOMAIN_TYPE enDomainType = DOMAIN_NONE);

private: // ���溯��������RequestExchangeKey�е���, �������ṩ
    /*****************************************************
     * ��ʼ��
     *****************************************************/
    bool Init();

    /*****************************************************
     * ����RSA��Կ
     * ����:      ���CCmd��Ҫ����Ϣ
     * ����ֵ:    �ɹ����� IVS_SUCCEED, ʧ�ܷ��� IVS_FAIL
     *****************************************************/
    int RequestPublicKey(
        uint32_t uiServiceID, int iNetElemType, 
        const char * pszNetLinkID, 
        const char * pszCameraCode = "", 
        const char * pszDomainCode = "", 
        const char * pszDestModule = "", 
        const char * pszDeviceCode = "", 
        const char * pszNVRCode = "", 
        enum DOMAIN_TYPE enDomainType = DOMAIN_NONE);

private: // ��Ϊ��ͬ��������Ϣ�ķ�ʽ, ������Ϊ˽�к���
    /*****************************************************
     * ����RSA��Կ�������Ӧ
     ****************************************************/
    int HandlePublicKeyResponse(CCmd * pRespPubKeyCmd);

    /*****************************************************
     * ������Կ�����������Ӧ
     ****************************************************/
    int HandleExchangeKeyResponse(CCmd * pRespExchKeyCmd);

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
     * pszInBuff:   �ַ������͵�����
     * pszOutBuff:  �������
     * iOutBuffLen: �������Ĵ�С, �ɹ��Ļ�, �ᱣ��'\0'
     * ����ֵ:       �Ƿ�ɹ�
     *****************************************************/
    bool EncryptString(const char * pszInBuff, 
        char * pszOutBuff, int iOutBuffLen) const;

    /*****************************************************
     * ����: ����Base64����, ����AES���ܳ��ַ�������
     * pszInBuff:   �ַ������͵�Base64���������
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

    /*****************************************************
     * ����: 
     *        ��֪����AES��������ʱ��Ҫ���ض���TransID
     *****************************************************/
    const std::string & GetSecurityDataTransID() const;

    /*****************************************************
     * ����: 
     *        ��֪����AES��������ʱ��Ҫ���ض���TransID
     *        nLen���㹻��, �����ʧ��
     *        �ɹ����� IVS_SUCCEED, ʧ�ܷ��� IVS_FAIL
     *****************************************************/
    int GetSecurityDataTransID(char * szTransactionNo, size_t nLen) const;

    /*****************************************************
     * ����: 
     *        �õ��ڲ�AES����ĸ���
     *        �ɹ����� IVS_SUCCEED, ʧ�ܷ��� IVS_FAIL
     *****************************************************/
    int DupAes(CBB::CAES * pAes) const;

private:
    /*****************************************************
     * ��������Ŀ�ĵ�ַ��Ϣ��XML
     ****************************************************/
    int BuildDestInfoXml(
        CBB::CXml & rDestInfoXml, 
        const char * pszCameraCode, 
        const char * pszDomainCode, 
        const char * pszDestModule, 
        const char * pszDeviceCode, 
        const char * pszNVRCode, 
        enum DOMAIN_TYPE enDomainType);

    /*****************************************************
     * ��XML�ļ��н�����RSA�Ĺ�Կ
     ****************************************************/
    int ParseRSAPublicKeyFromXML(CBB::CXml & rRespPubKeyXml);

    /*****************************************************
     * ������Կ���������XML
     ****************************************************/
    int BuildRequestExchangeKeyXml(
        CBB::CXml & rReqExchKeyXml, 
        const char * pszCameraCode, 
        const char * pszDomainCode, 
        const char * pszDestModule, 
        const char * pszDeviceCode, 
        const char * pszNVRCode, 
        enum DOMAIN_TYPE enDomainType);

private:
    bool                    m_bInit;                       // �Ƿ���������ʼ��
    uint32_t                m_uiSeqID;
    std::string             m_strSecurityDataTransID;      // ����AES��������ʱ������ض�TransID
    unsigned char         * m_pRSAEncryptBuff;             // ����RSA��Կ�����������ݵĻ���
    int                     m_iRSAEncryptLen;              // ����RSA��Կ�����������ݵĻ����С
    unsigned char         * m_pRSAEncryptBase64EncodeBuff; // ����RSA��Կ�����ٱ�Base64�����������ݵĻ���
    int                     m_iRSAEncryptBase64EncodeLen;  // ����RSA��Կ�����ٱ�Base64�����������ݵĻ����С
    CBB::CRSA_Public_Key  * m_pRSAPubKey;                  // ���󵽵�RSA��Կ
    CBB::CAES             * m_pAES;                        // AES�ӽ��ܶ���
    bool                    m_bGetPubKey;                  // �Ƿ�����RSA��Կ
    unsigned char           m_szExchKey[EXCHANGE_KEY_LENGTH]; // ���湤����Կ
};

NAMESPACE_BP_END


#endif // BP_SECURITY_CLIENT_H
