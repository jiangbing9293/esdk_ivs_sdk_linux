/********************************************************************
filename    :    bp_security_server.h
author      :    yWX150774
created     :    2012/11/08
description :    ��Ϣ��ȫ������ (��Կ������Ӧ��)
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2012/11/08 ��ʼ�汾
*********************************************************************/

#ifndef BP_SECURITY_SERVER_H
#define BP_SECURITY_SERVER_H


#include "bp_namespace.h"
#include "Cmd.h"

#include "ivs_xml.h"
#include "ivs_uncopy.hpp"
#include "ivs_security_aes.h"
#include "ivs_security_rsa_key.h"
#include "ivs_security_rsa_private_key.h"
#include "ivs_security_rsa_public_key_info.h"

#include "ace/Thread_Mutex.h"

#include <string>
#include <map>
#include <list>
#include <utility>

NAMESPACE_BP_BEGIN

/********************************************************
 * 
 * ��Ϣ��ȫ������ (��Կ������Ӧ��)
 * 
 *******************************************************/
class EXPORT_DLL CSecurityServer : private CBB::CUnCopy
{
public:
    CSecurityServer();
    ~CSecurityServer();

public:
    /*****************************************************
     * ��ʼ��
     *****************************************************/
    bool Init();

public: 
    /*****************************************************
     * ����RSA��Կ����
     * ����ֵ: �ɹ����� IVS_SUCCEED, ʧ�ܷ��� IVS_FAIL
     *****************************************************/
    int HandlePublicKeyRequest(CCmd * pReqPubKeyCmd);

    /*****************************************************
     * ������Կ�������� (pAes���ڷ���һ��AES����ĸ���)
     * ����ֵ: �ɹ����� IVS_SUCCEED, ʧ�ܷ��� IVS_FAIL
     *****************************************************/
    int HandleExchangeKeyRequest(
        CCmd * pReqExchKeyCmd, CBB::CAES * pAes = NULL);

public:
    /*****************************************************
     * ɾ�����ڵĽ�����Կ
     *****************************************************/
    void RemoveTimeoutExchangeKey();

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
        const std::string & strTransID, 
        const unsigned char * pszInBuff, int iLen, 
        unsigned char * pszOutBuff);

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
        const std::string & strTransID, 
        const unsigned char * pszInBuff, int iLen, 
        unsigned char * pszOutBuff);

    /*****************************************************
     * ����: ����AES����, ����Base64������ַ�����ʽ����
     * pszInBuff:   �ַ������͵�����
     * pszOutBuff:  �������
     * iOutBuffLen: �������Ĵ�С, �ɹ��Ļ�, �ᱣ��'\0'
     * ����ֵ:       �Ƿ�ɹ�
     *****************************************************/
    bool EncryptString(
        const std::string & strTransID, 
        const char * pszInBuff, 
        char * pszOutBuff, int iOutBuffLen);

    /*****************************************************
     * ����: ����Base64����, ����AES���ܳ��ַ�������
     * pszInBuff:   �ַ������͵�Base64���������
     * pszOutBuff:  �������
     * iOutBuffLen: �������Ĵ�С, �ɹ��Ļ�, �ᱣ��'\0'
     * ����ֵ:       �Ƿ�ɹ�
     *****************************************************/
    bool DecryptString(
        const std::string & strTransID, 
        const char * pszInBuff, 
        char * pszOutBuff, int iOutBuffLen);

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
     * �����󽻻���Կ��XML�ļ��н�����������Կ��TransID
     *****************************************************/
    int ParseReqExchangeKeyXml(
        CBB::CXml & rReqExchKeyXml, 
        std::string & strTransID, 
        unsigned char *& pExchKey);

private:
    /*****************************************************
     * ��������RSA��Կ�����ʵ�ֺ���
     *****************************************************/
    int HandlePublicKeyRequestImplement(CCmd & rReqPubKeyCmd);
    
    /*****************************************************
     * ����������Կ���������ʵ�ֺ���
     *****************************************************/
    int HandleExchangeKeyRequestImplement(
        CCmd & rReqExchKeyCmd, CBB::CAES * pAes);

    /*****************************************************
     * ����TransID��װ��Ӧ��AES����
     *****************************************************/
    bool GetAES(const std::string & strTransID, CBB::CAES & rAes);

    /*****************************************************
     * ��ȡ���潻����Կ�Ļ�����(32�ֽ�)
     *****************************************************/
    unsigned char * GetExchangeKeyBuffer();

    /*****************************************************
     * ���ձ��潻����Կ�Ļ�����(32�ֽ�)
     *****************************************************/
    void ReleaseExchangeKeyBuffer(unsigned char * pExchKey);

private:
    bool                                     m_bInit;                    // �Ƿ���������ʼ��
    const CBB::CRSA_Key                    & m_rRSA;                     // RSA����ʵ���ĳ�����
    CBB::CRSA_Private_Key                  * m_pRSAPriKey;               // RSA˽Կ��Ϣ
    CBB::CRSA_Public_Key_Info              * m_pRSAPubKeyInfo;           // RSA��Կ��N��E����Ϣ
    std::list<unsigned char *>               m_listExchKeyBuffer;        // ���潻����Կ�Ļ��������ڴ��
    ACE_Thread_Mutex                         m_aExchKeyBufferListThreadMutex; // ���潻����Կ���ڴ�ض�Ӧ���̻߳�����
    std::map<
             std::string, 
             std::pair<
                       unsigned char *, 
                       time_t
                      >
            >                                m_mapTransID2ExchKeyCTime;  // TransID��������Կ���䴴��ʱ���һһӳ��
    ACE_Thread_Mutex                         m_aExchKeyMapThreadMutex;   // ���潻����Կ��map��Ӧ���̻߳�����

    typedef std::map<
                     std::string, 
                     std::pair<
                               unsigned char *, 
                               time_t
                              >
                    >::iterator              TransID2ExchKeyCTimeMapIterator;
};

NAMESPACE_BP_END


#endif // BP_SECURITY_SERVER_H
